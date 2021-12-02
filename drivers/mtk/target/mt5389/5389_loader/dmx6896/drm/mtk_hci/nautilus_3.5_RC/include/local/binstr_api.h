/** @addtogroup base base
 * @{ */

/*
 * Nautilus version 3.5
 *
 * Any code and mechanism in this module may not be used
 * in any form without permissions.
 * Copyright (C) 2004,2005,2006,2007,2008,2009,2010 Sony Corporation, All Rights Reserved.
 *
 */

#ifndef BINSTR_API_H
#define BINSTR_API_H

/** @file
 * Binary String Type Definition and API
 *
 * @require cmn/base/mem
 * @author Norifumi Goto <n-goto@sm.sony.co.jp>
 */

NP_BEGIN_EXTERN_C

typedef np_str_t binstr_t;

typedef struct {
    /** Number of binstr */
    np_size_t			num;
    /** Array of binstr */
    binstr_t           *str[1];
} mbinstr_t;

/*---------------------------------------------------------------------------*
 * APIs
 *---------------------------------------------------------------------------*/

/**
 * static initializer
 */
#define NPI_BINSTR(a,b)		{ (a), (u_int8_t*)(b) }

/**
 * Length of string
 *
 * @param [in] in_str           string
 *
 * @return                         <br>Length of in_str
 */
#define npi_binstr_Len(in_str) ((in_str)->len)

/**
 * Buffer of string
 *
 * @param[in]  in_str                  string
 *
 * @return                              <br>Pointer of the buffer of in_str
 */
#define npi_binstr_Buf(in_str) ((in_str)->buf)


/**
 *@brief  Initilizes a binstr
 *
 * @param [in] in_context       context
 * @param [out] out_bs          string to initalize
 * @param [in] in_len           size of string you want to inialize
 *
 * @return out_bs                <br>initialized string
 */
extern binstr_t *
npi_binstr_Init(
    npi_cmncontext_t		in_context,
    binstr_t*			out_bs,
    np_size_t			in_len);


/**
 * @brief Checks if a binstr is correctly filled.
 *
 * @param[in]  in_str   string
 * @return      1        if binstr is correct
 * @return      0        otherwise
 */
#define npi_binstr_Check(in_str) ((((in_str) == NULL) || ((in_str)->len == 0) || ((in_str)->buf == NULL))? 0 : 1)

/* Today the stress tests require mem Alloc to be called from the location 
 * that uses the memory. This allows the alloc function to ignore stress
 * testing of certain components i.e. xml, smi_iter, asn. Once each of the
 * open valsuns for mem stress testing in these components is fixed then it 
 * should be possible to remove this temp workaround
 */

#ifdef NPOPT_ENABLE_STRESSTESTS
#include "mem_api.h" 
static binstr_t *
l_binstr_memcpy(binstr_t *in_dst, const void *in_src, np_size_t in_size)
{
    if ( (in_dst != NULL) && (in_src != NULL) ) {
        npi_mem_Copy(in_dst->buf, in_src, in_size);
    }
    return in_dst;
}

#define npi_binstr_Alloc(in_context, in_size, in_buf)                                                  \
    ( l_binstr_memcpy ( npi_binstr_Init( (in_context),                                                 \
                                         (npi_mem_Alloc((in_context), (np_size_t)(sizeof(binstr_t) + (in_size)))),  \
                                         (np_size_t)(in_size) ) ,                                                 \
                        in_buf,                                                                        \
                        in_size ) )
#else

/**
 *@brief Allocates and possibly initializes
 *
 * @param [in] in_context       context
 * @param [in] in_size          size of string you want to create
 * @param [in] in_buf           initial string
 * @note in_buf can be NULL if you do not want to initialize
 */
extern binstr_t *
npi_binstr_Alloc(
    npi_cmncontext_t		in_context,
    np_size_t			in_size,
    const u_int8_t*		in_buf);

#endif

/**
 * @brief Allocates string buffer and fills it by the given byte.
 *
 * @param [in] in_context       context
 * @param [in] in_size          size of string you want to create
 * @param [in] in_byte          the byte by which the buffer is fulfilled
 * 
 * @return 	                        <br>Allocated string
 */
extern binstr_t *
npi_binstr_AllocFill(
    npi_cmncontext_t		in_context,
    np_size_t			in_size,
    u_int8_t			in_byte);

/**
 * @brief Frees string buffer
 *
 * @param [in] in_context       context
 * @param [in] in_str           string to be freed
 *
 * @return                              <br>none
 * @note in_str can be NULL
 */
extern void
npi_binstr_Free(
    npi_cmncontext_t		in_context,
    binstr_t*			in_str);

/**
 * @brief Duplicates string
 *
 * @param [in] in_str   String to be duplicated.
 * @return Duplicated string typed binstr_t
 * @note in_str can be typed either binstr_t or vlbinstr_t
 */
#define npi_binstr_Dup(in_context, in_str) \
    npi_binstr_Alloc((in_context), (in_str)->len, (in_str)->buf)

/**
 * @brief Concatenates 2 strings
 * out_str = in_str1 . in_str2
 *
 * @param [in] in_context       context
 * @param [in] in_str1          string to be concatenated
 * @param [in] in_str2          string to be concatenated
 *
 * @return string                <br>newly-allocated string
 */
extern binstr_t *
npi_binstr_Concat(
    npi_cmncontext_t		in_context,
    const binstr_t*		in_str1,
    const binstr_t*		in_str2);

/**
 * @brief Validates Buffer Compare args for NULL
 *
 * @param [in] in_context       context
 * @param [in] in_arg1          first compare string
 * @param [in] in_arg2          second compare string 
 * @param [out] out_ret	        only valid if return is ERR_ILLEGALARGS (0==both NULL; -2==first is NULL; 2==second is NULL)
 *
 * @return retcode_t            <br>ERR_OK or ERR_ILLEGALARGS
 */
extern retcode_t
npi_binstr_CheckCmpArgs(
    const void *                in_arg1,
    const void *                in_arg2,
    int32_t *                   out_ret);

/**
 * @brief Compares 2 strings
 *
 * @param [in] in_context       context
 * @param [in] in_str1          string
 * @param [in] in_str2          string
 *
 * @return     >0        if in_str1 == in_str2
 * @return     >0       if in_str1 >  in_str2
 * @return     <0       if in_str1 <  in_str2
 */
extern int32_t
npi_binstr_CmpBuf(
    npi_cmncontext_t            in_context,
    np_size_t                   in_len1,
    const u_int8_t*             in_buf1,
    np_size_t                   in_len2,
    const u_int8_t*             in_buf2);

/**
 * @brief Compares 2 strings
 *
 * @param [in] in_context       context
 * @param [in] in_str1          string
 * @param [in] in_str2          string
 * 
 * @return    >0           if in_str1 == in_str2
 * @return    >0           if in_str1 >  in_str2
 * @return    <0           if in_str1 <  in_str2
 */
extern int32_t
npi_binstr_CmpFun(
    npi_cmncontext_t            in_context,
    const binstr_t*             in_str1,
    const binstr_t*             in_str2);

/**
 * @brief Compares 2 strings
 *
 * @param [in] in_context       context
 * @param [in] in_str1          string
 * @param [in] in_str2          string
 *
 * @return    0            if in_str1 == in_str2
 * @return    >0           if in_str1 >  in_str2
 * @return    <0           if in_str1 <  in_str2
 * @note in_str1 and in_str2 can be typed either binstr_t or vlbinstr_t
 */
extern int32_t
npi_binstr_Cmp(
    npi_cmncontext_t            in_context,
    const binstr_t*             in_str1,
    const binstr_t*             in_str2);

/**
 * Dumps the content of the string for debugging
 *
 * @param [in] in_context       context
 * @param [in] in_str           string to be dumped
 * @return none
 */
#define npi_binstr_Dump(in_str)  do{ if(in_str) {DUMP1(((in_str)->len,(in_str)->buf));} } while(0)
#define npi_binstr_Dump0(in_str) do{ if(in_str) {DUMP0(((in_str)->len,(in_str)->buf));} } while(0)
#define npi_binstr_Dump1(in_str) do{ if(in_str) {DUMP1(((in_str)->len,(in_str)->buf));} } while(0)
#define npi_binstr_Dump2(in_str) do{ if(in_str) {DUMP2(((in_str)->len,(in_str)->buf));} } while(0)
#define npi_binstr_Dump3(in_str) do{ if(in_str) {DUMP3(((in_str)->len,(in_str)->buf));} } while(0)

/**
 * Prints the content of the string for debugging
 *
 * @param [in] in_str           string to be printed
 * @return none
 */
#define npi_binstr_Print(in_str)  do{ if(in_str) {PRINT1(((in_str)->len,(in_str)->buf));} } while(0)
#define npi_binstr_Print0(in_str) do{ if(in_str) {PRINT0(((in_str)->len,(in_str)->buf));} } while(0)
#define npi_binstr_Print1(in_str) do{ if(in_str) {PRINT1(((in_str)->len,(in_str)->buf));} } while(0)
#define npi_binstr_Print2(in_str) do{ if(in_str) {PRINT2(((in_str)->len,(in_str)->buf));} } while(0)
#define npi_binstr_Print3(in_str) do{ if(in_str) {PRINT3(((in_str)->len,(in_str)->buf));} } while(0)

/*----------------------------------------------------*/

/**
 * @brief Clears the memory of io_string
 *
 * @param [in] in_context       context
 * @param [in,out] io_str       string
 *
 * @return io_str		                if succeeded
 * @return NULL		                otherwise.
 */
extern binstr_t *
npi_binstr_SetClear(
    npi_cmncontext_t		in_context,
    binstr_t*			io_str);

/**
 * @brief Sets string reference
 *
 * @param [in] in_context       context
 * @param [in,out] io_str       string
 * @param [in] in_size          the size of string
 * @param [in] in_buf           the string
 *
 * @return io_str			        if succeeded
 * @return NULL 			        otherwise.
 */
extern binstr_t *
npi_binstr_SetReference(
    npi_cmncontext_t		in_context,
    binstr_t*			io_str,
    np_size_t			in_size,
    u_int8_t*		in_buf);

/**
 * @brief Clones string and set its reference
 *
 * @param [in] in_context       context
 * @param [in,out] io_str       string
 * @param [in] in_size          the size of string
 * @param [in] in_buf           the string
 *
 * @return io_str			        if succeeded
 * @return NULL			        otherwise.
 *
 */
extern binstr_t *
npi_binstr_SetClone(
    npi_cmncontext_t		in_context,
    binstr_t*			io_str,
    np_size_t			in_size,
    const u_int8_t*		in_buf);

/**
 * @brief Frees cloned string buffer
 *
 * @param [in] in_context       context
 * @param [in, out] io_str      string whose internal buffer is freed
 *
 * @note io_str can be NULL
 * 
 * @return none
 */
extern void
npi_binstr_FreeClone(
    npi_cmncontext_t		in_context,
    binstr_t*			io_str);

/**
 * @brief Creates a null terminated c-string from a binstr type.
 *
 * This function takes a binstr_t and copies it to a char* array and
 * appends a null terminator.
 *
 * @param[in]  in_context   context
 * @param[in]  in_str       The binstr_t to copy
 * @param[out] out_dst      The char* array to allocate and populate.
 *
 * @return ::ERR_OK                     <br>Indicating success
 * @return ::ERR_NOMEM                  <br>A resource failure has occurred.
 * @return ::ERR_ILLEGALARGS            <br>The input arguments are incorrect. Arguments cannot be NULL.
 * @return ::ERR_INTERNALERROR          <br>An unrecoverable error has occurred in Nautilus. 
 */
extern retcode_t
npi_binstr_ToStr(npi_cmncontext_t     in_context,
                 const binstr_t      *in_str,
                 char               **out_dst);

/**
 * @brief Allocates multiple string buffer
 *
 * @param [in] in_context       context
 * @param [in] in_num           number of binstr
 * @param [in] in_str           array of binstr
 * @return Allocated string
 * @note in_buf can be NULL if you do not want to initialize
 */
extern mbinstr_t *
npi_mbinstr_Alloc(
    npi_cmncontext_t		in_context,
    np_size_t			in_num,
    binstr_t*			in_str[]);

/**
 * @brief Frees multiple string buffer
 *
 * @param [in] in_context       context
 * @param [in] in_mstr          array of binstr to be freed
 * @return none
 * @note in_str can be NULL
*/
extern void
npi_mbinstr_Free(
    npi_cmncontext_t		in_context,
    mbinstr_t*			in_mstr);

NP_END_EXTERN_C

#endif /* BINSTR_API_H */
/** @} */
