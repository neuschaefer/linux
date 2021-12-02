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
 * $RCSfile: edb_dvb_eng_cli.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *         This file implements CLI command table for EVCTX - DVB engine.
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
  include files
  ----------------------------------------------------------------------------*/

#include "inc/x_common.h"
#include "os/inc/x_os.h"
#include "cli/x_cli.h"
#include "dbg/x_dbg.h"
#include "edb/x_edb.h"
#include "edb_dvb_eng.h"
#include "edb/edb_slctr.h"
#include "handle/u_handle_grp.h"
#include "hash/hash_api.h"
#include "handle/u_handle.h"
#include "inc/x_sys_name.h"
#include "inc/u_device_type.h"
#include "dt/x_dt.h"
#include "edb/edb_eng.h"
#include "edb/edb_pool.h"
#include "edb/edb_hash_api.h"




/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
#ifdef CLI_SUPPORT

extern INT32 edb_cli_reg_sub_table(const CLI_EXEC_T* pt_exec);

extern INT32 edb_dvb_dump(VOID*    pt_obj);


extern INT32 edb_dvb_cli_dump_single_event_by_id(        
		HANDLE_T            	h_edb_eng,
        UINT32              	ui4_channel_id,
        UINT16              	ui2_event_id );


static INT32 _edb_dvb_cli_edb_dump(INT32 i4_argc, const CHAR **pps_argv);

static INT32 _edb_dvb_cli_get_specific_event_by_id(INT32 i4_argc, const CHAR **pps_argv);


#ifdef DBG_INFO
#undef DBG_INFO
#define DBG_INFO(x) x_dbg_stmt x
#endif



/* EVCTX - DVB engine command table */
static CLI_EXEC_T at_edb_dvb_cli_tbl[] = 
{
    {"dvb_eng_dump",        "deb",  _edb_dvb_cli_edb_dump,                  NULL,   "edb dvb engine dump",      CLI_SUPERVISOR},
	{"get_specific_event",	"gse",	_edb_dvb_cli_get_specific_event_by_id,  NULL,	"edb dvb engine dump",		CLI_SUPERVISOR},

    END_OF_CLI_CMD_TBL
};

/* EVCTX - DVB engine root command table */
static CLI_EXEC_T at_edb_dvb_root_cmd_tbl[] =
{
	{"dvb",        NULL,        NULL,       at_edb_dvb_cli_tbl,      "DVB engine commands",     CLI_SUPERVISOR},
	END_OF_CLI_CMD_TBL
};




static INT32 _edb_dvb_cli_iterator_edb_hash(
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
		/*DOING*/
		edb_dvb_dump(pt_eng_ctrl_entry->pt_eng_data);
		
    }


    DBG_INFO((_EDB_INFO"EDB Eng & Eng data END=======================================\r\n"));


    /*TODO dump EDB state*/    

    return i4_ret;
}




static INT32 _edb_dvb_cli_edb_dump(INT32 i4_argc, const CHAR **pps_argv)
{
    /*Iterator EDB handle list*/
    extern HANDLE_T    g_edb_handle_hash;/*EDB_T list*/

    INT32               i4_ret = 0;
    
    if (g_edb_handle_hash != NULL_HANDLE)
    {
        i4_ret = edb_hash_parse(g_edb_handle_hash,_edb_dvb_cli_iterator_edb_hash,NULL);
    }

    return i4_ret ;
}

static INT32 _edb_dvb_cli_get_specific_event_by_id(INT32 i4_argc, const CHAR **pps_argv)
{

	INT32                   i4_ret       = 0;
	UINT32					ui4_channel_id=0;
	UINT16					ui2_event_id=0;
	HANDLE_T				h_edb_eng=0;

	if ((i4_argc == 4) || (pps_argv != NULL))
    {
        h_edb_eng = (HANDLE_T) x_strtoull(pps_argv[1], NULL, 0);
		ui4_channel_id = (UINT32) x_strtoull(pps_argv[2], NULL, 0);
		ui2_event_id = (UINT16) x_strtoull(pps_argv[3], NULL, 0);
		i4_ret = edb_dvb_cli_dump_single_event_by_id(h_edb_eng,ui4_channel_id,ui2_event_id);
    }
    

    return i4_ret ;
}






#endif

INT32 edb_dvb_cli_init(VOID)
{
#ifdef CLI_SUPPORT
    INT32   i4_return = EDBR_OK;

	i4_return = edb_cli_reg_sub_table(at_edb_dvb_root_cmd_tbl);

	if (i4_return != CLIR_OK)
	{
		return EDBR_INTERNAL_ERROR;
	}



    return i4_return;
#else     
    return EDBR_OK;
#endif  /* CLI_SUPPORT */    
}

