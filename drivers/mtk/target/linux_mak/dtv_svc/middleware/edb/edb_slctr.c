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
#include "inc/common.h"
#include "os/inc/x_os.h"
#include "os/inc/os.h"
#include "handle/u_handle_grp.h"
#include "dbg/x_dbg.h"
#include "dbg/def_dbg_level_mw.h"
#include "res_mngr/x_rm.h"
#include "res_mngr/drv/u_drv_cust.h"
#include "res_mngr/u_rm_dev_types.h"



#include "edb/edb_eng.h"
#include "edb/edb_slctr.h"
#include "edb/edb_pool.h"


#ifdef CLI_SUPPORT
static INT32 os_show_mem_part_stats(HANDLE_T h_mem_hdl);
#endif

/*-----------------------------------------------------------------------------
  macros, defines, typedefs, enums
  ----------------------------------------------------------------------------*/
extern INT32 x_cli_parser(const CHAR* ps_cmd);


/*typedef DLIST_T(_EDB_T) EDB_LIST_T;*/
/*-----------------------------------------------------------------------------
  data declarations
  ----------------------------------------------------------------------------*/
static BOOL            b_edb_inited        = FALSE;
static THREAD_DESCR_T  t_edb_thread_descr  = { 
    DEFAULT_STACK_SIZE, 
    DEFAULT_PRIORITY, 
    DEFAULT_NUM_MSGS 
};

/* EDB Selector Thread info */
static HANDLE_T        h_edb_slctr_thread  = NULL_HANDLE;
static HANDLE_T        h_edb_slctr_msg_q   = NULL_HANDLE;


static UINT16          ui2_edb_dbg_level   = DBG_LEVEL_INFO;

static SIZE_T          z_memory_used       = 0;
static SIZE_T          z_memory_size       = 1024*1024*12;/*Default 12M*/

//#define EDB_INTERNAL_DEBUG_DYNAMIC_ALLOCATION_EDB_MEMORY

#ifdef EDB_INTERNAL_DEBUG_DYNAMIC_ALLOCATION_EDB_MEMORY
static VOID*    pv_mem_partition = NULL;
#endif


#ifdef MW_EPG_CACHE_NVM_SUPPORT 
static SIZE_T          z_event_size_for_flash = 1024*1024*12;
#endif
#define                EDB_MEM_PARTITION_SIZE    ((SIZE_T)EDB_MAX_MEMORY_SIZE)
#ifdef EDB_MEM_PART
static HANDLE_T        h_edb_mem_part = NULL_HANDLE;

VOID edb_show_part_info(VOID)
{
    x_dbg_stmt("EDB Partition info==================\r\n");
#ifdef CLI_SUPPORT
    os_show_mem_part_stats(h_edb_mem_part);
#endif
    x_dbg_stmt("EDB Partition info==================\r\n");
}
#endif
/*-----------------------------------------------------------------------------
  functions declaraions
  ----------------------------------------------------------------------------*/
static VOID _edb_slctr_thread_main(VOID* pv_arg);


/*-----------------------------------------------------------------------------
  static functions implementation
  ----------------------------------------------------------------------------*/


#ifdef CLI_SUPPORT
static INT32
os_show_mem_part_stats(HANDLE_T h_mem_hdl)
{
    SIZE_T z_total_free;
    SIZE_T z_biggest_free;
    SIZE_T z_free_obj;
    SIZE_T z_allocated_obj;
    SIZE_T z_total_size;
    SIZE_T z_alloc_size;
    INT32 i4_ret;
    CHAR s_name[32];


    extern INT32 os_mem_scan_mem(HANDLE_T    h_part_hdl);
    extern VOID
        os_mem_cli_stats(HANDLE_T h_part_hdl,
                         SIZE_T *z_totalfree,
                         SIZE_T *z_bigfree,
                         SIZE_T *z_freeobj,
                         SIZE_T *z_allocatedobj,
                         SIZE_T *z_totalsize,
                         SIZE_T *z_allocsize,
                         CHAR   *ps_name);


    i4_ret = os_mem_scan_mem(h_mem_hdl);
    if (i4_ret == OSR_OUT_BOUND)
    {
        x_dbg_stmt("Memory partition %d might have been corrupted\n",
                   h_mem_hdl);
    }

    os_mem_cli_stats(h_mem_hdl,
                     &z_total_free,
                     &z_biggest_free,
                     &z_free_obj,
                     &z_allocated_obj,
                     &z_total_size,
                     &z_alloc_size,
                     &(s_name[0]));

    x_dbg_stmt("\nMemory Partition Statistics:\n \
                Handle: %d\n \
                Name: %s\n \
                Allocation Type(%d): %s\n \
                Total Size:         %8d Bytes\n \
                Total Free Size:    %8d Bytes\n \
                Biggest Free Block: %8d Bytes\n \
                Allocated Blocks: %d\n \
                Free Blocks: %d\n",
                h_mem_hdl, s_name,
                z_alloc_size,
                ((z_alloc_size == 0) ? "Dynamic" : "Fixed"),
                z_total_size, z_total_free, z_biggest_free,
                z_allocated_obj, z_free_obj);

    return(EDBR_OK);
}
#endif




/*-----------------------------------------------------------------------------
 * Name: _edb_slctr_thread_main
 *
 * Description: main function of EDB Selector thread
 *
 * Inputs:  pv_arg References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _edb_slctr_thread_main(VOID*  pv_arg)
{
    INT32               i4_ret      = 0;
    UINT16              ui2_index   = 0;
    EDB_SLCTR_MSG_T     t_msg       = {0};
    SIZE_T              z_msg_size  = 0; 
    BOOL                b_shutdown  = FALSE;

    /* Create message queue for EDB Selector Thread */
    i4_ret = x_msg_q_create( &h_edb_slctr_msg_q,
                             (CHAR*)EDB_SLCTR_MSGQ_NAME,
                             sizeof( EDB_SLCTR_MSG_T ),
                             t_edb_thread_descr.ui2_num_msgs );
    if ( i4_ret != OSR_OK )
    {
        DBG_ABORT( DBG_MOD_EDB );
        /* program abort here. */
    }

    while( !b_shutdown )
    {
        EDB_ENG_T*   pt_eng = NULL;
        z_msg_size = sizeof( EDB_SLCTR_MSG_T );

        i4_ret = x_msg_q_receive( &ui2_index,
                                  &t_msg,
                                  &z_msg_size,
                                  &h_edb_slctr_msg_q,
                                  (UINT16)1,
                                  X_MSGQ_OPTION_WAIT );
        if ( i4_ret == OSR_OK )
        {
            switch( t_msg.e_msg_type )
            {
                case EDB_SLCTR_MSG_SHUTDOWN:
                    b_shutdown = TRUE;
                    break;
                case EDB_SLCTR_MSG_ACTIVATE_ENG_TASK:
                    pt_eng = edb_find_eng_by_brdcst_type(
                            t_msg.u_data.t_eng_task.e_brdcst_type);
                    if (pt_eng != NULL)
                    {
                        pt_eng->t_edb_fct_tbl.pf_engine_task();
                    }
                    break;
                default:
                    break;
            }
            if (b_shutdown)
            {
                break;
            }
        }
        x_thread_delay( 3 );
    } /* while: the main loop */

    x_msg_q_delete( h_edb_slctr_msg_q );

    b_edb_inited = FALSE;

    x_thread_exit();

}

/*-----------------------------------------------------------------------------
 * Name: _edb_handle_autofree
 *
 * Description: Autofree function.
 * Inputs:      -
 * Outputs:     -
 * Returns:     -
 *-----------------------------------------------------------------------------*/
INT32 _edb_handle_autofree (HANDLE_T       h_handle,
                            HANDLE_TYPE_T  e_type)
{
    INT32 i4_ret = EDBR_INTERNAL_ERROR;

    if (e_type < HT_GROUP_EDB || e_type >= EDBT_LAST_ENTRY) {
        return HR_INV_HANDLE_TYPE;
    }

    switch (e_type) {
        case EDBT_EDB:
            break;
        default:
            i4_ret = EDBR_INV_HANDLE;
            break;
    }

    return (i4_ret == EDBR_OK ? HR_OK : HR_INV_HANDLE);
}




/*-----------------------------------------------------------------------------
  functions declaraions
  ----------------------------------------------------------------------------*/

HANDLE_T edb_get_msg_q_hdl (VOID)
{
    return h_edb_slctr_msg_q;
}

#ifdef    MW_EDB_FBM_SHARE_MEM

INT32 edb_mem_partition_create(UINT8 ui1_m_size)
{
#ifndef EDB_INTERNAL_DEBUG_DYNAMIC_ALLOCATION_EDB_MEMORY
    INT32       i4_ret;
    SIZE_T      z_size = sizeof(DRV_CUST_OPERATION_INFO_T);
    UINT8       u1EDBSize = ui1_m_size;
    UINT32      u4EDBAddr  = 0;
    SIZE_T      z_addrSize = 0;
    DRV_CUST_OPERATION_INFO_T   t_op_info;

    
    x_dbg_stmt("_________________edb_mem_partition_create__________________ \n");

    x_memset(&t_op_info, 0, sizeof(DRV_CUST_OPERATION_INFO_T));
    t_op_info.e_op_type                                = DRV_CUSTOM_OPERATION_TYPE_MISC_SET;
    t_op_info.u.t_misc_set_info.e_misc_type            = DRV_CUSTOM_MISC_TYPE_FBM_SET_EDB_SIZE;
    t_op_info.u.t_misc_set_info.pv_set_info            = (VOID*)& u1EDBSize;
    t_op_info.u.t_misc_set_info.z_size                 = sizeof(UINT8);

    i4_ret = x_rm_set_comp(DRVT_CUST_DRV,
                      DRV_CUST_COMP_ID,
                      FALSE,
                      ANY_PORT_NUM,
                      0,
                      &t_op_info,
                      sizeof(DRV_CUST_OPERATION_INFO_T));

    if (i4_ret < RMR_OK)
    {
        return EDBR_INTERNAL_ERROR;
    }

    x_memset(&t_op_info, 0, sizeof(DRV_CUST_OPERATION_INFO_T));
    t_op_info.e_op_type                                = DRV_CUSTOM_OPERATION_TYPE_MISC_GET;
    t_op_info.u.t_misc_get_info.e_misc_type            = DRV_CUSTOM_MISC_TYPE_FBM_GET_EDB_ADDR;
    t_op_info.u.t_misc_get_info.pv_get_info            = &u4EDBAddr;
    t_op_info.u.t_misc_get_info.pz_size                = &z_addrSize;

    i4_ret = x_rm_get_comp(DRVT_CUST_DRV,
                      DRV_CUST_COMP_ID,
                      FALSE,
                      ANY_PORT_NUM,
                      0,
                      &t_op_info,
                      &z_size);

    if (i4_ret < RMR_OK)
    {
        return EDBR_INTERNAL_ERROR;
    }

    if(h_edb_mem_part !=NULL_HANDLE )
    {
        x_mem_part_delete(h_edb_mem_part) ;
    }
    if (x_mem_part_create (&h_edb_mem_part, "edb_part", (VOID*)u4EDBAddr, ((SIZE_T) (ui1_m_size*1024*1024)), 0) != OSR_OK)
    {
        DBG_ABORT( DBG_MOD_EDB );
    }

    edb_set_memory_size((SIZE_T) (ui1_m_size*1024*1024));
    z_memory_used = 0;
    
#else

    x_dbg_stmt("_________________enter edb_mem_partition_create__________________ \n");

    if(h_edb_mem_part !=NULL_HANDLE )
    {
        x_mem_part_delete(h_edb_mem_part) ;
    }

    if (x_mem_part_create (&h_edb_mem_part, "edb_part", (VOID*)pv_mem_partition, ((SIZE_T) (ui1_m_size*1024*1024)), 0) != OSR_OK)
    {
        x_dbg_stmt("EDB partition create ERROR \n");
        DBG_ABORT( DBG_MOD_EDB );
    }
   
    edb_set_memory_size((SIZE_T) (ui1_m_size*1024*1024));
    z_memory_used = 0;


#endif
    return EDBR_OK;
    
    
}

#endif
/*-----------------------------------------------------------------------------
 * Name: edb_init
 *
 * Description: initialization function of EDB component
 *
 * Inputs:  pt_edb_thread_descr References.
 *
 * Outputs: - 
 *
 * Returns: EDBR_OK              Success.
 *          EDBR_INTERNAL_ERROR  Fail.
 *          EDBR_ALREADY_INIT    Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 edb_init(
        UINT8                   ui1_edb_max_num,
        SIZE_T                  z_edb_mem_size,
        const THREAD_DESCR_T*   pt_edb_thread_descr)
{
    INT32                       i4_ret;
    UINT16                      ui2_idx;
    static handle_autofree_fct  apf_autofree_fcts[EDBT_LAST_ENTRY - HT_GROUP_EDB] = {0};
#ifdef     EDB_MEM_PART 
#ifndef    MW_EDB_FBM_SHARE_MEM
    VOID*       pv_mem_part = NULL;
#endif
#endif
    if ( b_edb_inited )
    {
        return EDBR_ALREADY_INIT;
    }


    for (ui2_idx = 0; ui2_idx < (EDBT_LAST_ENTRY - HT_GROUP_EDB); ui2_idx++) {
        apf_autofree_fcts[ui2_idx] = _edb_handle_autofree;
    }

    i4_ret = handle_set_autofree_tbl(HT_GROUP_EDB, apf_autofree_fcts);

    if ( i4_ret != HR_OK){
        return EDBR_INTERNAL_ERROR;
    }

    /* event database Selector Thread */
    if ( !pt_edb_thread_descr )
    {
        t_edb_thread_descr.ui1_priority = EDB_THREAD_DEFAULT_PRIORITY;
        t_edb_thread_descr.ui2_num_msgs = EDB_DEFAULT_NUM_OF_MSGS;
        t_edb_thread_descr.z_stack_size = EDB_THREAD_DEFAULT_STACK_SIZE;
    }
    else
    {
        t_edb_thread_descr.ui1_priority = 
            (pt_edb_thread_descr->ui1_priority == DEFAULT_PRIORITY)?
            EDB_THREAD_DEFAULT_PRIORITY:
            pt_edb_thread_descr->ui1_priority;

        t_edb_thread_descr.ui2_num_msgs = 
            (pt_edb_thread_descr->ui2_num_msgs == DEFAULT_NUM_MSGS)?
            EDB_DEFAULT_NUM_OF_MSGS:
            pt_edb_thread_descr->ui2_num_msgs;

        t_edb_thread_descr.z_stack_size = 
            (pt_edb_thread_descr->z_stack_size == DEFAULT_STACK_SIZE)?
            EDB_THREAD_DEFAULT_STACK_SIZE:
            pt_edb_thread_descr->z_stack_size;
    }

#ifdef EDB_MEM_PART 
    #ifndef    MW_EDB_FBM_SHARE_MEM
        #if 1
            if (z_edb_mem_size < EDB_MIN_MEMORY_SIZE)
            {
                edb_set_memory_size(12*1024*1024);
            }
            else
            {
                edb_set_memory_size(z_edb_mem_size);//use customize size
            }
        #else
            edb_set_memory_size(5*1024*1024);
    
        #endif   
   
        if ((pv_mem_part = x_mem_alloc (((SIZE_T) EDB_MEM_PARTITION_SIZE))) != NULL)
        {
            if (x_mem_part_create (&h_edb_mem_part, "edb_part", pv_mem_part, ((SIZE_T) EDB_MEM_PARTITION_SIZE), 0) != OSR_OK)
            {
                DBG_ABORT( DBG_MOD_EDB );
            }
        }
        else
        {
            DBG_ABORT( DBG_MOD_EDB );
        }
    #else
        #ifdef EDB_INTERNAL_DEBUG_DYNAMIC_ALLOCATION_EDB_MEMORY
        if ((pv_mem_partition = x_mem_alloc (((SIZE_T) (12*1024*1024)))) != NULL)
        {
            edb_set_memory_size(12*1024*1024);
            if (x_mem_part_create (&h_edb_mem_part, "edb_part", (VOID*)pv_mem_partition, ((SIZE_T) (12*1024*1024)), 0) != OSR_OK)
            {
                x_dbg_stmt("{EDB}____init create partition ERROR \r\n");
            }
            x_dbg_stmt("{EDB} init partition size 12M \r\n");
        }
        else
        {
            x_dbg_stmt("{EDB _____init alloc 6M memeory fail! fatal error!\r\n}");
            DBG_ABORT( DBG_MOD_EDB );
        }
        #endif
    #endif
#endif


    i4_ret = edb_eng_list_init();
    if ( i4_ret != EDBR_OK )
    {
        return EDBR_INTERNAL_ERROR;
    }

    i4_ret = edb_handle_init(EDB_DEFAULT_MAX_NUM,EDB_DEFAULT_CLIENT_MAX_NUM);
    if (i4_ret != EDBR_OK)
    {
        return EDBR_INTERNAL_ERROR;
    }

    i4_ret = x_thread_create( &h_edb_slctr_thread,
                              EDB_SLCTR_THREAD_NAME,
                              t_edb_thread_descr.z_stack_size,
                              t_edb_thread_descr.ui1_priority,
                              &_edb_slctr_thread_main,
                              0,
                              NULL );
    if ( i4_ret != OSR_OK )
    {
        i4_ret = EDBR_INTERNAL_ERROR;
        goto EDB_INIT_EXIT;
    }

    /* Initialize EVCTX CLI component */
    i4_ret = edb_cli_init();

    if (i4_ret != EDBR_OK)
    {
        return EDBR_INTERNAL_ERROR;
    }   

    b_edb_inited = TRUE;
    x_edb_set_dbg_level(DBG_INIT_LEVEL_MW_EDB);
    i4_ret = EDBR_OK;


EDB_INIT_EXIT:

    if (i4_ret != EDBR_OK)
    {
        edb_eng_list_exit();
    }

    return i4_ret;
}

#ifdef EDB_MEM_CHK
extern VOID*  edb_mem_alloc(SIZE_T z_len,CHAR* file,INT32 line)
{
    VOID* pv_obj = NULL;

    CRIT_STATE_T   t_crit_stat;
    t_crit_stat = x_crit_start();
    if (z_memory_used + z_len > EDB_MAX_MEMORY_SIZE)
    {
        DBG_INFO((_EDB_INFO"Current memory=%d,Do EDB POOL clean up\r\n",z_memory_used));
        edb_pool_clean();
    }

    pv_obj =  x_mem_dbg_alloc(z_len,file,line);
    if (z_len > (SIZE_T)200)
    {
        DBG_INFO((_EDB_INFO" z_len=%d\r\n",z_len));
    }
    if (pv_obj == NULL)
    {
        x_cli_parser("mw.os.sp");
        DBG_ABORT( DBG_MOD_EDB );
    }
    z_memory_used += z_len;
    x_crit_end( t_crit_stat );
    return pv_obj;

}
extern VOID   edb_mem_free(VOID* pt,CHAR* file,INT32 line)
{
    SIZE_T  z_size = 0;

    CRIT_STATE_T   t_crit_stat;
    t_crit_stat = x_crit_start();
    z_size = os_mem_get_mem_ptr_size(pt);
    if (z_memory_used > z_size)
    {
        z_memory_used -= z_size;
    }

    x_mem_dbg_free(pt,file,line);
    x_crit_end( t_crit_stat );
}


#else
VOID*   edb_mem_alloc(SIZE_T z_len)
{
    VOID* pv_obj = NULL;
    CRIT_STATE_T   t_crit_stat;
    t_crit_stat = x_crit_start();

    if (z_memory_used + z_len > EDB_MEM_PARTITION_SIZE)
    {
        //DBG_INFO((_EDB_INFO"Current memory=%d,Do EDB POOL clean up\r\n",z_memory_used));
    }
    x_crit_end( t_crit_stat );

#ifdef EDB_MEM_PART   
    if(h_edb_mem_part == NULL_HANDLE)
    {
        return NULL;
    }
    pv_obj = x_mem_part_alloc(h_edb_mem_part, z_len);
#else
    pv_obj = x_mem_alloc(z_len);
#endif

    if (pv_obj == NULL)
    {
        x_dbg_stmt("EDB memory alloc fail**************************\r\n");
        x_cli_parser("mw.os.sp");
        x_dbg_stmt("EDB memory used=%d\r\n",z_memory_used);
        x_dbg_stmt("EDB partition size=%d\r\n",EDB_MEM_PARTITION_SIZE);
        x_dbg_stmt("EDB alloc size=%d\r\n",z_len);
        edb_show_part_info();
        x_dbg_stmt("EDB clean pool and restart********************\r\n");
        edb_pool_restart();
        return NULL;
        //DBG_ABORT( DBG_MOD_EDB );
    }
    t_crit_stat = x_crit_start();
#ifdef CLI_SUPPORT
    #ifdef MEASURE_OVERHEAD
        z_len = (SIZE_T)(os_mem_get_mem_ptr_size(pv_obj) + 12);
    #else
        z_len = (SIZE_T)(os_mem_get_mem_ptr_size(pv_obj) + 20);
    #endif
#else
    z_len = (SIZE_T)(os_mem_get_mem_ptr_size(pv_obj) + 12);
#endif    
    z_memory_used += z_len;
    x_crit_end( t_crit_stat );
    return pv_obj;

}

VOID   edb_mem_free(VOID* pt)
{
    SIZE_T  z_size = 0;

    CRIT_STATE_T   t_crit_stat;
    if (pt == NULL){return;}
    t_crit_stat = x_crit_start();

#ifdef CLI_SUPPORT
#ifdef MEASURE_OVERHEAD
    z_size = (SIZE_T)(os_mem_get_mem_ptr_size(pt) + 12);
#else
    z_size = (SIZE_T)(os_mem_get_mem_ptr_size(pt) + 20);
#endif
#else
    z_size = (SIZE_T)(os_mem_get_mem_ptr_size(pt) + 12);
#endif    
    if (z_memory_used > z_size)
    {
        z_memory_used -= z_size;
    }

    x_crit_end( t_crit_stat );

    if (pt != NULL)
    {
        x_mem_free(pt);
    }
}

#endif

SIZE_T edb_get_mem(VOID)
{
    return z_memory_used;
}



/*-----------------------------------------------------------------------------
 * Name: edb_is_inited
 *
 * Description: API to return the initialziation status of EDB component
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: TRUE        The EDB component had been initialized.
 *          FALSE       The EDB component is not initialized.
 *
 ----------------------------------------------------------------------------*/
BOOL edb_is_inited(VOID)
{
    return b_edb_inited;
}



/*-----------------------------------------------------------------------------
 * Name: x_edb_get_dbg_level
 *
 * Description: This API returns the current setting of debug level.
 *
 * Inputs: -  
 *
 * Outputs: -
 *
 * Returns: The current setting of debug level.
 ----------------------------------------------------------------------------*/ 
UINT16 x_edb_get_dbg_level(VOID)
{
    if (edb_is_inited())
    {    
        return ui2_edb_dbg_level;    
    }      
    else
    {
        return 0;
    }    
}


/*-----------------------------------------------------------------------------
 * Name: x_edb_set_dbg_level
 *
 * Description: This API changes the setting of debug level to new one.
 *
 * Inputs: ui2_level            The new setting of debug level.  
 *
 * Outputs: -
 *
 * Returns: TRUE            Routine successful.
 *          FALSE           Routine failed.
 ----------------------------------------------------------------------------*/ 
BOOL x_edb_set_dbg_level(UINT16 ui2_level)
{
    if (edb_is_inited())
    {        
        ui2_edb_dbg_level = ui2_level;    
        return TRUE;
    }    
    else
    {
        return FALSE;
    }
}

SIZE_T edb_get_memory_size(VOID)
{
    return z_memory_size;
}


VOID edb_set_memory_size(SIZE_T z_size)
{
    if (z_size < EDB_MIN_MEMORY_SIZE)
    {
        DBG_ABORT( DBG_MOD_EDB );
    }
    z_memory_size = z_size;
    return;
}

#ifdef MW_EPG_CACHE_NVM_SUPPORT 
SIZE_T edb_get_flash_size_for_pool(VOID)
{
    return z_event_size_for_flash;
}


VOID edb_set_flash_size_for_pool(SIZE_T z_size)
{
    z_event_size_for_flash = z_size;
    return;
}
#endif





