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
/*  Module Name:            tuner_api_data.h												*/
/*  Author (Nationality):   Luke Grove (British)	                                        */
/*  Description:            Definition header of data used on tuner API (tuner_api.c)		*/
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

#ifndef _TUNER_API_DATA_H_
#define _TUNER_API_DATA_H_


#ifdef __cplusplus
extern "C"
{
#endif

    //#include <errno.h>


    /*
    Tuner API data declarations:
    */

#define LOCK_TIMEOUT    500                /* timeout [msec] for MPEG lock          */      
#define INTERRUPT_NO    48          /*IREQ port*/

#define P4IPC_MAXMSG    64
#define P4IPC_MSGSIZE   128


#define MAX_PLP  30		//PLPs in a channel
#define	MAX_CHANNELS	15		// Max no. of searched channels


    /* Some debug stings */
    extern const s8_t       tu_bandwidth_Strings[6][ 32 ];
    extern const s8_t       tu_mirror_Strings[3][ 32 ];
    extern const s8_t       tu_hierarchy_Strings[5][ 32 ];
    extern const s8_t       tu_priority_Strings[3][ 32 ];
    extern const s8_t       tu_code_rate_Strings[8][ 32 ];
    extern const s8_t       tu_guard_interval_Strings[8][ 32 ];
    extern const s8_t       tu_carrier_mode_Strings[3][ 32 ];
    extern const s8_t       tu_transmission_mode_Strings[8][ 32 ];
    extern const s8_t       tu_modulation_Strings[12][ 32 ];
    extern const s8_t       tu_rcvr_type_Strings[4][ 32 ];

    /*****************************************
    *
    *   ENUMERATION DECLARATION
    *
    *****************************************/

    /**
    * \enum tu_rx_status_t
    * \brief Receiver status.
    */
    typedef enum 
    {
        tue_RX_STATUS_locked                = 0,
        tue_RX_STATUS_not_locked,
        tue_RX_STATUS_RESET,					/* All modes (T/T2/C) */
        tue_RX_STATUS_TUNING,					/* All modes (T/T2/C) */
        tue_RX_STATUS_AGC_CAPTURE,				/* All modes (T/T2/C) */
        tue_RX_STATUS_T2_P1_DETECT,				/* T2 */
        tue_RX_STATUS_T2_GI_SEARCH,
        tue_RX_STATUS_T2_CLOCK_DETECT,
        tue_RX_STATUS_T2_TRANSIT,
        tue_RX_STATUS_T2_FREQ_DETECT,
        tue_RX_STATUS_T2_L1_PRE_DETECT,
        tue_RX_STATUS_T2_L1_POST_DETECT,
        tue_RX_STATUS_T_TM_SRCH,
        tue_RX_STATUS_T_CLOCK_DETECT,
        tue_RX_STATUS_T_TRANSIT,
        tue_RX_STATUS_T_FREQ_DETECT,
        tue_RX_STATUS_T_FRAME_DETECT,
        tue_RX_STATUS_C_SYM_SRCH,
        tue_RX_STATUS_C_TRANSIT,
        tue_RX_STATUS_C_TM_SRCH,
        tue_RX_STATUS_C_TM_DETECT,
        tue_RX_STATUS_C_FRAME_DETECT,
        tue_RX_STATUS_no_signal
 
} tu_rx_status_t,
    *ptu_rx_status_t;

    /**
    * \enum tu_return_t
    * \brief Return codes.
    */
    typedef enum
    {
        tue_RETURN_ok                       = 0,
        tue_RETURN_error,
        tue_RETURN_no_memory,
        tue_RETURN_invalid_param,
        tue_RETURN_file_io
    }                                    tu_return_t;


    /*****************************************
    *
    *   STRUCTURES DECLARATION
    *
    *****************************************/

    /*
    Tuner API structs declarations:
    */
/*    typedef struct
    {
        tu_frequency_t          freq;
        tu_modulation_t         modulation;
        tu_bandwidth_t          bandwidth;
        tu_mirror_t             mirror;
        tu_hierarchy_t          hierarchy;
        tu_priority_t           priority;
        tu_code_rate_t          code_rate;
        tu_guard_interval_t     guard_interval;
        tu_transmission_mode_t  transmission_mode;
        tu_carrier_mode_t       carrier_mode;
        tu_plp_id_t             plp_id;
        tu_pilot_pattern_t      ppt;
        tu_rcvr_type_t          receiverType;
		tu_symbol_rate_t		symbol_rate;
		tu_tune_mode_t			tune_mode;
    }
	*/

	// Found channel characteristics, including all info needed for a manual (normally fastest) tune.
/*    typedef struct
    {
		// Common to all...
        tu_frequency_t			frequency;				// Frequency of found channel
		tu_rcvr_type_t			mode;					// Rx mode
		tu_bandwidth_t			bandwidth;				// Bandwidth
		// DVB-C specific...
		tu_modulation_t			constellation;			// DVB-C constellation
		// DVB-T2/T specific...
		tu_guard_interval_t		guard_interval;			// T2/T GI
		tu_transmission_mode_t	fft;					// T FFT
		tu_pilot_pattern_t		pilot;					// T2 pilot pattern
		tu_carrier_mode_t		carrier_mode;			// T2 carrier mode (extended or normal)
		tu_mirror_t				spectrum;				// T2 spectrum (normal / inverted)
        u8_t					num_plps;				// T2 number of PLPs found
    }tu_found_freq_t,
    *ptu_found_freq_t;
*/
/*    typedef struct
    {
        tu_frequency_t  srch_start_freq;
        tu_frequency_t  srch_stop_freq;
		tu_frequency_t	srch_step_size;
		tu_rcvr_type_t	ReceiverMode;					// Rx mode
		tu_bandwidth_t	channel_bandwidth;
        tu_found_freq_t  found_freqs[ MAX_CHANNELS ];	// table of frequencies found
        u8_t            num_frequencies;				// number of frequencies found
    }tu_search_setup_t,
    *ptu_search_setup_t;
*/

/*
    typedef struct
    {
        tu_chan_setup_t setup;
        tu_rx_status_t  status;
        Bool_t          tune_aborted;
    }tu_chan_database_t;


    typedef struct
    {
        u8_t		            id;
        tu_modulation_t         modulation;
        tu_guard_interval_t     guard_interval;
        tu_code_rate_t          code_rate;
        tu_transmission_mode_t  transmission_mode;
    }tu_plp_info_t;

    typedef struct
    {
        u8_t			num_plp;
        tu_plp_info_t   plp_info[ MAX_PLP ];
    }tu_chan_plp_info_t;

*/

#ifdef __cplusplus
}
#endif
#endif  /* _TUNER_API_DATA_H_ */
