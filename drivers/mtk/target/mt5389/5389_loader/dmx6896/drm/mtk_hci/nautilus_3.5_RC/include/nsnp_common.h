/*
 * Nautilus version 3.5
 *
 * Any code and mechanism in this module may not be used
 * in any form without permissions.
 * Copyright (C) 2008,2009,2010 Sony Corporation, All Rights Reserved.
 */

/**
 * @addtogroup nsnp_common Common Nautilus API functions
 *  @ingroup nsnp
 * @{
 *
 * @brief nsnp APIs used in all instances (DRM, transfer, etc)
 *
 * @file
 * Common APIs for nsnp layer
 *
 * nsnp operates on a number of "protcols" such as @ref nsnp_drm "drm", @ref dorado "broadband", @ref pavo "license transfer".
 * Each of these protocols have functionality in common and use the APIs defined in this module.
 *
 * @see API.reference.pdf
 */


#ifndef NSNP_COMMON_API_H
#define NSNP_COMMON_API_H

NP_BEGIN_EXTERN_C

extern np_ret_t
nsnp_Init(void      *in_secmembody,
          np_size_t  in_secmemsize,
          void      *in_nsecmembody,
          np_size_t  in_nsecmemsize);

extern np_ret_t
nsnp_Fin(void);

extern np_ret_t
nsnp_CheckPersonality(np_handle_t in_handle, int in_type);

extern np_ret_t
nsnp_CheckDCSAssertion(np_handle_t in_handle, np_size_t in_dcsnlen, u_int8_t *in_dcsnbuf, np_str_t *in_validity);

/*---------------------------------------------------------------------------*/
/**
 * @brief Get Node ID
 *
 * @param[in]  in_handle      @ref example_smi "SMI" target info
 * @param[in]  in_node        Buffer of node(XML format)
 * @param[out] out_nodeid     Buffer where the NodeID will be stored
 *
 * @return ::ERR_NP_OK                 <br>Indicating success
 * @return ::ERR_NP_OUTOFMEMORY        <br>Unable to allocate memory
 * @return ::ERR_NP_ILLEGALARGS        <br>NULL was specified in in_node or out_nodeid
 * @return ::ERR_NP_ILLEGALNODE        <br>Illegal node
 * @return ::ERR_NP_ILLEGALHANDLE      <br>The @ref example_smi "SMI" handle is invalid
 * @return ::ERR_NP_SMI_MSG_CORRUPTED  <br>SMI communication failed.
 */
/*---------------------------------------------------------------------------*/
extern np_ret_t
nsnp_GetNodeID(np_handle_t in_handle, np_str_t *in_node, np_str_t **out_nodeid);

extern np_ret_t
nsnp_FreeString(np_str_t *io_string);

/*---------------------------------------------------------------------------*/
/**
 * @brief Allocate and initialize a certification standard structure
 *
 * @note The validity string is optional.
 *
 * @note For the <i>in_mandatory</i> flag a zero indicates a <b>MUST</b> value, non-zero
 * indicates <b>SHOULD</b>.
 *
 * @param[in]  in_dcsn_len      Data certification standard name length
 * @param[in]  in_dcsn_buf      Data certification standard name contents
 * @param[in]  in_validity_len  Certification standard validity string length
 * @param[in]  in_validity_buf  Certification standard validity string contents
 * @param[in]  in_mandatory     Mandatory flag
 * @param[out] out_certstd      Certification standard structure
 *
 * @return ::ERR_NP_OK            <br>Indicating success
 * @return ::ERR_NP_ILLEGALARGS   <br>The input arguments were not legal. You must specify
 *                                    valid non-NULL, non-zero length buffer for data certification standard
 *                                    name and the output argument must be non-NULL.
 * @return ::ERR_NP_OUTOFMEMORY   <br>Unable to allocate memory for the output certification standard.
 */
/*---------------------------------------------------------------------------*/
extern np_ret_t
nsnp_AllocCertStandard(np_size_t in_dcsn_len, u_int8_t *in_dcsn_buf,
                       np_size_t in_validity_len, u_int8_t * in_validity_buf,
                       u_int8_t in_mandatory, np_certstandard_t ** out_certstd);

/*---------------------------------------------------------------------------*/
/**
 * @brief Free a certification standard structure
 *
 * @note io_certstd is not set to NULL by this function. Calling this function twice 
 * with the same arguments has undefined behaviour.
 *
 * @param[in,out] io_certstd    Certification standard structure
 *
 * @return ::ERR_NP_OK            <br>Indicating success
 */
/*---------------------------------------------------------------------------*/
extern np_ret_t
nsnp_FreeCertStandard(np_certstandard_t * io_certstd);

extern np_ret_t
nsnp_FreeESB(np_esb_t *io_esb);

/**
 * @brief Check if CRL is up-to-date
 *
 * This function checks if the current CRL is up-to-date or not. If it is,
 * nothing happens and the function quits. Otherwise it tries to download
 * a new CRL using a given function and a given URL.
 *
 * @note if there is no CRL set, it first of all tries to load it from
 *       the Nautilus' store and check if the loaded CRL is valid or not.
 * @note if an error occurs while retrieving a CRL from a distribution point
 *       the old CRL is not removed from the system even if it is invalid.
 *
 * @param[in] in_handle  @ref example_smi "SMI" target info
 * @param[in] in_get A pointer to a function which will fetches a CRL from
 *                   a given URL. The prototype for this function is 
 *                   <br><br><tt>(void *, const char*, u_int8_t**, np_size_t*)</tt>
 *                   <br><br>The function parameters are as follows:
 *                   <ol>
 *                   <li> Void pointer to an optional structure, implementation dependent
 *                   <li> A pointer to a URL where a new CRL has to be
 *                      retrieved.
 *                   <li> A pointer to a buffer which will hold a pointer
 *                      to a buffer containing fetched CRL.
 *                   <li> A pointer to a buffer which will hold size of the
 *                      fetched CRL.
 *                   </ol>
 *                   This function should return 0 in case of success and
 *                   ~0 otherwise.
 * @param[in] in_free A pointer to a deallocation function that frees memory allocated
 *                    in the <i>in_get</i> function.
 *                   The function parameters are as follows:
 *                   <ol>
 *                   <li> Void pointer to an optional structure, implementation dependent
 *                   <li> A pointer to the buffer filled by the call to <i>in_get</i>
 *                   </ol>
 * @param[in] in_url A URL which should be used in case a new CRL has to be
 *                   retrieved.
 * @param[in] in_ptr Pointer to the first parameter for the function in_get
 *
 * @return ::ERR_NP_OK                  <br>Indicating success
 * @return ::ERR_NP_INTERNALERROR       <br>An unrecoverable error has occurred in Nautilus.
 * @return ::ERR_NP_ILLEGALHANDLE       <br>The @ref example_smi "SMI" handle is invalid
 * @return ::ERR_NP_SMI_MSG_CORRUPTED   <br>SMI communication failed.
 */
extern np_ret_t
nsnp_CheckCRL(np_handle_t in_handle,
              int32_t (*in_get)(void *, const char*, u_int8_t**, np_size_t*),
              int32_t (*in_free)(void *, u_int8_t*),
              const char* in_url,
              void *in_ptr);

/**
 * This function sets a new CRL in the system.
 * This function parses the CRL it, saving it in
 * the Nautilus registry and setting it as the current CRL.
 * @note The CRL must be in DER format.
 *
 * @param[in] in_handle  @ref example_smi "SMI" target info
 * @param[in] in_crl     A pointer to a string type containing a CRL.
 *
 * @return ::ERR_NP_OK                  <br>Indicating success.
 * @return ::ERR_NP_INTERNALERROR       <br>An unrecoverable error has occurred in Nautilus.
 * @return ::ERR_NP_ILLEGALHANDLE       <br>The @ref example_smi "SMI" handle is invalid
 * @return ::ERR_NP_SMI_MSG_CORRUPTED   <br>SMI communication failed.
 */
extern np_ret_t
nsnp_UpdateCRL(np_handle_t in_handle, np_str_t* in_crl);

/**
 * This function gets a CRL distribution point from a certificate chain,
 *
 * @param[in] in_handle           @ref example_smi "SMI" target info
 * @param[in] in_certchain_b64enc A pointer to a buffer containing a certchain
 *                                encoded as base64.
 * @param[out] out_cr_dp          A pointer to a string containing the distribution point URL.
 *
 * @return ::ERR_NP_OK                  <br>Indicating success
 * @return ::ERR_NP_INTERNALERROR       <br>An unrecoverable error has occurred in Nautilus.
 * @return ::ERR_NP_ILLEGALARGS         <br>The input arguments are incorrect. Arguments cannot be NULL.
 * @return ::ERR_NP_ILLEGALCERTSET      <br>The certificate chain failed validation.
 * @return ::ERR_NP_OUTOFMEMORY         <br>A resource failure has occurred.
 * @return ::ERR_NP_ILLEGALHANDLE       <br>The @ref example_smi "SMI" handle is invalid
 * @return ::ERR_NP_SMI_MSG_CORRUPTED   <br>SMI communication failed.
 */
extern np_ret_t
nsnp_GetCRLDistributionPoint(np_handle_t in_handle,
                             np_str_t*  in_certchain_b64enc,
                             np_str_t** out_cr_dp);

extern np_ret_t
nsnp_GenerateDomainID(np_str_t*  in_accountid,
                      np_str_t*  in_subscriptionid,
                      np_str_t** out_domainid);

extern np_ret_t
nsnp_InitAccountSession(np_handle_t in_handle, np_str_t* in_account);

extern np_ret_t
nsnp_FinAccountSession(np_handle_t in_handle);

extern np_ret_t
nsnp_CheckSdataStatus(np_handle_t in_handle, np_str_t *in_accountid, u_int32_t *out_status);

extern np_ret_t
nsnp_ClearRegistrationStatus(np_handle_t in_handle, np_str_t *in_accountid);

extern np_ret_t
nsnp_RecoverSdata(np_handle_t in_handle, np_str_t *in_accountid);

extern np_ret_t
nsnp_FreeSoapFault(np_faultinfo_t *io_info);

typedef struct {
    np_str_t   *ck_uid;
    u_int32_t   nb_content_refs;
    np_str_t   *content_ref_uids;
} lic_protector_t;

typedef struct {
    np_str_t         *uid;
    u_int32_t         nb_protectors;
    lic_protector_t  *protectors;
} lic_metadata_t;

extern np_ret_t
nsnp_GetLicenseMetadata(np_size_t         in_licensesize,
                        u_int8_t         *in_licensebuf,
                        lic_metadata_t  **out_metadata);

extern np_ret_t
nsnp_FreeLicenseMetadata(lic_metadata_t *io_metadata);

/**
 * @brief Frees the result of an action in an Action Token
 *
 * @param[in,out]   io_result   Action result to be freed
 */
extern void
nsnp_FreeActionResult(np_action_result_t *io_result);

extern np_ret_t
nsnp_GetDUIDParameters(np_handle_t     in_handle,
                       u_int32_t       in_cinfo,
                       np_str_t      **out_buf);


NP_END_EXTERN_C

#endif /* NSNP_COMMON_API_H */
/** @} */
