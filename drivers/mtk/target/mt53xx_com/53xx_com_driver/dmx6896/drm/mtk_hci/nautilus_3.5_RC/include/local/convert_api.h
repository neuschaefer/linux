/** @addtogroup base base
 * @{ */

/*
 * Nautilus version 3.5
 *
 * Any code and mechanism in this module may not be used
 * in any form without permissions.
 * Copyright (C) 2007,2008,2009,2010 Sony Corporation, All Rights Reserved.
 */

#ifndef CONVERT_API_H
#define CONVERT_API_H

NP_BEGIN_EXTERN_C


#define CONVERT_TYPE_INT     1
#define CONVERT_TYPE_HEX     2

/**
 * @brief Converts an integer to an ASCII resentation corresponding to
 * the indicated type, given the ouput buffer and required output
 * length in_len. If the ASCII representation is smaller than in_len,
 * the output will be filled with zeros.
 *
 * @note Negative values are not supported for type 'hex'.
 * @note The output buffer of enough size should be provided.
 * @note Condition: in_value < |2^64-1|
 *
 * @param in_value The value of the integer.
 * @param in_type  CONVERT_TYPE_INT or CONVERT_TYPE_HEX.
 * @param in_len   Length of the required output.
 * @param io_buf   Output placeholder. The caller has to allocate it.
 *
 * @return ::ERR_OK                        <br>Indicating success
 * @return ::ERR_ILLEGALARGS               <br>The arguments to the function are incorrect
 */
extern retcode_t
convert_ToAscii(int32_t in_value, u_int32_t in_type, 
                int32_t in_len, u_int8_t *io_buf);


/**
 * @brief Converts a string to an integer
 *
 * @note Condition: maxint < 2^31
 *
 * @param [in] in_str  Input string
 *
 * @return                         <br>Integer value
 */
extern int32_t
convert_Atoi(const char *in_str);

extern int32_t
convert_GetDigitCount(int32_t in_value, u_int32_t in_type);


NP_END_EXTERN_C

#endif /* CONVERT_API_H */
