/*
 * Nautilus version 3.5
 *
 * Any code and mechanism in this module may not be used
 * in any form without permissions.
 * Copyright (C) 2008,2009,2010,2011 Sony Corporation, All Rights Reserved.
 */

/** @addtogroup nsnp_drm Marlin DRM
 *  @ingroup nsnp
 * @{
 *
 * @file
 * DRM APIs
 *
 * @see API.reference.pdf
 */


#ifndef NSNP_DRM_API_H
#define NSNP_DRM_API_H

NP_BEGIN_EXTERN_C

extern np_ret_t
nsnp_InitSession(np_handle_t io_handle);

extern np_ret_t
nsnp_FinSession(np_handle_t io_handle);

extern np_ret_t
nsnp_SetLicense(np_handle_t  in_handle,
                np_size_t    in_licensesize,
                u_int8_t    *in_licensebuf);

extern np_ret_t
nsnp_ActionCheck(np_handle_t in_handle,
                 int         in_actiontype,
                 np_esb_t  **out_esb);

extern np_ret_t
nsnp_ActionPerform(np_handle_t in_handle,
                   int         in_actiontype,
                   np_esb_t  **out_esb);

extern np_ret_t
nsnp_ActionDescribe(np_handle_t in_handle,
                    int         in_actiontype,
                    np_esb_t  **out_esb);

extern np_ret_t
nsnp_ActionDescribePlay(np_handle_t in_handle,
                        np_str_t  **out_esbinfo);

extern np_ret_t
nsnp_Callback(np_handle_t          in_handle,
              np_callback_t       *in_callback,
              np_callback_args_t  *in_callback_args,
              np_esb_t           **out_esbinfo);

extern np_ret_t
nsnp_LinkConstraintDescribe(np_handle_t   in_handle,
                            np_str_t     *in_domainid,
                            np_esb_t    **out_esbinfo);

extern np_ret_t
nsnp_ExportCheck( np_handle_t       in_handle,
                  np_exportmode_t   in_mode,
                  np_str_t         *in_target_type,
                  np_esb_t        **out_esb );

extern np_ret_t
nsnp_ExportDescribe( np_handle_t       in_handle,
                     np_exportmode_t   in_mode,
                     np_str_t         *in_target_type,
                     np_esb_t        **out_esb );

extern np_ret_t
nsnp_ExportPerform( np_handle_t       in_handle,
                    np_exportmode_t   in_mode,
                    np_str_t         *in_target_type,
                    np_esb_t        **out_esb );

NP_END_EXTERN_C

#endif /* NSNP_DRM_API_H */
/** @} */
