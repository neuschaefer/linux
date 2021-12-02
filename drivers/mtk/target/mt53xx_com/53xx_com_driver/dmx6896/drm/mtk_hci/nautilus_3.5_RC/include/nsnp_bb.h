/*
 * Nautilus version 3.5
 *
 * Any code and mechanism in this module may not be used
 * in any form without permissions.
 * Copyright (C) 2008,2009,2010 Sony Corporation, All Rights Reserved.
 */

/** @addtogroup nsnp_bb Marlin Broadband functionality
 *  @ingroup nsnp
 * @{
 *
 * @brief APIs for Marlin Broadband specification
 *
 * @file
 * Broadband APIs
 *
 * @note For NEMO messaging it is recommended that the @ref dorado module is used instead.
 */

#ifndef NSNP_BB_API_H
#define NSNP_BB_API_H

NP_BEGIN_EXTERN_C

/*---------------------------------------------------------------------------*/
/**
 * @brief Initialize BB protocol handle
 *
 * @param[in,out] io_handle        BB handle to be set and SMI target info
 * @param[in]     in_protocoltype  BB protocol type
 * @param[in]     in_certstdlst    CertificationStandard list (OPTIONAL)
 *
 * @return ::ERR_NP_OK
 * @return ::ERR_NP_ILLEGALARGS
 * @return ::ERR_NP_OUTOFMEMORY
 *
 * @note in_certstdlst should be NULL for DCS and DUS protocol handles. These
 *       protocol do not require inclusion of DCSAs in the security headers
 *       of their request messages.
 */
/*---------------------------------------------------------------------------*/
extern np_ret_t
nsnp_InitBBHandle(np_handle_t             io_handle,
                  int32_t                 in_protocoltype,
                  np_certstandard_list_t *in_certstdlst);

/*---------------------------------------------------------------------------*/
/**
 * @brief Finalize a BB protocol handle
 *
 * @param[in.out] io_handle   BB handle and SMI target info
 *
 * @return ::ERR_NP_OK
 */
/*---------------------------------------------------------------------------*/
extern np_ret_t
nsnp_FinBBHandle(np_handle_t io_handle);


/*---------------------------------------------------------------------------*/
/**
 * @brief Registration service (user/subscription node acquisition) step 1:
 *        generate nodeAcquisitionRequest
 *
 * @param[in]  in_handle     BB handle and SMI target info
 * @param[in]  in_bztklen    Length of business-token
 * @param[in]  in_bztkbuf    Buffer of business-token
 * @param[in]  in_servernode Server node id
 * @param[in]  in_response   Response message to be parsed
 * @param[out] out_request   Buffer for the generated request message
 *
 * @return ::ERR_NP_OK
 * @return ::ERR_NP_ILLEGALHANDLE
 * @return ::ERR_NP_ILLEGALARGS
 * @return ::ERR_NP_OUTOFMEMORY
 * @return ::ERR_NP_INTERNALERROR
 * @return ::ERR_NP_NONEMOPERSONALITYREGISTERED   (Nemo Personality is not registered)
 * @return ::ERR_NP_ILLEGALPERSONALITY            (Nemo Personality is invalid)
 * @return ::ERR_NP_ASSERTIONNOTFOUND             (One or more DCS Assertion(s)
 *                                               are not registered)
 * @return ::ERR_NP_ILLEGALASSERTION              (Registered assertion is illegal)
 */
/*---------------------------------------------------------------------------*/
extern np_ret_t
nsnp_RegistrationNodeAcqStep1(np_handle_t io_handle,
                              np_size_t in_bztklen, u_int8_t *in_bztkbuf,
                              char *in_servernode, np_str_t **out_request);
/*---------------------------------------------------------------------------*/
/**
 * @brief Registration service (user/subscription node acquisition) step 2:
 *        parse nodeAcquisitionResponse and return user-node
 *
 * @param[in]  in_handle     BB handle and SMI target info
 * @param[in]  in_response   Response message to be parsed
 * @param[out] out_nodeid    Buffer where the nodeid will be stored
 *
 * @return ::ERR_NP_OK
 * @return ::ERR_NP_ILLEGALHANDLE
 * @return ::ERR_NP_ILLEGALARGS
 * @return ::ERR_NP_OUTOFMEMORY
 * @return ::ERR_NP_INTERNALERROR
 * @return ::ERR_NP_MESSAGECORRUPTED      (response message is corrupted)
 * @return ::ERR_NP_SOAPFAULT             (response is a soap-fault)
 */
/*---------------------------------------------------------------------------*/

extern np_ret_t
nsnp_RegistrationNodeAcqStep2(np_handle_t io_handle,
                              np_str_t *in_response, np_str_t **out_nodeid);

/*---------------------------------------------------------------------------*/
/**
 * @brief Registration service (acquire link) step 1:
 *        generate linkAcquisitionRequest
 *
 * @param[in]  in_handle        BB handle and SMI target info
 * @param[in]  in_bztklen       Length of business-token
 * @param[in]  in_bztkbuf       Buffer of business-token
 * @param[in]  in_servernode    Server node id
 * @param[in]  in_linkfrom      Source link node (NULL if LinkFrom is device node)
 * @param[in]  in_linkto        Destination link node
 * @param[out] out_request      Buffer for the generated request message
 *
 * @return ::ERR_NP_OK
 * @return ::ERR_NP_ILLEGALHANDLE
 * @return ::ERR_NP_ILLEGALARGS
 * @return ::ERR_NP_OUTOFMEMORY
 * @return ::ERR_NP_INTERNALERROR
 * @return ::ERR_NP_NONEMOPERSONALITYREGISTERED   (Nemo Personality is not registered)
 * @return ::ERR_NP_ILLEGALPERSONALITY            (Illegal Octopus or Nemo Personality)
 * @return ::ERR_NP_ASSERTIONNOTFOUND             (One or more DCS Assertion(s) are
 *                                               not registered)
 * @return ::ERR_NP_ILLEGALASSERTION              (Illegal Assertion)
 * @return ::ERR_NP_NOPERSONALITYREGISTERED       (Octpus Personality is not registered)
 * @return ::ERR_NP_ILLEGALNODE                   (Illegal linkfrom/linkto node)
 */
/*---------------------------------------------------------------------------*/
extern np_ret_t
nsnp_RegistrationLinkAcqStep1(np_handle_t io_handle,
                              np_size_t in_bztklen, u_int8_t *in_bztkbuf,
                              char *in_servernode,
                              np_str_t *in_linkfrom, np_str_t *in_linkto,
                              np_str_t **out_request);

/*---------------------------------------------------------------------------*/
/**
 * @brief Registration service (acquire link) step 2:
 *        parse linkAcquisitionResponse and return the link-id and
 *        generate the confirmation message
 *
 * @param[in]  in_handle         BB handle and SMI target info
 * @param[in]  in_response       response message to be parsed
 * @param[out] out_linkid        buffer where the link-ID(urn) will be stored
 * @param[out] out_confirmation  buffer for generated confirmation message
 *
 * @return ::ERR_NP_OK
 * @return ::ERR_NP_ILLEGALHANDLE
 * @return ::ERR_NP_ILLEGALARGS
 * @return ::ERR_NP_OUTOFMEMORY
 * @return ::ERR_NP_INTERNALERROR
 * @return ::ERR_NP_MESSAGECORRUPTED      (response is corrupted)
 * @return ::ERR_NP_SOAPFAULT             (response is a soap-fault)
 * @return ::ERR_NP_ILLEGALLINK           (illegal link)
 * @return ::ERR_NP_ALREADYREGISTERED     (link with given id is already registered)
 */
/*---------------------------------------------------------------------------*/
extern np_ret_t
nsnp_RegistrationLinkAcqStep2(np_handle_t io_handle,
                              np_str_t *in_response,
                              np_str_t **out_linkid,
                              np_str_t **out_confirmation);

/*---------------------------------------------------------------------------*/
/**
 * @brief License acquisition service step 1:
 *        generate licenseAcquisitionRequest
 *
 * @param[in]  in_handle     BB handle and SMI target info
 * @param[in]  in_bztklen    Length of business-token
 * @param[in]  in_bztkbuf    Buffer of business-token
 * @param[in]  in_servernode Server node id
 * @param[out] out_request   Buffer for generated request message
 *
 * @return ::ERR_NP_OK
 * @return ::ERR_NP_ILLEGALHANDLE
 * @return ::ERR_NP_ILLEGALARGS
 * @return ::ERR_NP_OUTOFMEMORY
 * @return ::ERR_NP_INTERNALERROR
 * @return ::ERR_NP_NONEMOPERSONALITYREGISTERED   (Nemo Personality is not registered)
 * @return ::ERR_NP_ILLEGALPERSONALITY            (Illegal Octopus or Nemo Personality)
 * @return ::ERR_NP_ASSERTIONNOTFOUND             (One or more DCS Assertion(s) are
 *                                               not registered)
 * @return ::ERR_NP_ILLEGALASSERTION              (Illegal Assertion)
 * @return ::ERR_NP_NOPERSONALITYREGISTERED       (Octopus Personality is not registered)
 * @return ::ERR_NP_ILLEGALNODE                   (Illegal node)
 */
/*---------------------------------------------------------------------------*/
extern np_ret_t
nsnp_LicenseAcqStep1(np_handle_t io_handle,
                     np_size_t in_bztklen, u_int8_t *in_bztkbuf,
                     char *in_servernode,
                     np_str_t *in_nodeid, np_str_t **out_request);

/*---------------------------------------------------------------------------*/
/**
 * @brief License acquisition service step 2:
 *        parse licenseAcquisitionResponse and return the license
 *
 * @param[in]  in_handle     BB handle and SMI target info
 * @param[in]  in_response   Response message to be parsed
 * @param[out] out_secbuf    Buffer where the license(XML format) will be stored
 *
 * @return ::ERR_NP_OK
 * @return ::ERR_NP_ILLEGALHANDLE
 * @return ::ERR_NP_ILLEGALARGS
 * @return ::ERR_NP_OUTOFMEMORY
 * @return ::ERR_NP_INTERNALERROR
 * @return ::ERR_NP_MESSAGECORRUPTED      (response is corrupted)
 * @return ::ERR_NP_SOAPFAULT             (response is a soap-fault)
 */
/*---------------------------------------------------------------------------*/
extern np_ret_t
nsnp_LicenseAcqStep2(np_handle_t io_handle,
                     np_str_t *in_response, np_str_t **out_license);

/*---------------------------------------------------------------------------*/
/**
 * @brief Data Certification service step 1:
 *        generate DataCertificationRequest
 *
 * @param[in]  in_handle       BB handle and SMI target info
 * @param[in]  in_servernode   Server node id
 * @param[in]  in_crt_stds     A list of Certification Standards (together with
 *                             metadata items) which have to be included in the
 *                             request.
 * @param[in]  in_num_crt_stds A number of Certification Standards.
 * @param[in]  in_response     response message to be parsed
 * @param[out] out_request     Buffer for generated request message
 *
 * @return ::ERR_NP_OK
 * @return ::ERR_NP_ILLEGALHANDLE
 * @return ::ERR_NP_ILLEGALARGS
 * @return ::ERR_NP_OUTOFMEMORY
 * @return ::ERR_NP_INTERNALERROR
 * @return ::ERR_NP_NONEMOPERSONALITYREGISTERED   (Nemo Personality is not registered)
 * @return ::ERR_NP_ILLEGALPERSONALITY            (Illegal nemo Personality)
 * @return ::ERR_NP_ILLEGALSL                     (Illegal SuspensionList)
 */
/*---------------------------------------------------------------------------*/
extern np_ret_t
nsnp_DataCertificationStep1(np_handle_t io_handle,
                            char          *in_servernode,
                            np_crt_std_t  *in_crt_stds,
                            np_size_t     in_num_crt_stds,
                            np_str_t      **out_request);

/*---------------------------------------------------------------------------*/
/**
 * @brief Data Certification service step 2:
 *        parse DataCertificationResponse
 *
 * @param[in] in_handle     BB handle and SMI target info
 * @param[in] in_response   response message to be parsed
 *
 * @return ::ERR_NP_OK
 * @return ::ERR_NP_ILLEGALHANDLE
 * @return ::ERR_NP_ILLEGALARGS
 * @return ::ERR_NP_OUTOFMEMORY
 * @return ::ERR_NP_INTERNALERROR
 * @return ::ERR_NP_MESSAGECORRUPTED      (response is corrupted)
 * @return ::ERR_NP_SOAPFAULT             (response is a soap-fault)
 * @return ::ERR_NP_NEEDTTUPDATE          (TrustedTime should be updated)
 * @return ::ERR_NP_NEEDSLUPDATE          (SuspensionList should be updated)
 * @return ::ERR_NP_NEEDUPDATE            (TrustedTime and SuspensionList should be updated)
 */
/*---------------------------------------------------------------------------*/
extern np_ret_t
nsnp_DataCertificationStep2(np_handle_t io_handle,
                            np_str_t *in_response);

/*---------------------------------------------------------------------------*/
/**
 * @brief Data Update service step 1:
 *        generate DataUpdateRequest
 *
 * @param[in]  in_handle      BB handle and SMI target info
 * @param[in]  in_servernode  Server node id
 * @param[in]  in_updateflags Specified which data will be updated
 *                             (NPBB_DUS_UPDATE_TT|NPBB_DUS_UPDATE_LSL)
 * @param[out] out_request    Buffer for generated request message
 *
 * @return ::ERR_NP_OK
 * @return ::ERR_NP_ILLEGALHANDLE
 * @return ::ERR_NP_ILLEGALARGS
 * @return ::ERR_NP_OUTOFMEMORY
 * @return ::ERR_NP_INTERNALERROR
 * @return ::ERR_NP_NONEMOPERSONALITYREGISTERED   (Nemo Personality is not registered)
 * @return ::ERR_NP_ILLEGALPERSONALITY            (Illegal nemo Personality)
 * @return ::ERR_NP_ILLEGALSL                     (Illegal SuspensionList)
 */
/*---------------------------------------------------------------------------*/
extern np_ret_t
nsnp_DataUpdateStep1(np_handle_t io_handle,
                     char *in_servernode, int32_t in_updateflags,
                     np_str_t **out_request);

/*---------------------------------------------------------------------------*/
/**
 * @brief Data Update service step 2:
 *        parse DataUpdateResponse
 *
 * @param[in] in_handle     BB handle and SMI target info
 * @param[in] in_response   response message to be parsed
 *
 * @return ::ERR_NP_OK
 * @return ::ERR_NP_ILLEGALHANDLE
 * @return ::ERR_NP_ILLEGALARGS
 * @return ::ERR_NP_OUTOFMEMORY
 * @return ::ERR_NP_INTERNALERROR
 * @return ::ERR_NP_MESSAGECORRUPTED      (response is corrupted)
 * @return ::ERR_NP_SOAPFAULT             (response is a soap-fault)
 * @return ::ERR_NP_ILLEGALSL             (Illegal SuspensionList)
 */
/*---------------------------------------------------------------------------*/
extern np_ret_t
nsnp_DataUpdateStep2(np_handle_t io_handle,
                     np_str_t *in_response);

/*---------------------------------------------------------------------------*/
/**
 * @brief Registration Link Release service step 1:
 *        generate LinkReleaseRequest
 *
 * @param[in]  in_handle     BB handle and SMI target info
 * @param[in]  in_bztklen    Length of business-token
 * @param[in]  in_bztkbuf    Buffer of business-token
 * @param[in]  in_servernode Server node id
 * @param[in]  in_linkfrom   Link source
 * @param[in]  in_linkto     Link destination
 * @param[out] out_request   Buffer for generated request message
 *
 * @return ::ERR_NP_OK
 * @return ::ERR_NP_ILLEGALHANDLE
 * @return ::ERR_NP_ILLEGALARGS
 * @return ::ERR_NP_OUTOFMEMORY
 * @return ::ERR_NP_INTERNALERROR
 * @return ::ERR_NP_NONEMOPERSONALITYREGISTERED   (Nemo Personality is not registered)
 * @return ::ERR_NP_ILLEGALPERSONALITY            (Illegal Octopus or Nemo Personality)
 * @return ::ERR_NP_ASSERTIONNOTFOUND             (One or more DCS Assertion(s) are
 *                                               not registered)
 * @return ::ERR_NP_ILLEGALASSERTION              (Illegal Assertion)
 * @return ::ERR_NP_NOLINKREGISTERED              (Link)
 */
/*---------------------------------------------------------------------------*/
extern np_ret_t
nsnp_DeregistrationStep1(np_handle_t io_handle,
                         np_size_t in_bztklen, u_int8_t *in_bztkbuf,
                         char *in_servernode, np_str_t *in_linkid,
                         np_str_t *in_linkfrom, np_str_t *in_linkto,
                         np_str_t **out_request);

/*---------------------------------------------------------------------------*/
/**
 * @brief Registration Link Release service step 2:
 *        parse LinkReleaseResponse
 *        generate LinkReleaseConfirmation
 *
 * @param[in]  in_handle         BB handle and SMI target info
 * @param[in]  in_response       Response message to be parsed
 * @param[out] out_confirmation  Buffer for generated confirmation message
 *
 * @return ::ERR_NP_OK
 * @return ::ERR_NP_ILLEGALHANDLE
 * @return ::ERR_NP_ILLEGALARGS
 * @return ::ERR_NP_OUTOFMEMORY
 * @return ::ERR_NP_INTERNALERROR
 * @return ::ERR_NP_MESSAGECORRUPTED      (response is corrupted)
 * @return ::ERR_NP_SOAPFAULT             (response is a soap-fault)
 * @return ::ERR_NP_NOLINKREGISTERED      (Link)
 */
/*---------------------------------------------------------------------------*/
extern np_ret_t
nsnp_DeregistrationStep2(np_handle_t io_handle,
                         np_str_t *in_response,
                         np_str_t **out_confirmation);

/*---------------------------------------------------------------------------*/
/**
 * @brief Get the Soap fault Information
 *   If ::ERR_NP_SOAPFAULT is returned by BB APIs,
 *   get information about the Soap-fault using this function.
 *
 * @param[in]  in_handle    BB handle and SMI target info
 * @param[out] out_info     Soap fault information
 *
 * @return ::ERR_NP_OK
 * @return ::ERR_NP_ILLEGALHANDLE
 * @return ::ERR_NP_ILLEGALARGS
 * @return ::ERR_NP_OUTOFMEMORY
 * @return ::ERR_NP_INTERNALERROR
 * @return ::ERR_NP_NOSOAPFAULT           (No Soap-fault information available)
 */
/*---------------------------------------------------------------------------*/
extern np_ret_t
nsnp_GetBBSoapFault(np_handle_t  io_handle,
                    np_faultinfo_t **out_info);

/*---------------------------------------------------------------------------*/
/**
 * Set encryption and signing certificates in the BB handle
 *
 * @param[in] in_handle         NPBB handle and SMI target info
 * @param[in] in_enccertsetlen  Length of certset
 * @param[in] in_enccertsetbuf  Buffer of certset
 * @param[in] in_signcertsetlen Length of certset
 * @param[in] in_signcertsetbuf Buffer of certset
 * @param[in] in_roleastlen     length of service role assertion
 * @param[in] in_roleastbuf     buffer of service role assertion
 * @return ::ERR_NP_OK
 * @return ::ERR_NP_ILLEGALARGS
 * @return ::ERR_NP_OUTOFMEMORY
 * @return ::ERR_NP_ILLEGALCERTSET
 */
/*---------------------------------------------------------------------------*/
extern np_ret_t
nsnp_SetBBCertSet(np_handle_t io_handle,
                  np_size_t in_enccertsetlen,
                  u_int8_t *in_enccertsetbuf,
                  np_size_t in_sigcertsetlen,
                  u_int8_t *in_sigcertsetbuf,
                  np_size_t in_roleastlen,
                  u_int8_t *in_roleastbuf);

/*---------------------------------------------------------------------------*/
/**
 * Sets svcid and sinfo for use by link registration
 *
 * @param[in] in_handle         NPBB handle and SMI target info
 * @param[in] in_svcid          broadband service id
 * @param[in] in_sinfo          service info
 * @return ::ERR_NP_OK
 * @return ::ERR_NP_ILLEGALARGS
 * @return ::ERR_NP_OUTOFMEMORY
 */
/*---------------------------------------------------------------------------*/
extern np_ret_t
nsnp_SetSid(
    np_handle_t io_handle,
    np_str_t   *in_svcid,
    np_str_t   *in_sinfo);

NP_END_EXTERN_C

#endif /* NSNP_BB_API_H */
/** @} */
