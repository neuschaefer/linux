/** @addtogroup npcommon Common error codes and structures
 *  @ingroup api
 * @{ 
 *
 * This module provides provides definitions for many useful structures
 * and values exposed in the Nautilus API.
 *
 * Global error codes are defined here, as are structures required by the 
 * Nautilus API for representing @ref np_str_t "strings" and @ref np_date_t
 * "dates"
 */

/*
 * Nautilus version 3.5
 *
 * Any code and mechanism in this module may not be used
 * in any form without permissions.
 * Copyright (C) 2008,2009,2010,2011 Sony Corporation, All Rights Reserved.
 */

#ifndef NPCOMMON_TYPES_H
#define NPCOMMON_TYPES_H

NP_BEGIN_EXTERN_C

/*===========================================================================*/
/** @file
 * Nautilus Library common types, error codes and constants
 * 
 * @see API.reference.pdf
 */
/*===========================================================================*/

/*---------------------------------------------------------------------------*
 * error code definitions
 *---------------------------------------------------------------------------*/

enum {
    ERR_NP_OK = 0,
    /* start */
    ERR_NP_BASE = 0x00000001,
    /* generic error */
    ERR_NP_OUTOFMEMORY,
    ERR_NP_ILLEGALHANDLE,
    ERR_NP_ILLEGALARGS,
    ERR_NP_FILEERROR,
    ERR_NP_NOTSUPPORTED,
    ERR_NP_INTERNALERROR,
    /* common */
    ERR_NP_NOTINITIALIZED,
    /* registration */
    ERR_NP_ILLEGALPERSONALITY,
    ERR_NP_ILLEGALLINK,				/* 10 */
    ERR_NP_ILLEGALNODE,
    ERR_NP_ILLEGALASSERTION,
    ERR_NP_ILLEGALLSL,
    ERR_NP_ALREADYREGISTERED,
    ERR_NP_NONEMOPERSONALITYREGISTERED,
    ERR_NP_NOPERSONALITYREGISTERED,		/* 0x10 */
    ERR_NP_NONODEREGISTERED,
    ERR_NP_NOLINKREGISTERED,
    ERR_NP_ASSERTIONNOTFOUND,
    ERR_NP_NOTEXIST,				/* 20 */
    ERR_NP_CONNECTIONERROR,
    ERR_NP_REGISTEREDMAX,
    /* contents/license setting */
    ERR_NP_ILLEGALLICENSE,
    ERR_NP_CONTENTSMISMATCH,
    ERR_NP_ALREADYSET,
    /* action */
    ERR_NP_CONTENTSNOTSET,
    ERR_NP_LICENSENOTSET,
    ERR_NP_ALREADYPERFORMED,
    ERR_NP_ACTIONNOTGRANTED,
    ERR_NP_NODESCRIPTION,			/* 30 */
    ERR_NP_NOERRORINFO,
    /* render */
    ERR_NP_NOKEY,				/* 0x20 */
    ERR_NP_TOOSMALLBUFFER,
    ERR_NP_DECRYPTERROR,
    ERR_NP_RENDERERROR,
    /* nemo */
    ERR_NP_MESSAGECORRUPTED,
    ERR_NP_NEEDTTUPDATE,
    ERR_NP_NEEDCRLUPDATE,
    ERR_NP_NEEDLSLUPDATE,
    ERR_NP_NEEDUPDATE,		                /* 40 */
    ERR_NP_SOAPFAULT,
    ERR_NP_NOSOAPFAULT,
    ERR_NP_ILLEGALCERTSET,
    /* Others */
    ERR_NP_ABORT,
    ERR_NP_NOCHALLENGE,
    ERR_NP_NOFREEHANDLE,
    /* SOAPFAULT */
    ERR_NP_UNKNOWNSOAPFAULT,
    ERR_NP_SOAPFAULTDCSASSERTIONSTALE,          /* 0x30 */		
    ERR_NP_SOAPFAULTMARLINSPECVERSION,
    ERR_NP_SOAPFAULTCLIENTATTRIBUTEMISMATCH,	/* 50 */
    ERR_NP_SOAPFAULTMEDIASERVICE,
    ERR_NP_SOAPFAULTSERVER,
    ERR_NP_SOAPFAULTCLIENT,
    ERR_NP_SOAPFAULTINVALIDSECURITY,
    ERR_NP_SOAPFAULTINVALIDSECURITYTOKEN,
    ERR_NP_SOAPFAULTUNSUPPORTEDSECURITYTOKEN,
    ERR_NP_SOAPFAULTFAILEDCHECK,
    ERR_NP_SOAPFAULTFAILEDAUTHENTICATION,
    ERR_NP_SOAPFAULTMESSAGEEXPIRED,
    ERR_NP_SOAPFAULTUNSUPPORTEDSECUREPROTOCOL,	/* 60 */
    /* DB */
    ERR_NP_DB_ALREADYOPENED,
    ERR_NP_DB_CORRUPTED,
    ERR_NP_DB_ENTRYNOTEXIST,
    ERR_NP_DB_FILENAMETOOLONG,			/* 0x40 */
    ERR_NP_DB_IDTOOLONG,
    /* ESB */
    ERR_NP_ILLEGALESB,
    /* SDATA */
    ERR_NP_NOTACTIVE,
    ERR_NP_SDATACORRUPTED,
    /* FirstPlay */
    ERR_NP_LILNOENTRY,
    /* Hard */
    ERR_NP_DEVICEERROR,				/* 70 */
    ERR_NP_CORRUPTEDLC,
    ERR_NP_ILLEGALAID,
    ERR_NP_ILLEGALSLS,
    ERR_NP_ILLEGALLIL,
    ERR_NP_REGISTEREDAIDMAX,
    ERR_NP_REGISTEREDSLSMAX,
    ERR_NP_REGISTEREDLILMAX,
    ERR_NP_CORRUPTEDSLS,
    ERR_NP_CORRUPTEDLIL,
    ERR_NP_DECRYPTDENIED,                       /* 80, 0x50 */
    /* Export action */
    ERR_NP_EXPORTDENIED,
    /* Token */
    ERR_NP_ILLEGALST,
    /* Protocol */
    ERR_NP_TL_CALLBACK,
    ERR_NP_MUSB_PROTOCOL,
    ERR_NP_HTTP_CALLBACK,
    ERR_NP_ATTACHED_DEVICE,
    ERR_NP_INVALID_INFO,
    ERR_NP_NO_CONFIG,
    /* MS3 */
    ERR_NP_NOSASFOUND,
    ERR_NP_ILLEGALSAS,
    ERR_NP_SASALREADYSET,
    ERR_NP_ILLEGALCURIT,
    ERR_NP_SASALREADYPROCESSED,
    ERR_NP_ILLEGALCOMPOUNDURI,
    ERR_NP_ILLEGALMS3AT,
    ERR_NP_ILLEGALMANIFESTFILE,
    ERR_NP_ACQUIRESAS,
    /* BBTS */
    ERR_NP_BBTS_INVALID_CA_DESC,
    ERR_NP_BBTS_INVALID_ECM,
    ERR_NP_BBTS_INVALID_EMM,
    /* OMLN */
    ERR_NP_OMLN_INVALID_OHDR,
    ERR_NP_OMLN_INVALID_8BDL,
    ERR_NP_OMLN_INVALID_MDRI,
    ERR_NP_OMLN_INVALID_SINF,
    ERR_NP_OMLN_INVALID_ODAF,
    ERR_NP_OMLN_MDRI_BOX_TOO_SMALL,

    /* end */
    ERR_NP_END = 0x000000FF,

    /* mlnsinf */
    ERR_NP_MLNSINF = 0x00000100,
    ERR_NP_MLNSINF_END = 0x000001FF,

    /* smi */
    ERR_NP_SMI_MSG_CORRUPTED,
    ERR_NP_SMI_MSG_TYPE_MISMATCH,
    ERR_NP_SMI_MSG_TOO_LARGE,
    ERR_NP_ILLEGALTRANSPORTLAYER,
    ERR_NP_TRANSPORTLAYERFAILURE,

    /* Dorado */
    ERR_NP_DORADO = 0x000F0000,
    ERR_NP_DORADO_END = 0x000FFFFF,

    /* ServiceTokenParser */
    ERR_NP_STOKEN = 0x001F0000,
    ERR_NP_STOKEN_END = 0x001FFFFF,

    /* PPProc */
    ERR_NP_PPPROC = 0x002F0000,
    ERR_NP_PPPROC_END = 0x002FFFFF,

    /* other */
    ERR_NP_USERERROR = 0x0000F000,
    ERR_NP_USERERROR_END = 0x0000FFFF
};

/*---------------------------------------------------------------------------*
 * type definitions
 *---------------------------------------------------------------------------*/

typedef enum {
    NP_SERV_DRM   = 0,
    NP_SERV_MS3   = 1,
    NP_SERV_BBTS  = 2,
    NP_SERV_NP    = 3,
    NP_SERV_LIMIT = 4,
} np_service_t;

typedef struct {
    /* Public Service Data */
    np_service_t     service;
    void            *keymng_context;
    /* Friend Service Data - Data shared between services in the secure world */
    void            *bbts_timestamp;
    void            *drm_actionperform_esb;
    /* Private Service Data */
    void            *service_context;
} np_context_t;

#define NP_DATE_SIZE 28

typedef struct {
    int year;
    int mon;
    int mday;
    int hour;
    int min;
    int sec;
    int msec;
} np_date_t;

typedef void *np_sechandle_t;

typedef struct {
    np_sechandle_t handle;
    np_size_t len;
} np_secbuffer_t;

/** @see np_faultdetail_t in API.reference.pdf */
typedef struct {
    np_secbuffer_t            name;
    np_secbuffer_t            redirectURL;
    np_secbuffer_t            details;
} nputil_faultdetail_t;

/** @see np_faultinfo_t in API.reference.pdf */
typedef struct {
    np_ret_t                  err;
    np_secbuffer_t            code;
    np_secbuffer_t            str;
    np_secbuffer_t            actor;
    nputil_faultdetail_t      *detail;
    np_size_t                 detail_count;
} nputil_faultinfo_t;

/*
 * This struct describes a metadata item.
 */
typedef struct {
    const np_str_t* name;
    const np_str_t* ns;
} np_metadata_item_t;

/*
 * This stucture describes a Data Certification Standard consisting of
 * a name of the standard and a list of metadata items.
 */
typedef struct {
    const np_str_t*     crt_std_name;
    np_size_t           num_metadata_items;
    np_metadata_item_t* metadata_items;
} np_crt_std_t;

typedef struct {
    u_int8_t        size;
    np_crt_std_t  **crt_stds;
} np_crt_std_list_t;

/** Certification standard structure */
typedef struct {
    np_str_t      *dcsn;      /**< Data certification standard name */
    np_str_t      *validity;  /**< Validity xsd:duration string (or NULL) */
    u_int8_t       mandatory; /**< Must (!= 0) or Should (== 0) flag */
} np_certstandard_t;

/** Certification standard list structure */
typedef struct {
    u_int8_t size;                 /**< Number of certificate standards */
    np_certstandard_t ** certstds; /**< List of certificate standards */
} np_certstandard_list_t;

/** The certification crl number structure that can handle 20 octets */
#define NP_MAX_NR_OCTET_CRL 20
#define NP_CERT_CRLNUMBER_SIZE 1+NP_MAX_NR_OCTET_CRL
typedef struct {
    u_int8_t valid;                  /**< 1 indicates set, 0 indicates net yet set (invalid)*/
    u_int8_t crlnumber[NP_MAX_NR_OCTET_CRL]; /**< Unsigned integer in Big Endian Format */
} np_cert_crlnumber_t;


typedef int32_t np_bool_t;


/**
 * default memory allocation size for @ref nsnp_Init
 */
#define NP_DEFAULT_MEM_SIZE ((np_size_t)0)

/*---------------------------------------------------------------------------*
 * personality type definitions
 *---------------------------------------------------------------------------*/
#define NP_PERSONALITY_NEMO     0
#define NP_PERSONALITY_OCTPUB   1
#define NP_PERSONALITY_OCTPRIV  2
#define NP_PERSONALITY_MAX      3

/* This macro can be used to initialize a crl number from a positive integer */
/* Note: the argument of this macro will be called 4 times if it is a function */
#define NP_CRL_NUMBER_FROM_INT(crlnumber_int) {1, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, (crlnumber_int)/256/256/256%256, (crlnumber_int)/256/256%256, (crlnumber_int)/256%256, (crlnumber_int)%256}}

#define NP_INVALID_CRL_NUMBER {0, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}

#define NP_NULL_SECHANDLE           NULL
#define NP_SECBUFFER_INITIALIZER    {NP_NULL_SECHANDLE, 0}
#define NP_SECBUFFER_SET(secbuf,str)    \
    do { (secbuf)->len = binstr_Len(str); (secbuf)->handle = (str); (str) = NULL; } while (0)
#define NP_SECBUFFER_GET(secbuf,str)    \
    do { str = (secbuf)->handle; (secbuf)->handle = NULL; } while (0)

#define NP_S_ISSUBSREG(s)             (s&(1<<23))
#define NP_S_ISPERS(s)                (s&(1<<24))
#define NP_S_ISUSERREG(s)             (s&(1<<25))
#define NP_S_ISCORRUPTED(s)           (s&((1<<28|1<<29|1<<30|1<<31)))
#define NP_S_ISPERSCORRUPTED(s)       (s&(1<<28))
#define NP_S_ISINTERNALLYCORRUPTED(s) (s&(1<<30))
#define NP_S_ISREGCORRUPTED(s)        (s&(1<<29|1<<31))
#define NP_S_ISUSERCORRUPTED(s)       (s&(1<<29))
#define NP_S_ISSUBSCORRUPTED(s)       (s&(1<<31))

#  define NP_EXPORT_MODE_RENDER     (0)
#  define NP_EXPORT_MODE_MOVE       (1)
#  define NP_EXPORT_MODE_COPY       (2)
#  define NP_EXPORT_MODE_DONTKNOW   (3)

#  define NP_EXPORT_RESULT_DTCP_RMM(e)           (((e.result.i)>> 1) & 0x01)
#  define NP_EXPORT_RESULT_DTCP_RS(e)            (((e.result.i)>> 2) & 0x07)
#  define NP_EXPORT_RESULT_DTCP_EPN(e)           (((e.result.i)>> 5) & 0x01)
#  define NP_EXPORT_RESULT_DTCP_CCI(e)           (((e.result.i)>> 6) & 0x03)
#  define NP_EXPORT_RESULT_DTCP_ICT(e)           (((e.result.i)>>13) & 0x01)
#  define NP_EXPORT_RESULT_DTCP_APS(e)           (((e.result.i)>>14) & 0x03)

#  define NP_EXPORT_RESULT_CPRM_DVD_EPN(e)       (((e.result.i)>> 2) & 0x01)
#  define NP_EXPORT_RESULT_CPRM_DVD_APSTB(e)     (((e.result.i)>> 4) & 0x03)
#  define NP_EXPORT_RESULT_CPRM_DVD_CGMS(e)      (((e.result.i)>> 6) & 0x03)

#  define NP_EXPORT_RESULT_CPRM_SDV_CCCI(e)      (((e.result.i))     & 0x0f)
#  define NP_EXPORT_RESULT_CPRM_SDV_CMCI(e)      (((e.result.i)>> 4) & 0x03)
#  define NP_EXPORT_RESULT_CPRM_SDV_IMCI(e)      (((e.result.i)>> 6) & 0x03)
#  define NP_EXPORT_RESULT_CPRM_SDV_APSTB(e)     (((e.result.i)>>12) & 0x03)

#  define NP_EXPORT_RESULT_CPRM_SDA_CCI(e)       (((e.result.i))     & 0x03)
#  define NP_EXPORT_RESULT_CPRM_SDA_COCI(e)      (((e.result.i)>> 8) & 0x0f)
#  define NP_EXPORT_RESULT_CPRM_SDA_MCI(e)       (((e.result.i)>>12) & 0x0f)

#  define NP_EXPORT_RESULT_VCPS_EPN2(e)          (((e.result.i))     & 0x01)
#  define NP_EXPORT_RESULT_VCPS_EPN1(e)          (((e.result.i)>> 1) & 0x01)
#  define NP_EXPORT_RESULT_VCPS_CGMS2(e)         (((e.result.i)>> 2) & 0x03)
#  define NP_EXPORT_RESULT_VCPS_CGMS1(e)         (((e.result.i)>> 4) & 0x03)
#  define NP_EXPORT_RESULT_VCPS_APS(e)           (((e.result.i)>> 6) & 0x03)

#  define NP_EXPORT_RESULT_MGR_SVR_ICT(e)        (((e.result.i)>> 2) & 0x01)
#  define NP_EXPORT_RESULT_MGR_SVR_APSTB(e)      (((e.result.i)>> 3) & 0x03)
#  define NP_EXPORT_RESULT_MGR_SVR_EPN(e)        (((e.result.i)>> 5) & 0x01)
#  define NP_EXPORT_RESULT_MGR_SVR_CCI(e)        (((e.result.i)>> 6) & 0x03)

#  define NP_EXPORT_RESULT_MGR_SAR_CAI(e)        (((e.result.i)>> 1) & 0x07)
#  define NP_EXPORT_RESULT_MGR_SAR_CGI(e)        (((e.result.i)>> 6) & 0x01)
#  define NP_EXPORT_RESULT_MGR_SAR_CPI(e)        (((e.result.i)>> 7) & 0x01)
#  define NP_EXPORT_RESULT_MGR_SAR_NCOI(e)       (((e.result.i)>> 8) & 0xff)

#  define NP_EXPORT_RESULT_SAFIA_TV_MB(e)        (((e.result.i))     & 0x01)
#  define NP_EXPORT_RESULT_SAFIA_TV_MU(e)        (((e.result.i)>> 1) & 0x01)
#  define NP_EXPORT_RESULT_SAFIA_TV_COUNT(e)     (((e.result.i)>> 2) & 0x0f)
#  define NP_EXPORT_RESULT_SAFIA_TV_FM(e)        (((e.result.i)>> 6) & 0x03)
#  define NP_EXPORT_RESULT_SAFIA_TV_CT(e)        (((e.result.i)>> 8) & 0xff)
#  define NP_EXPORT_RESULT_SAFIA_TV_APS(e)       (((e.result.i)>>18) & 0x03)
#  define NP_EXPORT_RESULT_SAFIA_TV_ICT(e)       (((e.result.i)>>20) & 0x01)
#  define NP_EXPORT_RESULT_SAFIA_TV_DOT(e)       (((e.result.i)>>21) & 0x01)
#  define NP_EXPORT_RESULT_SAFIA_TV_MC(e)        (((e.result.i)>>22) & 0x03)

#  define NP_EXPORT_RESULT_SAFIA_AUDIO_MB(e)     (((e.result.i))     & 0x01)
#  define NP_EXPORT_RESULT_SAFIA_AUDIO_MU(e)     (((e.result.i)>> 1) & 0x01)
#  define NP_EXPORT_RESULT_SAFIA_AUDIO_COUNT(e)  (((e.result.i)>> 2) & 0x0f)
#  define NP_EXPORT_RESULT_SAFIA_AUDIO_FM(e)     (((e.result.i)>> 6) & 0x03)

#  define NP_EXPORT_RESULT_AACS_REC_BD_CCI(e)    (((e.result.i))     & 0x03)
#  define NP_EXPORT_RESULT_AACS_REC_BD_EPN(e)    (((e.result.i)>> 2) & 0x01)
#  define NP_EXPORT_RESULT_AACS_REC_BD_APS(e)    (((e.result.i)>> 8) & 0x07)
#  define NP_EXPORT_RESULT_AACS_REC_BD_DOT(e)    (((e.result.i)>>11) & 0x01)
#  define NP_EXPORT_RESULT_AACS_REC_BD_ICT(e)    (((e.result.i)>>12) & 0x01)
#  define NP_EXPORT_RESULT_AACS_REC_BD_TI(e)     (((e.result.i)>>13) & 0x01)

#  define NP_EXPORT_RESULT_AACS_REC_BDRL_CCI(e)  (((e.result.i))     & 0x03)
#  define NP_EXPORT_RESULT_AACS_REC_BDRL_EPN(e)  (((e.result.i)>> 2) & 0x01)
#  define NP_EXPORT_RESULT_AACS_REC_BDRL_APS(e)  (((e.result.i)>> 8) & 0x07)
#  define NP_EXPORT_RESULT_AACS_REC_BDRL_DOT(e)  (((e.result.i)>>11) & 0x01)
#  define NP_EXPORT_RESULT_AACS_REC_BDRL_ICT(e)  (((e.result.i)>>12) & 0x01)
#  define NP_EXPORT_RESULT_AACS_REC_BDRL_TI(e)   (((e.result.i)>>13) & 0x01)

/* Output Control macros and definitions for nautilus' BBTS support*/

#define NP_OC_BASIC_VALID                        (1 << 0)
#define NP_OC_DTCP_VALID                         (1 << 1)

#define NP_OC_BASIC_ALL_FLAGS_VALID              ( NP_OC_BASIC_EPN_VALID | \
                                                   NP_OC_BASIC_CCI_VALID | \
                                                   NP_OC_BASIC_ICT_VALID | \
                                                   NP_OC_BASIC_DOT_VALID | \
                                                   NP_OC_BASIC_APS_VALID )

#define NP_OC_BASIC_EPN_VALID                    (1 << 0)
#define NP_OC_BASIC_CCI_VALID                    (1 << 1)
#define NP_OC_BASIC_ICT_VALID                    (1 << 2)
#define NP_OC_BASIC_DOT_VALID                    (1 << 3)
#define NP_OC_BASIC_APS_VALID                    (1 << 4)

#define NP_OC_DTCP_ALL_FLAGS_VALID               ( NP_OC_DTCP_RMM_VALID | \
                                                   NP_OC_DTCP_RS_VALID  | \
                                                   NP_OC_DTCP_EPN_VALID | \
                                                   NP_OC_DTCP_CCI_VALID | \
                                                   NP_OC_DTCP_ICT_VALID | \
                                                   NP_OC_DTCP_APS_VALID )

#define NP_OC_DTCP_RMM_VALID                     (1 << 0)
#define NP_OC_DTCP_RS_VALID                      (1 << 1)
#define NP_OC_DTCP_EPN_VALID                     (1 << 2)
#define NP_OC_DTCP_CCI_VALID                     (1 << 3)
#define NP_OC_DTCP_ICT_VALID                     (1 << 4)
#define NP_OC_DTCP_APS_VALID                     (1 << 5)

/**
 * @brief Generate a np_moc_t value from basicCCI values (see Marlin Output Control Specification 1.0.1 $ 4.1)
 *
 * Use the np_moc_t only through the macros NP_GET_BASIC_*
 * The 32-bit integer has 2 parts:
 * 1. The Most Significant 16bits hold the flag mask;
 *    a flag mask's set bit enables the corresponding CCI field (epn, cci, ...)
 * 2. The second part has holds the CCI fields (epn, cci, ...)
 *    The layout is as follows: | DOT | EPN | ICT | APS2 | APS1 | CCI2 | CCI1 |
 *    The layout according [iec]:     | RCT | CIT | APS1 | APS0 | EMI1 | EMI0 |
 *    This layout matches the bit pattern described in [iec] p 68, table 13.
 *    See also [BBTS] p 14, table 7.
 */
#define NP_BUILD_BASIC_MOC(flags,epn,cci,ict,dot,aps)    \
    ((np_moc_t)(NP_BASIC_FLAGS_TO_MOC(flags) |           \
                NP_BASIC_CCI_TO_MOC(cci) |               \
                NP_BASIC_APS_TO_MOC(aps) |               \
                NP_BASIC_ICT_TO_MOC(ict) |               \
                NP_BASIC_EPN_TO_MOC(epn) |               \
                NP_BASIC_DOT_TO_MOC(dot) ) )

/**
 * @brief Generate a np_moc_t value from DTCP CCI values (see Marlin Output Control Specification 1.0.1 $ 4.2)
 *
 * Use the np_moc_t only through the macros NP_GET_DTCP_*
 * The structure is similar to the one described in NP_BUILD_BASIC_MOC()
 * The layout is as follows: | APS2 | APS1 | ICT | CCI2 | CCI1 | EPN | RS3 | RS2 | RS1 | RMM |
 */
#define NP_BUILD_DTCP_MOC(flags,rmm,rs,epn,cci,ict,aps) \
    ((np_moc_t)(NP_DTCP_FLAGS_TO_MOC(flags) |           \
                NP_DTCP_RMM_TO_MOC(rmm)     |           \
                NP_DTCP_RS_TO_MOC(rs)       |           \
                NP_DTCP_EPN_TO_MOC(epn)     |           \
                NP_DTCP_CCI_TO_MOC(cci)     |           \
                NP_DTCP_ICT_TO_MOC(ict)     |           \
                NP_DTCP_APS_TO_MOC(aps) ) )

/** Extract the flags from a np_moc_t as integer*/
#define NP_GET_BASIC_FLAGS(moc)                  (0x1f & ((u_int32_t)(moc)>>16))
/** Extract the CCI (EMI) from a np_moc_t as integer*/
#define NP_GET_BASIC_CCI(moc)                    (0x03 & ((u_int32_t)(moc)>>0))
/** Extract the APS from a np_moc_t as integer*/
#define NP_GET_BASIC_APS(moc)                    (0x03 & ((u_int32_t)(moc)>>2))
/** Extract the ICT (CIT) from a np_moc_t as integer*/
#define NP_GET_BASIC_ICT(moc)                    (0x01 & ((u_int32_t)(moc)>>4))
/** Extract the EPN (RCT) from a np_moc_t as integer*/
#define NP_GET_BASIC_EPN(moc)                    (0x01 & ((u_int32_t)(moc)>>5))
/** Extract the DOT from a np_moc_t as integer*/
#define NP_GET_BASIC_DOT(moc)                    (0x01 & ((u_int32_t)(moc)>>6))

/** Extract the flags from a np_moc_t as integer*/
#define NP_GET_DTCP_FLAGS(moc)                   (0x3f & ((u_int32_t)(moc)>>16))
/** Extract the RetentionMoveMode from a np_moc_t as integer*/
#define NP_GET_DTCP_RMM(moc)                     (0x01 & ((u_int32_t)(moc)>>0))
/** Extract the RetentionState from a np_moc_t as integer*/
#define NP_GET_DTCP_RS(moc)                      (0x07 & ((u_int32_t)(moc)>>1))
/** Extract the EPN from a np_moc_t as integer*/
#define NP_GET_DTCP_EPN(moc)                     (0x01 & ((u_int32_t)(moc)>>4))
/** Extract the CCI from a np_moc_t as integer*/
#define NP_GET_DTCP_CCI(moc)                     (0x03 & ((u_int32_t)(moc)>>5))
/** Extract the ImageConstraintToken from a np_moc_t as integer*/
#define NP_GET_DTCP_ICT(moc)                     (0x01 & ((u_int32_t)(moc)>>7))
/** Extract the APS (reserved for future) from a np_moc_t as integer*/
#define NP_GET_DTCP_APS(moc)                     (0x03 & ((u_int32_t)(moc)>>8))

#define NP_BASIC_FLAGS_TO_MOC(flags)             ((0x1f & (u_int32_t)(flags))<<16)
#define NP_BASIC_CCI_TO_MOC(cci)                 ((0x03 & (u_int32_t)(cci))  <<0)
#define NP_BASIC_APS_TO_MOC(aps)                 ((0x03 & (u_int32_t)(aps))  <<2)
#define NP_BASIC_ICT_TO_MOC(ict)                 ((0x01 & (u_int32_t)(ict))  <<4)
#define NP_BASIC_EPN_TO_MOC(epn)                 ((0x01 & (u_int32_t)(epn))  <<5)
#define NP_BASIC_DOT_TO_MOC(dot)                 ((0x01 & (u_int32_t)(dot))  <<6)

#define NP_DTCP_FLAGS_TO_MOC(flags)              ((0x3f & (u_int32_t)(flags))<<16)
#define NP_DTCP_RMM_TO_MOC(rmm)                  ((0x01 & (u_int32_t)(rmm))  <<0)
#define NP_DTCP_RS_TO_MOC(rs)                    ((0x07 & (u_int32_t)(rs))   <<1)
#define NP_DTCP_EPN_TO_MOC(epn)                  ((0x01 & (u_int32_t)(epn))  <<4)
#define NP_DTCP_CCI_TO_MOC(cci)                  ((0x03 & (u_int32_t)(cci))  <<5)
#define NP_DTCP_ICT_TO_MOC(itc)                  ((0x01 & (u_int32_t)(itc))  <<7)
#define NP_DTCP_APS_TO_MOC(aps)                  ((0x03 & (u_int32_t)(aps))  <<8)

typedef u_int32_t   np_moc_t;    /**< Marlin Output control*/

typedef struct {
    u_int32_t      flags;        /**< Indicate which types are valid in the struct*/
    struct {
        u_int32_t  basic_flags;  /**< Flags indicating which fields are valid*/
        u_int32_t  epn;          /**< 1 bit (least significant)*/
        u_int32_t  cci;          /**< 2 bit Copy Control Information (least significant)*/
        u_int32_t  ict;          /**< 1 bit Image Constraint Token (least significant)*/
        u_int32_t  dot;          /**< 1 bit Digital Only Token (least significant)*/
        u_int32_t  aps;          /**< 2 bit (least significant)*/
    } basic;
    struct {
        u_int32_t  dtcp_flags;   /**< Flags indicating which fields are valid*/
        u_int32_t  rmm;          /**< 1 bit Retention Move Mode (least significant)*/
        u_int32_t  rs;           /**< 3 bit Retention State(least significant)*/
        u_int32_t  epn;          /**< 1 bit (least significant)*/
        u_int32_t  cci;          /**< 2 bit Copy Control Information (least significant)*/
        u_int32_t  ict;          /**< 1 bit Image Constraint Token (least significant)*/
        u_int32_t  aps;          /**< 2 bit (least significant)*/
    } dtcp;
    void *reserved_data;         /**< Future proofing to contain extended oc*/
} np_output_control_t;

NP_END_EXTERN_C

#endif /* NPCOMMON_TYPES_H */
/** @} */
