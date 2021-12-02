/*
 * Nautilus version 3.5
 *
 * Any code and mechanism in this module may not be used
 * in any form without permissions.
 * Copyright (C) 2010 Sony Corporation, All Rights Reserved.
 */
#ifndef NS_UTIL_SHA1_H
#define NS_UTIL_SHA1_H

NP_BEGIN_EXTERN_C

#define SHA1_DIGESTSIZE 20

typedef struct l_sha1 {
    u_int32_t		state[5];
    u_int32_t		count[2];
    unsigned char	buffer[64];
} ns_sha1_t;

void
ns_sha1_Create(
    ns_sha1_t*			context);

void
ns_sha1_Update(
    ns_sha1_t*			context,
    const unsigned char*	data,
    u_int32_t			len);

void
ns_sha1_Final(
    ns_sha1_t*			context,
    unsigned char		digest[20]);

NP_END_EXTERN_C

#endif
