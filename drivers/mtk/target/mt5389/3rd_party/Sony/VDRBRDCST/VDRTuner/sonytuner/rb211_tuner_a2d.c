/*------------------------------------------------------------------------------

 Copyright 2010 Sony Corporation

 This is UNPUBLISHED PROPRIETARY SOURCE CODE of Sony Corporation.
 No part of this file may be copied, modified, sold, and distributed in any
 form or by any means without prior explicit permission in writing from
 Sony Corporation.

 Date: 2010/12/06
 Revision: 1.0.4.0

------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------
 Based on ASCOT2D application note 2.2.0
------------------------------------------------------------------------------*/

#include "rb211_tuner.h"

/*------------------------------------------------------------------------------
 Defines
------------------------------------------------------------------------------*/
#define WAITCPUIDLE_POLL_INTERVAL_MS            10      /* Polling interval (in ms). */
#define WAITCPUIDLE_TIMEOUT_MS                  1000    /* Wait for CPU idle (in ms). */

#ifdef RB211_TUNER_MODEL_DYNAMIC_SWITCH
/* NOTE: This TRICK is for A2S/A2D dynamically configuration.                      */
/*       Generally, the user don't have to define RB211_TUNER_MODEL_DYNAMIC_SWITCH. */
/*       (Just select to build rb211_tuner_a2s.c or rb211_tuner_a2d.c.)              */
/*       Please check rb211_tuner_model_dynamic_switch.c.                           */
#define rb211_tuner_Create             rb211_tuner_a2d_Create
#define rb211_tuner_Initialize         rb211_tuner_a2d_Initialize
#define rb211_tuner_AnalogTune         rb211_tuner_a2d_AnalogTune
#define rb211_tuner_AnalogAfterTune    rb211_tuner_a2d_AnalogAfterTune
#define rb211_tuner_DigitalTune        rb211_tuner_a2d_DigitalTune
#define rb211_tuner_Sleep              rb211_tuner_a2d_Sleep
#define rb211_tuner_GPOCtrl            rb211_tuner_a2d_GPOCtrl
#define rb211_tuner_PrepareForKorea    rb211_tuner_a2d_PrepareForKorea
#endif /* RB211_TUNER_MODEL_DYNAMIC_SWITCH */

/*------------------------------------------------------------------------------
 Definitions of static functions
------------------------------------------------------------------------------*/
static rb211_result_t X_pon(rb211_tuner_t *pTuner);
static rb211_result_t A_init(rb211_tuner_t *pTuner, UINT8 ifagcsel, rb211_atv_video_t videoID,
							int isDvbC);
static rb211_result_t D_init(rb211_tuner_t *pTuner, UINT8 ifagcsel, rb211_dtv_system_t system);
static rb211_result_t A_tune(rb211_tuner_t *pTuner, UINT32 frequency,
							rb211_atv_video_t videoID, rb211_atv_audio_t audioID);
static rb211_result_t A_tune2(rb211_tuner_t *pTuner);
static rb211_result_t D_tune(rb211_tuner_t *pTuner, UINT32 frequency,
							rb211_dtv_system_t system);
static rb211_result_t X_fin(rb211_tuner_t *pTuner);
static rb211_result_t WaitCpuIdle(rb211_tuner_t *pTuner);

static rb211_result_t X_tune_start(rb211_tuner_t *pTuner); /* Called from A_tune/D_tune */
static rb211_result_t X_tune_end(rb211_tuner_t *pTuner);   /* Called from A_tune/D_tune */

/*------------------------------------------------------------------------------
 Implementation
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
								UINT8 i2cAddress, rb211_tuner_i2c_t *pI2c, UINT32 flags)
{
	RB211_TRACE_ENTER("rb211_tuner_Create");

	if((!pTuner) || (!pI2c)){
		RB211_TRACE_RETURN(RB211_RESULT_ERROR_ARG);
	}

	pTuner->state = RB211_TUNER_STATE_UNKNOWN; /* Chip is not accessed for now */
	pTuner->xtalFreq = xtalFreq;
	pTuner->pI2c = pI2c;
	pTuner->i2cAddress = i2cAddress;
	pTuner->flags = flags;
	pTuner->frequency = 0;
	pTuner->atvVideoID = RB211_ATV_VIDEO_UNKNOWN;
	pTuner->atvAudioID = RB211_ATV_AUDIO_UNKNOWN;
	pTuner->dtvSystem = RB211_DTV_SYSTEM_UNKNOWN;
	pTuner->specialValueForKorea = 0x80;
	pTuner->user = NULL;

	RB211_TRACE_RETURN(RB211_RESULT_OK);
}

/*--------------------------------------------------------------------
  Tuner initialization

  rb211_tuner_t     *pTuner      Tuner struct instance
--------------------------------------------------------------------*/
rb211_result_t rb211_tuner_Initialize(rb211_tuner_t *pTuner)
{
	rb211_result_t result = RB211_RESULT_OK;

	RB211_TRACE_ENTER("rb211_tuner_Initialize");

	if((!pTuner) || (!pTuner->pI2c)){
		RB211_TRACE_RETURN(RB211_RESULT_ERROR_ARG);
	}

	/* X_pon sequence */
	result = X_pon(pTuner);
	if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(result); }

	pTuner->state = RB211_TUNER_STATE_SLEEP;
	pTuner->frequency = 0;
	pTuner->atvVideoID = RB211_ATV_VIDEO_UNKNOWN;
	pTuner->atvAudioID = RB211_ATV_AUDIO_UNKNOWN;
	pTuner->dtvSystem = RB211_DTV_SYSTEM_UNKNOWN;

	RB211_TRACE_RETURN(RB211_RESULT_OK);
}

/*--------------------------------------------------------------------
  Tuning to analog channel

  rb211_tuner_t     *pTuner      Tuner struct instance
  UINT32         frequency    RF FP frequency (kHz)
  rb211_atv_video_t videoID      Video system ID (defined in rb211_atv.h)
  rb211_atv_audio_t audioID      Audio system ID (defined in rb211_atv.h)
--------------------------------------------------------------------*/
rb211_result_t rb211_tuner_AnalogTune(rb211_tuner_t *pTuner, UINT32 frequency,
									rb211_atv_video_t videoID, rb211_atv_audio_t audioID)
{
	rb211_result_t result = RB211_RESULT_OK;

	RB211_TRACE_ENTER("rb211_tuner_AnalogTune");

	if((!pTuner) || (!pTuner->pI2c)){
		RB211_TRACE_RETURN(RB211_RESULT_ERROR_ARG);
	}

	if((pTuner->state != RB211_TUNER_STATE_SLEEP) && (pTuner->state != RB211_TUNER_STATE_ACTIVE)){
		RB211_TRACE_RETURN(RB211_RESULT_ERROR_SW_STATE);
	}

	if(pTuner->state == RB211_TUNER_STATE_SLEEP){
		/* Set "Not initialized" (for safe) */
		pTuner->atvVideoID = RB211_ATV_VIDEO_UNKNOWN;
		pTuner->atvAudioID = RB211_ATV_AUDIO_UNKNOWN;
		pTuner->dtvSystem = RB211_DTV_SYSTEM_UNKNOWN;
	}

	if(videoID != pTuner->atvVideoID){ /* Video ID change */
		UINT8 ifagcsel = 0;
		switch(pTuner->flags & RB211_TUNER_CONFIG_IFAGCSEL_MASK){
		case RB211_TUNER_CONFIG_IFAGCSEL_ALL1:
		case RB211_TUNER_CONFIG_IFAGCSEL_A1D2:
		default:
			ifagcsel = 0;
			break;
		case RB211_TUNER_CONFIG_IFAGCSEL_ALL2:
		case RB211_TUNER_CONFIG_IFAGCSEL_D1A2:
			ifagcsel = 1;
			break;
		}

		result = A_init(pTuner, ifagcsel, videoID, 0);
		if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(result); }
	}

	result = A_tune(pTuner, frequency, videoID, audioID);
	if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(result); }

	pTuner->state = RB211_TUNER_STATE_ACTIVE;
	pTuner->frequency = frequency;
	pTuner->atvVideoID = videoID;
	pTuner->atvAudioID = audioID;
	pTuner->dtvSystem = RB211_DTV_SYSTEM_UNKNOWN;

	RB211_TRACE_RETURN(RB211_RESULT_OK);
}

/*--------------------------------------------------------------------
  Tuning to analog channel
  NOTE: rb211_tuner_AnalogTune -> wait 100ms -> rb211_tuner_AnalogAfterTune

  rb211_tuner_t     *pTuner      Tuner struct instance
--------------------------------------------------------------------*/
rb211_result_t rb211_tuner_AnalogAfterTune(rb211_tuner_t *pTuner)
{
	rb211_result_t result = RB211_RESULT_OK;

	RB211_TRACE_ENTER("rb211_tuner_AnalogAfterTune");

	if((!pTuner) || (!pTuner->pI2c)){
		RB211_TRACE_RETURN(RB211_RESULT_ERROR_ARG);
	}

	if(pTuner->state != RB211_TUNER_STATE_ACTIVE){
		RB211_TRACE_RETURN(RB211_RESULT_ERROR_SW_STATE);
	}

	result = A_tune2(pTuner);
	if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(result); }

	RB211_TRACE_RETURN(RB211_RESULT_OK);
}

/*--------------------------------------------------------------------
  Tuning to digital channel

  rb211_tuner_t      *pTuner      Tuner struct instance
  UINT32          frequency    RF center frequency (kHz)
  rb211_dtv_system_t system       Broadcast system ID (defined in rb211_dtv.h)
--------------------------------------------------------------------*/
rb211_result_t rb211_tuner_DigitalTune(rb211_tuner_t *pTuner, UINT32 frequency, rb211_dtv_system_t system)
{
	rb211_result_t result = RB211_RESULT_OK;

	RB211_TRACE_ENTER("rb211_tuner_DigitalTune");

	if((!pTuner) || (!pTuner->pI2c)){
		RB211_TRACE_RETURN(RB211_RESULT_ERROR_ARG);
	}

	if((pTuner->state != RB211_TUNER_STATE_SLEEP) && (pTuner->state != RB211_TUNER_STATE_ACTIVE)){
		RB211_TRACE_RETURN(RB211_RESULT_ERROR_SW_STATE);
	}

	if(pTuner->state == RB211_TUNER_STATE_SLEEP){
		/* Set "Not initialized" (for safe) */
		pTuner->atvVideoID = RB211_ATV_VIDEO_UNKNOWN;
		pTuner->atvAudioID = RB211_ATV_AUDIO_UNKNOWN;
		pTuner->dtvSystem = RB211_DTV_SYSTEM_UNKNOWN;
	}

	if(system != pTuner->dtvSystem){ /* System change */
		UINT8 ifagcsel = 0;
		switch(pTuner->flags & RB211_TUNER_CONFIG_IFAGCSEL_MASK){
		case RB211_TUNER_CONFIG_IFAGCSEL_ALL1:
		case RB211_TUNER_CONFIG_IFAGCSEL_D1A2:
		default:
			ifagcsel = 0;
			break;
		case RB211_TUNER_CONFIG_IFAGCSEL_ALL2:
		case RB211_TUNER_CONFIG_IFAGCSEL_A1D2:
			ifagcsel = 1;
			break;
		}

		switch(system){
		case RB211_DTV_SYSTEM_DVBC: /* Analog setting is used in DVB-C */
			result = A_init(pTuner, ifagcsel, RB211_ATV_VIDEO_UNKNOWN, 1); /* A_init (DVB-C) */
			if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(result); }
			break;
		default:
			result = D_init(pTuner, ifagcsel, system); /* D_init */
			if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(result); }
			break;
		}
	}

	result = D_tune(pTuner, frequency, system);
	if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(result); }

	pTuner->state = RB211_TUNER_STATE_ACTIVE;
	pTuner->frequency = frequency;
	pTuner->atvVideoID = RB211_ATV_VIDEO_UNKNOWN;
	pTuner->atvAudioID = RB211_ATV_AUDIO_UNKNOWN;
	pTuner->dtvSystem = system;

	RB211_TRACE_RETURN(RB211_RESULT_OK);
}

/*--------------------------------------------------------------------
  Tuner sleep

  rb211_tuner_t     *pTuner      Tuner struct instance
--------------------------------------------------------------------*/
rb211_result_t rb211_tuner_Sleep(rb211_tuner_t *pTuner)
{
	rb211_result_t result = RB211_RESULT_OK;

	RB211_TRACE_ENTER("rb211_tuner_Sleep");

	if((!pTuner) || (!pTuner->pI2c)){
		RB211_TRACE_RETURN(RB211_RESULT_ERROR_ARG);
	}

	if((pTuner->state != RB211_TUNER_STATE_SLEEP) && (pTuner->state != RB211_TUNER_STATE_ACTIVE)){
		RB211_TRACE_RETURN(RB211_RESULT_ERROR_SW_STATE);
	}

	result = X_fin(pTuner);
	if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(result); }
	
	pTuner->state = RB211_TUNER_STATE_SLEEP;
	pTuner->frequency = 0;
	pTuner->atvVideoID = RB211_ATV_VIDEO_UNKNOWN;
	pTuner->atvAudioID = RB211_ATV_AUDIO_UNKNOWN;
	pTuner->dtvSystem = RB211_DTV_SYSTEM_UNKNOWN;

	RB211_TRACE_RETURN(RB211_RESULT_OK);
}

/*--------------------------------------------------------------------
  Tuner GPO ctrl

  rb211_tuner_t     *pTuner      Tuner struct instance
  UINT8          id           GPIO id (0 / 1)
  UINT8          val          Lo(0) or Hi(1)
--------------------------------------------------------------------*/
rb211_result_t rb211_tuner_GPOCtrl(rb211_tuner_t *pTuner, UINT8 id, UINT8 val)
{
	rb211_result_t result = RB211_RESULT_OK;

	RB211_TRACE_ENTER("rb211_tuner_GPOCtrl");

	if((!pTuner) || (!pTuner->pI2c)){
		RB211_TRACE_RETURN(RB211_RESULT_ERROR_ARG);
	}

	switch(id){
	case 0:
		result = rb211_tuner_i2c_SetRegisterBits(pTuner->pI2c, pTuner->i2cAddress, 0x05, (UINT8)(val ? 0x01 : 0x00), 0x01);
		if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_I2C); }
		break;
	case 1:
		result = rb211_tuner_i2c_SetRegisterBits(pTuner->pI2c, pTuner->i2cAddress, 0x05, (UINT8)(val ? 0x02 : 0x00), 0x02);
		if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_I2C); }
		break;
	default:
		RB211_TRACE_RETURN(RB211_RESULT_ERROR_ARG);
	}

	RB211_TRACE_RETURN(RB211_RESULT_OK);
}

/*--------------------------------------------------------------------
  Prepare special value for analog tuning in Korea

  This function should be called if the system support analog tuning in Korea.

  rb211_tuner_t     *pTuner      Tuner struct instance
--------------------------------------------------------------------*/
rb211_result_t rb211_tuner_PrepareForKorea(rb211_tuner_t *pTuner)
{
	rb211_result_t result = RB211_RESULT_OK;

	RB211_TRACE_ENTER("rb211_tuner_PrepareForKorea");

	if((!pTuner) || (!pTuner->pI2c)){
		RB211_TRACE_RETURN(RB211_RESULT_ERROR_ARG);
	}

	if((pTuner->state != RB211_TUNER_STATE_SLEEP) && (pTuner->state != RB211_TUNER_STATE_ACTIVE)){
		RB211_TRACE_RETURN(RB211_RESULT_ERROR_SW_STATE);
	}

	/* Dummy tuning (ordinal Japan setting is used) */
	result = rb211_tuner_AnalogTune(pTuner, 91250, RB211_ATV_VIDEO_M, RB211_ATV_AUDIO_M_EIA_J_STEREO);
	if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(result); }

	{
		UINT8 data = 0x00;
		result = pTuner->pI2c->ReadRegister(pTuner->pI2c, pTuner->i2cAddress, 0xCE, &data, 1);
		if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_I2C); }
		pTuner->specialValueForKorea = data; /* Store in the tuner struct */
	}

	result = rb211_tuner_Sleep(pTuner);
	if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(result); }

	RB211_TRACE_RETURN(RB211_RESULT_OK);
}

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
rb211_result_t rb211_tuner_RFFilterConfig(rb211_tuner_t *pTuner, UINT8 coeff, UINT8 offset)
{
	rb211_result_t result = RB211_RESULT_OK;

	RB211_TRACE_ENTER("rb211_tuner_RFFilterConfig");

	if((!pTuner) || (!pTuner->pI2c)){
		RB211_TRACE_RETURN(RB211_RESULT_ERROR_ARG);
	}

	if((pTuner->state != RB211_TUNER_STATE_SLEEP) && (pTuner->state != RB211_TUNER_STATE_ACTIVE)){
		RB211_TRACE_RETURN(RB211_RESULT_ERROR_SW_STATE);
	}

	/* Logic wake up */
	result = pTuner->pI2c->WriteOneRegister(pTuner->pI2c, pTuner->i2cAddress, 0x03, 0xC4);
	if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_I2C); }

	/* CPU wake up */
	result = pTuner->pI2c->WriteOneRegister(pTuner->pI2c, pTuner->i2cAddress, 0x04, 0x40);
	if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_I2C); }

	{
		UINT8 data[3];

		/* Write VL_TRCKOUT_COEFF */
		data[0] = coeff;
		data[1] = 0x49;
		data[2] = 0x03;
		result = pTuner->pI2c->WriteRegister(pTuner->pI2c, pTuner->i2cAddress, 0x16, data, sizeof(data));
		if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_I2C); }

		result = WaitCpuIdle(pTuner);
		if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(result); }

		/* Write VL_TRCKOUT_OFS */
		data[0] = offset;
		data[1] = 0x4B;
		data[2] = 0x03;
		result = pTuner->pI2c->WriteRegister(pTuner->pI2c, pTuner->i2cAddress, 0x16, data, sizeof(data));
		if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_I2C); }

		result = WaitCpuIdle(pTuner);
		if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(result); }
	}

	/* CPU deep sleep */
	result = pTuner->pI2c->WriteOneRegister(pTuner->pI2c, pTuner->i2cAddress, 0x04, 0x00);
	if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_I2C); }

	/* Logic sleep */
	result = pTuner->pI2c->WriteOneRegister(pTuner->pI2c, pTuner->i2cAddress, 0x03, 0xC0);
	if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_I2C); }

	RB211_TRACE_RETURN(RB211_RESULT_OK);
}

/*------------------------------------------------------------------------------
 Implementation of static functions
------------------------------------------------------------------------------*/
static rb211_result_t X_pon(rb211_tuner_t *pTuner)
{
	rb211_result_t result = RB211_RESULT_OK;
	UINT8 data = 0;

	RB211_TRACE_ENTER("X_pon");

	if((!pTuner) || (!pTuner->pI2c)){
		RB211_TRACE_RETURN(RB211_RESULT_ERROR_ARG);
	}

	/* Xtal frequency setting */
	result = pTuner->pI2c->WriteOneRegister(pTuner->pI2c, pTuner->i2cAddress, 0x01, (UINT8)(pTuner->xtalFreq));
	if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_I2C); }

	/* Xtal==41MHz special setting */
	if(pTuner->xtalFreq == 41){
		result = pTuner->pI2c->WriteOneRegister(pTuner->pI2c, pTuner->i2cAddress, 0x44, 0x07);
		if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_I2C); }
	}

	/* NVM bank selection setting (INIT_NVM_CMD) */
	switch(pTuner->flags & RB211_TUNER_CONFIG_NVMSEL_MASK){
	case RB211_TUNER_CONFIG_NVMSEL_AUTO:
		data = 0x00;
		break;
	case RB211_TUNER_CONFIG_NVMSEL_0_1: /* default */
		data = 0x3C;
		break;
	case RB211_TUNER_CONFIG_NVMSEL_0_3:
		data = 0x1C;
		break;
	default:
		RB211_TRACE_RETURN(RB211_RESULT_ERROR_ARG);
	}
	result = pTuner->pI2c->WriteOneRegister(pTuner->pI2c, pTuner->i2cAddress, 0x4D, data);
	if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_I2C); }

	/* Logic wake up */
	result = pTuner->pI2c->WriteOneRegister(pTuner->pI2c, pTuner->i2cAddress, 0x03, 0xC4);
	if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_I2C); }

	/* CPU Boot */
	result = pTuner->pI2c->WriteOneRegister(pTuner->pI2c, pTuner->i2cAddress, 0x04, 0x40);
	if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_I2C); }

	/* RFAGC internal loop setting */
	/* RFVGA optimization setting */
	{
		/* MIX_RFCSW/MIX_AGCSW_DETIFSW, RF_DIST0 - RF_DIST5 */
		const UINT8 cdata[7] = {0x18, 0x40, 0x10, 0x10, 0x10, 0x10, 0x10};
		result = pTuner->pI2c->WriteRegister(pTuner->pI2c, pTuner->i2cAddress, 0x21, cdata, sizeof(cdata));
		if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_I2C); }
	}

	/* RF_GAIN auto control setting */
	result = pTuner->pI2c->WriteOneRegister(pTuner->pI2c, pTuner->i2cAddress, 0x4E, 0x01);
	if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_I2C); }

	if(pTuner->flags & RB211_TUNER_CONFIG_SYSTEML_REGION){
		/* System-L region setting (MIX_TOL=Slow) */
		result = rb211_tuner_i2c_SetRegisterBits(pTuner->pI2c, pTuner->i2cAddress, 0x0C, 0x00, 0x30);
		if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_I2C); }
	}else{
		/* Other region setting (MIX_TOL=4kOhm) */
		result = rb211_tuner_i2c_SetRegisterBits(pTuner->pI2c, pTuner->i2cAddress, 0x0C, 0x10, 0x30);
		if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_I2C); }
	}
	
	result = WaitCpuIdle(pTuner);
	if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(result); }

#ifndef RB211_TUNER_IGNORE_NVM_ERROR /* For no NVM tuner evaluation */
	/* Check CPU_ERR */
	result = pTuner->pI2c->ReadRegister(pTuner->pI2c, pTuner->i2cAddress, 0x1B, &data, 1);
	if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_I2C); }
	if((data & 0x3F) != 0x00){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_HW_STATE); }
#endif /*  RB211_TUNER_IGNORE_NVM_ERROR */

	/* XOSC_SEL setting */
	if(pTuner->flags & RB211_TUNER_CONFIG_EXT_REF){ /* Use external Xtal */
		/* XOSC_SEL=0(disable) */
		data = 0x00;
	}else if(pTuner->xtalFreq == 4){
		/* XOSC_SEL=100uA */
		data = 0x04;
	}else{
		/* XOSC_SEL=400uA */
		data = 0x10;
	}
	result = rb211_tuner_i2c_SetRegisterBits(pTuner->pI2c, pTuner->i2cAddress, 0x07, data, 0x1F);
	if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_I2C); }

	/* CPU deep sleep */
	result = pTuner->pI2c->WriteOneRegister(pTuner->pI2c, pTuner->i2cAddress, 0x04, 0x00);
	if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_I2C); }

	/* Logic sleep */
	result = pTuner->pI2c->WriteOneRegister(pTuner->pI2c, pTuner->i2cAddress, 0x03, 0xC0);
	if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_I2C); }

	/* Power save setting */
	result = pTuner->pI2c->WriteOneRegister(pTuner->pI2c, pTuner->i2cAddress, 0x14, 0x00);
	if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_I2C); }
	result = pTuner->pI2c->WriteOneRegister(pTuner->pI2c, pTuner->i2cAddress, 0x15, 0x04);
	if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_I2C); }

	RB211_TRACE_RETURN(RB211_RESULT_OK);
}

static rb211_result_t A_init(rb211_tuner_t *pTuner, UINT8 ifagcsel, rb211_atv_video_t videoID, int isDvbC)
{
	rb211_result_t result = RB211_RESULT_OK;
	UINT8 data = 0;

	RB211_TRACE_ENTER("A_init");

	if((!pTuner) || (!pTuner->pI2c)){
		RB211_TRACE_RETURN(RB211_RESULT_ERROR_ARG);
	}

	/* Disable power save */
	result = pTuner->pI2c->WriteOneRegister(pTuner->pI2c, pTuner->i2cAddress, 0x14, 0xFB);
	if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_I2C); }
	result = pTuner->pI2c->WriteOneRegister(pTuner->pI2c, pTuner->i2cAddress, 0x15, 0x0F);
	if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_I2C); }

	/* IF OUT SEL / AGC_SEL setting */
	if(ifagcsel == 0){
		/* IFOUT1/AGC1 */
		result = rb211_tuner_i2c_SetRegisterBits(pTuner->pI2c, pTuner->i2cAddress, 0x05, 0x00, 0x1C);
		if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_I2C); }
	}else{
		/* IFOUT2/AGC2 */
		result = rb211_tuner_i2c_SetRegisterBits(pTuner->pI2c, pTuner->i2cAddress, 0x05, 0x0C, 0x1C);
		if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_I2C); }
	}

	/* REF_R setting */
	result = pTuner->pI2c->WriteOneRegister(pTuner->pI2c, pTuner->i2cAddress, 0x06, (UINT8)((pTuner->xtalFreq == 41) ? 40 : (pTuner->xtalFreq)));
	if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_I2C); }

	/* XOSC_SEL setting */
	if(pTuner->flags & RB211_TUNER_CONFIG_EXT_REF){ /* Use external Xtal */
		/* XOSC_SEL=0(disable) */
		data = 0x00;
	}else if(pTuner->xtalFreq == 4){
		/* XOSC_SEL=50uA */
		data = 0x02;
	}else{
		/* XOSC_SEL=400uA */
		data = 0x10;
	}
	result = rb211_tuner_i2c_SetRegisterBits(pTuner->pI2c, pTuner->i2cAddress, 0x07, data, 0x1F);
	if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_I2C); }

	/* Set KBW Wc=1.5KHz */
	result = pTuner->pI2c->WriteOneRegister(pTuner->pI2c, pTuner->i2cAddress, 0x08, 0x02);
	if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_I2C); }

	/* Set R2_BANK=30K, C2_BANK=400pF, R2_RANGE=0, ORDER=1-1 MASH */
	result = rb211_tuner_i2c_SetRegisterBits(pTuner->pI2c, pTuner->i2cAddress, 0x0B, 0x0F, 0x3F);
	if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_I2C); }

	if(isDvbC){
		/* Set MIX_OLL=2.0Vpp */
		data = 0x02;
	}else if((videoID == RB211_ATV_VIDEO_L) || (videoID == RB211_ATV_VIDEO_L_DASH)){
		/* Set MIX_OLL=4.0Vpp */
		data = 0x05;
	}else{
		/* Set MIX_OLL=1.2Vpp */
		data = 0x00;
	}
	result = rb211_tuner_i2c_SetRegisterBits(pTuner->pI2c, pTuner->i2cAddress, 0x0C, data, 0x07);
	if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_I2C); }

	if((videoID == RB211_ATV_VIDEO_L) || (videoID == RB211_ATV_VIDEO_L_DASH)){
		/* Set IF_BPF_GC=0dB */
		data = 0x01;
	}else{
		/* Set IF_BPF_GC=+4dB */
		data = 0x03;
	}
	result = rb211_tuner_i2c_SetRegisterBits(pTuner->pI2c, pTuner->i2cAddress, 0x0E, data, 0x07);
	if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_I2C); }

	if((videoID == RB211_ATV_VIDEO_L) || (videoID == RB211_ATV_VIDEO_L_DASH)){
		/* Set RF_OLDET_ENX=0, RF_OLDET_OLL=-32dBm */
		data = 0x02;
	}else{
		/* Set RF_OLDET_ENX=0, RF_OLDET_OLL=-34dBm */
		data = 0x01;
	}
	result = pTuner->pI2c->WriteOneRegister(pTuner->pI2c, pTuner->i2cAddress, 0x49, data);
	if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_I2C); }

	RB211_TRACE_RETURN(RB211_RESULT_OK);
}

static rb211_result_t D_init(rb211_tuner_t *pTuner, UINT8 ifagcsel, rb211_dtv_system_t system)
{
	rb211_result_t result = RB211_RESULT_OK;
	UINT8 data = 0;

	RB211_TRACE_ENTER("D_init");

	if((!pTuner) || (!pTuner->pI2c)){
		RB211_TRACE_RETURN(RB211_RESULT_ERROR_ARG);
	}

	/* Disable power save */
	result = pTuner->pI2c->WriteOneRegister(pTuner->pI2c, pTuner->i2cAddress, 0x14, 0xFB);
	if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_I2C); }
	result = pTuner->pI2c->WriteOneRegister(pTuner->pI2c, pTuner->i2cAddress, 0x15, 0x0F);
	if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_I2C); }

	/* IF OUT SEL / AGC_SEL setting */
	if(ifagcsel == 0){
		/* IFOUT1/AGC1 */
		result = rb211_tuner_i2c_SetRegisterBits(pTuner->pI2c, pTuner->i2cAddress, 0x05, 0x00, 0x1C);
		if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_I2C); }
	}else{
		/* IFOUT2/AGC2 */
		result = rb211_tuner_i2c_SetRegisterBits(pTuner->pI2c, pTuner->i2cAddress, 0x05, 0x0C, 0x1C);
		if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_I2C); }
	}

	/* REF_R setting */
	result = pTuner->pI2c->WriteOneRegister(pTuner->pI2c, pTuner->i2cAddress, 0x06, (UINT8)(pTuner->xtalFreq/4));
	if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_I2C); }

	/* XOSC_SEL setting */
	if(pTuner->flags & RB211_TUNER_CONFIG_EXT_REF){ /* Use external Xtal */
		/* XOSC_SEL=0(disable) */
		data = 0x00;
	}else if(pTuner->xtalFreq == 4){
		/* XOSC_SEL=100uA */
		data = 0x04;
	}else{
		/* XOSC_SEL=400uA */
		data = 0x10;
	}
	result = rb211_tuner_i2c_SetRegisterBits(pTuner->pI2c, pTuner->i2cAddress, 0x07, data, 0x1F);
	if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_I2C); }

	/* Set KBW Wc=30KHz */
	result = pTuner->pI2c->WriteOneRegister(pTuner->pI2c, pTuner->i2cAddress, 0x08, 0x0E);
	if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_I2C); }

	/* Set R2_BANK=10K, C2_BANK=100pF, R2_RANGE=0, ORDER=1-1-1 MASH */
	result = rb211_tuner_i2c_SetRegisterBits(pTuner->pI2c, pTuner->i2cAddress, 0x0B, 0x25, 0x3F);
	if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_I2C); }

	/* Set MIX_OLL=2.5Vpp */
	result = rb211_tuner_i2c_SetRegisterBits(pTuner->pI2c, pTuner->i2cAddress, 0x0C, 0x03, 0x07);
	if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_I2C); }

	if(system == RB211_DTV_SYSTEM_DTMB){
		/* Set IF_BPF_GC=+8dB */
		result = rb211_tuner_i2c_SetRegisterBits(pTuner->pI2c, pTuner->i2cAddress, 0x0E, 0x05, 0x07);
		if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_I2C); }
	}else{
		/* Set IF_BPF_GC=+12dB */
		result = rb211_tuner_i2c_SetRegisterBits(pTuner->pI2c, pTuner->i2cAddress, 0x0E, 0x06, 0x07);
		if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_I2C); }
	}

	/* Set RF_OLDET_ENX=0, RF_OLDET_OLL=-26dBm */
	result = pTuner->pI2c->WriteOneRegister(pTuner->pI2c, pTuner->i2cAddress, 0x49, 0x05);
	if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_I2C); }

	RB211_TRACE_RETURN(RB211_RESULT_OK);
}

static rb211_result_t X_tune_start(rb211_tuner_t *pTuner)
{
	rb211_result_t result = RB211_RESULT_OK;

	RB211_TRACE_ENTER("X_tune_start");

	if((!pTuner) || (!pTuner->pI2c)){
		RB211_TRACE_RETURN(RB211_RESULT_ERROR_ARG);
	}

	/* RFAGC reset */
#if 0 /* Unnecessary for SUT-RB211 */
	result = pTuner->pI2c->WriteOneRegister(pTuner->pI2c, pTuner->i2cAddress, 0x3F, 0x01);
	if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_I2C); }
#endif

	/* Logic wake up */
	result = pTuner->pI2c->WriteOneRegister(pTuner->pI2c, pTuner->i2cAddress, 0x03, 0xC4);
	if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_I2C); }

	/* CPU wake up */
	result = pTuner->pI2c->WriteOneRegister(pTuner->pI2c, pTuner->i2cAddress, 0x04, 0x40);
	if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_I2C); }

	RB211_TRACE_RETURN(RB211_RESULT_OK);
}

static rb211_result_t X_tune_end(rb211_tuner_t *pTuner)
{
	rb211_result_t result = RB211_RESULT_OK;

	RB211_TRACE_ENTER("X_tune_end");

	if((!pTuner) || (!pTuner->pI2c)){
		RB211_TRACE_RETURN(RB211_RESULT_ERROR_ARG);
	}

	/* CPU deep sleep */
	result = pTuner->pI2c->WriteOneRegister(pTuner->pI2c, pTuner->i2cAddress, 0x04, 0x00);
	if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_I2C); }

	/* Logic sleep */
	result = pTuner->pI2c->WriteOneRegister(pTuner->pI2c, pTuner->i2cAddress, 0x03, 0xC0);
	if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_I2C); }

	/* Release RFAGC reset */
#if 0 /* Unnecessary for SUT-RB211 */
	result = pTuner->pI2c->WriteOneRegister(pTuner->pI2c, pTuner->i2cAddress, 0x3F, 0x00);
	if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_I2C); }
#endif

	RB211_TRACE_RETURN(RB211_RESULT_OK);
}

static rb211_result_t A_tune(rb211_tuner_t *pTuner, UINT32 frequency,
							rb211_atv_video_t videoID, rb211_atv_audio_t audioID)
{
	rb211_result_t result = RB211_RESULT_OK;

	RB211_TRACE_ENTER("A_tune");

	if((!pTuner) || (!pTuner->pI2c)){
		RB211_TRACE_RETURN(RB211_RESULT_ERROR_ARG);
	}

	result = X_tune_start(pTuner);
	if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(result); }

	{
		UINT8 data[5]; /* 0x10 - 0x14 */
		UINT8 nBW = 0;
		UINT8 nFIF_OFFSET = 0;
		UINT8 nBW_OFFSET = 0;

		data[0] = (UINT8)(frequency & 0xFF);         /* 0x10: FRF_L */
		data[1] = (UINT8)((frequency >> 8) & 0xFF);  /* 0x11: FRF_M */
		data[2] = (UINT8)((frequency >> 16) & 0x0F); /* 0x12: FRF_H [3:0] */
		data[2] |= 0x80; /* 0x12: IS_FP */
		data[3] = 0xFF; /* 0x13: VCO calibration enable */
		data[4] = 0xFF; /* 0x14: Analog block enable */

		/*
		<PORTING>
		Rb211 silicon tuner bandwidth and IF frequency setting for each broadcasting system.
		These values are optimized for CXD2823ER  analog terrestrial demodulator.
		The user have to change these values if other demodulator is used.
		Please check Rb211 silicon tuner application note for detail.

		nBW : 6MHzBW(0x00) or 7MHzBW(0x01) or 8MHzBW(0x02)
		nFIF_OFFSET : FIF_OFFSET[4:0] value (Sub address: 0x0E)
		nBW_OFFSET  : BW_OFFSET[4:0] value (Sub address: 0x0F)
		*/
		switch(videoID){
		case RB211_ATV_VIDEO_M:
			switch(audioID){
			case RB211_ATV_AUDIO_M_EIA_J_STEREO:   /* Japan */
			case RB211_ATV_AUDIO_N_BTSC:           /* US */
			default:
				nBW = 0x00;
				nFIF_OFFSET = 0x00;
				nBW_OFFSET = 0x00;
				break;
			case RB211_ATV_AUDIO_M_DUAL_FM_STEREO: /* Korea */
				nBW = 0x00;
				nFIF_OFFSET = 0x03;
				nBW_OFFSET = 0x00;
				break;
			}
			break;
		case RB211_ATV_VIDEO_BG:
		case RB211_ATV_VIDEO_B_AUS:
			nBW = 0x01;
			nFIF_OFFSET = 0x0B;
			nBW_OFFSET = 0x08;
			break;
		case RB211_ATV_VIDEO_DK:
		case RB211_ATV_VIDEO_I:
		case RB211_ATV_VIDEO_L:
		case RB211_ATV_VIDEO_DK_CHINA:
			nBW = 0x02;
			nFIF_OFFSET = 0x09;
			nBW_OFFSET = 0x07;
			break;
		case RB211_ATV_VIDEO_L_DASH:
			nBW = 0x02;
			nFIF_OFFSET = 0x1F;
			nBW_OFFSET = 0x07;
			data[2] |= 0x40; /* 0x12: IS_L_DASH */
			break;
		default:
			RB211_TRACE_RETURN(RB211_RESULT_ERROR_ARG);
		}
		
		/* Set FIF_OFFSET */
		result = rb211_tuner_i2c_SetRegisterBits(pTuner->pI2c, pTuner->i2cAddress, 0x0E, (UINT8)(nFIF_OFFSET << 3), 0xF8);
		if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_I2C); }

		/* Set BW_OFFSET */
		result = rb211_tuner_i2c_SetRegisterBits(pTuner->pI2c, pTuner->i2cAddress, 0x0F, nBW_OFFSET, 0x1F);
		if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_I2C); }

		/* Set BW */
		data[2] |= (UINT8)(nBW << 4);

		/* Tune (Burst write) */
		result = pTuner->pI2c->WriteRegister(pTuner->pI2c, pTuner->i2cAddress, 0x10, data, sizeof(data));
		if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_I2C); }
	}

	result = WaitCpuIdle(pTuner);
	if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(result); }

	result = X_tune_end(pTuner);
	if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(result); }

	RB211_TRACE_RETURN(RB211_RESULT_OK);
}

static rb211_result_t A_tune2(rb211_tuner_t *pTuner)
{
	rb211_result_t result = RB211_RESULT_OK;

	RB211_TRACE_ENTER("A_tune2");

	if((!pTuner) || (!pTuner->pI2c)){
		RB211_TRACE_RETURN(RB211_RESULT_ERROR_ARG);
	}

	/* Special setting for Korea */
	if((pTuner->atvVideoID == RB211_ATV_VIDEO_M)
		&& (pTuner->atvAudioID == RB211_ATV_AUDIO_M_DUAL_FM_STEREO)){

		/* Logic wake up */
		result = pTuner->pI2c->WriteOneRegister(pTuner->pI2c, pTuner->i2cAddress, 0x03, 0xC4);
		if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_I2C); }

		/* CPU wake up */
		result = pTuner->pI2c->WriteOneRegister(pTuner->pI2c, pTuner->i2cAddress, 0x04, 0x40);
		if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_I2C); }

		{
			UINT8 data[3];
			data[0] = pTuner->specialValueForKorea;
			data[1] = 0xCE;
			data[2] = 0x03;
			/* Send special value to CPU */
			result = pTuner->pI2c->WriteRegister(pTuner->pI2c, pTuner->i2cAddress, 0x16, data, sizeof(data));
			if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_I2C); }

			result = WaitCpuIdle(pTuner);
			if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(result); }
		}

		/* CPU deep sleep */
		result = pTuner->pI2c->WriteOneRegister(pTuner->pI2c, pTuner->i2cAddress, 0x04, 0x00);
		if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_I2C); }

		/* Logic sleep */
		result = pTuner->pI2c->WriteOneRegister(pTuner->pI2c, pTuner->i2cAddress, 0x03, 0xC0);
		if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_I2C); }
	}

	RB211_TRACE_RETURN(RB211_RESULT_OK);
}

static rb211_result_t D_tune(rb211_tuner_t *pTuner, UINT32 frequency,
							rb211_dtv_system_t system)
{
	rb211_result_t result = RB211_RESULT_OK;

	RB211_TRACE_ENTER("D_tune");

	if((!pTuner) || (!pTuner->pI2c)){
		RB211_TRACE_RETURN(RB211_RESULT_ERROR_ARG);
	}

	result = X_tune_start(pTuner);
	if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(result); }

	{
		UINT8 data[5]; /* 0x10 - 0x14 */
		UINT8 nBW = 0;
		UINT8 nFIF_OFFSET = 0;
		UINT8 nBW_OFFSET = 0;

		data[0] = (UINT8)(frequency & 0xFF);         /* 0x10: FRF_L */
		data[1] = (UINT8)((frequency >> 8) & 0xFF);  /* 0x11: FRF_M */
		data[2] = (UINT8)((frequency >> 16) & 0x0F); /* 0x12: FRF_H [3:0] */
		data[3] = 0xFF; /* 0x13: VCO calibration enable */
		data[4] = 0xFF; /* 0x14: Analog block enable */

		/*
		<PORTING>
		Rb211 silicon tuner bandwidth and IF frequency setting for each broadcasting system.
		These values are optimized for Cxd2828 digital terrestrial demodulators.
		The user have to change these values if other demodulator is used.
		Please check Rb211 silicon tuner application note for detail.

		nBW : 6MHzBW(0x00) or 7MHzBW(0x01) or 8MHzBW(0x02)
		nFIF_OFFSET : FIF_OFFSET[4:0] value (Sub address: 0x0E)
		nBW_OFFSET  : BW_OFFSET[4:0] value (Sub address: 0x0F)
		*/

		switch(system){
		case RB211_DTV_SYSTEM_QAM:
		case RB211_DTV_SYSTEM_ATSC:
			nBW = 0x00;
			nFIF_OFFSET = 0x1A;
			nBW_OFFSET = 0x1A;
			break;
		case RB211_DTV_SYSTEM_ISDBT:
			nBW = 0x00;
			nFIF_OFFSET = 0x00;
			nBW_OFFSET = 0x00;
			break;
		case RB211_DTV_SYSTEM_DVBT_6:
			nBW = 0x00;
			nFIF_OFFSET = 0x1E;
			nBW_OFFSET = 0x1C;
			break;
		case RB211_DTV_SYSTEM_DVBT_7:
			nBW = 0x01;
			nFIF_OFFSET = 0x1D;
			nBW_OFFSET = 0x00;
			break;
		case RB211_DTV_SYSTEM_DVBT_8:
			nBW = 0x02;
			nFIF_OFFSET = 0x1D;
			nBW_OFFSET = 0x00;
			break;
		case RB211_DTV_SYSTEM_DVBT2_5:
		case RB211_DTV_SYSTEM_DVBT2_6:
			nBW = 0x00;
			nFIF_OFFSET = 0x00;
			nBW_OFFSET = 0x00;
			break;
		case RB211_DTV_SYSTEM_DVBT2_7:
			nBW = 0x01;
			nFIF_OFFSET = 0x1A;
			nBW_OFFSET = 0x1E;
			break;
		case RB211_DTV_SYSTEM_DVBT2_8:
			nBW = 0x02;
			nFIF_OFFSET = 0x1A;
			nBW_OFFSET = 0x1D;
			break;
		case RB211_DTV_SYSTEM_DVBC:
			nBW = 0x02;
			nFIF_OFFSET = 0x1F;
			nBW_OFFSET = 0x03;
			break;
		case RB211_DTV_SYSTEM_DTMB:
			nBW = 0x02;
			nFIF_OFFSET = 0x1E;
			nBW_OFFSET = 0x1C;
			break;
		default:
			RB211_TRACE_RETURN(RB211_RESULT_ERROR_ARG);
		}

		/* Set FIF_OFFSET */
		result = rb211_tuner_i2c_SetRegisterBits(pTuner->pI2c, pTuner->i2cAddress, 0x0E, (UINT8)(nFIF_OFFSET << 3), 0xF8);
		if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_I2C); }

		/* Set BW_OFFSET */
		result = rb211_tuner_i2c_SetRegisterBits(pTuner->pI2c, pTuner->i2cAddress, 0x0F, nBW_OFFSET, 0x1F);
		if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_I2C); }

		/* Set BW */
		data[2] |= (UINT8)(nBW << 4);

		/* Tune (Burst write) */
		result = pTuner->pI2c->WriteRegister(pTuner->pI2c, pTuner->i2cAddress, 0x10, data, sizeof(data));
		if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_I2C); }
	}

	result = WaitCpuIdle(pTuner);
	if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(result); }

	result = X_tune_end(pTuner);
	if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(result); }

	RB211_TRACE_RETURN(RB211_RESULT_OK);
}

static rb211_result_t X_fin(rb211_tuner_t *pTuner)
{
	rb211_result_t result = RB211_RESULT_OK;

	RB211_TRACE_ENTER("X_fin");

	if((!pTuner) || (!pTuner->pI2c)){
		RB211_TRACE_RETURN(RB211_RESULT_ERROR_ARG);
	}

	/* Power save setting */
	result = pTuner->pI2c->WriteOneRegister(pTuner->pI2c, pTuner->i2cAddress, 0x14, 0x00);
	if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_I2C); }
	result = pTuner->pI2c->WriteOneRegister(pTuner->pI2c, pTuner->i2cAddress, 0x15, 0x04);
	if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_I2C); }

	RB211_TRACE_RETURN(RB211_RESULT_OK);
}

static rb211_result_t WaitCpuIdle(rb211_tuner_t *pTuner)
{
	rb211_result_t result = RB211_RESULT_OK;
	UINT8 data = 0;
	unsigned int cnt = 0;

	RB211_TRACE_ENTER("WaitCpuIdle");

	if((!pTuner) || (!pTuner->pI2c)){
		RB211_TRACE_RETURN(RB211_RESULT_ERROR_ARG);
	}
	
	/* Check CPU_STT */
	do{
		result = pTuner->pI2c->ReadRegister(pTuner->pI2c, pTuner->i2cAddress, 0x1A, &data, 1);
		if(result != RB211_RESULT_OK){ RB211_TRACE_RETURN(RB211_RESULT_ERROR_I2C); }
		
		if(data == 0x00){
			RB211_TRACE_RETURN(RB211_RESULT_OK);
		}
		
		cnt++;
		if(cnt > WAITCPUIDLE_TIMEOUT_MS){
			RB211_TRACE_RETURN(RB211_RESULT_ERROR_TIMEOUT);
		}
		RB211_SLEEP(WAITCPUIDLE_POLL_INTERVAL_MS);
	} while(1);
}
