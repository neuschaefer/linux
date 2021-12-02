/*------------------------------------------------------------------------------

 Copyright 2010-2011 Sony Corporation

 This is UNPUBLISHED PROPRIETARY SOURCE CODE of Sony Corporation.
 No part of this file may be copied, modified, sold, and distributed in any
 form or by any means without prior explicit permission in writing from
 Sony Corporation.

 Date: 2011/04/27
 Revision: 1.1.0.0

------------------------------------------------------------------------------*/

#ifndef RE215_ASCOT_COMMON_H
#define RE215_ASCOT_COMMON_H

#include "re215_common.h"
#include "re215_i2c.h"

/*------------------------------------------------------------------------------
  Defines
------------------------------------------------------------------------------*/
/* Default I2C slave address of the ASCOT tuner */
#define RE215_ASCOT_ADDRESS     0xC0

/*------------------------------------------------------------------------------
  Enums
------------------------------------------------------------------------------*/
/* ASCOT tuner state */
typedef enum {
	RE215_ASCOT_STATE_UNKNOWN,
	RE215_ASCOT_STATE_SLEEP,
	RE215_ASCOT_STATE_ACTIVE
} re215_ascot_state_t;

/*
  <PORTING>
  These ID definitions depend on CXD2823ER Sony analog terrestrial demodulator.
  The user may need to change these values if other demodulator is used.
*/

/* Analog terrestrial video ID definition supported by ASCOT */
typedef enum {
	RE215_ASCOT_ATV_VIDEO_UNKNOWN  = 0x00,
	RE215_ASCOT_ATV_VIDEO_M        = 0x01,       /* System-M */
	RE215_ASCOT_ATV_VIDEO_BG       = 0x03,       /* System-BG */
	RE215_ASCOT_ATV_VIDEO_DK       = 0x04,       /* System-DK */
	RE215_ASCOT_ATV_VIDEO_I        = 0x05,       /* System-I */
	RE215_ASCOT_ATV_VIDEO_L        = 0x06,       /* System-L */
	RE215_ASCOT_ATV_VIDEO_L_DASH   = 0x07,       /* System-L DASH */
	RE215_ASCOT_ATV_VIDEO_B_AUS    = 0x0A,       /* System-B (Australia) */
	RE215_ASCOT_ATV_VIDEO_DK_CHINA = 0x0B        /* System-DK (China) */
} re215_ascot_atv_video_t;

/* Analog terrestrial audio ID definition supported by ASCOT */
typedef enum {
	RE215_ASCOT_ATV_AUDIO_UNKNOWN            = 0x00,
	RE215_ASCOT_ATV_AUDIO_BTSC_STEREO        = 0x01,   /* BTSC STEREO */
	RE215_ASCOT_ATV_AUDIO_M_DUAL_FM_STEREO   = 0x02,   /* M DUAL FM STEREO */
	RE215_ASCOT_ATV_AUDIO_M_EIA_J_STEREO     = 0x03,   /* M EIA J STEREO */
	RE215_ASCOT_ATV_AUDIO_N_BTSC             = 0x04,   /* N BTSC */
	RE215_ASCOT_ATV_AUDIO_BG_DUAL_FM_STEREO  = 0x05,   /* BG DUAL FM STEREO */
	RE215_ASCOT_ATV_AUDIO_BG_NICAM_FM        = 0x06,   /* BG NICAM FM */
	RE215_ASCOT_ATV_AUDIO_DK1_DUAL_FM_STEREO = 0x08,   /* DK1 DUAL FM STEREO */
	RE215_ASCOT_ATV_AUDIO_DK2_DUAL_FM_STEREO = 0x09,   /* DK2 DUAL FM STEREO */
	RE215_ASCOT_ATV_AUDIO_DK3_DUAL_FM_STEREO = 0x0A,   /* DK3 DUAL FM STEREO */
	RE215_ASCOT_ATV_AUDIO_DK_NICAM_FM        = 0x0B,   /* DK NICAM FM */
	RE215_ASCOT_ATV_AUDIO_I_NICAM_FM         = 0x0C,   /* I NICAM FM */
	RE215_ASCOT_ATV_AUDIO_L_NICAM_AM         = 0x0D    /* L NICAM AM */
} re215_ascot_atv_audio_t;

/* Digital terrestrial system definitions supported by ASCOT */
typedef enum {
	RE215_ASCOT_DTV_SYSTEM_UNKNOWN,
	RE215_ASCOT_DTV_SYSTEM_QAM,      /* QAM */
	RE215_ASCOT_DTV_SYSTEM_ATSC,     /* ATSC(8VSB) */
	RE215_ASCOT_DTV_SYSTEM_ISDBT,    /* ISDB-T */
	RE215_ASCOT_DTV_SYSTEM_DVBT_5,   /* DVB-T 5MHzBW */
	RE215_ASCOT_DTV_SYSTEM_DVBT_6,   /* DVB-T 6MHzBW */
	RE215_ASCOT_DTV_SYSTEM_DVBT_7,   /* DVB-T 7MHzBW */
	RE215_ASCOT_DTV_SYSTEM_DVBT_8,   /* DVB-T 8MHzBW */
	RE215_ASCOT_DTV_SYSTEM_DVBT2_5,  /* DVB-T2 5MHzBW */
	RE215_ASCOT_DTV_SYSTEM_DVBT2_6,  /* DVB-T2 6MHzBW */
	RE215_ASCOT_DTV_SYSTEM_DVBT2_7,  /* DVB-T2 7MHzBW */
	RE215_ASCOT_DTV_SYSTEM_DVBT2_8,  /* DVB-T2 8MHzBW */
	RE215_ASCOT_DTV_SYSTEM_DVBC,     /* DVB-C */
	RE215_ASCOT_DTV_SYSTEM_DVBC2_6,  /* DVB-C2 6MHzBW */
	RE215_ASCOT_DTV_SYSTEM_DVBC2_8,  /* DVB-C2 8MHzBW */
	RE215_ASCOT_DTV_SYSTEM_DTMB      /* DTMB */
} re215_ascot_dtv_system_t;

/*------------------------------------------------------------------------------
  Structs
------------------------------------------------------------------------------*/
typedef struct re215_ascot_t {
	re215_ascot_state_t      state;
	uint32_t                xtalFreq;
	uint8_t                 i2cAddress;
	re215_i2c_t*             pI2c;
	uint32_t                flags;

	/* For saving current setting */
	uint32_t                frequency;
	re215_ascot_atv_video_t  atvVideoID;
	re215_ascot_atv_audio_t  atvAudioID;
	re215_ascot_dtv_system_t dtvSystem;

	/* Special value for Korea (System-M) */
	uint8_t                 specialValueForKorea;
	void*                   user;
} re215_ascot_t;

/* Config flag definitions (Apply as mask to "flags" member fo re215_ascot_t struct) */
#define RE215_ASCOT_CONFIG_EXT_REF         0x80000000  /* Use external XTAL (Ref input). */
#define RE215_ASCOT_CONFIG_SYSTEML_REGION  0x40000000  /* System is used in System-L region. */
#define RE215_ASCOT_CONFIG_LOW_PN          0x20000000  /* Low PN component setting. */

#define RE215_ASCOT_CONFIG_IFAGCSEL_ALL1   0x00000000  /* IFOUT/AGCIN 1 is used (default) */
#define RE215_ASCOT_CONFIG_IFAGCSEL_ALL2   0x04000000  /* IFOUT/AGCIN 2 is used */
#define RE215_ASCOT_CONFIG_IFAGCSEL_A1D2   0x08000000  /* IFOUT/AGCIN 1 is for Analog, 2 is for Digital */
#define RE215_ASCOT_CONFIG_IFAGCSEL_D1A2   0x0C000000  /* IFOUT/AGCIN 1 is for Digital, 2 is for Analog */
#define RE215_ASCOT_CONFIG_IFAGCSEL_MASK   0x0C000000

/* ONLY for ASCOT2D */
#define RE215_ASCOT_CONFIG_NVMSEL_0_1      0x00000000  /* NVM Bank 0/1 (default) */
#define RE215_ASCOT_CONFIG_NVMSEL_0_3      0x01000000  /* NVM Bank 0/3 */
#define RE215_ASCOT_CONFIG_NVMSEL_AUTO     0x02000000  /* NVM Bank Auto Select */
#define RE215_ASCOT_CONFIG_NVMSEL_MASK     0x03000000

#endif /* RE215_ASCOT_COMMON_H */
