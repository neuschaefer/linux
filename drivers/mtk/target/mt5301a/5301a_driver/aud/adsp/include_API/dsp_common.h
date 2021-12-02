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
 * $RCSfile: dsp_common.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dsp_common.h
 *  Brief of file dsp_common.h.
 *  Details of file dsp_common.h (optional).
 */

#ifndef DSP_COMMON_H
#define DSP_COMMON_H

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_common.h"
#include "dsp_msg_map.h"
#include "../include/dsp_intf.h"

//-----------------------------------------------------------------------------
// Configuration
//-----------------------------------------------------------------------------

#ifdef DSP_RAM_CODE_CHKSUM
  //#define DSP_RAM_CODE_XOR_CHKSUM // old DSP binary use XOR check sum
#endif

// Rice: Add this compile time definition for NPTV audio demodulation 
// sub-routines.
#define DSP_SUPPORT_NPTV
//#define DSP_SUPPORT_SRSWOW

#ifndef DSP_SUPPORT_SRSWOW   // choose one
//#define DSP_SUPPORT_SRSTSXT
#endif

///define at sys_build\..\*.cfg and drv_build\..\*.def
#ifdef CC_AUDIO_DSP_JPG_SUPPORT
#define ADSP_JPEG_DEC2      ///[Joel]
    #ifdef ADSP_JPEG_DEC2
    #define JPEG_DBG
    #endif
#endif

#define DSP_EQ2_SBASS_SUPPORT
//#define SUPPORT_MIXSOUND

#define BASS_FILTER_CUSTOMIZE
#define CC_SET_VOLUME
//-----------------------------------------------------------------------------
// DSP test define
//-----------------------------------------------------------------------------
//#define DSP_TEST_PUTBITS
//#define DSP_TEST_CORDIC
//#define DSP_TEST_MIXSOUND_ADPCM
//#define DSP_TEST_PSRDMA
//#define DSP_TEST_DRAM_BLOCK
#define DATA_DISC_WMA_SUPPORT
#if defined(CC_MT8223)
#define AUDIO_MASTER_CHECK_AOUT_WAIT
#endif
#define MATCH_ADSP_RAMCODE
//#define DSP_NO_INFO
#define DELUX_DSP_VERSION
#ifdef DELUX_DSP_VERSION
#define UPMIX_SUPPORT           // PrologicII, Neo6
#define DOWNMIX_SUPPORT         // Virtual surround, Sony VSS
#define DSP_POST_PROCESS_ENABLE // Super bass, bass boost, trimble boost, eq, reverb
#define DSP_IEC_RAW_ENABLE

#ifdef EMULATION_FPGA
    //#define VORBIS_SUPPORT
        #define ADSP_DEBUG
#endif // EMULATION_FPGA
#endif // DELUX_DSP_VERSION

#ifdef UPMIX_SUPPORT
  #define PROLOGICII_SUPPORT
#endif // UPMIX_SUPPORT

//-----------------------------------------------------------------------------
// common define
//-----------------------------------------------------------------------------

#define DSP_VERSION_NUMBER                                 0x700000
#define DSP_RISC_CODE_VERSION                              0x01
#define SHOW_PTS_ON                                        0x01

#define ON    1
#define OFF   0

#if defined(DSP_SUPPORT_NPTV) || defined(DSP_SUPPORT_SRSWOW) || \
    defined(DSP_SUPPORT_SRSTSXT) || defined(DSP_SUPPORT_SRSTSHD)
#define SV_OFF  0
#define SV_ON   1
#endif

// For Icache start block settings
//#ifndef CC_MT5391_AUD_SUPPORT
#if 1   //added by ling , wait for Nilson
#if 1// KK_FPGA
#define TYPE_ROM_ICACHE           0
#define TYPE_POST_ICACHE          1
#define TYPE_COMM_ICACHE          2
#define TYPE_DEC2_ICACHE          3
#define TYPE_AIN_ICACHE           4
#define TYPE_DEC1_ICACHE          5
#else
#define TYPE_ROM_ICACHE           0
#define TYPE_AIN_ICACHE           1
#define TYPE_POST_ICACHE          2
#define TYPE_COMM_ICACHE          3
#define TYPE_DEC1_ICACHE          4
#define TYPE_DEC2_ICACHE          5
#endif
#ifdef CC_MT5391_AUD_DYNAMICLOAD_SUPPORT
// dynamic load by Shawn
#define TYPE_BASIC_END_ID		(TYPE_DEC1_ICACHE)
#define TYPE_VCT_DEC3_ICACHE	(TYPE_BASIC_END_ID + 1)		// decoder 3 vector region
#define TYPE_DEC3_ICACHE		(TYPE_BASIC_END_ID + 2)		// decoder 3 code region
#ifdef CC_NEW_PROGRAM_DEF
// MT5360B/MT5393
#define TYPE_DEC2_EXT0_ICACHE	(TYPE_BASIC_END_ID + 3)		// decoder 2 extension 0 code region
#define TYPE_DEC2_EXT1_ICACHE	(TYPE_BASIC_END_ID + 4)		// decoder 2 extension 1 code region
#define TYPE_CRITICAL_ICACHE	(TYPE_BASIC_END_ID + 5)		// critical code region
#define ICACHE_MAX_IDX			TYPE_CRITICAL_ICACHE
#else
// MT5360A/MT5391
#define TYPE_DEC2_EXT0_ICACHE	(TYPE_BASIC_END_ID + 3)		// decoder 2 extension 0 code region
#define TYPE_DEC2_EXT1_ICACHE	(TYPE_BASIC_END_ID + 4)		// decoder 2 extension 1 code region
#define ICACHE_MAX_IDX			TYPE_DEC2_EXT1_ICACHE
#endif
#else
#define ICACHE_MAX_IDX			  TYPE_DEC1_ICACHE
#endif
#else
#define TYPE_ROM_ICACHE           0
#define TYPE_DEC3_ICACHE           1
#define TYPE_POST_ICACHE          2
#define TYPE_COMM_ICACHE         3
#define TYPE_DEC2_ICACHE          4
#define TYPE_AIN_ICACHE            5
#define TYPE_DEC1_ICACHE          6
#define ICACHE_MAX_IDX			  TYPE_DEC1_ICACHE
#endif

// For data DRAM settings
#define TYPE_DEC1_DRAM_IDX        0
#define TYPE_DEC2_DRAM_IDX        1
#define TYPE_MIX_DRAM_IDX         2
#define TYPE_COMM_DRAM_IDX        3


// Audio Stream Type Definition
// Decoder1 Type (0x00 ~ 0x1f)
#define TYPE_AUD_DEC1       0x00
#define AUD_AC3_DEC1        (TYPE_AUD_DEC1)
#define AUD_MPEG_DEC1       (TYPE_AUD_DEC1+0x01)
#define AUD_AAC_DEC1        (TYPE_AUD_DEC1+0x02)
#define AUD_PCM_DEC1        (TYPE_AUD_DEC1+0x03)
#define AUD_MP3_DEC1        (TYPE_AUD_DEC1+0x04)
#define AUD_WMA_DEC1        (TYPE_AUD_DEC1+0x05)
#define AUD_VORBIS_DEC1     (TYPE_AUD_DEC1+0x06)
#define AUD_PINK_DEC1       (TYPE_AUD_DEC1+0x07)
#ifdef DSP_SUPPORT_NPTV
#define AUD_MTS_DEC1        (TYPE_AUD_DEC1+0x08)
#define AUD_PAL_DEC1        (TYPE_AUD_DEC1+0x09)
#define AUD_MINER_DEC1      (TYPE_AUD_DEC1+0x0A)
#define AUD_A2DEC_DEC1      (TYPE_AUD_DEC1+0x0B)
#define AUD_DETECTOR_DEC1   (TYPE_AUD_DEC1+0x0C)
#define AUD_FMFM_DEC1       (TYPE_AUD_DEC1+0x0D)
//#define AUD_NICAM_DEC1    (TYPE_AUD_DEC1+0x0E)
//#define AUD_TTXACC_DEC1   (TYPE_AUD_DEC1+0x0F)
#define AUD_LPCM_DEC1       (TYPE_AUD_DEC1+0x0E)
#define AUD_FMRDO_DEC1    (TYPE_AUD_DEC1+0x0F)
#define AUD_FMRDODET_DEC1    (TYPE_AUD_DEC1+0x10)
#define AUD_SBC_DEC1         (TYPE_AUD_DEC1+0x11)
#define DECODER1_MAX_INDEX  (AUD_SBC_DEC1) //fixme when increasing decoders

#define DSP_DTV				(TYPE_AUD_DEC1+0x1E) // Light: Add DTV type to reduce analog output latency
#define DSP_TV		    	(TYPE_AUD_DEC1+0x1F)
#else
#define DECODER1_MAX_INDEX  0x7
#endif

// decoder2 Type (0x40~0x7f)
#define TYPE_AUD_DEC2     0x40
#define AUD_AC3_DEC2        (TYPE_AUD_DEC2)
#define AUD_MPEG_DEC2       (TYPE_AUD_DEC2+0x01)
#define AUD_AAC_DEC2        (TYPE_AUD_DEC2+0x02)
#define AUD_PCM_DEC2        (TYPE_AUD_DEC2+0x03)
#ifdef DSP_SUPPORT_NPTV
#define AUD_MTS_DEC2        (TYPE_AUD_DEC2+0x04)
#define AUD_PAL_DEC2        (TYPE_AUD_DEC2+0x05)
#define AUD_MINER_DEC2      (TYPE_AUD_DEC2+0x06)
#define AUD_A2DEC_DEC2      (TYPE_AUD_DEC2+0x07)
#define AUD_DETECTOR_DEC2   (TYPE_AUD_DEC2+0x08)
#define AUD_FMFM_DEC2       (TYPE_AUD_DEC2+0x09)
#ifdef ADSP_JPEG_DEC2
#define AUD_JPEG_DEC2       (TYPE_AUD_DEC2+0x0A)
#define DECODER2_MAX_INDEX  (AUD_JPEG_DEC2-TYPE_AUD_DEC2) //fixme when increasing decoders
#else
#define DECODER2_MAX_INDEX  (AUD_FMFM_DEC2)
#endif
//#define AUD_NICAM_DEC2    (TYPE_AUD_DEC2+0x0E)
//#define AUD_TTXACC_DEC2   (TYPE_AUD_DEC2+0x0F)
#else
#define DECODER2_MAX_INDEX  (AUD_PCM_DEC2)
#endif
#define AUD_COMM_DEC2     0x40

// mix sound Type (0x80~0xBf)
#define TYPE_AUD_MIX      0x80
#ifdef CC_MT5391_AUD_3_DECODER
//#if 0
#define AUD_MPEG_DEC3       (TYPE_AUD_MIX+0x00)
#define AUD_PCM_DEC3        (TYPE_AUD_MIX+0x01)
#define AUD_AAC_DEC3        (TYPE_AUD_MIX+0x02)
#define AUD_AC3_DEC3        (TYPE_AUD_MIX+0x03)

#define DECODER3_MAX_INDEX  (AUD_AC3_DEC3)
#endif
#define AUD_COMM_MIX      0x80

#ifdef DSP_SUPPORT_NPTV
#define TYPE_DSP_INT      0x90
#define DSP_INT_COMM      (TYPE_DSP_INT+0x00)
#define DSP_INT_NTSC      (TYPE_DSP_INT+0x01)
#define DSP_INT_PAL       (TYPE_DSP_INT+0x02)
#define DSP_INT_NICAM     (TYPE_DSP_INT+0x03)
#define DSP_INT_A2        (TYPE_DSP_INT+0x04)
#define DSP_INT_DETECTOR  (TYPE_DSP_INT+0x05)
#define DSP_INT_FMFM      (TYPE_DSP_INT+0x06)
#endif

// Common RAM
#define TYPE_AUD_CMM      0xC0
#define AUD_DDCO       0xF9
#ifdef  SIF_ADC
#define AUD_AININTRAM_MINER     0xFA
#endif
#define AUD_AININTRAM     0xFB  // Light added for 2nd cache (AintInt)
#define AUD_ROMCOD        0xFC
#define AUD_POSTRAM       0xFD
#define AUD_COMMRAM       0xFE
#define AUD_TRAP          0xFF  // Trap

// DSP check definition and variables
//#define ADSP_CHECK_RAM_CODE

#define DSP_NC_RAM_ROM				0x00000001
#define DSP_NC_RAM_POST				0x00000002
#define DSP_NC_RAM_COMM			       0x00000004
#define DSP_NC_RAM_DEC2				0x00000008
#define DSP_NC_RAM_AIN				       0x00000010
#define DSP_NC_RAM_DEC1				0x00000020

#define DSP_NC_TBL_AC3_DEC1			0x00000001
#define DSP_NC_TBL_MPEG_DEC1		       0x00000002
#define DSP_NC_TBL_AAC_DEC1			0x00000004
#define DSP_NC_TBL_MP3_DEC1			0x00000008
#define DSP_NC_TBL_WMA_N_DEC1		0x00000010
#define DSP_NC_TBL_WMA_C_DEC1		0x00000020
#define DSP_NC_TBL_VORBIS_N_DEC1		0x00000040
#define DSP_NC_TBL_VORBIS_C_DEC1		0x00000080
#define DSP_NC_TBL_MTS_DEC1			0x00000100

#define DSP_NC_TBL_AC3_DEC2			0x00010000
#define DSP_NC_TBL_MPEG_DEC2		       0x00020000
#define DSP_NC_TBL_AAC_DEC2			0x00040000
#define DSP_NC_TBL_MTS_DEC2			0x00080000


#define ADAC_CMD_FMT                0x01
#define ADAC_CMD_MUTE               0x02
#define ADAC_CMD_VOL                0x03
#define ADAC_CMD_EMPH               0x04
#define ADAC_CMD_DATA_FMT           0x05
#define ADAC_CMD_DAC_MUTE           0x06
#define ADAC_CMD_DSD_MODE           0x07
#define ADAC_SET_DSP_SPEED_DMPLL    0x08
#define ADAC_SET_DSP_SEEED_APLL     0x09
#define ADAC_CMD_AIN_SEL            0x10

//Code 3D
#define CODE3D_NONE                        0
#define CODE3D_VIRTUAL_SURROUND            1
#define CODE3D_PROLOGIC_II                 2
#define CODE3D_SRS_WOW                     3
#define CODE3D_SRS_TSXT                    4

//bonding related
#define BOND_2CH           (0x1)
#define BOND_INVALID       (0x1 << 15)

//for IEC max frequency
#define SV_48K                  1
#define SV_96K                  2
#define SV_192K                 3

/* MTS auto update use */
#define MTS_MONO      0x01
#define MTS_STEREO    0x02
#define MTS_SAP       0x04

/* Japan & Korea MPX */
#define MPX_MONO       0x08
#define MPX_STEREO     0x10
#define MPX_SUB        0x20
#define MPX_MAIN_SUB   0x40
#define MPX_MAIN       0x80

#define FM_MONO       0x01
#define A2_STEREO     0x02
#define A2_DUAL1      0x04
#define A2_DUAL2      0x08
#define NICAM_MONO    0x10
#define NICAM_STEREO  0x20
#define NICAM_DUAL1   0x40
#define NICAM_DUAL2   0x80

#define DSP_EN_CLK       0x1
#define DSP_SEND_INT     0x6
#define DSP_INT_SVC      0x7
#define DSP_UOP_SVC      0x8

#define _VDO_FLG_PORTING  1

#define FLASHC_SA         0
#define FLASHNC_SA        0
#define DRAMAC_SA         0
#define DRAMANC_SA        0
#define DRAMBC_SA         0
#define DRAMBNC_SA        0

#define EQUALIZER_BAND_NUM_1        9

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

// dsp state machine variable
typedef enum
{
    DSP_INIT,
    DSP_CK_INT,
    DSP_CK_SINT,
    DSP_RC_INIT,
    DSP_FLUSH_CACHE,
    DSP_WAIT_POWER_OFF,
    DSP_IDLE
}DSPSTATE;


#if (1) //def DSP_SUPPORT_SRSTSXT
/* SRS TSXT*/
enum SRSTSXT_MODE_LIST
{
	SV_A_SRSTSXT_OFF,
	SV_A_SRSTSXT_3DMONO,
	SV_A_SRSTSXT_STEREO
};
#endif

#ifdef DSP_SUPPORT_NPTV
// Copy From 8202 UI
/* Demodulaton Source Type */
enum 
{
	SV_SRC_AF,
	SV_SRC_SIF,
	SV_DEMOD_SRC_MAX
};

enum {
	SV_PAL_ALL_MODE,
	SV_PAL_FM_ONLY_MODE,
	SV_PAL_NICAM_ONLY_MODE
};

enum {
	SV_NICAM_DUAL_ALL,
	SV_NICAM_DUAL_A,
	SV_NICAM_DUAL_B
};

/* A2, NICAM */
enum {
	SV_FM_MONO,
	SV_A2_STEREO,
	SV_A2_DUAL1,
	SV_A2_DUAL2,
	SV_NICAM_MONO,
	SV_NICAM_STEREO,
	SV_NICAM_DUAL1,
	SV_NICAM_DUAL2,
	SV_A2_NICAM_MAX
};
//End of Copy From 8202 UI

#ifndef _TV_AUD_SYS_T
#define _TV_AUD_SYS_T
typedef enum {
	SV_NONE_DETECTED = 0x0,	    //0x0
	SV_MTS           = 0x1,     //0x1
	SV_FM_FM         = 0x2,     //0x2
	SV_NTSC_M        = 0x3,     //0x3
	SV_A2_BG         = 0x4,     //0x4
	SV_A2_DK1        = 0x5,     //0x5
	SV_A2_DK2        = 0x6,     //0x6
	SV_A2_DK3        = 0x7,     //0x7
	SV_PAL_I         = 0x8,     //0x8
	SV_PAL_BG        = 0x9,     //0x09
	SV_PAL_DK        = 0xa,     //0x0a
	SV_SECAM_L       = 0xb,     //0x0b
	SV_SECAM_L_PLUM  = 0xc,     //0x0c
	SV_SECAM_BG      = 0xd,	//0x0d, 0x0c is reserved for SECAM L' setting for tuner
	SV_SECAM_DK		 = 0xe      //0x0e
} TV_AUD_SYS_T;
#endif
#endif

// decoder1 & decoder2's bitstream type definition
#define COMMDEC_STREAM      0x00
#define AC3_STREAM          (COMMDEC_STREAM)
#define MPEG12_STREAM       (COMMDEC_STREAM+0x01)
#define AAC_STREAM          (COMMDEC_STREAM+0x02)
#define PCM_STREAM          (COMMDEC_STREAM+0x03)
#define MPEG3_STREAM        (COMMDEC_STREAM+0x04)
#define WMA_STREAM          (COMMDEC_STREAM+0x05)
#define VORBIS_STREAM       (COMMDEC_STREAM+0x06)
#define PINK_NOISE_STREAM   (COMMDEC_STREAM+0x07)
#ifdef DSP_SUPPORT_NPTV
#define NTSC_STREAM         (COMMDEC_STREAM+0x08)
#define PAL_STREAM          (COMMDEC_STREAM+0x09)
#define MINER_STREAM        (COMMDEC_STREAM+0x0A)
#define A2_STREAM           (COMMDEC_STREAM+0x0B)
#define DETECTOR_STREAM     (COMMDEC_STREAM+0x0C)
#define FMFM_STREAM         (COMMDEC_STREAM+0x0D)
//#define NICAM_STREAM        (COMMDEC_STREAM+0x0E)
//#define TTXACC_STREAM       (COMMDEC_STREAM+0x0F)
#endif
#define LPCM_STREAM             (COMMDEC_STREAM+0x0E)
#define FMRDO_STREAM          (COMMDEC_STREAM+0x0F)
#define FMRDODET_STREAM     (COMMDEC_STREAM+0x10)
#define SBC_STREAM          (COMMDEC_STREAM+0x11)
#ifdef ADSP_JPEG_DEC2
#define JPEG_STREAM         (COMMDEC_STREAM+0x12)
#endif

#define COMMMIX_STREAM      0x40

#define TRAP_STREAM               0xff

//detector related constant
#ifdef DSP_SUPPORT_NPTV
#define SOURCE_PAL_I	                0x01
#define SOURCE_PAL_BG	                0x02
#define SOURCE_PAL_DK	                0x04

#define SOURCE_SECAM_L	                0x0100
#define SOURCE_SECAM_BG	                0x0200
#define SOURCE_SECAM_DK	                0x0400

#define SOURCE_NTSC_M                   0x01
#define SOURCE_A2_BG                    0x02
#define SOURCE_A2_DK1                   0x04
#define SOURCE_A2_DK2                   0x08
#define SOURCE_A2_DK3                   0x10
#endif

/*--------------------- Data  Interface ----------------------*/
typedef struct 
{
    UINT16 wDspSpkCfg;
    UINT8 bFrntSpkrNm;
    UINT8 bSrndSpkrNm;
    BOOL fgIsSWExst;
    UINT8 bChNm;
}SPEAKER_SETTING_T;

typedef struct
{
    /* common code field*/
    UINT32 dwInputUnderRun;         /* error count for input underrun*/
    UINT32 dwInputUnderRunDec2;     /* error count for input underrun of decoder 2*/
#ifndef CC_MT5391_AUD_3_DECODER   
//#if 1
    UINT32 dwInputUnderRunMixSound; /* error count for input underrun of mixsound*/
#else
    UINT32 dwInputUnderRunDec3; /* error count for input underrun of decoder 3*/
#endif
    UINT32 dwOutputUnderRun;        /* error count for input output underrun*/
    UINT32 dwOutputUnderRunDec2;    /* error count for input output underrun of decoder2*/
#ifndef CC_MT5391_AUD_3_DECODER   
//#if 1    
    UINT32 dwOutputUnderRunMixSound;/* error count for input output underrun of mixsound*/
#else
    UINT32 dwOutputUnderRunDec3;/* error count for input output underrun of decoder 3*/
#endif
    UINT32 dwReserved0[9];          /* reserved field */
    /* first decoder's field*/
    UINT32 dwFrameRepeatCount;      /* repeat count for frames*/
    UINT32 dwFrameDropCount;        /* drop count for frames*/
    UINT32 dwTotalErrorCount;       /* total error count*/
    UINT32 dwTotalFrameCount;       /* total frame count*/
    UINT32 dwReserved1[3];          /* reserved field*/
    UINT32 dwDetailLogs[16] ;       /* detail log for decoder1*/
    /* second decoder's field*/
    UINT32 dwFrameRepeatCountDec2;  /* repeat count for frames*/
    UINT32 dwFrameDropCountDec2;    /* drop count for frames*/
    UINT32 dwTotalErrorCountDec2;   /* total error count*/
    UINT32 dwTotalFrameCountDec2;   /* total frame count*/
    UINT32 dwReserved2[3];          /* reserved field*/
    UINT32 dwDetailLogsDec2[16] ;   /* detail log for decoder2*/
#ifdef CC_MT5391_AUD_3_DECODER   
//#if 0
     /* third decoder's field*/
    UINT32 dwFrameRepeatCountDec3;  /* repeat count for frames*/
    UINT32 dwFrameDropCountDec3;    /* drop count for frames*/
    UINT32 dwTotalErrorCountDec3;   /* total error count*/
    UINT32 dwTotalFrameCountDec3;   /* total frame count*/
    UINT32 dwReserved3[3];          /* reserved field*/
    UINT32 dwDetailLogsDec3[16] ;   /* detail log for decoder2*/
#endif    
}ADSP_ERROR_LOG_T;


#ifndef __DSPSTATUS
#define __DSPSTATUS
typedef struct
{
    BOOL fgDspRealPlay;
    BOOL fgDspPlay;
    BOOL fgDspStop;
    BOOL fgDspPause;
    BOOL fgSendSample;
    BOOL fgDspFlush;
    BOOL fgDspStopIssue;
    BOOL fgDspPtsSet;
    BOOL fgSendPts2Dsp;
    BOOL fgDspForceReset;
    BOOL fgDspWakeUp;
    BOOL fgDACOn;
    BOOL fgDspReParsing;
    BOOL fgDspRInt;
    BOOL fgDspDelayParsing;
    BOOL fgSPLINEnable;
    BOOL fgDspFirstPlay;
    BOOL fgDspUop;
    BOOL fgDspSInt;
    BOOL fgVDCodeDwnld;
    BOOL fgMicSfreqOver;
    BOOL fgIECRAWOff;
    BOOL fgDspRamCodeLoad;
    BOOL fgDspForceResetDec;
    BOOL fgDspWakeUpOk;
    BOOL fgDspGetBitrate;
    BOOL fgDspBitrateGot;
    BOOL fgDspGetMpgTyp;
    BOOL fgDspMpgTypGot;
    BOOL fgIecMuteState;
    BOOL fgMicOnState;
    BOOL fgDspDetected;
    BOOL fgDspAoutMuted;
}DSPSTATUS_T;


typedef struct
{
    UINT32 dwDspFreq;
    UINT32 dwDspRamCodeType;
    UINT8 bDspStrTyp;
    UINT32 dwADacFreq;
    UINT32 dwStcDiff;
    UINT32 dwDspReInitPts;
    UINT32 dwTableFreq;
    UINT32 dwDacFreq;
    UINT8 bCode3D;
    UINT8 bCode3DUpmix;
    UINT32 dwDspSIntFail;
    UINT32 dwVolPrevUsr;
    UINT32 dwVolLastSetting;
    UINT32 dwVolSettingStep;
    UINT32 dwDspForceResetCnt;
    UINT32 dwDspBitrate;
    #ifdef DSP_SUPPORT_NPTV
    UINT8  bMtsSrcTyp;
    #endif
    #if (1) //defined(DSP_SUPPORT_SRSWOW) || defined(DSP_SUPPORT_SRSTSXT)
    UINT8  b3DSurrMode;
    #endif
// For mp4
    UINT32 dwDspMpgTyp;
// For speed control case
    UINT32 dwDspAckPTS;
    UINT32 dwDspPlaySpeed;
    UINT32 dwDspUop;
    UINT8 bDetectTyp; 
    SPEAKER_SETTING_T tSrcSpkCfg;
}DSPVARS_T;
#endif

// Top Level
typedef enum
{
    DSP_INIT_ON,
    DSP_INIT_WAIT,
    DSP_INIT_DONE
}DSPINITSTATE;

typedef struct 
{
    UINT8 u1IecOutBitNum;
    UINT8 u1IecChSel;
    BOOL fgIecMute;
    UINT8 u1IecDownSamp;
    UINT8 u1IecDecSel;
}AUDIO_CFG_T;

/* secondary decoder */
#define MAX_DSP_CMD_HIS_NS 40
typedef struct
{
    UINT32 u4Index;
    UINT32 u4Cmd[MAX_DSP_CMD_HIS_NS];
}DSP_CMD_HIS_T;

#define MAX_DSP_CMD_NS 5
typedef struct
{
    UINT8 ucWrIdx;
    UINT8 ucRdIdx;
    UINT8 ucCmdNs;
    UINT8 ucStatus;
    UINT32 u4Cmd[MAX_DSP_CMD_NS];
}DSP_CMD_T;

extern UINT32 _u4ShareMem;
extern UINT32 _u4DspBuf;
extern UINT32 _u4DspDramBuf[4];
extern UINT32 _u4DspCmptBuf[4];
extern UINT32 _u4DspCmptBuf_bk;       ///(ADSP_JPEG_DEC2)
extern UINT32 _u4AFIFO[2];
extern UINT32 _u4AFIFODec2[2];
extern UINT32 _u4MixSndBuf;

//#ifndef CC_MT5391_AUD_SUPPORT
#if 1  //added by ling , wait for Nilson
extern UINT32 _u4DspICacheAddr[6];
#else
extern UINT32 _u4DspICacheAddr[7];
#endif

extern BOOL _fgRamBoot;
extern UINT32 _u4DspSIntLD;
extern UINT8 _uDspSIntAddr;
extern UINT32 _u4DspSIntSD;
extern UINT8 _uDspRIntAddr;
extern UINT32 _u4DspRIntData;
extern UINT8 _uDspMixStrTyp;
extern UINT32 _u4PrsPts;
extern UINT32 _u4PrsPtsDec2;

extern volatile DSPSTATUS_T _rDspStatus;
extern volatile DSPSTATUS_T _rDspStatusDec2;
extern DSPVARS_T _rDspVars;
extern DSPVARS_T _rDspVarsDec2;
extern DSP_CMD_HIS_T _rDspCmdHistory;
#ifdef CC_MT5391_AUD_3_DECODER
//#if 0
extern volatile DSPSTATUS_T _rDspStatusDec3;
extern DSPVARS_T _rDspVarsDec3;
extern UINT32 _u4PrsPtsDec3;
#endif

#define _fgDspRealPlay      (_rDspStatus.fgDspRealPlay    )
#define _fgDspPlay          (_rDspStatus.fgDspPlay        )
#define _fgDspStop          (_rDspStatus.fgDspStop        )
#define _fgDspPause         (_rDspStatus.fgDspPause       )
#define _fgSendSample       (_rDspStatus.fgSendSample     )
#define _fgDspFlush         (_rDspStatus.fgDspFlush       )
#define _fgDspStopIssue     (_rDspStatus.fgDspStopIssue   )
#define _fgDspPtsSet        (_rDspStatus.fgDspPtsSet      )
#define _fgSendPts2Dsp      (_rDspStatus.fgSendPts2Dsp    )
#define _fgDspForceReset    (_rDspStatus.fgDspForceReset  )
#define _fgDspWakeUp        (_rDspStatus.fgDspWakeUp      )
#define _fgDACOn            (_rDspStatus.fgDACOn          )
#define _fgDspReParsing     (_rDspStatus.fgDspReParsing   )
#define _fgDspRInt          (_rDspStatus.fgDspRInt        )
#define _fgDspDelayParsing  (_rDspStatus.fgDspDelayParsing)
#define _fgDspFirstPlay     (_rDspStatus.fgDspFirstPlay   )
#define _fgDspUop           (_rDspStatus.fgDspUop         )
#define _fgDspSInt          (_rDspStatus.fgDspSInt        )
#define _fgVDCodeDwnld      (_rDspStatus.fgVDCodeDwnld    )
#define _fgMicSfreqOver     (_rDspStatus.fgMicSfreqOver   )
#define _fgIECRAWOff        (_rDspStatus.fgIECRAWOff      )
#define _fgDspRamCodeLoad   (_rDspStatus.fgDspRamCodeLoad )
#define _fgDspForceResetDec (_rDspStatus.fgDspForceResetDec)
#define _fgDspGetBitrate    (_rDspStatus.fgDspGetBitrate  )
#define _fgDspGetMpgTyp     (_rDspStatus.fgDspGetMpgTyp   )
#define _fgIecMuteState     (_rDspStatus.fgIecMuteState   )
#define _fgDspMpgTypGot     (_rDspStatus.fgDspMpgTypGot   )
#define _fgDspWakeupOK      (_rDspStatus.fgDspWakeUpOk    )
#define _fgDspDetected      (_rDspStatus.fgDspDetected    )
#define _fgDspAoutMuted     (_rDspStatus.fgDspAoutMuted   )

/* DSP control related variables */
#define _dwDspFreq           (_rDspVars.dwDspFreq         )
#define _dwDspRamCodeType    (_rDspVars.dwDspRamCodeType  )
#define _bDspStrTyp          (_rDspVars.bDspStrTyp        )
#define _dwADacFreq          (_rDspVars.dwADacFreq        )
#define _dwStcDiff           (_rDspVars.dwStcDiff         )
#define _dwDspReInitPts      (_rDspVars.dwDspReInitPts    )
#define _dwTableFreq         (_rDspVars.dwTableFreq       )
#define _dwDacFreq           (_rDspVars.dwDacFreq         )
#define _bCode3D             (_rDspVars.bCode3D           )
#define _bCode3DUpmix        (_rDspVars.bCode3DUpmix      )
#define _dwDspSIntFail       (_rDspVars.dwDspSIntFail     )
#define _dwVolPrevUsr        (_rDspVars.dwVolPrevUsr      )
#define _dwVolLastSetting    (_rDspVars.dwVolLastSetting  )
#define _dwVolSettingStep    (_rDspVars.dwVolSettingStep  )
#define _dwDspForceResetCntDec  (_rDspVars.dwDspForceResetCnt)
#define _dwDspBitrate        (_rDspVars.dwDspBitrate      )
#ifdef DSP_SUPPORT_NPTV
#define _bMtsSrcTyp          (_rDspVars.bMtsSrcTyp        )
#endif
#if (1) //defined(DSP_SUPPORT_SRSWOW) || defined(DSP_SUPPORT_SRSTSXT)
#define _b3DSurrMode         (_rDspVars.b3DSurrMode       )
#endif
#define _dwDspMpgTyp         (_rDspVars.dwDspMpgTyp       )
#define _dwDspAckPTS         (_rDspVars.dwDspAckPTS       )
#define _dwDspPlaySpeed      (_rDspVars.dwDspPlaySpeed    )
#define _dwDspUop            (_rDspVars.dwDspUop          )
#define _bDetectType         (_rDspVars.bDetectTyp)
#define _tSrcSpkCfg          (_rDspVars.tSrcSpkCfg        )

/* secondary decoder */
#define _fgDspRealPlayDec2   (_rDspStatusDec2.fgDspRealPlay)
#define _fgDspPlayDec2       (_rDspStatusDec2.fgDspPlay)
#define _fgDspStopDec2       (_rDspStatusDec2.fgDspStop)
#define _fgDspPauseDec2      (_rDspStatusDec2.fgDspPause)
#define _fgSendSampleDec2    (_rDspStatusDec2.fgSendSample)
#define _fgDspFlushDec2      (_rDspStatusDec2.fgDspFlush)
#define _fgDspStopIssueDec2  (_rDspStatusDec2.fgDspStopIssue)
#define _fgDspPtsSetDec2     (_rDspStatusDec2.fgDspPtsSet)
#define _fgSendPts2DspDec2   (_rDspStatusDec2.fgSendPts2Dsp)
#define _fgDspForceResetDec2 (_rDspStatusDec2.fgDspForceReset)
#define _fgDspReParsingDec2  (_rDspStatusDec2.fgDspReParsing)
#define _fgIECRAWOffDec2      (_rDspStatusDec2.fgIECRAWOff)
#define _fgDspRamCodeLoadDec2 (_rDspStatusDec2.fgDspRamCodeLoad)
#define _fgIecMuteStateDec2     (_rDspStatusDec2.fgIecMuteState)
#define _fgDspGetMpgTypDec2     (_rDspStatusDec2.fgDspGetMpgTyp)
#define _fgDspMpgTypGotDec2     (_rDspStatusDec2.fgDspMpgTypGot)
#define _fgDspAoutMutedDec2      (_rDspStatusDec2.fgDspAoutMuted    )

/* DSP control related variables */
#define _dwDspFreqDec2           (_rDspVarsDec2.dwDspFreq)
#define _dwDspRamCodeTypeDec2    (_rDspVarsDec2.dwDspRamCodeType)
#define _bDspStrTypDec2          (_rDspVarsDec2.bDspStrTyp)
#define _dwADacFreqDec2          (_rDspVarsDec2.dwADacFreq)
#define _dwStcDiffDec2           (_rDspVarsDec2.dwStcDiff)
#define _dwDspReInitPtsDec2      (_rDspVarsDec2.dwDspReInitPts)
#define _dwTableFreqDec2         (_rDspVarsDec2.dwTableFreq       )
#define _dwDacFreqDec2           (_rDspVarsDec2.dwDacFreq         )
#define _bCode3DDec2             (_rDspVarsDec2.bCode3D           )
#define _bCode3DUpmixDec2        (_rDspVarsDec2.bCode3DUpmix      )
#define _dwDspSIntFailDec2       (_rDspVarsDec2.dwDspSIntFail     )
#define _dwVolPrevUsrDec2        (_rDspVarsDec2.dwVolPrevUsr      )
#define _dwVolLastSettingDec2    (_rDspVarsDec2.dwVolLastSetting  )
#define _dwVolSettingStepDec2    (_rDspVarsDec2.dwVolSettingStep  )
#define _dwDspForceResetCntDec2  (_rDspVarsDec2.dwDspForceResetCnt)
#define _dwDspBitrateDec2        (_rDspVarsDec2.dwDspBitrate      )
#define _dwDspMpgTypDec2         (_rDspVarsDec2.dwDspMpgTyp       )
#define _dwDspAckPTSDec2         (_rDspVarsDec2.dwDspAckPTS       )
#define _dwDspPlaySpeedDec2      (_rDspVarsDec2.dwDspPlaySpeed    )
#define _bDetectTypeDec2         (_rDspVarsDec2.bDetectTyp)

#define _tSrcSpkCfgDec2          (_rDspVarsDec2.tSrcSpkCfg        )

#ifdef CC_MT5391_AUD_3_DECODER
//#if 0
/* third decoder */
#define _fgDspRealPlayDec3   (_rDspStatusDec3.fgDspRealPlay)
#define _fgDspPlayDec3       (_rDspStatusDec3.fgDspPlay)
#define _fgDspStopDec3       (_rDspStatusDec3.fgDspStop)
#define _fgDspPauseDec3      (_rDspStatusDec3.fgDspPause)
#define _fgSendSampleDec3    (_rDspStatusDec3.fgSendSample)
#define _fgDspFlushDec3      (_rDspStatusDec3.fgDspFlush)
#define _fgDspStopIssueDec3  (_rDspStatusDec3.fgDspStopIssue)
#define _fgDspPtsSetDec3     (_rDspStatusDec3.fgDspPtsSet)
#define _fgSendPts2DspDec3   (_rDspStatusDec3.fgSendPts2Dsp)
#define _fgDspForceResetDec3 (_rDspStatusDec3.fgDspForceReset)
#define _fgDspReParsingDec3  (_rDspStatusDec3.fgDspReParsing)
#define _fgIECRAWOffDec3      (_rDspStatusDec3.fgIECRAWOff)
#define _fgDspRamCodeLoadDec3 (_rDspStatusDec3.fgDspRamCodeLoad)
#define _fgIecMuteStateDec3     (_rDspStatusDec3.fgIecMuteState)
#define _fgDspGetMpgTypDec3     (_rDspStatusDec3.fgDspGetMpgTyp)
#define _fgDspMpgTypGotDec3     (_rDspStatusDec3.fgDspMpgTypGot)
#define _fgDspAoutMutedDec3      (_rDspStatusDec3.fgDspAoutMuted    )

/* DSP control related variables */
#define _dwDspFreqDec3           (_rDspVarsDec3.dwDspFreq)
#define _dwDspRamCodeTypeDec3    (_rDspVarsDec3.dwDspRamCodeType)
#define _bDspStrTypDec3          (_rDspVarsDec3.bDspStrTyp)
#define _dwADacFreqDec3          (_rDspVarsDec3.dwADacFreq)
#define _dwStcDiffDec3           (_rDspVarsDec3.dwStcDiff)
#define _dwDspReInitPtsDec3      (_rDspVarsDec3.dwDspReInitPts)
#define _dwTableFreqDec3         (_rDspVarsDec3.dwTableFreq       )
#define _dwDacFreqDec3           (_rDspVarsDec3.dwDacFreq         )
#define _bCode3DDec3             (_rDspVarsDec3.bCode3D           )
#define _bCode3DUpmixDec3        (_rDspVarsDec3.bCode3DUpmix      )
#define _dwDspSIntFailDec3       (_rDspVarsDec3.dwDspSIntFail     )
#define _dwVolPrevUsrDec3        (_rDspVarsDec3.dwVolPrevUsr      )
#define _dwVolLastSettingDec3    (_rDspVarsDec3.dwVolLastSetting  )
#define _dwVolSettingStepDec3    (_rDspVarsDec3.dwVolSettingStep  )
#define _dwDspForceResetCntDec3  (_rDspVarsDec3.dwDspForceResetCnt)
#define _dwDspBitrateDec3        (_rDspVarsDec3.dwDspBitrate      )
#define _dwDspMpgTypDec3         (_rDspVarsDec3.dwDspMpgTyp       )
#define _dwDspAckPTSDec3         (_rDspVarsDec3.dwDspAckPTS       )
#define _dwDspPlaySpeedDec3      (_rDspVarsDec3.dwDspPlaySpeed    )
#define _bDetectTypeDec3         (_rDspVarsDec3.bDetectTyp)

#define _tSrcSpkCfgDec3          (_rDspVarsDec3.tSrcSpkCfg        )
#endif

// FIXME
#ifndef CC_MT5387
#define SRAM_DECODER_CONTROL       (0x980)
#define SRAM_DECODER_CONTROL_DEC2  (0x3680)
#else
#define SRAM_DECODER_CONTROL       (0x1500)
#define SRAM_DECODER_CONTROL_DEC2  (0x2D80) ///[Joel](0x2a80)
#endif

#endif // DSP_COMMON_H

