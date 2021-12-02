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
/*==========================================================================================*/
/*  Panasonic System LSI Design Europe, a Division of Panasonic Europe Ltd.					*/
/*                                                                                          */
/*  Module Name:            demod_MN88471.h													*/
/*  Author (Nationality):   Luke Grove (British)	                                        */
/*  Description:            Header file for the MN specific driver functions				*/
/*																						    */
/*  Date First Created:     1st January 2010                                                */
/*  Related Specifications (PSDE document refs):                                            */
/*  Master file stored in CVS at: $Achive: $ */
/*                                                                                          */
/*  Status (choose one):    (*) under development           ( ) internal release            */
/*                          ( ) external release            ( ) example only                */
/*  Confidentiality:        ( ) Strictly Confidential       (*) Confidential                */
/*  (choose one)            ( ) Internal Use Only (IUO)     ( ) Open                        */
/*                                                                                          */
/*  Note: This code should only be viewed or otherwise used by customers of Panasonic after */
/*  an appropriate Non-Disclosure Agreement and/or Licence Agreement has been signed by.    */
/*  both Panasonic and the customer.  If in doubt, please check with your Panasonic         */
/*  representative. If you have reason to believe this code is NOT subject to the above,    */
/*  please delete all copies of it and related files from your systems immediately.         */
/*                                                                                          */
/*  (c) 2010 Panasonic Europe Limited.														*/
/*==========================================================================================*/

/*==========================================================================================*/
/*  THIS PROGRAM IS EXAMPLE SOFTWARE PROVIDED TO CUSTOMERS OF PANASONIC IN THE HOPE THAT IT */
/*  WILL BE USEFUL, BUT WITHOUT ANY WARRANTY EXPRESSED OR IMPLIED.  THE PROGRAM IS PROVIDED */
/*  ON AN "AS IS" BASIS.  THE ENTIRE RISK AS TO THE PROGRAM QUALITY AND PERFORMANCE IS WITH */
/*  THE USER. IN NO EVENT WILL PANASONIC BE LIABLE FOR DIRECT,  INDIRECT,  OR INCIDENTAL OR */
/*  CONSEQUENTIAL DAMAGES RESULTING FROM ANY DEFECT IN THE PROGRAM,   EVEN IF PANASONIC HAS */
/*  BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.                                        */
/*                                                                                          */
/*  THIS PROGRAM  HAS BEEN DEVELOPED  OUTSIDE THE SCOPE OF  PANASONIC SEMICONDUCTOR COMPANY */
/*  DEVELOPMENT PROCEDURES,  AND IS PROVIDED ONLY AS EXAMPLE SOFTWARE.                      */
/*==========================================================================================*/
#ifndef __DEMOD_MN88471_H__
#define __DEMOD_MN88471_H__

/*-------------------------------------------------------------------------
INCLUDES
-------------------------------------------------------------------------*/

#include "dd_types.h"

#include "dd_i2c.h"
#include "dd_tuner.h"
#include "dd_util.h"
//#include "PSC_BIOS.h"
#include "tuner_env57U.h"

#include "tuner_api_data.h"

// MTK include files
#include "PD_Def.h"  

#ifdef __cplusplus
extern "C"
{
#endif

    /*-------------------------------------------------------------------------
    TYPEDEFS
    -------------------------------------------------------------------------*/



    /**
    * \def MN_UNKNOWN
    * \brief Generic UNKNOWN value for MN enumerated types.
    *
    * Used to indicate that the parameter value is unknown or not yet initalized.
    */
#ifndef MN_UNKNOWN
#define MN_UNKNOWN (254)
#endif

    /**
    * \def MAX_PLP
    * \brief Max number of PLP to retrieve in search function.
    *
    */

#ifndef MAX_PLP
#define MAX_PLP  30
#endif

/*-------- Defines, configuring the API -------------------------------------*/

#define MASTER_OSCILLATOR_FREQ 20500L	/* Crystal reference, in kHz */
#define AGC_OUT_VOLTAGE_MAX 3300U		/* The voltage level (mV) corresponding to the 10-bit 0x3FF AGC level */
/*
 * Chunk/mode checking
 */
#ifndef MNIC_SINGLE_MASTER
#define MNIC_SINGLE_MASTER 1
#endif

/*
* Comments about MNIC_MAX_WCHUNKSIZE in single or multi master mode:
*
* In single master mode, data can be written by sending the register address
* first, then two or four bytes of data in the next packet.
* Because the device address plus a register address equals five bytes,
* the mimimum chunk size must be five.
* If ten-bit I2C device addresses are used, the minimum chunk size must be six,
* because the I2C device address will then occupy two bytes when writing.
*
* Data in single master mode is transferred as follows:
* <S> <devW>  a0  a1  a2  a3  <P>
* <S> <devW>  d0  d1 [d2  d3] <P>
* ..
* or
* ..
* <S> <devW>  a0  a1  a2  a3  <P>
* <S> <devR> --- <P>
*
* In multi-master mode, the data must immediately follow the address (an I2C
* stop resets the internal address), and hence the minimum chunk size is
* 1 <I2C address> + 4 (register address) + 2 (data to send) = 7 bytes (8 if
* 10-bit I2C device addresses are used).
*/
#if !defined( MNIC_MAX_WCHUNKSIZE)
#define  MNIC_MAX_WCHUNKSIZE 254
#endif

#if MNIC_SINGLE_MASTER
#define  MNIC_MAX_WCHUNKSIZE_MIN 5
#else
#define  MNIC_MAX_WCHUNKSIZE_MIN 7
#endif

#if  MNIC_MAX_WCHUNKSIZE <  MNIC_MAX_WCHUNKSIZE_MIN
#if MNIC_SINGLE_MASTER
#error  MNIC_MAX_WCHUNKSIZE must be at least 5 in single master mode
*;   /* illegal statement to force compiler error */
#else
#error  MNIC_MAX_WCHUNKSIZE must be at least 7 in multi master mode
*;   /* illegal statement to force compiler error */
#endif
#endif

#if !defined( MNIC_MAX_RCHUNKSIZE)
#define  MNIC_MAX_RCHUNKSIZE 254
#endif

#if  MNIC_MAX_RCHUNKSIZE < 2
#error  MNIC_MAX_RCHUNKSIZE must be at least 2
*;   /* illegal statement to force compiler error */
#endif

#if  MNIC_MAX_RCHUNKSIZE & 1
#error  MNIC_MAX_RCHUNKSIZE must be even
*;   /* illegal statement to force compiler error */
#endif


    /*-------------------------------------------------------------------------
    ENUM
    -------------------------------------------------------------------------*/

    /**
    * \enum MNBandwidth_t
    * \brief Channel bandwidth or channel spacing.
    */
    typedef enum
    {
        BANDWIDTH_8MHZ                       = 0,

        BANDWIDTH_7MHZ,

        BANDWIDTH_6MHZ,

        BANDWIDTH_5MHZ,

        BANDWIDTH_1dot7MHZ,

        BANDWIDTH_UNKNOWN                    //= MN_UNKNOWN   /**< Bandwidth unknown. */
    }        Bandwidth_t, *pBandwidth_t;


    /**
    * \enum MNMirror_t
    * \brief Indicate if channel spectrum is mirrored or not.
    */
    typedef enum
    {
        MIRROR_NO                                           = 0,

        MIRROR_YES,

        MIRROR_UNKNOWN                                    //   = MN_UNKNOWN

    }        Mirror_t, *pMirror_t;

    /**
    * \enum MNConstellation_t
    * \brief Constellation type of the channel.
    */
    typedef enum
    {
        CONSTELLATION_BPSK                       = 0,

        CONSTELLATION_QPSK,

        CONSTELLATION_PSK8,
 
        CONSTELLATION_QAM16,

        CONSTELLATION_QAM32,

        CONSTELLATION_QAM64,
  
        CONSTELLATION_QAM128,
 
        CONSTELLATION_QAM256,

        CONSTELLATION_QAM512,

        CONSTELLATION_QAM1024,

        CONSTELLATION_QPSK_NR,

        CONSTELLATION_UNKNOWN              //     = MN_UNKNOWN

    }        Constellation_t, *pConstellation_t;

    /**
    * \enum MNHierarchy_t
    * \brief Hierarchy of the channel.
    */
    typedef enum
    {
        HIERARCHY_NONE                               = 0,

        HIERARCHY_ALPHA1,

        HIERARCHY_ALPHA2,

        HIERARCHY_ALPHA4,

        HIERARCHY_UNKNOWN                        //    = MN_UNKNOWN

    }        Hierarchy_t, *pHierarchy_t;

    /**
    * \enum MNPriority_t
    * \brief Channel priority in case of hierarchical transmission.
    */
    typedef enum
    {
        PRIORITY_LOW                         = 0,

        PRIORITY_HIGH,

        PRIORITY_UNKNOWN                   //  = MN_UNKNOWN  /**< Priority unknown.      */
    }        Priority_t, *pPriority_t;

    /**
    * \enum MNCoderate_t
    * \brief Channel priority in case of hierarchical transmission.
    */
    typedef enum
    {
        CODERATE_1DIV2                       = 0,

        CODERATE_3DIV5,
   
        CODERATE_2DIV3,
   
        CODERATE_3DIV4,
 
        CODERATE_4DIV5,

        CODERATE_5DIV6,
  
        CODERATE_7DIV8,
 
        CODERATE_UNKNOWN                  //   = MN_UNKNOWN

    }        Coderate_t, *pCoderate_t;

    /**
    * \enum MNGuard_t
    * \brief Guard interval of a channel.
    */
    typedef enum
    {
        GUARD_1DIV32                                 = 0,
 
        GUARD_1DIV16,

        GUARD_1DIV8,

        GUARD_1DIV4,

        GUARD_1DIV128,

        GUARD_19DIV128,
    
        GUARD_19DIV256,
     
        GUARD_UNKNOWN                           //     = MN_UNKNOWN
 
    }        Guard_t, *pGuard_t;

    /**
    * \enum MNFftmode_t
    * \brief FFT mode.
    */
    typedef enum
    {
        FFTMODE_1K,					//added to comply with spec EN302755 V1.2.1
 
        FFTMODE_2K,
    
        FFTMODE_4K,

        FFTMODE_8K,
  
        FFTMODE_16K,
   
        FFTMODE_32K,
   
        FFTMODE_64K,
        FFTMODE_UNKNOWN                 //     = MN_UNKNOWN
 
    }        Fftmode_t, *pFftmode_t;

    /**
    * \enum MNCarriermode_t
    * \brief Carrier mode.
    */
    typedef enum
    {
        CARRIER_MODE_NORMAL                       = 0,
  
        CARRIER_MODE_EXTENDED,
     
        CARRIER_MODE_UNKNOWN                //      = MN_UNKNOWN
    
    }        Carrier_t, *pCarrier_t;


    /**
    * \enum MNOutputmode_t
    * \brief output mode.
    */
    typedef enum
    {
        OUTPUT_MODE_SERIAL                                   = 0,
   
        OUTPUT_MODE_PARALLEL_NORMAL,
  
        OUTPUT_MODE_PARALLEL_BURST,
    
        OUTPUT_MODE_UNKNOWN                        //          = MN_UNKNOWN                /*output mode unknown*/
    }        MNOutputmode_t, *pMNOutputmode_t, tu_output_mode_t;

    typedef enum
    {
        TUNE_MANUAL                       = 0,
        TUNE_AUTO
    }        Tune_mode_t, *pTune_mode_t;

    typedef enum
    {
        TUNE_SLOW                       = 0,
        TUNE_FAST
    }        Tune_speed_t, *pTune_speed_t;

    typedef enum
    {
        BER_VITERBI           = 0,	/* DVB-T Viterbi BER or DVB-T2 BER before LDPC */
        BER_LDPC_PER						/* DVB-T PER or DVB-T2 BER after LDPC */
    }        MNber_mode_t, *pMNber_mode_t, tu_ber_mode_t;

/* ReceiverMode_t in dd_types.h! */


    /**
    * \enum MNLockStatus_t
    * \brief Used to reflect current lock status of demodulator.
    *
    * The generic lock states have device dependent semantics.
    */
    typedef enum
    {
        MN_NOT_LOCKED = 0,				/* Device has no lock at all */
        MN_RESET = 0,					/* All modes (T/T2/C) */
        MN_TUNING = 1,					/* All modes (T/T2/C) */
        MN_AGC_CAPTURE = 2,				/* All modes (T/T2/C) */
        MN_T2_P1_DETECT = 3,				/* T2 */
        MN_T2_GI_SEARCH = 4,
        MN_T2_CLOCK_DETECT = 5,
        MN_T2_TRANSIT1 = 6,
        MN_T2_TRANSIT2 = 7,
        MN_T2_TRANSIT3 = 8,
        MN_T2_TRANSIT4 = 9,
        MN_T2_FREQ_DETECT = 10,
        MN_T2_L1_PRE_DETECT = 11,
        MN_T2_L1_POST_DETECT = 12,
        MN_T2_LOCKED = 13,                 /**< Device is in lock                     */
        MN_T2_NOSIGNAL,
        MN_T_TM_SRCH = 3,				
        MN_T_CLOCK_DETECT = 4,
        MN_T_TRANSIT1 = 5,
        MN_T_TRANSIT2 = 6,
        MN_T_TRANSIT3 = 7,
        MN_T_FREQ_DETECT = 8,
        MN_T_FRAME_DETECT = 9,
        MN_T_SYNC1 = 10,
        MN_T_SYNC2 = 11,
        MN_T_NOSIGNAL,
        MN_C_SYM_SRCH = 3,
        MN_C_TRANSIT1 = 4,
        MN_C_TRANSIT2 = 5,
        MN_C_TM_SRCH = 6,
        MN_C_TM_DETECT = 7,
        MN_C_FRAME_DETECT = 8,
        MN_C_SYNC1 = 9,
        MN_C_SYNC2 = 10,
        MN_C_NOSIGNAL
    }        MNLockStatus_t,
    *pMNLockStatus_t;

    /**
    * \enum MNpilot_pattern_t
    * \brief pilot pattern.
    */
    typedef enum
    {
        PILOT_PATTERN_1			= 0,
        PILOT_PATTERN_2,
        PILOT_PATTERN_3,
        PILOT_PATTERN_4,
        PILOT_PATTERN_5,
        PILOT_PATTERN_6,
        PILOT_PATTERN_7,
        PILOT_PATTERN_8,
        PILOT_PATTERN_UNKNOWN               /**< PPT mode unknown. */
    }        Pilot_pattern_t, *pPilot_pattern_t;
    typedef enum
    {
        NO_ROTATION			= 0,		/**< PLP not rotated */
        ROTATED
    }        Rotation_t;


    typedef u8_t	Plp_type_t;
    typedef Bool_t	Time_int_type_t;
    typedef u8_t	Time_int_length_t;

    typedef enum
    {
        AGC_LOOP_FILTER			= 0,		/**< Loop filter value */
        IF_AGC_CTRL,						/**< IF AGC Control Value */
        RF_AGC_CTRL,						/**< RF AGC Control Value */
        IF_RF_AGC_CTRL,						/**< IF & RF AGC Control Value */
		AGC_TEST_MON						/**< Test Monitor */
    }        AGC_state_t;

    typedef struct
    {
        Plp_id_t	            id;
        Plp_type_t	            plp_type;
        Time_int_type_t	        interleaver_type;
        Time_int_length_t	    interleaver_length;
        Rotation_t				rotation;
        Constellation_t         constellation;
        Coderate_t				PLP_coderate;
        Fftmode_t				fftmode;
    } Plp_info_t, tu_plp_info_t;

    typedef struct
    {
        u8_t			num_plps;				/* Total number of PLPs in channel */
        u8_t			num_data_plps;			/* Number of Data PLPs in channel */
        Plp_id_t		current_plp_id;			/* Currently chosen PLP ID (not number!) */
        u8_t			current_plp_num;		/* Currently chosen PLP number (not ID!) */
        Plp_info_t		plp_info[ MAX_PLP ];
    } Chan_plp_info_t, tu_chan_plp_info_t;
    /**
    * \struct MNSearch_t
    * \brief The set of parameters describing a search request parameters.
    *
    * Used by MN_CTRL_SEARCH_CHANNEL.
    * Only certain fields need to be used for a specfic standard.
    *
    */

    typedef struct
    {
        /* Common to all... */
        Frequency_t			frequency;				/* Frequency of found channel */
        ReceiverMode_t		mode;					/* Rx mode */
        Bandwidth_t			bandwidth;				/* Bandwidth */
        /* DVB-C specific... */
        Constellation_t		constellation;			/* DVB-C constellation */
        /* DVB-T2/T specific... */
        Guard_t				guard;					/* T2/T GI */
        Fftmode_t			fft;					/* T FFT */
        Pilot_pattern_t		pilot;					/* T2 pilot pattern */
        Carrier_t			carrier;				/* T2 carrier mode (extended or normal) */
        Mirror_t			spectrum;				/* T2 spectrum (normal / inverted) */
        Chan_plp_info_t		plp_data;				/* PLPs found */
    } MNfound_freq_t, *pMNfound_freq_t, tu_found_freq_t, *ptu_found_freq_t;

    typedef struct
    {
        Frequency_t		srch_start_freq;				/* Start frequency */
        Frequency_t		srch_stop_freq;					/* Stop frequency */
        Frequency_t		srch_step_size;
        ReceiverMode_t	ReceiverMode;				/* Rx mode */
        Bandwidth_t		channel_bandwidth;
        MNfound_freq_t  found_freqs[ MAX_CHANNELS ];	/* table of frequencies found */
        u8_t            num_frequencies;				/* number of frequencies found */
    } MNSearch_t, *pMNSearch_t, tu_search_setup_t, *ptu_search_setup_t;


    /**
    * \struct MNChannel_t
    * \brief The set of parameters describing a single channel.
    *
    * Used by MN_CTRL_SET_CHANNEL and MN_CTRL_GET_CHANNEL.
    * Only certain fields need to be used for a specfic standard.
    *
    */
    typedef struct
    {
        Frequency_t       frequency;      /**< frequency in kHz                 */
        Constellation_t   constellation;  /**< constellation                    */
        Bandwidth_t       bandwidth;      /**< bandwidth                        */
        Mirror_t          mirror;         /**< mirrored or not on RF            */
        Hierarchy_t       hierarchy;      /**< hierarchy                        */
        Priority_t        priority;       /**< priority                         */
        Coderate_t        L1_coderate;    /**< coderate                         */
        Coderate_t        PLP_coderate;   /**< coderate                         */
        Coderate_t        HP_coderate;    /**< coderate                         */
        Coderate_t        LP_coderate;    /**< coderate                         */
        Guard_t           guard;          /**< guard interval                   */
        Fftmode_t         fftmode;        /**< fftmode                          */
        Carrier_t         carrier;        /**< extended carrier mode            */
        Plp_id_t          plp_id;         /**< plp Id                           */
        Pilot_pattern_t   ppt;            /**< pilot pattern                    */
        ReceiverMode_t	  receiverType;		/**< Receiver Type					*/
        Symbol_rate_t	  symb_rate;		/**< DVB-C symbol rate				*/
        Tune_mode_t		  tune_mode;		/**< Auto / Preset Tune				*/
    } MNChannel_t, *pMNChannel_t, tu_chan_setup_t, *ptu_chan_setup_t;



    /*========================================*/



    /*============================================================================*/
    /*============================================================================*/
    /*== Data access structures ==================================================*/
    /*============================================================================*/
    /*============================================================================*/

    /* Address on device */
    typedef u8_t        MNaddr_t,
    *pMNaddr_t;

    /* Write block of data to device */
    typedef MNStatus_t ( *MNWriteBlockFunc_t ) ( pI2CDeviceAddr_t devAddr, /* address of I2C device        */
                                                 MNaddr_t         addr, /* address of register/memory   */
                                                 u16_t            datasize, /* size of data in bytes        */
                                                 pu8_t            data );         /* data to send                 */


    /* Read block of data from device */
    typedef MNStatus_t ( *MNReadBlockFunc_t ) ( pI2CDeviceAddr_t devAddr, /* address of I2C device        */
                                                MNaddr_t         addr, /* address of register/memory   */
                                                u16_t            datasize, /* size of data in bytes        */
                                                pu8_t            data );         /* receive buffer               */


    /* Write 8-bits value to device */
    typedef MNStatus_t ( *MNWriteReg8Func_t ) ( pI2CDeviceAddr_t devAddr, /* address of I2C device        */
                                                MNaddr_t         addr, /* address of register/memory   */
                                                pu8_t             data );         /* data to send                 */


    /* Read 8-bits value to device */
    typedef MNStatus_t ( *MNReadReg8Func_t ) ( pI2CDeviceAddr_t devAddr, /* address of I2C device        */
                                               MNaddr_t         addr, /* address of register/memory   */
                                               pu8_t            data );         /* receive buffer               */


    typedef struct
    {
        tu_chan_setup_t setup;
        tu_rx_status_t  lock_status;
        Bool_t          tune_aborted;
    } tu_chan_database_t;

	typedef struct
    {
        s16_t	signal_power;
		u8_t	SSI;
		u8_t	SQI;
    } SSI_t, *pSSI_t;

    /*============================================================================*/
    /*============================================================================*/
    /*== Demod instance data structures ==========================================*/
    /*============================================================================*/
    /*============================================================================*/

    typedef struct MNDemodInstance_s *pMNDemodInstance_t;

    /**
    * \struct MNCommonAttr_t
    * \brief Set of common attributes, shared by all MN devices.
    */
    typedef struct
    {
        /* Clocks and tuner attributes */
        Frequency_t		intermediateFreq;  /**< IF,if tuner instance not used. (kHz)*/
        Frequency_t		fADCK_Freq;        /**< ADC sampling frequency = fICK/4 (MN88471 spec, sec 8.1  (kHz)		*/
        Frequency_t		oscClockFreq;      /**< Oscillator clock frequency.  (kHz)  */
        u32_t		    fs_Freq;		   /**< Baseband re-sampling frequency fs (MN88471 spec, sec 11.3  (kHz)	*/
        s16_t           oscClockDeviation; /**< Oscillator clock deviation.  (ppm)  */
        Mirror_t		SpectrumMirror;    /**< Current Spectrum State: Mirrored?	*/
        ReceiverMode_t	receiverType;	   /**< Current Receiver Type				*/
        Bandwidth_t		bandwidth;		   /**< Current Bandwidth					*/
        MNChannel_t     currentChannel;
        Bool_t          currentChannelValid;
        u32_t           bitrate;           /**< Maximum bitrate in b/s in case static clockrate is selected */
        Bool_t          isOpened;          /**< if TRUE instance is already opened.    */
        /* Tuner */
		Frequency_t   tunerFreqRF;
        Frequency_t   tunerMinFreqRF;    /**< minimum RF input frequency, in kHz */
        Frequency_t   tunerMaxFreqRF;    /**< maximum RF input frequency, in kHz */
          Tune_speed_t	tune_speed;		   /**< Fast / Slow Tune					*/  
    } MNCommonAttr_t,
    *pMNCommonAttr_t;


    /**
    * \struct MNDemodInstance_t
    * \brief Top structure of demodulator instance.
    */
    typedef struct MNDemodInstance_s
    {

        pTUNERInstance_t    myTuner;       /**< tuner instance,if NULL then baseband */
        pI2CDeviceAddr_t    myI2CDevAddr;  /**< i2c address and device identifier    */
        pMNCommonAttr_t     myCommonAttr;  /**< common MN attributes                */
        /* generic demodulator data */
    } MNDemodInstance_t;

    /*------------------------------------------------------------------------------ */

    /**
    * \def LOCK_TIMEOUT
    * \brief Default time out for receive judgement.
    */
#define JUDGEMENT_TIMEOUT 4500
#define SRCH_JUDGEMENT_TIMEOUT_T2 3000		/*DVB-T2 search judgement timeout */
#define SRCH_JUDGEMENT_TIMEOUT_T 1000		/*DVB-T search judgement timeout */
#define SRCH_JUDGEMENT_TIMEOUT_C 5000		/*DVB-C search judgement timeout */

    /**
    * \def MN88471_DEF_I2C_ADDR
    * \brief Default I2C addres of a demodulator instance.
    */

#define MN88471_1ST_I2C_ADDR 0x38 /*0x38 for DVB-T2*/
#define MN88471_2ND_I2C_ADDR 0x30 /*0x30 for DVB-T and DVB-C*/



	typedef enum 
    {
        T2regs = MN88471_1ST_I2C_ADDR,
		TCregs = MN88471_2ND_I2C_ADDR
    } MNReg_set_t;

		typedef struct
    {
        MNReg_set_t	register_set;
		u8_t		register_address;
		u8_t		register_value;
    } MN_reg_t,
    *pMN_reg_t;


    /*-------------------------------------------------------------------------
    Exported FUNCTIONS
    -------------------------------------------------------------------------*/
    extern MNStatus_t enable_through_mode ( void );

    extern MNStatus_t MNIC_WriteBlock ( pI2CDeviceAddr_t devAddr, /* address of I2C device        */
                                     MNaddr_t         addr, /* address of register/memory   */
                                     u16_t            datasize, /* size of data                 */
                                        const pu8_t            data );         /* data to send                 */


    extern MNStatus_t MNIC_ReadBlock ( pI2CDeviceAddr_t devAddr, /* address of I2C device        */
                                    MNaddr_t         addr, /* address of register/memory   */
                                    u16_t            datasize, /* size of data                 */
                                    pu8_t            data );         /* data to send                 */



    extern MNStatus_t MNIC_WriteReg8 ( pI2CDeviceAddr_t devAddr, /* address of I2C device        */
                                    MNaddr_t        addr, /* address of register          */
                                    pu8_t            data );         /* data to write                */


    extern MNStatus_t MNIC_ReadReg8 ( pI2CDeviceAddr_t devAddr, /* address of I2C device        */
                                   MNaddr_t        addr, /* address of register          */
                                   pu8_t           data );         /* buffer to receive data       */

    extern MNStatus_t MN88471_Open ( pMNDemodInstance_t  demod );
    extern MNStatus_t MN88471_Close ( pMNDemodInstance_t demod );
	extern MNStatus_t MN88471_Wakeup ( pMNDemodInstance_t demod );
    extern MNStatus_t CtrlLockStatus ( const pMNDemodInstance_t   demod,
                                       pMNLockStatus_t  lockStat );
    extern MNStatus_t CtrlSetOutput ( MNOutputmode_t mode );
    extern MNStatus_t CtrlGetOutput ( MNOutputmode_t *mode );
    extern MNStatus_t CtrlGetAGC ( u16_t *agc, AGC_state_t state );

    extern MNStatus_t CtrlGetReg ( const pMNDemodInstance_t   demod, pMN_reg_t Reg_Set );
    extern MNStatus_t CtrlSetReg ( const pMNDemodInstance_t   demod, pMN_reg_t Reg_Set );
    extern MNStatus_t CtrlGetCN ( const pMNDemodInstance_t   demod, MNdouble_t *CN );
    extern MNStatus_t CtrlGetBER ( const pMNDemodInstance_t   demod, MNdouble_t *BER, MNber_mode_t berstate );
    extern MNStatus_t CtrlSetReceiverMode ( pMNDemodInstance_t   demod,
                                            ReceiverMode_t mode, Bandwidth_t bandwidth );
    extern MNStatus_t CtrlGetReceiverMode ( ReceiverMode_t *mode );

    extern MNStatus_t CtrlGetChannelInfo ( const pMNDemodInstance_t   demod,
                                           MNChannel_t *params );
    extern MNStatus_t CtrlSetChannel ( pMNDemodInstance_t   demod,
                                       pMNChannel_t   channel );

    extern MNStatus_t CtrlSearchChannel ( pMNDemodInstance_t   demod,
                                          pMNSearch_t   searchParams );
    extern MNStatus_t CtrlSetPlp ( u8_t plp_num );
    extern MNStatus_t CtrlGetPlpInfo ( Chan_plp_info_t* plpInfo );
    extern MNStatus_t CtrlSetPriority ( Priority_t priority );

	extern MNStatus_t CtrlGetRSSI ( pMNDemodInstance_t   demod, u16_t RF_AGC_Voltage, pSSI_t SSI_params );
    extern MNFloatStatus_t NoFloatDiv ( s32_t numerator, s32_t denominator, s8_t b_precision, MNfloat_t *result );
    extern MNfloat_t NoFloatLog10 ( const MNfloat_t *value );

    /*-------------------------------------------------------------------------
    Other FUNCTIONS
    -------------------------------------------------------------------------*/

    /*-------------------------------------------------------------------------
    Exported GLOBAL VARIABLES
    -------------------------------------------------------------------------*/
    extern I2CDeviceAddr_t      MN88471DefaultAddr_g;
    extern MNCommonAttr_t       MN88471DefaultCommAttr_g;
    extern MNDemodInstance_t    demodPanasonicMN88471;

    /*-------------------------------------------------------------------------
    THE END
    -------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
#endif /* __DEMOD_MN88471_H__ */

/*============================================================================================
    Revision History

    $Log: demod_MN88471.h,v $
    Revision 1.1  2010/09/16 17:30:37  bells
    Initial check-in to PSDE CVS repository

==============================================================================================*/
