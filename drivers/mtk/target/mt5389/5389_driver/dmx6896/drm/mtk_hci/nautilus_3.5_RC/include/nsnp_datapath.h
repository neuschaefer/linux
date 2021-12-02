/*
 * Nautilus version 3.5
 *
 * Any code and mechanism in this module may not be used
 * in any form without permissions.
 * Copyright (C) 2011 Sony Corporation, All Rights Reserved.
 */

/** @addtogroup nsnp_datapath Marlin DRM
 *  @ingroup nsnp
 * @{
 *
 * @file
 * Data Path APIs
 *
 * @see API.reference.pdf
 */


#ifndef NSNP_DATAPATH_API_H
#define NSNP_DATAPATH_API_H

NP_BEGIN_EXTERN_C

/* in_handle can be ms3, drm, bbts, ... */

extern np_ret_t
nsnp_InitTrack(np_handle_t       in_handle,
               np_trackinfo_t   *in_trackinfo,
               np_trackhandle_t *out_htrack);

extern np_ret_t
nsnp_FinTrack(np_handle_t in_handle, np_trackhandle_t io_htrack);

extern np_ret_t
nsnp_Decrypt(np_handle_t       in_handle,
             np_trackhandle_t  io_trackhandle,
             np_size_t         in_ibsize,
             u_int8_t         *in_buffer,
             np_size_t        *io_obsize,
             u_int8_t         *out_buffer);

extern np_ret_t
nsnp_CheckHMAC(np_handle_t      in_handle,
               np_trackhandle_t in_trackhandle,
               int              in_digestalgo,
               np_size_t        in_bufsize,
               u_int8_t        *in_buf,
               np_size_t        in_hmacsize,
               u_int8_t        *in_hmacval);

extern np_ret_t
nsnp_ExportGetKey( np_handle_t        in_handle,
                   np_trackhandle_t   in_trackhandle,
                   void              *in_reserved,
                   np_str_t         **out_ck );

extern np_ret_t
nsnp_MoveKey( np_handle_t        in_handle,
              np_trackhandle_t   in_trackhandle,
              np_str_t          *in_pv,
              np_str_t         **out_pv );

NP_END_EXTERN_C

#endif /* NSNP_DATAPATH_API_H */
/** @} */
