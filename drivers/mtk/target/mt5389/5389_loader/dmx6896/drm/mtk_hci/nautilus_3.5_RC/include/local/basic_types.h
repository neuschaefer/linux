/*
 * Nautilus version 3.5
 *
 * Any code and mechanism in this module may not be used
 * in any form without permissions.
 * Copyright (C) 2004,2005,2006,2007,2008,2009,2010,2011 Sony Corporation, All Rights Reserved.
 *
 */

#ifndef BASIC_TYPES_H
#define BASIC_TYPES_H

/*===========================================================================*/
/** @file
 * Basic types and macro definitions for error handling and endian-ness
 *
 * @reqire none
 * @author Norifumi Goto <n-goto@sm.sony.co.jp>
 */
/*===========================================================================*/

#include <np_basictypes.h>
#include <basic_types_.h>

NP_BEGIN_EXTERN_C

/**
 * def: TRUE, FALSE
 */

#ifndef TRUE
#  define TRUE 1
#endif
#ifndef FALSE
#  define FALSE 0
#endif

/**
 * def: Context for Common-library
 */
#define NPI_CMN_CONTEXT_SECURE		0
#define NPI_CMN_CONTEXT_NONSECURE	1

typedef int32_t		npi_cmncontext_t;

/**
 * def: error code
 */

typedef np_ret_t		retcode_t;

/*===========================================================================
 * ERROR MAP
 *
 *     3322 2222 2222 1111 1111 1100 0000 0000
 *     1098 7654 3210 9876 5432 1098 7654 3210
 *    |    |    |         |
 *     A   B    C         D
 *
 *    A: Layer          (4bit  : 0 - 15)
 *    B: Module         (4bit  : 0 - 15)
 *    C: Component      (8bit  : 0 - 255)
 *    D: Error          (16bit : 0 - 65535)
 *==========================================================================*/

/*--------- SHIFT SIZE ------------------------------------------------------*/
#define ERR_LYR_SFT		28
#define ERR_MOD_SFT		24
#define ERR_CMP_SFT		16

/*--------- ERROR MASK ------------------------------------------------------*/
#define ERR_LYR_MASK		0xF0000000
#define ERR_MOD_MASK		0x0F000000
#define ERR_CMP_MASK		0x00FF0000
#define ERR_BASE_MASK		0xFFFF0000

#define ERR_LYR(err)		((err) & ERR_LYR_MASK)
#define ERR_MOD(err)		((err) & ERR_MOD_MASK)
#define ERR_BASE(err)		((err) & ERR_BASE_MASK)

/*--------- ERROR LAYER -----------------------------------------------------*/
#define ERR_LYR_BASIC		(0<<ERR_LYR_SFT)
#define ERR_LYR_CMN		(1<<ERR_LYR_SFT)
#define ERR_LYR_SEC		(2<<ERR_LYR_SFT)
#define ERR_LYR_NSEC		(3<<ERR_LYR_SFT)

/*--------- ERROR MODULE ----------------------------------------------------*/
/*----- GENERIC -----*/
#define ERR_MOD_NONE		(0<<ERR_MOD_SFT)
/*----- CMN -----*/
#define ERR_MOD_BASE		(1<<ERR_MOD_SFT)
/*----- SEC -----*/
#define ERR_MOD_COMMON		(2<<ERR_MOD_SFT)
#define ERR_MOD_MARLIN		(3<<ERR_MOD_SFT)
#define ERR_MOD_API		(4<<ERR_MOD_SFT)
/*----- NSEC -----*/
/* There are no errors defined for this layer */

/*--------- ERROR BASE -----------------------------------------------------*/
/*
 * Basic
 */
#define ERR_BASIC_BASE		(ERR_LYR_BASIC)

/*
 * cmn/Base
 */
#define ERR_BASE_BASE		(ERR_LYR_CMN|ERR_MOD_BASE|0<<ERR_CMP_SFT)
#define ERR_BINSTR_BASE		(ERR_LYR_CMN|ERR_MOD_BASE|1<<ERR_CMP_SFT)
#define ERR_BYTE_BASE		(ERR_LYR_CMN|ERR_MOD_BASE|2<<ERR_CMP_SFT)
#define ERR_CONVERT_BASE	(ERR_LYR_CMN|ERR_MOD_BASE|3<<ERR_CMP_SFT)
#define ERR_DBG_BASE		(ERR_LYR_CMN|ERR_MOD_BASE|4<<ERR_CMP_SFT)
#define ERR_DEVICE_BASE		(ERR_LYR_CMN|ERR_MOD_BASE|5<<ERR_CMP_SFT)
#define ERR_FIO_BASE		(ERR_LYR_CMN|ERR_MOD_BASE|6<<ERR_CMP_SFT)
#define ERR_MEM_BASE		(ERR_LYR_CMN|ERR_MOD_BASE|7<<ERR_CMP_SFT)
#define ERR_MUTEX_BASE		(ERR_LYR_CMN|ERR_MOD_BASE|8<<ERR_CMP_SFT)
#define ERR_PROF_BASE		(ERR_LYR_CMN|ERR_MOD_BASE|9<<ERR_CMP_SFT)
#define ERR_STR_BASE		(ERR_LYR_CMN|ERR_MOD_BASE|11<<ERR_CMP_SFT)
#define ERR_TIME_BASE		(ERR_LYR_CMN|ERR_MOD_BASE|12<<ERR_CMP_SFT)
#define ERR_VLBINSTR_BASE	(ERR_LYR_CMN|ERR_MOD_BASE|13<<ERR_CMP_SFT)

/*
 * cmn/None
 */
#define ERR_B64_BASE		(ERR_LYR_CMN|ERR_MOD_NONE|0<<ERR_CMP_SFT)
#define ERR_XML_BASE		(ERR_LYR_CMN|ERR_MOD_NONE|1<<ERR_CMP_SFT)

/*
 * sec/common
 */
#define ERR_ASN_BASE		(ERR_LYR_SEC|ERR_MOD_COMMON|0<<ERR_CMP_SFT)
#define ERR_CERT_BASE		(ERR_LYR_SEC|ERR_MOD_COMMON|1<<ERR_CMP_SFT)
#define ERR_AES_BASE		(ERR_LYR_SEC|ERR_MOD_COMMON|2<<ERR_CMP_SFT)
#define ERR_CRYPTO_BASE		(ERR_LYR_SEC|ERR_MOD_COMMON|3<<ERR_CMP_SFT)
#define ERR_HMAC_BASE		(ERR_LYR_SEC|ERR_MOD_COMMON|4<<ERR_CMP_SFT)
#define ERR_RNG_BASE		(ERR_LYR_SEC|ERR_MOD_COMMON|5<<ERR_CMP_SFT)
#define ERR_RSA_BASE		(ERR_LYR_SEC|ERR_MOD_COMMON|6<<ERR_CMP_SFT)
#define ERR_SHA_BASE		(ERR_LYR_SEC|ERR_MOD_COMMON|7<<ERR_CMP_SFT)
#define ERR_RAND_BASE		(ERR_LYR_SEC|ERR_MOD_COMMON|8<<ERR_CMP_SFT)
#define ERR_PKCS8_BASE		(ERR_LYR_SEC|ERR_MOD_COMMON|9<<ERR_CMP_SFT)
#define ERR_XMLS_BASE		(ERR_LYR_SEC|ERR_MOD_COMMON|10<<ERR_CMP_SFT)
#define ERR_TMCXT_BASE		(ERR_LYR_SEC|ERR_MOD_COMMON|11<<ERR_CMP_SFT)
#define ERR_SW_BASE		(ERR_LYR_SEC|ERR_MOD_COMMON|12<<ERR_CMP_SFT)

/*
 * sec/marlin
 */
#define ERR_BB_BASE	        (ERR_LYR_SEC|ERR_MOD_MARLIN|0<<ERR_CMP_SFT)
#define ERR_DECR_BASE		(ERR_LYR_SEC|ERR_MOD_MARLIN|1<<ERR_CMP_SFT)
#define ERR_DRM_BASE		(ERR_LYR_SEC|ERR_MOD_MARLIN|2<<ERR_CMP_SFT)
#define ERR_HN_BASE		    (ERR_LYR_SEC|ERR_MOD_MARLIN|3<<ERR_CMP_SFT)
#define ERR_KEYMNG_BASE		(ERR_LYR_SEC|ERR_MOD_MARLIN|4<<ERR_CMP_SFT)
#define ERR_NEMO_BASE		(ERR_LYR_SEC|ERR_MOD_MARLIN|5<<ERR_CMP_SFT)
#define ERR_OCTCXT_BASE		(ERR_LYR_SEC|ERR_MOD_MARLIN|6<<ERR_CMP_SFT)
#define ERR_OCTOBJ_BASE		(ERR_LYR_SEC|ERR_MOD_MARLIN|7<<ERR_CMP_SFT)
#define ERR_PERS_BASE	    (ERR_LYR_SEC|ERR_MOD_MARLIN|8<<ERR_CMP_SFT)
#define ERR_PKT_BASE		(ERR_LYR_SEC|ERR_MOD_MARLIN|9<<ERR_CMP_SFT)
#define ERR_REG_BASE		(ERR_LYR_SEC|ERR_MOD_MARLIN|10<<ERR_CMP_SFT)
#define ERR_SEASHELL_BASE	(ERR_LYR_SEC|ERR_MOD_MARLIN|11<<ERR_CMP_SFT)
#define ERR_SSMEM_BASE		(ERR_LYR_SEC|ERR_MOD_MARLIN|12<<ERR_CMP_SFT)
#define ERR_SF_BASE	        (ERR_LYR_SEC|ERR_MOD_MARLIN|13<<ERR_CMP_SFT)
#define ERR_TT_BASE		    (ERR_LYR_SEC|ERR_MOD_MARLIN|14<<ERR_CMP_SFT)
#define ERR_XML2OCT_BASE	(ERR_LYR_SEC|ERR_MOD_MARLIN|15<<ERR_CMP_SFT)
#define ERR_LTP_BASE        (ERR_LYR_SEC|ERR_MOD_MARLIN|16<<ERR_CMP_SFT)
#define ERR_HCI_BASE        (ERR_LYR_SEC|ERR_MOD_MARLIN|17<<ERR_CMP_SFT)
#define ERR_BBTS_BASE       (ERR_LYR_SEC|ERR_MOD_MARLIN|18<<ERR_CMP_SFT)
#define ERR_MS3_BASE        (ERR_LYR_SEC|ERR_MOD_MARLIN|19<<ERR_CMP_SFT)

/*
 * sec/api
 */
/* There are no errors defined here for this component */

/*
 * nsec/None
 */
#define ERR_ESBMAN_BASE		(ERR_LYR_NSEC|ERR_MOD_NONE|0<<ERR_CMP_SFT)
#define ERR_SSTORE_BASE		(ERR_LYR_NSEC|ERR_MOD_NONE|1<<ERR_CMP_SFT)


/*--------- ERROR BASIC ----------------------------------------------------*/

#define ERR_OK 0

#define ERR_NG			(ERR_BASIC_BASE|1) /* 1 */
#define ERR_NOMEM		(ERR_BASIC_BASE|2)
#define ERR_ILLEGALARGS		(ERR_BASIC_BASE|3)
#define ERR_ILLEGALHANDLE	(ERR_BASIC_BASE|4)
#define ERR_INTERNALERROR	(ERR_BASIC_BASE|5)
#define ERR_CORRUPTED	        (ERR_BASIC_BASE|6)
#define ERR_NOTACTIVE	        (ERR_BASIC_BASE|7)

/*---------------------------------------------------------------------------*/
/**
 * @brief basic integer processing macros
 *
 * @verbatim
 * MAX(a,b): compare a with b and return the greater one
 * MIN(a,b): compare a with b and return the less one
 * TRUNC(v,a): maximum value of multiples of a which is less than or equals to v
 * ALIGN(v,a): minimum value of multiples of a which is greater than or equals to v
 * @endverbatim
 * Note that each a and b should be not expressions but a single variable
 *  because a and b may be evaluated twice.
 */
/*---------------------------------------------------------------------------*/

#ifdef CC_SECURE_WORLD
#define MAX(a,b)		(((a)>=(b))?(a):(b))
#define MIN(a,b)		(((a)<=(b))?(a):(b))
#define TRUNC(v,a)		((v)/(a)*(a))
#define ALIGN(v,a)		(((v)+((a)-1))/(a)*(a))
#endif

/*---------------------------------------------------------------------------*/
/**
 * @brief macros for error handling
 *
 * @verbatim
 * usage:
 * retcode_t foo(int32_t x)
 * {
 *   retcode_t ret; // you should declare the variable `ret' as retcode_t.
 *   ...
 *   RET(bar(x+1));
 *   ...
 *   RETR(bar(x+2), error_conversion(ret));
 *   ...
 *   RETX(bar(x+3), MSG1(("error: failed in bar(%d)\n", x+3)));
 *   ...
 *  EXIT: // you should put the label `EXIT' at the beggining of epilogue code.
 *   ... (epilogue)
 *   return ret;
 * }
 * @endverbatim
 */
/*---------------------------------------------------------------------------*/
#if defined(NS_DEBUG)
#define RET(f)		do { if ( (ret=(f)) != ERR_OK ) { dbg_l_WriteLog(ret,     #f, __FILE__, __FUNCTION__, __LINE__);    goto EXIT; } } while(0)
#define RETR(f,v)	do { if ( (ret=(f)) != ERR_OK ) { dbg_l_WriteLog(ret=(v), #v, __FILE__, __FUNCTION__, __LINE__);    goto EXIT; } } while(0)
#define RETX(f,x)	do { if ( (ret=(f)) != ERR_OK ) { dbg_l_WriteLog(ret,     #f, __FILE__, __FUNCTION__, __LINE__); x; goto EXIT; } } while(0)
#define RETRX(f,v,x)	do { if ( (ret=(f)) != ERR_OK ) { dbg_l_WriteLog(ret=(v), #v, __FILE__, __FUNCTION__, __LINE__); x; goto EXIT; } } while(0)
#else
#define RET(f)		do { if ( (ret=(f)) != ERR_OK ) {               goto EXIT; } } while (0)
#define RETR(f,v)	do { if ( (ret=(f)) != ERR_OK ) { ret = (v);    goto EXIT; } } while (0)
#define RETX(f,x)	do { if ( (ret=(f)) != ERR_OK ) {            x; goto EXIT; } } while (0)
#define RETRX(f,v,x)	do { if ( (ret=(f)) != ERR_OK ) { ret = (v); x; goto EXIT; } } while (0)
#endif

#define CONVR(r)  ((ERR_BASE(ret)==ERR_BASIC_BASE)?(ret):(r))


/*---------------------------------------------------------------------------*/
/**
 * @brief Definitions of Domain ID structures
 *
 * Given a Subscription ID and Account ID the Domain ID is of the form
 *
 *   [ SHA1(SID)[0:9] || PADDING || AID ]
 *
 * The SubscriptionID is hashed a truncated to 10 bytes, padding is two
 * NULL bytes, Account ID is 8 bytes. The Domain ID is a fixed 20 bytes.
 */
#define DOMAINID_SUBSFIELD_SIZE    10  /**< Subscription ID size in domain ID */
#define DOMAINID_PADDING_SIZE       2  /**< Padding size in domain ID */
#define DOMAINID_ACCFIELD_SIZE      8  /**< Account ID size in domain ID */

#define DOMAINID_SIZE ((DOMAINID_SUBSFIELD_SIZE)+(DOMAINID_PADDING_SIZE)+(DOMAINID_ACCFIELD_SIZE))


NP_END_EXTERN_C

#endif /* BASIC_TYPES_H */
