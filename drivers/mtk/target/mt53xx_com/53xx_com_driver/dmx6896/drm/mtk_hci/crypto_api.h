/** @addtogroup crypto crypto
 *  @ingroup common
 * @{
 */

/*
 * Nautilus version 3.2
 *
 * Any code and mechanism in this module may not be used
 * in any form without permissions.
 * Copyright (C) 2004,2005,2006,2007,2008,2009 Sony Corporation, All Rights Reserved.
 */

#ifndef CRYPTO_API_H
#define CRYPTO_API_H

#ifdef __cplusplus
extern "C" {
#endif

/*========================================================================*/
/** @file
 *  @brief  Crypto library - API definitions
 *
 *  @verbatim
 *  dependency: base, mem, aes
 *  @endverbatim
 *
 *  @author Yoshizumi Tanaka (y-tanaka@sm.sony.co.jp)
 *  @date   6 Dec., 2004 modified by y-tanaka
 */
/*========================================================================*/

/*------------------------------------------------------------------------*
 * type definition
 *------------------------------------------------------------------------*/
/** Pointer to a crypto address space */
typedef void *			crypto_handle_t;

#define CRYPTO_SALTSIZE		8

/* base */
#define CRYPTO_TYPE_BASE    0x000000FF
#define CRYPTO_DIR_BASE     0x0000FF00
#define CRYPTO_MODE_BASE    0x00FF0000
#define CRYPTO_PAD_BASE     0xFF000000

/* type */
#define CRYPTO_TYPE_UNDEF   0x00000000
#define CRYPTO_TYPE_AES     0x00000001

#define CRYPTO_TYPE_RSA     0x00000003
#define CRYPTO_TYPE_SHA     0x00000004
#define CRYPTO_TYPE_HMAC    0x00000005

#define CRYPTO_TYPE_RSASHA1   0x00000003
#define CRYPTO_TYPE_RSASHA256 0x00000007

#define CRYPTO_TYPE_AES_HW  0x00000010

/* dir */
#define CRYPTO_DIR_UNDEF    0x00000000
#define CRYPTO_DIR_ENC      0x00000100
#define CRYPTO_DIR_DEC      0x00000200
#define CRYPTO_DIR_CTR      0x00000300

/* mode */
#define CRYPTO_MODE_UNDEF   0x00000000
#define CRYPTO_MODE_ECB     0x00010000 /* Electric Code Block mode */
#define CRYPTO_MODE_CBC     0x00020000 /* Cipher Block Chaining mode */
#define CRYPTO_MODE_CTR	    0x00030000 /* Counter mode */

/* padding */
#define CRYPTO_PAD_UNDEF    0x00000000
#define CRYPTO_PAD_NONE     0x01000000 /* used when plain size is just n times of block size */
#define CRYPTO_PAD_PKCS5    0x02000000 /* used both text and binary encryption */
#define CRYPTO_PAD_NULL     0x03000000 /* used only text encryption */
#define CRYPTO_PAD_SPACE    0x04000000 /* used only text encryption */

/* aes */
#define AES_KEYLENGTH_128   16
#define AES_KEYLENGTH_192   24
#define AES_KEYLENGTH_256   32

/*------------------------------------------------------------------------*
 * error definition
 *------------------------------------------------------------------------*/

#define ERR_CRYPTO_STATUS               (ERR_CRYPTO_BASE|1)
#define ERR_CRYPTO_UNSUPPORTED          (ERR_CRYPTO_BASE|2)
#define ERR_CRYPTO_ENCRYPT              (ERR_CRYPTO_BASE|3)
#define ERR_CRYPTO_DECRYPT              (ERR_CRYPTO_BASE|4)
#define ERR_CRYPTO_KEY                  (ERR_CRYPTO_BASE|5)
#define ERR_CRYPTO_INIT                 (ERR_CRYPTO_BASE|6)
#define ERR_CRYPTO_FIN                  (ERR_CRYPTO_BASE|7)
#define ERR_CRYPTO_CRYPTO               (ERR_CRYPTO_BASE|8) /* any other crypto err */


/*========================================================================*/
/**
 * @brief  Initialize CRYPTO context
 *
 * @param[in,out] io_crypto crypto handle
 * @param[in] in_type cipher type (AES) |
 *                         direction (ENC, DEC, CTR) |
 *                         encryption mode (ECB, CBC, CTR) |
 *                         padding mode (NONE, PKCS5, NULL, SPACE)
 * @param[in] in_ivBuf initial vector
 * @param[in] in_ivLen initial vector length
 * @param[in] in_saltBuf salt value used in AES counter mode
 * @param[in] in_keyBuf key data
 * @param[in] in_keyLen key length
 * @return ::ERR_OK
 * @return ::ERR_CRYPTO_STATUS
 * @return ::ERR_CRYPTO_NOMEM
 */
/*========================================================================*/
retcode_t
crypto_Init(
    crypto_handle_t*		io_crypto,
    u_int32_t			in_type,
    const u_int8_t*		in_ivBuf,
    u_int32_t			in_ivLen,
    const u_int8_t*		in_saltBuf,
    const u_int8_t*		in_keyBuf,
    u_int32_t			in_keyLen);

/*========================================================================*/
/**
 * @brief  Finalize CRYPTO context
 *
 * @param[in,out] io_crypto crypto handle
 * @return ::ERR_OK
 */
/*========================================================================*/
retcode_t
crypto_Fin(
    crypto_handle_t		io_crypto);

/*========================================================================*/
/**
 * @brief  Encyption
 *
 * @param[in,out] io_crypto  crypto handle
 * @param[in] in_plainBuf  plain text buffer
 * @param[in] in_plainLen  plain text length
 * @param[out] out_cipherBuf cipher text buffer
 * @param[in,out] io_cipherLen  cipher text length
 * @return ::ERR_OK
 * @return ::ERR_CRYPTO_STATUS
 * @return ::ERR_CRYPTO_ENCRYPT
 * @return ::ERR_CRYPTO_MALFORMED_INPUT
 * NOTE: Prepared buffer size is input through io_cipherLen and
 *       written byte size is output throuth io_cipherLen
 */
/*========================================================================*/
retcode_t
crypto_Encrypt(
    crypto_handle_t		io_crypto,
    const u_int8_t*		in_plainBuf,
    np_size_t			in_plainLen,
    u_int8_t*			out_cipherBuf,
    np_size_t*			io_cipherLen);

/*========================================================================*/
/**
 * @brief  Decryption
 *
 * @param[in,out] io_crypto  crypto handle
 * @param[in] in_cipherBuf  cihper text buffer
 * @param[in] in_cipherLen  cipher text length
 * @param[out] out_plainBuf plain text buffer
 * @param[in,out] io_plainLen  plain text length
 * @return ::ERR_OK
 * @return ::ERR_CRYPTO_STATUS
 * @return ::ERR_CRYPTO_DECRYPT
 * @return ::ERR_CRYPTO_MALFORMED_INPUT
 * NOTE: Prepared buffer size is input through io_palinLen and
 *       written byte size is output throuth io_plainLen
 */
/*========================================================================*/
retcode_t
crypto_Decrypt(
    crypto_handle_t		io_crypto,
    const u_int8_t*		in_cipherBuf,
    np_size_t			in_cipherLen,
    u_int8_t*			out_plainBuf,
    np_size_t*			io_plainLen);

/*========================================================================*/
/**
 * @brief  Decryption
 *
 * @param[in] in_ivBuf  Initial Vector
 * @param[in] in_ivLen  Initial Vector Length
 * @param[in] in_saltValueBuf  Salt Value (64 bit)
 * @param[out] out_counterBuf Counter (128 bit)
 * @return ::ERR_OK
 */
/*========================================================================*/
retcode_t
crypto_GetAes128Counter(
    const u_int8_t*		in_ivBuf,
    u_int32_t			in_ivLen,
    const u_int8_t*		in_saltValueBuf,
    u_int8_t*			out_counterBuf);

/* encrypt n blocks with AES128-CBC mode/PKCS5 padding */
retcode_t
crypto_EncryptAES128CBC(
    const u_int8_t *in_ivBuf,
    const u_int8_t *in_keyBuf,
    const u_int8_t *in_plainBuf,
    np_size_t       in_plainLen,
    u_int8_t       *out_cipherBuf,
    np_size_t      *io_cipherLen);

/* decrypt n blocks with AES128-CBC mode/PKCS5 padding */
retcode_t
crypto_DecryptAES128CBC(
    const u_int8_t *in_ivBuf,
    const u_int8_t *in_keyBuf,
    const u_int8_t *in_cipherBuf,
    np_size_t       in_cipherLen,
    u_int8_t       *out_plainBuf,
    np_size_t      *io_plainLen);

/* decrypt 1 block */
retcode_t
crypto_DecryptAES128(
    const u_int8_t *in_keyBuf,
    const u_int8_t *in_cipherBuf,
    u_int8_t       *out_plainBuf);

#ifdef __cplusplus
}
#endif

#endif /* CRYPTO_API_H */
/** @} */
