/*
 * Nautilus version 3.5
 *
 * Any code and mechanism in this module may not be used
 * in any form without permissions.
 * Copyright (C) 2008,2009,2010 Sony Corporation, All Rights Reserved.
 */

#ifndef PPPROC_TYPES_H
#define PPPROC_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

/* Usage */
#define PPPROC_USAGE_PERSONALIZATION		(0)
#define PPPROC_USAGE_ACTIONTOKEN_ACQ_REG	(4)
#define PPPROC_USAGE_ACTIONTOKEN_ACQ_DEREG	(5)
#define PPPROC_USAGE_ACTIONTOKEN_ACQ_LICENSE	(6)
#define PPPROC_USAGE_NOTIFICATION_DL_START	(0x10)
#define PPPROC_USAGE_NOTIFICATION_DL_COMPLETE	(0x11)
/*
 * usage request-only check
 */
#define PPPROC_USAGECHECK_REQUESTONLY(u) ((u)!=0)


/* Error codes */
#define ERR_NP_PPPROC_SHORTOFBUFFERSIZE     (ERR_NP_PPPROC|1)

#define PPPROC_MINIMUM_REQUESTBUFFERSIZE 256

/* Encrypted ppproc package. */
#define PPPROC_ENC   0
#define PPPROC_CLEAR 1

#ifdef __cplusplus
}
#endif

#endif /* PPPROC_TYPES_H */
