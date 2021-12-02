/*------------------------------------------------------------------------------

 Copyright 2010 Sony Corporation

 This is UNPUBLISHED PROPRIETARY SOURCE CODE of Sony Corporation.
 No part of this file may be copied, modified, sold, and distributed in any
 form or by any means without prior explicit permission in writing from
 Sony Corporation.

 Date: 2010/07/05
 Revision: 1.0.1.0

------------------------------------------------------------------------------*/

#ifndef RB211_ATV_H
#define RB211_ATV_H

#include "rb211_tuner_common.h"
#include "rb211_tuner_i2c.h"

/*
  <PORTING>
  These ID definitions depend on CXD2823ER  analog terrestrial demodulator.
  The user may need to change these values if other demodulator is used.
*/

/* Video ID definition */
typedef enum {
	RB211_ATV_VIDEO_UNKNOWN  = 0x00,
	RB211_ATV_VIDEO_M        = 0x01,       /* System-M */
	RB211_ATV_VIDEO_BG       = 0x03,       /* System-BG */
	RB211_ATV_VIDEO_DK       = 0x04,       /* System-DK */
	RB211_ATV_VIDEO_I        = 0x05,       /* System-I */
	RB211_ATV_VIDEO_L        = 0x06,       /* System-L */
	RB211_ATV_VIDEO_L_DASH   = 0x07,       /* System-L DASH */
	RB211_ATV_VIDEO_B_AUS    = 0x0A,       /* System-B (Australia) */
	RB211_ATV_VIDEO_DK_CHINA = 0x0B        /* System-DK (China) */
} rb211_atv_video_t;

/* Audio ID definition */
typedef enum {
	RB211_ATV_AUDIO_UNKNOWN            = 0x00,
	RB211_ATV_AUDIO_BTSC_STEREO        = 0x01,   /* BTSC STEREO */
	RB211_ATV_AUDIO_M_DUAL_FM_STEREO   = 0x02,   /* M DUAL FM STEREO */
	RB211_ATV_AUDIO_M_EIA_J_STEREO     = 0x03,   /* M EIA J STEREO */
	RB211_ATV_AUDIO_N_BTSC             = 0x04,   /* N BTSC */
	RB211_ATV_AUDIO_BG_DUAL_FM_STEREO  = 0x05,   /* BG DUAL FM STEREO */
	RB211_ATV_AUDIO_BG_NICAM_FM        = 0x06,   /* BG NICAM FM */
	RB211_ATV_AUDIO_DK1_DUAL_FM_STEREO = 0x08,   /* DK1 DUAL FM STEREO */
	RB211_ATV_AUDIO_DK2_DUAL_FM_STEREO = 0x09,   /* DK2 DUAL FM STEREO */
	RB211_ATV_AUDIO_DK3_DUAL_FM_STEREO = 0x0A,   /* DK3 DUAL FM STEREO */
	RB211_ATV_AUDIO_DK_NICAM_FM        = 0x0B,   /* DK NICAM FM */
	RB211_ATV_AUDIO_I_NICAM_FM         = 0x0C,   /* I NICAM FM */
	RB211_ATV_AUDIO_L_NICAM_AM         = 0x0D    /* L NICAM AM */
} rb211_atv_audio_t;

#endif /* RB211_ATV_H */
