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

#ifndef AES_API_H
#define AES_API_H

#ifdef __cplusplus
extern "C" {
#endif

/*========================================================================*/
/** @file
 *  @brief  AES - API definitions
 *
 *  @verbatim
 *  dependency: base, mem
 *  @endverbatim
 *
 *  @author Yoshizumi Tanaka (y-tanaka@sm.sony.co.jp)
 *  @date   2 Dec., 2004 modified by y-tanaka
 */
/*========================================================================*/

#define AES_ENCRYPT		1  /*  Are we encrpyting?  */
#define AES_DECRYPT		2  /*  Are we decrpyting?  */

#define AES_BLOCKSIZE		16

/*------------------------------------------------------------------------*
 * type definition
 *------------------------------------------------------------------------*/
typedef void *	aes_handle_t;

/*------------------------------------------------------------------------*
 * type definition
 *------------------------------------------------------------------------*/

#define ERR_AES_KEY_DIR         (ERR_AES_BASE|1)
#define ERR_AES_KEY_MAT         (ERR_AES_BASE|2)
#define ERR_AES_KEY_INSTANCE    (ERR_AES_BASE|3)
#define ERR_AES_CIPHER_STATE    (ERR_AES_BASE|4)


/*========================================================================*/
/**
 * @brief  Initialize AES context.
 *
 *
 * Set key (initialize key schedule array).
 *
 * @param[in,out] io_aes aes handle
 * @param[in] in_dir direction (AES_ENCRYPT or AES_DECRYPT)
 * @param[in] in_keyLen key length (128 or 192 or 256)
 * @param[in] in_keyBuf key data
 * @return ::ERR_OK
 * @return ::ERR_AES_KEY_INSTANCE
 * @return ::ERR_AES_KEY_MAT
 * @return ::ERR_AES_KEY_DIR
 * @return ::ERR_ILLEGALARGS
 * @return ::ERR_NOMEM
 * @return ::ERR_INTERNALERROR
 * @return ::ERR_CRYPTO_CRYPTO
 */
/*========================================================================*/
retcode_t
aes_Init(aes_handle_t   *io_aes,
         u_int32_t       in_dir,
         u_int32_t       in_keyLen,
         const u_int8_t *in_keyBuf);


/*========================================================================*/
/**
 * @brief  Finalize AES context
 *
 * Destroy context
 *
 * @param[in,out] io_aes aes handle
 * @return ::ERR_OK
 */
/*========================================================================*/
retcode_t
aes_Fin(aes_handle_t io_aes);



/*========================================================================*/
/**
 * @brief  Encrypt a block of data
 *
 * @param[in,out] io_aes  aes handle
 * @param[in] in_plain  plain text
 * @param[out] out_cipher cipher text
 * @return ::ERR_OK
 * @return ::ERR_AES_CIPHER_STATE
 * @return ::ERR_AES_MALFORMED_INPUT
 * @return ::ERR_ILLEGALARGS
 */
/*========================================================================*/
retcode_t
aes_BlockEncrypt(aes_handle_t    io_aes,
                 const u_int8_t *in_plain,
                 u_int8_t       *out_cipher);

/*========================================================================*/
/**
 * @brief  Decrypt a block of data
 *
 * @param[in,out] io_aes  aes handle
 * @param[in] in_cipher  cipher text
 * @param[out] out_plain plain text
 * @return ::ERR_OK
 * @return ::ERR_AES_CIPHER_STATE
 * @return ::ERR_AES_MALFORMED_INPUT
 * @return ::ERR_ILLEGALARGS
 */
/*========================================================================*/
retcode_t
aes_BlockDecrypt(aes_handle_t    io_aes,
                 const u_int8_t *in_cipher,
                 u_int8_t       *out_plain);


#ifdef __cplusplus
}
#endif

#endif /* AES_API_H */
/** @} */
