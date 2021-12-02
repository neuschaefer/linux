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
/*  Module Name:            tuner_env57U.h													*/
/*  Author (Nationality):   Luke Grove (British)	                                        */
/*  Description:            Header file for ENV57 (ET29) Tuner Driver						*/
/*																							*/
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

#ifndef __TUNERENV57U_H__
#define __TUNERENV57U_H__
/*------------------------------------------------------------------------------
INCLUDES
------------------------------------------------------------------------------*/
#include "dd_tuner.h"

#ifdef __cplusplus
extern "C"
{
#endif



    /*------------------------------------------------------------------------------
    DEFINES
    ------------------------------------------------------------------------------*/
/* Ref frequency = 166.667 kHz = 1/6 MHz = 0.5/3 MHz */
#define refFreqNumerator_env57U 500  /*kHz*/
#define refFreqDenominator_env57U 3
#define IF_env57U 36000		/*This is the IF frequency. Changed from default of 36167 on 01/02/2010, and back from 36000 on 16/2/2010 */
/*#define DEMOD_MAX_FREQ_OFFSET 500 */ /* kHz */

/* AGC thresholds for use in search() routine */
#define	AGC_THRESHOLD_T2_57U 200
#define	AGC_THRESHOLD_T_57U  190
#define	AGC_THRESHOLD_C_57U  250

/* Ref frequency = 62.5 kHz = 1/16 MHz = 0.5/8 MHz */
#define refFreqDenominator_dvbc     8
#define refFreqDenominator_A  8

#ifndef MAXFRQ_ENV57U
#define MAXFRQ_ENV57U  ((Frequency_t)(0x7FFFFFFF))
#endif

#ifndef TUNER_I2C_ADDR_ENV57U

#define TUNER_I2C_ADDR_ENV57U ( 0xC2 )		
#endif

#ifndef TUNER_DEV_ID_ENV57U
#define TUNER_DEV_ID_ENV57U   (0)         /* default tuner device id */
#endif

    /*------------------------------------------------------------------------------
    TYPEDEFS
    ------------------------------------------------------------------------------*/
    typedef struct
    {
        Frequency_t   maxFreq; /* Maximum RF center frequency (kHz) for which these     */
        /* control bytes are still valid,                        */
        /*   set to ((u32_t)-1) for last entry in array          */
        /*                                                       */
        u8_t            CB1;    /* First  control byte                                   */
        u8_t            CB2;    /* Second control byte                                   */
        u8_t            CB3;    /* Third control byte                                    */
    } TUNERENV57UControlUnit_t,
    *pTUNERENV57UControlUnit_t;

    typedef enum
    {
        ENV57U_112,	/* 118dBuV  TOP      */
        ENV57U_110,
        ENV57U_108,	/*recommended for DVB-T2*/
        ENV57U_106,
        ENV57U_104,	/*recommended for DVB-C*/  
        ENV57U_102,
        ENV57U_100,
        ENV57U_Analog     /*recommended for Analog*/
    }        TunerENV57U_TOP_t;

    typedef enum
    {
        ENV57U_AGC_NORMAL_SPEED,	/* for channel tuning      */
        ENV57U_AGC_FAST_SPEED	/* for channel tuning      */
    }        TunerENV57U_ATC_t;

    typedef enum
    {
        ENV57U_GCA_ON,	/* for channel tuning      */
        ENV57U_GCA_OFF	/* for channel tuning      */
    }        TunerENV57U_DISGCA_t;

    typedef enum
    {
        ENV57U_IF_DIGITAL,	/* for channel tuning      */
        ENV57U_IF_ANALOGUE	/* for channel tuning      */
    }        TunerENV57U_IFDA_t;

    typedef enum
    {
        ENV57U_USE_IF_AMP,	/* IF amp used      */
        ENV57U_USE_MIX	/* Direct mixer output      */
    }        TunerENV57U_AISL_t;

    typedef enum
    {
        ENV57U_SAW_8MHZ,    
        ENV57U_SAW_7MHZ		/* for channel tuning      */
    }        TunerENV57U_SAW_BW_t;



    typedef struct
    {
        u8_t			lockMask;            /* Mask to apply to check tuner lock          */
        u8_t			lockValue;           /* Value that indicates tuner lock            */
        TunerENV57U_TOP_t		TOPValue;			/* Take-Over-Point setting */
        TunerENV57U_ATC_t		ATCValue;			/*	AGC Time-constant */
        TunerENV57U_SAW_BW_t	SAW_BWValue;		/* SAW BW */
        TunerENV57U_DISGCA_t	DISGCAValue;		/* Digital GCA control bit */
        TunerENV57U_IFDA_t	IFDAValue;			/* IF output control */
        TunerENV57U_AISL_t	AISLValue;			/* AGC detector input selection */
    } TUNERENV57UData_t,
    *pTUNERENV57UData_t;

    /*------------------------------------------------------------------------------
    ENUM
    ------------------------------------------------------------------------------*/

    /*------------------------------------------------------------------------------
    STRUCTS
    ------------------------------------------------------------------------------*/


    /*------------------------------------------------------------------------------
    Exported FUNCTIONS
    ------------------------------------------------------------------------------*/


    extern MNStatus_t TUNER_ENV57U_Open ( void );

    extern MNStatus_t TUNER_ENV57U_Close ( void );

    extern MNStatus_t TUNER_ENV57U_SetFrequency ( TUNERMode_t      mode,
            Frequency_t   centerFrequency );

    extern MNStatus_t TUNER_ENV57U_GetFrequency ( pFrequency_t  RFfrequency,
            pFrequency_t  IFfrequency );
	extern MNStatus_t TUNER_ENV57U_GetRSSI ( u16_t  IF_mV, u16_t  RF_mV, ps16_t Power );

    extern MNStatus_t TUNER_ENV57U_LockStatus ( pTUNERLockStatus_t lockStat );

    /*-------------------------------------------------------------------------
    Exported GLOBAL VARIABLES
    -------------------------------------------------------------------------*/
    extern TUNERFunc_t  TUNERENV57UFunctions_g;


    extern TUNERInstance_t          tunerPanasonicENV57U04D8F;


    /*----------------------------------------------------------------------------*/
    /*----end-Panasonic-ENV57U04D8F------------------------------------------------*/
    /*------------------------------------------------------------------------------
    THE END
    ------------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
#endif   /* TUNERENV57U_H */

/*============================================================================================
    Revision History

    Dlr_Log: tuner_env57U.h,v Dlr_
    Revision 1.1  2010/09/22 13:58:56  bells
    Initial check-in

    Revision 1.1  2010/09/16 17:30:39  bells
    Initial check-in to PSDE CVS repository

==============================================================================================*/
