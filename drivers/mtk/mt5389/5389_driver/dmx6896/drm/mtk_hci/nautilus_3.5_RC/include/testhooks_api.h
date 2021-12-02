/*
 * Nautilus version 3.5
 *
 * Any code and mechanism in this module may not be used
 * in any form without permissions.
 * Copyright (C) 2007,2008,2009,2010,2011 Sony Corporation, All Rights Reserved.
 */

#ifndef TESTHOOKS_H
#define TESTHOOKS_H

/*---------------------------------------------------------------------------*/
/* Store manipulation                                                        */
/*---------------------------------------------------------------------------*/

extern int32_t
hook_GetRegPersonalityType_OctPub(void);

extern int32_t
hook_GetRegPersonalityType_OctPriv(void);

extern int32_t
hook_GetRegPersonalityType_NemoPub(void);

extern int32_t
hook_GetRegPersonalityType_NemoPriv(void);

extern int32_t
hook_GetRegPersonalityType_RoleAssertion(void);

#if defined(NPOPT_USE_ENC_SDATA)
extern int32_t
hook_GetSsmemTagOctPub( void );

extern int32_t
hook_GetSsmemTagOctPriv( void );

extern int32_t
hook_GetSsmemTagNemoPub( void );

extern int32_t
hook_GetSsmemTagNemoPriv( void );

extern int32_t
hook_GetSsmemTagRoleAssertion( void );

extern int32_t
hook_GetSsmemTagTt( void );

extern int32_t
hook_GetSsmemTagCrl( void );

extern int32_t
hook_GetSsmemTagLil( void );

extern int32_t
hook_GetSsmemTagLink( void );

extern int32_t
hook_GetSsmemTagLinkMaster( void );

extern int32_t
hook_GetSsmemTagAssertion( void );

extern int32_t
hook_GetSsmemTagAssertionDcs( void );

extern int32_t
hook_GetSsmemTagLsl( void );

extern int32_t
hook_GetSsmemTagNode( void );

extern int32_t
hook_GetSsmemTagNodeMaster( void );

extern int32_t
hook_GetSsmemTagSeashell( void );
#endif

/**
 * @brief Register supspension list
 *
 * @param[in] in_handle Transport layer info
 * @param[in] in_ids_a  First set of ids
 * @param[in] in_ids_b  Second set of ids
 *
 * @return return values of ::RegisterLicenseSuspLists
 */
extern np_ret_t
hook_RegisterLicenseSuspLists(np_handle_t in_handle, char **in_ids_a, char **in_ids_b);

/**
 * @brief Register supspension list
 *
 * @param[in] in_handle     Transport layer info
 * @param[in] in_type Assertion type
 *
 * @return return values of ::reg_CheckAssertion
 */
extern np_ret_t
hook_reg_CheckAssertion(np_handle_t in_handle, int32_t in_type);

/**
 * @brief Retrieve a role assertion from the secure data cache
 *
 * @param[in]  in_handle      Transport layer info
 * @param[out] out_assertion  Assertion
 *
 * @return return values of ::reg_GetRoleAssertion
 */
extern np_ret_t
hook_reg_GetRoleAssertion(np_handle_t in_handle, np_str_t **out_assertion);


/**
 * @brief Register a link by ID
 *
 * @param[in] in_handle      Component handle and SMI target info
 * @param[in] in_toid        Destination node id for the link
 * @param[in] in_fromid      Source node id for the link
 * @param[in] in_link        Link data provided (buffer of link xml)
 * @param[in] in_svcid       Service id
 * @param[in] in_sinfo       Service info

 * @return return values of ::reg_RegLinkByID
 */
extern np_ret_t
hook_reg_RegistLinkByID(np_handle_t   in_handle,
                        np_str_t     *in_toid,
                        np_str_t     *in_fromid,
                        np_str_t     *in_link,
                        np_str_t     *in_svcid,
                        np_str_t     *in_sinfo);

/**
 * Load the secure store
 *
 * @param[in] in_handle     Transport layer info
 */
extern np_ret_t
hook_LoadSStorage(np_handle_t in_handle);

/**
 * Save the secure store
 *
 * @param[in] in_handle     Transport layer info
 */
extern np_ret_t
hook_UpdateSStorage(np_handle_t in_handle);

/**
 * Add the license controller id to the lil. Uses the
 * np_internal_LicenseIDListAppend on the sec side to do this
 *
 * @param[in] in_handle     Transport layer info
 * @param[in] in_license    License whose id is to be added to the lil
 */
extern np_ret_t
hook_LilAppend(np_handle_t in_handle, np_str_t *in_license);

/**
 * Modify values defined as #defines from ns_config.h
 * use npi_base_GetNsCfg to access and modify them
 *
 * @param[in] in_handle   Transport layer info
 * @param[in] in_index    Index
 * @param[in] in_value    new string value
 */
extern np_ret_t
hook_SetNsCfgVal(np_handle_t in_handle, u_int32_t in_index, char* in_value);

extern np_ret_t
hook_SendSmiMsg(np_handle_t        in_target,
                smi_id_t           in_msg_id,
                smi_params_t      *in_req_params_list,
                smi_params_t     **out_resp_params_list,
                np_ret_t          *out_resp_ret);

#ifdef NPOPT_USE_ENC_SDATA
/**
 * Encrypt data for saving as protected sdata. This is useful for tests
 * that run with -use_enc_sdata switched on. The personalities in nphome
 * must be pre-encrypted.
 *
 * @param[in]  in_handle   Transport layer info
 * @param[in]  in_tag      Data type
 * @param[in]  in_len      Data length
 * @param[in]  in_data     Data to encrypt
 * @param[in]  in_malloc   Callback to allocation function for output data
 * @param[out] out_len     Output encrypted data length
 * @param[out] out_buf     Output encrypted data
 */
extern np_ret_t
hook_GetSecuredSdata( np_handle_t     in_handle,
                      int             in_tag,
                      size_t          in_len,
                      u_int8_t       *in_data,
                      void           *(*in_malloc)(size_t),
                      unsigned int   *out_len,
                      u_int8_t      **out_buf );
#endif

#endif /* TESTHOOKS_H */
