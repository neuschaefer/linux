/*------------------------------------------------------------------------------

 Copyright 2010-2011 Sony Corporation

 This is UNPUBLISHED PROPRIETARY SOURCE CODE of Sony Corporation.
 No part of this file may be copied, modified, sold, and distributed in any
 form or by any means without prior explicit permission in writing from
 Sony Corporation.

 Date: 2011/04/26
 Revision: 1.0.0.0

------------------------------------------------------------------------------*/

#ifndef RE215_ASCOT2D_H
#define RE215_ASCOT2D_H

#include "re215_ascot_common.h"

/*------------------------------------------------------------------------------
  APIs
------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------
  Tuner instance creation

  re215_ascot_t     *pTuner      Tuner struct instance
  uint32_t         xtalFreq     Xtal frequency for silicon tuner
  uint8_t          i2cAddress   I2c slave address of the tuner
  re215_i2c_t       *pI2c        I2c struct instance
  uint32_t         flags        Configuration flags
--------------------------------------------------------------------*/
re215_result_t re215_ascot2d_Create(re215_ascot_t *pTuner, uint32_t xtalFreq,
	uint8_t i2cAddress, re215_i2c_t *pI2c, uint32_t flags);

/*--------------------------------------------------------------------
  Tuner initialization

  re215_ascot_t     *pTuner      Tuner struct instance
--------------------------------------------------------------------*/
re215_result_t re215_ascot2d_Initialize(re215_ascot_t *pTuner);

/*--------------------------------------------------------------------
  Tuning to analog channel

  re215_ascot_t     *pTuner      Tuner struct instance
  uint32_t         frequency    RF FP frequency (kHz)
  re215_ascot_atv_video_t videoID      Video system ID (defined in re215_atv.h)
  re215_ascot_atv_audio_t audioID      Audio system ID (defined in re215_atv.h)
--------------------------------------------------------------------*/
re215_result_t re215_ascot2d_AnalogTune(re215_ascot_t *pTuner, uint32_t frequency,
	re215_ascot_atv_video_t videoID, re215_ascot_atv_audio_t audioID);

/*--------------------------------------------------------------------
  Tuning to analog channel
  NOTE: re215_ascot2d_AnalogTune -> wait 100ms -> re215_ascot2d_AnalogAfterTune

  re215_ascot_t     *pTuner      Tuner struct instance
--------------------------------------------------------------------*/
re215_result_t re215_ascot2d_AnalogAfterTune(re215_ascot_t *pTuner);

/*--------------------------------------------------------------------
  Tuning to digital channel

  re215_ascot_t            *pTuner      Tuner struct instance
  uint32_t                frequency    RF center frequency (kHz)
  re215_ascot_dtv_system_t system       Broadcast system ID
--------------------------------------------------------------------*/
re215_result_t re215_ascot2d_DigitalTune(re215_ascot_t *pTuner, uint32_t frequency, re215_ascot_dtv_system_t system);

/*--------------------------------------------------------------------
  Tuner sleep

  re215_ascot_t     *pTuner      Tuner struct instance
--------------------------------------------------------------------*/
re215_result_t re215_ascot2d_Sleep(re215_ascot_t *pTuner);

/*--------------------------------------------------------------------
  Tuner GPO ctrl

  re215_ascot_t     *pTuner      Tuner struct instance
  uint8_t          id           GPIO id (0 / 1)
  uint8_t          val          Lo(0) or Hi(1)
--------------------------------------------------------------------*/
re215_result_t re215_ascot2d_GPOCtrl(re215_ascot_t *pTuner, uint8_t id, uint8_t val);

/*--------------------------------------------------------------------
  Prepare special value for analog tuning in Korea

  This function should be called if the system support analog tuning in Korea.

  re215_ascot_t     *pTuner      Tuner struct instance
--------------------------------------------------------------------*/
re215_result_t re215_ascot2d_PrepareForKorea(re215_ascot_t *pTuner);

/*--------------------------------------------------------------------
  RF filter compensation setting for VHF-L band.
  (Please see RFVGA Description of datasheet.)
  New setting will become effective after next tuning.

  mult = VL_TRCKOUT_COEFF(8bit unsigned) / 128
  ofs  = VL_TRCKOUT_OFS(8bit 2s complement)
  (compensated value) = (original value) * mult + ofs

  re215_ascot_t     *pTuner      Tuner struct instance
  uint8_t          coeff        VL_TRCKOUT_COEFF (multiplier)
  uint8_t          offset       VL_TRCKOUT_OFS (additional term)
--------------------------------------------------------------------*/
re215_result_t re215_ascot2d_RFFilterConfig(re215_ascot_t *pTuner, uint8_t coeff, uint8_t offset);

#endif /* RE215_ASCOT2D_H */
