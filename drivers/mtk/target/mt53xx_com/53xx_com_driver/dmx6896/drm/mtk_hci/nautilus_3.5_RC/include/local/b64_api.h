/** @addtogroup b64 b64
 *  @ingroup cmn
 *  @{ */

/*
 * Nautilus version 3.5
 *
 * Any code and mechanism in this module may not be used
 * in any form without permissions.
 * Copyright (C) 2004,2005,2006,2007,2008,2009,2010 Sony Corporation, All Rights Reserved.
 */

#ifndef B64_API_H
#define B64_API_H

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================*/
/** @file
 * @brief Base64 processing library
 *
 * dependency: base, mem
 *
 * @author Norifumi Goto <n-goto@sm.sony.co.jp>
 */
/*===========================================================================*/

#define B64_EOLCHARS_LF    0
#define B64_EOLCHARS_CRLF  1
#define B64_EOLCHARS_CR    2

#define B64_CHARSINLINE_DEFAULT 76

/*===========================================================================*/
/**
 * @brief base64-encode the string
 *
 * base64-encode the given string, create a new buffer
 * and write the result in the buffer
 *
 * @param[in] in_str            String to be encoded
 * @param[in] in_chars_inline   How may characters can be in the same line
 *                               >0: (should be multiple of 4)
 *                               =0: infinity... all characters are in the same line
 *                               B64_CHARSINLINE_DEFAULT: default (76)
 * @param[in] in_eolchars       Type of end-of-line character(s)
 *                               B64_EOLCHARS_LF:   '\n'
 *                               B64_EOLCHARS_CRLF: '\r\n'
 *                               B64_EOLCHARS_CR:   '\r'
 * @param[out] out_str          The result of base64-encoding
 * 
 * @return ::ERR_OK 
 * @return ::ERR_NOMEM
 * @return ::ERR_ILLEGALARGS
 */
/*===========================================================================*/
extern retcode_t
npi_b64_Encode(npi_cmncontext_t in_context, 
               const binstr_t *in_str, 
               int32_t in_chars_inline, 
               int32_t in_eolchars, 
               binstr_t **out_str);

/*===========================================================================*/
/**
 * @brief base64-decode the string
 *
 * base64-decode the given string, create a new buffer
 * and write the result in the buffer
 *
 * @param[in]  in_str   base64-encoded string
 * @param[out] out_str  the result of base64-decoding
 *
 * @return ::ERR_OK       
 * @return ::ERR_NOMEM
 */
/*===========================================================================*/
extern retcode_t
npi_b64_Decode(npi_cmncontext_t in_context, 
               const binstr_t *in_str, 
               binstr_t **out_str);

#ifdef __cplusplus
}
#endif

#endif /* B64_API_H */
/** @} */
