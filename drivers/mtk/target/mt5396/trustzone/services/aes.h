/**
 * \file aes.h
 */
#ifndef AES_H
#define AES_H

#include "x_typedef.h"

#define AES_ENCRYPT     0
#define AES_DECRYPT     1

/**
 * \brief          AES context structure
 */
typedef struct
{
    INT32 nr;                     /*!<  number of rounds  */
    UINT32 *rk;          /*!<  AES round keys    */
    UINT32 buf[68];      /*!<  unaligned data    */
} aes_context;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief          AES key schedule (encryption)
 *
 * \param ctx      AES context to be initialized
 * \param key      encryption key
 * \param keysize  must be 128, 192 or 256
 */
void aes_setkey_enc( aes_context *ctx, UINT8 *key, INT32 keysize );

/**
 * \brief          AES key schedule (decryption)
 *
 * \param ctx      AES context to be initialized
 * \param key      decryption key
 * \param keysize  must be 128, 192 or 256
 */
void aes_setkey_dec( aes_context *ctx, UINT8 *key, INT32 keysize );

/**
 * \brief          AES-ECB block encryption/decryption
 *
 * \param ctx      AES context
 * \param mode     AES_ENCRYPT or AES_DECRYPT
 * \param input    16-byte input block
 * \param output   16-byte output block
 */
void aes_crypt_ecb( aes_context *ctx,
                    INT32 mode,
                    UINT8 input[16],
                    UINT8 output[16] );

/**
 * \brief          AES-CBC buffer encryption/decryption
 *
 * \param ctx      AES context
 * \param mode     AES_ENCRYPT or AES_DECRYPT
 * \param length   length of the input data
 * \param iv       initialization vector (updated after use)
 * \param input    buffer holding the input data
 * \param output   buffer holding the output data
 */
void aes_crypt_cbc( aes_context *ctx,
                    INT32 mode,
                    INT32 length,
                    UINT8 iv[16],
                    UINT8 *input,
                    UINT8 *output );

/**
 * \brief          AES-CFB buffer encryption/decryption
 *
 * \param ctx      AES context
 * \param mode     AES_ENCRYPT or AES_DECRYPT
 * \param length   length of the input data
 * \param iv_off   offset in IV (updated after use)
 * \param iv       initialization vector (updated after use)
 * \param input    buffer holding the input data
 * \param output   buffer holding the output data
 */
void aes_crypt_cfb( aes_context *ctx,
                    INT32 mode,
                    INT32 length,
                    INT32 *iv_off,
                    UINT8 iv[16],
                    UINT8 *input,
                    UINT8 *output );

#ifdef __cplusplus
}
#endif

#endif /* aes.h */
