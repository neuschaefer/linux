/*------------------------------------------------------------------------------

 Copyright 2010 Sony Corporation

 This is UNPUBLISHED PROPRIETARY SOURCE CODE of Sony Corporation.
 No part of this file may be copied, modified, sold, and distributed in any
 form or by any means without prior explicit permission in writing from
 Sony Corporation.

 Date: 2010/10/04
 Revision: 1.0.2.0

------------------------------------------------------------------------------*/

#ifndef RB211_TUNER_H
#define RB211_TUNER_H

#include "rb211_atv.h"
#include "rb211_dtv.h"

/*------------------------------------------------------------------------------
  Defines
------------------------------------------------------------------------------*/
/* I2C slave address of the tuner */
#define RB211_TUNER_ADDRESS     0xC0

/*------------------------------------------------------------------------------
  Enums
------------------------------------------------------------------------------*/
/* Tuner state */
typedef enum {
	RB211_TUNER_STATE_UNKNOWN,
	RB211_TUNER_STATE_SLEEP,
	RB211_TUNER_STATE_ACTIVE
} rb211_tuner_state_t;

/*------------------------------------------------------------------------------
  Structs
------------------------------------------------------------------------------*/
typedef struct rb211_tuner_t {
	rb211_tuner_state_t   state;
	UINT32             xtalFreq;
	UINT8              i2cAddress;
	rb211_tuner_i2c_t*          pI2c;
	UINT32             flags;

	/* For saving current setting */
	UINT32             frequency;
	rb211_atv_video_t     atvVideoID;
	rb211_atv_audio_t     atvAudioID;
	rb211_dtv_system_t    dtvSystem;

	/* Special value for Korea (System-M) */
	UINT8              specialValueForKorea;
	void*                user;
} rb211_tuner_t;

/* Config flag definitions (Apply as mask to "flags" member fo rb211_tuner_t struct) */
#define RB211_TUNER_CONFIG_EXT_REF         0x80000000  /* Use external XTAL (Ref input). */
#define RB211_TUNER_CONFIG_SYSTEML_REGION  0x40000000  /* System is used in System-L region. */

#define RB211_TUNER_CONFIG_IFAGCSEL_ALL1   0x00000000  /* IFOUT/AGCIN 1 is used (default) */
#define RB211_TUNER_CONFIG_IFAGCSEL_ALL2   0x10000000  /* IFOUT/AGCIN 2 is used */
#define RB211_TUNER_CONFIG_IFAGCSEL_A1D2   0x20000000  /* IFOUT/AGCIN 1 is for Analog, 2 is for Digital */
#define RB211_TUNER_CONFIG_IFAGCSEL_D1A2   0x30000000  /* IFOUT/AGCIN 1 is for Digital, 2 is for Analog */
#define RB211_TUNER_CONFIG_IFAGCSEL_MASK   0x30000000

/* ONLY for ASCOT2D */
#define RB211_TUNER_CONFIG_NVMSEL_0_1      0x00000000  /* NVM Bank 0/1 (default) */
#define RB211_TUNER_CONFIG_NVMSEL_0_3      0x01000000  /* NVM Bank 0/3 */
#define RB211_TUNER_CONFIG_NVMSEL_AUTO     0x02000000  /* NVM Bank Auto Select */
#define RB211_TUNER_CONFIG_NVMSEL_MASK     0x03000000

/*------------------------------------------------------------------------------
  APIs
------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------
  Tuner instance creation

  rb211_tuner_t     *pTuner      Tuner struct instance
  UINT32         xtalFreq     Xtal frequency for silicon tuner
  UINT8          i2cAddress   I2c slave address of the tuner
  rb211_tuner_i2c_t       *pI2c        I2c struct instance
  UINT32         flags        Configuration flags
--------------------------------------------------------------------*/
rb211_result_t rb211_tuner_Create(rb211_tuner_t *pTuner, UINT32 xtalFreq,
								UINT8 i2cAddress, rb211_tuner_i2c_t *pI2c, UINT32 flags);

/*--------------------------------------------------------------------
  Tuner initialization

  rb211_tuner_t     *pTuner      Tuner struct instance
--------------------------------------------------------------------*/
rb211_result_t rb211_tuner_Initialize(rb211_tuner_t *pTuner);

/*--------------------------------------------------------------------
  Tuning to analog channel

  rb211_tuner_t     *pTuner      Tuner struct instance
  UINT32         frequency    RF FP frequency (kHz)
  rb211_atv_video_t videoID      Video system ID (defined in rb211_atv.h)
  rb211_atv_audio_t audioID      Audio system ID (defined in rb211_atv.h)
--------------------------------------------------------------------*/
rb211_result_t rb211_tuner_AnalogTune(rb211_tuner_t *pTuner, UINT32 frequency,
									rb211_atv_video_t videoID, rb211_atv_audio_t audioID);

/*--------------------------------------------------------------------
  Tuning to analog channel
  NOTE: rb211_tuner_AnalogTune -> wait 100ms -> rb211_tuner_AnalogAfterTune

  rb211_tuner_t     *pTuner      Tuner struct instance
--------------------------------------------------------------------*/
rb211_result_t rb211_tuner_AnalogAfterTune(rb211_tuner_t *pTuner);

/*--------------------------------------------------------------------
  Tuning to digital channel

  rb211_tuner_t      *pTuner      Tuner struct instance
  UINT32          frequency    RF center frequency (kHz)
  rb211_dtv_system_t system       Broadcast system ID (defined in rb211_dtv.h)
--------------------------------------------------------------------*/
rb211_result_t rb211_tuner_DigitalTune(rb211_tuner_t *pTuner, UINT32 frequency, rb211_dtv_system_t system);

/*--------------------------------------------------------------------
  Tuner sleep

  rb211_tuner_t     *pTuner      Tuner struct instance
--------------------------------------------------------------------*/
rb211_result_t rb211_tuner_Sleep(rb211_tuner_t *pTuner);

/*--------------------------------------------------------------------
  Tuner GPO ctrl

  rb211_tuner_t     *pTuner      Tuner struct instance
  UINT8          id           GPIO id (0 / 1)
  UINT8          val          Lo(0) or Hi(1)
--------------------------------------------------------------------*/
rb211_result_t rb211_tuner_GPOCtrl(rb211_tuner_t *pTuner, UINT8 id, UINT8 val);

/*--------------------------------------------------------------------
  Prepare special value for analog tuning in Korea

  This function should be called if the system support analog tuning in Korea.

  rb211_tuner_t     *pTuner      Tuner struct instance
--------------------------------------------------------------------*/
rb211_result_t rb211_tuner_PrepareForKorea(rb211_tuner_t *pTuner);

/*--------------------------------------------------------------------
  (ONLY for ASCOT2D)
  RF filter compensation setting for VHF-L band.
  (Please see RFVGA Description of datasheet.)
  New setting will become effective after next tuning.

  mult = VL_TRCKOUT_COEFF(8bit unsigned) / 128
  ofs  = VL_TRCKOUT_OFS(8bit 2s complement)
  (compensated value) = (original value) * mult + ofs

  rb211_tuner_t     *pTuner      Tuner struct instance
  UINT8          coeff        VL_TRCKOUT_COEFF (multiplier)
  UINT8          offset       VL_TRCKOUT_OFS (additional term)
--------------------------------------------------------------------*/
rb211_result_t rb211_tuner_RFFilterConfig(rb211_tuner_t *pTuner, UINT8 coeff, UINT8 offset);

#endif /* RB211_TUNER_H */
