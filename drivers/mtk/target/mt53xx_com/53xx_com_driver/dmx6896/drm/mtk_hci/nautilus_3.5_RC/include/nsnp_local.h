/*
 * Nautilus version 3.5
 *
 * Any code and mechanism in this module may not be used
 * in any form without permissions.
 * Copyright (C) 2008,2009,2010,2011 Sony Corporation, All Rights Reserved.
 */

/** @addtogroup nsnp_local nsnp_local
 *  @ingroup nsnp
 * @{
 */


#ifndef NSNP_LOCAL_API_H
#define NSNP_LOCAL_API_H

NP_BEGIN_EXTERN_C


/*---------------------------------------------------------------------------*/
/**
 * @brief Register NEMO public keys
 *
 * @param[in] in_handle      Component handle and SMI target info
 * @param[in] in_size   Length of the NEMO public keys
 * @param[in] in_buf    Buffer of the NEMO public keys
 *
 * @return ::ERR_NP_OK
 * @return ::ERR_NP_ILLEGALARGS        given size is 0 or given buf is NULL
 * @return ::ERR_NP_ALREADYREGISTERED
 * @return ::ERR_NP_INTERNALERROR
 */
/*---------------------------------------------------------------------------*/
extern np_ret_t
nsnp_RegistNEMOPublicKeys(np_handle_t in_handle,
                          np_size_t in_size,
                          u_int8_t  *in_buf);

/*---------------------------------------------------------------------------*/
/**
 * @brief Register NEMO private keys
 *
 * @param[in] in_handle      Component handle and SMI target info
 * @param[in] in_size   Length of the NEMO private keys
 * @param[in] in_buf    Buffer of the NEMO private keys
 *
 * @return ::ERR_NP_OK
 * @return ::ERR_NP_ILLEGALARGS        given size is 0 or given buf is NULL
 * @return ::ERR_NP_ALREADYREGISTERED
 * @return ::ERR_NP_INTERNALERROR
 */
/*---------------------------------------------------------------------------*/
extern np_ret_t
nsnp_RegistNEMOPrivateKeys(np_handle_t in_handle,
                           np_size_t in_size,
                           u_int8_t  *in_buf);

/*---------------------------------------------------------------------------*/
/**
 * @brief Register role assertion
 *
 * @param[in] in_handle      Component handle and SMI target info
 * @param[in] in_size   Length of the role assertion
 * @param[in] in_buf    Buffer of the role asserion
 *
 * @return ::ERR_NP_OK
 * @return ::ERR_NP_ILLEGALARGS        given size is 0 or given buf is NULL
 * @return ::ERR_NP_ALREADYREGISTERED
 * @return ::ERR_NP_INTERNALERROR
 */
/*---------------------------------------------------------------------------*/
extern np_ret_t
nsnp_RegistRoleAssertion(np_handle_t in_handle,
                         np_size_t in_size,
                         u_int8_t  *in_buf);

/*---------------------------------------------------------------------------*/
/**
 * @brief Register a public octopus personality
 *
 * @param[in] in_handle      Component handle and SMI target info
 * @param[in] in_pubsize  Length of the public personality XML
 * @param[in] in_pubbuf   Buffer of the public personality XML
 *
 * @return ::ERR_NP_OK
 * @return ::ERR_NP_ILLEGALARGS        given size is 0 or buf is NULL
 * @return ::ERR_NP_ALREADYREGISTERED
 * @return ::ERR_NP_INTERNALERROR
 */
/*---------------------------------------------------------------------------*/
extern np_ret_t
nsnp_RegistPublicPersonality(np_handle_t in_handle,
                             np_size_t in_pubsize,
                             u_int8_t *in_pubbuf);

/*---------------------------------------------------------------------------*/
/**
 * @brief Register a private octopus personality
 *
 * @param[in] in_handle      Component handle and SMI target info
 * @param[in] in_privsize Length of the private personality XML
 * @param[in] in_privbuf  Buffer of the private personality XML
 *
 * @return ::ERR_NP_OK
 * @return ::ERR_NP_ILLEGALARGS        given size is 0 or given buf is NULL
 * @return ::ERR_NP_ALREADYREGISTERED
 * @return ::ERR_NP_INTERNALERROR
 */
/*---------------------------------------------------------------------------*/
extern np_ret_t
nsnp_RegistPrivatePersonality(np_handle_t in_handle,
                              np_size_t in_privsize,
                              u_int8_t *in_privbuf);

/*---------------------------------------------------------------------------*/
/**
 * @brief Register a dcs assertion.
 *
 * @param[in] in_handle      Component handle and SMI target info
 * @param[in] in_dcsnsize  Length of the data certification standard name
 * @param[in] in_dcsnbuf   Buffer of the data certification standard name
 * @param[in] in_size      Length of the dcs assertion XML
 * @param[in] in_buf       Buffer of the dcs assertion XML
 *
 * @return ::ERR_NP_OK
 * @return ::ERR_NP_ILLEGALARGS        given size is 0 or given buf is NULL
 * @return ::ERR_NP_OUTOFMEMORY
 * @return ::ERR_NP_INTERNALERROR
 * @return ::ERR_NP_NOTSUPPORTED
 */
/*---------------------------------------------------------------------------*/
extern np_ret_t
nsnp_RegistDCSAssertion(np_handle_t in_handle,
                        np_size_t in_dcsnsize,
                        u_int8_t *in_dcsnbuf,
                        np_size_t in_size,
                        u_int8_t *in_buf);

/*---------------------------------------------------------------------------*/
/**
 * @brief Set the trusted time
 *
 * @param[in] in_handle      Component handle and SMI target info
 * @param[in] in_time    Trusted time
 *
 * @return ::ERR_NP_OK
 * @return ::ERR_NP_INTERNALERROR
 * @return ::ERR_NP_NOMEM
 * @return ::ERR_NP_ILLEGALARGS
 */
/*---------------------------------------------------------------------------*/
extern np_ret_t
nsnp_SetTrustedTime(np_handle_t in_handle, np_date_t* in_time);

/*---------------------------------------------------------------------------*/
/**
 * @brief Clear the secure data cache
 *
 * @return ::ERR_NP_OK
 */
/*---------------------------------------------------------------------------*/
extern np_ret_t
nsnp_ClearSdata(np_handle_t in_handle);

/**
 * This function returns a CRL number of the current CRL. If there is no
 * CRL set then np_cert_crlnumber_t.valid will be set to 0.
 *
 * @param[in] in_handle  @ref example_smi "SMI" target info
 *
 * @return A CRL number in a @ref np_cert_crlnumber_t structure
 */
extern np_cert_crlnumber_t
nsnp_GetCRLNumber(np_handle_t in_handle);

/**
 * This function clears the CRL.
 *
 * @param[in]  in_handle  @ref example_smi "SMI" target info
 */
extern void
nsnp_ClearCRL(np_handle_t in_handle);

NP_END_EXTERN_C

#endif /* NSNP_LOCAL_API_H */

/** @} */
