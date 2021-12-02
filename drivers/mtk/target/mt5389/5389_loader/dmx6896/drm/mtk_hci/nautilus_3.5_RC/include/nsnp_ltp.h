/*
 * Nautilus version 3.5
 *
 * Any code and mechanism in this module may not be used
 * in any form without permissions.
 * Copyright (C) 2008,2009,2010 Sony Corporation, All Rights Reserved.
 */

/**
 * @addtogroup nsnp_ltp License Transfer
 * @ingroup nsnp
 * @{
 *
 * @brief nsnp layer APIs for license transfer between two Marlin devices
 *
 * @file
 * Basic license transfer APIs
 *
 * @note For license transfer it is recommended that the @ref pavo module is used instead.
 */

#ifndef NSNP_LTP_API_H
#define NSNP_LTP_API_H

NP_BEGIN_EXTERN_C

extern np_ret_t
nsnp_ltpFreeCertStdLst(np_certstandard_list_t *io_certstdlst);

extern np_ret_t
nsnp_ltpInitDefaultCertStdLst(np_certstandard_list_t **out_certstdlst);

extern np_ret_t
nsnp_ltpInitSource( np_handle_t               io_handle,
                    np_certstandard_list_t   *in_certstdlist,
                    u_int32_t                *out_nenccerts,
                    np_str_t               ***out_enccerts,
                    u_int32_t                *out_nsigcerts,
                    np_str_t               ***out_sigcerts,
                    np_str_t                **out_roleast,
                    np_str_t                **out_nodeid );

extern np_ret_t
nsnp_ltpInitSink( np_handle_t               io_handle,
                  np_certstandard_list_t  **io_certstdlist,
                  u_int32_t                 in_nenccerts,
                  np_str_t                **io_enccerts,
                  u_int32_t                 in_nsigcerts,
                  np_str_t                **io_sigcerts,
                  np_str_t                 *in_roleast,
                  np_str_t                 *in_nodeid );

/*----------------------------------------------------------------------------*/
/**
 * @brief Perform the initialization of npltp_handle_t handles for source and sink
 *
 * @param[in,out] io_src_handle LTP handle for source and SMI target info.
 * @param[in,out] io_snk_handle LTP handle for sink   and SMI target info.
 *
 * @return ::ERR_NP_OK
 * @return ::ERR_NP_ILLEGALARGS
 * @return ::ERR_NP_OUTOFMEMORY
 * %%CR%JR%% missing functions
 * @return ::l_ltpInitDefaultCertStdLst
 * @return ::npltp_Init
 */
/*----------------------------------------------------------------------------*/
extern np_ret_t
nsnp_ltpInit(np_handle_t  io_src_handle,
	     np_handle_t  io_snk_handle);

/*----------------------------------------------------------------------------*/
/**
 * @brief Perform the finalization of a npltp_handle_t
 *
 * @param[in,out] io_handle        LTP and connection info handle.
 *
 * @return ::ERR_NP_OK
 * @return ::ERR_NP_INTERNALERROR
 * %%CR%JR%% missing functions
 */
/*----------------------------------------------------------------------------*/
extern np_ret_t
nsnp_ltpFin(np_handle_t io_handle );


/*----------------------------------------------------------------------------*/
/**
 * @brief License Transfer Protocol setup message build.
 *
 * Create a setup message on the sink that will be processed on the source
 *
 * @param[in] ihandle            LTP handle of the sink and SMI target info
 * @param[in] in_license         License that will be transfered.
 * @param[out] out_message       Buffer for the generated setup message.
 *
 * @return ::ERR_NP_OK
 * @return ::ERR_NP_ILLEGALHANDLE
 * @return ::ERR_NP_ILLEGALARGS
 * @return ::ERR_NP_OUTOFMEMORY
 * @return ::ERR_NP_INTERNALERROR
 * @return ::ERR_NP_ILLEGALTRANSPORTLAYER
 * @return ::ERR_NP_SMI_MSG_CORRUPTED
 * %%CR%JR%% missing functions
 */
/*----------------------------------------------------------------------------*/
extern np_ret_t
nsnp_ltpSetupMsgBuild(np_handle_t      in_handle,
                      np_str_t        *in_license,
                      np_str_t       **out_message);


/*----------------------------------------------------------------------------*/
/**
 * @brief License Transfer Protocol setup message processing.
 *
 * Process the setup message on the source, set the license in DRM
 * engine, run the transfer action perform operation and return the ESB.
 *
 * @param[in] io_src_handle              LTP handle of the source and SMI target info
 * @param[in] in_setup_message           Setup message provided by sink.
 * @param[out] out_run_agent_message     Buffer for generated message.
 *
 * @return ::ERR_NP_OK
 * @return ::ERR_NP_ILLEGALHANDLE
 * @return ::ERR_NP_ILLEGALARGS
 * @return ::ERR_NP_OUTOFMEMORY
 * @return ::ERR_NP_INTERNALERROR
 * @return ::ERR_NP_ILLEGALLICENSE
 * @return ::ERR_NP_ACTIONNOTGRANTED
 * @return ::ERR_NP_ILLEGALESB
 * @return ::ERR_NP_ILLEGALTRANSPORTLAYER
 * @return ::ERR_NP_SMI_MSG_CORRUPTED
 * %%CR%JR%% missing functions
 */
/*----------------------------------------------------------------------------*/
extern np_ret_t
nsnp_ltpSetupMsgProc(np_handle_t      in_handle,
                     np_str_t        *in_setup_message,
                     np_str_t       **out_run_agent_message);


/*----------------------------------------------------------------------------*/
/**
 * @brief License Transfer Protocol RunAgentOnPeer message processing.
 *
 * @param[in] io_snk_handle              LTP handle and SMI target info
 * @param[in] in_run_agent_message       message
 * @param[out] out_agent_res_message     Buffer for Body data.
 *
 * @return ::ERR_NP_OK
 * @return ::ERR_NP_ILLEGALARGS
 * @return ::ERR_NP_ILLEGALTRANSPORTLAYER
 * @return ::ERR_NP_SMI_MSG_CORRUPTED
 * @return ::npltp_RunAgentOnPeerMsgProc
 * @return ::npltp_AgentResultMsgBuild
 * @return ::ERR_NP_ILLEGALTRANSPORTLAYER
 * @return ::ERR_NP_SMI_MSG_CORRUPTED
 * %%CR%JR%% missing functions
 */
/*----------------------------------------------------------------------------*/
extern np_ret_t
nsnp_ltpRunAgentOnPeerMsgProc(np_handle_t      in_handle,
                              np_str_t        *in_run_agent_message,
                              np_str_t       **out_agent_res_message);


/*----------------------------------------------------------------------------*/
/**
 * @brief License Transfer Protocol AgentResult message processing.
 *
 * @param[in] io_src_handle             LTP handle and SMI target info
 * @param[in] agent_res_message         message
 * @param[out] out_teardown_message     Buffer for Body data.
 *
 * @return ::ERR_NP_OK
 * @return ::ERR_NP_ILLEGALARGS
 * @return ::l_GetCallbackInfosFromEsb
 * @return ::npltp_AgentResultMsgProc
 * @return ::npltp_TeardownMsgBuild
 * @return ::ERR_NP_ILLEGALTRANSPORTLAYER
 * @return ::ERR_NP_SMI_MSG_CORRUPTED
 * %%CR%JR%% missing functions
 */
/*----------------------------------------------------------------------------*/
extern np_ret_t
nsnp_ltpAgentResultMsgProc(np_handle_t      in_handle,
                           np_str_t        *agent_res_message,
                           np_str_t       **out_teardown_message);

/*----------------------------------------------------------------------------*/
/**
 * @brief Convert SRC license to SNK.
 *
 * This will replace the encrypted key data in the license with key data encrypted
 * by the SNK public key if the ESB action is GRANTED in the message
 *
 * @param[in]  in_snk_handle  Handle of the Sink device and SMI target info
 * @param[in]  in_license     SRC license
 * @param[in]  in_message     Teardown message containing the new content key list
 * @param[out] out_license    Output destination for new license
 *
 * @return ::ERR_NP_OK
 * @return ::ERR_NP_ILLEGALARGS
 * @return ::npltp_TeardownMsgProc
 * @return ::ERR_NP_ILLEGALTRANSPORTLAYER
 * @return ::ERR_NP_SMI_MSG_CORRUPTED
 * %%CR%JR%% missing functions
 */
/*----------------------------------------------------------------------------*/
extern np_ret_t
nsnp_ltpTeardownMsgProc( np_handle_t      in_handle,
                         np_str_t        *in_license,
                         np_str_t        *in_message,
                         np_str_t       **out_license);

NP_END_EXTERN_C

#endif /* NSNP_LTP_API_H */
/** @} */
