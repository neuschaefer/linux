/******************************************************************************
*
* Copyright 2009 - 2012  Broadcom Corporation
*
*  Unless you and Broadcom execute a separate written software license
*  agreement governing use of this software, this software is licensed to you
*  under the terms of the GNU General Public License version 2 (the GPL),
*  available at
*
*      http://www.broadcom.com/licenses/GPLv2.php
*
*  with the following added to such license:
*
*  As a special exception, the copyright holders of this software give you
*  permission to link this software with independent modules, and to copy and
*  distribute the resulting executable under terms of your choice, provided
*  that you also meet, for each linked independent module, the terms and
*  conditions of the license of that module.
*  An independent module is a module which is not derived from this software.
*  The special exception does not apply to any modifications of the software.
*
*  Notwithstanding the above, under no circumstances may you combine this
*  software in any way with any other Broadcom software provided under a
*  license other than the GPL, without Broadcom's express prior written
*  consent.
*
******************************************************************************/
/**
*
*   @file   csl_greek.h
*
*   @brief  This file contains DSP CSL API for Greek voices.
*
****************************************************************************/
#ifndef _CSL_GREEK_H_
#define _CSL_GREEK_H_

/*---- Include Files -------------------------------------------------------*/
#include "mobcom_types.h"

extern Dsp_SharedMem_t *cp_shared_mem;

/**
 * @addtogroup CSL Greek Voices
 * @{
 */
/*********************************************************************/
/**
*
*   CSL_SetOmegaVoiceParameters sets Omega Voice parameters.
*
*   @param    param					(in)	OV parameters
*
**********************************************************************/
void CSL_SetOmegaVoiceParameters(OmegaVoice_Sysparm_t *param);

/*********************************************************************/
/**
*
*   CSL_SetOvGainSpread sets Omega Voice gain spread
*
*   @param    gainSpread	(in)	gain spread
*
**********************************************************************/
void CSL_SetOvGainSpread(UInt16 gainSpread);

/*********************************************************************/
/**
*
*   CSL_ConfigUplinkNoiseSuppression configures Uplink Noise Suppressor.
*
*   @param    gainSlope				(in)	gain slope
*   @param    inputGain				(in)	gain applied at the input of
*				the Uplink Noise suppressor in linear Q9.6
*   @param    outputGain			(in)	gain applied at the output of
*				the Uplink Noise suppressor in linear Q9.6
*   @param    maxSuppression		(in)	maximum noise suppression in
*				dB Q10.5
*
**********************************************************************/
void CSL_ConfigUplinkNoiseSuppression(UInt16 gainSlope, UInt16 inputGain,
				      UInt16 outputGain, Int16 maxSuppression);

/*********************************************************************/
/**
*
*   CSL_SetDownlinkNoiseSuppressionGainSlope sets Downlink Noise
*	Suppressor gain slope
*
*   @param    gainSlope				(in)	gain slope
*
**********************************************************************/
void CSL_SetDownlinkNoiseSuppressionGainSlope(UInt16 gainSlope);

/*********************************************************************/
/**
*
*   CSL_ConfigDownlinkNoiseSuppression configures Downlink Noise Suppressor.
*
*   @param    outputGain			(in)	gain applied at the output of
*				the Downlink Noise suppressor in linear Q9.6
*   @param    maxSuppression		(in)	maximum noise suppression in
*				dB Q10.5
*
**********************************************************************/
void CSL_ConfigDownlinkNoiseSuppression(Int16 maxSuppression);

/*********************************************************************/
/**
*
*   CSL_SetNlpDistortionCoupling sets NLP distortion coupling
*
*   @param    nlp_distortion_coupling	(in)	nlp_distortion_coupling
*
**********************************************************************/
void CSL_SetNlpDistortionCoupling(UInt16 nlp_distortion_coupling);

/*********************************************************************/
/**
*
*   CSL_SetReverbTimeConstant sets Reverb time constant
*
*   @param    reverb_time_constant	(in)	reverb_time_constant
*
**********************************************************************/
void CSL_SetReverbTimeConstant(UInt16 reverb_time_constant);

/*********************************************************************/
/**
*
*   CSL_SetReverbLevel sets Reverb level
*
*   @param    reverb_level	(in)	reverb_level
*
**********************************************************************/
void CSL_SetReverbLevel(Int16 reverb_level);

/*********************************************************************/
/**
*
*   CSL_SetEchoPathChangeDetectionThreshold sets Detection Threshold
*
*   @param    detection_threshold	(in)	detection_threshold
*
**********************************************************************/
void CSL_SetEchoPathChangeDetectionThreshold(UInt16 detection_threshold);

/*********************************************************************/
/**
*
*   CSL_SetSmartCompressorG1Lin sets G1Lin
*
*   @param    g1lin	(in)	g1lin
*
**********************************************************************/
void CSL_SetSmartCompressorG1Lin(UInt16 g1lin);

/*********************************************************************/
/**
*
*   CSL_SetSmartCompressorG2T2 sets G2T2
*
*   @param    g2t2	(in)	g2t2
*
**********************************************************************/
void CSL_SetSmartCompressorG2T2(UInt16 g2t2);

/*********************************************************************/
/**
*
*   CSL_SetMBCompressor_G_Low sets g_low
*
*   @param    g_low	(in)	g_low
*
**********************************************************************/
void CSL_SetMBCompressor_G_Low(UInt16 g_low);

/*********************************************************************/
/**
*
*   CSL_SetMBCompressor_G_Mid sets g_mid
*
*   @param    g_mid	(in)	g_mid
*
**********************************************************************/
void CSL_SetMBCompressor_G_Mid(UInt16 g_mid);

/*********************************************************************/
/**
*
*   CSL_SetMBCompressor_G_High sets g_high
*
*   @param    g_high	(in)	g_high
*
**********************************************************************/
void CSL_SetMBCompressor_G_High(UInt16 g_high);

/*********************************************************************/
/**
*
*   CSL_SetMBCompressor_P_Low sets p_low
*
*   @param    p_low	(in)	p_low
*
**********************************************************************/
void CSL_SetMBCompressor_P_Low(UInt16 p_low);

/*********************************************************************/
/**
*
*   CSL_SetMBCompressor_P_Mid sets p_mid
*
*   @param    p_mid	(in)	p_mid
*
**********************************************************************/
void CSL_SetMBCompressor_P_Mid(UInt16 p_mid);

/*********************************************************************/
/**
*
*   CSL_SetMBCompressor_P_High sets p_high
*
*   @param    p_high	(in)	p_high
*
**********************************************************************/
void CSL_SetMBCompressor_P_High(UInt16 p_high);

/*********************************************************************/
/**
*
*   CSL_SetMBCompressor_Alpha_Low sets alpha_low
*
*   @param    alpha_low	(in)	alpha_low
*
**********************************************************************/
void CSL_SetMBCompressor_Alpha_Low(UInt16 alpha_low);

/*********************************************************************/
/**
*
*   CSL_SetMBCompressor_Alpha_Mid sets alpha_mid
*
*   @param    alpha_mid	(in)	alpha_mid
*
**********************************************************************/
void CSL_SetMBCompressor_Alpha_Mid(UInt16 alpha_mid);

/*********************************************************************/
/**
*
*   CSL_SetMBCompressor_Alpha_High sets alpha_high
*
*   @param    alpha_high	(in)	alpha_high
*
**********************************************************************/
void CSL_SetMBCompressor_Alpha_High(UInt16 alpha_high);

/*********************************************************************/
/**
*
*   CSL_SetMBCompressor_Beta_Low sets beta_low
*
*   @param    beta_low	(in)	beta_low
*
**********************************************************************/
void CSL_SetMBCompressor_Beta_Low(UInt16 beta_low);

/*********************************************************************/
/**
*
*   CSL_SetMBCompressor_Beta_Mid sets beta_mid
*
*   @param    beta_mid	(in)	beta_mid
*
**********************************************************************/
void CSL_SetMBCompressor_Beta_Mid(UInt16 beta_mid);

/*********************************************************************/
/**
*
*   CSL_SetMBCompressor_Beta_High sets beta_high
*
*   @param    beta_high	(in)	beta_high
*
**********************************************************************/
void CSL_SetMBCompressor_Beta_High(UInt16 beta_high);

/*********************************************************************/
/**
*
*   CSL_ConfigSidetoneFilter configures sidetone filter
*
*   @param    enable				(in)	0/1 diable/enable
*   @param    sysGain				(in)	gain
*   @param    scaleFactor			(in)	scale factor
*   @param    outputGain			(in)	output gain
*
**********************************************************************/
void CSL_ConfigSidetoneFilter(UInt16 enable, UInt16 sysGain, UInt16 scaleFactor,
			      UInt16 outputGain);

/*********************************************************************/
/**
*
*   CSL_ConfigECgain configures EC gains
*
*   @param    inputGain				(in)	input gain
*   @param    outputGain			(in)	output gain
*   @param    feedForwardGain		(in)	feed forward gain
*
**********************************************************************/
void CSL_ConfigECGain(UInt16 inputGain, UInt16 outputGain,
		      UInt16 feedForwardGain);

/*********************************************************************/
/**
*
*   CSL_ConfigECMic2gain configures EC mic2 gains
*
*   @param    inputGain				(in)	input gain
*   @param    outputGain			(in)	output gain
*   @param    feedForwardGain		(in)	feed forward gain
*
**********************************************************************/
void CSL_ConfigECMic2Gain(UInt16 inputGain, UInt16 outputGain,
			  UInt16 feedForwardGain);

/*********************************************************************/
/**
*
*   CSL_ConfigSidetoneExpander configures sidetone expander
*
*   @param    enable				(in)	0/1 - disable/enable
*   @param    alpha					(in)	alpha
*   @param    beta					(in)	beta
*   @param    b						(in)	b
*   @param    c						(in)	c
*   @param    c_div_b				(in)	c_div_b
*   @param    inv_b					(in)	inv_b
*
**********************************************************************/
void CSL_ConfigSidetoneExpander(UInt16 enable, UInt16 alpha, UInt16 beta,
				UInt16 b, UInt16 c, UInt16 c_div_b,
				UInt16 inv_b);

/*********************************************************************/
/**
*
*   CSL_ConfigDownlinkExpander configures downlink expander
*
*   @param    enable				(in)	0/1 - disable/enable
*   @param    alpha					(in)	alpha
*   @param    beta					(in)	beta
*   @param    b						(in)	b
*
**********************************************************************/
void CSL_ConfigDownlinkExpander(UInt16 enable, UInt16 alpha, UInt16 beta,
				UInt16 b);

/*********************************************************************/
/**
*
*   CSL_ConfigUplinkExpander configures uplink expander
*
*   @param    enable				(in)	0/1 - disable/enable
*   @param    alpha					(in)	alpha
*   @param    beta					(in)	beta
*   @param    b						(in)	b
*
**********************************************************************/
void CSL_ConfigUplinkExpander(UInt16 enable, UInt16 alpha, UInt16 beta,
			      UInt16 b);

/*********************************************************************/
/**
*
*   CSL_InitDownlinkCompressor initializes downlink compressor
*
*
**********************************************************************/
void CSL_InitDownlinkCompressor(void);

/*********************************************************************/
/**
*
*   CSL_InitUplinkCompressor initializes uplink compressor
*
*
**********************************************************************/
void CSL_InitUplinkCompressor(void);

/*********************************************************************/
/**
*
*   CSL_InitSubbandNLP initializes subband NLP
*
*
**********************************************************************/
void CSL_InitSubbandNLP(void);

/*********************************************************************/
/**
*
*   CSL_ConfigEC configures EC
*
*   @param    ecLength				(in)	EC length
*   @param    ERL_dB				(in)	ERL
*   @param    stepSizeGain			(in)	gain step size
*
**********************************************************************/
void CSL_ConfigEC(UInt16 ecLength, Int16 ERL_dB, Int16 stepSizeGain);

/*********************************************************************/
/**
*
*   CSL_InitCNGFilterCoeff initializes EC CNG filter coefficients
*
*
**********************************************************************/
void CSL_InitCNGFilterCoeff(Int16 *coefPtr);

/*********************************************************************/
/**
*
*   CSL_SetNLPExpanderUpperLimit sets NLP expander upper limit
*
*   @param    limit					(in)	NLP expander upper limit
*
**********************************************************************/
void CSL_SetNLPExpanderUpperLimit(UInt16 limit);

/*********************************************************************/
/**
*
*   CSL_SetCNG_bias sets CNG_bias
*
*   @param    CNG_bias	(in)	CNG bias
*
**********************************************************************/
void CSL_SetCNG_bias(Int16 CNG_bias);

/*********************************************************************/
/**
*
*   CSL_ConfigDualMicParms configures Dual Mic NS/NLP parameters
*
*   @param    dual_mic_ns_level	(in)	Dual mic NS level
*
**********************************************************************/
void CSL_ConfigDualMicParms(UInt16 dual_mic_ns_level);

/*********************************************************************/
/**
*
*   CSL_ConfigDlCompressorFIR1 configures DL Compressor FIR1
*
*
**********************************************************************/
void CSL_ConfigDlCompressorFIR1(void *config);

/*********************************************************************/
/**
*
*   CSL_ConfigDlCompressorFIR2 configures DL Compressor FIR2
*
*
**********************************************************************/
void CSL_ConfigDlCompressorFIR2(void *config);

/*********************************************************************/
/**
*
*   CSL_ConfigDlCompressorStream3 configures DL Compressor Stream3
*
*
**********************************************************************/
void CSL_ConfigDlCompressorStream3(void *config);

/*********************************************************************/
/**
*
*   CSL_ConfigDlCompressor configures DL Compressor
*
*
**********************************************************************/
void CSL_ConfigDlCompressor(void *config);

/*********************************************************************/
/**
*
*   CSL_EnableSubbandCompander controls subband compander
*
*   @param    control	(in)	0/1 - disable/enable
*
**********************************************************************/
void CSL_EnableSubbandCompander(UInt16 control);

/*********************************************************************/
/**
*
*   CSL_ConfigFIR1 configures FIR1 coefficients
*
*
**********************************************************************/
void CSL_ConfigFIR1(void *config);

/*********************************************************************/
/**
*
*   CSL_ConfigFIR2 configures FIR2 coefficients
*
*
**********************************************************************/
void CSL_ConfigFIR2(void *config);

/*********************************************************************/
/**
*
*   CSL_ConfigUlCompressor configures UL Compressor
*
*
**********************************************************************/
void CSL_ConfigUlCompressor(UInt16 t2lin, UInt16 g2t2, UInt16 g3t3, UInt16 g4t4,
			    UInt16 g1lin);

/*********************************************************************/
/**
*
*   CSL_ConfigBvUlCompressor configures Beta Voice UL Compressor
*
*
**********************************************************************/
void CSL_ConfigBvUlCompressor(void);

/*********************************************************************/
/**
*
*   CSL_ConfigNLP configures NLP
*
*   @param    mode					(in) NLP mode 0/1 - FULLBAND/SUBBAND_MODE
*   @param    minPowerDl			(in)
*   @param    minPowerUl			(in)
*   @param    dtFineControl			(in)
*   @param    erleReset				(in)
*
**********************************************************************/
void CSL_ConfigNLP(UInt16 mode, UInt16 minPowerDl, UInt16 minPowerUl,
		   UInt16 dtFineControl, Int16 erleReset);

/*********************************************************************/
/**
*
*   CSL_ConfigSubbandNlpDistortionThresh configures distortion thresholds
*
*
**********************************************************************/
void CSL_ConfigSubbandNlpDistortionThresh(void *config);

/*********************************************************************/
/**
*
*   CSL_ConfigSubbandNlpUlMargin configures UL margin
*
*
**********************************************************************/
void CSL_ConfigSubbandNlpUlMargin(void *config);

/*********************************************************************/
/**
*
*   CSL_ConfigSubbandNlpNoiseMargin configures noise margin
*
*
**********************************************************************/
void CSL_ConfigSubbandNlpNoiseMargin(void *config);

/*********************************************************************/
/**
*
*   CSL_ConfigAlfaVoice configures Alfa Voice
*
*   @param    enable				(in)
*   @param    avcTarget				(in)
*   @param    avcMaxGain			(in)
*   @param    avcStepUp				(in)
*   @param    avcStepDown			(in)
*
**********************************************************************/
void CSL_ConfigAlfaVoice(UInt16 enable, UInt16 avcTarget, UInt16 avcMaxGain,
			 UInt16 avcStepUp, UInt16 avcStepDown);

/*********************************************************************/
/**
*
*   CSL_ConfigKappaVoice configures Kappa Voice
*
*   @param    enable				(in)
*   @param    highPitchedToneTarget	(in)
*   @param    fastLoudnessTarget	(in)
*   @param    fastMaxSuppession		(in)
*   @param    slowLoudnessTarget	(in)
*   @param    slowMaxSuppession		(in)
*
**********************************************************************/
void CSL_ConfigKappaVoice(UInt16 enable, UInt16 highPitchedToneTarget,
			  Int16 fastLoudnessTarget, UInt16 fastMaxSuppession,
			  Int16 slowLoudnessTarget, UInt16 slowMaxSuppession);

/*********************************************************************/
/**
*
*   CSL_ConfigUlNoiseSuppressor configures UL max suppression
*
*   @param    maxSuppression	(in)	max suppression
*
**********************************************************************/
void CSL_ConfigUlNoiseSuppressor(Int16 maxSuppression);

/*********************************************************************/
/**
*
*   CSL_ConfigDlNoiseSuppressor configures DL max suppression
*
*   @param    maxSuppression	(in)	max suppression
*
**********************************************************************/
void CSL_ConfigDlNoiseSuppressor(Int16 maxSuppression);

/*********************************************************************/
/**
*
*   CSL_EnableLongTermPostFiltering controls Long Term Post Filtering
*
*   @param    control			(in)	0/1 - disable/enable
*
**********************************************************************/
void CSL_EnableLongTermPostFiltering(UInt16 control);

/** @} */

#endif /* _CSL_GREEK_H_ */
