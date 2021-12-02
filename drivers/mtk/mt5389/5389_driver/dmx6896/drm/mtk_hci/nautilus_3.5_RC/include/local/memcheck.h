/** @addtogroup base base
 * @{ */

/*
 * Nautilus version 3.5
 *
 * Any code and mechanism in this module may not be used
 * in any form without permissions.
 * Copyright (C) 2007,2008,2009,2010,2011 Sony Corporation, All Rights Reserved.
 */

#ifndef MEMCHECK_H__
#define MEMCHECK_H__

#define MC_MEM_DEBUG_FILE_LINE , __FILE__, __LINE__
#define MC_MEM_DEBUG_PARAMS    , const char *dbg_file, int32_t dbg_line

/* Internal API */
/**
 * @brief Initializes memory check manager.
 *
 * @param[in]     in_context   context
 * @param[in]     in_size      size of buffer to be newly allocated
 *
 * @return ::ERR_OK                     <br>Indicating success
 * @return ::ERR_NOMEM                  <br>A resource failure has occurred.
*/
extern retcode_t MCMemInitManager(npi_cmncontext_t in_context, long in_size);
/**
 * @brief Finalizes the memory check manager
 *
 * @param[in] in_context   context
 *
 * @return none
 */
extern void MCMemCloseManager(npi_cmncontext_t in_context);
/**
 * @brief Alloactes the memory 
 *
 * @param[in] in_context   context
 * @param[in] in_size      size of buffer to be allocated
 * @param[in] dbg_file     pointer of the debugging file
 * @param[in]  dbg_line    line number 
 *
 * @return::                            <br>pointer to the allocated string
 */
void* MCMemAlloc_priv(npi_cmncontext_t in_context, size_t in_size MC_MEM_DEBUG_PARAMS);
/**
 * @brief Allocates and clears the memory with zeros
 *
 * @param[in] in_context   context
 * @param[in] in_size      size of buffer to be cleared
 * @param[in] dbg_file     pointer of the debugging file
 * @param[in]  dbg_line    line number 
 *
 * @return::                            <br>pointer to the cleared string
 */
void* MCMemAllocClear_priv(npi_cmncontext_t in_context, long in_size MC_MEM_DEBUG_PARAMS);
/**
 * @brief Reallocates the content of in_str into a new buffer of size in_size
 *
 * @param[in] in_context    context
 * @param[in] *ptr          pointer to current memory
 * @param[in] in_size       size of buffer to be reallocated
 * @param[in] *dbg_file     pointer of the debugging file
 * @param[in]  dbg_line     line number
 *
 * @return::                            <br> pointer to the re-allocated string
 */
void* MCMemRealloc_priv(npi_cmncontext_t in_context, void *ptr, long in_size MC_MEM_DEBUG_PARAMS);
/**
 * @brief Frees  the memory
 *
 * @param[in] in_context   context
 * @param[in] *dbg_file    pointer of the debugging file
 * @param[in]  dbg_line    line number
 *
 * @return none
 */
void MCMemFree_priv(npi_cmncontext_t in_context, void *ptr MC_MEM_DEBUG_PARAMS);
/**
 * @brief Duplicates the string in the  memory
 *
 * @param[in] in_context   context

 * @param[in] *dbg_file    pointer of the debugging file
 * @param[in]  dbg_line    line number
*/
char* MCMemStrDup(npi_cmncontext_t in_context, const char *in_str MC_MEM_DEBUG_PARAMS);

#endif /* MEMCHECK_H__ */
