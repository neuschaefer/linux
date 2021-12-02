/*
 * Nautilus version 3.5
 *
 * Any code and mechanism in this module may not be used
 * in any form without permissions.
 * Copyright (C) 2008,2009,2010,2011 Sony Corporation, All Rights Reserved.
 */

#ifndef SMI_TYPES_H
#define SMI_TYPES_H

NP_BEGIN_EXTERN_C

/**
 * @brief The base of smi message id's.
 */
#define SMI_MSGID_BASE (1024)

/** 
 * @brief The range of Device specific smi message id's. 
 *        The device may use this range for its own purpose. 
 */
#define SMI_MSGID_DEVICE_SPECIFIC_BASE (SMI_MSGID_BASE + 0x300)                 
#define SMI_MSGID_DEVICE_SPECIFIC_END  (SMI_MSGID_BASE + 0x3FF) 

/**
 * @brief The smi_id_t enum contains id's for all np functions that can be
 *        called through the smi.
 */
typedef enum {
    SMI_MSGID_INIT_SESSION = SMI_MSGID_BASE,
    SMI_MSGID_FIN_SESSION,
    SMI_MSGID_CHECK_PERSONALITY,
    SMI_MSGID_CHECK_LINK,
    SMI_MSGID_CHECK_DCSASSERTION,
    SMI_MSGID_CHECK_CRL_UPTODATE,
    SMI_MSGID_CHECK_SDATA_STATUS,
    SMI_MSGID_GET_NODEID,
    SMI_MSGID_CRL_UPDATE,
    SMI_MSGID_CRL_CLEAR,
    SMI_MSGID_CRL_GETNUMBER,
    SMI_MSGID_CRL_GET_DISTRIBUTIONPOINT,
    SMI_MSGID_CRL_COMPARE,
    SMI_MSGID_CLEAR_REGISTRATIONSTATUS,
    SMI_MSGID_INIT_ACCOUNTSESSION,
    SMI_MSGID_FIN_ACCOUNTSESSION,
    SMI_MSGID_RECOVER_SDATA,
    SMI_MSGID_SET_SDATA,
    SMI_MSGID_STORE_REMOTE_SDATA,

    SMI_MSGID_INIT_PERS_HANDLE,
    SMI_MSGID_FIN_PERS_HANDLE,
    SMI_MSGID_NEMO_PERS_1,
    SMI_MSGID_NEMO_PERS_2,
    SMI_MSGID_NEMO_PERS_3,
    SMI_MSGID_OCT_PERS_1,
    SMI_MSGID_OCT_PERS_2,

    SMI_MSGID_INIT_BB_HANDLE,
    SMI_MSGID_FIN_BB_HANDLE,
    SMI_MSGID_BB_NODEACQ_1,
    SMI_MSGID_BB_NODEACQ_2,
    SMI_MSGID_BB_LINKACQ_1,
    SMI_MSGID_BB_LINKACQ_2,
    SMI_MSGID_BB_LICACQ_1,
    SMI_MSGID_BB_LICACQ_2,
    SMI_MSGID_BB_DEREG_1,
    SMI_MSGID_BB_DEREG_2,
    SMI_MSGID_BB_CHECK_CERTSET_DISCONTINUED,
    SMI_MSGID_BB_SET_CERTSET,
    SMI_MSGID_BB_GETSOAPFAULT,

    SMI_MSGID_PERS_SET_CERTSET,
    SMI_MSGID_PERS_GETSOAPFAULT,
    SMI_MSGID_SET_LICENSE,

    SMI_MSGID_INIT_TRACK,
    SMI_MSGID_FIN_TRACK,
    SMI_MSGID_DRM_ACTION_CHECK,
    SMI_MSGID_DRM_ACTION_PERFORM,
    SMI_MSGID_DRM_ACTION_DESCRIBE,
    SMI_MSGID_DRM_CALLBACK,
    SMI_MSGID_DRM_LINKCONSTRAINTDESCRIBE,
    SMI_MSGID_DRM_DECRYPT,
    SMI_MSGID_DRM_CHECK_HMAC,

    SMI_MSGID_DATA_CERTIFICATION_1,
    SMI_MSGID_DATA_CERTIFICATION_2,
    SMI_MSGID_DATA_UPDATE_1,
    SMI_MSGID_DATA_UPDATE_2,

    SMI_MSGID_REG_NEMOPERS,
    SMI_MSGID_REG_NEMOPUBKEYS,
    SMI_MSGID_REG_NEMOPRVKEYS,
    SMI_MSGID_REG_ROLEASSERTION,
    SMI_MSGID_REG_PUBPERSONALITY,
    SMI_MSGID_REG_PRVPERSONALITY,
    SMI_MSGID_REG_LINK,
    SMI_MSGID_REG_DCSA,
    SMI_MSGID_DEREG_LINKBYID,
    SMI_MSGID_SET_TRUSTEDTIME,
    SMI_MSGID_CLEAR_SDATA,

    SMI_MSGID_LTP_INIT,
    SMI_MSGID_LTP_FIN,
    SMI_MSGID_LTP_SETUP_MSGBUILD,
    SMI_MSGID_LTP_SETUP_MSGPROC,
    SMI_MSGID_LTP_RUNAGENT_MSGPROC,
    SMI_MSGID_LTP_AGENTRESULT_MSGPROC,
    SMI_MSGID_LTP_TEARDOWN_MSGPROC,

    SMI_MSGID_PPPROC_INIT,
    SMI_MSGID_PPPROC_FIN,
    SMI_MSGID_PPPROC_GENERATEREQUEST,
    SMI_MSGID_PPPROC_PARSERESPONSE,

    SMI_MSGID_DRM_ACTION_EXPORT_CHECK,
    SMI_MSGID_DRM_ACTION_EXPORT_DESCRIBE,
    SMI_MSGID_DRM_ACTION_EXPORT_PERFORM,
    SMI_MSGID_DRM_ACTION_EXPORT_GETKEY,

    SMI_MSGID_DRM_MOVEKEY,

    SMI_MSGID_GOBY_GET_AVAILABLE_LINKS,
    SMI_MSGID_GOBY_DELETE_LINK,
    SMI_MSGID_GOBY_GET_LINK_PERIOD,
    SMI_MSGID_BB_SET_SID,
    SMI_MSGID_GOBY_GET_LINKS_FOR_SERVICE_ID,
    SMI_MSGID_GOBY_PPPROC_GENERATEREQUEST,
    SMI_MSGID_GOBY_PPPROC_PROCESSRESPONSE,
    SMI_MSGID_GOBY_GET_DUID_PARAMETERS,

    SMI_MSGID_INIT_MS3_SESSION,
    SMI_MSGID_FIN_MS3_SESSION,
    SMI_MSGID_MS3_PARSE_SAS,
    SMI_MSGID_GET_NEMO_KEYS,
    SMI_MSGID_SAS_INFO_COMPLETE,
    SMI_MSGID_CURIT_EXPAND,

    SMI_MSGID_BBTS_INIT_SESSION,
    SMI_MSGID_BBTS_FIN_SESSION,
    SMI_MSGID_BBTS_SET_SOCID,
    SMI_MSGID_BBTS_INIT_ECM_STREAM,
	
    SMI_MSGID_MAX = 0x4fffffff
} smi_id_t;

#ifdef NPOPT_ENABLE_TESTHOOKS
/**
 * @brief The smi_th_id_t enum contains additional id's for all testhook np
 *        functions that can be called through the smi.
 */
typedef enum {
    SMI_MSGID_TH_LOADSSTORAGE = 0x50000000,
    SMI_MSGID_TH_UPDATESSTORAGE,
    SMI_MSGID_TH_LIL_APPEND,
    SMI_MSGID_TH_REGSUSPENSIONLIST,
    SMI_MSGID_TH_CHECK_ASSERTION,
    SMI_MSGID_TH_GETROLEASSERTION,
    SMI_MSGID_TH_LILAPPEND,
    SMI_MSGID_TH_SETNSCFGVAL,
    SMI_MSGID_TH_GETSECUREDSDATA,
    SMI_MSGID_TH_REG_LINK
} smi_th_id_t;
#endif

/**
 * @brief The smi_param_t enum contains id's for all the types of the message
 *        parameters that can be sent through the smi.
 */
typedef enum {
    SMI_PARAM_HANDLE = 0,
    SMI_PARAM_UINT8PTR,
    SMI_PARAM_INT32,
    SMI_PARAM_UINT32,
    SMI_PARAM_NP_DATE,
    SMI_PARAM_NP_SIZE,
    SMI_PARAM_NP_STR,
    SMI_PARAM_NP_BOOL,
    SMI_PARAM_NP_CRLNUMBER,
    SMI_PARAM_NP_CALLBACK,
    SMI_PARAM_NP_CRTSTDLST,
    SMI_PARAM_NP_CERTSTDLST,
    SMI_PARAM_NP_SOAPFAULT,
    SMI_PARAM_NP_TRACKINFO,
    SMI_PARAM_NP_SASINFO
} smi_param_t;


/**
 * @brief An smi value parameter, used to prepare all the parameters before
 *        they are packed inside a smi message.
 */
typedef struct smi_param_val_s {
    smi_param_t type;
    void *param;
} smi_param_val_t;

/**
 * @brief The smi_params_s struct is used to aggregate smi value parameters
 *        before they are used to contruct an smi message.
 */
typedef struct smi_params_s {
    u_int32_t nb;
    smi_param_val_t *params;
} smi_params_t;


/**
 * @brief Accessors for parameter structures.
 */
#define SMI_PARAMS_INIT                     { 0, NULL }

#define smi_setParamValue(par, t, v)        do { (par).type = (t); (par).param = (v);   } while(0)
#define smi_setPayload( pars, p )           do { (pars).nb = sizeof((p))/sizeof(smi_param_val_t); (pars).params = (p); } while(0)

#define smi_NumParams(smi)                  ((smi)->params ? (smi)->nb : 0)
#define smi_ParamsListLen(smi)              (smi)->nb
#define smi_ParamsListParams(smi)           (smi)->params
#define smi_ParamAtIdx(smi,idx)             &(((smi)->params)[(idx)])
#define smi_ParamTypeAtIdx(smi,idx)         (((smi)->params)[(idx)]).type

/**
 * @brief Accessors for smi_param_val_t.
 */
#define smi_ParamValue(smi)                 (smi)->param
#define smi_ParamType(smi)                  (smi)->type

/**
 * @brief header of smi messages / paramaters.
 */
#define SMI_HEADER_SIZE              16                                             /* Size of the header 4 x sizeof(u_int32_t) */
#define SMI_MSG_PARAM_SIZE           8                                               /* Size of the param 2 x sizeof(u_int32_t) */
#define smi_MsgParamValue(smi)       (smi)+8                                         /* Access to parameter value position. */

NP_END_EXTERN_C

#endif /* SMI_TYPES_H */

