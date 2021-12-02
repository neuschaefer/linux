/*
 * Nautilus version 3.5
 *
 * Any code and mechanism in this module may not be used
 * in any form without permissions.
 * Copyright (C) 2011 Sony Corporation, All Rights Reserved.
 */

/** @addtogroup nsnp_bbts BBTS functionality
 *  @ingroup nsnp
 * @{
 *
 * @brief nsnp layer APIs for BBTS
 *
 * @see API.reference.pdf
 *
 * @file
 * BBTS APIs
 *
 */

#ifndef NSNP_BBTS_API_H
#define NSNP_BBTS_API_H

NP_BEGIN_EXTERN_C


extern np_ret_t
nsnp_bbtsParseCADescriptor(np_size_t                   in_ca_descriptor_len,
                           u_int8_t                   *in_ca_descriptor,
                           np_bbts_mode_t             *out_bbts_mode,
                           u_int16_t                  *out_ca_pid,
                           np_bbts_serv_base_cids_t  **out_serv_base_cids);

extern void
nsnp_bbtsFreeServBaseCIDs(np_bbts_serv_base_cids_t *in_serv_base_cids);

extern np_ret_t
nsnp_bbtsParseEMM(np_size_t               in_emm_len,
                  u_int8_t               *in_emm,
                  np_bbts_urls_t        **out_urls);

extern void
nsnp_bbtsFreeUrls(np_bbts_urls_t *in_urls);

extern np_ret_t
nsnp_bbtsInitSession(np_output_control_t *in_default_oc,
                     np_bbts_mode_t       in_bbts_mode,
                     np_handle_t          io_bbts_handle);

extern np_ret_t
nsnp_bbtsFinSession(np_handle_t   io_bbts_handle);

extern np_ret_t
nsnp_bbtsSetSocID(np_handle_t   in_bbts_handle,
                  np_size_t     in_socid_len,
                  u_int8_t     *in_socid);

extern np_ret_t
nsnp_bbtsInitECMStream(np_handle_t      in_bbts_handle,
                       np_size_t        in_servicebase_cid_len,
                       u_int8_t        *in_servicebase_cid,
                       np_size_t        in_ecm_len,
                       u_int8_t        *in_ecm,
                       np_str_t       **out_serv_content_id,
                       np_str_t       **out_prog_content_id);

NP_END_EXTERN_C

#endif /* NSNP_BBTS_API_H */
/** @} */
