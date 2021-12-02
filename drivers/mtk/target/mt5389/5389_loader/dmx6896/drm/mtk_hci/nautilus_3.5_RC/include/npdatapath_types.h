/*
 * Nautilus version 3.5
 *
 * Any code and mechanism in this module may not be used
 * in any form without permissions.
 * Copyright (C) 2011 Sony Corporation, All Rights Reserved.
 */

/**
 * @file
 * Data path structures
 *
 * @see API.reference.pdf
 */
#ifndef NPDP_TYPES_H
#define NPDP_TYPES_H

NP_BEGIN_EXTERN_C

/* We kept the np type and not npdatapath to maintain backward compatibility */

/*---------------------------------------------------------------------------*
 * crypto type definitions
 *---------------------------------------------------------------------------*/

#define NP_CRYPTOTYPE_PLAIN             (0)
#define NP_CRYPTOTYPE_ISMA_AES128CTR    (1)
#define NP_CRYPTOTYPE_KF_CBC            (2)
#define NP_CRYPTOTYPE_BBTS_FULL         (3) /* BBTS using AES CBC or UNDEFINED */
#define NP_CRYPTOTYPE_BBTS_SKL          (4) /* BBTS using Single Key Layer */
#define NP_CRYPTOTYPE_BBTS_AES128CBC    (5) /* BBTS crypto using AES CBC */
#define NP_CRYPTOTYPE_UNDEFINED         (6) /* undefine crypto type - used for MoveKey() only */

/* encryption algorithm for KFormat CBC mode */
#define NP_CRYPTO_KFCBC_ALGO_AES        (0)
#define NP_CRYPTO_KFCBC_ALGO_DES        (1)

/* padding algorithm for KFormat CBC mode */
#define NP_CRYPTO_KFCBC_PAD_NONE        (0)
#define NP_CRYPTO_KFCBC_PAD_RFC2630     (1)
#define NP_CRYPTO_KFCBC_PAD_LENGTH      (2)

/* key indexes used for bbts content */
#define NP_CRYPTO_BBTSAES_KEY_IDX0      (0) /** Used for odd */
#define NP_CRYPTO_BBTSAES_KEY_IDX1      (1) /** Used for even */
#define NP_CRYPTO_BBTSAES_KEY_IDX2      (2) /** Used for single-key-layer */

/* key indexes used for bbts content */
#define NP_CRYPTO_BBTSFULL_KEY_IDX0      (0) /**< Used for an odd traffic key */
#define NP_CRYPTO_BBTSFULL_KEY_IDX1      (1) /**< Used for an even traffic key */

/* key indexes used for bbts content */
#define NP_CRYPTO_BBTSFULL_ALGO_AES128CBC (0) /**< Used for AES128 CBC */
#define NP_CRYPTO_BBTSFULL_ALGO_UNDEFINED (1) /**< Used for moving a traffic key ico Nautilus unsupported crypto algorithms*/

/* digest algorithms supported by CheckHmac() */
#define NP_DIGEST_SHA256                (1)
#define NP_DIGEST_SHA256_LENGTH         (32)

/*---------------------------------------------------------------------------*
 * type definitions
 *---------------------------------------------------------------------------*/

typedef np_protocol_t  np_trackhandle_t;

typedef struct {
    u_int32_t type;
    union {
        struct {
            u_int8_t selenc;
            u_int8_t keyindlen;
            u_int8_t ivlen;
            u_int8_t resv[1];
            u_int8_t salt[8];
        } ismacryp;
        struct {
            u_int8_t selenc;  
            u_int8_t algo;    
            u_int8_t padding;
        } kfcbc;
        struct {
            u_int8_t keyindex;  
        } bbtsaescbc;
        struct {
            u_int8_t algo;      /* AES128-CBC or undefined  */
            u_int8_t keyindex;  /* Only used for full case */
        } bbtsfull;
        u_int8_t dummy[12];
    } data;
} np_cryptoinfo_t;

typedef struct {
    np_str_t        *cid;        /* For MS3 it is not ok to have a NULL CID. For BBTS it can be set to NULL. */
    np_cryptoinfo_t  cryptoinfo;
} np_trackinfo_t;


NP_END_EXTERN_C

#endif /* NPDP_TYPES_H */
