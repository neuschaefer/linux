/** @addtogroup base base
 * @{ */

/*
 * Nautilus version 3.5
 *
 * Any code and mechanism in this module may not be used
 * in any form without permissions.
 * Copyright (C) 2004,2005,2006,2007,2008,2009,2010,2011 Sony Corporation, All Rights Reserved.
 *
 */

#ifndef MEM_API_H
#define MEM_API_H

/** @file
 * @brief Memory Allocation/De-allocation API
 *
 * @require none
 * @author Norifumi Goto <n-goto@sm.sony.co.jp>
 * @date   26 Nov, 2004 by n-goto
 */

NP_BEGIN_EXTERN_C


/*---------------------------------------------------------------------------*
 * APIs
 *---------------------------------------------------------------------------*/

#ifndef NPOPT_USE_MEMCHECK

/**
 * @brief Initializes heap memory
 *
 * @param [in] in_context       context
 * @param [in] in_start         pointer to the heap memory.
 * @param [in] in_size          bytes of the heap memory. Should be >= 1024.
 *
 * @return ::ERR_OK                      <br>Indicating success
 * @return ::ERR_NOMEM                   <br>A resource failure has occurred.
*/
retcode_t
npi_mem_Init(
    npi_cmncontext_t in_context,
    char*            in_start,
    np_size_t           in_size);

/**
 * @brief Finalizes heap memory
 *
 * @param [in] in_context       context
 *
 * @return none
 */
void
npi_mem_Fin(npi_cmncontext_t    in_context);

/**
 * @brief Allocates buffer
 *
 * @param [in] in_context       context
 * @param [in] in_size          size of buffer to be allocated
 *
 * @return allocated buffer
 *
 */
extern void *
npi_mem_Alloc(
    npi_cmncontext_t     in_context,
    np_size_t               in_size);

/**
 * @brief Re-allocates buffer
 *
 * @param [in] in_context       context
 * @param [in] in_buf           size of buffer to be re-allocated
 * @param [in] in_size          size of buffer to be newly allocated
 * @return newly allocated buffer
 *
 */
extern void *
npi_mem_Realloc(
    npi_cmncontext_t  in_context,
    void *            in_buf,
    np_size_t            in_size);

/**
 * @brief De-allocates buffer
 *
 * @param [in] in_context       context
 * @param [in] in_buf           buffer to be deallocated
 * @return none
 */
extern void
npi_mem_Free(
    npi_cmncontext_t  in_context,
    void *            in_buf);


/**
 * @brief Strdup() wrapper
 *
 * @param [in] in_context       context
 * @param [in] in_str           buffer to be duplicated
 *
 * @return valid pointer to duplicated string buffer
 */
extern char *
npi_mem_StrDup(
    npi_cmncontext_t    in_context,
    const char *        in_str);

#else /* NPOPT_USE_MEMCHECK */

#include "memcheck.h"

#define npi_mem_Init(in_context, in_start, in_size) \
     MCMemInitManager((in_context), (in_size))
#define npi_mem_Fin(in_context) \
     MCMemCloseManager((in_context))
#define npi_mem_Alloc(in_context, in_size) \
     MCMemAlloc_priv((in_context), (in_size) MC_MEM_DEBUG_FILE_LINE)
#define npi_mem_Realloc(in_context, in_buf, in_size) \
     MCMemRealloc_priv((in_context), (in_buf), (in_size) MC_MEM_DEBUG_FILE_LINE)
#define npi_mem_Free(in_context, in_buf) \
     MCMemFree_priv((in_context), (in_buf) MC_MEM_DEBUG_FILE_LINE)
#define npi_mem_StrDup(in_context, in_str) \
     MCMemStrDup((in_context), (in_str) MC_MEM_DEBUG_FILE_LINE)

#endif /* NPOPT_USE_MEMCHECK */

/**
 * @brief The function copies in_size bytes from memory area in_src to
 * io_dst.
 *
 * @see Unix memmove
 *
 * @param[in,out] io_dst  Destination pointer
 * @param[in]     in_src  Source pointer
 * @param[in]     in_size Number of bytes to move
 *
 * @return Pointer to the updated destination buffer
 */
extern void *
npi_mem_Move(
    void *       io_dst,
    const void * in_src,
    np_size_t       in_size);

/**
 * @brief The function copies in_size bytes from memory area in_src to
 * io_dst.
 *
 * @see Unix memcpy
 *
 * @param[in,out] io_dst  Destination pointer
 * @param[in]     in_src  Source pointer
 * @param[in]     in_size Number of bytes to copy
 *
 * @return Pointer to the updated destination buffer
 */
extern void *
npi_mem_Copy(
    void *       io_dst,
    const void * in_src,
    np_size_t       in_size);

/**
 * @brief The function compares its arguments.
 *
 * @see Unix memcmp
 *
 * @param[in]  in_buf1  Buffer 1
 * @param[in]  in_buf2  Buffer 2
 * @param[in]  in_size  Number of bytes to compare
 *
 * @return 0   
 * @return >0  
 * @return <0
 */
extern int32_t
npi_mem_Cmp(
    const void * in_buf1,
    const void * in_buf2,
    np_size_t       in_size);

/**
 * @brief The function sets the first in_size bytes in memory area
 * io_buf to the value of in_c.
 *
 * @see Unix memset
 *
 * @param[in,out]  io_buf  Buffer pointer
 * @param[in]      in_c    Byte value
 * @param[in]      in_size Number of bytes
 *
 * @return  Pointer to the updated buffer
 */
extern void *
npi_mem_Set(
    void *       io_buf,
    int32_t      in_c,
    np_size_t       in_size);
    
/**
 * @brief memory dump
 *
 * @param [in] in_context       context
 * @return none
 */
#ifdef MM_ANALYZE
extern void
npi_mem_Dump(
    npi_cmncontext_t    in_context);
#endif

NP_END_EXTERN_C

#endif /* MEM_API_H */
/** @} */
