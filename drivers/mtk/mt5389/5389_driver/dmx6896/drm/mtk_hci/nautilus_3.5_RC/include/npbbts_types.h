/** @addtogroup npbbts npbbts
 *  @ingroup api
 * @{ */

/*===========================================================================*/
/** @brief BBTS Data Structures and Enumerations
 *
 * @see API.reference.pdf
 * @file
 *
 */
/** @} */
/*===========================================================================*/

/*
 * Nautilus version 3.5
 *
 * Any code and mechanism in this module may not be used
 * in any form without permissions.
 * Copyright (C) 2011 Sony Corporation, All Rights Reserved.
 */

#ifndef NPBBTS_TYPES_H
#define NPBBTS_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    NP_BBTS_SINGLE_KEY_LAYER = 0x0,
    NP_BBTS_FULL             = 0x1
} np_bbts_mode_t;

typedef struct {
    u_int32_t           num_servBaseCIDs; 
    np_str_t          **servBaseCIDs; 
} np_bbts_serv_base_cids_t;

typedef enum {
    NP_BBTS_RIGHTS_HDR   = 0x02,
    NP_BBTS_SILENT_HDR   = 0x03,
    NP_BBTS_PREVIEW_HDR  = 0x04
} np_bbts_url_types_t;

typedef enum {
    NP_BBTS_ON_DEMAND_METHOD   = 0x00,
    NP_BBTS_IN_ADVANCE_METHOD  = 0x01
} np_bbts_silent_method_t;

typedef struct {
    np_bbts_url_types_t        url_type;
    np_str_t                  *url;
    np_bbts_silent_method_t    silent_method;
} np_bbts_url_t;

typedef struct {
    u_int32_t               num_urls;
    np_bbts_url_t         **urls;
} np_bbts_urls_t;

#ifdef __cplusplus
}
#endif

#endif /* NPBBTS_TYPES_H */

