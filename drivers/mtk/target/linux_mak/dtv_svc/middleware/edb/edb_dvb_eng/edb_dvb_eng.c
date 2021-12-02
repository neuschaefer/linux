/*----------------------------------------------------------------------------*
 * No Warranty                                                                *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MTK with respect to any MTK *
 * Deliverables or any use thereof, and MTK Deliverables are provided on an   *
 * "AS IS" basis.  MTK hereby expressly disclaims all such warranties,        *
 * including any implied warranties of merchantability, non-infringement and  *
 * fitness for a particular purpose and any warranties arising out of course  *
 * of performance, course of dealing or usage of trade.  Parties further      *
 * acknowledge that Company may, either presently and/or in the future,       *
 * instruct MTK to assist it in the development and the implementation, in    *
 * accordance with Company's designs, of certain softwares relating to        *
 * Company's product(s) (the "Services").  Except as may be otherwise agreed  *
 * to in writing, no warranties of any kind, whether express or implied, are  *
 * given by MTK with respect to the Services provided, and the Services are   *
 * provided on an "AS IS" basis.  Company further acknowledges that the       *
 * Services may contain errors, that testing is important and Company is      *
 * solely responsible for fully testing the Services and/or derivatives       *
 * thereof before they are used, sublicensed or distributed.  Should there be *
 * any third party action brought against MTK, arising out of or relating to  *
 * the Services, Company agree to fully indemnify and hold MTK harmless.      *
 * If the parties mutually agree to enter into or continue a business         *
 * relationship or other arrangement, the terms and conditions set forth      *
 * hereunder shall remain effective and, unless explicitly stated otherwise,  *
 * shall prevail in the event of a conflict in the terms in any agreements    *
 * entered into between the parties.                                          *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Copyright (c) 2006, MediaTek Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *-----------------------------------------------------------------------------
 * $RCSfile: ,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         
 *---------------------------------------------------------------------------*/


#include "inc/x_common.h"
#include "os/inc/x_os.h"
#include "hash/hash_api.h"
#include "edb/edb_hash_api.h"
#include "handle/handle.h"
#include "tbl_mngr/dvb_si_eng/x_dvb_si_eng.h"
#include "tbl_mngr/dvb_si_eng/dvb_si_txt_api.h"
#include "tbl_mngr/tm_api.h"
#include "tbl_mngr/tm.h"
#include "tbl_mngr/u_tm.h"
#include "dbg/x_dbg.h"
#include "edb/edb_slctr.h"
#include "edb/edb_pool.h"
#include "edb/x_edb.h"
#include "edb/u_edb_eng.h"
#ifdef MW_EPG_CACHE_NVM_SUPPORT
#include "edb/edb_flash.h"
#endif
#include "svl_bldr/u_sb.h"
#include "svl/x_svl.h"
#include "tsl/x_tsl.h"
#include "edb_dvb_section.h"
#include "util/x_sort_search.h"
#include "dt/x_dt.h"

#include "edb_dvb_eng.h"

#include "chip_spec/x_chip_spec.h"
#include "chip_spec/u_chip_spec.h"

#define EDB_DVB_MSGQ_NAME               "edb_dvb_msgq"
#define EDB_DVB_DEFAULT_NUM_OF_MSGS     ((UINT16)   160*8)  
#define EDB_DVB_LOG(x)                  x_dbg_stmt x





/* message type definition */
typedef UINT32 EDB_DVB_MSG_TYPE;

#define EDB_DVB_MSG_UNKNOWN             ((EDB_DVB_MSG_TYPE) 0)
#define EDB_DVB_MSG_ROOT_NFY            ((EDB_DVB_MSG_TYPE) 1)
#define EDB_DVB_MSG_ACTIVE_NFY          ((EDB_DVB_MSG_TYPE) 2) /* active notify */
#define EDB_DVB_MSG_EXPIRE_NFY          ((EDB_DVB_MSG_TYPE) 3) /* expired notify */
#define EDB_DVB_MSG_CHECK_EXPIRE_NFY    ((EDB_DVB_MSG_TYPE) 4) /* 3 hour check expire events */
#define EDB_DVB_MSG_DT_NFY              ((EDB_DVB_MSG_TYPE) 5) /* date/time notifiy */
#define EDB_DVB_MSG_CHANGE_CHANNEL_NFY  ((EDB_DVB_MSG_TYPE) 6) /* change channel notify*/
#define EDB_DVB_MSG_SVL_UPDATE_NFY      ((EDB_DVB_MSG_TYPE) 7) /* */
#define EDB_DVB_MSG_ENABLE_EDB          ((EDB_DVB_MSG_TYPE) 8) /* */
#define EDB_DVB_MSG_RESTART_EDB         ((EDB_DVB_MSG_TYPE) 9) /* */
#define EDB_DVB_MSG_TUNER_CHANGE        ((EDB_DVB_MSG_TYPE) 10)/* */
#define EDB_DVB_MSG_POOL_FULL           ((EDB_DVB_MSG_TYPE) 11)/* */
#define EDB_DVB_MSG_START_PARSE_SDT_DESC ((EDB_DVB_MSG_TYPE) 12)
#define EDB_DVB_MSG_SET_RISKTV          ((EDB_DVB_MSG_TYPE) 13)
#define EDB_DVB_MSG_CHANNEL_SCAN        ((EDB_DVB_MSG_TYPE) 14) /* */


#define EDB_DVB_MSG_FAKE_AVAIL_NFY      ((EDB_DVB_MSG_TYPE) 30)

#define EDB_DVB_PF_NFY_DT               ((UINT8)0x1)
#define EDB_DVB_PF_NFY_CHANGE_CHANNEL   ((UINT8)0x2)
#define EDB_DVB_CHECK_EXPIRE_TIMER      ((UINT32)3*60*60)



/*Copy from u_sb_dvbc_eng.h*/
#define DVBC_OPERATOR_NAME_OTHERS          ((UINT32) 0)
#define DVBC_OPERATOR_NAME_UPC             ((UINT32) 1)
#define DVBC_OPERATOR_NAME_COMHEM          ((UINT32) 2)
#define DVBC_OPERATOR_NAME_CANAL_DIGITAL   ((UINT32) 3)
#define DVBC_OPERATOR_NAME_TELE2           ((UINT32) 4)
#define DVBC_OPERATOR_NAME_STOFA           ((UINT32) 5)
#define DVBC_OPERATOR_NAME_YOUSEE          ((UINT32) 6)
#define DVBC_OPERATOR_NAME_ZIGGO           ((UINT32) 7)
#define DVBC_OPERATOR_NAME_UNITYMEDIA      ((UINT32) 8)


#define EDB_THRESHOLD      ((TIME_T)   6) /* seconds */


typedef struct _EDB_DVB_MSG_T
{
    EDB_DVB_MSG_TYPE        e_msg_type;
    UINT16                  ui2_flags;
    TIME_T                  t_delta;
    UINT32                  ui4_data;
    VOID*                   pv_tag;
    CHAR                    ps_name[EDB_MAX_TUNER_NAME];/*use for tuner name*/
} EDB_DVB_MSG_T;

typedef struct _HASH_PARSE_TAG
{
    VOID*                   pv_obj1;
    VOID*                   pv_obj2;
    VOID*                   pv_obj3;
    VOID*                   pv_obj4;
    VOID*                   pv_obj5;
	VOID*                   pv_obj6;
}HASH_PARSE_TAG;

typedef struct _CH_KEY
{
    BRDCST_TYPE_T           e_type;
    UINT16                  ui2_svl_id;
}CH_KEY;

typedef struct _EDB_PARSE_CHANNEL_T
{
    HANDLE_T                    h_edb_eng;
    EDB_EVENT_POOL_CH_KEY        t_locate;  
} EDB_PARSE_CHANNEL_T;

/*UINT16          g_edb_monitor_on_id ;*/
/*UINT16          g_edb_monitor_ts_id ;*/
/*UINT16          g_edb_monitor_svc_id ;*/





static INT32  _edb_dvb_start_cache(HANDLE_T h_edb_eng);
static INT32  _edb_dvb_stop_cache(HANDLE_T h_edb_eng);


static INT32 _edb_dvb_enable_edb(HANDLE_T h_edb_eng,BOOL fg_enabled);
static INT32 _edb_dvb_load_table(HANDLE_T h_edb_eng);
static INT32 _edb_dvb_free_table(HANDLE_T h_edb_eng);
static INT32 _edb_dvb_load_pf(HANDLE_T h_edb_eng);
static INT32 _edb_dvb_load_sdt(HANDLE_T     h_edb_eng);

static INT32 _edb_dvb_load_s(HANDLE_T h_edb_eng);
static BOOL  _edb_dvb_pre_process_section(
        DVB_ENG_DATA*           pt_eng_data,
        EDB_EVENT_POOL_CH_KEY*  pt_key,
        UINT8                   ui1_tbl_id,
        UINT8                   ui1_sect_num,
        UINT8                   ui1_version,
        UINT8**                 ppui1_sec_mask,
        UINT8*                  pui1_mask);


static INT32 _edb_dvb_add_eng_data_obj(EDB_ENG_CTRL_ENTRY_T* pt_eng_ctrl_entry,DVB_ENG_DATA* pt_eng_data);



static INT32 _edb_dvb_get_ch_locate_obj(DVB_ENG_DATA*           pt_eng_data,UINT32   ui4_channel_id,EDB_EVENT_POOL_CH_KEY** ppt_locate);
static INT32 _edb_dvb_add_ch_locate_obj(DVB_ENG_DATA*           pt_eng_data,UINT32   ui4_channel_id,EDB_EVENT_POOL_CH_KEY* pt_locate);
static INT32 _edb_dvb_clean_ch_locate_list(HANDLE_T h_edb_eng);
static INT32 _edb_set_edb_with_command (HANDLE_T h_edb_eng,DVB_ENG_DATA* pt_eng_data,EDB_COMMAND_T* pt_cmd);
static INT32 _edb_dvb_build_ch_locate(HANDLE_T h_edb_eng);
static INT32 _edb_dvb_set_enable_edb(HANDLE_T h_edb_eng,BOOL fg_flag);
#ifdef    MW_EDB_FBM_SHARE_MEM
static INT32 _edb_dvb_set_risktv(HANDLE_T h_edb_eng,BOOL fg_flag); 

#endif
static INT32 _edb_dvb_set_channel_scan(HANDLE_T h_edb_eng,BOOL fg_flag);
static INT32 _edb_dvb_set_restart_edb(HANDLE_T h_edb_eng);
static INT32 _edb_dvb_set_tuner_change(HANDLE_T h_edb_eng,EDB_ENG_OPEN_DATA_T*            pt_eng_open_data);


static INT32 _edb_dvb_set_current_service(HANDLE_T h_edb_eng,EDB_CH_ID_T* pt_ch_id);
static BOOL _dvb_country_code_match (
        ISO_3166_COUNT_T*   pt_country_code,
        ISO_3166_COUNT_T*   ps_default_country);

static INT32 _edb_dvb_gen_fake_event(
        EDB_POOL_EVENT_INFO_T** ppt_fake_event,
        TIME_T                  t_start_time,
        TIME_T                  t_duration
        );

static INT32 _edb_dvb_get_fake_event(
        UINT16      ui2_event_id,
        EDB_POOL_EVENT_INFO_T** ppt_fake_event
        );

static INT32 _edb_dvb_process_dt_nfy(EDB_DVB_MSG_T t_msg);


static INT32 _edb_dvb_start_check_expire_timer(
        HANDLE_T                h_edb_eng
        );

static INT32 _edb_dvb_process_event_detail(
        HANDLE_T            h_edb_eng,
        VOID*               pv_key_info,            /* EDB_CH_EVT_ID_T*          */
        SIZE_T*             pz_event_info_len,     /* SIZE_T*  */ 
        VOID*               pv_event_info,          /* CHAR* */
        EDB_COND_T*         pe_cond 
        );

static INT32 _edb_dvb_copy_single_event(
        HANDLE_T                h_edb_eng,
        EDB_EVENT_POOL_CH_T*    pt_channel,
        EDB_POOL_EVENT_INFO_T*  pt_pool_event,
        EDB_EVENT_INFO_T*       pt_event_info
        );

INT32 edb_dvb_restart(VOID);

static INT32 _edb_dvb_recreate_pool(HANDLE_T h_edb_eng);


static  CHAR* skip_spaces (CHAR*  ps_str);

static HANDLE_T                 h_eng_data_hash     = NULL_HANDLE;
static HANDLE_T                 h_dvb_msg_q         = NULL_HANDLE;
EDB_DVB_CONFIG_T                t_eng_cfg_flag      = 0;
static EDB_POOL_EVENT_INFO_T*   apt_pool_event[255]  = {0};
static UINT8                    ui1_apt_pool_event_num = 0;

static UINT16					ui2_filter_meet_num = 0;
static UINT16					ui2_desired_string_meet_num = 0;


static EDB_POOL_EVENT_INFO_T    apt_pool_fake_event[MAX_EMPTY_CELL_SUPPORT]         = {{0}};
static UINT8                    ui1_fake_event_idx                                  = 0;
extern CHAR                     s_event_detail_buffer[EDB_EVENT_DETAIL_BUFFER];
static UINT8    aui1_detail_with_space[EDB_TRANCATE_SPACE_BUFFER] = {0x0};

static UINT32            ui4_service_private_data_specifier                       = 0; 

static BOOL              b_edb_is_in_channel_scan = FALSE;

extern HANDLE_T    g_edb_handle_hash;/*EDB_T list*/
extern HANDLE_T    g_edb_client_hash;/*BASE_CTRL_T list*/

extern INT32 x_cli_parser(const CHAR* ps_cmd);

extern edb_restart_fct edb_dvb_restart_fct;

#ifdef    MW_EDB_FBM_SHARE_MEM
static UINT8           ui1_edb_from_fbm_size = 0;
#endif

#ifdef CLI_SUPPORT

static CHAR      s_time_log[100] = {0};
static CHAR*    edb_dvb_dbg_stmt_local_time(CHAR* s_pre,TIME_T t_time)
{
    DTG_T     t_dtg;
    DTG_T     t_local_dtg;
    x_memset(s_time_log,0x0,100);
    x_dt_utc_sec_to_dtg(t_time, &t_dtg);
    x_dt_conv_utc_local(&t_dtg, &t_local_dtg);
    x_sprintf(s_time_log,"%d/%d/%d %d:%d:%d ",
              t_local_dtg.ui2_yr,
              t_local_dtg.ui1_mo,
              t_local_dtg.ui1_day,
              t_local_dtg.ui1_hr,
              t_local_dtg.ui1_min,
              t_local_dtg.ui1_sec);
    x_dbg_stmt("%s %s",s_pre,s_time_log);
    return s_time_log;
}

#define DBG_STMT_TIME   edb_dvb_dbg_stmt_local_time

#endif


/*#define MW_EDB_DVB_ENG_MSG_CNT*/
#ifdef  MW_EDB_DVB_ENG_MSG_CNT
static INT32 _dvb_msg_q_send(HANDLE_T       h_msg_hdl,
                             const VOID*    pv_msg,
                             SIZE_T         z_size,
                             UINT8          ui1_pri)
{
    INT32   i4_ret;
    UINT16  ui2_msg_cnt = 0;

    i4_ret = x_msg_q_send(h_msg_hdl, pv_msg, z_size, ui1_pri);

    if (x_msg_q_num_msgs (h_msg_hdl, &ui2_msg_cnt) == OSR_OK)
    {

        if (ui2_msg_cnt >= (UINT16)10 )
        {

            x_dbg_stmt(_EDB_INFO"Number of msgs [%u] in queue handle [%x] [send msg return %d]\n",ui2_msg_cnt, h_msg_hdl, i4_ret);
            /*x_cli_parser("mw.os.sp");*/
        }
    }
    else
    {
        DBG_INFO((_EDB_INFO"Failed to get number of msgs in queue handle [%x]\n", h_msg_hdl));
    }

    return i4_ret;
}
#define DVB_MSG_Q_SEND      _dvb_msg_q_send
#else
#define DVB_MSG_Q_SEND      x_msg_q_send
#endif


static  CHAR* skip_spaces (CHAR*  ps_str)
{
    CHAR  c_char;

    while (((c_char = *ps_str) != '\0')
           &&
           (
                   (c_char == ' ')
                   /*|| (c_char == '\n')*/
           )
          )
    {
        ps_str++;
    }

    return ps_str;
}

static INT32 _edb_dvb_gen_fake_event(
        EDB_POOL_EVENT_INFO_T** ppt_fake_event,
        TIME_T                  t_start_time,
        TIME_T                  t_duration
        )
{
    INT32                   i4_ret          = EDBR_INFO_NOT_FOUND;
    EDB_POOL_EVENT_INFO_T*  pt_event_info   = NULL;
    EDB_CHECK_RET( 
            (ppt_fake_event == NULL),
            (i4_ret),
            ((_EDB_INFO"Invalid argument\r\n"))
            );
    ui1_fake_event_idx++;
    if (ui1_fake_event_idx >= MAX_EMPTY_CELL_SUPPORT)
    {
        ui1_fake_event_idx = (UINT8)0x0;
        /*DBG_ABORT( DBG_MOD_EDB );*/
    }


    pt_event_info = &(apt_pool_fake_event[ui1_fake_event_idx]);
    x_memset(pt_event_info,0x0,sizeof(EDB_POOL_EVENT_INFO_T));
    pt_event_info->ui2_event_id = EVENT_EMPTY_CELL_START_EVENT_ID + (UINT16)ui1_fake_event_idx;
    pt_event_info->t_start_time = t_start_time;
    pt_event_info->t_duration   = t_duration;
    *ppt_fake_event             = pt_event_info;


    i4_ret = EDBR_OK;
    return i4_ret;
}


static INT32 _edb_dvb_get_fake_event(
        UINT16      ui2_event_id,
        EDB_POOL_EVENT_INFO_T** ppt_fake_event
        )
{
    INT32       i4_ret  = EDBR_INFO_NOT_FOUND;
    INT32       i       = 0x0;

    EDB_CHECK_RET( 
            (ppt_fake_event == NULL),
            (i4_ret),
            ((_EDB_INFO"Invalid argument\r\n"))
            );

    for (i = 0; i<MAX_EMPTY_CELL_SUPPORT;i++ )
    {
        if (apt_pool_fake_event[i].ui2_event_id == ui2_event_id)
        {
            (*ppt_fake_event) = &(apt_pool_fake_event[i]);
            i4_ret            = EDBR_OK;
            break;
        }
    }


    return i4_ret;
}



static VOID _edb_dt_cond_change_nfy(
        HANDLE_T        h_hdl,
        VOID*           pv_tag,
        DT_COND_T       e_dt_cond,
        TIME_T          t_delta)
{
    EDB_DVB_MSG_T       t_msg       = {0};
    INT32               i4_retry    = 0x0;

    EDB_CHECK_RET( 
            (pv_tag == NULL),
            ,
            ((_EDB_INFO"Invalid argument\r\n"))
            );


    //Send msg switch thread
    DBG_INFO((_EDB_INFO"_edb_dt_cond_change_nfy\r\n"));
    t_msg.e_msg_type            = EDB_DVB_MSG_DT_NFY;
    t_msg.ui4_data              = (UINT32)e_dt_cond;
    t_msg.pv_tag                = (VOID*)pv_tag; /*handle of edb engine*/


    while ( ++i4_retry <= 10 )
    {
        if (x_msg_q_send(h_dvb_msg_q,
                         &t_msg,
                         sizeof(EDB_DVB_MSG_T),
                         0 /* highest priority */) != OSR_OK)
        {
            /* Send message fail, wait 10 ms and retry for 10 times */
            x_thread_delay( 10 );
            continue;
        }
        edb_activate_eng_task(EDB_DVB_BRDCST_GRP);

        return ;
    }


    switch (e_dt_cond)
    {
        case DT_SYNC_RUNNING:
            DBG_INFO((_EDB_INFO"Date/Time notification: SYNC_RUNNING...\n\r"));
            break;

        case DT_FREE_RUNNING:
            DBG_INFO((_EDB_INFO"Date/Time notification: FREE_RUNNING...\n\r"));
            break;

        case DT_SYNC_DISCONT:
            DBG_INFO((_EDB_INFO"Date/Time notification: SYNC_DISCONT (%lld seconds)...\n\r", t_delta));
            break;

        default:
            DBG_INFO((_EDB_INFO"Date/Time notification: unknown DT condition %d...\n\r", e_dt_cond));
            break;
    }
}


static INT32    _edb_dvb_build_ch_locate(HANDLE_T h_edb_eng)
{
    INT32                       i4_ret                  = EDBR_OK;
    UINT16                      ui2_rec_num             = 0;
    UINT32                      ui4_ver_id              = 0;
    UINT16                      ui2_idx                 = 0;
    SVL_REC_T                   t_svl_rec               = {0};
    UINT16                      ui2_tsl_id              = 0;
    UINT16                      ui2_tsl_rec_id          = 0;
    UINT32                      ui4_channel_id          = 0;
    TSL_REC_T                   t_tsl_rec               = {0};
    UINT32                      ui4_tsl_ver_id          = 0;
    HANDLE_T                    h_tsl                   = NULL_HANDLE;
    EDB_EVENT_POOL_CH_KEY*      pt_locate               = NULL;
    DVB_ENG_DATA*               pt_eng_data             = NULL;
    EDB_EVENT_POOL_CH_T*        pt_event_pool_ch        = NULL;
    EDB_EVENT_POOL_CH_VER*      pt_event_pool_ch_ver    = NULL;
    EDB_EVENT_POOL_T*           pt_edb_event_pool       = NULL;
    UINT32                      i                       = 0x0;
    BOOL                        fg_svl_locked           = FALSE;
    INT32                       i4_cnt                  = 0 ;


    EDB_CHECK_RET( 
            (h_edb_eng == NULL_HANDLE),
            (EDBR_INV_ARG),
            ((_EDB_INFO"Invalid argument\r\n"))
            );

    i4_ret = edb_dvb_get_eng_data_from_ctrl(h_edb_eng,(VOID**)&pt_eng_data);
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK || pt_eng_data == NULL|| pt_eng_data->pt_edb== NULL),
            (EDBR_INV_ARG),
            ((_EDB_INFO"Can not get engine data from client\r\n"))
            );

    EDB_CHECK_RET( 
            (pt_eng_data->h_edb_inter_ch_locate == NULL_HANDLE ),
            (EDBR_INV_ARG),
            ((_EDB_INFO"Invalid argument\r\n"))
            );


    DBG_INFO((_EDB_INFO"Begin build ch locate map from SVL TSL\r\n"));
    pt_edb_event_pool =  &(pt_eng_data->pt_edb->t_edb_event_pool);
    if (pt_edb_event_pool == NULL){return EDBR_INTERNAL_ERROR;}

    //x_svl_read_lock(pt_eng_data->h_svl);
    if (i4_cnt>=0x1 && pt_eng_data->h_svl != NULL_HANDLE)
    {
        DBG_ABORT(DBG_MOD_EDB);
    }

    if (fg_svl_locked == FALSE && pt_eng_data->h_svl != NULL_HANDLE)
    {
        i4_ret = x_svl_read_lock(pt_eng_data->h_svl );
        if (i4_ret == SVLR_OK)
        {
            fg_svl_locked = TRUE;
            i4_cnt++;
        }
        else
        {
            return EDBR_INTERNAL_ERROR;
        }
    }
    //x_dbg_stmt("svl lock=%d handle=%d\r\n",i4_ret,pt_eng_data->h_svl);
    EDB_CHECK_GOTO( 
            (i4_ret !=  SVLR_OK ),
            ERROR,
            ((_EDB_INFO"lock fail\r\n"))
            );
        
    
    i4_ret = x_svl_get_num_rec_by_brdcst_type(pt_eng_data->h_svl,
                                              BRDCST_TYPE_UNKNOWN,
                                              0xFFFFFFFF,
                                              &ui2_rec_num,
                                              &ui4_ver_id);

    if (ui2_rec_num <= 0)
    {
        i4_ret = EDBR_INFO_NOT_FOUND;
    }

    EDB_CHECK_GOTO( 
            (i4_ret < SVLR_OK || ui2_rec_num <= 0),
            ERROR,
            ((_EDB_INFO"Get svl number fail\r\n"))
            );


    
    DBG_INFO((_EDB_INFO"Get record number from svl = %d\r\n",ui2_rec_num));
    
    for(ui2_idx = 0 ; ui2_idx < ui2_rec_num ; ui2_idx++)
    {
        /* Get the record */
        i4_ret = x_svl_get_rec_by_brdcst_type(pt_eng_data->h_svl,
                                              BRDCST_TYPE_UNKNOWN,
                                              0xFFFFFFFF,
                                              ui2_idx,
                                              &t_svl_rec,
                                              &ui4_ver_id);

        /* SVL Update may happened, just bypass it. */
        if (i4_ret < SVLR_OK)
        {
            continue;  /* Search next available channel. */
        }

        /* Check the SVL_REC's TSL_REC_ID && Cache's focus TSL_REC_ID */
        ui4_channel_id = t_svl_rec.uheader.t_rec_hdr.ui4_channel_id;
        ui2_tsl_id   = t_svl_rec.uheader.t_rec_hdr.ui2_tsl_id;
        ui2_tsl_rec_id = t_svl_rec.uheader.t_rec_hdr.ui2_tsl_rec_id;


        i4_ret = x_tsl_open(ui2_tsl_id,NULL, NULL, &h_tsl);
        if (i4_ret < TSLR_OK )
        {
            continue;
        }

        i4_ret = x_tsl_get_rec (h_tsl,
                                ui2_tsl_rec_id,
                                &t_tsl_rec,
                                & ui4_tsl_ver_id);

        if(i4_ret < TSLR_OK)
        {
            DBG_ERROR(("Can't Get TSL Record.\r\n"));
        }

        if (h_tsl!= NULL_HANDLE)
        {
            x_tsl_close(h_tsl);
        }


        if (pt_locate == NULL)
        { 
            pt_locate = EDB_OS_ALLOC(sizeof(EDB_EVENT_POOL_CH_KEY));
            EDB_CHECK_GOTO( 
            (pt_locate == NULL),
            ERROR,
            ((_EDB_INFO"outof memory\r\n"))
            );

            x_memset(pt_locate,0x0,sizeof(EDB_EVENT_POOL_CH_KEY));
            pt_locate->ui2_svl_id       = pt_eng_data->ui2_svl_id;
            pt_locate->ui2_on_id        = t_tsl_rec.uheader.t_desc.ui2_on_id;
            pt_locate->ui2_ts_id        = t_tsl_rec.uheader.t_desc.ui2_ts_id;
            pt_locate->ui2_svc_id       = t_svl_rec.uheader.t_rec_hdr.ui2_prog_id;

            i4_ret = _edb_dvb_add_ch_locate_obj(pt_eng_data,ui4_channel_id,pt_locate);
            if (i4_ret != EDBR_OK)
            {
                DBG_INFO((_EDB_INFO"_edb_dvb_add_ch_locate_obj %d\r\n",i4_ret));
            }

            i4_ret = edb_pool_get_ch_obj(pt_edb_event_pool, pt_locate,&pt_event_pool_ch); 
            if (pt_event_pool_ch == NULL || i4_ret != EDBR_OK)
            {
                pt_event_pool_ch = EDB_PARTITION_ALLOC(sizeof(EDB_EVENT_POOL_CH_T));
                EDB_CHECK_GOTO( 
                (pt_event_pool_ch == NULL),
                ERROR,
                ((_EDB_INFO"outof memory\r\n"))
                );
                x_memset(pt_event_pool_ch,0x0,sizeof(EDB_EVENT_POOL_CH_T));
                pt_event_pool_ch->pv_special_data          = (VOID*)((UINT32)(BOOL)t_svl_rec.u_data.t_dvb.b_eit_pf_flag);
                i4_ret = edb_hash_create(EDB_POOL_CHANNEL_HASH_EVENT_TABLE_SIZE,&(pt_event_pool_ch->h_event_hash));
                EDB_CHECK_GOTO( 
                        (i4_ret != HASH_OK),
                        ERROR,
                        ((_EDB_INFO"Can not init channel event hash table\r\n"))
                        );
                pt_event_pool_ch_ver = EDB_PARTITION_ALLOC(sizeof(EDB_EVENT_POOL_CH_VER));
                EDB_CHECK_GOTO( 
                (pt_event_pool_ch_ver == NULL),
                ERROR,
                ((_EDB_INFO"outof memory\r\n"))
                );
                
                x_memset(pt_event_pool_ch_ver,0x0,sizeof(EDB_EVENT_POOL_CH_VER));

                for(i=0 ; i<2; i++)
                {
                    pt_event_pool_ch_ver->t_pf_tbl[i].ui1_version = 0xFF;
                }
                for(i=0 ; i<EDB_MAX_PER_CH_TBL; i++)
                {
                    pt_event_pool_ch_ver->t_s_act_tbl[i].ui1_version = 0xFF;
                    pt_event_pool_ch_ver->t_s_oth_tbl[i].ui1_version = 0xFF;
                }



                pt_event_pool_ch->pv_data           = pt_event_pool_ch_ver;
                pt_event_pool_ch->t_key             = *pt_locate;
                pt_event_pool_ch->ui2_svl_id        = pt_eng_data->ui2_svl_id;
                pt_event_pool_ch->ui4_channel_id    = ui4_channel_id;
                pt_event_pool_ch->ui4_time_stamp    = x_os_get_sys_tick();
                pt_event_pool_ch->pf_free_event     = edb_dvb_free_single_event_obj;

#ifdef CLI_SUPPORT
                x_strncpy(pt_event_pool_ch->ac_name,t_svl_rec.uheader.t_rec_hdr.ac_name,MAX_PROG_NAME_LEN);
                DBG_INFO((_EDB_INFO"ui4_channel_id=0x%08x  service_name  %s\r\n",
                          ui4_channel_id,
                          pt_event_pool_ch->ac_name));
#endif
                i4_ret = edb_pool_add_ch_obj(pt_edb_event_pool, pt_event_pool_ch);
             
                EDB_CHECK_GOTO( 
                        (i4_ret != HASH_OK),
                        ERROR,
                        ((_EDB_INFO"Can not add channel into pool\r\n"))
                        );

                pt_edb_event_pool->ui2_total_channels++;
                pt_event_pool_ch->ui4_time_stamp = x_os_get_sys_tick();
                pt_event_pool_ch = NULL;
                i4_ret = EDBR_OK;
            }
            else
            {
                pt_event_pool_ch->pv_special_data          = (VOID*)((UINT32)(BOOL)t_svl_rec.u_data.t_dvb.b_eit_pf_flag);
                pt_event_pool_ch->ui4_channel_id           = ui4_channel_id;
            }

            //          DBG_INFO((_EDB_INFO"edb_pool_get_ch_obj %d\r\n",i4_ret));

            pt_locate = NULL;
        }
    }
    DBG_INFO((_EDB_INFO"End build ch locate map from SVL TSL\r\n"));
    //x_svl_read_unlock(pt_eng_data->h_svl);
    if (fg_svl_locked == TRUE && pt_eng_data->h_svl != NULL_HANDLE)
    {
        i4_ret = x_svl_read_unlock(pt_eng_data->h_svl);
        i4_cnt --;
        fg_svl_locked = FALSE;
    }
    //x_dbg_stmt("svl unlock=%d handle=%d\r\n",i4_ret,pt_eng_data->h_svl);


    return i4_ret;
ERROR:
    if (h_tsl!= NULL_HANDLE)
    {
        x_tsl_close(h_tsl);
    }

    //x_svl_read_unlock(pt_eng_data->h_svl);
    if (fg_svl_locked == TRUE && pt_eng_data->h_svl != NULL_HANDLE)
    {
        i4_ret = x_svl_read_unlock(pt_eng_data->h_svl);
        i4_cnt--;
        fg_svl_locked = FALSE;
    }
    //x_dbg_stmt("svl unlock=%d handle=%d\r\n",i4_ret,pt_eng_data->h_svl);
    return i4_ret;
}

static NFY_RET_T _tm_open_brdcst_root_nfy(
        HANDLE_T            h_root,
        HANDLE_TYPE_T       e_obj_type,
        TM_COND_T           e_nfy_cond,
        VOID*               pv_nfy_tag,
        UINT32              ui4_data)
{
    INT32                i4_retry = 0;
    EDB_DVB_MSG_T        t_msg    = {0};

    if (e_nfy_cond != TM_COND_AVAILABLE &&
        e_nfy_cond != TM_COND_UNAVAILABLE &&
        e_nfy_cond != TM_COND_SRC_DISC    &&
        e_nfy_cond != TM_COND_SRC_CONN)
    {
        return NFY_RET_PROCESSED;
    }

    DBG_INFO((_EDB_INFO"_tm_open_brdcst_root_nfy e_nfy_cond=%d\r\n",e_nfy_cond));
    t_msg.e_msg_type            = EDB_DVB_MSG_ROOT_NFY;
    t_msg.ui4_data              = (UINT32)e_nfy_cond;
    t_msg.pv_tag                = pv_nfy_tag;

    while ( ++i4_retry <= 10 )
    {
        if (x_msg_q_send(h_dvb_msg_q,
                         &t_msg,
                         sizeof(EDB_DVB_MSG_T),
                         0 /* highest priority */) != OSR_OK)
        {
            /* Send message fail, wait 10 ms and retry for 10 times */
            x_thread_delay( 10 );
            continue;
        }
        edb_activate_eng_task(EDB_DVB_BRDCST_GRP);

        return NFY_RET_PROCESSED;
    }

    return NFY_RET_PROCESSED;
}

static INT32 _edb_dvb_recreate_pool(HANDLE_T h_edb_eng)
{
    INT32                       i4_ret      = 0x0;
    DVB_ENG_DATA*               pt_eng_data = NULL;

    i4_ret = edb_dvb_get_eng_data_from_ctrl(h_edb_eng,(VOID**)&pt_eng_data);
        
    if (i4_ret == EDBR_OK && pt_eng_data != NULL)
    {
        EDB_T*                pt_edb = NULL; 
        if (pt_eng_data->pt_edb != NULL)
        {
            edb_pool_lock_wr();
            pt_edb = pt_eng_data->pt_edb ;
            edb_pool_delete(&(pt_edb->t_edb_event_pool));

            i4_ret = edb_pool_create(
                    EDB_POOL_HASH_CHANNEL_TABLE_SIZE,
                    x_dt_get_brdcst_utc(NULL,NULL),
                    &(pt_edb->t_edb_event_pool)
                    );
            if (i4_ret != EDBR_OK)
            {
                DBG_INFO(("edb_pool_create Error ui2_total_channels_cap=%d\r\n",EDB_POOL_HASH_CHANNEL_TABLE_SIZE));
            }
            edb_pool_unlock();
        }
        else
        {
            i4_ret = EDBR_INTERNAL_ERROR;
        }
    }

    return i4_ret;

}

static INT32  _edb_dvb_start_cache(HANDLE_T h_edb_eng)
{
    INT32               i4_ret      = EDBR_OK;
    TM_COND_T           e_cond      = TM_COND_UNAVAILABLE;
    DVB_ENG_DATA*       pt_eng_data = NULL;
    CHAR                s_root_name[30] = {0};

    EDB_CHECK_RET( 
            (h_edb_eng == NULL_HANDLE),
            (EDBR_INV_ARG),
            ((_EDB_INFO"Invalid argument\r\n"))
            );

    i4_ret = edb_dvb_get_eng_data_from_ctrl(h_edb_eng,(VOID**)&pt_eng_data);
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK || pt_eng_data == NULL),
            (EDBR_INV_ARG),
            ((_EDB_INFO"Can not get engine data from client\r\n"))
            );


    /*If already enable then return OK*/
    if (pt_eng_data->t_edb_eng_state.fg_enable == TRUE)
    {
        DBG_INFO((_EDB_INFO"Already started,return\r\n"));
        return EDBR_OK;
    }


    /*Start build internal channel_id and on_id ts_id service_id mapping*/
    i4_ret = _edb_dvb_clean_ch_locate_list(h_edb_eng);
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK),
            (EDBR_INTERNAL_ERROR),
            ((_EDB_INFO"Reset internal channel id and locate list fail\r\n"))
            );

    DBG_INFO((_EDB_INFO"Begin build fron SVL TSL\r\n"));
    i4_ret = _edb_dvb_build_ch_locate(h_edb_eng);
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK),
            (EDBR_INTERNAL_ERROR),
            ((_EDB_INFO"Build internal ch locate list fail\r\n"))
            );
    DBG_INFO((_EDB_INFO"End build fron SVL TSL\r\n"));
    /*End build internal channel_id and on_id ts_id service_id mapping*/

    if (pt_eng_data->h_root != NULL_HANDLE)
    {
#if 1
        i4_ret = _edb_dvb_free_table(h_edb_eng);
        i4_ret = x_tm_free(pt_eng_data->h_root);
        if (i4_ret == TMR_OK)
        {
            pt_eng_data->h_root = NULL_HANDLE;
        }
        else
        {
            x_dbg_stmt("x_tm_free root fail %d\r\n",i4_ret);
        }
#else

        x_dbg_stmt("h_root valid,return\r\n");

        /*Load table*/
        i4_ret = _edb_dvb_load_table(h_edb_eng);
        x_dbg_stmt(_EDB_INFO"_edb_dvb_load_table Ok!\r\n");
        if (i4_ret == EDBR_OK)
        {
            pt_eng_data->t_edb_eng_state.fg_enable = TRUE;
        }
        return i4_ret;

#endif
    }


    /*Open root handle*/
    x_sprintf(s_root_name, "%s_%s", "EDB",pt_eng_data->ac_tuner_name);
    x_dbg_stmt("open root %s\r\n",s_root_name);
    i4_ret = x_tm_open_root(s_root_name,
                            TM_SRC_TYPE_MPEG_2_BRDCST,
                            (VOID*) (pt_eng_data->ac_tuner_name),
                            (VOID*)h_edb_eng,
                            _tm_open_brdcst_root_nfy,
                            &(pt_eng_data->h_root),
                            &e_cond);
    if ((i4_ret == TMR_OK) &&
        (e_cond == TM_COND_AVAILABLE))
    {
        i4_ret =  tm_set_src_conn_nfy_cond(pt_eng_data->h_root,
                                           TRUE,
                                           &e_cond);

        if ( i4_ret == TMR_OK )
        {
            /*Load table*/
            i4_ret = _edb_dvb_load_table(h_edb_eng);
            x_dbg_stmt(_EDB_INFO"_edb_dvb_load_table Ok!\r\n");
            if (i4_ret == EDBR_OK)
            {
                pt_eng_data->t_edb_eng_state.fg_enable = TRUE;
                pt_eng_data->e_cond                    = EDB_COND_STARTED;
            }
        }
        else
        {
            x_dbg_stmt(_EDB_INFO"TM root notification function reg failed.\r\n");
            i4_ret = EDBR_INTERNAL_ERROR;
        }
    }
    else
    {
        x_dbg_stmt(_EDB_INFO"x_tm_open_root failed.i4_ret=%d\n",i4_ret);
    }


    return i4_ret;
}

static INT32  _edb_dvb_stop_cache(HANDLE_T h_edb_eng)
{
    INT32               i4_ret      = EDBR_OK;
    DVB_ENG_DATA*       pt_eng_data = NULL;
    EDB_SECTION_MSG_T           t_msg               = {0x0};
     //INT32                       i4_retry            = 0x0;
    UINT8                       ui1_priority        = 1;
    

    EDB_CHECK_RET( 
            (h_edb_eng == NULL_HANDLE),
            (EDBR_INV_ARG),
            ((_EDB_INFO"Invalid argument\r\n"))
            );


    i4_ret = edb_dvb_get_eng_data_from_ctrl(h_edb_eng,(VOID**)&pt_eng_data);
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK || pt_eng_data == NULL),
            (EDBR_INV_ARG),
            ((_EDB_INFO"Can not get engine data from client\r\n"))
            );

    //Free dvb section cached message
    //i4_ret = edb_section_clean_msg(pt_eng_data);

    t_msg.e_msg_type                            = EDB_SECTION_MSG_CLEAN_NFY;
    t_msg.t_section_msg_nfy.pv_section_data     = pt_eng_data;

    i4_ret = DVB_MSG_Q_SEND(edb_dvb_section_get_msg_q_hdl(),
                            &t_msg,
                            sizeof(EDB_SECTION_MSG_T),
                            ui1_priority );


    /*Send MSG fail ,clear EIT mask
    if (i4_ret != OSR_OK) 
    {
    
    }
    */

    /*If already disabled then return OK*/
    if (pt_eng_data->t_edb_eng_state.fg_enable == FALSE)
    {
        DBG_INFO((_EDB_INFO"Already stoped,return\r\n"));
        return EDBR_OK;
    }

    i4_ret = _edb_dvb_clean_ch_locate_list(h_edb_eng);
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK),
            (EDBR_INTERNAL_ERROR),
            ((_EDB_INFO"Reset internal channel id and locate list fail\r\n"))
            );


    i4_ret = _edb_dvb_free_table(h_edb_eng);
    if (i4_ret == EDBR_OK)
    {
        x_dbg_stmt(_EDB_INFO"_edb_dvb_free_table Ok!\r\n");
    }


    //clean pool channel version
    if (pt_eng_data != NULL && pt_eng_data->pt_edb != NULL)
    {
        EDB_EVENT_POOL_T*       pt_edb_event_pool = &(pt_eng_data->pt_edb->t_edb_event_pool);
        HANDLE_TYPE_T           e_hdl_type;
        EDB_HASH_T*             pt_hash_obj     = NULL;
        EDB_EVENT_POOL_CH_T*    pt_pool_ch      = NULL;
        P_ELEMENT_OBJ_T         pt_elm          = NULL;
        UINT32                  ui4_i           = 0;
        UINT32                  i               = 0;
        EDB_EVENT_POOL_CH_VER*  pt_event_pool_ch_ver = NULL;

        if(pt_edb_event_pool != NULL)
        {
            i4_ret = handle_get_type_obj( pt_edb_event_pool->h_ch_hash , &e_hdl_type, (VOID**)&pt_hash_obj );
            if (i4_ret == HR_OK && e_hdl_type == EDBT_HASH)
            {
                for ( ui4_i=0; ui4_i<pt_hash_obj->z_table_size; ++ui4_i )
                {
                    for( pt_elm = pt_hash_obj->ppt_table[ui4_i]; pt_elm != NULL; )
                    {
                        pt_pool_ch = (EDB_EVENT_POOL_CH_T*)pt_elm->pv_obj;
                        if (pt_pool_ch != NULL && pt_pool_ch->pv_data != NULL)
                        {
                            pt_event_pool_ch_ver = pt_pool_ch->pv_data;
                            x_memset(pt_pool_ch->pv_data,0x0,sizeof(EDB_EVENT_POOL_CH_VER));

                            for(i=0 ; i<2; i++)
                            {
                                pt_event_pool_ch_ver->t_pf_tbl[i].ui1_version = 0xFF;
                        }
                            for(i=0 ; i<EDB_MAX_PER_CH_TBL; i++)
                            {
                                pt_event_pool_ch_ver->t_s_act_tbl[i].ui1_version = 0xFF;
                                pt_event_pool_ch_ver->t_s_oth_tbl[i].ui1_version = 0xFF;
                            }
                        }
                        pt_elm = pt_elm->pt_next;
                    }
                }
            }
        }

    }

#if 1
    if (pt_eng_data->h_root != NULL_HANDLE)
    {
        i4_ret = x_tm_free(pt_eng_data->h_root );
        if (i4_ret == TMR_OK)
        {
            pt_eng_data->h_root = NULL_HANDLE;
            x_dbg_stmt(_EDB_INFO"free root Ok!\r\n");
        }
        else
        {
            x_dbg_stmt("x_tm_free root fail %d\r\n",i4_ret);
        }
    }
#else


#endif

    pt_eng_data->t_edb_eng_state.fg_enable = FALSE;
    pt_eng_data->e_cond                    = EDB_COND_STOPPED;






    return i4_ret;
}


static UINT16 _edb_dvb_get_single_event_check_sum(
        EDB_POOL_EVENT_INFO_T* pt_event,
        EDB_EVENT_POOL_CH_T*   pt_channel_info
        )
{
    UINT32      ui2_idx     = 0x0;
    SIZE_T      i4_i        = 0x0;
    UINT32      ui4_code    = 0x0;

    if (pt_event == NULL)
    {
        return 0;
    }

    /* FNV hash algorithm */
    for( i4_i = 0; i4_i < sizeof(EDB_POOL_EVENT_INFO_T); ++i4_i )
    {
        ui4_code ^= (UINT32) (((UINT8*)pt_event)[i4_i]);

        ui4_code += ( (ui4_code<<1) + (ui4_code<<4) + (ui4_code<<7)
                      + (ui4_code<<8) + (ui4_code<<24) );
    }

    /* Event title */
    if (pt_event->ps_event_title != NULL)
    {
        UINT16  ui2_title_len = (UINT16)(x_strlen(pt_event->ps_event_title));
        for(ui2_idx = 0 ; 
            ui2_idx < ui2_title_len;
            ui2_idx++)
        {
            ui4_code += (UINT8) ((pt_event->ps_event_title)[ui2_idx]);
        }
    }


    /* Event detail */
    if (pt_event->ps_event_detail != NULL)
    {
        UINT16  ui2_detail_len = (UINT16)(x_strlen(pt_event->ps_event_detail));
        for(ui2_idx = 0 ; 
            ui2_idx < ui2_detail_len;
            ui2_idx++)
        {
            ui4_code += (UINT8) ((pt_event->ps_event_detail)[ui2_idx]);
        }
    }

    /* Event guidance */
    if (pt_event->ps_event_guidance != NULL)
    {
        UINT16  ui2_len = (UINT16)(x_strlen(pt_event->ps_event_guidance));
        for(ui2_idx = 0 ; 
            ui2_idx < ui2_len;
            ui2_idx++)
        {
            ui4_code += (UINT8) ((pt_event->ps_event_guidance)[ui2_idx]);
        }
    }

    if (pt_channel_info != NULL)
    {
        if (pt_channel_info->t_channel_guidance_info.ps_event_guidance != NULL) 
        {
            UINT16  ui2_len = (UINT16)(x_strlen(pt_channel_info->t_channel_guidance_info.ps_event_guidance));
            for(ui2_idx = 0 ; 
                ui2_idx < ui2_len;
                ui2_idx++)
            {
                ui4_code += (UINT8) ((pt_channel_info->t_channel_guidance_info.ps_event_guidance)[ui2_idx]);
            }
        }    
    }


    /* Event Rating Lst */
    if (pt_event->pt_rating_list != NULL)
    {
        if (pt_event->pt_rating_list->pt_rating != NULL)
        {
            ui4_code += pt_event->pt_rating_list->pt_rating->ui1_rating;
        }
    }

    /* Event category */
    for(ui2_idx = 0 ; 
    (ui2_idx < pt_event->ui1_num_event_category) && (ui2_idx < EDB_MAX_NUM_CATEGORY);
    ui2_idx++)
	{
	    ui4_code += pt_event->aui1_event_category[ui2_idx];
	}

    return (UINT16)(ui4_code);

}






INT32    buffer_parse_data(BUFFER_PARSER* pt_buffer_parser,UINT32 ui4_need_bits_temp,VOID* pv_out) 
{
    UINT8*      pui1_buffer         = NULL;
    UINT32      ui4_token           = 0x0;
    UINT32      ui4_bit_used        = 0x0;
    UINT32      ui4_valid_bit_pos   = 0x0;
    UINT32      ui4_need_bits       = 0x0;
    UINT32      ui4_all_need_bits   = 0x0;

    EDB_CHECK_RET( 
            (pt_buffer_parser == NULL || ui4_need_bits_temp == (UINT32)0x0),
            (EDBR_INTERNAL_ERROR),
            ((_EDB_INFO"Wrong parse argument\r\n"))
            );

    pui1_buffer         = pt_buffer_parser->pui1_buffer;
    ui4_token           = pt_buffer_parser->ui4_token;
    ui4_bit_used        = pt_buffer_parser->ui4_bit_used;
    ui4_valid_bit_pos   = pt_buffer_parser->ui4_valid_bit_pos;
    ui4_need_bits       = ui4_need_bits_temp;
    ui4_all_need_bits   = pt_buffer_parser->ui4_all_need_bits;


    //DBG_INFO((_EDB_INFO"bytetoken   ui4_valid_bit_pos  needsBit\r\n"));
    //ui4_token + " " + ui4_valid_bit_pos + " " + ui4_need_bits + " value=");
    ui4_all_need_bits = ui4_valid_bit_pos + ui4_need_bits;
    if (ui4_all_need_bits <= 8) 
    {
        UINT8 b = (UINT8) (
                ( pui1_buffer[ui4_token] &
                  ((0x1 << (8 - ui4_valid_bit_pos)) - 1)
                )
                >> 
                (8 - ui4_need_bits - ui4_valid_bit_pos)
                );
        if (pv_out != NULL)
        {
            *((UINT8*)pv_out) = b;
        }
    } 
    else if (ui4_all_need_bits <= 16) 
    {
        UINT16 _short = (UINT16) 
            (
                    ((pui1_buffer[ui4_token]) << 8) | 
                    (pui1_buffer[ui4_token + 1])
            );
        _short = (UINT16) 
            (
                    (_short & ((0x1 << (16 - ui4_valid_bit_pos)) - 1)) 
                    >> 
                    (16 - ui4_need_bits - ui4_valid_bit_pos)
            );
        if (pv_out != NULL)
        {
            *((UINT16*)pv_out) = _short;
        }
    } 
    else if (ui4_all_need_bits <= 32) 
    {
        UINT32 _int = (UINT32) 
            (
                    ((pui1_buffer[ui4_token] )    << 24) | 
                    ((pui1_buffer[ui4_token + 1] )<< 16) | 
                    ((pui1_buffer[ui4_token + 2]) << 8)  | 
                    ((pui1_buffer[ui4_token + 3]))
            );
        _int = (UINT32) 
            (
                    (_int & ((0x1 << (32 - ui4_valid_bit_pos)) - 1)) 
                    >> 
                    (32 - ui4_need_bits - ui4_valid_bit_pos)
            );

        if (pv_out != NULL)
        {
            *((UINT32*)pv_out) = _int;
        }
    } 
    else {
        if (pv_out != NULL)
        {
            if ((ui4_need_bits % 8) == 0x0)
            {
                x_memcpy(pv_out,(pui1_buffer + ui4_token),ui4_need_bits>>3);
            }
        }
        // Copy byte
    }

    ui4_bit_used     += ui4_need_bits;
    ui4_token         = ui4_bit_used >> 3;
    ui4_valid_bit_pos = ui4_bit_used - (ui4_token << 3);


    //pt_buffer_parser->pui1_buffer       =      pui1_buffer         ;
    pt_buffer_parser->ui4_token         =      ui4_token           ;
    pt_buffer_parser->ui4_bit_used      =      ui4_bit_used        ;
    pt_buffer_parser->ui4_valid_bit_pos =      ui4_valid_bit_pos   ;
    pt_buffer_parser->ui4_all_need_bits =      ui4_all_need_bits   ;

    return EDBR_OK;

}


static NFY_RET_T _tm_sdt_sec_nfy(
            HANDLE_T            h_eit,
            HANDLE_TYPE_T        e_obj_type,
            TM_COND_T            e_nfy_cond,
            VOID*                pv_nfy_tag, /*Is engine data*/
            UINT32                ui4_data)
{

    EDB_DVB_MSG_T       t_msg       = {0};
    INT32               i4_retry    = 0x0;
    HANDLE_T            h_edb_eng   = NULL_HANDLE;

    h_edb_eng = (HANDLE_T)pv_nfy_tag;

    EDB_CHECK_RET( 
            (h_edb_eng  == NULL_HANDLE),
            (NFY_RET_INVALID),
            ((_EDB_INFO"_tm_sdt_sec_nfy h_edb_eng NULL_HANDLE \r\n"))
            );
    if(e_nfy_cond == TM_COND_AVAILABLE)
    {
        DBG_INFO(("{EDB} sdt notify TM_COND_AVAILABLE \r\n"));
    }
    else if(e_nfy_cond == TM_COND_UPDATE)
    {
        DBG_INFO(("{EDB} sdt notify TM_COND_UPDATE \r\n"));
    }
    if(e_nfy_cond == TM_COND_AVAILABLE || e_nfy_cond == TM_COND_UPDATE)
    {
        //Send msg switch thread context
        DBG_INFO((_EDB_INFO"_tm_sdt_sec_nfy %d\r\n",ui4_data));
        t_msg.e_msg_type            = EDB_DVB_MSG_START_PARSE_SDT_DESC;
        t_msg.ui4_data              = ui4_data;
        t_msg.pv_tag                = (VOID*)h_edb_eng; /*HANDLE_T  */

        while ( ++i4_retry <= 10 )
        {
            if (x_msg_q_send(h_dvb_msg_q,
                             &t_msg,
                             sizeof(EDB_DVB_MSG_T),
                             0x0 /* highest priority */) != OSR_OK)
            {
                /* Send message fail, wait 10 ms and retry for 10 times */
                x_thread_delay( 10 );
                continue;
            }
            edb_activate_eng_task(EDB_DVB_BRDCST_GRP);

            break;
        }
    }

    return NFY_RET_PROCESSED;
    
}

static NFY_RET_T _tm_eit_sec_nfy(
        HANDLE_T            h_eit,
        HANDLE_TYPE_T       e_obj_type,
        TM_COND_T           e_nfy_cond,
        VOID*               pv_nfy_tag, /*Is engine data*/
        UINT32              ui4_data)
{
    INT32   i4_ret              = 0x0;
    UINT8   ui1_tbl_id          = 0x0;
    UINT16  ui2_sect_len        = 0x0;
    UINT16  ui2_svc_id          = 0x0;
    UINT8   ui1_version         = 0x0;
    BOOL    b_curr_next_ind     = FALSE;
    UINT8   ui1_sect_num        = 0x0;
    UINT16  ui2_ts_id           = 0x0;
    UINT16  ui2_on_id           = 0x0;

    BUFFER_PARSER               t_buffer_parser = {0x0};
    EDB_EVENT_POOL_CH_KEY       t_key           = {0x0};
    BOOL                        b_pre_process   = FALSE;
    DVB_ENG_DATA*               pt_eng_data     = NULL;
    DVB_SI_ON_TS_SVC_ID_SEC_INFO_T      t_sec_info = {0x0};
    TM_COND_T                           e_cond;

    UINT8*                  pui1_sec_mask = NULL;
    UINT8                   ui1_mask      = 0x0;


#if 0
    UINT8*  pui1_data           = (UINT8*) ui4_data;

    /* Extract the section header. */
    ui1_tbl_id        = pui1_data[0];
    ui2_sect_len      = GET_UINT16_FROM_PTR_BIG_END(&(pui1_data[1]))&0xfff;
    ui2_svc_id        = GET_UINT16_FROM_PTR_BIG_END (&(pui1_data[3]));
    ui1_version       = (pui1_data[5] & 0x3e) >> 1;
    b_curr_next_ind   = ((pui1_data[5] & 0x1) != 0);
    ui1_sect_num      = pui1_data[6];
    ui2_ts_id         = GET_UINT16_FROM_PTR_BIG_END (&(pui1_data[8]));
    ui2_on_id         = GET_UINT16_FROM_PTR_BIG_END (&(pui1_data[10]));

    DBG_INFO((_EDB_INFO"tag[0x%02x] tbl_id[0x%02x] onid[0x%04x] tsid[0x%04x] svcid[0x%04x] ver[%d] sec_num[%d] sec_len[%d] cr_nxt_ind[%s]\n\r",
              pv_nfy_tag, ui1_tbl_id, ui2_on_id, ui2_ts_id, ui2_svc_id, ui1_version, ui1_sect_num, ui2_sect_len,
              ((b_curr_next_ind)?"TRUE":"FALSE")
             ));
#endif
    pt_eng_data = (DVB_ENG_DATA*)pv_nfy_tag;
    EDB_CHECK_RET( 
            (pt_eng_data  == NULL),
            (NFY_RET_INVALID),
            ((_EDB_INFO"Can not get engine data\r\n",ui1_tbl_id))
            );



    //DBG_INFO((_EDB_INFO"Parse header start\r\n"));
    t_buffer_parser.pui1_buffer = (UINT8*)ui4_data;               //Raw data start address
    buffer_parse_data(&t_buffer_parser, 8, &ui1_tbl_id     );     //table_id 8 uimsbf
    buffer_parse_data(&t_buffer_parser, 1, NULL            );     //section_syntax_indicator 1 bslbf
    buffer_parse_data(&t_buffer_parser, 1, NULL            );     //reserved_future_use 1 bslbf
    buffer_parse_data(&t_buffer_parser, 2, NULL            );     //reserved 2 bslbf
    buffer_parse_data(&t_buffer_parser, 12, &ui2_sect_len  );     //section_length 12 uimsbf
    buffer_parse_data(&t_buffer_parser, 16, &ui2_svc_id    );     //service_id 16 uimsbf
    buffer_parse_data(&t_buffer_parser, 2, NULL            );     //reserved 2 bslbf
    buffer_parse_data(&t_buffer_parser, 5, &ui1_version    );     //version_number 5 uimsbf
    buffer_parse_data(&t_buffer_parser, 1, &b_curr_next_ind);     //current_next_indicator 1 bslbf
    buffer_parse_data(&t_buffer_parser, 8, &ui1_sect_num   );     //section_number 8 uimsbf
    buffer_parse_data(&t_buffer_parser, 8, NULL            );     //last_section_number 8 uimsbf
    buffer_parse_data(&t_buffer_parser, 16, &ui2_ts_id     );     //transport_stream_id 16 uimsbf
    buffer_parse_data(&t_buffer_parser, 16, &ui2_on_id     );     //original_network_id 16 uimsbf
    //DBG_INFO((_EDB_INFO"Parse header end\r\n"));

    EDB_CHECK_RET( 
            (ui1_tbl_id < (UINT8)0x4E || ui1_tbl_id > (UINT8)0x6F  ),
            (NFY_RET_INVALID),
            ((_EDB_INFO"Invalid Table id:0x%02x\r\n",ui1_tbl_id))
            );

    if (TRUE /*&& (*((UINT8*)pv_nfy_tag)) != ui1_tbl_id*/)
    {
        t_key.ui2_svl_id = pt_eng_data->ui2_svl_id;
        t_key.ui2_on_id  = ui2_on_id;
        t_key.ui2_ts_id  = ui2_ts_id;
        t_key.ui2_svc_id = ui2_svc_id;

#define TEST_ON_TS_SVC ((ui2_on_id == MONITOR_ON_ID &&  ui2_ts_id==MONITOR_TS_ID &&  ui2_svc_id== MONITOR_SVC_ID))
        if TEST_ON_TS_SVC
        {
        /*
            TIME_T    t_start_time = 0;
            UINT16    ui2_event_id = 0;
            UINT8     aui1_bcd[3]                = {0};
            DTG_T                       t_dtg                      = {0};
            
            buffer_parse_data(&t_buffer_parser, 16, NULL     ); 
            buffer_parse_data(&t_buffer_parser, 16  , & ui2_event_id      ); 
            buffer_parse_data(&t_buffer_parser, 40  , & t_start_time      ); 
            //buffer_parse_data(&t_buffer_parser, 24  , & (pt_edb_event_info->t_duration)           ); 
            buffer_parse_data(&t_buffer_parser, 8  ,  & (aui1_bcd[0])                             ); 
            buffer_parse_data(&t_buffer_parser, 8  ,  & (aui1_bcd[1])                             ); 
            buffer_parse_data(&t_buffer_parser, 8  ,  & (aui1_bcd[2])                             ); 
        
            x_dt_mjd_bcd_to_dtg( (UINT8*)(&(t_start_time)),&t_dtg);               //Get start time
            t_start_time = x_dt_dtg_to_sec(&t_dtg);                               //Get start time
            x_dbg_stmt("SECTION event_ID=0x%04x\n\t",ui2_event_id);
            DBG_STMT_TIME("SECTION event Start_time",t_start_time); x_dbg_stmt("\n");
            */
            x_dbg_stmt(_EDB_INFO"Engine data[0x%08x] tbl_id[0x%04x] onid[0x%04x] tsid[0x%04x] svcid[0x%04x] ver[%d] sec_num[%d] sec_len[%d] cr_nxt_ind[%s]\n\r",
                      pt_eng_data, ui1_tbl_id, ui2_on_id, ui2_ts_id, ui2_svc_id, ui1_version, ui1_sect_num, ui2_sect_len,
                      ((b_curr_next_ind)?"TRUE":"FALSE")
                     );

            
        }
#if 0
        else
        {
            return NFY_RET_PROCESSED;
        }
#endif

#if 0        
        if (ui1_tbl_id == (UINT8)0x4E || (ui1_tbl_id == (UINT8)0x4F))
        {
            x_dbg_stmt(_EDB_INFO"Engine data[0x%08x] tbl_id[0x%02x] onid[0x%04x] tsid[0x%04x] svcid[0x%04x] ver[%d] sec_num[%d] sec_len[%d] cr_nxt_ind[%s]\n\r",
                      pt_eng_data, ui1_tbl_id, ui2_on_id, ui2_ts_id, ui2_svc_id, ui1_version, ui1_sect_num, ui2_sect_len,
                      ((b_curr_next_ind)?"TRUE":"FALSE")
                     );
        }
#endif

        b_pre_process = _edb_dvb_pre_process_section(pt_eng_data,&t_key,ui1_tbl_id,ui1_sect_num,ui1_version,
                                                     &pui1_sec_mask,&ui1_mask
                                                    );

        if (b_pre_process == TRUE)
        {
            if TEST_ON_TS_SVC
            {
                /*Copy section data and send msg to EDB thread*/
                x_dbg_stmt(_EDB_INFO"[NEED PROCESS] Engine data[0x%08x] tbl_id[0x%02x] onid[0x%04x] tsid[0x%04x] svcid[0x%04x] ver[%d] sec_num[%d] sec_len[%d] cr_nxt_ind[%s]\n\r",
                          pt_eng_data, ui1_tbl_id, ui2_on_id, ui2_ts_id, ui2_svc_id, ui1_version, ui1_sect_num, ui2_sect_len,
                          ((b_curr_next_ind)?"TRUE":"FALSE")
                         );
            }



            /*Copy data and send MSG to EDB thread*/
            {
                EDB_SECTION_MSG_T           t_msg               = {0x0};
                VOID*                       pv_raw_eit_section  = NULL;
                //INT32                     i4_retry            = 0x0;
                UINT8                       ui1_priority        = 0x0;

                if (ui1_tbl_id == (UINT8)0x4E)
                {
                    ui1_priority = 1;                   /*EIT actual  P/F has highest priority*/
                }else if(ui1_tbl_id == (UINT8)0x4F)
                {
                    ui1_priority = 2;
                }
                else
                {
                    ui1_priority = 255;
                }

                if (ui2_sect_len <= 0)
                {
                    return NFY_RET_INVALID;
                }
#ifndef SEC_MEMORY_DEBUG
                pv_raw_eit_section = x_mem_alloc(ui2_sect_len * sizeof(UINT8));
#else
                pv_raw_eit_section = EDB_PARTITION_ALLOC(ui2_sect_len * sizeof(UINT8));
#endif
                if (pv_raw_eit_section == NULL)
                {
                    x_dbg_stmt("%s %d outof memory\r\n",__FILE__,__LINE__);
                    return NFY_RET_INVALID;
                    //DBG_ABORT( DBG_MOD_EDB );
                }
                else
                {
                    if (pv_raw_eit_section != NULL)
                    {
                        x_memset(pv_raw_eit_section,0x0,ui2_sect_len * sizeof(UINT8));
                    }
                    x_memcpy(pv_raw_eit_section,(UINT8*)ui4_data,ui2_sect_len);

                    t_msg.e_msg_type                            = EDB_SECTION_MSG_SECTION_NFY;
                    t_msg.t_section_msg_nfy.pv_section_data     = pv_raw_eit_section;
                    t_msg.t_section_msg_nfy.h_eit               = h_eit;
                    t_msg.t_section_msg_nfy.ui4_section_len     = (UINT32)ui2_sect_len;
                    t_msg.t_section_msg_nfy.pv_tag              = pt_eng_data;
                    t_msg.t_section_msg_nfy.t_locate.ui2_svl_id = pt_eng_data->ui2_svl_id;
                    t_msg.t_section_msg_nfy.t_locate.ui2_on_id  = ui2_on_id;
                    t_msg.t_section_msg_nfy.t_locate.ui2_ts_id  = ui2_ts_id;
                    t_msg.t_section_msg_nfy.t_locate.ui2_svc_id = ui2_svc_id;
                    t_msg.t_section_msg_nfy.ui1_table_id        = ui1_tbl_id;
                    t_msg.t_section_msg_nfy.ui1_sect_num        = ui1_sect_num;
                    t_msg.t_section_msg_nfy.pui1_sec_mask       = pui1_sec_mask;
                    t_msg.t_section_msg_nfy.ui1_mask            = ui1_mask;



#if 0
                    while ( ++i4_retry <= 10 )
                    {

                        i4_ret = DVB_MSG_Q_SEND(edb_dvb_section_get_msg_q_hdl(),
                                                &t_msg,
                                                sizeof(EDB_SECTION_MSG_T),
                                                ui1_priority );


                        if (i4_ret != OSR_OK)
                        {
                            /* Send message fail, wait 10 ms and retry for 10 times */
                            x_thread_delay( 10 );
                            continue;
                        }else
                        {
                            break;
                        }
                    }
#else

                    i4_ret = DVB_MSG_Q_SEND(edb_dvb_section_get_msg_q_hdl(),
                                            &t_msg,
                                            sizeof(EDB_SECTION_MSG_T),
                                            ui1_priority );
#endif


                    /*Send MSG fail ,clear EIT mask*/
                        if (i4_ret != OSR_OK) 
                    {
                        DBG_INFO(("%s %d SEND MSG fail\r\n",__FILE__,__LINE__));
                        t_sec_info.ui2_on_id = ui2_on_id;
                        t_sec_info.ui2_ts_id = ui2_ts_id;
                        t_sec_info.ui2_svc_id = ui2_svc_id;
                        t_sec_info.ui1_tbl_id = ui1_tbl_id;
                        t_sec_info.ui1_sec_num = ui1_sect_num;    
                        t_sec_info.b_all_sections = FALSE;

                        x_dvb_si_set_eit_section_mask_clear(h_eit, &t_sec_info, &e_cond);
                        (*pui1_sec_mask) &= (~ui1_mask);                //Clear section mask
                        
                        #ifndef SEC_MEMORY_DEBUG 
                                x_mem_free((VOID*)t_msg.t_section_msg_nfy.pv_section_data);
                        #else
                                EDB_PARTITION_FREE((VOID*)t_msg.t_section_msg_nfy.pv_section_data);
                        #endif

                    }
                }
            }
        }
#if 0   /*the version existed, just ignore this section*/     
        else
        {

            DBG_INFO((_EDB_INFO"Engine data[0x%08x] tbl_id[0x%02x] onid[0x%04x] tsid[0x%04x] svcid[0x%04x] ver[%d] sec_num[%d] sec_len[%d] cr_nxt_ind[%s]\n\r",
                      pt_eng_data, ui1_tbl_id, ui2_on_id, ui2_ts_id, ui2_svc_id, ui1_version, ui1_sect_num, ui2_sect_len,
                      ((b_curr_next_ind)?"TRUE":"FALSE")
                     ));
            DBG_INFO((_EDB_INFO"x_dvb_si_set_eit_section_mask_clear \r\n"));

            t_sec_info.ui2_on_id = ui2_on_id;
            t_sec_info.ui2_ts_id = ui2_ts_id;
            t_sec_info.ui2_svc_id = ui2_svc_id;
            t_sec_info.ui1_tbl_id = ui1_tbl_id;
            t_sec_info.ui1_sec_num = ui1_sect_num;    
            t_sec_info.b_all_sections = FALSE;

            x_dvb_si_set_eit_section_mask_clear(h_eit, &t_sec_info, &e_cond);
            (*pui1_sec_mask) &= (~ui1_mask);                //Clear section mask
        }
#endif        
    }

    return NFY_RET_PROCESSED;
}


static BOOL   _edb_dvb_pre_process_section(
        DVB_ENG_DATA*           pt_eng_data,
        EDB_EVENT_POOL_CH_KEY*  pt_key,
        UINT8                   ui1_tbl_id,
        UINT8                   ui1_sect_num,
        UINT8                   ui1_version,
        UINT8**                 ppui1_sec_mask,
        UINT8*                  pui1_mask
        )
{
    INT32                   i4_ret            = 0x0;
    EDB_EVENT_POOL_CH_T*    pt_ch_obj         = NULL;
    BOOL                    b_process         = FALSE;
    BOOL                    b_pf              = FALSE;
    BOOL                    b_act             = FALSE;

    EDB_EVENT_POOL_T*       pt_edb_event_pool = NULL;
#if 0
    EDB_EVENT_POOL_CH_T*             pt_event_pool_ch       = NULL;
#endif
    EDB_EVENT_POOL_CH_VER*           pt_event_pool_ch_ver   = NULL;
    UINT8                            ui1_index              = 0x0; 
    UINT8                            ui1_offset             = 0x0;
    UINT8                            ui1_tbl_idx            = 0x0;
    //UINT32*                          pui4_channel_id        = NULL;


    UINT8                            ui1_mask = 0x0;


    EDB_CHECK_RET( 
            (pt_key == NULL || pt_eng_data == NULL),
            (FALSE),
            ((_EDB_INFO"Can not get edb engine data\r\n"))
            );

    pt_edb_event_pool =  &(pt_eng_data->pt_edb->t_edb_event_pool);
    EDB_CHECK_RET( 
            (pt_edb_event_pool == NULL),
            (FALSE),
            ((_EDB_INFO"Can not get edb event pool\r\n"))
            );



    if (ui1_tbl_id <= (UINT8)0x4F)
    {
        b_pf = TRUE;
        if (ui1_tbl_id == (UINT8)0x4E)
        {
            b_act = TRUE;
        }
    }
    else/*schedule*/
    {
        if (ui1_tbl_id <= (UINT8)0x5F)
        {
            b_act = TRUE;
        }
    }



    i4_ret = edb_pool_get_ch_obj(pt_edb_event_pool, pt_key,&pt_ch_obj); 

    /*on ts service not in pool*/
    if (i4_ret != EDBR_OK || pt_ch_obj == NULL)
    {
        return FALSE; 
    }

#if 0
    if (pt_ch_obj == NULL)/*on ts service not in pool*/
    {
        /**/
        pt_event_pool_ch = EDB_ALLOC(sizeof(EDB_EVENT_POOL_CH_T));
        if (pt_event_pool_ch == NULL)
        {
            x_dbg_stmt("%s %d outof memory\r\n",__FILE__,__LINE__);
            return FALSE;
        }
        x_memset(pt_event_pool_ch,0x0,sizeof(EDB_EVENT_POOL_CH_T));
        i4_ret = edb_hash_create(EDB_POOL_CHANNEL_HASH_EVENT_TABLE_SIZE,&(pt_event_pool_ch->h_event_hash));
        EDB_CHECK_RET( 
                (i4_ret != HASH_OK),
                (FALSE),
                ((_EDB_INFO"Can not init channel event hash table\r\n"))
                );
        pt_event_pool_ch_ver = EDB_ALLOC(sizeof(EDB_EVENT_POOL_CH_VER));
        if (pt_event_pool_ch_ver == NULL)
        {
            DBG_ABORT( DBG_MOD_EDB );
        }
        x_memset(pt_event_pool_ch_ver,0x0,sizeof(EDB_EVENT_POOL_CH_VER));
        pt_event_pool_ch->pv_data = pt_event_pool_ch_ver;
        pt_event_pool_ch->t_key = (*pt_key);
        pt_event_pool_ch->pf_free_event = edb_dvb_free_single_event_obj;

        i4_ret = edb_pool_add_ch_obj(pt_edb_event_pool, pt_event_pool_ch);
        EDB_CHECK_RET( 
                (i4_ret != EDBR_OK),
                (FALSE),
                ((_EDB_INFO"Can not add channel into pool\r\n"))
                );
        pt_edb_event_pool->ui2_total_channels++;

        if (b_pf == TRUE) /*EIT PF section*/
        {
            ui1_tbl_idx = ui1_tbl_id - (UINT8)0x4E;
            if (ui1_tbl_idx > (UINT8)0x2){return FALSE;}
            pt_event_pool_ch_ver->t_pf_tbl[ui1_tbl_idx].ui1_version    = ui1_version;
            pt_event_pool_ch_ver->t_pf_tbl[ui1_tbl_idx].aui1_sec_mask |= (MAKE_BIT_MASK_8(ui1_sect_num));

            (*ppui1_sec_mask) = &(pt_event_pool_ch_ver->t_pf_tbl[ui1_tbl_idx].aui1_sec_mask);
            ui1_mask = (MAKE_BIT_MASK_8(ui1_sect_num));
        }
        else /*EIT S section*/
        {
            if (b_act == TRUE)
            {
                ui1_tbl_idx = ui1_tbl_id - (UINT8)0x50;

                if (ui1_tbl_idx >= EDB_MAX_PER_CH_TBL)
                {
                    return FALSE;
                }

                pt_event_pool_ch_ver->t_s_act_tbl[ui1_tbl_idx].ui1_version     = ui1_version;
                /*Set mask section number bit mask*/
                ui1_index = (UINT8)(ui1_sect_num / 8);
                ui1_offset= (UINT8)(ui1_sect_num % 8);

                pt_event_pool_ch_ver->t_s_act_tbl[ui1_tbl_idx].aui1_sec_mask[ui1_index] |= (MAKE_BIT_MASK_8(ui1_offset));

                (*ppui1_sec_mask) = &(pt_event_pool_ch_ver->t_s_act_tbl[ui1_tbl_idx].aui1_sec_mask[ui1_index]);
                ui1_mask = (MAKE_BIT_MASK_8(ui1_offset));
            }
            else/*EIT other*/
            {
                ui1_tbl_idx = ui1_tbl_id - (UINT8)0x10 - (UINT8)0x50;
                if (ui1_tbl_idx >= EDB_MAX_PER_CH_TBL)
                {
                    return FALSE;
                }
                pt_event_pool_ch_ver->t_s_oth_tbl[ui1_tbl_idx].ui1_version     = ui1_version;
                /*Set mask section number bit mask*/
                ui1_index = (UINT8)(ui1_sect_num / 8);
                ui1_offset= (UINT8)(ui1_sect_num % 8);

                pt_event_pool_ch_ver->t_s_oth_tbl[ui1_tbl_idx].aui1_sec_mask[ui1_index] |= (MAKE_BIT_MASK_8(ui1_offset));

                (*ppui1_sec_mask) = &(pt_event_pool_ch_ver->t_s_oth_tbl[ui1_tbl_idx].aui1_sec_mask[ui1_index]);
                ui1_mask = (MAKE_BIT_MASK_8(ui1_offset));

            }
        }

        b_process = TRUE;

    }
    else
#endif
    /*on ts service in pool*/
    {
        pt_event_pool_ch_ver = (EDB_EVENT_POOL_CH_VER*)pt_ch_obj->pv_data;
        EDB_CHECK_RET( 
                (pt_event_pool_ch_ver == NULL),
                (FALSE),
                ((_EDB_INFO"Can not find version info\r\n"))
                );

        if (b_pf == TRUE)
        {
            /*Is actual, and versin changed*/
            if (b_act == TRUE && pt_event_pool_ch_ver->t_pf_tbl[0].ui1_version != ui1_version)
            {
                /*update version and reset mask*/
                x_memset( &(pt_event_pool_ch_ver->t_pf_tbl[0].aui1_sec_mask),0x0,sizeof(UINT8));
                pt_event_pool_ch_ver->t_pf_tbl[0].ui1_version = ui1_version;
                pt_event_pool_ch_ver->t_pf_tbl[0].aui1_sec_mask |= (MAKE_BIT_MASK_8(ui1_sect_num));
                b_process = TRUE;

                (*ppui1_sec_mask) = &(pt_event_pool_ch_ver->t_pf_tbl[0].aui1_sec_mask);
                ui1_mask = (MAKE_BIT_MASK_8(ui1_sect_num));
            }
            /*Is actual ,no actual got*/
            else if ((b_act == TRUE) && 
                     !((pt_event_pool_ch_ver->t_pf_tbl[0].aui1_sec_mask) & (MAKE_BIT_MASK_8(ui1_sect_num))) 
                    )
            {
                pt_event_pool_ch_ver->t_pf_tbl[0].aui1_sec_mask |= (MAKE_BIT_MASK_8(ui1_sect_num));
                b_process = TRUE;

                (*ppui1_sec_mask) = &(pt_event_pool_ch_ver->t_pf_tbl[0].aui1_sec_mask);
                ui1_mask = (MAKE_BIT_MASK_8(ui1_sect_num));

            }
            /*Is actual ,Other already come,use actual*/
            else if( b_act == TRUE &&  
                     ((pt_event_pool_ch_ver->t_pf_tbl[1].aui1_sec_mask) & (MAKE_BIT_MASK_8(ui1_sect_num))) 
                   )
            {
                pt_event_pool_ch_ver->t_pf_tbl[0].aui1_sec_mask |= (MAKE_BIT_MASK_8(ui1_sect_num));
                b_process = TRUE;


                (*ppui1_sec_mask) = &(pt_event_pool_ch_ver->t_pf_tbl[0].aui1_sec_mask);
                ui1_mask = (MAKE_BIT_MASK_8(ui1_sect_num));
            }
            /*Is other ,no actual ,use other*/
            else if(b_act == FALSE && 
                    !((pt_event_pool_ch_ver->t_pf_tbl[0].aui1_sec_mask) & (MAKE_BIT_MASK_8(ui1_sect_num))) 
                   )
            {
                pt_event_pool_ch_ver->t_pf_tbl[1].ui1_version = ui1_version;
                pt_event_pool_ch_ver->t_pf_tbl[1].aui1_sec_mask |= (MAKE_BIT_MASK_8(ui1_sect_num));
                b_process = TRUE;


                (*ppui1_sec_mask) = &(pt_event_pool_ch_ver->t_pf_tbl[1].aui1_sec_mask);
                ui1_mask = (MAKE_BIT_MASK_8(ui1_sect_num));
            }
            /*ingore this section*/
            else
            {

            }

        }
        else /*EIT S section*/
        {
            ui1_index   = (UINT8)(ui1_sect_num / 8);
            ui1_offset  = (UINT8)(ui1_sect_num % 8);

            if (b_act == TRUE)
            {
                ui1_tbl_idx = ui1_tbl_id - (UINT8) 0x50;
            }
            else
            {
                ui1_tbl_idx = ui1_tbl_id - (UINT8) 0x50 - (UINT8)0x10;
            }

            if (ui1_tbl_idx >= EDB_MAX_PER_CH_TBL){return FALSE;}

            /*Is actual, and versin changed*/
            if (b_act == TRUE && ui1_tbl_idx < EDB_MAX_PER_CH_TBL && pt_event_pool_ch_ver->t_s_act_tbl[ui1_tbl_idx].ui1_version != ui1_version)
            {
                /*update version and reset mask*/
                x_memset(pt_event_pool_ch_ver->t_s_act_tbl[ui1_tbl_idx].aui1_sec_mask,0x0,sizeof(UINT8));
                pt_event_pool_ch_ver->t_s_act_tbl[ui1_tbl_idx].ui1_version = ui1_version;
                pt_event_pool_ch_ver->t_s_act_tbl[ui1_tbl_idx].aui1_sec_mask[ui1_index] |= (MAKE_BIT_MASK_8(ui1_offset));
                b_process = TRUE;


                (*ppui1_sec_mask) = &(pt_event_pool_ch_ver->t_s_act_tbl[ui1_tbl_idx].aui1_sec_mask[ui1_index]);
                ui1_mask = (MAKE_BIT_MASK_8(ui1_offset));
            }
            /*Is actual ,no actual got*/
            else if( b_act == TRUE &&   (ui1_tbl_idx < EDB_MAX_PER_CH_TBL) &&
                     !((pt_event_pool_ch_ver->t_s_act_tbl[ ui1_tbl_idx].aui1_sec_mask[ui1_index]) & (MAKE_BIT_MASK_8(ui1_sect_num)) )
                   )
            {
                pt_event_pool_ch_ver->t_s_act_tbl[ui1_tbl_idx].aui1_sec_mask[ui1_index] |= (MAKE_BIT_MASK_8(ui1_offset));
                b_process = TRUE;


                (*ppui1_sec_mask) = &(pt_event_pool_ch_ver->t_s_act_tbl[ui1_tbl_idx].aui1_sec_mask[ui1_index]);
                ui1_mask = (MAKE_BIT_MASK_8(ui1_offset));
            }
            /*Is actual ,Other already come,use actual*/
            else if(b_act == TRUE &&  
                    ((pt_event_pool_ch_ver->t_s_oth_tbl[ui1_tbl_idx].aui1_sec_mask[ui1_index]) & (MAKE_BIT_MASK_8(ui1_offset))) 
                   )
            {
                pt_event_pool_ch_ver->t_s_act_tbl[ui1_tbl_idx].aui1_sec_mask[ui1_index] |= (MAKE_BIT_MASK_8(ui1_offset));
                b_process = TRUE;


                (*ppui1_sec_mask) = &(pt_event_pool_ch_ver->t_s_act_tbl[ui1_tbl_idx].aui1_sec_mask[ui1_index]);
                ui1_mask = (MAKE_BIT_MASK_8(ui1_offset));
            }
            /*Is other ,no actual ,use other*/
            else if(b_act == FALSE && ( ui1_tbl_idx < EDB_MAX_PER_CH_TBL) &&
                    !((pt_event_pool_ch_ver->t_s_act_tbl[ ui1_tbl_idx].aui1_sec_mask[ui1_index]) & (MAKE_BIT_MASK_8(ui1_sect_num)) )
                   )
            {/*Other come ,no actual*/
                pt_event_pool_ch_ver->t_s_oth_tbl[ ui1_tbl_idx ].aui1_sec_mask[ui1_index] |= (MAKE_BIT_MASK_8(ui1_offset));
                b_process = TRUE;

                (*ppui1_sec_mask) = &(pt_event_pool_ch_ver->t_s_oth_tbl[ ui1_tbl_idx ].aui1_sec_mask[ui1_index]);
                ui1_mask = (MAKE_BIT_MASK_8(ui1_offset));
            }

        }


    }

    *pui1_mask = ui1_mask;

    return b_process;
}


static INT32 _edb_dvb_load_sdt(HANDLE_T     h_edb_eng)
{
    INT32                   i4_ret              = 0x0;
    TM_COND_T               e_cond;
    DVB_ENG_DATA*           pt_eng_data         = NULL;
    EDB_EVENT_POOL_CH_KEY*  pt_locate           = NULL;

    EDB_CHECK_RET( 
            (h_edb_eng == NULL_HANDLE),
            (EDBR_INV_ARG),
            ((_EDB_INFO"Invalid argument\r\n"))
            );

    i4_ret = edb_dvb_get_eng_data_from_ctrl(h_edb_eng,(VOID**)&pt_eng_data);
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK || pt_eng_data == NULL),
            (EDBR_INV_ARG),
            ((_EDB_INFO"Can not get engine data from client\r\n"))
            );

    i4_ret = edb_dvb_get_eng_data_from_ctrl(h_edb_eng, (VOID**)&pt_eng_data);

    i4_ret = _edb_dvb_get_ch_locate_obj(pt_eng_data,pt_eng_data->t_edb_eng_state.ui4_current_channel_id, &pt_locate);
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK || pt_locate == NULL),
            (EDBR_INTERNAL_ERROR),
            (("Get locate fail\r\n"))
            );
    if( 
        pt_eng_data->t_edb_eng_state.ui2_current_on_id != pt_locate->ui2_on_id ||
        pt_eng_data->t_edb_eng_state.ui2_current_ts_id != pt_locate->ui2_ts_id ||
        pt_eng_data->h_sdt == NULL_HANDLE
      )
    {

    }
    else 
    {
        return EDBR_OK; 
    }
    
    if(pt_eng_data->h_sdt != NULL_HANDLE)
    {
        i4_ret = x_tm_free(pt_eng_data->h_sdt);
        if (i4_ret == TMR_OK)
        {
            pt_eng_data->h_sdt = NULL_HANDLE;
        }
        else
        {
            x_dbg_stmt("x_tm_free SDT fail %d\r\n",i4_ret);
        }
    }
    DBG_INFO(("{EDB} begins to load sdt \r\n "));
    i4_ret = x_dvb_si_load_sdt ( pt_eng_data->h_root,
                                 pt_locate->ui2_on_id,
                                 pt_locate->ui2_ts_id,
                                 (VOID*)h_edb_eng,
                                 _tm_sdt_sec_nfy,
                                 &(pt_eng_data->h_sdt),
                                 &e_cond ) ;
    if(i4_ret == TMR_OK)
    {
        pt_eng_data->t_edb_eng_state.ui2_current_on_id = pt_locate->ui2_on_id;
        pt_eng_data->t_edb_eng_state.ui2_current_ts_id = pt_locate->ui2_ts_id;
    }
    else
    {
        x_dbg_stmt("{EDB} load sdt fail ! \r\n");
    }

    return i4_ret;
}


static INT32 _edb_dvb_load_pf(HANDLE_T h_edb_eng)
{
    UINT8               ui1_tbl_id          = 0x0 ;
    INT32               i4_ret              = 0x0;
    TM_COND_T           e_cond;
    DVB_ENG_DATA*       pt_eng_data         = NULL;

    EDB_CHECK_RET( 
            (h_edb_eng == NULL_HANDLE),
            (EDBR_INV_ARG),
            ((_EDB_INFO"Invalid argument\r\n"))
            );

    i4_ret = edb_dvb_get_eng_data_from_ctrl(h_edb_eng,(VOID**)&pt_eng_data);
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK || pt_eng_data == NULL),
            (EDBR_INV_ARG),
            ((_EDB_INFO"Can not get engine data from client\r\n"))
            );


    /*Load PF*/
    ui1_tbl_id = 0x4E;
    if (pt_eng_data->ah_eit[(UINT8)(ui1_tbl_id - 0x4E)] != NULL_HANDLE)
    {
        i4_ret = x_tm_free(pt_eng_data->ah_eit[(UINT8)(ui1_tbl_id - 0x4E)]);
        if (i4_ret == TMR_OK)
        {
            pt_eng_data->ah_eit[(UINT8)(ui1_tbl_id - 0x4E)] = NULL_HANDLE;
        }
        else
        {
            x_dbg_stmt("x_tm_free PF fail %d\r\n",i4_ret);
        }
    }
    i4_ret = x_dvb_si_load_eit_pf_sections(pt_eng_data->h_root,
                                           pt_eng_data->t_edb_eng_state.fg_actual_only,
                                           (VOID*)pt_eng_data,//(VOID *)(UINT32)ui1_tbl_id,
                                           _tm_eit_sec_nfy,
                                           &(pt_eng_data->ah_eit[(UINT8)(ui1_tbl_id - 0x4E)]),
                                           &e_cond);

    return i4_ret;
}


static INT32 _edb_dvb_load_s(HANDLE_T h_edb_eng)
{
    INT32               i4_ret              = 0x0;
#if 1  
    INT32               i                   = 0x0;
    UINT8               aui1_pf_tbl_id[16]   = {0x0};
    UINT8               ui1_tbl_id          = 0x0 ;
    TM_COND_T           e_cond;
    DVB_ENG_DATA*       pt_eng_data         = NULL;
    UINT8               ui1_max_days        = 0x0;

    EDB_CHECK_RET( 
            (h_edb_eng == NULL_HANDLE),
            (EDBR_INV_ARG),
            ((_EDB_INFO"Invalid argument\r\n"))
            );

    i4_ret = edb_dvb_get_eng_data_from_ctrl(h_edb_eng,(VOID**)&pt_eng_data);
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK || pt_eng_data == NULL),
            (EDBR_INV_ARG),
            ((_EDB_INFO"Can not get engine data from client\r\n"))
            );

    ui1_max_days = pt_eng_data->t_edb_eng_state.ui1_max_days;
    if (ui1_max_days == (UINT8)0x0 || (ui1_max_days % 4) != 0x0 || ui1_max_days >= (UINT8)64)
    {
        DBG_INFO((_EDB_INFO"Invalid ui1_max_days=%d\r\n",ui1_max_days));
        ui1_max_days = (UINT8)8;
        //return EDBR_INV_ARG;
    }

#ifdef EDB_FULL_64_DAYS    
    for (i=0x50;i<=0x5F;i++)
#else
        for (i=0x50;i<0x50 + (ui1_max_days / 4);i++)//max 64 days
#endif
        {
            aui1_pf_tbl_id[i-0x50] =(UINT8)i;
        }

    /*Load S*/

    for (i=0; i< (INT32)(ui1_max_days / 4); i++)
    {
        ui1_tbl_id = aui1_pf_tbl_id[i];
        if(ui1_tbl_id != (UINT8)0x0)
        {
            if ((UINT8)(ui1_tbl_id - (UINT8)0x4E) >= EDB_DVB_MAX_TBL)
            {
                return i4_ret;
            }

            if (pt_eng_data->ah_eit[(UINT8)(ui1_tbl_id - (UINT8)0x4E)] != NULL_HANDLE)
            {
                i4_ret = x_tm_free(pt_eng_data->ah_eit[(UINT8)(ui1_tbl_id - (UINT8)0x4E)]);
                if (i4_ret == TMR_OK)
                {
                    pt_eng_data->ah_eit[(UINT8)(ui1_tbl_id - (UINT8)0x4E)] = NULL_HANDLE;
                }
                else
                {
                    x_dbg_stmt("x_tm_free S fail %d\r\n",i4_ret);
                }
            }

            i4_ret = x_dvb_si_load_eit_s_sections(pt_eng_data->h_root,
                                                  pt_eng_data->t_edb_eng_state.fg_actual_only,
                                                  ui1_tbl_id, 
                                                  (VOID*)pt_eng_data,//(VOID *)(UINT32)ui1_tbl_id,
                                                  _tm_eit_sec_nfy,
                                                  &(pt_eng_data->ah_eit[(UINT8)(ui1_tbl_id - (UINT8)0x4E)]),
                                                  &e_cond);
        }

    }
#endif
    return i4_ret;
}


static INT32 _edb_dvb_load_table(HANDLE_T h_edb_eng)
{
    INT32   i4_ret = 0;

    EDB_CHECK_RET( 
            (h_edb_eng == NULL_HANDLE),
            (EDBR_INV_ARG),
            ((_EDB_INFO"Invalid argument\r\n"))
            );

    i4_ret = _edb_dvb_load_pf(h_edb_eng);
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK),
            (EDBR_INTERNAL_ERROR),
            ((_EDB_INFO"_edb_dvb_load_pf fail\r\n"))
            );

    i4_ret = _edb_dvb_load_s(h_edb_eng);
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK),
            (EDBR_INTERNAL_ERROR),
            ((_EDB_INFO"_edb_dvb_load_s fail\r\n"))
            );
    
    _edb_dvb_load_sdt(h_edb_eng);
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK),
            (EDBR_INTERNAL_ERROR),
            ((_EDB_INFO"_edb_dvb_load_sdt fail\r\n"))
            );
    DBG_INFO((_EDB_INFO"Load table OK\r\n"));

    return i4_ret;
}


static INT32 _edb_dvb_free_table(HANDLE_T h_edb_eng)
{
    INT32   i4_ret = 0x0;
    INT32   i      = 0x0;


    DVB_ENG_DATA*       pt_eng_data = NULL;

    EDB_CHECK_RET( 
            (h_edb_eng == NULL_HANDLE),
            (EDBR_INV_ARG),
            ((_EDB_INFO"Invalid argument\r\n"))
            );

    i4_ret = edb_dvb_get_eng_data_from_ctrl(h_edb_eng,(VOID**)&pt_eng_data);
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK || pt_eng_data == NULL),
            (EDBR_INV_ARG),
            ((_EDB_INFO"Can not get engine data from client\r\n"))
            );

    for (i=0; i < EDB_DVB_MAX_TBL; i++)
    {
        if (pt_eng_data->ah_eit[i] != NULL_HANDLE)
        {
            i4_ret = x_tm_free(pt_eng_data->ah_eit[i]);
            if (i4_ret == TMR_OK)
            {
                pt_eng_data->ah_eit[i] = NULL_HANDLE;
            }
            else
            {
                x_dbg_stmt("x_tm_free all table fail %d\r\n",i4_ret);
            }

            pt_eng_data->ah_eit[i] = NULL_HANDLE;
        }
    }

    if(pt_eng_data->h_sdt != NULL_HANDLE)
    {
        i4_ret = x_tm_free(pt_eng_data->h_sdt);
        if (i4_ret == TMR_OK)
        {
            pt_eng_data->h_sdt = NULL_HANDLE;
        }
        else
        {
            x_dbg_stmt("x_tm_free SDT fail %d\r\n",i4_ret);
        }
    }

    return i4_ret;
}


static INT32 _edb_set_edb_with_command (
        HANDLE_T               h_edb_eng,
        DVB_ENG_DATA*          pt_eng_data,
        EDB_COMMAND_T*         pt_cmd)
{
    INT32                   i4_ret           = 0;
    EDB_COMMAND_CODE_T      e_code;

    EDB_CHECK_RET( 
            (pt_eng_data == NULL|| pt_cmd == NULL),
            (EDBR_INV_ARG),
            ((_EDB_INFO"Invalid arguments\r\n"))
            );


    while ((e_code = pt_cmd->e_code) != EDB_CMD_END)
    {
        switch (e_code)
        {
#if 0 
            case EDB_CMD_OPEN_DATA:
                {
                    break;
                }
#endif
            case EDB_CMD_CFG_PF_ONLY:
                DBG_INFO(("EDB Set command EDB_CMD_CFG_PF_ONLY\r\n"));
                pt_eng_data->t_edb_eng_state.fg_pf_only = (BOOL)pt_cmd->u.b_bool;
                DBG_INFO(("EDB Set command EDB_CMD_CFG_PF_ONLY pf_only=%d\r\n",pt_eng_data->t_edb_eng_state.fg_pf_only));
                break;
            case EDB_CMD_CFG_ACTUAL_ONLY:
                DBG_INFO(("EDB Set command EDB_CMD_CFG_ACTUAL_ONLY\r\n"));
                pt_eng_data->t_edb_eng_state.fg_actual_only = (BOOL)pt_cmd->u.b_bool;
                DBG_INFO(("EDB Set command EDB_CMD_CFG_ACTUAL_ONLY actual_only=%d\r\n",pt_eng_data->t_edb_eng_state.fg_actual_only));
                break;
            case EDB_CMD_CFG_MAX_DAYS:
                DBG_INFO(("EDB Set command EDB_CMD_CFG_MAX_DAYS\r\n"));
                pt_eng_data->t_edb_eng_state.ui1_max_days = (UINT8)pt_cmd->u.ui2_number;
                DBG_INFO(("EDB Set command EDB_CMD_CFG_MAX_DAYS max_days=%d\r\n",pt_eng_data->t_edb_eng_state.ui1_max_days ));
                break;
            case EDB_CMD_CFG_PREF_LANG:
                DBG_INFO(("EDB Set command EDB_CMD_CFG_PREF_LANG\r\n"));
                pt_eng_data->t_edb_eng_state.t_pref_lang = *(pt_cmd->u.pt_pref_lang);
                DBG_INFO(("EDB Set command EDB_CMD_CFG_PREF_LANG num=%d %s %s %s %s\r\n",
                          pt_eng_data->t_edb_eng_state.t_pref_lang.ui1_num,
                          pt_eng_data->t_edb_eng_state.t_pref_lang.t_lang[0],
                          pt_eng_data->t_edb_eng_state.t_pref_lang.t_lang[1],
                          pt_eng_data->t_edb_eng_state.t_pref_lang.t_lang[2],
                          pt_eng_data->t_edb_eng_state.t_pref_lang.t_lang[3]
                          ));
                break;
            case EDB_CMD_CFG_DVBC_OPERATOR:
                DBG_INFO(("EDB Set command EDB_CMD_CFG_DVBC_OPERATOR\r\n"));
                pt_eng_data->t_edb_eng_state.t_operator =  pt_cmd->u.t_operator;
                DBG_INFO(("EDB Set command EDB_CMD_CFG_DVBC_OPERATOR operator=%s\r\n",pt_eng_data->t_edb_eng_state.t_operator));
                dvb_si_txt_set_operator(pt_eng_data->t_edb_eng_state.t_operator);
               
                //TEST code
#if 0
                { 
                    pt_eng_data->t_edb_eng_state.t_operator =  1;//TODO
                    dvb_si_txt_set_operator(pt_eng_data->t_edb_eng_state.t_operator);
                }
#endif                
                break;

            case EDB_CMD_CFG_COUNTRY_CODE:
                DBG_INFO(("EDB Set command EDB_CMD_CFG_COUNTRY_CODE\r\n"));
                x_strncpy(pt_eng_data->t_edb_eng_state.t_country,pt_cmd->u.t_country_code,ISO_3166_COUNT_LEN);
                DBG_INFO(("EDB Set command EDB_CMD_CFG_COUNTRY_CODE country=%s\r\n",pt_eng_data->t_edb_eng_state.t_country));
                break;
            case EDB_CMD_CFG_ACTIVE_WIN:
                DBG_INFO(("EDB Set command EDB_CMD_CFG_ACTIVE_WIN\r\n"));
                {
                    EDB_CACHE_ACTIVE_WIN_T *    pt_win      = NULL;
                    EDB_CH_ID_T*                pt_channel  = NULL;
                    UINT8                       i           = 0x0;
                    EDB_EVENT_POOL_CH_KEY  *    pt_locate   = NULL;
                    EDB_EVENT_POOL_CH_T  *      pt_ch_obj   = NULL;
                    INT32                       _i4_ret     = 0;
                    pt_eng_data->t_edb_eng_state.t_active_win = *(pt_cmd->u.pt_active_win);
                    /*updata active window time stamp*/

                    pt_win = &(pt_eng_data->t_edb_eng_state.t_active_win);

                    DBG_INFO(("EDB Set command EDB_CMD_CFG_ACTIVE_WIN num_channel=%d\r\n",pt_win->ui1_num_channel));
                    if (pt_win!=NULL && (pt_win->ui1_num_channel > (UINT8)0x0) && (pt_win->ui1_num_channel < EDB_MAX_NUM_ACTIVE_CHANNEL) && (pt_eng_data->pt_edb != NULL))
                    {
                        for(i=0;i<pt_win->ui1_num_channel;i++)
                        {
                            pt_channel = &( pt_win->at_channel_id[i]);
                            DBG_INFO(("EDB Set command EDB_CMD_CFG_ACTIVE_WIN channelId[%d]=%d\r\n",i,pt_channel->ui4_channel_id));
                            _i4_ret = _edb_dvb_get_ch_locate_obj(pt_eng_data, pt_channel->ui4_channel_id, &pt_locate);
                            if (_i4_ret == EDBR_OK && pt_locate != NULL)
                            {
                                _i4_ret = edb_pool_get_ch_obj(&(pt_eng_data->pt_edb->t_edb_event_pool),pt_locate,&pt_ch_obj);
                                if (pt_ch_obj != NULL)
                                {
                                    pt_ch_obj->ui4_time_stamp = x_os_get_sys_tick();//set time stamp for this channel
                                }
                            }
                        }

                    }
                    break; 
                }
            case EDB_CMD_CFG_EVENT_MIN_SECS:
                DBG_INFO(("EDB Set command EDB_CMD_CFG_EVENT_MIN_SECS\r\n"));
                pt_eng_data->t_edb_eng_state.ui4_event_min_secs = (UINT32)(pt_cmd->u.ui2_number);
                DBG_INFO(("EDB Set command EDB_CMD_CFG_EVENT_MIN_SECS min_sec=%d\r\n",pt_eng_data->t_edb_eng_state.ui4_event_min_secs));
                break;        
            case EDB_CMD_CFG_FAKE_EVENT_INSERTION_ENABLE:
                DBG_INFO(("EDB Set command EDB_CMD_CFG_FAKE_EVENT_INSERTION_ENABLE\r\n"));
                pt_eng_data->t_edb_eng_state.fg_fake_event_insertion_enable = (BOOL)(pt_cmd->u.b_bool);
                DBG_INFO(("EDB Set command EDB_CMD_CFG_FAKE_EVENT_INSERTION_ENABLE insertion_enable=%d\r\n",
                          pt_eng_data->t_edb_eng_state.fg_fake_event_insertion_enable));
                break;    
            case EDB_CMD_CFG_FAKE_EVENT_MIN_SECS:
                DBG_INFO(("EDB Set command EDB_CMD_CFG_FAKE_EVENT_MIN_SECS\r\n"));
                pt_eng_data->t_edb_eng_state.fake_event_min_secs = (UINT32)(pt_cmd->u.ui2_number);
                DBG_INFO(("EDB Set command EDB_CMD_CFG_FAKE_EVENT_MIN_SECS fake min sec=%d\r\n",
                          pt_eng_data->t_edb_eng_state.fake_event_min_secs));
                break;                    
            case EDB_CMD_CFG_TIME_CONFLICT_ALLOW:
                DBG_INFO(("EDB Set command EDB_CMD_CFG_TIME_CONFLICT_ALLOW\r\n"));
                pt_eng_data->t_edb_eng_state.fg_time_conflict_allow = (UINT32)(pt_cmd->u.ui2_number);
                DBG_INFO(("EDB Set command EDB_CMD_CFG_TIME_CONFLICT_ALLOW confilct_allow=%d\r\n",pt_eng_data->t_edb_eng_state.fg_time_conflict_allow));
                break;
            case EDB_CMD_CFG_TIME_PARTIAL_OVERLAP_ALLOW:
                DBG_INFO(("EDB Set command EDB_CMD_CFG_TIME_PARTIAL_OVERLAP_ALLOW\r\n"));
                pt_eng_data->t_edb_eng_state.fg_time_partial_overlap_allow = (UINT32)(pt_cmd->u.ui2_number);
                DBG_INFO(("EDB Set command EDB_CMD_CFG_TIME_PARTIAL_OVERLAP_ALLOW partial_overlap_allow=%d\r\n",
                          pt_eng_data->t_edb_eng_state.fg_time_partial_overlap_allow));
                break;
            case EDB_CMD_CFG_EVENT_DETAIL_SEPARATOR:
                DBG_INFO(("EDB Set command EDB_CMD_CFG_EVENT_DETAIL_SEPARATOR\r\n"));
                {
                    x_strncpy(pt_eng_data->t_edb_eng_state.s_event_detail_separator,pt_cmd->u.s_separator,4);
                    DBG_INFO(("EDB Set command EDB_CMD_CFG_EVENT_DETAIL_SEPARATOR detail_separator=%s \r\n",
                              pt_eng_data->t_edb_eng_state.s_event_detail_separator));
                    break;
                }
            case EDB_CMD_DO_CURRENT_SERVICE:/*Do command*/
                DBG_INFO(("EDB Set command EDB_CMD_DO_CURRENT_SERVICE\r\n"));
                _edb_dvb_set_current_service(h_edb_eng,pt_cmd->u.pt_ch_id);
                DBG_INFO(("EDB Set command EDB_CMD_DO_CURRENT_SERVICE ch_id=%d\r\n",pt_cmd->u.pt_ch_id->ui4_channel_id));
                break;
            case EDB_CMD_DO_ENABLE:/*Do command*/
                DBG_INFO(("EDB Set command EDB_CMD_DO_ENABLE\r\n"));
                /*DO start or stop EDB*/
                DBG_INFO(("EDB Set command EDB_CMD_DO_ENABLE enable=%d\r\n",pt_cmd->u.b_bool));
                _edb_dvb_set_enable_edb(h_edb_eng,(BOOL)pt_cmd->u.b_bool);
                break;

            case EDB_CMD_DO_RESTART:/*Do command*/
                DBG_INFO(("EDB Set command EDB_CMD_DO_RESTART\r\n"));
                _edb_dvb_set_restart_edb(h_edb_eng);
                break;
            case EDB_CMD_DO_CLEAN:
                _edb_dvb_recreate_pool(h_edb_eng);
                DBG_INFO(("EDB Set command EDB_CMD_DO_CLEAN\r\n"));
                #if 0
                {
                    EDB_T*                pt_edb = NULL; 
                    if (pt_eng_data->pt_edb != NULL)
                    {
                        pt_edb = pt_eng_data->pt_edb ;
                        edb_pool_delete(&(pt_edb->t_edb_event_pool));

                        i4_ret = edb_pool_create(
                                EDB_POOL_HASH_CHANNEL_TABLE_SIZE,
                                x_dt_get_brdcst_utc(NULL,NULL),
                                &(pt_edb->t_edb_event_pool)
                                );
                        if (i4_ret != EDBR_OK)
                        {
                            DBG_INFO(("edb_pool_create Error ui2_total_channels_cap=%d\r\n",EDB_POOL_HASH_CHANNEL_TABLE_SIZE));
                        }
                    }
                    break;
                }
                #endif
                break;
            case EDB_CMD_DO_TUNER_CHANGE:
                DBG_INFO(("EDB Set command EDB_CMD_DO_CLEANEDB_CMD_DO_TUNER_CHANGE\r\n"));
                {
                    _edb_dvb_set_tuner_change(h_edb_eng,pt_cmd->u.pt_eng_open_data);
                    break;
                }
#ifdef MW_EPG_CACHE_NVM_SUPPORT            
            case EDB_CMD_DO_FLASH_EVENTS:
            {    
     #ifdef CLI_SUPPORT   
                DBG_STMT_TIME("current time ",x_dt_get_brdcst_utc(NULL,NULL));    x_dbg_stmt("\n");            
     #endif
                x_dbg_stmt("EDB current cache %d days events maximally \n",pt_eng_data->t_edb_eng_state.ui1_max_days);
                i4_ret = edb_flash_event(pt_eng_data->t_edb_eng_state.ui1_max_days , pt_cmd->u.ui2_number);                
                break;
            }
            case EDB_CMD_DO_SYNC_EVENTS:
            {
                if(pt_cmd->u.b_bool == TRUE)
                {
                    i4_ret = edb_sync_event_data_from_flash_to_mem();
                }
                break;
            }
            case EDB_CMD_DO_ASU_MODE:
            {
                pt_eng_data->t_edb_eng_state.b_asu_mode = (UINT32)(pt_cmd->u.b_bool);
                break;
            }
#endif

#ifdef    MW_EDB_FBM_SHARE_MEM
            case EDB_CMD_RISKTV:
                {
                    _edb_dvb_set_risktv(h_edb_eng,pt_cmd->u.b_bool);
                    break;
                }
#endif 
            case EDB_CMD_CHANNEL_SCAN_ACTIVE:/*Do command*/
                /*DO start or stop EDB*/
                _edb_dvb_set_channel_scan(h_edb_eng,(BOOL)pt_cmd->u.b_bool);
                break;

            
            default:
                break;
        }
        pt_cmd++;
    }

    return i4_ret;
}


static INT32 _edb_dvb_set_tuner_change(
        HANDLE_T                    h_edb_eng,
        EDB_ENG_OPEN_DATA_T*        pt_eng_open_data)
{
    INT32               i4_ret      = EDBR_OK;
    EDB_DVB_MSG_T       t_msg       = {0};
    INT32               i4_retry    = 0x0;

    EDB_CHECK_RET( 
            (h_edb_eng == NULL_HANDLE),
            (EDBR_INV_ARG),
            ((_EDB_INFO"Invalid argument\r\n"))
            );

    //Send msg switch thread context
    t_msg.e_msg_type            = EDB_DVB_MSG_TUNER_CHANGE;
    t_msg.pv_tag                = (VOID*)h_edb_eng; /*HANDLE_T  */
    if (pt_eng_open_data->ps_tuner_name == NULL || x_strlen(pt_eng_open_data->ps_tuner_name)<=0)
    {
        DBG_ABORT( DBG_MOD_EDB );
    }
    DBG_INFO((_EDB_INFO"_edb_dvb_set_tuner_change %s\r\n",pt_eng_open_data->ps_tuner_name));
    x_strncpy(t_msg.ps_name,pt_eng_open_data->ps_tuner_name,x_strlen(pt_eng_open_data->ps_tuner_name));


    while ( ++i4_retry <= 10 )
    {
        if (x_msg_q_send(h_dvb_msg_q,
                         &t_msg,
                         sizeof(EDB_DVB_MSG_T),
                         0x0 /* highest priority */) != OSR_OK)
        {
            /* Send message fail, wait 10 ms and retry for 10 times */
            x_thread_delay( 10 );
            continue;
        }
        edb_activate_eng_task(EDB_DVB_BRDCST_GRP);

        return i4_ret;
    }




    return i4_ret;
}


static INT32 _edb_dvb_set_restart_edb(HANDLE_T h_edb_eng)
{
    INT32               i4_ret      = EDBR_OK;
    EDB_DVB_MSG_T       t_msg       = {0};
    INT32               i4_retry    = 0x0;

    EDB_CHECK_RET( 
            (h_edb_eng == NULL_HANDLE),
            (EDBR_INV_ARG),
            ((_EDB_INFO"Invalid argument\r\n"))
            );

    //Send msg switch thread context
    DBG_INFO((_EDB_INFO"_edb_dvb_set_restart_edb %d\r\n"));
    t_msg.e_msg_type            = EDB_DVB_MSG_RESTART_EDB;
    t_msg.pv_tag                = (VOID*)h_edb_eng; /*HANDLE_T  */

    while ( ++i4_retry <= 10 )
    {
        if (x_msg_q_send(h_dvb_msg_q,
                         &t_msg,
                         sizeof(EDB_DVB_MSG_T),
                         0x0 /* highest priority */) != OSR_OK)
        {
            /* Send message fail, wait 10 ms and retry for 10 times */
            x_thread_delay( 10 );
            continue;
        }
        edb_activate_eng_task(EDB_DVB_BRDCST_GRP);

        return i4_ret;
    }




    return i4_ret;
}


static INT32 _edb_dvb_set_enable_edb(HANDLE_T h_edb_eng,BOOL fg_flag)
{
    INT32               i4_ret      = EDBR_OK;
    EDB_DVB_MSG_T       t_msg       = {0};
    INT32               i4_retry    = 0x0;

    EDB_CHECK_RET( 
            (h_edb_eng == NULL_HANDLE),
            (EDBR_INV_ARG),
            ((_EDB_INFO"Invalid argument\r\n"))
            );

    //Send msg switch thread context
    DBG_INFO((_EDB_INFO"_edb_dvb_set_enable_edb %d\r\n",fg_flag));
    t_msg.e_msg_type            = EDB_DVB_MSG_ENABLE_EDB;
    t_msg.ui4_data              = (UINT32)fg_flag;
    t_msg.pv_tag                = (VOID*)h_edb_eng; /*HANDLE_T  */

    while ( ++i4_retry <= 10 )
    {
        if (x_msg_q_send(h_dvb_msg_q,
                         &t_msg,
                         sizeof(EDB_DVB_MSG_T),
                         0x0 /* highest priority */) != OSR_OK)
        {
            /* Send message fail, wait 10 ms and retry for 10 times */
            x_thread_delay( 10 );
            continue;
        }
        edb_activate_eng_task(EDB_DVB_BRDCST_GRP);

        return i4_ret;
    }




    return i4_ret;
}

static INT32 _edb_dvb_set_channel_scan(HANDLE_T h_edb_eng,BOOL fg_flag)
{
    INT32               i4_ret      = EDBR_OK;
    EDB_DVB_MSG_T       t_msg       = {0};
    INT32               i4_retry    = 0x0;

    EDB_CHECK_RET( 
            (h_edb_eng == NULL_HANDLE),
            (EDBR_INV_ARG),
            ((_EDB_INFO"Invalid argument\r\n"))
            );

    //Send msg switch thread context
    DBG_INFO((_EDB_INFO"_edb_dvb_set_channel_scan %d\r\n",fg_flag));
    t_msg.e_msg_type            = EDB_DVB_MSG_CHANNEL_SCAN;
    t_msg.ui4_data              = (UINT32)fg_flag;
    t_msg.pv_tag                = (VOID*)h_edb_eng; /*HANDLE_T  */

    while ( ++i4_retry <= 10 )
    {
        if (x_msg_q_send(h_dvb_msg_q,
                         &t_msg,
                         sizeof(EDB_DVB_MSG_T),
                         0x0 /* highest priority */) != OSR_OK)
        {
            /* Send message fail, wait 10 ms and retry for 10 times */
            x_thread_delay( 10 );
            continue;
        }
        edb_activate_eng_task(EDB_DVB_BRDCST_GRP);

        return i4_ret;
    }




    return i4_ret;
}


#ifdef    MW_EDB_FBM_SHARE_MEM
static INT32 _edb_dvb_set_risktv(HANDLE_T h_edb_eng,BOOL fg_flag)
{
    INT32               i4_ret      = EDBR_OK;
    EDB_DVB_MSG_T       t_msg       = {0};
    INT32               i4_retry    = 0x0;

    EDB_CHECK_RET( 
            (h_edb_eng == NULL_HANDLE),
            (EDBR_INV_ARG),
            ((_EDB_INFO"Invalid argument\r\n"))
            );

    //Send msg switch thread context
    DBG_INFO((_EDB_INFO"_edb_dvb_set_risktv %d\r\n",fg_flag));
    t_msg.e_msg_type            = EDB_DVB_MSG_SET_RISKTV;
    t_msg.ui4_data              = (UINT32)fg_flag;
    t_msg.pv_tag                = (VOID*)h_edb_eng; /*HANDLE_T  */

    while ( ++i4_retry <= 10 )
    {
        if (x_msg_q_send(h_dvb_msg_q,
                         &t_msg,
                         sizeof(EDB_DVB_MSG_T),
                         0x0 /* highest priority */) != OSR_OK)
        {
            /* Send message fail, wait 10 ms and retry for 10 times */
            x_thread_delay( 10 );
            continue;
        }
        edb_activate_eng_task(EDB_DVB_BRDCST_GRP);

        return i4_ret;
    }




    return i4_ret;
}



#endif

static INT32 _edb_dvb_set_current_service(HANDLE_T h_edb_eng,EDB_CH_ID_T* pt_ch_id)
{
    INT32               i4_ret      = EDBR_OK;
    DVB_ENG_DATA*       pt_eng_data = NULL;
    EDB_DVB_MSG_T       t_msg       = {0};
    INT32               i4_retry    = 0x0;
    EDB_CHECK_RET( 
            (h_edb_eng == NULL_HANDLE || pt_ch_id == NULL),
            (EDBR_INV_ARG),
            ((_EDB_INFO"Invalid argument\r\n"))
            );

    i4_ret = edb_dvb_get_eng_data_from_ctrl(h_edb_eng,(VOID**)&pt_eng_data);
    EDB_CHECK_RET( 
            (pt_eng_data == NULL),
            (EDBR_INV_ARG),
            ((_EDB_INFO"Engine data is null\r\n"))
            );

    /*For Matched engine*/
    if (
            (pt_eng_data->e_brdcst_type & (MAKE_BIT_MASK_8(pt_ch_id->e_brdcst_type))) &&
            (pt_eng_data->ui2_svl_id == pt_ch_id->ui2_svl_id)
       )
    {
    }
    else
    {
        DBG_INFO((_EDB_INFO"Engine not natch %d %d %d %d\r\n",
                  pt_eng_data->e_brdcst_type,pt_ch_id->e_brdcst_type,
                  pt_eng_data->ui2_svl_id , pt_ch_id->ui2_svl_id));
        return EDBR_OK;
    }


    //Send msg switch thread context
    DBG_INFO((_EDB_INFO"_edb_dvb_set_current_service\r\n"));
    t_msg.e_msg_type            = EDB_DVB_MSG_CHANGE_CHANNEL_NFY;
    t_msg.ui4_data              = (UINT32)pt_ch_id->ui4_channel_id;
    t_msg.pv_tag                = (VOID*)h_edb_eng; /*HANDLE_T  */

    while ( ++i4_retry <= 10 )
    {
        if (x_msg_q_send(h_dvb_msg_q,
                         &t_msg,
                         sizeof(EDB_DVB_MSG_T),
                         0x0 /* highest priority */) != OSR_OK)
        {
            /* Send message fail, wait 10 ms and retry for 10 times */
            x_thread_delay( 10 );
            continue;
        }
        edb_activate_eng_task(EDB_DVB_BRDCST_GRP);

        return i4_ret;
    }




    return i4_ret;
}


static INT32 _edb_dvb_enable_edb(HANDLE_T h_edb_eng,BOOL fg_enabled)
{
    INT32               i4_ret = EDBR_OK;

    EDB_CHECK_RET( 
            (h_edb_eng == NULL_HANDLE),
            (EDBR_INV_ARG),
            ((_EDB_INFO"Invalid argument\r\n"))
            );

    if (fg_enabled)
    {
        i4_ret = _edb_dvb_start_cache(h_edb_eng);

        i4_ret = _edb_dvb_start_check_expire_timer(h_edb_eng);
        if (i4_ret != EDBR_OK)
        {
            DBG_INFO((_EDB_INFO"timer start  fail\r\n"));
        }

    }
    else
    {
        i4_ret = _edb_dvb_stop_cache(h_edb_eng);
    }

    return i4_ret;
}


static VOID _edb_dvb_free_ch_locate_list( VOID* pv_obj )
{
    if (pv_obj != NULL)
    {
        EDB_OS_FREE(pv_obj);
    }
}

static INT32 _edb_dvb_clean_ch_locate_list(HANDLE_T h_edb_eng)
{
    INT32                   i4_ret      = 0x0 ;
    DVB_ENG_DATA*           pt_eng_data = NULL;

    EDB_CHECK_RET( 
            (h_edb_eng == NULL_HANDLE),
            (EDBR_INV_ARG),
            ((_EDB_INFO"Invalid argument\r\n"))
            );

    i4_ret = edb_dvb_get_eng_data_from_ctrl(h_edb_eng,(VOID**)&pt_eng_data);
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK || pt_eng_data == NULL),
            (EDBR_INV_ARG),
            ((_EDB_INFO"Can not get engine data from client\r\n"))
            );


    if (pt_eng_data->h_edb_inter_ch_locate != NULL_HANDLE)
    {
        edb_hash_delete(pt_eng_data->h_edb_inter_ch_locate, _edb_dvb_free_ch_locate_list);
        pt_eng_data->h_edb_inter_ch_locate = NULL_HANDLE;
    }


    i4_ret = edb_hash_create(EDB_MAX_INTER_CH_LOCATE, &(pt_eng_data->h_edb_inter_ch_locate));
    EDB_CHECK_RET(
            (pt_eng_data->h_edb_inter_ch_locate == NULL_HANDLE),
            (EDBR_INTERNAL_ERROR),
            (("Create EDB inter ch locate list fail\r\n"))
            );
    DBG_INFO((_EDB_INFO"Create EDB inter ch locate list success\r\n"));



    return i4_ret;
}


static INT32 _edb_dvb_add_ch_locate_obj(
        DVB_ENG_DATA*           pt_eng_data,UINT32   ui4_channel_id,EDB_EVENT_POOL_CH_KEY* pt_locate)
{
    INT32                   i4_ret      = 0x0 ;

    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK || pt_eng_data == NULL),
            (EDBR_INV_ARG),
            ((_EDB_INFO"Can not get engine data from client\r\n"))
            );


    EDB_CHECK_RET( 
            ( ui4_channel_id == (UINT32)0x0 || pt_locate == NULL ||pt_eng_data->h_edb_inter_ch_locate == NULL_HANDLE),
            (EDBR_INV_ARG),
            ((_EDB_INFO"Invalid argument\r\n"))
            );

    i4_ret = edb_hash_add_obj(pt_eng_data->h_edb_inter_ch_locate, 
                              (const INT8 *) (&ui4_channel_id), sizeof(UINT32), (VOID *) pt_locate, NULL);
    return i4_ret;
}

static INT32 _edb_dvb_get_ch_locate_obj(
        DVB_ENG_DATA*           pt_eng_data,UINT32   ui4_channel_id,EDB_EVENT_POOL_CH_KEY** ppt_locate)
{
    INT32   i4_ret = 0x0 ;
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK || pt_eng_data == NULL),
            (EDBR_INV_ARG),
            ((_EDB_INFO"Can not get engine data from client\r\n"))
            );

    EDB_CHECK_RET( 
            (pt_eng_data->h_edb_inter_ch_locate == NULL_HANDLE || ui4_channel_id == (UINT32)0x0),
            (EDBR_INV_ARG),
            ((_EDB_INFO"Invalid argument\r\n"))
            );

    i4_ret = edb_hash_get_obj(pt_eng_data->h_edb_inter_ch_locate, 
                              (const INT8 *)(&ui4_channel_id), sizeof(UINT32), (VOID**) ppt_locate);

    EDB_CHECK_RET( 
            (*ppt_locate == NULL),
            (EDBR_INTERNAL_ERROR),
            ((_EDB_INFO"Can not get locate by ch = 0x%x\r\n",ui4_channel_id))
            );

    return i4_ret;
}


static INT32 _edb_dvb_add_eng_data_obj(EDB_ENG_CTRL_ENTRY_T* pt_eng_ctrl_entry,DVB_ENG_DATA* pt_eng_data)
{
    INT32           i4_ret = 0x0 ;
    CH_KEY*         pt_key = NULL;
    SIZE_T          z_len  = 0x0;
    CH_KEY          t_key = {0x0};

    EDB_CHECK_RET( 
            ( pt_eng_ctrl_entry == NULL|| pt_eng_ctrl_entry->pt_eng == NULL || pt_eng_data == NULL || h_eng_data_hash == NULL_HANDLE),
            (EDBR_INV_ARG),
            ((_EDB_INFO"Invalid argument\r\n"))
            );

    pt_key = &t_key;
    z_len  = sizeof(CH_KEY);

    pt_key->e_type      = pt_eng_ctrl_entry->pt_eng->e_brdcst_type;
    pt_key->ui2_svl_id  = pt_eng_ctrl_entry->ui2_svl_id;

    i4_ret = edb_hash_add_obj(h_eng_data_hash, 
                              (const INT8 *) (pt_key), z_len, (VOID *) pt_eng_data, NULL);

    return i4_ret;
}




static INT32 _edb_dvb_iterator_channel_event_ids(
        HANDLE_T          h_hash,
        UINT32            ui4_counter,
        VOID*             pv_obj,
        VOID*             pv_parse_tag )
{
    INT32                       i4_ret              = 0x0;
    x_edb_filter_fct            pf_filter_fct		= NULL;
	VOID*                       pv_filter_tag		= NULL;
	EDB_CH_TIME_FILTER_T*       pt_ch_time_filter   = NULL;
    EDB_BUCKET_INFO_T*          pt_bucket_info      = NULL;
	EDB_EVENT_POOL_CH_T*		pt_channel_info     = NULL;
    EDB_POOL_EVENT_INFO_T*      pt_event_info       = NULL;
    HASH_PARSE_TAG*             pt_hash_parse_tag   = NULL;
	HANDLE_TYPE_T				e_hdl_type;
    EDB_HASH_T*                 pt_hash_obj     	= NULL;
	UINT32						ui4_i				= 0;
	P_ELEMENT_OBJ_T             pt_elm          	= NULL;
	EDB_EVENT_INFO_T			t_event;
	HANDLE_T					h_edb_eng       	= NULL_HANDLE;
	UINT16						ui2_num_events 		= 0;


    EDB_CHECK_RET( 
            (pv_obj == NULL || pv_parse_tag == NULL),
            (EDBR_INTERNAL_ERROR),
            (("Arguments is wrong\r\n"))
            );


    pt_hash_parse_tag  = (HASH_PARSE_TAG*)pv_parse_tag;
    pt_ch_time_filter  = pt_hash_parse_tag->pv_obj1;
	pf_filter_fct = pt_ch_time_filter->pf_filter_fct;
	pv_filter_tag = pt_ch_time_filter->pv_filter_tag;
	h_edb_eng = (HANDLE_T)(pt_hash_parse_tag->pv_obj2);		
	pt_bucket_info     = pt_hash_parse_tag->pv_obj3;        /* EDB_BUCKET_INFO_T*  */
	ui2_num_events	   = (UINT16)(UINT32)(pt_hash_parse_tag->pv_obj5);
	
 
    pt_channel_info  = (EDB_EVENT_POOL_CH_T*)pv_obj;

	i4_ret = handle_get_type_obj( pt_channel_info->h_event_hash, &e_hdl_type, (VOID**)&pt_hash_obj );
	
    if ( i4_ret != HR_OK || e_hdl_type != EDBT_HASH )
    {
        return EDBR_INV_HANDLE;
    }
    
    i4_ret          = EDBR_INFO_NOT_FOUND;

    for ( ui4_i=0; ui4_i<pt_hash_obj->z_table_size; ++ui4_i )
    {
        for( pt_elm = pt_hash_obj->ppt_table[ui4_i]; pt_elm != NULL; )
        {
        
            pt_event_info = (EDB_POOL_EVENT_INFO_T*)pt_elm->pv_obj;

			i4_ret = _edb_dvb_copy_single_event(h_edb_eng,pt_channel_info,pt_event_info,&t_event);
			
			if(i4_ret==EDBR_OK)
			{
				if(pf_filter_fct(&t_event,pv_filter_tag)==TRUE)
				{				
					pt_bucket_info->pui2_ch_event_id[ui2_filter_meet_num].ui2_event_id = t_event.ui2_event_id;
					pt_bucket_info->pui2_ch_event_id[ui2_filter_meet_num].ui4_channel_id = pt_channel_info->ui4_channel_id;
					pt_bucket_info->ui2_checksum += _edb_dvb_get_single_event_check_sum(pt_event_info,pt_channel_info) ;
					ui2_filter_meet_num++;	
					if(ui2_filter_meet_num >= ui2_num_events)
					{
						return EDBR_HASH_PARSE_BREAK;
					}
				}
			}
			
            pt_elm = pt_elm->pt_next;
        }

    }

	//*(pt_bucket_info->pui2_num_events)	 += ui2_actual_event_num;

    return i4_ret;
    

}


static INT32 _edb_dvb_iterator_channel_event_ids_by_string(
        HANDLE_T          h_hash,
        UINT32            ui4_counter,
        VOID*             pv_obj,
        VOID*             pv_parse_tag )
{
    INT32                       i4_ret              = 0x0;
	EDB_DESIRED_STRING_T*       pt_ch_desired_string   = NULL;
    EDB_BUCKET_INFO_T*          pt_bucket_info      = NULL;
	EDB_EVENT_POOL_CH_T*		pt_channel_info     = NULL;
    EDB_POOL_EVENT_INFO_T*      pt_event_info       = NULL;
    HASH_PARSE_TAG*             pt_hash_parse_tag   = NULL;
	HANDLE_TYPE_T				e_hdl_type;
    EDB_HASH_T*                 pt_hash_obj     	= NULL;
	UINT32						ui4_i				= 0;
	P_ELEMENT_OBJ_T             pt_elm          	= NULL;
/*	HANDLE_T					h_edb_eng       	= NULL_HANDLE;  */
	UINT16						ui2_num_events 		= 0;


    EDB_CHECK_RET( 
            (pv_obj == NULL || pv_parse_tag == NULL),
            (EDBR_INTERNAL_ERROR),
            (("Arguments is wrong\r\n"))
            );


    pt_hash_parse_tag  = (HASH_PARSE_TAG*)pv_parse_tag;
    pt_ch_desired_string  = pt_hash_parse_tag->pv_obj1;
	/*h_edb_eng = (HANDLE_T)(pt_hash_parse_tag->pv_obj2);		*/
	pt_bucket_info     = pt_hash_parse_tag->pv_obj3;        /* EDB_BUCKET_INFO_T*  */
	ui2_num_events	   = (UINT16)(UINT32)(pt_hash_parse_tag->pv_obj5);
	
 
    pt_channel_info  = (EDB_EVENT_POOL_CH_T*)pv_obj;

	i4_ret = handle_get_type_obj( pt_channel_info->h_event_hash, &e_hdl_type, (VOID**)&pt_hash_obj );
	
    if ( i4_ret != HR_OK || e_hdl_type != EDBT_HASH )
    {
        return EDBR_INV_HANDLE;
    }
    
    i4_ret          = EDBR_INFO_NOT_FOUND;

    if(pt_ch_desired_string->e_match_type == EDB_DESIRED_STRING_MATCH_TITLE)
    {
        for ( ui4_i=0; ui4_i<pt_hash_obj->z_table_size; ++ui4_i )
        {
            for( pt_elm = pt_hash_obj->ppt_table[ui4_i]; pt_elm != NULL; )
            {
            
                pt_event_info = (EDB_POOL_EVENT_INFO_T*)pt_elm->pv_obj;
                if(pt_event_info->ps_event_title != NULL)
                {
                    /* string contains Algorithm ,to be implemented*/
        		    if(x_strncmp(pt_event_info->ps_event_title,pt_ch_desired_string->ps_desired_string,3) == 0)
    				{				
    					pt_bucket_info->pui2_ch_event_id[ui2_desired_string_meet_num].ui2_event_id = pt_event_info->ui2_event_id;
    					pt_bucket_info->pui2_ch_event_id[ui2_desired_string_meet_num].ui4_channel_id = pt_channel_info->ui4_channel_id;
    					pt_bucket_info->ui2_checksum += _edb_dvb_get_single_event_check_sum(pt_event_info,pt_channel_info) ;
    					ui2_desired_string_meet_num++;	
    					if(ui2_desired_string_meet_num >= ui2_num_events)
    					{
    						return EDBR_HASH_PARSE_BREAK;
    					}
    				}
                }
                pt_elm = pt_elm->pt_next;
            }

        }
    }

    else if(pt_ch_desired_string->e_match_type == EDB_DESIRED_STRING_MATCH_TITLE_AND_DETAIL)
    {
        for ( ui4_i=0; ui4_i<pt_hash_obj->z_table_size; ++ui4_i )
        {
            for( pt_elm = pt_hash_obj->ppt_table[ui4_i]; pt_elm != NULL; )
            {
            
                pt_event_info = (EDB_POOL_EVENT_INFO_T*)pt_elm->pv_obj;
    			if(pt_event_info->ps_event_detail != NULL)
                {         
    			    if(x_strncmp(pt_event_info->ps_event_detail,pt_ch_desired_string->ps_desired_string,3) == 0)
        			{				
    					pt_bucket_info->pui2_ch_event_id[ui2_desired_string_meet_num].ui2_event_id = pt_event_info->ui2_event_id;
    					pt_bucket_info->pui2_ch_event_id[ui2_desired_string_meet_num].ui4_channel_id = pt_channel_info->ui4_channel_id;
    					pt_bucket_info->ui2_checksum += _edb_dvb_get_single_event_check_sum(pt_event_info,pt_channel_info) ;
    					ui2_desired_string_meet_num++;	
    					if(ui2_desired_string_meet_num >= ui2_num_events)
    					{
    						return EDBR_HASH_PARSE_BREAK;
    					}
    				}
                 }
    			
                pt_elm = pt_elm->pt_next;
            }

        }
    }

	//*(pt_bucket_info->pui2_num_events)	 += ui2_actual_event_num;

    return i4_ret;
    

}


static INT32 _edb_dvb_iterator_event_ids(
        HANDLE_T          h_hash,
        UINT32            ui4_counter,
        VOID*             pv_obj,
        VOID*             pv_parse_tag )
{
    INT32                       i4_ret              = 0x0;
    EDB_CH_TIME_FILTER_T*       pt_ch_time_filter   = NULL;
    EDB_BUCKET_INFO_T*          pt_bucket_info      = NULL;
    EDB_POOL_EVENT_INFO_T*      pt_event_info       = NULL;
	EDB_EVENT_INFO_T			t_event             = {0};
	EDB_EVENT_POOL_CH_T*   		pt_ch_obj           = NULL;
    HASH_PARSE_TAG*             pt_hash_parse_tag   = NULL;
    //UINT16                      ui2_index           = 0x0;
    //EDB_CH_EVT_ID_T*            pt_ch_evt_id        = NULL;
    UINT32                      ui4_event_min_secs  = 0x0;
    UINT16                      ui2_num_events      = 0 ;
	HANDLE_T					h_edb_eng           = NULL_HANDLE;

    EDB_CHECK_RET( 
            (pv_obj == NULL || pv_parse_tag == NULL),
            (EDBR_INTERNAL_ERROR),
            (("Arguments is wrong\r\n"))
            );

#if 0
    pt_tag->pv_obj1 = pt_ch_time_filter;    /* EDB_CH_TIME_FILTER_T*  */
    pt_tag->pv_obj2 = pz_event_info_len;    /* SIZE_T*  */
    pt_tag->pv_obj3 = pv_event_info;        /* EDB_BUCKET_INFO_T*  */
    pt_tag->pv_obj4 = (VOID*)ui4_event_min_secs;
    t_parse_tag.pv_obj5
#endif

        pt_hash_parse_tag  = (HASH_PARSE_TAG*)pv_parse_tag;
    pt_ch_time_filter  = pt_hash_parse_tag->pv_obj1;
	pt_ch_obj  		   = pt_hash_parse_tag->pv_obj2;
    pt_bucket_info     = pt_hash_parse_tag->pv_obj3;
    ui4_event_min_secs = (UINT32)(pt_hash_parse_tag->pv_obj4);
    ui2_num_events     = (UINT16)(UINT32)(pt_hash_parse_tag->pv_obj5);
	h_edb_eng		   = (HANDLE_T)(UINT32)(pt_hash_parse_tag->pv_obj6);

    if (*(pt_bucket_info->pui2_num_events) >  ui2_num_events)
    {
        return EDBR_OUT_OF_RESOURCE;
    }

    pt_event_info  = (EDB_POOL_EVENT_INFO_T*)pv_obj;
    if ( 
            (
                    (  
                            pt_event_info->t_start_time <  pt_ch_time_filter->t_start_time &&
                            pt_event_info->t_start_time + pt_event_info->t_duration >  pt_ch_time_filter->t_start_time
                    )
                    ||
                    (
                            (pt_event_info->t_start_time == pt_ch_time_filter->t_start_time)
                    )
                    ||
                    (
                            (pt_event_info->t_start_time >  pt_ch_time_filter->t_start_time && 
                             pt_event_info->t_start_time <  pt_ch_time_filter->t_end_time)
                    )
            )
            &&
            pt_event_info->t_duration >= (TIME_T)ui4_event_min_secs &&
            ui1_apt_pool_event_num < (sizeof(apt_pool_event)/sizeof(apt_pool_event[0]))
       )
    {
#if 0
        ui2_index = *(pt_bucket_info->pui2_num_events);
        pt_ch_evt_id = pt_bucket_info->pui2_ch_event_id + ui2_index;
        pt_ch_evt_id->ui2_event_id = pt_event_info->ui2_event_id;
        *(pt_bucket_info->pui2_num_events) = (UINT16)(ui2_index +1);
        pt_bucket_info->ui2_checksum += _edb_dvb_get_single_event_check_sum(pt_event_info);
#else

		if( pt_ch_time_filter->pf_filter_fct != NULL )
		{
			i4_ret = _edb_dvb_copy_single_event(h_edb_eng,pt_ch_obj,pt_event_info,&t_event);
			
			if(i4_ret==EDBR_OK)
			{
				if(pt_ch_time_filter->pf_filter_fct(&t_event,pt_ch_time_filter->pv_filter_tag)==TRUE)
				{				
        apt_pool_event[ui1_apt_pool_event_num] = pt_event_info;
        ui1_apt_pool_event_num ++;
				}
			}
			return EDBR_OK;
		}

        apt_pool_event[ui1_apt_pool_event_num] = pt_event_info;
        ui1_apt_pool_event_num ++;
#endif
    }

    return i4_ret;
}


static INT32 _edb_dvb_util_get_channel_by_edb_eng(
        HANDLE_T                 h_edb_eng,
        UINT32                   ui4_channel_id,
        EDB_EVENT_POOL_CH_T**    ppt_channel)
{
    INT32                   i4_ret              = EDBR_OK;
    DVB_ENG_DATA*           pt_eng_data         = NULL;
    EDB_EVENT_POOL_T*       pt_edb_event_pool   = NULL;
    EDB_EVENT_POOL_CH_KEY*  pt_locate           = NULL;


    EDB_CHECK_RET( 
            (h_edb_eng == NULL_HANDLE || ui4_channel_id == 0x0 || ppt_channel == NULL),
            (EDBR_INV_ARG),
            (("Wrong argument\r\n"))
            );


    i4_ret = edb_dvb_get_eng_data_from_ctrl(h_edb_eng, (VOID**)&pt_eng_data);
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK || pt_eng_data == NULL||pt_eng_data->pt_edb == NULL 
             || &(pt_eng_data->pt_edb->t_edb_event_pool)== NULL 
            ),
            (EDBR_INTERNAL_ERROR),
            ((_EDB_INFO"Can not get engine data from client\r\n"))
            );
    pt_edb_event_pool = &(pt_eng_data->pt_edb->t_edb_event_pool);

    i4_ret = _edb_dvb_get_ch_locate_obj(pt_eng_data,ui4_channel_id, &pt_locate);
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK),
            (EDBR_INTERNAL_ERROR),
            (("Get locate fail\r\n"))
            );

    i4_ret = edb_pool_get_ch_obj(pt_edb_event_pool,pt_locate, ppt_channel);
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK),
            (EDBR_INTERNAL_ERROR),
            (("Get pool channel fail\r\n"))
            );

    return i4_ret;
}


static INT32 _edb_dvb_process_event_ids_by_filter(
        HANDLE_T            h_edb_eng,
        VOID*               pv_key_info,
        SIZE_T*             pz_event_info_len,
        VOID*               pv_event_info,          /*EDB_BUCKET_INFO_T*/
        EDB_COND_T*         pe_cond 
        )
{
    INT32                   i4_ret                          = EDBR_OK;
    HASH_PARSE_TAG          t_parse_tag                     = {0};
    EDB_BUCKET_INFO_T*      pt_bucket                       = NULL;
    DVB_ENG_DATA*           pt_eng_data                     = NULL;
    UINT16                  ui2_num_events                  = 0;
	EDB_EVENT_POOL_T*       pt_edb_event_pool   			= NULL;
	EDB_CH_TIME_FILTER_T*   pt_ch_time_filter               = NULL;


    i4_ret = edb_dvb_get_eng_data_from_ctrl(h_edb_eng, (VOID**)&pt_eng_data);

	EDB_CHECK_RET( 
				(i4_ret != EDBR_OK || pt_eng_data == NULL||pt_eng_data->pt_edb == NULL 
				 || &(pt_eng_data->pt_edb->t_edb_event_pool)== NULL 
				),
				(EDBR_INTERNAL_ERROR),
				((_EDB_INFO"Can not get engine data from client\r\n"))
				);
	
	pt_edb_event_pool = &(pt_eng_data->pt_edb->t_edb_event_pool);



    EDB_CHECK_RET( 
            (h_edb_eng == NULL_HANDLE ||  pv_key_info == NULL || pz_event_info_len == NULL || pv_event_info == NULL),
            (EDBR_INV_ARG),
            (("Wrong argument\r\n"))
            );

    pt_ch_time_filter = (EDB_CH_TIME_FILTER_T*)pv_key_info;
	
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK || pt_ch_time_filter->pf_filter_fct == NULL),
            (EDBR_INV_ARG),
            (("Get pool channel fail\r\n"))
            );
    

    pt_bucket =(EDB_BUCKET_INFO_T*) pv_event_info;
	t_parse_tag.pv_obj1 = pt_ch_time_filter;	/* EDB_CH_TIME_FILTER_T*  */\
	t_parse_tag.pv_obj2 = (VOID*)h_edb_eng; 
    //t_parse_tag.pv_obj2 = pz_event_info_len;    /* SIZE_T*  */
    t_parse_tag.pv_obj3 = pt_bucket;        /* EDB_BUCKET_INFO_T*  */

    ui2_num_events      = *(pt_bucket->pui2_num_events);
    t_parse_tag.pv_obj5 = (VOID*) (UINT32)ui2_num_events;/*caller numer of events*/


    *(pt_bucket->pui2_num_events)   = (UINT16)0;
    pt_bucket->ui2_checksum         = (UINT16)0x0;

	x_memset(pt_bucket->pui2_ch_event_id,0x0,ui2_num_events*sizeof(EDB_CH_EVT_ID_T));
	
	ui2_filter_meet_num = 0;

    i4_ret = edb_hash_parse(pt_edb_event_pool->h_ch_hash, _edb_dvb_iterator_channel_event_ids, &t_parse_tag);

	*(pt_bucket->pui2_num_events)	 = ui2_filter_meet_num;

	if(i4_ret==EDBR_HASH_PARSE_BREAK)
	{
		i4_ret=EDBR_OK;
	}

    return i4_ret;
}

static INT32 _edb_dvb_process_event_ids_by_desired_string(
        HANDLE_T            h_edb_eng,
        VOID*               pv_key_info,
        SIZE_T*             pz_event_info_len,
        VOID*               pv_event_info,          /*EDB_BUCKET_INFO_T*/
        EDB_COND_T*         pe_cond 
        )
{
    INT32                   i4_ret                          = EDBR_OK;
    HASH_PARSE_TAG          t_parse_tag                     = {0};
    EDB_BUCKET_INFO_T*      pt_bucket                       = NULL;
    DVB_ENG_DATA*           pt_eng_data                     = NULL;
    UINT16                  ui2_num_events                  = 0;
	EDB_EVENT_POOL_T*       pt_edb_event_pool   			= NULL;
	EDB_DESIRED_STRING_T*   pt_ch_desired_string            = NULL;


    i4_ret = edb_dvb_get_eng_data_from_ctrl(h_edb_eng, (VOID**)&pt_eng_data);

	EDB_CHECK_RET( 
				(i4_ret != EDBR_OK || pt_eng_data == NULL||pt_eng_data->pt_edb == NULL 
				 || &(pt_eng_data->pt_edb->t_edb_event_pool)== NULL 
				),
				(EDBR_INTERNAL_ERROR),
				((_EDB_INFO"Can not get engine data from client\r\n"))
				);
	
	pt_edb_event_pool = &(pt_eng_data->pt_edb->t_edb_event_pool);



    EDB_CHECK_RET( 
            (h_edb_eng == NULL_HANDLE ||  pv_key_info == NULL || pz_event_info_len == NULL || pv_event_info == NULL),
            (EDBR_INV_ARG),
            (("Wrong argument\r\n"))
            );

    pt_ch_desired_string = (EDB_DESIRED_STRING_T*)pv_key_info;
	
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK || pt_ch_desired_string->ps_desired_string == NULL || pt_ch_desired_string->z_string_length == 0),
            (EDBR_INV_ARG),
            (("Get pool channel fail\r\n"))
            );
    

    pt_bucket =(EDB_BUCKET_INFO_T*) pv_event_info;
	t_parse_tag.pv_obj1 = pt_ch_desired_string;	/* EDB_CH_TIME_FILTER_T*  */\
	t_parse_tag.pv_obj2 = (VOID*)h_edb_eng; 
    //t_parse_tag.pv_obj2 = pz_event_info_len;    /* SIZE_T*  */
    t_parse_tag.pv_obj3 = pt_bucket;        /* EDB_BUCKET_INFO_T*  */

    ui2_num_events      = *(pt_bucket->pui2_num_events);
    t_parse_tag.pv_obj5 = (VOID*) (UINT32)ui2_num_events;/*caller numer of events*/

    *(pt_bucket->pui2_num_events)   = (UINT16)0;
    pt_bucket->ui2_checksum         = (UINT16)0x0;

	x_memset(pt_bucket->pui2_ch_event_id,0x0,ui2_num_events*sizeof(EDB_CH_EVT_ID_T));
	
	ui2_desired_string_meet_num = 0;

    i4_ret = edb_hash_parse(pt_edb_event_pool->h_ch_hash, _edb_dvb_iterator_channel_event_ids_by_string, &t_parse_tag);

	*(pt_bucket->pui2_num_events)	 = ui2_desired_string_meet_num;

	if(i4_ret==EDBR_HASH_PARSE_BREAK)
	{
		i4_ret=EDBR_OK;
	}

    return i4_ret;
}

static INT32 _edb_dvb_process_event_ids(
        HANDLE_T            h_edb_eng,
        VOID*               pv_key_info,
        SIZE_T*             pz_event_info_len,
        VOID*               pv_event_info,          /*EDB_BUCKET_INFO_T*/
        EDB_COND_T*         pe_cond 
        )
{
    INT32                   i4_ret                          = EDBR_OK;
    EDB_CH_TIME_FILTER_T*   pt_ch_time_filter               = NULL;
    EDB_EVENT_POOL_CH_T*    pt_ch_obj                       = NULL;
    HANDLE_T                h_event_hash                    = NULL_HANDLE;
    HASH_PARSE_TAG          t_parse_tag                     = {0};
    EDB_BUCKET_INFO_T*      pt_bucket                       = NULL;
    DVB_ENG_DATA*           pt_eng_data                     = NULL;
    UINT32                  ui4_event_min_secs              = 0;
    BOOL                    fg_fake_event_insertion_enable  = FALSE;
    BOOL                    fg_time_conflict_allow          = FALSE;
    BOOL                    fg_time_partial_overlap_allow   = FALSE;
    UINT32                  ui4_fake_event_min_secs         = 0 ;
    UINT16                  ui2_num_events                  = 0;

    i4_ret = edb_dvb_get_eng_data_from_ctrl(h_edb_eng, (VOID**)&pt_eng_data);
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK || pt_eng_data == NULL),
            (EDBR_INTERNAL_ERROR),
            ((_EDB_INFO"Can not get engine data from client\r\n"))
            );

    ui4_event_min_secs = 
        pt_eng_data->t_edb_eng_state.ui4_event_min_secs;

    fg_fake_event_insertion_enable = 
        pt_eng_data->t_edb_eng_state.fg_fake_event_insertion_enable;

    fg_time_conflict_allow  = 
        pt_eng_data->t_edb_eng_state.fg_time_conflict_allow;

    fg_time_partial_overlap_allow = 
        pt_eng_data->t_edb_eng_state.fg_time_partial_overlap_allow;

    ui4_fake_event_min_secs = 
        pt_eng_data->t_edb_eng_state.fake_event_min_secs;

    EDB_CHECK_RET( 
            (h_edb_eng == NULL_HANDLE ||  pv_key_info == NULL || pz_event_info_len == NULL || pv_event_info == NULL),
            (EDBR_INV_ARG),
            (("Wrong argument\r\n"))
            );

    pt_ch_time_filter = (EDB_CH_TIME_FILTER_T*)pv_key_info;
    i4_ret = _edb_dvb_util_get_channel_by_edb_eng(h_edb_eng,pt_ch_time_filter->ui4_channel_id, &pt_ch_obj);
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK || pt_ch_obj == NULL),
            (EDBR_INTERNAL_ERROR),
            (("Get pool channel fail\r\n"))
            );

    h_event_hash = pt_ch_obj->h_event_hash;
    EDB_CHECK_RET( 
            (h_event_hash == NULL_HANDLE),
            (EDBR_INTERNAL_ERROR),
            (("Event hash is null\r\n"))
            );

    pt_bucket =(EDB_BUCKET_INFO_T*) pv_event_info;

    t_parse_tag.pv_obj1 = pt_ch_time_filter;    /* EDB_CH_TIME_FILTER_T*  */
    t_parse_tag.pv_obj2 = pt_ch_obj;    /* SIZE_T*  */
    t_parse_tag.pv_obj3 = pt_bucket;        /* EDB_BUCKET_INFO_T*  */
    t_parse_tag.pv_obj4 = (VOID*)ui4_event_min_secs;

    ui2_num_events      = *(pt_bucket->pui2_num_events);
    t_parse_tag.pv_obj5 = (VOID*) (UINT32)ui2_num_events;/*caller numer of events*/
	t_parse_tag.pv_obj6 = (VOID*) (UINT32)h_edb_eng;


    *(pt_bucket->pui2_num_events)   = (UINT16)0;
    pt_bucket->ui2_checksum         = (UINT16)0x0;

    x_memset(&(apt_pool_event[0]) ,0x0,sizeof(apt_pool_event));
    ui1_apt_pool_event_num = 0;
    i4_ret = edb_hash_parse(h_event_hash, _edb_dvb_iterator_event_ids, &t_parse_tag);

    //Sort event ids by start time
    {
        UINT8                   ui1_id_num              = ui1_apt_pool_event_num;
        INT32                   i                       = 0;
        INT32                   j                       = 0;
        EDB_POOL_EVENT_INFO_T*  pt_event_tmp            = NULL;
        TIME_T                  t_start_time_active     = NULL_TIME;
        TIME_T                  t_end_time_active       = NULL_TIME;
        EDB_POOL_EVENT_INFO_T*  pt_fake_event           = NULL;

        pt_ch_obj->ui4_time_stamp = x_os_get_sys_tick();

        //sort event by start time
        for (i=0;i<(INT32)ui1_id_num;i++){
            for (j = (INT32)(ui1_id_num - 1); j > i; j--){
                if ( apt_pool_event[i]->t_start_time  > apt_pool_event[j]->t_start_time ) 
                {
                    pt_event_tmp      = apt_pool_event[i];
                    apt_pool_event[i] = apt_pool_event[j];
                    apt_pool_event[j] = pt_event_tmp;
                }
            }
        }


        /*check event overlap*/
        /*does not allow event overlap*/
        if (fg_time_conflict_allow == FALSE && ui1_id_num > (UINT8)0x1)
        {
            /*remove this event*/
            i=1;
            for (i = 1; i < ui1_id_num;i++)
            {
                if  (
                        apt_pool_event[i]->t_start_time < (apt_pool_event[i-1]->t_start_time + apt_pool_event[i-1]->t_duration)
                    )/*remove this event*/
                {
                    if (/*allow partial overlap*/
                            (fg_time_partial_overlap_allow == TRUE) && 
                            (
                                    (apt_pool_event[i]->t_start_time + apt_pool_event[i]->t_duration) >
                                    (apt_pool_event[i-1]->t_start_time + apt_pool_event[i-1]->t_duration)
                            )	&&
                            ( 
                               (apt_pool_event[i]->t_start_time > pt_ch_time_filter->t_start_time) ||
                               ((apt_pool_event[i-1]->t_start_time + apt_pool_event[i-1]->t_duration) < pt_ch_time_filter->t_end_time) 
                            )
                       )
                    {

                    }
                    else
                    {
                    j=i+1;
                    for (j = i+1; j < ui1_id_num; j++)
                    {
                        apt_pool_event[j -1] = apt_pool_event[j];
                    }
                    ui1_id_num --;
                    i--;
                }
            }
            }

            for (i=0;i<ui1_id_num;i++)
            {
                DBG_INFO(("Event ID=0x%04x [%8d] %8d time:\r\n",
                          apt_pool_event[i]->ui2_event_id,
                          (UINT32)apt_pool_event[i]->t_duration,
                          apt_pool_event[i]->t_start_time + apt_pool_event[i]->t_duration
                         ));
                PRINT_TIME("start time:",apt_pool_event[i]->t_start_time);
            }
        }




        //Insert fake event at head or tail
        if (fg_fake_event_insertion_enable == TRUE)
        {
            t_start_time_active = pt_ch_time_filter->t_start_time;//pt_eng_data->t_edb_eng_state.t_active_win.t_start_time;
            t_end_time_active   = pt_ch_time_filter->t_end_time;//t_start_time_active + pt_eng_data->t_edb_eng_state.t_active_win.t_duration;
            if (ui1_id_num > (UINT8)0x0)
            {
                //check first event with active window start
                if (
                        ((apt_pool_event[0]->t_start_time - t_start_time_active) > (TIME_T)ui4_fake_event_min_secs)
                   )//insert a fake event
                {
                    i4_ret = _edb_dvb_gen_fake_event(
                            &pt_fake_event,t_start_time_active, 
                            apt_pool_event[0]->t_start_time - t_start_time_active);
                    if (i4_ret == EDBR_OK && pt_fake_event!= NULL)
                    {
                        for (j = ui1_id_num -1; j >= 0 ;j--)
                        {
                            apt_pool_event[j+1] = apt_pool_event[j];
                        }
                        apt_pool_event[0] = pt_fake_event;
                        pt_fake_event     = NULL;
                        ui1_id_num++;
                    }

                }


                //check last event
                if (

                        (
                                apt_pool_event[ui1_id_num-1]->t_start_time 
                                +  apt_pool_event[ui1_id_num-1]->t_duration
                                +  (TIME_T)ui4_fake_event_min_secs
                        ) < t_end_time_active

                   )//insert a fake event
                {
                    i4_ret = _edb_dvb_gen_fake_event(
                            &pt_fake_event,
                            apt_pool_event[ui1_id_num-1]->t_start_time +  apt_pool_event[ui1_id_num-1]->t_duration, 
                            t_end_time_active - (apt_pool_event[ui1_id_num-1]->t_start_time +  apt_pool_event[ui1_id_num-1]->t_duration)
                            );
                    if (i4_ret == EDBR_OK && pt_fake_event!= NULL)
                    {
                        apt_pool_event[ui1_id_num] = pt_fake_event;
                        ui1_id_num++;
                    }
                }                
            }
            else //No events found
            {
                i4_ret = _edb_dvb_gen_fake_event(
                        &pt_fake_event,t_start_time_active, 
                        t_end_time_active - t_start_time_active);
                apt_pool_event[0] = pt_fake_event;
                ui1_id_num =1;
            }







            //check need insert fake events
            if (ui1_id_num > 1)
            {
                for (i = 1; i < ui1_id_num;i++)
                {
                    if (    (apt_pool_event[i] != NULL && apt_pool_event[i-1] != NULL) &&
                            (apt_pool_event[i]->t_start_time )
                            - 
                            (apt_pool_event[i-1]->t_start_time + apt_pool_event[i-1]->t_duration) > (TIME_T)ui4_fake_event_min_secs
                       )
                    {
                        i4_ret = _edb_dvb_gen_fake_event(
                                &pt_fake_event,(apt_pool_event[i-1]->t_start_time + apt_pool_event[i-1]->t_duration), 
                                (apt_pool_event[i]->t_start_time )
                                - 
                                (apt_pool_event[i-1]->t_start_time + apt_pool_event[i-1]->t_duration)
                                );

                        if (i4_ret == EDBR_OK && pt_fake_event!= NULL)
                        {
                            for (j = ui1_id_num;j >= i ;j--)
                            {
                                apt_pool_event[j+1] = apt_pool_event[j];
                            }
                            apt_pool_event[i] = pt_fake_event;
                            pt_fake_event     = NULL;
                            ui1_id_num++;
                        }

                    }
                }
            }

            if (ui1_id_num == 0x0)
            {
                DBG_ABORT( DBG_MOD_EDB );
            }

        }


        x_memset(pt_bucket->pui2_ch_event_id,0x0,ui2_num_events*sizeof(EDB_CH_EVT_ID_T));
        for (i=0;i<ui1_id_num && i<ui2_num_events;i++)
        {
            pt_bucket->pui2_ch_event_id[i].ui2_event_id = apt_pool_event[i]->ui2_event_id;
            DBG_INFO(("Event ID=0x%04x [%8d] time info:\r\n",
                      pt_bucket->pui2_ch_event_id[i].ui2_event_id,
                      (UINT32)apt_pool_event[i]->t_duration
                     ));

            PRINT_TIME("start time:",apt_pool_event[i]->t_start_time);
            PRINT_TIME("start time + duration:",apt_pool_event[i]->t_start_time + apt_pool_event[i]->t_duration);
            *(pt_bucket->pui2_num_events) = i + 1;
            pt_bucket->ui2_checksum += _edb_dvb_get_single_event_check_sum(apt_pool_event[i],pt_ch_obj);

            if  ( FALSE && 
                    (i == ui1_id_num -1) &&
                    (apt_pool_event[i]->t_start_time +  apt_pool_event[i]->t_duration +  (TIME_T)ui4_fake_event_min_secs)
                    < t_end_time_active
                )
            {
                apt_pool_event[i]->t_duration = t_end_time_active - apt_pool_event[i]->t_start_time;
        }

        }
        //x_dbg_stmt("%s[%d] get ids checksum=%d\r\n",__FILE__,__LINE__,pt_bucket->ui2_checksum);
    }


#if 0
    DBG_INFO(("channel_id =%d ,Event number=%d\r\n",pt_ch_time_filter->ui4_channel_id, (*(pt_bucket->pui2_num_events)) ));
    {
        UINT32 ui4_index = 0;
        for (ui4_index = 0;ui4_index < *(pt_bucket->pui2_num_events);ui4_index++)
        {
            DBG_INFO(("Event ID=0x%04x \r\n",pt_bucket->pui2_ch_event_id[ui4_index].ui2_event_id));
        }
    }
#endif


    return i4_ret;
}


static INT32 _edb_dvb_copy_single_event(
        HANDLE_T                h_edb_eng,
        EDB_EVENT_POOL_CH_T*    pt_channel,
        EDB_POOL_EVENT_INFO_T*  pt_pool_event,
        EDB_EVENT_INFO_T*       pt_event_info
        )
{
    UINT32              i                   = 0;
    INT32               i4_ret = 0;
    UINT8               ui1_event_title_len = 0;
    UINT8               ui1_event_guidance_len = 0;
    DVB_ENG_DATA*       pt_eng_data         = NULL;
    ISO_3166_COUNT_T    t_country            = {0};
    
    EDB_CHECK_RET( 
            (pt_channel == NULL ||  pt_pool_event == NULL ||  pt_event_info == NULL ),
            (EDBR_INV_ARG),
            (("Wrong argument\r\n"))
            );

    i4_ret = edb_dvb_get_eng_data_from_ctrl(h_edb_eng, (VOID**)&pt_eng_data);
    if (pt_eng_data != NULL)
    {
        x_strncpy(t_country,pt_eng_data->t_edb_eng_state.t_country,ISO_3166_COUNT_LEN);
    }

    pt_event_info->ui2_svl_id       = pt_channel->ui2_svl_id;
    pt_event_info->ui2_event_mask  |= EDB_EVENT_GOT_EVENT_ID;

    pt_event_info->ui4_channel_id   = pt_channel->ui4_channel_id;

    pt_event_info->ui2_event_id     = pt_pool_event->ui2_event_id;

    pt_event_info->t_start_time     = pt_pool_event->t_start_time; 
    pt_event_info->ui2_event_mask  |= EDB_EVENT_GOT_START_TIME;

    pt_event_info->t_duration       = pt_pool_event->t_duration;
    pt_event_info->ui2_event_mask  |= EDB_EVENT_GOT_DURATION;


    pt_event_info->b_caption        = pt_pool_event->b_caption;
    if (pt_event_info->b_caption == TRUE)
    {
        pt_event_info->ui2_event_mask |= EDB_EVENT_GOT_CAPTION;
    }


    pt_event_info->b_free_ca_mode   = pt_pool_event->b_free_ca_mode;
    pt_event_info->ui2_event_mask  |= EDB_EVENT_GOT_FREE_CA_MODE;

    if (pt_pool_event->ps_event_title != NULL)
    {
        ui1_event_title_len = (UINT8) (x_strlen(pt_pool_event->ps_event_title));
        ui1_event_title_len = (ui1_event_title_len> EDB_MAX_LENGTH_EVENT_TITLE) ?
            EDB_MAX_LENGTH_EVENT_TITLE:ui1_event_title_len;
        pt_event_info->ui1_event_title_len = ui1_event_title_len + 1;/*add \0*/
        x_strncpy(pt_event_info->ac_event_title, pt_pool_event->ps_event_title, ui1_event_title_len);
        pt_event_info->ui2_event_mask |= EDB_EVENT_GOT_EVENT_TITLE;
    }
    
    if (((pt_pool_event->ui4_got_mask) & EDB_EVENT_GOT_GUIDANCE) && (pt_pool_event->ps_event_guidance != NULL) )
    {
        pt_event_info->ui1_guidance_mode = pt_pool_event->ui1_guidance_mode ;
        ui1_event_guidance_len = (UINT8) (x_strlen(pt_pool_event->ps_event_guidance));
        ui1_event_guidance_len = (ui1_event_guidance_len > EDB_MAX_LENGTH_EVENT_GUIDANCE) ?
            EDB_MAX_LENGTH_EVENT_GUIDANCE:ui1_event_guidance_len;
        pt_event_info->ui1_event_guidance_len = ui1_event_guidance_len + 1;/*add \0*/
        x_strncpy(pt_event_info->ac_event_guidance, pt_pool_event->ps_event_guidance, ui1_event_guidance_len);
        pt_event_info->ui2_event_mask |= EDB_EVENT_GOT_GUIDANCE;    
    }
    else if (pt_channel->t_channel_guidance_info.ps_event_guidance != NULL)
    {        
        pt_event_info->ui1_guidance_mode = pt_channel->t_channel_guidance_info.ui1_guidance_mode ;
        ui1_event_guidance_len = (UINT8) (x_strlen(pt_channel->t_channel_guidance_info.ps_event_guidance));
        ui1_event_guidance_len = (ui1_event_guidance_len > EDB_MAX_LENGTH_EVENT_GUIDANCE) ?
            EDB_MAX_LENGTH_EVENT_GUIDANCE:ui1_event_guidance_len;
        pt_event_info->ui1_event_guidance_len = ui1_event_guidance_len + 1;/*add \0*/
        x_strncpy(pt_event_info->ac_event_guidance, pt_channel->t_channel_guidance_info.ps_event_guidance, ui1_event_guidance_len);
        pt_event_info->ui2_event_mask |= EDB_EVENT_GOT_GUIDANCE;    
    }
    
    x_memcpy(pt_event_info->aui2_ca_system_id,pt_pool_event->aui2_ca_system_id,sizeof(UINT16) * EDB_MAX_NUM_CA_SYSTEM);

    pt_event_info->ui1_num_rating = pt_pool_event->ui1_num_rating;
    if (pt_event_info->ui1_num_rating > (UINT8)0)
    {
        pt_event_info->ui2_event_mask |= EDB_EVENT_GOT_RATING;
    }

    pt_event_info->ui1_num_event_category = pt_pool_event->ui1_num_event_category;
    x_memcpy(pt_event_info->aui1_event_category, pt_pool_event->aui1_event_category,sizeof(UINT8) *EDB_MAX_NUM_CATEGORY );
    if (pt_event_info->ui1_num_event_category > (UINT8)0)
    {
        pt_event_info->ui2_event_mask |= EDB_EVENT_GOT_CONTENT;
    }

    for (i=0;i<EDB_MAX_NUM_CATEGORY;i++)
    {
        if (
                (pt_event_info->aui1_event_category[i] == (UINT16)0xF0) &&
                (x_strncmp(t_country,"GBR",3) == 0)
           )
        {
            if (pt_pool_event->b_has_private == TRUE)
            {

            }
            else
            {
                DBG_INFO(("Category is 0xF Country is UK,private data descriptor=%d\r\n",pt_pool_event->b_has_private));
                DBG_INFO(("change value to 0x0\r\n"));
                pt_event_info->aui1_event_category[i] = 0x0;
            }
        }
    }


    if (pt_pool_event->pt_component_list!=NULL && pt_pool_event->pt_component_list->pt_event_component != NULL)
    {
        UINT8   ui1_idx                             = 0;
        EDB_EVENT_COMPONENT*    pt_event_component  = pt_pool_event->pt_component_list->pt_event_component;
        pt_event_info->ui1_num_comp_info = pt_pool_event->pt_component_list->ui1_num;
        for( ui1_idx = 0;ui1_idx <pt_pool_event->pt_component_list->ui1_num && ui1_idx < MAX_COMPONENT_INFO; ui1_idx++)
        {
            pt_event_info->at_comp_info[ui1_idx].ui1_stream_content = pt_event_component->ui1_stream_content;
            pt_event_info->at_comp_info[ui1_idx].ui1_component_type = pt_event_component->ui1_component_type;
            pt_event_info->at_comp_info[ui1_idx].ui1_component_tag = pt_event_component->ui1_component_tag;
            x_memcpy(pt_event_info->at_comp_info[ui1_idx].t_lang,pt_event_component->t_lang,ISO_639_LANG_LEN);
            pt_event_component ++;
        }
    }

    if(pt_pool_event->pt_event_linkage_list!=NULL && pt_pool_event->pt_event_linkage_list->pt_event_linkage!=NULL)
    {
        UINT8   ui1_idx         = 0;
        UINT8   ui1_num         = 0;
        for(ui1_idx = 0;ui1_idx<pt_pool_event->pt_event_linkage_list->ui1_num_event_linkage && ui1_num<MAX_EVENT_LINKAGE_INFO; ui1_idx++)   
        {
            if(pt_pool_event->pt_event_linkage_list->pt_event_linkage[ui1_idx].ui1_linkage_type == (UINT8)0x0D)
            {
                pt_event_info->at_linkage_info[ui1_num].ui2_on_id =  pt_pool_event->pt_event_linkage_list->pt_event_linkage[ui1_idx].ui2_on_id;
                pt_event_info->at_linkage_info[ui1_num].ui2_ts_id =  pt_pool_event->pt_event_linkage_list->pt_event_linkage[ui1_idx].ui2_ts_id;
                pt_event_info->at_linkage_info[ui1_num].ui2_svc_id =  pt_pool_event->pt_event_linkage_list->pt_event_linkage[ui1_idx].ui2_svc_id;
                ui1_num++;
            }
        }
        if( ui1_num>0 )
        {
            pt_event_info->ui1_num_linkage_info = ui1_num;
            pt_event_info->ui2_event_mask |= EDB_EVENT_GOT_LINKAGE;
        }
    }
    pt_event_info->ui2_checksum = _edb_dvb_get_single_event_check_sum(pt_pool_event,pt_channel);//pt_pool_event->ui2_checksum;    




    return i4_ret;
}
static INT32 _edb_dvb_process_single_pool_serial_event(
        HANDLE_T            h_edb_eng,
        VOID*               pv_key_info,            /* EDB_CH_EVT_ID_T*          */
        SIZE_T*             pz_event_info_len,      /* sizeof(EDB_EVENT_INFO_T)  */ 
        VOID*               pv_event_info,          /* Serialized memory buffer* */
        EDB_COND_T*         pe_cond 
        )
{
    INT32                   i4_ret          = EDBR_OK;
    EDB_CH_EVT_ID_T*        pt_ch_evt_id    = NULL;
    EDB_EVENT_POOL_CH_T*    pt_channel      = NULL;
    HANDLE_T                h_event_hash    = NULL_HANDLE;
    EDB_POOL_EVENT_INFO_T*  pt_pool_event   = NULL;


    EDB_CHECK_RET( 
            (h_edb_eng == NULL_HANDLE ||  pv_key_info == NULL || pz_event_info_len == NULL || pv_event_info == NULL),
            (EDBR_INV_ARG),
            (("Wrong argument\r\n"))
            );

    pt_ch_evt_id = (EDB_CH_EVT_ID_T*)pv_key_info;
    i4_ret = _edb_dvb_util_get_channel_by_edb_eng(h_edb_eng,pt_ch_evt_id->ui4_channel_id, &pt_channel);
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK || pt_channel == NULL),
            (EDBR_INTERNAL_ERROR),
            (("Get pool channel fail\r\n"))
            );


    h_event_hash = pt_channel->h_event_hash;
    EDB_CHECK_RET( 
            (h_event_hash == NULL_HANDLE),
            (EDBR_INTERNAL_ERROR),
            (("Event hash is null\r\n"))
            );


    i4_ret = edb_pool_get_event_obj(pt_channel, pt_ch_evt_id->ui2_event_id, &pt_pool_event);
    if  (i4_ret != EDBR_OK || pt_pool_event == NULL)
    {
        //check is fake event
        i4_ret = _edb_dvb_get_fake_event(pt_ch_evt_id->ui2_event_id, &pt_pool_event);
        EDB_CHECK_RET( 
                (i4_ret == EDBR_INFO_NOT_FOUND || pt_pool_event == NULL),
                (EDBR_INTERNAL_ERROR),
                (("Can not get event by id:%d\r\n",pt_ch_evt_id->ui2_event_id))
                );
    }

   
#if 1
    DBG_INFO(("Event id=0x%04x\r\n",pt_pool_event->ui2_event_id));
#endif

    //serial pool event to memory 
    {
        UINT16  ui2_event_serial_len = 0;
        edb_serialize_pool_event(pt_pool_event,pv_event_info,&ui2_event_serial_len);
        *pz_event_info_len = ui2_event_serial_len;
    }
    return i4_ret;
}

static INT32 _edb_dvb_process_single_event(
        HANDLE_T            h_edb_eng,
        VOID*               pv_key_info,            /* EDB_CH_EVT_ID_T*          */
        SIZE_T*             pz_event_info_len,     /* sizeof(EDB_EVENT_INFO_T)  */ 
        VOID*               pv_event_info,           /* EDB_EVENT_INFO_T*          */
        EDB_COND_T*         pe_cond 
        )
{
    INT32                   i4_ret          = EDBR_OK;
    EDB_CH_EVT_ID_T*        pt_ch_evt_id    = NULL;
    EDB_EVENT_POOL_CH_T*    pt_channel      = NULL;
    HANDLE_T                h_event_hash    = NULL_HANDLE;
    EDB_POOL_EVENT_INFO_T*  pt_pool_event   = NULL;
    EDB_EVENT_INFO_T*       pt_event        = NULL;


    EDB_CHECK_RET( 
            (h_edb_eng == NULL_HANDLE ||  pv_key_info == NULL || pz_event_info_len == NULL || pv_event_info == NULL),
            (EDBR_INV_ARG),
            (("Wrong argument\r\n"))
            );

    pt_ch_evt_id = (EDB_CH_EVT_ID_T*)pv_key_info;
    i4_ret = _edb_dvb_util_get_channel_by_edb_eng(h_edb_eng,pt_ch_evt_id->ui4_channel_id, &pt_channel);
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK || pt_channel == NULL),
            (EDBR_INTERNAL_ERROR),
            (("Get pool channel fail\r\n"))
            );


    h_event_hash = pt_channel->h_event_hash;
    EDB_CHECK_RET( 
            (h_event_hash == NULL_HANDLE),
            (EDBR_INTERNAL_ERROR),
            (("Event hash is null\r\n"))
            );

    pt_event =(EDB_EVENT_INFO_T*) pv_event_info;

    i4_ret = edb_pool_get_event_obj(pt_channel, pt_ch_evt_id->ui2_event_id, &pt_pool_event);
    if  (i4_ret != EDBR_OK || pt_pool_event == NULL)
    {
        //check is fake event
        i4_ret = _edb_dvb_get_fake_event(pt_ch_evt_id->ui2_event_id, &pt_pool_event);
        EDB_CHECK_RET( 
                (i4_ret == EDBR_INFO_NOT_FOUND || pt_pool_event == NULL),
                (EDBR_INTERNAL_ERROR),
                (("Can not get event by id:%d\r\n",pt_ch_evt_id->ui2_event_id))
                );
    }


#if 1
    DBG_INFO(("Event id=0x%04x\r\n",pt_pool_event->ui2_event_id));
#endif

    //Copy event from pool to EDB_EVENT_INFO_T
    //
    i4_ret = _edb_dvb_copy_single_event(h_edb_eng,pt_channel,pt_pool_event,pt_event);

#if 0
    x_dbg_stmt("Event id=0x%04x name=%s\r\n",pt_pool_event->ui2_event_id,pt_pool_event->ps_event_title);
#endif

    //print channel id event id

    return i4_ret;
}



static INT32 _edb_dvb_process_single_event_by_utc(
        HANDLE_T            h_edb_eng,
        VOID*               pv_key_info,                //EDB_CH_TIME_FILTER_T*
        SIZE_T*             pz_event_info_len,          /* sizeof(EDB_EVENT_INFO_T)  */ 
        VOID*               pv_event_info,              /* EDB_EVENT_INFO_T*          */
        EDB_COND_T*         pe_cond 
        )
{
    INT32                   i4_ret      = EDBR_OK;
    EDB_CH_TIME_FILTER_T*   pt_ch_time_filter = NULL;
    EDB_EVENT_POOL_CH_T*    pt_ch_obj = NULL;
    TIME_T                  t_start_time = NULL_TIME;

    UINT32                  ui4_i           = 0;
    P_ELEMENT_OBJ_T         pt_elm          = NULL;
    HANDLE_TYPE_T           e_hdl_type;
    EDB_HASH_T*             pt_hash_obj     = NULL;
    EDB_POOL_EVENT_INFO_T*  pt_pool_event   = NULL;
    EDB_EVENT_INFO_T*       pt_event        = NULL;

    EDB_CHECK_RET( 
            (h_edb_eng == NULL_HANDLE ||  pv_key_info == NULL || pz_event_info_len == NULL || pv_event_info == NULL),
            (EDBR_INV_ARG),
            (("Wrong argument\r\n"))
            );

    pt_ch_time_filter = (EDB_CH_TIME_FILTER_T*)pv_key_info;
    t_start_time        = pt_ch_time_filter->t_start_time;

    i4_ret = _edb_dvb_util_get_channel_by_edb_eng(h_edb_eng,pt_ch_time_filter->ui4_channel_id, &pt_ch_obj);
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK || pt_ch_obj == NULL||pt_ch_obj->h_event_hash == NULL_HANDLE),
            (EDBR_INTERNAL_ERROR),
            (("Get pool channel fail\r\n"))
            );



    i4_ret = handle_get_type_obj( pt_ch_obj->h_event_hash, &e_hdl_type, (VOID**)&pt_hash_obj );
    if ( i4_ret != HR_OK || e_hdl_type != EDBT_HASH )
    {
        return EDBR_INV_HANDLE;
    }

    for ( ui4_i=0; ui4_i<pt_hash_obj->z_table_size; ++ui4_i )
    {
        for( pt_elm = pt_hash_obj->ppt_table[ui4_i]; pt_elm != NULL; )
        {
            pt_pool_event = (EDB_POOL_EVENT_INFO_T*)pt_elm->pv_obj;
            if (    (pt_pool_event != NULL) &&
                    (t_start_time >= pt_pool_event->t_start_time) &&
                    (t_start_time <= (pt_pool_event->t_start_time + pt_pool_event->t_duration) )
               )
            {
                //Copy event from pool to EDB_EVENT_INFO_T
                pt_event =(EDB_EVENT_INFO_T*) pv_event_info;
                i4_ret = _edb_dvb_copy_single_event(h_edb_eng,pt_ch_obj,pt_pool_event,pt_event);
                break;
            }
            pt_elm = pt_elm->pt_next;
        }
    }



    return i4_ret;
}


static VOID _edb_dvb_expired_timer_cb(
        HANDLE_T            h_timer,
        VOID*               pv_tag)
{
    INT32               i4_ret      = EDBR_OK;
    DVB_ENG_DATA*       pt_eng_data = NULL;
    EDB_DVB_MSG_T       t_msg       = {0};
    HANDLE_T            h_edb_eng   = NULL_HANDLE;


    if (pv_tag == NULL)
    {
        //DBG_INFO(("Invalid argument\r\n"));
        return;
    }


    h_edb_eng = (HANDLE_T)(pv_tag);

    i4_ret = edb_dvb_get_eng_data_from_ctrl(h_edb_eng,(VOID**)&pt_eng_data);
    if (i4_ret != EDBR_OK || pt_eng_data == NULL)
    {
        //DBG_INFO(("Invalid argument\r\n"));
        return;
    }

    //Send msg switch thread context
    //DBG_INFO((_EDB_INFO"expire timer callback\r\n"));
    t_msg.e_msg_type            = EDB_DVB_MSG_EXPIRE_NFY;
    t_msg.pv_tag                = (VOID*)h_edb_eng; /*HANDLE_T  */

    if (x_msg_q_send(h_dvb_msg_q,
                     &t_msg,
                     sizeof(EDB_DVB_MSG_T),
                     0x0 /* highest priority */) != OSR_OK)
    {
        /* Send message fail, wait 10 ms and retry for 10 times */
        //DBG_INFO(("Send message fail\r\n"));
    }
    edb_activate_eng_task(EDB_DVB_BRDCST_GRP);
    return ;
}

#if 1
static VOID _edb_dvb_check_expired_timer_cb(
        HANDLE_T            h_timer,
        VOID*               pv_tag)
{
    INT32               i4_ret      = EDBR_OK;
    DVB_ENG_DATA*       pt_eng_data = NULL;
    EDB_DVB_MSG_T       t_msg       = {0};
    HANDLE_T            h_edb_eng   = NULL_HANDLE;


    if (pv_tag == NULL)
    {
        //DBG_INFO(("Invalid argument\r\n"));
        return;
    }


    h_edb_eng = (HANDLE_T)(pv_tag);

    i4_ret = edb_dvb_get_eng_data_from_ctrl(h_edb_eng,(VOID**)&pt_eng_data);
    if (i4_ret != EDBR_OK || pt_eng_data == NULL)
    {
        //DBG_INFO(("Invalid argument\r\n"));
        return;
    }

    //Send msg switch thread context
    DBG_INFO((_EDB_INFO"expire timer callback\r\n"));
    t_msg.e_msg_type            = EDB_DVB_MSG_CHECK_EXPIRE_NFY;
    t_msg.pv_tag                = (VOID*)h_edb_eng; /*HANDLE_T  */

    if (x_msg_q_send(h_dvb_msg_q,
                     &t_msg,
                     sizeof(EDB_DVB_MSG_T),
                     0x0 /* highest priority */) != OSR_OK)
    {
        /* Send message fail, wait 10 ms and retry for 10 times */
        //DBG_INFO(("Send message fail\r\n"));
    }
    edb_activate_eng_task(EDB_DVB_BRDCST_GRP);
    return ;
}
#endif

static VOID _edb_dvb_active_timer_cb(
        HANDLE_T            h_timer,
        VOID*               pv_tag)
{
    INT32               i4_ret      = EDBR_OK;
    DVB_ENG_DATA*       pt_eng_data = NULL;
    EDB_DVB_MSG_T       t_msg       = {0};
    HANDLE_T            h_edb_eng   = NULL_HANDLE;

    if (pv_tag == NULL)
    {
        //DBG_INFO(("Invalid argument\r\n"));
        return;
    }


    h_edb_eng = (HANDLE_T)(pv_tag);

    i4_ret = edb_dvb_get_eng_data_from_ctrl(h_edb_eng,(VOID**)&pt_eng_data);
    if (i4_ret != EDBR_OK || pt_eng_data == NULL)
    {
        //DBG_INFO(("Invalid argument\r\n"));
        return;
    }

    //Send msg switch thread context
    //DBG_INFO((_EDB_INFO"active timer callback\r\n"));
    t_msg.e_msg_type            = EDB_DVB_MSG_ACTIVE_NFY;
    t_msg.pv_tag                = (VOID*)h_edb_eng; /*HANDLE_T  */

    if (x_msg_q_send(h_dvb_msg_q,
                     &t_msg,
                     sizeof(EDB_DVB_MSG_T),
                     0x0 /* highest priority */) != OSR_OK)
    {
        /* Send message fail, wait 10 ms and retry for 10 times */
        //DBG_INFO(("Send message fail\r\n"));
    }
    edb_activate_eng_task(EDB_DVB_BRDCST_GRP);
    return ;
}


static INT32 _edb_dvb_start_ae_timer(
        HANDLE_T                h_edb_eng,
        EDB_EVENT_POOL_CH_T*    pt_ch_obj,
        BOOL                    fg_current_ready,
        BOOL                    fg_next_ready
        )
{
    INT32                   i4_ret              = 0x0;
    DT_COND_T               t_dt_cond           = DT_NOT_RUNNING;
    TIME_T                  t_delta             = NULL_TIME;
    DVB_ENG_DATA*           pt_eng_data         = NULL;
    TIME_T                  t_current_time      = NULL_TIME;

    t_current_time = x_dt_get_brdcst_utc(NULL,&t_dt_cond);


    i4_ret = edb_dvb_get_eng_data_from_ctrl(h_edb_eng, (VOID**)&pt_eng_data);
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK || pt_eng_data == NULL),
            (EDBR_INTERNAL_ERROR),
            ((_EDB_INFO"Can not get engine data from client\r\n"))
            );


    /*start current event expire timer*************************************************************/ 
    if (t_dt_cond == DT_SYNC_RUNNING)
    {
        if (fg_current_ready == TRUE && pt_ch_obj->pt_present_event != NULL )
        {
            if (pt_ch_obj->pt_present_event->t_start_time < t_current_time)
            {
                t_delta = pt_ch_obj->pt_present_event->t_start_time + 
                    pt_ch_obj->pt_present_event->t_duration - 
                    t_current_time ;
            }

            if (pt_eng_data->h_expire_timer != NULL_HANDLE)
            {
                if (x_timer_stop(pt_eng_data->h_expire_timer) == OSR_OK)
                {
                    DBG_INFO((_EDB_INFO"current event start+duration=%llu brdcst_time=%llu\r\n",
                              pt_ch_obj->pt_present_event->t_start_time + pt_ch_obj->pt_present_event->t_duration,
                              t_current_time
                             ));
                    PRINT_TIME("",pt_ch_obj->pt_present_event->t_start_time + pt_ch_obj->pt_present_event->t_duration);
                    PRINT_TIME("current brdcst:",t_current_time);
                    DBG_INFO((_EDB_INFO"start expire time for current event %d\r\n",(UINT32)t_delta));
                    if ((UINT32)t_delta > 0)
                    {
                        x_timer_start(pt_eng_data->h_expire_timer,
                                      (UINT32)t_delta*1000,
                                      X_TIMER_FLAG_ONCE,
                                      _edb_dvb_expired_timer_cb,
                                      (VOID*) h_edb_eng);
                    }
                } 
            }
        }


        /*start next  event active timer*/
        if (fg_next_ready == TRUE && pt_ch_obj->pt_following_event != NULL )
        {
            if(pt_ch_obj->pt_following_event->t_start_time > t_current_time)
            {
                t_delta = pt_ch_obj->pt_following_event->t_start_time - t_current_time;
            }

            if (pt_eng_data->h_active_timer != NULL_HANDLE)
            {
                if (x_timer_stop(pt_eng_data->h_active_timer) == OSR_OK)
                {
                    DBG_INFO((_EDB_INFO"next event start=%llu brdcst_time=%llu\r\n",
                              pt_ch_obj->pt_following_event->t_start_time,
                              t_current_time
                             ));
                    PRINT_TIME("start time:",pt_ch_obj->pt_following_event->t_start_time);
                    PRINT_TIME("current brdcst:",t_current_time);
                    DBG_INFO((_EDB_INFO"start active time for next event %d\r\n",(UINT32)t_delta));
                    if ((UINT32)t_delta > 0)
                    {
                        x_timer_start(pt_eng_data->h_active_timer,
                                      (UINT32)t_delta*1000,
                                      X_TIMER_FLAG_ONCE,
                                      _edb_dvb_active_timer_cb,
                                      (VOID*) h_edb_eng);
                    }
                } 
            }
        }
    }
    else
    {

    }
    /******************************************************************************************/
    return i4_ret;
}



static INT32 _edb_dvb_start_check_expire_timer(
        HANDLE_T                h_edb_eng
        )
{
    INT32                   i4_ret              = 0x0;
#if 1
    TIME_T                  t_delta             = NULL_TIME;
    DVB_ENG_DATA*           pt_eng_data         = NULL;

    i4_ret = edb_dvb_get_eng_data_from_ctrl(h_edb_eng, (VOID**)&pt_eng_data);
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK || pt_eng_data == NULL),
            (EDBR_INTERNAL_ERROR),
            ((_EDB_INFO"Can not get engine data from client\r\n"))
            );

    t_delta = (TIME_T)EDB_DVB_CHECK_EXPIRE_TIMER;//3 hours

    /*start current event expire timer*************************************************************/ 
    x_timer_stop(pt_eng_data->h_check_expire_timer);
    i4_ret = x_timer_start(pt_eng_data->h_check_expire_timer,
                           (UINT32)t_delta * 1000,
                           X_TIMER_FLAG_REPEAT,
                           _edb_dvb_check_expired_timer_cb,
                           (VOID*) h_edb_eng);
#endif
    /******************************************************************************************/
    return i4_ret;
}



static INT32 _edb_dvb_search_present_event_from_pool_by_utc(EDB_EVENT_POOL_CH_T*    pt_ch_obj,
																	   TIME_T				    t_needed_time,
																	   EDB_POOL_EVENT_INFO_T**  ppt_needed_event
																	   )
{
    INT32                           i4_ret          = EDBR_INFO_NOT_FOUND;
    UINT32                          ui4_i           = 0;
    P_ELEMENT_OBJ_T                 pt_elm          = NULL;
    EDB_HASH_T*                     pt_hash_obj     = NULL;
    EDB_POOL_EVENT_INFO_T*          pt_pool_event   = NULL;
    HANDLE_TYPE_T                   e_hdl_type;

    EDB_CHECK_RET( 
            (pt_ch_obj == NULL),
            (EDBR_INTERNAL_ERROR),
            (("Channel obj is null\r\n"))
            );

    i4_ret = handle_get_type_obj( pt_ch_obj->h_event_hash, &e_hdl_type, (VOID**)&pt_hash_obj );
    if ( i4_ret != HR_OK || e_hdl_type != EDBT_HASH )
    {
        return EDBR_INV_HANDLE;
    }
    
    i4_ret          = EDBR_INFO_NOT_FOUND;

    for ( ui4_i=0; ui4_i<pt_hash_obj->z_table_size; ++ui4_i )
    {
        for( pt_elm = pt_hash_obj->ppt_table[ui4_i]; pt_elm != NULL; )
        {
            pt_pool_event = (EDB_POOL_EVENT_INFO_T*)pt_elm->pv_obj;
            if (    (pt_pool_event != NULL) &&
                    (t_needed_time >= pt_pool_event->t_start_time) &&
                    (t_needed_time < (pt_pool_event->t_start_time + pt_pool_event->t_duration) )
               )//Found event from schedule list
            {
                
                (*ppt_needed_event) = pt_pool_event;               
                i4_ret = EDBR_OK;
                break;
            }
            pt_elm = pt_elm->pt_next;
        }
    }

    return i4_ret;
}





static INT32 _edb_dvb_search_following_event_from_pool_by_utc(EDB_EVENT_POOL_CH_T*    pt_ch_obj,
														                 TIME_T				     t_needed_time,
														                 EDB_POOL_EVENT_INFO_T**  ppt_needed_event)
{
    INT32                           i4_ret          = EDBR_INFO_NOT_FOUND;
    UINT32                          ui4_i           = 0;
    P_ELEMENT_OBJ_T                 pt_elm          = NULL;
    EDB_HASH_T*                     pt_hash_obj     = NULL;
    EDB_POOL_EVENT_INFO_T*          pt_pool_event   = NULL;
    HANDLE_TYPE_T                   e_hdl_type;
    TIME_T                          t_start_time    = NULL_TIME;

    EDB_CHECK_RET( 
            (pt_ch_obj == NULL),
            (EDBR_INTERNAL_ERROR),
            (("Channel obj is null\r\n"))
            );
	
	PRINT_TIME("t_needed_time:",t_needed_time);

    i4_ret = handle_get_type_obj( pt_ch_obj->h_event_hash, &e_hdl_type, (VOID**)&pt_hash_obj );
    if ( i4_ret != HR_OK || e_hdl_type != EDBT_HASH )
    {
        return EDBR_INV_HANDLE;
    }
    
    i4_ret          = EDBR_INFO_NOT_FOUND;

    for ( ui4_i=0; ui4_i<pt_hash_obj->z_table_size; ++ui4_i )
    {
        for( pt_elm = pt_hash_obj->ppt_table[ui4_i]; pt_elm != NULL; )
        {
            pt_pool_event = (EDB_POOL_EVENT_INFO_T*)pt_elm->pv_obj;
            if (    (pt_pool_event != NULL) &&
                    (t_needed_time <= pt_pool_event->t_start_time) 
               )
            {
                if (t_start_time == NULL_TIME)
                {
                    t_start_time = pt_pool_event->t_start_time;
                    (*ppt_needed_event) = pt_pool_event;
                    i4_ret          = EDBR_OK;

                }
                else
                {
                    if(pt_pool_event->t_start_time < t_start_time)
                    {
                        t_start_time = pt_pool_event->t_start_time;
                        (*ppt_needed_event) = pt_pool_event;
                        i4_ret          = EDBR_OK;
                    }
                }
            }
            pt_elm = pt_elm->pt_next;
        }
    }

    return i4_ret;
}


static INT32 _edb_dvb_search_previous_event_from_pool_by_utc(EDB_EVENT_POOL_CH_T*    pt_ch_obj,
														                 TIME_T				     t_needed_time,
														                 EDB_POOL_EVENT_INFO_T**  ppt_needed_event)
{
    INT32                           i4_ret          = EDBR_INFO_NOT_FOUND;
    UINT32                          ui4_i           = 0;
    P_ELEMENT_OBJ_T                 pt_elm          = NULL;
    EDB_HASH_T*                     pt_hash_obj     = NULL;
    EDB_POOL_EVENT_INFO_T*          pt_pool_event   = NULL;
    HANDLE_TYPE_T                   e_hdl_type;
    TIME_T                          t_start_time    = NULL_TIME;

    EDB_CHECK_RET( 
            (pt_ch_obj == NULL),
            (EDBR_INTERNAL_ERROR),
            (("Channel obj is null\r\n"))
            );
	
	PRINT_TIME("t_needed_time:",t_needed_time);

    i4_ret = handle_get_type_obj( pt_ch_obj->h_event_hash, &e_hdl_type, (VOID**)&pt_hash_obj );
    if ( i4_ret != HR_OK || e_hdl_type != EDBT_HASH )
    {
        return EDBR_INV_HANDLE;
    }
    
    i4_ret          = EDBR_INFO_NOT_FOUND;

    for ( ui4_i=0; ui4_i<pt_hash_obj->z_table_size; ++ui4_i )
    {
        for( pt_elm = pt_hash_obj->ppt_table[ui4_i]; pt_elm != NULL; )
        {
            pt_pool_event = (EDB_POOL_EVENT_INFO_T*)pt_elm->pv_obj;
            if (    (pt_pool_event != NULL) &&
                    (t_needed_time > pt_pool_event->t_start_time) 
               )
            {
                if (t_start_time == NULL_TIME)
                {
                    t_start_time = pt_pool_event->t_start_time;
                    (*ppt_needed_event) = pt_pool_event;
                    i4_ret          = EDBR_OK;

                }
                else
                {
                    if(pt_pool_event->t_start_time > t_start_time)
                    {
                        t_start_time = pt_pool_event->t_start_time;
                        (*ppt_needed_event) = pt_pool_event;
                        i4_ret          = EDBR_OK;
                    }
                }
            }
            pt_elm = pt_elm->pt_next;
        }
    }

    return i4_ret;
}

static INT32 _edb_dvb_process_current_and_next_events_by_utc(
			HANDLE_T			h_edb_eng,
			VOID*				pv_key_info,			   //EDB_CH_TIME_FILTER_T*
			SIZE_T* 			pz_event_info_len,		  //SIZE_T*
			VOID*				pv_event_info,				//EDB_BUCKET_INFO_T*
			EDB_COND_T* 		pe_cond 
		)
{
#ifdef PARA_FROM_AP
    EDB_CH_TIME_FILTER_T*  & t_ch_time_filter;
    & t_size, sizeof(EDB_BUCKET_INFO_T);
    EDB_BUCKET_INFO_T*     & t_bucket_info;
#endif

    INT32                   i4_ret              = EDBR_OK;
    INT32                   i4_ret_out          = EDBR_INFO_NOT_FOUND;
    EDB_CH_TIME_FILTER_T*   pt_ch_time_filter   = NULL;
    EDB_EVENT_POOL_CH_T*    pt_ch_obj           = NULL;
    EDB_BUCKET_INFO_T*      pt_bucket           = NULL;
    TIME_T                  t_start_time        = NULL_TIME;
    UINT32                  ui4_channel_id      = 0x0;
    DVB_ENG_DATA*           pt_eng_data = NULL;
    EDB_POOL_EVENT_INFO_T*  pt_now_event = NULL;
    EDB_POOL_EVENT_INFO_T*  pt_next_event = NULL;
    TIME_T                  t_next_start_search_utc = NULL_TIME;
/*
    DTG_T     t_dtg;
    DTG_T     t_local_dtg;
*/
    EDB_CHECK_RET( 
            (h_edb_eng == NULL_HANDLE ||  pv_key_info == NULL || pz_event_info_len == NULL || pv_event_info == NULL),
            (EDBR_INV_ARG),
            (("Wrong argument\r\n"))
            );

    pt_ch_time_filter   = (EDB_CH_TIME_FILTER_T*)pv_key_info;
    t_start_time        = pt_ch_time_filter->t_start_time;
    t_next_start_search_utc = t_start_time ;
    ui4_channel_id = pt_ch_time_filter->ui4_channel_id;
    if (ui4_channel_id == 0x0)
    {
        i4_ret = edb_dvb_get_eng_data_from_ctrl(h_edb_eng,(VOID**)&pt_eng_data);
        if (pt_eng_data != NULL)
        {
            ui4_channel_id = pt_eng_data->t_edb_eng_state.ui4_current_channel_id;
        }
    }
    PRINT_TIME("tiezheng start time\r\n:",t_start_time);

    /*
    x_dt_utc_sec_to_dtg(t_start_time, &t_dtg);
    x_dt_conv_utc_local(&t_dtg, &t_local_dtg);
    x_dbg_stmt("tiezheng start time %llu %d/%d/%d %d:%d:%d ",
              t_start_time,
              t_local_dtg.ui2_yr,
              t_local_dtg.ui1_mo,
              t_local_dtg.ui1_day,
              t_local_dtg.ui1_hr,
              t_local_dtg.ui1_min,
              t_local_dtg.ui1_sec);
*/

    i4_ret = _edb_dvb_util_get_channel_by_edb_eng(h_edb_eng,ui4_channel_id, &pt_ch_obj);
    
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK || pt_ch_obj == NULL),
            (EDBR_INTERNAL_ERROR),
            (("Get pool channel fail\r\n"))
            );


    pt_bucket =(EDB_BUCKET_INFO_T*) pv_event_info;
    *(pt_bucket->pui2_num_events) = (UINT16)0;
    pt_bucket->ui2_checksum       = (UINT16)0;

    /*x_dbg_stmt("onid[0x%04x] tsid[0x%04x] svcid[0x%04x] Present event,0x%04x (0x%08x) time:\r\n",
                      pt_ch_obj->t_key.ui2_on_id,
                      pt_ch_obj->t_key.ui2_ts_id,
                      pt_ch_obj->t_key.ui2_svc_id,
                      pt_bucket->pui2_ch_event_id[ 0 ].ui2_event_id,
                      pt_ch_obj->pt_present_event
                     );
*/


    i4_ret = _edb_dvb_search_present_event_from_pool_by_utc(pt_ch_obj,t_start_time,&pt_now_event);

    if((i4_ret == EDBR_OK)&&(pt_now_event != NULL))
    {
        /*x_dbg_stmt("search now event by utc success \n");
        x_dbg_stmt("event id 0x%x \n",pt_now_event->ui2_event_id);*/
        pt_bucket->pui2_ch_event_id[ 0 ].ui2_event_id = pt_now_event->ui2_event_id;
        *(pt_bucket->pui2_num_events) = (UINT16) 2;     
        pt_bucket->ui2_checksum += _edb_dvb_get_single_event_check_sum(pt_now_event,pt_ch_obj);
        t_next_start_search_utc = pt_now_event->t_start_time + pt_now_event->t_duration ;
        i4_ret_out = EDBR_OK;

    }

    i4_ret = _edb_dvb_search_following_event_from_pool_by_utc(pt_ch_obj,t_next_start_search_utc,&pt_next_event);

    if((i4_ret == EDBR_OK)&&(pt_next_event != NULL))
    {
        /*x_dbg_stmt("search next event by utc success \n");
        x_dbg_stmt("event id 0x%x \n",pt_next_event->ui2_event_id);*/
        pt_bucket->pui2_ch_event_id[ 1 ].ui2_event_id = pt_next_event->ui2_event_id;
        *(pt_bucket->pui2_num_events) =(UINT16) 2;
        pt_bucket->ui2_checksum += _edb_dvb_get_single_event_check_sum(pt_next_event,pt_ch_obj) ;
        i4_ret_out = EDBR_OK;
    }


    DBG_INFO((_EDB_INFO"_edb_dvb_process_pf_num_by_utc pt_bucket->ui2_checksum=%d\r\n",
              pt_bucket->ui2_checksum));

    return i4_ret_out;
}


static INT32 _edb_dvb_process_previous_event_by_utc(
			HANDLE_T			h_edb_eng,
			VOID*				pv_key_info,			   //EDB_CH_TIME_FILTER_T*
			SIZE_T* 			pz_event_info_len,		  //SIZE_T*
			VOID*				pv_event_info,				//EDB_BUCKET_INFO_T*
			EDB_COND_T* 		pe_cond 
		)
{
	INT32                   i4_ret              = EDBR_OK;
    EDB_CH_TIME_FILTER_T*   pt_ch_time_filter   = NULL;
    EDB_EVENT_POOL_CH_T*    pt_ch_obj           = NULL;
    EDB_BUCKET_INFO_T*      pt_bucket           = NULL;
    TIME_T                  t_start_time        = NULL_TIME;
    UINT32                  ui4_channel_id      = 0x0;
    DVB_ENG_DATA*           pt_eng_data = NULL;
	EDB_POOL_EVENT_INFO_T*  pt_now_event = NULL;
	EDB_POOL_EVENT_INFO_T*	pt_previous_event = NULL;
	TIME_T 					t_previous_start_search_utc = NULL_TIME;
/*
	DTG_T     t_dtg;
    DTG_T     t_local_dtg;
*/
    EDB_CHECK_RET( 
            (h_edb_eng == NULL_HANDLE ||  pv_key_info == NULL || pz_event_info_len == NULL || pv_event_info == NULL),
            (EDBR_INV_ARG),
            (("Wrong argument\r\n"))
            );

    pt_ch_time_filter   = (EDB_CH_TIME_FILTER_T*)pv_key_info;
    t_start_time        = pt_ch_time_filter->t_start_time;
	t_previous_start_search_utc = t_start_time ;
    ui4_channel_id = pt_ch_time_filter->ui4_channel_id;
    if (ui4_channel_id == 0x0)
    {
        i4_ret = edb_dvb_get_eng_data_from_ctrl(h_edb_eng,(VOID**)&pt_eng_data);
        if (pt_eng_data != NULL)
        {
            ui4_channel_id = pt_eng_data->t_edb_eng_state.ui4_current_channel_id;
        }
    }
	PRINT_TIME("tiezheng start time\r\n:",t_start_time);

	/*
    x_dt_utc_sec_to_dtg(t_start_time, &t_dtg);
    x_dt_conv_utc_local(&t_dtg, &t_local_dtg);
    x_dbg_stmt("tiezheng start time %llu %d/%d/%d %d:%d:%d ",
              t_start_time,
              t_local_dtg.ui2_yr,
              t_local_dtg.ui1_mo,
              t_local_dtg.ui1_day,
              t_local_dtg.ui1_hr,
              t_local_dtg.ui1_min,
              t_local_dtg.ui1_sec);
*/

    i4_ret = _edb_dvb_util_get_channel_by_edb_eng(h_edb_eng,ui4_channel_id, &pt_ch_obj);
	
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK || pt_ch_obj == NULL),
            (EDBR_INTERNAL_ERROR),
            (("Get pool channel fail\r\n"))
            );


    pt_bucket =(EDB_BUCKET_INFO_T*) pv_event_info;
    *(pt_bucket->pui2_num_events) = (UINT16)0;
    pt_bucket->ui2_checksum       = (UINT16)0;

	i4_ret = _edb_dvb_search_present_event_from_pool_by_utc(pt_ch_obj,t_start_time,&pt_now_event);

	if((i4_ret == EDBR_OK)&&(pt_now_event != NULL))
	{
		/*x_dbg_stmt("search now event by utc success \n");
		x_dbg_stmt("event id 0x%x \n",pt_now_event->ui2_event_id);*/
		t_previous_start_search_utc = pt_now_event->t_start_time ;

	}

	i4_ret = _edb_dvb_search_previous_event_from_pool_by_utc(pt_ch_obj,t_previous_start_search_utc,&pt_previous_event);

	if((i4_ret == EDBR_OK)&&(pt_previous_event != NULL))
	{
		/*x_dbg_stmt("search next event by utc success \n");
		x_dbg_stmt("event id 0x%x \n",pt_next_event->ui2_event_id);*/
		pt_bucket->pui2_ch_event_id[ 0 ].ui2_event_id = pt_previous_event->ui2_event_id;
        *(pt_bucket->pui2_num_events) =(UINT16) 1;
        pt_bucket->ui2_checksum += _edb_dvb_get_single_event_check_sum(pt_previous_event,pt_ch_obj) ;
	}


    DBG_INFO((_EDB_INFO"_edb_dvb_process_previous_event_by_utc pt_bucket->ui2_checksum=%d\r\n",
              pt_bucket->ui2_checksum));

    return i4_ret;	
}



static INT32 _edb_dvb_process_pf_num_by_utc(
        HANDLE_T            h_edb_eng,
        VOID*               pv_key_info,               //EDB_CH_TIME_FILTER_T*
        SIZE_T*             pz_event_info_len,        //SIZE_T*
        VOID*               pv_event_info,              //EDB_BUCKET_INFO_T*
        EDB_COND_T*         pe_cond 
        )
{
#ifdef PARA_FROM_AP
    EDB_CH_TIME_FILTER_T*  & t_ch_time_filter;
    & t_size, sizeof(EDB_BUCKET_INFO_T);
    EDB_BUCKET_INFO_T*     & t_bucket_info;
#endif

    INT32                   i4_ret              = EDBR_OK;
    EDB_CH_TIME_FILTER_T*   pt_ch_time_filter   = NULL;
    EDB_EVENT_POOL_CH_T*    pt_ch_obj           = NULL;
    EDB_BUCKET_INFO_T*      pt_bucket           = NULL;
    TIME_T                  t_start_time        = NULL_TIME;
    UINT32                  ui4_channel_id      = 0x0;
    DVB_ENG_DATA*           pt_eng_data = NULL;
    DT_COND_T               t_dt_cond             = DT_NOT_RUNNING;
    BOOL                    b_check_time        = TRUE;
    
    x_dt_get_brdcst_utc(NULL,&t_dt_cond);

    EDB_CHECK_RET( 
            (h_edb_eng == NULL_HANDLE ||  pv_key_info == NULL || pz_event_info_len == NULL || pv_event_info == NULL),
            (EDBR_INV_ARG),
            (("Wrong argument\r\n"))
            );

    pt_ch_time_filter   = (EDB_CH_TIME_FILTER_T*)pv_key_info;
    t_start_time        = pt_ch_time_filter->t_start_time;
    if (t_start_time == NULL_TIME)
    {
        b_check_time = FALSE;
    }

    ui4_channel_id = pt_ch_time_filter->ui4_channel_id;
    if (ui4_channel_id == 0x0)
    {
        i4_ret = edb_dvb_get_eng_data_from_ctrl(h_edb_eng,(VOID**)&pt_eng_data);
        if (pt_eng_data != NULL)
        {
            ui4_channel_id = pt_eng_data->t_edb_eng_state.ui4_current_channel_id;
        }
    }

    i4_ret = _edb_dvb_util_get_channel_by_edb_eng(h_edb_eng,ui4_channel_id, &pt_ch_obj);
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK || pt_ch_obj == NULL),
            (EDBR_INTERNAL_ERROR),
            (("Get pool channel fail\r\n"))
            );


    pt_bucket =(EDB_BUCKET_INFO_T*) pv_event_info;
    *(pt_bucket->pui2_num_events) = (UINT16)0;
    pt_bucket->ui2_checksum       = (UINT16)0;


    
    if (pt_ch_obj->b_eit_pf_flag == FALSE)
    {
        i4_ret = edb_dvb_get_eng_data_from_ctrl(h_edb_eng,(VOID**)&pt_eng_data);
        if (pt_eng_data != NULL)
        {
            if(x_strncmp(pt_eng_data->t_edb_eng_state.t_country,"NZL",3) == 0)
            {
                DBG_INFO(("{EDB} CLIENT get pf ,country is NZL ! the eit pf flag is false! ,return !\r\n"));
                return EDBR_INFO_NOT_FOUND;
            }
            else
            {
                DBG_INFO(("{EDB} CLIENT get pf ,country is not NZL ,the eit pf flag is false,but it doesn't matter \r\n"));
            }
        }

    }
    else
    {
        DBG_INFO(("{EDB} CLIENT get pf ,the eit pf flag is true \r\n"));
    }
    

    
    if (pt_ch_obj->pt_present_event != NULL)
    {
        if (
                (!b_check_time) || (t_dt_cond != DT_SYNC_RUNNING) ||
                (pt_ch_obj->pt_present_event->t_start_time <= t_start_time && 
                pt_ch_obj->pt_present_event->t_start_time + pt_ch_obj->pt_present_event->t_duration >= t_start_time)
           )
        {    
#if 0
            pt_bucket->pui2_ch_event_id[ *(pt_bucket->pui2_num_events) ].ui2_event_id = 
                pt_ch_obj->pt_present_event->ui2_event_id;
            *(pt_bucket->pui2_num_events) = (UINT16) (*(pt_bucket->pui2_num_events) + 1);
#else
            
            pt_bucket->pui2_ch_event_id[ 0 ].ui2_event_id = pt_ch_obj->pt_present_event->ui2_event_id;
            *(pt_bucket->pui2_num_events) = (UINT16) 1;
            DBG_INFO(("onid[0x%04x] tsid[0x%04x] svcid[0x%04x] Present event,0x%04x (0x%08x) time:\r\n",
                      pt_ch_obj->t_key.ui2_on_id,
                      pt_ch_obj->t_key.ui2_ts_id,
                      pt_ch_obj->t_key.ui2_svc_id,
                      pt_bucket->pui2_ch_event_id[ 0 ].ui2_event_id,
                      pt_ch_obj->pt_present_event
                     ));

            PRINT_TIME("start time:",pt_ch_obj->pt_present_event->t_start_time);
            pt_bucket->ui2_checksum += _edb_dvb_get_single_event_check_sum(pt_ch_obj->pt_present_event,pt_ch_obj) ;//pt_ch_obj->pt_present_event->ui2_event_id;
#endif
       }
        else
        {
            DBG_INFO(("onid[0x%04x] tsid[0x%04x] svcid[0x%04x] present event start time error (0x%08x)\r\n ",
                      pt_ch_obj->t_key.ui2_on_id,
                      pt_ch_obj->t_key.ui2_ts_id,
                      pt_ch_obj->t_key.ui2_svc_id,
                      pt_ch_obj->pt_present_event
                     ));
            PRINT_TIME("start time:",pt_ch_obj->pt_present_event->t_start_time);
            PRINT_TIME("start time + duration:",pt_ch_obj->pt_present_event->t_start_time + pt_ch_obj->pt_present_event->t_duration);
            PRINT_TIME("brdcst:",t_start_time);
        }

    }
    else
    {
        DBG_INFO(("onid[0x%04x] tsid[0x%04x] svcid[0x%04x] present event is null (0x%08x)\r\n",
                  pt_ch_obj->t_key.ui2_on_id,
                  pt_ch_obj->t_key.ui2_ts_id,
                  pt_ch_obj->t_key.ui2_svc_id,
                  pt_ch_obj->pt_present_event
                 ));
    }

    if (pt_ch_obj->pt_following_event != NULL)
    {
        if (
                (!b_check_time) || (t_dt_cond != DT_SYNC_RUNNING) ||
                (pt_ch_obj->pt_following_event->t_start_time >= t_start_time)
           )
        {    
#if 0
            pt_bucket->pui2_ch_event_id[ *(pt_bucket->pui2_num_events) ].ui2_event_id = 
                pt_ch_obj->pt_following_event->ui2_event_id;
            *(pt_bucket->pui2_num_events) =(UINT16) ( *(pt_bucket->pui2_num_events) + 1 );
#else
            pt_bucket->pui2_ch_event_id[ 1 ].ui2_event_id = pt_ch_obj->pt_following_event->ui2_event_id;
            *(pt_bucket->pui2_num_events) =(UINT16) 2;

            DBG_INFO(("onid[0x%04x] tsid[0x%04x] svcid[0x%04x] Follow event,0x%04x (0x%08x) time:\r\n",
                      pt_ch_obj->t_key.ui2_on_id,
                      pt_ch_obj->t_key.ui2_ts_id,
                      pt_ch_obj->t_key.ui2_svc_id,
                      pt_bucket->pui2_ch_event_id[ 1 ].ui2_event_id,
                      pt_ch_obj->pt_following_event
                     ));
            PRINT_TIME("start time:",pt_ch_obj->pt_following_event->t_start_time);
            pt_bucket->ui2_checksum += _edb_dvb_get_single_event_check_sum(pt_ch_obj->pt_following_event,pt_ch_obj) ;//pt_ch_obj->pt_following_event->ui2_event_id;
#endif
        }
        else
        {
            DBG_INFO(("onid[0x%04x] tsid[0x%04x] svcid[0x%04x] Follow event start time error (0x%08x)\r\n ",
                      pt_ch_obj->t_key.ui2_on_id,
                      pt_ch_obj->t_key.ui2_ts_id,
                      pt_ch_obj->t_key.ui2_svc_id,
                      pt_ch_obj->pt_following_event
                     ));
            PRINT_TIME("start time:",pt_ch_obj->pt_following_event->t_start_time);
            PRINT_TIME("start time + duration:",pt_ch_obj->pt_following_event->t_start_time + pt_ch_obj->pt_following_event->t_duration);
            PRINT_TIME("brdcst:",t_start_time);
        }
    }
    else
    {
        DBG_INFO(("onid[0x%04x] tsid[0x%04x] svcid[0x%04x] Follow event is null (0x%08x)\r\n",
                  pt_ch_obj->t_key.ui2_on_id,
                  pt_ch_obj->t_key.ui2_ts_id,
                  pt_ch_obj->t_key.ui2_svc_id,
                  pt_ch_obj->pt_following_event
                 ));

    }


    DBG_INFO((_EDB_INFO"_edb_dvb_process_pf_num_by_utc pt_bucket->ui2_checksum=%d\r\n",
              pt_bucket->ui2_checksum));



    i4_ret = _edb_dvb_start_ae_timer(h_edb_eng,pt_ch_obj,TRUE,TRUE);
    return i4_ret;
}





static INT32 _edb_dvb_process_rating_len(
        HANDLE_T            h_edb_eng,
        VOID*               pv_key_info,            /* EDB_CH_EVT_ID_T **          */
        SIZE_T*             pz_event_info_len,     /* SIZE_T*  */ 
        EDB_COND_T*         pe_cond 
        )
{
    INT32                   i4_ret          = EDBR_OK;
    EDB_CH_EVT_ID_T*        pt_ch_evt_id    = NULL;
    EDB_EVENT_POOL_CH_T*    pt_channel      = NULL;
    HANDLE_T                h_event_hash    = NULL_HANDLE;
    EDB_POOL_EVENT_INFO_T*  pt_pool_event   = NULL;
    UINT16                  ui2_event_id    = 0 ;

    EDB_CHECK_RET( 
            (h_edb_eng == NULL_HANDLE ||  pv_key_info == NULL || pz_event_info_len == NULL),
            (EDBR_INV_ARG),
            (("Wrong argument\r\n"))
            );

    pt_ch_evt_id = (EDB_CH_EVT_ID_T*)pv_key_info;
    i4_ret = _edb_dvb_util_get_channel_by_edb_eng(h_edb_eng,pt_ch_evt_id->ui4_channel_id, &pt_channel);
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK || pt_channel == NULL),
            (EDBR_INTERNAL_ERROR),
            (("Get pool channel fail\r\n"))
            );

    h_event_hash = pt_channel->h_event_hash;
    EDB_CHECK_RET( 
            (h_event_hash == NULL_HANDLE),
            (EDBR_INTERNAL_ERROR),
            (("Event hash is null\r\n"))
            );

    if (pt_ch_evt_id->b_auto_event_id == TRUE)
    {
        if (pt_channel->pt_present_event == NULL)
        {
            DBG_INFO((_EDB_INFO"Present event null\r\n")); 
            *pz_event_info_len = sizeof(EDB_RATING_LIST_T);
            return EDBR_OK;
        }
        else
        {
            ui2_event_id = pt_channel->pt_present_event->ui2_event_id;
        }
    }
    else
    {
        ui2_event_id = pt_ch_evt_id->ui2_event_id;
    }

   
    DBG_INFO((_EDB_INFO"_edb_dvb_process_raing_len event id=0x%04x\r\n",ui2_event_id)); 

    i4_ret = edb_pool_get_event_obj(pt_channel, ui2_event_id, &pt_pool_event);
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK || pt_pool_event == NULL),
            (EDBR_INTERNAL_ERROR),
            (("Can not found event:%d\r\n",pt_ch_evt_id->ui2_event_id))
            );


    if (pt_pool_event->pt_rating_list == NULL)
    {
        DBG_INFO((_EDB_INFO"pt_rating_list is NULL event id=0x%04x\r\n",ui2_event_id)); 
        *pz_event_info_len = sizeof(EDB_RATING_LIST_T);
        return EDBR_OK;
    }

    if (pt_pool_event->pt_rating_list->ui1_num == (UINT8)0x0)
    {
        DBG_INFO((_EDB_INFO"pt_rating_list->ui1_num = 0x0 event id=0x%04x\r\n",ui2_event_id)); 
        *pz_event_info_len = sizeof(EDB_RATING_LIST_T);
        return EDBR_OK;
    }


    if (pt_pool_event->pt_rating_list->ui1_num > (UINT8)(0x0))
    {
        DBG_INFO((_EDB_INFO"rating len=%d avent id=0x%04x\r\n",*pz_event_info_len,ui2_event_id)); 
        *pz_event_info_len = sizeof(EDB_RATING_LIST_T);
        return EDBR_OK;
    }

    return i4_ret;
}

static INT32 _edb_dvb_process_rating(
        HANDLE_T            h_edb_eng,
        VOID*               pv_key_info,            /* EDB_CH_EVT_ID_T **          */
        SIZE_T*             pz_event_info_len,     /* SIZE_T*  */ 
        VOID*               pv_event_info,          /* EDB_RATING_LIST_T**        */        
        EDB_COND_T*         pe_cond 
        )
{
    INT32                   i4_ret          = EDBR_OK;
    EDB_CH_EVT_ID_T*        pt_ch_evt_id    = NULL;
    EDB_EVENT_POOL_CH_T*    pt_channel      = NULL;
    HANDLE_T                h_event_hash    = NULL_HANDLE;
    EDB_POOL_EVENT_INFO_T*  pt_pool_event   = NULL;
    EDB_RATING_LIST_T*      pt_rating_list  = NULL;
    DVB_ENG_DATA*           pt_eng_data     = NULL;
    ISO_3166_COUNT_T        t_country       = {0};
    EDB_EVENT_RATING_T*     pt_event_rating = NULL;
    UINT16                  ui2_idx         = 0;
    BOOL                    fg_rating_match = FALSE;
    UINT16                  ui2_max_rating  = 0;
    UINT16                  ui2_event_id    = 0 ;
    ISO_3166_COUNT_T        t_this_country_code_for_max_rating = {0};


    EDB_CHECK_RET( 
            (h_edb_eng == NULL_HANDLE ||  pv_key_info == NULL || pz_event_info_len == NULL || pv_event_info == NULL),
            (EDBR_INV_ARG),
            (("Wrong argument\r\n"))
            );



    pt_ch_evt_id = (EDB_CH_EVT_ID_T*)pv_key_info;
    i4_ret = _edb_dvb_util_get_channel_by_edb_eng(h_edb_eng,pt_ch_evt_id->ui4_channel_id, &pt_channel);
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK || pt_channel == NULL),
            (EDBR_INTERNAL_ERROR),
            (("Get pool channel fail\r\n"))
            );


    h_event_hash = pt_channel->h_event_hash;
    EDB_CHECK_RET( 
            (h_event_hash == NULL_HANDLE),
            (EDBR_INTERNAL_ERROR),
            (("Event hash is null\r\n"))
            );

    pt_rating_list = (EDB_RATING_LIST_T*)pv_event_info;

    if (pt_ch_evt_id->b_auto_event_id == TRUE)
    {
        //(No EIT_present_following_flag) && (No current event) && (Rating != None) && (broadcast type==DVB-C) && (Operator==Ziggo and Other) Sevice shall be blocked
        //When user set the "rating==None", system shall not block service on "No EIT_present_following_flag and No current event" case.
        if (
                pt_channel->pt_present_event == NULL && pt_channel != NULL &&/*|| pt_channel->fg_present_event_from_pf == FALSE*/
                ((BOOL)((UINT32)(pt_channel->pv_special_data)) == FALSE)
           )
        {
            pt_rating_list->ui2_rating_region           = 0 /* for DVB countries */;
            pt_rating_list->ui2_rated_dimensions        =(UINT16) 1;
            pt_rating_list->t_rating.ui2_dimension      = 0;
            pt_rating_list->t_rating.ui2_rating_value   = 0xFF;
            pt_rating_list->ps_country_code             = (CHAR*) (pt_rating_list->t_country_code);
            pt_rating_list->pt_ratings                  = &(pt_rating_list->t_rating);
            pt_rating_list->pt_next                     = NULL;
            pt_rating_list->t_rating_type               = EDB_RATING_NO_EIT;
            i4_ret = EDBR_OK;
            *pz_event_info_len = sizeof(EDB_RATING_LIST_T);
            DBG_INFO((_EDB_INFO"No eit\r\n" ));
            return i4_ret; 
        }

        //Has EIT-PF but no parent rating desctiptor
        if (
                (
                        pt_channel->pt_present_event != NULL && 
                        pt_channel->fg_present_event_from_pf == TRUE &&
                        pt_channel->pt_present_event->pt_rating_list == NULL
                )
                ||
                (
                        pt_channel->pt_present_event != NULL &&
                        pt_channel->fg_present_event_from_pf == TRUE &&
                        pt_channel->pt_present_event->pt_rating_list != NULL &&
                        pt_channel->pt_present_event->pt_rating_list->ui1_num == (UINT8)0x0
                )
           )
        {
            pt_rating_list->ui2_rating_region           = 0 /* for DVB countries */;
            pt_rating_list->ui2_rated_dimensions        =(UINT16) 1;
            pt_rating_list->t_rating.ui2_dimension      = 0;
            pt_rating_list->t_rating.ui2_rating_value   = 0xFF;
            pt_rating_list->ps_country_code             = (CHAR*) (pt_rating_list->t_country_code);
            pt_rating_list->pt_ratings                  = &(pt_rating_list->t_rating);
            pt_rating_list->pt_next                     = NULL;
            pt_rating_list->t_rating_type               = EDB_RATING_NO_DESC;
            i4_ret = EDBR_OK;
            *pz_event_info_len = sizeof(EDB_RATING_LIST_T);
            DBG_INFO((_EDB_INFO"No parent rating descriptor\r\n" ));
            return i4_ret; 
        }
    }

    //for svctx use 
    if (pt_ch_evt_id->b_auto_event_id == TRUE && pt_channel->pt_present_event != NULL)
    {
        ui2_event_id = pt_channel->pt_present_event->ui2_event_id;
    }
    else
    {
        ui2_event_id =pt_ch_evt_id->ui2_event_id;//use call event id
    }


    DBG_INFO((_EDB_INFO"_edb_dvb_process_raing event id=0x%04x\r\n",ui2_event_id)); 

    i4_ret = edb_pool_get_event_obj(pt_channel, ui2_event_id, &pt_pool_event);
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK || pt_pool_event == NULL),
            (EDBR_INTERNAL_ERROR),
            (("can not get this event by id \r\n"))
            );

    i4_ret = edb_dvb_get_eng_data_from_ctrl(h_edb_eng,(VOID**)&pt_eng_data);
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK || pt_eng_data == NULL),
            (EDBR_INTERNAL_ERROR),
            (("Can not get engine data\r\n"))
            );
#if 0
    {
        UINT32      ui4_ecd_channel_id = pt_eng_data->t_edb_eng_state.ui4_current_channel_id;
        UINT32      ui4_svctx_channel_id  =pt_ch_evt_id->ui4_channel_id;

        if (ui4_ecd_channel_id != ui4_svctx_channel_id)
        {
            DBG_INFO((_EDB_INFO"Channel id wrong\r\n",ui2_event_id)); 
        }
    }
#endif


    x_strncpy(t_country,pt_eng_data->t_edb_eng_state.t_country,ISO_3166_COUNT_LEN);


    if (pt_pool_event->pt_rating_list == NULL)
    {
        *pz_event_info_len = 0;
        DBG_INFO((_EDB_INFO"rating list is null event id=0x%04x\r\n",ui2_event_id)); 
        return EDBR_INFO_NOT_FOUND;
    }

    if (pt_pool_event->pt_rating_list->ui1_num == (UINT8)0x0)
    {
        *pz_event_info_len = 0;
        DBG_INFO((_EDB_INFO"rating list number is 0 event id=0x%04x\r\n",ui2_event_id)); 
        return EDBR_INFO_NOT_FOUND;
    }


    pt_event_rating = pt_pool_event->pt_rating_list->pt_rating;
    if (pt_event_rating == NULL)
    {
        return EDBR_INFO_NOT_FOUND;
    }



    for (ui2_idx = 0; ui2_idx < pt_pool_event->pt_rating_list->ui1_num; ui2_idx++)
    {
        if (_dvb_country_code_match(&(pt_event_rating[ui2_idx].t_country),&t_country) )
        {
            //match country
            fg_rating_match = TRUE;
            ui2_max_rating = (UINT16) (pt_event_rating[ui2_idx].ui1_rating ) ;

            DBG_INFO((_EDB_INFO"[MATCH] TS country=%s USER country=%s value=%d\r\n",
                      pt_rating_list->ps_country_code,
                      t_country,
                      pt_rating_list->t_rating.ui2_rating_value)); 
            break;
        }

        if ( (UINT16) (pt_event_rating[ui2_idx].ui1_rating) > ui2_max_rating)
        {
            x_memcpy(t_this_country_code_for_max_rating, pt_event_rating[ui2_idx].t_country, 3);
            ui2_max_rating = (UINT16) (pt_event_rating[ui2_idx].ui1_rating);
            DBG_INFO((_EDB_INFO"[MATCH] TS country=%s USER country=%s max value=%d\r\n",
                      pt_rating_list->ps_country_code,
                      t_country,
                      ui2_max_rating)); 

        }
    }



    if (fg_rating_match == TRUE)
    {
        pt_rating_list->ui2_rating_region           = 0 /* for DVB countries */;
        pt_rating_list->ui2_rated_dimensions        =(UINT16) 1;
        pt_rating_list->t_rating.ui2_dimension      = 0;
        pt_rating_list->t_rating.ui2_rating_value   = pt_event_rating[ui2_idx].ui1_rating;
        pt_rating_list->ps_country_code             = (CHAR*) (pt_rating_list->t_country_code);
        pt_rating_list->pt_ratings                  = &(pt_rating_list->t_rating);
        pt_rating_list->pt_next                     = NULL;
        x_memcpy(pt_rating_list->ps_country_code, pt_event_rating[ui2_idx].t_country, 3);//from ts
        pt_rating_list->ps_country_code[3]          = '\x0';
        x_edb_eng_unify_country_code(&(pt_rating_list->t_country_code));
        pt_rating_list->t_rating_type               = EDB_RATING_NORNAL;

        i4_ret = EDBR_OK;

        DBG_INFO((_EDB_INFO"[MATCH] TS country=%s USER country=%s  value=%d\r\n",
                  pt_rating_list->ps_country_code,
                  t_country,
                  pt_rating_list->t_rating.ui2_rating_value)); 
    }
    else/*not match*/
    {
        if (TRUE/*ui2_max_rating >= 0*/)
        {
            if ((x_strncmp(t_country,"ITA",3) == 0) ||
                (pt_eng_data->t_edb_eng_state.t_operator == DVBC_OPERATOR_NAME_UPC))
            {
                *pz_event_info_len = 0;
                i4_ret = EDBR_INFO_NOT_FOUND;
                //x_dbg_stmt("Country is Italy,rating NULL\r\n");
                DBG_INFO((_EDB_INFO"[DISMATCH] TS country=%s USER country=%s Country is Italy,rating NULL \r\n",
                          pt_rating_list->ps_country_code, t_country)); 

            }
            else
            {

                pt_rating_list->ui2_rating_region           = 0 /* for DVB countries */;
                pt_rating_list->ui2_rated_dimensions        = 1;
                pt_rating_list->t_rating.ui2_dimension      = 0;
                pt_rating_list->t_rating.ui2_rating_value   = ui2_max_rating;
                pt_rating_list->ps_country_code             = (CHAR*) (pt_rating_list->t_country_code);
                pt_rating_list->pt_ratings = &(pt_rating_list->t_rating);
                pt_rating_list->pt_next                     = NULL;
                x_memcpy(pt_rating_list->ps_country_code, t_this_country_code_for_max_rating, 3);
                pt_rating_list->ps_country_code[3]          = '\x0';
                x_edb_eng_unify_country_code(&(pt_rating_list->t_country_code));
                pt_rating_list->t_rating_type               = EDB_RATING_NORNAL;

                if ((x_strncmp(t_this_country_code_for_max_rating,"ESP",3) == 0))
                {
                    if (ui2_max_rating == (UINT16)0x1F)
                    {
                        ui2_max_rating = 15;
                        pt_rating_list->t_rating.ui2_rating_value = ui2_max_rating;
                        DBG_INFO((_EDB_INFO"[DISMATCH] TS country=%s USER country=%s Country is ESP\r\n",
                                  pt_rating_list->ps_country_code, t_country)); 

                    }
                }

                DBG_INFO((_EDB_INFO"[DISMATCH] TS country=%s USER country=%s  value=%d\r\n",
                          pt_rating_list->ps_country_code,
                          t_country,
                          pt_rating_list->t_rating.ui2_rating_value)); 

                i4_ret = EDBR_OK;
            }
        }
        else
        {
            *pz_event_info_len = 0;
            i4_ret = EDBR_INFO_NOT_FOUND;
        }
    }


    return i4_ret;
}


static BOOL _dvb_country_code_match (
        ISO_3166_COUNT_T*   pt_country_code,
        ISO_3166_COUNT_T*   ps_default_country)
{

    if (ps_default_country          == NULL ||
        pt_country_code             == NULL || 
        *pt_country_code            == NULL ||
        *((CHAR*) *pt_country_code) == '\x0')
    {
        return TRUE;
    }

    if (x_edb_eng_unify_country_code(pt_country_code) == EDBR_OK)
    {
        CHAR* ps_country_code = (CHAR *) (*pt_country_code);

        if (!x_strncmp((*ps_default_country), ps_country_code, 3))
        {
            return TRUE;
        }
    }

    return FALSE;
}







static INT32 _edb_dvb_process_present_event(
        HANDLE_T            h_edb_eng,
        VOID*               pv_key_info,            /* EDB_CH_EVT_ID_T*          */
        SIZE_T*             pz_event_info_len,     /* sizeof(EDB_EVENT_INFO_T)  */ 
        VOID*               pv_event_info,          /* EDB_EVENT_INFO_T**        */
        EDB_COND_T*         pe_cond 
        )
{
    INT32                   i4_ret       = EDBR_OK;
    EDB_CH_EVT_ID_T*        pt_ch_evt_id = NULL;
    EDB_EVENT_POOL_CH_T*    pt_channel   = NULL;
    EDB_EVENT_INFO_T*       pt_event     = NULL;

    EDB_CHECK_RET( 
            (h_edb_eng == NULL_HANDLE ||  pv_key_info == NULL || pz_event_info_len == NULL || pv_event_info == NULL),
            (EDBR_INV_ARG),
            (("Wrong argument\r\n"))
            );

    pt_ch_evt_id = (EDB_CH_EVT_ID_T*)pv_key_info;
    i4_ret = _edb_dvb_util_get_channel_by_edb_eng(h_edb_eng,pt_ch_evt_id->ui4_channel_id, &pt_channel);
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK || pt_channel == NULL),
            (EDBR_INTERNAL_ERROR),
            (("Get pool channel fail\r\n"))
            );

    pt_event =(EDB_EVENT_INFO_T*) pv_event_info;

    if (pt_channel->pt_present_event != NULL)
    {
        //Copy event from pool to EDB_EVENT_INFO_T
        i4_ret = _edb_dvb_copy_single_event(h_edb_eng,pt_channel,pt_channel->pt_present_event,pt_event);
    }

    return i4_ret;
}

static INT32 _edb_dvb_process_following_event(
        HANDLE_T            h_edb_eng,
        VOID*               pv_key_info,            /* EDB_CH_EVT_ID_T*          */
        SIZE_T*             pz_event_info_len,     /* sizeof(EDB_EVENT_INFO_T)  */ 
        VOID*               pv_event_info,          /* EDB_EVENT_INFO_T**        */
        EDB_COND_T*         pe_cond 
        )
{
    INT32                   i4_ret       = EDBR_OK;
    EDB_CH_EVT_ID_T*        pt_ch_evt_id = NULL;
    EDB_EVENT_POOL_CH_T*    pt_channel   = NULL;
    EDB_EVENT_INFO_T*       pt_event     = NULL;


    EDB_CHECK_RET( 
            (h_edb_eng == NULL_HANDLE ||  pv_key_info == NULL || pz_event_info_len == NULL || pv_event_info == NULL),
            (EDBR_INV_ARG),
            (("Wrong argument\r\n"))
            );

    pt_ch_evt_id = (EDB_CH_EVT_ID_T*)pv_key_info;
    i4_ret = _edb_dvb_util_get_channel_by_edb_eng(h_edb_eng,pt_ch_evt_id->ui4_channel_id, &pt_channel);
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK || pt_channel == NULL),
            (EDBR_INTERNAL_ERROR),
            (("Get pool channel fail\r\n"))
            );

    pt_event =(EDB_EVENT_INFO_T*) pv_event_info;

    if (pt_channel->pt_following_event != NULL)
    {
        //Copy event from pool to EDB_EVENT_INFO_T
        i4_ret = _edb_dvb_copy_single_event(h_edb_eng,pt_channel,pt_channel->pt_following_event,pt_event);
    }

    return i4_ret;
}

static INT32 _edb_dvb_process_static_len(
        HANDLE_T            h_edb_eng,
        VOID*               pv_key_info,            /* EDB_CH_EVT_ID_T*          */
        EDB_KEY_TYPE_T      e_key_type,
        SIZE_T*             pz_event_info_len,     /* SIZE_T*  */ 
        EDB_COND_T*         pe_cond 
        )
{
    INT32                   i4_ret          = EDBR_OK;
    EDB_CH_EVT_ID_T*        pt_ch_evt_id    = NULL;
    EDB_EVENT_POOL_CH_T*    pt_channel      = NULL;
    HANDLE_T                h_event_hash    = NULL_HANDLE;
    EDB_POOL_EVENT_INFO_T*  pt_pool_event   = NULL;
    UINT16                  ui2_event_id    = 0 ;

    EDB_CHECK_RET( 
            (h_edb_eng == NULL_HANDLE ||  pv_key_info == NULL || pz_event_info_len == NULL),
            (EDBR_INV_ARG),
            (("Wrong argument\r\n"))
            );

    pt_ch_evt_id = (EDB_CH_EVT_ID_T*)pv_key_info;
    i4_ret = _edb_dvb_util_get_channel_by_edb_eng(h_edb_eng,pt_ch_evt_id->ui4_channel_id, &pt_channel);
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK || pt_channel == NULL),
            (EDBR_INTERNAL_ERROR),
            (("Get pool channel fail\r\n"))
            );

    h_event_hash = pt_channel->h_event_hash;
    EDB_CHECK_RET( 
            (h_event_hash == NULL_HANDLE),
            (EDBR_INTERNAL_ERROR),
            (("Event hash is null\r\n"))
            );

    //for svctx use 
    if (pt_ch_evt_id->b_auto_event_id == TRUE)
    {
        EDB_CHECK_RET( 
                (pt_channel->pt_present_event == NULL),
                (EDBR_INTERNAL_ERROR),
                (("Present Event is null\r\n"))
                );
        ui2_event_id = pt_channel->pt_present_event->ui2_event_id;
    }
    else
    {
        ui2_event_id =pt_ch_evt_id->ui2_event_id;//use call event id
    }


    i4_ret = edb_pool_get_event_obj(pt_channel, ui2_event_id, &pt_pool_event);
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK || pt_pool_event == NULL),
            (EDBR_INTERNAL_ERROR),
            (("Can not found event:%d\r\n",pt_ch_evt_id->ui2_event_id))
            );


    switch (e_key_type)
    {        
        case EDB_KEY_TYPE_START_TIME:
            {
                *pz_event_info_len = sizeof(TIME_T);
                i4_ret = EDBR_OK;
                break;
            }
        case EDB_KEY_TYPE_DURATION:
            {
                *pz_event_info_len = sizeof(TIME_T);
                i4_ret = EDBR_OK;
                break;
            }
        default:
            i4_ret = EDBR_INFO_NOT_FOUND;
            break;
    }  

    return i4_ret;
}


static INT32 _edb_dvb_process_event_title_len(
        HANDLE_T            h_edb_eng,
        VOID*               pv_key_info,            /* EDB_CH_EVT_ID_T*          */
        SIZE_T*             pz_event_info_len,     /* SIZE_T*  */ 
        EDB_COND_T*         pe_cond 
        )
{
    INT32                   i4_ret          = EDBR_OK;
    EDB_CH_EVT_ID_T*        pt_ch_evt_id    = NULL;
    EDB_EVENT_POOL_CH_T*    pt_channel      = NULL;
    HANDLE_T                h_event_hash    = NULL_HANDLE;
    EDB_POOL_EVENT_INFO_T*  pt_pool_event   = NULL;
    UINT16                  ui2_event_id    = 0 ;

    EDB_CHECK_RET( 
            (h_edb_eng == NULL_HANDLE ||  pv_key_info == NULL || pz_event_info_len == NULL),
            (EDBR_INV_ARG),
            (("Wrong argument\r\n"))
            );

    pt_ch_evt_id = (EDB_CH_EVT_ID_T*)pv_key_info;
    i4_ret = _edb_dvb_util_get_channel_by_edb_eng(h_edb_eng,pt_ch_evt_id->ui4_channel_id, &pt_channel);
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK || pt_channel == NULL),
            (EDBR_INTERNAL_ERROR),
            (("Get pool channel fail\r\n"))
            );

    h_event_hash = pt_channel->h_event_hash;
    EDB_CHECK_RET( 
            (h_event_hash == NULL_HANDLE),
            (EDBR_INTERNAL_ERROR),
            (("Event hash is null\r\n"))
            );

    //for svctx use 
    if (pt_ch_evt_id->b_auto_event_id == TRUE)
    {
        EDB_CHECK_RET( 
                (pt_channel->pt_present_event == NULL),
                (EDBR_INFO_NOT_FOUND),
                (("present event is null\r\n"))
                );
        ui2_event_id = pt_channel->pt_present_event->ui2_event_id;
    }
    else
    {
        ui2_event_id =pt_ch_evt_id->ui2_event_id;//use call event id
    }


    i4_ret = edb_pool_get_event_obj(pt_channel, ui2_event_id, &pt_pool_event);
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK || pt_pool_event == NULL),
            (EDBR_INTERNAL_ERROR),
            (("Can not found event:%d\r\n",pt_ch_evt_id->ui2_event_id))
            );

    if (pt_pool_event->ps_event_title != NULL)
    {
        *pz_event_info_len = x_strlen(pt_pool_event->ps_event_title) +1;/*For zero add*/
    }



    return i4_ret;
}


static INT32 _edb_dvb_process_guidance_len(
        HANDLE_T            h_edb_eng,
        VOID*               pv_key_info,            /* EDB_CH_EVT_ID_T*          */
        SIZE_T*             pz_event_info_len,     /* SIZE_T*  */ 
        EDB_COND_T*         pe_cond 
        )
{
    INT32                   i4_ret          = EDBR_OK;
    EDB_CH_EVT_ID_T*        pt_ch_evt_id    = NULL;
    EDB_EVENT_POOL_CH_T*    pt_channel      = NULL;
    HANDLE_T                h_event_hash    = NULL_HANDLE;
    EDB_POOL_EVENT_INFO_T*  pt_pool_event   = NULL;
    UINT16                  ui2_event_id    = 0 ;

    EDB_CHECK_RET( 
            (h_edb_eng == NULL_HANDLE ||  pv_key_info == NULL || pz_event_info_len == NULL),
            (EDBR_INV_ARG),
            (("Wrong argument\r\n"))
            );

    pt_ch_evt_id = (EDB_CH_EVT_ID_T*)pv_key_info;
    i4_ret = _edb_dvb_util_get_channel_by_edb_eng(h_edb_eng,pt_ch_evt_id->ui4_channel_id, &pt_channel);
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK || pt_channel == NULL),
            (EDBR_INTERNAL_ERROR),
            (("Get pool channel fail\r\n"))
            );

    h_event_hash = pt_channel->h_event_hash;
    EDB_CHECK_RET( 
            (h_event_hash == NULL_HANDLE),
            (EDBR_INTERNAL_ERROR),
            (("Event hash is null\r\n"))
            );

    //for svctx use 
    if (pt_ch_evt_id->b_auto_event_id == TRUE)
    {
        EDB_CHECK_RET( 
                (pt_channel->pt_present_event == NULL),
                (EDBR_INFO_NOT_FOUND),
                (("present event is null\r\n"))
                );
        ui2_event_id = pt_channel->pt_present_event->ui2_event_id;
    }
    else
    {
        ui2_event_id =pt_ch_evt_id->ui2_event_id;//use call event id
    }


    i4_ret = edb_pool_get_event_obj(pt_channel, ui2_event_id, &pt_pool_event);
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK || pt_pool_event == NULL),
            (EDBR_INTERNAL_ERROR),
            (("Can not found event:%d\r\n",pt_ch_evt_id->ui2_event_id))
            );

    if (((pt_pool_event->ui4_got_mask) & EDB_EVENT_GOT_GUIDANCE) &&
        (pt_pool_event->ps_event_guidance != NULL)
        )
    {
        *pz_event_info_len = x_strlen(pt_pool_event->ps_event_guidance) +1;/*For zero add*/  
    }
    else if (pt_channel->t_channel_guidance_info.ps_event_guidance != NULL)
    {   
        *pz_event_info_len = x_strlen(pt_channel->t_channel_guidance_info.ps_event_guidance) +1;/*For zero add*/    
    }


    return i4_ret;
}

static INT32 _edb_dvb_process_event_detail_len(
        HANDLE_T            h_edb_eng,
        VOID*               pv_key_info,            /* EDB_CH_EVT_ID_T*          */
        SIZE_T*             pz_event_info_len,     /* SIZE_T*  */ 
        EDB_COND_T*         pe_cond 
        )
{
    INT32                   i4_ret                    = EDBR_OK;
    SIZE_T                  z_detail_len              = 0;
   
    EDB_CHECK_RET( 
            (h_edb_eng == NULL_HANDLE ||  pv_key_info == NULL || pz_event_info_len == NULL),
            (EDBR_INV_ARG),
            (("Wrong argument\r\n"))
            );

    z_detail_len = EDB_EVENT_DETAIL_BUFFER;

    x_memset(s_event_detail_buffer,0x0,EDB_EVENT_DETAIL_BUFFER);
    i4_ret = _edb_dvb_process_event_detail(
        h_edb_eng,
        pv_key_info,
        &z_detail_len,
        (VOID*) (&(s_event_detail_buffer[0])),
        pe_cond
        );
    
    if (i4_ret == EDBR_OK && z_detail_len > 0)
    {
       *pz_event_info_len =z_detail_len + 1;
    }
    
    return i4_ret;
}

static INT32 _edb_dvb_process_linkage_len(
        HANDLE_T            h_edb_eng,
        VOID*               pv_key_info,            /* EDB_CH_EVT_ID_T*          */
        SIZE_T*             pz_event_info_len,     /* SIZE_T*  */ 
        EDB_COND_T*         pe_cond 
        )
{
    INT32                   i4_ret          = EDBR_OK;
    EDB_CH_EVT_ID_T*        pt_ch_evt_id    = NULL;
    EDB_EVENT_POOL_CH_T*    pt_channel      = NULL;
    HANDLE_T                h_event_hash    = NULL_HANDLE;
    EDB_POOL_EVENT_INFO_T*  pt_pool_event   = NULL;
    UINT16                  ui2_event_id    = 0 ;
    UINT8                   i;
    UINT8                   ui1_num_of_linkage = 0;

    EDB_CHECK_RET( 
            (h_edb_eng == NULL_HANDLE ||  pv_key_info == NULL || pz_event_info_len == NULL),
            (EDBR_INV_ARG),
            (("Wrong argument\r\n"))
            );

    pt_ch_evt_id = (EDB_CH_EVT_ID_T*)pv_key_info;
    i4_ret = _edb_dvb_util_get_channel_by_edb_eng(h_edb_eng,pt_ch_evt_id->ui4_channel_id, &pt_channel);
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK || pt_channel == NULL),
            (EDBR_INTERNAL_ERROR),
            (("Get pool channel fail\r\n"))
            );

    h_event_hash = pt_channel->h_event_hash;
    EDB_CHECK_RET( 
            (h_event_hash == NULL_HANDLE),
            (EDBR_INTERNAL_ERROR),
            (("Event hash is null\r\n"))
            );

    //for svctx use 
    if (pt_ch_evt_id->b_auto_event_id == TRUE)
    {
        EDB_CHECK_RET( 
                (pt_channel->pt_present_event == NULL),
                (EDBR_INFO_NOT_FOUND),
                (("present event is null\r\n"))
                );
        ui2_event_id = pt_channel->pt_present_event->ui2_event_id;
    }
    else
    {
        ui2_event_id =pt_ch_evt_id->ui2_event_id;//use call event id
    }


    i4_ret = edb_pool_get_event_obj(pt_channel, ui2_event_id, &pt_pool_event);
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK || pt_pool_event == NULL),
            (EDBR_INTERNAL_ERROR),
            (("Can not found event:%d\r\n",pt_ch_evt_id->ui2_event_id))
            );

    if (pt_pool_event->pt_event_linkage_list != NULL)
    {
        for( i=0 ; i < pt_pool_event->pt_event_linkage_list->ui1_num_event_linkage ; i++ )
        {
            if(pt_pool_event->pt_event_linkage_list->pt_event_linkage[i].ui1_linkage_type==(UINT8)0x0D)
            {
                ui1_num_of_linkage++ ;
            }
        }
        if(ui1_num_of_linkage == 0)
        {
            return EDBR_INFO_NOT_FOUND;
        }
        *pz_event_info_len = sizeof(EDB_EVENT_LINKAGE_INFO_T) * ui1_num_of_linkage ;
    }
    else 
    {
        return EDBR_INFO_NOT_FOUND;
    }
    
    return i4_ret;
}


static INT32 _edb_dvb_process_event_id(
        HANDLE_T            h_edb_eng,
        VOID*               pv_key_info,            /* EDB_CH_EVT_ID_T*          */
        SIZE_T*             pz_event_info_len,     /* SIZE_T*  */ 
        VOID*               pv_event_info,          /* UINT16* */
        EDB_COND_T*         pe_cond 
        )
{
    INT32                   i4_ret          = EDBR_OK;
    EDB_CH_EVT_ID_T*        pt_ch_evt_id    = NULL;
    EDB_EVENT_POOL_CH_T*    pt_channel      = NULL;
    HANDLE_T                h_event_hash    = NULL_HANDLE;
    UINT16                  ui2_event_id    = 0 ;

    EDB_CHECK_RET( 
            (h_edb_eng == NULL_HANDLE ||  pv_key_info == NULL || pz_event_info_len == NULL || pv_event_info == NULL),
            (EDBR_INV_ARG),
            (("Wrong argument\r\n"))
            );

    pt_ch_evt_id = (EDB_CH_EVT_ID_T*)pv_key_info;
    i4_ret = _edb_dvb_util_get_channel_by_edb_eng(h_edb_eng,pt_ch_evt_id->ui4_channel_id, &pt_channel);
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK || pt_channel == NULL),
            (EDBR_INTERNAL_ERROR),
            (("Get pool channel fail\r\n"))
            );

    h_event_hash = pt_channel->h_event_hash;
    EDB_CHECK_RET( 
            (h_event_hash == NULL_HANDLE),
            (EDBR_INTERNAL_ERROR),
            (("Event hash is null\r\n"))
            );


    //for svctx use 
    if (pt_ch_evt_id->b_auto_event_id == TRUE)
    {
        EDB_CHECK_RET( 
                    (pt_channel->pt_present_event == NULL),
                    (EDBR_INFO_NOT_FOUND),
                    (("present event is null\r\n"))
                    );
        ui2_event_id = pt_channel->pt_present_event->ui2_event_id;
    }
    else
    {
        ui2_event_id =pt_ch_evt_id->ui2_event_id;//use call event id
    }



    *((UINT16*)pv_event_info) = ui2_event_id;

    return i4_ret;
}





static INT32 _edb_dvb_process_start_time(
        HANDLE_T            h_edb_eng,
        VOID*               pv_key_info,            /* EDB_CH_EVT_ID_T*          */
        SIZE_T*             pz_event_info_len,     /* SIZE_T*  */ 
        VOID*               pv_event_info,          /* UINT16* */
        EDB_COND_T*         pe_cond 
        )
{
    INT32                   i4_ret          = EDBR_OK;
    EDB_CH_EVT_ID_T*        pt_ch_evt_id    = NULL;
    EDB_EVENT_POOL_CH_T*    pt_channel      = NULL;
    HANDLE_T                h_event_hash    = NULL_HANDLE;
    UINT16                  ui2_event_id    = 0 ;
    EDB_POOL_EVENT_INFO_T*  pt_pool_event   = NULL;

    EDB_CHECK_RET( 
            (h_edb_eng == NULL_HANDLE ||  pv_key_info == NULL || pz_event_info_len == NULL || pv_event_info == NULL),
            (EDBR_INV_ARG),
            (("Wrong argument\r\n"))
            );

    pt_ch_evt_id = (EDB_CH_EVT_ID_T*)pv_key_info;
    i4_ret = _edb_dvb_util_get_channel_by_edb_eng(h_edb_eng,pt_ch_evt_id->ui4_channel_id, &pt_channel);
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK || pt_channel == NULL),
            (EDBR_INTERNAL_ERROR),
            (("Get pool channel fail\r\n"))
            );

    h_event_hash = pt_channel->h_event_hash;
    EDB_CHECK_RET( 
            (h_event_hash == NULL_HANDLE),
            (EDBR_INTERNAL_ERROR),
            (("Event hash is null\r\n"))
            );


    //for svctx use 
    ui2_event_id = pt_ch_evt_id->ui2_event_id;
    if (pt_ch_evt_id->b_auto_event_id == TRUE)
    {
        EDB_CHECK_RET( 
                    (pt_channel->pt_present_event == NULL),
                    (EDBR_INFO_NOT_FOUND),
                    (("present event is null\r\n"))
                    );
        ui2_event_id = pt_channel->pt_present_event->ui2_event_id;
    }
    else
    {
        ui2_event_id =pt_ch_evt_id->ui2_event_id;//use call event id
    }

    i4_ret = edb_pool_get_event_obj(pt_channel, ui2_event_id, &pt_pool_event);
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK || pt_pool_event == NULL),
            (EDBR_INTERNAL_ERROR),
            (("Can not found event by id=0x%04x\r\n",ui2_event_id))
            );

    *((TIME_T*)pv_event_info) = pt_pool_event->t_start_time;

    return i4_ret;
}

static INT32 _edb_dvb_process_duration(
        HANDLE_T            h_edb_eng,
        VOID*               pv_key_info,            /* EDB_CH_EVT_ID_T*          */
        SIZE_T*             pz_event_info_len,     /* SIZE_T*  */ 
        VOID*               pv_event_info,          /* UINT16* */
        EDB_COND_T*         pe_cond 
        )
{
    INT32                   i4_ret          = EDBR_OK;
    EDB_CH_EVT_ID_T*        pt_ch_evt_id    = NULL;
    EDB_EVENT_POOL_CH_T*    pt_channel      = NULL;
    HANDLE_T                h_event_hash    = NULL_HANDLE;
    UINT16                  ui2_event_id    = 0 ;
    EDB_POOL_EVENT_INFO_T*  pt_pool_event   = NULL;

    EDB_CHECK_RET( 
            (h_edb_eng == NULL_HANDLE ||  pv_key_info == NULL || pz_event_info_len == NULL || pv_event_info == NULL),
            (EDBR_INV_ARG),
            (("Wrong argument\r\n"))
            );

    pt_ch_evt_id = (EDB_CH_EVT_ID_T*)pv_key_info;
    i4_ret = _edb_dvb_util_get_channel_by_edb_eng(h_edb_eng,pt_ch_evt_id->ui4_channel_id, &pt_channel);
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK || pt_channel == NULL),
            (EDBR_INTERNAL_ERROR),
            (("Get pool channel fail\r\n"))
            );

    h_event_hash = pt_channel->h_event_hash;
    EDB_CHECK_RET( 
            (h_event_hash == NULL_HANDLE),
            (EDBR_INTERNAL_ERROR),
            (("Event hash is null\r\n"))
            );

    if (pt_ch_evt_id->b_auto_event_id == TRUE)
    {
        EDB_CHECK_RET( 
                    (pt_channel->pt_present_event == NULL),
                    (EDBR_INFO_NOT_FOUND),
                    (("present event is null\r\n"))
                    );
        ui2_event_id = pt_channel->pt_present_event->ui2_event_id;
    }
    else
    {
        ui2_event_id = pt_ch_evt_id->ui2_event_id;
    }

    
    i4_ret = edb_pool_get_event_obj(pt_channel, ui2_event_id, &pt_pool_event);
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK || pt_pool_event == NULL),
            (EDBR_INTERNAL_ERROR),
            (("Can not found event by id=0x%04x\r\n",ui2_event_id))
            );

    *((TIME_T*)pv_event_info) = pt_pool_event->t_duration;

    return i4_ret;
}


static INT32 _edb_dvb_process_event_title(
        HANDLE_T            h_edb_eng,
        VOID*               pv_key_info,            /* EDB_CH_EVT_ID_T*          */
        SIZE_T*             pz_event_info_len,     /* SIZE_T*  */ 
        VOID*               pv_event_info,          /* CHAR* */
        EDB_COND_T*         pe_cond 
        )
{
    INT32                   i4_ret          = EDBR_OK;
    EDB_CH_EVT_ID_T*        pt_ch_evt_id    = NULL;
    EDB_EVENT_POOL_CH_T*    pt_channel      = NULL;
    HANDLE_T                h_event_hash    = NULL_HANDLE;
    EDB_POOL_EVENT_INFO_T*  pt_pool_event   = NULL;
    SIZE_T                  z_len           = 0;
    CHAR*                   ps_event_title  = NULL;
    UINT16                  ui2_event_id    = 0 ;

    EDB_CHECK_RET( 
            (h_edb_eng == NULL_HANDLE ||  pv_key_info == NULL || pz_event_info_len == NULL || pv_event_info == NULL),
            (EDBR_INV_ARG),
            (("Wrong argument\r\n"))
            );

    pt_ch_evt_id = (EDB_CH_EVT_ID_T*)pv_key_info;
    i4_ret = _edb_dvb_util_get_channel_by_edb_eng(h_edb_eng,pt_ch_evt_id->ui4_channel_id, &pt_channel);
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK || pt_channel == NULL),
            (EDBR_INTERNAL_ERROR),
            (("Get pool channel fail\r\n"))
            );

    h_event_hash = pt_channel->h_event_hash;
    EDB_CHECK_RET( 
            (h_event_hash == NULL_HANDLE),
            (EDBR_INTERNAL_ERROR),
            (("Event hash is null\r\n"))
            );


    //for svctx use 
    if (pt_ch_evt_id->b_auto_event_id == TRUE)
    {
        EDB_CHECK_RET( 
                    (pt_channel->pt_present_event == NULL),
                    (EDBR_INFO_NOT_FOUND),
                    (("present event is null\r\n"))
                    );
        ui2_event_id = pt_channel->pt_present_event->ui2_event_id;
    }
    else
    {
        ui2_event_id = pt_ch_evt_id->ui2_event_id;
    }

   
    i4_ret = edb_pool_get_event_obj(pt_channel, ui2_event_id, &pt_pool_event);
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK || pt_pool_event == NULL),
            (EDBR_INTERNAL_ERROR),
            (("Can not found event by id=0x%04x\r\n",ui2_event_id))
            );

    //first memset 0
    x_memset(pv_event_info,0x0,*pz_event_info_len);
    ps_event_title = (CHAR*)pv_event_info;

    if (pt_pool_event->ps_event_title != NULL)
    {   
        x_strcat(ps_event_title,pt_pool_event->ps_event_title);
        z_len =  x_strlen(ps_event_title);
        if (z_len > *pz_event_info_len )
        {
            DBG_ABORT( DBG_MOD_EDB );
        }
        DBG_INFO((_EDB_INFO"Event title:%s\r\n",ps_event_title));
    }

    return i4_ret;
}


static INT32 _edb_dvb_process_guidance(
        HANDLE_T            h_edb_eng,
        VOID*               pv_key_info,            /* EDB_CH_EVT_ID_T*          */
        SIZE_T*             pz_event_info_len,     /* SIZE_T*  */ 
        VOID*               pv_event_info,          /* CHAR* */
        EDB_COND_T*         pe_cond 
        )
{
    INT32                   i4_ret          = EDBR_OK;
    EDB_CH_EVT_ID_T*        pt_ch_evt_id    = NULL;
    EDB_EVENT_POOL_CH_T*    pt_channel      = NULL;
    HANDLE_T                h_event_hash    = NULL_HANDLE;
    EDB_POOL_EVENT_INFO_T*  pt_pool_event   = NULL;
    CHAR*                   ps_guidance_text= NULL;
    UINT16                  ui2_event_id    = 0 ;

    EDB_CHECK_RET( 
            (h_edb_eng == NULL_HANDLE ||  pv_key_info == NULL || pz_event_info_len == NULL || pv_event_info == NULL),
            (EDBR_INV_ARG),
            (("Wrong argument\r\n"))
            );

    pt_ch_evt_id = (EDB_CH_EVT_ID_T*)pv_key_info;
    i4_ret = _edb_dvb_util_get_channel_by_edb_eng(h_edb_eng,pt_ch_evt_id->ui4_channel_id, &pt_channel);
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK || pt_channel == NULL),
            (EDBR_INTERNAL_ERROR),
            (("Get pool channel fail\r\n"))
            );

    h_event_hash = pt_channel->h_event_hash;
    EDB_CHECK_RET( 
            (h_event_hash == NULL_HANDLE),
            (EDBR_INTERNAL_ERROR),
            (("Event hash is null\r\n"))
            );


    //for svctx use 
    if (pt_ch_evt_id->b_auto_event_id == TRUE)
    {
        EDB_CHECK_RET( 
                    (pt_channel->pt_present_event == NULL),
                    (EDBR_INFO_NOT_FOUND),
                    (("present event is null\r\n"))
                    );
        ui2_event_id = pt_channel->pt_present_event->ui2_event_id;
    }
    else
    {
        ui2_event_id = pt_ch_evt_id->ui2_event_id;
    }

   
    i4_ret = edb_pool_get_event_obj(pt_channel, ui2_event_id, &pt_pool_event);
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK || pt_pool_event == NULL),
            (EDBR_INTERNAL_ERROR),
            (("Can not found event by id=0x%04x\r\n",ui2_event_id))
            );

    //first memset 0
    x_memset(pv_event_info,0x0,*pz_event_info_len);
    ps_guidance_text = (CHAR*)pv_event_info;

    if (((pt_pool_event->ui4_got_mask) & EDB_EVENT_GOT_GUIDANCE) &&
        (pt_pool_event->ps_event_guidance != NULL)
        )
    {   
        if( x_strlen(pt_pool_event->ps_event_guidance) > *pz_event_info_len )
        {
            return EDBR_OUT_OF_MEM ;
        }
    
        x_strcat(ps_guidance_text,pt_pool_event->ps_event_guidance);
        
        *pz_event_info_len =  x_strlen(ps_guidance_text);
        
        DBG_INFO((_EDB_INFO"Event Guidance:%s\r\n",ps_guidance_text));
    }
    else if (pt_channel->t_channel_guidance_info.ps_event_guidance != NULL)
    {        
        if( x_strlen(pt_channel->t_channel_guidance_info.ps_event_guidance) > *pz_event_info_len )
        {
            return EDBR_OUT_OF_MEM ;
        }
    
        x_strcat(ps_guidance_text,pt_channel->t_channel_guidance_info.ps_event_guidance);
        
        *pz_event_info_len =  x_strlen(ps_guidance_text);
        
        DBG_INFO((_EDB_INFO"Event Guidance:%s\r\n",ps_guidance_text));
    }

    return i4_ret;
}


static INT32 _edb_dvb_process_event_detail(
        HANDLE_T            h_edb_eng,
        VOID*               pv_key_info,            /* EDB_CH_EVT_ID_T*          */
        SIZE_T*             pz_event_info_len,     /* SIZE_T*  */ 
        VOID*               pv_event_info,          /* CHAR* */
        EDB_COND_T*         pe_cond 
        )
{
    INT32                           i4_ret                          = EDBR_OK;
    EDB_CH_EVT_ID_T*                pt_ch_evt_id                    = NULL;
    EDB_EVENT_POOL_CH_T*            pt_channel                      = NULL;
    HANDLE_T                        h_event_hash                    = NULL_HANDLE;
    EDB_POOL_EVENT_INFO_T*          pt_pool_event                   = NULL;
#if 0    
    EDB_EVENT_EXTENDED_LIST_T*      pt_event_extended_list          = NULL;
    DVB_ENG_DATA*                   pt_eng_data                     = NULL;
    BOOL                            fg_has_short_event_detail       = FALSE;
#endif    
    SIZE_T                          z_length                           = 0;
    CHAR*                           ps_event_detail                 = NULL;
    UINT16                          ui2_event_id                    = 0x0;
    UINT16                            ui2_i                            = 0;

    EDB_CHECK_RET( 
            (h_edb_eng == NULL_HANDLE ||  pv_key_info == NULL || pz_event_info_len == NULL || pv_event_info == NULL),
            (EDBR_INV_ARG),
            (("Wrong argument\r\n"))
            );

    pt_ch_evt_id = (EDB_CH_EVT_ID_T*)pv_key_info;
    i4_ret = _edb_dvb_util_get_channel_by_edb_eng(h_edb_eng,pt_ch_evt_id->ui4_channel_id, &pt_channel);
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK || pt_channel == NULL),
            (EDBR_INTERNAL_ERROR),
            (("Get pool channel fail\r\n"))
            );

    h_event_hash = pt_channel->h_event_hash;
    EDB_CHECK_RET( 
            (h_event_hash == NULL_HANDLE),
            (EDBR_INTERNAL_ERROR),
            (("Event hash is null\r\n"))
            );

    if (pt_ch_evt_id->b_auto_event_id == TRUE)
    {
        EDB_CHECK_RET( 
                    (pt_channel->pt_present_event == NULL),
                    (EDBR_INFO_NOT_FOUND),
                    (("present event is null\r\n"))
                    );
        ui2_event_id = pt_channel->pt_present_event->ui2_event_id;
    }
    else
    { 
        ui2_event_id = pt_ch_evt_id->ui2_event_id;
    }
    

    i4_ret = edb_pool_get_event_obj(pt_channel, ui2_event_id, &pt_pool_event);
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK || pt_pool_event == NULL),
            (EDBR_INTERNAL_ERROR),
            (("Event hash is null\r\n"))
            );

    //first memset 0
    x_memset(pv_event_info,0x0,*pz_event_info_len);
    ps_event_detail = (CHAR*)pv_event_info;

    if (pt_pool_event->ps_event_detail != NULL)
    {   
                x_strcat(ps_event_detail,pt_pool_event->ps_event_detail);
        z_length =  x_strlen(ps_event_detail);
        *pz_event_info_len = z_length;
        }


    /*Compress more than 5 (>=5) SPACE to one SPACE*/
    if (t_eng_cfg_flag & EDB_DVB_CONFIG_COMPRESS_SPACE)
    {
        if (ps_event_detail != NULL)
        {
            CHAR*  pui1_source = ps_event_detail;
            CHAR*  pui1_source_token = NULL;
            SIZE_T  z_len = 0;
            do{
                if (x_strstr(ps_event_detail,"     ") == NULL)
                {
                    break;
                }
                x_memset(aui1_detail_with_space,0x0,EDB_TRANCATE_SPACE_BUFFER);
                while ( (pui1_source_token =  x_strstr(pui1_source,"     ")) != NULL )
                {
                    x_strncat((CHAR*)aui1_detail_with_space,pui1_source,(pui1_source_token - pui1_source));
                    pui1_source +=(pui1_source_token - pui1_source);
                    aui1_detail_with_space[EDB_TRANCATE_SPACE_BUFFER -1] = '\x0';
                    z_len =x_strlen((CHAR*)aui1_detail_with_space);
                    if (z_len < EDB_TRANCATE_SPACE_BUFFER -3)
                    {
                        aui1_detail_with_space[z_len] = ' ';
                    }
                    pui1_source = skip_spaces(pui1_source);
                }
                if (pui1_source != NULL)
                {
                    aui1_detail_with_space[EDB_TRANCATE_SPACE_BUFFER -1] = '\x0';
                    if(x_strlen((CHAR*)aui1_detail_with_space) + x_strlen(pui1_source) < EDB_TRANCATE_SPACE_BUFFER)
                    {
                        x_strcat((CHAR*)aui1_detail_with_space,pui1_source);
                        aui1_detail_with_space[EDB_TRANCATE_SPACE_BUFFER -1] = '\x0';
                    }
                }
                aui1_detail_with_space[EDB_TRANCATE_SPACE_BUFFER -1] = '\x0';
                if (x_strlen((CHAR*)aui1_detail_with_space) < z_length)
                {
                    x_memset(ps_event_detail,0x0,z_length);
                    x_memcpy(ps_event_detail,(CHAR*)aui1_detail_with_space,x_strlen((CHAR*)aui1_detail_with_space));
                }
            }while (0);
        }
    }

    if (t_eng_cfg_flag & EDB_DVB_CONFIG_REPLACE_TAB_WITH_SPACE)
    {
        DVB_ENG_DATA*       pt_eng_data = NULL;
        i4_ret = edb_dvb_get_eng_data_from_ctrl(h_edb_eng, (VOID**)&pt_eng_data);
        if((i4_ret == EDBR_OK) && (pt_eng_data != NULL))
        {
            if((x_strncmp(pt_eng_data->t_edb_eng_state.t_country,"GBR",3) != 0) ||
		       (pt_eng_data->ui2_svl_id != EDB_SVL_ID_DVB_T))
            {
                for(ui2_i=0;ui2_i<x_strlen(ps_event_detail);ui2_i++)
                {
                    if(*(ps_event_detail + ui2_i) == (CHAR)0x09)
                    {
                        *(ps_event_detail + ui2_i) = ' ';
                    }
                }
            }
        }
    }
    

    DBG_INFO((_EDB_INFO"Event detail:%s\r\n",ps_event_detail));

    return i4_ret;
}

static INT32 _edb_dvb_process_linkage(
        HANDLE_T            h_edb_eng,
        VOID*               pv_key_info,            /* EDB_CH_EVT_ID_T*          */
        SIZE_T*             pz_event_info_len,     /* SIZE_T*  */ 
        VOID*               pv_event_info,          /* UINT16* */
        EDB_COND_T*         pe_cond 
        )
{
    INT32                   i4_ret          = EDBR_OK;
    EDB_CH_EVT_ID_T*        pt_ch_evt_id    = NULL;
    EDB_EVENT_POOL_CH_T*    pt_channel      = NULL;
    HANDLE_T                h_event_hash    = NULL_HANDLE;
    UINT16                  ui2_event_id    = 0 ;
    EDB_POOL_EVENT_INFO_T*  pt_pool_event   = NULL;
    UINT8                   i=0;
    UINT8                   ui1_num_of_linkage = 0;
    EDB_EVENT_LINKAGE_INFO_T*   pt_edb_linkage = NULL;
    
    EDB_CHECK_RET( 
            (h_edb_eng == NULL_HANDLE ||  pv_key_info == NULL || pz_event_info_len == NULL || pv_event_info == NULL),
            (EDBR_INV_ARG),
            (("Wrong argument\r\n"))
            );

    pt_ch_evt_id = (EDB_CH_EVT_ID_T*)pv_key_info;
    i4_ret = _edb_dvb_util_get_channel_by_edb_eng(h_edb_eng,pt_ch_evt_id->ui4_channel_id, &pt_channel);
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK || pt_channel == NULL),
            (EDBR_INTERNAL_ERROR),
            (("Get pool channel fail\r\n"))
            );

    h_event_hash = pt_channel->h_event_hash;
    EDB_CHECK_RET( 
            (h_event_hash == NULL_HANDLE),
            (EDBR_INTERNAL_ERROR),
            (("Event hash is null\r\n"))
            );


    //for svctx use 
    ui2_event_id = pt_ch_evt_id->ui2_event_id;
    if (pt_ch_evt_id->b_auto_event_id == TRUE)
    {
        EDB_CHECK_RET( 
                    (pt_channel->pt_present_event == NULL),
                    (EDBR_INFO_NOT_FOUND),
                    (("present event is null\r\n"))
                    );
        ui2_event_id = pt_channel->pt_present_event->ui2_event_id;
    }
    else
    {
        ui2_event_id =pt_ch_evt_id->ui2_event_id;//use call event id
    }

    i4_ret = edb_pool_get_event_obj(pt_channel, ui2_event_id, &pt_pool_event);
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK || pt_pool_event == NULL),
            (EDBR_INTERNAL_ERROR),
            (("Can not found event by id=0x%04x\r\n",ui2_event_id))
            );

    x_memset(pv_event_info,0x0,*pz_event_info_len);
    pt_edb_linkage = (EDB_EVENT_LINKAGE_INFO_T*)pv_event_info;
    if (pt_pool_event->pt_event_linkage_list != NULL)
    {   
        for( i=0 ; i < pt_pool_event->pt_event_linkage_list->ui1_num_event_linkage ; i++ )
        {
            if(pt_pool_event->pt_event_linkage_list->pt_event_linkage[i].ui1_linkage_type==(UINT8)0x0D)
            {
                ui1_num_of_linkage++ ;
            }
        }

        if ( sizeof(EDB_EVENT_LINKAGE_INFO_T) * ui1_num_of_linkage > (*pz_event_info_len) )
        {
            return EDBR_INV_ARG;
        }
        ui1_num_of_linkage = 0;
        for( i=0 ; i < pt_pool_event->pt_event_linkage_list->ui1_num_event_linkage ; i++ )
        {
            if(pt_pool_event->pt_event_linkage_list->pt_event_linkage[i].ui1_linkage_type==(UINT8)0x0D)
            {
                pt_edb_linkage[ui1_num_of_linkage].ui2_on_id = pt_pool_event->pt_event_linkage_list->pt_event_linkage[i].ui2_on_id;
                pt_edb_linkage[ui1_num_of_linkage].ui2_ts_id = pt_pool_event->pt_event_linkage_list->pt_event_linkage[i].ui2_ts_id;
                pt_edb_linkage[ui1_num_of_linkage].ui2_svc_id = pt_pool_event->pt_event_linkage_list->pt_event_linkage[i].ui2_svc_id;
                ui1_num_of_linkage++ ;
            }
        }
        if(ui1_num_of_linkage == 0)
        {
            return EDBR_INFO_NOT_FOUND;
        }
        *pz_event_info_len = sizeof(EDB_EVENT_LINKAGE_INFO_T) * ui1_num_of_linkage ;
    }
    else 
    {
        return EDBR_INFO_NOT_FOUND;
    }
    

    return i4_ret;
}


static INT32 _edb_dvb_process_detail_language(
        HANDLE_T            h_edb_eng,
        VOID*               pv_key_info,            /* EDB_CH_EVT_ID_T*          */
        SIZE_T*             pz_event_info_len,     /* SIZE_T*  */ 
        VOID*               pv_event_info,          /* UINT16* */
        EDB_COND_T*         pe_cond 
        )
{
    INT32                   i4_ret          = EDBR_OK;
    EDB_CH_EVT_ID_T*        pt_ch_evt_id    = NULL;
    EDB_EVENT_POOL_CH_T*    pt_channel      = NULL;
    HANDLE_T                h_event_hash    = NULL_HANDLE;
    UINT16                  ui2_event_id    = 0 ;
    EDB_POOL_EVENT_INFO_T*  pt_pool_event   = NULL;

    
    EDB_CHECK_RET( 
            (h_edb_eng == NULL_HANDLE ||  pv_key_info == NULL || pz_event_info_len == NULL || pv_event_info == NULL),
            (EDBR_INV_ARG),
            (("Wrong argument\r\n"))
            );

    pt_ch_evt_id = (EDB_CH_EVT_ID_T*)pv_key_info;
    i4_ret = _edb_dvb_util_get_channel_by_edb_eng(h_edb_eng,pt_ch_evt_id->ui4_channel_id, &pt_channel);
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK || pt_channel == NULL),
            (EDBR_INTERNAL_ERROR),
            (("Get pool channel fail\r\n"))
            );

    h_event_hash = pt_channel->h_event_hash;
    EDB_CHECK_RET( 
            (h_event_hash == NULL_HANDLE),
            (EDBR_INTERNAL_ERROR),
            (("Event hash is null\r\n"))
            );


    //for svctx use 
    ui2_event_id = pt_ch_evt_id->ui2_event_id;
    if (pt_ch_evt_id->b_auto_event_id == TRUE)
    {
        EDB_CHECK_RET( 
                    (pt_channel->pt_present_event == NULL),
                    (EDBR_INFO_NOT_FOUND),
                    (("present event is null\r\n"))
                    );
        ui2_event_id = pt_channel->pt_present_event->ui2_event_id;
    }
    else
    {
        ui2_event_id =pt_ch_evt_id->ui2_event_id;//use call event id
    }

    i4_ret = edb_pool_get_event_obj(pt_channel, ui2_event_id, &pt_pool_event);
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK || pt_pool_event == NULL),
            (EDBR_INTERNAL_ERROR),
            (("Can not found event by id=0x%04x\r\n",ui2_event_id))
            );

    if(*pz_event_info_len < sizeof(ISO_639_LANG_T))
    {
        return EDBR_INV_ARG;
    }
    x_memset(pv_event_info,0x0,*pz_event_info_len);

    x_memcpy(pv_event_info,&(pt_pool_event->t_detail_lang),3);

    *pz_event_info_len = sizeof(ISO_639_LANG_T);

    return i4_ret;

}

static INT32 _edb_dvb_process_root_nfy(EDB_DVB_MSG_T t_msg)
{
    INT32               i4_ret      = 0x0;
    HANDLE_T            h_edb_eng   = NULL_HANDLE;
    DVB_ENG_DATA*       pt_eng_data = NULL;
    EDB_SECTION_MSG_T           t_section_msg               = {0x0};
     //INT32                       i4_retry            = 0x0;
    UINT8                       ui1_priority        = 1;

    h_edb_eng = (HANDLE_T)t_msg.pv_tag;

    EDB_CHECK_RET( 
            (h_edb_eng == NULL_HANDLE),
            (EDBR_INV_ARG),
            ((_EDB_INFO"Receive EDB_DVB_MSG_ROOT_NFY, h_edb_eng is null\r\n\r\n"))
            );

    i4_ret = edb_dvb_get_eng_data_from_ctrl(h_edb_eng,(VOID**)&pt_eng_data);
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK || pt_eng_data == NULL),
            (EDBR_INV_ARG),
            ((_EDB_INFO"Can not get engine data from client\r\n"))
            );
#if 0
    if (pt_eng_data->t_edb_eng_state.fg_enable == TRUE)
    {
        DBG_INFO((_EDB_INFO"EDB disabled ignore this notify\r\n"));
        return i4_ret;
    }
#endif    

    edb_eng_list_lock();
    if ((TM_COND_T)t_msg.ui4_data == TM_COND_SRC_CONN)
    {
        DBG_INFO((_EDB_INFO"TM_COND_SRC_CONN _edb_dvb_load_table\r\n"));
        i4_ret = _edb_dvb_load_table(h_edb_eng);
        if (i4_ret == EDBR_OK)
        {
            pt_eng_data->t_edb_eng_state.fg_enable = TRUE;
        }   
    }
    else if((TM_COND_T)t_msg.ui4_data == TM_COND_SRC_DISC)
    {
        DBG_INFO((_EDB_INFO"TM_COND_SRC_DISC _edb_dvb_free_table\r\n"));
        i4_ret = _edb_dvb_free_table(h_edb_eng);
        if (i4_ret == EDBR_OK)
        {
            pt_eng_data->t_edb_eng_state.fg_enable = FALSE;
        }  

        //Free section in queue
        //i4_ret = edb_section_clean_msg(pt_eng_data);
        t_section_msg.e_msg_type                            = EDB_SECTION_MSG_CLEAN_NFY;
        t_section_msg.t_section_msg_nfy.pv_section_data     = pt_eng_data;

         DVB_MSG_Q_SEND(edb_dvb_section_get_msg_q_hdl(),
                                &t_section_msg,
                                sizeof(EDB_SECTION_MSG_T),
                                ui1_priority );


        /*Send MSG fail ,clear EIT mask
        if (i4_ret != OSR_OK) 
        {
        
        }
        */
    }
    edb_eng_list_unlock();
    return i4_ret;
}


static INT32 _edb_dvb_process_svl_update(EDB_DVB_MSG_T t_msg)
{
    INT32                   i4_ret      = 0x0;
    HANDLE_T                h_edb_eng   = NULL_HANDLE;
    DVB_ENG_DATA*           pt_eng_data = NULL;

    h_edb_eng = (HANDLE_T)t_msg.pv_tag;

    EDB_CHECK_RET( 
            (h_edb_eng == NULL_HANDLE),
            (EDBR_INV_ARG),
            ((_EDB_INFO"Receive EDB_DVB_MSG_SVL_UPDATE_NFY, h_edb_eng is null\r\n\r\n"))
            );

    i4_ret = edb_dvb_get_eng_data_from_ctrl(h_edb_eng,(VOID**)&pt_eng_data);
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK || pt_eng_data == NULL || (& (pt_eng_data->pt_edb->t_edb_event_pool)) == NULL ),
            (EDBR_INV_ARG),
            ((_EDB_INFO"Can not get engine data from client\r\n"))
            );
    edb_eng_list_lock();    
    if ((SVL_COND_T)(t_msg.ui4_data) ==  SVL_UPDATED)
    {
        i4_ret = _edb_dvb_stop_cache(h_edb_eng);
        i4_ret = _edb_dvb_start_cache(h_edb_eng); 
    }
    edb_eng_list_unlock();

    return i4_ret;
}


static INT32 _edb_dvb_process_ae_nfy(EDB_DVB_MSG_T t_msg)
{
    INT32                   i4_ret                              = 0x0;
    HANDLE_T                h_edb_eng                           = NULL_HANDLE;
    DVB_ENG_DATA*           pt_eng_data                         = NULL;
    UINT32                  ui4_current_channel_id              = 0x0;
    EDB_EVENT_POOL_CH_T*    pt_channel              = NULL;
    EDB_EVENT_POOL_CH_KEY*  pt_locate               = NULL;


    if (t_msg.ps_name == NULL)
    {
        DBG_ABORT( DBG_MOD_EDB );
    }

    h_edb_eng = (HANDLE_T)t_msg.pv_tag;

    x_dbg_stmt("_edb_dvb_process_ae_nfy  type=%d(2-active 3-expire)\r\n",t_msg.e_msg_type);

    EDB_CHECK_RET( 
            (h_edb_eng == NULL_HANDLE),
            (EDBR_INV_ARG),
            ((_EDB_INFO"Receive EDB_DVB_MSG_ACTIVE/EXPIRE_NFY, h_edb_eng is null\r\n"))
            );

    edb_eng_list_lock();    

    i4_ret = edb_dvb_get_eng_data_from_ctrl(h_edb_eng,(VOID**)&pt_eng_data);
    if  (i4_ret != EDBR_OK || pt_eng_data == NULL)
    {
        DBG_INFO((_EDB_INFO"Can not get engine data from client\r\n"));
        edb_eng_list_unlock();
        return i4_ret;
    }
    ui4_current_channel_id =  pt_eng_data->t_edb_eng_state.ui4_current_channel_id;
    i4_ret = _edb_dvb_get_ch_locate_obj(pt_eng_data,ui4_current_channel_id, &pt_locate);
    if (pt_locate != NULL)
    {
        i4_ret = edb_pool_get_ch_obj(& (pt_eng_data->pt_edb->t_edb_event_pool), pt_locate, &pt_channel);
        if (pt_channel != NULL)
        {
            x_memset(pt_channel->pv_data,0x0,sizeof(EDB_EVENT_POOL_CH_VER));  
            //check present && following event 
            i4_ret = edb_dvb_check_pf_event(pt_channel,h_edb_eng,EDB_DVB_PF_NFY_DT);
        }
    }


    /*For active/expire notify just notify caller*/
    edb_handle_notify(EDB_REASON_NOW_EVENT_UPDATED, pt_eng_data->ui2_svl_id, pt_eng_data->t_edb_eng_state.ui4_current_channel_id);   
    edb_handle_notify(EDB_REASON_NEX_EVENT_UPDATED, pt_eng_data->ui2_svl_id, pt_eng_data->t_edb_eng_state.ui4_current_channel_id);   

    edb_eng_list_unlock();


    return i4_ret;
}


static INT32 _edb_dvb_check_expire_ch( 
        HANDLE_T          h_hash,
        UINT32            ui4_counter,
        VOID*             pv_obj,
        VOID*             pv_parse_tag )
{
    INT32                           i4_ret = EDBR_OK;
    EDB_EVENT_POOL_CH_T*            pt_ch_obj       = NULL;
    HANDLE_T                        h_event_hash    = NULL_HANDLE;
    UINT32                          ui4_i           = 0;
    P_ELEMENT_OBJ_T                 pt_elm          = NULL;
    EDB_HASH_T*                     pt_hash_obj     = NULL;
    EDB_POOL_EVENT_INFO_T*          pt_pool_event   = NULL;
    TIME_T                          t_current_time  = NULL_TIME;
    DT_COND_T                       t_dt_cond       = DT_NOT_RUNNING;
    HANDLE_TYPE_T                   e_hdl_type;


    t_current_time = x_dt_get_brdcst_utc(NULL,&t_dt_cond);

    pt_ch_obj = (EDB_EVENT_POOL_CH_T*)pv_obj;
    EDB_CHECK_RET( 
            (pt_ch_obj == NULL),
            (EDBR_INTERNAL_ERROR),
            ((_EDB_INFO"Channel object is null\r\n"))
            );


    h_event_hash = pt_ch_obj->h_event_hash;
    if (h_event_hash == NULL_HANDLE)
    {
        return EDBR_INFO_NOT_FOUND; 
    }


    i4_ret = handle_get_type_obj( h_event_hash, &e_hdl_type, (VOID**)&pt_hash_obj );
    if ( i4_ret != HR_OK || e_hdl_type != EDBT_HASH )
    {
        return EDBR_INV_HANDLE;
    }

    for ( ui4_i=0; ui4_i<pt_hash_obj->z_table_size; ++ui4_i )
    {
        for( pt_elm = pt_hash_obj->ppt_table[ui4_i]; pt_elm != NULL; )
        {
            pt_pool_event = (EDB_POOL_EVENT_INFO_T*)pt_elm->pv_obj;
            if (pt_pool_event != NULL)
            {
                UINT16                  ui2_event_id = 0;
                EDB_POOL_EVENT_INFO_T*  pt_event_found = NULL;
                ui2_event_id = pt_pool_event->ui2_event_id;
                edb_pool_get_event_obj(pt_ch_obj,ui2_event_id,&pt_event_found);
                if(pt_event_found == NULL)
                {
                    pt_elm = pt_elm->pt_next;
                    continue;
                }
            }     

            if (    
                    (pt_pool_event != NULL) &&
                    (t_current_time > (pt_pool_event->t_start_time + pt_pool_event->t_duration) )
               )
            {
                pt_elm = pt_elm->pt_next;
                edb_pool_del_event_obj(pt_ch_obj,pt_pool_event);
                continue;
            }
            pt_elm = pt_elm->pt_next;
        }
    }


    return i4_ret;
}

static INT32 _edb_dvb_process_check_expire_nfy(EDB_DVB_MSG_T t_msg)
{
    INT32                   i4_ret                              = 0x0;
    HANDLE_T                h_edb_eng                           = NULL_HANDLE;
    DVB_ENG_DATA*           pt_eng_data                         = NULL;

    if (t_msg.ps_name == NULL)
    {
        DBG_ABORT( DBG_MOD_EDB );
    }

    h_edb_eng = (HANDLE_T)t_msg.pv_tag;

    x_dbg_stmt("_edb_dvb_process_check_expire_nfy\r\n");

    EDB_CHECK_RET( 
            (h_edb_eng == NULL_HANDLE),
            (EDBR_INV_ARG),
            ((_EDB_INFO"Receive EDB_DVB_MSG_CHECK_EXPIRE_NFY, h_edb_eng is null\r\n"))
            );

    edb_eng_list_lock();    

    i4_ret = edb_dvb_get_eng_data_from_ctrl(h_edb_eng,(VOID**)&pt_eng_data);
    if  (i4_ret != EDBR_OK || pt_eng_data == NULL)
    {
        DBG_INFO((_EDB_INFO"Can not get engine data from client\r\n"));
        edb_eng_list_unlock();
        return i4_ret;
    }
    /*Delete old event*/
    if (pt_eng_data->pt_edb != NULL && pt_eng_data->pt_edb->t_edb_event_pool.h_ch_hash != NULL_HANDLE)
    {
        i4_ret =  edb_hash_parse(pt_eng_data->pt_edb->t_edb_event_pool.h_ch_hash, _edb_dvb_check_expire_ch, (VOID*)NULL);
    }
    edb_eng_list_unlock();

    return i4_ret;
}

static INT32 _edb_dvb_process_tuner_change_nfy(EDB_DVB_MSG_T t_msg)
{
    INT32                   i4_ret                              = 0x0;
    HANDLE_T                h_edb_eng                           = NULL_HANDLE;
    CHAR                    s_tuner_name[EDB_MAX_TUNER_NAME]    = {0};
    DVB_ENG_DATA*           pt_eng_data                         = NULL;

    if (t_msg.ps_name == NULL)
    {
        DBG_ABORT( DBG_MOD_EDB );
    }

    h_edb_eng = (HANDLE_T)t_msg.pv_tag;
    x_strncpy(s_tuner_name, t_msg.ps_name, EDB_MAX_TUNER_NAME);

    x_dbg_stmt("_edb_dvb_process_tuner_change_nfy %s\r\n",s_tuner_name);

    EDB_CHECK_RET( 
            (h_edb_eng == NULL_HANDLE),
            (EDBR_INV_ARG),
            ((_EDB_INFO"Receive EDB_DVB_MSG_TUNER_CHANGE, h_edb_eng is null\r\n"))
            );

    edb_eng_list_lock();    

    i4_ret = edb_dvb_get_eng_data_from_ctrl(h_edb_eng,(VOID**)&pt_eng_data);
    if  (i4_ret != EDBR_OK || pt_eng_data == NULL)
    {
        DBG_INFO((_EDB_INFO"Can not get engine data from client\r\n"));edb_eng_list_unlock();return i4_ret;
    }
    x_strncpy(pt_eng_data->ac_tuner_name, s_tuner_name, EDB_MAX_TUNER_NAME);

    if (pt_eng_data->t_edb_eng_state.fg_enable == TRUE)
    {
        i4_ret = _edb_dvb_stop_cache(h_edb_eng);
        i4_ret = _edb_dvb_start_cache(h_edb_eng);
    }

    edb_eng_list_unlock();

    return i4_ret;
}


static INT32 _edb_dvb_process_restart_edb_nfy(EDB_DVB_MSG_T t_msg)
{
    INT32                   i4_ret                  = 0x0;
    HANDLE_T                h_edb_eng               = NULL_HANDLE;

    h_edb_eng = (HANDLE_T)t_msg.pv_tag;

    x_dbg_stmt("_edb_dvb_process_restart_edb_nfy\r\n");

    EDB_CHECK_RET( 
            (h_edb_eng == NULL_HANDLE),
            (EDBR_INV_ARG),
            ((_EDB_INFO"Receive EDB_DVB_MSG_RESTART_EDB, h_edb_eng is null\r\n"))
            );

    edb_eng_list_lock();    

    _edb_dvb_stop_cache(h_edb_eng);
    _edb_dvb_start_cache(h_edb_eng);

    edb_eng_list_unlock();

    return i4_ret;
}

static INT32 _edb_dvb_process_enable_edb_nfy(EDB_DVB_MSG_T t_msg)
{
    INT32                   i4_ret                  = 0x0;
    HANDLE_T                h_edb_eng               = NULL_HANDLE;
    BOOL                    fg_flag;

    fg_flag   = (BOOL)t_msg.ui4_data;
    h_edb_eng = (HANDLE_T)t_msg.pv_tag;

    x_dbg_stmt("_edb_dvb_process_enable_edb_nfy fg_flag=%d\r\n",fg_flag);

    EDB_CHECK_RET( 
            (h_edb_eng == NULL_HANDLE),
            (EDBR_INV_ARG),
            ((_EDB_INFO"Receive EDB_DVB_MSG_ENABLE_EDB, h_edb_eng is null\r\n"))
            );

    edb_eng_list_lock();    
    _edb_dvb_enable_edb(h_edb_eng,fg_flag);
    edb_eng_list_unlock();

    return i4_ret;
}

static INT32 _edb_dvb_process_enable_channel_scan(EDB_DVB_MSG_T t_msg)
{
    INT32                   i4_ret                  = 0x0;
    HANDLE_T                h_edb_eng               = NULL_HANDLE;
    BOOL                    b_scan_flag;
    BOOL                    b_edb_should_start_cache;
    b_scan_flag   = (BOOL)t_msg.ui4_data;
    h_edb_eng = (HANDLE_T)t_msg.pv_tag;

    EDB_CHECK_RET( 
            (h_edb_eng == NULL_HANDLE),
            (EDBR_INV_ARG),
            ((_EDB_INFO"Receive EDB_DVB_MSG_CHANNEL_SCAN, h_edb_eng is null\r\n"))
            );
    if(b_edb_is_in_channel_scan == b_scan_flag)
    {
        return EDBR_OK;
    }
    x_dbg_stmt("_edb_dvb_process_enable_channel_scan b_scan_flag=%d\r\n",b_scan_flag);
    
    if(b_scan_flag == TRUE)
    {
        b_edb_should_start_cache = FALSE;
    }       /* if it is in channel scan , should stop edb cache */
    else 
    {
        b_edb_should_start_cache = TRUE;
    }
    edb_eng_list_lock();    
    _edb_dvb_enable_edb(h_edb_eng,b_edb_should_start_cache);
    b_edb_is_in_channel_scan = b_scan_flag;
    edb_eng_list_unlock();

    return i4_ret;
}



static INT32 _edb_dvb_process_dt_nfy(EDB_DVB_MSG_T t_msg)
{
    INT32                   i4_ret                  = 0x0;
    HANDLE_T                h_edb_eng               = NULL_HANDLE;
    DVB_ENG_DATA*           pt_eng_data             = NULL;
    UINT32                  ui4_current_channel_id  = 0;
    EDB_EVENT_POOL_CH_T*    pt_channel              = NULL;
    EDB_EVENT_POOL_CH_KEY*  pt_locate               = NULL;

    h_edb_eng = (HANDLE_T)t_msg.pv_tag;
    //
#ifdef DEBUG
    {
        DT_COND_T               e_dt_cond;
        e_dt_cond = (DT_COND_T)t_msg.ui4_data;
        DBG_INFO((_EDB_INFO"_edb_dvb_process_dt_nfy e_dt_cond=%d\r\n",e_dt_cond));
    }

#endif

    EDB_CHECK_RET( 
            (h_edb_eng == NULL_HANDLE),
            (EDBR_INV_ARG),
            ((_EDB_INFO"Receive EDB_DVB_MSG_DT_NFY, h_edb_eng is null\r\n\r\n"))
            );

    i4_ret = edb_dvb_get_eng_data_from_ctrl(h_edb_eng,(VOID**)&pt_eng_data);
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK || pt_eng_data == NULL || (& (pt_eng_data->pt_edb->t_edb_event_pool)) == NULL ),
            (EDBR_INV_ARG),
            ((_EDB_INFO"Can not get engine data from client\r\n"))
            );
    edb_eng_list_lock();    
    ui4_current_channel_id =  pt_eng_data->t_edb_eng_state.ui4_current_channel_id;
    i4_ret = _edb_dvb_get_ch_locate_obj(pt_eng_data,ui4_current_channel_id, &pt_locate);
    if (pt_locate != NULL)
    {
        i4_ret = edb_pool_get_ch_obj(& (pt_eng_data->pt_edb->t_edb_event_pool), pt_locate, &pt_channel);
        if (pt_channel != NULL)
        {
            //check present && following event 
            i4_ret = edb_dvb_check_pf_event(pt_channel,h_edb_eng,EDB_DVB_PF_NFY_DT);
        }
    }


    edb_eng_list_unlock();

    return i4_ret;
}


static INT32 _edb_dvb_process_nfy_change_channel(EDB_DVB_MSG_T t_msg)
{
#if 0
    t_msg.e_msg_type            = EDB_DVB_MSG_CHANGE_CHANNEL_NFY;
    t_msg.ui4_data              = (UINT32)pt_ch_id->ui4_channel_id;
    t_msg.pv_tag                = pt_eng_data; /*DVB_ENG_DATA*  */
#endif

    INT32                   i4_ret      = 0x0;
    EDB_EVENT_POOL_CH_T*    pt_ch_obj   = NULL;
    HANDLE_T                h_edb_eng   = NULL_HANDLE;
    DVB_ENG_DATA*           pt_eng_data = NULL;

    h_edb_eng = (HANDLE_T)t_msg.pv_tag;

    EDB_CHECK_RET( 
            (h_edb_eng == NULL_HANDLE),
            (EDBR_INV_ARG),
            ((_EDB_INFO"Receive EDB_DVB_MSG_CHANGE_CHANNEL_NFY, h_edb_eng is null\r\n\r\n"))
            );

    i4_ret = edb_dvb_get_eng_data_from_ctrl(h_edb_eng,(VOID**)&pt_eng_data);
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK || pt_eng_data == NULL || (& (pt_eng_data->pt_edb->t_edb_event_pool)) == NULL ),
            (EDBR_INV_ARG),
            ((_EDB_INFO"Can not get engine data from client\r\n"))
            );

    edb_eng_list_lock();
    pt_eng_data->t_edb_eng_state.ui4_current_channel_id = t_msg.ui4_data;

    //Start dvb_engien
    i4_ret = _edb_dvb_enable_edb(h_edb_eng,TRUE);
    if (i4_ret != EDBR_OK)
    {
        DBG_INFO(("Start EDB error %d\r\n",i4_ret));
        edb_eng_list_unlock();
        return i4_ret;
    }


    i4_ret = _edb_dvb_util_get_channel_by_edb_eng(h_edb_eng,pt_eng_data->t_edb_eng_state.ui4_current_channel_id, &pt_ch_obj);
    if (i4_ret != EDBR_OK || pt_ch_obj == NULL)
    {
        edb_eng_list_unlock();
        DBG_INFO(("Get pool channel fail\r\n"));
        return i4_ret;
    }

    _edb_dvb_load_sdt(h_edb_eng);
    
    i4_ret = edb_dvb_check_pf_event(pt_ch_obj,h_edb_eng,EDB_DVB_PF_NFY_CHANGE_CHANNEL);
    pt_ch_obj->ui4_time_stamp = x_os_get_sys_tick();//set time stamp for this channel
    edb_eng_list_unlock();
    return i4_ret;
}





static INT32 _edb_dvb_iterator_edb_hash(
        HANDLE_T          h_hash,
        UINT32            ui4_counter,
        VOID*             pv_obj,
        VOID*             pv_parse_tag )
{
    INT32                   i4_ret   = 0;
    EDB_T*                  pt_edb   = NULL; 
    EDB_ENG_CTRL_ENTRY_T*   pt_eng_ctrl_entry = NULL;
    DVB_ENG_DATA*           pt_eng_data        = NULL;
    
    EDB_CHECK_RET( 
            (pv_obj == NULL),
            (EDBR_INTERNAL_ERROR),
            (("Arguments is wrong\r\n"))
            );

    pt_edb = (EDB_T*)pv_obj;
    EDB_CHECK_RET( 
            (pt_edb == NULL),
            (EDBR_INTERNAL_ERROR),
            (("Invalid handle\r\n"))
            );

    DLIST_FOR_EACH(pt_eng_ctrl_entry,&( pt_edb->t_eng_ctrl_list), t_link)
    {
        pt_eng_data =pt_eng_ctrl_entry->pt_eng_data  ;

        DBG_INFO((_EDB_INFO"ac_tuner_name   = %s \r\n",pt_eng_ctrl_entry->ac_tuner_name  ));
        DBG_INFO((_EDB_INFO"ui2_svl_id      = %d \r\n",pt_eng_ctrl_entry->ui2_svl_id  ));
        DBG_INFO((_EDB_INFO"h_eng_ctrl      = 0x%08x \r\n",pt_eng_ctrl_entry->h_eng_ctrl  ));
        DBG_INFO((_EDB_INFO"pt_eng          = 0x%08x \r\n",pt_eng_ctrl_entry->pt_eng  ));
        DBG_INFO((_EDB_INFO"pt_edb          = 0x%08x \r\n",pt_eng_ctrl_entry->pt_edb  ));
        DBG_INFO((_EDB_INFO"pt_eng_data     = 0x%08x \r\n",pt_eng_ctrl_entry->pt_eng_data  ));
        DBG_INFO((_EDB_INFO"state           = %s  \r\n",  (pt_eng_data->t_edb_eng_state.fg_enable?"TRUE":"FALSE")  ));
        
        if (pt_eng_data != NULL && pt_eng_data->t_edb_eng_state.fg_enable == TRUE)
        {

            if(pt_eng_ctrl_entry != NULL && pt_eng_ctrl_entry->h_eng_ctrl  != NULL_HANDLE)
            {
                //Stop EDB
                _edb_dvb_stop_cache(pt_eng_ctrl_entry->h_eng_ctrl);

                //Clean pool
                i4_ret = _edb_dvb_recreate_pool(pt_eng_ctrl_entry->h_eng_ctrl);

                //Start EDB
                _edb_dvb_start_cache(pt_eng_ctrl_entry->h_eng_ctrl);

                edb_handle_notify(EDB_REASON_EVENT_IN_ACTIVE_WIN_UPDATED, pt_eng_data->ui2_svl_id,0);
            }
        }
        

    }


    DBG_INFO((_EDB_INFO"EDB Eng & Eng data END=======================================\r\n"));


    /*TODO dump EDB state*/    

    return i4_ret;
}


static INT32 _edb_dvb_process_pool_full(EDB_DVB_MSG_T t_msg)
{
    INT32                   i4_ret                  = 0x0;

    edb_eng_list_lock();    
    DBG_INFO((_EDB_INFO"EDB Max memory =%d[byte] %d[k]\r\n",edb_get_memory_size(),edb_get_memory_size()/1024));
    DBG_INFO((_EDB_INFO"EDB memory =%d[byte] %d[k]\r\n",edb_get_mem(),edb_get_mem()/1024));

    
    
    edb_pool_lock_wr();
    if (g_edb_handle_hash != NULL_HANDLE)
    {
        i4_ret = edb_hash_parse(g_edb_handle_hash,_edb_dvb_iterator_edb_hash,NULL);
    }
    edb_pool_unlock();
    
    edb_eng_list_unlock();
    return i4_ret;
}


static PARSE_RET_T _parse_service_guidance (
        HANDLE_T            h_obj,
        const UINT8*        pui1_obj_data,
        SIZE_T              z_obj_data_len,
        UINT16              ui2_count,
        VOID*               pv_data,
        SIZE_T              z_data_len)
{
    DVB_ENG_DATA*           pt_eng_data             = NULL;
    UINT8                    ui1_descriptor_tag         = 0;
    UINT8                    ui1_descriptor_length    = 0;
    EDB_PARSE_CHANNEL_T*      pt_parse_channel         = NULL;
    EDB_EVENT_POOL_T*         pt_edb_event_pool         =NULL; 
    EDB_EVENT_POOL_CH_T*      pt_channel              = NULL;


    if ( pui1_obj_data == NULL || 
        (pui1_obj_data[0] != DVB_DESCR_TAG_PRIVATE_DATA &&
         pui1_obj_data[0] != DVB_USER_DTT_DESCR_TAG_GUIDANCE) ||
        pv_data == NULL)
    {
        return PARSE_RET_NEXT;
    }

    ui1_descriptor_tag    = pui1_obj_data[0];
    ui1_descriptor_length = pui1_obj_data[1];

    if(z_obj_data_len != (SIZE_T) (ui1_descriptor_length + 2))
    {
        return PARSE_RET_NEXT;
    }    

    if(ui1_descriptor_tag == DVB_DESCR_TAG_PRIVATE_DATA)
    {
        ui4_service_private_data_specifier =  GET_UINT32_FROM_PTR_BIG_END(&(pui1_obj_data[2]));
        return PARSE_RET_NEXT;
    }
    else if (
             (ui1_descriptor_tag == DVB_USER_DTT_DESCR_TAG_GUIDANCE) &&
             (_dvb_check_private_is_valid(ui1_descriptor_tag,ui4_service_private_data_specifier) == TRUE)
            )
    {
        INT32                i4_ret       = EDBR_INFO_NOT_FOUND;
        ISO_639_LANG_T       t_lang       = {0};
        EDB_PREF_LANG_T*     pt_pref_lang = NULL; 

        DBG_INFO(("{EDB} process DVB_USER_DTT_DESCR_TAG_GUIDANCE  \r\n"));

        pt_parse_channel = (EDB_PARSE_CHANNEL_T*)(pv_data);    
        i4_ret = edb_dvb_get_eng_data_from_ctrl(pt_parse_channel->h_edb_eng,(VOID**)&pt_eng_data);
        EDB_CHECK_RET( 
            (i4_ret != EDBR_OK || pt_eng_data == NULL ),
            (PARSE_RET_END),
            ((_EDB_INFO"Can not get engine data from eng_ctrl\r\n"))
            );
    
        if (pt_eng_data != NULL)
        {
            pt_pref_lang = &(pt_eng_data->t_edb_eng_state.t_pref_lang); /*Matched language*/
        }
    
        /*Get all language from short event descriptors*/
       
    
        if (pt_pref_lang != NULL)
        {                        
            if((pui1_obj_data[2] & 0x3) == 0x0)
            {
                x_memcpy(&t_lang, pui1_obj_data + 3, 3);
                if (_dvb_lang_match(&t_lang, &(pt_pref_lang->t_lang[0])) == FALSE)
                {
                    return PARSE_RET_NEXT;
                }
            }
            else if((pui1_obj_data[2] & 0x3) == 0x1)
            {
                x_memcpy(&t_lang, pui1_obj_data + 4, 3);
                if (_dvb_lang_match(&t_lang, &(pt_pref_lang->t_lang[0])) == FALSE)
                {
                    return PARSE_RET_NEXT; 
                }                
            }

            
        }

        //find the channel
        
        pt_edb_event_pool = &(pt_eng_data->pt_edb->t_edb_event_pool);

        edb_pool_lock_wr();

        i4_ret = edb_pool_get_ch_obj(pt_edb_event_pool,&(pt_parse_channel->t_locate), &pt_channel);

        if(i4_ret != EDBR_OK || pt_channel ==NULL)
        {
            edb_pool_unlock();
            return PARSE_RET_END;
        }
        /*Start decode*/
        {
            UINT8*                    pui1_guidance_text_start_addr = NULL;
            UINT8                    ui1_guidance_type              = 0;
            UINT8                    ui1_guidance_text_length      = 0;
            DVB_SI_TXT_INFO_T        t_dvb_si_txt                  = {0};
            UINT8                    ui1_guidance_mode              = GUIDANCE_MODE_NOT_DEFINE;

            ui1_guidance_type = (pui1_obj_data[2] & 0x3);
    
            if(ui1_guidance_type == 0x0)
            {
                ui1_guidance_text_length = ui1_descriptor_length - 4;
                pui1_guidance_text_start_addr = (UINT8 *)pui1_obj_data + 6;
            }
            else if(ui1_guidance_type == 0x1)
            {            
                ui1_guidance_mode = (pui1_obj_data[2] & 0x1);
                if(ui1_guidance_mode == 1)
                {
                    pt_channel->t_channel_guidance_info.ui1_guidance_mode = UNSUITABLE_UNTIL_AFTER_THE_WATERSHED; 
                }
                ui1_guidance_text_length = ui1_descriptor_length - 5;
                pui1_guidance_text_start_addr = (UINT8 *)pui1_obj_data + 7;
            }
            
            if (ui1_guidance_text_length>0)
            {
                x_memset(&t_dvb_si_txt,0x0,sizeof(DVB_SI_TXT_INFO_T));
                edb_convert_raw_text_data(&pui1_guidance_text_start_addr,ui1_guidance_text_length,
                                           pt_eng_data->t_edb_eng_state.t_country,
                                           pt_eng_data->ui2_svl_id);

                i4_ret = _dvb_txt_process(pui1_guidance_text_start_addr,ui1_guidance_text_length,
                                          pt_eng_data->t_edb_eng_state.t_country,
                                          pt_eng_data->t_edb_eng_state.t_operator, DVB_TEXT_CHAR_CODE_INVALID,
                                          &t_dvb_si_txt);



                if (i4_ret == EDBR_OK && 
                    t_dvb_si_txt.ps_txt!= NULL /*&& (*(t_dvb_si_txt.pz_txt_len)) > 0*/
                    )
                {    //event title in t_dvb_si_txt.ps_txt (UTF8)

                    DBG_INFO(("{EDB} get guidance text=%s  \r\n",t_dvb_si_txt.ps_txt));
                    if(pt_channel->t_channel_guidance_info.ps_event_guidance != NULL)
                    {
                        EDB_PARTITION_FREE(pt_channel->t_channel_guidance_info.ps_event_guidance);
                        pt_channel->t_channel_guidance_info.ps_event_guidance = NULL;
                    }
                    pt_channel->t_channel_guidance_info.ps_event_guidance = t_dvb_si_txt.ps_txt;
                    //x_dbg_stmt("%s[%d] Got guidance=%s\r\n",__FILE__,__LINE__,pt_channel->t_channel_guidance_info.ps_event_guidance);

                    //Check current channel and notify
                    if (pt_channel->ui4_channel_id == pt_eng_data->t_edb_eng_state.ui4_current_channel_id)
                    {
                        edb_handle_notify(EDB_REASON_NOW_EVENT_UPDATED, pt_eng_data->ui2_svl_id,
                                          pt_eng_data->t_edb_eng_state.ui4_current_channel_id);    
                        edb_handle_notify(EDB_REASON_NEX_EVENT_UPDATED, pt_eng_data->ui2_svl_id,
                                          pt_eng_data->t_edb_eng_state.ui4_current_channel_id);    
                    }
                     //x_dbg_stmt("%s[%d]Notify updated channelId = %d\r\n",__FILE__,__LINE__,pt_channel->ui4_channel_id);

                    edb_handle_notify(EDB_REASON_EVENT_IN_ACTIVE_WIN_UPDATED, pt_eng_data->ui2_svl_id,pt_channel->ui4_channel_id);

                }
                else
                {
                    if(t_dvb_si_txt.ps_txt != NULL) {EDB_PARTITION_FREE(t_dvb_si_txt.ps_txt);}
                }
            }    
            
        }
        edb_pool_unlock();
        /*Get user setting pref language*/
        //EDB_FREE(pt_lang);
        
        return PARSE_RET_END;

    }

    return PARSE_RET_NEXT;
}

static INT32 _edb_dvb_process_sdt_descriptor(EDB_DVB_MSG_T t_msg)
{
    INT32                       i4_ret                      = 0x0;
    HANDLE_T                    h_edb_eng                   = NULL_HANDLE;
    DVB_ENG_DATA*               pt_eng_data                 = NULL;
    UINT8                       aui1_descr_tag[2]           = {0};
    UINT16                      ui2_service_num             = 0 ;
    UINT16                      ui2_idx                     = 0 ;        
    TM_COND_T                   e_tm_cond;
    TM_FIRST_LOOP_DESCR_SEL_T   t_first_loop_descr_sel      = {0};
    DVB_SI_SVC_ENTRY_INFO_T     t_svc_info                  = {0};
    EDB_EVENT_POOL_CH_KEY       t_locate                    = {0};
    EDB_PARSE_CHANNEL_T         t_parse_channel             = {0};
    EDB_EVENT_POOL_T*           pt_edb_event_pool           = NULL; 
    EDB_EVENT_POOL_CH_T*        pt_channel                  = NULL;
    EDB_EVENT_POOL_CH_KEY*      pt_current_channel_locate   = NULL;
    BOOL                        b_should_notify_pf_update   = FALSE;
    BOOL                        b_temp_eit_pf_flag          = FALSE;


    h_edb_eng = (HANDLE_T)t_msg.pv_tag;

    EDB_CHECK_RET( 
            (h_edb_eng == NULL_HANDLE),
            (EDBR_INV_ARG),
            ((_EDB_INFO"Receive EDB_DVB_MSG_START_PARSE_SDT_DESC, h_edb_eng is null\r\n\r\n"))
            );

    i4_ret = edb_dvb_get_eng_data_from_ctrl(h_edb_eng,(VOID**)&pt_eng_data);
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK || pt_eng_data == NULL ||  pt_eng_data->pt_edb == NULL ),
            (EDBR_INV_ARG),
            ((_EDB_INFO"Can not get engine data from eng_ctrl\r\n"))
            );




    i4_ret = x_dvb_si_get_num_svc_entries (pt_eng_data->h_sdt, 
                                           &ui2_service_num,
                                           &e_tm_cond);
    if (i4_ret != TMR_OK)
    {
        return EDBR_INTERNAL_ERROR;
    }

    edb_pool_lock_wr();
    
    i4_ret = _edb_dvb_get_ch_locate_obj(pt_eng_data,pt_eng_data->t_edb_eng_state.ui4_current_channel_id, &pt_current_channel_locate);

    for (ui2_idx = 0; ui2_idx < ui2_service_num; ui2_idx++)
    {   /* Only load the PMT of the data broadcast services */
        i4_ret = x_dvb_si_get_svc_entry(pt_eng_data->h_sdt, ui2_idx, &t_svc_info, &e_tm_cond);
        if ((i4_ret != TMR_OK) ||
            (i4_ret == TMR_OK && (e_tm_cond != TM_COND_AVAILABLE && 
                                  e_tm_cond != TM_COND_UPDATE)))
        {
            continue;
        }

        t_locate.ui2_svl_id       = pt_eng_data->ui2_svl_id;
        t_locate.ui2_on_id        = pt_eng_data->t_edb_eng_state.ui2_current_on_id;
        t_locate.ui2_ts_id        = pt_eng_data->t_edb_eng_state.ui2_current_ts_id;
        t_locate.ui2_svc_id       = t_svc_info.ui2_svc_id;

        //find the channel
        
        pt_edb_event_pool = &(pt_eng_data->pt_edb->t_edb_event_pool);

        

        i4_ret = edb_pool_get_ch_obj(pt_edb_event_pool,&t_locate, &pt_channel);

        //first delete guidance infomation and notify ap update.
        if (pt_channel != NULL && pt_channel->ui4_channel_id == pt_eng_data->t_edb_eng_state.ui4_current_channel_id)
        {
            if(pt_channel->t_channel_guidance_info.ps_event_guidance != NULL)
            {
                EDB_PARTITION_FREE(pt_channel->t_channel_guidance_info.ps_event_guidance);
                pt_channel->t_channel_guidance_info.ps_event_guidance = NULL;
            }

            edb_handle_notify(EDB_REASON_NOW_EVENT_UPDATED, pt_eng_data->ui2_svl_id,
                              pt_eng_data->t_edb_eng_state.ui4_current_channel_id);    
            edb_handle_notify(EDB_REASON_NEX_EVENT_UPDATED, pt_eng_data->ui2_svl_id,
                              pt_eng_data->t_edb_eng_state.ui4_current_channel_id);    

        }
        edb_handle_notify(EDB_REASON_EVENT_IN_ACTIVE_WIN_UPDATED, pt_eng_data->ui2_svl_id,pt_channel->ui4_channel_id);

        if(i4_ret == EDBR_OK && pt_channel != NULL)
        {
            if(t_svc_info.ui1_flags & SVC_FLAG_EIT_PF_FLAG)
            {
                b_temp_eit_pf_flag = TRUE;
            }
            else
            {
                b_temp_eit_pf_flag = FALSE;
            }
          
            if((x_memcmp(pt_current_channel_locate , &t_locate ,sizeof(EDB_EVENT_POOL_CH_KEY)) == 0)&&
                (pt_channel->b_eit_pf_flag != b_temp_eit_pf_flag)
                )
            {
                b_should_notify_pf_update = TRUE;
                DBG_INFO(("{EDB} current channel pf flag is  %d \r\n",pt_channel->b_eit_pf_flag));
                DBG_INFO(("{EDB} current channel pf flag will be  \r\n",b_temp_eit_pf_flag));
            }
            
            pt_channel->b_eit_pf_flag = b_temp_eit_pf_flag ;
        }

        t_first_loop_descr_sel.ui2_loop_idx        = ui2_idx;
        aui1_descr_tag[0] = DVB_DESCR_TAG_PRIVATE_DATA ; 
        aui1_descr_tag[1] = DVB_USER_DTT_DESCR_TAG_GUIDANCE ; 
        t_first_loop_descr_sel.pui1_descr_tag_list = aui1_descr_tag;
        t_first_loop_descr_sel.z_num_descr_tags    = 2;

        t_parse_channel.h_edb_eng = h_edb_eng;
        t_parse_channel.t_locate  = t_locate;

        ui4_service_private_data_specifier = 0;

        i4_ret = x_tm_parse_obj(pt_eng_data->h_sdt,
                                TM_PARSE_TYPE_FIRST_LOOP_DESCRS,
                                &t_first_loop_descr_sel,
                                &t_parse_channel,
                                sizeof(t_parse_channel),
                                _parse_service_guidance,
                                &e_tm_cond);

        if ((i4_ret != TMR_OK) ||
            (i4_ret == TMR_OK && (e_tm_cond != TM_COND_AVAILABLE && 
                                  e_tm_cond != TM_COND_UPDATE)))
        {
            continue;
        }

       
        
    }
    
    edb_pool_unlock();
    
    if((b_should_notify_pf_update == TRUE) && (x_strncmp(pt_eng_data->t_edb_eng_state.t_country,"NZL",3) == 0))
    {
        edb_handle_notify(EDB_REASON_NOW_EVENT_UPDATED, pt_eng_data->ui2_svl_id, pt_eng_data->t_edb_eng_state.ui4_current_channel_id);   
        edb_handle_notify(EDB_REASON_NEX_EVENT_UPDATED, pt_eng_data->ui2_svl_id, pt_eng_data->t_edb_eng_state.ui4_current_channel_id);   
    }
    
    return i4_ret;
}

#ifdef    MW_EDB_FBM_SHARE_MEM
static INT32 _edb_dvb_process_set_risktv(EDB_DVB_MSG_T t_msg)
{

    HANDLE_T                h_edb_eng               = NULL_HANDLE;
    BOOL                    fg_flag;
    INT32                   i4_ret                  = EDBR_OK;
    DVB_ENG_DATA*           pt_eng_data             = NULL;
    UINT8                   ui1_m_size              = 0;

    fg_flag   = (BOOL)t_msg.ui4_data;
    h_edb_eng = (HANDLE_T)t_msg.pv_tag;

    x_dbg_stmt("_edb_dvb_process_set_risktv fg_flag=%d\r\n",fg_flag);

    EDB_CHECK_RET( 
            (h_edb_eng == NULL_HANDLE),
            (EDBR_INV_ARG),
            ((_EDB_INFO"Receive EDB_DVB_MSG_SET_RISKTV, h_edb_eng is null\r\n"))
            );

    if(fg_flag == TRUE)
    {
        ui1_m_size = 6;
    }
    else
    {
        ui1_m_size = 12;
    }

    if(ui1_edb_from_fbm_size == ui1_m_size)
    {
        return EDBR_OK;
    }

    edb_eng_list_lock();    

    _edb_dvb_stop_cache(h_edb_eng);
    
    i4_ret = edb_dvb_get_eng_data_from_ctrl(h_edb_eng,(VOID**)&pt_eng_data);
        
    if (i4_ret == EDBR_OK && pt_eng_data != NULL)
    {
        EDB_T*                pt_edb = NULL; 
        if (pt_eng_data->pt_edb != NULL)
        {
            pt_edb = pt_eng_data->pt_edb ;
            edb_pool_lock_wr();
            edb_pool_delete(&(pt_edb->t_edb_event_pool));
            
            i4_ret = edb_mem_partition_create(ui1_m_size);

            if(i4_ret == EDBR_OK)
            {
                ui1_edb_from_fbm_size = ui1_m_size;
                i4_ret = edb_pool_create(
                        EDB_POOL_HASH_CHANNEL_TABLE_SIZE,
                        x_dt_get_brdcst_utc(NULL,NULL),
                        &(pt_edb->t_edb_event_pool)
                        );
                if (i4_ret != EDBR_OK)
                {
                    DBG_INFO(("edb_pool_create Error ui2_total_channels_cap=%d\r\n",EDB_POOL_HASH_CHANNEL_TABLE_SIZE));
                }
            }            
            else
            {
                x_dbg_stmt("edb_mem_partition_create fail!");
            }
            edb_pool_unlock();
        }
        else
        {
            i4_ret = EDBR_INTERNAL_ERROR;
        }
    }
    
    _edb_dvb_start_cache(h_edb_eng);

    edb_eng_list_unlock();

    if(i4_ret == EDBR_OK)
    {
        pt_eng_data->t_edb_eng_state.fg_risktv = fg_flag;
    }
    
    return i4_ret;
}
#endif

static INT32 _edb_dvb_search_following_event_from_schedule(EDB_EVENT_POOL_CH_T*    pt_ch_obj)
{
    INT32                           i4_ret          = EDBR_INFO_NOT_FOUND;
    UINT32                          ui4_i           = 0;
    P_ELEMENT_OBJ_T                 pt_elm          = NULL;
    EDB_HASH_T*                     pt_hash_obj     = NULL;
    EDB_POOL_EVENT_INFO_T*          pt_pool_event   = NULL;
    TIME_T                          t_base_time  = NULL_TIME;
    DT_COND_T                       t_dt_cond       = DT_NOT_RUNNING;
    HANDLE_TYPE_T                   e_hdl_type;
    TIME_T                          t_start_time    = NULL_TIME;
    EDB_POOL_EVENT_INFO_T*          pt_pool_event_following   = NULL;

    EDB_CHECK_RET( 
            (pt_ch_obj == NULL),
            (EDBR_INTERNAL_ERROR),
            (("Channel obj is null\r\n"))
            );

    if(pt_ch_obj->pt_present_event!=NULL)
    {
        t_base_time = pt_ch_obj->pt_present_event->t_start_time + pt_ch_obj->pt_present_event->t_duration;
        DBG_INFO(("use present end time to search next event \n"));
    }
    else
    {
        t_base_time = x_dt_get_brdcst_utc(NULL,&t_dt_cond) + EDB_THRESHOLD;
        DBG_INFO(("use current utc  to search next event \n"));        
    }
    PRINT_TIME("search next from database base time:",t_base_time);

    i4_ret = handle_get_type_obj( pt_ch_obj->h_event_hash, &e_hdl_type, (VOID**)&pt_hash_obj );
    if ( i4_ret != HR_OK || e_hdl_type != EDBT_HASH )
    {
        return EDBR_INV_HANDLE;
    }
    
    i4_ret          = EDBR_INFO_NOT_FOUND;

    for ( ui4_i=0; ui4_i<pt_hash_obj->z_table_size; ++ui4_i )
    {
        for( pt_elm = pt_hash_obj->ppt_table[ui4_i]; pt_elm != NULL; )
        {
            pt_pool_event = (EDB_POOL_EVENT_INFO_T*)pt_elm->pv_obj;
            if (    (pt_pool_event != NULL) &&
                    (t_base_time <= pt_pool_event->t_start_time) 
               )
            {
                if (t_start_time == NULL_TIME)
                {
                    t_start_time = pt_pool_event->t_start_time;
                    pt_pool_event_following = pt_pool_event;
                    i4_ret          = EDBR_OK;

                }
                else
                {
                    if(pt_pool_event->t_start_time < t_start_time)
                    {
                        t_start_time = pt_pool_event->t_start_time;
                        pt_pool_event_following = pt_pool_event;
                        i4_ret          = EDBR_OK;
                    }
                }
            }
            pt_elm = pt_elm->pt_next;
        }
    }


    if (i4_ret == EDBR_OK && pt_pool_event_following != NULL)
    {
        if (TRUE/*pt_ch_obj->pt_following_event != NULL*/)//
        {
            pt_ch_obj->pt_following_event = pt_pool_event_following;
            i4_ret = EDBR_OK;
        }
    }
    else
    {
        if (pt_ch_obj->pt_following_event != NULL)//p
        {
            pt_ch_obj->pt_following_event = NULL;
            i4_ret = EDBR_OK;
        }
    }

    return i4_ret;
}

#if 1
/*
   switch P to schedule
   */
static INT32 _edb_dvb_search_present_event_from_schedule(EDB_EVENT_POOL_CH_T*    pt_ch_obj)
{
    INT32                           i4_ret          = EDBR_INFO_NOT_FOUND;
    UINT32                          ui4_i           = 0;
    P_ELEMENT_OBJ_T                 pt_elm          = NULL;
    EDB_HASH_T*                     pt_hash_obj     = NULL;
    EDB_POOL_EVENT_INFO_T*          pt_pool_event   = NULL;
    TIME_T                          t_current_time  = NULL_TIME;
    DT_COND_T                       t_dt_cond       = DT_NOT_RUNNING;
    HANDLE_TYPE_T                   e_hdl_type;


    t_current_time = x_dt_get_brdcst_utc(NULL,&t_dt_cond);

    EDB_CHECK_RET( 
            (pt_ch_obj == NULL),
            (EDBR_INTERNAL_ERROR),
            (("Channel obj is null\r\n"))
            );

    i4_ret = handle_get_type_obj( pt_ch_obj->h_event_hash, &e_hdl_type, (VOID**)&pt_hash_obj );
    if ( i4_ret != HR_OK || e_hdl_type != EDBT_HASH )
    {
        return EDBR_INV_HANDLE;
    }
    
    i4_ret          = EDBR_INFO_NOT_FOUND;

    for ( ui4_i=0; ui4_i<pt_hash_obj->z_table_size; ++ui4_i )
    {
        for( pt_elm = pt_hash_obj->ppt_table[ui4_i]; pt_elm != NULL; )
        {
            pt_pool_event = (EDB_POOL_EVENT_INFO_T*)pt_elm->pv_obj;
            if (    (pt_pool_event != NULL) &&
                    (t_current_time >= pt_pool_event->t_start_time) &&
                    (t_current_time < (pt_pool_event->t_start_time + pt_pool_event->t_duration) )
               )//Found event from schedule list
            {
                //Copy event from pool to EDB_EVENT_INFO_T
                if (TRUE/*pt_ch_obj->pt_present_event != NULL*/)//pt_present_event is invalid ,delete it
                {
                    pt_ch_obj->pt_present_event = pt_pool_event;
                    //pt_ch_obj->fg_present_event_from_pf = FALSE;
                    DBG_INFO(("{EDB} has searched a present event successfully\r\n"));
                    PRINT_TIME("present event start time :",pt_ch_obj->pt_present_event->t_start_time);
                    DBG_INFO((_EDB_INFO"channle id = 0x%x event id = 0x%x\r\n",pt_ch_obj->ui4_channel_id,pt_ch_obj->pt_present_event->ui2_event_id));
                }
                i4_ret = EDBR_OK;
                break;
            }
            pt_elm = pt_elm->pt_next;
        }
    }

    if (i4_ret == EDBR_INFO_NOT_FOUND)//Switch PF to schedule fail
    {
        if (pt_ch_obj->pt_present_event != NULL)//pt_present_event is invalid ,delete it
        {
            DBG_INFO(("{EDB} hasn't search a valid present event \r\n"));
            pt_ch_obj->pt_present_event = NULL;
            //pt_ch_obj->fg_present_event_from_pf = FALSE;
            i4_ret = EDBR_OK;
        }
    }
    return i4_ret;
}
#endif


INT32 edb_dvb_check_pf_event(
        EDB_EVENT_POOL_CH_T*    pt_ch_obj,
        HANDLE_T                h_edb_eng,
        UINT8                   ui1_type
        )
{
    INT32                           i4_ret             = 0x0;
    TIME_T                          t_current_time     = NULL_TIME;
    DT_COND_T                       t_dt_cond          = DT_NOT_RUNNING;
    EDB_POOL_EVENT_INFO_T*          pt_present_event   = NULL;
    EDB_POOL_EVENT_INFO_T*          pt_following_event = NULL;
    BOOL                            fg_current_ready   = FALSE;
    BOOL                            fg_next_ready      = FALSE;
    DVB_ENG_DATA*                   pt_eng_data        = NULL;
    BOOL                            fg_notify          = TRUE;

    EDB_CHECK_RET( 
            (pt_ch_obj == NULL|| h_edb_eng  == NULL_HANDLE),
            (EDBR_INTERNAL_ERROR),
            (("Channel obj or handle of engine data is null\r\n"))
            );


    i4_ret = edb_dvb_get_eng_data_from_ctrl(h_edb_eng,(VOID**)&pt_eng_data);
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK || pt_eng_data == NULL),
            (EDBR_INTERNAL_ERROR),
            ((_EDB_INFO"Can not get engine data from client\r\n"))
            );


    t_current_time = x_dt_get_brdcst_utc(NULL,&t_dt_cond);

    PRINT_TIME("edb_dvb_check_pf_event current time:",t_current_time);

    pt_present_event   = pt_ch_obj->pt_present_event ;
    pt_following_event = pt_ch_obj->pt_following_event;

    if(ui1_type == EDB_DVB_PF_NFY_CHANGE_CHANNEL)
    {
        //When AP channge channel,does not notify caller
        //SVCTX will start event query,
        DBG_INFO(("change channel notify,does not notify caller\r\n"));
        fg_notify = FALSE;
    }

    if(pt_present_event != NULL)
    {
        PRINT_TIME("present event start time:",pt_present_event->t_start_time);
    }
    else
    {
        DBG_INFO(("present event NULL\r\n"));
    }

    if(pt_following_event != NULL)
    {
        PRINT_TIME("following event start time:",pt_following_event->t_start_time);
    }
    else
    {
        DBG_INFO(("following event NULL\r\n"));
    }

    if (TRUE)
    {
        i4_ret = EDBR_INFO_NOT_FOUND;
        if (    (pt_present_event != NULL) &&
                (
                        t_current_time >= pt_present_event->t_start_time && 
                        t_current_time < (pt_present_event->t_start_time + pt_present_event->t_duration)
                )
           )//valid current event//
        {
            DBG_INFO(("current present event is NOT NULL ,and accroding the time chcke it is a valid present event\r\n"));
            if (fg_notify == TRUE)
            {
                edb_handle_notify(EDB_REASON_NOW_EVENT_UPDATED, pt_eng_data->ui2_svl_id,
                                  pt_eng_data->t_edb_eng_state.ui4_current_channel_id);
            }
            i4_ret = EDBR_OK;
        }
        else if(t_dt_cond != DT_SYNC_RUNNING)
        {
            DBG_INFO(("dt NOT sync running ,do not search the present event in pool\r\n"));
            
            if (fg_notify == TRUE)
            {
                edb_handle_notify(EDB_REASON_NOW_EVENT_UPDATED, pt_eng_data->ui2_svl_id,
                                  pt_eng_data->t_edb_eng_state.ui4_current_channel_id);
            }
            i4_ret = EDBR_OK;
        }
        else if (t_dt_cond == DT_SYNC_RUNNING)//switch PF to schedule
        {
#if 1
            DBG_INFO(("dt sync running ,search the present event in pool\r\n"));
            i4_ret = _edb_dvb_search_present_event_from_schedule(pt_ch_obj);
            if (i4_ret == EDBR_OK)
            {
                if (fg_notify == TRUE)
                {
                    edb_handle_notify(EDB_REASON_NOW_EVENT_UPDATED, pt_eng_data->ui2_svl_id,
                                      pt_eng_data->t_edb_eng_state.ui4_current_channel_id);    
                }
            }
#endif            
        }
        if (i4_ret == EDBR_OK)
        {
            fg_current_ready = TRUE;
        }
    }


    if (TRUE)
    {
        i4_ret = EDBR_INFO_NOT_FOUND;
        if (    (pt_following_event != NULL) &&
                (
                        t_current_time < pt_following_event->t_start_time 
                )
           )//valid next event
        {
            DBG_INFO((" next event is NOT NULL ,and accroding the time chcke it is a valid next event\r\n"));
            if (fg_notify == TRUE)
            {
                edb_handle_notify(EDB_REASON_NEX_EVENT_UPDATED, pt_eng_data->ui2_svl_id,
                                  pt_eng_data->t_edb_eng_state.ui4_current_channel_id);
            }
            i4_ret = EDBR_OK;
        }
        else if(t_dt_cond != DT_SYNC_RUNNING)
        {
            DBG_INFO(("dt NOT sync running ,do not search the next event in the edb pool\r\n"));
            if (fg_notify == TRUE)
            {
                edb_handle_notify(EDB_REASON_NEX_EVENT_UPDATED, pt_eng_data->ui2_svl_id,
                                  pt_eng_data->t_edb_eng_state.ui4_current_channel_id);
            }
            i4_ret = EDBR_OK;
        }
        else if (t_dt_cond == DT_SYNC_RUNNING)//switch PF to schedule
        {
            DBG_INFO(("dt sync running ,search the next event in the edb pool\r\n"));
            i4_ret = _edb_dvb_search_following_event_from_schedule(pt_ch_obj);
            if (i4_ret == EDBR_OK)
            {
                if (fg_notify == TRUE)
                {
                    edb_handle_notify(EDB_REASON_NEX_EVENT_UPDATED, pt_eng_data->ui2_svl_id,
                                      pt_eng_data->t_edb_eng_state.ui4_current_channel_id);    
                }
            }
        }
        if (i4_ret == EDBR_OK)
        {
            fg_next_ready = TRUE;
        }
    }


    if (i4_ret == EDBR_OK && (fg_current_ready ==TRUE || fg_next_ready == TRUE))
    {
        i4_ret = _edb_dvb_start_ae_timer(h_edb_eng,pt_ch_obj,fg_current_ready,fg_next_ready);
    }


    return i4_ret;
}


INT32 edb_dvb_engine_task( VOID )
{
    INT32                   i4_ret      = 0;
    UINT16                  ui2_index   = 0;
    EDB_DVB_MSG_T           t_msg       = {0};
    SIZE_T                  z_msg_size  = 0;

    z_msg_size = sizeof( EDB_DVB_MSG_T );
    i4_ret = x_msg_q_receive( &ui2_index,
                              &t_msg,
                              &z_msg_size,
                              &h_dvb_msg_q,
                              (UINT16)1,
                              X_MSGQ_OPTION_NOWAIT );
    if ( i4_ret == OSR_OK )
    {
        switch( t_msg.e_msg_type )
        {        
            case EDB_DVB_MSG_ROOT_NFY:
                {
                    i4_ret = _edb_dvb_process_root_nfy(t_msg);
                    break;
                }
            case EDB_DVB_MSG_CHANGE_CHANNEL_NFY:
                {
                    i4_ret = _edb_dvb_process_nfy_change_channel(t_msg);
                    break;
                }
            case EDB_DVB_MSG_SVL_UPDATE_NFY:
                {
                    i4_ret = _edb_dvb_process_svl_update(t_msg);
                    break;
                }
            case EDB_DVB_MSG_DT_NFY:
                {
                    _edb_dvb_process_dt_nfy(t_msg);
                    break;
                }
            case EDB_DVB_MSG_ENABLE_EDB:
                {
                    _edb_dvb_process_enable_edb_nfy(t_msg);
                    break;
                }
            case EDB_DVB_MSG_RESTART_EDB:
                {
                    _edb_dvb_process_restart_edb_nfy(t_msg);
                    break;
                }
            case EDB_DVB_MSG_TUNER_CHANGE:
                {
                    _edb_dvb_process_tuner_change_nfy(t_msg);
                    break;
                }
            case EDB_DVB_MSG_ACTIVE_NFY:
            case EDB_DVB_MSG_EXPIRE_NFY:
                {
                    _edb_dvb_process_ae_nfy(t_msg);
                    break;
                }
            case EDB_DVB_MSG_CHECK_EXPIRE_NFY:
                _edb_dvb_process_check_expire_nfy(t_msg);
                break;
            case EDB_DVB_MSG_POOL_FULL:
                _edb_dvb_process_pool_full(t_msg);
                break;
            case EDB_DVB_MSG_START_PARSE_SDT_DESC:
                _edb_dvb_process_sdt_descriptor(t_msg);
                break;
#ifdef    MW_EDB_FBM_SHARE_MEM
            case EDB_DVB_MSG_SET_RISKTV:
                _edb_dvb_process_set_risktv(t_msg);
                break;
#endif
            case EDB_DVB_MSG_CHANNEL_SCAN:
                {
                    _edb_dvb_process_enable_channel_scan(t_msg);
                    break;
                }

            default:
                break;
        }
    }
    else if ( i4_ret != OSR_NO_MSG )
    {
        return EDBR_INTERNAL_ERROR;
    }

    return EDBR_OK;
}


static VOID _edb_dvb_svl_nfy (HANDLE_T    h_svl,
                              SVL_COND_T  e_cond,
                              UINT32      ui4_reason,
                              VOID*       pv_tag,
                              UINT32      ui4_data)
{
    EDB_DVB_MSG_T       t_msg       = {0};
    INT32               i4_retry    = 0x0;

    if (e_cond != SVL_UPDATED || b_edb_is_in_channel_scan == TRUE)
    {
        return;
    }

    EDB_CHECK_RET( 
            (pv_tag == NULL || e_cond != SVL_UPDATED ),
            ,
            ((_EDB_INFO"Invalid argument\r\n"))
            );

    EDB_CHECK_RET( 
            (ui4_reason == SVL_REASON_UNKNOWN),
            ,
            ((_EDB_INFO"Unknow SVL updata reason\r\n"))
            );




    //Send msg switch thread
    DBG_INFO((_EDB_INFO"_edb_dvb_svl_nfy\r\n"));
    t_msg.e_msg_type            = EDB_DVB_MSG_SVL_UPDATE_NFY;
    t_msg.ui4_data              = e_cond;
    t_msg.pv_tag                = (VOID*)pv_tag; /*handle of edb engine*/


    while ( ++i4_retry <= 10 )
    {
        if (x_msg_q_send(h_dvb_msg_q,
                         &t_msg,
                         sizeof(EDB_DVB_MSG_T),
                         0 /* highest priority */) != OSR_OK)
        {
            /* Send message fail, wait 10 ms and retry for 10 times */
            x_thread_delay( 10 );
            continue;
        }
        edb_activate_eng_task(EDB_DVB_BRDCST_GRP);

        return ;
    }

    return ;
}

/*
   h_edb_eng ref:EDB_ENG_CTRL_ENTRY_T
   */
INT32 edb_dvb_create_eng_obj (
        HANDLE_T            h_edb_eng,
        EDB_COMMAND_T*      pt_edb_cmd,
        VOID**              ppv_eng_obj_data,
        EDB_COND_T*         pe_cond)
{
    INT32                       i4_ret            = 0;
    DVB_ENG_DATA*               pt_eng_data       = NULL;
    EDB_T*                      pt_edb            = NULL;
    EDB_ENG_CTRL_ENTRY_T*       pt_eng_ctrl_entry = NULL;
    EDB_CHECK_RET( 
            (h_edb_eng == NULL_HANDLE || pt_edb_cmd == NULL || pe_cond == NULL),
            (EDBR_INV_ARG),
            ((_EDB_INFO"Invalid argument\r\n"))
            );

    i4_ret = edb_handle_get_ctrl_obj( h_edb_eng, &pt_eng_ctrl_entry);
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK || pt_eng_ctrl_entry == NULL),
            (EDBR_INV_ARG),
            ((_EDB_INFO"Engien ctrl object isnull\r\n"))
            );

    i4_ret = edb_dvb_get_eng_data_from_ctrl(h_edb_eng,(VOID**)&pt_eng_data);
    if (i4_ret != EDBR_OK || pt_eng_data == NULL)//Engine data not found,create new one
    {
        pt_eng_data = EDB_OS_ALLOC(sizeof(DVB_ENG_DATA));
        if (pt_eng_data == NULL)
        {
            return i4_ret;
        }
        x_memset(pt_eng_data,0x0,sizeof(DVB_ENG_DATA));
        pt_eng_data->e_brdcst_type = pt_eng_ctrl_entry->pt_eng->e_brdcst_type;
        pt_eng_data->ui2_svl_id    = pt_eng_ctrl_entry->ui2_svl_id;
        x_strncpy(pt_eng_data->ac_tuner_name, pt_eng_ctrl_entry->ac_tuner_name, EDB_MAX_TUNER_NAME);
        pt_eng_data->t_edb_eng_state.fg_enable = FALSE;/*Default true*/

        i4_ret = edb_handle_get_edb_obj_by_ctrl(h_edb_eng,&pt_edb);
        if (i4_ret != EDBR_OK || pt_edb == NULL)
        {
            EDB_OS_FREE(pt_eng_data);
            return i4_ret;
        }

        pt_eng_data->pt_edb = pt_edb;

        i4_ret = _edb_dvb_add_eng_data_obj(pt_eng_ctrl_entry, pt_eng_data);
        if (i4_ret != EDBR_OK)
        {
            EDB_OS_FREE(pt_eng_data);
            return i4_ret;
        }

        if (pt_eng_data->ui2_svl_id != pt_eng_ctrl_entry->ui2_svl_id 
            || pt_eng_data->h_svl == NULL_HANDLE)
        {
            pt_eng_data->ui2_svl_id = pt_eng_ctrl_entry->ui2_svl_id;
            /*Save Tuner name*/
            x_strncpy(pt_eng_data->ac_tuner_name,pt_eng_ctrl_entry->ac_tuner_name,EDB_MAX_TUNER_NAME+1);
            if (pt_eng_data->h_svl != NULL_HANDLE)
            {
                x_svl_close(pt_eng_data->h_svl);
                pt_eng_data->h_svl = NULL_HANDLE;
            }

            i4_ret = x_svl_open(pt_eng_data->ui2_svl_id,(VOID*)h_edb_eng,_edb_dvb_svl_nfy, &(pt_eng_data->h_svl));
            /*Get SVL handle*/
            if (x_handle_valid(pt_eng_data->h_svl) != TRUE) 
            {
                x_svl_close(pt_eng_data->h_svl);
            }
        }

        //create AE timer
        x_timer_create(&(pt_eng_data->h_expire_timer));
        if (i4_ret != EDBR_OK)
        {
            DBG_INFO((_EDB_INFO"timer create fail\r\n"));
        }

        x_timer_create(&(pt_eng_data->h_active_timer));
        if (i4_ret != EDBR_OK)
        {
            DBG_INFO((_EDB_INFO"timer create fail\r\n"));
        }

        //create 3 hour check expire timer
        x_timer_create(&(pt_eng_data->h_check_expire_timer));
        if (i4_ret != EDBR_OK)
        {
            DBG_INFO((_EDB_INFO"timer create fail\r\n"));
        }


        //register DT notify
        i4_ret = x_dt_reg_nfy_fct(_edb_dt_cond_change_nfy,
                                  (VOID*)h_edb_eng,
                                  &(pt_eng_data->h_dt_nfy));

    }

    *ppv_eng_obj_data = pt_eng_data;

    i4_ret = _edb_set_edb_with_command(h_edb_eng,pt_eng_data,pt_edb_cmd);


    if (pe_cond != NULL && pt_eng_data != NULL)
    {    
        (*pe_cond) = pt_eng_data->e_cond;
    }

    return i4_ret;
}


INT32 edb_dvb_delete_eng_obj (
        VOID*               pv_eng_obj_data)
{
#if 0
    DVB_ENG_DATA*       pt_eng_data         = NULL;
    EDB_CHECK_RET( 
            (pv_eng_obj_data == NULL),
            (EDBR_INV_ARG),
            ((_EDB_INFO"pv_eng_obj_data is null\r\n"))
            );

    pt_eng_data = (DVB_ENG_DATA*)pv_eng_obj_data;


    if (pt_eng_data->h_svl != NULL_HANDLE)
    {
        x_svl_close(pt_eng_data->h_svl);
        pt_eng_data->h_svl = NULL_HANDLE;
    }
#endif

    return EDBR_OK;

}

static INT32 edb_dvb_get_event_info_len (
        HANDLE_T            h_edb_eng,
        EDB_KEY_TYPE_T      e_key_type,
        VOID*               pv_key_info,
        SIZE_T*             pz_event_info_len,
        EDB_COND_T*         pe_cond)
{
    INT32                   i4_ret      = EDBR_OK;
    DVB_ENG_DATA*           pt_eng_data     = NULL;

    switch (e_key_type)
    {
        case    EDB_KEY_TYPE_EVENT_TITLE_BY_ID:
            {
                i4_ret = _edb_dvb_process_event_title_len(h_edb_eng,pv_key_info,pz_event_info_len,pe_cond);
                break;
            }
        case    EDB_KEY_TYPE_GUIDANCE_TEXT_BY_ID:
            {
                i4_ret = _edb_dvb_process_guidance_len(h_edb_eng,pv_key_info,pz_event_info_len,pe_cond);
                break;
            }
        case    EDB_KEY_TYPE_EVENT_DETAIL_BY_ID:
            {
                i4_ret = _edb_dvb_process_event_detail_len(h_edb_eng,pv_key_info,pz_event_info_len,pe_cond);
                break;
            }

        case    EDB_KEY_TYPE_RATING_LIST_BY_ID:
            {
                /*
                   EDB_CH_EVT_ID_T * 
                   */
                i4_ret = _edb_dvb_process_rating_len(h_edb_eng,pv_key_info,pz_event_info_len,pe_cond);
                break;
            }
        case    EDB_KEY_TYPE_LINKAGE:
            {
                i4_ret = _edb_dvb_process_linkage_len(h_edb_eng,pv_key_info,pz_event_info_len,pe_cond);
                break;
            }
        case EDB_KEY_TYPE_START_TIME:
        case EDB_KEY_TYPE_DURATION:
            {
                i4_ret = _edb_dvb_process_static_len(h_edb_eng,pv_key_info,e_key_type,pz_event_info_len,pe_cond);
                break;
            }
        default:
            i4_ret = EDBR_INTERNAL_ERROR;
            break;
    }

    if (i4_ret != EDBR_OK)
    {
        return i4_ret;
    }

    i4_ret = edb_dvb_get_eng_data_from_ctrl(h_edb_eng, (VOID**)&pt_eng_data);
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK || pt_eng_data == NULL),
            (EDBR_INTERNAL_ERROR),
            ((_EDB_INFO"Can not get engine data from client\r\n"))
            );
    if (pe_cond != NULL)
    {    
        (*pe_cond) = pt_eng_data->e_cond;
    }
    return i4_ret;

}

INT32 edb_dvb_get_event_info (
        HANDLE_T            h_edb_eng,
        EDB_KEY_TYPE_T      e_key_type,
        VOID*               pv_key_info,
        SIZE_T*             pz_event_info_len,
        VOID*               pv_event_info,
        EDB_COND_T*         pe_cond)
{
    INT32                   i4_ret      = EDBR_OK;
    DVB_ENG_DATA*           pt_eng_data     = NULL;

    switch (e_key_type)
    {
        case    EDB_KEY_TYPE_EVENT_TITLE_BY_ID:
            {
                DBG_INFO(("_edb_dvb_process_event_title\r\n"));
                _edb_dvb_process_event_title(h_edb_eng,pv_key_info,pz_event_info_len,pv_event_info,pe_cond);
                break;
            }
        case    EDB_KEY_TYPE_GUIDANCE_TEXT_BY_ID:
            {
                DBG_INFO(("_edb_dvb_process_guidance\r\n"));
                _edb_dvb_process_guidance(h_edb_eng,pv_key_info,pz_event_info_len,pv_event_info,pe_cond);
                break;
            }
        case    EDB_KEY_TYPE_EVENT_DETAIL_BY_ID:
            {
                DBG_INFO(("_edb_dvb_process_event_detail\r\n"));
                _edb_dvb_process_event_detail(h_edb_eng,pv_key_info,pz_event_info_len,pv_event_info,pe_cond);
                break;
            }
        case EDB_KEY_TYPE_EVENT_ID:
            {
                DBG_INFO(("_edb_dvb_process_event_id\r\n"));
                _edb_dvb_process_event_id(h_edb_eng,pv_key_info,pz_event_info_len,pv_event_info,pe_cond);
                break;
            }
        case EDB_KEY_TYPE_START_TIME:
            {
                DBG_INFO(("_edb_dvb_process_start_time\r\n"));
                i4_ret = _edb_dvb_process_start_time(h_edb_eng,pv_key_info,pz_event_info_len,pv_event_info,pe_cond);
                break;
            }

        case EDB_KEY_TYPE_DURATION:
            {
                DBG_INFO(("_edb_dvb_process_duration\r\n"));
                i4_ret = _edb_dvb_process_duration(h_edb_eng,pv_key_info,pz_event_info_len,pv_event_info,pe_cond);
                break;
            }

        case    EDB_KEY_TYPE_PRESENT_EVENT:
            {
#ifdef PARA_FROM_AP
                EDB_KEY_TYPE_PRESENT_EVENT  get present event information;
                pv_key_info    (EDB_CH_ID_T*);
                pv_event_info (EDB_EVENT_INFO_T*);
#endif
               DBG_INFO(("_edb_dvb_process_present_event\r\n"));
               _edb_dvb_process_present_event(h_edb_eng,pv_key_info,pz_event_info_len,pv_event_info,pe_cond);

               break;
            }
        case    EDB_KEY_TYPE_FOLLOWING_EVENT:
            {
#ifdef PARA_FROM_AP
                EDB_KEY_TYPE _FOLLOWING_EVENT - get following event information;
                pv_key_info (EDB_CH_ID_T*);
                pv_event_info (EDB_EVENT_INFO_T*);
#endif
               DBG_INFO(("_edb_dvb_process_following_event\r\n"));
               _edb_dvb_process_following_event(h_edb_eng,pv_key_info,pz_event_info_len,pv_event_info,pe_cond);
               break;
            }
        case    EDB_KEY_TYPE_SINGLE_EVENT_BY_UTC:
            {
#ifdef PARA_FROM_AP
                EDB_CH_TIME_FILTER_T*  & t_ch_time_filter;
                & t_size, sizeof(EDB_BUCKET_INFO_T);
                EDB_BUCKET_INFO_T*     & t_bucket_info;
#endif
                DBG_INFO(("_edb_dvb_process_single_event_by_utc\r\n"));                    
                i4_ret = _edb_dvb_process_single_event_by_utc(h_edb_eng, pv_key_info,pz_event_info_len,pv_event_info,pe_cond);
                break;
            }
        case    EDB_KEY_TYPE_CURRENT_NEXT_EVENT_BY_UTC:
            {
                DBG_INFO(("EDB_KEY_TYPE_CURRENT_NEXT_EVENT_BY_UTC TODO\r\n")); 
                i4_ret = _edb_dvb_process_current_and_next_events_by_utc(h_edb_eng, pv_key_info,pz_event_info_len,pv_event_info,pe_cond);
                break;
            }
        case    EDB_KEY_TYPE_CURRENT_PREVIOUS_EVENT_BY_UTC:
            {
                DBG_INFO(("EDB_KEY_TYPE_CURRENT_PREVIOUS_EVENT_BY_UTC TODO\r\n")); 
                break;
            }
        case    EDB_KEY_TYPE_EVENT_IDS_BY_FILTER:
            {
                DBG_INFO(("EDB_KEY_TYPE_EVENT_IDS_BY_FILTER TODO\r\n"));  
				i4_ret = _edb_dvb_process_event_ids_by_filter(h_edb_eng, pv_key_info,pz_event_info_len,pv_event_info,pe_cond);
				
                break;
            }
        case    EDB_KEY_TYPE_EVENT_IDS_BY_DESIRED_STRING:
            {
                DBG_INFO(("EDB_KEY_TYPE_EVENT_IDS_BY_FILTER TODO\r\n"));  
				i4_ret = _edb_dvb_process_event_ids_by_desired_string(h_edb_eng, pv_key_info,pz_event_info_len,pv_event_info,pe_cond);
				
                break;
            }
#if 0
        case    EDB_KEY_TYPE_EVENT_NUM_BY_RANGE_ASYNC:
            {
                //TODO 
                break;
            }
        case    EDB_KEY_TYPE_NOW_NEXT_EVENT_NUM_BY_UTC_ASYNC:
            {
                //TODO 
                break;
            }
#endif
        case    EDB_KEY_TYPE_RATING_LIST_BY_ID:
            {
                /*
                   EDB_CH_EVT_ID_T
                   */
                DBG_INFO(("_edb_dvb_process_rating\r\n"));                  
                i4_ret = _edb_dvb_process_rating(h_edb_eng, pv_key_info,pz_event_info_len,pv_event_info,pe_cond);
                break;
            }
		case    EDB_KEY_TYPE_PREVIOUS_EVENT_BY_UTC:
            {
                DBG_INFO(("EDB_KEY_TYPE_CURRENT_PREVIOUS_EVENT_BY_UTC TODO\r\n")); 
				i4_ret = _edb_dvb_process_previous_event_by_utc(h_edb_eng, pv_key_info,pz_event_info_len,pv_event_info,pe_cond);
                break;
            }
        case    EDB_KEY_TYPE_NOW_NEXT_EVENT_NUM_BY_UTC:
            {
#ifdef PARA_FROM_AP
                EDB_CH_TIME_FILTER_T*  & t_ch_time_filter;
                & t_size, sizeof(EDB_BUCKET_INFO_T);
                EDB_BUCKET_INFO_T*     & t_bucket_info;
#endif
                DBG_INFO(("_edb_dvb_process_pf_num_by_utc\r\n"));  
                i4_ret = _edb_dvb_process_pf_num_by_utc(h_edb_eng, pv_key_info,pz_event_info_len,pv_event_info,pe_cond);

                break;
            }

        case EDB_KEY_TYPE_EVENT_IDS_BY_RANGE:
            {
                DBG_INFO(("_edb_dvb_process_event_ids\r\n"));  
                i4_ret = _edb_dvb_process_event_ids(h_edb_eng, pv_key_info,pz_event_info_len,pv_event_info,pe_cond);
                break;
            }
        case EDB_KEY_TYPE_SINGLE_EVENT_BY_ID:
            DBG_INFO(("_edb_dvb_process_single_event\r\n"));  
            i4_ret = _edb_dvb_process_single_event(h_edb_eng, pv_key_info,pz_event_info_len,pv_event_info,pe_cond);
            break;

        case EDB_KEY_TYPE_SINGLE_POOL_SERIAL_EVENT_BY_ID:
            DBG_INFO(("_edb_dvb_process_single_pool_serial_event\r\n"));  
            i4_ret = _edb_dvb_process_single_pool_serial_event(h_edb_eng, pv_key_info,pz_event_info_len,pv_event_info,pe_cond);
            break;

        case EDB_KEY_TYPE_LINKAGE:
            DBG_INFO(("_edb_dvb_process_linkage\r\n"));  
            i4_ret = _edb_dvb_process_linkage(h_edb_eng, pv_key_info,pz_event_info_len,pv_event_info,pe_cond);
            break;
        case EDB_KEY_TYPE_DETAIL_LANGUAGE:
            DBG_INFO(("_edb_dvb_process_detail_language\r\n"));  
            i4_ret = _edb_dvb_process_detail_language(h_edb_eng, pv_key_info,pz_event_info_len,pv_event_info,pe_cond);
            break;
        default:
            DBG_INFO(("***********UNKNOW e_key_type=%d\r\n",e_key_type));  
            break;
    }

    if (i4_ret != EDBR_OK)
    {
        return i4_ret;
    }

    i4_ret = edb_dvb_get_eng_data_from_ctrl(h_edb_eng, (VOID**)&pt_eng_data);
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK || pt_eng_data == NULL),
            (EDBR_INTERNAL_ERROR),
            ((_EDB_INFO"Can not get engine data from client\r\n"))
            );
    if (pe_cond != NULL)
    {    
        (*pe_cond) = pt_eng_data->e_cond;
    }
    return i4_ret;

}


INT32 edb_dvb_set(
        HANDLE_T            h_edb_eng,
        EDB_COMMAND_T*      pt_set_cmd)
{
    INT32                       i4_ret              = 0;
    EDB_ENG_CTRL_ENTRY_T*       pt_eng_ctrl_entry   = NULL;
    DVB_ENG_DATA*               pt_eng_data         = NULL;

    EDB_CHECK_RET( 
            (h_edb_eng == NULL_HANDLE || pt_set_cmd == NULL),
            (EDBR_INV_ARG),
            ((_EDB_INFO"Invalid argument\r\n"))
            );

    i4_ret = edb_handle_get_ctrl_obj(h_edb_eng,&pt_eng_ctrl_entry);
    EDB_CHECK_RET( 
            (pt_eng_ctrl_entry == NULL),
            (EDBR_INV_ARG),
            ((_EDB_INFO"Can not get edb client object\r\n"))
            );

    pt_eng_data = (DVB_ENG_DATA*)pt_eng_ctrl_entry->pt_eng_data;

    i4_ret = _edb_set_edb_with_command(h_edb_eng,pt_eng_data,pt_set_cmd);
    return i4_ret;
}

INT32 edb_dvb_get (
        HANDLE_T            h_edb_eng,
        EDB_COMMAND_T*      pt_get_cmd)
{
    /*DBG_INFO((_EDB_INFO"EDB dvb engine config:%d\r\n",t_eng_cfg_flag));*/
    return EDBR_OK;

}




INT32 x_edb_dvb_eng_init(const EDB_DVB_ENG_CONFIG_T*   pt_config)
{
    INT32                   i4_ret            = 0;
    EDB_ENG_FCT_TBL_T       t_edb_eng_fct_tbl = {0};
    BOOL                    b_is_bond = FALSE;
    
    do
    {
        i4_ret = x_chip_spec_get_DVB_bond(&b_is_bond);
        if ((CHIPSPECR_OK == i4_ret && b_is_bond) && (CHIPSPECR_UNKNOWN != i4_ret))
        {
             break;
        }

        i4_ret = x_chip_spec_get_DMBT_bond(&b_is_bond);
        if ((CHIPSPECR_OK == i4_ret && b_is_bond) && (CHIPSPECR_UNKNOWN != i4_ret))
        {
             break;
        }

            return EDBR_CMD_NOT_SUPPORT;
    }    while (0);

    if (!edb_is_inited())
    {
        return EDBR_NOT_INIT;
    }

    edb_section_init(NULL);

    if (pt_config == NULL /*||
                            pt_config->ui2_num_event_objs == 0*/)
    {
        return EDBR_INV_ARG;
    }

    t_eng_cfg_flag     = pt_config->t_eng_cfg_flag;

    i4_ret = edb_hash_create(EDB_MAX_ENG_DATA, &(h_eng_data_hash));
    EDB_CHECK_RET(
            (h_eng_data_hash == NULL_HANDLE),
            (EDBR_INTERNAL_ERROR),
            (("Create DVB engine data fail\r\n"))
            );
    DBG_INFO((_EDB_INFO"Create DVB engine data list success\r\n"));




    /* Create message queue for DVB engine */
    i4_ret = x_msg_q_create( &h_dvb_msg_q,
                             (CHAR*)EDB_DVB_MSGQ_NAME,
                             sizeof( EDB_DVB_MSG_T ),
                             EDB_DVB_DEFAULT_NUM_OF_MSGS );
    if (i4_ret != OSR_OK)
    {
        if (i4_ret == OSR_EXIST)
        {
            return EDBR_DUP_BRDCST_TYPE;
        }

        return EDBR_INTERNAL_ERROR;
    }



    t_edb_eng_fct_tbl.pf_create_eng_obj         = edb_dvb_create_eng_obj;
    t_edb_eng_fct_tbl.pf_delete_eng_obj         = edb_dvb_delete_eng_obj;
    t_edb_eng_fct_tbl.pf_get_event_info_len     = edb_dvb_get_event_info_len;
    t_edb_eng_fct_tbl.pf_get_event_info         = edb_dvb_get_event_info;
    t_edb_eng_fct_tbl.pf_engine_task            = edb_dvb_engine_task;
    t_edb_eng_fct_tbl.pf_set                    = edb_dvb_set;
    t_edb_eng_fct_tbl.pf_get                    = edb_dvb_get;

    i4_ret = edb_reg_engine(EDB_DVB_BRDCST_GRP,
                            &t_edb_eng_fct_tbl,
                            0/*pt_config->ui2_num_event_objs*/);

    if (i4_ret != EDBR_OK)
    {
        x_msg_q_delete(h_dvb_msg_q);
    }

    /* Initialize EDB - DVB engine CLI component */
    i4_ret = edb_dvb_cli_init();

    if (i4_ret != EDBR_OK)
    {
        DBG_ABORT( DBG_MOD_EDB );
        return EDBR_INTERNAL_ERROR;
    }

    //Init fake event id$
    {
        UINT32  i = 0;
        for(i=0; i<MAX_EMPTY_CELL_SUPPORT; i++)
        {
            apt_pool_fake_event[i].ui2_event_id = 0xffff;
        }
    }


    edb_dvb_restart_fct = edb_dvb_restart;

    return i4_ret;
}


INT32 edb_dvb_restart(VOID)
{
    INT32               i4_ret      = EDBR_OK;
    EDB_DVB_MSG_T       t_msg       = {0};
    INT32               i4_retry    = 0x0;

    //Send msg switch thread context
    DBG_INFO((_EDB_INFO"EDB_DVB_MSG_POOL_FULL\r\n"));
    t_msg.e_msg_type            = EDB_DVB_MSG_POOL_FULL;

    while ( ++i4_retry <= 10 )
    {
        if (x_msg_q_send(h_dvb_msg_q,
                         &t_msg,
                         sizeof(EDB_DVB_MSG_T),
                         0x0 /* highest priority */) != OSR_OK)
        {
            /* Send message fail, wait 10 ms and retry for 10 times */
            x_thread_delay( 10 );
            continue;
        }
        edb_activate_eng_task(EDB_DVB_BRDCST_GRP);

        return i4_ret;
    }


    return i4_ret;
}

#ifdef CLI_SUPPORT

INT32 edb_dvb_cli_dump_single_event_by_id(
        HANDLE_T                h_edb_eng,
        UINT32                  ui4_channel_id,
        UINT16                  ui2_event_id 
        )
{
    INT32                   i4_ret          = EDBR_OK;
    EDB_EVENT_POOL_CH_T*    pt_channel      = NULL;
    HANDLE_T                h_event_hash    = NULL_HANDLE;
    EDB_POOL_EVENT_INFO_T*  pt_pool_event   = NULL;

    EDB_CHECK_RET( 
            (h_edb_eng == NULL_HANDLE),
            (EDBR_INV_ARG),
            (("Wrong argument\r\n"))
            );

    i4_ret = _edb_dvb_util_get_channel_by_edb_eng(h_edb_eng,ui4_channel_id, &pt_channel);
    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK || pt_channel == NULL),
            (EDBR_INTERNAL_ERROR),
            (("Get pool channel fail\r\n"))
            );

    h_event_hash = pt_channel->h_event_hash;
    EDB_CHECK_RET( 
            (h_event_hash == NULL_HANDLE),
            (EDBR_INTERNAL_ERROR),
            (("Event hash is null\r\n"))
            );


    i4_ret = edb_pool_get_event_obj(pt_channel, ui2_event_id, &pt_pool_event);
    if  (i4_ret != EDBR_OK || pt_pool_event == NULL)
    {
        //check is fake event
        i4_ret = _edb_dvb_get_fake_event(ui2_event_id, &pt_pool_event);
        EDB_CHECK_RET( 
                (i4_ret == EDBR_INFO_NOT_FOUND || pt_pool_event == NULL),
                (EDBR_INTERNAL_ERROR),
                (("Can not get event by id:%d\r\n",ui2_event_id))
                );
    }

   
    if (
            pt_pool_event != NULL
       )
    {    //print event start time and duration
        //x_dbg_stmt("ID=0x%04x\n\t",pt_pool_event->ui2_event_id);
        INT32 i;
        x_dbg_stmt("Event id=0x%04x name=%s\r\n",pt_pool_event->ui2_event_id,pt_pool_event->ps_event_title);
        DBG_STMT_TIME("Start_time",pt_pool_event->t_start_time); x_dbg_stmt("\t");
        DBG_STMT_TIME("End_time",pt_pool_event->t_start_time + pt_pool_event->t_duration); x_dbg_stmt("\t");
        if (pt_pool_event->ps_event_title != NULL)
        {
            x_dbg_stmt(" Title:%s \n",pt_pool_event->ps_event_title);
        }

        if (pt_pool_event->ps_event_detail != NULL)
        {
            x_dbg_stmt(" Detail:%s\r\n",pt_pool_event->ps_event_detail);
        }
        
        if(pt_pool_event->pt_event_extended_list!=NULL)
        {    
            x_dbg_stmt("extend discriptor num = %d  \n",pt_pool_event->pt_event_extended_list->ui1_event_extended_num);
            for(i=0;i<pt_pool_event->pt_event_extended_list->ui1_event_extended_num;i++)
            {

                x_dbg_stmt("extended discriptor[%d] : %s \n",i,pt_pool_event->pt_event_extended_list->pt_event_extended[i].ps_extended_text);

            }
            x_dbg_stmt("\r\n");
        }
        if(pt_pool_event->ps_event_guidance!=NULL)
        {
            x_dbg_stmt("event guidance length : %d \n",x_strlen(pt_pool_event->ps_event_guidance));
            x_dbg_stmt("ps_event_guidance : %s \n",pt_pool_event->ps_event_guidance);
        }
    }

    


    //print channel id event id

    return i4_ret;
}

static VOID _edb_dvb_free_all_eng_data( VOID* pv_obj )
{
    DVB_ENG_DATA*   dvb_eng_data = NULL;
    dvb_eng_data = (DVB_ENG_DATA*)pv_obj;
    if(dvb_eng_data != NULL)
    {
        //EDB_FREE(dvb_eng_data);
    }
}


INT32 edb_dvb_reset_eng_data_list(VOID)
{
    INT32           i4_ret = 0;
    edb_hash_delete(h_eng_data_hash, _edb_dvb_free_all_eng_data);

    i4_ret = edb_hash_create(EDB_MAX_ENG_DATA, &(h_eng_data_hash));
    EDB_CHECK_RET(
            (h_eng_data_hash == NULL_HANDLE),
            (EDBR_INTERNAL_ERROR),
            (("Create DVB engine data fail\r\n"))
            );
    DBG_INFO((_EDB_INFO"Create DVB engine data list success\r\n"));
    return i4_ret;
}


static INT32 _edb_dvb_dump_ch_info( 
        HANDLE_T          h_hash,
        UINT32            ui4_counter,
        VOID*             pv_obj,
        VOID*             pv_parse_tag )
{
#if 0
    INT32 i                = 0;
    INT32 j                = 0;
    UINT8 ui1_section_mask = 0;
    UINT8 ui1_section      = 0;
    UINT8 ui1_count        = 0;

#endif    
    EDB_EVENT_POOL_CH_T*    pt_ch_obj         = NULL;
    EDB_EVENT_POOL_CH_VER*  pt_ch_obj_ver     = NULL;
    EDB_POOL_EVENT_INFO_T*    pt_pool_event      = NULL;
#if 0
    INT32                   index             = 0;
    EDB_PF_PER_TBL_T*       pt_pf_tbl         = NULL;/*0x4E 0x4F*/
    EDB_S_PER_TBL_T*        pt_s_act_tbl      = NULL;/* 0x50 0x51*/
    EDB_S_PER_TBL_T*        pt_s_oth_tbl      = NULL;/* 0x60 0x61*/
#endif
    HANDLE_T                h_event_hash      = NULL_HANDLE;
    SIZE_T*                 pz_total_event    = NULL ;

    pt_ch_obj = (EDB_EVENT_POOL_CH_T*)pv_obj;
    EDB_CHECK_RET( 
            (pt_ch_obj == NULL),
            (EDBR_INTERNAL_ERROR),
            ((_EDB_INFO"Channel object is null\r\n"))
            );

    x_dbg_stmt("Channel info svl=0x%04x on=0x%04x ts=0x%04x svc=0x%04x(%d) channel_id=0x%08x ch_name:%s\r\n",
       pt_ch_obj->t_key.ui2_svl_id,
       pt_ch_obj->t_key.ui2_on_id,
       pt_ch_obj->t_key.ui2_ts_id,
       pt_ch_obj->t_key.ui2_svc_id,
       pt_ch_obj->t_key.ui2_svc_id,
       pt_ch_obj->ui4_channel_id,
       pt_ch_obj->ac_name
    );

    
    pt_pool_event = pt_ch_obj->pt_present_event;
    x_dbg_stmt("current:  ");
    if (
            pt_pool_event != NULL
       )
    {    //print event start time and duration
        x_dbg_stmt("ID=0x%04x\n\t",pt_pool_event->ui2_event_id);
        DBG_STMT_TIME("Start_time",pt_pool_event->t_start_time); x_dbg_stmt("\t");
        DBG_STMT_TIME("End_time",pt_pool_event->t_start_time + pt_pool_event->t_duration); x_dbg_stmt("\t");
        if (pt_pool_event->ps_event_title != NULL)
        {
            x_dbg_stmt(" Title:%s",pt_pool_event->ps_event_title);
        }
        x_dbg_stmt("\r\n");

    }
    else
    {
        x_dbg_stmt("NULL\r\n");
    }
    
    pt_pool_event = pt_ch_obj->pt_following_event;
    x_dbg_stmt("next:  ");
    if (
            pt_pool_event != NULL
       )
    {    //print event start time and duration
        x_dbg_stmt("ID=0x%04x\n\t",pt_pool_event->ui2_event_id);
        DBG_STMT_TIME("Start_time",pt_pool_event->t_start_time); x_dbg_stmt("\t");
        DBG_STMT_TIME("End_time",pt_pool_event->t_start_time + pt_pool_event->t_duration); x_dbg_stmt("\t");
        if (pt_pool_event->ps_event_title != NULL)
        {
            x_dbg_stmt(" Title:%s",pt_pool_event->ps_event_title);
        }
        x_dbg_stmt("\r\n");

    }
    else
    {
        x_dbg_stmt("NULL\r\n");
    }

    pt_ch_obj_ver = (EDB_EVENT_POOL_CH_VER*)pt_ch_obj->pv_data;
    EDB_CHECK_RET( 
            (pt_ch_obj_ver == NULL),
            (EDBR_INTERNAL_ERROR),
            ((_EDB_INFO"Channel object version info is null\r\n"))
            );

    if (pv_parse_tag != NULL)
    {
        pz_total_event = (SIZE_T*)pv_parse_tag;
    }

#if 0
    {
        pt_pf_tbl       = &(pt_ch_obj_ver->t_pf_tbl[0]);
        pt_s_act_tbl    = &(pt_ch_obj_ver->t_s_act_tbl[0]);
        pt_s_oth_tbl    = &(pt_ch_obj_ver->t_s_oth_tbl[0]);

        x_dbg_stmt("ON:0x%04x TS:0x%04x SVC:0x%04x \r\n",
                   pt_ch_obj->t_key.ui2_on_id,
                   pt_ch_obj->t_key.ui2_ts_id,
                   pt_ch_obj->t_key.ui2_svc_id
                  );

        if (pt_ch_obj->pt_present_event != NULL)
        {
            x_dbg_stmt("Present event id=0x%04x\r\n",
                       pt_ch_obj->pt_present_event->ui2_event_id
                      );
        }

        if (pt_ch_obj->pt_following_event != NULL)
        {
            x_dbg_stmt("following event id=0x%04x\r\n",
                       pt_ch_obj->pt_following_event->ui2_event_id
                      );
        }

        for (index = 0;index<2;index++)/*PF*/
        {
            x_dbg_stmt("[0x%02x]%08x ",(0x4E + index),pt_pf_tbl[index].aui1_sec_mask);
        }
        x_dbg_stmt( "\r\n");

        for (index = 0;index<EDB_MAX_PER_CH_TBL;index++)/*EIT S actual & other*/
        {
            x_dbg_stmt("[0x%02x]",0x50 + index);
            ui1_count        = 0;
            for (i=0;i<32;i++)
            {
                ui1_section_mask =  pt_s_act_tbl[index].aui1_sec_mask[i];
                ui1_section      = 0;

                for (j=0;j<8;j++)
                {
                    if (ui1_section_mask & MAKE_BIT_MASK_8(j))
                    {
                        ui1_section = (i*8) + j;

                        x_dbg_stmt("%3d ",ui1_section);

                        ui1_count++;
                    }
                }
            }
            x_dbg_stmt( "count:%d\r\n",ui1_count);

            x_dbg_stmt("[0x%02x]",0x50 + 0x10  +index);
            ui1_count        = 0;
            for (i=0;i<32;i++)
            {
                ui1_section_mask =  pt_s_oth_tbl[index].aui1_sec_mask[i];
                ui1_section      = 0;

                for (j=0;j<8;j++)
                {
                    if (ui1_section_mask & MAKE_BIT_MASK_8(j))
                    {
                        ui1_section = (i*8) + j;

                        x_dbg_stmt("%3d ",ui1_section);

                        ui1_count++;
                    }
                }
            }
            x_dbg_stmt( "count:%d\r\n",ui1_count);

        }
    }
#endif

    h_event_hash = pt_ch_obj->h_event_hash;
    if(h_event_hash != NULL_HANDLE)
    {
        SIZE_T  z_event_num = 0;
        edb_hash_get_size(h_event_hash, &z_event_num);
        x_dbg_stmt("Event total number:%d\r\n",z_event_num);
        if(pz_total_event!=NULL)
        {
            *pz_total_event += z_event_num;
        }
    }
    x_dbg_stmt("\r\n");


    return EDBR_OK;
}

INT32 edb_dvb_dump(VOID*    pt_obj)
{
    SIZE_T                  z_size      = 0x0;
    INT32                   i4_ret      = 0x0;
    DVB_ENG_DATA*           pt_eng_data = NULL;

    EDB_CHECK_RET( 
            (pt_obj == NULL),
            (EDBR_INV_ARG),
            ((_EDB_INFO"Invalid argument\r\n"))
            );

    pt_eng_data = (DVB_ENG_DATA*)pt_obj;

    if (pt_eng_data->h_edb_inter_ch_locate != NULL_HANDLE)
    {
        edb_hash_get_size(pt_eng_data->h_edb_inter_ch_locate, &z_size);
    }

    EDB_DVB_LOG((_EDB_INFO"EDB DVB INFO START=========================================\r\n"));
    EDB_DVB_LOG((_EDB_INFO"ui2_svl_id                     = 0x%x \r\n",pt_eng_data->ui2_svl_id                   ));
    EDB_DVB_LOG((_EDB_INFO"h_svl                          = 0x%x \r\n",pt_eng_data->h_svl                        ));
    EDB_DVB_LOG((_EDB_INFO"ac_tuner_name                  = %s   \r\n",pt_eng_data->ac_tuner_name                ));
    EDB_DVB_LOG((_EDB_INFO"h_root                         = 0x%x \r\n",pt_eng_data->h_root                       ));
    EDB_DVB_LOG((_EDB_INFO"ah_eit                         = 0x%x \r\n",pt_eng_data->ah_eit                       ));

    EDB_DVB_LOG((_EDB_INFO"fg_pf_only                     = %d \r\n",pt_eng_data->t_edb_eng_state.fg_pf_only                      ));
    EDB_DVB_LOG((_EDB_INFO"fg_actual_only                 = %d \r\n",pt_eng_data->t_edb_eng_state.fg_actual_only                  ));
    EDB_DVB_LOG((_EDB_INFO"ui1_max_days                   = %d \r\n",pt_eng_data->t_edb_eng_state.ui1_max_days                    ));
    EDB_DVB_LOG((_EDB_INFO"t_pref_lang                    = %d \r\n",pt_eng_data->t_edb_eng_state.t_pref_lang.ui1_num             ));
    {
        INT32   i =0;
        for(i=0; ((i<pt_eng_data->t_edb_eng_state.t_pref_lang.ui1_num)&&(i<EDB_MAX_NUM_PREF_LANG));i++)
        {
            EDB_DVB_LOG((_EDB_INFO"t_pref_lang[%d]                 = %s \r\n",i,pt_eng_data->t_edb_eng_state.t_pref_lang.t_lang[i]));
        }
    }
    EDB_DVB_LOG((_EDB_INFO"t_country                      = %s \r\n",pt_eng_data->t_edb_eng_state.t_country                       ));
    EDB_DVB_LOG((_EDB_INFO"t_operator                     = %d \r\n",pt_eng_data->t_edb_eng_state.t_operator                      ));
    EDB_DVB_LOG((_EDB_INFO"fg_enable                      = %d \r\n",pt_eng_data->t_edb_eng_state.fg_enable                       ));
    EDB_DVB_LOG((_EDB_INFO"t_active_win                   = 0x%x \r\n",&(pt_eng_data->t_edb_eng_state.t_active_win)               ));
    {
        INT32   i = 0;
        DBG_INFO((_EDB_INFO"t_active_win.channel number=%d \r\n",pt_eng_data->t_edb_eng_state.t_active_win.ui1_num_channel));        
        for (i=0;i<pt_eng_data->t_edb_eng_state.t_active_win.ui1_num_channel;i++)
        {
            EDB_DVB_LOG((_EDB_INFO"t_active_win.channel id=0x%08x \r\n",pt_eng_data->t_edb_eng_state.t_active_win.at_channel_id[i].ui4_channel_id));        
        }
    }
    DBG_STMT_TIME(_EDB_INFO"t_active_win.t_start_time",pt_eng_data->t_edb_eng_state.t_active_win.t_start_time); 
    DBG_STMT_TIME(_EDB_INFO"t_active_win.t_end_time",pt_eng_data->t_edb_eng_state.t_active_win.t_start_time + pt_eng_data->t_edb_eng_state.t_active_win.t_duration);

    EDB_DVB_LOG((_EDB_INFO"ui4_event_min_secs             = %d \r\n",pt_eng_data->t_edb_eng_state.ui4_event_min_secs              ));
    EDB_DVB_LOG((_EDB_INFO"fg_fake_event_insertion_enable = %d \r\n",pt_eng_data->t_edb_eng_state.fg_fake_event_insertion_enable  ));
    EDB_DVB_LOG((_EDB_INFO"fake_event_min_secs            = %d \r\n",pt_eng_data->t_edb_eng_state.fake_event_min_secs             ));
    EDB_DVB_LOG((_EDB_INFO"fg_time_conflict_allow         = %d \r\n",pt_eng_data->t_edb_eng_state.fg_time_conflict_allow          ));
    EDB_DVB_LOG((_EDB_INFO"s_event_detail_separator       = %s \r\n",pt_eng_data->t_edb_eng_state.s_event_detail_separator        ));
    EDB_DVB_LOG((_EDB_INFO"ui4_current_channel_id         = 0x%x \r\n",pt_eng_data->t_edb_eng_state.ui4_current_channel_id        ));
    EDB_DVB_LOG((_EDB_INFO"h_edb_inter_ch_locate size     = %d \r\n",z_size                                                       ));
    EDB_DVB_LOG((_EDB_INFO"EDB DVB INFO END========================================\r\n\r\n"));



    EDB_DVB_LOG((_EDB_INFO"EDB DVB Pool info START=========================================\r\n"));
    {
        EDB_EVENT_POOL_T*       pt_edb_event_pool = NULL;
        HANDLE_T                h_ch_hash         = NULL_HANDLE;/*hash of EDB_EVENT_POOL_CH_T */
        SIZE_T                  z_ch_hash_size    = 0;
        SIZE_T                  z_total_event     = 0 ;

        pt_edb_event_pool = &(pt_eng_data->pt_edb->t_edb_event_pool);
        if (pt_edb_event_pool == NULL)
        {
            return i4_ret;
        }

        h_ch_hash = pt_edb_event_pool->h_ch_hash;
        EDB_DVB_LOG((_EDB_INFO"ui2_total_channels        = %d   \r\n",pt_edb_event_pool->ui2_total_channels               ));
        EDB_DVB_LOG((_EDB_INFO"ui4_last_time_stamp       = %d   \r\n",pt_edb_event_pool->ui4_last_time_stamp              ));
        EDB_DVB_LOG((_EDB_INFO"h_ch_hash                 = 0x%x \r\n",&(pt_edb_event_pool->h_ch_hash)                     ));

        if (h_ch_hash != NULL_HANDLE)
        {
            i4_ret = edb_hash_get_size(h_ch_hash,&z_ch_hash_size);
            if (i4_ret != EDBR_OK)
            {
                return i4_ret;
            }
        }
        EDB_DVB_LOG((_EDB_INFO"h_ch_hash size            = 0x%x \r\n",z_ch_hash_size                                     ));

        i4_ret =  edb_hash_parse(h_ch_hash, _edb_dvb_dump_ch_info, (VOID*)(&z_total_event));
        EDB_DVB_LOG((_EDB_INFO"total events              = %d \r\n",z_total_event                                        ));
    }
    EDB_DVB_LOG((_EDB_INFO"EDB DVB Pool info END===========================================\r\n"));
    return i4_ret;

#if 0
    if (_h_edb_inter_ch_locate != NULL_HANDLE)
    {
        hash_parse(_h_edb_inter_ch_locate, _edb_dvb_iterator_ch_locate, NULL);
    }
#endif

}
#endif

