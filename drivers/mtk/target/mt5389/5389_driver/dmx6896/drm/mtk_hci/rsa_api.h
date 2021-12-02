/** @addtogroup crypto crypto
 *  @ingroup common
 * @{ */

/*
 * Nautilus version 3.2
 *
 * Any code and mechanism in this module may not be used
 * in any form without permissions.
 * Copyright (C) 2004,2005,2006,2007,2008,2009 Sony Corporation, All Rights Reserved.
 */

#ifndef RSA_API_H
#define RSA_API_H

#ifdef __cplusplus
extern "C" {
#endif

/*========================================================================*/
/** @file
 *  @brief  RSA - API definitions
 *
 *  @verbatim
 *  dependency: base, mem, binstr
 *  @endverbatim
 *
 *  @author Yoshizumi Tanaka (y-tanaka@sm.sony.co.jp)
 *  @date   2 Dec., 2004 modified by y-tanaka
 */
/*========================================================================*/
/*------------------------------------------------------------------------*
 * type definition
 *------------------------------------------------------------------------*/
typedef void *	rsa_handle_t;

typedef struct rsa_key {
    binstr_t    *n;     /** modulus                             */
    binstr_t    *e;     /** publicExponent                      */
    binstr_t    *d;     /** privateExponent                     */
    binstr_t    *p;     /** prime1                              */
    binstr_t    *q;     /** prime2                              */
    binstr_t    *dP;    /** exponent1 d mod (p-1)               */
    binstr_t    *dQ;    /** exponent2 d mod (q-1)               */
    binstr_t    *qInv;  /** coefficient (inverse of q) mod p    */
} rsa_key_t;

/*------------------------------------------------------------------------*
 * error definition
 *------------------------------------------------------------------------*/

#define ERR_RSA_OUT_OF_RANGE            (ERR_RSA_BASE|1)
#define ERR_RSA_INVALID                 (ERR_RSA_BASE|2)
#define ERR_RSA_INVALID_SIGNATURE       (ERR_RSA_BASE|3)
#define ERR_RSA_MSG_TOO_LONG            (ERR_RSA_BASE|4)
#define ERR_RSA_MASK_TOO_LONG           (ERR_RSA_BASE|5)
#define ERR_RSA_EMSG_TOO_SHORT          (ERR_RSA_BASE|6)
#define ERR_RSA_INTEGER_TOO_LONG        (ERR_RSA_BASE|7)
#define ERR_RSA_INTEGER_TOO_LARGE       (ERR_RSA_BASE|8)
#define ERR_RSA_MOD_TOO_SHORT           (ERR_RSA_BASE|9)
#define ERR_RSA_DECRYPTION              (ERR_RSA_BASE|10)
#define ERR_RSA_DECODING                (ERR_RSA_BASE|11)
#define ERR_RSA_OTHERS                  (ERR_RSA_BASE|12)
#define ERR_RSA_NOTSUPPORTED            (ERR_RSA_BASE|13)

/*------------------------------------------------------------------------*
 * macro definition
 *------------------------------------------------------------------------*/

/**
 * @brief digest algorithm definitions
 */
#define RSA_DIGESTALGO_SHA1   0
#define RSA_DIGESTALGO_SHA256 1

/*========================================================================*/
/**
 * @brief  Initialize RSA context
 *
 * @param[in,out] io_rsa rsa handle
 * @param[in] in_n 
 * @param[in] in_e 
 * @param[in] in_d 
 * @param[in] in_p 
 * @param[in] in_q 
 * @param[in] in_dP 
 * @param[in] in_dQ 
 * @param[in] in_dInv 
 * @return ::ERR_OK
 * @return ::ERR_ILLEGALARGS
 * @return ::ERR_RSA_NOMEM
 * @return ::ERR_INTERNALERROR
 * @return ::ERR_CRYPTO_CRYPTO
 */
/*========================================================================*/
extern retcode_t
rsa_Init(rsa_handle_t   *io_rsa,
         const binstr_t *in_n,
         const binstr_t *in_e,
         const binstr_t *in_d,
         const binstr_t *in_p,
         const binstr_t *in_q,
         const binstr_t *in_dP,
         const binstr_t *in_dQ,
         const binstr_t *in_qInv);

/*========================================================================*/
/**
 * @brief  Finalize RSA context
 *
 * @param[in,out] io_rsa rsa handle
 * @return ::ERR_OK
 */
/*========================================================================*/
extern retcode_t
rsa_Fin(rsa_handle_t io_rsa);

/*========================================================================*/
/**
 * @brief Create and set the structure of an RSA public key object
 *
 * @param[out]      out_key     Populated key
 * @param[in]       in_n        Modulus
 * @param[in]       in_e        Public Exponent
 *
 * @return ::ERR_OK, ::ERR_NOMEM
 */
/*========================================================================*/
extern retcode_t
rsa_CreatePubKey(
    rsa_key_t   **out_key,
    binstr_t     *in_n,
    binstr_t     *in_e);

/*========================================================================*/
/**
 * @brief Create and set the structure of an RSA private key object
 *
 *
 * @param[out]      out_key     Populated key
 * @param[in]       in_n        Modulus
 * @param[in]       in_e        Public Exponent
 * @param[in]       in_d        Private Exponent
 * @param[in]       in_p        Prime 1
 * @param[in]       in_q        Prime 2
 * @param[in]       in_dP       Exponent1 d mod (p-1)
 * @param[in]       in_dQ       Exponent2 d mod (q-1)
 * @param[in]       in_dInv     Coefficient (inverse of q) mod p
 *
 * @return ::ERR_OK, ::ERR_NOMEM
 */
/*========================================================================*/
extern retcode_t
rsa_CreatePrivKey(
    rsa_key_t   **out_key,
    binstr_t     *in_n,
    binstr_t     *in_e,
    binstr_t     *in_d,
    binstr_t     *in_p,
    binstr_t     *in_q,
    binstr_t     *in_dP,
    binstr_t     *in_dQ,
    binstr_t     *in_qInv);

/*========================================================================*/
/**
 * Free the resources of an allocated key object
 *
 * @param[in,out] io_key Pointer to key object
 * @return ::ERR_OK
 */
/*========================================================================*/
extern retcode_t
rsa_FreeKey(rsa_key_t *io_key);

/*========================================================================*/
/**
 * @brief  RSA OAEP Encryption
 *
 * @param[in,out] io_rsa  rsa handle
 * @param[in] in_M  message
 * @param[in] in_P  encoding parameter
 * @param[out] out_C cipher text
 * @return ::ERR_OK
 * @return ::ERR_NOMEM
 * @return ::ERR_ILLEGALARGS
 * @return ::ERR_INTERNALERROR
 * @return ::ERR_CRYPTO_CRYPTO
 */
/*========================================================================*/
extern retcode_t 
rsa_RsaesOaepEncrypt(rsa_handle_t     io_rsa,
                     const binstr_t  *in_M,
                     const binstr_t  *in_P,
                     binstr_t       **out_C);

/*========================================================================*/
/**
 * @brief  RSA OAES Decryption
 *
 * @param[in,out] io_rsa  rsa handle
 * @param[in] in_C  cipher text
 * @param[in] in_P  encoding parameter
 * @param[out] out_M message
 * @return ::ERR_OK
 * @return ::ERR_NOMEM
 * @return ::ERR_ILLEGALARGS
 * @return ::ERR_INTERNALERROR
 * @return ::ERR_CRYPTO_CRYPTO
 */
/*========================================================================*/
extern retcode_t
rsa_RsaesOaepDecrypt(rsa_handle_t     io_rsa,
                     const binstr_t  *in_C,
                     const binstr_t  *in_P,
                     binstr_t       **out_M);

/*========================================================================*/
/**
 * @brief  RSAES PKCS1 V1.5 Encryption
 *
 *
 * @param[in,out] io_rsa  rsa handle
 * @param[in] in_M  message
 * @param[out] out_C cipher text
 * @return ::ERR_OK
 * @return ::ERR_NOMEM
 * @return ::ERR_ILLEGALARGS
 * @return ::ERR_INTERNALERROR
 * @return ::ERR_CRYPTO_CRYPTO
 * @return ::ERR_RSA_NOTSUPPORTED
 */
/*========================================================================*/
extern retcode_t
rsa_RsaesPkcs1V1_5Encrypt(rsa_handle_t     io_rsa,
                          const binstr_t  *in_M,
                          binstr_t       **out_C);

/*========================================================================*/
/**
 * @brief  RSAES PKCS1 V1.5 Decryption
 *
 *
 * @param[in,out] io_rsa  rsa handle
 * @param[in] in_C  cipher text
 * @param[out] out_M message
 * @return ::ERR_OK
 * @return ::ERR_NOMEM
 * @return ::ERR_ILLEGALARGS
 * @return ::ERR_INTERNALERROR
 * @return ::ERR_CRYPTO_CRYPTO
 * @return ::ERR_RSA_NOTSUPPORTED
 */
/*========================================================================*/
extern retcode_t
rsa_RsaesPkcs1V1_5Decrypt(rsa_handle_t     io_rsa,
                          const binstr_t  *in_C,
                          binstr_t       **out_M);

/*========================================================================*/
/**
 * @brief  RSASSA PKCS1 V1.5 Signature
 *
 * @param[in,out] io_rsa  rsa handle
 * @param[in] in_dalgo  digest algorithm
 * @param[in] in_M  message
 * @param[out] out_S signature
 *
 * @return ::ERR_OK
 * @return ::ERR_RSA_NOTSUPPORTED
 * @return ::ERR_ILLEGALARGS
 * @return ::ERR_NOMEM
 * @return ::ERR_CRYPTO_CRYPTO
 */
/*========================================================================*/
extern retcode_t
rsa_RsassaPkcs1V1_5Sign(rsa_handle_t     io_rsa,
                        int              in_dalgo,
                        const binstr_t  *in_M,
                        binstr_t       **out_S);

/*========================================================================*/
/**
 * @brief  RSASSA PKCS1 V1.5 Verification
 *
 * @param[in,out] io_rsa rsa handle
 * @param[in] in_dalgo digest algorithm
 * @param[in] in_M message
 * @param[in] in_S signature
 * @return ::ERR_OK
 * @return ::ERR_NOMEM
 * @return ::ERR_ILLEGALARGS
 * @return ::ERR_INTERNALERROR
 * @return ::ERR_CRYPTO_CRYPTO
 * @return ::ERR_RSA_INVALID_SIGNATURE
 */
/*========================================================================*/
extern retcode_t
rsa_RsassaPkcs1V1_5Verify(rsa_handle_t    io_rsa,
                          int             in_dalgo,
                          const binstr_t *in_M,
                          const binstr_t *in_S);

#ifdef __cplusplus
}
#endif

#endif /* RSA_API_H */
/** @} */
