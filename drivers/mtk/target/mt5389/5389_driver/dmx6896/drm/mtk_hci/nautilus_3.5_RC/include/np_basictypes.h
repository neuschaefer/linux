/*
 * Nautilus version 3.5
 *
 * Any code and mechanism in this module may not be used
 * in any form without permissions.
 * Copyright (C) 2004,2005,2006,2007,2008,2009,2010 Sony Corporation, All Rights Reserved.
 *
 */

#ifndef NP_BASICTYPES_H
#define NP_BASICTYPES_H

/*===========================================================================*/
/** @file
 * Basic type and macro definition
 *
 * @require none
 * @author  Norifumi Goto <n-goto@sm.sony.co.jp>
 *
 * @see API.reference.pdf
 */
/*===========================================================================*/

#include <np_basictypes_.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * def: extern "C"
 */
#ifdef __cplusplus
#define NP_BEGIN_EXTERN_C		extern "C" {
#define NP_END_EXTERN_C			}
#else
#define NP_BEGIN_EXTERN_C
#define NP_END_EXTERN_C
#endif

//typedef u_int32_t               np_ret_t;
typedef unsigned int               np_ret_t;

typedef struct {
    np_size_t   len;
    u_int8_t   *buf;
} np_str_t;

/**
 * Protocol type for @ref np_transporthandle_t "transport layer"
 */
typedef int32_t                 np_protocol_t;

#define NP_IS_VALID_PROTO(hndl)  (hndl >= 0)
#define NP_PROTO_NOT_SET         -1

typedef u_int32_t   np_exportmode_t;

#define NP_PROTOCOL_TYPE_LOCAL         0
#define NP_PROTOCOL_TYPE_USB           1

#define NP_RESULT_TYPE_REGISTER_NODE           1
#define NP_RESULT_TYPE_REGISTER_LINK           2
#define NP_RESULT_TYPE_DEREGISTER_LINK         3
#define NP_RESULT_TYPE_LICENSE                 4
#define NP_RESULT_TYPE_WRITE_FILE              5
#define NP_RESULT_TYPE_PROTOCOL_INFO           6
#define NP_RESULT_TYPE_SOAPFAULT              16

#define NP_RESULT_WRITE_FILE_LICENSE           1







typedef struct np_faultdetail {
    np_str_t            *name;
    np_str_t            *redirectURL;
    np_str_t            *details;
} np_faultdetail_t;

typedef struct np_faultinfo {
    ///TODO: fix me
     u_int32_t       protocol;
} np_faultinfo_t;

typedef struct np_usb_info {
    np_str_t   *general_info;
} np_usb_info_t;

typedef struct np_protocol_info {
    u_int32_t       protocol;
    union {
        np_usb_info_t   usb;
    } info;
} np_protocol_info_t;

/**
 * @brief Result of one action token processing step
 */
typedef struct np_action_result {
    /**
     * One of
     *   <br>::NP_RESULT_TYPE_REGISTER_NODE
     *   <br>::NP_RESULT_TYPE_REGISTER_LINK
     *   <br>::NP_RESULT_TYPE_DEREGISTER_LINK
     *   <br>::NP_RESULT_TYPE_LICENSE
     *   <br>::NP_RESULT_TYPE_SOAPFAULT
     *   <br>::NP_RESULT_TYPE_PROTOCOL_INFO
     */
    int        result_type;
    np_str_t  *srv_id;                     /**< Service id (i.e. svcid in [MUSB]) */
    np_str_t  *srv_info;                   /**< Service information (i.e. relatedToken in [MUSB]) */
    union {
        np_str_t            *node_id;         /**< A registered node ID */
        struct {
            np_str_t        *from_id;         /**< Node ID of the orgin of a (de)registered link */
            np_str_t        *to_id;           /**< Node ID of the end of a (de)registered link */
        } link;
        np_str_t            *license;         /**< License bundle */
        struct {
            u_int32_t        type;
            np_str_t        *file;
        } write;
        np_faultinfo_t      *faultinfo;       /**< Details of a SOAP fault */
        np_protocol_info_t   protocol_info;   /**< general info from host */
    } result;
} np_action_result_t;


#ifdef __cplusplus
}
#endif

#endif /* NP_BASICTYPES_H */
