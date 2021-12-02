/** @addtogroup crypto crypto
 *  @ingroup common
 * @{ */

/*
 * Nautilus version 3.2
 *
 * Any code and mechanism in this module may not be used
 * in any form without permissions.
 * Copyright (C) 2008,2009 Sony Corporation, All Rights Reserved.
 */

#ifndef SHA256_API_H
#define SHA256_API_H

NP_BEGIN_EXTERN_C

/*========================================================================*/
/** @file
 *  @brief  SHA-256 library - API definitions
 *
 *  @verbatim
 *  dependency: base, mem
 *  @endverbatim
 *
 *  @author Yoshizumi Tanaka (y-tanaka@sm.sony.co.jp)
 *  @date   1 Feb., 2005 modified by y-tanaka
 */
/*========================================================================*/

#define SHA256_DIGESTSIZE 32
#define SHA256_DATASIZE	  64

/*------------------------------------------------------------------------*
 * type definition
 *------------------------------------------------------------------------*/
typedef void* sha256_handle_t;

/*------------------------------------------------------------------------*
 * error definition
 *------------------------------------------------------------------------*/

#define ERR_SHA_STATUS               (ERR_SHA_BASE|1)
#define ERR_SHA_DIGEST               (ERR_SHA_BASE|2)

/*========================================================================*/
/**
 * @brief  Initialize SHA-256 context
 *
 * SHA-256 - Initialize new context.
 *
 * @param[in,out] io_sha sha handle
 * @return ::ERR_OK
 * @return ::ERR_NOMEM
 * @return ::ERR_ILLEGALARGS
 * @return ::ERR_INTERNALERROR
 * @return ::ERR_CRYPTO_CRYPTO
 */
/*========================================================================*/
extern retcode_t
sha256_Init(sha256_handle_t *io_sha);

/*========================================================================*/
/**
 * @brief  Finalize SHA-256
 *
 * Add padding and return the message digest.
 *
 * @param[in,out] io_sha  sha handle
 * @param[out] out_digest digest
 * @return ::ERR_OK
 * @return ::ERR_ILLEGALARGS
 * @return ::ERR_INTERNALERROR
 * @return ::ERR_CRYPTO_CRYPTO
 */
/*========================================================================*/
extern retcode_t
sha256_Fin(sha256_handle_t io_sha,
           u_int8_t        out_digest[SHA256_DIGESTSIZE]);

/*========================================================================*/
/**
 * @brief  Update SHA-256
 *
 * Run your data through this.
 *
 * @param[in,out] io_sha sha handle
 * @param[in] in_buf message
 * @param[in] in_len message length
 * @return ::ERR_OK
 * @return ::ERR_ILLEGALARGS
 * @return ::ERR_INTERNALERROR
 * @return ::ERR_CRYPTO_CRYPTO
 */
/*========================================================================*/
extern retcode_t
sha256_Update(sha256_handle_t io_sha,
              const u_int8_t *in_buf,
              u_int32_t       in_len);

/*========================================================================*/
/**
 * @brief  Make Digest SHA-256
 *
 * @param[in] in_buf  message
 * @param[in] in_len  message length
 * @param[out] out_digest digest
 * @return ::ERR_OK
 * @return ::ERR_ILLEGALARGS
 * @return ::ERR_INTERNALERROR
 * @return ::ERR_CRYPTO_CRYPTO
 */
/*========================================================================*/
extern retcode_t
sha256_MakeDigest(const u_int8_t *in_buf,
                  u_int32_t       in_len,
                  u_int8_t        out_digest[SHA256_DIGESTSIZE]);

NP_END_EXTERN_C

#endif /* SHA256_API_H */
/** @} */
