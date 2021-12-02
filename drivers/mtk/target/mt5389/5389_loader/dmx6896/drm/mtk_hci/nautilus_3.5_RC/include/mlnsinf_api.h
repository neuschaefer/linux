/*
 * Nautilus version 3.5
 *
 * Any code and mechanism in this module may not be used
 * in any form without permissions.
 * Copyright (C) 2007,2008,2009,2010,2011 Sony Corporation, All Rights Reserved.
 */

#ifndef MLNSINF_API_H
#define MLNSINF_API_H

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------*
 * Maximum number of Security attributes
 *---------------------------------------------------------------------------*/
#define MLNSINF_SATR_MAXCOUNT 16

/*---------------------------------------------------------------------------*
 * Content-type definition (value of styp)
 *
 * Note that Content-type value is NULL-terminated URN string and
 * styp box contains the last NULL character.
 *---------------------------------------------------------------------------*/
#define MLNSINF_CTYPE_AUDIO "urn:marlin:organization:sne:content-type:audio"
#define MLNSINF_CTYPE_AUDIOVAL ((u_int8_t*)MLNSINF_CTYPE_AUDIO)
#define MLNSINF_CTYPE_AUDIOLEN sizeof(MLNSINF_CTYPE_AUDIO)
#define MLNSINF_CTYPE_VIDEO "urn:marlin:organization:sne:content-type:video"
#define MLNSINF_CTYPE_VIDEOVAL ((u_int8_t*)MLNSINF_CTYPE_VIDEO)
#define MLNSINF_CTYPE_VIDEOLEN sizeof(MLNSINF_CTYPE_VIDEO)

/*---------------------------------------------------------------------------*
 * Error code
 *---------------------------------------------------------------------------*/
#define ERR_NP_MLNSINF_BASE      ERR_NP_MLNSINF
#define ERR_NP_MLNSINF_NOACBC    (ERR_NP_MLNSINF_BASE + 0)
#define ERR_NP_MLNSINF_CORRUPTED (ERR_NP_MLNSINF_BASE + 1)
#define ERR_NP_MLNSINF_NOSUPPORT (ERR_NP_MLNSINF_BASE + 2)

/*---------------------------------------------------------------------------*
 * Type definition
 *---------------------------------------------------------------------------*/
typedef struct {
    u_int32_t       name;
    u_int8_t       *val;
    int             len;
} mlnsinf_satr_t;

typedef struct {
    u_int8_t       *octid;
    int             octidlen;
    u_int8_t       *octbdl;
    int             octbdllen;
    u_int8_t       *hmac; /* 32 bytes fixed */
    u_int8_t       *satr;
    int             satrlen;
    int             nsatrs;
    mlnsinf_satr_t  satrs[MLNSINF_SATR_MAXCOUNT];
} mlnsinf_t;

/*---------------------------------------------------------------------------*
 * Global function
 *---------------------------------------------------------------------------*/
np_ret_t
mlnsinf_ParseIPMP(u_int8_t *in_buf, int in_len, mlnsinf_t *out_sinf);

#ifdef __cplusplus
}
#endif

#endif /* MLNSINF_API_H */
