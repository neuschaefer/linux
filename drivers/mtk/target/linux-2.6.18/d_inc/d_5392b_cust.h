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
 * $RCSfile: d_5392b_cust.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *---------------------------------------------------------------------------*/

#ifndef _D_5381_CUST_H_
#define _D_5381_CUST_H_

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#include "u_common.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/

/*
   Video driver custom types.
*/
typedef enum
{
    DRV_CUSTOM_VID_TYPE_BEGIN    =    0,

    /* ADC correction */
    DRV_CUSTOM_VID_TYPE_ADC_R_GAIN,              /* Red/Y gain (by source) */
    DRV_CUSTOM_VID_TYPE_ADC_G_GAIN,              /* Green/Pr gain (by source) */
    DRV_CUSTOM_VID_TYPE_ADC_B_GAIN,              /* Blue gain/Pb (by source) */
    DRV_CUSTOM_VID_TYPE_ADC_R_OFFSET,            /* Red/Y offset (by source) */
    DRV_CUSTOM_VID_TYPE_ADC_G_OFFSET,            /* Green/Pr offset (by source) */
    DRV_CUSTOM_VID_TYPE_ADC_B_OFFSET,            /* Blue/Pb offset (by source) */

    /* Default value */
    DRV_CUSTOM_VID_TYPE_BRIGHTNESS_DFT,         /* Brightness default (by source) */
    DRV_CUSTOM_VID_TYPE_CONTRAST_DFT,           /* Contrast default (by source) */
    DRV_CUSTOM_VID_TYPE_SATURATION_DFT,         /* Saturation (color) default (by source) */
    DRV_CUSTOM_VID_TYPE_HUE_DFT,                /* Hue (tint) default (by source) */

    /* YPbPr Position */
    DRV_CUSTOM_VID_TYPE_YPBPR_H_POS,             /* YPbPr horizontal position (by timing) */
    DRV_CUSTOM_VID_TYPE_YPBPR_V_POS,             /* YPbPr vertical position (by timing) */

    /* YPbPr SOGY slicer setting */
    DRV_CUSTOM_VID_TYPE_YPBPR_VTH,               /* YPbPr Slicer high threshold */
    DRV_CUSTOM_VID_TYPE_YPBPR_VTL,               /* YPbPr Slicer high threshold */
    /* IRE */
    DRV_CUSTOM_VID_TYPE_IRE,                     /* IRE, ON/OFF */

    /* Non-linear factor */
    DRV_CUSTOM_VID_TYPE_NON_LINEAR_FACTOR,       /* Non-linear factor */

    /* Backlight value */
    DRV_CUSTOM_DISP_TYPE_BACK_LIGHT_MIN,         /* Backlight minimum (by source) */
    DRV_CUSTOM_DISP_TYPE_BACK_LIGHT_MAX,         /* Backlight maximum (by source) */
    DRV_CUSTOM_DISP_TYPE_BACK_LIGHT_DFT,         /* Backlight maximum (by source) */
    DRV_CUSTOM_DISP_TYPE_ADP_BACK_LIGHT_NORMAL,  /* Adaptive back light normal */
    DRV_CUSTOM_DISP_TYPE_ADP_BACK_LIGHT_MIDDLE,  /* Adaptive back light middle */
    DRV_CUSTOM_DISP_TYPE_ADP_BACK_LIGHT_HIGH,    /* Adaptive back light high */

	/* Color temperature */
	DRV_CUSTOM_DISP_TYPE_CT_RED_GAIN,			/* Color temperature red gain */
	DRV_CUSTOM_DISP_TYPE_CT_GREEN_GAIN,			/* Color temperature green gain */
	DRV_CUSTOM_DISP_TYPE_CT_BLUE_GAIN,			/* Color temperature blue gain */
	DRV_CUSTOM_DISP_TYPE_CT_RED_OFFSET,			/* Color temperature red offset */
	DRV_CUSTOM_DISP_TYPE_CT_GREEN_OFFSET,		/* Color temperature green offset */
	DRV_CUSTOM_DISP_TYPE_CT_BLUE_OFFSET,		/* Color temperature blue offset */

	/* Gamma table */
	DRV_CUSTOM_DISP_TYPE_GAMMA,
	DRV_CUSTOM_DISP_TYPE_OUTRGB_COLOR_ONOFF,
	DRV_CUSTOM_DISP_TYPE_OUTRGB_COLOR,
	DRV_CUSTOM_DISP_TYPE_SOFTBWS,                        /*SoftBWS*/

    /* Spread spectrum */
    DRV_CUSTOM_DISP_TYPE_SPREAD_SPECTRUM,       /* Spread spectrum. 0-1000 */

	/* Smart Picture */
	DRV_CUSTOM_DISP_TYPE_SMART_PICTURE,

	/* Feature Selection */
	DRV_CUSTOM_DISP_TYPE_FEATURE_SELECTIOIN,

	/* PC/HD mode change */
	DRV_CUSTOM_DISP_TYPE_HDMI_PCHD_MODE,

	/* Smart Picture Index */
	DRV_CUSTOM_DISP_TYPE_SMART_PICTURE_INDEX,

	/* PQ ver */
	DRV_CUSTOM_DISP_TYPE_PQ_VER,

	/* Lightguide */
	DRV_CUSTOM_DISP_TYPE_LIGHT_GUIDE,

    /* MJC setting.*/
	DRV_CUSTOM_DISP_TYPE_MJC_LVL,
	DRV_CUSTOM_DISP_TYPE_MJC_DEMO,
	
    /* Black Corrector */
    DRV_CUSTOM_VID_TYPE_BLACK_CORRECTOR,	

	DRV_CUSTOM_VID_TYPE_END

}   DRV_CUSTOM_VID_TYPE_T;

#define DRV_CUSTOM_VID_MAIN                      ((UINT32)    0)
#define DRV_CUSTOM_VID_SUB                       ((UINT32)    1)

typedef enum
{
    DTV_CUSTOM_VID_INP_FLAG_NONE,
    DRV_CUSTOM_VID_INP_FLAG_AV,
	DRV_CUSTOM_VID_INP_FLAG_ATV,
	DRV_CUSTOM_VID_INP_FLAG_DTV,
	DRV_CUSTOM_VID_INP_FLAG_YPBPR,
	DRV_CUSTOM_VID_INP_FLAG_VGA,
	DRV_CUSTOM_VID_INP_FLAG_HDMI,
	DRV_CUSTOM_VID_INP_FLAG_DVI
}DRV_CUSTOM_VID_INP_T;

typedef enum
{
	DRV_CUSTOM_DISP_CT0,
	DRV_CUSTOM_DISP_CT1,
	DRV_CUSTOM_DISP_CT2,
	DRV_CUSTOM_DISP_CT3,
	DRV_CUSTOM_DISP_CT4,
	DRV_CUSTOM_DISP_CT5,
	DRV_CUSTOM_DISP_CT6
}DRV_CUSTOM_COLOR_TEMP_T;

typedef enum
{
	DRV_CUSTOM_HDMI_AUTO,
	DRV_CUSTOM_HDMI_PC,
	DRV_CUSTOM_HDMI_HD
}DRV_CUSTOM_HDMI_PCHD_MODE;

typedef struct
{
    UINT8           ui1_val;    /* 0~100 % */
}   DRV_CUSTOM_LIGHT_GUIDE_PARAM_T;

typedef struct _VID_NON_LINEAR_INFO_T
{
    BOOL            b_auto;
    UINT32          ui4_path;    /* DRV_CUSTOM_VID_MAIN or DRV_CUSTOM_VID_SUB */
    UINT32          ui4_start;
    UINT32          ui4_slope;
    UINT32          ui4_middle;
    UINT32          ui4_stop;
} VID_NON_LINEAR_INFO_T;

typedef struct _VID_INFO_T
{
    DRV_CUSTOM_VID_INP_T	e_vid_inp;
    UINT16					ui2Value;
	BOOL					b_ignore_hw;
} VID_INFO_T;

typedef struct _GAMMA_INFO_T
{
	DRV_CUSTOM_COLOR_TEMP_T	e_color_temp;
    UINT8	ui1Channel;
    UINT8	ui1Tbl[256];
} GAMMA_INFO_T;

typedef struct _OUTRGB_INFO_T
{
    UINT8	ui1_r_value;
    UINT8	ui1_g_value;
    UINT8	ui1_b_value;
} OUTRGB_INFO_T;

typedef struct _SMART_PIC_INFO_T
{
	UINT8	bSmartPicContrast;
	UINT8	bSmartPicBright;
	UINT8	bSmartPicHue;
	UINT8	bSmartPicSaturation;
	UINT8	bSmartPicSharpness;
	UINT8	bSmartPicColorTemp;
	UINT8	bSmartPicRes1;
	UINT8	bSmartPicRes2;
	UINT16	wSmartPicFeatureSel[12];
}SMART_PIC_INFO_T;

typedef struct _SMART_PIC_GROUP_INFO_T
{
	struct	_SMART_PIC_INFO_T aSmartPic[6];
	UINT8	bSmartPicPqRes[32];
}SMART_PIC_GROUP_INFO_T;

typedef enum
{
	DRV_CUSTOM_AUD_AVL_TARGET_LEVEL,
	DRV_CUSTOM_AUD_AVL_THRESHOLD,
	DRV_CUSTOM_AUD_AVL_ADJUST_RATE
}DRV_CUSTOM_AVL_ADJ_T;

typedef enum
{
	DRV_CUSTOM_AUD_SPK_LARGE,
	DRV_CUSTOM_AUD_SPK_SMALL
}DRV_CUSTOM_AUD_SPK_SIZE_T;

typedef enum
{
	DRV_CUSTOM_AUD_SPK_TYPE_A = 0,
	DRV_CUSTOM_AUD_SPK_TYPE_B
}DRV_CUSTOM_AUD_SPK_TYPE_T;

typedef enum
{
	DRV_CUSTOM_AUD_CUT_OFF_70 = 70,
	DRV_CUSTOM_AUD_CUT_OFF_100 = 100,
	DRV_CUSTOM_AUD_CUT_OFF_200 = 200,
	DRV_CUSTOM_AUD_CUT_OFF_300 = 300
}DRV_CUSTOM_AUD_CUT_OFF_FREQ_T;

typedef enum
{
	DRV_CUSTOM_AUD_LOUDNESS_OFF = 0,
	DRV_CUSTOM_AUD_LOUDNESS_ON
}DRV_CUSTOM_AUD_LOUDNESS_T;

typedef struct
{
    UINT8   ui1_point_num;       /* number of point number. ui1_point_num >= 2 */
    UINT8   ui1_vol_points[8];   /* points of volume curve. The value should be in incremental sequence.
                                    ui1_vol_points[0] must be 0
                                    ui1_vol_points[ui1_point_num-1] must be 100 */
    UINT32  ui4_vol_values[8];   /* values of volume curve. */
}   DRV_CUSTOM_AUD_VOL_CURVE_T;

/*
   Audio driver custom types.
*/
typedef enum
{
    DRV_CUSTOM_AUD_TYPE_BEGIN    =    0,

    DRV_CUSTOM_AUD_TYPE_VOL_GAIN,               /* Volume (by source) */
    DRV_CUSTOM_AUD_TYPE_HEADPHONE_VOL,          /* Head phone volume */
    DRV_CUSTOM_AUD_TYPE_AVL_GAIN,           	/* AVL setting */
    DRV_CUSTOM_AUD_TYPE_SPEAKER_SIZE,           /* Speaker size, large or small */
    DRV_CUSTOM_AUD_TYPE_SPEAKER_TYPE,           /* Speaker type. depend on customer's definition */
    DRV_CUSTOM_AUD_TYPE_SPECTRUM_INFO,          /* Spectrum Info */
    /* MTS FACTORY MODE */
    DRV_CUSTOM_AUD_TYPE_MTS_NUM_CHECK,          /* UINT16, numbers of check */
    DRV_CUSTOM_AUD_TYPE_MTS_NUM_PILOT,          /* UINT16, numbers of pilot */
    DRV_CUSTOM_AUD_TYPE_MTS_PILOT_HIGH,         /* UINT16, pilot higher threshold */
    DRV_CUSTOM_AUD_TYPE_MTS_PILOT_LOW,          /* UINT16, pilot lower threshold */
    DRV_CUSTOM_AUD_TYPE_MTS_NUM_SAP,            /* UINT16, numbers of sap */
    DRV_CUSTOM_AUD_TYPE_MTS_SAP_HIGH,           /* UINT16, sap higher threshold */
    DRV_CUSTOM_AUD_TYPE_MTS_SAP_LOW,            /* UINT16, sap lower threshold */
    DRV_CUSTOM_AUD_TYPE_MTS_HDEV,               /* BOOL, high deviation mode on/off */
    DRV_CUSTOM_AUD_TYPE_MTS_CARRIER_SHIFT,      /* BOOL, carrier shift mode on/off */
    DRV_CUSTOM_AUD_TYPE_MTS_FM_SATU,            /* BOOL, fm saturation mute mode on/off */
    DRV_CUSTOM_AUD_TYPE_MTS_FM_SATU_HIGH,       /* UINT8, fm saturation mute mode on/off */
    DRV_CUSTOM_AUD_TYPE_MTS_FM_SATU_LOW,         /* UINT8, fm saturation mute mode on/off */
    DRV_CUSTOM_AUD_TYPE_MTS_FM_MUTE,            /* BOOL, carrier sense mute mode on/off */
    DRV_CUSTOM_AUD_TYPE_MTS_FM_MUTE_HIGH,       /* UINT8, higher carrier sense mute threshold */
    DRV_CUSTOM_AUD_TYPE_MTS_FM_MUTE_LOW,        /* UINT8, lower carrier sense mute threshold */
    DRV_CUSTOM_AUD_TYPE_MTS_FINE_VOLUME,        /* UINT8, fine tune volume for mono and stereo */
    DRV_CUSTOM_AUD_TYPE_SAP_FINE_VOLUME,        /* UINT8, fine tune volume for sap */
    DRV_CUSTOM_AUD_TYPE_MTS_PILOT_OFFSET,       /* BOOL, pilot shift calibration mode on/off */
    DRV_CUSTOM_AUD_TYPE_SAP_MUTE_LOW,           /* UINT8, sap mute lower threshold */
    DRV_CUSTOM_AUD_TYPE_SAP_MUTE_HIGH,          /* UINT8, sap mute higher threshold */
    /* A2 FACTORY MODE */
    DRV_CUSTOM_AUD_TYPE_A2_NUM_CHECK,           /* UINT16, numbers of check */
    DRV_CUSTOM_AUD_TYPE_A2_NUM_DOUBLE,          /* UINT16, numbers of double check */
    DRV_CUSTOM_AUD_TYPE_A2_MONO_WEIGHT,         /* UINT16, mono wieght */
    DRV_CUSTOM_AUD_TYPE_A2_STEREO_WEIGHT,       /* UINT16, stereo wieght */
    DRV_CUSTOM_AUD_TYPE_A2_DUAL_WEIGHT,         /* UINT16, dual wieght */
    DRV_CUSTOM_AUD_TYPE_A2_HDEV,                /* BOOL, high deviation mode on/off */
    DRV_CUSTOM_AUD_TYPE_A2_CARRIER_SHIFT,       /* BOOL, carrier shift mode on/off */
    DRV_CUSTOM_AUD_TYPE_A2_FM_MUTE,             /* UINT8, carrier sense mute mode on/off */
    DRV_CUSTOM_AUD_TYPE_A2_FM_MUTE_HIGH,        /* UINT8, higher carrier sense mute threshold */
    DRV_CUSTOM_AUD_TYPE_A2_FM_MUTE_LOW,         /* UINT8, lower carrier sense mute threshold */
    DRV_CUSTOM_AUD_TYPE_A2_FINE_VOLUME,         /* UINT8, fine tune volume */
    /* PAL FACTORY MODE */
    DRV_CUSTOM_AUD_TYPE_PAL_CORRECT,            /* UINT16, correct threshold */
    DRV_CUSTOM_AUD_TYPE_PAL_SYNC_LOOP,          /* UINT16, total sync loop */
    DRV_CUSTOM_AUD_TYPE_PAL_ERROR,              /* UINT16, error threshold */
    DRV_CUSTOM_AUD_TYPE_PAL_PARITY_ERROR,       /* UINT16, parity error threshold */
    DRV_CUSTOM_AUD_TYPE_PAL_NUM_FRAMES,         /* UINT16, every number frames */
    DRV_CUSTOM_AUD_TYPE_PAL_HDEV,               /* BOOL, high deviation mode on/off */
    DRV_CUSTOM_AUD_TYPE_PAL_AM_MUTE,            /* UINT8, AM carrier sense mute mode on/off */
    DRV_CUSTOM_AUD_TYPE_PAL_AM_MUTE_HIGH,       /* UINT8, AM higher carrier sense mute threshold */
    DRV_CUSTOM_AUD_TYPE_PAL_AM_MUTE_LOW,        /* UINT8, AM lower carrier sense mute threshold */
    DRV_CUSTOM_AUD_TYPE_PAL_CARRIER_SHIFT,      /* BOOL, carrier shift mode on/off */
    DRV_CUSTOM_AUD_TYPE_PAL_FM_MUTE,            /* UINT8, FM carrier sense mute mode on/off */
    DRV_CUSTOM_AUD_TYPE_PAL_FM_MUTE_HIGH,       /* UINT8, FM higher carrier sense mute threshold */
    DRV_CUSTOM_AUD_TYPE_PAL_FM_MUTE_LOW,        /* UINT8, FM lower carrier sense mute threshold */
    DRV_CUSTOM_AUD_TYPE_PAL_FINE_VOLUME,        /* UINT8, fine tune volume */
    DRV_CUSTOM_AUD_TYPE_AM_FINE_VOLUME,         /* UINT8, fine tune volume */
    DRV_CUSTOM_AUD_TYPE_NICAM_FINE_VOLUME,      /* UINT8, fine tune volume */
    /* EU FACTORY MODE */
    DRV_CUSTOM_AUD_TYPE_FM_SATU_MUTE,           /* BOOL, fm saturation mute mode on/off */
    DRV_CUSTOM_AUD_TYPE_NON_EU,                 /* BOOL, non-EU mode on/off */
    /* BBE FACTORY MODE */
    DRV_CUSTOM_AUD_TYPE_BBE_PRESCALE_LEVEL,     /* INT8, prescale level */
    DRV_CUSTOM_AUD_TYPE_BBE_PROCESSS,           /* UINT8, BBE process */
    DRV_CUSTOM_AUD_TYPE_BBE_LO_CONTOUR,         /* UINT8, Lo contour */
    DRV_CUSTOM_AUD_TYPE_BBE_3D_GAIN,            /* UINT8, 3D gain */
    /* TEST TONE */
    DRV_CUSTOM_AUD_TYPE_TEST_TONE,              /* Test Tone Setting */
    /* CUTOFF FREQ */
    DRV_CUSTOM_AUD_TYPE_CUT_OFF_FREQ,           /* cut-off frequency */
    /* LOUDNESS */
    DRV_CUSTOM_AUD_TYPE_LOUDNESS,               /* loudness */
    /* SMART AUDIO */
    DRV_CUSTOM_AUD_TYPE_SMART_AUDIO,            /* Smart Audio */
    /* Volume Curve */
    DRV_CUSTOM_AUD_VOL_CURVE,                   /* Volume Curve */

    DRV_CUSTOM_AUD_TYPE_END

}   DRV_CUSTOM_AUD_TYPE_T;

#define DRV_CUSTOM_AUD_INP_FLAG_NONE     MAKE_BIT_MASK_32(0)
#define DRV_CUSTOM_AUD_INP_FLAG_ATV      MAKE_BIT_MASK_32(1)
#define DRV_CUSTOM_AUD_INP_FLAG_DTV      MAKE_BIT_MASK_32(2)
#define DRV_CUSTOM_AUD_INP_FLAG_YPBPR    MAKE_BIT_MASK_32(3)
#define DRV_CUSTOM_AUD_INP_FLAG_VGA      MAKE_BIT_MASK_32(4)
#define DRV_CUSTOM_AUD_INP_FLAG_HDMI     MAKE_BIT_MASK_32(5)
#define DRV_CUSTOM_AUD_INP_FLAG_DVI      MAKE_BIT_MASK_32(6)
#define DRV_CUSTOM_AUD_INP_FLAG_DVD      MAKE_BIT_MASK_32(7)

/* DRV_CUSTOM_AUD_TYPE_VOL_GAIN */
typedef struct _AUD_TYPE_VOL_GAIN_INFO_T
{
    UINT32        ui4_input_mask;
    INT16         i2_vol_gain;     /* in 0.5 db per unit */
} AUD_TYPE_VOL_GAIN_INFO_T;

/* DRV_CUSTOM_AUD_TYPE_AVL_GAIN */
typedef struct _AUD_TYPE_AVL_GAIN_INFO_T
{
    DRV_CUSTOM_AVL_ADJ_T       t_avl_gain;
    INT32         i4_gain;
} AUD_TYPE_AVL_GAIN_INFO_T;

/* DRV_CUSTOM_AUD_TYPE_SPECTRUM_INFO */
typedef struct _AUD_TYPE_SPECTRUM_INFO_T
{
    INT32         ai4_bar[15];     /* 15 Spectrum Bars */
} AUD_TYPE_SPECTRUM_INFO_T;

/* DRV_CUSTOM_AUD_TYPE_TEST_TONE */
typedef struct _AUD_TYPE_TEST_TONE
{
    BOOL        fgPlayTestTone;
    UINT8       u1DecId;
    UINT32      u4OutputChlSelect;
    UINT32      u4ToneFreq;
}AUD_TYPE_TEST_TONE;

/* DRV_CUSTOM_AUD_TYPE_SMART_AUDIO */
typedef struct _SMART_AUD_INFO_T
{
    UINT8	u1SmartAudSound1;
    UINT8   u1SmartAudSound2;
    UINT8   u1SmartAudSound3;
    UINT8	u1SmartAudSound4;
    UINT8	u1SmartAudSound5;
    UINT8	u1SmartAudSound6;
    UINT8	u1SmartAudSound7;
    UINT8	u1SmartAudBass;
    UINT8	u1SmartAudTreble;
    UINT8	u1SmartAudVSurr;
    UINT8	u1SmartAudBBE;
    UINT8	u1SmartAudFreq;
    UINT8   u1SmartAVL;
    UINT8   u1SmartAudLoudness;
    UINT8   u1SmartRev5;
    UINT8   u1SmartRev6;
}SMART_AUD_INFO_T;

typedef struct _SMART_AUD_GROUP_INFO_T
{
    SMART_AUD_INFO_T aSmartAud[6];
}SMART_AUD_GROUP_INFO_T;

/*
    Tuner driver custom types.
*/
typedef struct _TUNER_TYPE_DIAG_CONTROL_T
{
    UINT16		  u2PollPeriodMs;		/* Polling period in unit of ms*/
    UCHAR		  ucCntOfDiagStruc;		/* How many diag strucs to be displayed on OSD*/
} TUNER_TYPE_DIAG_CONTROL_T;

typedef struct _TUNER_TYPE_DIAG_CONTENT_T
{
	CHAR		  cDisplayName[16];		/* A zero-end string indicating diag name*/
	CHAR		  cDisplayValue[16];	/* A zero-end string indicating diag value*/
} TUNER_TYPE_DIAG_CONTENT_T;

typedef struct _MISC_TYPE_TUNER_SIF_T
{
    UINT32      ui4_addr;		/*device address*/
    UINT8		ui1_offset_len;	/*lenth of ui4_offset, from 0 up to 3*/
    UINT32      ui4_offset;		/*sub-address (or called offset)*/
    UINT8       ui1_data;
} MISC_TYPE_TUNER_SIF_T;

typedef struct _MISC_TYPE_SIF_ADVANCED_T
{
    UINT8       ui1_channel;
    UINT16      ui2_clkdiv;
    UINT32      ui4_addr;		/*device address*/
    UINT8       ui1_offset_len;	/*lenth of ui4_offset, from 0 up to 3*/
    UINT32      ui4_offset;		/*sub-address (or called offset)*/
    UINT8       ui1_data_len;   /*Length if data*/
    UINT8*      pui1_data;
} MISC_TYPE_SIF_ADVANCED_T;

typedef enum
{
	DRV_CUSTOM_TUNER_TYPE_BEGIN = 0,

	DRV_CUSTOM_TUNER_TYPE_ANALOG,
	DRV_CUSTOM_TUNER_TYPE_DIGITAL,

	DRV_CUSTOM_TUNER_TYPE_END

} DRV_CUSTOM_TUNER_TYPE_T;

typedef enum
{
    DRV_CUSTOM_TUNER_SET_TYPE_BEGIN    =    0,

    DRV_CUSTOM_TUNER_SET_TYPE_DIAG_OPEN,
    DRV_CUSTOM_TUNER_SET_TYPE_DIAG_CLOSE,

    DRV_CUSTOM_TUNER_SET_TYPE_END

}   DRV_CUSTOM_TUNER_SET_TYPE_T;

typedef enum
{
    DRV_CUSTOM_TUNER_GET_TYPE_BEGIN    =    0,

    DRV_CUSTOM_TUNER_GET_TYPE_DIAG_CONTROL,
    DRV_CUSTOM_TUNER_GET_TYPE_DIAG_CONTENT,

    DRV_CUSTOM_TUNER_GET_TYPE_END

}   DRV_CUSTOM_TUNER_GET_TYPE_T;

/*
    Misc driver custom types.
*/
typedef enum
{
    DRV_CUSTOM_MISC_TYPE_BEGIN    =    0,

    DRV_CUSTOM_MISC_TYPE_TUNER_ID,               /* Tuner ID */
    DRV_CUSTOM_MISC_TYPE_PANEL_ID,               /* Panel ID */
    DRV_CUSTOM_MISC_TYPE_PANEL_NAME,             /* Panel Name */
    DRV_CUSTOM_MISC_TYPE_VALID_HDCP,             /* Is HDCP valid? */

    DRV_CUSTOM_MISC_TYPE_SET_ERROR_CODES,        /* Set current Error Codes */
    DRV_CUSTOM_MISC_TYPE_START_LED_BLINKING,     /* Request to start LED Blinking according to the error codes. */

    DRV_CUSTOM_MISC_TYPE_TUNER_SIF,
    DRV_CUSTOM_MISC_TYPE_HDMI_EDID,
    #ifdef CC_SCPOS_FORCE_UNMUTE
    DTV_CUSTOM_MISC_TYPE_FORCE_MUTE_OFF,
    #endif
    DRV_CUSTOM_MISC_TYPE_SIF_ADVANCED,
    DRV_CUSTOM_MISC_TYPE_LIGHT_GUIDE,
    DRV_CUSTOM_MISC_TYPE_ALIVE_CHECK_CTRL,

    DRV_CUSTOM_MISC_TYPE_END

}   DRV_CUSTOM_MISC_TYPE_T;

typedef struct _MISC_TYPE_HDMI_EDID_T
{
    UINT8 ui1_hdmi_comp_id;                      /* HDMI port comp. ID. */
    UINT8 aui1_edid_buf[256];                    /* Buffer to store EDID data. */
}   MISC_TYPE_HDMI_EDID_T;

typedef struct _EDID_T
{
    UINT32      u1EDID_ID;            /*device address*/
    UINT8             u1SubAddr;      /*lenth of ui4_offset, from 0 up to 3*/
     UINT8       ui1_data;
} EDID_T;


#define MISC_TYPE_ALIVE_CHECK_CTRL_SDM_MODE         MAKE_BIT_MASK_32(3)
#define MISC_TYPE_ALIVE_CHECK_CTRL_FACTORY_MODE     MAKE_BIT_MASK_32(5)
#define MISC_TYPE_ALIVE_CHECK_CTRL_CSM_MODE         MAKE_BIT_MASK_32(6)
#define MISC_TYPE_ALIVE_CHECK_CTRL_SAM_MODE         MAKE_BIT_MASK_32(7)

#define MISC_TYPE_ALIVE_CHECK_CTRL_MASK_ALL             \
    ( MISC_TYPE_ALIVE_CHECK_CTRL_SDM_MODE |             \
      MISC_TYPE_ALIVE_CHECK_CTRL_FACTORY_MODE |         \
      MISC_TYPE_ALIVE_CHECK_CTRL_CSM_MODE |             \
      MISC_TYPE_ALIVE_CHECK_CTRL_SAM_MODE )

/*-----------------------------------------------------------------------------
                    function declarations
-----------------------------------------------------------------------------*/
/* Video */
extern INT32 d_custom_vid_set (
    DRV_CUSTOM_VID_TYPE_T	e_vid_type,
    VOID*					pv_set_info,
    SIZE_T                  z_size,
    BOOL					b_store
);

extern INT32 d_custom_vid_get (
    DRV_CUSTOM_VID_TYPE_T	e_vid_type,
	DRV_CUSTOM_VID_INP_T	e_vid_inp,
    VOID*					pv_get_info,
    SIZE_T*					pz_size
);

extern INT32 d_custom_vid_get_gamma(
	GAMMA_INFO_T * pu1GammaTbl
);

extern INT32 d_custom_vid_get_min_max (
    DRV_CUSTOM_VID_TYPE_T	e_vid_type,
	DRV_CUSTOM_VID_INP_T	e_vid_inp,
    UINT16*					pui2_min,
    UINT16*					pui2_max
);

/* Audio */
extern INT32 d_custom_aud_set (
    DRV_CUSTOM_AUD_TYPE_T     e_aud_type,
    VOID*                     pv_set_info,
    SIZE_T                    z_size,
    BOOL                      b_store
);

extern INT32 d_custom_aud_get (
    DRV_CUSTOM_AUD_TYPE_T     e_aud_type,
    VOID*                     pv_get_info,
    SIZE_T*                   pz_size
);

extern INT32 d_custom_aud_get_min_max (
    DRV_CUSTOM_AUD_TYPE_T     e_aud_type,
    INT16*                    pui2_min,
    INT16*                    pui2_max
);

/* Tuner */
extern INT32 d_custom_tuner_set (
	DRV_CUSTOM_TUNER_TYPE_T			e_tuner_type,
    DRV_CUSTOM_TUNER_SET_TYPE_T   	e_tuner_set_type,
    VOID*                     		pv_set_info,
    SIZE_T                   		z_size
);

extern INT32 d_custom_tuner_get (
	DRV_CUSTOM_TUNER_TYPE_T			e_tuner_type,
    DRV_CUSTOM_TUNER_GET_TYPE_T   	e_tuner_get_type,
    VOID*                     		pv_get_info,
    SIZE_T*                   		pz_size
);

/* Misc */
extern INT32 d_custom_misc_set (
    DRV_CUSTOM_MISC_TYPE_T    e_misc_type,
    VOID*                     pv_set_info,
    SIZE_T                    z_size,
    BOOL                      b_store
);

extern INT32 d_custom_misc_get (
    DRV_CUSTOM_MISC_TYPE_T    e_misc_type,
    VOID*                     pv_get_info,
    SIZE_T*                   pz_size
);

extern INT32 d_custom_misc_get_min_max (
    DRV_CUSTOM_MISC_TYPE_T    e_misc_type,
    UINT16*                   pui2_min,
    UINT16*                   pui2_max
);

extern INT32 d_custom_misc_get_loader_info(
    UINT8 *szVersion,
    UINT32 *pu4VersionSize,
    UINT32 *pu4CheckSum
);

extern INT32 d_custom_misc_set_loader_upgrade(
    BOOL    b_upgrade_enable
);

extern INT32 d_custom_get_custom_mode(
    void
);

extern INT32 d_custom_setquery_image_bank(
    UINT32 *pu4Bank
);

extern INT32 d_custom_setquery_8032uP_bank(
    UINT32 *pu4Bank
);

extern INT32 d_custom_irrx_system_code_filter(
	BOOL fgEnable, 
	BOOL fgPositiveFilter, 
	UINT32 u4Num, 
	UINT32 au4FilterArray[]);

#endif /* _D_5381_CUST_H_ */
