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

/*-----------------------------------------------------------------------------
  include files
  ----------------------------------------------------------------------------*/
#include "inc/x_common.h"
#include "os/inc/x_os.h"
#include "handle/u_handle_grp.h"
#include "hash/hash_api.h"
#include "handle/u_handle.h"

#include "dbg/x_dbg.h"
#include "inc/x_sys_name.h"
#include "inc/u_device_type.h"
#include "cli/x_cli.h"
#include "dt/x_dt.h"
#include "x_edb.h"
#include "edb_slctr.h"
#include "edb_eng.h"
#include "edb_pool.h"
#include "edb/edb_hash_api.h"
#include "edb/edb_dvb_eng/edb_dvb_eng.h"
#include "util/x_sort_search.h"

#ifdef  CLI_SUPPORT
static INT32 _edb_cli_register(INT32 i4_argc, const CHAR **pps_argv);
static INT32 _edb_cli_create(INT32 i4_argc, const CHAR **pps_argv);
static INT32 _edb_cli_config(INT32 i4_argc, const CHAR **pps_argv);
static INT32 _edb_cli_get(INT32 i4_argc, const CHAR **pps_argv);
static INT32 _edb_cli_set(INT32 i4_argc, const CHAR **pps_argv);

static INT32 _edb_cli_start(INT32 i4_argc, const CHAR **pps_argv);
static INT32 _edb_cli_stop(INT32 i4_argc, const CHAR **pps_argv);

static INT32 _edb_cli_delete(INT32 i4_argc, const CHAR **pps_argv);
static INT32 _edb_cli_edb_dump(INT32 i4_argc, const CHAR **pps_argv);


static INT32 _edb_cli_get_dbg_level(INT32 i4_argc,const CHAR** pps_argv);
static INT32 _edb_cli_set_dbg_level(INT32 i4_argc,const CHAR** pps_argv);
#if 0
static INT32 _edb_cli_delete_all(INT32 i4_argc, const CHAR **pps_argv);
#endif
static INT32 _edb_cli_dvb_msg(INT32 i4_argc, const CHAR **pps_argv);
static INT32 _edb_cli_memory_overhead(INT32 i4_argc, const CHAR **pps_argv);
static INT32 _edb_cli_fake_event     (INT32 i4_argc, const CHAR **pps_argv);


static INT32 _edb_cli_edb_dump_event(INT32 i4_argc, const CHAR **pps_argv);
static INT32 _edb_cli_dump_pool_event(EDB_EVENT_POOL_CH_T* pt_pool_ch );

extern SIZE_T  os_mem_get_mem_ptr_size (const VOID*  pv_mem);
#ifdef DBG_INFO
#undef DBG_INFO
#define DBG_INFO(x) x_dbg_stmt x
#endif

static CHAR      s_time_log[100] = {0};
static CHAR*    edb_dvb_print_local_time(CHAR* s_pre,TIME_T t_time)
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

VOID _edb_cli_nfy_fct(
        HANDLE_T                h_edb,
        EDB_NFY_REASON_T        e_reason,
        VOID*                   pv_nfy_tag,
        UINT32                  ui4_data1,
        UINT32                  ui4_data2);



static CLI_EXEC_T at_edb_cli_tbl[] = 
{

    {"reg",          "register"         ,_edb_cli_register,           NULL,    "_edb_cli_register",              CLI_SUPERVISOR},
    {"create",       "create"           ,_edb_cli_create,             NULL,    "_edb_cli_create",                CLI_SUPERVISOR},
    {"config",       "config"           ,_edb_cli_config,             NULL,    "_edb_cli_config",                CLI_SUPERVISOR},
    {"start",        "start"            ,_edb_cli_start,              NULL,    "_edb_cli_start",                 CLI_SUPERVISOR},
    {"get",          "get"              ,_edb_cli_get,                NULL,    "_edb_cli_get",                   CLI_SUPERVISOR},
    {"set",          "set"              ,_edb_cli_set,                NULL,    "_edb_cli_set",                   CLI_SUPERVISOR},
    {"stop",         "stop"             ,_edb_cli_stop,               NULL,    "_edb_cli_stop",                  CLI_SUPERVISOR},
    {"delete",       "delete"           ,_edb_cli_delete,             NULL,    "_edb_cli_delete",                CLI_SUPERVISOR},
#if 0
    {"delete_all",   "delete_all"       ,_edb_cli_delete_all,         NULL,    "_edb_cli_delete_all",            CLI_SUPERVISOR},
#endif    
    {"ed",           "edb_dump"         ,_edb_cli_edb_dump,           NULL,    "_edb_cli_edb_dump",              CLI_SUPERVISOR},
    {"edm",          "dvb_msg"          ,_edb_cli_dvb_msg,            NULL,    "_edb_cli_dvb_msg",               CLI_SUPERVISOR},        
    {"de",           "dump_event"       ,_edb_cli_edb_dump_event,     NULL,    "_edb_cli_edb_dump_event",        CLI_GUEST},        
    {"mo",           "memory_overhead"  ,_edb_cli_memory_overhead ,   NULL,    "_edb_cli_memory_overhead",       CLI_SUPERVISOR},        
    {"efe",          "insert fake event",_edb_cli_fake_event,         NULL,    "_edb_cli_fake_event",            CLI_SUPERVISOR},        

    {CLI_GET_DBG_LVL_STR,   NULL,       _edb_cli_get_dbg_level,      NULL,    CLI_GET_DBG_LVL_HELP_STR,         CLI_SUPERVISOR},
    {CLI_SET_DBG_LVL_STR,   NULL,       _edb_cli_set_dbg_level,      NULL,    CLI_SET_DBG_LVL_HELP_STR,         CLI_SUPERVISOR},
    /* Last entry is used as an ending indicator */
	END_OF_CLI_CMD_TBL,
    END_OF_CLI_CMD_TBL
};

static CLI_EXEC_T at_edb_root_cmd_tbl[] =
{
    {"edb",     NULL,        NULL,       at_edb_cli_tbl,      "EDB commands",     CLI_GUEST},
    END_OF_CLI_CMD_TBL
};

static const SIZE_T z_cmd_tbl    = sizeof(at_edb_cli_tbl)/sizeof(CLI_EXEC_T)-1;
static SIZE_T       z_last_entry = 15;

/*-----------------------------------------------------------------------------
  macros, defines, typedefs, enums
  ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
  data declarations
  ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
  functions declaraions
  ----------------------------------------------------------------------------*/

extern INT32 edb_dvb_dump(VOID*    pt_obj);
extern INT32 edb_dvb_reset_eng_data_list(VOID);
extern HANDLE_T edb_dvb_section_get_msg_q_hdl (VOID);
static HANDLE_T                h_edb_client = NULL_HANDLE;


static INT32 _edb_cli_register(INT32 i4_argc, const CHAR **pps_argv)
{
    INT32                   i4_ret       = 0;
    EDB_COMMAND_T*          pt_cmd       = NULL;
    UINT16                  ui2_idx      = 0;
    HANDLE_T                h_edb_client_temp = NULL_HANDLE;
    EDB_ENG_OPEN_DATA_T     t_eng_open_data = {0};

    /*Register DVB [SN_TUNER_TER_DIG_0]*/
    {
        edb_eng_list_lock();
        ui2_idx      = 0;
        pt_cmd = EDB_OS_ALLOC(sizeof(EDB_COMMAND_T) * 2);
        if (pt_cmd == NULL)
        {
            return CLIR_CMD_EXEC_ERROR;
        }
        x_memset(pt_cmd,0x0,sizeof(EDB_COMMAND_T) * 2);

        t_eng_open_data.e_brdcst_type = BRDCST_TYPE_DVB;
        t_eng_open_data.ui2_svl_id    = 1;            
        t_eng_open_data.ps_tuner_name = SN_TUNER_TER_DIG_0;        

        pt_cmd[ui2_idx].e_code = EDB_CMD_OPEN_DATA;
        pt_cmd[ui2_idx].u.pt_eng_open_data = &t_eng_open_data;
        ui2_idx++;

        pt_cmd[ui2_idx].e_code = EDB_CMD_END;
        i4_ret = x_edb_create(EDB_MAIN, pt_cmd, NULL,NULL,&h_edb_client_temp);

        EDB_CHECK_GOTO( 
                (i4_ret != EDBR_OK || h_edb_client_temp == NULL_HANDLE),
                FREE_SOURCE,
                (("x_edb_create error name=%s,tuner=%s\r\n",EDB_MAIN,SN_TUNER_TER_DIG_0))
                );
        DBG_INFO((_EDB_INFO"EDB client create successful handle=0x%x\r\n",h_edb_client_temp));
        EDB_OS_FREE(pt_cmd);
        x_edb_destroy(h_edb_client_temp);
        edb_eng_list_unlock();
    }

    {
        ui2_idx      = 0;
        pt_cmd = EDB_OS_ALLOC(sizeof(EDB_COMMAND_T) * 2);
        if (pt_cmd == NULL)
        {
            return CLIR_CMD_EXEC_ERROR;
        }
        x_memset(pt_cmd,0x0,sizeof(EDB_COMMAND_T) * 2);

        edb_eng_list_lock();

        pt_cmd[ui2_idx].e_code = EDB_CMD_OPEN_DATA;
        t_eng_open_data.e_brdcst_type = BRDCST_TYPE_DVB;
        t_eng_open_data.ui2_svl_id    = 2;            
        t_eng_open_data.ps_tuner_name = SN_TUNER_CAB_DIG_0;        
        pt_cmd[ui2_idx].u.pt_eng_open_data = &t_eng_open_data;
        ui2_idx++;

        pt_cmd[ui2_idx].e_code = EDB_CMD_END;
        i4_ret = x_edb_create(EDB_MAIN, pt_cmd, NULL,NULL,&h_edb_client_temp);

        EDB_CHECK_GOTO( 
                (i4_ret != EDBR_OK || h_edb_client_temp == NULL_HANDLE),
                FREE_SOURCE,
                (("x_edb_create error name=%s,tuner=%s\r\n",EDB_MAIN,SN_TUNER_CAB_DIG_0))
                );
        DBG_INFO((_EDB_INFO"EDB client create successful handle=0x%x\r\n",h_edb_client_temp));
        EDB_OS_FREE(pt_cmd);
        x_edb_destroy(h_edb_client_temp);
        edb_eng_list_unlock();
    }

    return i4_ret;

FREE_SOURCE:
    if (pt_cmd!= NULL) {EDB_OS_FREE(pt_cmd);}
    edb_eng_list_unlock();
    return CLIR_CMD_EXEC_ERROR;
}




static INT32 _edb_cli_create(INT32 i4_argc, const CHAR **pps_argv)
{
    INT32                   i4_ret       = 0;
    EDB_COMMAND_T*          pt_cmd       = NULL;
    UINT16                  ui2_idx      = 0;
    EDB_ENG_OPEN_DATA_T     t_eng_open_data = {0};

    
    pt_cmd = EDB_OS_ALLOC(sizeof(EDB_COMMAND_T) * 2);
    if (pt_cmd == NULL)
    {
        return CLIR_CMD_EXEC_ERROR;
    }
    x_memset(pt_cmd,0x0,sizeof(EDB_COMMAND_T) * 2);

    edb_eng_list_lock();

    pt_cmd[ui2_idx].e_code = EDB_CMD_OPEN_DATA;
    t_eng_open_data.e_brdcst_type = BRDCST_TYPE_DVB;
    t_eng_open_data.ui2_svl_id    = 1;            
    t_eng_open_data.ps_tuner_name = SN_TUNER_TER_DIG_0;  
    pt_cmd[ui2_idx].u.pt_eng_open_data = &t_eng_open_data;
    ui2_idx++;

    pt_cmd[ui2_idx].e_code = EDB_CMD_END;
    i4_ret = x_edb_create(EDB_MAIN, pt_cmd, NULL,_edb_cli_nfy_fct,&h_edb_client);

    EDB_CHECK_GOTO( 
            (i4_ret != EDBR_OK || h_edb_client == NULL_HANDLE),
            FREE_SOURCE,
            (("x_edb_create error name=%s,tuner=%s\r\n",EDB_MAIN,SN_TUNER_TER_DIG_0))
            );
    DBG_INFO((_EDB_INFO"EDB client create successful handle=0x%x\r\n",h_edb_client));
    EDB_OS_FREE(pt_cmd);

    edb_eng_list_unlock();
    return i4_ret;

FREE_SOURCE:
    if (pt_cmd!= NULL) {EDB_OS_FREE(pt_cmd);}
    edb_eng_list_unlock();
    return CLIR_CMD_EXEC_ERROR;
}

static INT32 _edb_cli_config(INT32 i4_argc, const CHAR **pps_argv)
{
    INT32                   i4_ret       = 0;
    EDB_COMMAND_T*          pt_cmd       = NULL;
    EDB_CACHE_ACTIVE_WIN_T*       pt_win       = NULL;
    UINT16                  ui2_idx      = 0;


    pt_cmd = EDB_OS_ALLOC(sizeof(EDB_COMMAND_T) * 13);
    if (pt_cmd == NULL)
    {
        return CLIR_CMD_EXEC_ERROR;
    }

    edb_eng_list_lock();
    ui2_idx = 0;

    pt_cmd[ui2_idx].e_code      = EDB_CMD_CFG_PF_ONLY;
    pt_cmd[ui2_idx].u.b_bool    = FALSE;
    ui2_idx++;

    pt_cmd[ui2_idx].e_code      = EDB_CMD_CFG_ACTUAL_ONLY;
    pt_cmd[ui2_idx].u.b_bool    = FALSE;
    ui2_idx++;

    pt_cmd[ui2_idx].e_code      = EDB_CMD_CFG_MAX_DAYS;
    pt_cmd[ui2_idx].u.ui2_number= 8;
    ui2_idx++;

    pt_cmd[ui2_idx].e_code      = EDB_CMD_CFG_PREF_LANG;
    {
        EDB_PREF_LANG_T         t_pref_lang =
        {
            3,
            {
                "eng\0","gla\0","wel\0"
            }
        };
        pt_cmd[ui2_idx].u.pt_pref_lang   = &t_pref_lang;
    }
    ui2_idx++;


    pt_cmd[ui2_idx].e_code              = EDB_CMD_CFG_COUNTRY_CODE;
    x_strncpy(pt_cmd[ui2_idx].u.t_country_code,"GBR",ISO_3166_COUNT_LEN);
    ui2_idx++;




    pt_cmd[ui2_idx].e_code              = EDB_CMD_CFG_ACTIVE_WIN;
    {
        UINT32 i =0 ;
        UINT32  ui4_channel_ids[6] = 
        {
            262529 ,
            524929 ,
            787329 ,
            1049729,
            1312129 ,           
            1574529
        };
        pt_win = EDB_OS_ALLOC(sizeof(EDB_CACHE_ACTIVE_WIN_T));

        if ((pt_cmd == NULL)||(pt_win==NULL))
        {
            edb_eng_list_unlock();
            return CLIR_CMD_EXEC_ERROR;
        }
        x_memset(pt_win,0x0,sizeof(EDB_CACHE_ACTIVE_WIN_T));
        pt_win->ui1_num_channel = 6;
        for (i=0;i<6;i++)
        {   
            pt_win->at_channel_id[i].ui2_svl_id = 1;
            pt_win->at_channel_id[i].ui4_channel_id= ui4_channel_ids[i];
            pt_win->at_channel_id[i].e_brdcst_type = BRDCST_TYPE_DVB;
        }
        pt_win->t_start_time = x_dt_get_brdcst_utc(NULL,NULL);
        pt_win->t_duration   = (TIME_T)(3600*24);/* 1 day*/
        pt_cmd[ui2_idx].u.pt_active_win = pt_win;

        ui2_idx++;
    }


    pt_cmd[ui2_idx].e_code              = EDB_CMD_CFG_EVENT_MIN_SECS;
    pt_cmd[ui2_idx].u.ui2_number        = 60;
    ui2_idx++;

    pt_cmd[ui2_idx].e_code              = EDB_CMD_CFG_FAKE_EVENT_INSERTION_ENABLE;/*???*/
    pt_cmd[ui2_idx].u.b_bool            = FALSE;
    ui2_idx++;

    pt_cmd[ui2_idx].e_code              = EDB_CMD_CFG_FAKE_EVENT_MIN_SECS;
    pt_cmd[ui2_idx].u.ui2_number        = 0;
    ui2_idx++;

    pt_cmd[ui2_idx].e_code              = EDB_CMD_CFG_TIME_CONFLICT_ALLOW;
    pt_cmd[ui2_idx].u.b_bool            = TRUE;
    ui2_idx++;

    pt_cmd[ui2_idx].e_code              = EDB_CMD_CFG_EVENT_DETAIL_SEPARATOR;
    x_strncpy(pt_cmd[ui2_idx].u.s_separator,"\r\n",4);
    ui2_idx++;

    pt_cmd[ui2_idx].e_code              = EDB_CMD_END;

    DBG_INFO((_EDB_INFO"Command size=%d\r\n",sizeof(pt_cmd)));

    if (h_edb_client != NULL_HANDLE)
    {
        i4_ret = x_edb_set(h_edb_client, pt_cmd);
    }

    EDB_OS_FREE(pt_win);
    EDB_OS_FREE(pt_cmd);
    edb_eng_list_unlock();
    return i4_ret;
}

static INT32 _edb_cli_start(INT32 i4_argc, const CHAR **pps_argv)
{
    INT32                   i4_ret       = 0;
    EDB_COMMAND_T*          pt_cmd       = NULL;
    UINT16                  ui2_idx      = 0;
    HANDLE_T                h_client     = NULL_HANDLE;


    if ((i4_argc == 2) || (pps_argv != NULL))
    {
        h_client = (HANDLE_T) x_strtoull(pps_argv[1], NULL, 0);
    }    




    {
        pt_cmd = EDB_OS_ALLOC(sizeof(EDB_COMMAND_T) * 2);
        if (pt_cmd == NULL)
        {
            return CLIR_CMD_EXEC_ERROR;
        }

        edb_eng_list_lock();
        ui2_idx = 0;

        pt_cmd[ui2_idx].e_code          = EDB_CMD_DO_CURRENT_SERVICE;
        pt_cmd[ui2_idx].u.ui2_number    = (UINT16)262529;
        ui2_idx++;

        pt_cmd[ui2_idx].e_code          = EDB_CMD_END;
        x_edb_set((h_client== NULL_HANDLE)?h_edb_client:h_client, pt_cmd);

        EDB_OS_FREE(pt_cmd);
        edb_eng_list_unlock();
    }





    pt_cmd = EDB_OS_ALLOC(sizeof(EDB_COMMAND_T) * 2);
    if (pt_cmd == NULL)
    {
        return CLIR_CMD_EXEC_ERROR;
    }
    edb_eng_list_lock();
    ui2_idx = 0;

    pt_cmd[ui2_idx].e_code      = EDB_CMD_DO_ENABLE;
    pt_cmd[ui2_idx].u.b_bool    = TRUE;
    ui2_idx++;

    x_edb_set((h_client== NULL_HANDLE)?h_edb_client:h_client, pt_cmd);

    if (pt_cmd != NULL) {EDB_OS_FREE(pt_cmd);}
    edb_eng_list_unlock();
    return i4_ret;
}

static INT32 _edb_cli_get(INT32 i4_argc, const CHAR **pps_argv)
{
    INT32                   i4_ret       = 0;
    SIZE_T               t_size;
    EDB_CH_EVT_ID_T      t_ch_evt_id;
    SIZE_T               z_len = 0;
	CHAR* 			     ps_event_detail=NULL;

    x_memset(&t_ch_evt_id, 0, sizeof(t_ch_evt_id));


    if ((i4_argc == 4) || (pps_argv != NULL))
    {     
    /* Key info prepare */
	    t_ch_evt_id.e_brdcst_type     = BRDCST_TYPE_DVB;
	    t_ch_evt_id.ui2_svl_id        = (UINT16) x_strtoull(pps_argv[1], NULL, 0);
	    t_ch_evt_id.ui4_channel_id    = (UINT32) x_strtoull(pps_argv[2], NULL, 0);
	    t_ch_evt_id.ui2_event_id      = (UINT16) x_strtoull(pps_argv[3], NULL, 0);
	}    
 	else
 	{
 		x_dbg_stmt("Usage : first you need create , then get [svl_id] [channel_id] [event_id] \n");
		
 	}


    
	
	/* detail */
	z_len = 0;
	i4_ret = x_edb_get_info_len (h_edb_client,
								 EDB_KEY_TYPE_EVENT_DETAIL_BY_ID,
								 & t_ch_evt_id,
								 & z_len);	 /* will include \0 */
	if (i4_ret < EDBR_OK || z_len == 0)
	{
		x_dbg_stmt("get detail fail");
	}
	else
	{
		/* the z_len has already include \0 */
	
		ps_event_detail = x_mem_alloc(z_len+1);
		if(ps_event_detail == NULL)
		{
			x_dbg_stmt("malloc fail");
			return i4_ret;
		}
		x_memset(ps_event_detail, 0, z_len+1);
	
		t_size = z_len;
		i4_ret = x_edb_get_info (h_edb_client,
								 EDB_KEY_TYPE_EVENT_DETAIL_BY_ID,
								 & t_ch_evt_id,
								 & t_size,	  /* will not include \0 */
								 ps_event_detail);
		if (i4_ret < EDBR_OK)
		{
			/* it's OK if no detail */			  
			x_dbg_stmt("get detail fail");
		}
		else
		{
			/* OK */
			x_dbg_stmt(" Detail:%s\r\n",ps_event_detail);

		}
	}

    return i4_ret;
}

static INT32 _edb_cli_set(INT32 i4_argc, const CHAR **pps_argv)
{
    INT32                   i4_ret       = 0;  
    UINT16               ui2_cmd_idx = 0;
    EDB_COMMAND_T        at_cmd[10];
    UINT8                ui1_temp = 0;

    if ((i4_argc == 2) && (pps_argv != NULL))
    {     
	    ui1_temp = (UINT8) x_strtoull(pps_argv[1], NULL, 0);
	}    
 	else
 	{
 		x_dbg_stmt("Usage : first you need create , then set [1/2/3] \n");
        x_dbg_stmt(" 1 : recreate event pool \n");
        x_dbg_stmt(" 2 : disable edb cache \n");
        x_dbg_stmt(" 3 : enable edb cache \n");
      /*  x_dbg_stmt(" 4 : write event pool to flash \n");
        x_dbg_stmt(" 5 : read event information from flash to DDR \n");*/
        x_dbg_stmt(" 6 : config asu mode \n");
        x_dbg_stmt(" 7 : flash events \n");
        x_dbg_stmt(" 8 : sync events \n");		
        x_dbg_stmt(" 9 : set risk tv true --------config edb 6M memory \n");	
        x_dbg_stmt("10 : set risk tv false --------config edb 12M memory \n");	
 	}

    x_memset (at_cmd, 0, sizeof(at_cmd));

    if(ui1_temp == 1)
    {
        /* flush */

        at_cmd[ui2_cmd_idx].e_code = EDB_CMD_DO_CLEAN;
        at_cmd[ui2_cmd_idx].u.b_bool = TRUE;
        ui2_cmd_idx++;

        /* restart */
    /*    at_cmd[ui2_cmd_idx].e_code = EDB_CMD_DO_RESTART;
        at_cmd[ui2_cmd_idx].u.b_bool = TRUE;
        ui2_cmd_idx++;
        */

        at_cmd[ui2_cmd_idx].e_code = EDB_CMD_END;
        ui2_cmd_idx++;


        i4_ret = x_edb_set (h_edb_client, at_cmd);
    }

    else if(ui1_temp == 2)
    {
        at_cmd[ui2_cmd_idx].e_code = EDB_CMD_DO_ENABLE;
        at_cmd[ui2_cmd_idx].u.b_bool = FALSE;
        ui2_cmd_idx++;


        at_cmd[ui2_cmd_idx].e_code = EDB_CMD_END;
        ui2_cmd_idx++;


        i4_ret = x_edb_set (h_edb_client, at_cmd);
    }

    else if(ui1_temp == 3)
    {
        at_cmd[ui2_cmd_idx].e_code = EDB_CMD_DO_ENABLE;
        at_cmd[ui2_cmd_idx].u.b_bool = TRUE;
        ui2_cmd_idx++;



        at_cmd[ui2_cmd_idx].e_code = EDB_CMD_END;
        ui2_cmd_idx++;


        i4_ret = x_edb_set (h_edb_client, at_cmd);
    }
/*
    else if(ui1_temp == 4)
    {
        edb_flash_event();
    }

    else if(ui1_temp == 5)
    {
        edb_sync_event_data_from_flash_to_mem();
    }
    */
    else if(ui1_temp == 6)
    {
            
        /* default parameters */
        
        at_cmd[ui2_cmd_idx].e_code = EDB_CMD_CFG_MAX_DAYS;
        at_cmd[ui2_cmd_idx].u.ui2_number = 4;
        ui2_cmd_idx++;

        at_cmd[ui2_cmd_idx].e_code = EDB_CMD_CFG_ACTUAL_ONLY;
        at_cmd[ui2_cmd_idx].u.b_bool = TRUE;
        ui2_cmd_idx++;

        /* flush */
        at_cmd[ui2_cmd_idx].e_code = EDB_CMD_DO_CLEAN;
        at_cmd[ui2_cmd_idx].u.b_bool = TRUE;
        ui2_cmd_idx++;

        /* restart */
        at_cmd[ui2_cmd_idx].e_code = EDB_CMD_DO_RESTART;
        at_cmd[ui2_cmd_idx].u.b_bool = TRUE;
        ui2_cmd_idx++;

        /* ASU mode */
        at_cmd[ui2_cmd_idx].e_code = EDB_CMD_DO_ASU_MODE;
        at_cmd[ui2_cmd_idx].u.b_bool = TRUE;
        ui2_cmd_idx++;
        
        at_cmd[ui2_cmd_idx].e_code = EDB_CMD_END;
        ui2_cmd_idx++;

        i4_ret = x_edb_set (h_edb_client, at_cmd);
    }

    else if(ui1_temp == 7)
    {          
        /* default parameters */
        
        at_cmd[ui2_cmd_idx].e_code = EDB_CMD_DO_FLASH_EVENTS;
        at_cmd[ui2_cmd_idx].u.ui2_number = 3;
        ui2_cmd_idx++;
        
        at_cmd[ui2_cmd_idx].e_code = EDB_CMD_END;
        ui2_cmd_idx++;
        
        i4_ret = x_edb_set (h_edb_client, at_cmd);

    }

    else if(ui1_temp == 8)
    {          
        /* default parameters */
        
        at_cmd[ui2_cmd_idx].e_code = EDB_CMD_DO_SYNC_EVENTS;
        at_cmd[ui2_cmd_idx].u.b_bool = TRUE;
        ui2_cmd_idx++;
        
        at_cmd[ui2_cmd_idx].e_code = EDB_CMD_END;
        ui2_cmd_idx++;
        
        i4_ret = x_edb_set (h_edb_client, at_cmd);


    }

    else if(ui1_temp == 9)
    {          
        /* default parameters */
        
        at_cmd[ui2_cmd_idx].e_code = EDB_CMD_RISKTV;
        at_cmd[ui2_cmd_idx].u.b_bool = TRUE;
        ui2_cmd_idx++;
        
        at_cmd[ui2_cmd_idx].e_code = EDB_CMD_END;
        ui2_cmd_idx++;
        
        i4_ret = x_edb_set (h_edb_client, at_cmd);


    }

    else if(ui1_temp == 10)
    {          
        /* default parameters */
        
        at_cmd[ui2_cmd_idx].e_code = EDB_CMD_RISKTV;
        at_cmd[ui2_cmd_idx].u.b_bool = FALSE;
        ui2_cmd_idx++;
        
        at_cmd[ui2_cmd_idx].e_code = EDB_CMD_END;
        ui2_cmd_idx++;
        
        i4_ret = x_edb_set (h_edb_client, at_cmd);


    }
    return i4_ret;
}

static INT32 _edb_cli_stop(INT32 i4_argc, const CHAR **pps_argv)
{
    INT32                   i4_ret       = 0;
    EDB_COMMAND_T*          pt_cmd       = NULL;
    UINT16                  ui2_idx      = 0;

    
    pt_cmd = EDB_OS_ALLOC(sizeof(EDB_COMMAND_T) * 3);
    if (pt_cmd == NULL)
    {
        return CLIR_CMD_EXEC_ERROR;
    }
    edb_eng_list_lock();
    ui2_idx = 0;


    pt_cmd[ui2_idx].e_code      = EDB_CMD_DO_ENABLE;
    pt_cmd[ui2_idx].u.b_bool    = FALSE;
    ui2_idx++;

    if (h_edb_client != NULL_HANDLE)
    {
        i4_ret = x_edb_set(h_edb_client, pt_cmd);
    }

    if (pt_cmd != NULL) {EDB_OS_FREE(pt_cmd);}
    edb_eng_list_unlock();
    return i4_ret;
}


static INT32 _edb_cli_delete(INT32 i4_argc, const CHAR **pps_argv)
{
    INT32       i4_ret  = 0;
    VOID*       pv_obj  = NULL;

    edb_eng_list_lock();

    i4_ret              = edb_handle_get_client_obj(h_edb_client,&pv_obj);
    if (i4_ret == EDBR_OK)
    {
        DBG_INFO(("EDB client handle=0x%x\r\n",h_edb_client));
    }

    if (h_edb_client != NULL_HANDLE)
    {
        i4_ret = x_edb_destroy(h_edb_client); 
        h_edb_client = NULL_HANDLE;
    }
    edb_eng_list_unlock();
    return i4_ret;
}


VOID _edb_cli_delete_client( VOID* pv_obj )
{
    EDB_REQUEST_BASE_CTRL_T*    pt_edb_client   = (EDB_REQUEST_BASE_CTRL_T*)pv_obj; 

    if (pt_edb_client != NULL)
    {
        EDB_OS_FREE(pt_edb_client);
    }
}

VOID _edb_cli_delete_edb( VOID* pv_obj )
{
    edb_delete_edb(pv_obj);
}

extern HANDLE_T                 h_edb_section_msg_q;

static INT32 _edb_cli_dvb_msg(INT32 i4_argc, const CHAR **pps_argv)
{
    /*   UINT16  ui2_msg_cnt = 0;  */
    SIZE_T  z_size = 0;
    {
        UINT8*  pui1_buffer = EDB_OS_ALLOC(32*sizeof(UINT8));
        if (pui1_buffer == NULL)
        {
            return CLIR_CMD_EXEC_ERROR;
        }

        x_memset(pui1_buffer,0xFF,32*sizeof(UINT8));
        x_dbg_stmt("EDB memory used=%d\r\n",edb_get_mem());
        x_dbg_stmt("EDB partition size=%d\r\n",edb_get_memory_size());
        if (pui1_buffer != NULL)
        {
            z_size = (SIZE_T)(os_mem_get_mem_ptr_size(pui1_buffer));
            x_dbg_stmt("sizeof pui1_buffer = %d\r\n",z_size);
            EDB_OS_FREE(pui1_buffer);
        }

        x_dbg_stmt("EDB memory used=%d\r\n",edb_get_mem());
        x_dbg_stmt("EDB partition size=%d\r\n",edb_get_memory_size());

    }

#if 0/*TODO*/
    if (x_msg_q_num_msgs (edb_dvb_section_get_msg_q_hdl(), &ui2_msg_cnt) == OSR_OK)
    {
        x_dbg_stmt(_EDB_INFO"Number of msgs in queue [%d] \r\n",ui2_msg_cnt);
        x_cli_parser("mw.os.sp");
    }
#endif	
    return CLIR_OK;
}






#if 0
static INT32 _edb_cli_delete_all(INT32 i4_argc, const CHAR **pps_argv)
{
    extern  HANDLE_T    g_edb_client_hash;
    extern  HANDLE_T    g_edb_handle_hash;

    INT32       i4_ret = 0;


    edb_eng_list_lock();

    //_edb_cli_stop(0,NULL);

    if (g_edb_client_hash != NULL_HANDLE)
    {
        edb_hash_delete(g_edb_client_hash, _edb_cli_delete_client );
        g_edb_client_hash = NULL_HANDLE;
    }

    if (g_edb_handle_hash != NULL_HANDLE)
    {
        edb_hash_delete(g_edb_handle_hash, _edb_cli_delete_edb );
        g_edb_handle_hash = NULL_HANDLE;
    }


    /*DVB engien data list reset*/
    i4_ret = edb_dvb_reset_eng_data_list();


    i4_ret = edb_handle_init(EDB_DEFAULT_MAX_NUM,EDB_DEFAULT_CLIENT_MAX_NUM);
    h_edb_client = NULL_HANDLE;
    edb_eng_list_unlock();
    return i4_ret;
}
#endif




static INT32 _edb_cli_iterator_edb_hash(
        HANDLE_T          h_hash,
        UINT32            ui4_counter,
        VOID*             pv_obj,
        VOID*             pv_parse_tag )
{
    INT32                   i4_ret   = 0;
    EDB_T*                  pt_edb   = NULL; 
    EDB_ENG_CTRL_ENTRY_T*   pt_eng_ctrl_entry = NULL;

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


    DBG_INFO((_EDB_INFO"EDB INFO START=======================================\r\n"));
    DBG_INFO((_EDB_INFO"pt_edb Addr      = 0x%x \r\n",pt_edb                    ));
    DBG_INFO((_EDB_INFO"s_name           = %s   \r\n",pt_edb->s_name            ));
    DBG_INFO((_EDB_INFO"pt_edb_event_pool= 0x%x \r\n",&(pt_edb->t_edb_event_pool) ));
    DBG_INFO((_EDB_INFO"e_cond           = %d   \r\n",pt_edb->e_cond            ));
    DBG_INFO((_EDB_INFO"EDB INFO END=========================================\r\n\r\n"));


    DBG_INFO((_EDB_INFO"EDB Eng & Eng data START=======================================\r\n"));
    DLIST_FOR_EACH(pt_eng_ctrl_entry,&( pt_edb->t_eng_ctrl_list), t_link)
    {

        DBG_INFO((_EDB_INFO"ac_tuner_name   = %s \r\n",pt_eng_ctrl_entry->ac_tuner_name  ));
        DBG_INFO((_EDB_INFO"ui2_svl_id      = %d \r\n",pt_eng_ctrl_entry->ui2_svl_id  ));
        DBG_INFO((_EDB_INFO"h_eng_ctrl      = 0x%08x \r\n",pt_eng_ctrl_entry->h_eng_ctrl  ));
        DBG_INFO((_EDB_INFO"pt_eng          = 0x%08x \r\n",pt_eng_ctrl_entry->pt_eng  ));
        DBG_INFO((_EDB_INFO"pt_edb          = 0x%08x \r\n",pt_eng_ctrl_entry->pt_edb  ));
        DBG_INFO((_EDB_INFO"pt_eng_data     = 0x%08x \r\n",pt_eng_ctrl_entry->pt_eng_data  ));
		#if 0/*TODO*/
		edb_dvb_dump(pt_eng_ctrl_entry->pt_eng_data);
		#endif
    }


    DBG_INFO((_EDB_INFO"EDB Eng & Eng data END=======================================\r\n"));


    /*TODO dump EDB state*/    

    return i4_ret;
}

#if 0
static INT32 _edb_cli_free_edb_client_hash(
        HANDLE_T          h_hash,
        UINT32            ui4_counter,
        VOID*             pv_obj,
        VOID*             pv_parse_tag )
{
    INT32                       i4_ret          = 0;
    EDB_REQUEST_BASE_CTRL_T*    pt_edb_client   = NULL; 
    HANDLE_T*                   ph_key          = NULL;
    EDB_CHECK_RET( 
            (pv_obj == NULL),
            (EDBR_INTERNAL_ERROR),
            (("Arguments is wrong\r\n"))
            );

    ph_key = (HANDLE_T*)pv_parse_tag;
    pt_edb_client = (EDB_REQUEST_BASE_CTRL_T*)pv_obj;
    DBG_INFO((_EDB_INFO"pt_edb_client addr=0x%x ui4_counter=%d\r\n",pt_edb_client,ui4_counter));

    if (pt_edb_client->h_edb_client != NULL_HANDLE)
    {
        ph_key[ui4_counter] = pt_edb_client->h_edb_client;

    }
    return i4_ret;
}
#endif

static INT32 _edb_cli_iterator_edb_client_hash(
        HANDLE_T          h_hash,
        UINT32            ui4_counter,
        VOID*             pv_obj,
        VOID*             pv_parse_tag )
{
    INT32                       i4_ret          = 0;
    EDB_REQUEST_BASE_CTRL_T*    pt_edb_client   = NULL; 

    EDB_CHECK_RET( 
            (pv_obj == NULL),
            (EDBR_INTERNAL_ERROR),
            (("Arguments is wrong\r\n"))
            );

    pt_edb_client = (EDB_REQUEST_BASE_CTRL_T*)pv_obj;


    DBG_INFO((_EDB_INFO"EDB Client INFO START=====================================\r\n"));
    DBG_INFO((_EDB_INFO"h_edb_client  = 0x%x  \r\n",pt_edb_client->h_edb_client     ));
    DBG_INFO((_EDB_INFO"pt_edb_client = 0x%x  \r\n",pt_edb_client                   ));
    DBG_INFO((_EDB_INFO"pf_nfy        = 0x%x  \r\n",pt_edb_client->pf_nfy           ));
    DBG_INFO((_EDB_INFO"pv_nfy_tag    = 0x%x  \r\n",pt_edb_client->pv_nfy_tag       ));
    DBG_INFO((_EDB_INFO"EDB Client INFO END=======================================\r\n\r\n"));

    return i4_ret;
}




static INT32 _edb_cli_edb_dump(INT32 i4_argc, const CHAR **pps_argv)
{
    /*Iterator EDB handle list*/
    extern HANDLE_T    g_edb_handle_hash;/*EDB_T list*/
    extern HANDLE_T    g_edb_client_hash;/*BASE_CTRL_T list*/

    INT32               i4_ret = 0;

    DBG_INFO((_EDB_INFO"EDB Max memory =%d[byte] %d[k]\r\n",edb_get_memory_size(),edb_get_memory_size()/1024));
    DBG_INFO((_EDB_INFO"EDB memory =%d[byte] %d[k]\r\n",edb_get_mem(),edb_get_mem()/1024));
    
    if (g_edb_handle_hash != NULL_HANDLE)
    {
        i4_ret = edb_hash_parse(g_edb_handle_hash,_edb_cli_iterator_edb_hash,NULL);
    }

    if (g_edb_client_hash != NULL_HANDLE)
    {
        i4_ret = edb_hash_parse(g_edb_client_hash,_edb_cli_iterator_edb_client_hash,NULL);
    }




    return i4_ret ;
}

#if 0
static RC_CMP_FCT_T _desc_cmp_key_fct (const VOID*    pv_a,
                                         const VOID*    pv_b,
                                         VOID*          pv_obj)
{
    RC_CMP_FCT_T  t_cmp;

    if (
          ((EDB_POOL_EVENT_INFO_T*)(pv_a))->t_start_time > ((EDB_POOL_EVENT_INFO_T*)(pv_b))->t_start_time
        )
    {
        t_cmp = RC_GREATER;
    }
    else if (
                ((EDB_POOL_EVENT_INFO_T*)(pv_a))->t_start_time < ((EDB_POOL_EVENT_INFO_T*)(pv_b))->t_start_time            )
    {
        t_cmp = RC_SMALLER;
    }
    else
    {
        t_cmp = RC_EQUAL;
    }

    return t_cmp;
}
#endif

static EDB_POOL_EVENT_INFO_T*    apt_sort_event[1024*8] = {0};
static UINT32                    ui4_sort_event_count = 0;

static VOID exchange(EDB_POOL_EVENT_INFO_T* p[],INT32 i,INT32 j)
{
    EDB_POOL_EVENT_INFO_T* t = NULL;
    t = *(p+i);
    *(p+i) = *(p+j);
    *(p+j) = t;
}

static VOID cmpExchange(EDB_POOL_EVENT_INFO_T* item[],INT32 i,INT32 j){
    if (item[i]->t_start_time < item[j]->t_start_time ? TRUE:FALSE)
        exchange(item,i,j);
}

static VOID bubble(EDB_POOL_EVENT_INFO_T* item[],INT32 len)
{
    INT32 i=0;
    INT32 j=0;
    for (i=0;i<len;i++){
        for (j=len-1;j>i;j--){
            cmpExchange(item,j,j-1);
        }
    }
}


static INT32 _edb_cli_dump_pool_event(EDB_EVENT_POOL_CH_T* pt_pool_ch )
{
    INT32                   i4_ret          = EDBR_OK;
    UINT32                  ui4_i           = 0;
    P_ELEMENT_OBJ_T         pt_elm          = NULL;
    HANDLE_TYPE_T           e_hdl_type;
    EDB_HASH_T*             pt_hash_obj     = NULL;
    EDB_POOL_EVENT_INFO_T*  pt_pool_event   = NULL;

    EDB_CHECK_RET( 
            (pt_pool_ch == NULL),
            (EDBR_INTERNAL_ERROR),
            (("pool channel is null\r\n"))
            );

    i4_ret = handle_get_type_obj(pt_pool_ch->h_event_hash ,&e_hdl_type, (VOID**)&pt_hash_obj );
    if ( i4_ret != HR_OK || e_hdl_type != EDBT_HASH )
    {
        return EDBR_INV_HANDLE;
    }

    x_memset(apt_sort_event,0x0,sizeof(apt_sort_event));
    ui4_sort_event_count = 0;

    for ( ui4_i=0; ui4_i<pt_hash_obj->z_table_size; ++ui4_i )
    {
        for( pt_elm = pt_hash_obj->ppt_table[ui4_i]; pt_elm != NULL; )
        {
            pt_pool_event = (EDB_POOL_EVENT_INFO_T*)pt_elm->pv_obj;
            if (
                    pt_pool_event != NULL
                    )
            {   //print event start time and duration
                apt_sort_event[ui4_sort_event_count++] = pt_pool_event;
            }
            pt_elm = pt_elm->pt_next;
        }
    }

    x_dbg_stmt("ui4_sort_event_count=%d\r\n",ui4_sort_event_count);
    if (ui4_sort_event_count>0)
    {
        bubble(apt_sort_event,ui4_sort_event_count);
    }

    for ( ui4_i=0; ui4_i<ui4_sort_event_count; ++ui4_i )
    {
        pt_pool_event = (EDB_POOL_EVENT_INFO_T*)apt_sort_event[ui4_i];

        DBG_INFO(("%d\t%llu\t",pt_pool_ch->t_key.ui2_svc_id,pt_pool_event->t_start_time));
        DBG_INFO(("ID=0x%04x\t",pt_pool_event->ui2_event_id));
        PRINT_TIME("Start_time:",pt_pool_event->t_start_time); DBG_INFO(("\t"));
        PRINT_TIME("End_time:",pt_pool_event->t_start_time + pt_pool_event->t_duration); DBG_INFO(("\t"));
        if (pt_pool_event->ps_event_title != NULL)
        {
            DBG_INFO((" Title:%s",pt_pool_event->ps_event_title));
        }

        DBG_INFO(("\r\n"));

    }

    return EDBR_OK;    
}



static INT32 _edb_cli_iterator_edb_pool(
        HANDLE_T          h_hash,
        UINT32            ui4_counter,
        VOID*             pv_obj,
        VOID*             pv_parse_tag )
{
    INT32                   i4_ret   = 0;
    EDB_T*                  pt_edb   = NULL; 
    EDB_EVENT_POOL_T*       pt_edb_event_pool = NULL;
    P_ELEMENT_OBJ_T         pt_elm          = NULL;
    HANDLE_TYPE_T           e_hdl_type;
    EDB_HASH_T*             pt_hash_obj     = NULL;
    EDB_EVENT_POOL_CH_T*    pt_pool_ch      = NULL;
    UINT32                  ui4_i           = 0;
    UINT32                  ui4_ch_id       = 0;
    BOOL                    b_dump_special  = FALSE;

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
    ui4_ch_id = (UINT32)pv_parse_tag;
    if (ui4_ch_id != 0)
    {
        b_dump_special = TRUE;
    }


    DBG_INFO((_EDB_INFO"EDB INFO START=======================================\r\n"));
    DBG_INFO((_EDB_INFO"pt_edb Addr      = 0x%x \r\n",pt_edb                    ));
    DBG_INFO((_EDB_INFO"s_name           = %s   \r\n",pt_edb->s_name            ));
    DBG_INFO((_EDB_INFO"pt_edb_event_pool= 0x%x \r\n",&(pt_edb->t_edb_event_pool) ));
    DBG_INFO((_EDB_INFO"e_cond           = %d   \r\n",pt_edb->e_cond            ));
    DBG_INFO((_EDB_INFO"EDB INFO END=========================================\r\n\r\n"));
    
    pt_edb_event_pool = &(pt_edb->t_edb_event_pool);
    if (pt_edb_event_pool == NULL)
        return EDBR_INFO_NOT_FOUND;

    i4_ret = handle_get_type_obj( pt_edb_event_pool->h_ch_hash , &e_hdl_type, (VOID**)&pt_hash_obj );
    if ( i4_ret != HR_OK || e_hdl_type != EDBT_HASH )
    {
        return EDBR_INV_HANDLE;
    }

    for ( ui4_i=0; ui4_i<pt_hash_obj->z_table_size; ++ui4_i )
    {
        for( pt_elm = pt_hash_obj->ppt_table[ui4_i]; pt_elm != NULL; )
        {
            pt_pool_ch = (EDB_EVENT_POOL_CH_T*)pt_elm->pv_obj;

            if (b_dump_special == TRUE)
            {
                if (ui4_ch_id != pt_pool_ch->ui4_channel_id)
                {
                    pt_elm = pt_elm->pt_next;
                    continue;
                }
            }
            DBG_INFO(("Channel info on=0x%04x ts=0x%04x svc=0x%04x(%d) channel_id=0x%08x ch_name:%s\r\n",
               pt_pool_ch->t_key.ui2_on_id,
               pt_pool_ch->t_key.ui2_ts_id,
               pt_pool_ch->t_key.ui2_svc_id,
               pt_pool_ch->t_key.ui2_svc_id,
               pt_pool_ch->ui4_channel_id,
               pt_pool_ch->ac_name
            ));

            _edb_cli_dump_pool_event(pt_pool_ch);
            pt_elm = pt_elm->pt_next;
        }
    }


    return i4_ret;
}


static INT32 _edb_cli_insert_pool_event(EDB_EVENT_POOL_CH_T* pt_pool_ch )
{
    INT32                   i4_ret          = EDBR_OK;
    HANDLE_TYPE_T           e_hdl_type;
    EDB_HASH_T*             pt_hash_obj     = NULL;
    EDB_POOL_EVENT_INFO_T*  pt_pool_event   = NULL;
    TIME_T                  t_start_time    = x_dt_get_brdcst_utc(NULL,NULL);
    UINT32                  i               = 0;
    CHAR                    ac_title[20]    = {0}    ;
    EDB_CHECK_RET( 
            (pt_pool_ch == NULL),
            (EDBR_INTERNAL_ERROR),
            (("pool channel is null\r\n"))
            );

    i4_ret = handle_get_type_obj(pt_pool_ch->h_event_hash ,&e_hdl_type, (VOID**)&pt_hash_obj );
    if ( i4_ret != HR_OK || e_hdl_type != EDBT_HASH )
    {
        return EDBR_INV_HANDLE;
    }

    for (i=0;i<300;i++)
    {
        pt_pool_event = EDB_OS_ALLOC(sizeof(EDB_POOL_EVENT_INFO_T));
        if (pt_pool_event == NULL)
        {
            return EDBR_OUT_OF_MEM;
        }
        x_memset(pt_pool_event,0x0,sizeof(EDB_POOL_EVENT_INFO_T));
        pt_pool_event->ui2_event_id = 0xAB+i;
        pt_pool_event->t_start_time = t_start_time + (i)*600;
        pt_pool_event->t_duration   = 60;
        x_sprintf(ac_title,"%s %d","EVENT ",i);
        pt_pool_event->ps_event_title = EDB_OS_ALLOC(20*sizeof(CHAR));
        if (pt_pool_event->ps_event_title == NULL)
        {
            return EDBR_OUT_OF_MEM;
        }
        x_strcpy(pt_pool_event->ps_event_title,ac_title); 

        edb_pool_add_event_obj(pt_pool_ch,pt_pool_event); 
    }

    return EDBR_OK;    
}

static INT32 _edb_cli_insert_edb_pool(
        HANDLE_T          h_hash,
        UINT32            ui4_counter,
        VOID*             pv_obj,
        VOID*             pv_parse_tag )
{
    INT32                   i4_ret   = 0;
    EDB_T*                  pt_edb   = NULL; 
    EDB_EVENT_POOL_T*       pt_edb_event_pool = NULL;
    P_ELEMENT_OBJ_T         pt_elm          = NULL;
    HANDLE_TYPE_T           e_hdl_type;
    EDB_HASH_T*             pt_hash_obj     = NULL;
    EDB_EVENT_POOL_CH_T*    pt_pool_ch      = NULL;
    UINT32                  ui4_i           = 0;


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


    DBG_INFO((_EDB_INFO"EDB INFO START=======================================\r\n"));
    DBG_INFO((_EDB_INFO"pt_edb Addr      = 0x%x \r\n",pt_edb                    ));
    DBG_INFO((_EDB_INFO"s_name           = %s   \r\n",pt_edb->s_name            ));
    DBG_INFO((_EDB_INFO"pt_edb_event_pool= 0x%x \r\n",&(pt_edb->t_edb_event_pool) ));
    DBG_INFO((_EDB_INFO"e_cond           = %d   \r\n",pt_edb->e_cond            ));
    DBG_INFO((_EDB_INFO"EDB INFO END=========================================\r\n\r\n"));
    
    pt_edb_event_pool = &(pt_edb->t_edb_event_pool);
    if (pt_edb_event_pool == NULL)
        return EDBR_INFO_NOT_FOUND;

    i4_ret = handle_get_type_obj( pt_edb_event_pool->h_ch_hash , &e_hdl_type, (VOID**)&pt_hash_obj );
    if ( i4_ret != HR_OK || e_hdl_type != EDBT_HASH )
    {
        return EDBR_INV_HANDLE;
    }

    for ( ui4_i=0; ui4_i<pt_hash_obj->z_table_size; ++ui4_i )
    {
        for( pt_elm = pt_hash_obj->ppt_table[ui4_i]; pt_elm != NULL; )
        {
            pt_pool_ch = (EDB_EVENT_POOL_CH_T*)pt_elm->pv_obj;
            DBG_INFO(("Channel info on=0x%04x ts=0x%04x svc=0x%04x(%d) channel_id=0x%08x ch_name:%s\r\n",
               pt_pool_ch->t_key.ui2_on_id,
               pt_pool_ch->t_key.ui2_ts_id,
               pt_pool_ch->t_key.ui2_svc_id,
               pt_pool_ch->t_key.ui2_svc_id,
               pt_pool_ch->ui4_channel_id,
               pt_pool_ch->ac_name
            ));

            _edb_cli_insert_pool_event(pt_pool_ch);
            pt_elm = pt_elm->pt_next;
            //return EDBR_OK;//TODO
        }
    }


    return i4_ret;
}

static INT32 _edb_cli_edb_dump_event(INT32 i4_argc, const CHAR **pps_argv)
{
    /*Iterator EDB handle list*/
    extern HANDLE_T    g_edb_handle_hash;/*EDB_T list*/

    INT32               i4_ret = 0;
    UINT32              ui4_ch_id = 0;

    if ((i4_argc == 2) || 
	    (pps_argv == NULL))
    {
        ui4_ch_id        = (UINT32) x_strtoull(pps_argv[1], NULL, 0);
        x_dbg_stmt("Dump channel id=0x%08x (%d)\r\n",ui4_ch_id,ui4_ch_id);
    } 
    
    if (g_edb_handle_hash != NULL_HANDLE)
    {
        i4_ret = edb_hash_parse(g_edb_handle_hash,_edb_cli_iterator_edb_pool,(VOID*)ui4_ch_id);
    }

    return i4_ret ;
}

static INT32 _edb_cli_fake_event(INT32 i4_argc, const CHAR **pps_argv)
{
    /*Iterator EDB handle list*/
    extern HANDLE_T    g_edb_handle_hash;/*EDB_T list*/
    INT32               i4_ret = 0;
    
    if (g_edb_handle_hash != NULL_HANDLE)
    {
        i4_ret = edb_hash_parse(g_edb_handle_hash,_edb_cli_insert_edb_pool,NULL);
    }

    return i4_ret ;
}




static INT32 _edb_cli_get_dbg_level(INT32        i4_argc, 
                                    const CHAR** pps_argv)
{
    INT32       i4_return;

    /* Check arguments */
    if (i4_argc != 1)
    {
        DBG_INFO((_EDB_INFO"edb.gdl\n\r"));
        return CLIR_OK;
    }    

    i4_return = x_cli_show_dbg_level(x_edb_get_dbg_level());

    return i4_return;
}


/*-----------------------------------------------------------------------------
 * Name: _edb_cli_set_dbg_level
 *
 * Description: This API sets the debug level of EDB.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _edb_cli_set_dbg_level(INT32        i4_argc,
                                    const CHAR** pps_argv)
{
    INT32       i4_return;
    UINT16      ui2_dbg_level;     

    /* edb.sdl [level] */


    i4_return = x_cli_parse_dbg_level(i4_argc, pps_argv, &ui2_dbg_level);

    if (i4_return == CLIR_OK)
    {
        if (x_edb_set_dbg_level(ui2_dbg_level) == FALSE)
        {
            i4_return = CLIR_CMD_EXEC_ERROR;  
        }
        DBG_INFO((_EDB_INFO"set debug level %s\n\r", (i4_return == CLIR_OK) ? "successful" : "failed"));
    }


    return i4_return;
}



VOID _edb_cli_nfy_fct(
        HANDLE_T                h_edb,
        EDB_NFY_REASON_T        e_reason,
        VOID*                   pv_nfy_tag,
        UINT32                  ui4_data1,
        UINT32                  ui4_data2)
{
    x_dbg_stmt("nfy h_edb=0x%08x,reason=%d,tag=0x%08x,ui4_data1=0x%08x,ui4_data2=0x%08x\r\n",
               h_edb,
               e_reason,
               pv_nfy_tag,
               ui4_data1,
               ui4_data2);


}

static INT32 _edb_cli_memory_overhead(INT32 i4_argc, const CHAR **pps_argv)
{
#ifdef MEASURE_OVERHEAD
    extern UINT32 ui4_section_raw_memory_used;
    DBG_INFO((_EDB_INFO"EDB Max memory =%d[byte] %d[k]\r\n",edb_get_memory_size(),edb_get_memory_size()/1024));
    DBG_INFO((_EDB_INFO"EDB memory =%d[byte] %d[k]\r\n",edb_get_mem(),edb_get_mem()/1024));
    DBG_INFO((_EDB_INFO"Section used memory = %d\r\n",ui4_section_raw_memory_used));
    return CLIR_OK;
#else
    return CLIR_OK;
#endif 
}

/*-----------------------------------------------------------------------------
 * Name: edb_cli_reg_sub_table
 *
 * Description: This API hooks a CLI command table into EVCTX CLI 
 *              component.
 *
 * Inputs:  -
 *
 * Outputs: -		
 *
 * Returns: EVCTXR_OK              Routine successful.
 *          EVCTXR_INV_ARG         Invalid arguments.
 *          EVCTXR_OUT_OF_MEM      Buffer is not enough.    
 ----------------------------------------------------------------------------*/
INT32 edb_cli_reg_sub_table(const CLI_EXEC_T* pt_exec)
{
    if (!pt_exec)
    {
        return EDBR_INV_ARG;
    }

    if (z_last_entry >= z_cmd_tbl)
    {
        return EDBR_OUT_OF_MEM;
    }

    at_edb_cli_tbl[z_last_entry++] = *pt_exec;
     
    return EDBR_OK;
}



#endif

INT32 edb_cli_init(VOID)
{
#ifdef CLI_SUPPORT 
    INT32   i4_return;

    /* Attach EDB CLI command table to CLI */
    i4_return = x_cli_attach_cmd_tbl(at_edb_root_cmd_tbl, CLI_CAT_MW, CLI_GRP_NONE);

    if (i4_return != CLIR_OK)
    {
        return EDBR_INTERNAL_ERROR;
    }
    return EDBR_OK;
#else
    return EDBR_OK;
#endif  /* CLI_SUPPORT */


}





