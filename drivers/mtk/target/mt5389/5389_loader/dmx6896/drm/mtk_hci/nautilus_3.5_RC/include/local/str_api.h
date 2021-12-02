/** @addtogroup base base
 * @{ */

/*
 * Nautilus version 3.5
 *
 * Any code and mechanism in this module may not be used
 * in any form without permissions.
 * Copyright (C) 2007,2008,2009,2010,2011 Sony Corporation, All Rights Reserved.
 *
 */

#ifndef STR_API_H
#define STR_API_H

NP_BEGIN_EXTERN_C

/*---------------------------------------------------------------------------*
 * APIs
 *---------------------------------------------------------------------------*/

/**
 * @brief The function returns the number of bytes in in_str, not
 * including the terminating null character.
 *
 * @see Unix strlen
 *
 * @param[in]  in_str   String
 *
 * @return Length of the string
 */
extern np_size_t
npi_str_Len(const char *in_str);


/**
 * @brief The function copies exactly in_n bytes, truncating in_src
 * or adding null characters to out_dest if necessary. The result will
 * not be null-terminated if the length of in_src is in_n or more.
 * In each case, function returns out_dest.
 *
 * @see Unix strncpy
 *
 * @param[out] out_dest   Destination string 
 * @param[in]     in_src     Source string
 * @param[in]     in_n       Number of bytes to copy
 *
 * @return Pointer to the destination string
 *
 */
extern char*
npi_str_NCopy(char *out_dest, const char *in_src, np_size_t in_n);

/**
 * @brief Compares two strings
 *
 * @see Unix strcmp
 *
 * @param[in]  in_str1   First string
 * @param[in]  in_str2   Second string
 *
 * @return =0  The strings are equal
 * @return <0  First string is less than second string
 * @return >0  Second string is less than first string
 */
extern int32_t
npi_str_Cmp(const char * in_str1, const char * in_str2);

/**
 * @brief Compares at most in_n characters of two strings
 *
 * @see Unix strncmp
 *
 * @param[in]  in_str1   First string
 * @param[in]  in_str2   Second string
 *
 * @return =0  The strings are equal
 * @return <0  First string is less than second string
 * @return >0  Second string is less than first string
 * 
 */
extern int32_t
npi_str_NCmp(const char *in_str1, const char *in_str2, size_t in_n);

/**
 * @brief The function appends exactly in_n bytes of string in_buf2,
 * to the end of string io_buf1. The  result will be a pointer to the
 * null-terminated string io_buf1. The
 * initial character of in_buf2 overrides the null character at the
 * end of io_buf1.
 *
 * @see Unix strncat
 *
 * @param[in,out] io_buf1  Destination string 
 * @param[in]     in_buf2  String to be appended
 * @param[in]     in_n     Number of bytes to append
 *
 * @return Pointer to the concatenated string
 */
extern char*
npi_str_NCat(char *io_buf1, const char *in_buf2, np_size_t in_n);


/** 
 * @brief The function locates the first occurrence of the string s2
 * (excluding the terminating null character) in string str1 and
 * returns a pointer to the located string, or a null pointer if the
 * string is not found. If s2 points to a string with zero length
 * (that is, the string ""), the function returns s1.
 *
 * @see Unix strstr
 *
 * @param[in]  in_str1 First string
 * @param[in]  in_str2 Second string
 *
 * @return Pointer to the first occurrence of str2 in str1, or NULL if no match is found
 * 
 */
extern const char*
npi_str_Str(const char *in_str1, const char *in_str2);

/** 
 * @brief The function converts the initial portion of the
 * string pointed to by in_str to a type long int representation.
 *
 * @see Unix strtol
 *
 * @param[in]  in_str       String
 * @param[out] out_endptr   If not NULL, returns a string beginning 
 *                          with the first character that npi_str_ToL() 
 *                          does not attempt to convert.
 * @param[in]  in_base      Base of the string (value between 0-36)
 *
 * @return long value
 */
extern int32_t
npi_str_ToL(const char *in_str, char **out_endptr, int32_t in_base);

/**
 * @brief  The  function  shall  locate the first occurrence of c
 * in the string pointed to by s. The terminating  null byte is 
 * considered to be part of the string.
 *
 * @see Unix strchr
 *
 * @param[in] in_s   String where searching will be made
 * @param[in] in_c   A character we are looking for
 *
 * @return           A pointer to the found character or NULL if the
 *                   character was not found.
 */
extern char*
npi_str_Chr(const char* in_s, int32_t in_c);

/**
 * @brief  The  function  shall  locate the last occurrence of c
 * in the string pointed to by s. The terminating  null byte is 
 * considered to be part of the string.
 *
 * @see Unix strrchr
 *
 * @param[in] in_s   String where searching will be made
 * @param[in] in_c   A character we are looking for
 *
 * @return           A pointer to the found character or NULL if the
 *                   character was not found.
 */
extern char*
npi_str_RChr(const char* in_s, int32_t in_c);


/**
 * @brief Same behaviour as strnicmp
 *
 * @see Unix strnicmp
 *
 * @param[in]      in_context  context
 * @param[in]      in_str1     First string
 * @param[in]      in_str2     Second string
 * @param[in]      in_len      Max length for the comparison
 *
 * @return =0  The strings are equal
 * @return <0  First string is less than second string
 * @return >0  Second string is less than first string
 * 
 * @note To do : handle UTF-8 strings
 */
extern int32_t
npi_str_NCaseCmp(npi_cmncontext_t in_context, const char *in_str1, const char *in_str2, size_t in_len);

/**
 * @brief The strspn() function shall compute the length (in bytes) of
 *        the maximum initial segment of the string pointed to by in_s1
 *        which consists entirely of bytes from the string pointed
 *        to by in_s2.
 *
 * @see Unix strspn
 *
 * @param[in] in_s1  A string.
 * @param[in] in_s2  A skip set string.
 *
 * @return the length (in bytes) of the maximum initial segment of the string
 *         pointed to by in_s1 which consists entirely of bytes from the
 *         string pointed to by in_s2.
 *         
 */
extern np_size_t
npi_str_Spn(const char* in_s1, const char* in_s2);

NP_END_EXTERN_C

#endif /* STR_API_H */
