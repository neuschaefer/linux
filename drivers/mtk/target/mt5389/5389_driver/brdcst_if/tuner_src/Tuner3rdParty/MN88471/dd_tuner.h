/*==========================================================================================*/
/*  Panasonic System LSI Design Europe, a Division of Panasonic Europe Ltd.					*/
/*                                                                                          */
/*  Module Name:            dd_tuner.h														*/
/*  Author (Nationality):   Luke Grove (British)	                                        */
/*  Description:			Header file for Generic Tuner Driver							*/
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
/******************************************************************************
* %Id: by PSL $
*
* FileName   :dd_tuner.h
*
* Description  : tuner common declarations
*
*
* Copyright  :PSL/SC-SSDC
*
*****************************************************************************/

#ifndef __DD_TUNER_H__
#define __DD_TUNER_H__
/*------------------------------------------------------------------------------
INCLUDES
------------------------------------------------------------------------------*/
#include "dd_types.h"
#include "dd_i2c.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /*------------------------------------------------------------------------------
    DEFINES
    ------------------------------------------------------------------------------*/
#define DEMOD_MAX_FREQ_OFFSET 500 /* kHz */

    /* Sub-mode bits should be adjacent and incremental */
#define TUNER_MODE_SUB0 0x0001    /* for sub-mode (e.g. RF-AGC setting)    */
#define TUNER_MODE_SUB1 0x0002    /* for sub-mode (e.g. RF-AGC setting)    */
#define TUNER_MODE_SUB2 0x0004    /* for sub-mode (e.g. RF-AGC setting)    */
#define TUNER_MODE_SUB3 0x0008    /* for sub-mode (e.g. RF-AGC setting)    */
#define TUNER_MODE_SUB4 0x0010    /* for sub-mode (e.g. RF-AGC setting)    */
#define TUNER_MODE_SUB5 0x0020    /* for sub-mode (e.g. RF-AGC setting)    */
#define TUNER_MODE_SUB6 0x0040    /* for sub-mode (e.g. RF-AGC setting)    */
#define TUNER_MODE_SUB7 0x0080    /* for sub-mode (e.g. RF-AGC setting)    */

#define TUNER_MODE_DIGITAL 0x0100 /* for digital channel (e.g. DVB-T)      */
#define TUNER_MODE_ANALOG 0x0200  /* for analog channel  (e.g. PAL)        */
#define TUNER_MODE_SWITCH 0x0400  /* during channel switch & scanning      */
#define TUNER_MODE_LOCK 0x0800    /* after tuner has locked                */
#define TUNER_MODE_6MHZ 0x1000    /* for 6MHz bandwidth channels           */
#define TUNER_MODE_7MHZ 0x2000    /* for 7MHz bandwidth channels           */
#define TUNER_MODE_8MHZ 0x4000    /* for 8MHz bandwidth channels           */

#define TUNER_MODE_SUB_MAX  8
#define TUNER_MODE_SUBALL   ( TUNER_MODE_SUB0 | TUNER_MODE_SUB1 | \
    TUNER_MODE_SUB2 | TUNER_MODE_SUB3 | \
    TUNER_MODE_SUB4 | TUNER_MODE_SUB5 | \
    TUNER_MODE_SUB6 | TUNER_MODE_SUB7 )
    /*------------------------------------------------------------------------------
    TYPEDEFS
    ------------------------------------------------------------------------------*/

    typedef u32_t           TUNERMode_t;
    typedef pu32_t          pTUNERMode_t;

    typedef enum
    {
        TUNER_LOCKED,
        TUNER_NOT_LOCKED
    }        TUNERLockStatus_t,
    *pTUNERLockStatus_t;


    typedef struct
    {
        s8_t				*name;          /* Tuner brand & type name, e.g. for use in listboxes    */
        Frequency_t			minFreqRF;      /* Lowest  RF input frequency, in kHz        */
        Frequency_t			maxFreqRF;      /* Highest RF input frequency, in kHz        */
        void                            *selfCheck;     /* gives proof of initialization            */
        Bool_t                          programmed;     /* only valid if selfCheck is OK            */
        Frequency_t         RFfrequency;    /* only valid if programmed                 */
        Frequency_t         IFfrequency;    /* only valid if programmed                 */
        pReceiverMode_t		ReceiverMode;	/* Demod Receiver Mode */
        I2CDeviceAddr_t     Demod_1st_addr;     /* pointer to associated demod instance     */
        u16_t				T2_AGC;			/* AGC reference for use in channel search - default 0 */
        u16_t				T_AGC;			/* AGC reference for use in channel search - default 0 */
        u16_t				C_AGC;			/* AGC reference for use in channel search - default 0 */
    } TUNERCommonAttr_t,
    *pTUNERCommonAttr_t;


    /*
    * Generic functions for MN devices.
    */
    typedef struct TUNERInstance_s  *pTUNERInstance_t;

    typedef MNStatus_t ( *TUNEROpenFunc_t )  ();
    typedef MNStatus_t ( *TUNERCloseFunc_t ) ();
    typedef MNStatus_t ( *TUNERSetFrequencyFunc_t )( TUNERMode_t      mode, Frequency_t   frequency );
    typedef MNStatus_t ( *TUNERGetFrequencyFunc_t )( pFrequency_t  RFfrequency, pFrequency_t  IFfrequency );
    typedef MNStatus_t ( *TUNERGetRSSIFunc_t )(u16_t  IF_mV, u16_t  RF_mV, ps16_t Power );

    typedef MNStatus_t ( *TUNERi2cWriteReadFunc_t )
( pI2CDeviceAddr_t wDevAddr,
  u16_t            wCount,
  pu8_t            wData,
  pI2CDeviceAddr_t rDevAddr,
  u16_t            rCount,
  pu8_t            rData );
    typedef struct
    {
        TUNEROpenFunc_t         openFunc;
        TUNERCloseFunc_t        closeFunc;
        TUNERSetFrequencyFunc_t setFrequencyFunc;
        TUNERGetFrequencyFunc_t getFrequencyFunc;
		TUNERGetRSSIFunc_t      getRSSIFunc;
        TUNERi2cWriteReadFunc_t i2cWriteReadFunc;
    } TUNERFunc_t,
    *pTUNERFunc_t;

    typedef struct TUNERInstance_s
    {
        I2CDeviceAddr_t     myI2CDevAddr;
        pTUNERCommonAttr_t  myCommonAttr;     /* should not be initialized by application  */
        void                *myExtAttr;
        pTUNERFunc_t        myFunct;
    } TUNERInstance_t;


    /*------------------------------------------------------------------------------
    ENUM
    ------------------------------------------------------------------------------*/

    /*------------------------------------------------------------------------------
    STRUCTS
    ------------------------------------------------------------------------------*/



    /*------------------------------------------------------------------------------
    THE END
    ------------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
#endif   /* __DD_TUNER_H__ */

/* End of file */
