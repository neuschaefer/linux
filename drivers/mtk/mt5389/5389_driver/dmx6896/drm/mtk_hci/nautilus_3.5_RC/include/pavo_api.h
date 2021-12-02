/*
 * Nautilus version 3.5
 *
 * Any code and mechanism in this module may not be used
 * in any form without permissions.
 * Copyright (C) 2008,2009,2010 Sony Corporation, All Rights Reserved.
 */

/**
 * @addtogroup pavo pavo
 * @ingroup nsec
 * @{ 
 *
 * @nsdepends
 * basic_types.h, npcommon_types.h and np_transportlayer.h
 *
 * @file
 * License transfer API
 *
 * @see API.reference.pdf
 */

#ifndef PAVO_API_H
#define PAVO_API_H

NP_BEGIN_EXTERN_C

np_ret_t 
pavo_psnInit ( np_handle_t      io_src_handle,
               np_handle_t      io_snk_handle );

np_ret_t 
pavo_psnFin ( np_handle_t in_src_handle, 
              np_handle_t in_snk_handle );

np_ret_t
pavo_psnMov(np_handle_t   in_src_handle,
            np_handle_t   in_snk_handle, 
            np_str_t     *in_license,
            np_str_t    **out_license);


NP_END_EXTERN_C

#endif /* PAVO_API_H */
/** @} */
