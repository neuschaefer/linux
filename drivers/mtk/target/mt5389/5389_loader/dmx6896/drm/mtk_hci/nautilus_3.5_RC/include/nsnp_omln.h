/*
 * Nautilus version 3.5
 *
 * Any code and mechanism in this module may not be used
 * in any form without permissions.
 * Copyright (C) 2010,2011 Sony Corporation, All Rights Reserved.
 */

/** @addtogroup omln omln
 *  @ingroup nsec
 * @{
 *
 * @file
 * @brief Support for interoperable download, streaming, sharing and consumption 
 * of content between OMA and Marlin DRM systems.
 *
 * @see API.reference.pdf
 */

#ifndef NSNP_OMLN_H
#define NSNP_OMLN_H

NP_BEGIN_EXTERN_C

/* -------------------------------------------------------------------------- */
/*                                                     PUBLIC DATA STRUCTURES */
/* -------------------------------------------------------------------------- */

/** Expected OMArlin version */
#define NP_OMLN_OMARLIN_DRM_VERSION   0x0
/** Default length of the plaintext */
#define NP_OMLN_PDCF_PLAINTEXT_LEN    0x0


/**
 * OMLN Marlin textual header type
 *
 * Type to specify the Marlin textual header parameter
 **/
typedef enum {
    NP_OMLN_SILENT_HDR             = 0,  /** Marlin Silent textual header */
    NP_OMLN_PREVIEW_HDR            = 1,  /** Marlin Preview textual header */
    NP_OMLN_RIGHTS_HDR             = 2,  /** Marlin Rights Issuer textual header */
} np_omln_txt_hdr_types_t;


/**
 * OMLN Marlin Silent textual header method type
 *
 * Type to specify the Marlin silent textual header method
 **/
typedef enum {
    NP_OMLN_ON_DEMAND_METHOD       = 0,  /** Silent on demand */
    NP_OMLN_IN_ADVANCE_METHOD      = 1,  /** Silent advance */
} np_omln_silent_method_t;


/**
 * OMLN Marlin Preview textual header method type
 *
 * Type to specify the Marlin preview textual header method
 **/
typedef enum {
    NP_OMLN_INSTANT_METHOD         = 0,  /** Preview instant */
    NP_OMLN_PREVIEW_RIGHTS_METHOD  = 1   /** Preview rights */
} np_omln_preview_method_t;


/**
 * OMLN Encryption method found in the OMArlin ohdr box
 *
 * Type to specify the encryption method
 **/
typedef enum {
    NP_OMLN_NULL_ENC               = 0,  /** NULL encryption */
    NP_OMLN_AES128_CBC             = 1,  /** AES CBC with key length 128 bits */
    NP_OMLN_AES128_CTR             = 2,  /** AES CTR with key length 128 bits */
} np_omln_enc_method_t;


/**
 * OMLN Padding scheme found in the OMArlin ohdr box
 *
 * Type to specify the padding to use
 **/
typedef enum {
    NP_OMLN_NULL_PAD               = 0,  /** NULL padding */
    NP_OMLN_RFC2630_PAD            = 1,  /** RFC2630 padding */
} np_omln_padding_scheme_t;


/**
 * OMLN Marlin Extension header found in the OMArlin ohdr box
 *
 * Type of extension header
 **/
typedef enum {
    NP_OMLN_EXT_HDR_UNKNOWN        = 0,  /** Unknown extension header type */
} np_omln_ext_hdr_type_t;


/**
 * OMLN Marlin Extension header type
 *
 * Extension header type
 **/
typedef struct {
    np_omln_ext_hdr_type_t        header_type;     /** Type of header data */
    union {
        np_str_t            *unknown_header;  /** Header data */
    } hdr;
} np_omln_ext_header_t;


/**
 * OMLN Marlin textual header type
 *
 * Type to specify a Marlin textual header
 * 
 * @note: Unknown textual headers are ignored  when processing Marlin ones
 */
typedef struct  {
    np_omln_txt_hdr_types_t       txt_hdr_type;    /** Type of the header */
    np_str_t                *hdr_parameter;   /** Header data which can be a url or uri */
    /** Header method */
    union {
        np_omln_silent_method_t   silent_param;    /** Silent header method */
        np_omln_preview_method_t  preview_param;   /** Preview header method */
    } method;
} np_omln_txt_header_t;


/**
 * OMLN Marlin Octopus (8bdl) box type
 *
 * Type to specify a Marlin license found in a 8bdl box
 **/
typedef struct  {
    np_bool_t                is_present;      /** Flag to indicate if a license was found */
    np_str_t                *license;         /** Marlin license in xml format */
} np_omln_8bdl_box_t;


/**
 * OMLN MDRI box type
 *
 * Type to specify a mdri box containing Marlin licenses
 **/
typedef struct  {
    u_int32_t        num_licenses;             /** Number of marlin licenses in the mdri box */
    np_str_t       **licenses;                 /** Marlin licenses found in the mdri box */
} np_omln_mdri_box_t;


/**
 * OMLN OHDR box type
 *
 * Type to specify the content of a ohdr box
 **/
typedef struct  {
    np_omln_enc_method_t          encryption_method; /** Content encryption method */
    np_omln_padding_scheme_t      padding_scheme;    /** Content padding scheme */
    u_int32_t                plaintext_len_hi;  /** Upper 32 bits of the 64bit plaintext length */
    u_int32_t                plaintext_len_lo;  /** Bottom 32 bits of the 64bit plaintext length */
    np_str_t                *content_id;        /** Content ID */
    struct { 
        u_int32_t            num_hdrs;          /** Number of Marlin textual headers found */
        np_omln_txt_header_t    **hdrs;            /** Textual headers. Order will be preserved */
    } txt_headers;
    struct {
        u_int32_t            num_hdrs;          /** Number of extension headers */
        np_omln_ext_header_t    **hdrs;            /** Extension headers. Order will be preserved */
    } ext_headers;
} np_omln_ohdr_box_t;


/**
 * OMLN ODAF box type
 *
 * Type to specify the content of a odaf box
 **/
typedef struct {
    np_bool_t   selective_encryption;           /** Flag indicating the selective encryption */
    u_int32_t   key_indicator_len;              /** Length of the key indicator */
    u_int32_t   iv_len;                         /** Length of the iv */
} np_omln_odaf_box_t;

/**
 * OMA DRM box type 
 *
 * Type used for key management
 **/
typedef struct {
    np_omln_ohdr_box_t *ohdr_box;                 /** ohdr data */
    np_omln_odaf_box_t *odaf_box;                 /** odaf data */
} np_omln_odkm_box_t;

/**
 * OMLN OMArlin sinf box type
 *
 * Type to specify a OMArlin sinf box
 *
 * @note chm and frma are not parsed but skipped by our parser
 **/
typedef struct {
    /** schi box data */
    struct {
        u_int32_t            num_odkm_boxes;           /** Number of odkm boxes present */
        np_omln_odkm_box_t **odkm_boxes;               /** ODKM boxes */
        u_int32_t            num_bdl_boxes;            /** Number of 8bdl boxes present */
        np_omln_8bdl_box_t **oct_bdl_boxes;            /** 8bdl license data */
    } schi_box; 
} np_omln_sinf_box_t; 

/* -------------------------------------------------------------------------- */
/*                                                                 PUBLIC API */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/** 
 * @brief Parse the sinf 
 *
 * @param[in]     in_sinf_len         Sinf box length
 * @param[in]     in_sinf             Binary string containing a sinf box
 * @param[out]    out_sinf_box        Parsed sinf box
 *
 * @return ::ERR_OK
 * @return ::ERR_NP_ILLEGALARGS
 * @return ::ERR_NP_OUTOFMEMORY
 * @return ::ERR_NP_INTERNALERROR
 * @return ::ERR_NP_NOTSUPPORTED
 * @return ::ERR_NP_OMLN_INVALID_ODAF
 * @return ::ERR_NP_OMLN_INVALID_SINF
 */
/* -------------------------------------------------------------------------- */
extern np_ret_t
nsnp_omlnParseSinfBox(np_size_t                in_sinf_len,
                      u_int8_t                *in_sinf,
                      np_omln_sinf_box_t     **out_sinf_box);


/* -------------------------------------------------------------------------- */
/** 
 * @brief Parse the mdri for a marlin license 
 *
 * @param[in]     in_mdri_len         Mdri box length
 * @param[in]     in_mdri             Binary string containing a mdri box
 * @param[out]    out_license         Marlin license
 *
 * @return ::ERR_NP_OK
 * @return ::ERR_NP_ILLEGALARGS
 * @return ::ERR_NP_OUTOFMEMORY
 * @return ::ERR_NP_INTERNALERROR
 * @return ::ERR_NP_NOTSUPPORTED
 * @return ::ERR_NP_OMLN_INVALID_MDRI
 * @return ::ERR_NP_OMLN_INVALID_8BDL
 */
/* -------------------------------------------------------------------------- */
extern np_ret_t
nsnp_omlnParse8bdlLicenseMdriBox(np_size_t             in_mdri_len,
                                 u_int8_t             *in_mdri,
                                 np_omln_mdri_box_t  **out_license);

/* -------------------------------------------------------------------------- */
/** 
 * @brief Add a Marlin license to the already allocated mdri box 
 *
 * @param[in]         in_license_len   Marlin License length
 * @param[in]         in_license       Marlin License
 * @param[in]         in_truncate_free If a Free Box is present in the mdri
 *                                     then add the license to its space and
 *                                     truncate it.
 * @param[in,out]     io_mdri_len      Length of mdri box to insert the license
 *                                     into. This length is updated with the
 *                                     size of the license box that was inserted
 *                                     into the mdri one
 * @param[in,out]     io_mdri          Binary string containing a mdri box that
 *                                     the Marlin License will be inserted into
 *
 * @return ::ERR_OK
 * @return ::ERR_NP_ILLEGALARGS
 * @return ::ERR_NP_INTERNALERROR
 * @return ::ERR_NP_OMLN_MDRI_BOX_TOO_SMALL
 * @return ::ERR_NP_OMLN_INVALID_MDRI
 */
/* -------------------------------------------------------------------------- */
extern np_ret_t
nsnp_omlnAdd8bdlLicenseMdriBox(np_size_t    in_license_len,
                               u_int8_t    *in_license,
                               np_bool_t    in_truncate_free,
                               np_size_t   *io_mdri_len,
                               u_int8_t    *io_mdri);


/* -------------------------------------------------------------------------- */
/** 
 * @brief Parse the ohdr
 *
 * @param[in]     in_ohdr_len         Ohdr length
 * @param[in]     in_ohdr             Binary string containing a ohdr box
 * @param[out]    out_ohdr_box        Parsed ohdr box
 *
 * @return ::ERR_NP_OK
 * @return ::ERR_NP_ILLEGALARGS
 * @return ::ERR_NP_OUTOFMEMORY
 * @return ::ERR_NP_INTERNALERROR
 * @return ::ERR_NP_NOTSUPPORTED
 * @return ::ERR_NP_OMLN_INVALID_OHDR
 */
/* -------------------------------------------------------------------------- */
extern np_ret_t
nsnp_omlnParseOhdrBox(np_size_t                  in_ohdr_len,
                      u_int8_t                  *in_ohdr,
                      np_omln_ohdr_box_t       **out_ohdr_box);


/* -------------------------------------------------------------------------- */
/** 
 * @brief Free the Ohdr box struct 
 *
 * @param[in]     io_ohdr_box         ohdr structure to be freed
 */
/* -------------------------------------------------------------------------- */
extern np_ret_t
nsnp_omlnFreeOhdrBox(np_omln_ohdr_box_t        *io_ohdr_box);


/* -------------------------------------------------------------------------- */
/** 
 * @brief Free the Sinf box struct 
 *
 * @param[in]     io_sinf_box         sinf structure to be freed
 */
/* -------------------------------------------------------------------------- */
extern np_ret_t
nsnp_omlnFreeSinfBox(np_omln_sinf_box_t        *io_sinf_box);


/* -------------------------------------------------------------------------- */
/** 
 * @brief Free the mdri box struct 
 *
 * @param[in]     io_mdri_box         mdri structure to be freed
 */
/* -------------------------------------------------------------------------- */
extern np_ret_t
nsnp_omlnFreeMdriBox(np_omln_mdri_box_t        *io_mdri_box);


NP_END_EXTERN_C

#endif /* NSNP_OMLN_H */
/** @} */
