//-----------------------------------------------------------------------------
// $Header: 
// (C) Copyright 2001 NXP Semiconductors, All rights reserved
//
// This source code and any compilation or derivative thereof is the sole
// property of NXP Corporation and is provided pursuant to a Software
// License Agreement.  This code is the proprietary information of NXP
// Corporation and is confidential in nature.  Its use and dissemination by
// any party other than NXP Corporation is strictly limited by the
// confidential information provisions of the Agreement referenced above.
//-----------------------------------------------------------------------------
// FILE NAME:    tmbslTDA182I2Instance.c
//
// DESCRIPTION:  define the static Objects
//
// DOCUMENT REF: DVP Software Coding Guidelines v1.14
//               DVP Board Support Library Architecture Specification v0.5
//
// NOTES:        
//-----------------------------------------------------------------------------
//

#include "tmNxTypes.h"
#include "tmCompId.h"
#include "tmFrontEnd.h"
#include "tmUnitParams.h"
#include "tmbslFrontEndTypes.h"

#ifndef TMBSL_TDA18272
#define TMBSL_TDA18272
#endif
#ifdef TMBSL_TDA18272
 #include "tmbslTDA18272.h"
#else /* TMBSL_TDA18272 */
 #include "tmbslTDA18212.h"
#endif /* TMBSL_TDA18272 */

#include "tmbslTDA182I2local.h"
#include "tmbslTDA182I2Instance.h"

//-----------------------------------------------------------------------------
// Global data:
//-----------------------------------------------------------------------------
//
//
// default instance
tmTDA182I2Object_t gTDA182I2Instance[] = 
{
    {
        (tmUnitSelect_t)(-1),                                   /* tUnit */
        (tmUnitSelect_t)(-1),                                   /* tUnit temporary */
        Null,                                                   /* pMutex */
        False,                                                  /* init (instance initialization default) */
        {                                                       /* sRWFunc */
            Null,
            Null
        },
        {                                                       /* sTime */
            Null,
            Null
        },
        {                                                       /* sDebug */
            Null
        },
        {                                                       /* sMutex */
            Null,
            Null,
            Null,
            Null
        },
        tmTDA182I2_PowerStandbyWithXtalOn,                      /* Power state */
        0,                                                      /* RF */
        tmTDA182I2_DVBT_8MHz,                                   /* Standard mode */
        {
            {   /* Std_Array */                                 /* DVB-T 6MHz */                               
                3250000,                                        /* IF */
                0,                                              /* CF_Offset */
                tmTDA182I2_LPF_6MHz,                            /* LPF */
                tmTDA182I2_LPFOffset_0pc,                       /* LPF_Offset */
                tmTDA182I2_IF_AGC_Gain_1Vpp_min_6_24dB,         /* IF_Gain */
                tmTDA182I2_IF_Notch_Enabled,                    /* IF_Notch */
                tmTDA182I2_IF_HPF_0_4MHz,                       /* IF_HPF */
                tmTDA182I2_DC_Notch_Enabled,                    /* DC_Notch */
                tmTDA182I2_AGC1_LNA_TOP_d95_u89dBuV,            /* AGC1_LNA_TOP */
                tmTDA182I2_AGC2_RF_Attenuator_TOP_d90_u84dBuV,  /* AGC2_RF_Attenuator_TOP */
                tmTDA182I2_AGC3_RF_AGC_TOP_100dBuV,             /* AGC3_RF_AGC_TOP_Low_band */
                tmTDA182I2_AGC3_RF_AGC_TOP_102dBuV,             /* AGC3_RF_AGC_TOP_High_band */
                tmTDA182I2_AGC4_IR_Mixer_TOP_d110_u105dBuV,     /* AGC4_IR_Mixer_TOP */
                tmTDA182I2_AGC5_IF_AGC_TOP_d110_u105dBuV,       /* AGC5_IF_AGC_TOP */
                tmTDA182I2_AGC5_Detector_HPF_Disabled,          /* AGC5_Detector_HPF */
                tmTDA182I2_AGC3_Adapt_Enabled,                  /* AGC3_Adapt */
                tmTDA182I2_AGC3_Adapt_TOP_98dBuV,               /* AGC3_Adapt_TOP */
                tmTDA182I2_AGC5_Atten_3dB_Enabled,              /* AGC5_Atten_3dB */
                0x02,                                           /* GSK : settings V2.0.0  */
                tmTDA182I2_H3H5_VHF_Filter6_Enabled,            /* H3H5_VHF_Filter6 */
                tmTDA182I2_LPF_Gain_Free                        /* LPF_Gain */
            },
            {                                                   // DVB-T 7MHz */
                3500000,                                        /* IF */
                0,                                              /* CF_Offset */
                tmTDA182I2_LPF_7MHz,                            /* LPF */
                tmTDA182I2_LPFOffset_min_8pc,                   /* LPF_Offset */
                tmTDA182I2_IF_AGC_Gain_1Vpp_min_6_24dB,         /* IF_Gain */
                tmTDA182I2_IF_Notch_Enabled,                    /* IF_Notch */
                tmTDA182I2_IF_HPF_Disabled,                     /* IF_HPF */
                tmTDA182I2_DC_Notch_Enabled,                    /* DC_Notch */
                tmTDA182I2_AGC1_LNA_TOP_d95_u89dBuV,            /* AGC1_LNA_TOP */
                tmTDA182I2_AGC2_RF_Attenuator_TOP_d90_u84dBuV,  /* AGC2_RF_Attenuator_TOP */
                tmTDA182I2_AGC3_RF_AGC_TOP_100dBuV,             /* AGC3_RF_AGC_TOP_Low_band */
                tmTDA182I2_AGC3_RF_AGC_TOP_102dBuV,             /* AGC3_RF_AGC_TOP_High_band */
                tmTDA182I2_AGC4_IR_Mixer_TOP_d110_u105dBuV,     /* AGC4_IR_Mixer_TOP */
                tmTDA182I2_AGC5_IF_AGC_TOP_d110_u105dBuV,       /* AGC5_IF_AGC_TOP */
                tmTDA182I2_AGC5_Detector_HPF_Disabled,          /* AGC5_Detector_HPF */
                tmTDA182I2_AGC3_Adapt_Enabled,                  /* AGC3_Adapt */
                tmTDA182I2_AGC3_Adapt_TOP_98dBuV,               /* AGC3_Adapt_TOP */
                tmTDA182I2_AGC5_Atten_3dB_Enabled,              /* AGC5_Atten_3dB */
                0x02,                                           /* GSK : settings V2.0.0  */
                tmTDA182I2_H3H5_VHF_Filter6_Enabled,            /* H3H5_VHF_Filter6 */
                tmTDA182I2_LPF_Gain_Free                        /* LPF_Gain */
            },
            {                                                   /* DVB-T 8MHz */
                4000000,                                        /* IF */
                0,                                              /* CF_Offset */
                tmTDA182I2_LPF_8MHz,                            /* LPF */
                tmTDA182I2_LPFOffset_0pc,                       /* LPF_Offset */
                tmTDA182I2_IF_AGC_Gain_1Vpp_min_6_24dB,         /* IF_Gain */
                tmTDA182I2_IF_Notch_Enabled,                    /* IF_Notch */
                tmTDA182I2_IF_HPF_Disabled,                     /* IF_HPF */
                tmTDA182I2_DC_Notch_Enabled,                    /* DC_Notch */
                tmTDA182I2_AGC1_LNA_TOP_d95_u89dBuV,            /* AGC1_LNA_TOP */
                tmTDA182I2_AGC2_RF_Attenuator_TOP_d90_u84dBuV,  /* AGC2_RF_Attenuator_TOP */
                tmTDA182I2_AGC3_RF_AGC_TOP_100dBuV,             /* AGC3_RF_AGC_TOP_Low_band */
                tmTDA182I2_AGC3_RF_AGC_TOP_102dBuV,             /* AGC3_RF_AGC_TOP_High_band */
                tmTDA182I2_AGC4_IR_Mixer_TOP_d110_u105dBuV,     /* AGC4_IR_Mixer_TOP */
                tmTDA182I2_AGC5_IF_AGC_TOP_d110_u105dBuV,       /* AGC5_IF_AGC_TOP */
                tmTDA182I2_AGC5_Detector_HPF_Disabled,          /* AGC5_Detector_HPF */
                tmTDA182I2_AGC3_Adapt_Enabled,                  /* AGC3_Adapt */
                tmTDA182I2_AGC3_Adapt_TOP_98dBuV,               /* AGC3_Adapt_TOP */
                tmTDA182I2_AGC5_Atten_3dB_Enabled,              /* AGC5_Atten_3dB */
                0x02,                                           /* GSK : settings V2.0.0  */
                tmTDA182I2_H3H5_VHF_Filter6_Enabled,            /* H3H5_VHF_Filter6 */
                tmTDA182I2_LPF_Gain_Free                        /* LPF_Gain */
            },
#ifdef TMBSL_TDA18272
            {                                                   /* Analog M/N */
                5400000,                                        /* IF */
                1750000,                                        /* CF_Offset */
                tmTDA182I2_LPF_6MHz,                            /* LPF */
                tmTDA182I2_LPFOffset_0pc,                       /* LPF_Offset */
                tmTDA182I2_IF_AGC_Gain_0_7Vpp_min_9_21dB,       /* IF_Gain */
                tmTDA182I2_IF_Notch_Disabled,                   /* IF_Notch */
                tmTDA182I2_IF_HPF_Disabled,                     /* IF_HPF */
                tmTDA182I2_DC_Notch_Disabled,                   /* DC_Notch */
                tmTDA182I2_AGC1_LNA_TOP_d95_u89dBuV,            /* AGC1_LNA_TOP */
                tmTDA182I2_AGC2_RF_Attenuator_TOP_d90_u84dBuV,  /* AGC2_RF_Attenuator_TOP */
                tmTDA182I2_AGC3_RF_AGC_TOP_96dBuV,              /* AGC3_RF_AGC_TOP_Low_band */
                tmTDA182I2_AGC3_RF_AGC_TOP_96dBuV,              /* AGC3_RF_AGC_TOP_High_band */
                tmTDA182I2_AGC4_IR_Mixer_TOP_d105_u100dBuV,     /* AGC4_IR_Mixer_TOP */
                tmTDA182I2_AGC5_IF_AGC_TOP_d105_u100dBuV,       /* AGC5_IF_AGC_TOP */
                tmTDA182I2_AGC5_Detector_HPF_Enabled,           /* AGC5_Detector_HPF */
                tmTDA182I2_AGC3_Adapt_Disabled,                 /* AGC3_Adapt */
                tmTDA182I2_AGC3_Adapt_TOP_102dBuV,              /* AGC3_Adapt_TOP */
                tmTDA182I2_AGC5_Atten_3dB_Disabled,             /* AGC5_Atten_3dB */
                0x01,                                           /* GSK : settings V2.0.0  */
                tmTDA182I2_H3H5_VHF_Filter6_Disabled,           /* H3H5_VHF_Filter6 */
                tmTDA182I2_LPF_Gain_Frozen                      /* LPF_Gain */
            },
            {                                                   /* Analog B */
                6400000,                                        /* IF */
                2250000,                                        /* CF_Offset */
                tmTDA182I2_LPF_7MHz,                            /* LPF */
                tmTDA182I2_LPFOffset_0pc,                       /* LPF_Offset */
                //tmTDA182I2_IF_AGC_Gain_0_7Vpp_min_9_21dB,       /* IF_Gain */
                tmTDA182I2_IF_AGC_Gain_1_25Vpp_min_4_26dB,
                tmTDA182I2_IF_Notch_Disabled,                   /* IF_Notch */
                tmTDA182I2_IF_HPF_Disabled,                     /* IF_HPF */
                tmTDA182I2_DC_Notch_Disabled,                   /* DC_Notch */
                tmTDA182I2_AGC1_LNA_TOP_d95_u89dBuV,            /* AGC1_LNA_TOP */
                tmTDA182I2_AGC2_RF_Attenuator_TOP_d90_u84dBuV,  /* AGC2_RF_Attenuator_TOP */
                tmTDA182I2_AGC3_RF_AGC_TOP_96dBuV,              /* AGC3_RF_AGC_TOP_Low_band */
                tmTDA182I2_AGC3_RF_AGC_TOP_96dBuV,              /* AGC3_RF_AGC_TOP_High_band*/
                tmTDA182I2_AGC4_IR_Mixer_TOP_d105_u100dBuV,     /* AGC4_IR_Mixer_TOP */
                tmTDA182I2_AGC5_IF_AGC_TOP_d105_u100dBuV,       /* AGC5_IF_AGC_TOP */
                tmTDA182I2_AGC5_Detector_HPF_Enabled,           /* AGC5_Detector_HPF */
                tmTDA182I2_AGC3_Adapt_Disabled,                 /* AGC3_Adapt */
                tmTDA182I2_AGC3_Adapt_TOP_102dBuV,              /* AGC3_Adapt_TOP */
                tmTDA182I2_AGC5_Atten_3dB_Disabled,             /* AGC5_Atten_3dB */
                0x01,                                           /* GSK : settings V2.0.0  */
                tmTDA182I2_H3H5_VHF_Filter6_Disabled,           /* H3H5_VHF_Filter6 */
                tmTDA182I2_LPF_Gain_Frozen                      /* LPF_Gain */
            },
            {                                                   /* Analog G/H */
                6750000,                                        /* IF */
                2750000,                                        /* CF_Offset */
                tmTDA182I2_LPF_8MHz,                            /* LPF */
                tmTDA182I2_LPFOffset_0pc,                       /* LPF_Offset */
                //tmTDA182I2_IF_AGC_Gain_0_7Vpp_min_9_21dB,       /* IF_Gain */
                tmTDA182I2_IF_AGC_Gain_1_25Vpp_min_4_26dB,
                tmTDA182I2_IF_Notch_Disabled,                   /* IF_Notch */
                tmTDA182I2_IF_HPF_Disabled,                     /* IF_HPF */
                tmTDA182I2_DC_Notch_Disabled,                   /* DC_Notch */
                tmTDA182I2_AGC1_LNA_TOP_d95_u89dBuV,            /* AGC1_LNA_TOP */
                tmTDA182I2_AGC2_RF_Attenuator_TOP_d90_u84dBuV,  /* AGC2_RF_Attenuator_TOP */
                tmTDA182I2_AGC3_RF_AGC_TOP_96dBuV,              /* AGC3_RF_AGC_TOP_Low_band */
                tmTDA182I2_AGC3_RF_AGC_TOP_96dBuV,              /* AGC3_RF_AGC_TOP_High_band */
                tmTDA182I2_AGC4_IR_Mixer_TOP_d105_u100dBuV,     /* AGC4_IR_Mixer_TOP */
                tmTDA182I2_AGC5_IF_AGC_TOP_d105_u100dBuV,       /* AGC5_IF_AGC_TOP */
                tmTDA182I2_AGC5_Detector_HPF_Enabled,           /* AGC5_Detector_HPF */
                tmTDA182I2_AGC3_Adapt_Disabled,                 /* AGC3_Adapt */
                tmTDA182I2_AGC3_Adapt_TOP_102dBuV,              /* AGC3_Adapt_TOP */
                tmTDA182I2_AGC5_Atten_3dB_Disabled,             /* AGC5_Atten_3dB */
                0x01,                                           /* GSK : settings V2.0.0  */
                tmTDA182I2_H3H5_VHF_Filter6_Disabled,           /* H3H5_VHF_Filter6 */
                tmTDA182I2_LPF_Gain_Frozen                      /* LPF_Gain */
            },
            {                                                   /* Analog I */
                7250000,                                        /* IF */
                2750000,                                        /* CF_Offset */
                tmTDA182I2_LPF_8MHz,                            /* LPF */
                tmTDA182I2_LPFOffset_0pc,                       /* LPF_Offset */
               // tmTDA182I2_IF_AGC_Gain_0_7Vpp_min_9_21dB,       /* IF_Gain */
                tmTDA182I2_IF_AGC_Gain_1_25Vpp_min_4_26dB,
                tmTDA182I2_IF_Notch_Disabled,                   /* IF_Notch */
                tmTDA182I2_IF_HPF_Disabled,                     /* IF_HPF */
                tmTDA182I2_DC_Notch_Disabled,                   /* DC_Notch */
                tmTDA182I2_AGC1_LNA_TOP_d95_u89dBuV,            /* AGC1_LNA_TOP */
                tmTDA182I2_AGC2_RF_Attenuator_TOP_d90_u84dBuV,  /* AGC2_RF_Attenuator_TOP */
                tmTDA182I2_AGC3_RF_AGC_TOP_96dBuV,              /* AGC3_RF_AGC_TOP_Low_band */
                tmTDA182I2_AGC3_RF_AGC_TOP_96dBuV,              /* AGC3_RF_AGC_TOP_High_band */
                tmTDA182I2_AGC4_IR_Mixer_TOP_d105_u100dBuV,     /* AGC4_IR_Mixer_TOP */
                tmTDA182I2_AGC5_IF_AGC_TOP_d105_u100dBuV,       /* AGC5_IF_AGC_TOP */
                tmTDA182I2_AGC5_Detector_HPF_Enabled,           /* AGC5_Detector_HPF */
                tmTDA182I2_AGC3_Adapt_Disabled,                 /* AGC3_Adapt */
                tmTDA182I2_AGC3_Adapt_TOP_102dBuV,              /* AGC3_Adapt_TOP */
                tmTDA182I2_AGC5_Atten_3dB_Disabled,             /* AGC5_Atten_3dB */
                0x01,                                           /* GSK : settings V2.0.0  */
                tmTDA182I2_H3H5_VHF_Filter6_Disabled,           /* H3H5_VHF_Filter6 */
                tmTDA182I2_LPF_Gain_Frozen                      /* LPF_Gain */
            },
            {                                                   /* Analog D/K */
                6850000,                                        /* IF */
                2750000,                                        /* CF_Offset */
                tmTDA182I2_LPF_8MHz,                            /* LPF */
                tmTDA182I2_LPFOffset_0pc,                       /* LPF_Offset */
                //tmTDA182I2_IF_AGC_Gain_0_7Vpp_min_9_21dB,       /* IF_Gain */
                tmTDA182I2_IF_AGC_Gain_1_25Vpp_min_4_26dB,
                tmTDA182I2_IF_Notch_Enabled,                    /* IF_Notch */
                tmTDA182I2_IF_HPF_Disabled,                     /* IF_HPF */
                tmTDA182I2_DC_Notch_Disabled,                   /* DC_Notch */
                tmTDA182I2_AGC1_LNA_TOP_d95_u89dBuV,            /* AGC1_LNA_TOP */
                tmTDA182I2_AGC2_RF_Attenuator_TOP_d90_u84dBuV,  /* AGC2_RF_Attenuator_TOP */
                tmTDA182I2_AGC3_RF_AGC_TOP_96dBuV,              /* AGC3_RF_AGC_TOP_Low_band */
                tmTDA182I2_AGC3_RF_AGC_TOP_96dBuV,              /* AGC3_RF_AGC_TOP_High_band */
                tmTDA182I2_AGC4_IR_Mixer_TOP_d105_u100dBuV,     /* AGC4_IR_Mixer_TOP */
                tmTDA182I2_AGC5_IF_AGC_TOP_d105_u100dBuV,       /* AGC5_IF_AGC_TOP */
                tmTDA182I2_AGC5_Detector_HPF_Enabled,           /* AGC5_Detector_HPF */
                tmTDA182I2_AGC3_Adapt_Disabled,                 /* AGC3_Adapt */
                tmTDA182I2_AGC3_Adapt_TOP_102dBuV,              /* AGC3_Adapt_TOP */
                tmTDA182I2_AGC5_Atten_3dB_Disabled,             /* AGC5_Atten_3dB */
                0x01,                                           /* GSK : settings V2.0.0  */
                tmTDA182I2_H3H5_VHF_Filter6_Disabled,           /* H3H5_VHF_Filter6 */
                tmTDA182I2_LPF_Gain_Frozen                      /* LPF_Gain */
            },
            {                                                   /* Analog L */
                6750000,                                        /* IF */
                2750000,                                        /* CF_Offset */
                tmTDA182I2_LPF_8MHz,                            /* LPF */
                tmTDA182I2_LPFOffset_0pc,                       /* LPF_Offset */
                //tmTDA182I2_IF_AGC_Gain_0_7Vpp_min_9_21dB,       /* IF_Gain */
                tmTDA182I2_IF_AGC_Gain_1_25Vpp_min_4_26dB,
                tmTDA182I2_IF_Notch_Enabled,                    /* IF_Notch */
                tmTDA182I2_IF_HPF_Disabled,                     /* IF_HPF */
                tmTDA182I2_DC_Notch_Disabled,                   /* DC_Notch */
                tmTDA182I2_AGC1_LNA_TOP_d95_u89dBuV,            /* AGC1_LNA_TOP */
                tmTDA182I2_AGC2_RF_Attenuator_TOP_d90_u84dBuV,  /* AGC2_RF_Attenuator_TOP */
                tmTDA182I2_AGC3_RF_AGC_TOP_96dBuV,              /* AGC3_RF_AGC_TOP_Low_band */
                tmTDA182I2_AGC3_RF_AGC_TOP_96dBuV,              /* AGC3_RF_AGC_TOP_High_band */
                tmTDA182I2_AGC4_IR_Mixer_TOP_d105_u100dBuV,     /* AGC4_IR_Mixer_TOP */
                tmTDA182I2_AGC5_IF_AGC_TOP_d105_u100dBuV,       /* AGC5_IF_AGC_TOP */
                tmTDA182I2_AGC5_Detector_HPF_Enabled,           /* AGC5_Detector_HPF */
                tmTDA182I2_AGC3_Adapt_Disabled,                 /* AGC3_Adapt */
                tmTDA182I2_AGC3_Adapt_TOP_102dBuV,              /* AGC3_Adapt_TOP */
                tmTDA182I2_AGC5_Atten_3dB_Disabled,             /* AGC5_Atten_3dB */
                0x01,                                           /* GSK : settings V2.0.0  */
                tmTDA182I2_H3H5_VHF_Filter6_Disabled,           /* H3H5_VHF_Filter6 */
                tmTDA182I2_LPF_Gain_Frozen                      /* LPF_Gain */
            },
            {                                                   /* Analog L' */
                1250000,                                        /* IF */
                -2750000,                                       /* CF_Offset */
                tmTDA182I2_LPF_8MHz,                            /* LPF */
                tmTDA182I2_LPFOffset_0pc,                       /* LPF_Offset */
                //tmTDA182I2_IF_AGC_Gain_0_7Vpp_min_9_21dB,       /* IF_Gain */
                tmTDA182I2_IF_AGC_Gain_1_25Vpp_min_4_26dB,
                tmTDA182I2_IF_Notch_Disabled,                   /* IF_Notch */
                tmTDA182I2_IF_HPF_Disabled,                     /* IF_HPF */
                tmTDA182I2_DC_Notch_Disabled,                   /* DC_Notch */
                tmTDA182I2_AGC1_LNA_TOP_d95_u89dBuV,            /* AGC1_LNA_TOP */
                tmTDA182I2_AGC2_RF_Attenuator_TOP_d90_u84dBuV,  /* AGC2_RF_Attenuator_TOP */
                tmTDA182I2_AGC3_RF_AGC_TOP_96dBuV,              /* AGC3_RF_AGC_TOP_Low_band */
                tmTDA182I2_AGC3_RF_AGC_TOP_96dBuV,              /* AGC3_RF_AGC_TOP_High_band */
                tmTDA182I2_AGC4_IR_Mixer_TOP_d105_u100dBuV,     /* AGC4_IR_Mixer_TOP */
                tmTDA182I2_AGC5_IF_AGC_TOP_d105_u100dBuV,       /* AGC5_IF_AGC_TOP */
                tmTDA182I2_AGC5_Detector_HPF_Disabled,          /* AGC5_Detector_HPF */
                tmTDA182I2_AGC3_Adapt_Disabled,                 /* AGC3_Adapt */
                tmTDA182I2_AGC3_Adapt_TOP_102dBuV,              /* AGC3_Adapt_TOP */
                tmTDA182I2_AGC5_Atten_3dB_Disabled,             /* AGC5_Atten_3dB */
                0x01,                                           /* GSK : settings V2.0.0  */
                tmTDA182I2_H3H5_VHF_Filter6_Disabled,           /* H3H5_VHF_Filter6 */
                tmTDA182I2_LPF_Gain_Frozen                      /* LPF_Gain */
            },
            {                                                   /* Analog FM Radio */
                1250000,                                        /* IF */
                0,                                              /* CF_Offset */
                tmTDA182I2_LPF_1_5MHz,                          /* LPF */
                tmTDA182I2_LPFOffset_0pc,                       /* LPF_Offset */
                tmTDA182I2_IF_AGC_Gain_0_7Vpp_min_9_21dB,       /* IF_Gain */
                tmTDA182I2_IF_Notch_Disabled,                   /* IF_Notch */
                tmTDA182I2_IF_HPF_0_85MHz,                      /* IF_HPF */
                tmTDA182I2_DC_Notch_Enabled,                    /* DC_Notch */
                tmTDA182I2_AGC1_LNA_TOP_d95_u89dBuV,            /* AGC1_LNA_TOP */
                tmTDA182I2_AGC2_RF_Attenuator_TOP_d90_u84dBuV,  /* AGC2_RF_Attenuator_TOP */
                tmTDA182I2_AGC3_RF_AGC_TOP_96dBuV,              /* AGC3_RF_AGC_TOP_Low_band */
                tmTDA182I2_AGC3_RF_AGC_TOP_96dBuV,              /* AGC3_RF_AGC_TOP_High_band */
                tmTDA182I2_AGC4_IR_Mixer_TOP_d105_u100dBuV,     /* AGC4_IR_Mixer_TOP */
                tmTDA182I2_AGC5_IF_AGC_TOP_d105_u100dBuV,       /* AGC5_IF_AGC_TOP */
                tmTDA182I2_AGC5_Detector_HPF_Disabled,          /* AGC5_Detector_HPF */
                tmTDA182I2_AGC3_Adapt_Disabled,                 /* AGC3_Adapt */
                tmTDA182I2_AGC3_Adapt_TOP_102dBuV,              /* AGC3_Adapt_TOP */
                tmTDA182I2_AGC5_Atten_3dB_Disabled,             /* AGC5_Atten_3dB */
                0x02,                                           /* GSK : settings V2.0.0  */
                tmTDA182I2_H3H5_VHF_Filter6_Disabled,           /* H3H5_VHF_Filter6 */
                tmTDA182I2_LPF_Gain_Frozen                      /* LPF_Gain */
            },
            {                                                   /* Blind Scanning copy of PAL-I */
                7250000,                                        /* IF */
                2750000,                                        /* CF_Offset */
                tmTDA182I2_LPF_8MHz,                            /* LPF */
                tmTDA182I2_LPFOffset_0pc,                       /* LPF_Offset */
                tmTDA182I2_IF_AGC_Gain_0_7Vpp_min_9_21dB,       /* IF_Gain */
                tmTDA182I2_IF_Notch_Disabled,                   /* IF_Notch */
                tmTDA182I2_IF_HPF_Disabled,                     /* IF_HPF */
                tmTDA182I2_DC_Notch_Disabled,                   /* DC_Notch */
                tmTDA182I2_AGC1_LNA_TOP_d95_u89dBuV,            /* AGC1_LNA_TOP */
                tmTDA182I2_AGC2_RF_Attenuator_TOP_d90_u84dBuV,  /* AGC2_RF_Attenuator_TOP */
                tmTDA182I2_AGC3_RF_AGC_TOP_96dBuV,              /* AGC3_RF_AGC_TOP_Low_band */
                tmTDA182I2_AGC3_RF_AGC_TOP_96dBuV,              /* AGC3_RF_AGC_TOP_High_band */
                tmTDA182I2_AGC4_IR_Mixer_TOP_d105_u100dBuV,     /* AGC4_IR_Mixer_TOP */
                tmTDA182I2_AGC5_IF_AGC_TOP_d105_u100dBuV,       /* AGC5_IF_AGC_TOP */
                tmTDA182I2_AGC5_Detector_HPF_Enabled,           /* AGC5_Detector_HPF */
                tmTDA182I2_AGC3_Adapt_Disabled,                 /* AGC3_Adapt */
                tmTDA182I2_AGC3_Adapt_TOP_102dBuV,              /* AGC3_Adapt_TOP */
                tmTDA182I2_AGC5_Atten_3dB_Disabled,             /* AGC5_Atten_3dB */
                0x01,                                           /* GSK : settings V2.0.0  */
                tmTDA182I2_H3H5_VHF_Filter6_Disabled,           /* H3H5_VHF_Filter6 */
                tmTDA182I2_LPF_Gain_Frozen                      /* LPF_Gain */
            },
#endif
            {                                                   /* QAM 6MHz */
                3600000,                                        /* IF */
                0,                                              /* CF_Offset */
                tmTDA182I2_LPF_6MHz,                            /* LPF */
                tmTDA182I2_LPFOffset_min_8pc,                   /* LPF_Offset */
                tmTDA182I2_IF_AGC_Gain_1Vpp_min_6_24dB,         /* IF_Gain */
                tmTDA182I2_IF_Notch_Disabled,                   /* IF_Notch */
                tmTDA182I2_IF_HPF_Disabled,                     /* IF_HPF */
                tmTDA182I2_DC_Notch_Enabled,                    /* DC_Notch */
                tmTDA182I2_AGC1_LNA_TOP_d95_u89dBuV,            /* AGC1_LNA_TOP */
                tmTDA182I2_AGC2_RF_Attenuator_TOP_d90_u84dBuV,  /* AGC2_RF_Attenuator_TOP */
                tmTDA182I2_AGC3_RF_AGC_TOP_100dBuV,             /* AGC3_RF_AGC_TOP_Low_band */
                tmTDA182I2_AGC3_RF_AGC_TOP_100dBuV,             /* AGC3_RF_AGC_TOP_High_band */
                tmTDA182I2_AGC4_IR_Mixer_TOP_d110_u105dBuV,     /* AGC4_IR_Mixer_TOP */
                tmTDA182I2_AGC5_IF_AGC_TOP_d110_u105dBuV,       /* AGC5_IF_AGC_TOP */
                tmTDA182I2_AGC5_Detector_HPF_Disabled,          /* AGC5_Detector_HPF */
                tmTDA182I2_AGC3_Adapt_Disabled,                 /* AGC3_Adapt */
                tmTDA182I2_AGC3_Adapt_TOP_102dBuV,              /* AGC3_Adapt_TOP */
                tmTDA182I2_AGC5_Atten_3dB_Disabled,             /* AGC5_Atten_3dB */
                0x02,                                           /* GSK : settings V2.0.0  */
                tmTDA182I2_H3H5_VHF_Filter6_Disabled,           /* H3H5_VHF_Filter6 */
                tmTDA182I2_LPF_Gain_Free                        /* LPF_Gain */
            },
            {                                                   /* QAM 8MHz */
                5000000,                                        /* IF */
                0,                                              /* CF_Offset */
                tmTDA182I2_LPF_9MHz,                            /* LPF */
                tmTDA182I2_LPFOffset_min_8pc,                   /* LPF_Offset */
                tmTDA182I2_IF_AGC_Gain_1Vpp_min_6_24dB,         /* IF_Gain */
                tmTDA182I2_IF_Notch_Disabled,                   /* IF_Notch */
                tmTDA182I2_IF_HPF_0_85MHz,                      /* IF_HPF */
                tmTDA182I2_DC_Notch_Enabled,                    /* DC_Notch */
                tmTDA182I2_AGC1_LNA_TOP_d95_u89dBuV,            /* AGC1_LNA_TOP */
                tmTDA182I2_AGC2_RF_Attenuator_TOP_d90_u84dBuV,  /* AGC2_RF_Attenuator_TOP */
                tmTDA182I2_AGC3_RF_AGC_TOP_100dBuV,             /* AGC3_RF_AGC_TOP_Low_band */
                tmTDA182I2_AGC3_RF_AGC_TOP_100dBuV,             /* AGC3_RF_AGC_TOP_High_band */
                tmTDA182I2_AGC4_IR_Mixer_TOP_d110_u105dBuV,     /* AGC4_IR_Mixer_TOP */
                tmTDA182I2_AGC5_IF_AGC_TOP_d110_u105dBuV,       /* AGC5_IF_AGC_TOP */
                tmTDA182I2_AGC5_Detector_HPF_Disabled,          /* AGC5_Detector_HPF */
                tmTDA182I2_AGC3_Adapt_Disabled,                 /* AGC3_Adapt */
                tmTDA182I2_AGC3_Adapt_TOP_102dBuV,              /* AGC3_Adapt_TOP */
                tmTDA182I2_AGC5_Atten_3dB_Disabled,             /* AGC5_Atten_3dB */
                0x02,                                           /* GSK : settings V2.0.0  */
                tmTDA182I2_H3H5_VHF_Filter6_Disabled,           /* H3H5_VHF_Filter6 */
                tmTDA182I2_LPF_Gain_Free                        /* LPF_Gain */
            },
            {                                                   /* ISDBT 6MHz */
                3250000,                                        /* IF */
                0,                                              /* CF_Offset */
                tmTDA182I2_LPF_6MHz,                            /* LPF */
                tmTDA182I2_LPFOffset_0pc,                       /* LPF_Offset */
                tmTDA182I2_IF_AGC_Gain_0_6Vpp_min_10_3_19_7dB,  /* IF_Gain */
                tmTDA182I2_IF_Notch_Enabled,                    /* IF_Notch */
                tmTDA182I2_IF_HPF_0_4MHz,                       /* IF_HPF */
                tmTDA182I2_DC_Notch_Enabled,                    /* DC_Notch */
                tmTDA182I2_AGC1_LNA_TOP_d95_u89dBuV,            /* AGC1_LNA_TOP */
                tmTDA182I2_AGC2_RF_Attenuator_TOP_d90_u84dBuV,  /* AGC2_RF_Attenuator_TOP */
                tmTDA182I2_AGC3_RF_AGC_TOP_100dBuV,             /* AGC3_RF_AGC_TOP_Low_band */
                tmTDA182I2_AGC3_RF_AGC_TOP_102dBuV,             /* AGC3_RF_AGC_TOP_High_band */
                tmTDA182I2_AGC4_IR_Mixer_TOP_d110_u105dBuV,     /* AGC4_IR_Mixer_TOP */
                tmTDA182I2_AGC5_IF_AGC_TOP_d110_u105dBuV,       /* AGC5_IF_AGC_TOP */
                tmTDA182I2_AGC5_Detector_HPF_Disabled,          /* AGC5_Detector_HPF */
                tmTDA182I2_AGC3_Adapt_Enabled,                  /* AGC3_Adapt */
                tmTDA182I2_AGC3_Adapt_TOP_98dBuV,               /* AGC3_Adapt_TOP */
                tmTDA182I2_AGC5_Atten_3dB_Enabled,              /* AGC5_Atten_3dB */
                0x02,                                           /* GSK : settings V2.0.0  */
                tmTDA182I2_H3H5_VHF_Filter6_Enabled,            /* H3H5_VHF_Filter6 */
                tmTDA182I2_LPF_Gain_Free                        /* LPF_Gain */
            },
            {                                                   /* ATSC 6MHz */
                3250000,                                        /* IF */
                0,                                              /* CF_Offset */
                tmTDA182I2_LPF_6MHz,                            /* LPF */
                tmTDA182I2_LPFOffset_0pc,                       /* LPF_Offset */
                tmTDA182I2_IF_AGC_Gain_0_6Vpp_min_10_3_19_7dB,  /* IF_Gain */
                tmTDA182I2_IF_Notch_Enabled,                    /* IF_Notch */
                tmTDA182I2_IF_HPF_0_4MHz,                       /* IF_HPF */
                tmTDA182I2_DC_Notch_Enabled,                    /* DC_Notch */
                tmTDA182I2_AGC1_LNA_TOP_d100_u94dBuV,           /* AGC1_LNA_TOP */
                tmTDA182I2_AGC2_RF_Attenuator_TOP_d90_u84dBuV,  /* AGC2_RF_Attenuator_TOP */
                tmTDA182I2_AGC3_RF_AGC_TOP_104dBuV,             /* AGC3_RF_AGC_TOP_Low_band */
                tmTDA182I2_AGC3_RF_AGC_TOP_104dBuV,             /* AGC3_RF_AGC_TOP_High_band */
                tmTDA182I2_AGC4_IR_Mixer_TOP_d112_u107dBuV,     /* AGC4_IR_Mixer_TOP */
                tmTDA182I2_AGC5_IF_AGC_TOP_d112_u107dBuV,       /* AGC5_IF_AGC_TOP */
                tmTDA182I2_AGC5_Detector_HPF_Disabled,          /* AGC5_Detector_HPF */
                tmTDA182I2_AGC3_Adapt_Enabled,                  /* AGC3_Adapt */
                tmTDA182I2_AGC3_Adapt_TOP_96dBuV,               /* AGC3_Adapt_TOP */
                tmTDA182I2_AGC5_Atten_3dB_Enabled,              /* AGC5_Atten_3dB */
                0x02,                                           /* GSK : settings V2.0.0  */
                tmTDA182I2_H3H5_VHF_Filter6_Enabled,            /* H3H5_VHF_Filter6 */
                tmTDA182I2_LPF_Gain_Free                        /* LPF_Gain */
            },
            {                                                   /* DMB-T 8MHz */
                4000000,                                        /* IF */
                0,                                              /* CF_Offset */
                tmTDA182I2_LPF_8MHz,                            /* LPF */
                tmTDA182I2_LPFOffset_0pc,                       /* LPF_Offset */
                tmTDA182I2_IF_AGC_Gain_1Vpp_min_6_24dB,         /* IF_Gain */
                tmTDA182I2_IF_Notch_Enabled,                    /* IF_Notch */
                tmTDA182I2_IF_HPF_Disabled,                     /* IF_HPF */
                tmTDA182I2_DC_Notch_Enabled,                    /* DC_Notch */
                tmTDA182I2_AGC1_LNA_TOP_d95_u89dBuV,            /* AGC1_LNA_TOP */
                tmTDA182I2_AGC2_RF_Attenuator_TOP_d90_u84dBuV,  /* AGC2_RF_Attenuator_TOP */
                tmTDA182I2_AGC3_RF_AGC_TOP_100dBuV,             /* AGC3_RF_AGC_TOP_Low_band */
                tmTDA182I2_AGC3_RF_AGC_TOP_102dBuV,             /* AGC3_RF_AGC_TOP_High_band */
                tmTDA182I2_AGC4_IR_Mixer_TOP_d110_u105dBuV,     /* AGC4_IR_Mixer_TOP */
                tmTDA182I2_AGC5_IF_AGC_TOP_d110_u105dBuV,       /* AGC5_IF_AGC_TOP */
                tmTDA182I2_AGC5_Detector_HPF_Disabled,          /* AGC5_Detector_HPF */
                tmTDA182I2_AGC3_Adapt_Enabled,                  /* AGC3_Adapt */
                tmTDA182I2_AGC3_Adapt_TOP_98dBuV,               /* AGC3_Adapt_TOP */
                tmTDA182I2_AGC5_Atten_3dB_Enabled,              /* AGC5_Atten_3dB */
                0x02,                                           /* GSK : settings V2.0.0  */
                tmTDA182I2_H3H5_VHF_Filter6_Enabled,            /* H3H5_VHF_Filter6 */
                tmTDA182I2_LPF_Gain_Free                        /* LPF_Gain */
            }
        }
    },
    {
        (tmUnitSelect_t)(-1),                                   /* tUnit */
        (tmUnitSelect_t)(-1),                                   /* tUnit temporary */
        Null,                                                   /* pMutex */
        False,                                                  /* init (instance initialization default) */
        {                                                       /* sRWFunc */
            Null,
            Null
        },
        {                                                       /* sTime */
            Null,
            Null
        },
        {                                                       /* sDebug */
            Null
        },
        {                                                       /* sMutex */
            Null,
            Null,
            Null,
            Null
        },
        tmTDA182I2_PowerStandbyWithXtalOn,                      /* Power state */
        0,                                                      /* RF */
        tmTDA182I2_DVBT_8MHz,                                   /* Standard mode */
        {
            {   /* Std_Array */                                 /* DVB-T 6MHz */                               
                3250000,                                        /* IF */
                0,                                              /* CF_Offset */
                tmTDA182I2_LPF_6MHz,                            /* LPF */
                tmTDA182I2_LPFOffset_0pc,                       /* LPF_Offset */
                tmTDA182I2_IF_AGC_Gain_1Vpp_min_6_24dB,         /* IF_Gain */
                tmTDA182I2_IF_Notch_Enabled,                    /* IF_Notch */
                tmTDA182I2_IF_HPF_0_4MHz,                       /* IF_HPF */
                tmTDA182I2_DC_Notch_Enabled,                    /* DC_Notch */
                tmTDA182I2_AGC1_LNA_TOP_d95_u89dBuV,            /* AGC1_LNA_TOP */
                tmTDA182I2_AGC2_RF_Attenuator_TOP_d90_u84dBuV,  /* AGC2_RF_Attenuator_TOP */
                tmTDA182I2_AGC3_RF_AGC_TOP_100dBuV,             /* AGC3_RF_AGC_TOP_Low_band */
                tmTDA182I2_AGC3_RF_AGC_TOP_102dBuV,             /* AGC3_RF_AGC_TOP_High_band */
                tmTDA182I2_AGC4_IR_Mixer_TOP_d110_u105dBuV,     /* AGC4_IR_Mixer_TOP */
                tmTDA182I2_AGC5_IF_AGC_TOP_d110_u105dBuV,       /* AGC5_IF_AGC_TOP */
                tmTDA182I2_AGC5_Detector_HPF_Disabled,          /* AGC5_Detector_HPF */
                tmTDA182I2_AGC3_Adapt_Enabled,                  /* AGC3_Adapt */
                tmTDA182I2_AGC3_Adapt_TOP_98dBuV,               /* AGC3_Adapt_TOP */
                tmTDA182I2_AGC5_Atten_3dB_Enabled,              /* AGC5_Atten_3dB */
                0x02,                                           /* GSK : settings V2.0.0  */
                tmTDA182I2_H3H5_VHF_Filter6_Enabled,            /* H3H5_VHF_Filter6 */
                tmTDA182I2_LPF_Gain_Free                        /* LPF_Gain */
            },
            {                                                   // DVB-T 7MHz */
                3500000,                                        /* IF */
                0,                                              /* CF_Offset */
                tmTDA182I2_LPF_7MHz,                            /* LPF */
                tmTDA182I2_LPFOffset_min_8pc,                   /* LPF_Offset */
                tmTDA182I2_IF_AGC_Gain_1Vpp_min_6_24dB,         /* IF_Gain */
                tmTDA182I2_IF_Notch_Enabled,                    /* IF_Notch */
                tmTDA182I2_IF_HPF_Disabled,                     /* IF_HPF */
                tmTDA182I2_DC_Notch_Enabled,                    /* DC_Notch */
                tmTDA182I2_AGC1_LNA_TOP_d95_u89dBuV,            /* AGC1_LNA_TOP */
                tmTDA182I2_AGC2_RF_Attenuator_TOP_d90_u84dBuV,  /* AGC2_RF_Attenuator_TOP */
                tmTDA182I2_AGC3_RF_AGC_TOP_100dBuV,             /* AGC3_RF_AGC_TOP_Low_band */
                tmTDA182I2_AGC3_RF_AGC_TOP_102dBuV,             /* AGC3_RF_AGC_TOP_High_band */
                tmTDA182I2_AGC4_IR_Mixer_TOP_d110_u105dBuV,     /* AGC4_IR_Mixer_TOP */
                tmTDA182I2_AGC5_IF_AGC_TOP_d110_u105dBuV,       /* AGC5_IF_AGC_TOP */
                tmTDA182I2_AGC5_Detector_HPF_Disabled,          /* AGC5_Detector_HPF */
                tmTDA182I2_AGC3_Adapt_Enabled,                  /* AGC3_Adapt */
                tmTDA182I2_AGC3_Adapt_TOP_98dBuV,               /* AGC3_Adapt_TOP */
                tmTDA182I2_AGC5_Atten_3dB_Enabled,              /* AGC5_Atten_3dB */
                0x02,                                           /* GSK : settings V2.0.0  */
                tmTDA182I2_H3H5_VHF_Filter6_Enabled,            /* H3H5_VHF_Filter6 */
                tmTDA182I2_LPF_Gain_Free                        /* LPF_Gain */
            },
            {                                                   /* DVB-T 8MHz */
                4000000,                                        /* IF */
                0,                                              /* CF_Offset */
                tmTDA182I2_LPF_8MHz,                            /* LPF */
                tmTDA182I2_LPFOffset_0pc,                       /* LPF_Offset */
                tmTDA182I2_IF_AGC_Gain_1Vpp_min_6_24dB,         /* IF_Gain */
                tmTDA182I2_IF_Notch_Enabled,                    /* IF_Notch */
                tmTDA182I2_IF_HPF_Disabled,                     /* IF_HPF */
                tmTDA182I2_DC_Notch_Enabled,                    /* DC_Notch */
                tmTDA182I2_AGC1_LNA_TOP_d95_u89dBuV,            /* AGC1_LNA_TOP */
                tmTDA182I2_AGC2_RF_Attenuator_TOP_d90_u84dBuV,  /* AGC2_RF_Attenuator_TOP */
                tmTDA182I2_AGC3_RF_AGC_TOP_100dBuV,             /* AGC3_RF_AGC_TOP_Low_band */
                tmTDA182I2_AGC3_RF_AGC_TOP_102dBuV,             /* AGC3_RF_AGC_TOP_High_band */
                tmTDA182I2_AGC4_IR_Mixer_TOP_d110_u105dBuV,     /* AGC4_IR_Mixer_TOP */
                tmTDA182I2_AGC5_IF_AGC_TOP_d110_u105dBuV,       /* AGC5_IF_AGC_TOP */
                tmTDA182I2_AGC5_Detector_HPF_Disabled,          /* AGC5_Detector_HPF */
                tmTDA182I2_AGC3_Adapt_Enabled,                  /* AGC3_Adapt */
                tmTDA182I2_AGC3_Adapt_TOP_98dBuV,               /* AGC3_Adapt_TOP */
                tmTDA182I2_AGC5_Atten_3dB_Enabled,              /* AGC5_Atten_3dB */
                0x02,                                           /* GSK : settings V2.0.0  */
                tmTDA182I2_H3H5_VHF_Filter6_Enabled,            /* H3H5_VHF_Filter6 */
                tmTDA182I2_LPF_Gain_Free                        /* LPF_Gain */
            },
#ifdef TMBSL_TDA18272
            {                                                   /* Analog M/N */
                5400000,                                        /* IF */
                1750000,                                        /* CF_Offset */
                tmTDA182I2_LPF_6MHz,                            /* LPF */
                tmTDA182I2_LPFOffset_0pc,                       /* LPF_Offset */
                tmTDA182I2_IF_AGC_Gain_0_7Vpp_min_9_21dB,       /* IF_Gain */
                tmTDA182I2_IF_Notch_Disabled,                   /* IF_Notch */
                tmTDA182I2_IF_HPF_Disabled,                     /* IF_HPF */
                tmTDA182I2_DC_Notch_Disabled,                   /* DC_Notch */
                tmTDA182I2_AGC1_LNA_TOP_d95_u89dBuV,            /* AGC1_LNA_TOP */
                tmTDA182I2_AGC2_RF_Attenuator_TOP_d90_u84dBuV,  /* AGC2_RF_Attenuator_TOP */
                tmTDA182I2_AGC3_RF_AGC_TOP_96dBuV,              /* AGC3_RF_AGC_TOP_Low_band */
                tmTDA182I2_AGC3_RF_AGC_TOP_96dBuV,              /* AGC3_RF_AGC_TOP_High_band */
                tmTDA182I2_AGC4_IR_Mixer_TOP_d105_u100dBuV,     /* AGC4_IR_Mixer_TOP */
                tmTDA182I2_AGC5_IF_AGC_TOP_d105_u100dBuV,       /* AGC5_IF_AGC_TOP */
                tmTDA182I2_AGC5_Detector_HPF_Enabled,           /* AGC5_Detector_HPF */
                tmTDA182I2_AGC3_Adapt_Disabled,                 /* AGC3_Adapt */
                tmTDA182I2_AGC3_Adapt_TOP_102dBuV,              /* AGC3_Adapt_TOP */
                tmTDA182I2_AGC5_Atten_3dB_Disabled,             /* AGC5_Atten_3dB */
                0x01,                                           /* GSK : settings V2.0.0  */
                tmTDA182I2_H3H5_VHF_Filter6_Disabled,           /* H3H5_VHF_Filter6 */
                tmTDA182I2_LPF_Gain_Frozen                      /* LPF_Gain */
            },
            {                                                   /* Analog B */
                6400000,                                        /* IF */
                2250000,                                        /* CF_Offset */
                tmTDA182I2_LPF_7MHz,                            /* LPF */
                tmTDA182I2_LPFOffset_0pc,                       /* LPF_Offset */
                tmTDA182I2_IF_AGC_Gain_0_7Vpp_min_9_21dB,       /* IF_Gain */
                tmTDA182I2_IF_Notch_Disabled,                   /* IF_Notch */
                tmTDA182I2_IF_HPF_Disabled,                     /* IF_HPF */
                tmTDA182I2_DC_Notch_Disabled,                   /* DC_Notch */
                tmTDA182I2_AGC1_LNA_TOP_d95_u89dBuV,            /* AGC1_LNA_TOP */
                tmTDA182I2_AGC2_RF_Attenuator_TOP_d90_u84dBuV,  /* AGC2_RF_Attenuator_TOP */
                tmTDA182I2_AGC3_RF_AGC_TOP_96dBuV,              /* AGC3_RF_AGC_TOP_Low_band */
                tmTDA182I2_AGC3_RF_AGC_TOP_96dBuV,              /* AGC3_RF_AGC_TOP_High_band */
                tmTDA182I2_AGC4_IR_Mixer_TOP_d105_u100dBuV,     /* AGC4_IR_Mixer_TOP */
                tmTDA182I2_AGC5_IF_AGC_TOP_d105_u100dBuV,       /* AGC5_IF_AGC_TOP */
                tmTDA182I2_AGC5_Detector_HPF_Enabled,           /* AGC5_Detector_HPF */
                tmTDA182I2_AGC3_Adapt_Disabled,                 /* AGC3_Adapt */
                tmTDA182I2_AGC3_Adapt_TOP_102dBuV,              /* AGC3_Adapt_TOP */
                tmTDA182I2_AGC5_Atten_3dB_Disabled,             /* AGC5_Atten_3dB */
                0x01,                                           /* GSK : settings V2.0.0  */
                tmTDA182I2_H3H5_VHF_Filter6_Disabled,           /* H3H5_VHF_Filter6 */
                tmTDA182I2_LPF_Gain_Frozen                      /* LPF_Gain */
            },
            {                                                   /* Analog G/H */
                6750000,                                        /* IF */
                2750000,                                        /* CF_Offset */
                tmTDA182I2_LPF_8MHz,                            /* LPF */
                tmTDA182I2_LPFOffset_0pc,                       /* LPF_Offset */
                tmTDA182I2_IF_AGC_Gain_0_7Vpp_min_9_21dB,       /* IF_Gain */
                tmTDA182I2_IF_Notch_Disabled,                   /* IF_Notch */
                tmTDA182I2_IF_HPF_Disabled,                     /* IF_HPF */
                tmTDA182I2_DC_Notch_Disabled,                   /* DC_Notch */
                tmTDA182I2_AGC1_LNA_TOP_d95_u89dBuV,            /* AGC1_LNA_TOP */
                tmTDA182I2_AGC2_RF_Attenuator_TOP_d90_u84dBuV,  /* AGC2_RF_Attenuator_TOP */
                tmTDA182I2_AGC3_RF_AGC_TOP_96dBuV,              /* AGC3_RF_AGC_TOP_Low_band */
                tmTDA182I2_AGC3_RF_AGC_TOP_96dBuV,              /* AGC3_RF_AGC_TOP_High_band */
                tmTDA182I2_AGC4_IR_Mixer_TOP_d105_u100dBuV,     /* AGC4_IR_Mixer_TOP */
                tmTDA182I2_AGC5_IF_AGC_TOP_d105_u100dBuV,       /* AGC5_IF_AGC_TOP */
                tmTDA182I2_AGC5_Detector_HPF_Enabled,           /* AGC5_Detector_HPF */
                tmTDA182I2_AGC3_Adapt_Disabled,                 /* AGC3_Adapt */
                tmTDA182I2_AGC3_Adapt_TOP_102dBuV,              /* AGC3_Adapt_TOP */
                tmTDA182I2_AGC5_Atten_3dB_Disabled,             /* AGC5_Atten_3dB */
                0x01,                                           /* GSK : settings V2.0.0  */
                tmTDA182I2_H3H5_VHF_Filter6_Disabled,           /* H3H5_VHF_Filter6 */
                tmTDA182I2_LPF_Gain_Frozen                      /* LPF_Gain */
            },
            {                                                   /* Analog I */
                7250000,                                        /* IF */
                2750000,                                        /* CF_Offset */
                tmTDA182I2_LPF_8MHz,                            /* LPF */
                tmTDA182I2_LPFOffset_0pc,                       /* LPF_Offset */
                tmTDA182I2_IF_AGC_Gain_0_7Vpp_min_9_21dB,       /* IF_Gain */
                tmTDA182I2_IF_Notch_Disabled,                   /* IF_Notch */
                tmTDA182I2_IF_HPF_Disabled,                     /* IF_HPF */
                tmTDA182I2_DC_Notch_Disabled,                   /* DC_Notch */
                tmTDA182I2_AGC1_LNA_TOP_d95_u89dBuV,            /* AGC1_LNA_TOP */
                tmTDA182I2_AGC2_RF_Attenuator_TOP_d90_u84dBuV,  /* AGC2_RF_Attenuator_TOP */
                tmTDA182I2_AGC3_RF_AGC_TOP_96dBuV,              /* AGC3_RF_AGC_TOP_Low_band */
                tmTDA182I2_AGC3_RF_AGC_TOP_96dBuV,              /* AGC3_RF_AGC_TOP_High_band */
                tmTDA182I2_AGC4_IR_Mixer_TOP_d105_u100dBuV,     /* AGC4_IR_Mixer_TOP */
                tmTDA182I2_AGC5_IF_AGC_TOP_d105_u100dBuV,       /* AGC5_IF_AGC_TOP */
                tmTDA182I2_AGC5_Detector_HPF_Enabled,           /* AGC5_Detector_HPF */
                tmTDA182I2_AGC3_Adapt_Disabled,                 /* AGC3_Adapt */
                tmTDA182I2_AGC3_Adapt_TOP_102dBuV,              /* AGC3_Adapt_TOP */
                tmTDA182I2_AGC5_Atten_3dB_Disabled,             /* AGC5_Atten_3dB */
                0x01,                                           /* GSK : settings V2.0.0  */
                tmTDA182I2_H3H5_VHF_Filter6_Disabled,           /* H3H5_VHF_Filter6 */
                tmTDA182I2_LPF_Gain_Frozen                      /* LPF_Gain */
            },
            {                                                   /* Analog D/K */
                6850000,                                        /* IF */
                2750000,                                        /* CF_Offset */
                tmTDA182I2_LPF_8MHz,                            /* LPF */
                tmTDA182I2_LPFOffset_0pc,                       /* LPF_Offset */
                tmTDA182I2_IF_AGC_Gain_0_7Vpp_min_9_21dB,       /* IF_Gain */
                tmTDA182I2_IF_Notch_Enabled,                    /* IF_Notch */
                tmTDA182I2_IF_HPF_Disabled,                     /* IF_HPF */
                tmTDA182I2_DC_Notch_Disabled,                   /* DC_Notch */
                tmTDA182I2_AGC1_LNA_TOP_d95_u89dBuV,            /* AGC1_LNA_TOP */
                tmTDA182I2_AGC2_RF_Attenuator_TOP_d90_u84dBuV,  /* AGC2_RF_Attenuator_TOP */
                tmTDA182I2_AGC3_RF_AGC_TOP_96dBuV,              /* AGC3_RF_AGC_TOP_Low_band */
                tmTDA182I2_AGC3_RF_AGC_TOP_96dBuV,              /* AGC3_RF_AGC_TOP_High_band */
                tmTDA182I2_AGC4_IR_Mixer_TOP_d105_u100dBuV,     /* AGC4_IR_Mixer_TOP */
                tmTDA182I2_AGC5_IF_AGC_TOP_d105_u100dBuV,       /* AGC5_IF_AGC_TOP */
                tmTDA182I2_AGC5_Detector_HPF_Enabled,           /* AGC5_Detector_HPF */
                tmTDA182I2_AGC3_Adapt_Disabled,                 /* AGC3_Adapt */
                tmTDA182I2_AGC3_Adapt_TOP_102dBuV,              /* AGC3_Adapt_TOP */
                tmTDA182I2_AGC5_Atten_3dB_Disabled,             /* AGC5_Atten_3dB */
                0x01,                                           /* GSK : settings V2.0.0  */
                tmTDA182I2_H3H5_VHF_Filter6_Disabled,           /* H3H5_VHF_Filter6 */
                tmTDA182I2_LPF_Gain_Frozen                      /* LPF_Gain */
            },
            {                                                   /* Analog L */
                6750000,                                        /* IF */
                2750000,                                        /* CF_Offset */
                tmTDA182I2_LPF_8MHz,                            /* LPF */
                tmTDA182I2_LPFOffset_0pc,                       /* LPF_Offset */
                tmTDA182I2_IF_AGC_Gain_0_7Vpp_min_9_21dB,       /* IF_Gain */
                tmTDA182I2_IF_Notch_Enabled,                    /* IF_Notch */
                tmTDA182I2_IF_HPF_Disabled,                     /* IF_HPF */
                tmTDA182I2_DC_Notch_Disabled,                   /* DC_Notch */
                tmTDA182I2_AGC1_LNA_TOP_d95_u89dBuV,            /* AGC1_LNA_TOP */
                tmTDA182I2_AGC2_RF_Attenuator_TOP_d90_u84dBuV,  /* AGC2_RF_Attenuator_TOP */
                tmTDA182I2_AGC3_RF_AGC_TOP_96dBuV,              /* AGC3_RF_AGC_TOP_Low_band */
                tmTDA182I2_AGC3_RF_AGC_TOP_96dBuV,              /* AGC3_RF_AGC_TOP_High_band */
                tmTDA182I2_AGC4_IR_Mixer_TOP_d105_u100dBuV,     /* AGC4_IR_Mixer_TOP */
                tmTDA182I2_AGC5_IF_AGC_TOP_d105_u100dBuV,       /* AGC5_IF_AGC_TOP */
                tmTDA182I2_AGC5_Detector_HPF_Enabled,           /* AGC5_Detector_HPF */
                tmTDA182I2_AGC3_Adapt_Disabled,                 /* AGC3_Adapt */
                tmTDA182I2_AGC3_Adapt_TOP_102dBuV,              /* AGC3_Adapt_TOP */
                tmTDA182I2_AGC5_Atten_3dB_Disabled,             /* AGC5_Atten_3dB */
                0x01,                                           /* GSK : settings V2.0.0  */
                tmTDA182I2_H3H5_VHF_Filter6_Disabled,           /* H3H5_VHF_Filter6 */
                tmTDA182I2_LPF_Gain_Frozen                      /* LPF_Gain */
            },
            {                                                   /* Analog L' */
                1250000,                                        /* IF */
                -2750000,                                       /* CF_Offset */
                tmTDA182I2_LPF_8MHz,                            /* LPF */
                tmTDA182I2_LPFOffset_0pc,                       /* LPF_Offset */
                tmTDA182I2_IF_AGC_Gain_0_7Vpp_min_9_21dB,       /* IF_Gain */
                tmTDA182I2_IF_Notch_Disabled,                   /* IF_Notch */
                tmTDA182I2_IF_HPF_Disabled,                     /* IF_HPF */
                tmTDA182I2_DC_Notch_Disabled,                   /* DC_Notch */
                tmTDA182I2_AGC1_LNA_TOP_d95_u89dBuV,            /* AGC1_LNA_TOP */
                tmTDA182I2_AGC2_RF_Attenuator_TOP_d90_u84dBuV,  /* AGC2_RF_Attenuator_TOP */
                tmTDA182I2_AGC3_RF_AGC_TOP_96dBuV,              /* AGC3_RF_AGC_TOP_Low_band */
                tmTDA182I2_AGC3_RF_AGC_TOP_96dBuV,              /* AGC3_RF_AGC_TOP_High_band */
                tmTDA182I2_AGC4_IR_Mixer_TOP_d105_u100dBuV,     /* AGC4_IR_Mixer_TOP */
                tmTDA182I2_AGC5_IF_AGC_TOP_d105_u100dBuV,       /* AGC5_IF_AGC_TOP */
                tmTDA182I2_AGC5_Detector_HPF_Disabled,          /* AGC5_Detector_HPF */
                tmTDA182I2_AGC3_Adapt_Disabled,                 /* AGC3_Adapt */
                tmTDA182I2_AGC3_Adapt_TOP_102dBuV,              /* AGC3_Adapt_TOP */
                tmTDA182I2_AGC5_Atten_3dB_Disabled,             /* AGC5_Atten_3dB */
                0x01,                                           /* GSK : settings V2.0.0  */
                tmTDA182I2_H3H5_VHF_Filter6_Disabled,           /* H3H5_VHF_Filter6 */
                tmTDA182I2_LPF_Gain_Frozen                      /* LPF_Gain */
            },
            {                                                   /* Analog FM Radio */
                1250000,                                        /* IF */
                0,                                              /* CF_Offset */
                tmTDA182I2_LPF_1_5MHz,                          /* LPF */
                tmTDA182I2_LPFOffset_0pc,                       /* LPF_Offset */
                tmTDA182I2_IF_AGC_Gain_0_7Vpp_min_9_21dB,       /* IF_Gain */
                tmTDA182I2_IF_Notch_Disabled,                   /* IF_Notch */
                tmTDA182I2_IF_HPF_0_85MHz,                      /* IF_HPF */
                tmTDA182I2_DC_Notch_Enabled,                    /* DC_Notch */
                tmTDA182I2_AGC1_LNA_TOP_d95_u89dBuV,            /* AGC1_LNA_TOP */
                tmTDA182I2_AGC2_RF_Attenuator_TOP_d90_u84dBuV,  /* AGC2_RF_Attenuator_TOP */
                tmTDA182I2_AGC3_RF_AGC_TOP_96dBuV,              /* AGC3_RF_AGC_TOP_Low_band */
                tmTDA182I2_AGC3_RF_AGC_TOP_96dBuV,              /* AGC3_RF_AGC_TOP_High_band */
                tmTDA182I2_AGC4_IR_Mixer_TOP_d105_u100dBuV,     /* AGC4_IR_Mixer_TOP */
                tmTDA182I2_AGC5_IF_AGC_TOP_d105_u100dBuV,       /* AGC5_IF_AGC_TOP */
                tmTDA182I2_AGC5_Detector_HPF_Disabled,          /* AGC5_Detector_HPF */
                tmTDA182I2_AGC3_Adapt_Disabled,                 /* AGC3_Adapt */
                tmTDA182I2_AGC3_Adapt_TOP_102dBuV,              /* AGC3_Adapt_TOP */
                tmTDA182I2_AGC5_Atten_3dB_Disabled,             /* AGC5_Atten_3dB */
                0x02,                                           /* GSK : settings V2.0.0  */
                tmTDA182I2_H3H5_VHF_Filter6_Disabled,           /* H3H5_VHF_Filter6 */
                tmTDA182I2_LPF_Gain_Frozen                      /* LPF_Gain */
            },
            {                                                   /* Blind Scanning copy of PAL-I */
                7250000,                                        /* IF */
                2750000,                                        /* CF_Offset */
                tmTDA182I2_LPF_8MHz,                            /* LPF */
                tmTDA182I2_LPFOffset_0pc,                       /* LPF_Offset */
                tmTDA182I2_IF_AGC_Gain_0_7Vpp_min_9_21dB,       /* IF_Gain */
                tmTDA182I2_IF_Notch_Disabled,                   /* IF_Notch */
                tmTDA182I2_IF_HPF_Disabled,                     /* IF_HPF */
                tmTDA182I2_DC_Notch_Disabled,                   /* DC_Notch */
                tmTDA182I2_AGC1_LNA_TOP_d95_u89dBuV,            /* AGC1_LNA_TOP */
                tmTDA182I2_AGC2_RF_Attenuator_TOP_d90_u84dBuV,  /* AGC2_RF_Attenuator_TOP */
                tmTDA182I2_AGC3_RF_AGC_TOP_96dBuV,              /* AGC3_RF_AGC_TOP_Low_band */
                tmTDA182I2_AGC3_RF_AGC_TOP_96dBuV,              /* AGC3_RF_AGC_TOP_High_band */
                tmTDA182I2_AGC4_IR_Mixer_TOP_d105_u100dBuV,     /* AGC4_IR_Mixer_TOP */
                tmTDA182I2_AGC5_IF_AGC_TOP_d105_u100dBuV,       /* AGC5_IF_AGC_TOP */
                tmTDA182I2_AGC5_Detector_HPF_Enabled,           /* AGC5_Detector_HPF */
                tmTDA182I2_AGC3_Adapt_Disabled,                 /* AGC3_Adapt */
                tmTDA182I2_AGC3_Adapt_TOP_102dBuV,              /* AGC3_Adapt_TOP */
                tmTDA182I2_AGC5_Atten_3dB_Disabled,             /* AGC5_Atten_3dB */
                0x01,                                           /* GSK : settings V2.0.0  */
                tmTDA182I2_H3H5_VHF_Filter6_Disabled,           /* H3H5_VHF_Filter6 */
                tmTDA182I2_LPF_Gain_Frozen                      /* LPF_Gain */
            },
#endif
            {                                                   /* QAM 6MHz */
                3600000,                                        /* IF */
                0,                                              /* CF_Offset */
                tmTDA182I2_LPF_6MHz,                            /* LPF */
                tmTDA182I2_LPFOffset_min_8pc,                   /* LPF_Offset */
                tmTDA182I2_IF_AGC_Gain_1Vpp_min_6_24dB,         /* IF_Gain */
                tmTDA182I2_IF_Notch_Disabled,                   /* IF_Notch */
                tmTDA182I2_IF_HPF_Disabled,                     /* IF_HPF */
                tmTDA182I2_DC_Notch_Enabled,                    /* DC_Notch */
                tmTDA182I2_AGC1_LNA_TOP_d95_u89dBuV,            /* AGC1_LNA_TOP */
                tmTDA182I2_AGC2_RF_Attenuator_TOP_d90_u84dBuV,  /* AGC2_RF_Attenuator_TOP */
                tmTDA182I2_AGC3_RF_AGC_TOP_100dBuV,             /* AGC3_RF_AGC_TOP_Low_band */
                tmTDA182I2_AGC3_RF_AGC_TOP_100dBuV,             /* AGC3_RF_AGC_TOP_High_band */
                tmTDA182I2_AGC4_IR_Mixer_TOP_d110_u105dBuV,     /* AGC4_IR_Mixer_TOP */
                tmTDA182I2_AGC5_IF_AGC_TOP_d110_u105dBuV,       /* AGC5_IF_AGC_TOP */
                tmTDA182I2_AGC5_Detector_HPF_Disabled,          /* AGC5_Detector_HPF */
                tmTDA182I2_AGC3_Adapt_Disabled,                 /* AGC3_Adapt */
                tmTDA182I2_AGC3_Adapt_TOP_102dBuV,              /* AGC3_Adapt_TOP */
                tmTDA182I2_AGC5_Atten_3dB_Disabled,             /* AGC5_Atten_3dB */
                0x02,                                           /* GSK : settings V2.0.0  */
                tmTDA182I2_H3H5_VHF_Filter6_Disabled,           /* H3H5_VHF_Filter6 */
                tmTDA182I2_LPF_Gain_Free                        /* LPF_Gain */
            },
            {                                                   /* QAM 8MHz */
                5000000,                                        /* IF */
                0,                                              /* CF_Offset */
                tmTDA182I2_LPF_9MHz,                            /* LPF */
                tmTDA182I2_LPFOffset_min_8pc,                   /* LPF_Offset */
                tmTDA182I2_IF_AGC_Gain_1Vpp_min_6_24dB,         /* IF_Gain */
                tmTDA182I2_IF_Notch_Disabled,                   /* IF_Notch */
                tmTDA182I2_IF_HPF_0_85MHz,                      /* IF_HPF */
                tmTDA182I2_DC_Notch_Enabled,                    /* DC_Notch */
                tmTDA182I2_AGC1_LNA_TOP_d95_u89dBuV,            /* AGC1_LNA_TOP */
                tmTDA182I2_AGC2_RF_Attenuator_TOP_d90_u84dBuV,  /* AGC2_RF_Attenuator_TOP */
                tmTDA182I2_AGC3_RF_AGC_TOP_100dBuV,             /* AGC3_RF_AGC_TOP_Low_band */
                tmTDA182I2_AGC3_RF_AGC_TOP_100dBuV,             /* AGC3_RF_AGC_TOP_High_band */
                tmTDA182I2_AGC4_IR_Mixer_TOP_d110_u105dBuV,     /* AGC4_IR_Mixer_TOP */
                tmTDA182I2_AGC5_IF_AGC_TOP_d110_u105dBuV,       /* AGC5_IF_AGC_TOP */
                tmTDA182I2_AGC5_Detector_HPF_Disabled,          /* AGC5_Detector_HPF */
                tmTDA182I2_AGC3_Adapt_Disabled,                 /* AGC3_Adapt */
                tmTDA182I2_AGC3_Adapt_TOP_102dBuV,              /* AGC3_Adapt_TOP */
                tmTDA182I2_AGC5_Atten_3dB_Disabled,             /* AGC5_Atten_3dB */
                0x02,                                           /* GSK : settings V2.0.0  */
                tmTDA182I2_H3H5_VHF_Filter6_Disabled,           /* H3H5_VHF_Filter6 */
                tmTDA182I2_LPF_Gain_Free                        /* LPF_Gain */
            },
            {                                                   /* ISDBT 6MHz */
                3250000,                                        /* IF */
                0,                                              /* CF_Offset */
                tmTDA182I2_LPF_6MHz,                            /* LPF */
                tmTDA182I2_LPFOffset_0pc,                       /* LPF_Offset */
                tmTDA182I2_IF_AGC_Gain_0_6Vpp_min_10_3_19_7dB,  /* IF_Gain */
                tmTDA182I2_IF_Notch_Enabled,                    /* IF_Notch */
                tmTDA182I2_IF_HPF_0_4MHz,                       /* IF_HPF */
                tmTDA182I2_DC_Notch_Enabled,                    /* DC_Notch */
                tmTDA182I2_AGC1_LNA_TOP_d95_u89dBuV,            /* AGC1_LNA_TOP */
                tmTDA182I2_AGC2_RF_Attenuator_TOP_d90_u84dBuV,  /* AGC2_RF_Attenuator_TOP */
                tmTDA182I2_AGC3_RF_AGC_TOP_100dBuV,             /* AGC3_RF_AGC_TOP_Low_band */
                tmTDA182I2_AGC3_RF_AGC_TOP_102dBuV,             /* AGC3_RF_AGC_TOP_High_band */
                tmTDA182I2_AGC4_IR_Mixer_TOP_d110_u105dBuV,     /* AGC4_IR_Mixer_TOP */
                tmTDA182I2_AGC5_IF_AGC_TOP_d110_u105dBuV,       /* AGC5_IF_AGC_TOP */
                tmTDA182I2_AGC5_Detector_HPF_Disabled,          /* AGC5_Detector_HPF */
                tmTDA182I2_AGC3_Adapt_Enabled,                  /* AGC3_Adapt */
                tmTDA182I2_AGC3_Adapt_TOP_98dBuV,               /* AGC3_Adapt_TOP */
                tmTDA182I2_AGC5_Atten_3dB_Enabled,              /* AGC5_Atten_3dB */
                0x02,                                           /* GSK : settings V2.0.0  */
                tmTDA182I2_H3H5_VHF_Filter6_Enabled,            /* H3H5_VHF_Filter6 */
                tmTDA182I2_LPF_Gain_Free                        /* LPF_Gain */
            },
            {                                                   /* ATSC 6MHz */
                3250000,                                        /* IF */
                0,                                              /* CF_Offset */
                tmTDA182I2_LPF_6MHz,                            /* LPF */
                tmTDA182I2_LPFOffset_0pc,                       /* LPF_Offset */
                tmTDA182I2_IF_AGC_Gain_0_6Vpp_min_10_3_19_7dB,  /* IF_Gain */
                tmTDA182I2_IF_Notch_Enabled,                    /* IF_Notch */
                tmTDA182I2_IF_HPF_0_4MHz,                       /* IF_HPF */
                tmTDA182I2_DC_Notch_Enabled,                    /* DC_Notch */
                tmTDA182I2_AGC1_LNA_TOP_d100_u94dBuV,           /* AGC1_LNA_TOP */
                tmTDA182I2_AGC2_RF_Attenuator_TOP_d90_u84dBuV,  /* AGC2_RF_Attenuator_TOP */
                tmTDA182I2_AGC3_RF_AGC_TOP_104dBuV,             /* AGC3_RF_AGC_TOP_Low_band */
                tmTDA182I2_AGC3_RF_AGC_TOP_104dBuV,             /* AGC3_RF_AGC_TOP_High_band */
                tmTDA182I2_AGC4_IR_Mixer_TOP_d112_u107dBuV,     /* AGC4_IR_Mixer_TOP */
                tmTDA182I2_AGC5_IF_AGC_TOP_d112_u107dBuV,       /* AGC5_IF_AGC_TOP */
                tmTDA182I2_AGC5_Detector_HPF_Disabled,          /* AGC5_Detector_HPF */
                tmTDA182I2_AGC3_Adapt_Enabled,                  /* AGC3_Adapt */
                tmTDA182I2_AGC3_Adapt_TOP_96dBuV,               /* AGC3_Adapt_TOP */
                tmTDA182I2_AGC5_Atten_3dB_Enabled,              /* AGC5_Atten_3dB */
                0x02,                                           /* GSK : settings V2.0.0  */
                tmTDA182I2_H3H5_VHF_Filter6_Enabled,            /* H3H5_VHF_Filter6 */
                tmTDA182I2_LPF_Gain_Free                        /* LPF_Gain */
            },
            {                                                   /* DMB-T 8MHz */
                4000000,                                        /* IF */
                0,                                              /* CF_Offset */
                tmTDA182I2_LPF_8MHz,                            /* LPF */
                tmTDA182I2_LPFOffset_0pc,                       /* LPF_Offset */
                tmTDA182I2_IF_AGC_Gain_1Vpp_min_6_24dB,         /* IF_Gain */
                tmTDA182I2_IF_Notch_Enabled,                    /* IF_Notch */
                tmTDA182I2_IF_HPF_Disabled,                     /* IF_HPF */
                tmTDA182I2_DC_Notch_Enabled,                    /* DC_Notch */
                tmTDA182I2_AGC1_LNA_TOP_d95_u89dBuV,            /* AGC1_LNA_TOP */
                tmTDA182I2_AGC2_RF_Attenuator_TOP_d90_u84dBuV,  /* AGC2_RF_Attenuator_TOP */
                tmTDA182I2_AGC3_RF_AGC_TOP_100dBuV,             /* AGC3_RF_AGC_TOP_Low_band */
                tmTDA182I2_AGC3_RF_AGC_TOP_102dBuV,             /* AGC3_RF_AGC_TOP_High_band */
                tmTDA182I2_AGC4_IR_Mixer_TOP_d110_u105dBuV,     /* AGC4_IR_Mixer_TOP */
                tmTDA182I2_AGC5_IF_AGC_TOP_d110_u105dBuV,       /* AGC5_IF_AGC_TOP */
                tmTDA182I2_AGC5_Detector_HPF_Disabled,          /* AGC5_Detector_HPF */
                tmTDA182I2_AGC3_Adapt_Enabled,                  /* AGC3_Adapt */
                tmTDA182I2_AGC3_Adapt_TOP_98dBuV,               /* AGC3_Adapt_TOP */
                tmTDA182I2_AGC5_Atten_3dB_Enabled,              /* AGC5_Atten_3dB */
                0x02,                                           /* GSK : settings V2.0.0  */
                tmTDA182I2_H3H5_VHF_Filter6_Enabled,            /* H3H5_VHF_Filter6 */
                tmTDA182I2_LPF_Gain_Free                        /* LPF_Gain */
            }
        }
    }
};


//-----------------------------------------------------------------------------
// FUNCTION:    TDA182I2AllocInstance:
//
// DESCRIPTION: allocate new instance
//
// RETURN:      
//
// NOTES:       
//-----------------------------------------------------------------------------
//
tmErrorCode_t
TDA182I2AllocInstance
(
    tmUnitSelect_t          tUnit,      /* I: Unit number */
    pptmTDA182I2Object_t    ppDrvObject /* I: Device Object */
)
{ 
    tmErrorCode_t       err = TDA182I2_ERR_BAD_UNIT_NUMBER;
    ptmTDA182I2Object_t pObj = Null;
    UInt32              uLoopCounter = 0;    

    /* Find a free instance */
    for(uLoopCounter = 0; uLoopCounter<TDA182I2_MAX_UNITS; uLoopCounter++)
    {
        pObj = &gTDA182I2Instance[uLoopCounter];
        if(pObj->init == False)
        {
            pObj->tUnit = tUnit;
            pObj->tUnitW = tUnit;

            *ppDrvObject = pObj;
            err = TM_OK;
            break;
        }
    }

    /* return value */
    return err;
}

//-----------------------------------------------------------------------------
// FUNCTION:    TDA182I2DeAllocInstance:
//
// DESCRIPTION: deallocate instance
//
// RETURN:      always TM_OK
//
// NOTES:       
//-----------------------------------------------------------------------------
//
tmErrorCode_t
TDA182I2DeAllocInstance
(
    tmUnitSelect_t  tUnit   /* I: Unit number */
)
{     
    tmErrorCode_t       err = TDA182I2_ERR_BAD_UNIT_NUMBER;
    ptmTDA182I2Object_t pObj = Null;

    /* check input parameters */
    err = TDA182I2GetInstance(tUnit, &pObj);

    /* check driver state */
    if (err == TM_OK)
    {
        if (pObj == Null || pObj->init == False)
        {
            err = TDA182I2_ERR_NOT_INITIALIZED;
        }
    }

    if ((err == TM_OK) && (pObj != Null)) 
    {
        pObj->init = False;
    }

    /* return value */
    return err;
}

//-----------------------------------------------------------------------------
// FUNCTION:    TDA182I2GetInstance:
//
// DESCRIPTION: get the instance
//
// RETURN:      always True
tmErrorCode_t TDA182I2GetInstance (tmUnitSelect_t tUnit, pptmTDA182I2Object_t ppDrvObject)
{     
    tmErrorCode_t       err = TDA182I2_ERR_NOT_INITIALIZED;
    ptmTDA182I2Object_t pObj = Null;
    UInt32              uLoopCounter = 0;    

    /* get instance */
    for(uLoopCounter = 0; uLoopCounter<TDA182I2_MAX_UNITS; uLoopCounter++)
    {
        pObj = &gTDA182I2Instance[uLoopCounter];

		mcSHOW_DBG_MSG(("TDA182I2GetInstance:tUnit(0x%08X) .\n", pObj->tUnit));
		mcSHOW_DBG_MSG(("TDA182I2GetInstance:tUnitW(0x%08X) .\n", pObj->tUnitW));
		mcSHOW_DBG_MSG(("TDA182I2GetInstance:init(0x%08X) .\n", pObj->init));
		if ((pObj->sRWFunc.Read !=Null) &&(pObj->sRWFunc.Write!= Null))
			{

             mcSHOW_DBG_MSG(("TDA182I2GetInstance:sRWFuncis not null.\n"));
		    }
		else
			{
              mcSHOW_DBG_MSG(("TDA182I2GetInstance:sRWFunc is  null.\n"));

		    }
		
        if(pObj->init == True && pObj->tUnit == GET_INDEX_TYPE_TUNIT(tUnit))
        {
            pObj->tUnitW = tUnit;
            
            *ppDrvObject = pObj;
            err = TM_OK;
            break;
        }
    }

    /* return value */
    return err;
}

