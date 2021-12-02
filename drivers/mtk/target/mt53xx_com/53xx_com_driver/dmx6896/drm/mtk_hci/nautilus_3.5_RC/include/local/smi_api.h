/** @addtogroup smi smi
 *  @ingroup cmn
 * @{ */

/*
 * Nautilus version 3.5
 *
 * Any code and mechanism in this module may not be used
 * in any form without permissions.
 * Copyright (C) 2008,2009,2010,2011 Sony Corporation, All Rights Reserved.
 */

#ifndef SMI_API_H
#define SMI_API_H

NP_BEGIN_EXTERN_C

#include "smi_types.h"

/**
 * @brief Set SMI message ID
 *
 * @param[out] out_smi  The SMI buffer to fill
 * @param[in]  in_val   The value to set in the buffer
 */
void
npi_smi_setMsgId(u_int8_t *out_smi, u_int32_t in_val);

/**
 * @brief Set SMI message size
 *
 * @param[out] out_smi  The SMI buffer to fill
 * @param[in]  in_val   The value to set in the buffer
 */
void
npi_smi_setMsgSize(u_int8_t *out_smi, u_int32_t in_val);

/**
 * @brief Set SMI return value
 *
 * @param[out] out_smi  The SMI buffer to fill
 * @param[in]  in_val   The value to set in the buffer
 */
void
npi_smi_setMsgRet(u_int8_t *out_smi, u_int32_t in_val);

/**
 * @brief Set SMI reserved field
 *
 * @param[out] out_smi  The SMI buffer to fill
 * @param[in]  in_val   The value to set in the buffer
 */
void
npi_smi_setMsgResvd(u_int8_t *out_smi, u_int32_t in_val);

/**
 * @brief Set SMI parameter type
 *
 * @param[out] out_smi  The SMI buffer to fill
 * @param[in]  in_val   The value to set in the buffer
 */
void
npi_smi_setMsgParamType(u_int8_t *out_smi, u_int32_t in_val);

/**
 * @brief Set SMI parameter size
 *
 * @param[out] out_smi  The SMI buffer to fill
 * @param[in]  in_val   The value to set in the buffer
 */
void
npi_smi_setMsgParamSize(u_int8_t *out_smi, u_int32_t in_val);

/**
 * @brief Get SMI message ID
 *
 * @param[in] in_smi  The SMI buffer
 */
u_int32_t
npi_smi_getMsgId(u_int8_t *in_smi);

/**
 * @brief Get SMI message size
 *
 * @param[in] in_smi  The SMI buffer
 */
u_int32_t
npi_smi_getMsgSize(u_int8_t *in_smi);

/**
 * @brief Get SMI return value
 *
 * @param[in] in_smi  The SMI buffer
 */
u_int32_t
npi_smi_getMsgRet(u_int8_t *in_smi);

/**
 * @brief Get SMI reserved field
 *
 * @param[in] in_smi  The SMI buffer
 */
u_int32_t
npi_smi_getMsgResvd(u_int8_t *in_smi);

/**
 * @brief Get SMI parameter type
 *
 * @param[in] in_smi  The SMI buffer
 */
u_int32_t
npi_smi_getMsgParamType(u_int8_t *in_smi);

/**
 * @brief Get SMI parameter size
 *
 * @param[in] in_smi  The SMI buffer
 */
u_int32_t
npi_smi_getMsgParamSize(u_int8_t *in_smi);

/**
 * @brief Definition for a processing function used
 *        by ::npi_smi_IterateMsgParamsExec.
 */
typedef retcode_t(*msg_param_process_t)(npi_cmncontext_t in_context, u_int8_t *in_param, void *io_aggregator);

/**
 * @brief Check that a parameter list has the correct number of elements expected
 *
 * @param[in] in_params    Parameter list
 * @param[in] in_sz        Number of elemets to expect in the list
 *
 * @return ::ERR_OK
 * @return ::ERR_ILLEGALARGS
 */
extern retcode_t
npi_smi_AssertParamsSize( smi_params_t *in_params, u_int32_t in_sz );

/**
 * @brief This function can be used to construct an smi message (both requests
 *        and responses).
 *
 * @param[in]   in_context      The memory context that needs to be used.
 * @param[in]   in_msg_id       The message id of the np function that needs to
 *                                  be called.
 * @param[in]   in_msg_ret      For an smi request, ::ERR_NP_OK. For an smi
 *                                  response, the return code of the np function
 *                                  that was called.
 * @param[in]   in_params_list  A list of parameters that need to be provided to
 *                                  the np call.
 * @param[out]  out_msg         An smi message that can be sent across the
 *                                  transport layer.
 *
 * @return ::ERR_OK
 * @return ::ERR_ILLEGALARGS
 * @return ::ERR_NOMEM
 * @return ::ERR_INTERNALERROR
 */
extern retcode_t
npi_smi_PrepareMsg(npi_cmncontext_t     in_context,
                   smi_id_t             in_msg_id,
                   np_ret_t             in_msg_ret,
                   smi_params_t        *in_params_list,
                   binstr_t           **out_msg);

/**
 * @brief This function can be used to extract the parameter values from a smi
 *        message.
 *
 * @param[in]   in_context      The memory context that needs to be used.
 * @param[in]   in_msg          An smi message from which parameters need to be
 *                                  extracted.
 * @param[out]  out_params_list A list of parameters extracted from the smi
 *                                  message.
 *
 * @return ::ERR_OK
 * @return ::ERR_ILLEGALARGS
 * @return ::ERR_NOMEM
 * @return ::npi_smi_IterateMsgParams
 */
extern retcode_t
npi_smi_ExtractParamVals(npi_cmncontext_t     in_context,
                         binstr_t            *in_msg,
                         smi_params_t       **out_params_list);

#ifdef NPOPT_DECRYPT_USE_SHARED_MEMORY
/**
 * @brief This function can be used to iterate on every parameters of an SMI
 *        message. For each parameter, the specified function will be executed.
 *
 * @param[in]   in_context      The memory context that needs to be used.
 * @param[in]   in_msg          An smi message from which parameters need to be
 *                              extracted.
 * @param[in]   in_process      A function pointer. This function will be called
 *                              over each SMI parameters.
 * @param[out]  io_aggregator   An argument used by in_process function.
 *
 * @return ::ERR_OK
 * @return ::ERR_ILLEGALARGS
 * @return ::npi_smi_l_IterateMsgParams
 */
retcode_t
npi_smi_IterateMsgParamsExec(npi_cmncontext_t     in_context,
                             binstr_t            *in_msg,
                             msg_param_process_t  in_process,
                             void                *io_aggregator);
#endif

/**
 * @brief Allocate a parameter list with enough space for a specified number
 *        of parameters.
 *
 * @param[in]   in_context       The memory context in which the parameter list
 *                                  and its contents were allocated.
 * @param[in]   in_nb            The number of parameters in the list
 * @param[out]  out_params_list  The parameter list whose memory has to be
 *
 * @return ::ERR_OK
 * @return ::ERR_ILLEGALARGS
 * @return ::ERR_NOMEM
 */
extern retcode_t
npi_smi_params_Alloc(npi_cmncontext_t in_context, u_int32_t in_nb, smi_params_t **out_params_lst);

/**
 * @brief This function can be used to free a parameter list extracted from an
 *        smi message with the npi_smi_ExtractParamVals function.
 *
 * @param[in]   in_context      The memory context in which the parameter list
 *                                  and its contents were allocated.
 * @param[in]   in_params_list  The parameter list whose memory has to be
 *                                  freed.
 */
extern void
npi_smi_params_Free(npi_cmncontext_t  in_context,
                    smi_params_t     *in_params_list);

/**
 * @brief Read a SMI parameter and convert it to a np_faultinfo_t type.
 *
 * @param[in]   in_context  The memory context that needs to be used.
 * @param[in]   in_param    The parameter in the SMI packet.
 * @param[out]  out_info    The faultinfo structure to populate.
 *
 * @return ::ERR_OK
 * @return ::ERR_NOMEM
 * @return ::ERR_ILLEGALARGS
 * @return ::ERR_INTERNALERROR
 */
retcode_t
npi_smi_ReadParamAsFaultInfo(npi_cmncontext_t     in_context,
                             smi_param_val_t     *in_param,
                             np_faultinfo_t     **out_info);

/**
 * @brief Read a SMI parameter and convert it to a binstr_t type.
 *
 * @param[in]   in_context  The memory context that needs to be used.
 * @param[in]   in_param    The parameter in the SMI packet.
 * @param[out]  out_buf     The binstr_t structure to populate.
 *
 * @return ::ERR_OK
 * @return ::ERR_NOMEM
 * @return ::ERR_ILLEGALARGS
 */

retcode_t
npi_smi_ReadParamAsStr(npi_cmncontext_t   in_context,
                       smi_param_val_t   *in_param,
                       binstr_t         **out_buf);

/**
 * @brief Read a SMI parameter and convert it to a np_cert_crlnumber_t type.
 *
 * @param[in]   in_context  The memory context that needs to be used.
 * @param[in]   in_param    The parameter in the SMI packet.
 * @param[out]  out_crl     The CRL number to populate.
 *
 * @return ::ERR_OK
 * @return ::ERR_NOMEM
 * @return ::ERR_ILLEGALARGS
 */
retcode_t
npi_smi_ReadParamAsCrlNumber(npi_cmncontext_t         in_context,
                             smi_param_val_t         *in_param,
                             np_cert_crlnumber_t     *out_crl);

/**
 * @brief Read a SMI parameter and convert it to an integer type.
 *
 * @param[in]   in_context  The memory context that needs to be used.
 * @param[in]   in_param    The parameter in the SMI packet.
 * @param[out]  out_int32   The integer to populate.
 *
 * @return ::ERR_NP_OK
 * @return ::ERR_NP_OUTOFMEMORY
 * @return ::ERR_NP_ILLEGALARGS
 */
retcode_t
npi_smi_ReadParamAsInt32(npi_cmncontext_t     in_context,
                         smi_param_val_t     *in_param,
                         int32_t             *out_int32);

/**
 * @brief Read a SMI parameter and convert it to an unsigned integer type.
 *
 * @param[in]   in_context  The memory context that needs to be used.
 * @param[in]   in_param    The parameter in the SMI packet.
 * @param[out]  out_uint32  The integer to populate.
 *
 * @return ::ERR_OK
 * @return ::ERR_NOMEM
 * @return ::ERR_ILLEGALARGS
 */
retcode_t
npi_smi_ReadParamAsUInt32(npi_cmncontext_t    in_context,
                          smi_param_val_t    *in_param,
                          u_int32_t          *out_uint32);

/**
 * @brief Read a SMI parameter and convert it to a handle type.
 *
 * @param[in]   in_context  The memory context that needs to be used.
 * @param[in]   in_param    The parameter in the SMI packet.
 * @param[out]  out_hndl    The handle to populate.
 *
 * @return ::ERR_OK
 * @return ::ERR_NOMEM
 * @return ::ERR_ILLEGALARGS
 */
retcode_t
npi_smi_ReadParamAsHandle(npi_cmncontext_t     in_context,
                          smi_param_val_t     *in_param,
                          np_protocol_t       *out_hndl);

#ifdef NPOPT_DECRYPT_USE_SHARED_MEMORY
/**
 * @brief Read a SMI parameter and convert it to a pointer on u_in8_t type.
 *
 * @param[in]   in_context     The memory context that needs to be used.
 * @param[in]   in_param       The parameter in the SMI packet.
 * @param[out]  out_uint8_ptr  The pointer to populate.
 *
 * @return ::ERR_OK
 * @return ::ERR_NOMEM
 * @return ::ERR_ILLEGALARGS
 */
retcode_t
npi_smi_ReadParamAsUInt8Ptr(npi_cmncontext_t     in_context,
                            smi_param_val_t     *in_param,
                            u_int8_t           **out_uint8_ptr);
#endif

/**
 * @brief Read a SMI parameter and convert it to a boolean type.
 *
 * @param[in]   in_context  The memory context that needs to be used.
 * @param[in]   in_param    The parameter in the SMI packet.
 * @param[out]  out_bool    The boolean to populate.
 *
 * @return ::ERR_OK
 * @return ::ERR_NOMEM
 * @return ::ERR_ILLEGALARGS
 */
retcode_t
npi_smi_ReadParamAsBool(npi_cmncontext_t      in_context,
                        smi_param_val_t      *in_param,
                        np_bool_t            *out_bool);

/**
 * @brief Read a SMI parameter and convert it to a size type.
 *
 * @param[in]   in_context  The memory context that needs to be used.
 * @param[in]   in_param    The parameter in the SMI packet.
 * @param[out]  out_size    The size to populate.
 *
 * @return ::ERR_OK
 * @return ::ERR_NOMEM
 * @return ::ERR_ILLEGALARGS
 */
retcode_t
npi_smi_ReadParamAsSize(npi_cmncontext_t      in_context,
                        smi_param_val_t      *in_param,
                        np_size_t            *out_size);

/**
 * @brief Read a SMI parameter and convert it to a date type.
 *
 * @param[in]   in_context  The memory context that needs to be used.
 * @param[in]   in_param    The parameter in the SMI packet.
 * @param[out]  out_date    The date to populate.
 *
 * @return ::ERR_OK
 * @return ::ERR_NOMEM
 * @return ::ERR_ILLEGALARGS
 */
retcode_t
npi_smi_ReadParamAsDate(npi_cmncontext_t      in_context,
                        smi_param_val_t      *in_param,
                        np_date_t            *out_date);

/**
 * @brief Read a SMI parameter and convert it to a np_callback_t type.
 *
 * @param[in]   in_context      The memory context that needs to be used.
 * @param[in]   in_param        The parameter in the SMI packet.
 * @param[out]  out_callback    The callback to populate.
 *
 * @return ::ERR_OK
 * @return ::ERR_NOMEM
 * @return ::ERR_ILLEGALARGS
 */
retcode_t
npi_smi_ReadParamAsCallback(npi_cmncontext_t      in_context,
                            smi_param_val_t      *in_param,
                            np_callback_t       **out_callback);

/**
 * @brief Read a SMI parameter and convert it to a np_crt_std_list_t type.
 *
 * @param[in]   in_context      The memory context that needs to be used.
 * @param[in]   in_param        The parameter in the SMI packet.
 * @param[out]  out_crtstdlist  The crt_std list to populate.
 *
 * @return ::ERR_OK
 * @return ::ERR_NOMEM
 * @return ::ERR_ILLEGALARGS
 */
retcode_t
npi_smi_ReadParamAsCrtStdList(npi_cmncontext_t    in_context,
                              smi_param_val_t    *in_param,
                              np_crt_std_list_t **out_crtstdlist);

/**
 * @brief Read a SMI parameter and convert it to a np_certstandard_list_t type.
 *
 * @param[in]   in_context      The memory context that needs to be used.
 * @param[in]   in_param        The parameter in the SMI packet.
 * @param[out]  out_certstdlist The certstandard list to populate.
 *
 * @return ::ERR_OK
 * @return ::ERR_NOMEM
 * @return ::ERR_ILLEGALARGS
 */
retcode_t
npi_smi_ReadParamAsCertStdList(npi_cmncontext_t           in_context,
                               smi_param_val_t           *in_param,
                               np_certstandard_list_t   **out_certstdlist);

/**
 * @brief Read a SMI parameter and convert it to a np_trackinfo_t type.
 *
 * @param[in]   in_context      The memory context that needs to be used.
 * @param[in]   in_param        The parameter in the SMI packet.
 * @param[out]  out_trackinfo   The track info to populate.
 *
 * @return ::ERR_OK
 * @return ::ERR_NOMEM
 * @return ::ERR_ILLEGALARGS
 */
retcode_t
npi_smi_ReadParamAsTrackInfo(npi_cmncontext_t       in_context,
                             smi_param_val_t       *in_param,
                             np_trackinfo_t       **out_trackinfo);


#if defined( NPOPT_ENABLE_MS3 )
/**
 * @brief Read a SMI parameter and convert it to a np_ms3_sas_info_t type.
 *
 * @param[in]   in_context      The memory context that needs to be used.
 * @param[in]   in_param        The parameter in the SMI packet.
 * @param[out]  out_sasinfo     The sas info to populate.
 *
 * @return ::ERR_OK
 * @return ::ERR_NOMEM
 * @return ::ERR_ILLEGALARGS
 */

retcode_t
npi_smi_ReadParamAsSASInfo(npi_cmncontext_t     in_context,
                           smi_param_val_t     *in_param,
                           np_ms3_sas_info_t  **out_sasinfo);
#endif
NP_END_EXTERN_C

#endif /* SMI_API_H */
/** @} */
