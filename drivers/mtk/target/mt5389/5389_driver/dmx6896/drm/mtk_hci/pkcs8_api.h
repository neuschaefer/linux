/** @addtogroup pkcs8 pkcs8
 *  @ingroup common
 * @{ */

/*
 * Nautilus version 3.2
 *
 * Any code and mechanism in this module may not be used
 * in any form without permissions.
 * Copyright (C) 2004,2005,2006,2007,2008,2009 Sony Corporation, All Rights Reserved.
 *
 * $Id: //DTV/MP_BR/DTV_X_IDTV0801_002150_7_001_11_001/project_x/target/mt53xx_com/53xx_com_driver/dmx6896/drm/mtk_hci/pkcs8_api.h#1 $
 */

#ifndef PKCS8_API_H
#define PKCS8_API_H

/** @file
 * PKCS8 Key structure definition
 *
 * @require common/asn
 * @require platform/base/binstr
 * @require platform/base/mem
 * @author Takayuki TSUZUKI <tsutaka@sm.sony.co.jp>
 * @author SAKAMOTO Takahiro <takas@sm.sony.co.jp>
 */

NP_BEGIN_EXTERN_C

/**
 * PKCS8 Error codes
 */
#define ERR_PKCS8_ILLEGALFORMAT (ERR_PKCS8_BASE|1) /** Illegal Pkcs8 key format */

/**
 * PKCS8 Key Structure
 */
typedef struct {
    /** Clone or not */
    int flag;
    /** modulus */
    binstr_t n;
    /** publicExponent */
    binstr_t e;
    /** privateExponent */
    binstr_t d;
    /** prime1 */
    binstr_t p;
    /** prime2 */
    binstr_t q;
    /** exponent1 d mod (p-1) */
    binstr_t dp;
    /** exponent2 d mod (q-1) */
    binstr_t dq;
    /** coefficient (inverse of q) mod p*/
    binstr_t qinv;
} pkcs8_key_t;

/*---------------------------------------------------------------------------*
 * APIs
 *---------------------------------------------------------------------------*/

/**
 * @brief Parse an ASN.1 encoded PKCS8 key 
 * 
 * @note out_key must be freed with pkcs8_FreeKey() function
 *
 * @param [in]  in_pkcs8                PKCS8 key 
 * @param [out] out_key                 PKCS8 key structure
 *
 * @return ::ERR_OK                       
 * @return ::ERR_ILLEGALARGS              
 * @return ::ERR_NOMEM                    
 * @return ::ERR_PKCS8_ILLEGALFORMAT      
 */
extern retcode_t
pkcs8_GetKey(binstr_t *in_pkcs8, pkcs8_key_t **out_key);

/**
 * @brief Parse and clone an ASN.1 encoded PKCS8 key 
 *
 * @note out_key must be freed with pkcs8_FreeKey() function
 *
 * @param [in]  in_pkcs8                PKCS8 key
 * @param [out] out_key                 PKCS8 key structure
 *
 * @return ::ERR_OK                       
 * @return ::ERR_ILLEGALARGS              
 * @return ::ERR_NOMEM                    
 * @return ::ERR_PKCS8_ILLEGALFORMAT      
 */
extern retcode_t
pkcs8_GetCloneKey(binstr_t *in_pkcs8, pkcs8_key_t **out_key);

/**
 *
 * @brief Free a PKCS8 key structure
 *
 * @note io_key is not set to NULL by this function
 * 
 * @param [in,out] io_key               PKCS8 key structure
 * @return ::ERR_OK                       
 */
extern retcode_t
pkcs8_FreeKey(pkcs8_key_t *io_key);

NP_END_EXTERN_C

#endif /* PKCS8_API_H */
/** @} */
