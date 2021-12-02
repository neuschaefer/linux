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

#ifndef HMAC_API_H
#define HMAC_API_H

#ifdef __cplusplus
extern "C" {
#endif

/*========================================================================*/
/** @file
 *  @brief  HMAC library - API definitions
 *
 *  @verbatim
 *  dependency: base, binstr
 *  @endverbatim
 *
 *  @author Yoshizumi Tanaka (y-tanaka@sm.sony.co.jp)
 *  @date   2 Dec., 2004 modified by y-tanaka
 */
/*========================================================================*/

/*------------------------------------------------------------------------*
 * error definition
 *------------------------------------------------------------------------*/


#define HMAC_ALGO_SHA1		1  /*  hmac with SHA1    */
#define HMAC_ALGO_SHA256	2  /*  hmac with SHA256  */

/*========================================================================*/
/**
 * @brief  Calculate HMAC
 *
 * Calculate SHA1 and SHA256 based HMAC
 *
 * @param[in] in_digest_algo  Digest algorithm to be used, 
 *                            i.e.., HMAC_ALGO_SHA1, HMAC_ALGO_SHA256
 * @param[in]  in_key         key
 * @param[in]  in_data        message
 * @param[out] out_hmac       hmac
 * @return ::ERR_OK
 * @return ::ERR_ILLEGALARGS
 * @return ::ERR_NOMEM
 */
/*========================================================================*/
extern retcode_t
hmac_Calculate(int             in_digest_algo,
               const binstr_t  *in_key,
               const binstr_t  *in_data,
               binstr_t       **out_hmac);

#ifdef __cplusplus
}
#endif

#endif /* HMAC_API_H */
/** @} */
