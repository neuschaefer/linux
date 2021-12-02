/*----------------------------------------------------------------------------*
 * Copyright Statement:                                                       *
 *                                                                            *
 *   This software/firmware and related documentation ("MediaTek Software")   *
 * are protected under international and related jurisdictions'copyright laws *
 * as unpublished works. The information contained herein is confidential and *
 * proprietary to MediaTek Inc. Without the prior written permission of       *
 * MediaTek Inc., any reproduction, modification, use or disclosure of        *
 * MediaTek Software, and information contained herein, in whole or in part,  *
 * shall be strictly prohibited.                                              *
 * MediaTek Inc. Copyright (C) 2010. All rights reserved.                     *
 *                                                                            *
 *   BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND     *
 * AGREES TO THE FOLLOWING:                                                   *
 *                                                                            *
 *   1)Any and all intellectual property rights (including without            *
 * limitation, patent, copyright, and trade secrets) in and to this           *
 * Software/firmware and related documentation ("MediaTek Software") shall    *
 * remain the exclusive property of MediaTek Inc. Any and all intellectual    *
 * property rights (including without limitation, patent, copyright, and      *
 * trade secrets) in and to any modifications and derivatives to MediaTek     *
 * Software, whoever made, shall also remain the exclusive property of        *
 * MediaTek Inc.  Nothing herein shall be construed as any transfer of any    *
 * title to any intellectual property right in MediaTek Software to Receiver. *
 *                                                                            *
 *   2)This MediaTek Software Receiver received from MediaTek Inc. and/or its *
 * representatives is provided to Receiver on an "AS IS" basis only.          *
 * MediaTek Inc. expressly disclaims all warranties, expressed or implied,    *
 * including but not limited to any implied warranties of merchantability,    *
 * non-infringement and fitness for a particular purpose and any warranties   *
 * arising out of course of performance, course of dealing or usage of trade. *
 * MediaTek Inc. does not provide any warranty whatsoever with respect to the *
 * software of any third party which may be used by, incorporated in, or      *
 * supplied with the MediaTek Software, and Receiver agrees to look only to   *
 * such third parties for any warranty claim relating thereto.  Receiver      *
 * expressly acknowledges that it is Receiver's sole responsibility to obtain *
 * from any third party all proper licenses contained in or delivered with    *
 * MediaTek Software.  MediaTek is not responsible for any MediaTek Software  *
 * releases made to Receiver's specifications or to conform to a particular   *
 * standard or open forum.                                                    *
 *                                                                            *
 *   3)Receiver further acknowledge that Receiver may, either presently       *
 * and/or in the future, instruct MediaTek Inc. to assist it in the           *
 * development and the implementation, in accordance with Receiver's designs, *
 * of certain softwares relating to Receiver's product(s) (the "Services").   *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MediaTek Inc. with respect  *
 * to the Services provided, and the Services are provided on an "AS IS"      *
 * basis. Receiver further acknowledges that the Services may contain errors  *
 * that testing is important and it is solely responsible for fully testing   *
 * the Services and/or derivatives thereof before they are used, sublicensed  *
 * or distributed. Should there be any third party action brought against     *
 * MediaTek Inc. arising out of or relating to the Services, Receiver agree   *
 * to fully indemnify and hold MediaTek Inc. harmless.  If the parties        *
 * mutually agree to enter into or continue a business relationship or other  *
 * arrangement, the terms and conditions set forth herein shall remain        *
 * effective and, unless explicitly stated otherwise, shall prevail in the    *
 * event of a conflict in the terms in any agreements entered into between    *
 * the parties.                                                               *
 *                                                                            *
 *   4)Receiver's sole and exclusive remedy and MediaTek Inc.'s entire and    *
 * cumulative liability with respect to MediaTek Software released hereunder  *
 * will be, at MediaTek Inc.'s sole discretion, to replace or revise the      *
 * MediaTek Software at issue.                                                *
 *                                                                            *
 *   5)The transaction contemplated hereunder shall be construed in           *
 * accordance with the laws of Singapore, excluding its conflict of laws      *
 * principles.  Any disputes, controversies or claims arising thereof and     *
 * related thereto shall be settled via arbitration in Singapore, under the   *
 * then current rules of the International Chamber of Commerce (ICC).  The    *
 * arbitration shall be conducted in English. The awards of the arbitration   *
 * shall be final and binding upon both parties and shall be entered and      *
 * enforceable in any court of competent jurisdiction.                        *
 *---------------------------------------------------------------------------*/


#include "inc/u_common.h"
#include "dbg/x_dbg.h"
#include "res_mngr/drv/u_irrc_btn_def.h"
#include "os/inc/x_os.h"
#include "handle/handle.h"
#include "handle/u_handle_grp.h"
#include "conn_mngr/brdcst/u_brdcst.h"
#include "conn_mngr/x_cm.h"
#include "tbl_mngr/x_tm.h"
#include "tbl_mngr/psi_eng/x_psi_eng.h"
#include "tbl_mngr/dvb_si_eng/x_dvb_si_eng.h"
#include "svl_bldr/sb_dvb_eng/u_sb_dvb_eng.h"
#include "tsl/x_tsl.h"
#include "svl/x_svl.h"
#include "svctx/svctx_api.h"
#include "evctx/x_evctx.h"
#include "evctx/evctx_atsc_eng/u_evctx_atsc_eng.h"
#ifdef MW_EDB_SUPPORT
#include "edb/x_edb.h"
#endif 
#include "util/x_lnk_list.h"
#include "mheg5/x_mheg5.h"
#include "scdb/u_scdb.h"
#include "svl_bldr/u_sb.h"
#include "dt/u_dt.h"
#include "dt/x_dt.h"
#include <string.h>

#include "mheg5/OceanBlue/glue/si_loader/sil.h"
#include "mheg5/OceanBlue/glue/mheg5_OceanBlue_eng.h"


/* MHEG-5 stack header files */
#include "mheg5/OceanBlue/mhegPortFuncs/inc/pm_types.h"
#include "mheg5/OceanBlue/mhegPortFuncs/inc/pm_errors.h"
#include "mheg5/OceanBlue/mhegPortFuncs/inc/pm_si.h"
#include "mheg5/OceanBlue/mhegPortFuncs/inc/pm_tune.h"

#ifdef DEBUG
#if 0
#define SI_DEBUG(x)  x_dbg_stmt("[%s %d] ",__FILE__,__LINE__);x_dbg_stmt x
#else
#define SI_DEBUG(x)  
#endif
#else
#define SI_DEBUG(x)  
#endif

/*-----------------------------------------------------------------------------
 * structure, constants, macro definitions
 *---------------------------------------------------------------------------*/
#ifndef MW_DTMB_SUPPORT
#define MHEG5_SIL_DEFAULT_BRDCST_TYPE BRDCST_TYPE_DVB
#else
#define MHEG5_SIL_DEFAULT_BRDCST_TYPE BRDCST_TYPE_DTMB
#endif
#define _EVCTX_ERROR                    "{EVCTX} ERR: "
#define _EVCTX_API                      "{EVCTX} "
#define _EVCTX_INFO                     "{EVCTX} "

#define DBG_SIL_ERR(x)  x_dbg_stmt("[MHEG5-SIL] ERR:"x) 
#define DBG_SIL_INFO(x) x_dbg_stmt("[MHEG5-SIL] INFO:"x)

#define ISO_3166_UK     "GBR"
#define ISO_3166_NZ     "NZL"
#define ISO_3166_IRL    "IRL"
#define ISO_3166_AUS    "AUS"
#define ISO_3166_CHN    "CHN"

typedef enum __LOADER_STATE_T
{
    LDR_ST_UNINIT = 0,
    LDR_ST_OPENING,
    LDR_ST_OPENED,
    LDR_ST_CONNECTING,
    LDR_ST_CONNECTED,
    LDR_ST_LOADING_NIT,
    LDR_ST_NIT_LOADED,
    LDR_ST_LOADING_SDT,
    LDR_ST_SDT_LOADED,
    LDR_ST_LOADING_PERSISTENT_TBL,
    LDR_ST_PERSISTENT_TBL_LOADED,
    LDR_ST_WAIT_FOR_NOTIFY,
    LDR_ST_CLOSED
} _LOADER_STATE_T;

typedef struct __SI_QUERY_T             _SI_QUERY_T;
typedef struct __SVC_INFO_T             _SVC_INFO_T;
typedef struct __SDT_INFO_T             _SDT_INFO_T;
typedef struct __SVC_INDEX_T            _SVC_INDEX_T;

typedef SLIST_T(__SI_QUERY_T)           SLIST_SI_QUERY_T;
typedef SLIST_T(__SVC_INFO_T)           SLIST_SVC_INFO_T;
typedef SLIST_T(__SDT_INFO_T)           SLIST_SDT_INFO_T;
typedef SLIST_T(__SVC_INDEX_T)          SLIST_SVC_INDEX_T;

typedef SLIST_ENTRY_T(__SI_QUERY_T)     SLIST_ENTRY_SI_QUERY_T;
typedef SLIST_ENTRY_T(__SVC_INFO_T)     SLIST_ENTRY_SVC_INFO_T;
typedef SLIST_ENTRY_T(__SDT_INFO_T)     SLIST_ENTRY_SDT_INFO_T;
typedef SLIST_ENTRY_T(__SVC_INDEX_T)    SLIST_ENTRY_SVC_INDEX_T;

typedef struct __STREAM_ANNOTATION_T
{
    STREAM_TYPE_T   e_strm_type;
    ISO_639_LANG_T  s_lang;
    ISO_639_LANG_T  s_gui_lang;
    AUD_TYPE_T      e_aud_type;
} _STREAM_ANNOTATION_T;

struct __SI_QUERY_T
{
    HANDLE_T                h_this;
    
    VOID*                   pv_query_ref;
    VOID*                   pv_usr_data;

    UINT8                   ui1_status;
    UINT8                   ui1_source;
    BOOL                    b_strm_ant_valid;
    
    tmMHEG5SiQuery_t        t_query;

    _STREAM_ANNOTATION_T    t_strm_ant;
    
    SLIST_ENTRY_SI_QUERY_T  t_link;
};

struct __SVC_INFO_T
{
    HANDLE_T                h_pmt;
    UINT8                   ui1_status;
    UINT16                  ui2_svc_id;
    SLIST_ENTRY_SVC_INFO_T  t_link;
};

struct __SDT_INFO_T
{
    HANDLE_T                h_sdt;
    BOOL                    b_loaded;
    UINT16                  ui2_on_id;
    UINT16                  ui2_ts_id;
    SLIST_ENTRY_SDT_INFO_T  t_link;
};

struct __SVC_INDEX_T
{    
    UINT32  ui4_channel_id;
    UINT32  ui4_svc_index;
    
    SLIST_ENTRY_SVC_INDEX_T t_link;
};


typedef struct __SI_LOADER_INFO_T
{
    HANDLE_T            h_lock;
    _LOADER_STATE_T     e_state;
    UINT32              ui4_seq_num;
    
    HANDLE_T            h_msg_q;
    
    HANDLE_T            h_tsl;
    HANDLE_T            h_svl;
    
    UINT16              ui2_tsl_id;
    UINT16              ui2_tsl_rec_id;
    
    UINT16              ui2_svl_id;
    UINT16              ui2_svl_rec_id;
    
    UINT16              ui2_on_id;
    UINT16              ui2_ts_id;
    UINT16              ui2_active_svc_id;
    
    MPEG_2_PID_T        t_boot_pid;
    UINT32              ui4_boot_carousel_id;
    UINT8               ui1_boot_comp_tag;
    BOOL                b_data_broadcast_id_desc_found;
    
    BOOL                b_nb_info_present;
    BOOL                ui1_nb_version;
    
    BOOL                b_new_conn;

    UINT32              ui4_tbl_status;
    
    HANDLE_T            h_pipe;
    HANDLE_T            h_conn;
    
    /* SI/PSI table */
    HANDLE_T            h_root;
    HANDLE_T            h_nit;
    HANDLE_T            h_sdt_actual;
    HANDLE_T            h_pat;
    HANDLE_T            h_pmt_active;

    SLIST_SDT_INFO_T    t_sdt_other_lst;
    
    SLIST_SI_QUERY_T    t_si_query_lst;
    SLIST_SVC_INFO_T    t_svc_info_lst;

    SLIST_SI_QUERY_T    t_si_close_lst;

    SLIST_SVC_INDEX_T   t_svc_index_lst;

    x_mheg5_eng_load_nfy_fct    pf_load_nfy;
    VOID*                       pv_load_tag;

    #ifdef MHEG5_HD_SUPPORT
    MPEG_2_PID_T        t_si_carousel_pid;
    #endif
} _SI_LOADER_INFO_T;

typedef struct __SIL_NOTIFY_T
{
    UINT16      ui2_on_id;
    UINT16      ui2_ts_id;
    UINT16      ui2_svc_id;
    UINT32      ui4_seq_num;
} _SIL_NOTIFY_T;

typedef struct __SIL_TIMER_T
{
    UINT16      ui2_msg_type;
    UINT16      ui2_on_id;
    UINT16      ui2_ts_id;
    UINT16      ui2_svc_id;
    HANDLE_T    h_this;
    HANDLE_T    h_table;
    UINT32      ui4_seq_num;
} _SIL_TIMER_T;

typedef struct __PARSE_PARAM_T
{
    UINT8       ui1_flag;
    UINT8       ui1_mask;
    
    UINT8       ui1_comp_tag;
    UINT8       ui1_nb_version;
    UINT8       ui1_nb_action;

    UINT8       ui1_ref_len;
    UINT32      ui4_ref;
    
    UINT8*      pui1_descriptor;
    UINT16      ui2_desc_len;
    
    UINT16      ui2_association_tag;
    
    UINT16      ui2_ts_id;
    UINT16      ui2_svc_id;
    UINT16      ui2_on_id;

    INT16       i2_boot_priority_hint;
} _PARSE_PARAM_T;

typedef struct
{
    ISO_639_LANG_T      s_lang;
    AUD_TYPE_T          e_aud_type;
    BOOL                b_lang_found;
    BOOL                b_gui_lang;
    BOOL                b_ad_matched;
} _ISO_639_DESC;

typedef struct
{
    _STREAM_ANNOTATION_T    *pt_strm_ant;
    _ISO_639_DESC           *pt_desc;
} _GET_LANG_INFO_T;

#define BRDCST_ISO_639_NUM                  16    /*current support 2 languages descriptor*/
#define ISO_639_LANG_LEN                    4    /**<        */

typedef struct
{
    CHAR                s_lang[BRDCST_ISO_639_NUM][ISO_639_LANG_LEN];
    INT8                ui1_index;
    AUD_TYPE_T          e_aud_type;
    BOOL                b_ad_matched;
} _COMP_ISO_639_DESC;

typedef struct
{
    _STREAM_ANNOTATION_T    *pt_strm_ant;
    _COMP_ISO_639_DESC      *pt_desc;
} _GET_COMP_LANG_INFO_T;

#if 0
typedef enum
{
    AUD_MATCHED_WITH_AD = 0,
    GUI_MATCHED_WITH_AD = 1,
    AUD_MATCHED_NO_AD   = 2,
    GUI_MATCHED_NO_AD   = 3,

    BOTH_NOT_MATCHED
} LANG_MATCH_STATUS_T;
#else
typedef UINT8 LANG_MATCH_STATUS_T;
#define BOTH_NOT_MATCHED       ((UINT8) 0)
#define GUI_MATCHED_NO_AD      ((UINT8) 1)
#define AUD_MATCHED_NO_AD      ((UINT8) 2)
#define GUI_MATCHED_WITH_AD    ((UINT8) 3)
#define AUD_MATCHED_WITH_AD    ((UINT8) 4)
#endif

typedef UINT8 AUDIO_CODEC_WEIGHTING_T;
#define HEAACV4_CODEC_WEIGHTING ((UINT8) 6)
#define HEAACV2_CODEC_WEIGHTING ((UINT8) 5)
#define AACV4_CODEC_WEIGHTING   ((UINT8) 4)
#define EAC3_CODEC_WEIGHTING    ((UINT8) 3)
#define AC3_CODEC_WEIGHTING     ((UINT8) 2)
#define MPEG1_CODEC_WEIGHTING   ((UINT8) 1)

typedef struct _AC3_AAC_AUDIO_INFO_T
{
    BOOL                    b_found;
    AUD_ENC_T               e_aud_enc;
    AUD_PROF_LVL_T          e_prof_lvl;
    AUDIO_CODEC_WEIGHTING_T t_weighting;  
} AC3_AAC_AUDIO_INFO_T;

/* S639_LANG Try Times, above 4 lang plus NULL */
#define MHEG_HK_S639_LANG_TRY_TIMES ((UINT8)5)
typedef struct _MHEG5_EVENT_LOADING_INFO_T
{
    HANDLE_T h_sema;
    HANDLE_T h_evt;
    
    EVCTX_COND_T e_cond;
} MHEG5_EVENT_LOADING_INFO_T;



#ifdef MW_EDB_SUPPORT
#else
static CHAR* mheg_hk_s639_lang_pool[MHEG_HK_S639_LANG_TRY_TIMES] = {"eng", "chi", "zho", "und", NULL};
#endif



#define SIL_MSG_PIPE_OPENED     ((UINT16)  0)
#define SIL_MSG_PIPE_CLOSED     ((UINT16)  1)
#define SIL_MSG_CONNECTED       ((UINT16)  2)
#define SIL_MSG_DISCONNECTED    ((UINT16)  3)
#define SIL_MSG_SDT_UPDATE      ((UINT16)  4)
#define SIL_MSG_SDT_UNAVAIL     ((UINT16)  5)
#define SIL_MSG_SDT_TIMEOUT     ((UINT16)  6)
#define SIL_MSG_PAT_UPDATE      ((UINT16)  7)
#define SIL_MSG_PAT_UNAVAIL     ((UINT16)  8)
#define SIL_MSG_PAT_TIMEOUT     ((UINT16)  9)
#define SIL_MSG_PMT_UPDATE      ((UINT16) 10)
#define SIL_MSG_PMT_UNAVAIL     ((UINT16) 11)
#define SIL_MSG_PMT_TIMEOUT     ((UINT16) 12)
#define SIL_MSG_NIT_UPDATE      ((UINT16) 13)
#define SIL_MSG_NIT_UNAVAIL     ((UINT16) 14)
#define SIL_MSG_NIT_TIMEOUT     ((UINT16) 15)

#define LOCK(l)             x_sema_lock((l)->h_lock, X_SEMA_OPTION_WAIT)
#define UNLOCK(l)           x_sema_unlock((l)->h_lock)

#define SEQ_NUM(l)          ((l)->ui4_seq_num)

#define ST_OPENED(l)        (LDR_ST_OPENED == (l)->e_state)

#define SI_CLOSE_LIST(l)    ((l)->t_si_close_lst)
#define SI_QUERY_LIST(l)    ((l)->t_si_query_lst)
#define SVC_INFO_LIST(l)    ((l)->t_svc_info_lst)
#define SDT_OTHER_LIST(l)   ((l)->t_sdt_other_lst)
#define SVC_INDEX_LIST(l)   ((l)->t_svc_index_lst)


#define TM_ROOT_NAME_LEN        32
#define MHEG5_ROOT_NAME_PREFIX  "mheg5_"

#define SIL_SUCCEEDED           ((INT32)  0)
#define SIL_FAILED              ((INT32) -1)
#define SIL_ASYNC_NOTIFY        ((INT32)  1)

#define SI_QUERY_STATUS_PENDING         0
#define SI_QUERY_STATUS_CLOSED          1

#define SI_QUERY_SOURCE_MHEG5_STACK     0
#define SI_QUERY_SOURCE_GLUE_LAYER      1

#define SVC_INFO_STATUS_LOADED          MAKE_BIT_MASK_8(0)
#define SVC_INFO_STATUS_SUBSCRIBED      MAKE_BIT_MASK_8(1)
#define SVC_INFO_STATUS_REMOVED         MAKE_BIT_MASK_8(2)

#define SVC_INFO_LOADED(s)              (0 != ((s)->ui1_status & SVC_INFO_STATUS_LOADED))
#define SVC_INFO_SUBSCRIBED(s)          (0 != ((s)->ui1_status & SVC_INFO_STATUS_SUBSCRIBED))

#define MHEG5_SIL_MSG_Q_NAME            "sil_msg_q"
#define MHEG5_SIL_NUM_OF_MSGS           ((UINT16) 256)
#define MHEG5_SIL_MSG_DEFAULT_PRIORITY  ((UINT8)  128)

#define MHEG5_SIL_MSG_SEND_RETRY        ((UINT16) 10)

#define MHEG5_TIMEOUT_LOAD_NIT          ((UINT32) 10000)
#define MHEG5_TIMEOUT_LOAD_SDT          ((UINT32) 15000)
#define MHEG5_TIMEOUT_LOAD_PAT          ((UINT32)  1500)
#define MHEG5_TIMEOUT_LOAD_PMT          ((UINT32)  2000)

#define BIT_SDT_ACTUAL                  MAKE_BIT_MASK_32(0)
#define BIT_PAT                         MAKE_BIT_MASK_32(1)
#define BIT_PMT_ACTIVE                  MAKE_BIT_MASK_32(2)

#if defined(SYS_MHEG5_HK_SUPPORT) || defined(SYS_MHEG5_AUS_SUPPORT)
#define PERSISTENT_TBL_MASK             (BIT_SDT_ACTUAL | BIT_PAT | BIT_PMT_ACTIVE)
#else
#define PERSISTENT_TBL_MASK             (/*BIT_SDT_ACTUAL |*/ BIT_PAT | BIT_PMT_ACTIVE)
#endif

#define PERSISTENT_TBL_DONE(x)          (((x) & PERSISTENT_TBL_MASK) == PERSISTENT_TBL_MASK)
#define SDT_ACTUAL_DONE(x)              (0 != ((x) & BIT_SDT_ACTUAL))
#define PAT_DONE(x)                     (0 != ((x) & BIT_PAT))
#define PMT_ACTIVE_DONE(x)              (0 != ((x) & BIT_PMT_ACTIVE))

/* descriptor tag */
#define TAG_CAROUSEL_ID_DESCRIPTOR              ((UINT8) 0x13)
#define TAG_ASSOCIATION_TAG_DESCRIPTOR          ((UINT8) 0x14)
#define TAG_DATA_BROADCAST_ID_DESCRIPTOR        ((UINT8) 0x66)
#define TAG_STREAM_IDENTIFIER_DESCRIPTOR        ((UINT8) 0x52)
#define TAG_DEFERRED_ASSOCIATION_TAG_DESCRIPTOR ((UINT8) 0x15)
#define TAG_LOGICAL_CHANNEL_DESCRIPTOR          ((UINT8) 0x83)
#define TAG_ISO_639_LANG_DESCRIPTOR             ((UINT8) 0x0A)
#define TAG_AC_3_DESCRIPTOR                     ((UINT8) 0x6A)
#define TAG_EAC_3_DESCRIPTOR                    ((UINT8) 0x7A)
#define TAG_DTS_DESCRIPTOR                      ((UINT8) 0x7B)
#define TAG_AAC_DESCRIPTOR                      ((UINT8) 0x7C)
#define TAG_COMP_DESCRIPTOR                     ((UINT8) 0x50)


/* sub descriptor tag */
#define TAG_NETWORK_BOOT_INFO_SUB_DESCRIPTOR    ((UINT8) 0x01)
#define TAG_SERVICE_BOOT_INFO_SUB_DESCRIPTOR    ((UINT8) 0x02)

#define BOOT_DATA_BROADCAST_ID                  ((UINT16) 0x0106)

#define APPLICATION_TYPE_CODE_LAUNCH_PROFILE    ((UINT16) 0x0101)
#define APPLICATION_TYPE_CODE_BASELINE_PROFILE  ((UINT16) 0x0505)
#define APPLICATION_TYPE_CODE_105_PROFILE       ((UINT16) 0x0102)

/* DSMCC stream type */
#define STREAMTYPE_DSMCC_UN_MESSAGE             ((UINT8) 0x0b)
#define STREAMTYPE_DSMCC_STREAM_DESCRIPTOR      ((UINT8) 0x0c)
#define STREAMTYPE_DSMCC_SECTION                ((UINT8) 0x0d)

/* Audio/video stream type */
#define STREAMTYPE_VIDEO_11172_2                ((UINT8)   1)
#define STREAMTYPE_VIDEO_13818_2                ((UINT8)   2)
#define STREAMTYPE_AUDIO_11172_3                ((UINT8)   3)
#define STREAMTYPE_AUDIO_13818_3                ((UINT8)   4)
#define STREAMTYPE_VIDEO_14496_2                ((UINT8)  16)
#define STREAMTYPE_VIDEO_DIGICIPHER_2           ((UINT8) 128)
#define STREAMTYPE_AUDIO_ATSC_A53               ((UINT8) 129)
#define STREAMTYPE_VIDEO_H264                   ((UINT8) 27)
#define STREAMTYPE_AUDIO_HE_ACC                 ((UINT8) 17)

#define STREAMTYPE_PES_PRIV_13818_1             ((UINT8) 6)

/* job flag */
#define FLAG_FIND_REF_DATA                      ((UINT8)  0x1)
#define FLAG_FIND_ASSOCIATION_TAG               ((UINT8)  0x2)
#define FLAG_GET_COMP_TAG                       ((UINT8)  0x4)
#define FLAG_GET_CAROUSEL_ID_DESC               ((UINT8)  0x8)
#define FLAG_GET_NETWORK_BOOT_INFO              ((UINT8) 0x10)
#define FLAG_GET_SERVICE_BOOT_INFO              ((UINT8) 0x20)
#define FLAG_CHECK_AP_TYPE_RPIORITY             ((UINT8) 0x40)

#define HT_MHEG5_SI_QUERY                       ((HANDLE_TYPE_T) (HT_GROUP_MHEG_5 + 1))

#define VALID_DESCRIPTOR_LEN(_min_size)         (z_obj_data_len >= (_min_size))

#define MIN(x, y)                               (((x) < (y)) ? (x) : (y))

#define IS_CURRENT_MULTIPLEX(onid, tsid)        (((onid) == _t_loader.ui2_on_id) && ((tsid) == _t_loader.ui2_ts_id))

#define MHEG5_DEF_COMP_TAG                      ((INT32) -1)

#define MHEG5_UK_ON_ID                          ((UINT16) 0x233a)
#define MHEG5_NZ_ON_ID                          ((UINT16) 0x222a)
#define MHEG5_IRL_ON_ID                         ((UINT16) 0x2174)

#define VALID_DESCRIPTOR(_tag, _min_size) \
((pui1_obj_data[0] == (_tag)) && (z_obj_data_len >= (_min_size)))

#define MHEG5_DBG_APP_SI(x)    \
    do {                        \
        if (_b_dbg_msg_si)     \
        {                       \
            MHEG5_DBG_APP(x);   \
        }                       \
    } while (0)

/*-----------------------------------------------------------------------------
 * variable declarations
 *---------------------------------------------------------------------------*/
static _SI_LOADER_INFO_T    _t_loader;
static HANDLE_T             _h_signal = NULL_HANDLE;
static UINT32               _ui4_eng_code;
static MHEG5_PROFILE_ID_T   _e_profile = MHEG5_PROFILE_NONE;
static UINT32               _ui4_svc_index = 1;
static BOOL                 _b_dbg_msg_si                  = TRUE;

/*-----------------------------------------------------------------------------
 * private methods declarations
 *---------------------------------------------------------------------------*/
static INT32 _add_query(
    ptmMHEG5SiQuery_t       pt_in_query,
    VOID*                   pv_query_ref,
    VOID*                   pv_usr_data,
    SLIST_SI_QUERY_T*       pt_query_lst,
    _STREAM_ANNOTATION_T*   pt_strm_ant,
    BOOL                    b_load_pmt,
    HANDLE_T*               ph_query);

static BOOL _check_persistent_tbl_validity(_SI_LOADER_INFO_T* pt_loader);

static INT32 _check_svc_lst_for_query(
    ptmMHEG5SiQuery_t       pt_in_query,
    SLIST_SVC_INFO_T*       pt_svc_info_lst,
    ptmMHEG5SiQueryResult_t pt_qry_res,
    _STREAM_ANNOTATION_T*   pt_strm_ant,
    BOOL*                   pb_pmt_loading,
    MPEG_2_PID_T*           pt_pcr_pid,
    UINT8*                  pui1_enc_type);

static VOID _check_svc_subscription_by_pat(
    HANDLE_T            h_pat,
    SLIST_SVC_INFO_T*   pt_si_list);

static VOID _check_svc_subscription_by_sdt(
    HANDLE_T            h_sdt,
    SLIST_SVC_INFO_T*   pt_si_list);

static VOID _cm_connect_nfy(
      HANDLE_T  h_conn,
      CM_COND_T e_nfy_cond,
      VOID*     pv_nfy_tag,
      UINT32    ui4_data);

static VOID _cm_open_pipe_nfy(
    HANDLE_T  h_pipe,
    CM_COND_T e_nfy_cond,
    VOID*     pv_nfy_tag,
    UINT32    ui4_data);

static INT32 _connect(
    HANDLE_T    h_pipe,
    UINT16      ui2_svl_id,
    UINT16      ui2_svl_rec_id,
    UINT32      ui4_seq_num,
    HANDLE_T*   ph_conn);

static _SVC_INFO_T* _create_svc_info_entry(
    HANDLE_T    h_pat,
    UINT16      ui2_svc_id,
    UINT32      ui4_seq_num,
    BOOL        b_subscribed);

static PARSE_RET_T _find_specified_data(
    HANDLE_T      h_pmt,
    const UINT8*  pui1_obj_data,
    SIZE_T        z_obj_data_len,
    UINT16        ui2_count,
    VOID*         pv_data,
    SIZE_T        z_data_len);

static _SDT_INFO_T* _find_sdt_info(UINT16 ui2_on_id, UINT16 ui2_ts_id);

static _SVC_INFO_T* _find_svc_info(UINT16 ui2_svc_id);

static VOID _free_loader_tables(_SI_LOADER_INFO_T* pt_loader);

#if 0
static VOID _free_deferred_query(SLIST_SI_QUERY_T* pt_query_lst);
#endif

static VOID _free_sdt_info(_SDT_INFO_T* pt_sdt_info);

static VOID _free_sdt_info_lst(SLIST_SDT_INFO_T* pt_sdt_lst);

static VOID _free_si_table(HANDLE_T h_tbl);

static VOID _free_svc_info(_SVC_INFO_T* pt_svc_info);

static VOID _free_svc_info_lst(SLIST_SVC_INFO_T* pt_si_lst);

static INT32 _get_boot_carousel(
    HANDLE_T        h_pmt,
    MPEG_2_PID_T*   pt_pid,
    UINT8*          pui1_comp_tag,
    UINT16*         pui2_association_tag,
    UINT32*         pui4_carousel_id,
    UINT8*          pui1_carousel_id_desc);

static INT32 _get_carousel(
    HANDLE_T        h_pmt,
    UINT32          ui4_carousel_id,
    MPEG_2_PID_T*   pt_pid,
    UINT8*          pui1_comp_tag,
    UINT8*          pui1_carousel_id_desc
    #ifdef MHEG5_HD_SUPPORT
    ,
    UINT16*         pui2_association_tag
    #endif
    );

static INT32 _get_comp_tag_and_carousel_id_desc(
    HANDLE_T    h_pmt,
    UINT16      ui2_idx,
    UINT8       ui1_flag,
    UINT8*      pui1_comp_tag,
    UINT8*      pui1_carousel_id_desc);

static INT32 _get_conn_from_svctx(
    HANDLE_T    h_svctx,
    UINT32      ui4_seq_num,
    HANDLE_T*   ph_conn);

static INT32 _get_default_stream_pid(
    HANDLE_T                h_pmt,
    _STREAM_ANNOTATION_T*   pt_strm_ant,
    MPEG_2_PID_T*           pt_pid,
    UINT8*                  pui1_enc_type);

static INT32 _get_deferred_svc(
    HANDLE_T    h_pmt,
    UINT16      ui2_association_tag,
    UINT16*     pui2_ts_id,
    UINT16*     pui2_svc_id,
    UINT16*     pui2_on_id);

static VOID _get_iso_639_language(
    HANDLE_T              h_pmt,
    UINT16                ui2_index,
    _STREAM_ANNOTATION_T  *pt_strm_ant,
    _ISO_639_DESC*        pt_desc,
    SIZE_T                z_desc_len);

static VOID _get_audio_description(
    HANDLE_T                h_pmt,
    UINT16                  ui2_index,
    _STREAM_ANNOTATION_T   *pt_strm_ant,
    _COMP_ISO_639_DESC*     pt_desc,
    SIZE_T                  z_desc_len);

static INT32 _get_pid_list(
    HANDLE_T    h_pmt,
    UINT16      ui2_sz_pid_lst,
    UINT16*     pui2_num_pid,
    UINT16*     pui2_pid_lst,
    UINT8*      pui1_comp_tag_lst);

#if 0
static INT32 _get_running_status(
    HANDLE_T    h_sdt,
    UINT16      ui2_svc_id,
    UINT8*      pui1_status);
#endif

static INT32 _get_stream_pid(
    HANDLE_T        h_pmt,
    UINT16          ui2_association_tag,
    MPEG_2_PID_T*   pt_pid,
    UINT8*          pui1_enc_type);

static BOOL _get_svc_indicator(
    _SI_LOADER_INFO_T*  pt_loader,
    UINT16              ui2_svl_id, 
    UINT16              ui2_svl_rec_id,
    UINT16*             pui2_on_id,
    UINT16*             pui2_ts_id,
    UINT16*             pui2_svc_id);

static INT32 _handle_msg_connected(
    MHEG5_MSG_T*        pt_msg,
    _SI_LOADER_INFO_T*  pt_loader);

static INT32 _handle_msg_nit_timeout(
    MHEG5_MSG_T*        pt_msg,
    _SI_LOADER_INFO_T*  pt_loader);

static INT32 _handle_msg_nit_update(
    MHEG5_MSG_T*        pt_msg,
    _SI_LOADER_INFO_T*  pt_loader);

static INT32 _handle_msg_sdt_timeout(
    MHEG5_MSG_T*        pt_msg,
    _SI_LOADER_INFO_T*  pt_loader);

static INT32 _handle_msg_sdt_update(
    MHEG5_MSG_T*        pt_msg,
    _SI_LOADER_INFO_T*  pt_loader);

static INT32 _handle_query(
    ptmMHEG5SiQuery_t       pt_in_query,
    VOID*                   pv_query_ref,
    VOID*                   pv_usr_data,
    ptmMHEG5SiQueryResult_t pt_qry_res,
    _STREAM_ANNOTATION_T*   pt_strm_ant,
    HANDLE_T*               ph_query,
    MPEG_2_PID_T*           pt_pcr_pid,
    UINT8*                  pui1_enc_type);

#if 0
static BOOL _load_nit(
    HANDLE_T    h_root, 
    UINT32      ui4_seq_num,
    HANDLE_T*   ph_nit);
#endif

static BOOL _load_pat(
    HANDLE_T    h_tsl,
    UINT16      ui2_tsl_rec_id,    
    HANDLE_T    h_root, 
    UINT32      ui4_seq_num,
    HANDLE_T*   ph_pat);

static INT32 _load_pmt(
    HANDLE_T    h_pat,
    UINT16      ui2_svc_id,
    UINT32      ui4_seq_num,
    HANDLE_T*   ph_pmt);

static BOOL _load_sdt(
    HANDLE_T    h_root,
    UINT16      ui2_on_id,
    UINT16      ui2_ts_id,
    UINT32      ui4_seq_num,
    HANDLE_T*   ph_sdt);

static VOID _notify_load_status(
    _SI_LOADER_INFO_T*  pt_loader,
    MHEG5_NFY_COND_T    e_cond);

static INT32 _open_pipe(
    const CHAR* ps_tuner_name,
    UINT32      ui4_seq_num,
    HANDLE_T*   ph_pipe);

static BOOL _open_tbl_root(
    const CHAR* ps_tuner_name,
    HANDLE_T*   ph_root);

static PARSE_RET_T _parse_comp_tag_and_carousel_id_desc(
    HANDLE_T      h_pmt,
    const UINT8*  pui1_obj_data,
    SIZE_T        z_obj_data_len,
    UINT16        ui2_count,
    VOID*         pv_data,
    SIZE_T        z_data_len);

static PARSE_RET_T _parse_deferred_association_tag(
    HANDLE_T      h_pmt,
    const UINT8*  pui1_obj_data,
    SIZE_T        z_obj_data_len,
    UINT16        ui2_count,
    VOID*         pv_data,
    SIZE_T        z_data_len);

static PARSE_RET_T _parse_iso_639_language (
    HANDLE_T      h_pmt,
    const UINT8*  pui1_obj_data,
    SIZE_T        z_obj_data_len,
    UINT16        ui2_count,
    VOID*         pv_data,
    SIZE_T        z_data_len);

static PARSE_RET_T _parse_ad_descriptors (
    HANDLE_T      h_pmt,
    const UINT8*  pui1_obj_data,
    SIZE_T        z_obj_data_len,
    UINT16        ui2_count,
    VOID*         pv_data,
    SIZE_T        z_data_len);

static PARSE_RET_T _parse_ac3_aac_streams (
    HANDLE_T      h_pmt,
    const UINT8*  pui1_obj_data,
    SIZE_T        z_obj_data_len,
    UINT16        ui2_count,
    VOID*         pv_data,
    SIZE_T        z_data_len);

#ifndef MHEG5_HD_SUPPORT
static INT32 _parse_network_boot_info_sub_desc(
    HANDLE_T    h_pmt,
    UINT32      ui4_max_len,
    UINT8*      pui1_boot_info,
    UINT32*     pui4_actual_len,
    UINT8*      pui1_nb_version,
    UINT8*      pui1_nb_action);
#endif
static BOOL _parse_query(
    HANDLE_T                h_pmt,
    ptmMHEG5SiQuery_t       pt_in_query,
    ptmMHEG5SiQueryResult_t pt_qry_res,
    _STREAM_ANNOTATION_T*   pt_strm_ant,
    UINT8*                  pui1_enc_type);

static VOID _prepare_connection(
    _SI_LOADER_INFO_T*  pt_loader,
    UINT16              ui2_on_id,
    UINT16              ui2_ts_id,
    UINT16              ui2_svc_id);

static VOID _reply_deferred_query(
    HANDLE_T            h_pmt,
    UINT16              ui2_svc_id,
    SLIST_SI_QUERY_T*   pt_query_lst,
    SLIST_SI_QUERY_T*   pt_close_lst);

static VOID _reject_deferred_query(
    UINT16              ui2_svc_id,
    SLIST_SI_QUERY_T*   pt_query_lst,
    SLIST_SI_QUERY_T*   pt_close_lst);

static BOOL _release_svc_info_resource(
    UINT16              ui2_svc_id,
    SLIST_SVC_INFO_T*   pt_si_lst);

static INT32 _search_descriptors_with_data(
    HANDLE_T        h_pmt,
    UINT8           ui1_tag,
    _PARSE_PARAM_T* pt_parse,
    UINT16*         pui2_idx,
    MPEG_2_PID_T*   pt_pid,
    UINT8*          pui1_enc_type);

static INT32 _send_notify(
    UINT16      ui2_msg_type,
    UINT32      ui4_seq_num,
    HANDLE_T    h_obj,
    UINT16      ui2_on_id,
    UINT16      ui2_ts_id,
    UINT16      ui2_svc_id,
    VOID*       pv_nfy_tag);

static BOOL _si_query_free_cb(
    HANDLE_T       h_handle,
    HANDLE_TYPE_T  e_type,
    VOID*          pv_obj,
    VOID*          pv_tag,
    BOOL           b_req_handle);

static INT32 _start_timer(
    UINT16      ui2_msg_type,
    UINT32      ui4_seq_num,
    HANDLE_T    h_table,
    UINT16      ui2_on_id,
    UINT16      ui2_ts_id,
    UINT16      ui2_svc_id,
    UINT32      ui4_delay);

#if 0
static NFY_RET_T _tm_load_nit_nfy(
    HANDLE_T       h_nit,
    HANDLE_TYPE_T  e_obj_type,
    TM_COND_T      e_nfy_cond,
    VOID*          pv_nfy_tag,
    UINT32         ui4_data);
#endif

static NFY_RET_T _tm_load_pat_nfy(
    HANDLE_T       h_pat,
    HANDLE_TYPE_T  e_obj_type,
    TM_COND_T      e_nfy_cond,
    VOID*          pv_nfy_tag,
    UINT32         ui4_data);

static NFY_RET_T _tm_load_pmt_nfy(
    HANDLE_T       h_pmt,
    HANDLE_TYPE_T  e_obj_type,
    TM_COND_T      e_nfy_cond,
    VOID*          pv_nfy_tag,
    UINT32         ui4_data);

static NFY_RET_T _tm_load_sdt_nfy(
    HANDLE_T       h_sdt,
    HANDLE_TYPE_T  e_obj_type,
    TM_COND_T      e_nfy_cond,
    VOID*          pv_nfy_tag,
    UINT32         ui4_data);

static VOID _tm_timer_cb(
    HANDLE_T    h_timer,
    VOID*       pv_tag);

static VOID _unload(_SI_LOADER_INFO_T* pt_loader);

static _SVC_INDEX_T* _find_svc_index(UINT32 ui4_ch_id);

static _SVC_INDEX_T* _new_svc_index(UINT32 ui4_ch_id);

static VOID _deinit_svc_index_lst(VOID);

static VID_ENC_T _streamtype_to_vid_enc(
    UINT8  ui1_stream_type);

#ifdef MHEG5_HD_SUPPORT
static PARSE_RET_T _find_specified_data_ex(
    HANDLE_T      h_pmt,
    const UINT8*  pui1_obj_data,
    SIZE_T        z_obj_data_len,
    UINT16        ui2_count,
    VOID*         pv_data,
    SIZE_T        z_data_len);


static INT32 _search_descriptors_with_data_ex(
    HANDLE_T        h_pmt,
    UINT8           ui1_tag,
    _PARSE_PARAM_T* pt_parse,
    UINT16*         pui2_idx,
    MPEG_2_PID_T*   pt_pid,
    UINT8*          pui1_enc_type,
    MPEG_2_PID_T    t_pid);


static INT32 _parse_network_boot_info_sub_desc_ex(
    HANDLE_T    h_pmt,
    UINT32      ui4_max_len,
    UINT8*      pui1_boot_info,
    UINT32*     pui4_actual_len,
    UINT8*      pui1_nb_version,
    UINT8*      pui1_nb_action,
    MPEG_2_PID_T t_pid);


static INT32 _parse_service_boot_info_ex(
    HANDLE_T        h_pmt,
    MPEG_2_PID_T*   pt_pid,
    UINT16*         pui2_association_tag,
    MPEG_2_PID_T    t_pid);

#endif


#ifdef MW_EDB_SUPPORT

#else
static INT32 _get_event_info(
    HANDLE_T    h_evt,
    EVCTX_KEY_TYPE_T data_type, 
    VOID*     s_evt_lang_info,
    VOID**     data, 
    UINT32    *data_len );
#endif

static INT32  _parse_event_mjd_data_time(
    TIME_T *pui8_utg,
    U32BIT *pui4_mjd_data,
    S_Time *pui4_mjd_time);
    


/*-----------------------------------------------------------------------------
 * public methods implementations
 *---------------------------------------------------------------------------*/
INT32 sil_get_current_service(ptmMHEG5DvbLocator_t pt_locator)
{
    /* TODO: Retrieve the curent service. If the current service was tuned quietly, 
           the user-contex service should be retrieved. */
    LOCK(&_t_loader);
    
    pt_locator->original_network_id = _t_loader.ui2_on_id;
    pt_locator->transport_stream_id = _t_loader.ui2_ts_id;
    pt_locator->service_id          = _t_loader.ui2_active_svc_id;
    pt_locator->ComponentTag        = -1;

    UNLOCK(&_t_loader);

    return MHEG5R_OK;
}

INT32 sil_get_basic_si(
    UINT32              ui4_svc_index,
    tmMHEG5BasicSI_t*   pt_basic_si)
{
    SVL_REC_T   t_svl_rec;
    TSL_REC_T   t_tsl_rec;
    UINT32      ui4_ver_id;
    INT32       i4_ret;    
    UINT32      ui4_channel_id;

    LOCK(&_t_loader);

    i4_ret = MHEG5R_INTERNAL_ERROR;
    
    ui4_ver_id = SVL_NULL_VER_ID;

    if (MHEG5R_OK == sil_svc_index_to_ch_id(ui4_svc_index, &ui4_channel_id))
    {        
        if (SVLR_OK == x_svl_get_rec_by_channel(
            _t_loader.h_svl, 
            ui4_channel_id, 
            0xffffffff, 
            0, 
            &t_svl_rec, 
            &ui4_ver_id))
        {
            if (TSLR_OK == x_tsl_get_rec(
                _t_loader.h_tsl, 
                t_svl_rec.uheader.t_rec_hdr.ui2_tsl_rec_id, 
                &t_tsl_rec, 
                &ui4_ver_id))
            {
                pt_basic_si->original_network_id = t_tsl_rec.uheader.t_desc.ui2_on_id;
                pt_basic_si->network_id          = t_tsl_rec.uheader.t_desc.ui2_nw_id;
                pt_basic_si->transport_stream_id = t_tsl_rec.uheader.t_desc.ui2_ts_id;
                pt_basic_si->service_id          = t_svl_rec.uheader.t_rec_hdr.ui2_prog_id;

                i4_ret = MHEG5R_OK;
            }
        }
    }

    UNLOCK(&_t_loader);

    return (i4_ret);
}

INT32 sil_get_boot_info(
    UINT32  ui4_max_len,
    UINT8*  pui1_boot_info,
    UINT32* pui4_actual_len)
{
    INT32 i4_ret;
    
    LOCK(&_t_loader);
    
#ifndef MHEG5_HD_SUPPORT
    i4_ret = _parse_network_boot_info_sub_desc(
        _t_loader.h_pmt_active,
        ui4_max_len,
        pui1_boot_info,
        pui4_actual_len,
        NULL,
        NULL);
#else
{
    MPEG_2_PID_T t_find_in_pid;

    if (_t_loader.t_si_carousel_pid != MPEG_2_NULL_PID)
    {
        t_find_in_pid = _t_loader.t_si_carousel_pid;
    }
    else
    {
        t_find_in_pid = _t_loader.t_boot_pid;
    }
    i4_ret = _parse_network_boot_info_sub_desc_ex(
        _t_loader.h_pmt_active,
        ui4_max_len,
        pui1_boot_info,
        pui4_actual_len,
        NULL,
        NULL,
        t_find_in_pid);
}
#endif

    UNLOCK(&_t_loader);
    
    return (i4_ret);
}

INT32 sil_get_available_svc_index(
    UINT16  ui2_on_id,
    UINT16  ui2_ts_id,
    UINT16  ui2_svc_id,
    UINT32* pui4_svc_index)
{
    INT32                       i4_ret;
    INT32                       i;
    SVL_REC_T                   t_svl_rec;
    TSL_REC_T                   t_tsl_rec;
    UINT32                      ui4_ver_id;
    UINT32                      ui4_channel_id = 0;
    UINT16                      ui2_idx;
    BOOL                        b_first_found = FALSE;
    
    LOCK(&_t_loader);

    i4_ret = MHEG5R_SVC_NOT_FOUND;

    if (SVLR_OK == x_svl_lock(_t_loader.h_svl))
    {
        ui4_ver_id = SVL_NULL_VER_ID;
        
        if (SVLR_OK == x_svl_get_num_rec_by_brdcst_type(
            _t_loader.h_svl,
            MHEG5_SIL_DEFAULT_BRDCST_TYPE/*BRDCST_TYPE_DVB*/,
            0xffffffff,
            &ui2_idx,
            &ui4_ver_id))
        {
            for (i = 0; i < (INT32) ui2_idx; i++)
            {
                ui4_ver_id = SVL_NULL_VER_ID;
        
                if (SVLR_OK == x_svl_get_rec_by_brdcst_type(
                    _t_loader.h_svl,
                    MHEG5_SIL_DEFAULT_BRDCST_TYPE/*BRDCST_TYPE_DVB*/,
                    0xffffffff,
                    (UINT16) i,
                    &t_svl_rec,
                    &ui4_ver_id))
                {
                    if (ui2_svc_id == t_svl_rec.uheader.t_rec_hdr.ui2_prog_id)
                    {
                        ui4_ver_id = SVL_NULL_VER_ID;
                        
                        if (TSLR_OK == x_tsl_get_rec(
                            _t_loader.h_tsl, 
                            t_svl_rec.uheader.t_rec_hdr.ui2_tsl_rec_id, 
                            &t_tsl_rec, 
                            &ui4_ver_id))
                        {
                            if ((ui2_on_id == t_tsl_rec.uheader.t_desc.ui2_on_id) &&
                                ((0 == ui2_ts_id) || (ui2_ts_id == t_tsl_rec.uheader.t_desc.ui2_ts_id)))
                            {
                                if (IS_CURRENT_MULTIPLEX(
                                    t_tsl_rec.uheader.t_desc.ui2_on_id,
                                    t_tsl_rec.uheader.t_desc.ui2_ts_id))
                                {
                                    ui4_channel_id = t_svl_rec.uheader.t_rec_hdr.ui4_channel_id;
                                    i4_ret = MHEG5R_OK;
                                    break;
                                }
                                else if (!b_first_found)
                                {
                                    ui4_channel_id = t_svl_rec.uheader.t_rec_hdr.ui4_channel_id;

                                    b_first_found = TRUE;
                                    i4_ret = MHEG5R_OK;
                                }
                            }
                        }
                    }
                }
            }
            
            if (i4_ret == MHEG5R_OK)
            {
                _SVC_INDEX_T* pt_svc_index;

                pt_svc_index = _find_svc_index(ui4_channel_id);
                        
                if (pt_svc_index == NULL)
                {
                    pt_svc_index = _new_svc_index(ui4_channel_id);

                    if (pt_svc_index != NULL)
                    {                
                        *pui4_svc_index = pt_svc_index->ui4_svc_index;
                
                        SLIST_INSERT_HEAD(pt_svc_index, &(SVC_INDEX_LIST(&_t_loader)), t_link);
                    }
                    else
                    {
                        i4_ret = MHEG5R_OUT_OF_MEMORY;
                    }
                }
                else
                {
                    *pui4_svc_index = pt_svc_index->ui4_svc_index;
                }
            }
        }
                
        x_svl_unlock(_t_loader.h_svl);
    }
            
#if 0
        /* look for the service with the specified on_id and svc_id */
        if (MHEG5R_OK == i4_ret)
        {
            UINT8 ui1_running_status;
            
            /* check SDT actual */
            i4_ret = _get_running_status(
                _t_loader.h_sdt_actual,
                ui2_svc_id,
                &ui1_running_status);

            if (MHEG5R_OK != i4_ret)
            {
                _SDT_INFO_T* pt_sdt_info;

                /* check SDT other */
                SLIST_FOR_EACH(pt_sdt_info, &(SDT_OTHER_LIST(&_t_loader)), t_link)
                {
                    i4_ret = _get_running_status(
                        pt_sdt_info->h_sdt,
                        ui2_svc_id,
                        &ui1_running_status);

                    if (MHEG5R_OK == i4_ret)
                    {
                        break;
                    }
                }
            }
            
            if (MHEG5R_OK == i4_ret)
            {
                if ((SVC_FLAG_RUNNING          != ui1_running_status) &&
                    (SVC_FLAG_STATUS_UNDEFINED != ui1_running_status))
                {
                    i4_ret = MHEG5R_SVC_UNAVAILABLE;
                }
            }
        }
#endif

    UNLOCK(&_t_loader);

    return (i4_ret);
}

INT32 sil_get_lcn_locator(
    UINT32  ui4_lcn,
    UINT16* pui2_on_id,
    UINT16* pui2_ts_id,
    UINT16* pui2_svc_id)
{
    INT32       i4_ret;
    INT32       i;
    UINT16      ui2_idx = 0;
    SVL_REC_T   t_svl_rec;
    TSL_REC_T   t_tsl_rec;
    UINT32      ui4_ver_id;
    BOOL        b_first_found = FALSE;
    
    LOCK(&_t_loader);

    i4_ret = MHEG5R_INTERNAL_ERROR;

    if (SVLR_OK == x_svl_lock(_t_loader.h_svl))
    {
        ui4_ver_id = SVL_NULL_VER_ID;
            
        if (SVLR_OK == x_svl_get_num_rec_by_brdcst_type(
            _t_loader.h_svl,
            MHEG5_SIL_DEFAULT_BRDCST_TYPE/*BRDCST_TYPE_DVB*/,
            0xffffffff,
            &ui2_idx,
            &ui4_ver_id))
        {
            for (i = 0; i < (INT32) ui2_idx; i++)
            {
                ui4_ver_id = SVL_NULL_VER_ID;
            
                if (SVLR_OK == x_svl_get_rec_by_brdcst_type(
                    _t_loader.h_svl,
                    MHEG5_SIL_DEFAULT_BRDCST_TYPE/*BRDCST_TYPE_DVB*/,
                    0xffffffff,
                    (UINT16) i,
                    &t_svl_rec,
                    &ui4_ver_id))
                {
                    if (ui4_lcn == (UINT32) ( t_svl_rec.u_data.t_dvb.ui2_lcn) )
                    {
                        if (TSLR_OK == x_tsl_get_rec(
                            _t_loader.h_tsl, 
                            t_svl_rec.uheader.t_rec_hdr.ui2_tsl_rec_id, 
                            &t_tsl_rec, 
                            &ui4_ver_id))
                        {
                            if (IS_CURRENT_MULTIPLEX(
                                t_tsl_rec.uheader.t_desc.ui2_on_id,
                                t_tsl_rec.uheader.t_desc.ui2_ts_id))
                            {
                                *pui2_on_id  = t_tsl_rec.uheader.t_desc.ui2_on_id;
                                *pui2_ts_id  = t_tsl_rec.uheader.t_desc.ui2_ts_id;
                                *pui2_svc_id = t_svl_rec.uheader.t_rec_hdr.ui2_prog_id;

                                i4_ret = MHEG5R_OK;
                                break;
                            }
                            else if (!b_first_found)
                            {
                                *pui2_on_id  = t_tsl_rec.uheader.t_desc.ui2_on_id;
                                *pui2_ts_id  = t_tsl_rec.uheader.t_desc.ui2_ts_id;
                                *pui2_svc_id = t_svl_rec.uheader.t_rec_hdr.ui2_prog_id;

                                b_first_found = TRUE;
                                i4_ret = MHEG5R_OK;
                            }
                        }
                    }
                }
            }
        }
        
        x_svl_unlock(_t_loader.h_svl);
    }
    
    UNLOCK(&_t_loader);

    return (i4_ret);
}

INT32 sil_init(
    UINT32      ui4_eng_code,
    const CHAR* ps_tuner_name)
{
    INT32       i4_ret;
    
    i4_ret = MHEG5R_INIT_FAIL;

    _ui4_eng_code = ui4_eng_code;
    _h_signal     = NULL_HANDLE;

    _t_loader.h_lock            = NULL_HANDLE;
    _t_loader.e_state           = LDR_ST_UNINIT;
    _t_loader.ui4_seq_num       = 0;
    _t_loader.ui4_tbl_status    = 0;
    _t_loader.ui2_on_id         = 0;
    _t_loader.ui2_ts_id         = 0;

    _t_loader.b_nb_info_present = FALSE;
    _t_loader.ui1_nb_version    = 0;

    _t_loader.h_msg_q           = NULL_HANDLE;
    
    _t_loader.h_pipe            = NULL_HANDLE;
    _t_loader.h_conn            = NULL_HANDLE;
    _t_loader.h_root            = NULL_HANDLE;
    _t_loader.h_nit             = NULL_HANDLE;
    _t_loader.h_sdt_actual      = NULL_HANDLE;
    _t_loader.h_pat             = NULL_HANDLE;
    _t_loader.h_pmt_active      = NULL_HANDLE;

    #ifdef MHEG5_HD_SUPPORT
    _t_loader.t_si_carousel_pid = MPEG_2_NULL_PID;
    #endif
    
    SLIST_INIT(&(SVC_INFO_LIST(&_t_loader)));
    SLIST_INIT(&(SDT_OTHER_LIST(&_t_loader)));
    SLIST_INIT(&(SI_QUERY_LIST(&_t_loader)));
    SLIST_INIT(&(SI_CLOSE_LIST(&_t_loader)));
    SLIST_INIT(&(SVC_INDEX_LIST(&_t_loader)));

    if (OSR_OK == x_sema_create(
        &(_h_signal), 
        X_SEMA_TYPE_BINARY, 
        X_SEMA_STATE_UNLOCK))
    {
        if (OSR_OK == x_sema_create(
            &(_t_loader.h_lock), 
            X_SEMA_TYPE_MUTEX, 
            X_SEMA_STATE_UNLOCK))
        {
            if (OSR_OK == x_msg_q_create(
                &(_t_loader.h_msg_q), 
                MHEG5_SIL_MSG_Q_NAME,
                sizeof(MHEG5_MSG_T), 
                MHEG5_SIL_NUM_OF_MSGS))
            {
                if (_open_tbl_root(ps_tuner_name, &(_t_loader.h_root)))
                {
                    i4_ret = _open_pipe(
                        ps_tuner_name, 
                        _t_loader.ui4_seq_num,
                        &(_t_loader.h_pipe));
                
                    if (SIL_SUCCEEDED == i4_ret)
                    {
                        _t_loader.e_state = LDR_ST_OPENED;
                        i4_ret = MHEG5R_OK;
                    }
                    else if (SIL_ASYNC_NOTIFY == i4_ret)
                    {
                        /* state is already set */
                        if (ST_OPENED(&_t_loader))
                        {
                            i4_ret = MHEG5R_OK;
                        }
                        else
                        {
                            i4_ret = MHEG5R_INIT_FAIL;
                        }
                    }
                    else
                    {
                        _t_loader.e_state = LDR_ST_UNINIT;
                        i4_ret = MHEG5R_INIT_FAIL;
                    }
                }
            }
        }
    }
    
    if (MHEG5R_OK != i4_ret)
    {
        if (NULL_HANDLE != _t_loader.h_root)
        {
            x_tm_free(_t_loader.h_root);
        }

        if (NULL_HANDLE != _t_loader.h_msg_q)
        {
            x_msg_q_delete(_t_loader.h_msg_q);
        }

        if (NULL_HANDLE != _t_loader.h_lock)
        {
            x_sema_delete(_t_loader.h_lock);
        }
        
        if (NULL_HANDLE != _h_signal)
        {
            x_sema_delete(_h_signal);
        }

        _ui4_eng_code = 0;
    }
    
    return (i4_ret);
}

BOOL sil_is_pmt_available(VOID)
{
    return PMT_ACTIVE_DONE(_t_loader.ui4_tbl_status);
}

INT32 sil_load(
    HANDLE_T                    h_svctx,
    UINT16                      ui2_svl_id,
    UINT16                      ui2_svl_rec_id,
    x_mheg5_eng_load_nfy_fct    pf_load_nfy,
    VOID*                       pv_tag)
{
    INT32       i4_ret;
    CM_COND_T   e_cond;
    UINT16      ui2_on_id;
    UINT16      ui2_ts_id;
    UINT16      ui2_svc_id;
    
    LOCK(&_t_loader);
    
    i4_ret = MHEG5R_SVC_NOT_FOUND;
    
    _t_loader.pf_load_nfy = pf_load_nfy;
    _t_loader.pv_load_tag = pv_tag;
    
    if (_get_svc_indicator(
        &_t_loader, 
        ui2_svl_id, 
        ui2_svl_rec_id,
        &ui2_on_id,
        &ui2_ts_id,
        &ui2_svc_id))
    {
        /*if (((_e_profile == MHEG5_PROFILE_UK) || (_e_profile == MHEG5_PROFILE_NZ) || (_e_profile == MHEG5_PROFILE_IRL))&&
            ((ui2_on_id == MHEG5_UK_ON_ID) ||(ui2_on_id == MHEG5_NZ_ON_ID) || (ui2_on_id == MHEG5_IRL_ON_ID)))*/
    #if 0        
        if (ui2_on_id  != _t_loader.ui2_on_id ||
            ui2_ts_id  != _t_loader.ui2_ts_id ||
            ui2_svc_id != _t_loader.ui2_active_svc_id)
    #endif
        {
            if ((NULL_HANDLE != _t_loader.h_conn) &&
                (_t_loader.b_new_conn))
            {
                /* disconnect the previous connection */
                x_cm_disconnect(_t_loader.h_conn, &e_cond);
                _t_loader.h_conn = NULL_HANDLE;
            }
            
            _prepare_connection(
                &_t_loader,
                ui2_on_id,
                ui2_ts_id,
                ui2_svc_id);
            
            if (NULL_HANDLE == h_svctx)
            {
                i4_ret = _connect(
                    _t_loader.h_pipe,
                    ui2_svl_id,
                    ui2_svl_rec_id,
                    _t_loader.ui4_seq_num,
                    &(_t_loader.h_conn));
                
                if (MHEG5R_OK == i4_ret)
                {
                    _t_loader.b_new_conn = TRUE;
                    _t_loader.e_state    = LDR_ST_CONNECTING;
                }
            }
            else
            {
                i4_ret = _get_conn_from_svctx(
                    h_svctx,
                    _t_loader.ui4_seq_num,
                    &(_t_loader.h_conn));
                
                if (MHEG5R_OK == i4_ret)
                {
                    _t_loader.b_new_conn = FALSE;
                    _t_loader.e_state    = LDR_ST_CONNECTING;
                }
            }
        }
    }

    UNLOCK(&_t_loader);
    
    if (!((
        #ifdef SYS_MHEG5_HK_SUPPORT
        (_e_profile == MHEG5_PROFILE_HK) ||
        #endif
        (_e_profile == MHEG5_PROFILE_UK) || (_e_profile == MHEG5_PROFILE_NZ) || (_e_profile == MHEG5_PROFILE_IRL))&&
            ((ui2_on_id == MHEG5_UK_ON_ID) ||(ui2_on_id == MHEG5_NZ_ON_ID) || (ui2_on_id == MHEG5_IRL_ON_ID))))
    {
        i4_ret = MHEG5R_SVC_NOT_FOUND;
    }
    
    return (i4_ret);
}

INT32 sil_proc_msg(MHEG5_MSG_T* pt_msg)
{
    INT32 i4_ret = MHEG5R_INTERNAL_ERROR;

    LOCK(&_t_loader);
    
    if ((SEQ_NUM(&_t_loader) == pt_msg->ui4_seq_num) ||
        (SIL_MSG_NIT_TIMEOUT == pt_msg->ui2_msg_type) ||
        (SIL_MSG_SDT_TIMEOUT == pt_msg->ui2_msg_type) ||
        (SIL_MSG_PAT_TIMEOUT == pt_msg->ui2_msg_type) ||
        (SIL_MSG_PMT_TIMEOUT == pt_msg->ui2_msg_type))
    {
        switch (pt_msg->ui2_msg_type)
        {
            case SIL_MSG_PIPE_OPENED:
                _t_loader.e_state = LDR_ST_OPENED;
                x_sema_unlock(_h_signal);
                break;
                
            case SIL_MSG_PIPE_CLOSED:
                /* free the attached tag */
                if (pt_msg->pv_nfy_tag)
                {
                    SIL_MFREE(pt_msg->pv_nfy_tag, sizeof(_SIL_NOTIFY_T));
                }

                _t_loader.h_pipe  = NULL_HANDLE;
                _t_loader.e_state = LDR_ST_UNINIT;
                x_sema_unlock(_h_signal);
                break;

            case SIL_MSG_CONNECTED:
                i4_ret = _handle_msg_connected(pt_msg, &_t_loader);
                break;
                
            case SIL_MSG_DISCONNECTED:
                /* free the attached tag */
                if (pt_msg->pv_nfy_tag)
                {
                    SIL_MFREE(pt_msg->pv_nfy_tag, sizeof(_SIL_NOTIFY_T));
                }
                
                _t_loader.e_state = LDR_ST_OPENED;
                break;

            case SIL_MSG_NIT_UPDATE:
                i4_ret = _handle_msg_nit_update(pt_msg, &_t_loader);
                break;
                
            case SIL_MSG_NIT_UNAVAIL:
                /* free the attached tag */
                if (pt_msg->pv_nfy_tag)
                {
                    SIL_MFREE(pt_msg->pv_nfy_tag, sizeof(_SIL_NOTIFY_T));
                }
                break;
                
            case SIL_MSG_NIT_TIMEOUT:
                i4_ret = _handle_msg_nit_timeout(pt_msg, &_t_loader);
                break;
                
            case SIL_MSG_SDT_UPDATE:
                i4_ret = _handle_msg_sdt_update(pt_msg, &_t_loader);
                break;

            case SIL_MSG_SDT_UNAVAIL:
                /* free the attached tag */
                if (pt_msg->pv_nfy_tag)
                {
                    SIL_MFREE(pt_msg->pv_nfy_tag, sizeof(_SIL_NOTIFY_T));
                }
                break;
                
            case SIL_MSG_SDT_TIMEOUT:
                i4_ret = _handle_msg_sdt_timeout(pt_msg, &_t_loader);
                break;
                
            case SIL_MSG_PAT_UPDATE:
                if (pt_msg->ui4_seq_num == _t_loader.ui4_seq_num)
                {
                    if (PAT_DONE(_t_loader.ui4_tbl_status))
                    {
                        MHEG5_DBG_ERROR((
                            "{MHEG5 Stack} PAT update (change notify) for service %d arrived.\r\n", 
                            _t_loader.ui2_active_svc_id));
                    
                        /* handle PAT update */
                        _check_svc_subscription_by_pat(
                            pt_msg->h_obj,
                            &(SVC_INFO_LIST(&_t_loader)));
                    }
                    else
                    {
                        /* handle PAT available (first notify) */
                        _SVC_INFO_T* pt_svc_info;
                        
                        MHEG5_DBG_ERROR((
                            "{MHEG5 Stack} PAT update (first notify) for service %d arrived.\r\n", 
                            _t_loader.ui2_active_svc_id));
                        
                        _t_loader.ui4_tbl_status |= BIT_PAT;

                        pt_svc_info = _create_svc_info_entry(
                            pt_msg->h_obj,
                            _t_loader.ui2_active_svc_id,
                            pt_msg->ui4_seq_num,
                            FALSE);

                        if (pt_svc_info)
                        {
                            _t_loader.h_pmt_active    = pt_svc_info->h_pmt;
                            _t_loader.ui4_tbl_status &= ~BIT_PMT_ACTIVE;
                        }
                        else
                        {
                            _t_loader.ui4_tbl_status &= ~BIT_PAT;
                        #if 0
                            /* free PAT */
                            _free_si_table(pt_msg->h_obj);
                            _t_loader.h_pat = NULL_HANDLE;
                            
                            if (PERSISTENT_TBL_DONE(_t_loader.ui4_tbl_status))
                            {
                                _t_loader.e_state = LDR_ST_CONNECTED;
                                _notify_load_status(&_t_loader, MHEG5_NFY_COND_TABLE_FAILED);
                            }
                        #endif
                        }
                    }
                }
                break;

            case SIL_MSG_PAT_UNAVAIL:
                /* free the attached tag */
                if (pt_msg->pv_nfy_tag)
                {
                    SIL_MFREE(pt_msg->pv_nfy_tag, sizeof(_SIL_NOTIFY_T));
                }
                break;
                
            case SIL_MSG_PAT_TIMEOUT:
                /* free the attached tag */
                if (pt_msg->pv_nfy_tag)
                {
                    _SIL_TIMER_T* pt_timer = (_SIL_TIMER_T*) (pt_msg->pv_nfy_tag);
                    
                    x_timer_delete(pt_timer->h_this);
                    SIL_MFREE(pt_timer, sizeof(_SIL_TIMER_T));
                }

            #if 0
                if (pt_msg->ui4_seq_num == _t_loader.ui4_seq_num)
                {
                    if (!PAT_DONE(_t_loader.ui4_tbl_status))
                    {
                        _t_loader.ui4_tbl_status |= BIT_PAT;

                        /* free PAT */
                        _free_si_table(pt_msg->h_obj);
                        _t_loader.h_pat = NULL_HANDLE;
                        
                        if (PERSISTENT_TBL_DONE(_t_loader.ui4_tbl_status))
                        {
                            _t_loader.e_state = LDR_ST_CONNECTED;
                            _notify_load_status(&_t_loader, MHEG5_NFY_COND_TABLE_FAILED);
                        }
                    }
                }
            #endif
                break;
                
            case SIL_MSG_PMT_UPDATE:
                i4_ret = MHEG5R_MISMATCHED_RUN;
                
                if (pt_msg->ui4_seq_num == _t_loader.ui4_seq_num)
                {
                    i4_ret = MHEG5R_INV_HANDLE;
                    
                    if (x_handle_valid(pt_msg->h_obj))
                    {
                        _SVC_INFO_T*    pt_svc_info;
                        BOOL            b_first_notify = FALSE;

                        MHEG5_DBG_APP_SI(("[SIL]Start to handle PMT updated...\n"));
                        
                        pt_svc_info = _find_svc_info(pt_msg->ui2_svc_id);

                        if (pt_svc_info)
                        {
                            if (SVC_INFO_LOADED(pt_svc_info))
                            {
                                MHEG5_DBG_APP_SI((
                                    "[SIL]PMT update (change notify) for service %d arrived.\r\n", 
                                    pt_msg->ui2_svc_id));
                        
                                /* handle PMT update */
                                if (SVC_INFO_SUBSCRIBED(pt_svc_info))
                                {
                                    MHEG5_DBG_APP_SI(("[SIL] Serice info changed, notify the MHEG-5 Stack.\r\n"));
                                    tmMHEG5NotifySiServiceInfoChanged(
                                        pt_msg->ui2_svc_id,
                                        MHEG5_SI_SERVICE_CHANGED);
                                }
                            }
                            else
                            {
                                MHEG5_DBG_APP_SI((
                                    "[SIL] PMT update (first notify) for service %d arrived.\r\n", 
                                    pt_msg->ui2_svc_id));
                                    
                                b_first_notify = TRUE;
                                
                                /* handle PMT available (first notify) */
#ifndef CI_PLUS_SUPPORT
                                if (PMT_ACTIVE_DONE(_t_loader.ui4_tbl_status))
#endif                          
                                {
                                /* Commented by Chun Zhou
                                    Here could be a problem for MHEG5 CI+. 
                                    For normal broadcasted MHEG5 applications, they will only be started after PMT is loaded. So there are
                                    none deferred queries on the current active service. But there could be deferred queries on other services,
                                    when the corresponding PMT is loaded, it should enter here.
                                    But for the MHEG5 CI+ applications, they are not binding on a special service. It is possible that the application
                                    started some queries before the PMT of current active service is loaded. In such a case, it won't be here even
                                    PMT is loaded later, so the deferred queries will never be replied.
                                    So here we should not check the PMT_ACTIVE_DONE(_t_loader.ui4_tbl_status), but always try to reply the
                                    deferred queries.
                                */


                                    _reply_deferred_query(
                                        pt_svc_info->h_pmt,
                                        pt_svc_info->ui2_svc_id,
                                        &(SI_QUERY_LIST(&_t_loader)),
                                        &(SI_CLOSE_LIST(&_t_loader)));
                                }
#ifndef CI_PLUS_SUPPORT
                                else
#else
                                if (!PMT_ACTIVE_DONE(_t_loader.ui4_tbl_status))
#endif                                    
                                {
                                    _t_loader.ui4_tbl_status |= BIT_PMT_ACTIVE;
                                    _t_loader.b_nb_info_present = FALSE;
                                    _t_loader.ui1_nb_version    = 0;

                                    if (LDR_ST_LOADING_PERSISTENT_TBL == _t_loader.e_state)
                                    {
                                        if (PERSISTENT_TBL_DONE(_t_loader.ui4_tbl_status))
                                        {
                                            BOOL b_ok = _check_persistent_tbl_validity(&_t_loader);

                                            _t_loader.e_state = (b_ok) ? LDR_ST_PERSISTENT_TBL_LOADED : LDR_ST_CONNECTED;
                                            _notify_load_status(
                                                &_t_loader, 
                                                (b_ok ? MHEG5_NFY_COND_TABLE_LOADED : MHEG5_NFY_COND_TABLE_FAILED));
                                        }
                                    }
                                }
                                
                                pt_svc_info->ui1_status |= SVC_INFO_STATUS_LOADED;
                            }

                            if (pt_msg->ui2_svc_id == _t_loader.ui2_active_svc_id)                       
                            {
                                /* Test data_broadcast_id descriptor & carousel_id */
                                MPEG_2_PID_T    t_pid;
                                UINT16          ui2_association_tag;
                                UINT32          ui4_carousel_id;
                                UINT8           aui1_carousel_id_desc[257];
                                UINT8           ui1_comp_tag;

                                MHEG5_DBG_APP_SI((
                                    "[SIL] service %d is current active service. Try to get boot carousel again...\r\n", 
                                    pt_msg->ui2_svc_id));
                                
                                if (MHEG5R_OK == _get_boot_carousel(
                                    pt_msg->h_obj,
                                    &t_pid,
                                    &ui1_comp_tag,
                                    &ui2_association_tag,
                                    &ui4_carousel_id,
                                    aui1_carousel_id_desc))
                                {
                                    MHEG5_DBG_APP_SI((
                                    "[SIL] Result: PID:%d, CompTag:%d, AssociationTag:%d, CarouselID:%d...\r\n", t_pid, ui1_comp_tag,ui2_association_tag,ui4_carousel_id));
                                    if (b_first_notify)
                                    {
                                        MHEG5_DBG_APP_SI(("First notify, replace the boot_comp_tag, boot_pid, boot_carousel_id in SI loader\n "));
                                        _t_loader.b_data_broadcast_id_desc_found = TRUE;
                                        _t_loader.ui1_boot_comp_tag    = ui1_comp_tag;
                                        _t_loader.t_boot_pid           = t_pid;
                                        _t_loader.ui4_boot_carousel_id = ui4_carousel_id;

                                        _notify_load_status(
                                            &_t_loader, MHEG5_NFY_COND_DATA_BROADCASD_ID_DESC_RETURNS);
                                        
                                        #ifdef MHEG5_HD_SUPPORT
                                            _t_loader.t_si_carousel_pid = MPEG_2_NULL_PID;
                                        #endif
                                        
                                        break;
                                    }
                                    else
                                    {
                                        if (FALSE == _t_loader.b_data_broadcast_id_desc_found)
                                        {
                                            MHEG5_DBG_APP_SI(("Not the first notify, replace the boot_comp_tag, boot_pid, boot_carousel_id in SI loader, NFY AP with BROADCASD_ID_DESC_RETURNS\n "));
                                            _t_loader.b_data_broadcast_id_desc_found = TRUE;
                                            _t_loader.ui1_boot_comp_tag    = ui1_comp_tag;
                                            _t_loader.t_boot_pid           = t_pid;
                                            _t_loader.ui4_boot_carousel_id = ui4_carousel_id;
                                            
                                            _notify_load_status(
                                                &_t_loader, MHEG5_NFY_COND_DATA_BROADCASD_ID_DESC_RETURNS);
                                            #ifdef MHEG5_HD_SUPPORT
                                            _t_loader.t_si_carousel_pid = MPEG_2_NULL_PID;
                                            #endif
                                            break;
                                        }
                                        else
                                        {
                                            BOOL b_changed = (_t_loader.ui4_boot_carousel_id != ui4_carousel_id);
                                            
                                            _t_loader.ui1_boot_comp_tag    = ui1_comp_tag;
                                            _t_loader.t_boot_pid           = t_pid;
                                            _t_loader.ui4_boot_carousel_id = ui4_carousel_id;
                                            
                                            MHEG5_DBG_APP_SI(("Not the first notify, replace the boot_comp_tag, boot_pid, boot_carousel_id in SI loader, casousel ID %s\n ", b_changed?" changed":" not changed"));

                                            if (b_changed)
                                            {
                                                _notify_load_status(
                                                    &_t_loader, MHEG5_NFY_COND_CAROUSEL_ID_CHANGED);
                                                #ifdef MHEG5_HD_SUPPORT
                                                _t_loader.t_si_carousel_pid = MPEG_2_NULL_PID;
                                                #endif
                                                break;
                                            }
                                        }
                                    }
                                }
                                else
                                {
                                    if (b_first_notify)
                                    {
                                        MHEG5_DBG_APP_SI(("[SIL] Result: Failed, first notify, data_broadcast_id_desc not found\r\n"));
                                        _t_loader.b_data_broadcast_id_desc_found = FALSE;
                                    }
                                    else
                                    {
                                        if (TRUE == _t_loader.b_data_broadcast_id_desc_found)
                                        {
                                            MHEG5_DBG_APP_SI(("[SIL] Result: Failed, not first notify, data_broadcast_id_desc gone\r\n"));

                                            _t_loader.b_data_broadcast_id_desc_found = FALSE;
                                            _notify_load_status(
                                                &_t_loader, MHEG5_NFY_COND_DATA_BROADCASD_ID_DESC_GONE);
                                            #ifdef MHEG5_HD_SUPPORT
                                            _t_loader.t_si_carousel_pid = MPEG_2_NULL_PID;
                                            #endif
                                            break;
                                        }
                                        break;
                                    }
                                }
                                
                                if (!b_first_notify)
                                {
                                    /* Test network_boot_info sub-descriptor */
                                    BOOL    b_present;
                                    UINT8   ui1_nb_version;
                                    UINT8   ui1_nb_action;

                                    MHEG5_DBG_APP_SI(("[SIL]Not the first notify, parse network boot info sub desc "));
                                    #ifndef MHEG5_HD_SUPPORT
                                    if (MHEG5R_OK == _parse_network_boot_info_sub_desc(
                                        pt_msg->h_obj, 
                                        0, 
                                        NULL, 
                                        NULL,
                                        &ui1_nb_version,
                                        &ui1_nb_action))
                                    #else
                                    MPEG_2_PID_T t_find_in_pid;

                                    if (_t_loader.t_si_carousel_pid != MPEG_2_NULL_PID)
                                    {
                                        t_find_in_pid = _t_loader.t_si_carousel_pid;
                                        MHEG5_DBG_APP_SI(("in carousel PID:%d\n", t_find_in_pid));
                                    }
                                    else
                                    {
                                        t_find_in_pid = _t_loader.t_boot_pid;
                                        MHEG5_DBG_APP_SI(("in boot PID:%d\n", t_find_in_pid));
                                    }
                                    if (MHEG5R_OK == _parse_network_boot_info_sub_desc_ex(
                                        pt_msg->h_obj, 
                                        0, 
                                        NULL, 
                                        NULL,
                                        &ui1_nb_version,
                                        &ui1_nb_action,
                                        t_find_in_pid))
                                    #endif
                                    {
                                        MHEG5_DBG_APP_SI(("[SIL] NBI Result: NB_version:%d, NB_action:%d\n ", ui1_nb_version, ui1_nb_action));

                                        b_present = TRUE;

                                        if ((_t_loader.b_nb_info_present != b_present) ||
                                            (_t_loader.ui1_nb_version    != ui1_nb_version))
                                        {
                                            _t_loader.b_nb_info_present = b_present;
                                            _t_loader.ui1_nb_version    = ui1_nb_version;
                                            
                                            switch (ui1_nb_action)
                                            {
                                                case 0:
                                                    tmMHEG5NotifyNBAction(MHEG5_NETWORK_BOOT_INFO_ACTION_NONE);
                                                    break;

                                                case 1:
                                                    /* tmMHEG5NotifyNBAction(MHEG5_NETWORK_BOOT_INFO_ACTION_CI); */
                                                    _notify_load_status(
                                                        &_t_loader, MHEG5_NFY_COND_NB_ACTION_INITIATE_AUTOBOOT);
                                                    #ifdef MHEG5_HD_SUPPORT
                                                    _t_loader.t_si_carousel_pid = MPEG_2_NULL_PID;
                                                    #endif
                                                    break;

                                                case 2: 
                                                    tmMHEG5NotifyNBAction(MHEG5_NETWORK_BOOT_INFO_ACTION_ALL);
                                                    break;

                                                default:
                                                    break;
                                            }
                                        }
                                    }
                                    else
                                    {
                                        MHEG5_DBG_APP_SI(("[SIL] NBI Result: Not found\n "));
                                        _t_loader.b_nb_info_present = FALSE;
                                    }

                                    _notify_load_status(
                                        &_t_loader, MHEG5_NFY_COND_SERVICE_CHANGED);
                                }
                            }
                        }
                        else
                        {   /* should not arrive here*/
                        #if 0
                            /* free PMT */
                            _free_si_table(pt_msg->h_obj);
                            
                            if (PMT_ACTIVE_DONE(_t_loader.ui4_tbl_status))
                            {
                                _reject_deferred_query(
                                    pt_msg->ui2_svc_id,
                                    &(SI_QUERY_LIST(&_t_loader)),
                                    &(SI_CLOSE_LIST(&_t_loader)));
                            }
                            else
                            {
                                _t_loader.ui4_tbl_status |= BIT_PMT_ACTIVE;
                            }
                        #else
                            MHEG5_ASSERT(0);
                        #endif
                        }
                    }
                }
                break;

            case SIL_MSG_PMT_UNAVAIL:
                /* free the attached tag */
                if (pt_msg->pv_nfy_tag)
                {
                    _SVC_INFO_T*    pt_svc_info = NULL;
                        
                    pt_svc_info = _find_svc_info(pt_msg->ui2_svc_id);

                    if (pt_svc_info)
                    {
                        pt_svc_info->ui1_status  &= ~SVC_INFO_STATUS_LOADED;
                        _t_loader.ui4_tbl_status &= ~BIT_PMT_ACTIVE;
                    }
                    
                    SIL_MFREE(pt_msg->pv_nfy_tag, sizeof(_SIL_NOTIFY_T));
                }
                break;
                
            case SIL_MSG_PMT_TIMEOUT:
                /* free the attached tag */
                if (pt_msg->pv_nfy_tag)
                {
                    _SIL_TIMER_T* pt_timer = (_SIL_TIMER_T*) (pt_msg->pv_nfy_tag);
                    
                    x_timer_delete(pt_timer->h_this);
                    SIL_MFREE(pt_timer, sizeof(_SIL_TIMER_T));
                }

                if (pt_msg->ui4_seq_num == _t_loader.ui4_seq_num)
                {
                    _SVC_INFO_T* pt_svc_info;

                    pt_svc_info = _find_svc_info(pt_msg->ui2_svc_id);

                    if (pt_svc_info)
                    {
                        if (!SVC_INFO_LOADED(pt_svc_info))
                        {
                            if (SVC_INFO_SUBSCRIBED(pt_svc_info))
                            {
                                tmMHEG5NotifySiServiceInfoChanged(
                                    pt_msg->ui2_svc_id,
                                    MHEG5_SI_SERVICE_NOT_FOUND);
                            }

                            _reject_deferred_query(
                                pt_msg->ui2_svc_id,
                                &(SI_QUERY_LIST(&_t_loader)),
                                &(SI_CLOSE_LIST(&_t_loader)));

                            /* free the svc_info entry */
                            _free_svc_info(pt_svc_info);
                        }
                    }
                    
                    if (!PMT_ACTIVE_DONE(_t_loader.ui4_tbl_status))
                    {
                        /* free PMT */
                        _free_si_table(pt_msg->h_obj);
                        _t_loader.ui4_tbl_status |= BIT_PMT_ACTIVE;

                        if (PERSISTENT_TBL_DONE(_t_loader.ui4_tbl_status))
                        {
                            _t_loader.e_state = LDR_ST_CONNECTED;
                            _notify_load_status(&_t_loader, MHEG5_NFY_COND_TABLE_FAILED);
                        }
                    }
                }
                
                break;

            default:
                break;
        }
    }
    
    UNLOCK(&_t_loader);
    
    return (i4_ret);
}

INT32 sil_query(
    ptmMHEG5SiQuery_t       pt_in_query,
    tmMHEG5SiQueryRef_t     t_query_ref,
    VOID*                   pv_usr_data,
    ptmMHEG5SiQueryResult_t pt_qry_res,
    HANDLE_T*               ph_query)
{
    INT32 i4_ret;
    
    LOCK(&_t_loader);

    i4_ret = _handle_query(
        pt_in_query,
        (VOID*) t_query_ref,
        pv_usr_data,
        pt_qry_res,
        NULL,
        ph_query,
        NULL,
        NULL);
    
    UNLOCK(&_t_loader);
    
    return (i4_ret);
}

BOOL sil_is_aud_pid_belong_to_cur_svc(
	UINT16          ui2_on_id,
    UINT16          ui2_ts_id,
    UINT16          ui2_svc_id,
	MPEG_2_PID_T    t_pid)
{
    BOOL b_yes = FALSE;
	
    LOCK(&_t_loader);

	/* Check it's current active service */
    if ((ui2_on_id == _t_loader.ui2_on_id) &&
        (ui2_ts_id == _t_loader.ui2_ts_id) &&
        (ui2_svc_id == _t_loader.ui2_active_svc_id))
    {
		_SVC_INFO_T*	pt_svc_info;
    
	    SLIST_FOR_EACH(pt_svc_info, &(SVC_INFO_LIST(&_t_loader)), t_link)
	    {
	        if (ui2_svc_id == pt_svc_info->ui2_svc_id)
	        {
	            if (SVC_INFO_LOADED(pt_svc_info))
	            {
	            	PSI_STREAM_INFO_T           t_stream_info;
				    TM_COND_T                   e_stream_cond;
				    UINT16                      ui2_idx;
				    
				    ui2_idx = 0;
				    
				    while (1)
				    {
				        if (TMR_OK != x_psi_get_stream_entry(
							pt_svc_info->h_pmt,
				            ui2_idx,
				            &t_stream_info,
				            &e_stream_cond))
				        {
				            break;
				        }

						if (t_stream_info.t_pid == t_pid)
						{
							b_yes = TRUE;
							break;
						}
						ui2_idx++;
				    }
	            }
				break;
	        }
	    }
    }
	
    UNLOCK(&_t_loader);

	return b_yes;
}


INT32 sil_resolve(
    MHEG5_STREAM_DESC_T*    pt_strm_desc,
    x_mheg5_resolve_nfy     pf_resolve_nfy,
    VOID*                   pv_tag,
    STREAM_PID_INFO_T*      pt_pid_info)
{
    INT32 i4_ret;
    
    LOCK(&_t_loader);
    
    i4_ret = MHEG5R_INV_ARG;
    
#if 0
    if ((pt_strm_desc->ui2_on_id == _t_loader.ui2_on_id) &&
        (pt_strm_desc->ui2_ts_id == _t_loader.ui2_ts_id))
#else
    if (pt_strm_desc->ui2_on_id == _t_loader.ui2_on_id)
#endif
    {
        tmMHEG5SiQuery_t        t_query;
        tmMHEG5SiQueryResult_t  t_query_res;
        _STREAM_ANNOTATION_T    t_strm_ant;
        _STREAM_ANNOTATION_T*   pt_strm_ant;
        
        if (MHEG5_DEF_COMP_TAG == pt_strm_desc->i4_comp_tag)
        {
            t_strm_ant.e_strm_type = pt_strm_desc->e_strm_type;
            
            if (ST_AUDIO == pt_strm_desc->e_strm_type)
            {
                x_memcpy(t_strm_ant.s_lang, pt_strm_desc->s_lang, sizeof(ISO_639_LANG_T));
                x_memcpy(t_strm_ant.s_gui_lang, pt_strm_desc->s_gui_lang, sizeof(ISO_639_LANG_T));
                t_strm_ant.e_aud_type = pt_strm_desc->e_aud_type;
            }
            
            pt_strm_ant = &t_strm_ant;
        }
        else
        {
            pt_strm_ant = NULL;
        }
        
        t_query.kind = MHEG5_SI_PID;
        t_query.service_id = (U16BIT) pt_strm_desc->ui2_svc_id;
        t_query.data.association_tag = (U16BIT) (pt_strm_desc->i4_comp_tag & 0xffff);

        i4_ret = _handle_query(
            &t_query, 
            pv_tag, 
            (VOID*) pf_resolve_nfy, 
            &t_query_res, 
            pt_strm_ant,
            NULL,
            &pt_pid_info->t_pcr_pid,
            &pt_pid_info->ui1_enc_type);

        if (MHEG5R_OK == i4_ret)
        {
            pt_pid_info->t_pid = t_query_res.data.PID;
        }
    }
    
    UNLOCK(&_t_loader);
    
    return (i4_ret);
}

INT32 sil_stop_query(
    HANDLE_T            h_qry,
    tmMHEG5SiQueryRef_t queryRef)
{
    HANDLE_TYPE_T   e_type;
    _SI_QUERY_T*    pt_qry;
    INT32           i4_ret = MHEG5R_INV_HANDLE;
    
    LOCK(&_t_loader);
    
    if (HR_OK == handle_get_type_obj(
        h_qry,
        &e_type,
        (VOID**) &pt_qry))
    {
        if ((HT_MHEG5_SI_QUERY == e_type) && (NULL != pt_qry))
        {
            if (queryRef == (tmMHEG5SiQueryRef_t) pt_qry->pv_query_ref)
            {
                SLIST_REMOVE(pt_qry, t_link);
                
                if (SI_QUERY_STATUS_CLOSED != pt_qry->ui1_status)
                {
                    tmMHEG5NotifySiQueryResult(
                        (tmMHEG5SiQueryRef_t) pt_qry->pv_query_ref,
                        pt_qry->pv_usr_data,
                        MHEG5_SI_ABORTED,
                        NULL,
                        (VOID*) h_qry);
                }
                
                x_handle_free(h_qry);
                
                i4_ret = MHEG5R_OK;
            }
            else
            {
                i4_ret = MHEG5R_INV_ARG;
            }
        }
    }

    UNLOCK(&_t_loader);

    return (i4_ret);
}

INT32 sil_subscribe_service(
    UINT16  ui2_svc_id,
    BOOL    b_subscribe)
{
    INT32           i4_ret;
    _SVC_INFO_T*    pt_svc_info;

    LOCK(&_t_loader);

    i4_ret = MHEG5R_OK;
    
    pt_svc_info = _find_svc_info(ui2_svc_id);

    if (pt_svc_info)
    {
        if (b_subscribe)
        {
            pt_svc_info->ui1_status |= SVC_INFO_STATUS_SUBSCRIBED;
        }
        else
        {
            pt_svc_info->ui1_status &= ~SVC_INFO_STATUS_SUBSCRIBED;
        }
    }
    else
    {
        if (NULL == _create_svc_info_entry(
            _t_loader.h_pat, 
            ui2_svc_id, 
            _t_loader.ui4_seq_num, 
            TRUE))
        {
            i4_ret = MHEG5R_LOAD_PMT_FAIL;
        }
    }

    UNLOCK(&_t_loader);
    
    return (i4_ret);
}

INT32 sil_unload(VOID)
{
    LOCK(&_t_loader);

    if (LDR_ST_CLOSED != _t_loader.e_state)
    {
        _unload(&_t_loader);
    }
    _e_profile = MHEG5_PROFILE_NONE;
    
#ifdef MHEG5_HD_SUPPORT
    _t_loader.t_si_carousel_pid = MPEG_2_NULL_PID;
#endif

    _deinit_svc_index_lst();

    UNLOCK(&_t_loader);
    
    return MHEG5R_OK;
}

INT32 sil_set_active_prof(MHEG5_PROFILE_ID_T e_profile_id)
{
    /* _e_profile would be reset to NULL only after sil_unload() */
    if ((_e_profile == MHEG5_PROFILE_NONE) || (_e_profile == e_profile_id))
    {
        _e_profile = e_profile_id;
        return MHEG5R_OK;
    }
    else
    {
        return MHEG5R_INTERNAL_ERROR;
    }
}

INT32 sil_svc_index_to_ch_id(UINT32 ui4_svc_index, UINT32* pui4_ch_id)
{
    _SVC_INDEX_T* pt_svc_index = NULL;

    if (pui4_ch_id == NULL)
    {
        return MHEG5R_INV_ARG;
    }
    
    LOCK(&_t_loader);

    SLIST_FOR_EACH(pt_svc_index, &(SVC_INDEX_LIST(&_t_loader)), t_link)
    {
        if (pt_svc_index->ui4_svc_index == ui4_svc_index)
        {
            break;
        }
    }

    if (pt_svc_index != NULL)
    {
        *pui4_ch_id = pt_svc_index->ui4_channel_id;
       
        UNLOCK(&_t_loader);

        return MHEG5R_OK;
    }
    else
    {
        UNLOCK(&_t_loader);
 
        return MHEG5R_INTERNAL_ERROR;
    }

}


#ifdef MW_EDB_SUPPORT

#else
static VOID _sil_evctx_nfy_fct(
    HANDLE_T          h_event,
    EVCTX_COND_T      e_cond,
    VOID*             pv_nfy_tag,
    UINT32            ui4_data)
{
    MHEG5_EVENT_LOADING_INFO_T* pt_loading_info = (MHEG5_EVENT_LOADING_INFO_T*)pv_nfy_tag;

    x_dbg_stmt("[SIL]%s(h_evctx:%d, e_cond:%d, ui4_data:%lu)\n", __FUNCTION__, h_event, e_cond, ui4_data);
    
    if (pt_loading_info != NULL)
    {
        if (h_event == pt_loading_info->h_evt)
        {
            pt_loading_info->e_cond = e_cond;
            x_sema_unlock(pt_loading_info->h_sema);
        }
    }
}
#endif

static PARSE_RET_T _parse_service_name_type_provider(
    HANDLE_T      h_sdt,
    const UINT8*  pui1_obj_data,
    SIZE_T        z_obj_data_len,
    UINT16        ui2_count,
    VOID*         pv_data,
    SIZE_T        z_data_len)
{
    UINT32 i = 0;
    S_ServiceDetails* pt_svc_details = (S_ServiceDetails*)pv_data;
    
    if (pui1_obj_data != NULL && pui1_obj_data[0] == 0x48 && pt_svc_details != NULL)
    {
        pt_svc_details->dvb_user_data = NULL;
    
        i = 2;
        pt_svc_details->serviceType = (U32BIT)(pui1_obj_data[i++]);
        pt_svc_details->serviceProvider.s_len = (U32BIT)(pui1_obj_data[i++]);
        pt_svc_details->serviceProvider.s_data = x_mem_alloc(pt_svc_details->serviceProvider.s_len);
        if (pt_svc_details->serviceProvider.s_data != NULL)
        {
            x_memcpy(pt_svc_details->serviceProvider.s_data, (VOID*)(pui1_obj_data + i), pt_svc_details->serviceProvider.s_len);
        }
        i += pt_svc_details->serviceProvider.s_len;
        pt_svc_details->serviceName.s_len = (U32BIT)(pui1_obj_data[i++]);
        pt_svc_details->serviceName.s_data = x_mem_alloc(pt_svc_details->serviceName.s_len);
        if (pt_svc_details->serviceName.s_data != NULL)
        {
            x_memcpy(pt_svc_details->serviceName.s_data, (VOID*)(pui1_obj_data + i), pt_svc_details->serviceName.s_len);
        }
    }

    return PARSE_RET_END;
}


tmMHEG5Err_t  sil_get_service_details(
    UINT32 ui4_svc_idx,
    S_ServiceDetails *pt_svc_details)
{
    INT32       i4_ret;    
    UINT32      ui4_channel_id;

    if( pt_svc_details == NULL )
    {   
        return MHEG5_ERR_BAD_PARAMETER;
    }

    LOCK(&_t_loader);

    pt_svc_details->serviceName.s_data = NULL;
    pt_svc_details->serviceProvider.s_data = NULL;
    pt_svc_details->dvb_user_data = NULL;

    i4_ret = MHEG5R_INTERNAL_ERROR;

    if (MHEG5R_OK == sil_svc_index_to_ch_id(ui4_svc_idx, &ui4_channel_id))
    {
        SVL_REC_T   t_svl_rec;
        UINT32      ui4_ver_id;

        ui4_ver_id = SVL_NULL_VER_ID;

        if (SVLR_OK == x_svl_get_rec_by_channel(
            _t_loader.h_svl, 
            ui4_channel_id, 
            0xffffffff, 
            0, 
            &t_svl_rec, 
            &ui4_ver_id))
        {
            TSL_REC_T   t_tsl_rec;

            ui4_ver_id = TSL_NULL_VER_ID;

            if (TSLR_OK == x_tsl_get_rec(
                _t_loader.h_tsl, 
                t_svl_rec.uheader.t_rec_hdr.ui2_tsl_rec_id, 
                &t_tsl_rec, 
                &ui4_ver_id))
            {
                UINT16      ui2_service_id;
                UINT16      ui2_entry_num;
                TM_COND_T   e_cond;
                HANDLE_TYPE_T ui2_sdt_hdl_type;
                VOID * pui4_sdt_obj;

                ui2_service_id = t_svl_rec.uheader.t_rec_hdr.ui2_prog_id;

                handle_get_type_obj(_t_loader.h_sdt_actual, &ui2_sdt_hdl_type,& pui4_sdt_obj);

                x_dbg_stmt("[MHEG5-SIL] INFO: %s#%d: the SDT info: handle: 0x%x, type: %d, obj: %p \r\n", 
                                            __FUNCTION__, __LINE__, _t_loader.h_sdt_actual, ui2_sdt_hdl_type, pui4_sdt_obj );

               i4_ret = x_dvb_si_get_num_svc_entries(
                                    _t_loader.h_sdt_actual,
                                    &ui2_entry_num,
                                    &e_cond);
                
                if (i4_ret == TMR_OK)
                {
                    UINT16 ui2_index;
                    DVB_SI_SVC_ENTRY_INFO_T     t_dvb_si_entry;
                    
                    for (ui2_index = 0; ui2_index < ui2_entry_num; ui2_index++)
                    {
                        if (TMR_OK == x_dvb_si_get_svc_entry(
                            _t_loader.h_sdt_actual, 
                            ui2_index, 
                            &t_dvb_si_entry,
                            &e_cond))
                        {
                            if (ui2_service_id == t_dvb_si_entry.ui2_svc_id)
                            {
                                /* Found the service */
                                TM_FIRST_LOOP_DESCR_SEL_T   t_first_loop_descr_sel;
                                UINT8 ui1_descr_tag;

                                /* Get service type and name */
                                ui1_descr_tag = 0x48;
                                t_first_loop_descr_sel.ui2_loop_idx = ui2_index;
                                t_first_loop_descr_sel.pui1_descr_tag_list = &ui1_descr_tag;
                                t_first_loop_descr_sel.z_num_descr_tags    = 1;

                                if (TMR_OK == x_tm_parse_obj(
                                               _t_loader.h_sdt_actual,
                                               TM_PARSE_TYPE_FIRST_LOOP_DESCRS,
                                               & t_first_loop_descr_sel,
                                               pt_svc_details,
                                               sizeof(S_ServiceDetails),
                                               _parse_service_name_type_provider,
                                               NULL))
                                {
                                    /* sevice info is ready */
                                    i4_ret = MHEG5R_OK;
                                    break;
                                }

                                break;
                            }
                        }
                    }
                }
                else
                {
                    x_dbg_stmt("[MHEG5-SIL] ERR:%s#%d: get sdt num entry failed(ret = %d )\r\n", __FUNCTION__, __LINE__,  i4_ret);
                }
            }
        }
    }
    else
    {
        x_dbg_stmt("[MHEG5-SIL] ERR: %s#%d: sil_svc_index_to_ch_id filed!!!  \r\n",  __FUNCTION__, __LINE__);
    }

    UNLOCK(&_t_loader);

    if (i4_ret != MHEG5R_OK)
    {
        if (pt_svc_details != NULL)
        {
            if (pt_svc_details->serviceName.s_data != NULL)
            {
                x_mem_free(pt_svc_details->serviceName.s_data);
            }
            if (pt_svc_details->serviceProvider.s_data != NULL)
            {
                x_mem_free(pt_svc_details->serviceProvider.s_data);
            }
        }
        
        return MHEG5_ERR_INTERNAL;
    }
    else
    {
        return MHEG5_OK;
    }
}

#ifdef MW_EDB_SUPPORT
#define EDB_MAX_CMD                 (10)
#define EDB_MAX_OPEN_DATA           ( 8)
static HANDLE_T    h_edb_client = NULL_HANDLE;

static INT32 _sil_create_edb_handle(VOID)
{   INT32                i4_ret                                 = 0;
    EDB_COMMAND_T        at_cmd[EDB_MAX_CMD]                    ;
    EDB_ENG_OPEN_DATA_T  at_eng_open_data[EDB_MAX_OPEN_DATA]    ;

    x_memset (at_cmd, 0, sizeof(at_cmd));
    x_memset (at_eng_open_data, 0, sizeof(at_eng_open_data));

    LOCK(&_t_loader);


    at_eng_open_data[0].e_brdcst_type = BRDCST_TYPE_DVB;
    at_eng_open_data[0].ui2_svl_id    = _t_loader.ui2_svl_id;
    //at_eng_open_data[0].ps_tuner_name = SN_TUNER_GRP_0;// SN_TUNER_GRP_0;

    /*Create EDB HANDLE if needed*/
    if (h_edb_client == NULL_HANDLE)
    {
        i4_ret = x_edb_create("EDB_MAIN", at_cmd, NULL, NULL, &h_edb_client);
    }

    UNLOCK(&_t_loader);
    return i4_ret;
}
/*
* Returns : MHEG5_OK on success
*           MHEG5_ERR_BAD_PARAMETER if serviceIndex is invalid
*           MHEG5_ERR_OTHER otherwise.
*/

tmMHEG5Err_t sil_get_event_details(
    UINT32 ui4_svc_idx, 
    S_EventDetails* pt_evt_details,
    BOOL b_porf)
{
    tmMHEG5Err_t            t_err                   = MHEG5_ERR_OTHER;
    INT32                   i4_ret                  = 0;
    EDB_BUCKET_INFO_T       t_bucket_info           ;
    EDB_CH_TIME_FILTER_T    t_ch_time_filter        ;
    UINT16                  ui2_event_id_bucket_num = 2;
    EDB_CH_EVT_ID_T         t_ch_evt_id[2]          ;
    EDB_CH_EVT_ID_T*        pt_ch_evt_id            = NULL;
    UINT32                  ui4_channel_id          = 0 ;
    SIZE_T                  t_size                  = 0;
    EDB_EVENT_INFO_T        t_edb_event_info        = {0};
    S_Time                  t_start_time            = {0};
    U32BIT                  ui4_start_data          = 0;
    SIZE_T                  z_len                   = 0;
    UINT64                  ui8_duation             = 0;

    if (pt_evt_details == NULL)
    {
        x_dbg_stmt("%s#%d: Invalid argument pt_evt_details! \r\n",__FUNCTION__, __LINE__);
        t_err = MHEG5_ERR_BAD_PARAMETER;
        return t_err;
    }

    if (h_edb_client == NULL_HANDLE)
    {
        _sil_create_edb_handle();
    }

    i4_ret = sil_svc_index_to_ch_id(ui4_svc_idx, &ui4_channel_id);
    if ( MHEG5R_OK != i4_ret )
    {
        x_dbg_stmt("%s#%d: change service index to channel id err : %d \r\n",__FUNCTION__, __LINE__, i4_ret);
        t_err = MHEG5_ERR_INTERNAL;
        return t_err;
    }

    LOCK(&_t_loader);
    x_memset (&t_bucket_info, 0, sizeof(EDB_BUCKET_INFO_T));
    x_memset (&t_ch_time_filter, 0, sizeof(EDB_CH_TIME_FILTER_T));    

    /* Key info prepare */
    t_ch_time_filter.e_brdcst_type  = BRDCST_TYPE_DVB;
    t_ch_time_filter.ui2_svl_id     = _t_loader.ui2_svl_id; 
    t_ch_time_filter.ui4_channel_id = ui4_channel_id;
    t_ch_time_filter.t_start_time   = NULL_TIME;//x_dt_get_brdcst_utc(NULL,NULL); 
    t_ch_time_filter.t_end_time     = 0;
    t_ch_time_filter.pf_filter_fct  = NULL;
    t_ch_time_filter.pv_filter_tag  = NULL;
    t_ch_time_filter.pf_nfy_fct     = NULL; 
    t_ch_time_filter.pv_nfy_tag     = NULL;
    UNLOCK(&_t_loader);

    /* Bucket info translation */
    t_bucket_info.pui2_num_events   = &ui2_event_id_bucket_num;

    x_memset(&(t_ch_evt_id[0]), 0, sizeof(t_ch_evt_id));
    t_bucket_info.pui2_ch_event_id  = & (t_ch_evt_id[0]);

    t_size = sizeof(EDB_BUCKET_INFO_T);

    i4_ret = x_edb_get_info (h_edb_client,
                             EDB_KEY_TYPE_NOW_NEXT_EVENT_NUM_BY_UTC,
                             & t_ch_time_filter,
                             & t_size, 
                             & t_bucket_info);


    if (i4_ret != EDBR_OK)
    {
        x_dbg_stmt("%s[%d] EDB get PF num error\r\n",__FILE__,__LINE__);
        return MHEG5_ERR_OTHER;
    }

    //
    t_size = sizeof (t_edb_event_info);

    if (b_porf == (BOOL)0x0)
    {
        pt_ch_evt_id = & (t_ch_evt_id[0]);
        t_ch_evt_id[0].e_brdcst_type  = BRDCST_TYPE_DVB;
        t_ch_evt_id[0].ui2_svl_id     = _t_loader.ui2_svl_id; 
        t_ch_evt_id[0].ui4_channel_id = ui4_channel_id;
    }
    else
    {
        pt_ch_evt_id = & (t_ch_evt_id[1]);
        t_ch_evt_id[1].e_brdcst_type  = BRDCST_TYPE_DVB;
        t_ch_evt_id[1].ui2_svl_id     = _t_loader.ui2_svl_id; 
        t_ch_evt_id[1].ui4_channel_id = ui4_channel_id;
    }

    i4_ret = x_edb_get_info (h_edb_client,
                             EDB_KEY_TYPE_SINGLE_EVENT_BY_ID,
                             pt_ch_evt_id,
                             & t_size, 
                             & t_edb_event_info);
    if (i4_ret < EDBR_OK)
    {
        x_dbg_stmt("%s[%d] EDB get event error b_porf=%d\r\n",__FILE__,__LINE__,b_porf);
        return MHEG5_ERR_OTHER;
    }

    x_memset(pt_evt_details,0x0,sizeof(S_EventDetails));

    /*Event title*/
    pt_evt_details->eventName.s_data = x_mem_alloc(t_edb_event_info.ui1_event_title_len);
    if (pt_evt_details->eventName.s_data != NULL)
    {
        pt_evt_details->eventName.s_len = (U32BIT)t_edb_event_info.ui1_event_title_len -1;
        if (pt_evt_details->eventName.s_len < EDB_MAX_LENGTH_EVENT_TITLE)
        {
            x_memcpy(pt_evt_details->eventName.s_data,t_edb_event_info.ac_event_title,pt_evt_details->eventName.s_len);
        }
        t_err = MHEG5_OK;
    }

    /*Event detail*/
    i4_ret = x_edb_get_info_len (h_edb_client,
                                 EDB_KEY_TYPE_EVENT_DETAIL_BY_ID,
                                 pt_ch_evt_id,
                                 & z_len);   /* will include \0 */
    if (i4_ret < EDBR_OK || z_len == 0)
    {
        /* it's OK if no detail */            
    }
    else
    {
        /* the z_len has already include \0 */
        pt_evt_details->shortDescription.s_data  = x_mem_alloc(z_len+1);
        if(pt_evt_details->shortDescription.s_data == NULL)
        {
            x_dbg_stmt("%s[%d] Memory error\r\n",__FILE__,__LINE__);
            t_err = MHEG5_ERR_OTHER;
            return t_err;
        }
        //pt_evt_details->shortDescription.s_len = z_len;
        x_memset(pt_evt_details->shortDescription.s_data, 0, z_len+1);

        t_size = z_len;
        i4_ret = x_edb_get_info (h_edb_client,
                                 EDB_KEY_TYPE_EVENT_DETAIL_BY_ID,
                                 pt_ch_evt_id,
                                 & t_size,    /* will not include \0 */
                                 pt_evt_details->shortDescription.s_data);
        if (i4_ret < EDBR_OK)
        {
            /* it's OK if no detail */            
        }
        pt_evt_details->shortDescription.s_len = x_strlen((CHAR*)pt_evt_details->shortDescription.s_data);
        t_err = MHEG5_OK;
    }
    /*End event detail*/


    /* rating */
    z_len = 0;
    i4_ret = x_edb_get_info_len (h_edb_client,
                                 EDB_KEY_TYPE_RATING_LIST_BY_ID,
                                 pt_ch_evt_id,
                                 & z_len);
    if (i4_ret < EDBR_OK || z_len == 0)
    {
        /* it's OK if no rating */            
    }
    else
    {
        EDB_RATING_LIST_T* pt_rating = (EDB_RATING_LIST_T*)x_mem_alloc(z_len+1);
        if(pt_rating == NULL)
        {
            t_err = MHEG5_ERR_OTHER;
            return t_err;            
        }

        x_memset(pt_rating, 0, z_len+1);

        t_size = z_len;
        i4_ret = x_edb_get_info (h_edb_client,
                                 EDB_KEY_TYPE_RATING_LIST_BY_ID,
                                 pt_ch_evt_id,
                                 & t_size,
                                 pt_rating);
        if (i4_ret < EDBR_OK)
        {
            /* it's OK if no rating */            
        }
        else
        {
            /* OK */
            pt_evt_details->parentalRating = (U32BIT)(pt_rating->pt_ratings->ui2_rating_value);
            t_err = MHEG5_OK;
        }
        x_mem_free (pt_rating);
    }

    //Date and time
    i4_ret = _parse_event_mjd_data_time(
            (TIME_T*) (&t_edb_event_info.t_start_time),
            &ui4_start_data,
            &t_start_time);
    pt_evt_details->startDate = ui4_start_data;
    pt_evt_details->startTime = t_start_time;


    ui8_duation = t_edb_event_info.t_duration;
    pt_evt_details->duration.hours = (U16BIT)(ui8_duation/3600);
    pt_evt_details->duration.minutes = (U8BIT)((ui8_duation%3600)/60);
    pt_evt_details->duration.seconds = (U8BIT)((ui8_duation%3600)%60);

    //Event category
    pt_evt_details->category.s_data = x_mem_alloc(t_edb_event_info.ui1_num_event_category);
    if (pt_evt_details->category.s_data != NULL)
    {
        pt_evt_details->category.s_len = (U32BIT)t_edb_event_info.ui1_num_event_category;
        x_memcpy(pt_evt_details->category.s_data,t_edb_event_info.aui1_event_category,pt_evt_details->category.s_len);
        t_err = MHEG5_OK;
    }

    //Event free CA mode
    pt_evt_details->freeNotCA = (BOOLEAN)(!t_edb_event_info.b_free_ca_mode); 
    return t_err;
}

#else


tmMHEG5Err_t sil_get_event_details(
    UINT32 ui4_svc_idx, 
    S_EventDetails* pt_evt_details,
    BOOL b_porf)
{
#if 1
    tmMHEG5Err_t t_err;
    INT32        i4_ret;  
    UINT32       ui4_channel_id; 
    UINT32       ui4_ver_id;
    SVL_REC_T    t_svl_rec;   
    TSL_REC_T    t_tsl_rec;
    MHEG5_EVENT_LOADING_INFO_T t_loading_info;

    if (pt_evt_details == NULL)
    {
        x_dbg_stmt("%s#%d: the parameter details is NULL\r\n",__FUNCTION__, __LINE__);
        return MHEG5_ERR_BAD_PARAMETER;
    }
    
    pt_evt_details->dvb_user_data = NULL;
    pt_evt_details->eventName.s_data = NULL;
    pt_evt_details->shortDescription.s_data = NULL;
    pt_evt_details->category.s_data = NULL;

    t_loading_info.h_sema = NULL_HANDLE;
    t_loading_info.e_cond = EVCTX_COND_UNAVAIL;
    t_loading_info.h_evt  = NULL_HANDLE;
    
    t_err = MHEG5_OK;

    LOCK(&_t_loader);

    do
    {
        EVCTX_COMMAND_T at_cmd[6];
        EVCTX_COND_T    e_cond = EVCTX_COND_UNAVAIL;

        i4_ret = sil_svc_index_to_ch_id(ui4_svc_idx, &ui4_channel_id);
        if ( MHEG5R_OK != i4_ret )
        {
            x_dbg_stmt("%s#%d: change sevic index to channel id err : %d \r\n",__FUNCTION__, __LINE__, i4_ret);
            t_err = MHEG5_ERR_INTERNAL;
            break;
        }

        ui4_ver_id = SVL_NULL_VER_ID;

        i4_ret = x_svl_get_rec_by_channel(
                    _t_loader.h_svl, 
                    ui4_channel_id, 
                    0xffffffff, 
                    0, 
                    &t_svl_rec, 
                    &ui4_ver_id);
        if ( SVLR_OK != i4_ret )
        {
            x_dbg_stmt("%s#%d: get svl record by channel err : %d \r\n",__FUNCTION__, __LINE__, i4_ret);
            t_err = MHEG5_ERR_INTERNAL;
            break;
        }

        ui4_ver_id = TSL_NULL_VER_ID;  

        i4_ret = x_tsl_get_rec(
                        _t_loader.h_tsl, 
                        t_svl_rec.uheader.t_rec_hdr.ui2_tsl_rec_id, 
                        &t_tsl_rec, 
                        &ui4_ver_id);
        if( TSLR_OK != i4_ret )
        {
            x_dbg_stmt("%s#%d: get tsl record  err : %d \r\n",__FUNCTION__, __LINE__, i4_ret);
            t_err = MHEG5_ERR_INTERNAL;
            break;
        }

        i4_ret = x_sema_create(&t_loading_info.h_sema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK);
        if ( OSR_OK != i4_ret )
        {
            x_dbg_stmt("%s#%d:create lock semaphore fail:%d\r\n", __FUNCTION__, __LINE__, i4_ret);
            t_err = MHEG5_ERR_INTERNAL;
            break;
        }
        
        at_cmd[0].e_code       = EVCTX_CMD_DEV_TYPE;
        at_cmd[0].u.e_dev_type = DEV_TUNER;
        at_cmd[1].e_code       = EVCTX_CMD_SRC_INFO;
        at_cmd[1].u.ps_name    = SN_MAIN_TUNER_GRP;
        at_cmd[2].e_code       = EVCTX_CMD_CONN_HANDLE;
        at_cmd[2].u.h_obj      = _t_loader.h_conn;
        at_cmd[3].e_code       = EVCTX_CMD_SVL_ID;
        at_cmd[3].u.ui2_number = _t_loader.ui2_svl_id;
        at_cmd[4].e_code       = EVCTX_CMD_SVL_REC_ID;
        at_cmd[4].u.ui2_number = t_svl_rec.ui2_svl_rec_id;
        at_cmd[5].e_code       = EVCTX_CMD_END;

        x_dbg_stmt("[MHEG5-SIL]INFO:%s#%d: svc_index: %d, svc_channel_id: %d, h_conn: %p, svl_id: %d, svl_rec_id: %d \r\n ",
                                __FUNCTION__, __LINE__, ui4_svc_idx, ui4_channel_id, _t_loader.h_conn, _t_loader.ui2_svl_id, t_svl_rec.ui2_svl_rec_id);

        if (b_porf == FALSE)
        {
            /* Load present event */
            i4_ret = x_evctx_load_current_event(
                                at_cmd,
                                (VOID*)&t_loading_info,
                                _sil_evctx_nfy_fct,
                                &(t_loading_info.h_evt),
                                &e_cond);

            x_dbg_stmt("[SIL]x_evctx_load_current_event return OK, h_evctx:%d, e_cond:%d\n", t_loading_info.h_evt, e_cond);
        }
        else
        {
            /* Load next event */
            i4_ret = x_evctx_load_next_event(
                                at_cmd,
                                (VOID*)&t_loading_info,
                                _sil_evctx_nfy_fct,
                                &(t_loading_info.h_evt),
                                &e_cond);
            
            x_dbg_stmt("[SIL]x_evctx_load_next_event return OK, h_evctx:%d, e_cond:%d\n", t_loading_info.h_evt, e_cond);
        }

        if ( EVCTXR_OK != i4_ret )
        {
            x_sema_unlock(t_loading_info.h_sema);
            
            x_dbg_stmt("%s#%d:load next event fail:%d\r\n", __FUNCTION__, __LINE__, i4_ret);
            t_err = MHEG5_ERR_INTERNAL;
            break;
        }

        if (e_cond == EVCTX_COND_AVAIL)
        {
            x_sema_unlock(t_loading_info.h_sema);
            
            x_dbg_stmt("%s#%d:load event OK(e_cond:%d)\r\n", __FUNCTION__, __LINE__, e_cond);
        }
        else if (e_cond == EVCTX_COND_TRANSITION)
        {
            i4_ret = x_sema_lock(t_loading_info.h_sema, X_SEMA_OPTION_WAIT);
            if ( OSR_OK != i4_ret )
            {
                x_dbg_stmt("%s#%d:lock semaphore fail:%d\r\n", __FUNCTION__, __LINE__, i4_ret);
                t_err = MHEG5_ERR_INTERNAL;
                break;
            }
            
            x_sema_unlock(t_loading_info.h_sema);

            if (t_loading_info.e_cond != EVCTX_COND_AVAIL && t_loading_info.e_cond != EVCTX_COND_UPDATED)
            {
                x_dbg_stmt("%s#%d:load event fail(e_cond:%d)\r\n", __FUNCTION__, __LINE__, t_loading_info.e_cond);
                t_err = MHEG5_ERR_INTERNAL;
                break;
            }
            else
            {
                x_dbg_stmt("%s#%d:load event OK(e_cond:%d)\r\n", __FUNCTION__, __LINE__, t_loading_info.e_cond);
            }
        }
        else
        {
            x_sema_unlock(t_loading_info.h_sema);
            
            x_dbg_stmt("%s#%d:load event fail(e_cond:%d)\r\n", __FUNCTION__, __LINE__, e_cond);
            t_err = MHEG5_ERR_INTERNAL;
            break;
        }

        /* Get Event Details */
        {
            UINT32     z_info_len = 0;
            VOID       *p_data = NULL;
            UINT8      ui1_try_times = 0;
            HANDLE_T   h_event = t_loading_info.h_evt;

            /** Get event name **/
            for (ui1_try_times = 0; ui1_try_times < MHEG_HK_S639_LANG_TRY_TIMES; ui1_try_times++)
            {
                i4_ret = _get_event_info(
                                    h_event,
                                    EVCTX_KEY_TYPE_EVENT_TITLE,
                                    mheg_hk_s639_lang_pool[ui1_try_times],
                                    &p_data,
                                    &z_info_len);

                if (EVCTXR_INFO_NOT_FOUND == i4_ret)
                {
                    x_dbg_stmt("%s#%d:get event name with lang:%s error:%d\r\n", __FUNCTION__, __LINE__, mheg_hk_s639_lang_pool[ui1_try_times], i4_ret);
                }
                else if (EVCTXR_OK == i4_ret)
                {
                    pt_evt_details->eventName.s_len = (U32BIT)z_info_len;
                    pt_evt_details->eventName.s_data = (U8BIT*)p_data;

                    /* Don't free p_data here */
                    z_info_len  = 0;
                    p_data = NULL;
                    break;
                }
                else
                {
                    /* do nothing */
                }
            }
            
            if (EVCTXR_OK != i4_ret)
            {
                x_dbg_stmt("%s#%d:get event name error:%d\r\n", __FUNCTION__, __LINE__, i4_ret);
                
                t_err = MHEG5_ERR_INTERNAL;
                break;
            }
            
            /** Get event short descriptor **/
            for (ui1_try_times = 0; ui1_try_times < MHEG_HK_S639_LANG_TRY_TIMES; ui1_try_times++)
            {
                i4_ret = _get_event_info(
                                    h_event,
                                    EVCTX_KEY_TYPE_EVENT_TEXT,
                                    mheg_hk_s639_lang_pool[ui1_try_times],
                                    &p_data,
                                    &z_info_len);
                
                if (EVCTXR_INFO_NOT_FOUND == i4_ret)
                {
                    x_dbg_stmt("%s#%d:get event text with lang:%s error:%d\r\n", __FUNCTION__, __LINE__, mheg_hk_s639_lang_pool[ui1_try_times], i4_ret);
                }
                else if (EVCTXR_OK == i4_ret)
                {
                    pt_evt_details->shortDescription.s_len = (U32BIT)z_info_len;
                    pt_evt_details->shortDescription.s_data = (U8BIT*)p_data;
                    
                    /* Don't free p_data here */
                    z_info_len  = 0;
                    p_data = NULL;
                    break;
                }
                else
                {
                    /* do nothing */
                }
            }
            
            if (EVCTXR_OK != i4_ret)
            {
                x_dbg_stmt("%s#%d:get event text error:%d\r\n", __FUNCTION__, __LINE__, i4_ret);
                
                t_err = MHEG5_ERR_INTERNAL;
                break;
            }

            /** Get event parental rating **/
            for (ui1_try_times = 0; ui1_try_times < MHEG_HK_S639_LANG_TRY_TIMES; ui1_try_times++)
            {
                i4_ret = _get_event_info(
                                    h_event,
                                    EVCTX_KEY_TYPE_RATING,
                                    mheg_hk_s639_lang_pool[ui1_try_times],
                                    &p_data,
                                    &z_info_len);
                
                if (EVCTXR_INFO_NOT_FOUND == i4_ret)
                {
                    x_dbg_stmt("%s#%d:get event parental rating with lang:%s error:%d\r\n", __FUNCTION__, __LINE__, mheg_hk_s639_lang_pool[ui1_try_times], i4_ret);
                }
                else if (EVCTXR_OK == i4_ret)
                {
                    EVCTX_ATSC_RATING_LIST_T* p_rating = (EVCTX_ATSC_RATING_LIST_T*)p_data;
                    pt_evt_details->parentalRating = (U32BIT)(p_rating->pt_ratings->ui2_rating_value);
                    
                    x_mem_free(p_data);
                    z_info_len  = 0;
                    p_data = NULL;
                    break;
                }
                else
                {
                    /* do nothing */
                }
            }
            
            if (EVCTXR_OK != i4_ret)
            {
                x_dbg_stmt("%s#%d:get event parental rating error:%d\r\n", __FUNCTION__, __LINE__, i4_ret);
                
                t_err = MHEG5_ERR_INTERNAL;
                break;
            }
            
            /** Get event start date and start time **/
            i4_ret = _get_event_info(
                                h_event,
                                EVCTX_KEY_TYPE_START_TIME,
                                NULL,
                                &p_data,
                                &z_info_len);
            if (EVCTXR_OK != i4_ret)
            {
                x_dbg_stmt("%s#%d:get event start date and start time error:%d\r\n", __FUNCTION__, __LINE__, i4_ret);
                
                t_err = MHEG5_ERR_INTERNAL;
                break;
            }
            else
            {
                S_Time t_start_time;
                U32BIT ui4_start_data = 0;
                
                /** Parse event start data and time **/
                i4_ret = _parse_event_mjd_data_time(
                                    (TIME_T*)p_data,
                                    &ui4_start_data,
                                    &t_start_time);
                
                if( MHEG5R_OK != i4_ret)
                {
                    x_dbg_stmt("%s#%d: Parse event start data and time error:%d\r\n", __FUNCTION__, __LINE__, i4_ret);
                    
                    t_err = MHEG5_ERR_INTERNAL;
                    break;
                }
                else
                {
                    pt_evt_details->startDate = ui4_start_data;
                    pt_evt_details->startTime = t_start_time;
                    
                    x_mem_free(p_data);
                    p_data = NULL;
                    z_info_len = 0;
                }
            }
            
            /** Get event duration time **/
            i4_ret = _get_event_info(
                            h_event,
                            EVCTX_KEY_TYPE_DURATION,
                            NULL,
                            &p_data,
                            &z_info_len);
            if (EVCTXR_OK != i4_ret)
            {
                x_dbg_stmt("%s#%d:get event duration time error:%d\r\n", __FUNCTION__, __LINE__, i4_ret);
                
                t_err = MHEG5_ERR_INTERNAL;
                break;
            }
            else
            {
                UINT64 ui8_duation;
                
                ui8_duation = *((TIME_T*)p_data);
                pt_evt_details->duration.hours = (U16BIT)(ui8_duation/3600);
                pt_evt_details->duration.minutes = (U8BIT)((ui8_duation%3600)/60);
                pt_evt_details->duration.seconds = (U8BIT)((ui8_duation%3600)%60);  
                
                x_mem_free(p_data);
                p_data = NULL;
                z_info_len = 0;     
            }

            /** Get event category **/
            for (ui1_try_times = 0; ui1_try_times < MHEG_HK_S639_LANG_TRY_TIMES; ui1_try_times++)
            {
                i4_ret = _get_event_info(
                                h_event,
                                EVCTX_KEY_TYPE_EVENT_CATEGORY,
                                mheg_hk_s639_lang_pool[ui1_try_times],
                                &p_data,
                                &z_info_len);                
                if (EVCTXR_INFO_NOT_FOUND == i4_ret)
                {
                    x_dbg_stmt("%s#%d:get event category with lang:%s error:%d\r\n", __FUNCTION__, __LINE__, mheg_hk_s639_lang_pool[ui1_try_times], i4_ret);
                }
                else if (EVCTXR_OK == i4_ret)
                {
                    pt_evt_details->category.s_len = (U32BIT)z_info_len;
                    pt_evt_details->category.s_data = (U8BIT*)p_data;
                                    
                    /* Don't free p_data here */
                    p_data = NULL;
                    z_info_len = 0;
                    break;
                }
                else
                {
                    /* do nothing */
                }
            }
            
            if (EVCTXR_OK != i4_ret)
            {
                x_dbg_stmt("%s#%d:get event category error:%d\r\n", __FUNCTION__, __LINE__, i4_ret);
                
                t_err = MHEG5_ERR_INTERNAL;
                break;
            }
            
            /** Get event freeNotCA **/
            i4_ret = _get_event_info(
                            h_event,
                            EVCTX_KEY_TYPE_FREE_CA_MODE,
                            NULL,
                            &p_data,
                            &z_info_len);
            if (EVCTXR_OK != i4_ret)
            {
                x_dbg_stmt("%s#%d:Get event freeNotCA error:%d\r\n", __FUNCTION__, __LINE__, i4_ret);
                
                t_err = MHEG5_ERR_INTERNAL;
                break;
            }
            else
            {
                pt_evt_details->freeNotCA = (BOOLEAN)(*((UINT8*)p_data));
                                
                x_mem_free(p_data);
                p_data = NULL;
                z_info_len = 0;     
            }
        }
    }while(0);
    
    UNLOCK(&_t_loader);
    
    if (t_loading_info.h_evt != NULL_HANDLE)
    {
        x_evctx_delete_event(t_loading_info.h_evt);
        t_loading_info.h_evt = NULL_HANDLE;
    }

    if (t_loading_info.h_sema != NULL_HANDLE)
    {
        x_sema_delete(t_loading_info.h_sema);
        t_loading_info.h_sema = NULL_HANDLE;
    }

    if( MHEG5_OK != t_err )
    {
        if ( pt_evt_details->eventName.s_data != NULL )
        {
            x_mem_free(pt_evt_details->eventName.s_data);
            pt_evt_details->eventName.s_data = NULL;
        }

        if ( pt_evt_details->shortDescription.s_data != NULL )
        {
            x_mem_free(pt_evt_details->shortDescription.s_data);
            pt_evt_details->shortDescription.s_data = NULL;
        }

        if ( pt_evt_details->category.s_data != NULL )
        {
            x_mem_free(pt_evt_details->category.s_data);
            pt_evt_details->category.s_data = NULL;
        }
        
    }
    
    return t_err; 

#else

    return MHEG5_ERR_INTERNAL;

#endif
}
#endif

void sil_release_service_details(
    S_ServiceDetails *pt_svc_details)
{
    if( pt_svc_details != NULL )
    {
        if (pt_svc_details->serviceName.s_data != NULL)
        {
            x_mem_free(pt_svc_details->serviceName.s_data);
            pt_svc_details->serviceName.s_data = NULL;
        }
        if (pt_svc_details->serviceProvider.s_data != NULL)
        {
            x_mem_free(pt_svc_details->serviceProvider.s_data);
            pt_svc_details->serviceProvider.s_data = NULL;
        }
    }
}

void sil_release_event_details(S_EventDetails *pt_evt_details)
{
    if( pt_evt_details != NULL )
    {
        if(pt_evt_details->eventName.s_data != NULL)
        {
            x_mem_free(pt_evt_details->eventName.s_data);
            pt_evt_details->eventName.s_data = NULL;
        }

        if(pt_evt_details->shortDescription.s_data != NULL)
        {
            x_mem_free(pt_evt_details->shortDescription.s_data);
            pt_evt_details->shortDescription.s_data=NULL;
        }
        if(pt_evt_details->category.s_data != NULL)
        {
            x_mem_free(pt_evt_details->category.s_data);
            pt_evt_details->category.s_data=NULL;
        }
    }
}



/*-----------------------------------------------------------------------------
 * private methods implementations
 *---------------------------------------------------------------------------*/
static INT32 _add_query(
    ptmMHEG5SiQuery_t       pt_in_query,
    VOID*                   pv_query_ref,
    VOID*                   pv_usr_data,
    SLIST_SI_QUERY_T*       pt_query_lst,
    _STREAM_ANNOTATION_T*   pt_strm_ant,
    BOOL                    b_load_pmt,
    HANDLE_T*               ph_query)
{
    _SI_QUERY_T*    pt_qry = NULL;
    HANDLE_T        h_qry  = NULL_HANDLE;
    INT32           i4_ret = MHEG5R_OUT_OF_MEMORY;
    
    pt_qry = SIL_MALLOC(sizeof(_SI_QUERY_T));

    if (pt_qry)
    {
        i4_ret = MHEG5R_OUT_OF_HANDLE;
        
        if (HR_OK == handle_alloc(
            HT_MHEG5_SI_QUERY, 
            pt_qry, 
            NULL, 
            _si_query_free_cb, 
            &h_qry))
        {
            pt_qry->h_this       = h_qry;
            pt_qry->ui1_status   = SI_QUERY_STATUS_PENDING;
            pt_qry->ui1_source   = (UINT8)((ph_query) ? SI_QUERY_SOURCE_MHEG5_STACK : SI_QUERY_SOURCE_GLUE_LAYER);
            pt_qry->pv_usr_data  = pv_usr_data;
            pt_qry->pv_query_ref = pv_query_ref;
            pt_qry->t_query      = *pt_in_query;

            if (pt_strm_ant)
            {
                pt_qry->b_strm_ant_valid = TRUE;
                pt_qry->t_strm_ant       = *pt_strm_ant;
            }
            else
            {
                pt_qry->b_strm_ant_valid = FALSE;
            }
            
            i4_ret = MHEG5R_OK;
            
            if (b_load_pmt)
            {
                if (NULL == _create_svc_info_entry(
                    _t_loader.h_pat,
                    (UINT16) pt_in_query->service_id,
                    _t_loader.ui4_seq_num,
                    FALSE))
                {
                    i4_ret = MHEG5R_LOAD_PMT_FAIL;
                }
            }
            
            if (MHEG5R_OK == i4_ret)
            {
                SLIST_INSERT_HEAD(pt_qry, pt_query_lst, t_link);

                if (ph_query)
                {
                    *ph_query = h_qry;
                }
                
                i4_ret = MHEG5R_OK;
            }    
            else
            {
                x_handle_free(h_qry);
            }
        }
        else
        {
            SIL_MFREE(pt_qry, sizeof(_SI_QUERY_T));
        }
    }

    return (i4_ret);
}

static BOOL _check_persistent_tbl_validity(_SI_LOADER_INFO_T* pt_loader)
{
    BOOL b_ret = FALSE;
    
    if (
        #if defined(SYS_MHEG5_HK_SUPPORT) || defined(SYS_MHEG5_AUS_SUPPORT)
        x_handle_valid(pt_loader->h_sdt_actual) &&
        #endif
        x_handle_valid(pt_loader->h_pat)        &&
        x_handle_valid(pt_loader->h_pmt_active))
    {
        b_ret = TRUE;
    }

    return (b_ret);
}

static INT32 _check_svc_lst_for_query(
    ptmMHEG5SiQuery_t       pt_in_query,
    SLIST_SVC_INFO_T*       pt_svc_info_lst,
    ptmMHEG5SiQueryResult_t pt_qry_res,
    _STREAM_ANNOTATION_T*   pt_strm_ant,
    BOOL*                   pb_pmt_loading,
    MPEG_2_PID_T*           pt_pcr_pid,
    UINT8*                  pui1_enc_type)
{
    UINT16          ui2_svc_id;
    _SVC_INFO_T*    pt_svc_info;
    INT32           i4_ret = MHEG5R_ASYNC_RESPONSE;

    *pb_pmt_loading = FALSE;
    
    ui2_svc_id = (UINT16) pt_in_query->service_id;
    
    SLIST_FOR_EACH(pt_svc_info, pt_svc_info_lst, t_link)
    {
        if (ui2_svc_id == pt_svc_info->ui2_svc_id)
        {
            if (SVC_INFO_LOADED(pt_svc_info))
            {
                if (MHEG5R_OK == _parse_query(
                    pt_svc_info->h_pmt,
                    pt_in_query,
                    pt_qry_res,
                    pt_strm_ant,
                    pui1_enc_type))
                {
                    i4_ret = MHEG5R_OK;

                    if (pt_pcr_pid)
                    {
                        TM_COND_T   e_cond;
                        
                        x_psi_get_pcr_pid(pt_svc_info->h_pmt, pt_pcr_pid, &e_cond);
                    }
                }
                else
                {
                    i4_ret = MHEG5R_PARSE_PMT_FAIL;
                }
            }
            else
            {
                *pb_pmt_loading = TRUE;
            }
            
            break;
        }
    }

    return (i4_ret);
}

static VOID _check_svc_subscription_by_pat(
    HANDLE_T            h_pat,
    SLIST_SVC_INFO_T*   pt_si_list)
{
    PSI_PID_SVC_ID_INFO_T   t_pid_svc_info;
    _SVC_INFO_T*            pt_svc_info;
    UINT16                  ui2_entry_num;
    BOOL                    b_found;
    SIZE_T                  z_len;
    TM_COND_T               e_cond;
    INT32                   i;
    
    z_len = sizeof(ui2_entry_num);
    
    if (TMR_OK == x_tm_get_obj(
        h_pat,
        TM_GET_TYPE_NUM_FIRST_LOOP_ENTRIES,
        NULL,
        (VOID*) &ui2_entry_num,
        &z_len,
        &e_cond))
    {
        SLIST_FOR_EACH(pt_svc_info, pt_si_list, t_link)
        {
            if (SVC_INFO_SUBSCRIBED(pt_svc_info) ||
            	  /* always monitor the active service, even it's UNSUBSCRIBED */
            	  pt_svc_info->ui2_svc_id == _t_loader.ui2_active_svc_id)
            {
                b_found = FALSE;
                
                for (i = 0; i < (INT32) ui2_entry_num; i++)
                {
                    if (TMR_OK == x_psi_get_svc_entry(
                        h_pat, 
                        (UINT16) i, 
                        &t_pid_svc_info,
                        &e_cond))
                    {
                        if (pt_svc_info->ui2_svc_id == t_pid_svc_info.ui2_svc_id)
                        {
                            b_found = TRUE;
                            break;
                        }
                    }
                }

                if ((pt_svc_info->ui1_status & SVC_INFO_STATUS_REMOVED) &&
                    (TRUE == b_found))
                {
                    pt_svc_info->ui1_status &= ~SVC_INFO_STATUS_REMOVED;

                    if (pt_svc_info->ui2_svc_id == _t_loader.ui2_active_svc_id)
                    {

                        SI_DEBUG(("start load_pmt\r\n"));
                        _load_pmt(
                            h_pat,
                            pt_svc_info->ui2_svc_id,
                            _t_loader.ui4_seq_num,
                            &pt_svc_info->h_pmt);
                        
                        /* current service returns */
                        _notify_load_status(
                            &_t_loader, 
                            MHEG5_NFY_COND_SERVICE_RETURNS);
                    }
                    else
                    {
                        tmMHEG5NotifySiServiceInfoChanged(
                            pt_svc_info->ui2_svc_id,
                            MHEG5_SI_SERVICE_CHANGED);
                    }
                }
                else if ((0 == (pt_svc_info->ui1_status & SVC_INFO_STATUS_REMOVED)) &&
                         (FALSE == b_found))
                {
                    pt_svc_info->ui1_status |= SVC_INFO_STATUS_REMOVED;

                    if (pt_svc_info->ui2_svc_id == _t_loader.ui2_active_svc_id)
                    {
                        /* current service is removed */
                        _notify_load_status(
                            &_t_loader, 
                            MHEG5_NFY_COND_SERVICE_GONE);
                    }
                    else
                    {
                        tmMHEG5NotifySiServiceInfoChanged(
                            pt_svc_info->ui2_svc_id,
                            MHEG5_SI_SERVICE_DELETED);
                    }
                }
            }
        }
    }
}

static VOID _check_svc_subscription_by_sdt(
    HANDLE_T            h_sdt,
    SLIST_SVC_INFO_T*   pt_si_list)
{
    DVB_SI_SVC_ENTRY_INFO_T     t_dvb_si_entry;
    _SVC_INFO_T*                pt_svc_info;
    UINT16                      ui2_entry_num;
    UINT8                       ui1_running_status;
    BOOL                        b_found;
    TM_COND_T                   e_cond;
    INT32                       i;
    
    if (TMR_OK == x_dvb_si_get_num_svc_entries(
        h_sdt,
        &ui2_entry_num,
        &e_cond))
    {
        SLIST_FOR_EACH(pt_svc_info, pt_si_list, t_link)
        {
            if (SVC_INFO_SUBSCRIBED(pt_svc_info))
            {
                b_found = FALSE;
                
                for (i = 0; i < (INT32) ui2_entry_num; i++)
                {
                    if (TMR_OK == x_dvb_si_get_svc_entry(
                        h_sdt, 
                        (UINT16) i, 
                        &t_dvb_si_entry,
                        &e_cond))
                    {
                        if (pt_svc_info->ui2_svc_id == t_dvb_si_entry.ui2_svc_id)
                        {
                            ui1_running_status = t_dvb_si_entry.ui1_flags & SVC_MASK_RUNNING_STATUS;
                            
                            if ((SVC_FLAG_RUNNING          == ui1_running_status) ||
                                (SVC_FLAG_STATUS_UNDEFINED == ui1_running_status))
                            {
                                b_found = TRUE;
                            }
                            else
                            {
                                b_found = FALSE;
                            }
                            break;
                        }
                    }
                }

                if ((pt_svc_info->ui1_status & SVC_INFO_STATUS_REMOVED) &&
                    (TRUE == b_found))
                {
                    pt_svc_info->ui1_status &= ~SVC_INFO_STATUS_REMOVED;

                    if (pt_svc_info->ui2_svc_id == _t_loader.ui2_active_svc_id)
                    {
                        /* current service returns */
                        _notify_load_status(
                            &_t_loader, 
                            MHEG5_NFY_COND_SERVICE_RETURNS);
                    }
                    else
                    {
                        tmMHEG5NotifySiServiceInfoChanged(
                            pt_svc_info->ui2_svc_id,
                            MHEG5_SI_SERVICE_CHANGED);
                    }
                }
                else if ((0 == (pt_svc_info->ui1_status & SVC_INFO_STATUS_REMOVED)) &&
                         (FALSE == b_found))
                {
                    pt_svc_info->ui1_status |= SVC_INFO_STATUS_REMOVED;

                    if (pt_svc_info->ui2_svc_id == _t_loader.ui2_active_svc_id)
                    {
                        /* current service is removed */
                        _notify_load_status(
                            &_t_loader, 
                            MHEG5_NFY_COND_SERVICE_GONE);
                    }
                    else
                    {
                        tmMHEG5NotifySiServiceInfoChanged(
                            pt_svc_info->ui2_svc_id,
                            MHEG5_SI_SERVICE_DELETED);
                    }
                }
            }
        }
    }
}

static VOID _cm_connect_nfy(
      HANDLE_T  h_conn,
      CM_COND_T e_nfy_cond,
      VOID*     pv_nfy_tag,
      UINT32    ui4_data)
{
    if (pv_nfy_tag)
    {
        if ((e_nfy_cond == CM_COND_CONNECTED) ||
            (e_nfy_cond == CM_COND_DISCONNECTED))
        {
            _send_notify(
                (CM_COND_CONNECTED == e_nfy_cond) ? SIL_MSG_CONNECTED : SIL_MSG_DISCONNECTED,
                ((_SIL_NOTIFY_T*) pv_nfy_tag)->ui4_seq_num,
                h_conn,
                0,
                0,
                0,
                (CM_COND_CONNECTED == e_nfy_cond) ? NULL : pv_nfy_tag);
        }
    }
}

static VOID _cm_open_pipe_nfy(
    HANDLE_T  h_pipe,
    CM_COND_T e_nfy_cond,
    VOID*     pv_nfy_tag,
    UINT32    ui4_data)
{
    if (pv_nfy_tag)
    {
        if ((e_nfy_cond == CM_COND_OPENED) ||
            (e_nfy_cond == CM_COND_CLOSED))
        {
            _send_notify(
                (CM_COND_OPENED == e_nfy_cond) ? SIL_MSG_PIPE_OPENED : SIL_MSG_PIPE_CLOSED,
                ((_SIL_NOTIFY_T*) pv_nfy_tag)->ui4_seq_num,
                h_pipe,
                0,
                0,
                0,
                (CM_COND_OPENED == e_nfy_cond) ? NULL : pv_nfy_tag);
        }
    }
}

static INT32 _connect(
    HANDLE_T    h_pipe,
    UINT16      ui2_svl_id,
    UINT16      ui2_svl_rec_id,
    UINT32      ui4_seq_num,
    HANDLE_T*   ph_conn)
{
    CM_COMMAND_T    at_cmds[3]; /* command arrays */
    CM_COND_T       e_cond;
    _SIL_NOTIFY_T*  pt_nfy;
    INT32           i4_ret;
    HANDLE_T        h_conn = NULL_HANDLE;
    
    i4_ret = MHEG5R_OUT_OF_MEMORY;
    
    pt_nfy = SIL_MALLOC(sizeof(_SIL_NOTIFY_T));

    if (pt_nfy)
    {
        i4_ret = MHEG5R_CONNECT_FAIL;

        pt_nfy->ui4_seq_num = ui4_seq_num;
        pt_nfy->ui2_svc_id  = 0;
        
        at_cmds[0].e_code         = BRDCST_CONN_SVC_LST_ID;
        at_cmds[0].u.ui2_number = ui2_svl_id;
        at_cmds[1].e_code         = BRDCST_CONN_SVL_REC_ID;
        at_cmds[1].u.ui2_number = ui2_svl_rec_id;
        at_cmds[2].e_code         = CONN_END;

        if (CMR_OK == x_cm_connect(
            h_pipe,
            at_cmds,
            (VOID*) pt_nfy,
            _cm_connect_nfy,
            &h_conn,
            &e_cond))
        {
            i4_ret = MHEG5R_OK;

            *ph_conn = h_conn;
            
            if (CM_COND_CONNECTED == e_cond)
            {
                i4_ret = _send_notify(
                    SIL_MSG_CONNECTED, 
                    SEQ_NUM(&_t_loader), 
                    h_conn, 
                    0, 
                    0,
                    0,
                    NULL);
            }
            else if (CM_COND_DISCONNECTED == e_cond)
            {
                i4_ret = MHEG5R_CONNECT_FAIL;
            }
            else
            {
                ;
            }
        }
    }

    if (MHEG5R_OK != i4_ret)
    {
        if (pt_nfy)
        {
            SIL_MFREE(pt_nfy, sizeof(_SIL_NOTIFY_T));
        }

        if (NULL_HANDLE != h_conn)
        {
            x_cm_disconnect(h_conn, &e_cond);
        }
    }

    return (i4_ret);
}

static _SDT_INFO_T* _find_sdt_info(
    UINT16  ui2_on_id, 
    UINT16  ui2_ts_id)
{
    _SDT_INFO_T* pt_sdt = NULL;
    
    SLIST_FOR_EACH(pt_sdt, &(SDT_OTHER_LIST(&_t_loader)), t_link)
    {
        if ((pt_sdt->ui2_on_id == ui2_on_id) &&
            (pt_sdt->ui2_ts_id == ui2_ts_id))
        {
            break;
        }
    }

    return (pt_sdt);
}

static PARSE_RET_T _find_specified_data(
    HANDLE_T      h_pmt,
    const UINT8*  pui1_obj_data,
    SIZE_T        z_obj_data_len,
    UINT16        ui2_count,
    VOID*         pv_data,
    SIZE_T        z_data_len)
{

    _PARSE_PARAM_T* pt_parse = (_PARSE_PARAM_T*) pv_data;
    PARSE_RET_T     t_ret    = PARSE_RET_NEXT;
    INT32           i4_len   = (INT32) pt_parse->ui1_ref_len;
    UINT32          ui4_data = 0;
    const UINT8*    pui1_buf;

    if (pt_parse->ui1_flag & FLAG_FIND_REF_DATA)
    {
        if (VALID_DESCRIPTOR_LEN((SIZE_T)(i4_len + 2)))
        {
            pui1_buf = pui1_obj_data + 2;
            
            while (i4_len--)
            {
                ui4_data = (UINT32) (ui4_data << 8) + (UINT32) pui1_buf[0];
                pui1_buf++;
            }

            if (ui4_data == pt_parse->ui4_ref)
            {
                if (pt_parse->ui1_flag & (FLAG_GET_NETWORK_BOOT_INFO | FLAG_GET_SERVICE_BOOT_INFO))
                {
#if 0
                    if (VALID_DESCRIPTOR_LEN(pui1_obj_data[1] + 2) &&
                        (APPLICATION_TYPE_CODE_LAUNCH_PROFILE == 
                            (UINT16) GET_UINT16_FROM_PTR_BIG_END((pui1_obj_data + 4))))
#else
                    if (VALID_DESCRIPTOR_LEN((SIZE_T)(pui1_obj_data[1] + 2)))
#endif
                    {
                        UINT16 ui2_app_type = (UINT16) GET_UINT16_FROM_PTR_BIG_END((pui1_obj_data + 4));
                        INT16  i2_app_priority = (INT16) pui1_obj_data[6];
                        
                        if ((APPLICATION_TYPE_CODE_LAUNCH_PROFILE   == ui2_app_type ||
                             APPLICATION_TYPE_CODE_BASELINE_PROFILE == ui2_app_type ||
                             APPLICATION_TYPE_CODE_105_PROFILE == ui2_app_type) &&
                            (i2_app_priority > pt_parse->i2_boot_priority_hint))
                        {
                            INT32 i = 0;
                            INT32 i4_n2 = (INT32) pui1_obj_data[7];

                            pt_parse->ui1_flag &= ~FLAG_CHECK_AP_TYPE_RPIORITY;
                            pt_parse->i2_boot_priority_hint = i2_app_priority;
                            
                            pui1_buf = pui1_obj_data + 8;

                            while (i < i4_n2)
                            {
                                if (TAG_NETWORK_BOOT_INFO_SUB_DESCRIPTOR == pui1_buf[0])
                                {
                                    INT32 i4_sub_len = (INT32) pui1_buf[1];
                                    
                                    if (pt_parse->ui1_flag & FLAG_GET_NETWORK_BOOT_INFO)
                                    {
                                        pt_parse->ui2_desc_len = MIN((UINT16)(i4_sub_len - 2), pt_parse->ui2_desc_len);
                                        
                                        if ((pt_parse->pui1_descriptor) &&
                                            (pt_parse->ui2_desc_len > 0))
                                        {
                                            x_memcpy(
                                                pt_parse->pui1_descriptor, 
                                                pui1_buf + 4, 
                                                pt_parse->ui2_desc_len);
                                        }

                                        pt_parse->ui1_nb_version = pui1_buf[2];
                                        pt_parse->ui1_nb_action  = pui1_buf[3];
                                    
                                        pt_parse->ui1_flag &= ~FLAG_GET_NETWORK_BOOT_INFO;
                                    }

                                    pui1_buf += i4_sub_len + 2;
                                    i += i4_sub_len + 2;
                                    #ifndef MHEG5_HD_SUPPORT
                                    /* NDT009 Scene2 */
                                    break;
                                    #endif
                                }
                                else if (TAG_SERVICE_BOOT_INFO_SUB_DESCRIPTOR == pui1_buf[0])
                                {
                                    if (pt_parse->ui1_flag & FLAG_GET_SERVICE_BOOT_INFO)
                                    {
                                        pt_parse->ui2_association_tag = (UINT16) GET_UINT16_FROM_PTR_BIG_END((pui1_buf + 2));
                                        pt_parse->ui1_flag &= ~FLAG_GET_SERVICE_BOOT_INFO;
                                    }
                                    
                                    pui1_buf += 4;
                                    i += 4;
                                }
                                else
                                {
                                    break;
                                }
                            }
                        }
                    }
                }

                pt_parse->ui1_flag &= ~FLAG_FIND_REF_DATA;
            }
        }
    }
    
    if (0 == pt_parse->ui1_flag)
    {
        t_ret = PARSE_RET_END;
    }
    
    return (t_ret);
}

static _SVC_INFO_T* _find_svc_info(UINT16 ui2_svc_id)
{
    _SVC_INFO_T* pt_svc = NULL;
    
    SLIST_FOR_EACH(pt_svc, &(SVC_INFO_LIST(&_t_loader)), t_link)
    {
        if (pt_svc->ui2_svc_id == ui2_svc_id)
        {
            break;
        }
    }

    return (pt_svc);
}

static VOID _free_loader_tables(_SI_LOADER_INFO_T* pt_loader)
{
    /* free all loaded PMT */
    _free_svc_info_lst(&(SVC_INFO_LIST(pt_loader)));
    
    if (NULL_HANDLE != pt_loader->h_nit)
    {
        _free_si_table(pt_loader->h_nit);
        pt_loader->h_nit = NULL_HANDLE;
    }

    if (NULL_HANDLE != pt_loader->h_sdt_actual)
    {
        _free_si_table(pt_loader->h_sdt_actual);
        pt_loader->h_sdt_actual = NULL_HANDLE;
    }
    
    if (NULL_HANDLE != pt_loader->h_pat)
    {
        _free_si_table(pt_loader->h_pat);
        pt_loader->h_pat = NULL_HANDLE;
    }

    /* free all loaded SDT_other */
    _free_sdt_info_lst(&(SDT_OTHER_LIST(pt_loader)));
    
    pt_loader->ui4_tbl_status = 0;
}

#if 0
static VOID _free_deferred_query(SLIST_SI_QUERY_T* pt_query_lst)
{
    _SI_QUERY_T* pt_query;
    
    while (1)
    {
        pt_query = SLIST_FIRST(pt_query_lst);

        if (pt_query)
        {
            SLIST_REMOVE(pt_query, t_link);
            
            tmMHEG5NotifySiQueryResult(
                pt_query->t_query_ref,
                pt_query->pv_usr_data,
                MHEG5_SI_FAILURE,
                NULL,
                (VOID*) pt_query->h_this);
            
            x_handle_free(pt_query->h_this);
        }
        else
        {
            break;
        }
    }
}
#endif

static VOID _free_sdt_info(_SDT_INFO_T* pt_sdt_info)
{
    /* free the svc_info entry */
    SLIST_REMOVE(pt_sdt_info, t_link);
    _free_si_table(pt_sdt_info->h_sdt);
    SIL_MFREE(pt_sdt_info, sizeof(_SDT_INFO_T));
}

static VOID _free_sdt_info_lst(
    SLIST_SDT_INFO_T*   pt_sdt_lst)
{
    _SDT_INFO_T* pt_sdt = NULL;
    
    while (1)
    {
        pt_sdt = SLIST_FIRST(pt_sdt_lst);

        if (pt_sdt)
        {
            _free_sdt_info(pt_sdt);
        }
        else
        {
            break;
        }
    }
}

static VOID _free_si_table(HANDLE_T h_tbl)
{
    VOID* pv_tag;
    
    if (HR_OK == x_handle_get_tag(h_tbl, &pv_tag))
    {
        if (pv_tag)
        {
            SIL_MFREE(pv_tag, sizeof(_SIL_NOTIFY_T));
        }
        
        x_tm_free(h_tbl);
    }
}

static VOID _free_svc_info(_SVC_INFO_T* pt_svc_info)
{
    /* free the svc_info entry */
    SLIST_REMOVE(pt_svc_info, t_link);
    _free_si_table(pt_svc_info->h_pmt);
    SIL_MFREE(pt_svc_info, sizeof(_SVC_INFO_T));
}

static VOID _free_svc_info_lst(
    SLIST_SVC_INFO_T*   pt_si_lst)
{
    _SVC_INFO_T* pt_svc      = NULL;
    
    while (1)
    {
        pt_svc = SLIST_FIRST(pt_si_lst);

        if (pt_svc)
        {
            _free_svc_info(pt_svc);
        }
        else
        {
            break;
        }
    }
}

static INT32 _get_boot_carousel(
    HANDLE_T        h_pmt,
    MPEG_2_PID_T*   pt_pid,
    UINT8*          pui1_comp_tag,
    UINT16*         pui2_association_tag,
    UINT32*         pui4_carousel_id,
    UINT8*          pui1_carousel_id_desc)
{
    _PARSE_PARAM_T  t_parse;
    UINT16          ui2_idx;
    INT32           i4_ret;
    
    t_parse.ui4_ref     = BOOT_DATA_BROADCAST_ID;
    t_parse.ui1_ref_len = (UINT8) 2;
    t_parse.ui1_flag    = FLAG_FIND_REF_DATA | FLAG_CHECK_AP_TYPE_RPIORITY | FLAG_GET_SERVICE_BOOT_INFO;
    t_parse.ui1_mask    = FLAG_FIND_REF_DATA | FLAG_CHECK_AP_TYPE_RPIORITY;

    t_parse.i2_boot_priority_hint = -1;
    
    i4_ret = _search_descriptors_with_data(
        h_pmt, 
        TAG_DATA_BROADCAST_ID_DESCRIPTOR, 
        &t_parse,
        &ui2_idx, 
        pt_pid,
        NULL);

    if (MHEG5R_OK == i4_ret)
    {
        /* if service_boot_info sub-descriptor is found, use the assoTag field as the association tag,
           otherwise returns 0xffff instead */
        *pui2_association_tag = 
            (0 == t_parse.ui1_flag) ? t_parse.ui2_association_tag : (UINT16) MHEG5_LIFE_CYCLE_TAG_NOT_FOUND;
        
        MHEG5_DBG_APP_SI(("DATA_BROADCAST_ID_DESCRIPTOR is found..\n"));

        MHEG5_DBG_APP_SI(("%s\n", (0 == t_parse.ui1_flag) ?"and service_boot_info sub-descriptor is also found.." :"but service_boot_info sub-descriptor is not found.."));

        MHEG5_DBG_APP_SI(("Try to get component tag and carousel id in ESCompIdx:%d, PID:%d\n", ui2_idx, *pt_pid));

        i4_ret = _get_comp_tag_and_carousel_id_desc(
            h_pmt,
            ui2_idx, 
            FLAG_GET_COMP_TAG | FLAG_GET_CAROUSEL_ID_DESC,
            pui1_comp_tag, 
            pui1_carousel_id_desc);

        if (MHEG5R_OK == i4_ret)
        {
            *pui4_carousel_id = (UINT32) GET_UINT32_FROM_PTR_BIG_END((pui1_carousel_id_desc + 2));
            MHEG5_DBG_APP_SI(("OK, found component tag:%d carousel id:%lu\n", *pui1_comp_tag,*pui4_carousel_id));
        }
        else
        {
            MHEG5_DBG_APP_SI(("Oops, not found the component tag and carousel id\n"));
        }
    }
    
    return (i4_ret);
}

static INT32 _get_carousel(
    HANDLE_T        h_pmt,
    UINT32          ui4_carousel_id,
    MPEG_2_PID_T*   pt_pid,
    UINT8*          pui1_comp_tag,
    UINT8*          pui1_carousel_id_desc
    #ifdef MHEG5_HD_SUPPORT
    ,
    UINT16*         pui2_association_tag
    #endif
    )
{
    _PARSE_PARAM_T  t_parse;
    UINT16          ui2_idx;
    INT32           i4_ret;
    
    t_parse.ui4_ref     = ui4_carousel_id;
    t_parse.ui1_ref_len = (UINT8) sizeof(ui4_carousel_id);
    t_parse.ui1_flag    = FLAG_FIND_REF_DATA;
    t_parse.ui1_mask    = FLAG_FIND_REF_DATA;
    
    i4_ret = _search_descriptors_with_data(
        h_pmt, 
        TAG_CAROUSEL_ID_DESCRIPTOR, 
        &t_parse,
        &ui2_idx, 
        pt_pid,
        NULL);

    if (MHEG5R_OK == i4_ret)
    {
        i4_ret = _get_comp_tag_and_carousel_id_desc(
            h_pmt,
            ui2_idx, 
            FLAG_GET_COMP_TAG | FLAG_GET_CAROUSEL_ID_DESC,
            pui1_comp_tag, 
            pui1_carousel_id_desc);

        #ifdef MHEG5_HD_SUPPORT
        if (MHEG5R_OK == i4_ret)
        {
            _parse_service_boot_info_ex(
                h_pmt,
                NULL,
                pui2_association_tag,
                *pt_pid);
        }
        #endif
    }
    
    return (i4_ret);
}

static INT32 _get_comp_tag_and_carousel_id_desc(
    HANDLE_T    h_pmt,
    UINT16      ui2_idx,
    UINT8       ui1_flag,
    UINT8*      pui1_comp_tag,
    UINT8*      pui1_carousel_id_desc)
{
    TM_FIRST_LOOP_DESCR_SEL_T   t_first_loop_descr_sel;
    _PARSE_PARAM_T              t_parse;
    INT32                       i4_ret;
   
    UINT8 aui1_desc_tag[2] = {TAG_STREAM_IDENTIFIER_DESCRIPTOR, TAG_CAROUSEL_ID_DESCRIPTOR};
    
    t_first_loop_descr_sel.ui2_loop_idx        = ui2_idx;
    t_first_loop_descr_sel.pui1_descr_tag_list = &(aui1_desc_tag[0]);
    t_first_loop_descr_sel.z_num_descr_tags    = sizeof(aui1_desc_tag) / sizeof(UINT8);
    
    t_parse.ui1_flag        = ui1_flag;
    t_parse.pui1_descriptor = pui1_carousel_id_desc;
    
    i4_ret = x_tm_parse_obj(
        h_pmt,
        TM_PARSE_TYPE_FIRST_LOOP_DESCRS,
        &t_first_loop_descr_sel,
        &t_parse,
        sizeof(t_parse),
        _parse_comp_tag_and_carousel_id_desc,
        NULL);

    if ((TMR_OK == i4_ret) &&
        (0 == t_parse.ui1_flag))
    {
        *pui1_comp_tag = t_parse.ui1_comp_tag;
        i4_ret = MHEG5R_OK;
    }
    else
    {
        i4_ret = MHEG5R_PARSE_PMT_FAIL;
    }

    return (i4_ret);
}

static INT32 _get_conn_from_svctx(
    HANDLE_T    h_svctx,
    UINT32      ui4_seq_num,
    HANDLE_T*   ph_conn)
{
    INT32       i4_ret;
    
    i4_ret = MHEG5R_CONNECT_FAIL;

    if (SVCTXR_OK == svctx_get_conn_handle(
        h_svctx,
        ST_VIDEO,
        ph_conn))
    {
        i4_ret = _send_notify(
            SIL_MSG_CONNECTED,
            ui4_seq_num,
            *ph_conn,
            0,
            0,
            0,
            NULL);
    }

    return (i4_ret);
}

static INT32 _get_default_stream_pid(
    HANDLE_T                h_pmt,
    _STREAM_ANNOTATION_T*   pt_strm_ant,
    MPEG_2_PID_T*           pt_pid,
    UINT8*                  pui1_enc_type)
{
    PSI_STREAM_INFO_T           t_stream_info;
    TM_COND_T                   e_stream_cond;
    UINT16                      ui2_idx;
    _ISO_639_DESC               t_desc;
    BOOL                        b_found = FALSE;
    LANG_MATCH_STATUS_T         e_best_matched = BOTH_NOT_MATCHED;
    
    ui2_idx = 0;
    
    while (1)
    {
        if (TMR_OK != x_psi_get_stream_entry(
            h_pmt,
            ui2_idx,
            &t_stream_info,
            &e_stream_cond))
        {
            break;
        }
        
        if ((ST_VIDEO == pt_strm_ant->e_strm_type) &&
            (STREAMTYPE_VIDEO_11172_2      == t_stream_info.ui1_stream_type ||
             STREAMTYPE_VIDEO_13818_2      == t_stream_info.ui1_stream_type ||
             STREAMTYPE_VIDEO_DIGICIPHER_2 == t_stream_info.ui1_stream_type ||
             STREAMTYPE_VIDEO_14496_2      == t_stream_info.ui1_stream_type ||
             STREAMTYPE_VIDEO_H264 == t_stream_info.ui1_stream_type))
        {
            if(!b_found)
            {
            b_found = TRUE;
            *pt_pid = t_stream_info.t_pid;
            if (pui1_enc_type)
            {
                *pui1_enc_type = _streamtype_to_vid_enc(t_stream_info.ui1_stream_type);
            }
            }
            
            if(STREAMTYPE_VIDEO_14496_2 == t_stream_info.ui1_stream_type ||
                STREAMTYPE_VIDEO_H264 == t_stream_info.ui1_stream_type)
            {
                *pt_pid = t_stream_info.t_pid;
                if (pui1_enc_type)
                {
                    *pui1_enc_type = _streamtype_to_vid_enc(t_stream_info.ui1_stream_type);
                }
            break;
        }
        }
        else if (
             (ST_AUDIO == pt_strm_ant->e_strm_type) &&
             (STREAMTYPE_AUDIO_11172_3  == t_stream_info.ui1_stream_type ||
              STREAMTYPE_AUDIO_13818_3  == t_stream_info.ui1_stream_type ||
              STREAMTYPE_AUDIO_ATSC_A53 == t_stream_info.ui1_stream_type))
        {
            if(AUD_TYPE_VISUAL_IMPAIRED != pt_strm_ant->e_aud_type)
            {
            if (!b_found)
            {
                b_found = TRUE;
                *pt_pid = t_stream_info.t_pid;
                if (pui1_enc_type)
                {
                    *pui1_enc_type = AUD_ENC_MPEG_2;
                }
            }
            /* look for the ISO639 language descriptor */
            _get_iso_639_language(
                h_pmt, 
                ui2_idx,
                pt_strm_ant,
                &t_desc, 
                sizeof(ISO_639_LANG_T));
        
            if (t_desc.b_lang_found)
            {
                LANG_MATCH_STATUS_T  e_matched;
                
                if (!t_desc.b_gui_lang)
                {
                    e_matched = (t_desc.b_ad_matched)? AUD_MATCHED_WITH_AD: AUD_MATCHED_NO_AD;
                }
                else
                {
                    e_matched = (t_desc.b_ad_matched)? GUI_MATCHED_WITH_AD: GUI_MATCHED_NO_AD;
                }
                
                e_matched += MPEG1_CODEC_WEIGHTING;

                if (e_matched > e_best_matched)
                {
                    e_best_matched  = e_matched;
                    *pt_pid         = t_stream_info.t_pid;
                    if (pui1_enc_type)
                    {
                        *pui1_enc_type = AUD_ENC_MPEG_2;
                        }
                    }
                }
            }
            else
            {   
                _COMP_ISO_639_DESC               t_ad_desc;
                
                _get_audio_description(
                    h_pmt, 
                    ui2_idx,
                    pt_strm_ant,
                    &t_ad_desc, 
                    sizeof(_COMP_ISO_639_DESC));
            
                if (t_ad_desc.b_ad_matched)
                {
                    LANG_MATCH_STATUS_T  e_matched;
                    INT8            ui1_idx;

                    if (!b_found)
                    {
                        b_found = TRUE;
                        *pt_pid = t_stream_info.t_pid;
                        if (pui1_enc_type)
                        {
                            *pui1_enc_type = AUD_ENC_MPEG_2;
                    }
                }

                    for(ui1_idx=0; ui1_idx<t_ad_desc.ui1_index; ui1_idx++)
                    {
                        if(x_strcmp(t_ad_desc.s_lang[ui1_idx], pt_strm_ant->s_lang) == 0)
                        {
                            e_matched = AUD_MATCHED_WITH_AD;
                        }
                        else if(x_strcmp(t_ad_desc.s_lang[ui1_idx], pt_strm_ant->s_gui_lang) == 0)
                        {
                            e_matched = GUI_MATCHED_WITH_AD;
                        }
                        else
                        {
                            e_matched = BOTH_NOT_MATCHED;
                        }

                        if (e_matched > e_best_matched)
                        {
                            e_best_matched  = e_matched;
                            *pt_pid         = t_stream_info.t_pid;

                            if(AUD_MATCHED_WITH_AD == e_best_matched)
                            {
                                break;
                            }
                        }
                            
                    }
                }
            }
        }
        else if ((ST_AUDIO == pt_strm_ant->e_strm_type) &&
                 (STREAMTYPE_PES_PRIV_13818_1 == t_stream_info.ui1_stream_type ||
                  STREAMTYPE_AUDIO_HE_ACC == t_stream_info.ui1_stream_type))
        {
            TM_FIRST_LOOP_DESCR_SEL_T   t_descr;
            AC3_AAC_AUDIO_INFO_T        t_ac3_aac;
            UINT8                       aui1_descriptor_tag[3];

            aui1_descriptor_tag[0] = TAG_AC_3_DESCRIPTOR;
            aui1_descriptor_tag[1] = TAG_EAC_3_DESCRIPTOR;
            aui1_descriptor_tag[2] = TAG_AAC_DESCRIPTOR;
            
            t_descr.ui2_loop_idx        = ui2_idx;
            t_descr.pui1_descr_tag_list = aui1_descriptor_tag;
            t_descr.z_num_descr_tags    = 3;

            x_memset(&t_ac3_aac, 0, sizeof(AC3_AAC_AUDIO_INFO_T));
            
            x_tm_parse_obj(h_pmt,
                           TM_PARSE_TYPE_FIRST_LOOP_DESCRS,
                           &t_descr,
                           &t_ac3_aac,
                           sizeof(AC3_AAC_AUDIO_INFO_T),
                           _parse_ac3_aac_streams,
                           NULL);

            if (t_ac3_aac.b_found == TRUE)
            {
                if(AUD_TYPE_VISUAL_IMPAIRED != pt_strm_ant->e_aud_type)
                {
                if (!b_found)
                {
                    b_found = TRUE;
                    *pt_pid = t_stream_info.t_pid;
                    if (pui1_enc_type)
                    {
                        *pui1_enc_type = t_ac3_aac.e_aud_enc;
                    }
                }
                
                /* look for the ISO639 language descriptor */
                _get_iso_639_language(
                    h_pmt, 
                    ui2_idx,
                    pt_strm_ant,
                    &t_desc, 
                    sizeof(ISO_639_LANG_T));

                if (t_desc.b_lang_found)
                {
                    LANG_MATCH_STATUS_T  e_matched;
                    
                    if (!t_desc.b_gui_lang)
                    {
                        e_matched = (t_desc.b_ad_matched)? AUD_MATCHED_WITH_AD: AUD_MATCHED_NO_AD;
                    }
                    else
                    {
                        e_matched = (t_desc.b_ad_matched)? GUI_MATCHED_WITH_AD: GUI_MATCHED_NO_AD;
                    }
                    e_matched += (t_ac3_aac.t_weighting);
    
                    if (e_matched > e_best_matched)
                    {
                        e_best_matched  = e_matched;
                        *pt_pid         = t_stream_info.t_pid;
                        if (pui1_enc_type)
                            {
                                *pui1_enc_type = t_ac3_aac.e_aud_enc;
                            }
                        }
                    }
                }
                else
                {   
                    _COMP_ISO_639_DESC               t_ad_desc;
                    
                    _get_audio_description(
                        h_pmt, 
                        ui2_idx,
                        pt_strm_ant,
                        &t_ad_desc, 
                        sizeof(_COMP_ISO_639_DESC));
                
                    if (t_ad_desc.b_ad_matched)
                    {
                        LANG_MATCH_STATUS_T  e_matched;
                        INT8            ui1_idx;

                        if (!b_found)
                        {
                            b_found = TRUE;
                            *pt_pid = t_stream_info.t_pid;
                            if (pui1_enc_type)
                            {
                                *pui1_enc_type = t_ac3_aac.e_aud_enc;
                            }
                        }

                        for(ui1_idx=0; ui1_idx<t_ad_desc.ui1_index; ui1_idx++)
                        {
                            if(x_strcmp(t_ad_desc.s_lang[ui1_idx], pt_strm_ant->s_lang) == 0)
                            {
                                e_matched = AUD_MATCHED_WITH_AD;
                            }
                            else if(x_strcmp(t_ad_desc.s_lang[ui1_idx], pt_strm_ant->s_gui_lang) == 0)
                            {
                                e_matched = GUI_MATCHED_WITH_AD;
                            }
                            else
                            {
                                e_matched = BOTH_NOT_MATCHED;
                            }

                            if (e_matched > e_best_matched)
                            {
                                e_best_matched  = e_matched;
                                *pt_pid         = t_stream_info.t_pid;

                                if(AUD_MATCHED_WITH_AD == e_best_matched)
                                {
                                    break;
                                }
                            }
                                
                        }
                    }
                }
            }
        }       
        ui2_idx++;
    }

    return ((b_found) ? MHEG5R_OK : MHEG5R_PARSE_PMT_FAIL);
}

static INT32 _get_deferred_svc(
    HANDLE_T    h_pmt,
    UINT16      ui2_association_tag,
    UINT16*     pui2_ts_id,
    UINT16*     pui2_svc_id,
    UINT16*     pui2_on_id)
{
    INT32                       i4_ret;
    TM_MAIN_BODY_DESCR_SEL_T    t_descr;
    _PARSE_PARAM_T              t_parse;
    UINT8                       ui1_descriptor_tag;
    
    ui1_descriptor_tag = TAG_DEFERRED_ASSOCIATION_TAG_DESCRIPTOR;
    
    t_descr.pui1_descr_tag_list = &ui1_descriptor_tag;
    t_descr.z_num_descr_tags    = 1;

    t_parse.ui4_ref  = ui2_association_tag;
    t_parse.ui1_flag = FLAG_FIND_ASSOCIATION_TAG;
    
    i4_ret = MHEG5R_PARSE_PMT_FAIL;
    
    if (TMR_OK == x_tm_parse_obj(
        h_pmt,
        TM_PARSE_TYPE_MAIN_BODY_DESCRS,
        &t_descr,
        &t_parse,
        sizeof(t_parse),
        _parse_deferred_association_tag,
        NULL))
    {
        if (0 == t_parse.ui1_flag)
        {
            *pui2_ts_id  = t_parse.ui2_ts_id;
            *pui2_svc_id = t_parse.ui2_svc_id;
            *pui2_on_id  = t_parse.ui2_on_id;
            
            i4_ret = MHEG5R_OK;
        }
    }

    return (i4_ret);
}

static VOID _get_iso_639_language(
    HANDLE_T              h_pmt,
    UINT16                ui2_index,
    _STREAM_ANNOTATION_T  *pt_strm_ant,
    _ISO_639_DESC*        pt_desc,
    SIZE_T                z_desc_len)
{
    TM_FIRST_LOOP_DESCR_SEL_T   t_descr;
    UINT8                       ui1_descriptor_tag;
    _GET_LANG_INFO_T            t_lang_info;
    
    ui1_descriptor_tag = TAG_ISO_639_LANG_DESCRIPTOR; /* ISO 639 language descriptor tag */
    
    t_descr.ui2_loop_idx        = ui2_index;
    t_descr.pui1_descr_tag_list = &ui1_descriptor_tag;
    t_descr.z_num_descr_tags    = 1;
    
    pt_desc->s_lang[0]    = '\0';
    pt_desc->e_aud_type   = AUD_TYPE_UNKNOWN;
    pt_desc->b_lang_found = FALSE;
    pt_desc->b_gui_lang   = FALSE;
    pt_desc->b_ad_matched = FALSE;

    t_lang_info.pt_desc     = pt_desc;
    t_lang_info.pt_strm_ant = pt_strm_ant;
    
    x_tm_parse_obj(h_pmt,
                   TM_PARSE_TYPE_FIRST_LOOP_DESCRS,
                   & t_descr,
                   &t_lang_info,
                   z_desc_len,
                   _parse_iso_639_language,
                   NULL);
}

static VOID _get_audio_description(
    HANDLE_T                h_pmt,
    UINT16                  ui2_index,
    _STREAM_ANNOTATION_T   *pt_strm_ant,
    _COMP_ISO_639_DESC*     pt_desc,
    SIZE_T                  z_desc_len)
{
    TM_FIRST_LOOP_DESCR_SEL_T   t_descr;
    UINT8                       ui1_descriptor_tag[2];
    _GET_COMP_LANG_INFO_T       t_lang_info;
    
    ui1_descriptor_tag[0] = TAG_COMP_DESCRIPTOR;
    ui1_descriptor_tag[1] = TAG_ISO_639_LANG_DESCRIPTOR;    
    
    t_descr.ui2_loop_idx        = ui2_index;
    t_descr.pui1_descr_tag_list = ui1_descriptor_tag;
    t_descr.z_num_descr_tags    = 2;
    
    pt_desc->s_lang[0][0]    = '\0';
    pt_desc->ui1_index    = 0;
    pt_desc->e_aud_type   = AUD_TYPE_UNKNOWN;
    pt_desc->b_ad_matched = FALSE;

    t_lang_info.pt_desc     = pt_desc;
    t_lang_info.pt_strm_ant = pt_strm_ant;
    
    x_tm_parse_obj(h_pmt,
                   TM_PARSE_TYPE_FIRST_LOOP_DESCRS,
                   & t_descr,
                   &t_lang_info,
                   z_desc_len,
                   _parse_ad_descriptors,
                   NULL);

}
#define CHECK_PID_LIST_SIZE(id)     (((id) < MAX_PID_LIST_SIZE)? (id): 0)

static INT32 _get_pid_list(
    HANDLE_T    h_pmt,
    UINT16      ui2_sz_pid_lst,
    UINT16*     pui2_num_pid,
    UINT16*     pui2_pid_lst,
    UINT8*      pui1_comp_tag_lst)
{
    PSI_STREAM_INFO_T   t_stream_info;
    TM_COND_T           e_stream_cond;
    INT32               i4_ret;
    UINT16              ui2_idx = 0;
    UINT16              ui2_cnt = 0;
    
    while (1)
    {
        i4_ret = x_psi_get_stream_entry(
            h_pmt,
            ui2_idx,
            &t_stream_info,
            &e_stream_cond);

        if (i4_ret != TMR_OK)
        {
            i4_ret = MHEG5R_OK;
            break;
        }

        if ((STREAMTYPE_DSMCC_UN_MESSAGE        == t_stream_info.ui1_stream_type) ||
            (STREAMTYPE_DSMCC_STREAM_DESCRIPTOR == t_stream_info.ui1_stream_type) ||
            (STREAMTYPE_DSMCC_SECTION           == t_stream_info.ui1_stream_type))
        {
            UINT8 ui1_comp_tag;
            
            i4_ret = _get_comp_tag_and_carousel_id_desc(
                h_pmt,
                ui2_idx,
                FLAG_GET_COMP_TAG,
                &ui1_comp_tag,
                NULL);

            if (MHEG5R_OK == i4_ret)
            {
                pui2_pid_lst[CHECK_PID_LIST_SIZE(ui2_cnt)]      = (UINT16) t_stream_info.t_pid;
                pui1_comp_tag_lst[CHECK_PID_LIST_SIZE(ui2_cnt)] = (UINT8)  ui1_comp_tag;

                if (++ui2_cnt >= ui2_sz_pid_lst)
                {
                    break;
                }
            }
        }
        
        ui2_idx++;
    }

    return (i4_ret);
}

#if 0
static INT32 _get_running_status(
    HANDLE_T    h_sdt,
    UINT16      ui2_svc_id,
    UINT8*      pui1_status)
{
    INT32                       i4_ret  = MHEG5R_SVC_UNAVAILABLE;
    UINT16                      ui2_idx = 0;
    UINT16                      i;
    TM_COND_T                   e_cond;
    DVB_SI_SVC_ENTRY_INFO_T     t_dvb_si_entry;
    
    if (TMR_OK == x_dvb_si_get_num_svc_entries(
        h_sdt,
        &ui2_idx,
        &e_cond))
    {
        for (i = 0; i < ui2_idx; i++)
        {
            if (TMR_OK == x_dvb_si_get_svc_entry(
                h_sdt, 
                i, 
                &t_dvb_si_entry,
                &e_cond))
            {
                if (ui2_svc_id == t_dvb_si_entry.ui2_svc_id)
                {
                    *pui1_status = t_dvb_si_entry.ui1_flags & SVC_MASK_RUNNING_STATUS;
                    i4_ret = MHEG5R_OK;
                    break;
                }
            }
        }
    }

    return (i4_ret);
}
#endif

static INT32 _get_stream_pid(
    HANDLE_T        h_pmt,
    UINT16          ui2_association_tag,
    MPEG_2_PID_T*   pt_pid,
    UINT8*          pui1_enc_type)
{
    _PARSE_PARAM_T  t_parse;
    INT32           i4_ret;
    
    t_parse.ui4_ref     = ui2_association_tag;
    t_parse.ui1_ref_len = (UINT8) sizeof(ui2_association_tag);
    t_parse.ui1_flag    = FLAG_FIND_REF_DATA;
    t_parse.ui1_mask    = FLAG_FIND_REF_DATA;
    
    i4_ret = _search_descriptors_with_data(
        h_pmt, 
        TAG_ASSOCIATION_TAG_DESCRIPTOR, 
        &t_parse,
        NULL, 
        pt_pid,
        pui1_enc_type);

    if (MHEG5R_OK != i4_ret)
    {
        t_parse.ui4_ref     = (ui2_association_tag & 0xff);
        t_parse.ui1_ref_len = 1;
        t_parse.ui1_flag    = FLAG_FIND_REF_DATA;
        
        i4_ret = _search_descriptors_with_data(
            h_pmt, 
            TAG_STREAM_IDENTIFIER_DESCRIPTOR, 
            &t_parse,
            NULL, 
            pt_pid,
            pui1_enc_type);
    }
    
    return (i4_ret);
}

static BOOL _get_svc_indicator(
    _SI_LOADER_INFO_T*  pt_loader,
    UINT16              ui2_svl_id, 
    UINT16              ui2_svl_rec_id,
    UINT16*             pui2_on_id,
    UINT16*             pui2_ts_id,
    UINT16*             pui2_svc_id)
{
    HANDLE_T    h_t_svl = NULL_HANDLE;
    HANDLE_T    h_t_tsl = NULL_HANDLE;
    HANDLE_T    h_tsl   = NULL_HANDLE;
    HANDLE_T    h_svl   = NULL_HANDLE;
    SVL_REC_T   t_svl_rec;
    TSL_REC_T   t_tsl_rec;
    UINT32      ui4_ver_id;
    UINT16      ui2_tsl_id = 0;
    BOOL        b_ret = TRUE;
    
    if (ui2_svl_id != pt_loader->ui2_svl_id)
    {
        h_t_svl = pt_loader->h_svl;
        
        /* open SVL */
        if (SVLR_OK != x_svl_open(
            ui2_svl_id,
            NULL, 
            NULL, 
            &h_svl))
        {
            return FALSE;
        }
    }
    else
    {
        h_svl = pt_loader->h_svl;
    }

    do 
    {
        ui4_ver_id = SVL_NULL_VER_ID;
            
        if (SVLR_OK == x_svl_get_rec(
            h_svl,
            ui2_svl_rec_id,
            &t_svl_rec,
            &ui4_ver_id))
        {
            ui2_tsl_id = t_svl_rec.uheader.t_rec_hdr.ui2_tsl_id;
            
            if (ui2_tsl_id != pt_loader->ui2_tsl_id)
            {
                if (TSLR_OK == x_tsl_open(
                    ui2_tsl_id,
                    NULL,
                    NULL,
                    &h_tsl))
                {
                    h_t_tsl = pt_loader->h_tsl;
                }
                else
                {
                    b_ret = FALSE;
                    break;
                }
            }
            else
            {
                h_tsl = pt_loader->h_tsl;
            }

            if (TSLR_OK == x_tsl_get_rec(
                h_tsl, 
                t_svl_rec.uheader.t_rec_hdr.ui2_tsl_rec_id, 
                &t_tsl_rec, 
                &ui4_ver_id))
            {
                *pui2_on_id  = t_tsl_rec.uheader.t_desc.ui2_on_id;
                *pui2_ts_id  = t_tsl_rec.uheader.t_desc.ui2_ts_id;
                *pui2_svc_id = t_svl_rec.uheader.t_rec_hdr.ui2_prog_id;
            }
            else
            {
                b_ret = FALSE;
                break;
            }
        }
    } while (0);  
    
    if (NULL_HANDLE != h_t_svl)
    {
        x_svl_close(h_t_svl);
    }

    if (NULL_HANDLE != h_svl)
    {
        pt_loader->h_svl          = h_svl;
        pt_loader->ui2_svl_id     = ui2_svl_id;
        pt_loader->ui2_svl_rec_id = ui2_svl_rec_id;
    }
    
    if (NULL_HANDLE != h_t_tsl)
    {
        x_tsl_close(h_t_tsl);
    }

    if (NULL_HANDLE != h_tsl)
    {
        pt_loader->h_tsl          = h_tsl;
        pt_loader->ui2_tsl_id     = ui2_tsl_id;
        pt_loader->ui2_tsl_rec_id = t_svl_rec.uheader.t_rec_hdr.ui2_tsl_rec_id;
    }
    
    return (b_ret);
}

static INT32 _handle_msg_connected(
    MHEG5_MSG_T*        pt_msg,
    _SI_LOADER_INFO_T*  pt_loader)
{
    if (pt_msg->ui4_seq_num == pt_loader->ui4_seq_num)
    {
        pt_loader->e_state = LDR_ST_LOADING_PERSISTENT_TBL;
        _t_loader.ui4_tbl_status = PERSISTENT_TBL_MASK;

#if defined(SYS_MHEG5_HK_SUPPORT) || defined(SYS_MHEG5_AUS_SUPPORT)
        if (MHEG5R_OK == _load_sdt(
            _t_loader.h_root,
            _t_loader.ui2_on_id,
            _t_loader.ui2_ts_id,
            _t_loader.ui4_seq_num,
            &(_t_loader.h_sdt_actual)))
        {
            _t_loader.ui4_tbl_status &= ~BIT_SDT_ACTUAL;
#endif
           SI_DEBUG(("start load_pat\r\n"));
            if (MHEG5R_OK == _load_pat(
                _t_loader.h_tsl,
                _t_loader.ui2_tsl_rec_id,
                _t_loader.h_root,
                _t_loader.ui4_seq_num,
                &(_t_loader.h_pat)))
            {
                SI_DEBUG(("start load_pat command send OK\r\n"));
                _t_loader.ui4_tbl_status &= ~BIT_PAT;
            #if 0
                _load_nit(
                    _t_loader.h_root,
                    _t_loader.ui4_seq_num,
                    &(_t_loader.h_nit));
            #endif
            }
#if defined(SYS_MHEG5_HK_SUPPORT) || defined(SYS_MHEG5_AUS_SUPPORT)
        }
        else
        {
            _t_loader.e_state = LDR_ST_CONNECTED;
            _notify_load_status(&_t_loader, MHEG5_NFY_COND_TABLE_FAILED);
        }
#endif
    }

    return MHEG5R_OK;
}

static INT32 _handle_msg_nit_timeout(
    MHEG5_MSG_T*        pt_msg, 
    _SI_LOADER_INFO_T*  pt_loader)
{
    /* free the attached tag */
    if (pt_msg->pv_nfy_tag)
    {
        _SIL_TIMER_T* pt_timer = (_SIL_TIMER_T*) (pt_msg->pv_nfy_tag);
        
        x_timer_delete(pt_timer->h_this);
        SIL_MFREE(pt_timer, sizeof(_SIL_TIMER_T));
    }
    
    if (pt_msg->ui4_seq_num == pt_loader->ui4_seq_num)
    {
        /* free NIT */
        _free_si_table(pt_msg->h_obj);
        pt_loader->h_nit = NULL_HANDLE;
    }

    return MHEG5R_OK;
}

static INT32 _handle_msg_nit_update(
    MHEG5_MSG_T*        pt_msg, 
    _SI_LOADER_INFO_T*  pt_loader)
{
    INT32 i4_ret = MHEG5R_OK;
    
    if (pt_msg->ui4_seq_num == pt_loader->ui4_seq_num)
    {
        DVB_SI_ON_TS_ID_INFO_T  t_ts_info;
        UINT16                  ui2_idx = 0;
        TM_COND_T               e_cond;
        _SDT_INFO_T*            pt_sdt_info;
        SLIST_SDT_INFO_T        t_backup_lst;

        MHEG5_DBG_ERROR(("{MHEG5 Stack} NIT update arrives.\r\n"));

        SLIST_INIT(&t_backup_lst);
        
        while (TMR_OK == x_dvb_si_get_ts_entry(
            pt_msg->h_obj, 
            ui2_idx,
            &t_ts_info, 
            &e_cond))
        {
            if ((pt_loader->ui2_on_id != t_ts_info.ui2_on_id) ||
                (pt_loader->ui2_ts_id != t_ts_info.ui2_ts_id))
            {
                pt_sdt_info = _find_sdt_info(t_ts_info.ui2_on_id, t_ts_info.ui2_ts_id);

                if (pt_sdt_info)
                {
                    SLIST_REMOVE(pt_sdt_info, t_link);
                    SLIST_INSERT_HEAD(pt_sdt_info, &t_backup_lst, t_link);
                }
                else
                {
                    pt_sdt_info = (_SDT_INFO_T*) SIL_MALLOC(sizeof(_SDT_INFO_T));

                    if (pt_sdt_info)
                    {
                        /* load SDT_other */
                        i4_ret = _load_sdt(
                            pt_loader->h_root,
                            t_ts_info.ui2_on_id,
                            t_ts_info.ui2_ts_id,
                            SEQ_NUM(pt_loader),
                            &(pt_sdt_info->h_sdt));

                        if (MHEG5R_OK == i4_ret)
                        {
                            pt_sdt_info->b_loaded  = FALSE;
                            pt_sdt_info->ui2_on_id = t_ts_info.ui2_on_id;
                            pt_sdt_info->ui2_ts_id = t_ts_info.ui2_ts_id;
                            SLIST_INSERT_HEAD(pt_sdt_info, &t_backup_lst, t_link);
                        }
                        else
                        {
                            MHEG5_DBG_ERROR((
                                "{MHEG5 Stack} %s(L%d): Load SDT other failed (on_id = %d, ts_id = %d).\r\n",
                                __FUNCTION__,
                                __LINE__,
                                t_ts_info.ui2_on_id,
                                t_ts_info.ui2_ts_id));
                            
                            SIL_MFREE(pt_sdt_info, sizeof(_SDT_INFO_T));
                        }
                    }
                    else
                    {
                        MHEG5_DBG_ERROR((
                            "{MHEG5 Stack} %s(L%d): Allocate memory failed.\r\n",
                            __FUNCTION__,
                            __LINE__));
                    }
                }
            }

            ui2_idx++;
        }

        _free_sdt_info_lst(&(SDT_OTHER_LIST(pt_loader)));

        /* copy the list back */
        while (1)
        {
            pt_sdt_info = SLIST_FIRST(&t_backup_lst);

            if (pt_sdt_info)
            {
                SLIST_REMOVE(pt_sdt_info, t_link);
                SLIST_INSERT_HEAD(pt_sdt_info, &(SDT_OTHER_LIST(pt_loader)), t_link);
            }
            else
            {
                break;
            }
        }
    }

    return MHEG5R_OK;
}

static INT32 _handle_msg_sdt_timeout(
    MHEG5_MSG_T*        pt_msg,
    _SI_LOADER_INFO_T*  pt_loader)
{
    /* free the attached tag */
    if (pt_msg->pv_nfy_tag)
    {
        _SIL_TIMER_T* pt_timer = (_SIL_TIMER_T*) (pt_msg->pv_nfy_tag);
        
        x_timer_delete(pt_timer->h_this);
        SIL_MFREE(pt_timer, sizeof(_SIL_TIMER_T));
    }

    if (pt_msg->ui4_seq_num == pt_loader->ui4_seq_num)
    {
        if ((pt_msg->ui2_on_id == pt_loader->ui2_on_id) &&
            (pt_msg->ui2_ts_id == pt_loader->ui2_ts_id))
        {
            if (!SDT_ACTUAL_DONE(pt_loader->ui4_tbl_status))
            {
                pt_loader->ui4_tbl_status |= BIT_SDT_ACTUAL;

                /* free SDT */
                _free_si_table(pt_msg->h_obj);
                pt_loader->h_sdt_actual = NULL_HANDLE;
                
                if (PERSISTENT_TBL_DONE(pt_loader->ui4_tbl_status))
                {
                    pt_loader->e_state = LDR_ST_CONNECTED;
                    _notify_load_status(pt_loader, MHEG5_NFY_COND_TABLE_FAILED);
                }
            }
        }
        else
        {
            _SDT_INFO_T* pt_sdt_info;

            pt_sdt_info = _find_sdt_info(pt_msg->ui2_on_id, pt_msg->ui2_ts_id);
            
            /* free SDT */
            if (pt_sdt_info)
            {
                if (!pt_sdt_info->b_loaded)
                {
                    _free_sdt_info(pt_sdt_info);
                }
            }
        }
    }

    return MHEG5R_OK;
}

static INT32 _handle_msg_sdt_update(
    MHEG5_MSG_T*        pt_msg,
    _SI_LOADER_INFO_T*  pt_loader)
{
    if (pt_msg->ui4_seq_num == pt_loader->ui4_seq_num)
    {
        if ((pt_msg->ui2_on_id == pt_loader->ui2_on_id) &&
            (pt_msg->ui2_ts_id == pt_loader->ui2_ts_id))
        {
            if (SDT_ACTUAL_DONE(pt_loader->ui4_tbl_status))
            {
                MHEG5_DBG_ERROR(("{MHEG5 Stack} SDT autual update (change notify) arrives\r\n"));
                
                /* handle SDT update */
                _check_svc_subscription_by_sdt(
                    pt_msg->h_obj,
                    &(SVC_INFO_LIST(&_t_loader)));
            }
            else
            {
                MHEG5_DBG_ERROR(("{MHEG5 Stack} SDT actual update (first notify) arrives.\r\n"));

                /* handle SDT available (first notify) */
                _t_loader.ui4_tbl_status |= BIT_SDT_ACTUAL;

                if (PERSISTENT_TBL_DONE(_t_loader.ui4_tbl_status))
                {
                    BOOL b_ok = _check_persistent_tbl_validity(&_t_loader);

                    _t_loader.e_state = (b_ok) ? LDR_ST_PERSISTENT_TBL_LOADED : LDR_ST_CONNECTED;
                    _notify_load_status(
                        &_t_loader, 
                        (b_ok ? MHEG5_NFY_COND_TABLE_LOADED : MHEG5_NFY_COND_TABLE_FAILED));
                }
            }
        }
        else
        {
            _SDT_INFO_T* pt_sdt_info;

            pt_sdt_info = _find_sdt_info(pt_msg->ui2_on_id, pt_msg->ui2_ts_id);
            
            /* free SDT */
            if (pt_sdt_info)
            {
                pt_sdt_info->b_loaded = TRUE;
            }
        }
    }

    return MHEG5R_OK;
}

static INT32 _handle_query(
    ptmMHEG5SiQuery_t       pt_in_query,
    VOID*                   pv_query_ref,
    VOID*                   pv_usr_data,
    ptmMHEG5SiQueryResult_t pt_qry_res,
    _STREAM_ANNOTATION_T*   pt_strm_ant,
    HANDLE_T*               ph_query,
    MPEG_2_PID_T*           pt_pcr_pid,
    UINT8*                  pui1_enc_type)
{
    INT32   i4_ret;
    BOOL    b_pmt_loading;
    
    i4_ret = _check_svc_lst_for_query(
        pt_in_query,
        &(SVC_INFO_LIST(&_t_loader)),
        pt_qry_res,
        pt_strm_ant,
        &b_pmt_loading,
        pt_pcr_pid,
        pui1_enc_type);

    if (ph_query)
    {
        *ph_query = NULL_HANDLE;
    }
    
    if (MHEG5R_ASYNC_RESPONSE == i4_ret)
    {
        i4_ret = _add_query(
            pt_in_query,
            pv_query_ref,
            pv_usr_data,
            &(SI_QUERY_LIST(&_t_loader)),
            pt_strm_ant,
            !b_pmt_loading,
            ph_query);

        if (MHEG5R_OK == i4_ret)
        {
            i4_ret = MHEG5R_ASYNC_RESPONSE;
        }
    }

    return (i4_ret);
}

#if 0
static BOOL _load_nit(
    HANDLE_T    h_root, 
    UINT32      ui4_seq_num,
    HANDLE_T*   ph_nit)
{
    TM_COND_T       e_cond;
    _SIL_NOTIFY_T*  pt_nfy;
    HANDLE_T        h_nit  = NULL_HANDLE;
    INT32           i4_ret = MHEG5R_OUT_OF_MEMORY;
    
    pt_nfy = SIL_MALLOC(sizeof(_SIL_NOTIFY_T));

    if (pt_nfy)
    {
        i4_ret = MHEG5R_LOAD_NIT_FAIL;

        pt_nfy->ui4_seq_num = ui4_seq_num;
    
        if (TMR_OK == x_dvb_si_load_nit_actual(
            h_root,
            pt_nfy,
            _tm_load_nit_nfy,
            &h_nit,
            &e_cond))
        {
            /* only handle two possible conditions: AVAILABLE and TRANSITION */
            if (TM_COND_AVAILABLE == e_cond)
            {
                i4_ret = _send_notify(
                    SIL_MSG_SDT_UPDATE, 
                    SEQ_NUM(&_t_loader), 
                    h_nit, 
                    0, 
                    0, 
                    0, 
                    NULL);
            }
            else if (TM_COND_TRANSITION == e_cond)
            {
                i4_ret = _start_timer(
                    SIL_MSG_NIT_TIMEOUT,
                    ui4_seq_num,
                    h_nit,
                    0, 
                    0, 
                    0,
                    MHEG5_TIMEOUT_LOAD_SDT);
            }
            else
            {
                MHEG5_ASSERT(0);
                i4_ret = MHEG5R_INTERNAL_ERROR;
            }
        }
    }

    if (MHEG5R_OK == i4_ret)
    {
        *ph_nit = h_nit;
    }
    else
    {
        _free_si_table(h_nit);
    }
        
    return (i4_ret);
}
#endif

static BOOL _load_pat(
    HANDLE_T    h_tsl,
    UINT16      ui2_tsl_rec_id,    
    HANDLE_T    h_root, 
    UINT32      ui4_seq_num,
    HANDLE_T*   ph_pat)
{
    TM_COND_T       e_cond;
    TSL_REC_T       t_tsl_rec;
    UINT32          ui4_ver_id;
    _SIL_NOTIFY_T*  pt_nfy;
    HANDLE_T        h_pat  = NULL_HANDLE;
    INT32           i4_ret = MHEG5R_OUT_OF_MEMORY;
    
    pt_nfy = SIL_MALLOC(sizeof(_SIL_NOTIFY_T));

    if (pt_nfy)
    {
        i4_ret = MHEG5R_INTERNAL_ERROR;

        pt_nfy->ui4_seq_num = ui4_seq_num;
    
        if (TSLR_OK == x_tsl_get_rec(
            h_tsl,
            ui2_tsl_rec_id,
            &t_tsl_rec,
            &ui4_ver_id))
        {
            i4_ret = MHEG5R_LOAD_PAT_FAIL;
            
            if (TMR_OK == x_psi_load_pat_by_ts_descr(
                h_root,
                &(t_tsl_rec.uheader.t_desc),
                TM_SRC_TYPE_SAME_AS_ROOT,
                NULL,
                pt_nfy,
                _tm_load_pat_nfy,
                &h_pat,
                &e_cond))
            {

                /* only handle two possible conditions: AVAILABLE and TRANSITION */
                if (TM_COND_AVAILABLE == e_cond)
                {

                    SI_DEBUG(("load_pat status TM_COND_AVAILABLE\r\n"));
                    i4_ret = _send_notify(
                        SIL_MSG_PAT_UPDATE, 
                        SEQ_NUM(&_t_loader), 
                        h_pat, 
                        0, 
                        0, 
                        0, 
                        NULL);
                }
                else if (TM_COND_TRANSITION == e_cond)
                {
                    SI_DEBUG(("load_pat status TM_COND_TRANSITION start time %d\r\n",MHEG5_TIMEOUT_LOAD_PAT));
                    i4_ret = _start_timer(
                        SIL_MSG_PAT_TIMEOUT,
                        ui4_seq_num,
                        h_pat,
                        0,
                        0,
                        0,
                        MHEG5_TIMEOUT_LOAD_PAT);
                }
                else
                {
                    SI_DEBUG(("load_pat status %d\r\n",e_cond));
                    MHEG5_ASSERT(0);
                    i4_ret = MHEG5R_INTERNAL_ERROR;
                }
            }
        }
    }

    if (MHEG5R_OK == i4_ret)
    {
        *ph_pat = h_pat;
    }
    else
    {
        _free_si_table(h_pat);
    }
        
    return (i4_ret);
}

static INT32 _load_pmt(
    HANDLE_T    h_pat,
    UINT16      ui2_svc_id,
    UINT32      ui4_seq_num,
    HANDLE_T*   ph_pmt)
{
    TM_COND_T       e_cond;
    _SIL_NOTIFY_T*  pt_nfy;
    HANDLE_T        h_pmt  = NULL_HANDLE;
    INT32           i4_ret = MHEG5R_OUT_OF_MEMORY;
    
    pt_nfy = SIL_MALLOC(sizeof(_SIL_NOTIFY_T));

    if (pt_nfy)
    {
        i4_ret = MHEG5R_LOAD_PMT_FAIL;
        
        pt_nfy->ui2_svc_id  = ui2_svc_id;
        pt_nfy->ui4_seq_num = ui4_seq_num;
        
        SI_DEBUG(("x_psi_load_pmt_by_svc_id \r\n"));
        i4_ret = x_psi_load_pmt_by_svc_id(
            h_pat,
            ui2_svc_id,
            TM_SRC_TYPE_SAME_AS_ROOT,
            NULL,
            pt_nfy,
            _tm_load_pmt_nfy,
            &h_pmt,
            &e_cond);
        
        if (TMR_OK == i4_ret)
        {
            /* only handle two possible conditions: AVAILABLE and TRANSITION */
            if (TM_COND_AVAILABLE == e_cond)
            {
                SI_DEBUG(("load_pmt TM_COND_AVAILABLE\r\n"));
                i4_ret = _send_notify(
                    SIL_MSG_PMT_UPDATE, 
                    SEQ_NUM(&_t_loader), 
                    h_pmt,
                    0,
                    0,
                    ui2_svc_id, 
                    NULL);
            }
            else if (TM_COND_TRANSITION == e_cond)
            {
                SI_DEBUG(("load_pmt TM_COND_TRANSITION start time %d\r\n",MHEG5_TIMEOUT_LOAD_PMT));
                i4_ret = _start_timer(
                    SIL_MSG_PMT_TIMEOUT,
                    ui4_seq_num,
                    h_pmt,
                    0,
                    0,
                    ui2_svc_id,
                    MHEG5_TIMEOUT_LOAD_PMT);
            }
            else
            {
                MHEG5_ASSERT(0);
                i4_ret = MHEG5R_INTERNAL_ERROR;
            }
        }
        else if (TMR_OUT_OF_RESOURCES == i4_ret)
        {
            i4_ret = MHEG5R_OUT_OF_RESOURCES;
        }
        else
        {
            i4_ret = MHEG5R_LOAD_PMT_FAIL;
        }
    }
    
    if (MHEG5R_OK == i4_ret)
    {
        *ph_pmt = h_pmt;
    }
    else
    {
        _free_si_table(h_pmt);
    }
    
    return (i4_ret);
}

static BOOL _load_sdt(
    HANDLE_T    h_root, 
    UINT16      ui2_on_id,
    UINT16      ui2_ts_id,
    UINT32      ui4_seq_num,
    HANDLE_T*   ph_sdt)
{
    TM_COND_T       e_cond;
    _SIL_NOTIFY_T*  pt_nfy;
    HANDLE_T        h_sdt  = NULL_HANDLE;
    INT32           i4_ret = MHEG5R_OUT_OF_MEMORY;
    
    pt_nfy = SIL_MALLOC(sizeof(_SIL_NOTIFY_T));

    if (pt_nfy)
    {
        i4_ret = MHEG5R_LOAD_SDT_FAIL;

        pt_nfy->ui2_on_id   = ui2_on_id;
        pt_nfy->ui2_ts_id   = ui2_ts_id;
        pt_nfy->ui4_seq_num = ui4_seq_num;
#if  0  
        if (TMR_OK == x_dvb_si_load_sdt(
            h_root,
            ui2_on_id,
            ui2_ts_id,
            pt_nfy,
            _tm_load_sdt_nfy,
            &h_sdt,
            &e_cond))
#else
        if(TMR_OK == x_dvb_si_load_sdt_actual(
            h_root,
            pt_nfy,
            _tm_load_sdt_nfy,
            &h_sdt,
            &e_cond))
#endif            
        {
            /* only handle two possible conditions: AVAILABLE and TRANSITION */
            if (TM_COND_AVAILABLE == e_cond)
            {
                i4_ret = _send_notify(
                    SIL_MSG_SDT_UPDATE, 
                    SEQ_NUM(&_t_loader), 
                    h_sdt, 
                    ui2_on_id, 
                    ui2_ts_id, 
                    0, 
                    NULL);
            }
            else if (TM_COND_TRANSITION == e_cond)
            {
                i4_ret = _start_timer(
                    SIL_MSG_SDT_TIMEOUT,
                    ui4_seq_num,
                    h_sdt,
                    ui2_on_id, 
                    ui2_ts_id, 
                    0,
                    MHEG5_TIMEOUT_LOAD_SDT);
            }
            else
            {
                MHEG5_ASSERT(0);
                i4_ret = MHEG5R_INTERNAL_ERROR;
            }
        }
    }

    if (MHEG5R_OK == i4_ret)
    {
        HANDLE_TYPE_T ui2_sdt_hdl_type = 0;
        VOID *pui4_sdt_obj = NULL;
    
        handle_get_type_obj(h_sdt, &ui2_sdt_hdl_type,& pui4_sdt_obj);
    
        x_dbg_stmt("[MHEG5-SIL] INFO: %s#%d: loadind SDT info: handle: %d, type: %d, obj: %p \r\n", 
                                            __FUNCTION__, __LINE__, h_sdt, ui2_sdt_hdl_type, pui4_sdt_obj );
        *ph_sdt = h_sdt;
    }
    else
    {
        _free_si_table(h_sdt);
    }
        
    return (i4_ret);
}

static _SVC_INFO_T* _create_svc_info_entry(
    HANDLE_T    h_pat,
    UINT16      ui2_svc_id,
    UINT32      ui4_seq_num,
    BOOL        b_subscribed)
{
    _SVC_INFO_T*    pt_svc_info;
    INT32           i4_ret;
    
    i4_ret = MHEG5R_OUT_OF_MEMORY;

    pt_svc_info = SIL_MALLOC(sizeof(_SVC_INFO_T));

    if (pt_svc_info)
    {
        while (1)
        {
            SI_DEBUG(("start load_pmt\r\n"));
            i4_ret = _load_pmt(
                h_pat,
                ui2_svc_id,
                ui4_seq_num,
                &(pt_svc_info->h_pmt));

            if (MHEG5R_OUT_OF_RESOURCES == i4_ret)
            {
                SI_DEBUG(("load_pmt MHEG5R_OUT_OF_RESOURCES\r\n"));

                if (_release_svc_info_resource(
                    _t_loader.ui2_active_svc_id,
                    &(SVC_INFO_LIST(&_t_loader))))
                {
                    continue;
                }
            }

            break;
        }
        
        if (MHEG5R_OK == i4_ret)
        {
            pt_svc_info->ui2_svc_id = ui2_svc_id;
            pt_svc_info->ui1_status = (b_subscribed ? SVC_INFO_STATUS_SUBSCRIBED : 0);
            
            SLIST_INSERT_HEAD(
                pt_svc_info, 
                &(SVC_INFO_LIST(&_t_loader)), 
                t_link);
        }
        else
        {
            SIL_MFREE(pt_svc_info, sizeof(_SVC_INFO_T));
            pt_svc_info = NULL;
        }
    }

    return (pt_svc_info);
}

static VOID _notify_load_status(
    _SI_LOADER_INFO_T*  pt_loader, 
    MHEG5_NFY_COND_T    e_cond)
{
    if (pt_loader->pf_load_nfy)
    {
        (*(pt_loader->pf_load_nfy))(
            e_cond,
            pt_loader->pv_load_tag);
    }
}

static INT32 _open_pipe(
    const CHAR* ps_tuner_name,
    UINT32      ui4_seq_num,
    HANDLE_T*   ph_pipe)
{
    CM_COND_T       e_cond;
    CM_COMMAND_T    at_cmds[3]; /* command arrays */
    _SIL_NOTIFY_T*  pt_nfy = NULL;
    HANDLE_T        h_pipe = NULL_HANDLE;
    INT32           i4_ret = SIL_FAILED;

    pt_nfy = (_SIL_NOTIFY_T*) SIL_MALLOC(sizeof(_SIL_NOTIFY_T));

    if (pt_nfy)
    {
        x_memset(pt_nfy, 0, sizeof(_SIL_NOTIFY_T));

        pt_nfy->ui4_seq_num = ui4_seq_num;
        
        at_cmds[0].e_code    = CONN_SRC_TYPE;
        at_cmds[0].u.ps_name = BRDCST_HANDLER_DEFAULT_NAME;
        at_cmds[1].e_code    = CONN_SRC_NAME;
        at_cmds[1].u.ps_name = ps_tuner_name;
        at_cmds[2].e_code    = CONN_END;

        x_sema_lock(_h_signal, X_SEMA_OPTION_WAIT);
               
        if (x_cm_open_pipe(
            at_cmds, 
            0,  /* priority */
            0,  /* flag */
            (VOID*) pt_nfy,
            _cm_open_pipe_nfy,
            &h_pipe,
            &e_cond) == CMR_OK)
        {
            if (CM_COND_OPENED == e_cond)
            {
                *ph_pipe = h_pipe;
                i4_ret = SIL_SUCCEEDED;
            }
            else if (CM_COND_OPENING == e_cond)
            {
                x_sema_lock(_h_signal, X_SEMA_OPTION_WAIT);
                
                *ph_pipe = h_pipe;
                i4_ret = SIL_ASYNC_NOTIFY;
            }
            else
            {
                ;
            }
        }
        
        x_sema_unlock(_h_signal);
    }

    if (SIL_FAILED == i4_ret)
    {
        if (pt_nfy)
        {
            SIL_MFREE(pt_nfy, sizeof(_SIL_NOTIFY_T));
        }

        if (NULL_HANDLE != h_pipe)
        {
            x_cm_close_pipe(h_pipe, &e_cond);
        }
    }
    
    return (i4_ret);
}

static BOOL _open_tbl_root(
    const CHAR* ps_tuner_name,
    HANDLE_T*   ph_root)
{
    BOOL        b_ret = FALSE;
    TM_COND_T   e_cond;
    CHAR        s_root_name[TM_ROOT_NAME_LEN];
    SIZE_T      z_name, z_prefix;

    /* create a unique TM root name here */
    z_name   = x_strlen(ps_tuner_name);
    z_prefix = x_strlen(MHEG5_ROOT_NAME_PREFIX);
    
    if ((z_name + z_prefix) >= TM_ROOT_NAME_LEN)
    {
        z_name = TM_ROOT_NAME_LEN - z_prefix - 1;
    }

    x_strcpy(s_root_name, MHEG5_ROOT_NAME_PREFIX);
    
    x_memcpy(
        &(s_root_name[z_prefix]),
        ps_tuner_name,
        z_name);
    
    s_root_name[z_prefix + z_name] = '\0';

    *ph_root = NULL_HANDLE;
    
    /* open table root */
    if (x_tm_open_root(
        s_root_name,
        TM_SRC_TYPE_MPEG_2_BRDCST,
        (VOID*) ps_tuner_name,
        NULL,
        NULL,
        ph_root,
        &e_cond) == TMR_OK)
    {
        if (TM_COND_AVAILABLE == e_cond)
        {
            b_ret = TRUE;
        }
        else
        {
            x_tm_free(*ph_root);
            *ph_root = NULL_HANDLE;
        }
    }
    
    return (b_ret);
}

static PARSE_RET_T _parse_comp_tag_and_carousel_id_desc(
    HANDLE_T      h_pmt,
    const UINT8*  pui1_obj_data,
    SIZE_T        z_obj_data_len,
    UINT16        ui2_count,
    VOID*         pv_data,
    SIZE_T        z_data_len)
{
    _PARSE_PARAM_T* pt_parse     = (_PARSE_PARAM_T*) pv_data;
    PARSE_RET_T     t_ret        = PARSE_RET_NEXT;

    if (VALID_DESCRIPTOR_LEN(2))
    {
        switch (pui1_obj_data[0])
        {
            case TAG_STREAM_IDENTIFIER_DESCRIPTOR:
                if (VALID_DESCRIPTOR_LEN(3))
                {
                    if (pt_parse->ui1_flag & FLAG_GET_COMP_TAG)
                    {
                        pt_parse->ui1_comp_tag = (UINT8) pui1_obj_data[2];
                        pt_parse->ui1_flag    &= ~FLAG_GET_COMP_TAG;
                    }
                }
                break;

            case TAG_CAROUSEL_ID_DESCRIPTOR:
                if (pt_parse->ui1_flag & FLAG_GET_CAROUSEL_ID_DESC)
                {
                    if (VALID_DESCRIPTOR_LEN((SIZE_T)(pui1_obj_data[1] + 2)))
                    {
                    #ifdef DEBUG
                        {
                            INT32 len;
                            
                            MHEG5_DBG_ERROR(("{MHEG5 Stack} carousel id descriptor (%d):", pui1_obj_data[1] + 2));
                            for (len = 0; len < pui1_obj_data[1] + 2; len++)
                            {
                                MHEG5_DBG_ERROR((" %02x", pui1_obj_data[len]));
                            }
                            MHEG5_DBG_ERROR(("\r\n"));
                        }
                    #endif
                    
                        x_memcpy(
                            pt_parse->pui1_descriptor,
                            pui1_obj_data,
                            (SIZE_T)(pui1_obj_data[1] + 2));
                        
                        pt_parse->ui1_flag &= ~FLAG_GET_CAROUSEL_ID_DESC;
                    }
                }
                break;

            default:
                break;
        }
        
        if (0 == pt_parse->ui1_flag)
        {
            t_ret = PARSE_RET_END;
        }
    }
    
    return (t_ret);
}

static PARSE_RET_T _parse_deferred_association_tag(
    HANDLE_T      h_pmt,
    const UINT8*  pui1_obj_data,
    SIZE_T        z_obj_data_len,
    UINT16        ui2_count,
    VOID*         pv_data,
    SIZE_T        z_data_len)
{
    #define ASSOCIATION_TAG_LOOP_OFFSET     3
    
    _PARSE_PARAM_T* pt_parse = (_PARSE_PARAM_T*) pv_data;
    PARSE_RET_T     t_ret    = PARSE_RET_NEXT;
    INT32           i4_num;
    UINT32          ui4_association_tag;
    const UINT8*    pui1_buf;

    if (VALID_DESCRIPTOR_LEN(3))
    {
        i4_num = (INT32) (pui1_obj_data[2] >> 1);

        if (VALID_DESCRIPTOR_LEN((SIZE_T)(3 + pui1_obj_data[2])))
        {
            pui1_buf = pui1_obj_data + ASSOCIATION_TAG_LOOP_OFFSET;

            while (i4_num)
            {
                ui4_association_tag = (UINT32) GET_UINT16_FROM_PTR_BIG_END(pui1_buf);
                
                if (ui4_association_tag == pt_parse->ui4_ref)
                {
                    pui1_buf += (i4_num << 1);
                    pt_parse->ui1_flag  &= ~FLAG_FIND_ASSOCIATION_TAG;
                    pt_parse->ui2_ts_id  = GET_UINT16_FROM_PTR_BIG_END(pui1_buf);
                    pt_parse->ui2_svc_id = GET_UINT16_FROM_PTR_BIG_END((pui1_buf + 2));
                    pt_parse->ui2_on_id  = GET_UINT16_FROM_PTR_BIG_END((pui1_buf + 4));
                    t_ret = PARSE_RET_END;
                    break;
                }

                pui1_buf += 2;
                i4_num--;
            }
        }
    }
    
    return (t_ret);
}

static PARSE_RET_T _parse_ac3_aac_streams (
    HANDLE_T      h_pmt,
    const UINT8*  pui1_obj_data,
    SIZE_T        z_obj_data_len,
    UINT16        ui2_count,
    VOID*         pv_data,
    SIZE_T        z_data_len)
{
    PARSE_RET_T  e_parse_ret = PARSE_RET_NEXT;
    
    if (VALID_DESCRIPTOR(0x6A, 1)
        ||
        VALID_DESCRIPTOR(0x7A, 1)
        ||
        VALID_DESCRIPTOR(0x7C, 3))
    {
        AUD_ENC_T             e_aud_enc;
        AUD_PROF_LVL_T        e_prof_lvl;
        AUDIO_CODEC_WEIGHTING_T t_weighting = 0;
        AC3_AAC_AUDIO_INFO_T* pt_ac3_aac = (AC3_AAC_AUDIO_INFO_T*)pv_data;

        e_prof_lvl = AUD_PROF_LVL_RESERVED;
        
        switch (pui1_obj_data[0])
        {
            case 0x6A:
                e_aud_enc = AUD_ENC_AC3;
                t_weighting = AC3_CODEC_WEIGHTING;
                break;
            case 0x7A:
                e_aud_enc = AUD_ENC_E_AC3;
                t_weighting = EAC3_CODEC_WEIGHTING;
                break;
            case 0x7C:
                e_aud_enc = AUD_ENC_AAC;
                switch (pui1_obj_data[2])
                {
                    case 0x52:
                        e_prof_lvl = AUD_PROF_LVL_AAC_LV4;
                        t_weighting = AACV4_CODEC_WEIGHTING;
                        break;
                    case 0x58:
                        e_prof_lvl = AUD_PROF_LVL_HEAAC_LVL2;
                        t_weighting = HEAACV2_CODEC_WEIGHTING;
                        break;
                    case 0x5A:
                        e_prof_lvl = AUD_PROF_LVL_HEAAC_LVL4;
                        t_weighting = HEAACV4_CODEC_WEIGHTING;
                        break;
                }
                break;
            default:
                e_aud_enc = AUD_ENC_UNKNOWN;
                t_weighting = 0;
                break;
        }

        if (NULL != pt_ac3_aac)
        {
            pt_ac3_aac->b_found     = TRUE;
            pt_ac3_aac->e_aud_enc   = e_aud_enc;
            pt_ac3_aac->e_prof_lvl  = e_prof_lvl;
            pt_ac3_aac->t_weighting = t_weighting;
        }
    }

    return e_parse_ret;
}


static PARSE_RET_T _parse_ad_descriptors (
    HANDLE_T      h_pmt,
    const UINT8*  pui1_obj_data,
    SIZE_T        z_obj_data_len,
    UINT16        ui2_count,
    VOID*         pv_data,
    SIZE_T        z_data_len)
{
    PARSE_RET_T             e_parse_ret = PARSE_RET_NEXT;
    _GET_COMP_LANG_INFO_T  *pt_lang_info;
    UINT8                   ui1_i, ui1_loop_size;
    BOOL                    b_ad_found = FALSE;
    _COMP_ISO_639_DESC     *pt_desc;

    pt_lang_info = (_GET_COMP_LANG_INFO_T *)pv_data;
    pt_desc = pt_lang_info->pt_desc;
    
    if (VALID_DESCRIPTOR(0x0A, 6))
    {
        if (ui2_count<BRDCST_ISO_639_NUM)
        {
            ui1_loop_size = (UINT8)(pui1_obj_data[1] / 4);
            
            for (ui1_i = 0; ui1_i < ui1_loop_size; ui1_i++)
            {
                pt_desc->e_aud_type = pui1_obj_data[5 + ui1_i * 4];

                if (pt_desc->e_aud_type == pt_lang_info->pt_strm_ant->e_aud_type)
                {
                    pt_desc->s_lang[pt_desc->ui1_index][0] = (CHAR)pui1_obj_data[2 + ui1_i * 4];
                    pt_desc->s_lang[pt_desc->ui1_index][1] = (CHAR)pui1_obj_data[3 + ui1_i * 4];
                    pt_desc->s_lang[pt_desc->ui1_index][2] = (CHAR)pui1_obj_data[4 + ui1_i * 4];
                    pt_desc->s_lang[pt_desc->ui1_index][3] = '\0'; 
                    pt_desc->ui1_index++;
                    b_ad_found   = TRUE;
                }
            }
        }
    }
    else if(VALID_DESCRIPTOR(0x50, 8))
    {    
        UINT8                       ui1_stream_content;
        
        if (ui2_count<BRDCST_ISO_639_NUM)
        {
            /* from 300 468 table 26 */
            ui1_stream_content = (UINT8) (pui1_obj_data[2] & 0x0F);
            if (ui1_stream_content == 0x02)
            {
                if (pui1_obj_data[3] == 0x40)
                    pt_desc->e_aud_type = AUD_TYPE_VISUAL_IMPAIRED;
                else if (pui1_obj_data[3] == 0x41)
                    pt_desc->e_aud_type = AUD_TYPE_HEARING_IMPAIRED;
            }
            if (pt_desc->e_aud_type == pt_lang_info->pt_strm_ant->e_aud_type)
            {
                pt_desc->s_lang[pt_desc->ui1_index][0] = (CHAR) pui1_obj_data[5];
                pt_desc->s_lang[pt_desc->ui1_index][1] = (CHAR) pui1_obj_data[6];
                pt_desc->s_lang[pt_desc->ui1_index][2] = (CHAR) pui1_obj_data[7];
                pt_desc->s_lang[pt_desc->ui1_index][3] = '\0';
                pt_desc->ui1_index++;
                b_ad_found   = TRUE;
            }
        }
    }     

    if(b_ad_found)
    {
        pt_lang_info->pt_desc->b_ad_matched = TRUE;
    }
        
    return (e_parse_ret);
}

static PARSE_RET_T _parse_iso_639_language (
    HANDLE_T      h_pmt,
    const UINT8*  pui1_obj_data,
    SIZE_T        z_obj_data_len,
    UINT16        ui2_count,
    VOID*         pv_data,
    SIZE_T        z_data_len)
{
    PARSE_RET_T         e_parse_ret = PARSE_RET_NEXT;
    _GET_LANG_INFO_T    *pt_lang_info;
    UINT8               ui1_i, ui1_loop_size;
    BOOL                b_lang_found = FALSE;
    BOOL                b_ad_found = FALSE;
    ISO_639_LANG_T      s_lang;

    pt_lang_info = (_GET_LANG_INFO_T *)pv_data;

    /* minimum size = 6 bytes (i.e. at least one language in the loop) */
    ui1_loop_size = (UINT8)(pui1_obj_data[1] / 4);
    
    if (VALID_DESCRIPTOR_LEN(6))
    {
        for (ui1_i = 0; ui1_i < ui1_loop_size; ui1_i++)
        {
            _ISO_639_DESC   *pt_desc;
            CHAR*           ps_language;

            pt_desc = pt_lang_info->pt_desc;
            
            ps_language = pt_desc->s_lang;
            
            ps_language[0] = (CHAR)pui1_obj_data[2 + ui1_i * 4];
            ps_language[1] = (CHAR)pui1_obj_data[3 + ui1_i * 4];
            ps_language[2] = (CHAR)pui1_obj_data[4 + ui1_i * 4];
            ps_language[3] = '\0';

            pt_desc->e_aud_type = pui1_obj_data[5 + ui1_i * 4];

            if (x_strcmp(ps_language, pt_lang_info->pt_strm_ant->s_lang) == 0 &&
                pt_desc->e_aud_type == pt_lang_info->pt_strm_ant->e_aud_type)
            {
                b_lang_found = TRUE;
                b_ad_found   = TRUE;
                x_strcpy(s_lang, ps_language);
                break;
            }
            else if (x_strcmp(ps_language, pt_lang_info->pt_strm_ant->s_lang) == 0)
            {
                b_lang_found = TRUE;
                x_strcpy(s_lang, ps_language);
            }
        }
        
        e_parse_ret = PARSE_RET_END;

        if (b_lang_found)
        {
            pt_lang_info->pt_desc->b_lang_found = TRUE;

            if (b_ad_found)
            {
                pt_lang_info->pt_desc->b_ad_matched = TRUE;
            }
            
            x_strcpy(((_GET_LANG_INFO_T *)pv_data)->pt_desc->s_lang, s_lang);
        }
        /* use the GUI languate to search */
        else
        {
            for (ui1_i = 0; ui1_i < ui1_loop_size; ui1_i++)
            {
                _ISO_639_DESC   *pt_desc;
                CHAR            *ps_language;

                pt_lang_info = (_GET_LANG_INFO_T *)pv_data;

                pt_desc = pt_lang_info->pt_desc;
                
                ps_language = pt_desc->s_lang;
                
                ps_language[0] = (CHAR)pui1_obj_data[2 + ui1_i * 4];
                ps_language[1] = (CHAR)pui1_obj_data[3 + ui1_i * 4];
                ps_language[2] = (CHAR)pui1_obj_data[4 + ui1_i * 4];
                ps_language[3] = '\0';

                pt_desc->e_aud_type = pui1_obj_data[5 + ui1_i * 4];

                if (x_strcmp(ps_language, pt_lang_info->pt_strm_ant->s_gui_lang) == 0 &&
                    pt_desc->e_aud_type == pt_lang_info->pt_strm_ant->e_aud_type)
                {
                    x_strcpy(s_lang, ps_language);
                    b_lang_found = TRUE;
                    b_ad_found   = TRUE;
                    break;
                }
                else if (x_strcmp(ps_language, pt_lang_info->pt_strm_ant->s_gui_lang) == 0)
                {
                    x_strcpy(s_lang, ps_language);
                    b_lang_found = TRUE;
                }
            }

            if (b_lang_found)
            {
                pt_lang_info->pt_desc->b_lang_found = TRUE;
                pt_lang_info->pt_desc->b_gui_lang   = TRUE;

                if (b_ad_found)
                {
                    pt_lang_info->pt_desc->b_ad_matched = TRUE;
                }
                
                x_strcpy(((_GET_LANG_INFO_T *)pv_data)->pt_desc->s_lang, s_lang);
            }
        }
    }
    
    return (e_parse_ret);
}
#ifndef MHEG5_HD_SUPPORT
static INT32 _parse_network_boot_info_sub_desc(
    HANDLE_T    h_pmt,
    UINT32      ui4_max_len,
    UINT8*      pui1_boot_info,
    UINT32*     pui4_actual_len,
    UINT8*      pui1_nb_version,
    UINT8*      pui1_nb_action)
{
    _PARSE_PARAM_T  t_parse;
    INT32           i4_ret;
    
    t_parse.ui4_ref         = BOOT_DATA_BROADCAST_ID;
    t_parse.ui1_ref_len     = (UINT8) 2;
    t_parse.ui1_flag        = FLAG_FIND_REF_DATA | FLAG_CHECK_AP_TYPE_RPIORITY | FLAG_GET_NETWORK_BOOT_INFO;
    t_parse.ui1_mask        = FLAG_FIND_REF_DATA | FLAG_CHECK_AP_TYPE_RPIORITY | FLAG_GET_NETWORK_BOOT_INFO;
    t_parse.pui1_descriptor = pui1_boot_info;
    t_parse.ui2_desc_len    = (UINT16) ui4_max_len;
    
    t_parse.i2_boot_priority_hint = -1;
    
    i4_ret = _search_descriptors_with_data(
        h_pmt, 
        TAG_DATA_BROADCAST_ID_DESCRIPTOR, 
        &t_parse,
        NULL, 
        NULL,
        NULL);

    if (MHEG5R_OK == i4_ret)
    {
        if (pui4_actual_len)
        {
            *pui4_actual_len = (UINT32) t_parse.ui2_desc_len;
        }
        
        if (pui1_nb_version)
        {
            *pui1_nb_version = t_parse.ui1_nb_version;
        }
        
        if (pui1_nb_action)
        {
            *pui1_nb_action = t_parse.ui1_nb_action;
        }
    }
    
    return (i4_ret);
}
#endif
#ifdef MHEG5_HD_SUPPORT
static PARSE_RET_T _find_specified_data_ex(
    HANDLE_T      h_pmt,
    const UINT8*  pui1_obj_data,
    SIZE_T        z_obj_data_len,
    UINT16        ui2_count,
    VOID*         pv_data,
    SIZE_T        z_data_len)
{

    _PARSE_PARAM_T* pt_parse = (_PARSE_PARAM_T*) pv_data;
    PARSE_RET_T     t_ret    = PARSE_RET_NEXT;
    INT32           i4_len   = (INT32) pt_parse->ui1_ref_len;
    UINT32          ui4_data = 0;
    const UINT8*    pui1_buf;

    if (pt_parse->ui1_flag & FLAG_FIND_REF_DATA)
    {
        if (VALID_DESCRIPTOR_LEN((SIZE_T)(i4_len + 2)))
        {
            pui1_buf = pui1_obj_data + 2;
            
            while (i4_len--)
            {
                ui4_data = (UINT32) (ui4_data << 8) + (UINT32) pui1_buf[0];
                pui1_buf++;
            }

            if (ui4_data == pt_parse->ui4_ref)
            {
                if (pt_parse->ui1_flag & (FLAG_GET_NETWORK_BOOT_INFO | FLAG_GET_SERVICE_BOOT_INFO))
                {
#if 0
                    if (VALID_DESCRIPTOR_LEN(pui1_obj_data[1] + 2) &&
                        (APPLICATION_TYPE_CODE_LAUNCH_PROFILE == 
                            (UINT16) GET_UINT16_FROM_PTR_BIG_END((pui1_obj_data + 4))))
#else
                    if (VALID_DESCRIPTOR_LEN((SIZE_T)(pui1_obj_data[1] + 2)))
#endif
                    {
                        UINT16 ui2_app_type = (UINT16) GET_UINT16_FROM_PTR_BIG_END((pui1_obj_data + 4));
                        INT16  i2_app_priority = (INT16) pui1_obj_data[6];
                        
                        if ((APPLICATION_TYPE_CODE_LAUNCH_PROFILE   == ui2_app_type ||
                             APPLICATION_TYPE_CODE_BASELINE_PROFILE == ui2_app_type ||
                             APPLICATION_TYPE_CODE_105_PROFILE == ui2_app_type      ||
                             0x0000 == ui2_app_type))
                        {
                            INT32 i = 0;
                            INT32 i4_n2 = (INT32) pui1_obj_data[7];

                            pt_parse->ui1_flag &= ~FLAG_CHECK_AP_TYPE_RPIORITY;
                            pt_parse->i2_boot_priority_hint = i2_app_priority;
                            
                            pui1_buf = pui1_obj_data + 8;

                            while (i < i4_n2)
                            {
                                if (TAG_NETWORK_BOOT_INFO_SUB_DESCRIPTOR == pui1_buf[0])
                                {
                                    INT32 i4_sub_len = (INT32) pui1_buf[1];
                                    
                                    if (pt_parse->ui1_flag & FLAG_GET_NETWORK_BOOT_INFO)
                                    {
                                        pt_parse->ui2_desc_len = MIN((UINT16)(i4_sub_len - 2), pt_parse->ui2_desc_len);
                                        
                                        if ((pt_parse->pui1_descriptor) &&
                                            (pt_parse->ui2_desc_len > 0))
                                        {
                                            x_memcpy(
                                                pt_parse->pui1_descriptor, 
                                                pui1_buf + 4, 
                                                pt_parse->ui2_desc_len);
                                        }

                                        pt_parse->ui1_nb_version = pui1_buf[2];
                                        pt_parse->ui1_nb_action  = pui1_buf[3];
                                    
                                        pt_parse->ui1_flag &= ~FLAG_GET_NETWORK_BOOT_INFO;
                                    }

                                    pui1_buf += i4_sub_len + 2;
                                    i += i4_sub_len + 2;
                                    
                                    /*break;*/
                                }
                                else if (TAG_SERVICE_BOOT_INFO_SUB_DESCRIPTOR == pui1_buf[0])
                                {
                                    if (pt_parse->ui1_flag & FLAG_GET_SERVICE_BOOT_INFO)
                                    {
                                        pt_parse->ui2_association_tag = (UINT16) GET_UINT16_FROM_PTR_BIG_END((pui1_buf + 2));
                                        pt_parse->ui1_flag &= ~FLAG_GET_SERVICE_BOOT_INFO;
                                    }
                                    
                                    pui1_buf += 4;
                                    i += 4;
                                }
                                else
                                {
                                    break;
                                }
                            }
                        }
                    }
                }

                pt_parse->ui1_flag &= ~FLAG_FIND_REF_DATA;
            }
        }
    }
    
    if (0 == pt_parse->ui1_flag)
    {
        t_ret = PARSE_RET_END;
    }
    
    return (t_ret);
}

static INT32 _search_descriptors_with_data_ex(
    HANDLE_T        h_pmt,
    UINT8           ui1_tag,
    _PARSE_PARAM_T* pt_parse,
    UINT16*         pui2_idx,
    MPEG_2_PID_T*   pt_pid,
    UINT8*          pui1_enc_type,
    MPEG_2_PID_T    t_pid)
{
    PSI_STREAM_INFO_T           t_stream_info;
    TM_COND_T                   e_stream_cond;
    TM_FIRST_LOOP_DESCR_SEL_T   t_first_loop_descr_sel;
    INT32                       i4_ret;
    UINT16                      ui2_idx;
    UINT8                       ui1_descr_tag = ui1_tag;
    UINT8                       ui1_flag = pt_parse->ui1_flag;
    BOOL                        b_found = FALSE;
    UINT8                       ui1_tmp = 0;
    
    t_first_loop_descr_sel.pui1_descr_tag_list = &ui1_descr_tag;
    t_first_loop_descr_sel.z_num_descr_tags    = 1;
    
    ui2_idx = 0;
    
    while (1)
    {
        i4_ret = x_psi_get_stream_entry(
            h_pmt,
            ui2_idx,
            &t_stream_info,
            &e_stream_cond);

        if (i4_ret != TMR_OK)
        {
            i4_ret = MHEG5R_PARSE_PMT_FAIL;
            break;
        }

        if (t_pid != MPEG_2_NULL_PID)
        {
            if (t_pid != t_stream_info.t_pid)
            {
                ui2_idx++;
                continue;
            }
        }
        
        t_first_loop_descr_sel.ui2_loop_idx = ui2_idx;

        i4_ret = x_tm_parse_obj(
            h_pmt,
            TM_PARSE_TYPE_FIRST_LOOP_DESCRS,
            &t_first_loop_descr_sel,
            pt_parse,
            sizeof(_PARSE_PARAM_T),
            _find_specified_data_ex,
            NULL);

        if (TMR_OK == i4_ret)
        {
            if (0 == ((pt_parse->ui1_flag) & (pt_parse->ui1_mask)))
            {
                b_found = TRUE;
                
                if (pui2_idx)
                {
                    *pui2_idx = ui2_idx;
                }
                
                if (pt_pid)
                {
                    *pt_pid = t_stream_info.t_pid;
                }

                if (pui1_enc_type)
                {
                    if (STREAMTYPE_VIDEO_11172_2      == t_stream_info.ui1_stream_type ||
                        STREAMTYPE_VIDEO_13818_2      == t_stream_info.ui1_stream_type ||
                        STREAMTYPE_VIDEO_DIGICIPHER_2 == t_stream_info.ui1_stream_type ||
                        STREAMTYPE_VIDEO_H264 == t_stream_info.ui1_stream_type)
                    {
                        *pui1_enc_type = _streamtype_to_vid_enc(t_stream_info.ui1_stream_type);
                    }
                    else if (STREAMTYPE_AUDIO_11172_3  == t_stream_info.ui1_stream_type ||
                             STREAMTYPE_AUDIO_13818_3  == t_stream_info.ui1_stream_type ||
                             STREAMTYPE_AUDIO_ATSC_A53 == t_stream_info.ui1_stream_type)
                    {
                        *pui1_enc_type = AUD_ENC_MPEG_2;
                    }
                    else if (STREAMTYPE_PES_PRIV_13818_1  == t_stream_info.ui1_stream_type ||
                             STREAMTYPE_AUDIO_HE_ACC == t_stream_info.ui1_stream_type)
                    {
                        TM_FIRST_LOOP_DESCR_SEL_T   t_descr;
                        AC3_AAC_AUDIO_INFO_T        t_ac3_aac;
                        UINT8                       aui1_descriptor_tag[3];
                        
                        aui1_descriptor_tag[0] = TAG_AC_3_DESCRIPTOR;
                        aui1_descriptor_tag[1] = TAG_EAC_3_DESCRIPTOR;
                        aui1_descriptor_tag[2] = TAG_AAC_DESCRIPTOR;
                        
                        t_descr.ui2_loop_idx        = ui2_idx;
                        t_descr.pui1_descr_tag_list = aui1_descriptor_tag;
                        t_descr.z_num_descr_tags    = 3;
                        
                        x_memset(&t_ac3_aac, 0, sizeof(AC3_AAC_AUDIO_INFO_T));
                            
                            x_tm_parse_obj(h_pmt,
                                           TM_PARSE_TYPE_FIRST_LOOP_DESCRS,
                                           &t_descr,
                                           &t_ac3_aac,
                                           sizeof(AC3_AAC_AUDIO_INFO_T),
                                           _parse_ac3_aac_streams,
                                           NULL);
                        
                        if (t_ac3_aac.b_found == TRUE)
                        {                        
                            *pui1_enc_type = t_ac3_aac.e_aud_enc;
                        }
                        else
                        {
                            *pui1_enc_type = 0;/*unknown*/
                        }
                    }
                    else
                    {
                        *pui1_enc_type = 0;/*unknown*/
                    }
                }

                ui1_tmp = pt_parse->ui1_flag;
            }
        }
        else
        {
            break;
        }

        pt_parse->ui1_flag = ui1_flag;
        ui2_idx++;
    }

    if (b_found)
    {
        pt_parse->ui1_flag = ui1_tmp;
        return MHEG5R_OK;
    }
    else
    {
        return MHEG5R_PARSE_PMT_FAIL;
    }
}

static INT32 _parse_network_boot_info_sub_desc_ex(
    HANDLE_T    h_pmt,
    UINT32      ui4_max_len,
    UINT8*      pui1_boot_info,
    UINT32*     pui4_actual_len,
    UINT8*      pui1_nb_version,
    UINT8*      pui1_nb_action,
    MPEG_2_PID_T t_pid)
{
    _PARSE_PARAM_T  t_parse;
    INT32           i4_ret;
    
    t_parse.ui4_ref         = BOOT_DATA_BROADCAST_ID;
    t_parse.ui1_ref_len     = (UINT8) 2;
    t_parse.ui1_flag        = FLAG_FIND_REF_DATA | FLAG_CHECK_AP_TYPE_RPIORITY | FLAG_GET_NETWORK_BOOT_INFO;
    t_parse.ui1_mask        = FLAG_FIND_REF_DATA | FLAG_CHECK_AP_TYPE_RPIORITY | FLAG_GET_NETWORK_BOOT_INFO;
    t_parse.pui1_descriptor = pui1_boot_info;
    t_parse.ui2_desc_len    = (UINT16) ui4_max_len;
    
    t_parse.i2_boot_priority_hint = -1;
    
    i4_ret = _search_descriptors_with_data_ex(
        h_pmt, 
        TAG_DATA_BROADCAST_ID_DESCRIPTOR, 
        &t_parse,
        NULL, 
        NULL,
        NULL,
        t_pid);

    if (MHEG5R_OK == i4_ret)
    {
        if (pui4_actual_len)
        {
            *pui4_actual_len = (UINT32) t_parse.ui2_desc_len;
        }
        
        if (pui1_nb_version)
        {
            *pui1_nb_version = t_parse.ui1_nb_version;
        }
        
        if (pui1_nb_action)
        {
            *pui1_nb_action = t_parse.ui1_nb_action;
        }
    }
    
    return (i4_ret);
}

static INT32 _parse_service_boot_info_ex(
    HANDLE_T        h_pmt,
    MPEG_2_PID_T*   pt_pid,
    UINT16*         pui2_association_tag,
    MPEG_2_PID_T    t_pid)
{
    _PARSE_PARAM_T  t_parse;
    UINT16          ui2_idx;
    INT32           i4_ret;
    
    t_parse.ui4_ref     = BOOT_DATA_BROADCAST_ID;
    t_parse.ui1_ref_len = (UINT8) 2;
    t_parse.ui1_flag    = FLAG_FIND_REF_DATA | FLAG_CHECK_AP_TYPE_RPIORITY | FLAG_GET_SERVICE_BOOT_INFO;
    t_parse.ui1_mask    = FLAG_FIND_REF_DATA | FLAG_CHECK_AP_TYPE_RPIORITY | FLAG_GET_SERVICE_BOOT_INFO;

    t_parse.i2_boot_priority_hint = -1;
    
    i4_ret = _search_descriptors_with_data_ex(
        h_pmt, 
        TAG_DATA_BROADCAST_ID_DESCRIPTOR, 
        &t_parse,
        &ui2_idx, 
        pt_pid,
        NULL,
        t_pid);

    if (MHEG5R_OK == i4_ret)
    {
        /* if service_boot_info sub-descriptor is found, use the assoTag field as the association tag,
           otherwise returns 0xffff instead */
        *pui2_association_tag = 
            (0 == t_parse.ui1_flag) ? t_parse.ui2_association_tag : (UINT16) MHEG5_LIFE_CYCLE_TAG_NOT_FOUND;
    }
    else
    {
        *pui2_association_tag = (UINT16) MHEG5_LIFE_CYCLE_TAG_NOT_FOUND;
    }
    
    return (i4_ret);
}

#endif


#ifdef MW_EDB_SUPPORT

#else
static INT32 _get_event_info(    
    HANDLE_T    h_evt,
    EVCTX_KEY_TYPE_T e_key_type, 
    VOID*     pv_key_info,
    VOID**     data, 
    UINT32*  data_len )
{
    INT32  i4_ret = 0;
    SIZE_T            z_info_len = 0;
    EVCTX_COND_T  e_cond;

    i4_ret = x_evctx_get_event_info_len(
        h_evt,
        e_key_type,
        pv_key_info, 
        &z_info_len,
        &e_cond);

    if (i4_ret == EVCTXR_OK)
    {
        VOID*  ps_title = NULL;

        *data_len = z_info_len;  /* return data len */
        ps_title = (CHAR*) x_mem_alloc(z_info_len);

        if (ps_title != NULL)
        {
            i4_ret = x_evctx_get_event_info(
                h_evt,
                e_key_type,
                pv_key_info,  
                &z_info_len,
                ps_title,
                &e_cond);
            
            if( i4_ret == EVCTXR_OK )
            {
                *data = ps_title; /* return gotten data */
            }
            else
            {
                x_mem_free(ps_title);
            }
        }        
        else
        {            
            i4_ret = EVCTXR_OUT_OF_MEM;
        }
    }

    return i4_ret;
}
#endif

static INT32 _parse_event_mjd_data_time(
    TIME_T *pui8_utg,
    U32BIT *pui4_mjd_data,
    S_Time *pt_mjd_time)
{
    UINT16 mjd_data;
    DTG_T  t_dtg = {0}; 
    DTG_T  t_local_dtg = {0}; 

    if (pui8_utg == NULL || pui4_mjd_data == NULL || pt_mjd_time == NULL)
    {
        return MHEG5R_INV_ARG;
    }

    if( x_dt_utc_sec_to_dtg((*pui8_utg), &t_dtg) != DTR_OK )
    {
        x_dbg_stmt( "convert UTC to DTG_T failed. \r\n" );
        return MHEG5R_INTERNAL_ERROR;
    }

    x_dt_conv_utc_local(&t_dtg, &t_local_dtg);

    if( x_dt_dtg_to_mjd(&t_local_dtg, &mjd_data) != DTR_OK )
    {
        x_dbg_stmt( "get MJD data failed. \r\n" );
        return MHEG5R_INTERNAL_ERROR;
    }

    *pui4_mjd_data = (U32BIT)mjd_data;
    pt_mjd_time->hours = (U16BIT)t_local_dtg.ui1_hr;
    pt_mjd_time->minutes = (U8BIT)t_local_dtg.ui1_min;
    pt_mjd_time->seconds = (U8BIT)t_local_dtg.ui1_sec;

    return MHEG5R_OK;
}


static BOOL _parse_query(
    HANDLE_T                h_pmt,
    ptmMHEG5SiQuery_t       pt_in_query,
    ptmMHEG5SiQueryResult_t pt_qry_res,
    _STREAM_ANNOTATION_T*   pt_strm_ant,
    UINT8*                  pui1_enc_type)
{
    INT32 i4_ret = MHEG5R_INV_ARG;
    
    switch (pt_in_query->kind)
    {
        case MHEG5_SI_PID:
        {
            MPEG_2_PID_T t_pid;
            
            if (pt_strm_ant)
            {
                i4_ret = _get_default_stream_pid(
                    h_pmt,
                    pt_strm_ant,
                    &t_pid,
                    pui1_enc_type);
            }
            else
            {
                i4_ret = _get_stream_pid(
                    h_pmt,
                    (UINT16) pt_in_query->data.association_tag,
                    &t_pid,
                    pui1_enc_type);
            }
            
            if (MHEG5R_OK == i4_ret)
            {
                pt_qry_res->data.PID = (U16BIT) t_pid;
            }
            else
            {
                MHEG5_DBG_APP_SI(("SI query type: MHEG5_SI_PID, query result: Failed!\n"));
            }

            break;
        }
        
        case MHEG5_SI_BOOT_CAROUSEL:
        {
            MPEG_2_PID_T    t_pid;
            UINT16          ui2_association_tag;
            UINT8           ui1_comp_tag;
            UINT32          ui4_carousel_id;
            
            i4_ret = _get_boot_carousel(
                h_pmt,
                &t_pid,
                &ui1_comp_tag,
                &ui2_association_tag,
                &ui4_carousel_id,
                (UINT8*) pt_qry_res->data.carouselBootInfo.carousel_id_descriptor);
            
            if (MHEG5R_OK == i4_ret)
            {
                pt_qry_res->data.carouselBootInfo.PID           = (U16BIT) t_pid;
                pt_qry_res->data.carouselBootInfo.component_tag = (U8BIT)  ui1_comp_tag;
                pt_qry_res->data.carouselBootInfo.life_cycle_association_tag = (U16BIT) ui2_association_tag;


                if (h_pmt == _t_loader.h_pmt_active)
                {
                    _t_loader.ui1_boot_comp_tag    = ui1_comp_tag;
                    _t_loader.t_boot_pid           = t_pid;
                    _t_loader.ui4_boot_carousel_id = ui4_carousel_id;
                    #ifdef MHEG5_HD_SUPPORT
                    _t_loader.t_si_carousel_pid = MPEG_2_NULL_PID;
                    #endif
                }
            }
            else
            {
                MHEG5_DBG_APP_SI(("SI query type: MHEG5_SI_BOOT_CAROUSEL, query result:Failed!\n"));
            }
            
            break;
        }

        case MHEG5_SI_CAROUSEL:
        {
            MPEG_2_PID_T    t_pid;
            UINT8           ui1_comp_tag;
            #ifdef MHEG5_HD_SUPPORT
            UINT16          ui2_association_tag;
            #endif
            
            i4_ret = _get_carousel(
                h_pmt,
                (UINT32) pt_in_query->data.carousel_id,
                &t_pid,
                &ui1_comp_tag,
                (UINT8*) pt_qry_res->data.carouselBootInfo.carousel_id_descriptor
                #ifdef MHEG5_HD_SUPPORT
                ,
                &ui2_association_tag
                #endif
                );
            
            if (MHEG5R_OK == i4_ret)
            {                
                pt_qry_res->data.carouselBootInfo.PID           = (U16BIT) t_pid;
                pt_qry_res->data.carouselBootInfo.component_tag = (U8BIT)  ui1_comp_tag;
                #ifndef MHEG5_HD_SUPPORT
                pt_qry_res->data.carouselBootInfo.life_cycle_association_tag = (UINT16) MHEG5_LIFE_CYCLE_TAG_NOT_FOUND;
                #else
                pt_qry_res->data.carouselBootInfo.life_cycle_association_tag = ui2_association_tag;
                #endif
                #ifdef MHEG5_HD_SUPPORT
                _t_loader.t_si_carousel_pid = t_pid;
                #endif
            }
            else
            {
                MHEG5_DBG_APP_SI(("SI query type: MHEG5_SI_CAROUSEL, query result:Failed!\n"));
            }
            
            break;
        }

        case MHEG5_SI_PIDLIST:
        {
            UINT16 ui2_num_pid = 0;
            
            i4_ret = _get_pid_list(
                h_pmt,
                MAX_PID_LIST_SIZE,
                &ui2_num_pid,
                (UINT16*) pt_qry_res->data.PIDList.PIDList,
                (UINT8*)  pt_qry_res->data.PIDList.component_tag);
            
            if (MHEG5R_OK == i4_ret)
            {
                pt_qry_res->data.PIDList.numPIDs = (U16BIT) ui2_num_pid;
            }
            
            break;
        }

        case MHEG5_SI_DEFERRED_SERVICE:
        {
            UINT16 ui2_ts_id, ui2_svc_id, ui2_on_id;

            i4_ret = _get_deferred_svc(
                h_pmt,
                (UINT16) pt_in_query->data.association_tag,
                &ui2_ts_id,
                &ui2_svc_id,
                &ui2_on_id);
            
            if (MHEG5R_OK == i4_ret)
            {
                pt_qry_res->data.deferredService.transport_stream_id = (U16BIT) ui2_ts_id;
                pt_qry_res->data.deferredService.service_id          = (U16BIT) ui2_svc_id;
                pt_qry_res->data.deferredService.original_network_id = (U16BIT) ui2_on_id;
            }
            else
            {
                pt_qry_res->data.deferredService.transport_stream_id = (U16BIT) _t_loader.ui2_ts_id;
                pt_qry_res->data.deferredService.service_id          = (U16BIT) _t_loader.ui2_active_svc_id;
                pt_qry_res->data.deferredService.original_network_id = (U16BIT) _t_loader.ui2_on_id;
                i4_ret = MHEG5R_OK;
            }
            
            break;
        }
        
        default:
            i4_ret = MHEG5_ERR_BAD_PARAMETER;
    }

    if (MHEG5R_OK == i4_ret)
    {
        pt_qry_res->kind = pt_in_query->kind;
    }

    return (i4_ret);
}

static VOID _prepare_connection(
    _SI_LOADER_INFO_T*  pt_loader,
    UINT16              ui2_on_id,
    UINT16              ui2_ts_id,
    UINT16              ui2_svc_id)
{
    if (LDR_ST_CLOSED != pt_loader->e_state)
    {
        _unload(pt_loader);
    }
    
    pt_loader->ui2_on_id         = ui2_on_id;
    pt_loader->ui2_ts_id         = ui2_ts_id;
    pt_loader->ui2_active_svc_id = ui2_svc_id;
}

static VOID _reply_deferred_query(
    HANDLE_T            h_pmt,
    UINT16              ui2_svc_id,
    SLIST_SI_QUERY_T*   pt_query_lst,
    SLIST_SI_QUERY_T*   pt_close_lst)
{
    _SI_QUERY_T*            pt_query_done = NULL;
    _SI_QUERY_T*            pt_query;
    tmMHEG5SiQueryResult_t  t_qry_res;
    tmMHEG5SiStatus_t       t_status;
    
    SLIST_FOR_EACH(pt_query, pt_query_lst, t_link)
    {
        if (pt_query_done)
        {
            SLIST_REMOVE(pt_query_done, t_link);
            
            if (SI_QUERY_SOURCE_MHEG5_STACK == pt_query_done->ui1_source)
            {
                SLIST_INSERT_HEAD(pt_query_done, pt_close_lst, t_link);
            }
            else /* SI_QUERY_SOURCE_GLUE_LAYER == pt_query_done->ui1_source */
            {
                x_handle_free(pt_query_done->h_this);
            }
            
            pt_query_done = NULL;
        }
        
        if (ui2_svc_id == (UINT16) pt_query->t_query.service_id)
        {
            UINT8 ui1_enc_type;
            
            if (MHEG5R_OK == _parse_query(
                h_pmt,
                &(pt_query->t_query),
                &t_qry_res,
                ((pt_query->b_strm_ant_valid) ? &(pt_query->t_strm_ant) : NULL),
                &ui1_enc_type))
            {
                t_status = MHEG5_SI_SUCCESS;
            }
            else
            {
                t_status = MHEG5_SI_FAILURE;
            }
            #ifdef DEBUG
            switch (pt_query->t_query.kind)
            {
                case MHEG5_SI_PID:
                {
                    MHEG5_DBG_APP_SI((
                        "Deferred SI query: SiQuery {kind=MHEG5_SI_PID, service_id=%d, association_tag=%d}\r\n",
                        pt_query->t_query.service_id,
                        pt_query->t_query.data.association_tag));
                    
                    if (MHEG5_SI_SUCCESS == t_status)
                    {
                        MHEG5_DBG_APP_SI((
                            "SiQueryResult {kind=%s, PID=%d}\r\n",
                            "MHEG5_SI_PID",
                            t_qry_res.data.PID));
                    }
                    else
                    {
                        MHEG5_DBG_APP_SI(("SiQueryResult:Failed\n"));
                    }
                    
                    break;
                }
                
                case MHEG5_SI_BOOT_CAROUSEL:
                {
                    MHEG5_DBG_APP_SI((
                        "Deferred SI query: SiQuery {kind=MHEG5_SI_BOOT_CAROUSEL, service_id=%d}\r\n",
                        pt_query->t_query.service_id));
                    
                    if (MHEG5_SI_SUCCESS == t_status)
                    {
                        MHEG5_DBG_APP_SI((
                            "SiQueryResult {kind=%s, PID=%d, component_tag=%d, life_cycle_association_tag=%d}\r\n",
                            "MHEG5_SI_BOOT_CAROUSEL",
                            t_qry_res.data.carouselBootInfo.PID,
                            t_qry_res.data.carouselBootInfo.component_tag,
                            t_qry_res.data.carouselBootInfo.life_cycle_association_tag));
                    }
                    else
                    {
                        MHEG5_DBG_APP_SI(("SiQueryResult:Failed\n"));
                    }
                    
                    break;
                }

                case MHEG5_SI_CAROUSEL:
                {
                    MHEG5_DBG_APP_SI((
                        "Deferred SI query: SiQuery {kind=MHEG5_SI_CAROUSEL, service_id=%d, carousel_id = %d}\r\n",
                        pt_query->t_query.service_id,
                        pt_query->t_query.data.carousel_id));
                    
                    if (MHEG5_SI_SUCCESS == t_status)
                    {
                        MHEG5_DBG_APP_SI((
                            "SiQueryResult {kind=%s, PID=%d, component_tag=%d, life_cycle_association_tag=%d}\r\n",
                            "MHEG5_SI_CAROUSEL",
                            t_qry_res.data.carouselBootInfo.PID,
                            t_qry_res.data.carouselBootInfo.component_tag,
                            t_qry_res.data.carouselBootInfo.life_cycle_association_tag));
                    }
                    else
                    {
                        MHEG5_DBG_APP_SI(("SiQueryResult:Failed\n"));
                    }
                    
                    break;
                }

                case MHEG5_SI_PIDLIST:
                {
                    MHEG5_DBG_APP_SI((
                        "Deferred SI query: SiQuery {kind=MHEG5_SI_PIDLIST, service_id=%d}\r\n",
                        pt_query->t_query.service_id));

                    if (MHEG5_SI_SUCCESS == t_status)
                    {
                        INT32 i = 0;

                        for (i=0;i<t_qry_res.data.PIDList.numPIDs;i++)
                        {
                            MHEG5_DBG_APP_SI((
                            "SiQueryResult {kind=%s, PID=%d, component_tag=%d}\r\n",
                            "MHEG5_SI_PIDLIST",
                            t_qry_res.data.PIDList.PIDList[i],
                            t_qry_res.data.PIDList.component_tag[i]));
                        }
                        
                    }
                    else
                    {
                        MHEG5_DBG_APP_SI(("SiQueryResult:Failed\n"));
                    }
                    
                    break;
                }

                case MHEG5_SI_DEFERRED_SERVICE:
                {
                    MHEG5_DBG_APP_SI((
                        "Deferred SI query: SiQuery {kind=MHEG5_SI_DEFERRED_SERVICE, service_id=%d, association_tag = %d}\r\n",
                        pt_query->t_query.service_id,
                        pt_query->t_query.data.association_tag));
                    
                    if (MHEG5_SI_SUCCESS == t_status)
                    {
                        MHEG5_DBG_APP_SI((
                            "SiQueryResult {kind=%s, transport_stream_id=%d, service_id=%d, original_network_id=%d}\r\n",
                            "MHEG5_SI_DEFERRED_SERVICE",
                            t_qry_res.data.deferredService.transport_stream_id,
                            t_qry_res.data.deferredService.service_id,
                            t_qry_res.data.deferredService.original_network_id));
                    }
                    else
                    {
                        MHEG5_DBG_APP_SI(("SiQueryResult:Failed\n"));
                    }
                    
                    break;
                }
                
                default:
                    break;
            }
            #endif

           
            pt_query->ui1_status = SI_QUERY_STATUS_CLOSED;
            
            if (SI_QUERY_SOURCE_MHEG5_STACK == pt_query->ui1_source)
            {
                tmMHEG5NotifySiQueryResult(
                    (tmMHEG5SiQueryRef_t) pt_query->pv_query_ref,
                    pt_query->pv_usr_data,
                    t_status,
                    &t_qry_res,
                    (VOID*) pt_query->h_this);
            }
            else /* SI_QUERY_SOURCE_GLUE_LAYER == pt_query->ui1_source */
            {
                MPEG_2_PID_T    t_pcr_pid;
                TM_COND_T       e_cond;
                
                x_mheg5_resolve_nfy pf_resolve_nfy = (x_mheg5_resolve_nfy) pt_query->pv_usr_data;

                x_psi_get_pcr_pid(h_pmt, &t_pcr_pid, &e_cond);

                if (pf_resolve_nfy)
                {
                    (*pf_resolve_nfy)(
                        (MHEG5_SI_SUCCESS == t_status),
                        (MPEG_2_PID_T) t_qry_res.data.PID,
                        t_pcr_pid,
                        ui1_enc_type,
                        pt_query->pv_query_ref);
                }
            }

            /* remove the deferred query */
            pt_query_done = pt_query;
        }
    }

    if (pt_query_done)
    {
        SLIST_REMOVE(pt_query_done, t_link);
        
        if (SI_QUERY_SOURCE_MHEG5_STACK == pt_query_done->ui1_source)
        {
            SLIST_INSERT_HEAD(pt_query_done, pt_close_lst, t_link);
        }
        else /* SI_QUERY_SOURCE_GLUE_LAYER == pt_query_done->ui1_source */
        {
            x_handle_free(pt_query_done->h_this);
        }
    }
}

static VOID _reject_deferred_query(
    UINT16              ui2_svc_id,
    SLIST_SI_QUERY_T*   pt_query_lst,
    SLIST_SI_QUERY_T*   pt_close_lst)
{
    _SI_QUERY_T*        pt_prv_query = NULL;
    _SI_QUERY_T*        pt_query;

    SLIST_FOR_EACH(pt_query, pt_query_lst, t_link)
    {
        if (pt_prv_query)
        {
            SLIST_REMOVE(pt_prv_query, t_link);
            
            if (SI_QUERY_SOURCE_MHEG5_STACK == pt_prv_query->ui1_source)
            {
                SLIST_INSERT_HEAD(pt_prv_query, pt_close_lst, t_link);
            }
            else /* SI_QUERY_SOURCE_GLUE_LAYER == pt_prv_query->ui1_source */
            {
                x_handle_free(pt_prv_query->h_this);
            }
            
            pt_prv_query = NULL;
        }
        
        if (ui2_svc_id == (UINT16) pt_query->t_query.service_id)
        {
            pt_query->ui1_status = SI_QUERY_STATUS_CLOSED;
            
            if (SI_QUERY_SOURCE_MHEG5_STACK == pt_query->ui1_source)
            {
                tmMHEG5NotifySiQueryResult(
                    (tmMHEG5SiQueryRef_t) pt_query->pv_query_ref,
                    pt_query->pv_usr_data,
                    MHEG5_SI_FAILURE,
                    NULL,
                    (VOID*) pt_query->h_this);
            }
            else /* SI_QUERY_SOURCE_GLUE_LAYER == pt_query->ui1_source */
            {
                x_mheg5_resolve_nfy pf_resolve_nfy = (x_mheg5_resolve_nfy) pt_query->pv_usr_data;

                if (pf_resolve_nfy)
                {
                    (*pf_resolve_nfy)(
                        FALSE,
                        0,
                        0,
                        0,
                        pt_query->pv_query_ref);
                }
            }

            pt_prv_query = pt_query;
        }
    }
    
    if (pt_prv_query)
    {
        SLIST_REMOVE(pt_prv_query, t_link);
        
        if (SI_QUERY_SOURCE_MHEG5_STACK == pt_prv_query->ui1_source)
        {
            SLIST_INSERT_HEAD(pt_prv_query, pt_close_lst, t_link);
        }
        else /* SI_QUERY_SOURCE_GLUE_LAYER == pt_prv_query->ui1_source */
        {
            x_handle_free(pt_prv_query->h_this);
        }
    }
#if 0
    _free_deferred_query(&t_backup);
#endif
}

static BOOL _release_svc_info_resource(
    UINT16              ui2_svc_id,
    SLIST_SVC_INFO_T*   pt_si_lst)
{
    BOOL         b_ret = FALSE;
    _SVC_INFO_T* pt_svc;
    
    /* select one service info */
    pt_svc = SLIST_FIRST(pt_si_lst);
    
    while (pt_svc)
    {
        if ((ui2_svc_id != pt_svc->ui2_svc_id) &&
            (!SVC_INFO_SUBSCRIBED(pt_svc)))
        {
            _free_svc_info(pt_svc);
            b_ret = TRUE;
            break;
        }

        pt_svc = SLIST_NEXT(pt_svc, t_link);
    }
    
    return (b_ret);
}

static INT32 _search_descriptors_with_data(
    HANDLE_T        h_pmt,
    UINT8           ui1_tag,
    _PARSE_PARAM_T* pt_parse,
    UINT16*         pui2_idx,
    MPEG_2_PID_T*   pt_pid,
    UINT8*          pui1_enc_type)
{
    PSI_STREAM_INFO_T           t_stream_info;
    TM_COND_T                   e_stream_cond;
    TM_FIRST_LOOP_DESCR_SEL_T   t_first_loop_descr_sel;
    INT32                       i4_ret;
    UINT16                      ui2_idx;
    UINT8                       ui1_descr_tag = ui1_tag;
    UINT8                       ui1_flag = pt_parse->ui1_flag;
    BOOL                        b_found = FALSE;
    UINT8                       ui1_tmp = 0;
    
    t_first_loop_descr_sel.pui1_descr_tag_list = &ui1_descr_tag;
    t_first_loop_descr_sel.z_num_descr_tags    = 1;
    
    ui2_idx = 0;
    
    while (1)
    {
        i4_ret = x_psi_get_stream_entry(
            h_pmt,
            ui2_idx,
            &t_stream_info,
            &e_stream_cond);

        if (i4_ret != TMR_OK)
        {
            i4_ret = MHEG5R_PARSE_PMT_FAIL;
            break;
        }
        
        t_first_loop_descr_sel.ui2_loop_idx = ui2_idx;

        i4_ret = x_tm_parse_obj(
            h_pmt,
            TM_PARSE_TYPE_FIRST_LOOP_DESCRS,
            &t_first_loop_descr_sel,
            pt_parse,
            sizeof(_PARSE_PARAM_T),
            _find_specified_data,
            NULL);

        if (TMR_OK == i4_ret)
        {
            if (0 == ((pt_parse->ui1_flag) & (pt_parse->ui1_mask)))
            {
                b_found = TRUE;
                
                if (pui2_idx)
                {
                    *pui2_idx = ui2_idx;
                }
                
                if (pt_pid)
                {
                    *pt_pid = t_stream_info.t_pid;
                }

                if (pui1_enc_type)
                {
                    if (STREAMTYPE_VIDEO_11172_2      == t_stream_info.ui1_stream_type ||
                        STREAMTYPE_VIDEO_13818_2      == t_stream_info.ui1_stream_type ||
                        STREAMTYPE_VIDEO_DIGICIPHER_2 == t_stream_info.ui1_stream_type ||
                        STREAMTYPE_VIDEO_H264 == t_stream_info.ui1_stream_type)
                    {
                        *pui1_enc_type = _streamtype_to_vid_enc(t_stream_info.ui1_stream_type);
                    }
                    else if (STREAMTYPE_AUDIO_11172_3  == t_stream_info.ui1_stream_type ||
                             STREAMTYPE_AUDIO_13818_3  == t_stream_info.ui1_stream_type ||
                             STREAMTYPE_AUDIO_ATSC_A53 == t_stream_info.ui1_stream_type)
                    {
                        *pui1_enc_type = AUD_ENC_MPEG_2;
                    }
                    else if (STREAMTYPE_PES_PRIV_13818_1  == t_stream_info.ui1_stream_type ||
                             STREAMTYPE_AUDIO_HE_ACC == t_stream_info.ui1_stream_type)
                    {
                        TM_FIRST_LOOP_DESCR_SEL_T   t_descr;
                        AC3_AAC_AUDIO_INFO_T        t_ac3_aac;
                        UINT8                       aui1_descriptor_tag[3];
                        
                        aui1_descriptor_tag[0] = TAG_AC_3_DESCRIPTOR;
                        aui1_descriptor_tag[1] = TAG_EAC_3_DESCRIPTOR;
                        aui1_descriptor_tag[2] = TAG_AAC_DESCRIPTOR;
                        
                        t_descr.ui2_loop_idx        = ui2_idx;
                        t_descr.pui1_descr_tag_list = aui1_descriptor_tag;
                        t_descr.z_num_descr_tags    = 3;
                        
                        x_memset(&t_ac3_aac, 0, sizeof(AC3_AAC_AUDIO_INFO_T));
                            
                            x_tm_parse_obj(h_pmt,
                                           TM_PARSE_TYPE_FIRST_LOOP_DESCRS,
                                           &t_descr,
                                           &t_ac3_aac,
                                           sizeof(AC3_AAC_AUDIO_INFO_T),
                                           _parse_ac3_aac_streams,
                                           NULL);
                        
                        if (t_ac3_aac.b_found == TRUE)
                        {                        
                            *pui1_enc_type = t_ac3_aac.e_aud_enc;
                        }
                        else
                        {
                            *pui1_enc_type = 0;/*unknown*/
                        }
                    }
                    else
                    {
                        *pui1_enc_type = 0;/*unknown*/
                    }
                }

                ui1_tmp = pt_parse->ui1_flag;
            }
        }
        else
        {
            break;
        }

        pt_parse->ui1_flag = ui1_flag;
        ui2_idx++;
    }

    if (b_found)
    {
        pt_parse->ui1_flag = ui1_tmp;
        return MHEG5R_OK;
    }
    else
    {
        return MHEG5R_PARSE_PMT_FAIL;
    }
}

static INT32 _send_notify(
    UINT16      ui2_msg_type,
    UINT32      ui4_seq_num,
    HANDLE_T    h_obj,
    UINT16      ui2_on_id,
    UINT16      ui2_ts_id,
    UINT16      ui2_svc_id,
    VOID*       pv_nfy_tag)
{
    INT32       i4_ret;
    MHEG5_MSG_T t_msg;

    t_msg.ui1_module   = MHEG5_MOD_SIL;
    t_msg.ui2_msg_type = ui2_msg_type;
    t_msg.ui4_seq_num  = ui4_seq_num;
    t_msg.h_obj        = h_obj;
    t_msg.ui2_on_id    = ui2_on_id;
    t_msg.ui2_ts_id    = ui2_ts_id;
    t_msg.ui2_svc_id   = ui2_svc_id;
    t_msg.pv_nfy_tag   = pv_nfy_tag;
    
    i4_ret = x_mheg5_send_msg(_ui4_eng_code, &t_msg, MHEG5_SIL_MSG_SEND_RETRY);

    MHEG5_ASSERT(i4_ret == MHEG5R_OK);

    return (i4_ret);
}

static BOOL _si_query_free_cb(
    HANDLE_T       h_handle,
    HANDLE_TYPE_T  e_type,
    VOID*          pv_obj,
    VOID*          pv_tag,
    BOOL           b_req_handle)
{
    if (pv_obj)
    {
        SIL_MFREE(pv_obj, sizeof(_SI_QUERY_T));
    }

    return TRUE;
}

static INT32 _start_timer(
    UINT16      ui2_msg_type,
    UINT32      ui4_seq_num,
    HANDLE_T    h_table,
    UINT16      ui2_on_id,
    UINT16      ui2_ts_id,
    UINT16      ui2_svc_id,
    UINT32      ui4_delay)
{
    _SIL_TIMER_T*   pt_timer = NULL;
    HANDLE_T        h_timer  = NULL_HANDLE;
    INT32           i4_ret;
    
    i4_ret = MHEG5R_CREATE_TIMER_FAIL;
    
    if (OSR_OK == x_timer_create(&h_timer))
    {
        i4_ret = MHEG5R_OUT_OF_MEMORY;
        
        pt_timer = (_SIL_TIMER_T *) SIL_MALLOC(sizeof(_SIL_TIMER_T));

        if (pt_timer)
        {
            x_memset(pt_timer, 0, sizeof(_SIL_TIMER_T));

            pt_timer->ui2_msg_type = ui2_msg_type;
            pt_timer->h_table      = h_table;
            pt_timer->ui2_on_id    = ui2_on_id;
            pt_timer->ui2_ts_id    = ui2_ts_id;
            pt_timer->ui2_svc_id   = ui2_svc_id;
            pt_timer->ui4_seq_num  = ui4_seq_num;

            i4_ret = MHEG5R_START_TIMER_FAIL;
            
            if (OSR_OK == x_timer_start(
                h_timer,
                ui4_delay,
                X_TIMER_FLAG_ONCE,
                _tm_timer_cb,
                pt_timer))
            {
                i4_ret = MHEG5R_OK;
            }
        }
    }


    if (MHEG5R_OK != i4_ret)
    {
        if (pt_timer)
        {
            SIL_MFREE(pt_timer, sizeof(_SIL_TIMER_T));
        }
        
        if (NULL_HANDLE != h_timer)
        {
            x_timer_delete(h_timer);
        }
    }

    return (i4_ret);
}

#if 0
static NFY_RET_T _tm_load_nit_nfy(
    HANDLE_T       h_nit,
    HANDLE_TYPE_T  e_obj_type,
    TM_COND_T      e_nfy_cond,
    VOID*          pv_nfy_tag,
    UINT32         ui4_data)
{
    if (pv_nfy_tag)
    {
        if ((e_nfy_cond == TM_COND_UPDATE)    ||
            (e_nfy_cond == TM_COND_AVAILABLE) ||
            (e_nfy_cond == TM_COND_UNAVAILABLE))
        {
            _send_notify(
                (TM_COND_UNAVAILABLE == e_nfy_cond) ? SIL_MSG_NIT_UNAVAIL: SIL_MSG_NIT_UPDATE,
                ((_SIL_NOTIFY_T*) pv_nfy_tag)->ui4_seq_num,
                h_nit,
                0,
                0,
                0,
                (TM_COND_UNAVAILABLE == e_nfy_cond) ? pv_nfy_tag : NULL);
        }
    }
    
    return NFY_RET_PROCESSED;
}
#endif

static NFY_RET_T _tm_load_pat_nfy(
    HANDLE_T       h_pat,
    HANDLE_TYPE_T  e_obj_type,
    TM_COND_T      e_nfy_cond,
    VOID*          pv_nfy_tag,
    UINT32         ui4_data)
{
    if (pv_nfy_tag)
    {
        if ((e_nfy_cond == TM_COND_UPDATE)    ||
            (e_nfy_cond == TM_COND_AVAILABLE) ||
            (e_nfy_cond == TM_COND_UNAVAILABLE))
        {
            SI_DEBUG(("_tm_load_pat_nfy status e_nfy_cond =%d  TM_COND_UNAVAILABLE = 0, TM_COND_TRANSITION, TM_COND_AVAILABLE, TM_COND_UPDATE, \r\n",
                      e_nfy_cond));
            _send_notify(
                (TM_COND_UNAVAILABLE == e_nfy_cond) ? SIL_MSG_PAT_UNAVAIL: SIL_MSG_PAT_UPDATE,
                ((_SIL_NOTIFY_T*) pv_nfy_tag)->ui4_seq_num,
                h_pat,
                0,
                0,
                0,
                (TM_COND_UNAVAILABLE == e_nfy_cond) ? pv_nfy_tag : NULL);
        }
    }
    
    return NFY_RET_PROCESSED;
}

static NFY_RET_T _tm_load_pmt_nfy(
    HANDLE_T       h_pmt,
    HANDLE_TYPE_T  e_obj_type,
    TM_COND_T      e_nfy_cond,
    VOID*          pv_nfy_tag,
    UINT32         ui4_data)
{
    if (pv_nfy_tag)
    {
        if ((e_nfy_cond == TM_COND_UPDATE)    ||
            (e_nfy_cond == TM_COND_AVAILABLE) ||
            (e_nfy_cond == TM_COND_UNAVAILABLE))
        {
            SI_DEBUG(("_tm_load_pmt_nfy e_nfy_cond=%d TM_COND_UNAVAILABLE = 0, TM_COND_TRANSITION, TM_COND_AVAILABLE, TM_COND_UPDATE,\r\n",
                      e_nfy_cond));

            _send_notify(
                (TM_COND_UNAVAILABLE == e_nfy_cond) ? SIL_MSG_PMT_UNAVAIL: SIL_MSG_PMT_UPDATE,
                ((_SIL_NOTIFY_T*) pv_nfy_tag)->ui4_seq_num,
                h_pmt,
                0,
                0,
                ((_SIL_NOTIFY_T*) pv_nfy_tag)->ui2_svc_id,
                (TM_COND_UNAVAILABLE == e_nfy_cond) ? pv_nfy_tag : NULL);
        }
    }
    
    return NFY_RET_PROCESSED;
}

static NFY_RET_T _tm_load_sdt_nfy(
    HANDLE_T       h_sdt,
    HANDLE_TYPE_T  e_obj_type,
    TM_COND_T      e_nfy_cond,
    VOID*          pv_nfy_tag,
    UINT32         ui4_data)
{
    if (pv_nfy_tag)
    {
        if ((e_nfy_cond == TM_COND_UPDATE)    ||
            (e_nfy_cond == TM_COND_AVAILABLE) ||
            (e_nfy_cond == TM_COND_UNAVAILABLE))
        {
            _send_notify(
                (TM_COND_UNAVAILABLE == e_nfy_cond) ? SIL_MSG_SDT_UNAVAIL: SIL_MSG_SDT_UPDATE,
                ((_SIL_NOTIFY_T*) pv_nfy_tag)->ui4_seq_num,
                h_sdt,
                ((_SIL_NOTIFY_T*) pv_nfy_tag)->ui2_on_id,
                ((_SIL_NOTIFY_T*) pv_nfy_tag)->ui2_ts_id,
                0,
                (TM_COND_UNAVAILABLE == e_nfy_cond) ? pv_nfy_tag : NULL);
        }
    }
    
    return NFY_RET_PROCESSED;
}

static VOID _tm_timer_cb(
    HANDLE_T    h_timer,
    VOID*       pv_tag)
{
    _SIL_TIMER_T*   pt_timer;

    if (pv_tag)
    {
        pt_timer = (_SIL_TIMER_T*) pv_tag;

        pt_timer->h_this = h_timer;

        _send_notify(
            pt_timer->ui2_msg_type,
            pt_timer->ui4_seq_num,
            pt_timer->h_table,
            pt_timer->ui2_on_id,
            pt_timer->ui2_ts_id,
            pt_timer->ui2_svc_id,
            pt_timer);
    }
}

static VOID _unload(_SI_LOADER_INFO_T* pt_loader)
{
    /* delete the PAT and all PMTs */
    _free_loader_tables(pt_loader);
    
    pt_loader->ui2_on_id         = 0;
    pt_loader->ui2_ts_id         = 0;
    pt_loader->ui2_active_svc_id = 0;
    
    /* start next run */
    SEQ_NUM(pt_loader)++;

    pt_loader->e_state = LDR_ST_CLOSED;
}

static _SVC_INDEX_T* _find_svc_index(UINT32 ui4_ch_id)
{
    _SVC_INDEX_T* pt_svc_index = NULL;

    /*  Processed in caller's lock, it's safe */
    SLIST_FOR_EACH(pt_svc_index, &(SVC_INDEX_LIST(&_t_loader)), t_link)
    {
        if (pt_svc_index->ui4_channel_id == ui4_ch_id)
        {
            break;
        }
    }

    return (pt_svc_index);
}

static _SVC_INDEX_T* _new_svc_index(UINT32 ui4_ch_id)
{
    _SVC_INDEX_T* pt_svc_index = NULL;

    /*  Processed in caller's lock, it's safe */
    pt_svc_index = (_SVC_INDEX_T*)x_mem_alloc(sizeof(_SVC_INDEX_T));
    
    if (pt_svc_index != NULL)
    {
        pt_svc_index->ui4_channel_id = ui4_ch_id;
        pt_svc_index->ui4_svc_index = _ui4_svc_index++;
    }

    return (pt_svc_index);
}

static VOID _deinit_svc_index_lst()
{
    _SVC_INDEX_T* pt_svc_index;

    /*  Processed in caller's lock, it's safe */
    while(FALSE == SLIST_IS_EMPTY(&(SVC_INDEX_LIST(&_t_loader))))
    {
        pt_svc_index = (&(SVC_INDEX_LIST(&_t_loader)))->pt_head;

        if (pt_svc_index != NULL)
        {
            SLIST_REMOVE(pt_svc_index, t_link);

            x_mem_free(pt_svc_index);
        }
    }

    _ui4_svc_index = 1;
}

static VID_ENC_T _streamtype_to_vid_enc(UINT8  ui1_stream_type)
{
    VID_ENC_T t_vdec = VID_ENC_UNKNOWN;
    switch(ui1_stream_type)
    {
        case STREAMTYPE_VIDEO_11172_2:
            t_vdec = VID_ENC_MPEG_1;
            break;
        case STREAMTYPE_VIDEO_14496_2:
            t_vdec = VID_ENC_MPEG_4;
            break;
        case STREAMTYPE_VIDEO_13818_2:
        case STREAMTYPE_VIDEO_DIGICIPHER_2:
            t_vdec = VID_ENC_MPEG_2;
            break;
            
        case STREAMTYPE_VIDEO_H264:
            t_vdec = VID_ENC_H264;
            break;

        default:
            t_vdec = VID_ENC_MPEG_2;
            break;
    }

    return t_vdec;
}

INT32 sil_apply_supported_prof(
    ISO_3166_COUNT_T    s3166_country,
    MHEG5_PROFILE_ID_T* pe_profile_id,
    BOOL                b_apply)
{
    if (s3166_country == NULL || pe_profile_id == NULL)
    {
        return MHEG5R_INV_ARG;
    }
    /* UK and IRL MHEG5 is turned on by default */
    if (x_strcmp(ISO_3166_UK, (CHAR *)s3166_country) == 0)
    {
        *pe_profile_id = MHEG5_PROFILE_UK;
    }
    else if (x_strcmp(ISO_3166_IRL, (CHAR *)s3166_country) == 0)
    {
        *pe_profile_id = MHEG5_PROFILE_IRL;
    }
    else if (x_strcmp(ISO_3166_NZ, (CHAR *)s3166_country) == 0)
    {
        *pe_profile_id = MHEG5_PROFILE_NZ;
    }
    else if (x_strcmp(ISO_3166_AUS, (CHAR *)s3166_country) == 0)
    {
        *pe_profile_id = MHEG5_PROFILE_AUS;
    }
    else if (x_strcmp(ISO_3166_CHN, (CHAR *)s3166_country) == 0)
    {
        *pe_profile_id = MHEG5_PROFILE_HK;
    }
    else
    {
        *pe_profile_id = MHEG5_PROFILE_NONE;
        return MHEG5R_NO_SUPPORTED_PROFILE;
    }
    if (TRUE == b_apply)
    {
        if (MHEG5R_OK == sil_set_active_prof(*pe_profile_id))
        {
            return MHEG5R_OK;
        }
        else
        {
            return MHEG5R_INTERNAL_ERROR;
        }
    }
    else
    {
        return MHEG5R_OK;
    }
}



