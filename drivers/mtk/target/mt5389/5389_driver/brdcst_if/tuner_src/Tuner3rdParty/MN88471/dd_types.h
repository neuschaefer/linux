/*==========================================================================================*/
/*  Panasonic System LSI Design Europe, a Division of Panasonic Europe Ltd.					*/
/*                                                                                          */
/*  Module Name:            dd_types.h														*/
/*  Author (Nationality):   Luke Grove (British)	                                        */
/*  Description:            Types definitions for MN Driver								    */
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
/******************************************************************************
* %Id: by PSL $
*
* FileName   :dd_types.h
*
* Description  : types definition. Also defines DEBUG_PRINT Macro
*
*
* Copyright  :PSL/SC-SSDC
*
*****************************************************************************/
#ifndef __DD_TYPES_H__
#define __DD_TYPES_H__
/*-------------------------------------------------------------------------
INCLUDES
-------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C"
{
#endif


    /*-------------------------------------------------------------------------
    TYPEDEFS
    -------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------- */
#define CODE_RELEASE 170	/* V1.70: 06-10-2010 */
/*Frequencies to be scanned */
#define	ONLY_7_8_MHZ		/* Limit tune and scan to 7&8 MHz only - Europe */

/* Floating-point choice */
#define NO_FLOAT_PROC       /* Uncomment if there is no floating-point processor */
//#define FLOAT_COMPARE		/* Uncomment to give both floating-point and non-fp calc results */

//#define DEBUG_PRINTING         /*uncomment to enable debug message*/

/* The remaining code below is dependent on the above 'defines'. */

#ifndef NO_FLOAT_PROC
#undef FLOAT_COMPARE
#endif

/* End of Floating-point choice */
/*---------------------------------------------------------------------------------------------- */



    /**
    * \typedef unsigned char u8_t
    * \brief type definition of an unsigned 8 bits integer
    */
    typedef unsigned char   u8_t;
    /**
    * \typedef char s8_t
    * \brief type definition of a signed 8 bits integer
    */
    typedef char            s8_t;
    /**
    * \typedef unsigned short u16_t *pu16_t
    * \brief type definition of an unsigned 16 bits integer
    */
    typedef unsigned short  u16_t;
    /**
    * \typedef short s16_t
    * \brief type definition of a signed 16 bits integer
    */
    typedef short           s16_t;
    /**
    * \typedef unsigned long u32_t
    * \brief type definition of an unsigned 32 bits integer
    */
    typedef unsigned long   u32_t;
    /**
    * \typedef long s32_t
    * \brief type definition of a signed 32 bits integer
    */
    typedef long            s32_t;
    /**
    * \typedef float float32_t
    * \brief type definition of a 32 bit float
    */
    typedef float           float32_t;
    /**
    * \typedef double float64_t
    * \brief type definition of a 64 bit float
    */
    typedef double			float64_t;
    /**
    * \typedef long double float128_t
    * \brief type definition of a 64 bit float
    */
    /*
    * \typedef struct ... u64_t
    * \brief type definition of an usigned 64 bits integer
    */
    typedef struct
    {
        u32_t   MSLW;
        u32_t   LSLW;
    } u64_t;
    /*
    * \typedef struct ... i64_t
    * \brief type definition of a signed 64 bits integer
    */
    typedef struct
    {
        s32_t                   MSLW;
        u32_t                   LSLW;
    } s64_t;

    /**
    * \typedef u8_t *pu8_t
    * \brief type definition of pointer to an unsigned 8 bits integer
    */
    typedef u8_t            *pu8_t;
    /**
    * \typedef s8_t *ps8_t
    * \brief type definition of pointer to a signed 8 bits integer
    */
    typedef s8_t            *ps8_t;
    /**
    * \typedef u16_t *pu16_t
    * \brief type definition of pointer to an unsigned 16 bits integer
    */
    typedef u16_t           *pu16_t;
    /**
    * \typedef s16_t *ps16_t
    * \brief type definition of pointer to a signed 16 bits integer
    */
    typedef s16_t           *ps16_t;
    /**
    * \typedef u32_t *pu32_t
    * \brief type definition of pointer to an unsigned 32 bits integer
    */
    typedef u32_t           *pu32_t;
    /**
    * \typedef s32_t *ps32_t
    * \brief type definition of pointer to a signed 32 bits integer
    */
    typedef s32_t           *ps32_t;
    /**
    * \typedef u64_t *pu64_t
    * \brief type definition of pointer to an usigned 64 bits integer
    */
    typedef u64_t           *pu64_t;
    /**
    * \typedef s64_t *ps64_t
    * \brief type definition of pointer to a signed 64 bits integer
    */
    typedef s64_t           *ps64_t;


    /**
    * \typedef s32_t MNFrequency_t
    * \brief type definition of frequency
    */
    typedef s32_t           Frequency_t;

    /**
    * \typedef u32_t pMNFrequency_t
    * \brief type definition of a pointer to a frequency
    */
    typedef Frequency_t   *pFrequency_t;

    typedef u8_t   Plp_id_t;           /**< PLP ID              */

    /**
    * \typedef u32_t pMNFrequency_t
    * \brief type definition of a pointer to a symbol rate
    */
    typedef enum
    {
        RECEIVER_MODE_C              = 0,
        /**< DVB-C       */
        RECEIVER_MODE_T,
        /**< DVB-T       */ 
        RECEIVER_MODE_T2,
        /*DVB T2*/
        RECEIVER_MODE_UNKNOWN
        /*Unknown*/
    } ReceiverMode_t, *pReceiverMode_t;


    /*-------------------------------------------------------------------------
    DEFINES
    -------------------------------------------------------------------------*/
    /**
    * \def NULL
    * \brief Define NULL for target.
    */
#ifndef NULL
#define NULL            (0)
#endif

#if 0
#ifdef DEBUG_PRINTING
    extern void DemodDebugPrint ( s8_t *format, ... );
#define TNR_DEBUG  DemodDebugPrint
#else
#define TNR_DEBUG  {}
#endif
#endif
    /*-------------------------------------------------------------------------
    ENUM
    -------------------------------------------------------------------------*/

    /*
    * Boolean datatype. Only define if not already defined TRUE or FALSE.
    */
//#if defined (TRUE) || defined (FALSE)
    typedef int             Bool_t;
//#else
    /**
    * \enum Bool_t
    * \brief Boolean type
    */
 //   typedef enum
 //   {
//        FALSE           = 0,
 //       TRUE
 //   }                                                             Bool_t;
//#endif
    typedef Bool_t  *pBool_t;

    /**
    * \enum MNStatus_t
    * \brief Various return statusses
    */
    typedef enum
    {
        MN_STS_READY                                                = 3,
        /**< indicating that device/service is ready */
        MN_STS_BUSY                                                 = 2,
        /**< indicating that device/service is busy */
        MN_STS_OK                                                   = 1,
        MN_STS_FAIL													= 0,
        /**< indicating that everything is OK */
        MN_STS_INVALID_ARG                                          = -1,
        /**< indicating invalid arguments */
        MN_STS_ERROR                                                = -2,
        /**< indicating general error */
        MN_STS_FUNC_NOT_AVAILABLE                                   = -3        /**< indicating unavaiable functionality */
    }        MNStatus_t,
    *pMNStatus_t;

    typedef enum
    {
        FLOAT_OK	= 0,
        DECIMALS_ERR,		/* Error with the decimal precision value */
        NUMERATOR_ERR,
        DENOM_ERR
    }        MNFloatStatus_t,
    *pMNFloatStatus_t;

    typedef enum
    {
        POSITIVE	= 0,
        NEGATIVE
    }        MNSign_t;


    /*-------------------------------------------------------------------------
    STRUCTS
    -------------------------------------------------------------------------*/
    /* Floating-point number representation for use with processors without a fp processor */

    typedef struct
    {
        MNSign_t	sign;	/* Shared between binary and exponential results */
        /* Binary result: result = [sign] significand >> precision */
        u32_t	significand;	/*This number needs to be >> "precision" to get integer portion */
        u8_t	precision;
        /* Decimal result: result = [sign] (integer + (decimals*10^-exponent)) */
        u32_t	integer;
        u32_t	decimals;
        u8_t	exponent;
    } MNfloat_t,
    *pMNfloat_t;

#ifdef NO_FLOAT_PROC
    typedef MNfloat_t           MNdouble_t;
#else
    typedef float64_t           MNdouble_t;
#endif

    /**
    * \typedef float128_t    Symbol_rate_t
    * \brief type definition of DVB-C symbol rate
    */
#ifdef NO_FLOAT_PROC
    typedef MNfloat_t           Symbol_rate_t;
#else
    typedef float64_t           Symbol_rate_t;
#endif

    /**
    * \typedef u32_t pFrequency_t
    * \brief type definition of a pointer to a symbol rate
    */
    typedef Symbol_rate_t   *pSymbol_rate_t;


    /**
    Exported FUNCTIONS
    -------------------------------------------------------------------------*/

    /*-------------------------------------------------------------------------
    THE END
    -------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
#endif /* __DD_TYPES_H__ */

/*============================================================================================
    Revision History

    $Log: dd_types.h,v $
    Revision 1.1  2010/09/16 17:30:37  bells
    Initial check-in to PSDE CVS repository

==============================================================================================*/
