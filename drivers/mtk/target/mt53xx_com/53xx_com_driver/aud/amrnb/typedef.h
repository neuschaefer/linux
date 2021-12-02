/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2008
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/
/*******************************************************************************
 *
 * Filename:
 * ---------
 *   typedef.h
 *
 * Project:
 * --------
 *   DUMA
 *
 * Description:
 * ------------
 *   AMR codec implementation
 *
 * Author:
 * -------
 *   Stan Huang
 *
 *------------------------------------------------------------------------------
 * $Revision: #2 $ 1.0.0
 * $Modtime:$
 * $Log:$
 *
 * 09 24 2010 scholar.chang
 * [WCPSP00000519] Remove AMR codec SWIP assert function
 * .
 *
 *
 *******************************************************************************/


#ifndef AMR_TYPEDEF_DOT_H
#define AMR_TYPEDEF_DOT_H

#include "amr_exp.h"

#if !defined(__CC_ARM)
// avoid nucleaus redefinition warning
#define __CC_ARM	0
#endif

#if !defined(__TARGET_FEATURE_DSPMUL)
// avoid nucleaus redefinition warning
#define __TARGET_FEATURE_DSPMUL 0
#endif

#ifndef NULL
#define NULL    0
#endif


#if !__CC_ARM
#define __align(x)
#endif


#define AMR_OPT_VER  1 // 2    1
#define MMS_IO

/*
	AMR_OPT_VER = 1 High precision Version (Bit-not-exact but more precise)
	AMR_OPT_VER = 2 Bit-exact version
*/



/*
********************************************************************************
*                         DEFINITION OF CONSTANTS
********************************************************************************
*/

#ifdef CC_MTK_PLATFORM
//MTK BSP
#include "x_typedef.h"
#include "x_assert.h"

//#define INLINE      extern // dtv mtk platform does not support inline function defined in header

#define assert ASSERT

typedef UINT32    UWord32;
typedef INT32     Word32;
typedef INT32     Flag;
typedef UINT16    UWord16;
typedef INT16     Word16;
typedef UINT8     UWord8;
typedef UINT8     UChar;
typedef INT8      Char;
typedef INT8      Word8;
typedef INT64     Word64;
typedef UINT64    UWord64;

#else

/*
 ********* define char type
 */
#define false 0
#define true 1
typedef int           Bool;
typedef char          Char;

typedef signed char  Word8;
typedef unsigned char  UWord8;

typedef signed short  Word16;
typedef unsigned short  UWord16;

typedef signed int    Word32;
typedef unsigned int  UWord32;

typedef int           Flag;


#if __CC_ARM
typedef long long Word64;
#else
typedef __int64   Word64;
#endif


#endif      // CC_MTK_PLATFORM

#define aligned_sizeof(_type) ((sizeof(_type)+3)&(~3))
#define is_aligned(_ptr) (0==((UWord32)(_ptr)&3))

#define lo64(a) (((UWord32*) &a)[0])    /* low 32 bits of a long long */
#define hi64(a) (((Word32*) &a)[1])        /* high 32 bits of a long long */

#ifndef ARMDSP_WORD32_DEFINED
#define ARMDSP_WORD32_DEFINED
#define ARMDSP_WORD32_MAX ((Word32)0x7FFFFFFF)
#define ARMDSP_WORD32_MIN ((Word32)0x80000000)
#define ARMDSP_WORD16_MAX ((Word16)0x7FFF)
#define ARMDSP_WORD16_MIN ((Word16)0x8000)
#endif


typedef union {
    UWord32 armdsp_flags_word;
    struct {
#ifdef __BIG_ENDIAN
        Flag armdsp_n:1, armdsp_z:1, armdsp_c:1, armdsp_v:1, armdsp_q:1, armdsp_unused:27;
#else
        Flag armdsp_unused:27, armdsp_q:1, armdsp_v:1, armdsp_c:1, armdsp_z:1, armdsp_n:1;
#endif /* __BIG_ENDIAN */
    } armdsp_bitfields;
} armdsp_flagdata_union;

extern armdsp_flagdata_union armdsp_flagdata_AMR;

/*
********************************************************************************
*                         DEFINITION OF ENUM
********************************************************************************
*/
enum Mode { MR475 = 0,
            MR515,
            MR59,
            MR67,
            MR74,
            MR795,
            MR102,
            MR122,
	         MRDTX,
            N_MODES     /* number of (SPC) modes */
          };
enum RXFrameType { RX_SPEECH_GOOD = 0,
                   RX_SPEECH_DEGRADED,
                   RX_ONSET,
                   RX_SPEECH_BAD,
                   RX_SID_FIRST,
                   RX_SID_UPDATE,
                   RX_SID_BAD,
                   RX_NO_DATA,
                   RX_N_FRAMETYPES     /* number of frame types */
};

enum TXFrameType { TX_SPEECH_GOOD = 0,
                   TX_SID_FIRST,
                   TX_SID_UPDATE,
                   TX_NO_DATA,
                   TX_SPEECH_DEGRADED,
                   TX_SPEECH_BAD,
                   TX_SID_BAD,
                   TX_ONSET,
                   TX_N_FRAMETYPES     /* number of frame types */
};


/* Channel decoded frame type */
enum CHDECFrameType { CHDEC_SID_FIRST = 0,
                      CHDEC_SID_FIRST_INCOMPLETE,
                      CHDEC_SID_UPDATE_INCOMPLETE,
                      CHDEC_SID_UPDATE,
                      CHDEC_SPEECH,
                      CHDEC_SPEECH_ONSET,
                      CHDEC_ESCAPE_MARKER,
                      CHDEC_ESCAPE_DATA,
                      CHDEC_NO_DATA };

/* Channel decoded frame quality */
enum CHDECFrameQuality { CHDEC_GOOD = 0,
                         CHDEC_PROBABLY_DEGRADED,
                         CHDEC_PROBABLY_BAD,
                         CHDEC_BAD };


/*--------------------------------------------------------------------------*
 *       ENCODER constant parameters                                        *
 *--------------------------------------------------------------------------*/
#define L_TOTAL      320       /* Total size of speech buffer.             */
#define L_WINDOW     240       /* Window size in LP analysis               */
#define L_FRAME      160       /* Frame size                               */
#define L_FRAME_BY2  80        /* Frame size divided by 2                  */
#define L_SUBFR      40        /* Subframe size                            */
#define L_CODE       40        /* codevector length                        */
#define NB_TRACK     5         /* number of tracks                         */
#define STEP         5         /* codebook step size                       */
#define NB_TRACK_MR102  4      /* number of tracks mode mr102              */
#define STEP_MR102      4      /* codebook step size mode mr102            */
#define M            10        /* Order of LP filter                       */
#define MP1          (M+1)     /* Order of LP filter + 1                   */
#define LSF_GAP      205       /* Minimum distance between LSF after quan-
                                  tization; 50 Hz = 205                    */
#define LSP_PRED_FAC_MR122 21299 /* MR122 LSP prediction factor (0.65 Q15) */
#define AZ_SIZE       (4*M+4)  /* Size of array of LP filters in 4 subfr.s */
#define PIT_MIN_MR122 18       /* Minimum pitch lag (MR122 mode)           */
#define PIT_MIN       20       /* Minimum pitch lag (all other modes)      */
#define PIT_MAX       143      /* Maximum pitch lag                        */
#define L_INTERPOL    (10+1)   /* Length of filter for interpolation       */
#define L_INTER_SRCH  4        /* Length of filter for CL LTP search
                                  interpolation                            */

#define MU       26214         /* Factor for tilt compensation filter 0.8  */
#define AGC_FAC  29491         /* Factor for automatic gain control 0.9    */

#define L_NEXT       40        /* Overhead in LP analysis                  */
#define SHARPMAX  13017        /* Maximum value of pitch sharpening        */
#define SHARPMIN  0            /* Minimum value of pitch sharpening        */


#define MAX_PRM_SIZE    57     /* max. num. of params                      */
#define MAX_SERIAL_SIZE 244    /* max. num. of serial bits                 */

#define GP_CLIP   15565        /* Pitch gain clipping = 0.95               */
#define N_FRAME   7            /* old pitch gains in average calculation   */

#define EHF_MASK 0x0008        /* encoder homing frame pattern             */


#define LTPG_MEM_SIZE 5 /* number of stored past LTP coding gains + 1 */


#define DTX_HIST_SIZE 8
#define DTX_ELAPSED_FRAMES_THRESH (24 + 7 -1)
#define DTX_HANG_CONST 7             /* yields eight frames of SP HANGOVER  */



#define		YES		1
#define		NO		0
#define		ON		1
#define		OFF		0
#ifndef CC_MTK_PLATFORM
#define		TRUE		1
#define		FALSE		0
#endif

#define         FRM_LEN                 80
#define         DELAY                   24
#define         FFT_LEN                 128

#define         NUM_CHAN                16
#define         LO_CHAN                 0
#define         HI_CHAN                 15

#define         UPDATE_THLD             35
#define         HYSTER_CNT_THLD         6
#define         UPDATE_CNT_THLD         50

#define		SHIFT_STATE_0		0		/* channel energy scaled as 22,9 */
#define		SHIFT_STATE_1		1		/* channel energy scaled as 27,4 */

#define		NOISE_FLOOR_CHAN_0	512		/* 1.0    scaled as 22,9 */
#define		MIN_CHAN_ENRG_0		32		/* 0.0625 scaled as 22,9 */
#define		MIN_NOISE_ENRG_0	32		/* 0.0625 scaled as 22,9 */
#define		INE_NOISE_0		8192		/* 16.0   scaled as 22,9 */
#define		FRACTIONAL_BITS_0	9		/* used as input to fn10Log10() */

#define		NOISE_FLOOR_CHAN_1	16		/* 1.0    scaled as 27,4 */
#define		MIN_CHAN_ENRG_1		1		/* 0.0625 scaled as 27,4 */
#define		MIN_NOISE_ENRG_1	1		/* 0.0625 scaled as 27,4 */
#define		INE_NOISE_1		256		/* 16.0   scaled as 27,4 */
#define		FRACTIONAL_BITS_1	4		/* used as input to fn10Log10() */

#define		STATE_1_TO_0_SHIFT_R	(FRACTIONAL_BITS_1-FRACTIONAL_BITS_0)	/* state correction factor */
#define		STATE_0_TO_1_SHIFT_R	(FRACTIONAL_BITS_0-FRACTIONAL_BITS_1)	/* state correction factor */

#define         HIGH_ALPHA              29491		/* 0.9 scaled as 0,15 */
#define         LOW_ALPHA               22938		/* 0.7 scaled as 0,15 */
#define         ALPHA_RANGE             (HIGH_ALPHA - LOW_ALPHA)
#define         DEV_THLD                7168		/* 28.0 scaled as 7,8 */

#define         PRE_EMP_FAC             (-26214)	/* -0.8 scaled as 0,15 */

#define         CEE_SM_FAC              18022		/* 0.55 scaled as 0,15 */
#define         ONE_MINUS_CEE_SM_FAC    14746		/* 0.45 scaled as 0,15 */

#define         CNE_SM_FAC              3277		/* 0.1 scaled as 0,15 */
#define         ONE_MINUS_CNE_SM_FAC    29491		/* 0.9 scaled as 0,15 */

#define         FFT_HEADROOM            2


#define FRAME_LEN 160    /* Length (samples) of the input frame          */
#define COMPLEN 9        /* Number of sub-bands used by VAD              */
#define INV_COMPLEN 3641 /* 1.0/COMPLEN*2^15                             */
#define LOOKAHEAD 40     /* length of the lookahead used by speech coder */

#define UNITY 512        /* Scaling used with SNR calculation            */
#define UNIRSHFT 6       /* = log2(MAX_16/UNITY)                         */

#define TONE_THR (Word16)(0.65*MAX_16) /* Threshold for tone detection   */

/* Constants for background spectrum update */
#define ALPHA_UP1   (Word16)((1.0 - 0.95)*MAX_16)  /* Normal update, upwards:   */
#define ALPHA_DOWN1 (Word16)((1.0 - 0.936)*MAX_16) /* Normal update, downwards  */
#define ALPHA_UP2   (Word16)((1.0 - 0.985)*MAX_16) /* Forced update, upwards    */
#define ALPHA_DOWN2 (Word16)((1.0 - 0.943)*MAX_16) /* Forced update, downwards  */
#define ALPHA3      (Word16)((1.0 - 0.95)*MAX_16)  /* Update downwards          */
#define ALPHA4      (Word16)((1.0 - 0.9)*MAX_16)   /* For stationary estimation */
#define ALPHA5      (Word16)((1.0 - 0.5)*MAX_16)   /* For stationary estimation */

/* Constants for VAD threshold */
#define VAD_THR_HIGH 1260 /* Highest threshold                 */
#define VAD_THR_LOW  720  /* Lowest threshold                  */
#define VAD_P1 0          /* Noise level for highest threshold */
#define VAD_P2 6300       /* Noise level for lowest threshold  */
#define VAD_SLOPE (Word16)(MAX_16*(float)(VAD_THR_LOW-VAD_THR_HIGH)/(float)(VAD_P2-VAD_P1))

/* Parameters for background spectrum recovery function */
#define STAT_COUNT 20         /* threshold of stationary detection counter         */
#define STAT_COUNT_BY_2 10    /* threshold of stationary detection counter         */
#define CAD_MIN_STAT_COUNT 5  /* threshold of stationary detection counter         */

#define STAT_THR_LEVEL 184    /* Threshold level for stationarity detection        */
#define STAT_THR 1000         /* Threshold for stationarity detection              */

/* Limits for background noise estimate */
#define NOISE_MIN 40          /* minimum */
#define NOISE_MAX 16000       /* maximum */
#define NOISE_INIT 150        /* initial */

/* Constants for VAD hangover addition */
#define HANG_NOISE_THR 100
#define BURST_LEN_HIGH_NOISE 4
#define HANG_LEN_HIGH_NOISE 7
#define BURST_LEN_LOW_NOISE 5
#define HANG_LEN_LOW_NOISE 4

/* Thresholds for signal power */
#define VAD_POW_LOW (Word32)15000     /* If input power is lower,                    */
                                      /*     VAD is set to 0                         */
#define POW_PITCH_THR (Word32)343040  /* If input power is lower, pitch              */
                                      /*     detection is ignored                    */

#define POW_COMPLEX_THR (Word32)15000 /* If input power is lower, complex            */
                                      /* flags  value for previous frame  is un-set  */


/* Constants for the filter bank */
#define LEVEL_SHIFT 0      /* scaling                                  */
#define COEFF3   13363     /* coefficient for the 3rd order filter     */
#define COEFF5_1 21955     /* 1st coefficient the for 5th order filter */
#define COEFF5_2 6390      /* 2nd coefficient the for 5th order filter */

/* Constants for pitch detection */
#define LTHRESH 4
#define NTHRESH 4

/* Constants for complex signal VAD  */
#define CVAD_THRESH_ADAPT_HIGH  (Word16)(0.6 * MAX_16) /* threshold for adapt stopping high    */
#define CVAD_THRESH_ADAPT_LOW  (Word16)(0.5 * MAX_16)  /* threshold for adapt stopping low     */
#define CVAD_THRESH_IN_NOISE  (Word16)(0.65 * MAX_16)  /* threshold going into speech on
                                                          a short term basis                   */

#define CVAD_THRESH_HANG  (Word16)(0.70 * MAX_16)      /* threshold                            */
#define CVAD_HANG_LIMIT  (Word16)(100)                 /* 2 second estimation time             */
#define CVAD_HANG_LENGTH  (Word16)(250)                /* 5 second hangover                    */

#define CVAD_LOWPOW_RESET (Word16) (0.40 * MAX_16)     /* init in low power segment            */
#define CVAD_MIN_CORR (Word16) (0.40 * MAX_16)         /* lowest adaptation value              */

#define CVAD_BURST 20                                  /* speech burst length for speech reset */
#define CVAD_ADAPT_SLOW (Word16)(( 1.0 - 0.98) * MAX_16)        /* threshold for slow adaption */
#define CVAD_ADAPT_FAST (Word16)((1.0 - 0.92) * MAX_16)         /* threshold for fast adaption */
#define CVAD_ADAPT_REALLY_FAST (Word16)((1.0 - 0.80) * MAX_16)  /* threshold for really fast
                                                                   adaption                    */
#define L_H 22  /* size of truncated impulse response of A(z/g1)/A(z/g2) */

/*
********************************************************************************
*                         DEFINITION OF ENCODER DATA TYPES
********************************************************************************
*/


/*-------------------------------------------------------------------------------*/
typedef struct {
   Word16 past_qua_en[4];         /* normal MA predictor memory,         Q10 */
                                  /* (contains 20*log10(qua_err))            */
   Word16 past_qua_en_MR122[4];   /* MA predictor memory for MR122 mode, Q10 */
                                  /* (contains log2(qua_err))                */
} gc_predState;

typedef struct {
    Word16 onset;                   /* onset state,                   Q0  */
    Word16 prev_alpha;              /* previous adaptor output,       Q15 */
    Word16 prev_gc;                 /* previous code gain,            Q1  */

    Word16 ltpg_mem[LTPG_MEM_SIZE]; /* LTP coding gain history,       Q13 */
                                    /* (ltpg_mem[0] not used for history) */
} GainAdaptState;


typedef struct {
    Word16 sf0_exp_gcode0;
    Word16 sf0_frac_gcode0;
    Word16 sf0_exp_target_en;
    Word16 sf0_frac_target_en;
    Word16 sf0_exp_coeff[5];
    Word16 sf0_frac_coeff[5];
    Word16 *gain_idx_ptr;

    gc_predState     gc_predSt;
    gc_predState     gc_predUnqSt;
    GainAdaptState   adaptSt;
} gainQuantState;


/*-------------------------------------------------------------------------------*/
/* state variable */
typedef struct {
   Word16 old_T0_med;
   Word16 ada_w;
   Word16 wght_flg;
} pitchOLWghtState;


/*-------------------------------------------------------------------------------*/
/* state variable */
typedef struct {

   /* counters */
   Word16 count;

   /* gain history Q11 */
   Word16 gp[N_FRAME];

} tonStabState;
/*-------------------------------------------------------------------------------*/


typedef struct {

   Word16 bckr_est[COMPLEN];    /* background noise estimate                */
   Word16 ave_level[COMPLEN];   /* averaged input components for stationary */
                                /*    estimation                            */
   Word16 old_level[COMPLEN];   /* input levels of the previous frame       */
   Word16 sub_level[COMPLEN];   /* input levels calculated at the end of
                                      a frame (lookahead)                   */
   Word16 a_data5[3][2];        /* memory for the filter bank               */
   Word16 a_data3[5];           /* memory for the filter bank               */

   Word16 burst_count;          /* counts length of a speech burst          */
   Word16 hang_count;           /* hangover counter                         */
   Word16 stat_count;           /* stationary counter                       */

   /* Note that each of the following three variables (vadreg, pitch and tone)
      holds 15 flags. Each flag reserves 1 bit of the variable. The newest
      flag is in the bit 15 (assuming that LSB is bit 1 and MSB is bit 16). */
   Word16 vadreg;               /* flags for intermediate VAD decisions     */
   Word16 pitch;                /* flags for pitch detection                */
   Word16 tone;                 /* flags for tone detection                 */
   Word16 complex_high;         /* flags for complex detection              */
   Word16 complex_low;          /* flags for complex detection              */

   Word16 oldlag_count, oldlag; /* variables for pitch detection            */

   Word16 complex_hang_count;   /* complex hangover counter, used by VAD    */
   Word16 complex_hang_timer;   /* hangover initiator, used by CAD          */

   Word16 best_corr_hp;         /* FIP filtered value Q15                   */

   Word16 speech_vad_decision;  /* final decision                           */
   Word16 complex_warning;      /* complex background warning               */

   Word16 sp_burst_count;       /* counts length of a speech burst incl
                                   HO addition                              */
   Word16 corr_hp_fast;         /* filtered value                           */
} vadState1;


#define vadState vadState1


/*-------------------------------------------------------------------------------*/
typedef struct {
   Word16 lsp_hist[M * DTX_HIST_SIZE];
   Word16 log_en_hist[DTX_HIST_SIZE];
   Word16 hist_ptr;
   Word16 log_en_index;
   Word16 init_lsf_vq_index;
   Word16 lsp_index[3];

   /* DTX handler stuff */
   Word16 dtxHangoverCount;
   Word16 decAnaElapsedCount;
} dtx_encState;
/*-------------------------------------------------------------------------------*/

typedef struct {
    Word16 sid_update_rate;  /* Send SID Update every sid_update_rate frame */
    Word16 sid_update_counter; /* Number of frames since last SID          */
    Word16 sid_handover_debt;  /* Number of extra SID_UPD frames to schedule*/
    enum TXFrameType prev_ft;
} sid_syncState;



typedef struct {
  Word32 y1;
  Word32 y2;
  Word16 x0;
  Word16 x1;
} PreState;



/*==========================================================================*/


typedef struct
{
   //-- For encode and decode   // 116 + 96 + 320 = 532 bytes
   Word16 parm[MAX_PRM_SIZE + 1];  		/* Synthesis parameters     58        */
   Word16 Az_dec[AZ_SIZE+4];         	/* Decoded Az for post-filter          */
   Word16 syn[L_FRAME];        			/* Buffer for synthesis speech           */

	// shabuf for Encoder, Decoder
   union
	{
		// Decoder Buffer
      struct // stD 20+80+80+80+160 = 420 bytes
      {
		   Word16 prev_lsf[M];       	 	/* LSFs */
		   Word16 code[L_SUBFR];      	/* Algebraic codevector */
         union // u0D 80 bytes
			{
			   Word16 lsp_new[M];
			   Word16 lsf_i[M];
			   Word16 exc_enhanced[L_SUBFR];
         } u0D;

         union // u1D 80 bytes
			{
			   Word16 lsp_mid[M];
			   Word16 excp[L_SUBFR];
         } u1D;

		   Word32 f1[6];
			Word32 f2[6];

         union // u2D 160 bytes
			{
            struct { // s0D
				   Word16 lsf1_r[M];
					Word16 lsf2_r[M];
				   Word16 lsf1_q[M];
					Word16 lsf2_q[M];
					Word16 lsp[M];

            } s0D;

            struct { // s1D
				   Word16 pos[4];
				   Word16 linear_signs[NB_TRACK_MR102];
				   Word16 linear_codewords[8];
					Word16 tmp[M];
            } s1D;

            struct { // s2D
				   Word16 inno_sav[L_SUBFR];
				   Word16 ps_poss[L_SUBFR];
            } s2D;

            struct { // s3D
				   Word32 tmp_w[M];				/* Syn_filt */
				   Word16 Ap3[MP1+1];			/* bandwidth expanded LP parameters */
					Word16 Ap4[MP1+1];
				   Word16 h[L_H];
            } s3D;

         } u2D;

      } stD;

		// DTX_DEC Buffer
      struct // stDtx_D  288 + 48 = 336
      {
		   Word32 L_lsf[M];
		   Word16 lsp_int[M];
   		Word16 acoeff[M + 1 +1];
		   Word16 refl[M];
		   Word16 ex[L_SUBFR];
		   Word16 lsf_int[M];
		   Word16 lsf_int_variab[M];
		   Word16 lsp_int_variab[M];
		   Word16 acoeff_variab[M + 1 +1];
		   Word16 lsf[M];

		   Word32 f1[6];
			Word32 f2[6];
         union // u0D  48 bytes
			{
			   Word32 tmp_w[M];
			   Word16 lsf1_q[M];
            struct {
				   Word16 aState[M];
				   Word16 bState[M];
            } s1D;
         } u0D;
      }stDtx_D;

      // Encoder Buffer
      struct // stE
      {
      	/* 1052 + 80 = 1132 bytes*/
	   	/* LPC coefficients */
	   	Word16 A_t[(MP1+1) * 4];      		/* A(z) unquantized for the 4 subframes */
   		Word16 Aq_t[(MP1+1) * 4];     		/* A(z)   quantized for the 4 subframes */

			/* Other vectors */
		   Word16 xn[L_SUBFR];         	/* Target vector for pitch search       */
		   Word16 xn2[L_SUBFR];        	/* Target vector for codebook search    */
		   Word16 code[L_SUBFR];       	/* Fixed codebook excitation            */
		   Word16 y1[L_SUBFR];         	/* Filtered adaptive excitation         */
	   	Word16 y2[L_SUBFR];         	/* Filtered fixed codebook excitation   */
		   Word16 res[L_SUBFR];        	/* Short term (LPC) prediction residual */
		   Word16 res2[L_SUBFR];       	/* Long term (LTP) prediction residual  */

			/* Vector and scalars needed for the MR475 */
		   Word16 xn_sf0[L_SUBFR];     	/* Target vector for pitch search       */
		   Word16 code_sf0[L_SUBFR];   	/* Fixed codebook excitation            */
		   Word16 h1_sf0[L_SUBFR];     	/* The impulse response of sf0          */
		   Word16 mem_syn_save[M];     	/* Filter memory                        */
		   Word16 mem_w0_save[M];      	/* Filter memory                        */
		   Word16 mem_err_save[M];     	/* Filter memory                        */

			union // u1E  80 bytes
			{
				/* LPC coefficients */
			   Word16 lsp_new[M];
			   /* Other vectors */
			   Word16 gCoeff[6];           	/* Correlations between xn, y1, & y2:   */
				/* Vector and scalars needed for the MR475 */
			   Word16 y2_sf0[L_SUBFR];     	/* Filtered codebook innovation         */
         } u0E;

         union // u2E 3824 bytes
			{
            struct { // s0E  338 bytes
				   Word16 level[COMPLEN];			/* vad1() */
					Word16 tmp_buf[FRAME_LEN];		/* filter_bank() */
            } s0E;

            struct { // s1E 612 bytes
					Word32 r[MP1];						/* lpc() */
				   Word16 y_tmp[L_WINDOW];
					Word32 AD[M + 1];					/* Levinson() */
					Word32 An[M + 1];
            } s1E;

            struct { // s2E 284 bytes
					Word16 lsp_new_q[M];    		/* LSPs at 4th subframe           */
				   Word16 lsp_mid[M];     			/* LSPs at 2nd subframe           */
					Word16 lsp_mid_q[M];				/* lsp() */

				   Word16 f1[M / 2 + 1];			/* Az_lsp() */
					Word16 f2[M / 2 + 1];			/* Az_lsp() */

					Word16 lsp[M];						/* Int_lpc_1and3_2(), Int_lpc_1and3()
																Int_lpc_1to3_2   , Int_lpc_1to3() */

				   Word32 f11[M / 2 + 1];			/* Az_lsp() */
					Word32 f22[M / 2 + 1];			/* Az_lsp() */

					Word16 wf1[M]; 					/* Q_plsf_5() , Q_plsf_3() */
					Word16 wf2[M];
					Word16 lsf_p[M];
					Word16 lsf_r1[M];
					Word16 lsf_r2[M];
				   Word16 lsf1_q[M];
					Word16 lsf2_q[M];
					Word16 lsf1[M];
					Word16 lsf2[M];

            } s2E;

            struct { // s3E  100 bytes
				   Word32 L_lsp[M];					/* dtx_enc() */
				   Word16 lsf[M];
				   Word16 lsp[M];
				   Word16 lsp_q[M];
            } s3E;

            struct { // s4E  64 bytes
				   Word16 Ap1[MP1+1];				/* A(z) with spectral expansion         */
															/* pre_big() , subframePreProc() */
				   Word32 tmp_w[M];					/* Syn_filt() */
            } s4E;

            struct { // s5E 1184 bytes
					Word32 corr[PIT_MAX+1];			/* Pitch_ol() , Pitch_ol_wgh() */
				   Word16 scaled_signal[L_FRAME + PIT_MAX+1];
            } s5E;

			   Word16 corr_v[40];					/* Total length = t0_max-t0_min+1+2*L_INTER_SRCH */
															/* Pitch_fr() */

            struct { // s6E  3688 bytes
				   Word32 y32[L_CODE];				/* cor_h_x()   */
				   Word16 h2[L_CODE];				/* cor_h() */

				   Word16 codvec[4];					/* code_2i40_9bits() , code_2i40_11bits() */
				   Word16 dn[L_CODE];				/* code_3i40_14bits , code_4i40_17bits() */
					Word16 dn2[L_CODE];
					Word16 dn_sign[L_CODE];
				   Word16 rr[L_CODE][L_CODE];
            } s6E;

            struct { // s7E	 3824 bytes					/* code_8i40_31bits() */
				   Word32 y32[L_CODE];						/* cor_h_x2()   */
				   Word16 en[L_CODE];                  /* set_sign12k2() correlation vector */
				   Word16 h2[L_CODE];						/* cor_h() */
				   Word16 rrv[L_CODE];						/* search_10and8i40() */

				   Word16 ipos[8];
					Word16 pos_max[NB_TRACK_MR102];
					Word16 codvec[8];
				   Word16 dn[L_CODE];
					Word16 sign[L_CODE];
				   Word16 rr[L_CODE][L_CODE];
				   Word16 linear_signs[NB_TRACK_MR102];
				   Word16 linear_codewords[8];
            } s7E;

            struct { // s8E	3812 bytes						/* code_10i40_35bits() */
				   Word32 y32[L_CODE];						/* cor_h_x2()   */
				   Word16 en[L_CODE];                  /* set_sign12k2() correlation vector */
				   Word16 h2[L_CODE];						/* cor_h() */
				   Word16 rrv[L_CODE];						/* search_10and8i40() */

				   Word16 ipos[10];
					Word16 pos_max[6];
					Word16 codvec[10];
				   Word16 dn[L_CODE];
					Word16 sign[L_CODE];
				   Word16 rr[L_CODE][L_CODE];

            } s8E;

            struct { // s9E	88 bytes
					Word32 coeff[10];							/* MR475_gain_quant() , Qua_gain() */

				   Word16 frac_en[4];						/* MR795_gain_quant() */
				   Word16 exp_en[4];
				   Word16 g_pitch_cand[3];      			/* pitch gain candidates   Q14 */
				   Word16 g_pitch_cind[3];      			/* pitch gain indices      Q0  */

				   Word16 frac_coeff[5];					/* gainQuant() */
				   Word16 exp_coeff[5];
            } s9E;

         } u2E;

      } stE;

   } shabuf;

} AMR_Shared_Buf;


/*-----------------------------------------------------------*
 *    Coder constant parameters (defined in "cnst.h")        *
 *-----------------------------------------------------------*
 *   L_WINDOW    : LPC analysis window size.                 *
 *   L_NEXT      : Samples of next frame needed for autocor. *
 *   L_FRAME     : Frame size.                               *
 *   L_FRAME_BY2 : Half the frame size.                      *
 *   L_SUBFR     : Sub-frame size.                           *
 *   M           : LPC order.                                *
 *   MP1         : LPC order+1                               *
 *   L_TOTAL7k4  : Total size of speech buffer.              *
 *   PIT_MIN7k4  : Minimum pitch lag.                        *
 *   PIT_MAX     : Maximum pitch lag.                        *
 *   L_INTERPOL  : Length of filter for interpolation        *
 *-----------------------------------------------------------*/
typedef struct
{
   Flag dtx;
   Word16 *speech, *p_window, *p_window_12k2;
   Word16 *new_speech;             /* Global variable */
	Word16 *error;
   Word16 *wsp;

   /* Speech vector */
   Word16 old_exc[L_FRAME + PIT_MAX + L_INTERPOL];
   Word16 old_speech[L_TOTAL];
   Word16 old_wsp[L_FRAME + PIT_MAX +1];
   Word16 hvec[L_SUBFR * 2];
   Word16 ai_zero[L_SUBFR + MP1+1];
   Word16 mem_syn[M], mem_w0[M], mem_w[M];
   Word16 mem_err[M + L_SUBFR];

   /* Past LSPs */
   Word16 lsp_old[M];
   Word16 lsp_old_q[M];
   /* Quantization state */
	Word16 past_rq[M];    /* Past quantized prediction error, Q15 */

   Word16 old_lags[5];
   Word16 ol_gain_flg[2];

   /* Weight speech vector */
   /* OL LTP states */
   /* Excitation vector */
   Word16 *exc;
   /* Zero vector */
   Word16 *zero;
   /* Impulse response vector */
   Word16 *h1;

   /* Substates */
	Word16 old_A[M + 1];
	Word16 T0_prev_subframe;
   gainQuantState  gainQuantSt;
   pitchOLWghtState pitchOLWghtSt;
   tonStabState tonStabSt;
   vadState vadSt;

   dtx_encState dtx_encSt;
   /* Filter's memory */
   Word16 sharp;
} cod_amrState;


typedef struct{
	Word32         dtx;
   AMR_Shared_Buf *share;
	cod_amrState   cod_amr_state;
	PreState       prestate;
	sid_syncState  sid_state;
	AMR_ENC_HANDLE amr_enc_handle;
} AMR_Encode_FrameState;


/*--------------------------------------------------------------------------*
 *       DECODER constant parameters                                        *
 *--------------------------------------------------------------------------*/

/*Bgnscd.h*/
#define L_ENERGYHIST 60
#define INV_L_FRAME 102
/* 2*(160*x)^2 / 65536  where x is FLP values 150,5 and 50 */
#define FRAMEENERGYLIMIT  17578         /* 150 */
#define LOWERNOISELIMIT      20         /*   5 */
#define UPPERNOISELIMIT    1953         /*  50 */

/*C_g_aver.h*/
#define L_CBGAINHIST 7

/*lsp_avg.h*/
#define EXPCONST          5243               /* 0.16 in Q15 */

/*ph_disp.h*/
#define PHDGAINMEMSIZE 5
#define PHDTHR1LTP     9830  /* 0.6 in Q14 */
#define PHDTHR2LTP     14746 /* 0.9 in Q14 */
#define ONFACTPLUS1    16384 /* 2.0 in Q13   */
#define ONLENGTH 2

/*dtx_dec.h*/
#define DTX_MAX_EMPTY_THRESH 50


/*
********************************************************************************
*                         DEFINITION OF DECODER DATA TYPES
********************************************************************************
*/

typedef struct{
   /* history vector of past synthesis speech energy */
   Word16 frameEnergyHist[L_ENERGYHIST];
   /* state flags */
   Word16 bgHangover;       /* counter; number of frames after last speech frame */
} Bgn_scdState;

typedef struct{
   /* history vector of past synthesis speech energy */
   Word16 cbGainHistory[L_CBGAINHIST];
   /* state flags */
   Word16 hangVar;       /* counter; */
   Word16 hangCount;     /* counter; */
} Cb_gain_averageState;

typedef struct {
  Word16 lsp_meanSave[M];          /* Averaged LSPs saved for efficiency  */
} lsp_avgState;

typedef struct {
  Word16 past_r_q[M];   /* Past quantized prediction error, Q15 */
  Word16 past_lsf_q[M]; /* Past dequantized lsfs,           Q15 */
} D_plsfState;

typedef struct {
  Word16 pbuf[5];
  Word16 past_gain_pit;
  Word16 prev_gp;
} ec_gain_pitchState;

typedef struct {
  Word16 gbuf[5];
  Word16 past_gain_code;
  Word16 prev_gc;
} ec_gain_codeState;

typedef struct {
  Word16 gainMem[PHDGAINMEMSIZE];
  Word16 prevState;
  Word16 prevCbGain;
  Word16 lockFull;
  Word16 onset;
} ph_dispState;

enum DTXStateType {SPEECH = 0, DTX, DTX_MUTE};

typedef struct {
   Word32 L_pn_seed_rx;

   Word16 since_last_sid;
   Word16 true_sid_period_inv;
   Word16 log_en;
   Word16 old_log_en;
   Word16 lsf_hist_ptr;
   Word16 log_pg_mean;

   Word16 lsp[M];
   Word16 lsp_old[M];
   Word16 lsf_hist[M*DTX_HIST_SIZE];
   Word16 lsf_hist_mean[M*DTX_HIST_SIZE];
   Word16 log_en_hist[DTX_HIST_SIZE];

   Word16 log_en_hist_ptr;
   Word16 log_en_adjust;
   Word16 dtxHangoverCount;
   Word16 decAnaElapsedCount;
   Word16 sid_frame;
   Word16 valid_data;
   Word16 dtxHangoverAdded;
   Word16 data_updated;      /* marker to know if CNI data is ever renewed */

   enum DTXStateType dtxGlobalState;     /* contains previous state */
                                         /* updated in main decoder */
} dtx_decState;

typedef struct{
	Word16 res2[L_SUBFR];
	Word16 mem_syn_pst[M];
	Word16 synth_buf[M + L_FRAME];
	Word16 mem_pre;          /* filter state */
	Word16 past_gain;
}Post_FilterState;

typedef struct {
   Word32 y2;
   Word32 y1;
   Word16 x0;
   Word16 x1;
} Post_ProcessState;

typedef struct
{
	/* Excitation vector */
	Word16 *exc;
	Word16 old_exc[L_SUBFR + PIT_MAX + L_INTERPOL];

	D_plsfState lsfState;
   gc_predState pred_state;

	/* Lsp (Line spectral pairs) */
   /* Word16 lsp[M]; */      /* Used by CN codec */
	Word16 lsp_old[M];

	/* Filter's memory */
	Word16 mem_syn[M];

	/* pitch sharpening */
	Word16 sharp;
	Word16 old_T0;

	/* Memories for bad frame handling */
	Word16 prev_bf;
	Word16 prev_pdf;
	Word16 state;
	Word16 excEnergyHist[9];

	/* Variable holding received ltpLag, used in background noise and BFI */
	Word16 T0_lagBuff;

	/* Variables for the source characteristic detector (SCD) */
	Word16 inBackgroundNoise;
	Word16 voicedHangover;
	Word16 ltpGainHistory[9];
	Word16 nodataSeed;

	Bgn_scdState background_state;
	Cb_gain_averageState Cb_gain_averState;
	lsp_avgState lsp_avg_st;
   ec_gain_pitchState ec_gain_p_st;
   ec_gain_codeState ec_gain_c_st;
   dtx_decState dtxDecoderState;
   ph_dispState ph_disp_st;
} Decoder_amrState;

typedef struct
{
	enum Mode prev_mode;
	AMR_Shared_Buf *share;
	Decoder_amrState decoder_amrState;
	Post_FilterState  post_state;
	Post_ProcessState postHP_state;
	Word16 reset_flag;
	Word16 reset_flag_old;
	AMR_DEC_HANDLE amr_dec_handle;
} AMR_Decode_FrameState;


/* implemented on target to provide Assert Check*/
//extern void AMR_ASSERT( int expression );

#endif
