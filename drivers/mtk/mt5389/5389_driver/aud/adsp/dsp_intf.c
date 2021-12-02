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
 * $Date: 2012/09/26 $
 * $RCSfile: dsp_intf.c,v $
 * $Revision: #9 $
 *
 *---------------------------------------------------------------------------*/

/** @file dsp_intf.c
 *  Brief of file dsp_intf.c.
 *    Interface definition between emulation and target.
 *    This named "DspIntf.cpp" on emulation program
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN

#include "x_util.h"

#include "dsp_common.h"
#include "dsp_shm.h"
#include "dsp_func.h"
#include "dsp_uop.h"
#include "dsp_intf.h"
#include "dsp_reg.h"
#include "dsp_data.h"
#include "dsp_table.h"
#include "dsp_rg_ctl.h"
#include "d2rc_shm.h"
#include "adsp_task.h"

#include "aud_if.h"
#include "aud_cfg.h"
#include "aud_hw.h"
#include "aud_drv.h"
#include "aud_debug.h"
//#include "drv_stc.h"
#include "stc_drvif.h"
#include "drv_adsp.h"
#include "dmx_if.h"
#include "../periph/codec_mt5365.h"
#include "x_mid.h"
#include "x_os.h"
#include "x_timer.h"
#include "x_assert.h"
#include "x_hal_5381.h"
#include "x_chip_id.h"
#include "x_aud_dec.h"

LINT_EXT_HEADER_END

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define DSP_IMAGE_ALIGN_IN_BYTE  	4
#define DSP_IMAGE_HEADER_SIZE    		(0x20)

#define DSP_SRAM_TOP_45M_DEC1             (0x158B)
#define DSP_SRAM_TOP_55M_DEC1             (0x158C)
#define DSP_SRAM_TOP_60M_DEC1             (0x158D)
#define DSP_SRAM_TOP_65M_DEC1             (0x158E)
#define DSP_SRAM_TOP_655M_DEC1           (0x1590)
#define DSP_SRAM_TOP_45M_DEC2             (0x2A0B)
#define DSP_SRAM_TOP_55M_DEC2             (0x2A0C)
#define DSP_SRAM_TOP_60M_DEC2             (0x2A0D)
#define DSP_SRAM_TOP_65M_DEC2             (0x2A0E)
#define DSP_SRAM_TOP_655M_DEC2           (0x2A10)

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

typedef enum
{
	A2_SRC_MODE_M = 1,
	A2_SRC_MODE_BG = 0x2,
	A2_SRC_MODE_DK1 = 0x4,
	A2_SRC_MODE_DK2 = 0x8,
	A2_SRC_MODE_DK3 = 0x10
} A2_SRC_MODE;

typedef enum
{
	PAL_SRC_MODE_I = 1,
	PAL_SRC_MODE_BG = 0x2,
	PAL_SRC_MODE_DK = 0x4,
	PAL_SRC_MODE_SECAML = 0x100
} PAL_SRC_MODE;

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

#define GET_DSP_DECODER_TYPE(u1DecId)  ((u1DecId == AUD_DEC_MAIN) ? _rDspVars[AUD_DEC_MAIN].bDspStrTyp : _rDspVars[AUD_DEC_AUX].bDspStrTyp)

//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------
extern BOOL _fgFMRadioDetectionResult;
//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------

extern UINT32 _DSP_GetBinarySize(void);
// FIXME
extern BOOL AUD_DRVGetDecodeType(UINT8 u1DecId,  AUD_DEC_STREAM_FROM_T * peStreamFrom,
	                      AUD_FMT_T * peDecType);
extern BOOL _AUD_GetNotifyFunc(AudDecNfyFct * pfNfyFunc);

//extern void _AUD_DspSetDDCOEnable(BOOL fgEnable);
extern AUD_IEC_T _GetIECFlag(UINT8 u1DecId);
extern void _AUD_DspSetAoutReinit(void);
extern void _AUD_DspSetIEC(UINT8 u1DecId);

#ifdef CC_AUD_DATASRAM
extern void vDspResetDataSram(void);
#endif

//-----------------------------------------------------------------------------
// Static function forward declarations
//-----------------------------------------------------------------------------

#ifndef CC_FPGA
static BOOL _fgEnableDspMidProtect(void);
static BOOL _fgDisableDspMidProtect(void);
static BOOL _fgInitDspMidProtect(void);
#endif
static void _GetAudInfo(UINT8 u1DecId, AUD_DEC_INFO_T * prAudInfo);
static void _GetAacAudInfo(UINT8 u1DecId, AUD_DEC_INFO_T * prAudInfo);
static void _GetSbcAudInfo(UINT8 u1DecId, AUD_DEC_INFO_T * prAudInfo);
static void _GetDraAudInfo(UINT8 u1DecId, AUD_DEC_INFO_T * prAudInfo);
static void _GetDTSAudInfo(UINT8 u1DecId, AUD_DEC_INFO_T * prAudInfo);

//---------------------------------------------------------------------------
// Global variables
//---------------------------------------------------------------------------
UINT8 _guAudDemodTypeChange;
AUD_WMAErr_NOTIFY _hWMAErrNotifyFunc = NULL;
#ifdef CC_53XX_SWDMX_V2
VOID *_prWMAErrInst = NULL;  // megaa 20090828
#endif
UINT8              _guAudDspTrigAtvFmtchg;
AUD_FMT_T      _eAtvFmtChgDecFmt;

UINT32 _gu4FMNoiseLevelTHL = 0x2C000;
UINT32 _gu4FMInBandPwrTHL = 0x600;
INT32 _gi4FMRatioTHL = 0xd;

BOOL  _fgAudMMnotifySWDMX[AUD_DEC_MAX]= {FALSE,FALSE,FALSE,FALSE};
//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
#ifndef CC_FPGA
static UINT32 _u4MidRegion;
static HANDLE_T _hSemaLoadCode;
#endif
static TV_AUD_SYS_T _eChannelSrc;

static BOOL  _fgAoutEnable[AUD_DEC_MAX] = {FALSE,FALSE, FALSE, FALSE};

volatile DSPSTATUS_T _rDspStatus[AUD_DEC_MAX];       // DSP status of control flag for first decoder and common code
DSPVARS_T _rDspVars[AUD_DEC_MAX];     // variable for first decoder

UINT32 _u4ShareMem; // DSP/RISC shared memory
UINT32 _u4DspBuf;   // DSP working area start address for common page
UINT32 _u4DspDramBuf[8]; // DSP working area start address for each page (Normal mode)
UINT32 _u4DspCmptBuf[8]; // DSP working area start address for each page (Compact mode)

#ifdef DSP_SUPPORT_17BIT
UINT32 _u4DspICacheAddr[12]; //5368/96 // DSP instruction cache start address for each page
#else
UINT32 _u4DspICacheAddr[8]; //5365 // DSP instruction cache start address for each page
#endif

UINT32 _u4AFIFO[4][2]; // 0: start,1:end
UINT32 _u4MixSndBuf; // start address of mix sound buffer
#ifdef CC_AUD_HDMI_PARSER_2_0_SUPPORT
UINT32 _u4AFIFOHdmiParser[2]; // 0: start,1:end //this AFIFO is used for hdmi pre_parser
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
UINT32 _u4SBCEncBuf[2]; // 0: start, 1: end
#endif

DSP_CMD_HIS_T   _rDspCmdHistory;
extern const UINT8 _pbD[];

UINT32 _u4AtvTvSys = 0;
MW_TV_AUD_SYS_T _rTvAudSys;
AUD_TYPE_T      _aeAudType[AUD_DEC_MAX];
TV_AUD_SYS_T    _eAudTvSystem = SV_MTS;
static TV_AUD_SYS_T _eDetTvSrcMode = SV_PAL_DK;
static BOOL _fgEuroCanalPlusMode = FALSE;


static UINT32 gu4Energy45M = 0;
static UINT32 gu4Energy55M = 0;
static UINT32 gu4Energy60M = 0;
static UINT32 gu4Energy65M = 0;
//static UINT32 gu4Energy655M = 0;

UINT8 _uStcDiffBound[3] = {0,0,0};

#ifdef MP3ENC_SUPPORT
PBS_BLK_SET _tPbsBlk[2];
#endif

#if 1///def CC_VORBIS_SUPPORT ///def DSP_VORBIS_SUPPORT
UINT8 _uVorbisMkTbl[AUD_DEC_MAX] = {FALSE, FALSE, FALSE, FALSE};
UINT8 _uVorbisTblDone[AUD_DEC_MAX] = {FALSE, FALSE, FALSE, FALSE};
UINT8 _uVorbisIntDec = 0;
#endif

static UINT32 _u4Capability[2] = {0,0};

/*
  CAUTION : The order is related to the block index in dsp_common.h
*/
UINT32 _u4DspDramBufLen[] =
{
    DSP_DAT_DEC1_SZ,
    DSP_DAT_DEC2_SZ,
    DSP_DAT_DEC3_SZ,
    (DSP_DAT_CMM_NORMAL_SZ+DSP_DAT_CMM_SZ),
    DSP_DAT_DEC4_SZ,
    DSP_MIXSNDBUF_ALL_SZ,
    0,
    0
};
UINT32 _u4DspICacheLen[] =
{
    DSP_ROM_SZ,
    DSP_POS_RAM_SZ,
    DSP_CTR_RAM_SZ,
    DSP_CMM_RAM_SZ,
    DSP_DEC4_RAM_SZ,
    DSP_DEC1_RAM_SZ,
    DSP_DEC3_RAM_SZ,
  #ifdef DSP_SUPPORT_17BIT
    DSP_DEC2_RAM_SZ-0x20000,
    DSP_DEC4_RAM_SZ,
    DSP_DEC1_RAM_SZ,
    DSP_DEC3_RAM_SZ,
    DSP_DEC2_RAM_SZ-0x20000
  #else
    DSP_DEC2_RAM_SZ
  #endif
};

static CHAR * _aszTvSysMode[] =
{
	"SV_NONE_DETECTED",
	"SV_MTS",
	"SV_FM_FM",
	"SV_NTSC_M",
	"SV_A2_BG",
	"SV_A2_DK1",
	"SV_A2_DK2",
	"SV_A2_DK3",
	"SV_PAL_I",
	"SV_PAL_BG",
	"SV_PAL_DK",
	"SV_SECAM_L",
	"SV_SECAM_L_PLUM",
	"SV_SECAM_BG",
	"SV_SECAM_DK"
};

static CHAR * _aszAudOutputMode[] =
{
    "UNKNOWN",
    "MONO",                  /* 1/0 */
    "MONO_SUB",              /* 1+sub-language */
    "DUAL_MONO",             /* 1+1 */
    "STEREO",                /* 2/0 */
    "STEREO_SUB",            /* 2+sub-language */
    "STEREO_DOLBY_SURROUND", /* 2/0, dolby surround */
    "SURROUND_2CH",          /* 2/1 */
    "SURROUND",              /* 3/1 */
    "3_0",                   /* 3/0 */
    "4_0",                   /* 2/2 */
    "5_1",                   /* 3/2 */
    "7_1",                   /* 5/2 */
    "FM_MONO_NICAM_MONO",
    "FM_MONO_NICAM_STEREO",
    "FM_MONO_NICAM_DUAL",
    "OTHERS",
    "5_0",                   /* 3/2 */
    "JOINT_STEREO",
    "FMRDO_MONO",
    "FMRDO_STEREO"
};


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

static void _GetAudInfo(UINT8 u1DecId, AUD_DEC_INFO_T * prAudInfo)  // AC3 stream
{
    AUD_DEC_STREAM_FROM_T  rAudDecStreamFrom;
    AUD_FMT_T              eAudDecFormat;
    UINT8  u1Acmode;
    UINT8 u1Ac3DecType;
    UINT8  u1SmpRate;
    UINT32 u4DataRate;
    BOOL u1DecTypRet;

    if (prAudInfo != NULL)
    {
        // Get decoding format
        u1DecTypRet = AUD_DRVGetDecodeType(u1DecId, &rAudDecStreamFrom, &eAudDecFormat);
        UNUSED(u1DecTypRet);
        prAudInfo->e_aud_fmt = eAudDecFormat;
        UNUSED(rAudDecStreamFrom);

        // Get audio information
        if (prAudInfo->e_aud_fmt == AUD_FMT_AC3)
        {
            AUD_GetStreamInfo_AC3(u1DecId, &u1Acmode, &u1Ac3DecType, &u1SmpRate, &u4DataRate);
	     switch(u1Ac3DecType & 0x01)
	     {
            case 0x00:
                prAudInfo->e_dec_type = AUD_DECODE_TYPE_AC3;
                break;
            case 0x01:
                prAudInfo->e_dec_type = AUD_DECODE_TYPE_EAC3;
                break;
            default:
                prAudInfo->e_dec_type = AUD_DECODE_TYPE_UNKNOWN;
                break;
	     }
        }
        else
        {
            AUD_GetStreamInfo(u1DecId, &u1Acmode, &u1SmpRate, &u4DataRate);
        }

        switch (u1Acmode)
        {
            case 0: // 1+1
            	prAudInfo->e_aud_type = AUD_TYPE_DUAL_MONO;
           	    break;
            case 1: // 1/0
            	prAudInfo->e_aud_type = AUD_TYPE_MONO;
           	    break;
            case 2: // 2/0
            	prAudInfo->e_aud_type = AUD_TYPE_STEREO;
           	    break;
            case 3: // 3/0
            	prAudInfo->e_aud_type = AUD_TYPE_3_0;
           	    break;
            case 4: // 2/1
            	prAudInfo->e_aud_type = AUD_TYPE_SURROUND_2CH;
           	    break;
            case 5: // 3/1
            	prAudInfo->e_aud_type = AUD_TYPE_SURROUND;
           	    break;
            case 6: // 2/2
            	prAudInfo->e_aud_type = AUD_TYPE_4_0;
           	    break;
            case 7: // 3/2
            		prAudInfo->e_aud_type = AUD_TYPE_5_0;
           	    break;
            case 0x11: // 1/0.L
                prAudInfo->e_aud_type = AUD_TYPE_MONO_LFE;
                break;
            case 0x12: // 2/0.L
                prAudInfo->e_aud_type = AUD_TYPE_STEREO_LFE;
                break;
            case 0x13: // 3/0.L
                prAudInfo->e_aud_type = AUD_TYPE_3_1;
                break;
            case 0x14: // 2/1.L
                prAudInfo->e_aud_type = AUD_TYPE_SURROUND_2CH_LFE;
                break;
            case 0x15: // 3/1.L
                prAudInfo->e_aud_type = AUD_TYPE_SURROUND_LFE;
                break;
            case 0x16: // 2/2.L
                prAudInfo->e_aud_type = AUD_TYPE_4_1;
                break;
            case 0x17: // 3/2.L
            	prAudInfo->e_aud_type = AUD_TYPE_5_1;
           	    break;
            default: //
            	LOG(5, "%s,", "Error acmode");
            	prAudInfo->e_aud_type = AUD_TYPE_OTHERS;
            	break;
        }

        switch (u1SmpRate)
        {
     	    case 2: // 16 KHz
     	    	prAudInfo->ui4_sample_rate = 16000;
                break;
           case 3: // 32 KHz
                prAudInfo->ui4_sample_rate = 32000;
                break;
           case 4: // 8 KHz
                prAudInfo->ui4_sample_rate = 8000;
                break;
     	    case 5: // 11KHz
     	         prAudInfo->ui4_sample_rate = 11000;
                break;
           case 6: // 12 KHz
                prAudInfo->ui4_sample_rate = 12000;
                break;
           case 7: // 22 KHz
                prAudInfo->ui4_sample_rate = 22000;
                break;
     	    case 8: // 44.1KHz
                prAudInfo->ui4_sample_rate = 44100;
                break;
           case 9: // 88.2KHz
                prAudInfo->ui4_sample_rate = 88200;
                break;
           case 10: // 176 KHz
                prAudInfo->ui4_sample_rate = 176000;
                break;
           case 12: // 24 KHz
                prAudInfo->ui4_sample_rate = 24000;
                break;
     	    case 13: // 48KHz
     	    	prAudInfo->ui4_sample_rate = 48000;
                break;
           case 14: // 96KHz
                prAudInfo->ui4_sample_rate = 96000;
                break;
           case 15: // 192 KHz
                prAudInfo->ui4_sample_rate = 192000;
                break;
            default: // Error sample rate
                prAudInfo->ui4_sample_rate = 48000;
                LOG(5, "Unknown sample rate");
                break;
        }

        //Printf("%d Kbps,",u2DataRate);
        prAudInfo->ui4_data_rate = u4DataRate ;

        // FIXME // TODO
        prAudInfo->ui1_bit_depth = 24;
    }
}

BOOL _GetMpegAudInfo(UINT8 u1DecId, AUD_DEC_INFO_T * prAudInfo)
{
    AUD_DEC_STREAM_FROM_T eAudDecStreamFrom;
    AUD_FMT_T eAudDecFormat;
    UINT8 u1Mode;
    static AUD_TYPE_T _eModePre = AUD_TYPE_STEREO;
    BOOL fgIsUpdate = FALSE;

#ifdef CC_MT5391_AUD_3_DECODER
    AUD_DEC_ID_VALIDATE_3(u1DecId);
#else
    AUD_DEC_ID_VALIDATE_2(u1DecId);
#endif

    if (prAudInfo != NULL)
    {
        // Get decoding format
        UNUSED(AUD_DRVGetDecodeType(u1DecId, &eAudDecStreamFrom, &eAudDecFormat));
        prAudInfo->e_aud_fmt = eAudDecFormat;
        UNUSED(eAudDecStreamFrom);

        if (u1DecId == AUD_DEC_MAIN)
        {
            u1Mode = (UINT8)(wReadDspWORD(ADDR_D2RC_MPEG_MODE));
            prAudInfo->ui4_sample_rate = dReadDspCommDram(ADDR_D2RC_MPEG_SAMPLINGFREQ);
            prAudInfo->ui4_data_rate = dReadDspCommDram(ADDR_D2RC_MPEG_BITRATE);
        }
#ifdef CC_MT5391_AUD_3_DECODER
        else if (u1DecId == AUD_DEC_AUX)
#else
        else
#endif
        {
            u1Mode = (UINT8)(wReadDspWORD(ADDR_D2RC_MPEG_MODE_DEC2));
            prAudInfo->ui4_sample_rate = dReadDspCommDram(ADDR_D2RC_MPEG_SAMPLINGFREQ_DEC2);
            prAudInfo->ui4_data_rate = dReadDspCommDram(ADDR_D2RC_MPEG_BITRATE_DEC2);
        }
#ifdef CC_MT5391_AUD_3_DECODER
        else
        {
            u1Mode = (UINT8)(wReadDspWORD(ADDR_D2RC_MPEG_MODE_DEC3));
            prAudInfo->ui4_sample_rate = dReadDspCommDram(ADDR_D2RC_MPEG_SAMPLINGFREQ_DEC3);
            prAudInfo->ui4_data_rate = dReadDspCommDram(ADDR_D2RC_MPEG_BITRATE_DEC3);
        }
#endif

        // Get mode to determine type
        if (u1Mode == 3)
        {
            prAudInfo->e_aud_type = AUD_TYPE_MONO;
        }
        else if (u1Mode == 2)
        {
            prAudInfo->e_aud_type = AUD_TYPE_DUAL_MONO;
        }
        else
        {   // For both stereo and joint stereo
            prAudInfo->e_aud_type = AUD_TYPE_STEREO;
        }

        if (_eModePre != prAudInfo->e_aud_type)
        {
            fgIsUpdate = TRUE;
            _eModePre = prAudInfo->e_aud_type;
        }

        prAudInfo->ui1_bit_depth = 24;
    }

    return fgIsUpdate;
}

BOOL _GetMpegAudInfoWithLayer(UINT8 u1DecId, AUD_DEC_INFO_T * prAudInfo, UINT8 *prMPEGLayer)
{
    AUD_DEC_STREAM_FROM_T eAudDecStreamFrom;
    AUD_FMT_T eAudDecFormat;
    UINT8 u1Mode,u1Layer=0;
    static AUD_TYPE_T _eModePre = AUD_TYPE_STEREO;
    BOOL fgIsUpdate = FALSE;

#ifdef CC_MT5391_AUD_3_DECODER
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX) || (u1DecId == AUD_DEC_THIRD));
#else
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));
#endif

    if (prAudInfo != NULL)
    {
        // Get decoding format
        UNUSED(AUD_DRVGetDecodeType(u1DecId, &eAudDecStreamFrom, &eAudDecFormat));
        prAudInfo->e_aud_fmt = eAudDecFormat;
        UNUSED(eAudDecStreamFrom);

        if (u1DecId == AUD_DEC_MAIN)
        {
            u1Layer= (UINT8)(dReadDspCommDram(ADDR_D2RC_MPEG_ID_LAYER));
			u1Mode = (UINT8)(wReadDspWORD(ADDR_D2RC_MPEG_MODE));
            prAudInfo->ui4_sample_rate = dReadDspCommDram(ADDR_D2RC_MPEG_SAMPLINGFREQ);
            prAudInfo->ui4_data_rate = dReadDspCommDram(ADDR_D2RC_MPEG_BITRATE);
        }
#ifdef CC_MT5391_AUD_3_DECODER
        else if (u1DecId == AUD_DEC_AUX)
#else
        else
#endif
        {
            u1Layer= (UINT8)(dReadDspCommDram(ADDR_D2RC_MPEG_ID_LAYER_DEC2));
			u1Mode = (UINT8)(wReadDspWORD(ADDR_D2RC_MPEG_MODE_DEC2));
            prAudInfo->ui4_sample_rate = dReadDspCommDram(ADDR_D2RC_MPEG_SAMPLINGFREQ_DEC2);
            prAudInfo->ui4_data_rate = dReadDspCommDram(ADDR_D2RC_MPEG_BITRATE_DEC2);
        }
#ifdef CC_MT5391_AUD_3_DECODER
        else
        {
            u1Layer= (UINT8)(dReadDspCommDram(ADDR_D2RC_MPEG_ID_LAYER_DEC3));
			u1Mode = (UINT8)(wReadDspWORD(ADDR_D2RC_MPEG_MODE_DEC3));
            prAudInfo->ui4_sample_rate = dReadDspCommDram(ADDR_D2RC_MPEG_SAMPLINGFREQ_DEC3);
            prAudInfo->ui4_data_rate = dReadDspCommDram(ADDR_D2RC_MPEG_BITRATE_DEC3);
        }
#endif

        // Get mode to determine type
        if (u1Mode == 3)
        {
            prAudInfo->e_aud_type = AUD_TYPE_MONO;
        }
        else if (u1Mode == 2)
        {
            prAudInfo->e_aud_type = AUD_TYPE_DUAL_MONO;
        }
        else
        {   // For both stereo and joint stereo
            prAudInfo->e_aud_type = AUD_TYPE_STEREO;
        }

        if (_eModePre != prAudInfo->e_aud_type)
        {
            fgIsUpdate = TRUE;
            _eModePre = prAudInfo->e_aud_type;
        }

        prAudInfo->ui1_bit_depth = 24;
    }
	*prMPEGLayer=u1Layer;
    return fgIsUpdate;
}

static void _GetSbcAudInfo(UINT8 u1DecId, AUD_DEC_INFO_T * prAudInfo)
{
    AUD_DEC_STREAM_FROM_T eAudDecStreamFrom;
    AUD_FMT_T eAudDecFormat;
    UINT8 u1Mode;

#ifdef CC_MT5391_AUD_3_DECODER
    AUD_DEC_ID_VALIDATE_3(u1DecId);
#else
    AUD_DEC_ID_VALIDATE_2(u1DecId);
#endif

    if (prAudInfo != NULL)
    {
        // Get decoding format
        UNUSED(AUD_DRVGetDecodeType(u1DecId, &eAudDecStreamFrom, &eAudDecFormat));
        prAudInfo->e_aud_fmt = eAudDecFormat;
        UNUSED(eAudDecStreamFrom);

        if (u1DecId == AUD_DEC_MAIN)
        {
            u1Mode = (UINT8)(wReadDspWORD(ADDR_D2RC_SBC_MODE));
            prAudInfo->ui4_sample_rate = dReadDspCommDram(ADDR_D2RC_SBC_SAMPLINGFREQ);
            prAudInfo->ui4_data_rate = dReadDspCommDram(ADDR_D2RC_SBC_BITRATE);
        }
        else
        {
            u1Mode = (UINT8)(wReadDspWORD(ADDR_D2RC_SBC_MODE_DEC2));
            prAudInfo->ui4_sample_rate = dReadDspCommDram(ADDR_D2RC_SBC_SAMPLINGFREQ_DEC2);
            prAudInfo->ui4_data_rate = dReadDspCommDram(ADDR_D2RC_SBC_BITRATE_DEC2);
        }

        // Get mode to determine type
        if (u1Mode == 0)
        {
            prAudInfo->e_aud_type = AUD_TYPE_MONO;
        }
        else if (u1Mode == 1)
        {
            prAudInfo->e_aud_type = AUD_TYPE_DUAL_MONO;
        }
        else
        {   // For both stereo and joint stereo
            prAudInfo->e_aud_type = AUD_TYPE_STEREO;
        }

        prAudInfo->ui1_bit_depth = 24;
    }
}

UINT32 DSP_GetMpegFrameSize(UINT8 u1DecId)
{
        if (u1DecId == AUD_DEC_MAIN)
        {
            return (dReadDspCommDram(ADDR_D2RC_MPEG_FRAME_SIZE) >> 8);
        }
#ifndef CC_MT5391_AUD_3_DECODER
        else
#else
        else if (u1DecId == AUD_DEC_AUX)
#endif
        {
            return (dReadDspCommDram(ADDR_D2RC_MPEG_FRAME_SIZE_DEC2) >> 8);
        }
#ifdef CC_MT5391_AUD_3_DECODER
        else
        {
            return (dReadDspCommDram(ADDR_D2RC_MPEG_FRAME_SIZE_DEC3) >> 8);
        }
#endif
}


static void _GetDTSAudInfo(UINT8 u1DecId, AUD_DEC_INFO_T * prAudInfo)
{
    UINT32 sample_rate;
	UINT32 input_ch_num;

#ifdef CC_MT5391_AUD_3_DECODER
    AUD_DEC_ID_VALIDATE_3(u1DecId);
#else
    AUD_DEC_ID_VALIDATE_2(u1DecId);
#endif

    if (prAudInfo != NULL)
    {
        // Get decoding format
	    prAudInfo->e_aud_fmt = AUD_FMT_DTS;
	    prAudInfo->ui1_bit_depth = 24;

        prAudInfo->ui4_data_rate = DSP_GetDataRate(u1DecId);

		LOG(0,"data rate = %d ",prAudInfo->ui4_data_rate);

        sample_rate = u4DspGetSampleRate(u1DecId);


        switch (sample_rate)
        {
     	    case 1: // 8 KHz
     	    	prAudInfo->ui4_sample_rate = 8000;
                break;
           case 2: // 16 KHz
                prAudInfo->ui4_sample_rate = 16000;
                break;
           case 3: // 32 KHz
                prAudInfo->ui4_sample_rate = 32000;
                break;
     	    case 6: // 11KHz
     	         prAudInfo->ui4_sample_rate = 11000;
                break;
           case 7: // 22 KHz
                prAudInfo->ui4_sample_rate = 22000;
                break;
     	    case 8: // 44.1KHz
                prAudInfo->ui4_sample_rate = 44100;
                break;
           case 11: // 12 KHz
                prAudInfo->ui4_sample_rate = 12000;
                break;
     	    case 12: // 24KHz
     	    	prAudInfo->ui4_sample_rate = 24000;
                break;
           case 13: // 48KHz
                prAudInfo->ui4_sample_rate = 48000;
                break;
            default: // Error sample rate
                prAudInfo->ui4_sample_rate = 48000;
                LOG(5, "Invalid sample rate");
                break;
        	}

		if (u1DecId == AUD_DEC_MAIN)
		{
			input_ch_num = wReadDspWORD (ADDR_D2RC_INPUT_CH_CFG);
		}
		else
		{
			input_ch_num = wReadDspWORD (ADDR_D2RC_INPUT_CH_CFG_DEC2);
		}

        switch (input_ch_num & 0xF)
        	{
        	   case 7:
				prAudInfo->e_aud_type = AUD_TYPE_5_1;
			   	break;
			   case 1:
			   	prAudInfo->e_aud_type =AUD_TYPE_MONO;
				break;
			   case 6:
			   	prAudInfo->e_aud_type =AUD_TYPE_4_0;
				break;
			   case 2:
				prAudInfo->e_aud_type =AUD_TYPE_STEREO;
				break;
			   default:
				prAudInfo->e_aud_type =AUD_TYPE_STEREO;
				LOG(5, "%s,", "Other AMODE");
 				break;
        	}

    }
}


static void _GetAacAudInfo(UINT8 u1DecId, AUD_DEC_INFO_T * prAudInfo)
{
    UINT32 u4Mode, u4SmpRate, u4Version, u4Format;
    UINT32 u4DataRate;
#ifdef CC_MT5391_AUD_3_DECODER
    AUD_DEC_ID_VALIDATE_3(u1DecId);
#else
    AUD_DEC_ID_VALIDATE_2(u1DecId);
#endif


    if (prAudInfo != NULL)
    {
        if (u1DecId == AUD_DEC_MAIN)
        {
            // Get mode to determine type
            u4Mode = dReadDspCommDram(ADDR_D2RC_AAC_IS_DUAL_MONO) >> 8;
            if (u4Mode == 1)
            {
                prAudInfo->e_aud_type = AUD_TYPE_DUAL_MONO;
            }
            else
            {
                u4Mode = dReadDspCommDram(ADDR_D2RC_AAC_CH_MODE) >> 8;
                switch (u4Mode)
                {
                    case 1: // 1/0
                    	prAudInfo->e_aud_type = AUD_TYPE_MONO;
                    	break;
                    case 2: // 2/0
                    	prAudInfo->e_aud_type = AUD_TYPE_STEREO;
                    	break;
                    case 3: // 3/0
                    	prAudInfo->e_aud_type = AUD_TYPE_3_0;
                    	break;
                    case 4: // 2/1
                    	prAudInfo->e_aud_type = AUD_TYPE_SURROUND_2CH;
                    	break;
                    case 5: // 3/1
                    	prAudInfo->e_aud_type = AUD_TYPE_SURROUND;
                    	break;
                    case 6: // 2/2
                    	prAudInfo->e_aud_type = AUD_TYPE_4_0;
                    	break;
                    case 7: // 3/2
                    	prAudInfo->e_aud_type = AUD_TYPE_5_0;
                    	break;
                    case 8: // 3/2
                    	prAudInfo->e_aud_type = AUD_TYPE_5_1;
                    	break;
                    case 9: // 2+LFE
                        prAudInfo->e_aud_type = AUD_TYPE_STEREO_LFE;
                        break;
                    default: //
                    	LOG(5, "%s,", "Error acmode");
                    	prAudInfo->e_aud_type = AUD_TYPE_OTHERS;
                    	break;
                }
            }

            // Get AAC version
            u4Version = dReadDspCommDram(ADDR_D2RC_AAC_VERSION) >> 8;

            // Get AAC transmission format
            u4Format = dReadDspCommDram(ADDR_D2RC_AAC_TRANS_FORMAT) >> 8;
            if (u4Format == 0)
            {
                LOG(5,"This is LATM/LOAS format\n");
            }
            else
            {
                LOG(5,"This is ADTS format\n");
            }
            // Get sampling frequency
            u4SmpRate = dReadDspCommDram(ADDR_D2RC_AAC_SAMPLINGRATE) >> 8;
            if (u4Version == 0)     //plain aac
            {
    		switch (u4SmpRate)
            	{
                    case 3: // 48 KHz
                        prAudInfo->ui4_sample_rate = 48000;
                        break;
                    case 4: // 44.1 KHz
                        prAudInfo->ui4_sample_rate = 44100;
                        break;
                    case 5: // 32 KHz
                        prAudInfo->ui4_sample_rate = 32000;
                        break;
                    case 6: // 24 KHz
                        	prAudInfo->ui4_sample_rate = 24000;
                        break;
                    case 7: // 22 KHz
                        prAudInfo->ui4_sample_rate = 22050;
                        break;
                    case 8: // 16 KHz
                        prAudInfo->ui4_sample_rate = 16000;
                        break;
                    case 9: // 12 KHz
                        	prAudInfo->ui4_sample_rate = 12000;
                        break;
                    case 10: // 11 KHz
                        prAudInfo->ui4_sample_rate = 11025;
                        break;
                    case 11: // 8 KHz
                        prAudInfo->ui4_sample_rate = 8000;
                        break;
                    default: // Error sample rate
                        prAudInfo->ui4_sample_rate = 48000;
                        LOG(5, "Unsupported sample rate");
                        break;
            	}
            }
            else        //HE-AAC
            {
                switch (u4SmpRate)
            	{
                    case 6: // 24 KHz
                 	    prAudInfo->ui4_sample_rate = 48000;
                        break;
                    case 7: // 22 KHz
                        prAudInfo->ui4_sample_rate = 44100;
                        break;
                    case 8: // 16 KHz
                        prAudInfo->ui4_sample_rate = 32000;
                        break;
                    case 9: // 12 KHz
             	    	prAudInfo->ui4_sample_rate = 24000;
                        break;
                    case 10: // 11 KHz
                        prAudInfo->ui4_sample_rate = 22050;
                        break;
                    case 11: // 8 KHz
                        prAudInfo->ui4_sample_rate = 16000;
                        break;
                    default: // Error sample rate
                        prAudInfo->ui4_sample_rate = 48000;
                        LOG(5, "Unsupported sample rate");
                        break;
                }

            }

            // Get birate
            u4DataRate = dReadDspCommDram(ADDR_D2RC_AAC_BITRATE);
            prAudInfo->ui4_data_rate = u4DataRate;
        }
        else if (u1DecId == AUD_DEC_AUX)
        {
            // Get mode to determine type
            u4Mode = dReadDspCommDram(ADDR_D2RC_AAC_IS_DUAL_MONO_DEC2) >> 8;
            if (u4Mode == 1)
            {
                prAudInfo->e_aud_type = AUD_TYPE_DUAL_MONO;
            }
            else
            {
                u4Mode = dReadDspCommDram(ADDR_D2RC_AAC_CH_MODE_DEC2) >> 8;
                switch (u4Mode)
                {
                    case 1: // 1/0
                    	prAudInfo->e_aud_type = AUD_TYPE_MONO;
                    	break;
                    case 2: // 2/0
                    	prAudInfo->e_aud_type = AUD_TYPE_STEREO;
                    	break;
                    case 3: // 3/0
                    	prAudInfo->e_aud_type = AUD_TYPE_3_0;
                    	break;
                    case 4: // 2/1
                    	prAudInfo->e_aud_type = AUD_TYPE_SURROUND_2CH;
                    	break;
                    case 5: // 3/1
                    	prAudInfo->e_aud_type = AUD_TYPE_SURROUND;
                    	break;
                    case 6: // 2/2
                    	prAudInfo->e_aud_type = AUD_TYPE_4_0;
                    	break;
                    case 7: // 3/2
                    	prAudInfo->e_aud_type = AUD_TYPE_5_0;
                    	break;
                    case 8: // 3/2
                    	prAudInfo->e_aud_type = AUD_TYPE_5_1;
                    	break;
                    case 9: // 2+LFE
                        prAudInfo->e_aud_type = AUD_TYPE_STEREO_LFE;
                        break;
                    default: //
                    	LOG(5, "%s,", "Error acmode");
                    	prAudInfo->e_aud_type = AUD_TYPE_OTHERS;
                    	break;
            	}
            }

            // Get AAC version
            u4Version = dReadDspCommDram(ADDR_D2RC_AAC_VERSION_DEC2) >> 8;

            // Get AAC transmission format
            u4Format = dReadDspCommDram(ADDR_D2RC_AAC_TRANS_FORMAT_DEC2) >> 8;
            if (u4Format == 0)
            {
                LOG(5,"This is LATM/LOAS format\n");
            }
            else
            {
                LOG(5,"This is ADTS format\n");
            }

            // Get sampling frequency
            u4SmpRate = dReadDspCommDram(ADDR_D2RC_AAC_SAMPLINGRATE_DEC2) >> 8;
            if (u4Version == 0)     //plain aac
            {
                switch (u4SmpRate)
            	{
                    case 3: // 48 KHz
                    	prAudInfo->ui4_sample_rate = 48000;
                        break;
                    case 4: // 44.1 KHz
                        prAudInfo->ui4_sample_rate = 44100;
                        break;
                    case 5: // 32 KHz
                        prAudInfo->ui4_sample_rate = 32000;
                        break;
                    case 6: // 24 KHz
                        prAudInfo->ui4_sample_rate = 24000;
                        break;
                    case 7: // 22 KHz
                        prAudInfo->ui4_sample_rate = 22050;
                        break;
                    case 8: // 16 KHz
                        prAudInfo->ui4_sample_rate = 16000;
                        break;
                    case 9: // 12 KHz
                       	prAudInfo->ui4_sample_rate = 12000;
                        break;
                    case 10: // 11 KHz
                        prAudInfo->ui4_sample_rate = 11025;
                        break;
                    case 11: // 8 KHz
                        prAudInfo->ui4_sample_rate = 8000;
                        break;
                    default: // Error sample rate
                        prAudInfo->ui4_sample_rate = 48000;
                        LOG(5, "Unsupported sample rate");
                        break;
            	}
            }
            else        //HE-AAC
            {
                switch (u4SmpRate)
            	{
                    case 6: // 24 KHz
                        prAudInfo->ui4_sample_rate = 48000;
                        break;
                    case 7: // 22 KHz
                        prAudInfo->ui4_sample_rate = 44100;
                        break;
                    case 8: // 16 KHz
                        prAudInfo->ui4_sample_rate = 32000;
                        break;
                    case 9: // 12 KHz
                        	prAudInfo->ui4_sample_rate = 24000;
                        break;
                    case 10: // 11 KHz
                        prAudInfo->ui4_sample_rate = 22050;
                        break;
                    case 11: // 8 KHz
                        prAudInfo->ui4_sample_rate = 16000;
                        break;
                    default: // Error sample rate
                        prAudInfo->ui4_sample_rate = 48000;
                        LOG(5, "Unsupported sample rate");
                        break;
                }
            }
            // Get birate
            u4DataRate = dReadDspCommDram(ADDR_D2RC_AAC_BITRATE_DEC2);
            prAudInfo->ui4_data_rate = u4DataRate;
        }
#ifdef CC_MT5391_AUD_3_DECODER
        else if (u1DecId == AUD_DEC_THIRD)
        {
            // Get mode to determine type
            u4Mode = dReadDspCommDram(ADDR_D2RC_AAC_IS_DUAL_MONO_DEC3) >> 8;
            if (u4Mode == 1)
            {
            	prAudInfo->e_aud_type = AUD_TYPE_DUAL_MONO;
            }
            else
            {
                u4Mode = dReadDspCommDram(ADDR_D2RC_AAC_CH_MODE_DEC3) >> 8;
                switch (u4Mode)
                {
                    case 1: // 1/0
                    	prAudInfo->e_aud_type = AUD_TYPE_MONO;
                    	break;
                    case 2: // 2/0
                    	prAudInfo->e_aud_type = AUD_TYPE_STEREO;
                    	break;
                    case 3: // 3/0
                    	prAudInfo->e_aud_type = AUD_TYPE_3_0;
                    	break;
                    case 4: // 2/1
                    	prAudInfo->e_aud_type = AUD_TYPE_SURROUND_2CH;
                    	break;
                    case 5: // 3/1
                    	prAudInfo->e_aud_type = AUD_TYPE_SURROUND;
                    	break;
                    case 6: // 2/2
                    	prAudInfo->e_aud_type = AUD_TYPE_4_0;
                    	break;
                    case 7: // 3/2
                    	prAudInfo->e_aud_type = AUD_TYPE_5_0;
                    	break;
                    case 8: // 3/2
                    	prAudInfo->e_aud_type = AUD_TYPE_5_1;
                    	break;
                    case 9: // 2+LFE
                        prAudInfo->e_aud_type = AUD_TYPE_STEREO_LFE;
                        break;
                    default: //
                    	LOG(5, "%s,", "Error acmode");
                    	prAudInfo->e_aud_type = AUD_TYPE_OTHERS;
                    	break;
                }
            }

            // Get sampling frequency
            u4SmpRate = dReadDspCommDram(ADDR_D2RC_AAC_SAMPLINGRATE_DEC3) >> 8;

            switch (u4SmpRate)
            {
                case 3: // 48 KHz
                	prAudInfo->ui4_sample_rate = 48000;
                    break;
                case 4: // 44.1 KHz
                    prAudInfo->ui4_sample_rate = 44100;
                    break;
                case 5: // 32 KHz
                    prAudInfo->ui4_sample_rate = 32000;
                    break;
                case 6: // 24 KHz
                    	prAudInfo->ui4_sample_rate = 24000;
                    break;
                case 7: // 22 KHz
                    prAudInfo->ui4_sample_rate = 22000;
                    break;
                case 8: // 16 KHz
                    prAudInfo->ui4_sample_rate = 16000;
                    break;
                case 9: // 12 KHz
                    	prAudInfo->ui4_sample_rate = 12000;
                    break;
                case 10: // 11 KHz
                    prAudInfo->ui4_sample_rate = 11000;
                    break;
                case 11: // 8 KHz
                    prAudInfo->ui4_sample_rate = 8000;
                    break;
                default: // Error sample rate
                    prAudInfo->ui4_sample_rate = 48000;
                    LOG(5, "Unsupported sample rate");
                    break;
            }
            // Get birate
            u4DataRate = dReadDspCommDram(ADDR_D2RC_AAC_BITRATE_DEC3);
            prAudInfo->ui4_data_rate = u4DataRate;
        }
#endif
	    prAudInfo->e_aud_fmt = AUD_FMT_AAC;
	    prAudInfo->ui1_bit_depth = 24;
    }
}

static void _GetDraAudInfo(UINT8 u1DecId, AUD_DEC_INFO_T * prAudInfo)
{
    UINT8 u1ChNum = 0;
    UINT8 u1FS = 0;

#ifdef CC_MT5391_AUD_3_DECODER
    AUD_DEC_ID_VALIDATE_3(u1DecId);
#else
    AUD_DEC_ID_VALIDATE_2(u1DecId);
#endif

    u1FS = DSP_GetSampleRate(u1DecId);
    if (prAudInfo != NULL)
    {
        prAudInfo->e_aud_fmt = AUD_FMT_DRA;
        prAudInfo->ui4_data_rate = 96000;    // no such info in DRA header.
        prAudInfo->ui1_bit_depth = 24;

        if (u1DecId == AUD_DEC_MAIN)
        {
            u1ChNum = (UINT8)(wReadDspWORD(ADDR_D2RC_DRA_CH_NUM));
        }
#ifdef CC_MT5391_AUD_3_DECODER
        else if (u1DecId == AUD_DEC_AUX)
#else
        else
#endif
        {
            u1ChNum = (UINT8)(wReadDspWORD(ADDR_D2RC_DAR_CH_NUM_DEC2));
        }
#ifdef CC_MT5391_AUD_3_DECODER
        else
        {
            //u1ChNum = (UINT8)(wReadDspWORD(ADDR_D2RC_MPEG_MODE_DEC3));
            u1ChNum = 2;    //current there is no dra dec3.
        }
#endif

        switch (u1FS)
        {
        case 2: // 16 KHz
     	    prAudInfo->ui4_sample_rate = 16000;
            break;
        case 3: // 32 KHz
            prAudInfo->ui4_sample_rate = 32000;
            break;
        case 4: // 8 KHz
            prAudInfo->ui4_sample_rate = 8000;
            break;
        case 5: // 11KHz
            prAudInfo->ui4_sample_rate = 11000;
            break;
        case 6: // 12 KHz
            prAudInfo->ui4_sample_rate = 12000;
            break;
        case 7: // 22 KHz
            prAudInfo->ui4_sample_rate = 22000;
            break;
        case 8: // 44.1KHz
            prAudInfo->ui4_sample_rate = 44100;
            break;
        case 9: // 88.2KHz
            prAudInfo->ui4_sample_rate = 88200;
            break;
        case 10: // 176 KHz
            prAudInfo->ui4_sample_rate = 176000;
            break;
        case 12: // 24 KHz
            prAudInfo->ui4_sample_rate = 24000;
            break;
        case 13: // 48KHz
            prAudInfo->ui4_sample_rate = 48000;
            break;
        case 14: // 96KHz
            prAudInfo->ui4_sample_rate = 96000;
            break;
        case 15: // 192 KHz
            prAudInfo->ui4_sample_rate = 192000;
            break;
        default: // Error sample rate
            prAudInfo->ui4_sample_rate = 48000;
            break;
        }

        switch (u1ChNum)
        {
            case 0: // 1+1
            	prAudInfo->e_aud_type = AUD_TYPE_DUAL_MONO;
           	    break;
            case 1: // 1/0
            	prAudInfo->e_aud_type = AUD_TYPE_MONO;
           	    break;
            case 2: // 2/0
            	prAudInfo->e_aud_type = AUD_TYPE_STEREO;
           	    break;
            case 3: // 3/0
            	prAudInfo->e_aud_type = AUD_TYPE_3_0;
           	    break;
            case 5:
            	prAudInfo->e_aud_type = AUD_TYPE_5_1;
           	    break;
            default:
            	prAudInfo->e_aud_type = AUD_TYPE_STEREO;
            	break;
        }
    }
}

#if 1///def CC_VORBIS_SUPPORT
static void _GetVorbisAudInfo(UINT8 u1DecId, AUD_DEC_INFO_T * prAudInfo)
{
    AUD_DEC_STREAM_FROM_T eAudDecStreamFrom;
    AUD_FMT_T eAudDecFormat;
    UINT8 u1Channel, u1SampleRate;

#ifdef CC_MT5391_AUD_3_DECODER
    AUD_DEC_ID_VALIDATE_3(u1DecId);
#else
    AUD_DEC_ID_VALIDATE_2(u1DecId);
#endif

    if (prAudInfo != NULL)
    {
        // Get decoding format
        UNUSED(AUD_DRVGetDecodeType(u1DecId, &eAudDecStreamFrom, &eAudDecFormat));
        prAudInfo->e_aud_fmt = eAudDecFormat;
        UNUSED(eAudDecStreamFrom);

        if (u1DecId == AUD_DEC_MAIN)
        {
            u1SampleRate = dReadDspCommDram(ADDR_D2RC_VORBIS_SAMPLING_RATE)>>8;
            prAudInfo->ui4_data_rate = dReadDspCommDram(ADDR_D2RC_VORBIS_BIT_RATE);
            u1Channel = dReadDspCommDram(ADDR_D2RC_VORBIS_INPUT_CH_CFG)>>8;
        }
#ifdef CC_MT5391_AUD_3_DECODER
        else if (u1DecId == AUD_DEC_AUX)
#else
        else
#endif
        {
            u1SampleRate = dReadDspCommDram(ADDR_D2RC_VORBIS_SAMPLING_RATE_DEC2)>>8;
            prAudInfo->ui4_data_rate = dReadDspCommDram(ADDR_D2RC_VORBIS_BIT_RATE_DEC2);
            u1Channel = dReadDspCommDram(ADDR_D2RC_VORBIS_INPUT_CH_CFG_DEC2)>>8;
        }
#ifdef CC_MT5391_AUD_3_DECODER
        else
        {
            u1SampleRate = dReadDspCommDram(ADDR_D2RC_VORBIS_SAMPLING_RATE_DEC3)>>8;
            prAudInfo->ui4_data_rate = dReadDspCommDram(ADDR_D2RC_VORBIS_BIT_RATE_DEC3);
            u1Channel = dReadDspCommDram(ADDR_D2RC_VORBIS_INPUT_CH_CFG_DEC3)>>8;
        }
#endif

        switch (u1SampleRate)
        {
      	    case 0x13: // 6 KHz
      	    	 prAudInfo->ui4_sample_rate = 6000;
                 break;   
      	    case 0x04: // 8 KHz
      	    	 prAudInfo->ui4_sample_rate = 8000;
                 break;
            case 0x05: // 11 KHz
                 prAudInfo->ui4_sample_rate = 11000;
                 break;
            case 0x06: // 12 KHz
                 prAudInfo->ui4_sample_rate = 12000;
                 break;
            case 0x02: // 16 KHz
                 prAudInfo->ui4_sample_rate = 16000;
                 break;
            case 0x07: // 22 KHz
                 prAudInfo->ui4_sample_rate = 16000;
                 break;
            case 0x0C: // 24 KHz
                 prAudInfo->ui4_sample_rate = 24000;
                 break;
            case 0x03: // 32 KHz
                 prAudInfo->ui4_sample_rate = 32000;
                 break;
            case 0x08: // 44.1 KHz
                 prAudInfo->ui4_sample_rate = 44100;
                 break;
            case 0x0D: // 48 KHz
                 prAudInfo->ui4_sample_rate = 48000;
                 break;
            case 0x09: // 88.2KHz
                 prAudInfo->ui4_sample_rate = 88200;
                 break;
            case 0x0E: // 96 KHz
                 prAudInfo->ui4_sample_rate = 96000;
                 break;
            default: // Error sample rate
                 prAudInfo->ui4_sample_rate = 48000;
                 LOG(5, "Invalid sample rate (%d). \n", u1SampleRate);
                 break;
        }

        switch (u1Channel)
        {
            case 1: // 1/0
                prAudInfo->e_aud_type = AUD_TYPE_MONO;
                break;
            case 2: // 2/0
                prAudInfo->e_aud_type = AUD_TYPE_STEREO;
                break;
            case 3: // 3/0
                prAudInfo->e_aud_type = AUD_TYPE_3_0;
                break;
            case 6: // 2/2
                prAudInfo->e_aud_type = AUD_TYPE_4_0;
                break;
            case 7: // 3/2
                prAudInfo->e_aud_type = AUD_TYPE_5_0;
                break;
            case 27: // 2+LFE
                prAudInfo->e_aud_type = AUD_TYPE_5_1;
                break;
            default: //
                LOG(5, "%s,", "Error acmode");
                prAudInfo->e_aud_type = AUD_TYPE_OTHERS;
                break;
        }


        LOG(5, "prAudInfo->ui4_sample_rate = %d\n", prAudInfo->ui4_sample_rate);
        LOG(5, "prAudInfo->ui4_data_rate = %d\n", prAudInfo->ui4_data_rate);
        LOG(5, "prAudInfo->e_aud_type = %d\n", prAudInfo->e_aud_type);
        prAudInfo->ui1_bit_depth = 24;
    }
}
#endif

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------
BOOL _IsDualDecMode(void)
{
    return ((u2ReadShmUINT16(W_SYSTEM_CTRL) & 0x1) == SHM_SYSTEM_CTRL_DUAL_DEC_MODE);
    // tv + pcm_dec1, W_SYSTEM_CTRL= 0x3;
    // dtv +pcm_dec1 + A.D, W_SYSTEM_CTRL = 0xb;
}

void vDspSendSample(UINT8 u1DecId)
{
    _rDspStatus[AUD_DEC_MAIN].fgDspSInt = TRUE;
    _uDspSIntAddr = INT_RC2D_FLOW_CONTROL;

    if (u1DecId == AUD_DEC_MAIN)
    {
        _uDspSIntAddr = INT_RC2D_FLOW_CONTROL;
    }
#ifdef CC_MT5391_AUD_3_DECODER
    else if (u1DecId == AUD_DEC_THIRD)
    {
        _uDspSIntAddr = INT_RC2D_FLOW_CONTROL_DEC3;
    }
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
    else if (u1DecId == AUD_DEC_4TH)
    {
        _uDspSIntAddr = INT_RC2D_FLOW_CONTROL_DEC4;
    }
#endif
    else
    {
        _uDspSIntAddr = INT_RC2D_FLOW_CONTROL_DEC2;
    }

    // Always need to do AOUT_WAIT
    // System Master : wait PTS = STC
    // Audio Master  : wait Video trigger
    if (_rDspStatus[u1DecId].fgDspRealPlay) // || !fgWaitInitSync)
    {
        // No need initial AV-sync during playing period (RealPlay)
        _u4DspSIntSD = FLOW_CONTROL_SAMPLING_RATE;
    }
    else
    {
        _u4DspSIntSD = FLOW_CONTROL_SAMPLING_RATE+FLOW_CONTROL_AOUT_WAIT_ENABLE;
		_fgAudMMnotifySWDMX[u1DecId] = TRUE; 
    }
}

void vDspAOutEnable(UINT8 u1DecId)
{
    _rDspStatus[u1DecId].fgDspRealPlay = TRUE;
    _rDspStatus[u1DecId].fgDspStop = FALSE;
    _fgAoutEnable[u1DecId] = TRUE;
}


void vDspFlushDone(UINT8 u1DecId)
{
    _rDspStatus[u1DecId].fgDspStop = TRUE;
    _rDspStatus[u1DecId].fgDspRealPlay = FALSE;
    _rDspStatus[u1DecId].fgSendSample = FALSE;
    _rDspStatus[u1DecId].fgDspFlush = FALSE;
	_fgAudMMnotifySWDMX[u1DecId] = FALSE; 
    _fgAoutEnable[u1DecId] = FALSE;

    if (u1DecId == AUD_DEC_MAIN)
    {
        WriteDspCommDram(ADDR_D2RC_RISC_INFO_APUT_BANK_DEC1,0); //dsp will not clear this common dram when stop. so we clean by driver
        vWriteDspShmWORD(W_SPEED,0x0);
#ifdef DATA_DISC_WMA_SUPPORT
    //PLAYMGR_SetWmaErrorCode(dReadDspCommDram(ADDR_D2RC_WMA_ERROR_MODE)>>8);
    if ( ((_rDspVars[AUD_DEC_MAIN].bDspStrTyp == WMA_STREAM) || (_rDspVars[AUD_DEC_MAIN].bDspStrTyp == WMAPRO_STREAM))
          &&(_hWMAErrNotifyFunc != NULL))
    {
        //_hWMAErrNotifyFunc(dReadDspCommDram(ADDR_D2RC_WMA_ERROR_MODE)>>8);
#ifdef CC_53XX_SWDMX_V2
        _hWMAErrNotifyFunc(_prWMAErrInst, DSP_GetWmaErrorCode());  // megaa 20090828
#else
        _hWMAErrNotifyFunc(DSP_GetWmaErrorCode());
#endif
    }
#endif
    }
    else if (u1DecId == AUD_DEC_AUX)
    {
        vWriteDspShmWORD(W_SPEED_DEC2,0x0);
        WriteDspCommDram(ADDR_D2RC_RISC_INFO_APUT_BANK_DEC2,0); //dsp will not clear this common dram when stop. so we clean by driver
    }
	else if (u1DecId == AUD_DEC_THIRD)
	{	
	    WriteDspCommDram(ADDR_D2RC_RISC_INFO_APUT_BANK_DEC3,0); //dsp will not clear this common dram when stop. so we clean by driver
	}
}

BOOL DSP_GetAoutEnable(UINT8 u1DecId)
{
    return _fgAoutEnable[u1DecId];
}

BOOL fgDspRealPlay(UINT8 u1DecId)
{
    return _rDspStatus[u1DecId].fgDspRealPlay;
}

void vDspAdacFmt(UINT8 u1DecId, UINT32 u4ClkFmt)
{
    AUD_SetSampleFreq(u1DecId, (SAMPLE_FREQ_T)u4ClkFmt);
}

void vDspMemInit(UINT8* puWorkingBuffer)
{
    UINT32 u4DramAdr = PHYSICAL((UINT32)(puWorkingBuffer));
    UINT32 u4MemAddr;
    UINT32 u4MemSize;

    _u4DspICacheAddr[TYPE_ROM_ICACHE] =  ((DSP_AREA_IC_0_SA+u4DramAdr )>>8);   // Set rom as post cache
    _u4DspICacheAddr[TYPE_POST_ICACHE] = ((DSP_AREA_IC_1_SA+u4DramAdr )>>8);
    _u4DspICacheAddr[TYPE_CRI_ICACHE] = ((DSP_AREA_IC_2_SA+u4DramAdr )>>8);
    _u4DspICacheAddr[TYPE_COMM_ICACHE] = ((DSP_AREA_IC_3_SA+u4DramAdr )>>8);
    _u4DspICacheAddr[TYPE_DEC4_ICACHE] = ((DSP_AREA_IC_4_SA+u4DramAdr )>>8);
    _u4DspICacheAddr[TYPE_DEC1_ICACHE] = ((DSP_AREA_IC_5_SA+u4DramAdr )>>8);
    _u4DspICacheAddr[TYPE_DEC3_ICACHE] = ((DSP_AREA_IC_6_SA+u4DramAdr )>>8);
    _u4DspICacheAddr[TYPE_DEC2_ICACHE] = ((DSP_AREA_IC_7_SA+u4DramAdr )>>8);
    #ifdef DSP_SUPPORT_17BIT
    _u4DspICacheAddr[TYPE_DEC4_ICACHE2] = (_u4DspICacheAddr[TYPE_DEC4_ICACHE]+0x200);
    _u4DspICacheAddr[TYPE_DEC1_ICACHE2] = (_u4DspICacheAddr[TYPE_DEC1_ICACHE]+0x200);
    _u4DspICacheAddr[TYPE_DEC3_ICACHE2] = (_u4DspICacheAddr[TYPE_DEC3_ICACHE]+0x200);
    _u4DspICacheAddr[TYPE_DEC2_ICACHE2] = (_u4DspICacheAddr[TYPE_DEC2_ICACHE]+0x200);
    #endif

    /* DATA DRAM PAGE SETTING */
    _u4DspCmptBuf[TYPE_DEC1_DRAM_IDX] = (DSP_AREA_CMPT_0_SA+u4DramAdr);
    _u4DspCmptBuf[TYPE_DEC2_DRAM_IDX] = (DSP_AREA_CMPT_1_SA+u4DramAdr);
    _u4DspCmptBuf[TYPE_DEC3_DRAM_IDX] = (DSP_AREA_CMPT_2_SA+u4DramAdr);

    _u4DspCmptBuf[TYPE_COMM_DRAM_IDX] = (DSP_AREA_CMPT_3_SA+u4DramAdr);
    _u4DspCmptBuf[TYPE_DEC4_DRAM_IDX] = (DSP_AREA_CMPT_4_SA+u4DramAdr);
    _u4DspCmptBuf[TYPE_MIX_DRAM_IDX] = (DSP_MIXSNDBUF_SA+u4DramAdr);
    _u4DspCmptBuf[TYPE_RESERVED_6_IDX] = 0;
    _u4DspCmptBuf[TYPE_RESERVED_7_IDX] = 0;

#ifndef PATTERN_DRAM_TEST
    _u4DspDramBuf[TYPE_DEC1_DRAM_IDX] = (DSP_AREA_CMPT_0_SA+u4DramAdr);
    _u4DspDramBuf[TYPE_DEC2_DRAM_IDX] = (DSP_AREA_CMPT_1_SA+u4DramAdr);
    _u4DspDramBuf[TYPE_DEC3_DRAM_IDX] = (DSP_AREA_CMPT_2_SA+u4DramAdr);
    _u4DspDramBuf[TYPE_COMM_DRAM_IDX] = (DSP_AREA_DRAM_3_SA+u4DramAdr);
    _u4DspDramBuf[TYPE_DEC4_DRAM_IDX] = (DSP_AREA_CMPT_4_SA+u4DramAdr);
    _u4DspDramBuf[TYPE_MIX_DRAM_IDX] = (DSP_MIXSNDBUF_SA+u4DramAdr);
    _u4DspDramBuf[TYPE_RESERVED_6_IDX] = 0;
    _u4DspDramBuf[TYPE_RESERVED_7_IDX] = 0;
#else
    _u4DspDramBuf[TYPE_DEC1_DRAM_IDX] = (DSP_AREA_CMPT_0_SA+0x10000+u4DramAdr);
    _u4DspDramBuf[TYPE_DEC2_DRAM_IDX] = (DSP_AREA_CMPT_1_SA+0x10000+u4DramAdr);
    _u4DspDramBuf[TYPE_DEC3_DRAM_IDX] = (DSP_AREA_CMPT_2_SA+0x10000+u4DramAdr);
    _u4DspDramBuf[TYPE_DEC4_DRAM_IDX] = (DSP_AREA_CMPT_3_SA+0x10000+u4DramAdr);
    _u4DspDramBuf[TYPE_COMM_DRAM_IDX] = (DSP_AREA_CMPT_4_SA+0x10000+u4DramAdr);
    _u4DspDramBuf[TYPE_RESERVED_6_IDX] = 0;
    _u4DspDramBuf[TYPE_RESERVED_7_IDX] = 0;
#endif
    _u4DspBuf = _u4DspDramBuf[TYPE_COMM_DRAM_IDX];
    _u4ShareMem = (UINT32)puWorkingBuffer;
    _u4AFIFO[AUD_DEC_MAIN][0]=(DSP_AFIFO_SA+u4DramAdr);
    _u4AFIFO[AUD_DEC_MAIN][1]=(DSP_AFIFO_SA+DSP_AFIFO_SZ+u4DramAdr);
    _u4AFIFO[AUD_DEC_AUX][0]=(DSP_AFIFO_SA_DEC2+u4DramAdr);
    _u4AFIFO[AUD_DEC_AUX][1]=(DSP_AFIFO_SA_DEC2+DSP_AFIFO_SZ_DEC2+u4DramAdr);
    _u4AFIFO[AUD_DEC_THIRD][0]=(DSP_AFIFO_SA_DEC3+u4DramAdr);
    _u4AFIFO[AUD_DEC_THIRD][1]=(DSP_AFIFO_SA_DEC3+DSP_AFIFO_SZ_DEC3+u4DramAdr);
    _u4AFIFO[AUD_DEC_4TH][0]=(DSP_AFIFO_SA_DEC4+u4DramAdr);
    _u4AFIFO[AUD_DEC_4TH][1]=(DSP_AFIFO_SA_DEC4+DSP_AFIFO_SZ_DEC4+u4DramAdr);
    _u4MixSndBuf= (DSP_MIXSNDBUF_SA+u4DramAdr);
#ifdef CC_AUD_HDMI_PARSER_2_0_SUPPORT
    _u4AFIFOHdmiParser[0]=(DSP_AFIFO_HDMI_PARSER_SA+u4DramAdr);
    _u4AFIFOHdmiParser[1]=(DSP_AFIFO_HDMI_PARSER_SA+DSP_AFIFO_HDMI_PARSER_SZ+u4DramAdr);
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
    _u4SBCEncBuf[0] = (_u4DspCmptBuf[TYPE_COMM_DRAM_IDX] + AUD_UPLOAD_BUFFER_BASE);
    _u4SBCEncBuf[1] = (_u4DspCmptBuf[TYPE_COMM_DRAM_IDX] + AUD_UPLOAD_BUFFER_BASE + AUD_UPLOAD_BUFFER_SIZE);
#endif

	// Check if memory usage over allocate range
    VERIFY(fgDspGetDspMem(&u4MemAddr, &u4MemSize) == TRUE);
	VERIFY(PHYSICAL(u4MemAddr + u4MemSize) >= PHYSICAL(_u4MixSndBuf));

    // Update decoder status
    {
        AUD_DECODER_T * parDecStatus[AUD_DEC_NUM];
        INT32  i;

        for(i = 0; i < AUD_DEC_NUM; i++)
        {
            parDecStatus[i] = AUD_DrvGetDecodeState(i);
        }

        parDecStatus[AUD_DEC_MAIN]->u4FifoStart = _u4AFIFO[AUD_DEC_MAIN][0];
        parDecStatus[AUD_DEC_MAIN]->u4FifoEnd   = _u4AFIFO[AUD_DEC_MAIN][1];
        parDecStatus[AUD_DEC_MAIN]->u4FifoSz    = _u4AFIFO[AUD_DEC_MAIN][1] - _u4AFIFO[AUD_DEC_MAIN][0];

        parDecStatus[AUD_DEC_AUX]->u4FifoStart = _u4AFIFO[AUD_DEC_AUX][0];
        parDecStatus[AUD_DEC_AUX]->u4FifoStart = _u4AFIFO[AUD_DEC_AUX][1];
        parDecStatus[AUD_DEC_AUX]->u4FifoSz    = _u4AFIFO[AUD_DEC_AUX][1] - _u4AFIFO[AUD_DEC_AUX][0];

        #ifdef CC_MT5391_AUD_3_DECODER

        parDecStatus[AUD_DEC_THIRD]->u4FifoStart = _u4AFIFO[AUD_DEC_MAIN][0];
        parDecStatus[AUD_DEC_THIRD]->u4FifoEnd   = _u4AFIFO[AUD_DEC_MAIN][1];
        parDecStatus[AUD_DEC_THIRD]->u4FifoSz    = _u4AFIFO[AUD_DEC_MAIN][1] - _u4AFIFO[AUD_DEC_MAIN][0];

      #ifdef MP3ENC_SUPPORT
        ///Set RP,WP pointer address
        _tPbsBlk[0].dwRdPtrAdr = ADDR_RC2D_ENCODER_OUTBUF_READ_PNT_DEC3;
        _tPbsBlk[0].dwWrPtrAdr = ADDR_RC2D_ENCODER_OUTBUF_WRITE_PNT_DEC3;

        ///[Joel]Map DramMapCommon.inc DRAM_UPLOAD_DATA1 range
        ///Putbit buffer in common dram page 3 : 37400h~3CB80h, SZ=05780*4=87.5KBytes
        _tPbsBlk[0].wSblk = (_u4DspCmptBuf[TYPE_COMM_DRAM_IDX] + AUD_UPLOAD_BUFFER_BASE)>>8;
        _tPbsBlk[0].wEblk = _tPbsBlk[0].wSblk + MP3_OUT_BUF_SZ;
        ///_tPbsBlk[0].wEblk = (_u4DspCmptBuf[TYPE_COMM_DRAM_IDX] + (0x3F480)*4)>>8;
        _tPbsBlk[1].wSblk = _tPbsBlk[0].wSblk;
        _tPbsBlk[1].wEblk = _tPbsBlk[0].wEblk;
      #endif
        #endif
    }
    // Init dsp micro-code memory protection
    #ifndef CC_FPGA
    UNUSED(_fgInitDspMidProtect());
    #endif
}

void vFlash2DramDma(UINT32 u4Dest, UINT32 u4Src , UINT32 u4Len)
{
  // dwDest: physical DRAM address
  // dwSrc: physical flash Memory address
  // NOTE: should be modified in MT5351 condition

  UINT8* puDest=(UINT8*)VIRTUAL(u4Dest);
  UINT8* puSrc=(UINT8*)VIRTUAL(u4Src);
  UINT32 u4Index=0;

  #ifndef CC_FPGA
  VERIFY(x_sema_lock(_hSemaLoadCode, X_SEMA_OPTION_WAIT) == OSR_OK);
  UNUSED(_fgDisableDspMidProtect());
  #endif

  for(u4Index=0;u4Index<u4Len;u4Index++)
  {
    puDest[u4Index]=puSrc[u4Index];
  }

  DSP_FlushInvalidateDCacheFree(u4Dest, u4Len);

  #ifndef CC_FPGA
  UNUSED(_fgEnableDspMidProtect());
  VERIFY(x_sema_unlock(_hSemaLoadCode) == OSR_OK);
  #endif
}

void vFlash2DramDmaShift(UINT32 u4Dest, UINT32 u4Src , UINT32 u4Len)
{
  // dwDest: physical DRAM address
  // dwSrc: physical flash Memory address
  // NOTE: should be modified in MT5351 condition

  UINT8* puDest=(UINT8*)VIRTUAL(u4Dest);
  UINT8* puSrc=(UINT8*)VIRTUAL(u4Src);
  UINT32 u4Index=0;

  #ifndef CC_FPGA
  VERIFY(x_sema_lock(_hSemaLoadCode, X_SEMA_OPTION_WAIT) == OSR_OK);
  UNUSED(_fgDisableDspMidProtect());
  #endif

  for(u4Index=0;u4Index<u4Len;u4Index+=4)
  {
    puDest[u4Index] = puSrc[u4Index+3];
    puDest[u4Index+1] = puSrc[u4Index];
    puDest[u4Index+2] = puSrc[u4Index+1];
    puDest[u4Index+3] = puSrc[u4Index+2];
  }

  DSP_FlushInvalidateDCacheFree(u4Dest, u4Len);

  #ifndef CC_FPGA
  UNUSED(_fgEnableDspMidProtect());
  VERIFY(x_sema_unlock(_hSemaLoadCode) == OSR_OK);
  #endif
}

//lint -e{774}
UINT32 u4GetDspImgAddr(void)
{
  static BOOL    fgAlignedImage = FALSE;
  UINT32 u4ImageAddr;
  UINT32 u4Unaligned;

  u4Unaligned = (UINT32)_pbD % DSP_IMAGE_ALIGN_IN_BYTE;

  // If original address is aligned, then do nothing, else do alignment
  // Note that this operation will corrupt the image header !
  if (u4Unaligned == 0)
  {
        fgAlignedImage = TRUE;
  }
  else if ((u4Unaligned != 0) && (!fgAlignedImage))
  {
      UINT8 * pu1TmpBuf;
      UINT8 * pu1ImagAlignAddr;
      UINT8 * pu1ImagUnAlignAddr;
      UINT32 u4ImageSize;

      VERIFY(DSP_IMAGE_HEADER_SIZE >= u4Unaligned);

      pu1ImagUnAlignAddr = (UINT8 *)_pbD + DSP_IMAGE_HEADER_SIZE;
      pu1ImagAlignAddr = pu1ImagUnAlignAddr - u4Unaligned;
      u4ImageSize = _DSP_GetBinarySize() - DSP_IMAGE_HEADER_SIZE;
      pu1TmpBuf = (UINT8 *)x_mem_alloc(u4ImageSize);

      VERIFY(pu1TmpBuf != NULL);

      // Copy to temp address
      x_memcpy((VOID *)VIRTUAL((UINT32)pu1TmpBuf), (const VOID *) pu1ImagUnAlignAddr, u4ImageSize);
      // Copy from temp to aligned address
      x_memcpy((VOID *)VIRTUAL((UINT32)pu1ImagAlignAddr), (const VOID *)pu1TmpBuf, u4ImageSize);

      x_mem_free(pu1TmpBuf);

      fgAlignedImage = TRUE;
  }

  u4ImageAddr =  (UINT32)(((UINT8 *)_pbD + DSP_IMAGE_HEADER_SIZE) - u4Unaligned);

  return u4ImageAddr; // point to header of flash file
}

UINT32 u4GetFlashData(UINT32 u4Addr)
{ // read data from correspoding flash address
  // NOTE: should be modified in MT5351 codition
  return *((UINT32*)(u4Addr));
}

UINT32 u4GetDspFlashVersion(void)
{
    UINT8  *pu1FlashVer;
    UINT32 u4Ret;
    UINT32 u4Cnt;

    u4Ret = 0;
    pu1FlashVer = (UINT8 *)u4GetDspImgAddr();
    for(u4Cnt = 0 ; u4Cnt < 4 ; u4Cnt++)
    {
        u4Ret <<= 8;
        u4Ret |= *pu1FlashVer++;
    }

    return (u4Ret);
}

UINT32 DSP_InternalLogicalAddr(UINT32 u4Addr)
{
	return u4DspInternalLogicalAddress(u4Addr);
}

UINT32 DSP_PhysicalAddr(UINT32 u4Addr)
{
	return u4DspPhysicalAddress(u4Addr);
}

UINT32 DSP_VirtualAddr(UINT32 u4Addr)
{
	return u4DspVirtualAddress(u4Addr);
}

#ifdef MP3ENC_SUPPORT
UINT32 DSP_CommonDramRead(UINT16 u2Addr)
{
        return dReadDspCommDram(u2Addr);
}
#endif

/******************************************************************************
* Function      : u4GetMixSoundStartByteAddr
* Description   : get audio Mixsound buffer start address
* Parameter     :
* Return        : Mixsound buffer start address
******************************************************************************/
UINT32 u4GetMixSoundStartByteAddr(void)
{
  return _u4MixSndBuf;
}

/******************************************************************************
* Function      : u4GetMixSoundBufSize
* Description   : get audio Mixsound buffer Size
* Parameter     :
* Return        : Mixsound buffer size
******************************************************************************/
UINT32 u4GetMixSoundBufSize(void)
{
  return DSP_MIXSNDBUF_SZ;
}

UINT32 u4GetMixSoundStartByteAddr2(void)
{
  return _u4MixSndBuf + DSP_MIXSNDBUF_SZ;
}

UINT32 u4GetMixSoundBufSize2(void)
{
  return DSP_MIXSNDBUF_SZ2;
}

#ifdef ALSA_MIXSND_PATH
UINT32 u4GetMixSoundStartByteAddr3(void)
{
  return _u4MixSndBuf + DSP_MIXSNDBUF_SZ + DSP_MIXSNDBUF_SZ2 + DSP_MIXSNDBUF_TMP_SZ2;
}

UINT32 u4GetMixSoundBufSize3(void)
{
  return DSP_MIXSNDBUF_SZ3;
}
#endif

/******************************************************************************
* Function      : u2GetMixSoundStatus
* Description   : get audio Mixsound processing status
* Parameter     :
* Return        : status: 1: working, 0: idle
******************************************************************************/
UINT16 u2GetMixSoundStatus(void)
{
    DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_DATA, TYPE_COMM_DRAM_IDX);
    return (UINT16)wReadDspWORD (ADDR_RC2D_DRAM_MIXSOUND_STATUS);
}

BOOL fgGetDspCodeRegion(UINT32* pu4StartAddr, UINT32* pu4EndAddr)
{
    BOOL fgRet = TRUE;

    if( (pu4StartAddr != NULL) && (pu4EndAddr != NULL) )
    {
        *pu4StartAddr = _u4DspICacheAddr[TYPE_ROM_ICACHE] << 8;
        *pu4EndAddr = (_u4DspICacheAddr[TYPE_ROM_ICACHE] << 8)  + DSP_CODE_SZ;

        // Check if the address is valid
        if ((*pu4StartAddr == (UINT32)NULL) || (*pu4EndAddr == (UINT32)NULL) || (*pu4StartAddr >= *pu4EndAddr))
        {
            fgRet = FALSE;
        }
    }
    else
    {
        fgRet = FALSE;
    }

    return fgRet;
}

#ifndef CC_FPGA
static BOOL _fgEnableDspMidProtect(void)
{
    return MID_EnableRegionProtect(_u4MidRegion);
}

static BOOL _fgDisableDspMidProtect(void)
{
    return MID_DisableRegionProtect(_u4MidRegion);
}

static BOOL _fgInitDspMidProtect(void)
{
    UINT32 u4StartAddr, u4EndAddr;
    UINT32 u4LowAddr, u4HighAddr;

    VERIFY(x_sema_create(&_hSemaLoadCode, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);

    if (!fgGetDspCodeRegion(&u4StartAddr, &u4EndAddr))
    {
        return FALSE;
    }

    u4LowAddr = (((UINT32)u4StartAddr + MID_ADDR_ALIGNMENT) - 1) &
        ~(MID_ADDR_ALIGNMENT - 1);
    u4HighAddr = ((UINT32)u4EndAddr - 1) & ~(MID_ADDR_ALIGNMENT - 1);

    // Init MID
    if (!MID_Init())
    {
        return FALSE;
    }

    // Allocate a read-write region
    if (!MID_AllocateFreeRegion(FALSE, &_u4MidRegion))
    {
        return FALSE;
    }

    // Set micro-code to read-write
    if (!MID_SetRegion(_u4MidRegion, MID_AGENT_AUDIO, u4LowAddr, u4HighAddr))
    {
        return FALSE;
    }

    return TRUE;
}
#endif

static BOOL _fgDspCmdQInitialed = FALSE;

BOOL fgGetDspCmd(UINT32  pu4Cmd)
{
	if (!_fgDspCmdQInitialed)
    {
		_rDspCmdHistory.u4Index=0;
		_fgDspCmdQInitialed = TRUE;
	}

    if ((pu4Cmd&0xff)==ADSPTASK_MSG_COMMAND)
    {
        return FALSE; // Light: don't log interrupt event
    }
    VERIFY(_rDspStatus[AUD_DEC_MAIN].fgDspWakeUpOk == TRUE);
    LOG(9, "[DSP_TASK] GetDspCmd = 0x%x\n", pu4Cmd);
    /* for debug storing command history*/
    _rDspCmdHistory.u4Cmd[_rDspCmdHistory.u4Index]=pu4Cmd;
    _rDspCmdHistory.u4Index=(_rDspCmdHistory.u4Index +1 ) % MAX_DSP_CMD_HIS_NS;
    return TRUE;
}

void vLogFlowControl(UINT32 u4DspRIntData)
{
    if ((u4DspRIntData == D2RC_FLOW_CONTROL_AOUT_ENABLE) || (u4DspRIntData == D2RC_FLOW_CONTROL_AOUT_ENABLE_DEC2))
    {
        UCHAR ucDecId;
        //HAL_TIME_T rTime;
        //DMX_AUDIO_PES_T rAudPes;
        //STC_CLOCK rStc;

        if (u4DspRIntData == D2RC_FLOW_CONTROL_AOUT_ENABLE)
        {
            ucDecId = AUD_DEC_MAIN;
            //STC_GetSrc(STC_SRC_A1, &rStc);  // FIXME TODO
        }
        else
        {
            ucDecId = AUD_DEC_AUX;
            //STC_GetSrc(STC_SRC_A2, &rStc); // FIXME TODO
        }

        //HAL_GetTime(&rTime);

        // To suppress warning messages for release version
        UNUSED(ucDecId);
        //LOG(3, "Audio Start DecId(%d) Time(%d.%06d) PTS(0x%08x) STC(0x%08x)\n", ucDecId, rTime.u4Seconds, rTime.u4Micros);
        LOG(3, "[DSP_IRQ]Audio Start Dec(%d) \n", ucDecId);
    }
}

void vDspTvSysDetectedNotify(UINT8 u1DecId, TV_AUD_SYS_T eTvSys)
{
    UINT32         u4TvSys = 0;
    AudDecNfyFct   pfAudDecNfy = NULL;
    TV_AUD_SYS_T   eDetTvSys = eTvSys;

/*
TOP_ENERGY_45M                 .set DRAM_DSP_TO_UP_PARAMETER
TOP_ENERGY_55M                 .set TOP_ENERGY_45M+1
TOP_ENERGY_60M                 .set TOP_ENERGY_55M+1
TOP_ENERGY_65M                 .set TOP_ENERGY_60M+1
TOP_ENERGY_585M                .set TOP_ENERGY_65M+1
TOP_ENERGY_655M                .set TOP_ENERGY_585M+1
*/

    UINT32 u4Energy45M = 0;
    UINT32 u4Energy55M = 0;
    UINT32 u4Energy60M = 0;
    UINT32 u4Energy65M = 0;
    UINT32 u4Energy655M = 0;
    UINT32 u4TempEnergy = 0;

    UINT32 u4LogVar1 = 0;
    UINT32 u4LogVar2 = 0;
    UINT32 u4LogVar3 = 0;
    UINT32 u4LogVar4 = 0;


    UNUSED(u4Energy45M);
    UNUSED(u4Energy55M);
    UNUSED(u4Energy60M);
    UNUSED(u4Energy65M);
    UNUSED(u4Energy655M);
    UNUSED(u4TempEnergy);
    UNUSED(u4LogVar1);
    UNUSED(u4LogVar2);
    UNUSED(u4LogVar3);
    UNUSED(u4LogVar4);

    AUD_DEC_ID_VALIDATE_2(u1DecId);

    _eAudTvSystem = eTvSys;
    // Get notify function
    VERIFY(_AUD_GetNotifyFunc(&pfAudDecNfy) == TRUE);


     //if (u1DecId == AUD_DEC_AUX)
    if (_rDspStatus[AUD_DEC_AUX].fgDspPlay) //when do ATV scan, if the 2nd decoder is still played, then it must be detector
    {
        ReadDspDec2CacheDram(0x800,u4Energy45M);
        ReadDspDec2CacheDram(0x801,u4Energy55M);
        ReadDspDec2CacheDram(0x802,u4Energy60M);
        ReadDspDec2CacheDram(0x803,u4Energy65M);
	    ReadDspDec2CacheDram(0x805,u4Energy655M);

	    ReadDspDec2CacheDram(0x814,u4LogVar1);
        ReadDspDec2CacheDram(0x815,u4LogVar2);
        ReadDspDec2CacheDram(0x816,u4LogVar3);
        ReadDspDec2CacheDram(0x817,u4LogVar4);
	}
	else
	{
        ReadDspDecCacheDram(0x800,u4Energy45M);
        ReadDspDecCacheDram(0x801,u4Energy55M);
        ReadDspDecCacheDram(0x802,u4Energy60M);
        ReadDspDecCacheDram(0x803,u4Energy65M);
        ReadDspDecCacheDram(0x805,u4Energy655M);

        ReadDspDecCacheDram(0x814,u4LogVar1);
        ReadDspDecCacheDram(0x815,u4LogVar2);
        ReadDspDecCacheDram(0x816,u4LogVar3);
        ReadDspDecCacheDram(0x817,u4LogVar4);
	}
      //change the energy level
      gu4Energy45M = u4Energy45M;
	  gu4Energy55M = u4Energy55M;
	  gu4Energy60M = u4Energy60M;
	  gu4Energy65M = u4Energy65M;
	  //gu4Energy655M = u4Energy655M;
      //UNUSED(gu4Energy655M);
#ifdef USE_ATD_IF_DEMOD
      u4Energy65M=u4Energy65M*2;
      u4Energy655M=u4Energy655M*2;
      if (eDetTvSys == SV_PAL_I)
      {
           if (u4Energy60M < u4Energy65M)
           {
                eDetTvSys = SV_PAL_DK;
		  _eAudTvSystem = eDetTvSys;
		  eTvSys=eDetTvSys;
                LOG(5, "WOW!!!!!! ATD Filter problem, need from PAL_I  to PAL_DK  \n");
	     }
	     else
	     {
                LOG(5, "WOW!!!!!!Still hold in PAL_I \n");
	     }
      	}

#endif

    // Determine SV_PAL/ SV_SECAM_L/ SV_SECAM_L_PLUM
    if ((eTvSys == SV_PAL_DK) || (eTvSys == SV_SECAM_L) || (eTvSys == SV_SECAM_L_PLUM))
    {
        eDetTvSys = DSP_GetDetTvSrcMode(eTvSys);
    }

    switch (eDetTvSys)
    {
		case SV_MTS: // BTSC, MTS
			_rTvAudSys.u4TvSysMask = AUD_TV_SYS_MASK_M |
															AUD_TV_SYS_MASK_N;
			_rTvAudSys.u4AudSysMask = AUD_SYS_MASK_BTSC;
			u4TvSys = _rTvAudSys.u4TvSysMask;
			break;
		case SV_FM_FM:	 // Japan
			_rTvAudSys.u4TvSysMask = AUD_TV_SYS_MASK_M |
															AUD_TV_SYS_MASK_N;
			_rTvAudSys.u4AudSysMask = AUD_SYS_MASK_FM_EIA_J;
			u4TvSys = _rTvAudSys.u4TvSysMask;
			break;
		case SV_NTSC_M:	// Korea
			_rTvAudSys.u4TvSysMask = AUD_TV_SYS_MASK_M |
															AUD_TV_SYS_MASK_N;
			_rTvAudSys.u4AudSysMask = AUD_SYS_MASK_FM_A2;
			u4TvSys = _rTvAudSys.u4TvSysMask;
			break;
		case SV_A2_BG:
			_rTvAudSys.u4TvSysMask = AUD_TV_SYS_MASK_B |
								AUD_TV_SYS_MASK_G|
								AUD_TV_SYS_MASK_A2	;
			_rTvAudSys.u4AudSysMask = AUD_SYS_MASK_FM_A2;
			u4TvSys = _rTvAudSys.u4TvSysMask;
			break;
		case SV_A2_DK1:
			_rTvAudSys.u4TvSysMask = AUD_TV_SYS_MASK_D |
								AUD_TV_SYS_MASK_K |
								AUD_TV_SYS_MASK_A2	;
			_rTvAudSys.u4AudSysMask = AUD_SYS_MASK_FM_A2_DK1;
			u4TvSys = _rTvAudSys.u4TvSysMask;
			break;
		case SV_A2_DK2:
			_rTvAudSys.u4TvSysMask = AUD_TV_SYS_MASK_D |
								AUD_TV_SYS_MASK_K |
								AUD_TV_SYS_MASK_A2;
			_rTvAudSys.u4AudSysMask = AUD_SYS_MASK_FM_A2_DK2;
			u4TvSys = _rTvAudSys.u4TvSysMask;
			break;
		case SV_A2_DK3:
			_rTvAudSys.u4TvSysMask = AUD_TV_SYS_MASK_D |
								AUD_TV_SYS_MASK_K |
								AUD_TV_SYS_MASK_A2;
			_rTvAudSys.u4AudSysMask = AUD_SYS_MASK_FM_A2;
			u4TvSys = _rTvAudSys.u4TvSysMask;
			break;
		case SV_PAL_I:
			_rTvAudSys.u4TvSysMask = AUD_TV_SYS_MASK_I |
								AUD_TV_SYS_MASK_PAL;
			_rTvAudSys.u4AudSysMask = AUD_SYS_MASK_FM_MONO |
																AUD_SYS_MASK_NICAM;
			u4TvSys = _rTvAudSys.u4TvSysMask;
			break;
		case SV_PAL_BG:
			_rTvAudSys.u4TvSysMask = AUD_TV_SYS_MASK_B |
								AUD_TV_SYS_MASK_G |
								AUD_TV_SYS_MASK_PAL;
			_rTvAudSys.u4AudSysMask = AUD_SYS_MASK_FM_MONO |
																AUD_SYS_MASK_NICAM;
			u4TvSys = _rTvAudSys.u4TvSysMask;
			break;
		case SV_PAL_DK:
			_rTvAudSys.u4TvSysMask = AUD_TV_SYS_MASK_D |
								AUD_TV_SYS_MASK_K |
								AUD_TV_SYS_MASK_PAL;
			_rTvAudSys.u4AudSysMask = AUD_SYS_MASK_FM_MONO |
																AUD_SYS_MASK_NICAM;
			u4TvSys = _rTvAudSys.u4TvSysMask;
			break;
		case SV_SECAM_L:
			_rTvAudSys.u4TvSysMask = AUD_TV_SYS_MASK_L |
															AUD_TV_SYS_MASK_PAL;
			_rTvAudSys.u4AudSysMask = AUD_SYS_MASK_FM_MONO |
																AUD_SYS_MASK_NICAM;
			u4TvSys = _rTvAudSys.u4TvSysMask;
			break;
		case SV_SECAM_L_PLUM:
			_rTvAudSys.u4TvSysMask = AUD_TV_SYS_MASK_L_PRIME|
															AUD_TV_SYS_MASK_PAL;
			_rTvAudSys.u4AudSysMask = AUD_SYS_MASK_FM_MONO |
																AUD_SYS_MASK_NICAM;
			u4TvSys = _rTvAudSys.u4TvSysMask;
			break;
		case SV_SECAM_BG:
			_rTvAudSys.u4TvSysMask = AUD_TV_SYS_MASK_B|
								AUD_TV_SYS_MASK_G|
															AUD_TV_SYS_MASK_PAL;
			_rTvAudSys.u4AudSysMask = AUD_SYS_MASK_FM_MONO |
																AUD_SYS_MASK_NICAM;
			u4TvSys = _rTvAudSys.u4TvSysMask;
			break;
		default:
			_rTvAudSys.u4TvSysMask = AUD_TV_SYS_MASK_M;
			_rTvAudSys.u4AudSysMask = AUD_SYS_MASK_BTSC;
			u4TvSys = _rTvAudSys.u4TvSysMask;
			break;
    }

    // Release driver sync semaphore ..
    AUD_DetectionSyncSema(FALSE);

    if (pfAudDecNfy != NULL)
    {
        pfAudDecNfy((void *)AUD_NFY_STREAM, u1DecId, AUD_COND_TV_SYS_DETECTED,
                        _rTvAudSys.u4TvSysMask,_rTvAudSys.u4AudSysMask);
    }

	_u4AtvTvSys = 	u4TvSys;

	LOG(5, "TV sys = %s \n", _aszTvSysMode[eTvSys]);

	if (_rDspStatus[AUD_DEC_AUX].fgDspPlay) //when do ATV scan, if the 2nd decoder is still played, then it must be detector
	{
        LOG(5, "Detector at AUX decoder : \n");
        LOG(5, "Top_45M = %x\n", u4Energy45M);
        LOG(5, "Top_55M = %x\n", u4Energy55M);
        LOG(5, "Top_60M = %x\n", u4Energy60M);
        LOG(5, "Top_65M = %x\n", u4Energy65M);
        LOG(5, "Top_655M = %x\n",u4Energy655M);

        LOG(6, "LOG_VAR1 = %x\n",u4LogVar1);
        LOG(6, "LOG_VAR2 = %x\n",u4LogVar2);
        LOG(6, "LOG_VAR3 = %x\n",u4LogVar3);
        LOG(6, "LOG_VAR4 = %x\n",u4LogVar4);

	}
	else
	{
        LOG(5, "Detector at MAIN decoder : \n");
        LOG(5, "Top_45M = %x\n", u4Energy45M);
        LOG(5, "Top_55M = %x\n", u4Energy55M);
        LOG(5, "Top_60M = %x\n", u4Energy60M);
        LOG(5, "Top_65M = %x\n", u4Energy65M);
        LOG(5, "Top_655M = %x\n", u4Energy655M);

        //LOG(5, "Hdev_E_L = %x\n", u4ReadDspSram(0xB27));
        //LOG(5, "Hdev_E_C1 = %x\n", u4ReadDspSram(0xB28));
        //LOG(5, "Hdev_E_C2 = %x\n", u4ReadDspSram(0xB29));
        //LOG(5, "Hdev_E_R = %x\n", u4ReadDspSram(0xB2A));

        LOG(6, "LOG_VAR1 = %x\n",u4LogVar1);
        LOG(6, "LOG_VAR2 = %x\n",u4LogVar2);
        LOG(6, "LOG_VAR3 = %x\n",u4LogVar3);
        LOG(6, "LOG_VAR4 = %x\n",u4LogVar4);
	}

	// To suppress warning message in release mode
	UNUSED(_aszTvSysMode);

}

void vDspSetFMRadioNoiseThreshold(UINT32 u4Thre)
{
    _gu4FMNoiseLevelTHL = u4Thre;
}
void vDspSetFMRadioInBandThreshold(UINT32 u4Thre)
{
    _gu4FMInBandPwrTHL = u4Thre;
}
void vDspSetFMRadioRatioThreshold(INT32 i4Thre)
{
    _gi4FMRatioTHL = i4Thre;
}
UINT32 vDspGetFMRadioNoiseThreshold(void)
{
     return _gu4FMNoiseLevelTHL;
}
UINT32 vDspGetFMRadioInBandThreshold(void)
{
    return _gu4FMInBandPwrTHL;
}
INT32 vDspGetFMRadioRatioThreshold(void)
{
    return _gi4FMRatioTHL;
}

void vDspFMRadioDetectionNotify(AUD_FM_RADIO_DET_T u4Msg)
{
    UINT32 u4FmInbandPwr, u4FmNoisePwr, u4FmInbandPwrLvl = 0;
    AudDecNfyFct   pfAudDecNfy = NULL;
    AUD_FM_RADIO_DET_T eDetFM = u4Msg;

    // Get notify function
    VERIFY(_AUD_GetNotifyFunc(&pfAudDecNfy) == TRUE);

    ReadDspDecCacheDram(0x800, u4FmInbandPwr);
    ReadDspDecCacheDram(0x801, u4FmNoisePwr);

    // Calculate signal level
    u4FmInbandPwrLvl = (UINT32)((u4FmInbandPwr * 255) / (0x7FFF));
    if (u4FmInbandPwrLvl > 255)
    {
        u4FmInbandPwrLvl = 255;
    }
    // prevent divide zero exception
    if (u4FmInbandPwr == 0)
    {
        u4FmInbandPwr = 1;
    }

    eDetFM = AUD_FM_RADIO_NON_DETECTED;
    if ((u4FmNoisePwr < _gu4FMNoiseLevelTHL) && (u4FmInbandPwr > _gu4FMInBandPwrTHL) && ((int) (u4FmNoisePwr / u4FmInbandPwr) < _gi4FMRatioTHL))
        {
            eDetFM = AUD_FM_RADIO_DETECTED;
            LOG(2, "FM detected by Driver\n");
        }

    if (pfAudDecNfy != NULL)
    {
        pfAudDecNfy((void *)AUD_NFY_STREAM, AUD_DEC_MAIN, AUD_COND_FM_RADIO_DET_DONE,
                        eDetFM, u4FmInbandPwrLvl);
    }

    switch(eDetFM)
    {
        case AUD_FM_RADIO_NON_DETECTED:
           _fgFMRadioDetectionResult = 0;
           LOG(1, "   None of any FM channel detected!\n");
           LOG(1, "   FM radio in-band energy  = 0x%x \n", u4FmInbandPwr);
           LOG(1, "   FM radio output noise level  = 0x%x \n", u4FmNoisePwr);
           break;
        case AUD_FM_RADIO_DETECTED:
           _fgFMRadioDetectionResult = 1;
           LOG(1, "   FM channel detected !\n");
           LOG(1, "   FM radio in-band energy  = 0x%x \n", u4FmInbandPwr);
           LOG(1, "   FM radio output noise level  = 0x%x \n", u4FmNoisePwr);
           break;
        default: //exception
           break;
    }
    UNUSED(u4FmInbandPwr);
    UNUSED(u4FmNoisePwr);
}

void vDspHdevModeChangeNotify(UINT32 u4Msg)
{
    UINT32 u4DramValue;
    ReadDspDec2CacheDram(7,u4DramValue);
    LOG(7, "   hdev_E_L  = 0x%x \n", u4DramValue);
    ReadDspDec2CacheDram(6,u4DramValue);
    LOG(7, "   hdev_E_C  = 0x%x \n", u4DramValue);
    ReadDspDec2CacheDram(8,u4DramValue);
    LOG(7, "   hdev_E_R  = 0x%x \n", u4DramValue);
    ReadDspDec2CacheDram(9,u4DramValue);
    LOG(7, "   hdev_a2_result  = 0x%x \n", u4DramValue);
    LOG(7, "Note that minimum level of hdev_E_L and hdev_E_R is 0x600!\n");
    switch(u4Msg)
    {
        case HDEV_MODE_OFF:
           LOG(7, "HDEV_MODE auto-switch : ON -> OFF !\n");
           break;
        case HDEV_MODE_ON:
           LOG(7, "HDEV_MODE auto-switch : OFF -> ON !\n");
           break;
        default: //exception
           break;
    }

    UNUSED(u4DramValue);
}

void vDspTvSysChangeNotify(TV_AUD_SYS_T eTvSys)
{
    UINT32 u4DramValue;

    ReadDspDec2CacheDram(0,u4DramValue);
    LOG(7, "   5.74M = 0x%x \n", u4DramValue);
    ReadDspDec2CacheDram(1,u4DramValue);
    LOG(7, "   5.85M = 0x%x \n", u4DramValue);
    ReadDspDec2CacheDram(2,u4DramValue);
    LOG(7, "   6.25M = 0x%x\n", u4DramValue);
    ReadDspDec2CacheDram(3,u4DramValue);
    LOG(7, "   6.74M = 0x%x \n", u4DramValue);
    ReadDspDec2CacheDram(4,u4DramValue);
    LOG(7, "   7.00M = 0x%x \n", u4DramValue);
    ReadDspDec2CacheDram(5,u4DramValue);
    LOG(7, "   main  = 0x%x \n", u4DramValue);

    ReadDspDec2CacheDram(7,u4DramValue);
    LOG(7, "   hdev_E_L  = 0x%x \n", u4DramValue);
    ReadDspDec2CacheDram(6,u4DramValue);
    LOG(7, "   hdev_E_C  = 0x%x \n", u4DramValue);
    ReadDspDec2CacheDram(8,u4DramValue);
    LOG(7, "   hdev_E_R  = 0x%x \n", u4DramValue);
    ReadDspDec2CacheDram(9,u4DramValue);
    LOG(7, "   hdev_a2_result  = 0x%x \n", u4DramValue);

    _guAudDspTrigAtvFmtchg = 0;

    switch (eTvSys)
    {
    case SV_A2_BG:
    case SV_A2_DK1:
    case SV_A2_DK2:
    case SV_A2_DK3:
        DSP_SetTvSrcMode(eTvSys);
        if (_guAudDemodTypeChange)
        {
            //AUD_SetDecType(AUD_DEC_AUX, AUD_STREAM_FROM_ANALOG_TUNER, AUD_FMT_A2);
            _eAtvFmtChgDecFmt = AUD_FMT_A2;
            _guAudDspTrigAtvFmtchg= 1;
        }
    	break;
    case SV_PAL_BG:
    case SV_PAL_DK:
        DSP_SetTvSrcMode(eTvSys);
        if (_guAudDemodTypeChange)
        {
            //AUD_SetDecType(AUD_DEC_AUX, AUD_STREAM_FROM_ANALOG_TUNER, AUD_FMT_PAL);
            _eAtvFmtChgDecFmt = AUD_FMT_PAL;
            _guAudDspTrigAtvFmtchg = 1;
        }
     	break;
    case SV_PAL_I:
    case SV_SECAM_L:
    case SV_SECAM_BG:
    case SV_SECAM_L_PLUM:
    case SV_NONE_DETECTED:
    case SV_MTS:
    case SV_FM_FM:
    case SV_NTSC_M:
    case SV_SECAM_DK:
       // no these cases
    	break;
    default :
    	break;
    }

    _guAudDemodTypeChange = 0;
    vDspTvSysDetectedNotify(AUD_DEC_AUX, eTvSys);
    UNUSED(u4DramValue);

}

void DSP_SetEuroCanalPlusMode(BOOL fgEnable)
{
    _fgEuroCanalPlusMode = fgEnable;
}

BOOL DSP_IsEuroCanalPlusMode(void)
{
    return _fgEuroCanalPlusMode;
}

void vDspFlowControlNotify(UINT32 u4DspRIntData)
{
    AUD_DEC_INFO_T rAudDecInfo;
    UINT32 u4Tmp = 0;
    AudDecNfyFct   pfAudDecNfy = NULL;
    UINT8          u1DecId = AUD_DEC_MAIN;

    // Get notify function
    VERIFY(_AUD_GetNotifyFunc(&pfAudDecNfy) == TRUE);

    switch (u4DspRIntData)
    {
    case D2RC_FLOW_CONTROL_SAMPLING_RATE:
        break;
    case D2RC_FLOW_CONTROL_AOUT_ENABLE:

#ifdef CC_ENABLE_AV_SYNC
             if (_fgwaitForAoutReinit==TRUE || _fgwaitForPIPAoutReinit==TRUE)
             {
                  AUD_DrvCallback(AUD_DEC_MAIN, AUD_DSP_CB_UNMUTE);
                  //AUD_DspDecPlayMuteEnable(AUD_DEC_MAIN, FALSE);
                  _fgwaitForAoutReinit=FALSE;
                  _fgSetChannelDelay = FALSE;
                  _fgwaitForPIPAoutReinit=FALSE;
                  LOG(7,"[SRM_AVSYNC] ########### For AV sync , UN-MUTE Audio !!!! \n");
              }
             else
             {
                  LOG(7,"[SRM_AVSYNC] D2RC_FLOW_CONTROL_AOUT_ENABLE, without UNMUTE \n");
              }
#endif
    
        ADAC_SpeakerPopEnable(TRUE);
        u1DecId = AUD_DEC_MAIN;
        if (_rDspVars[AUD_DEC_MAIN].bDspStrTyp == AC3_STREAM)
        {
            if (pfAudDecNfy != NULL)
            {
                rAudDecInfo.e_aud_fmt = AUD_FMT_AC3;
                rAudDecInfo.ui1_bit_depth = 24;
                _GetAudInfo(AUD_DEC_MAIN, &rAudDecInfo);

                pfAudDecNfy((void *)AUD_NFY_STREAM, u1DecId, AUD_COND_AUD_INFO_CHG,
                    (UINT32)&rAudDecInfo, u4Tmp);
        	    LOG(5, "[DSP_TASK]AC3 Dec0 Aout Enable Notify\n");
            }
        }
        else if ((_rDspVars[AUD_DEC_MAIN].bDspStrTyp == PCM_STREAM)  ||(_rDspVars[AUD_DEC_MAIN].bDspStrTyp == LPCM_STREAM) )
        {
            if (pfAudDecNfy != NULL)
            {
                if (_IsDualDecMode() || (_fgEuroCanalPlusMode))
                // CANAL+ / CANAL+ back to ATV mode
                {

                    rAudDecInfo.e_aud_fmt = (AUD_FMT_T)13; // FIXME;
                    rAudDecInfo.e_aud_type = DSP_GetAudChlInfo(AUD_DEC_AUX);
                    rAudDecInfo.ui4_sample_rate = 44;
                    rAudDecInfo.ui1_bit_depth = 24;
#ifndef CC_MT5391_AUD_3_DECODER
                    pfAudDecNfy((void *)AUD_NFY_STREAM, AUD_DEC_AUX, AUD_COND_AUD_INFO_CHG,
                        (UINT32)&rAudDecInfo, u4Tmp);
                    LOG(5, "[DSP_TASK]CANAL+ Dec0 Aout Enable Notify\n");
#else
                    //Under 3 decoder architecture, if the PCM input is from Dec2(ATV/DTV),
                    //then we notify "Dec1 Aout Enable" with the information of decoder2
                    if ((_rDspVars[AUD_DEC_AUX].bDspStrTyp == PAL_STREAM) ||
                        (_rDspVars[AUD_DEC_AUX].bDspStrTyp == A2_STREAM) ||
                        (_rDspVars[AUD_DEC_AUX].bDspStrTyp == NTSC_STREAM))
                    {
                        rAudDecInfo.ui4_sample_rate = 32;
                        if (_fgEuroCanalPlusMode)
                        {
                            rAudDecInfo.e_aud_fmt = AUD_FMT_EU_CANAL_PLUS;
                        }
						if (rAudDecInfo.e_aud_type == AUD_TYPE_UNKNOWN)
						{
							rAudDecInfo.e_aud_type = AUD_TYPE_MONO;
						}
                        pfAudDecNfy((void *)AUD_NFY_STREAM, AUD_DEC_MAIN, AUD_COND_AUD_INFO_CHG,
                        (UINT32)&rAudDecInfo, u4Tmp);
                        if (_fgEuroCanalPlusMode)
                        {
                            LOG(5, "[DSP_TASK]CANAL+ Dec0 Aout Enable Notify\n");
                        }
                        else
                        {
                        	LOG(5, "[DSP_TASK]ATV Source Dec0 Aout Enable Notify\n");
                    	}
                    }
                    else if (_rDspVars[AUD_DEC_AUX].bDspStrTyp == MPEG12_STREAM)
                    {
                        UNUSED(_GetMpegAudInfo(AUD_DEC_AUX, &rAudDecInfo));
                        pfAudDecNfy((void *)AUD_NFY_STREAM, AUD_DEC_MAIN, AUD_COND_AUD_INFO_CHG,
                        (UINT32)&rAudDecInfo, u4Tmp);
                        LOG(5, "[DSP_TASK]DTV Source Dec0 (MP2 Dec1) Aout Enable Notify\n");
                    }
                    else if (_rDspVars[AUD_DEC_AUX].bDspStrTyp == AC3_STREAM)
                    {
                        rAudDecInfo.e_aud_fmt = AUD_FMT_AC3;
                        rAudDecInfo.ui1_bit_depth = 24;
                        _GetAudInfo(AUD_DEC_AUX, &rAudDecInfo);
                       pfAudDecNfy((void *)AUD_NFY_STREAM, AUD_DEC_MAIN, AUD_COND_AUD_INFO_CHG,
                       (UINT32)&rAudDecInfo, u4Tmp);
                       LOG(5, "[DSP_TASK]DTV Source Dec0 (AC3 Dec1) Aout Enable Notify\n");
                    }
                    else if (_rDspVars[AUD_DEC_AUX].bDspStrTyp == AAC_STREAM)
                    {
                        _GetAacAudInfo(AUD_DEC_AUX, &rAudDecInfo);
                        pfAudDecNfy((void *)AUD_NFY_STREAM, AUD_DEC_MAIN, AUD_COND_AUD_INFO_CHG,
                        (UINT32)&rAudDecInfo, u4Tmp);
                        LOG(5, "[DSP_TASK]DTV Source Dec0 (AAC Dec1) Aout Enable Notify\n");
                    }
                    else if (_rDspVars[AUD_DEC_AUX].bDspStrTyp == DRA_STREAM)
                    {
                        _GetDraAudInfo(AUD_DEC_AUX, &rAudDecInfo);
                        pfAudDecNfy((void *)AUD_NFY_STREAM, AUD_DEC_MAIN, AUD_COND_AUD_INFO_CHG,
                        (UINT32)&rAudDecInfo, u4Tmp);
                        LOG(5, "[DSP_TASK]DTV Source Dec0 (DRA Dec1) Aout Enable Notify\n");
                    }
                    else
                    {
                        LOG(5, "[DSP_TASK]No matched decoder type for TV source Dec1 Aout Enable Notify\n");
                    }
#endif
                }
                else
                {
                    rAudDecInfo.e_aud_fmt = AUD_FMT_PCM;
                    rAudDecInfo.e_aud_type = AUD_TYPE_STEREO;
                    rAudDecInfo.ui4_sample_rate = 44;
                    rAudDecInfo.ui1_bit_depth = 24;
                    pfAudDecNfy((void *)AUD_NFY_STREAM, u1DecId, AUD_COND_AUD_INFO_CHG,
                        (UINT32)&rAudDecInfo, u4Tmp);
                    LOG(5, "[DSP_TASK]PCM Dec0 Aout Enable Notify\n");
                }
            }
        }
        else if ((_rDspVars[AUD_DEC_MAIN].bDspStrTyp == MPEG12_STREAM) || (_rDspVars[AUD_DEC_MAIN].bDspStrTyp == MPEG3_STREAM))
        {
            if (pfAudDecNfy != NULL)
            {
                UNUSED(_GetMpegAudInfo(AUD_DEC_MAIN, &rAudDecInfo));
                pfAudDecNfy((void *)AUD_NFY_STREAM, u1DecId, AUD_COND_AUD_INFO_CHG,
                    (UINT32)&rAudDecInfo, u4Tmp);
                LOG(5, "[DSP_TASK]MPG Dec0 Aout Enable Notify\n");
            }
        }

        else if (_rDspVars[AUD_DEC_MAIN].bDspStrTyp == AAC_STREAM)
        {
        	if (pfAudDecNfy != NULL)
        	{
        		_GetAacAudInfo(AUD_DEC_MAIN, &rAudDecInfo);
        		pfAudDecNfy((void *)AUD_NFY_STREAM, u1DecId, AUD_COND_AUD_INFO_CHG,
        			(UINT32)&rAudDecInfo, u4Tmp);
        		LOG(5, "[DSP_TASK]AAC Dec0 Aout Enable Notify\n");
        	}
        }
#ifdef DATA_DISC_WMA_SUPPORT
        else if (_rDspVars[AUD_DEC_MAIN].bDspStrTyp == WMA_STREAM)
        {
        	if (pfAudDecNfy != NULL)
        	{
        		rAudDecInfo.e_aud_fmt = AUD_FMT_WMA;
        		rAudDecInfo.ui1_bit_depth = 24;
        		//_GetWmaAudInfo(AUD_DEC_MAIN, &rAudDecInfo);   koro: to be done...
                rAudDecInfo.e_aud_type = AUD_TYPE_STEREO;
                rAudDecInfo.ui4_sample_rate = 44;
                pfAudDecNfy((void *)AUD_NFY_STREAM, u1DecId, AUD_COND_AUD_INFO_CHG,
        			(UINT32)&rAudDecInfo, u4Tmp);
        		LOG(5, "[DSP_TASK]WMA Dec0 Aout Enable Notify\n");
        	}
        }
#endif
        else if ((_rDspVars[AUD_DEC_MAIN].bDspStrTyp == PAL_STREAM) || (_rDspVars[AUD_DEC_MAIN].bDspStrTyp == A2_STREAM))
        {
            if (pfAudDecNfy != NULL)
            {
                rAudDecInfo.e_aud_fmt = (AUD_FMT_T)13; // FIXME!! Hardcode here
                rAudDecInfo.e_aud_type = AUD_TYPE_MONO;
                rAudDecInfo.ui4_sample_rate = 48;
                rAudDecInfo.ui1_bit_depth = 24;
                pfAudDecNfy((void *)AUD_NFY_STREAM, u1DecId, AUD_COND_AUD_INFO_CHG,
                    (UINT32)&rAudDecInfo, u4Tmp);
                LOG(5, "[DSP_TASK]PAL/A2 Dec0 Aout Enable Notify\n");
            }

            if (_aeAudType[AUD_DEC_MAIN] == AUD_TYPE_UNKNOWN)
            {
                _aeAudType[AUD_DEC_MAIN] = AUD_TYPE_MONO;
            }
        }
        else if (_rDspVars[AUD_DEC_MAIN].bDspStrTyp == SBC_STREAM)
        {
            if (pfAudDecNfy != NULL)
            {
                _GetSbcAudInfo(AUD_DEC_MAIN, &rAudDecInfo);

                pfAudDecNfy((void *)AUD_NFY_STREAM, u1DecId, AUD_COND_AUD_INFO_CHG,
                    (UINT32)&rAudDecInfo, u4Tmp);
        	    LOG(5, "[DSP_TASK]SBC Dec0 Aout Enable Notify\n");
            }
        }
        else if (_rDspVars[AUD_DEC_MAIN].bDspStrTyp == DRA_STREAM)
       	{
            if (pfAudDecNfy != NULL)
        	{
        		_GetDraAudInfo(AUD_DEC_MAIN, &rAudDecInfo);
        		pfAudDecNfy((void *)AUD_NFY_STREAM, u1DecId, AUD_COND_AUD_INFO_CHG, (UINT32)&rAudDecInfo, u4Tmp);
            }

       		LOG(5, "[DSP_TASK]DRA Dec0 Aout Enable Notify\n");
       	}
        else if (_rDspVars[AUD_DEC_MAIN].bDspStrTyp == DTSDEC_STREAM)
       	{
       	    if (pfAudDecNfy != NULL)
        	{
        		_GetDTSAudInfo(AUD_DEC_MAIN, &rAudDecInfo);
        		pfAudDecNfy((void *)AUD_NFY_STREAM, u1DecId, AUD_COND_AUD_INFO_CHG, (UINT32)&rAudDecInfo, u4Tmp);
            }
       		LOG(5, "[DSP_TASK]DTS Dec0 Aout Enable Notify\n");
       	}

       	else if (_rDspVars[AUD_DEC_MAIN].bDspStrTyp == COOK_STREAM)
       	{
            if (pfAudDecNfy != NULL)
        	{
        		_GetDraAudInfo(AUD_DEC_MAIN, &rAudDecInfo);
        		pfAudDecNfy((void *)AUD_NFY_STREAM, u1DecId, AUD_COND_AUD_INFO_CHG, (UINT32)&rAudDecInfo, u4Tmp);
            }
       		LOG(5, "[DSP_TASK]COOK Dec0 Aout Enable Notify\n");
       	}
        else if (_rDspVars[AUD_DEC_MAIN].bDspStrTyp == G729DEC_STREAM)
        {
            if (pfAudDecNfy != NULL)
            {
                    rAudDecInfo.e_aud_fmt = AUD_FMT_G729DEC;
                    rAudDecInfo.e_aud_type = AUD_TYPE_MONO;
                    rAudDecInfo.ui4_sample_rate = 8;
                    rAudDecInfo.ui1_bit_depth = 24;
                    pfAudDecNfy((void *)AUD_NFY_STREAM, u1DecId, AUD_COND_AUD_INFO_CHG,
                    (UINT32)&rAudDecInfo, u4Tmp);
        	      LOG(5, "[DSP_TASK]G729 Dec0 Aout Enable Notify\n");
            }
        }
        else if (_rDspVars[AUD_DEC_MAIN].bDspStrTyp == WMAPRO_STREAM)
        {
        	if (pfAudDecNfy != NULL)
        	{
                    rAudDecInfo.e_aud_fmt = AUD_FMT_WMAPRO;
                    rAudDecInfo.e_aud_type = AUD_TYPE_STEREO;   //WMAPROCHECK
                    rAudDecInfo.ui4_sample_rate = 44;
                    rAudDecInfo.ui1_bit_depth = 24;
                    pfAudDecNfy((void *)AUD_NFY_STREAM, u1DecId, AUD_COND_AUD_INFO_CHG,
                    (UINT32)&rAudDecInfo, u4Tmp);
        	      LOG(5, "[DSP_TASK]WMA PRO Dec0 Aout Enable Notify\n");
        	}
        }
#if 1///def CC_VORBIS_SUPPORT
        else if (_rDspVars[AUD_DEC_MAIN].bDspStrTyp == VORBIS_STREAM)
        {
            if (pfAudDecNfy != NULL)
            {
                UNUSED(_GetVorbisAudInfo(AUD_DEC_MAIN, &rAudDecInfo));
                pfAudDecNfy((void *)AUD_NFY_STREAM, u1DecId, AUD_COND_AUD_INFO_CHG,
                    (UINT32)&rAudDecInfo, u4Tmp);
                LOG(5, "[DSP_TASK]VORBIS Dec0 Aout Enable Notify\n");
            }
        }
#endif

        AUD_DrvMMAoutEnableNotify(AUD_DEC_MAIN);
        break;
    case D2RC_FLOW_CONTROL_MODE_CHANGE:
        u1DecId = AUD_DEC_MAIN;
        //vApiNotifyAfSoundPropertiesChanged();
        //vApiNotifyAfDRCAllowedChanged();
        if (_rDspVars[AUD_DEC_MAIN].bDspStrTyp == AC3_STREAM)
        {
            if (pfAudDecNfy != NULL)
            {
                _GetAudInfo(AUD_DEC_MAIN, &rAudDecInfo);
		  pfAudDecNfy((void *)AUD_NFY_STREAM, u1DecId, AUD_COND_AUD_INFO_CHG,
                        (UINT32)&rAudDecInfo, u4Tmp);
            }
        }
        else if ((_rDspVars[AUD_DEC_MAIN].bDspStrTyp == MPEG12_STREAM) || (_rDspVars[AUD_DEC_MAIN].bDspStrTyp == MPEG3_STREAM))
        {
            if (pfAudDecNfy != NULL)
            {
                BOOL fgIsUpdate;

                fgIsUpdate = _GetMpegAudInfo(AUD_DEC_MAIN, &rAudDecInfo);
                if (fgIsUpdate)
                {
                    pfAudDecNfy((void *)AUD_NFY_STREAM, u1DecId, AUD_COND_AUD_INFO_CHG,
                        (UINT32)&rAudDecInfo, u4Tmp);
                }
            }
        }
        else if (_rDspVars[AUD_DEC_MAIN].bDspStrTyp == AAC_STREAM)
        {
            if (pfAudDecNfy != NULL)
            {
                _GetAacAudInfo(AUD_DEC_MAIN, &rAudDecInfo);
                pfAudDecNfy((void *)AUD_NFY_STREAM, u1DecId, AUD_COND_AUD_INFO_CHG,
                    (UINT32)&rAudDecInfo, u4Tmp);
            }
        }
		LOG(3, " [DSP_TASK]: D2RC_FLOW_CONTROL_MODE_CHANGE Notify MW \n");
        break;
    case D2RC_FLOW_CONTROL_FLUSH_DONE:
        AUD_UopCommandDone(AUD_DEC_MAIN, DSP_STOP);
        _aeAudType[AUD_DEC_MAIN] = AUD_TYPE_UNKNOWN;
        break;
    case D2RC_FLOW_CONTROL_PAUSE_OK:
	    AUD_UopCommandDone(AUD_DEC_MAIN, DSP_PAUSE);
        // none handling
        break;
        /* secondary decoder */
    case D2RC_FLOW_CONTROL_SAMPLING_RATE_DEC2:
        break;
    case D2RC_FLOW_CONTROL_AOUT_ENABLE_DEC2:
        u1DecId = AUD_DEC_AUX;
        if (_rDspVars[AUD_DEC_AUX].bDspStrTyp == AC3_STREAM)
        {
            if (pfAudDecNfy != NULL)
            {
                rAudDecInfo.e_aud_fmt = AUD_FMT_AC3;
                rAudDecInfo.ui1_bit_depth = 24;
                _GetAudInfo(AUD_DEC_AUX, &rAudDecInfo);

                pfAudDecNfy((void *)AUD_NFY_STREAM, u1DecId, AUD_COND_AUD_INFO_CHG,
                    (UINT32)&rAudDecInfo, u4Tmp);
                LOG(5, "[DSP_TASK]AC3 Dec1 Aout Enable Notify\n");
            }
        }
        else if ((_rDspVars[AUD_DEC_AUX].bDspStrTyp == PCM_STREAM) || (_rDspVars[AUD_DEC_AUX].bDspStrTyp == LPCM_STREAM))
        {
            if (pfAudDecNfy != NULL)
            {
                //rAudDecInfo.e_aud_fmt = AUD_FMT_PCM;
#if 0
                rAudDecInfo.e_aud_fmt = (AUD_FMT_T)13; // FIXME
                rAudDecInfo.e_aud_type = AUD_TYPE_STEREO;
#else
                rAudDecInfo.e_aud_fmt = (AUD_FMT_T)13; // FIXME
                rAudDecInfo.e_aud_type = AUD_TYPE_MONO;
                if (_aeAudType[AUD_DEC_AUX] == AUD_TYPE_UNKNOWN)
                {
                    _aeAudType[AUD_DEC_AUX] = AUD_TYPE_MONO;
                }
#endif
#if 0
                rAudDecInfo.ui4_sample_rate = 44;
                rAudDecInfo.ui1_bit_depth = 24;
                pfAudDecNfy((void *)AUD_NFY_STREAM, u1DecId, AUD_COND_AUD_INFO_CHG,
                    (UINT32)&rAudDecInfo, u4Tmp);
#endif
                LOG(5, "[DSP_TASK]PCM Dec1 Aout Enable Notify\n");
            }
        }
        else if ((_rDspVars[AUD_DEC_AUX].bDspStrTyp == MPEG12_STREAM) || (_rDspVars[AUD_DEC_AUX].bDspStrTyp == MPEG3_STREAM))
        {
            if (pfAudDecNfy != NULL)
            {
                UNUSED(_GetMpegAudInfo(AUD_DEC_AUX, &rAudDecInfo));
                pfAudDecNfy((void *)AUD_NFY_STREAM, u1DecId, AUD_COND_AUD_INFO_CHG,
                    (UINT32)&rAudDecInfo, u4Tmp);
                LOG(5, "[DSP_TASK]MPG Dec1 Aout Enable Notify\n");
            }
        }
        else if (_rDspVars[AUD_DEC_AUX].bDspStrTyp == AAC_STREAM)
        {
        	if (pfAudDecNfy != NULL)
        	{
        		_GetAacAudInfo(AUD_DEC_AUX, &rAudDecInfo);
        		pfAudDecNfy((void *)AUD_NFY_STREAM, u1DecId, AUD_COND_AUD_INFO_CHG,
        			(UINT32)&rAudDecInfo, u4Tmp);
        		LOG(5, "[DSP_TASK]AAC Dec1 Aout Enable Notify\n");

#ifdef CC_MT5391_AUD_3_DECODER
                _AUD_DspSetIEC (AUD_DEC_MAIN);
#endif
        	}
        }
        else if ((_rDspVars[AUD_DEC_AUX].bDspStrTyp == PAL_STREAM) || (_rDspVars[AUD_DEC_AUX].bDspStrTyp == A2_STREAM))
        {
            if (pfAudDecNfy != NULL)
            {
                rAudDecInfo.e_aud_fmt = (AUD_FMT_T)13; // FIXME!! Hardcode here
                rAudDecInfo.e_aud_type = AUD_TYPE_MONO;
                //rAudDecInfo.ui4_sample_rate = 48;
               // rAudDecInfo.ui1_bit_depth = 24;
                //_aeAudType[AUD_DEC_AUX] = AUD_TYPE_MONO;
                if (_aeAudType[AUD_DEC_AUX] == AUD_TYPE_UNKNOWN)
                {
                _aeAudType[AUD_DEC_AUX] = AUD_TYPE_MONO;
                }
                LOG(5, "_aeAudType[AUX] --3= %d\n", _aeAudType[AUD_DEC_AUX]);
               // pfAudDecNfy((void *)AUD_NFY_STREAM, u1DecId, AUD_COND_AUD_INFO_CHG,
              //      (UINT32)&rAudDecInfo, u4Tmp);
                LOG(5, "[DSP_TASK]PAL/A2 Dec1 Aout Enable Notify\n");
            }
        }
        else if (_rDspVars[AUD_DEC_AUX].bDspStrTyp == DRA_STREAM)
       	{
            if (pfAudDecNfy != NULL)
        	{
        		_GetDraAudInfo(AUD_DEC_AUX, &rAudDecInfo);
        		pfAudDecNfy((void *)AUD_NFY_STREAM, u1DecId, AUD_COND_AUD_INFO_CHG, (UINT32)&rAudDecInfo, u4Tmp);
            }
       		LOG(5, "[DSP_TASK]DRA Dec1 Aout Enable Notify\n");
       	}
        else if (_rDspVars[AUD_DEC_AUX].bDspStrTyp == DTSDEC_STREAM)
       	{
       	    if (pfAudDecNfy != NULL)
        	{
        		_GetDTSAudInfo(AUD_DEC_AUX, &rAudDecInfo);
        		pfAudDecNfy((void *)AUD_NFY_STREAM, u1DecId, AUD_COND_AUD_INFO_CHG, (UINT32)&rAudDecInfo, u4Tmp);
            }
       		LOG(5, "[DSP_TASK]DTS Dec1 Aout Enable Notify\n");
       	}

       	else if (_rDspVars[AUD_DEC_AUX].bDspStrTyp == COOK_STREAM)
       	{
            if (pfAudDecNfy != NULL)
        	{
        		_GetDraAudInfo(AUD_DEC_AUX, &rAudDecInfo);
        		pfAudDecNfy((void *)AUD_NFY_STREAM, u1DecId, AUD_COND_AUD_INFO_CHG, (UINT32)&rAudDecInfo, u4Tmp);
            }
            LOG(5, "[DSP_TASK]COOK Dec1 Aout Enable Notify\n");
       	}

        else if (_rDspVars[AUD_DEC_AUX].bDspStrTyp == G729DEC_STREAM)
        {
            if (pfAudDecNfy != NULL)
            {
                rAudDecInfo.e_aud_fmt = AUD_FMT_G729DEC;
                rAudDecInfo.e_aud_type = AUD_TYPE_MONO;
                rAudDecInfo.ui4_sample_rate = 8;
                rAudDecInfo.ui1_bit_depth = 24;
                pfAudDecNfy((void *)AUD_NFY_STREAM, u1DecId, AUD_COND_AUD_INFO_CHG,
                    (UINT32)&rAudDecInfo, u4Tmp);
        	    LOG(5, "[DSP_TASK]G729 Dec1 Aout Enable Notify\n");
            }
        }
        break;
     case D2RC_FLOW_CONTROL_MODE_CHANGE_DEC2:
        u1DecId = AUD_DEC_AUX;
        if (_rDspVars[AUD_DEC_AUX].bDspStrTyp == AC3_STREAM)
        {
            if (pfAudDecNfy != NULL)
            {
                _GetAudInfo(AUD_DEC_AUX, &rAudDecInfo);
		  pfAudDecNfy((void *)AUD_NFY_STREAM, u1DecId, AUD_COND_AUD_INFO_CHG,
                        (UINT32)&rAudDecInfo, u4Tmp);
		  LOG(3, " [DSP_TASK]: D2RC_FLOW_CONTROL_MODE_CHANGE_DEC2 \n");
#ifdef CC_MT5391_AUD_3_DECODER
               //Under 3 decoder architecture, if current source is DTV, then dec1 is PCM and
               // its input is from dec2. In this condition, if there's any format change of dec2
               // we also need to notify this info to MW with dec1 for MW to do MTS control.
               if (_IsDualDecMode()&&(_rDspVars[AUD_DEC_MAIN].bDspStrTyp == PCM_STREAM))
               {
                   rAudDecInfo.e_aud_fmt = AUD_FMT_AC3;
                   rAudDecInfo.ui1_bit_depth = 24;
                   pfAudDecNfy((void *)AUD_NFY_STREAM, AUD_DEC_MAIN, AUD_COND_AUD_INFO_CHG,
                   (UINT32)&rAudDecInfo, u4Tmp);
                   LOG(5, "[DSP_TASK]DTV Source Dec0 (AC3 Dec1) Mode Change Notify\n");
                }
#endif
            }
        }
        else if (_rDspVars[AUD_DEC_AUX].bDspStrTyp == MPEG12_STREAM)
        {
            if (pfAudDecNfy != NULL)
            {
                BOOL fgIsUpdate;

                fgIsUpdate = _GetMpegAudInfo(AUD_DEC_AUX, &rAudDecInfo);
                if (fgIsUpdate)
                {
                    pfAudDecNfy((void *)AUD_NFY_STREAM, u1DecId, AUD_COND_AUD_INFO_CHG,
                        (UINT32)&rAudDecInfo, u4Tmp);

#ifdef CC_MT5391_AUD_3_DECODER
                    //Under 3 decoder architecture, if current source is DTV, then dec1 is PCM and
                    // its input is from dec2. In this condition, if there's any format change of dec2
                    // we also need to notify this info to MW with dec1 for MW to do MTS control.
                    if (_IsDualDecMode() && (_rDspVars[AUD_DEC_MAIN].bDspStrTyp == PCM_STREAM))
                    {
                        pfAudDecNfy((void *)AUD_NFY_STREAM, AUD_DEC_MAIN, AUD_COND_AUD_INFO_CHG,
                        (UINT32)&rAudDecInfo, u4Tmp);
                        LOG(5, "[DSP_TASK]DTV Source Dec0 (MP2 Dec1) Mode Change Notify\n");
                    }
#endif
                }
            }
        }
        else if (_rDspVars[AUD_DEC_AUX].bDspStrTyp == AAC_STREAM)
        {
            if (pfAudDecNfy != NULL)
            {
                _GetAacAudInfo(AUD_DEC_AUX, &rAudDecInfo);
                pfAudDecNfy((void *)AUD_NFY_STREAM, u1DecId, AUD_COND_AUD_INFO_CHG,
                    (UINT32)&rAudDecInfo, u4Tmp);
#ifdef CC_MT5391_AUD_3_DECODER
               //Under 3 decoder architecture, if current source is DTV, then dec1 is PCM and
               // its input is from dec2. In this condition, if there's any format change of dec2
               // we also need to notify this info to MW with dec1 for MW to do MTS control.
               if (_IsDualDecMode()&&(_rDspVars[AUD_DEC_MAIN].bDspStrTyp == PCM_STREAM))
               {
                   pfAudDecNfy((void *)AUD_NFY_STREAM, AUD_DEC_MAIN, AUD_COND_AUD_INFO_CHG,
                   (UINT32)&rAudDecInfo, u4Tmp);
                   LOG(5, "[DSP_TASK]DTV Source Dec0 (AAC Dec1) Mode Change Notify\n");
            }
#endif
        }
        }

        break;
    case D2RC_FLOW_CONTROL_FLUSH_DONE_DEC2:
        AUD_UopCommandDone(AUD_DEC_AUX, DSP_STOP_DEC2);
        _aeAudType[AUD_DEC_AUX] = AUD_TYPE_UNKNOWN;
        break;
#ifdef CC_MT5391_AUD_3_DECODER
        /* third decoder */
    case D2RC_FLOW_CONTROL_SAMPLING_RATE_DEC3:
        if (_rDspVars[AUD_DEC_THIRD].bDspStrTyp == AAC_STREAM)
        {
             _GetAacAudInfo(AUD_DEC_THIRD, &rAudDecInfo);
             if (rAudDecInfo.e_aud_type != AUD_TYPE_MONO)
             {
                   //disable Pan
                   AUD_SetAdPanEnable(FALSE);
             }
             else
             {
                   AUD_SetAdPanEnable(TRUE);
             }
        }
        break;
    case D2RC_FLOW_CONTROL_AOUT_ENABLE_DEC3:
        u1DecId = AUD_DEC_THIRD;
        if (_rDspVars[AUD_DEC_THIRD].bDspStrTyp == PCM_STREAM)
        {
            if (pfAudDecNfy != NULL)
            {
                rAudDecInfo.e_aud_fmt = (AUD_FMT_T)13; // FIXME
                rAudDecInfo.e_aud_type = AUD_TYPE_MONO;
                if (_aeAudType[AUD_DEC_THIRD] == AUD_TYPE_UNKNOWN)
                {
                    _aeAudType[AUD_DEC_THIRD] = AUD_TYPE_MONO;
                }
                LOG(5, "[DSP_TASK]PCM Dec2 Aout Enable Notify\n");
            }
        }
        else if ((_rDspVars[AUD_DEC_THIRD].bDspStrTyp == MPEG12_STREAM) || (_rDspVars[AUD_DEC_THIRD].bDspStrTyp == MPEG3_STREAM))
        {
            if (pfAudDecNfy != NULL)
            {
                UNUSED(_GetMpegAudInfo(AUD_DEC_THIRD, &rAudDecInfo));
                pfAudDecNfy((void *)AUD_NFY_STREAM, u1DecId, AUD_COND_AUD_INFO_CHG,
                    (UINT32)&rAudDecInfo, u4Tmp);
                LOG(5, "[DSP_TASK]MPG Dec2 Aout Enable Notify\n");
            }
        }
        else if (_rDspVars[AUD_DEC_THIRD].bDspStrTyp == AAC_STREAM)
        {
            if (pfAudDecNfy != NULL)
            {
        		_GetAacAudInfo(AUD_DEC_THIRD, &rAudDecInfo);
        		pfAudDecNfy((void *)AUD_NFY_STREAM, u1DecId, AUD_COND_AUD_INFO_CHG,
        			(UINT32)&rAudDecInfo, u4Tmp);
        		LOG(5, "[DSP_TASK]AAC Dec2 Aout Enable Notify\n");
            }
        }

        break;
    case D2RC_FLOW_CONTROL_MODE_CHANGE_DEC3:
        u1DecId = AUD_DEC_THIRD;
        if (_rDspVars[AUD_DEC_THIRD].bDspStrTyp == MPEG12_STREAM)
        {
            if (pfAudDecNfy != NULL)
            {
                BOOL fgIsUpdate;

                fgIsUpdate = _GetMpegAudInfo(AUD_DEC_THIRD, &rAudDecInfo);
                if (fgIsUpdate)
                {
                    pfAudDecNfy((void *)AUD_NFY_STREAM, u1DecId, AUD_COND_AUD_INFO_CHG,
                        (UINT32)&rAudDecInfo, u4Tmp);
                    LOG(5, "[DSP_TASK]MPG Dec2 Mode Change Notify\n");
                }
            }
        }
        break;
    case D2RC_FLOW_CONTROL_FLUSH_DONE_DEC3:
#ifdef MP3ENC_DBG
    	if(_rDspVars[AUD_DEC_THIRD].bDspStrTyp == MP3ENC_STREAM)
    	{
          LOG(7, "[DSP_TASK] D2RC->MP3 Encoder STOP. \n");
    	}
#endif

#ifdef MP3ENC_SUPPORT
		if(_rDspVars[AUD_DEC_THIRD].bDspStrTyp == MP3ENC_STREAM)
		{
			vWriteDspSram(SRAM_INTBUF4_INPUT_FLAG, 0x00);
		}
#endif

        AUD_UopCommandDone(AUD_DEC_THIRD, DSP_STOP_DEC3);
        _aeAudType[AUD_DEC_THIRD] = AUD_TYPE_UNKNOWN;
        break;
#endif

    case D2RC_FLOW_CONTROL_SAMPLING_RATE_CHANGE:
	 //do nothing !!
	 LOG(5,"[DSP_TASK] Get sampling change of MPEG dec0, but do nothing !!! ######\n");
     break;
     case D2RC_FLOW_CONTROL_SAMPLING_RATE_CHANGE_DEC2:
	 if(   (_AudGetStrSource(AUD_DEC_AUX) == AUD_STREAM_FROM_DIGITAL_TUNER) &&
		 (_AudGetStrSource(AUD_DEC_MAIN) == AUD_STREAM_FROM_DIGITAL_TUNER)) //only consider in DTV sousrce
	 {
         if (_rDspVars[AUD_DEC_AUX].bDspStrTyp == MPEG12_STREAM)
         {
		     LOG(5,"[DSP_TASK] Get sampling change of MPEG dec1, need to do aout-reinit for dec2 & dec1 ######\n");
		     _AUD_DspSetAoutReinit();
	     }
/*		 else if (_rDspVars[AUD_DEC_AUX].bDspStrTyp == AAC_STREAM)
		 {
             LOG(5,"[DSP_TASK] Get sampling change of AAC dec1, need to do aout-reinit for dec1 ######\n");
             DSP_SendDspTaskCmd(UOP_DSP_AOUT_REINIT_FOR_SAMPLING_RATE_CHANGE);
		 }
*/		 else
		 {
             //do nothing !!
             LOG(5,"[DSP_TASK] Get sampling change of Dec1, but do nothing !!! ######\n");
		 }
	 }
	  break;

#ifdef CC_AUD_4_DECODER_SUPPORT
    case D2RC_FLOW_CONTROL_SAMPLING_RATE_DEC4:
        break;
    case D2RC_FLOW_CONTROL_AOUT_ENABLE_DEC4:
        u1DecId = AUD_DEC_4TH;
        if (_rDspVars[AUD_DEC_4TH].bDspStrTyp == SBC_STREAM)
        {
            LOG(5, "[DSP_TASK]SBCENC Dec3 Aout Enable Notify\n");
        }
        break;
    case D2RC_FLOW_CONTROL_MODE_CHANGE_DEC4:
        u1DecId = AUD_DEC_4TH;
        if (_rDspVars[AUD_DEC_4TH].bDspStrTyp == SBC_STREAM)
        {
            LOG(5, "[DSP_TASK]SBCENC Dec3 Mode Change Notify\n");
        }
        break;
    case D2RC_FLOW_CONTROL_FLUSH_DONE_DEC4:
        AUD_UopCommandDone(AUD_DEC_4TH, DSP_STOP_DEC4);
        _aeAudType[AUD_DEC_4TH] = AUD_TYPE_UNKNOWN;
        break;
#endif

    default:
        // exception
        break;
    }
}

TV_AUD_SYS_T DSP_GetAtvTvSystem(void)
{
    return _eAudTvSystem;
}

UINT32 DSP_GetAtvTvSystemStrength(TV_AUD_SYS_T eAudSys)
{
    UINT32 u1AtvStrength=0;
	switch (eAudSys)
    {
    case SV_NTSC_M:
    	u1AtvStrength=gu4Energy45M;
		break;
    case SV_PAL_BG:
    	u1AtvStrength=gu4Energy55M;
		break;
    case SV_PAL_I:
        u1AtvStrength=gu4Energy60M;
		break;
    case SV_PAL_DK:
    case SV_SECAM_L:
    	u1AtvStrength=gu4Energy65M;
		break;
    default:
    	u1AtvStrength=0;
		break;
    }
	return (UINT32)u1AtvStrength;
}

void DSP_GetAtvTvSys(MW_TV_AUD_SYS_T * prTvAudSys)
{
	if (prTvAudSys != NULL)
	{
		prTvAudSys->u4TvSysMask = _rTvAudSys.u4TvSysMask;
		prTvAudSys->u4AudSysMask = _rTvAudSys.u4AudSysMask;
	}
}

BOOL DSP_RealStop(UINT8 u1DecId)
{
    return _rDspStatus[u1DecId].fgDspStop;
}

void DSP_ShareInforDump(UINT8 u1Group)
{
    UINT32 u4Index;

    Printf("\n");
	Printf("  +---- Group %2d ----+\n", u1Group);

    Printf("  +----+-------------+\n");
			for (u4Index = 0; u4Index < 0x20; u4Index+=4)
			{
				Printf("   0x%02x| %02x %02x %02x %02x \n",
					u4Index,
					uReadDspShmBYTE(((u1Group*32) + u4Index+3)),
					uReadDspShmBYTE(((u1Group*32) + u4Index+2)),
					uReadDspShmBYTE(((u1Group*32) + u4Index+1)),
					uReadDspShmBYTE(((u1Group*32) + u4Index)) );
			}

	Printf("  +----+-------------+\n");
}

// Light added for AVC
void DSP_ShareInforWriteWord(UINT32 ADDR, UINT16 VALUE)
{
    vWriteDspShmWORD(ADDR, VALUE);
}

void DSP_CommDramDump(UINT32 u4Address, UINT32 u4Len)
{
    UINT32 u4Idx;

    Printf("\n");
    DSP_FlushInvalidateDCacheFree((_u4DspBuf+((u4Address)<<2)), (u4Len << 2));

	for (u4Idx = 0;
		u4Idx < u4Len;
		u4Idx += 4)
	{
                Printf("0x%04X (0x%08x)| %08X %08X %08X %08X\n"
                	, (u4Address + u4Idx), dReadDspCommDramAddr(u4Address + u4Idx + 0)
        #if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
            , dReadDspCommDram32(u4Address + u4Idx + 0)
            , dReadDspCommDram32(u4Address + u4Idx + 1)
            , dReadDspCommDram32(u4Address + u4Idx + 2)
            , dReadDspCommDram32(u4Address + u4Idx + 3)
        #else
			, dReadDspCommDram(u4Address + u4Idx + 0)
			, dReadDspCommDram(u4Address + u4Idx + 1)
			, dReadDspCommDram(u4Address + u4Idx + 2)
			, dReadDspCommDram(u4Address + u4Idx + 3)
        #endif
			);
	}
}

void DSP_NormDramDump(UINT8 u1Page, UINT32 u4Address, UINT32 u4Len)
{
    UINT32 u4Idx;

    Printf("\n");
    DSP_FlushInvalidateDCacheFree((_u4DspDramBuf[u1Page]+((u4Address)<<2)), (u4Len << 2));

	for (u4Idx = 0;
		u4Idx < u4Len;
		u4Idx += 4)
	{
        Printf("0x%04X (0x%08x)| %08X %08X %08X %08X\n"
        	, (u4Address + u4Idx), dReadDspNormDramAddr(u1Page, u4Address + u4Idx + 0)
        	, dReadDspNormDram(u1Page, u4Address + u4Idx + 0)
        	, dReadDspNormDram(u1Page, u4Address + u4Idx + 1)
        	, dReadDspNormDram(u1Page, u4Address + u4Idx + 2)
        	, dReadDspNormDram(u1Page, u4Address + u4Idx + 3)
        	);
	}
}

void DSP_CmptDramDump(UINT8 u1Page, UINT32 u4Address, UINT32 u4Len)
{
    UINT32 u4Idx;

    Printf("\n");
    DSP_FlushInvalidateDCacheFree((_u4DspCmptBuf[u1Page]+((u4Address)<<2)), (u4Len << 2));

	for (u4Idx = 0;
		u4Idx < u4Len;
		u4Idx += 4)
	{
        Printf("0x%04X (0x%08x)| %08X %08X %08X %08X\n"
        	, (u4Address + u4Idx), dReadDspCmptDramAddr(u1Page, u4Address + u4Idx + 0)
        	, dReadDspCmptDram(u1Page, u4Address + u4Idx + 0)
        	, dReadDspCmptDram(u1Page, u4Address + u4Idx + 1)
        	, dReadDspCmptDram(u1Page, u4Address + u4Idx + 2)
        	, dReadDspCmptDram(u1Page, u4Address + u4Idx + 3)
        	);
	}
}

void Dsp_WriteDspCommDram(UINT32 u4SrcAddr, UINT32 u4Len)
{
	WriteDspCommDram(u4SrcAddr, u4Len);
}

UINT32 DSP_CommDramRead(UINT32 u4Address)
{
    return dReadDspCommDram(u4Address);
}

#ifdef CC_AUD_FIR_SUPPORT
UINT32 DSP_CommDramAddr(UINT32 u4Address)  //CC_AUD_FIR_SUPPORT
{
    return dReadDspCommDramAddr(u4Address);
}
#endif

UINT32 DSP_DecDramRead(UINT32 u4Address)
{
    UINT32 u4DramValue;
    ReadDspDecCacheDram(u4Address, u4DramValue);
    return u4DramValue;
}
UINT32 DSP_Dec2DramRead(UINT32 u4Address)
{
    UINT32 u4DramValue;
    ReadDspDec2CacheDram(u4Address, u4DramValue);
    return u4DramValue;
}
void DSP_LoadRamCode(UINT32 u4Type)
{
	vDspLoadRamCode(u4Type);
}

void DSP_ResetDec(UINT8 u1DecId)
{
	AUD_DEC_ID_VALIDATE(u1DecId);

    if (_rDspStatus[u1DecId].fgDspPlay)
    {
        vDspReset(u1DecId);
    }
}

INT32 DSP_SendPts(UINT8 u1DecId, UINT32 u4Pts, UINT32 u4ParPnt)
{
	AUD_DEC_ID_VALIDATE(u1DecId);

    return i4DspSendPts(u1DecId, u4Pts, u4ParPnt);
}

void DSP_GetAFIFOVirtualAddr(UINT8 u1DecId, UINT32 * pu4AddrStart, UINT32 * pu4AddrEnd)
{
	AUD_DEC_ID_VALIDATE(u1DecId);

    if ((pu4AddrStart != NULL) && (pu4AddrEnd != NULL))
    {
#ifdef CC_MT5391_AUD_3_DECODER
        if (u1DecId == AUD_DEC_THIRD)
        {
            u1DecId = AUD_DEC_MAIN;
        }
#endif
        *pu4AddrStart = DSP_VIRTUAL_ADDR(_u4AFIFO[u1DecId][0]);
        *pu4AddrEnd = DSP_VIRTUAL_ADDR(_u4AFIFO[u1DecId][1]);
    }
}

void DSP_GetAFIFOPhysicalAddr(UINT8 u1DecId, UINT32 * pu4AddrStart, UINT32 * pu4AddrEnd)
{
	AUD_DEC_ID_VALIDATE(u1DecId);

    if ((pu4AddrStart != NULL) && (pu4AddrEnd != NULL) )
    {
#ifdef CC_MT5391_AUD_3_DECODER
  #ifdef CC_AUD_DEC3_USE_FIFO1 //ndef CC_AUD_SKYPE_SUPPORT
        if (u1DecId == AUD_DEC_THIRD)
        {
            u1DecId = AUD_DEC_MAIN;
        }
#endif
#endif
        *pu4AddrStart = DSP_PHYSICAL_ADDR(_u4AFIFO[u1DecId][0]);
        *pu4AddrEnd = DSP_PHYSICAL_ADDR(_u4AFIFO[u1DecId][1]);
    }
}
#ifdef CC_AUD_HDMI_PARSER_2_0_SUPPORT
void DSP_GetHDMIParserFIFOVirtualAddr(UINT32 * pu4AddrStart, UINT32 * pu4AddrEnd)
{
        *pu4AddrStart = DSP_VIRTUAL_ADDR(_u4AFIFOHdmiParser[0]);
        *pu4AddrEnd = DSP_VIRTUAL_ADDR(_u4AFIFOHdmiParser[1]);
}

void DSP_GetHDMIParserFIFOPhysicalAddr(UINT32 * pu4AddrStart, UINT32 * pu4AddrEnd)
{
        *pu4AddrStart = DSP_PHYSICAL_ADDR(_u4AFIFOHdmiParser[0]);
        *pu4AddrEnd = DSP_PHYSICAL_ADDR(_u4AFIFOHdmiParser[1]);
}
#endif
/******************************************************************************
* Function      : DSP_GetDspReadPtrPhysicalAddr
* Description   : get audio DSP read pointer (REG_BUF_PNT/REG_BUF_PNT_DEC2)
* Parameter     : uDecIndex: 0: first decoder,1: second decoder
* Return        : ADSP read pointer of the decoder
******************************************************************************/
UINT32 DSP_GetDspReadPtrPhysicalAddr(UINT8 u1DecId)
{
    UINT32 u4ReadPtrAddr;
    UINT32 u4ReadPtr;

    //DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_DATA, TYPE_COMM_DRAM_IDX);
#ifndef CC_MT5391_AUD_3_DECODER
    u4ReadPtrAddr = (u1DecId == AUD_DEC_MAIN) ?
    	            ADDR_D2RC_RISC_INFO_REG_BUF_PNT : ADDR_D2RC_RISC_INFO_REG_BUF_PNT_DEC2;
#else
    if (u1DecId == AUD_DEC_MAIN)
    {
        u4ReadPtrAddr = ADDR_D2RC_RISC_INFO_REG_BUF_PNT;
    }
    else if (u1DecId == AUD_DEC_AUX)
    {
        u4ReadPtrAddr = ADDR_D2RC_RISC_INFO_REG_BUF_PNT_DEC2;
    }
    else
    {
        u4ReadPtrAddr = ADDR_D2RC_RISC_INFO_REG_BUF_PNT_DEC3;
    }
#endif
    DSP_FlushInvalidateDCacheSmall(CACHE_FLUSH_TYPE_DATA, TYPE_COMM_DRAM_IDX, (u4ReadPtrAddr<<2) ,1);
	u4ReadPtr = DSP_PHYSICAL_ADDR(dReadDspCommDram(u4ReadPtrAddr));
    return u4ReadPtr;
}

void DSP_SendDspTaskCmd(UINT32 u4Cmd)
{
    VERIFY(_rDspStatus[AUD_DEC_MAIN].fgDspWakeUpOk == TRUE);
    vSendADSPTaskMessage(u4Cmd);
}

#ifdef FRAC_SPEED_SUPPORT
//sunman for play speed
void DSP_SpeedSetMode(UINT32 u4Mode)
{
    vWriteDspShmDWRD(D_PLAY_SPEED, u4Mode);
    vDspCmd(UOP_DSP_FRAC_SPEED);
    return;
}
#endif

#ifdef CC_AUD_SILENCE_SUPPORT
//for silence fuction
void DSP_SilenceSetMode(BOOL fgEnable)
{
    if(fgEnable == TRUE)
    {
         WriteDspCommDram (ADDR_RC2D_SILENCE_GAIN_MODE, 0x200);
    }
    else
    {
         WriteDspCommDram (ADDR_RC2D_SILENCE_GAIN_MODE, 0x0);
    }
}

void DSP_SilenceSetThreshold(UINT32 u1Threshold)
{
	WriteDspCommDram(ADDR_RC2D_SILENCE_FIX_THRESHOLD, u1Threshold);
}


void DSP_SilenceSetWait(UINT32 u1Wait)
{
    UINT32 u4Wait1;
	u4Wait1 = (u1Wait*48/256) << 8;
	WriteDspCommDram(ADDR_RC2D_SILENCE_WAIT_NUM, u4Wait1);
	WriteDspCommDram(ADDR_RC2D_SILENCE_GAIN_BASE+1, u4Wait1);
}

void DSP_SilenceSetFixGain(UINT32 u4AttackFixGain)
 {
	 WriteDspCommDram(ADDR_RC2D_SILENCE_ATTACK_GAIN, u4AttackFixGain);
 }

void DSP_SilenceSetReleaseStep(UINT32 u4ReleaseStep)
{
    UINT32 u4Release;
	u4Release = 0x7fffff/(u4ReleaseStep*48/256);
	WriteDspCommDram(ADDR_RC2D_SILENCE_RELEASE_STEP, u4Release);
}
void DSP_SilenceSetAttackStep(UINT32 u4AttackStep)
{
    UINT32 u4Attack;
	u4Attack = 0x7fffff/(u4AttackStep*48/256);
	WriteDspCommDram(ADDR_RC2D_SILENCE_ATTACK_STEP, u4Attack);
}


void DSP_QrySilenceMode(UINT32 *u4Enable)
{
	*u4Enable = DSP_CommDramRead(ADDR_RC2D_SILENCE_GAIN_MODE);
	if(*u4Enable == 0x200)
	{
		*u4Enable = 1;
	}
	else
	{
		*u4Enable = 0;
	}
}

void DSP_QrySilenceSetThreshold(UINT32 *u4Threshold)
{
	*u4Threshold = DSP_CommDramRead(ADDR_RC2D_SILENCE_FIX_THRESHOLD);
}

void DSP_QrySilencePeakValue(UINT32 *u4PeakValue)
{
	*u4PeakValue = DSP_CommDramRead(ADDR_RC2D_SILENCE_PEAK_VALUE);
}

void DSP_QrySilenceWait(UINT32 *u4Wait)
{
	*u4Wait = (DSP_CommDramRead(ADDR_RC2D_SILENCE_WAIT_NUM) >> 8)*256/48;
}

void DSP_QrySilenceAttackFixGain(UINT32 *u4AttackFixGain)
{
	*u4AttackFixGain = DSP_CommDramRead(ADDR_RC2D_SILENCE_ATTACK_GAIN);

}

void DSP_QrySilenceReleaseStep(UINT32 *u4ReleaseStep)
{
	*u4ReleaseStep = (0x7fffff/DSP_CommDramRead(ADDR_RC2D_SILENCE_RELEASE_STEP))*256/48;

}

void DSP_QrySilenceAttackStep(UINT32 *u4AttackStep)
{
	*u4AttackStep = (0x7fffff/DSP_CommDramRead(ADDR_RC2D_SILENCE_ATTACK_STEP))*256/48;
}
#endif

#ifdef DSP_SUPPORT_SRSTSXT
/******************************************************************************
* Function      : void DSP_SRSTSXTSetMode()
* Description   : Set SRS TSXT mode of output audio.
* Parameters    : u1Mode
* Return        : None
******************************************************************************/
void DSP_SRSTSXTSetMode(UINT8 u1Mode)
{
    if (_rDspVars[AUD_DEC_MAIN].bCode3D == u1Mode)
    {
        return;
    }
    if (u1Mode != (UINT8)SV_A_SRSTSXT_OFF)
    {
        UINT32 	u4Tmp = u4ReadDspShmDWRD(D_SRS_TSXT_FLAGS);
        switch(u1Mode)
        {
        case SV_A_SRSTSXT_3DMONO:
            u4Tmp &= 0xFFFF0FF0;  //Bypass: 0, Channel mode: 0: mono
			break;
		case SV_A_SRSTSXT_STEREO:
            u4Tmp &= 0xFFFF0FF0;  //Bypass: 0, and clean INPUT_MODE
            u4Tmp |= 0x00001000;  //Channel mode: 1: stereo
            break;
        default:
            return;               //exception
		}
        vWriteDspShmDWRD(D_SRS_TSXT_FLAGS, u4Tmp);
#ifndef CC_NEW_POST_MULTI_VS_SUPPORT
        vWriteDspShmBYTE(B_VSURRFLAG, VSS_SRS_TSXT);
#else
        vWriteDspShmWORD(W_VSURRFLAG, VSS_SRS_TSXT);
#endif
        vDspCmd(UOP_DSP_SRS_TSXT_FLAG);
    }
    else
    {
#ifndef CC_NEW_POST_MULTI_VS_SUPPORT
        if (uReadDspShmBYTE(B_VSURRFLAG) != VSS_NONE)
#else
        if (u2ReadDspShmWORD(W_VSURRFLAG) != VSS_NONE)
#endif
        {
#ifndef CC_NEW_POST_MULTI_VS_SUPPORT
            vWriteDspShmBYTE(B_VSURRFLAG, VSS_NONE);
#else
            vWriteDspShmWORD(W_VSURRFLAG, VSS_NONE);
#endif
            vDspCmd(UOP_DSP_SRS_TSXT_FLAG);
        }
    }

    _rDspVars[AUD_DEC_MAIN].bCode3D = u1Mode;

}
/******************************************************************************
* Function      : void DSP_SRSTSXTSetSpkSize()
* Description   : Set SRS TSXT speaker size.
                  Call after Share Info. Init. For initialization only.
                  For dynamicly changing speaker size, please contact with Rice.
                  Speaker size table: 0: 40Hz, 1: 60Hz, 2: 100Hz, 3: 150Hz,
                                      4: 200Hz, 5: 250Hz, 6: 300Hz, 7: 400Hz
* Parameters    : u1Mode {0~7}
* Return        : None
******************************************************************************/
#define SRSTSXT_MAX_SPK_SIZE        (7)
#define SRSTSXT_MIN_SPK_SIZE        (0)
void DSP_SRSTSXTSetSpkSize(UINT8 u1Mode)
{
    if (u1Mode <= SRSTSXT_MAX_SPK_SIZE) // UINT always >=0
    {
        UINT32 	tmp = u4ReadDspShmDWRD(D_SRS_TSXT_FLAGS);
        tmp &= 0xF0FFFFFF;
        tmp |= ((UINT32)(u1Mode&0x0f))<<24;
        vWriteDspShmDWRD(D_SRS_TSXT_FLAGS, tmp);
        vDspCmd(UOP_DSP_SRS_TSXT_FLAG);//do re-init.
    }
    return;
}

/*
    bit  0~ 3:  Bypass (0: No, 1: Yes)
    bit  4~ 7:  TS Disable (0: No, 1: Yes)
    bit  8~11:  TS Headphone Mode (0: No, 1: Yes)
    bit 12~15:  Input Mode  (0~12)
    bit 16~19:  FOCUS Enable (0: No, 1: Yes)
    bit 20~23:  TruBass Enable (0: No, 1: Yes)
    bit 24~27:  TruBass Speaker Size (0~7)
*/
void DSP_SRSTSXTTBLvl(UINT8 u1Mode)
{

    if (u1Mode == 0)
    {
        vWriteDspShmDWRD(D_SRS_TSXT_TB_TBCtrl,	0);
    }
    else if (u1Mode == 1) //38%
    {
        vWriteDspShmDWRD(D_SRS_TSXT_TB_TBCtrl,	(UINT32)(0.38*0x07FFFFF));
    }
    else if (u1Mode == 2) //60%
    {
        vWriteDspShmDWRD(D_SRS_TSXT_TB_TBCtrl,	(UINT32)(0.6*0x07FFFFF));
    }
    else if (u1Mode == 3) //100%
    {
        vWriteDspShmDWRD(D_SRS_TSXT_TB_TBCtrl,	DRVCUST_OptGet(eAudioTSXTTBCtrl));
    }

    vDspCmd(UOP_DSP_SRS_TSXT_FLAG); //do re-init.

    return;
}

void DSP_SRSTSXTTBLvlRawData(UINT32 u4Lvl)
{
    vWriteDspShmDWRD(D_SRS_TSXT_TB_TBCtrl,	u4Lvl);

    vDspCmd(UOP_DSP_SRS_TSXT_FLAG); //do re-init.

    return;
}

void DSP_SRSTSXTinputgain(UINT32 u4Mode)
{


    vWriteDspShmDWRD(D_SRS_TSXT_Input_gain,	u4Mode);
    vDspCmd(UOP_DSP_SRS_TSXT_FLAG); //do re-init.

    return;
}

void DSP_SRSTSXTElev(UINT8 u1Mode)
{
    if (u1Mode == 0)
    {
        vWriteDspShmDWRD(D_SRS_TSXT_FC_Elevation,	0);
    }
    else if (u1Mode == 1) //10%
    {
        vWriteDspShmDWRD(D_SRS_TSXT_FC_Elevation,	(UINT32)(0.1*0x07FFFFF));
    }
    else if (u1Mode == 2) //50%
    {
        vWriteDspShmDWRD(D_SRS_TSXT_FC_Elevation,	(UINT32)(0.5*0x07FFFFF));
    }
    else if (u1Mode == 3) //100%
    {
        vWriteDspShmDWRD(D_SRS_TSXT_FC_Elevation,	DRVCUST_OptGet(eAudioTSXTElev));
    }

    vDspCmd(UOP_DSP_SRS_TSXT_FLAG); //do re-init.

    return;
}
#endif

//sunman_tshd
void DSP_SRSTSHDSurrLvl(UINT8 u1Mode)
{
    UINT32 u4Mode1;

    u4Mode1 = 0x66666;
    u4Mode1 *= (UINT32)u1Mode;
    vWriteDspShmDWRD (D_SRS_TSHD_Surr_Level, u4Mode1);
    vDspCmd(UOP_DSP_SRS_TSXT_FLAG); //do re-init.
}

void DSP_SRSTSHDInpg(UINT8 u1Mode)
{
    UINT32 u4Mode1;

    u4Mode1 = 0x66666;
    u4Mode1 *= (UINT32)u1Mode;
    vWriteDspShmDWRD (D_SRS_TSHD_INPUT_GAIN, u4Mode1);
    vDspCmd(UOP_DSP_SRS_TSXT_FLAG); //do re-init.
}

void DSP_SRSTSHDOutg(UINT8 u1Mode)
{
    UINT32 u4Mode1;

    u4Mode1 = 0x66666;
    u4Mode1 *= (UINT32)u1Mode;
    vWriteDspShmDWRD (D_SRS_TSHD_OUTPUT_GAIN, u4Mode1);
    vDspCmd(UOP_DSP_SRS_TSXT_FLAG); //do re-init.
}

void DSP_SRSTSHDSetSpkSize(UINT8 u1Mode)
{
    vWriteDspShmBYTE(B_SRS_TSHD_SPKSIZE, u1Mode);
    vDspCmd(UOP_DSP_SRS_TSXT_FLAG);//do re-init.
}



void DSP_SRSTSHDSetTSCtrl(void)
{
	UINT32 u4Temp;

    u4Temp = u4ReadDspShmDWRD(D_SRS_TSHD_FLAGS);
    u4Temp |= 0x00000800;
    vWriteDspShmDWRD(D_SRS_TSHD_FLAGS, u4Temp);
    vDspCmd(UOP_DSP_SRS_TSXT_FLAG);//do re-init.
}

void DSP_SRSTSHDTBLvl(UINT8 u1Mode)
{
    UINT32 u4Mode1;

    u4Mode1 = 0x66666;
    u4Mode1 *= (UINT32)u1Mode;
    vWriteDspShmDWRD (D_SRS_TSHD_TB_TBCtrl, u4Mode1);
    vDspCmd(UOP_DSP_SRS_TSXT_FLAG); //do re-init.
}



void DSP_SRSTSHDDef(UINT8 u1Mode)
{
    UINT32 u4Mode1;

    u4Mode1 = 0x66666;
    u4Mode1 *= (UINT32)u1Mode;
    vWriteDspShmDWRD (D_SRS_TSHD_DefCtrl, u4Mode1);
    vDspCmd(UOP_DSP_SRS_TSXT_FLAG); //do re-init.
}


void DSP_SRSTSHDElev(UINT8 u1Mode)
{
    UINT32 u4Mode1;

    u4Mode1 = 0x66666;
    u4Mode1 *= (UINT32)u1Mode;
    vWriteDspShmDWRD (D_SRS_TSHD_FC_Elevation, u4Mode1);
    vWriteDspShmDWRD (D_SRS_TSHD_WOW_FC_ctrl, u4Mode1);
    vDspCmd(UOP_DSP_SRS_TSXT_FLAG); //do re-init.
}

void DSP_SRSTSHDFntCCtrl(UINT8 u1Mode)
{
    UINT32 u4Mode1;

    u4Mode1 = 0x66666;
    u4Mode1 *= (UINT32)u1Mode;
    vWriteDspShmDWRD (D_SRS_TSHD_Center_Ctrl, u4Mode1);
    vDspCmd(UOP_DSP_SRS_TSXT_FLAG); //do re-init.
}

void DSP_SRSTSHDFntSCtrl(UINT8 u1Mode)
{
    UINT32 u4Mode1;

    u4Mode1 = 0x66666;
    u4Mode1 *= (UINT32)u1Mode;
    vWriteDspShmDWRD (D_SRS_TSHD_Space_Ctrl, u4Mode1);
    vDspCmd(UOP_DSP_SRS_TSXT_FLAG); //do re-init.
}

void DSP_SRSTSHDMode(UINT32 u4Mode)
{
    vWriteDspShmDWRD (D_SRS_TSHD_FLAGS, u4Mode);
    vDspCmd(UOP_DSP_SRS_TSXT_FLAG); //do re-init.
}


#ifdef DSP_SUPPORT_SRS_SSHD
//sunman_peq
void DSP_SRSPeqOn(UINT32 u4Mode)
{
	u4Mode |= (u4ReadDspShmDWRD (W_SRS_STUDIO_FLAG) & 0xFFFFFDFF);
	vWriteDspShmWORD (W_SRS_STUDIO_FLAG, u4Mode);
       DSP_SendDspTaskCmd(UOP_DSP_SRS_STUDIO_FLAG);
}
void DSP_SRSPeqInputGain(UINT32 u4Mode)
{
       u4Mode = 0x6666 * u4Mode;
       vWriteDspShmDWRD (D_SRS_PEQ_INPUT_GAIN, u4Mode);
       DSP_SendDspTaskCmd(UOP_DSP_SRS_STUDIO_FLAG);
}
void DSP_SRSPeqOutputGain(UINT32 u4Mode)
{
       u4Mode = 0x6666 * u4Mode;
       vWriteDspShmDWRD (D_SRS_PEQ_OUTPUT_GAIN, u4Mode);
       DSP_SendDspTaskCmd(UOP_DSP_SRS_STUDIO_FLAG);
}
void DSP_SRSHpfOn(UINT32 u4Mode)
{
	u4Mode |= (u4ReadDspShmDWRD (W_SRS_STUDIO_FLAG) & 0xFFFFFBFF);
	vWriteDspShmWORD (W_SRS_STUDIO_FLAG, u4Mode);
       DSP_SendDspTaskCmd(UOP_DSP_SRS_STUDIO_FLAG);
}
void DSP_SRSPEQHpfSetCoef(UINT32 u4Adr, UINT32 u4Coef)
{
       UINT32 u4Base;
       u4Base = dReadDspCommDram(ADDR_RC2D_SRS_PEQ_BASE) + ADDR_SRS_PEQ_HPF_COEF;
       WriteDspCommDram(u4Base+u4Adr, u4Coef);
}

void DSP_SRSPEQSetCoef(UINT32 u4Adr, UINT32 u4Coef)
{
       UINT32 u4Base;
       u4Base = dReadDspCommDram(ADDR_RC2D_SRS_PEQ_BASE) + ADDR_SRS_PEQ_COEF;
       WriteDspCommDram(u4Base+u4Adr, u4Coef);
}

void DSP_SRSHpfSetCoef(UINT32 u4Adr, UINT32 u4Coef)
{
       UINT32 u4Base;
       u4Base = dReadDspCommDram(ADDR_RC2D_SRS_PEQ_BASE) + ADDR_SRS_HPF_COEF;
       WriteDspCommDram(u4Base+u4Adr, u4Coef);
}

void DSP_SRSLmtOn(UINT32 u4Mode) //sunman_lmt
{
       u4Mode |= (u4ReadDspShmDWRD (W_SRS_STUDIO_FLAG) & 0xFFFFF7FF);
	vWriteDspShmWORD (W_SRS_STUDIO_FLAG, u4Mode);
       DSP_SendDspTaskCmd(UOP_DSP_SRS_STUDIO_FLAG);
}
void DSP_SRSLmtInputGain(UINT32 u4Mode) //sunman_lmt
{
       u4Mode *=  0x1999a;
       vWriteDspShmDWRD (D_SRS_LMT_INPUT_GAIN, u4Mode);
       DSP_SendDspTaskCmd(UOP_DSP_SRS_STUDIO_FLAG);
}
void DSP_SRSLmtOutputGain(UINT32 u4Mode)
{
       u4Mode *=  0x1999a ;
       vWriteDspShmDWRD (D_SRS_LMT_OUTPUT_GAIN, u4Mode);
       DSP_SendDspTaskCmd(UOP_DSP_SRS_STUDIO_FLAG);
}
void DSP_SRSLmtBoost(UINT32 u4Mode)
{
       //u4Mode = 0x20 * u4Mode;
       vWriteDspShmDWRD (D_SRS_LMT_BOOST, u4Mode);
       DSP_SendDspTaskCmd(UOP_DSP_SRS_STUDIO_FLAG);
}
void DSP_SRSLmtLvl(UINT32 u4Mode)
{
       //u4Mode = 0x20C4 * u4Mode;
       vWriteDspShmDWRD (D_SRS_LMT_LEVEL, u4Mode);
       DSP_SendDspTaskCmd(UOP_DSP_SRS_STUDIO_FLAG);
}
void DSP_SRSLmtDly(UINT32 u4Mode)
{
       vWriteDspShmDWRD (D_SRS_LMT_DELAY, u4Mode << 8);
       DSP_SendDspTaskCmd(UOP_DSP_SRS_STUDIO_FLAG);
}
void DSP_SRSLmtCoeff(UINT32 u4Mode)
{
       vWriteDspShmDWRD (D_SRS_LMT_COEFF, u4Mode);
       DSP_SendDspTaskCmd(UOP_DSP_SRS_STUDIO_FLAG);
}
void DSP_SRSLmtThresh(UINT32 u4Mode)
{
       vWriteDspShmDWRD (D_SRS_LMT_THRESH, u4Mode);
       DSP_SendDspTaskCmd(UOP_DSP_SRS_STUDIO_FLAG);
}
#endif

#ifdef DSP_SUPPORT_SRS_PURESOUND
void DSP_SRSLmtOn(UINT32 u4Mode) //sunman_lmt
{
       u4Mode |= (u4ReadDspShmDWRD (W_SRS_PURESOUND_FLAG) & 0xFFFFF7FF);
	vWriteDspShmWORD (W_SRS_PURESOUND_FLAG, u4Mode);
       DSP_SendDspTaskCmd(UOP_DSP_SRS_PURESOUND_FLAG);
}
void DSP_SRSLmtInputGain(UINT32 u4Mode) //sunman_lmt
{
       u4Mode *=  0x1999a;
       vWriteDspShmDWRD (D_SRS_LMT_INPUT_GAIN, u4Mode);
       DSP_SendDspTaskCmd(UOP_DSP_SRS_PURESOUND_FLAG);
}
void DSP_SRSLmtOutputGain(UINT32 u4Mode)
{
       u4Mode *=  0x1999a ;
       vWriteDspShmDWRD (D_SRS_LMT_OUTPUT_GAIN, u4Mode);
       DSP_SendDspTaskCmd(UOP_DSP_SRS_PURESOUND_FLAG);
}
void DSP_SRSLmtBoost(UINT32 u4Mode)
{
       //u4Mode = 0x20 * u4Mode;
       vWriteDspShmDWRD (D_SRS_LMT_BOOST, u4Mode);
       DSP_SendDspTaskCmd(UOP_DSP_SRS_PURESOUND_FLAG);
}
void DSP_SRSLmtLvl(UINT32 u4Mode)
{
       //u4Mode = 0x20C4 * u4Mode;
       vWriteDspShmDWRD (D_SRS_LMT_LEVEL, u4Mode);
       DSP_SendDspTaskCmd(UOP_DSP_SRS_PURESOUND_FLAG);
}
void DSP_SRSLmtDly(UINT32 u4Mode)
{
       vWriteDspShmDWRD (D_SRS_LMT_DELAY, u4Mode << 8);
       DSP_SendDspTaskCmd(UOP_DSP_SRS_PURESOUND_FLAG);
}
void DSP_SRSLmtCoeff(UINT32 u4Mode)
{
       vWriteDspShmDWRD (D_SRS_LMT_COEFF, u4Mode);
       DSP_SendDspTaskCmd(UOP_DSP_SRS_PURESOUND_FLAG);
}
void DSP_SRSLmtThresh(UINT32 u4Mode)
{
       vWriteDspShmDWRD (D_SRS_LMT_THRESH, u4Mode);
       DSP_SendDspTaskCmd(UOP_DSP_SRS_PURESOUND_FLAG);
}

void DSP_SRSAeqOn(UINT32 u4Mode)
{
	u4Mode |= (u4ReadDspShmDWRD (W_SRS_PURESOUND_FLAG) & 0xFFFFFDFF);
	vWriteDspShmWORD (W_SRS_PURESOUND_FLAG, u4Mode);
       DSP_SendDspTaskCmd(UOP_DSP_SRS_PURESOUND_FLAG);
}

void DSP_SRSAeqInputGain(UINT32 u4Mode) //sunman_lmt
{
       u4Mode *=  0x1999a;
       vWriteDspShmDWRD (D_SRS_AEQ_INPUT_GAIN, u4Mode);
       DSP_SendDspTaskCmd(UOP_DSP_SRS_PURESOUND_FLAG);
}
void DSP_SRSAeqOutputGain(UINT32 u4Mode)
{
       u4Mode *=  0x1999a ;
       vWriteDspShmDWRD (D_SRS_AEQ_OUTPUT_GAIN, u4Mode);
       DSP_SendDspTaskCmd(UOP_DSP_SRS_PURESOUND_FLAG);
}
void DSP_SRSAeqFilterSize(UINT32 u4Mode)
{
       vWriteDspShmDWRD (D_SRS_AEQ_FILTER_SIZE, u4Mode);
       DSP_SendDspTaskCmd(UOP_DSP_SRS_PURESOUND_FLAG);
}
void DSP_SRSPuresoundTrubsssOn(UINT32 u4Mode)
{
	u4Mode |= (u4ReadDspShmDWRD (W_SRS_PURESOUND_FLAG) & 0xFFFFEFFF);
	vWriteDspShmWORD (W_SRS_PURESOUND_FLAG, u4Mode);
       DSP_SendDspTaskCmd(UOP_DSP_SRS_PURESOUND_FLAG);
}

#endif

#ifdef DSP_SUPPORT_SRSVIQ
//sunman_viq
void DSP_SRSVIQRefLvl(UINT32 u4Mode)
{
       UINT16 u2UopIndex = UOP_DSP_AVC_CONTROL;
       vWriteDspShmDWRD (D_VIQ_REF_LVL, u4Mode);
       DSP_SendDspTaskCmd(u2UopIndex);
}
#ifdef VIQ_MODE_OLD
void DSP_SRSVIQMode(UINT32 u4Mode)
{
    UINT16 u2UopIndex = UOP_DSP_AVC_CONTROL;
    u4Mode=u4Mode<<8;
    vWriteDspShmDWRD (D_VIQ_MODE, u4Mode);
    DSP_SendDspTaskCmd(u2UopIndex);
}
#else
void DSP_SRSVIQMode(UINT32 u4Mode)
{
    UINT16 u2UopIndex = UOP_DSP_AVC_CONTROL;
    UINT32 u4Ori;

    u4Ori=u4ReadDspShmDWRD(D_VIQ_MODE);
    u4Ori = u4Ori & 0xffffcfff;
    u4Ori = u4Ori | (u4Mode<<12);
    vWriteDspShmDWRD (D_VIQ_MODE, u4Ori);
    DSP_SendDspTaskCmd(u2UopIndex);
}
#endif
void DSP_SRSVIQMaxGain(UINT32 u4Mode)
{
	UINT32 u4Mode1;
       UINT16 u2UopIndex = UOP_DSP_AVC_CONTROL;
       if (u4Mode==256)
           u4Mode1 = 0x7fffff;
       else
	    u4Mode1 = u4Mode << 15;
	vWriteDspShmDWRD (D_VIQ_MAX_GAIN, u4Mode1);
       DSP_SendDspTaskCmd(u2UopIndex);
}
void DSP_SRSVIQInputGain(UINT32 u4Mode)
{
       UINT16 u2UopIndex = UOP_DSP_AVC_CONTROL;
	vWriteDspShmDWRD (D_VIQ_INPUT_GAIN, u4Mode);
       DSP_SendDspTaskCmd(u2UopIndex);
}

void DSP_SRSVIQOutputGain(UINT32 u4Mode)
{
       UINT16 u2UopIndex = UOP_DSP_AVC_CONTROL;
	vWriteDspShmDWRD (D_VIQ_OUTPUT_GAIN, u4Mode);
       DSP_SendDspTaskCmd(u2UopIndex);
}

void DSP_SRSVIQNoiseTh(UINT8 u1Mode)
{
    UINT32 u4Mode1;
    u4Mode1 = 0x66666;
    u4Mode1 *= (UINT32)u1Mode;
    vWriteDspShmDWRD (D_VIQ_NOISE_TH, u4Mode1);
    vDspCmd(UOP_DSP_AVC_CONTROL); //do re-init.
}

#endif //DSP_SUPPORT_SRSVIQ

#ifdef DSP_SUPPORT_NPTV
/******************************************************************************
* Function      : void vReturnChannelSource()
* Description   :
* Parameters    :
* Return        : None
******************************************************************************/
TV_AUD_SYS_T DSP_ReturnChannelSource(void)
{
#ifdef SUPPORT_KOREA
    _eChannelSrc = SV_NTSC_M;
#endif
#ifdef SUPPORT_JAPAN
	_eChannelSrc = SV_FM_FM;
#endif
    return _eChannelSrc;
}


// *********************************************************************
// Function : DSP_AudioSetChannelSrc
// Description : set channel source type for demodulation function
// Parameter :
// Return    :
// *********************************************************************
void DSP_AudioSetChannelSrc(TV_AUD_SYS_T eTvSys)
{
	_eChannelSrc = eTvSys;
}

/******************************************************************************
* Function      : AUD_TYPE_T DSP_GetMtsSrcTyp()
* Description   :
* Parameters    :
* Return        : None
******************************************************************************/
AUD_TYPE_T DSP_GetMtsSrcTyp(UINT8 u1DecId)
{
	  return _aeAudType[u1DecId];
}

/******************************************************************************
* Function      : void vUpdateMtsSrcTyp()
* Description   :
* Parameters    :
* Return        : None
******************************************************************************/
static void vUpdateMtsSrcTyp(UINT8 u1Type)
{
	_rDspVars[AUD_DEC_MAIN].bMtsSrcTyp = u1Type;
}

static AUD_SOUND_MODE_T _rSoundMode = AUD_SOUND_MODE_UNKNOWN;

AUD_SOUND_MODE_T DSP_GetSoundMode(UINT8 u1DecId)
{
    UNUSED(u1DecId);
    return _rSoundMode;
}

void DSP_ClearSoundMode(UINT8 u1DecId)
{
    UNUSED(u1DecId);
    _rSoundMode = AUD_SOUND_MODE_UNKNOWN;
}

/******************************************************************************
* Function      : void vDspDemodDetModeNotify()
* Description   : Update the DSP Demodulation mode of notify info. for MW.
*                 Currently, only support Dec 1
* Parameters    : u4IntGroup, u4IntVector
* Return        : None
******************************************************************************/
void vDspDemodDetModeNotify(UINT32 u4IntGroup , UINT32 u4IntVector)
{
    AUD_DEC_INFO_T rAudDecInfo;
    UINT32         u4Tmp = 0;
    AudDecNfyFct   pfAudDecNfy = NULL;
    UINT8          u1DecId = AUD_DEC_AUX;

    // Get notify function
    VERIFY(_AUD_GetNotifyFunc(&pfAudDecNfy) == TRUE);

	// Fill the related information to rAudDecInfo
	rAudDecInfo.e_aud_fmt = (AUD_FMT_T)13;  // FIXME !! Hardcode here
	rAudDecInfo.ui4_sample_rate = 32;
	rAudDecInfo.ui1_bit_depth = 24;

    switch (u4IntGroup)
    {
    case INT_DSP_DETECTOR_NOTIFY:
        _rDspVars[AUD_DEC_MAIN].bDetectTyp = u4IntVector;
        rAudDecInfo.e_aud_type = AUD_TYPE_UNKNOWN;
        break;
    case INT_DSP_MINER_NOTIFY:
        switch (u4IntVector)
        {
        case MINER_CAPTURE_START:
           LOG(8, "Miner Start \n");
           break;
        case MINER_CAPTURE_DONE:
           LOG(8, "Miner Done \n");
           break;
        default: //exception
           break;
        }
        rAudDecInfo.e_aud_type = AUD_TYPE_UNKNOWN;
        break;
    case INT_DSP_PAL_DETECTED_MODE:
        switch (u4IntVector)
        {
        //Notify the detected NICAM modes for internal.
        case NICAM_DETECTED_STEREO:
            vUpdateMtsSrcTyp (NICAM_STEREO | FM_MONO );
            rAudDecInfo.e_aud_type = AUD_TYPE_FM_MONO_NICAM_STEREO;
            break;
        case NICAM_DETECTED_DUAL:
            vUpdateMtsSrcTyp ((NICAM_DUAL1 | NICAM_DUAL2 | FM_MONO));
            rAudDecInfo.e_aud_type = AUD_TYPE_FM_MONO_NICAM_DUAL;
            break;
        case NICAM_DETECTED_MONO:
            vUpdateMtsSrcTyp ((NICAM_MONO    | FM_MONO));
            rAudDecInfo.e_aud_type = AUD_TYPE_FM_MONO_NICAM_MONO;
            break;
        case PAL_FM_MONO:
            vUpdateMtsSrcTyp (FM_MONO);
            rAudDecInfo.e_aud_type = AUD_TYPE_MONO;
            break;
        default: //exception
            rAudDecInfo.e_aud_type = AUD_TYPE_MONO;
            break;
        }
        DSP_ClearSoundMode(AUD_DEC_AUX);
        break;
    case INT_DSP_MTS_DETECTED_MODE:
        rAudDecInfo.e_aud_fmt = AUD_FMT_MTS;
        switch (u4IntVector)
        {
        //Notify the detected MTS modes for internal.
        case MTS_DETECTED_STEREO_SAP:
            vUpdateMtsSrcTyp ((MTS_STEREO | MTS_SAP | MTS_MONO));
            rAudDecInfo.e_aud_type = AUD_TYPE_STEREO_SUB;
            _rSoundMode = (AUD_SOUND_MODE_T)(u2ReadDspShmWORD(W_USER_MODE)+1);
            break;
        case MTS_DETECTED_STEREO:
            vUpdateMtsSrcTyp ((MTS_STEREO | MTS_MONO));
            rAudDecInfo.e_aud_type = AUD_TYPE_STEREO;
            _rSoundMode = (AUD_SOUND_MODE_T)((u2ReadDspShmWORD(W_USER_MODE)& 0x0001)+1);
            if (_rDspVars[AUD_DEC_MAIN].bDspStrTyp == FMRDO_STREAM)
            {
                rAudDecInfo.e_aud_type = AUD_TYPE_FMRDO_STEREO;
            }
            break;
        case MTS_DETECTED_SAP:
            vUpdateMtsSrcTyp ((MTS_SAP    | MTS_MONO));
            rAudDecInfo.e_aud_type = AUD_TYPE_MONO_SUB;
            _rSoundMode = (AUD_SOUND_MODE_T)((u2ReadDspShmWORD(W_USER_MODE)& 0x0002)+1);
            break;
        case MTS_DETECTED_MONO:
            vUpdateMtsSrcTyp (MTS_MONO);
            rAudDecInfo.e_aud_type = AUD_TYPE_MONO;
            _rSoundMode = AUD_SOUND_MODE_MONO;
            if (_rDspVars[AUD_DEC_MAIN].bDspStrTyp == FMRDO_STREAM)
            {
                rAudDecInfo.e_aud_type = AUD_TYPE_FMRDO_MONO;
            }
            break;
        default: //exception
            rAudDecInfo.e_aud_type = AUD_TYPE_UNKNOWN;
            _rSoundMode = AUD_SOUND_MODE_UNKNOWN;
            break;
        }
        break;
    //case INT_DSP_PAL_DETECTED_MODE:
    // Notes that A2 and JPN share the same mode
    case INT_DSP_JPN_DETECTED_MODE:
        switch (u4IntVector)
        {
        case JPN_DETECTED_MONO:
	        if ((DSP_ReturnChannelSource () == SV_FM_FM) || (DSP_ReturnChannelSource () == SV_NTSC_M))
	        {
		        vUpdateMtsSrcTyp (MPX_MONO);
	        }
	        else
	        {
			    vUpdateMtsSrcTyp (FM_MONO);
	        }
	        rAudDecInfo.e_aud_type = AUD_TYPE_MONO;
            break;
        case JPN_DETECTED_STEREO:
	        if ((DSP_ReturnChannelSource () == SV_FM_FM) || (DSP_ReturnChannelSource () == SV_NTSC_M))
	        {
		        vUpdateMtsSrcTyp (MPX_STEREO | MPX_MONO);
	        }
	        else
	        {
		        vUpdateMtsSrcTyp (A2_STEREO | FM_MONO);
	        }
	        rAudDecInfo.e_aud_type = AUD_TYPE_STEREO;
	        break;
        case JPN_DETECTED_DUAL:
	        if ((DSP_ReturnChannelSource () == SV_FM_FM) || (DSP_ReturnChannelSource () ==  SV_NTSC_M))
	        {
				vUpdateMtsSrcTyp (MPX_MAIN | MPX_SUB);
	        }
	        else
	        {
				vUpdateMtsSrcTyp (A2_DUAL1 | A2_DUAL2);
	        }
	        rAudDecInfo.e_aud_type = AUD_TYPE_DUAL_MONO;
	        break;
	    default: //exception
	        rAudDecInfo.e_aud_type = AUD_TYPE_MONO;
	        break;
        }
        DSP_ClearSoundMode(AUD_DEC_AUX);
        break;
    default:
        rAudDecInfo.e_aud_type = AUD_TYPE_UNKNOWN;
        break;
    }

    _aeAudType[AUD_DEC_MAIN] = rAudDecInfo.e_aud_type;
    _aeAudType[AUD_DEC_AUX] = rAudDecInfo.e_aud_type;

    // work around!!!
    if (_rDspVars[AUD_DEC_MAIN].bDspStrTyp == FMRDO_STREAM)
    {
        u1DecId = AUD_DEC_MAIN;
        LOG (1, "Transfer ID to MAIN for FM radio!\n");
    }
#ifndef CC_MT5391_AUD_3_DECODER
    if (AUD_GetTargetCountry() != COUNTRY_EU)
    {
        // if not in 3 decoder architecture, and country code is not EU, means ATV is in Dec0.
        u1DecId = AUD_DEC_MAIN;
    }
#endif

    if (pfAudDecNfy != NULL)
    {
        pfAudDecNfy((void *)AUD_NFY_STREAM, u1DecId, AUD_COND_AUD_INFO_CHG,
                    (UINT32)&rAudDecInfo, u4Tmp);
#ifdef CC_MT5391_AUD_3_DECODER
        //Under 3 decoder architecture, if input source is ATV, then any detected information
        // of PAL_DEC2 or A2_DEC2 should be notified to MW for MTS button control.
       if ( (_IsDualDecMode()&&(_rDspVars[AUD_DEC_MAIN].bDspStrTyp == PCM_STREAM)&&((_rDspVars[AUD_DEC_AUX].bDspStrTyp == PAL_STREAM) || (_rDspVars[AUD_DEC_AUX].bDspStrTyp == A2_STREAM) || (_rDspVars[AUD_DEC_AUX].bDspStrTyp == NTSC_STREAM)))
          || ((_fgEuroCanalPlusMode) && (_rDspVars[AUD_DEC_MAIN].bDspStrTyp == PCM_STREAM) && (_rDspVars[AUD_DEC_AUX].bDspStrTyp == PAL_STREAM)) )
       {
            pfAudDecNfy((void *)AUD_NFY_STREAM, AUD_DEC_MAIN, AUD_COND_AUD_INFO_CHG,
                    (UINT32)&rAudDecInfo, u4Tmp);
            LOG(5, "ATV Source Dec1 (PAL/A2 Dec2) sound mode change notify\n");
       }
#endif
    }

	LOG(5, "Detect audio mode (0x%x, 0x%x) => %s\n", u4IntGroup, u4IntVector, _aszAudOutputMode[rAudDecInfo.e_aud_type]);

    // To suppress warning message in release mode
	UNUSED(_aszAudOutputMode);
}

void DSP_DualDecAtvNotifyChlInfor(void)
{
    AudDecNfyFct   pfAudDecNfy = NULL;
    UINT8          u1DecId = AUD_DEC_AUX;
    AUD_DEC_INFO_T rAudDecInfo;

    // Get notify function
    VERIFY(_AUD_GetNotifyFunc(&pfAudDecNfy) == TRUE);

	// Fill the related information to rAudDecInfo
    rAudDecInfo.e_aud_fmt = (AUD_FMT_T)13;  // FIXME !! Hardcode here
	rAudDecInfo.ui4_sample_rate = 32;
	rAudDecInfo.ui1_bit_depth = 24;
    rAudDecInfo.e_aud_type = _aeAudType[AUD_DEC_AUX];

    if (pfAudDecNfy)
    {
        pfAudDecNfy((void *)AUD_NFY_STREAM, u1DecId, AUD_COND_AUD_INFO_CHG,
                    (UINT32)&rAudDecInfo,AUD_OK);
    }

    LOG(5, "Detect audio mode (0xff, 0xff) => %s\n", _aszAudOutputMode[rAudDecInfo.e_aud_type]);
}

AUD_TYPE_T DSP_GetAudChlInfo(UINT8 u1DecId)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

    return _aeAudType[u1DecId];
}

/******************************************************************************
* Function      : void DSP_UpdateDemodMode()
* Description   : Update DSP Demodulation mode.
                  Serve DSP_SetDemodOutputMode()
* Parameters    : u2UserMode, u2SourceMode, u2AdcGain
* Return        : None
******************************************************************************/
static void DSP_UpdateDemodMode (UINT16 u2UserMode, UINT16 u2SourceMode,
                                      UINT16 u2AdcGain)
{
    if ((u2UserMode      != u2ReadDspShmWORD (W_USER_MODE))  ||
        (u2SourceMode    != u2ReadDspShmWORD (W_SOURCE_MODE))||
        (u2AdcGain       != u2ReadDspShmWORD (W_ADC_GAIN)))
    {
        vWriteDspShmWORD (W_USER_MODE,     u2UserMode);
        vWriteDspShmWORD (W_SOURCE_MODE,   u2SourceMode);
        vWriteDspShmWORD (W_ADC_GAIN,      u2AdcGain);

        vDspCmd (UOP_DSP_DEMOD_CONFIG);
    }
}

// Note that DSP cannot distinguish from PAL_DK/ SECAM-L / SECAM-PLUM
// So set TV src mode before DSP performing detection ..
void DSP_SetDetTvSrcMode(TV_AUD_SYS_T eAudSys)
{
    switch (eAudSys)
    {
        case SV_PAL_DK:
            _eDetTvSrcMode = SV_PAL_DK;
		    break;
        case SV_SECAM_L:
            _eDetTvSrcMode = SV_SECAM_L;
            break;
        case SV_SECAM_L_PLUM:
            _eDetTvSrcMode = SV_SECAM_L_PLUM;
            break;
        default:
            _eDetTvSrcMode = SV_PAL_DK;
            break;
    }
}
//lint -e{715} suppress "eAudSys not referenced"
TV_AUD_SYS_T DSP_GetDetTvSrcMode(TV_AUD_SYS_T eAudSys)
{
    return _eDetTvSrcMode;
}


TV_AUD_SYS_T DSP_GetTvSrcMode (void)
{
    return _eChannelSrc;
}

void DSP_SetTvSrcMode (TV_AUD_SYS_T eAudSys)
{
	UINT16 u2SrcMode;

       if  (_eChannelSrc != eAudSys)
       {
           _guAudDemodTypeChange = 1;
       }
       else
       {
           _guAudDemodTypeChange = 0;
       }
	_eChannelSrc = eAudSys;

    LOG(8, "DSP_SetTvSrcMode (%s)\n", _aszTvSysMode[_eChannelSrc]);

	switch (eAudSys)
	{
		case SV_A2_BG:
			u2SrcMode = (UINT16)A2_SRC_MODE_BG;
			break;
		case SV_A2_DK1:
			u2SrcMode = (UINT16)A2_SRC_MODE_DK1;
			break;
		case SV_A2_DK2:
			u2SrcMode = (UINT16)A2_SRC_MODE_DK2;
			break;
		case SV_A2_DK3:
			u2SrcMode = (UINT16)A2_SRC_MODE_DK3;
			break;
		case SV_PAL_I:
			u2SrcMode = (UINT16)PAL_SRC_MODE_I;
			break;
		case SV_PAL_BG:
			u2SrcMode = (UINT16)PAL_SRC_MODE_BG;
			break;
		case SV_PAL_DK:
			u2SrcMode = (UINT16)PAL_SRC_MODE_DK;
			break;
		case SV_SECAM_L:
			u2SrcMode = (UINT16)PAL_SRC_MODE_SECAML;
			break;
        case SV_NTSC_M:
            u2SrcMode = (UINT16)A2_SRC_MODE_M;
            break;
        case SV_MTS:
            u2SrcMode = (UINT16)A2_SRC_MODE_M; //MTS should be M also.
            break;
		default:
			u2SrcMode = 0;

	}
#ifdef CC_AUD_TV_SYS_FINE_TUNE_VOL
	AUD_SetTVSysFineTuneVol(eAudSys);
#endif
	DSP_UpdateDemodMode (0, u2SrcMode,u2ReadDspShmWORD (W_ADC_GAIN));
}

/******************************************************************************
* Function      : void DSP_SetDemodOutputMode()
* Description   : Set MTS output mode.
* Parameters    : u1Output
*    SV_MONO  (0) - Output (M,M)
*    SV_STEREO(1) - If stereo exists, Output (L,R) otherwise (M,M)
*    SV_SAP   (2) - If SAP exists, Output (S,S) otherwise (L,R) otherwise (M,M)
* Return        : None
******************************************************************************/
void DSP_SetDemodOutputMode (UINT8 u1Output)
{
#if 0//SUPPORT_TUNER_INTERNAL_MTS
    vDrvTunerSetMTS (u1Output);
#else
	if (DSP_ReturnChannelSource() >= SV_PAL_I)
	{
		if (u1Output >= SV_NICAM_MONO)
		{
			return;
		}
		// < SV_NICAM_MONO ==>
		// SV_FM_MONO
		// SV_A2_STEREO
		// SV_A2_DUAL1
		// SV_A2_DUAL2
	}
	else
	{
		// < PAL_I ==>
		// SV_MTS
		// SV_FM_FM
		// SV_NTSC_M
		// SV_A2_BG
		// SV_A2_DK1
		// SV_A2_DK2
		// SV_A2_DK3
		if ((DSP_ReturnChannelSource() == SV_FM_FM) || // Japan
			(DSP_ReturnChannelSource() == SV_NTSC_M)) // Korea
		{
			//SV_MPX_MONO,		// 3
			//SV_MPX_STEREO,	// 4
			//SV_MPX_SUB,		// 5
			//SV_MPX_MAIN_SUB,	// 6
			//SV_MPX_MAIN,		// 7
			DSP_UpdateDemodMode ((UINT16) u1Output, u2ReadDspShmWORD (W_SOURCE_MODE),
														u2ReadDspShmWORD (W_ADC_GAIN));
		}
		else
		{
			//SV_MONO,		// 0
			//SV_STEREO,	// 1
			//SV_SAP,		// 2

			//SV_FM_MONO,	// 0
			//SV_A2_STEREO, // 1
			//SV_A2_DUAL1,	// 2
			//SV_A2_DUAL2,	// 3
			DSP_UpdateDemodMode ((UINT16) u1Output, u2ReadDspShmWORD (W_SOURCE_MODE),
														u2ReadDspShmWORD (W_ADC_GAIN));
		}
	}
#endif
    return;
}

// *********************************************************************
// Function : void vApiAudioDEMOD_PAL() large
// Description : demodulation related configuration
// Parameter :
// Return    :
// *********************************************************************
static void DSP_UpdateDemodMode_PAL (UINT16 u2UserMode, UINT16 u2AdcGain, UINT16 u2EnforcedMode)
{
	//#if SUPPORT_PAL_SYSTEM
	if ((u2UserMode != u2ReadDspShmWORD (W_USER_MODE)) ||
		(u2AdcGain != u2ReadDspShmWORD (W_ADC_GAIN)) ||
		(u2EnforcedMode != u2ReadDspShmWORD (W_ENFORCED_MODE)))
	{
		vWriteDspShmWORD (W_USER_MODE, u2UserMode);
		vWriteDspShmWORD (W_ADC_GAIN, u2AdcGain);
		vWriteDspShmWORD (W_ENFORCED_MODE, u2EnforcedMode);

		vDspCmd (UOP_DSP_DEMOD_CONFIG);
	}
	//#endif //SUPPORT_PAL_SYSTEM
//#if !SUPPORT_PAL_SYSTEM
	UNUSED(u2UserMode);
	UNUSED(u2AdcGain);
	UNUSED(u2EnforcedMode);
//#endif
}

// *********************************************************************
// Function : void DSP_SetPALOutput() large
// Description : demodulation related configuration
// Parameter :
// Return    :
// *********************************************************************
void DSP_SetPALOutput (UINT8 u1UserMode, UINT8 u1EnforcedMode)
{
	//#if SUPPORT_PAL_SYSTEM
	DSP_UpdateDemodMode_PAL ((UINT16) u1UserMode, 0x8006, (UINT16) u1EnforcedMode);
	//#endif //SUPPORT_PAL_SYSTEM
//#if !SUPPORT_PAL_SYSTEM
	UNUSED(u1UserMode);
	UNUSED(u1EnforcedMode);
//#endif
}

//********************************************************************
//   Description:
//     Set fine tune volume for PAL FM signal
//  Parameters:
//     bValue	0 - 40. default: 20
//     bValue = 0 , -10 dB
//     ......
//     bValue = 18 , -1 dB
//     bValue = 19 , -0.5 dB
//     bValue = 20 , 0 dB
//     bValue = 21 , 0.5 dB
//     bValue = 22 , 1 dB
//     ......
//     bValue = 40 , 10 dB
//***********************************************************************
void DSP_SetPALFineVolume (UINT8 u1Value)
{
	if (VOLUME_FINE_TUNE[u1Value] != (u4ReadDspShmDWRD (D_PAL_FINE_TUNE_VOLUME) >> 8))
	{
		vWriteDspShmDWRD(D_PAL_FINE_TUNE_VOLUME, ((UINT32) VOLUME_FINE_TUNE[u1Value] << 8));
		vDspCmd (UOP_DSP_DEMOD_FINE_TUNE_VOLUME);
	}
}

//********************************************************************
//   Description:
//     Set fine tune volume for Nicam signal
//  Parameters:
//     bValue	0 - 40. default: 20
//     bValue = 0 , -10 dB
//     ......
//     bValue = 18 , -1 dB
//     bValue = 19 , -0.5 dB
//     bValue = 20 , 0 dB
//     bValue = 21 , 0.5 dB
//     bValue = 22 , 1 dB
//     ......
//     bValue = 40 , 10 dB
//***********************************************************************
void DSP_SetNICAMFineVolume (UINT8 u1Value)
{
	if (VOLUME_FINE_TUNE[u1Value] != (u4ReadDspShmDWRD (D_NICAM_FINE_TUNE_VOLUME) >> 8))
	{
		vWriteDspShmDWRD(D_NICAM_FINE_TUNE_VOLUME, ((UINT32) VOLUME_FINE_TUNE[u1Value] << 8));
		vDspCmd (UOP_DSP_DEMOD_FINE_TUNE_VOLUME);
	}
}

//********************************************************************
//   Description:
//     Set fine tune volume for AM signal
//  Parameters:
//     bValue	0 - 40. default: 20
//     bValue = 0 , -10 dB
//     ......
//     bValue = 18 , -1 dB
//     bValue = 19 , -0.5 dB
//     bValue = 20 , 0 dB
//     bValue = 21 , 0.5 dB
//     bValue = 22 , 1 dB
//     ......
//     bValue = 40 , 10 dB
//***********************************************************************
void DSP_SetAMFineVolume (UINT8 u1Value)
{
	if (VOLUME_FINE_TUNE[u1Value] != (u4ReadDspShmDWRD (D_AM_FINE_TUNE_VOLUME) >> 8))
	{
		vWriteDspShmDWRD(D_AM_FINE_TUNE_VOLUME, ((UINT32) VOLUME_FINE_TUNE[u1Value] << 8));
		vDspCmd (UOP_DSP_DEMOD_FINE_TUNE_VOLUME);
	}
}

//********************************************************************
//   Description:
//     Set fine tune volume for AM signal
//  Parameters:
//     bValue	0 - 40. default: 20
//     bValue = 0 , -10 dB
//     ......
//     bValue = 18 , -1 dB
//     bValue = 19 , -0.5 dB
//     bValue = 20 , 0 dB
//     bValue = 21 , 0.5 dB
//     bValue = 22 , 1 dB
//     ......
//     bValue = 40 , 10 dB
//***********************************************************************
void DSP_SetA2FineVolume (UINT8 u1Value)
{
	if (VOLUME_FINE_TUNE[u1Value] != (u4ReadDspShmDWRD (D_A2_FINE_TUNE_VOLUME) >> 8))
	{
		vWriteDspShmDWRD(D_A2_FINE_TUNE_VOLUME, ((UINT32) VOLUME_FINE_TUNE[u1Value] << 8));
		vDspCmd (UOP_DSP_DEMOD_FINE_TUNE_VOLUME);
	}
}

//********************************************************************
//   Description:
//     Set fine tune volume for PAL FM signal
//  Parameters:
//     bValue	0 - 40. default: 20
//     bValue = 0 , -10 dB
//     ......
//     bValue = 18 , -1 dB
//     bValue = 19 , -0.5 dB
//     bValue = 20 , 0 dB
//     bValue = 21 , 0.5 dB
//     bValue = 22 , 1 dB
//     ......
//     bValue = 40 , 10 dB
//***********************************************************************
void DSP_SetMTSFineVolume (UINT8 u1Value)
{
	if (VOLUME_FINE_TUNE[u1Value] != (u4ReadDspShmDWRD (D_MTS_FINE_TUNE_VOLUME) >> 8))
	{
		vWriteDspShmDWRD(D_MTS_FINE_TUNE_VOLUME, ((UINT32) VOLUME_FINE_TUNE[u1Value] << 8));
		vDspCmd (UOP_DSP_DEMOD_FINE_TUNE_VOLUME);
	}
}

//********************************************************************
//   Description:
//     Set fine tune volume for PAL FM signal
//  Parameters:
//     bValue	0 - 40. default: 20
//     bValue = 0 , -10 dB
//     ......
//     bValue = 18 , -1 dB
//     bValue = 19 , -0.5 dB
//     bValue = 20 , 0 dB
//     bValue = 21 , 0.5 dB
//     bValue = 22 , 1 dB
//     ......
//     bValue = 40 , 10 dB
//***********************************************************************
void DSP_SetSAPFineVolume (UINT8 u1Value)
{
	if (VOLUME_FINE_TUNE[u1Value] != (u4ReadDspShmDWRD (D_SAP_FINE_TUNE_VOLUME) >> 8))
	{
		vWriteDspShmDWRD(D_SAP_FINE_TUNE_VOLUME, ((UINT32) VOLUME_FINE_TUNE[u1Value] << 8));
		vDspCmd (UOP_DSP_DEMOD_FINE_TUNE_VOLUME);
	}
}

//********************************************************************
//   Description:
//     Set fine tune volume for PAL FM signal
//  Parameters:
//     bValue	0 - 40. default: 20
//     bValue = 0 , -10 dB
//     ......
//     bValue = 18 , -1 dB
//     bValue = 19 , -0.5 dB
//     bValue = 20 , 0 dB
//     bValue = 21 , 0.5 dB
//     bValue = 22 , 1 dB
//     ......
//     bValue = 40 , 10 dB
//***********************************************************************
void DSP_SetFmfmMonoFineVolume (UINT8 u1Value)
{
	if (VOLUME_FINE_TUNE[u1Value] != (u4ReadDspShmDWRD (D_FMFM_MONO_FINE_TUNE_VOLUME) >> 8))
	{
		vWriteDspShmDWRD(D_FMFM_MONO_FINE_TUNE_VOLUME, ((UINT32) VOLUME_FINE_TUNE[u1Value] << 8));
		vDspCmd (UOP_DSP_DEMOD_FINE_TUNE_VOLUME);
	}
}

//********************************************************************
//   Description:
//     Set fine tune volume for PAL FM signal
//  Parameters:
//     bValue	0 - 40. default: 20
//     bValue = 0 , -10 dB
//     ......
//     bValue = 18 , -1 dB
//     bValue = 19 , -0.5 dB
//     bValue = 20 , 0 dB
//     bValue = 21 , 0.5 dB
//     bValue = 22 , 1 dB
//     ......
//     bValue = 40 , 10 dB
//***********************************************************************
void DSP_SetFmfmDualFineVolume (UINT8 u1Value)
{
	if (VOLUME_FINE_TUNE[u1Value] != (u4ReadDspShmDWRD (D_FMFM_DUAL_FINE_TUNE_VOLUME) >> 8))
	{
		vWriteDspShmDWRD(D_FMFM_DUAL_FINE_TUNE_VOLUME, ((UINT32) VOLUME_FINE_TUNE[u1Value] << 8));
		vDspCmd (UOP_DSP_DEMOD_FINE_TUNE_VOLUME);
	}
}

//********************************************************************
//   Description:
//     Set fine tune volume for FM radio signal
//  Parameters:
//     bValue	0 - 40. default: 20
//     bValue = 0 , -10 dB
//     ......
//     bValue = 18 , -1 dB
//     bValue = 19 , -0.5 dB
//     bValue = 20 , 0 dB
//     bValue = 21 , 0.5 dB
//     bValue = 22 , 1 dB
//     ......
//     bValue = 40 , 10 dB
//***********************************************************************
void DSP_SetFmRadioFineVolume (UINT8 u1Value)
{
	if (VOLUME_FINE_TUNE[u1Value] != (u4ReadDspShmDWRD (D_FMRADIO_FINE_TUNE_VOLUME) >> 8))
	{
		vWriteDspShmDWRD(D_FMRADIO_FINE_TUNE_VOLUME, ((UINT32) VOLUME_FINE_TUNE[u1Value] << 8));
		vDspCmd (UOP_DSP_DEMOD_FINE_TUNE_VOLUME);
	}
}

void DSP_SetAtvOutputMode(UINT8 u1DecId, AUD_SOUND_MODE_T eSoundMode)
{
	UNUSED(u1DecId);

    //_rSoundMode = eSoundMode;

	switch (GET_DSP_DECODER_TYPE(u1DecId))
	{
	case NTSC_STREAM:
		//SV_MONO,		// 0
		//SV_STEREO,	// 1
		//SV_SAP,		// 2
		if (eSoundMode == AUD_SOUND_MODE_MONO)
		{
		       _rSoundMode = eSoundMode;
			DSP_UpdateDemodMode ((UINT16) 0, u2ReadDspShmWORD (W_SOURCE_MODE),
														u2ReadDspShmWORD (W_ADC_GAIN));

		}
		else if (eSoundMode == AUD_SOUND_MODE_STEREO)
		{
		       _rSoundMode = eSoundMode;
			DSP_UpdateDemodMode ((UINT16) 1, u2ReadDspShmWORD (W_SOURCE_MODE),
														u2ReadDspShmWORD (W_ADC_GAIN));

		}
		else if (eSoundMode == AUD_SOUND_MODE_SUB_LANG)
		{
		       _rSoundMode = eSoundMode;
			DSP_UpdateDemodMode ((UINT16) 2, u2ReadDspShmWORD (W_SOURCE_MODE),
														u2ReadDspShmWORD (W_ADC_GAIN));

		}
		else
		{
			LOG(1, "Error MTS sound mode\n");
		}
		break;
	case PAL_STREAM:
		//                      user_mode                  enforced_mode
		//  fm  mono  =>           0                             1
		//  nicam_MONO =>          0                             0
		//  nicam_stereo =>        0                             0
		//  nicam_dual 1 =>        1                             0
		//  nicam_dual 2 =>        2                             0
		//  nicam_dual 1 + 2 =>    4                             0
		if (eSoundMode == AUD_SOUND_MODE_MONO)
		{
		       _rSoundMode = eSoundMode;
			DSP_UpdateDemodMode_PAL((UINT16) 0, u2ReadDspShmWORD (W_ADC_GAIN), 1);
		}
		else if (eSoundMode == AUD_SOUND_MODE_NICAM_MONO)
		{
		       _rSoundMode = eSoundMode;
			DSP_UpdateDemodMode_PAL((UINT16) 0, u2ReadDspShmWORD (W_ADC_GAIN), 0);
		}
		else if (eSoundMode == AUD_SOUND_MODE_NICAM_STEREO)
		{
		       _rSoundMode = eSoundMode;
			DSP_UpdateDemodMode_PAL((UINT16) 0, u2ReadDspShmWORD (W_ADC_GAIN), 0);
		}
		else if (eSoundMode == AUD_SOUND_MODE_NICAM_DUAL1)
		{
		       _rSoundMode = eSoundMode;
			DSP_UpdateDemodMode_PAL((UINT16) 1, u2ReadDspShmWORD (W_ADC_GAIN), 0);
		}
		else if (eSoundMode == AUD_SOUND_MODE_NICAM_DUAL2)
		{
		       _rSoundMode = eSoundMode;
			DSP_UpdateDemodMode_PAL((UINT16) 2, u2ReadDspShmWORD (W_ADC_GAIN), 0);
		}
		else if (eSoundMode == AUD_SOUND_MODE_NICAM_DUAL1_DUAL2)
		{
		       _rSoundMode = eSoundMode;
			DSP_UpdateDemodMode_PAL((UINT16) 4, u2ReadDspShmWORD (W_ADC_GAIN), 0);
		}
		else
		{
			LOG(1, "Error NICAM sound mode\n");
		}
		break;
	case A2_STREAM:
		//SV_FM_MONO,	// 0
		//SV_A2_STEREO, // 1
		//SV_A2_DUAL1,	// 0
		//SV_A2_DUAL2,	// 2
		//SV_A2_DUAL1_DUAL2, //4
		if (eSoundMode == AUD_SOUND_MODE_MONO)
		{
		       _rSoundMode = eSoundMode;
			DSP_UpdateDemodMode ((UINT16) 0, u2ReadDspShmWORD (W_SOURCE_MODE),
														u2ReadDspShmWORD (W_ADC_GAIN));
		}
		else if (eSoundMode == AUD_SOUND_MODE_STEREO)
		{
		       _rSoundMode = eSoundMode;
			DSP_UpdateDemodMode ((UINT16) 1, u2ReadDspShmWORD (W_SOURCE_MODE),
														u2ReadDspShmWORD (W_ADC_GAIN));
		}
		else if (eSoundMode == AUD_SOUND_MODE_DUAL1)
		{
		       _rSoundMode = eSoundMode;
			DSP_UpdateDemodMode ((UINT16) 0, u2ReadDspShmWORD (W_SOURCE_MODE),
														u2ReadDspShmWORD (W_ADC_GAIN));
		}
		else if (eSoundMode == AUD_SOUND_MODE_DUAL2)
		{
		       _rSoundMode = eSoundMode;
			DSP_UpdateDemodMode ((UINT16) 2, u2ReadDspShmWORD (W_SOURCE_MODE),
														u2ReadDspShmWORD (W_ADC_GAIN));
		}
		else if (eSoundMode == AUD_SOUND_MODE_A2_DUAL1_DUAL2)
		{
		       _rSoundMode = eSoundMode;
			DSP_UpdateDemodMode ((UINT16) 4, u2ReadDspShmWORD (W_SOURCE_MODE),
														u2ReadDspShmWORD (W_ADC_GAIN));
		}
		else
		{
			LOG(1, "Error A2 sound mode, eSoundMode = d\n", eSoundMode);
		}
		break;
	case FMFM_STREAM: // Japan;
	//case NTSC_M: // Korea  // TODO
		//SV_MPX_MONO,		// 3
		//SV_MPX_STEREO,	// 4
		//SV_MPX_SUB,		// 5
		//SV_MPX_MAIN_SUB,	// 6
		//SV_MPX_MAIN,		// 7
		if (eSoundMode == AUD_SOUND_MODE_MONO)
		{
		       _rSoundMode = eSoundMode;
			DSP_UpdateDemodMode ((UINT16) 3, u2ReadDspShmWORD (W_SOURCE_MODE),
														u2ReadDspShmWORD (W_ADC_GAIN));
		}
		else if (eSoundMode == AUD_SOUND_MODE_STEREO)
		{
		       _rSoundMode = eSoundMode;
			DSP_UpdateDemodMode ((UINT16) 4, u2ReadDspShmWORD (W_SOURCE_MODE),
														u2ReadDspShmWORD (W_ADC_GAIN));
		}
		else if (eSoundMode == AUD_SOUND_MODE_SUB_LANG)
		{
		       _rSoundMode = eSoundMode;
			DSP_UpdateDemodMode ((UINT16) 5, u2ReadDspShmWORD (W_SOURCE_MODE),
														u2ReadDspShmWORD (W_ADC_GAIN));
		}
		else
		{
			// TODO !!  for SV_MPX_MAIN_SUB/SV_MPX_MAIN
			LOG(1, "Error FMFM/NTSC_M sound mode\n");
		}
		break;
	case FMRDO_STREAM:
	case FMRDODET_STREAM:
		//SV_MONO,		// 0
		//SV_STEREO,	// 1
		if (eSoundMode == AUD_SOUND_MODE_FM_MONO)
		{
		       _rSoundMode = eSoundMode;
			DSP_UpdateDemodMode ((UINT16) 0, u2ReadDspShmWORD (W_SOURCE_MODE),
														u2ReadDspShmWORD (W_ADC_GAIN));

		}
		else if (eSoundMode == AUD_SOUND_MODE_FM_STEREO)
		{
		       _rSoundMode = eSoundMode;
			DSP_UpdateDemodMode ((UINT16) 1, u2ReadDspShmWORD (W_SOURCE_MODE),
														u2ReadDspShmWORD (W_ADC_GAIN));

		}
		else
		{
			_rSoundMode = eSoundMode;
			DSP_UpdateDemodMode ((UINT16) 1, u2ReadDspShmWORD (W_SOURCE_MODE),
														u2ReadDspShmWORD (W_ADC_GAIN));
			LOG(1, "Error FM sound mode: set default as Stereo\n");
		}
		break;
	default:
	//case NTSC_STREAM:
		//SV_MONO,		// 0
		//SV_STEREO,	// 1
		//SV_SAP,		// 2
		if (eSoundMode == AUD_SOUND_MODE_MONO)
		{
		       _rSoundMode = eSoundMode;
			DSP_UpdateDemodMode ((UINT16) 0, u2ReadDspShmWORD (W_SOURCE_MODE),
														u2ReadDspShmWORD (W_ADC_GAIN));

		}
		else if (eSoundMode == AUD_SOUND_MODE_STEREO)
		{
		       _rSoundMode = eSoundMode;
			DSP_UpdateDemodMode ((UINT16) 1, u2ReadDspShmWORD (W_SOURCE_MODE),
														u2ReadDspShmWORD (W_ADC_GAIN));

		}
		else if (eSoundMode == AUD_SOUND_MODE_SUB_LANG)
		{
		       _rSoundMode = eSoundMode;
			DSP_UpdateDemodMode ((UINT16) 2, u2ReadDspShmWORD (W_SOURCE_MODE),
														u2ReadDspShmWORD (W_ADC_GAIN));

		}
		else
		{
			LOG(5, "Error MTS sound mode\n");
		}
		break;
	}
}

#endif

UINT8 DSP_GetAcmod(UINT8 u1DecId)
{
    UINT8 u1Acmode;

    DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_DATA, TYPE_COMM_DRAM_IDX);
    u1Acmode = (u1DecId == AUD_DEC_MAIN) ? (dReadDspCommDram(ADDR_D2RC_AC3_ACMOD)>>8) :
    	       (dReadDspCommDram(ADDR_D2RC_AC3_ACMOD_DEC2)>>8);
    LOG(3, " DSP_GetAcmod: u1Acmode = 0x%x \n", u1Acmode);
    return u1Acmode;
}

UINT8 DSP_GetAc3DecType(UINT8 u1DecId)
{
    UINT8 u1Ac3DecType;

    DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_DATA, TYPE_COMM_DRAM_IDX);
    u1Ac3DecType = (u1DecId == AUD_DEC_MAIN) ? (dReadDspCommDram(ADDR_D2RC_AC3_DECTYPE)>>8) :
    	       (dReadDspCommDram(ADDR_D2RC_AC3_DECTYPE_DEC2)>>8);
    LOG(3, " DSP_GetAc3DecType: u1Ac3DecType = 0x%x \n", u1Ac3DecType);
    return u1Ac3DecType;
}


void DSP_SetStartPtsToShm(UINT8 u1DecId, UINT32 u4Pts, UINT32 u4Addr)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

#ifdef CC_AUD_4_DECODER_SUPPORT
    if (u1DecId == AUD_DEC_4TH)
    {
        // Dec4 doesn't need to do this
        return;
    }
#endif

    if (u1DecId == AUD_DEC_MAIN)
    {
        vWriteShmUINT32(D_1ST_PTS_PRS_PNT, u4Addr);
        vWriteShmUINT16(W_1ST_PTS_STCH, (UINT16)((u4Pts & 0xffff0000) >> 16));
        vWriteShmUINT16(W_1ST_PTS_STCL, (UINT16)(u4Pts & 0x0000ffff));
#ifdef DATA_DISC_WMA_SUPPORT
        //workaround for WMA trick mode pull STC
        if (u4Pts !=0)
        {
            //from middle of file, set wma_header_source bit15
            vWriteShmUINT16(W_WMA_HEADER_SOURCE, u2ReadShmUINT16(W_WMA_HEADER_SOURCE)| 0x8000);
        }
#endif
    }
#ifdef CC_MT5391_AUD_3_DECODER
    else if (u1DecId == AUD_DEC_AUX)
#else
    else
#endif
    {
        vWriteShmUINT32(D_1ST_PTS_PRS_PNT_DEC2, u4Addr);
        vWriteShmUINT16(W_1ST_PTS_STCH_DEC2, (UINT16)((u4Pts & 0xffff0000) >> 16));
        vWriteShmUINT16(W_1ST_PTS_STCL_DEC2, (UINT16)(u4Pts & 0x0000ffff));
    }
#ifdef CC_MT5391_AUD_3_DECODER
    else
    {
        vWriteShmUINT32(D_1ST_PTS_PRS_PNT_DEC3, u4Addr);
        vWriteShmUINT16(W_1ST_PTS_STCH_DEC3, (UINT16)((u4Pts & 0xffff0000) >> 16));
        vWriteShmUINT16(W_1ST_PTS_STCL_DEC3, (UINT16)(u4Pts & 0x0000ffff));
    }
#endif
}

/***************************************************************************
     Function : vDspSetStcDiff
  Description : STC difference setting
                Note: set the difference before play
    Parameter :
                u2StcWorst  : STC Difference worst bound for system master in ms
    Return    : None
***************************************************************************/
void DSP_SetStcDiff(UCHAR ucDecId, UINT16 u2StcWorst)
{
    AUD_DEC_ID_VALIDATE_2(ucDecId);

    if (ucDecId == AUD_DEC_MAIN)
    {
        vWriteShmUINT32(D_STC_DIFF_WS, u2StcWorst*90);
    }
    else /* (ucDecId == AUD_DEC_AUX) */
    {
        vWriteShmUINT32(D_STC_DIFF_WS_DEC2, u2StcWorst*90);
    }
}

void DSP_SetAudLogPeriod(UINT32 u4Period)
{
#ifdef AUD_STATISTICS
	SetAudLogPeriod(u4Period);
#else
	UNUSED(u4Period);
#endif
}

UINT32 DSP_GetEQSpectrumAddr(UINT8 u1DecId)
{
    UNUSED(u1DecId);

    //if (u1DecId == AUD_DEC_MAIN) //EQ Spectrum for decoder2 is not ready...
    {
        return (_u4DspBuf + (ADDR_D2RC_SPECTRUM_BASE<<2));
    }
}

//Andrew Wen 2007/2/2 for AC3 Data Rate Test
void DSP_SetStcDiffBound(UINT8 uStcDiffLo, UINT8 uStcDiffHi, UINT8 uStcDiffWs)
{
    _uStcDiffBound[AUD_STC_LO] = uStcDiffLo;
    _uStcDiffBound[AUD_STC_HI] = uStcDiffHi;
    _uStcDiffBound[AUD_STC_WS] = uStcDiffWs;

    vWriteShmUINT32(D_STC_DIFF_LO, STC_DIFF_BOUND_032MS * (1+uStcDiffLo));
    vWriteShmUINT32(D_STC_DIFF_HI, STC_DIFF_BOUND_064MS * 2 * (1+uStcDiffHi));
    vWriteShmUINT32(D_STC_DIFF_WS, STC_DIFF_BOUND_500MS * (1+uStcDiffWs)) ;
}

UINT8 u4GetStcDiffFactor(AUD_STC_DIFF_SET eStcDiffType)
{
    UINT8 u4StcDiff;

    switch (eStcDiffType)
    {
        case AUD_STC_LO:
            u4StcDiff = _uStcDiffBound[AUD_STC_LO];
            break;
        case AUD_STC_HI:
            u4StcDiff = _uStcDiffBound[AUD_STC_HI];
            break;
        case AUD_STC_WS:
            u4StcDiff = _uStcDiffBound[AUD_STC_WS];
            break;
        default:
            u4StcDiff = 0;
            break;
    }

    return u4StcDiff;
}

void DSP_SetDualDecMode(BOOL fgEnable)
{
    UINT16 u2Data;

    u2Data = u2ReadShmUINT16(W_SYSTEM_CTRL);
    if (fgEnable)
    {
        u2Data |= SHM_SYSTEM_CTRL_DUAL_DEC_MODE;
    }
    else
    {
        u2Data &= (~SHM_SYSTEM_CTRL_DUAL_DEC_MODE);
    }
    vWriteShmUINT16(W_SYSTEM_CTRL, u2Data);
}

#ifdef CC_AUD_SKYPE_SUPPORT
void DSP_SetAuxMixMainMode(BOOL fgEnable)
{
    UINT16 u2Data;

    u2Data = u2ReadShmUINT16(W_SYSTEM_CTRL2);
    if (fgEnable)
    {
        u2Data |= SHM_SYSTEM_CTRL2_AUX_MIX_MAIN_MODE;
    }
    else
    {
        u2Data &= (~SHM_SYSTEM_CTRL2_AUX_MIX_MAIN_MODE);
    }
    vWriteShmUINT16(W_SYSTEM_CTRL2, u2Data);
}
#endif

#ifdef CC_AUD_PCM_LINE_IN_LPF
void DSP_SetPCMLineInLPF(BOOL fgEnable)
{
    UINT8 u1Index;

    if (fgEnable)
    {
        vWriteDspWORD(ADDR_RD2D_PCM_LINE_IN_LPF_FLAG, 0x01);
        for (u1Index=0; u1Index<20; u1Index++)
        {
            WriteDspCommDram ((ADDR_R2CD_DRAM_PCM_LPF_COEF + u1Index),
                              PCM_LINE_IN_LPF_TABLE[u1Index]);
        }
    }
    else
    {
        vWriteDspWORD(ADDR_RD2D_PCM_LINE_IN_LPF_FLAG, 0x00);
    }
}
#endif

#if (defined(CC_AUD_SUPPORT_MS10) && defined(CC_MT5391_AUD_3_DECODER))
void DSP_SetAdFreeRun(BOOL fgEnable)
{
    UINT8 u1Data;

    u1Data = uReadShmUINT8(B_ADFLAG);
    if (fgEnable)
    {
        u1Data &= ~(0x80);         //bit7: Sample-by-sample sync
        u1Data |= 0x40;         //bit6: Check one buffer only

    }
    else
    {
        u1Data &= ~(0x40);         //bit6: Check one buffer only
        u1Data |= 0x80;        //bit7: Sample-by-sample sync
    }
    LOG(0,"!!!DSP_SetAdFreeRun!!!\n");
    vWriteDspShmBYTE(B_ADFLAG,u1Data);
    vWriteDspWORD (ADDR_RC2D_AD_FLAG, u1Data);

}
#endif
#ifdef CC_MT5391_AUD_3_DECODER
void DSP_SetTriDecMode(BOOL fgEnable)
{
    UINT16 u2Data;

    u2Data = u2ReadShmUINT16(W_SYSTEM_CTRL);
    if (fgEnable)
    {
        u2Data |= SHM_SYSTEM_CTRL_TRI_DEC_MODE;
    }
    else
    {
        u2Data &= (~SHM_SYSTEM_CTRL_TRI_DEC_MODE);
    }
    vWriteShmUINT16(W_SYSTEM_CTRL, u2Data);
}
#endif

#ifdef CC_AUD_4_DECODER_SUPPORT
void DSP_Set4thDecMode(BOOL fgEnable)
{
    UINT16 u2Data;

    u2Data = u2ReadShmUINT16(W_SYSTEM_CTRL2);
    if (fgEnable)
    {
        u2Data |= SHM_SYSTEM_CTRL_4TH_DEC_MODE;
    }
    else
    {
        u2Data &= (~SHM_SYSTEM_CTRL_4TH_DEC_MODE);
    }
    vWriteShmUINT16(W_SYSTEM_CTRL2, u2Data);
}
#endif

//-----------------------------------------------------------------------------
/** DSP_SetAsrcTrackingMode
 *   Set ASRC tracking mode or fixed mode for Dec1.
 *   Set when in HDMI and Line-in
 *
 *  @param  fgEnable FALSE: fixed mode, TRUE: tracking mode
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void DSP_SetAsrcTrackingMode(BOOL fgEnable)
{
    UINT16 u2Data;

    u2Data = u2ReadShmUINT16(W_SYSTEM_CTRL2);
    if (fgEnable)
    {
        // Tacking mode
        u2Data |= SHM_ASRC_TRACKING_MODE;
    }
    else
    {
        // Fixed mode
        u2Data &= (~SHM_ASRC_TRACKING_MODE);
    }
    vWriteShmUINT16(W_SYSTEM_CTRL2, u2Data);
}


void DSP_LogAtvTvSys(void)
{
    Printf("Curren channel system is %s \n", _aszTvSysMode[_eChannelSrc]);
}

#ifdef CC_AUD_BBE_SUPPORT
// *********************************************************************
// Function : DSP_SetBbeLevel
// Description : Set BBE level (by table BBEV4_LEVEL_DB[7])
// Parameter : u1Index: 0 ~ 5
// Return    : void
// *********************************************************************
void DSP_SetBbeLevel(UINT8 u1Index)
{
    UINT32 u4DramAdrIdx;

    if (u1Index > BBE_LEVEL_MAX)
    {
        u1Index = BBE_LEVEL_MAX;
    }

    vWriteShmUINT8(B_BBE_LEVEL, u1Index);
    u4DramAdrIdx = dReadDspCommDram(ADDR_RC2D_BBE_BASE) + ADDR_BBEV4_LEVEL_COEF;
    WriteDspCommDram (u4DramAdrIdx, (UINT32)BBEV4_LEVEL_DB[u1Index]);
}

// *********************************************************************
// Function : DSP_SetBbeProcess
// Description : Set BBE Process boost gain (by table BBEV4_DB_8[31])
// Parameter : u1Index: 3*2 ~ 12*2
// Return    : void
// *********************************************************************
void DSP_SetBbeProcess(UINT8 u1Index)
{
    UINT32 u4DramAdrIdx;

    if (u1Index < BBE_PROCESS_MIN)
    {
        u1Index = BBE_PROCESS_MIN;
    }

    if (u1Index > BBE_PROCESS_MAX)
    {
        u1Index = BBE_PROCESS_MAX;
    }

    vWriteShmUINT8(B_BBE_PROCESS, u1Index);
    u4DramAdrIdx = dReadDspCommDram(ADDR_RC2D_BBE_BASE) + ADDR_BBEV4_HIGH_COEF;
    WriteDspCommDram (u4DramAdrIdx, (UINT32)BBEV4_DB_8[u1Index]);
}

// *********************************************************************
// Function : DSP_SetBbeLoContour
// Description : Set BBE Lo Contour boost gain (by table BBEV4_DB_8[31])
// Parameter : u1Index: 3*2 ~ 12*2
// Return    : void
// *********************************************************************
void DSP_SetBbeLoContour(UINT8 u1Index)
{
    UINT32 u4DramAdrIdx;

    if (u1Index < BBE_LO_CONTOUR_MIN)
    {
        u1Index = BBE_LO_CONTOUR_MIN;
    }

    if (u1Index > BBE_LO_CONTOUR_MAX)
    {
        u1Index = BBE_LO_CONTOUR_MAX;
    }

    vWriteShmUINT8(B_BBE_LO_CONTOUR, u1Index);
    u4DramAdrIdx = dReadDspCommDram(ADDR_RC2D_BBE_BASE) + ADDR_BBEV4_LOW_COEF;
    WriteDspCommDram (u4DramAdrIdx, (UINT32)BBEV4_DB_8[u1Index]);
}

// *********************************************************************
// Function : DSP_SetBbe3dGain
// Description : Set BBE ViVA 3D gain (by table BBEV4_3D_GAIN[101])
// Parameter : u1Index: 0 ~ 100
// Return    : void
// *********************************************************************
void DSP_SetBbe3dGain(UINT8 u1Index)
{
    UINT32 u4DramAdrIdx;

    if (u1Index > BBE_3D_GAIN_MAX)
    {
        u1Index = BBE_3D_GAIN_MAX;
    }

    vWriteShmUINT8(B_BBE_3D_GAIN, u1Index);
    u4DramAdrIdx = dReadDspCommDram(ADDR_RC2D_BBE_BASE) + ADDR_BBEV4_3D_GAIN;
    WriteDspCommDram (u4DramAdrIdx, (UINT32)BBEV4_3D_GAIN[u1Index]);
}
#endif

#ifdef CC_AUD_DSP_SUPPORT_CDNOTCH
void DSP_SetCDNotchQandFc(UINT32 u4UserQIdx, UINT32 u4UserFc)
{
    UINT32 u4DramAdrBase;

    if (/*u4UserQIdx < CDNOTCH_Q_IDX_MIN ||*/ u4UserFc < CDNOTCH_FC_MIN ||
		u4UserQIdx > CDNOTCH_Q_IDX_MAX || u4UserFc > CDNOTCH_FC_MAX)
    {
        Printf("Input Out of Range!\n");
		Printf("[Q] 0 ~ 3.7 [Fc] 60Hz ~ 15kHz\n");
		return;
    }

    u4DramAdrBase = dReadDspCommDram(ADDR_RC2D_CDNOTCH_BASE);
    vWriteShmUINT8(B_CDNOTCH_USER_Q, (UINT8)u4UserQIdx);
    vWriteShmUINT16(W_CDNOTCH_USER_FC,(UINT16)u4UserFc);
    DSP_SendDspTaskCmd(UOP_DSP_CDNOTCH_CFG);
}
#endif

#ifdef  KARAOKE_SUPPORT
void DSP_SetKeyshiftFlag(BOOL fgflag)
{
    vWriteShmUINT8(B_KEYSHIFT_FLAG, (UINT32)fgflag);
    DSP_SendDspTaskCmd(UOP_DSP_KEYSHIFT_FLAG);
}
void DSP_SetKeyshiftKey(INT8 i1key)
{
    vWriteShmUINT8(B_KEYSHIFT_KEY, (UINT32)i1key);
    DSP_SendDspTaskCmd(UOP_DSP_KEYSHIFT_KEY);
}


#endif

// *********************************************************************
// Function : DSP_SetPcmLrckMode
// Description : Set LRCK mode for PCM decoder
// Parameter : u1DecId: AUD_DEC_MAIN or AUD_DEC_AUX
//                 fgEable
// Return    : void
// *********************************************************************
void DSP_SetPcmLrckMode(UINT8 u1DecId, BOOL fgEable)
{
    UINT32 u4Address;
    UINT8 u1Value;

    AUD_DEC_ID_VALIDATE_2(u1DecId);
    u4Address = (u1DecId == AUD_DEC_MAIN) ?
                ADDR_RC2D_PCM_LRCK_MODE : ADDR_RC2D_PCM_LRCK_MODE_DEC2;
    u1Value = ((AUD_READ32(REG_EXTADC_CFG) & SPLIN_LR_INVT) == 0) ?
            0x1 : 0x3;
    if (fgEable)
    {
        vWriteDspWORD(u4Address,wReadDspWORD(u4Address)|u1Value);
    }
    else
    {
        vWriteDspWORD(u4Address, wReadDspWORD(u4Address)&(~u1Value));
    }
}

// *********************************************************************
// Function : DSP_SetLineInLrInverse
// Description : Set LR inverse for line in
// Parameter : u1DecId: AUD_DEC_MAIN or AUD_DEC_AUX
//                 fgEable
// Return    : void
// *********************************************************************
void DSP_SetLineInLrInverse(UINT8 u1DecId, BOOL fgEable)
{
    UINT32 u4Address = 0;

    AUD_DEC_ID_VALIDATE(u1DecId);

    // PCM decoder doesn't run at dec3
    if (u1DecId == AUD_DEC_MAIN)
    {
        u4Address = ADDR_RC2D_PCM_LRCK_MODE;
    }
    else if (u1DecId == AUD_DEC_AUX)
    {
        u4Address = ADDR_RC2D_PCM_LRCK_MODE_DEC2;
    }
    else if (u1DecId == AUD_DEC_THIRD)
    {
        u4Address = ADDR_RC2D_PCM_LRCK_MODE_DEC3;
    }
    else if (u1DecId == AUD_DEC_4TH)
    {
        u4Address = ADDR_RC2D_PCM_LRCK_MODE_DEC4;
    }

    if (fgEable)
    {
        vWriteDspWORD(u4Address, wReadDspWORD(u4Address)|(0x1<<2));
    }
    else
    {
        vWriteDspWORD(u4Address, wReadDspWORD(u4Address)&(0xfffb));
    }
}

UINT32 DSP_GetDataRate(UINT8 u1DecId)    //Andrew : For enhance DD Banner
{
    return dReadDspCommDram((u1DecId == AUD_DEC_MAIN)?
    	ADDR_D2RC_RISC_INFO_BIT_RATE : ADDR_D2RC_RISC_INFO_BIT_RATE_DEC2);
}

UINT8 DSP_GetSampleRate(UINT8 u1DecId)
{
    return (UINT8)u4DspGetSampleRate(u1DecId);
}

// *********************************************************************
// Function : DSP_SetPcmType
// Description : Set PCM Type
// Parameter : u1DecId: AUD_DEC_MAIN or AUD_DEC_AUX
//                 u2Value : 0 -> pcm24
//                              1 ->  IMA
//                              2 ->  pcm16
//                              3 ->  MS
//                              msb=1: little endian
// Return    : void
// *********************************************************************
void DSP_SetPcmType(UINT8 u1DecId, PCM_INPUT_TYPE_T eType)
{
    UINT32 u4Address = 0;

    AUD_DEC_ID_VALIDATE(u1DecId);

    if (u1DecId == AUD_DEC_MAIN)
    {
        u4Address = W_PCM_INPUT_TYPE;
    }
    else if (u1DecId == AUD_DEC_AUX)
    {
        u4Address = W_PCM_INPUT_TYPE_DEC2;
    }
    else if (u1DecId == AUD_DEC_THIRD)
    {
        u4Address = W_PCM_INPUT_TYPE_DEC3;
    }
    else if (u1DecId == AUD_DEC_4TH)
    {
        u4Address = W_PCM_INPUT_TYPE_DEC4;
    }

    vWriteShmUINT16(u4Address, (UINT32)eType);
}

// *********************************************************************
// Function : DSP_SetPcmFs
// Description : Set PCM input sampling rate
// Parameter : u1DecId: AUD_DEC_MAIN or AUD_DEC_AUX
//                 eFs : see SAMPLE_FREQ_T
// Return    : void
// *********************************************************************
void DSP_SetPcmFs(UINT8 u1DecId, SAMPLE_FREQ_T eFs)
{
    UINT32 u4Address = 0;
    UINT16 u2FsTbl[]=
    {
        SFREQ_16K, // FS_16K
        SFREQ_22K, // FS_22K
        SFREQ_24K,// FS_24K
        SFREQ_32K,// FS_32K
        SFREQ_44K,// FS_44K
        SFREQ_48K,// FS_48K
        SFREQ_48K,// FS_64K--> default
        SFREQ_88K,// FS_88K
        SFREQ_96K,// FS_96K
        SFREQ_176K,// FS_176K
        SFREQ_192K,// FS_192K
        SFREQ_8K,//FS_8K
        SFREQ_11K, // FS_11K
        SFREQ_12K, // FS_12K
        SFREQ_52K, // FS_52K
        SFREQ_56K, // FS_56K
        SFREQ_62K,	// FS_62K
        SFREQ_64K   // FS_64K
    };

    AUD_DEC_ID_VALIDATE(u1DecId);

    if (u1DecId == AUD_DEC_MAIN)
    {
        u4Address = W_PCM_INPUT_SAMPLING_RATE;
    }
    else if (u1DecId == AUD_DEC_AUX)
    {
        u4Address = W_PCM_INPUT_SAMPLING_RATE_DEC2;
    }
    else if (u1DecId == AUD_DEC_THIRD)
    {
        u4Address = W_PCM_INPUT_SAMPLING_RATE_DEC3;
    }
    else if (u1DecId == AUD_DEC_4TH)
    {
        u4Address = W_PCM_INPUT_SAMPLING_RATE_DEC4;
    }

    vWriteShmUINT16(u4Address, u2FsTbl[eFs]);
}

// *********************************************************************
// Function : DSP_SetAc3Endian
// Description : Set AC3 endian
// Parameter : u1DecId: AUD_DEC_MAIN or AUD_DEC_AUX
//                 u2Value :
// Return    : void
// *********************************************************************
void DSP_SetAc3Endian (UINT8 u1DecId, DATA_ENDIAN_T eEndian)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

    if (u1DecId == AUD_DEC_MAIN)
    {
        vWriteShmUINT8(B_DEC_ENDIAN, (UINT8)eEndian);
    }
}

void DSP_Get_VSurr_Cfg(AUD_VSURR_CFG_T* prVsurrCfg)
{
#if 0
    prVsurrCfg->u4Xtalk =      dReadDspCommDram(ADDR_VSURR_CROS_PARA1);    //0x8400
    prVsurrCfg->u4Width=       dReadDspCommDram(ADDR_VSURR_CROS_PARA2);    //0x8401
    prVsurrCfg->u4LRGain =     dReadDspCommDram(ADDR_VSURR_CROS_PARA3);    //0x8402
    prVsurrCfg->u4Clarity =    dReadDspCommDram(ADDR_VSURR_GAIN_MPY);      //0x8406
    prVsurrCfg->u4Fo =         dReadDspCommDram((ADDR_VSURR_BASS_COEF + 3)); //0x843c
    prVsurrCfg->u4BassGain =   dReadDspCommDram(ADDR_VSURR_CROS_PARA4);    //0x843e
    prVsurrCfg->u4OutputGain = dReadDspCommDram(ADDR_VSURR_CROS_PARA5);    //0x843f
#else
    prVsurrCfg->u4Xtalk =      u4ReadDspShmDWRD(D_VSURR_CROS_TALK);
    prVsurrCfg->u4Width=       u4ReadDspShmDWRD(D_VSURR_WIDTH);
    prVsurrCfg->u4LRGain =     u4ReadDspShmDWRD(D_VSURR_LR_GAIN);
    prVsurrCfg->u4Clarity =    u4ReadDspShmDWRD(D_VSURR_CLARITY);
    //prVsurrCfg->u4Fo =         u4ReadDspShmDWRD((D_VSURR_FO));
    prVsurrCfg->u4Fo =         dReadDspCommDram((ADDR_VSURR_BASS_COEF + 3)); //0x843c
    prVsurrCfg->u4BassGain =   u4ReadDspShmDWRD(D_VSURR_BASS_GAIN);
    prVsurrCfg->u4OutputGain = u4ReadDspShmDWRD(D_VSURR_OUTPUT_GAIN);
#endif
}

void DSP_Set_VSurr_Cfg(AUD_VSURR_CFG_TYPE_T eVSCfgType, UINT32 u4VSParameter)
{
    switch(eVSCfgType)
    {
        case AUD_SUR_CLARITY:
            vWriteDspShmDWRD (D_VSURR_CLARITY,   u4VSParameter);   //center gain,
            break;
        case AUD_SUR_WIDTH:
            vWriteDspShmDWRD (D_VSURR_WIDTH,   u4VSParameter);   //surround gain,
            break;
        case AUD_SUR_LRGAIN:
            vWriteDspShmDWRD (D_VSURR_LR_GAIN,   u4VSParameter); //L/R gain,
            break;
        case AUD_SUR_XTALK:
            vWriteDspShmDWRD (D_VSURR_CROS_TALK,   u4VSParameter); //crosstalk,
            break;
        case AUD_SUR_OUTPUT_GAIN:
            vWriteDspShmDWRD (D_VSURR_OUTPUT_GAIN,   u4VSParameter); //output gain,
            break;
        case AUD_SUR_BASS_GAIN:
            vWriteDspShmDWRD (D_VSURR_BASS_GAIN,   u4VSParameter); //bass enhancement,
            break;
        case AUD_SUR_FO:
            vWriteDspShmDWRD (D_VSURR_FO,   u4VSParameter); //FO,
            break;
        default :
            break;
    }
    vDspCmd (UOP_DSP_NEW_MTK_VSURR);
}

void DSP_GetDtvAudInfo(UINT8 u1DecId, AUD_DEC_INFO_T * prAudInfo)
{
    UINT8 u1DspStrTyp;

    u1DspStrTyp = _rDspVars[u1DecId].bDspStrTyp;

    switch(u1DspStrTyp)
    {
    case AC3_STREAM:
    	_GetAudInfo(u1DecId,  prAudInfo);
    	break;
    case MPEG12_STREAM:
    	UNUSED(_GetMpegAudInfo(u1DecId,  prAudInfo));
    	break;
    case AAC_STREAM:
    	_GetAacAudInfo(u1DecId,  prAudInfo);
    	break;
    default:
    	break;
    }

}
#ifdef __MODEL_slt__
void DSP_ChangeSourceMode(UINT8 u1Mode)
{
    vWriteDspShmWORD (W_SOURCE_MODE, u1Mode);
}

void  DSP_ReadDecDram (UINT32 u4Addr, UINT32 *u4Data)
{
    ReadDspDecCacheDram (u4Addr,  *u4Data);
}
#endif

#ifdef	DATA_DISC_WMA_SUPPORT
UINT32 DSP_GetWmaErrorCode(void)
{
    if (!_rDspStatus[AUD_DEC_MAIN].fgDspStopIssue && _rDspStatus[AUD_DEC_MAIN].fgDspStop)	//stop without issue DSP_STOP command && DSP already flush, it's WMA error
        return( (dReadDspCommDram(ADDR_D2RC_WMA_ERROR_MODE)>>8)+1);	//shift error code by 1 because there is error code=0
    else        //normal stop
        return(0);
}
UINT32 DSP_GetWmaFrameSize(void)
{
    if (_rDspStatus[AUD_DEC_MAIN].fgDspRealPlay)
    {
        return(u4ReadDspShmDWRD (D_WMA_PACKET_SIZE));
    }
    else
    {
        return(0);
    }
}

#endif

#if 1///def CC_VORBIS_SUPPORT    ///CC_VORBIS_SUPPORT_ERR
UINT32 DSP_GetVorbisErrorCode(void)
{
    UINT32  u4Temp1;
    UINT32  u4Temp2 = 0;

    if (!_rDspStatus[AUD_DEC_MAIN].fgDspStopIssue && _rDspStatus[AUD_DEC_MAIN].fgDspStop)
    {
        for(u4Temp1=D2RC_VORBIS_SYSTEM_ERROR_CODE;u4Temp1<=(D2RC_VORBIS_PACKET_ERROR_CODE);u4Temp1++)
        {
            ///LOG(0,"########### Address = 0x%x. ################\n.", u4Temp1);
            u4Temp2 += (DSP_DecDramRead(u4Temp1)>>8);
        }
        return(u4Temp2+1);   /// +1 for decode have stop.
    }
    else        //normal stop
    {
        return(0);
    }
}
#endif

UINT8 u1GetAacChNum(UINT8 u1DecId)
{
    UINT8 u1Mode;
    if (u1DecId != AUD_DEC_AUX)
    {
        ASSERT(0);
    }
//    u1SmpRate = dReadDspCommDram(ADDR_D2RC_AAC_SAMPLINGRATE) >> 8;
    u1Mode = (UINT8)(dReadDspCommDram(ADDR_D2RC_AAC_CH_MODE_DEC2) >> 8);
	return (u1Mode);
}

UINT8 u1GetAacVersion(UINT8 u1DecId)
{
    //VERIFY(u1DecId == AUD_DEC_AUX);
    if(u1DecId==AUD_DEC_MAIN)
    {
        return((UINT8)(dReadDspCommDram(ADDR_D2RC_AAC_VERSION) >> 8));
    }
	else if(u1DecId==AUD_DEC_AUX)
	{
    return((UINT8)(dReadDspCommDram(ADDR_D2RC_AAC_VERSION_DEC2) >> 8));
}
	else
	{
	    return 0xff;
	}

}

UINT8 u1GetAacTSFormat(UINT8 u1DecId)
{
    //VERIFY(u1DecId == AUD_DEC_AUX);
    if(u1DecId==AUD_DEC_MAIN)
    {
        return((UINT8)(dReadDspCommDram(ADDR_D2RC_AAC_TRANS_FORMAT) >> 8));
    }
	else if(u1DecId==AUD_DEC_AUX)
	{
	    return((UINT8)(dReadDspCommDram(ADDR_D2RC_AAC_TRANS_FORMAT_DEC2) >> 8));
	}
	else
	{
	    return 0xff;
	}

}


//-----------------------------------------------------------------------------
/** DSP_SetMp3PreparsingMode
 *   Set MP3 decoder to preparsing mode
 *
 *  @param  fgEnable      TRUE(enable) FALSE(disable).
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void DSP_SetMp3PreparsingMode(BOOL fgEnable)
{
    vWriteDspShmWORD(ADDR_RC2D_MP3_PREPARSING_MODE, ((UINT16)fgEnable));
}

//-----------------------------------------------------------------------------
/** DSP_IsMp3Vbr
 *   Is MP3 VBR
 *
 *  @retval  TRUE(VBR) FALSE(CBR)
 */
//-----------------------------------------------------------------------------
BOOL DSP_IsMp3Vbr(void)
{
    return ((BOOL)wReadDspWORD(ADDR_D2RC_MP3_VBR));
}

//-----------------------------------------------------------------------------
/** DSP_FlushInvalidateDCache
 *   Flush invalidate DCache
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void DSP_FlushInvalidateDCache(UINT8 u1Type, UINT8 u1Block)
{
//CACHE_FLUSH_TYPE_ICACHE        0
//CACHE_FLUSH_TYPE_DATA          1
//CACHE_FLUSH_TYPE_FREE          2
//HalFlushInvalidateDCache
// Flush the all program/data dram block
// The flush size unit is 32 byte, so we add 32 byte (0x20).

    UNUSED(DSP_FlushInvalidateDCache);
    if (u1Type == CACHE_FLUSH_TYPE_DATA)
    {
         HalFlushInvalidateDCacheMultipleLine(VIRTUAL(_u4DspDramBuf[u1Block]), _u4DspDramBufLen[u1Block] + 0x20);
         //LOG(3, "DSP_FlushInvalidateDCache(DATA), Str=0x%x, Len=0x%x\n", VIRTUAL(_u4DspDramBuf[u1Block]), _u4DspDramBufLen[u1Block]);
    }
    else if (u1Type == CACHE_FLUSH_TYPE_ICACHE)
    {
        HalFlushInvalidateDCacheMultipleLine(VIRTUAL(_u4DspICacheAddr[u1Block] << 8), _u4DspICacheLen[u1Block] + 0x20);
        //LOG(3, "DSP_FlushInvalidateDCache(ICACHE), Str=0x%x, Len=0x%x\n", VIRTUAL(_u4DspICacheAddr[u1Block] << 8), _u4DspICacheLen[u1Block]);
    }
}

void DSP_FlushInvalidateDCacheSmall(UINT8 u1Type, UINT8 u1Block, UINT32 u4Offset, UINT32 u4Len) // u4Offset is byte address.
{
// Flush the program/data dram block with offset and size.
// address and length must be 32 byte alignment.

    UINT32 u4StartAddr = 0;
    UNUSED(DSP_FlushInvalidateDCacheSmall);
    UNUSED(u4StartAddr);
    UNUSED(u4Len);
    UNUSED(u4Offset);

    if (u1Block > TYPE_RESERVED_7_IDX)
    {
        ASSERT(0);
    }
    if (u1Type == CACHE_FLUSH_TYPE_DATA)
    {
        u4StartAddr = _u4DspDramBuf[u1Block] + u4Offset;
        u4StartAddr &= 0xFFFFFFE0; //make sure addr & len is 32 byte alignment.
        u4Len *= 4;
        u4Len += 0x40;             //add 64 bytes for safety.

        #if defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)
        HalFlushInvalidateDCacheMultipleLine(VIRTUAL(u4StartAddr), u4Len);
        #else
        DSP_FlushInvalidateDCache(u1Type, u1Block);
        #endif
        //LOG(3, "DSP_FlushInvalidateDCache(DATA), Str=0x%x, Len=0x%x\n", VIRTUAL(u4StartAddr), u4Len);
    }
    else if (u1Type == CACHE_FLUSH_TYPE_ICACHE)
    {
        DSP_FlushInvalidateDCache(u1Type, u1Block); //ICache must be all block, just pass it to all block API.
    }
}


void DSP_FlushInvalidateDCacheFree(UINT32 u4Addr, UINT32 u4Len)
{
    UNUSED(DSP_FlushInvalidateDCacheFree);

    HalFlushInvalidateDCacheMultipleLine( VIRTUAL(u4Addr), u4Len + 0x20);
    //LOG(3, "DSP_FlushInvalidateDCache(FREE), Str=0x%x, Len=0x%x\n", VIRTUAL(u4Addr), u4Len);
}

void DSP_FlushInvalidateDCacheSmall2(UINT32 u4CommonIdx)
{
    // Flush the program/data dram block with offset and size.
    // address and length must be 32 byte alignment.
    UINT32 u4StartAddr = 0;
    u4StartAddr = _u4DspDramBuf[TYPE_COMM_DRAM_IDX] + (u4CommonIdx<<2);
    u4StartAddr &= 0xFFFFFFE0; //make sure addr & len is 32 byte alignment.
    HalFlushInvalidateDCacheMultipleLine(VIRTUAL(u4StartAddr), 0x20);
}

void DSP_InvalidateDCache(UINT32 u4Addr, UINT32 u4Len)
{
    UNUSED(DSP_InvalidateDCache);

    HalInvalidateDCacheMultipleLine( VIRTUAL(u4Addr), u4Len + 0x20);
    //LOG(3, "DSP_FlushInvalidateDCache(FREE), Str=0x%x, Len=0x%x\n", VIRTUAL(u4Addr), u4Len);
}



//-----------------------------------------------------------------------------
/** DSP_GetEffectChannelNum
 *   Get effect channel number
 *
 *  @param  void
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
UINT8 DSP_GetEffectChannelNum(void)
{
    UINT8 u1ChNum;

    u1ChNum = dReadDspCommDram(ADDR_D2RC_RISC_INFO_EFFECT_CH_NUM) >> 8;

    return u1ChNum;
}

//-----------------------------------------------------------------------------
/** DSP_DrvResume
 *  DSP driver resume
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void DSP_DrvResume(void)
{
#ifndef CC_FPGA
    UNUSED(_fgDisableDspMidProtect());
#endif
}

//-----------------------------------------------------------------------------
/** DSP_DrvSuspend
 *  DSP driver suspend
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void DSP_DrvSuspend(void)
{
#ifdef CC_AUD_DATASRAM
    vDspResetDataSram();
#endif
}

#if 1///def CC_VORBIS_SUPPORT ///def DSP_VORBIS_SUPPORT
/***************************************************************************
     Function : fgIsDspVorbisTblDone
  Description :
    Parameter :
       Author : sammy
    Return    : TRUE : it is done
                FLASE: it is not
***************************************************************************/
BOOL fgIsDspVorbisTblDone (UINT8 u1DecId)
{
  if (_rDspStatus[u1DecId].fgDspStop && _uVorbisTblDone[u1DecId])
  {
    _uVorbisTblDone[u1DecId] = FALSE;
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

void DspVorbisCodeBookResult (BOOL fgStatus)
{
    if (fgStatus == TRUE)
    {

        _rDspStatus[AUD_DEC_MAIN].fgDspSInt = TRUE;
        _uDspSIntAddr = INT_RC2D_VORBIS_TABLE;
        if(_uVorbisIntDec == 1)
        {
        _u4DspSIntSD = (UINT32)VORBIS_CODEBOOK_READY;
    }
        else if(_uVorbisIntDec == 2)
        {
            _u4DspSIntSD = (UINT32)VORBIS_CODEBOOK_READY_DEC2;
        }
    }
    else
    {
        if ((_uVorbisIntDec == 1) && (_rDspStatus[AUD_DEC_MAIN].fgDspPlay) && (_rDspVars[AUD_DEC_MAIN].bDspStrTyp == VORBIS_STREAM))
        {
            vDspCmd(DSP_STOP);
        }
        if ((_uVorbisIntDec == 2) && (_rDspStatus[AUD_DEC_AUX].fgDspPlay) && (_rDspVars[AUD_DEC_AUX].bDspStrTyp == VORBIS_STREAM))
        {
            vDspCmd(DSP_STOP_DEC2);
        }
    }
}

#endif // DSP_GET_MPEG_TYPE

//-----------------------------------------------------------------------------
/** DSP_SetCapability
 *  Set capability
 *
 *  @param      void
 *  @retval     void
 *
 *  Bonding info
 *
 *  AC3     : support AC3 (if EAC3=1 or DDCO=1, always support EAC3)
 *  EAC3    : support EAC3
 *  DDCO    : support EAC3 + HE-AAC + DDCO
 *  MS10    : support EAC3 + HE-AAC + DDCO + DP
 *  H264    : support HE-AAC
 *  DTS     : support DTS
 *  DRA     : support DRA
 */
//-----------------------------------------------------------------------------
void DSP_SetCapability(void)
{
    _u4Capability[0] = 0;
    // TV audio formats
    if (AUD_GetTargetCountry() == COUNTRY_US)
    {   // US capability
        _u4Capability[0] = AUD_DEC_CAP_FMT_MPEG            |
                        AUD_DEC_CAP_FMT_PCM             |
                        AUD_DEC_CAP_FMT_TV_SYS          |
                        AUD_DEC_CAP_FMT_FMRDO           |
                        AUD_DEC_CAP_FMT_FMRDO_DET       |
                        AUD_DEC_CAP_FMT_MTS;
    }
    else
    {   // EU, CN, AUS, TWN capability
        _u4Capability[0] = AUD_DEC_CAP_FMT_MPEG            |
                        AUD_DEC_CAP_FMT_PCM             |
                        AUD_DEC_CAP_FMT_TV_SYS          |
                        AUD_DEC_CAP_FMT_FMRDO           |
                        AUD_DEC_CAP_FMT_FMRDO_DET       |
                        AUD_DEC_CAP_FMT_EU_CANAL_PLUS;
    }

#ifdef ENABLE_MULTIMEDIA
    _u4Capability[0] |= AUD_DEC_CAP_FMT_MP3;

#ifdef CC_AUD_MMP_FULL_SUPPORT
    _u4Capability[0] |= (AUD_DEC_CAP_FMT_WMA | AUD_DEC_CAP_FMT_COOK);
#endif
#endif

#ifdef CC_AUD_SKYPE_SUPPORT
    _u4Capability[0] |= (AUD_DEC_CAP_FMT_SBCDEC | AUD_DEC_CAP_FMT_SBCENC);
#endif

    if (IS_SupportDTS())
    {
        LOG(3, "[Bonding] Enable DTS format\n");
        //FIXME!!!===>Should Enable it when DTS decoder is ready.
        //_u4Capability[0] |= AUD_DEC_CAP_FMT_DTS;
    }

    if (IS_SupportAVS())
    {
        LOG(3, "[Bonding] Enable DRA format\n");
        _u4Capability[0] |= AUD_DEC_CAP_FMT_DRA;
    }

#ifndef CC_AUD_DISABLE_AC3_AAC

#if defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)
		if ((IS_SupportH264() || (IS_SupportDOLBY()  && (IS_SW_SupportDDCO() || IS_SW_SupportMS10())))&&AUD_IsSupportMpeg4())
		{
			LOG(3, "[Bonding] Enable AAC and DDCO format\n");
			_u4Capability[0] |= AUD_DEC_CAP_FMT_AAC;
		}

		if ((IS_SW_SupportAC3() || IS_SW_SupportEAC3() || IS_SW_SupportMS10())&&AUD_IsSupportMpeg4())
		{
			LOG(3, "[Bonding] Enable AC3 format\n");
			_u4Capability[0] |= AUD_DEC_CAP_FMT_AC3;

		}
#else
		if (IS_SupportDDCO() || IS_SupportMS10() || IS_SupportH264())
		{
			LOG(3, "[Bonding] Enable AAC and DDCO format\n");
			_u4Capability[0] |= AUD_DEC_CAP_FMT_AAC;
		}

		if (IS_SupportAC3() || IS_SupportEAC3() || IS_SupportDDCO() || IS_SupportMS10())
		{
			LOG(3, "[Bonding] Enable AC3 format\n");
			_u4Capability[0] |= AUD_DEC_CAP_FMT_AC3;
		}
#endif

#endif

#if 1///def CC_VORBIS_SUPPORT
    _u4Capability[0] |= AUD_DEC_CAP_FMT_VORBIS;
#endif

    //over 32bits range, put in _u4Capability_ext
#if (!defined(CC_MT5365) && !defined(CC_MT5395))
    _u4Capability[1] |= AUD_DEC_CAP_FMT_WMAPRO;
#endif
    _u4Capability[1] |= AUD_DEC_CAP_FMT_AMRNB;
    _u4Capability[1] |= AUD_DEC_CAP_FMT_AMRWB;
    LOG(3,"_u4Capability[1] %d\n",_u4Capability[1]);

}

//-----------------------------------------------------------------------------
/** DSP_GetCapability
 *  Get capability
 *
 *  @param      void
 *  @retval     format capability
 */
//-----------------------------------------------------------------------------
UINT32 DSP_GetCapability(UINT8 u1Idx)
{
    return _u4Capability[u1Idx];
}

//-----------------------------------------------------------------------------
/** DSP_IsFormatValid
 *  Is format valid
 *
 *  @param      u4FormatInfo: format index
 *  @retval     TRUE(valid). FALSE(invalid)
 */
//-----------------------------------------------------------------------------
BOOL DSP_IsFormatValid(UINT32 u4FormatInfo)
{
    UINT32 u4FmtMask;
    if (u4FormatInfo < 32)      //check if in 32bits range
    {
        u4FmtMask = ((UINT32)1) << u4FormatInfo;
        if ((u4FmtMask & _u4Capability[0]) == 0)
        {
            return FALSE;
        }
    }
    else if (u4FormatInfo < 64)
    {
        u4FmtMask = ((UINT32)1) << (u4FormatInfo-32);
        if ((u4FmtMask & _u4Capability[1]) == 0)
        {
            return FALSE;
        }
    }
    else
    {
        return FALSE;       //to be extended
    }
    return TRUE;
}

//-----------------------------------------------------------------------------
/** DSP_GetChannelDelay
 *
 *
 *  @param      u1ChIdx
 *  @retval     Delay in ms
 */
//-----------------------------------------------------------------------------
UINT32 DSP_GetChannelDelay(UINT8 u1ChIdx) //QUERY_DELAY
{
    /*
      Channel (0)-C, (1)-L, (2)-R, (3)-Ls, (4)-Rs, (5)-BypassL, (6)-ByPassR, (7)-Sw, (8)-DmxL, (9)-DmxR
      ; 7, C,   R,  L      --> audio output channel config0
      ; 8, LFE, SR, SL     --> audio output channel config1
      ; null, null, ch9, ch10 --> audio output channel config2
    */
    UINT32 u4Delay;
    UINT32 u4ChCfg;

    UNUSED(DSP_GetChannelDelay);

    // Query Channel Location from Sram.
    u4ChCfg = u4ReadDspSram(SRAM_AOUT_CH_CFG0);
    switch (u1ChIdx)
    {
    case 0 ://C
        u4ChCfg = (u4ChCfg >> (8+8)) & 0xF;
        break;
    case 1 ://L
        u4ChCfg = (u4ChCfg >> (8+0)) & 0xF;
        break;
    case 2 ://R
        u4ChCfg = (u4ChCfg >> (8+4)) & 0xF;
        break;
    case 3 ://Ls
        u4ChCfg = (u4ChCfg >> (8+0)) & 0xF;
        break;
    case 4 ://Rs
        u4ChCfg = (u4ChCfg >> (8+4)) & 0xF;
        break;
    case 5 ://7
        u4ChCfg = (u4ChCfg >> (8+12)) & 0xF;
        break;
    case 6 ://8
        u4ChCfg = (u4ChCfg >> (8+12)) & 0xF;
        break;
    case 7 ://Sw
        u4ChCfg = (u4ChCfg >> (8+8)) & 0xF;
        break;
    case 8 ://DmxL
        u4ChCfg = (u4ChCfg >> (8+4)) & 0xF;
        break;
    case 9 ://DmxR
        u4ChCfg = (u4ChCfg >> (8+0)) & 0xF;
        break;
    default:
        u4ChCfg = 0xF;
        break;
    }

    // Get Delay Offset from Dram, and change to ms.
    if (u4ChCfg == 0xF)
    {
        u4Delay = 0;
    }
    else
    {
        u4ChCfg -= 1; //channel config starts from 1
        u4Delay = wReadDspWORD((ADDR_DELAY_OFFSET_C + u4ChCfg));
        u4Delay = (u4Delay*4) / (3*48);  // u4Delay is dword.  *(4/3) to get sample.  /48 to get ms.
        //u4Delay += 30;  // initial 6 banks delay.
}

    return u4Delay;
}

//-----------------------------------------------------------------------------
/** DSP_SetPtsStcCtrlFlag
 *
 *
 *  @param      u4Flag
 *  @retval
 */
//-----------------------------------------------------------------------------
void DSP_SetPtsStcCtrlFlag(UINT32 u4Flag)
{
    UNUSED(DSP_SetPtsStcCtrlFlag);

    vWriteDspWORD(ADDR_RC2D_PTS_STC_CTRL_FLAG, u4Flag);
    _AUD_DspSetAoutReinit();
}

UINT16 DSP_GetPtsStcCtrlFlag(void)
{
    UNUSED(DSP_GetPtsStcCtrlFlag);

    return wReadDspWORD(ADDR_RC2D_PTS_STC_CTRL_FLAG);
}

void DSP_SetASRCNumeratorQuarter(BOOL fgEnable)
{
    if (fgEnable)
    {
        vWriteDspShmWORD (W_SYSTEM_CTRL2, u2ReadDspShmWORD(W_SYSTEM_CTRL2) | 0x100);
    }
    else
    {
        vWriteDspShmWORD (W_SYSTEM_CTRL2, u2ReadDspShmWORD(W_SYSTEM_CTRL2) & (~0x100));
    }
}

BOOL DSP_IsSupportAC3(void)
{
#if defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)
    if (IS_SW_SupportAC3() || IS_SW_SupportEAC3() || IS_SW_SupportMS10())
    {
        return TRUE;
    }
#else
    if (IS_SupportAC3() || IS_SupportDDCO() || IS_SupportMS10())
    {
        return TRUE;
    }
#endif
    return FALSE;
}

BOOL DSP_IsSupportEAC3(void)
{
#if defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)
    if (IS_SW_SupportEAC3() || IS_SW_SupportMS10())
    {
        return TRUE;
    }
#else
    if (IS_SupportEAC3() || IS_SupportDDCO() || IS_SupportMS10())
    {
        return TRUE;
    }
#endif
    return FALSE;
}

BOOL DSP_IsSupportDDCO(void)
{
#if defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)
    if (IS_SupportDOLBY())
    {
        return TRUE;
    }
#else
    if (IS_SupportDDCO() || IS_SupportMS10())
    {
        return TRUE;
    }
#endif
    return FALSE;
}
// *********************************************************************
// Function : DSP_SetMixsndVolume
// Description : Set Mixsound volume 
// Parameter : u4Volume
// Return    : void
// *********************************************************************
void DSP_SetMixsndVolume(UINT32 u4Volume)
{
#ifdef CC_AUD_DSP_SUPPORT_MIXSOUND_MEMPLAY
    #ifndef MIXSOUND_GAIN_CHG_WITH_VOL
	UINT8 i = 0;
	LOG(9, "DSP_SetMixsndVolume(), Vol (%d)\n", u4Volume);
    for (i=0;i<MAX_AUD_MIXSND_STREAM_NUM;i++)
    {
        WriteDspCommDram (ADDR_RC2D_DRAM_MIXSOUND_CLIP1_GAIN+i*16, u4Volume);
    }
    #endif
#endif
}
