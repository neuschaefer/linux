/** @addtogroup nsppproc nsppproc
 *  @ingroup nsec
 * @{ */

/*
 * Nautilus version 3.5
 *
 * Any code and mechanism in this module may not be used
 * in any form without permissions.
 * Copyright (C) 2007,2008,2009,2010 Sony Corporation, All Rights Reserved.
 */

#ifndef NSPPPROC_API_H
#define NSPPPROC_API_H

#define MINIMUM_REQUESTBUFFERSIZE 256

NP_BEGIN_EXTERN_C

/**
 * @file
 * nsppproc library
 *
 * depends on:
 *      - ppproc_types.h
 *
 * @see API.reference.pdf
 */

extern np_ret_t
nsppproc_Init( np_handle_t io_handle );

extern np_ret_t
nsppproc_Fin( np_handle_t io_handle );

/*---------------------------------------------------------------------------*/
/**
 ** @brief Create Request Payload
 **
 ** @param[in,out] io_handle  PPProc handle and target info
 ** @param[in,out] io_len     Length of payload
 ** @param[out]    out_buf    Payload text
 ** @param[in]     in_usage   usage
 **
 ** @return ::ERR_NP_OK
 ** @return ::ERR_NP_ILLEGALARGS
 ** @return ::ERR_NP_ILLEGALHANDLE
 ** @return ::ERR_NP_PPPROC_SHORTOFBUFFERSIZE
 ** @return ::nssmi_MsgSend
 ** @return ::RET( smi_AssertParamsSize
 ** @return ::smi_ReadParamAsStr
 ** @return ::ppproc_GenerateRequest
 **/
/*---------------------------------------------------------------------------*/
extern np_ret_t
nsppproc_GenerateRequest( np_handle_t  io_handle,
                          u_int32_t   *io_len,
                          u_int8_t    *out_buf,
                          u_int32_t    in_usage );

extern np_ret_t
nsppproc_ParseResponse( np_handle_t  io_handle,
                        u_int32_t    in_len,
                        u_int8_t    *in_buf );

NP_END_EXTERN_C

#endif /* NSPPPROC_API_H */
/** @} */
