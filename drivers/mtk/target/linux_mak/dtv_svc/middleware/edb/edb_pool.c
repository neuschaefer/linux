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
#include "os/inc/x_os.h"
#include "handle/u_handle_grp.h"
#include "hash/hash_api.h"
#include "edb/edb_hash_api.h"
#include "dbg/x_dbg.h"
#include "x_edb.h"
#include "edb_pool.h"
#include "edb_slctr.h"




#define MAX_EVENT_SIZE_FOR_FLASH    (6000) //eit max is 4096 bytes , we assume that a single event max size is 6000 bytes


/*-----------------------------------------------------------------------------
  macros, defines, typedefs, enums
  ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
  data declarations
  ----------------------------------------------------------------------------*/
#ifdef EDB_INTERNAL_MONITOR_EVENT
extern UINT16           ui2_monitor_event_id ;
#endif
static HANDLE_T         h_edb_pool_lock = NULL_HANDLE;

UINT16          g_edb_monitor_on_id = 0;/*0x233a;*/
UINT16          g_edb_monitor_ts_id = 0;/*0x301;*/
UINT16          g_edb_monitor_svc_id =0;/*768;*/



/*-----------------------------------------------------------------------------
  functions declaraions
  ----------------------------------------------------------------------------*/


static VOID _edb_pool_del_chs(
        VOID*             pv_obj
        );




static INT32 edb_pool_free_single_ch_obj(
        EDB_EVENT_POOL_CH_T*    pt_channel_remove
        );



static VOID _edb_pool_del_chs(
        VOID*             pv_obj
        )
{
    EDB_EVENT_POOL_CH_T*            pt_channel  = NULL;

    pt_channel          = (EDB_EVENT_POOL_CH_T*)pv_obj;

    if (pt_channel != NULL)
    {
        edb_pool_free_single_ch_obj(pt_channel);
    }
    return ;
}

INT32 edb_pool_free_single_event_obj(
        x_edb_eng_free_single_event  pf_free,
        EDB_POOL_EVENT_INFO_T*       pt_event)
{
    EDB_CHECK_RET( 
            (pf_free == NULL),
            (EDBR_INV_ARG),
            (("Arguments is wrong\r\n"))
            );
    (pf_free)(pt_event);

    return EDBR_OK;
}

static INT32 edb_pool_free_single_ch_obj(
        EDB_EVENT_POOL_CH_T*                pt_channel_remove
        )
{
    HANDLE_T        h_event_hash = NULL_HANDLE;

    if (pt_channel_remove != NULL)
    {

        if (pt_channel_remove->pv_data != NULL)
        {
            EDB_PARTITION_FREE(pt_channel_remove->pv_data);
        }

		if (pt_channel_remove->t_channel_guidance_info.ps_event_guidance != NULL)
		{
			EDB_PARTITION_FREE(pt_channel_remove->t_channel_guidance_info.ps_event_guidance);
			pt_channel_remove->t_channel_guidance_info.ps_event_guidance = NULL;
		}

        /*Delete all events attach this channel*/
        /*iterator all hash table and delete events*/
        h_event_hash = pt_channel_remove->h_event_hash;

        if (h_event_hash != NULL_HANDLE)
        {
            if (pt_channel_remove->pf_free_event == NULL)
            {
                DBG_ABORT( DBG_MOD_EDB );
            }
            edb_hash_delete(h_event_hash, pt_channel_remove->pf_free_event);
            pt_channel_remove->h_event_hash = NULL_HANDLE;
            /*DLIST_INIT( & (pt_channel_remove->t_event_list));*/
        }

        /*Delete this channel*/
        EDB_PARTITION_FREE(pt_channel_remove);

    }

    return EDBR_OK;
}


INT32 edb_pool_init(VOID)
{
    INT32 i4_ret = 0;
    i4_ret = edb_pool_lock_init();

    return i4_ret;
}



INT32 edb_pool_lock_init(VOID)
{
    if (rwl_create_lock(&h_edb_pool_lock) != RWLR_OK)
    {
        DBG_ABORT( DBG_MOD_EDB );
    }
    return EDBR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: edb_pool_lock_rd_db
 *
 * Description: This API locks device state list for read operation.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID edb_pool_lock_rd(VOID)
{
    if (rwl_read_lock(h_edb_pool_lock, RWL_OPTION_WAIT) != RWLR_OK)
    {
        DBG_ABORT( DBG_MOD_EDB );
    }
}


/*-----------------------------------------------------------------------------
 * Name: edb_pool_lock_wr_db
 *
 * Description: This API locks device state list for write or update operation.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID edb_pool_lock_wr(VOID)
{
    if (rwl_write_lock(h_edb_pool_lock, RWL_OPTION_WAIT) != RWLR_OK)
    {
        DBG_ABORT( DBG_MOD_EDB );
    }
}


/*-----------------------------------------------------------------------------
 * Name: edb_pool_unlock
 *
 * Description: This API unlocks device state list that was locked for read or
 *              write operation before.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID edb_pool_unlock(VOID)
{
    if (rwl_release_lock(h_edb_pool_lock) != RWLR_OK)
    {
        DBG_ABORT( DBG_MOD_EDB );
    }
}

/*
   edb_pool add get del lookup
   function should be wrap in R/W lock
   */

INT32    edb_pool_create(
        UINT16                  ui2_total_channels_cap,
        TIME_T                  t_last_time_stamp,
        EDB_EVENT_POOL_T*       pt_pool)
{
    INT32                   i4_ret    = 0;

    EDB_CHECK_RET( 
            (ui2_total_channels_cap <= 0 ),
            (EDBR_INV_ARG),
            (("Arguments is wrong\r\n"))
            );

    pt_pool->ui2_total_channels   = 0;
    pt_pool->ui4_last_time_stamp  = x_os_get_sys_tick();
    i4_ret = edb_hash_create(ui2_total_channels_cap,&(pt_pool->h_ch_hash));

    if (i4_ret != HASH_OK)
    {
        return EDBR_INTERNAL_ERROR;
    }


    return EDBR_OK;
}


INT32    edb_pool_delete(
        EDB_EVENT_POOL_T*      pt_pool)
{
    INT32                   i4_ret    = 0;
    HANDLE_T                h_ch_hash = NULL_HANDLE;

    EDB_CHECK_RET( 
            (pt_pool == NULL ),
            (EDBR_INV_ARG),
            (("Arguments is wrong\r\n"))
            );

    h_ch_hash = pt_pool->h_ch_hash;
    EDB_CHECK_RET( 
            (h_ch_hash == NULL_HANDLE ),
            (EDBR_INTERNAL_ERROR),
            (("Pool channel hash table is null\r\n"))
            );


    /*Delete all channel data*/
    edb_hash_delete(h_ch_hash, _edb_pool_del_chs);
    pt_pool->h_ch_hash = NULL_HANDLE;
    pt_pool->ui2_total_channels =0;

    return i4_ret;
}




INT32    edb_pool_get_ch_obj(
        EDB_EVENT_POOL_T*       pt_edb_pool, 
        EDB_EVENT_POOL_CH_KEY*  pt_ch_key,
        EDB_EVENT_POOL_CH_T**   ppt_ch_obj)
{
    INT32                   i4_ret      = EDBR_INFO_NOT_FOUND;
    HANDLE_T                h_ch_hash   = NULL_HANDLE;
    EDB_EVENT_POOL_CH_T*    pt_channel  = NULL;

    EDB_CHECK_RET( 
            (pt_edb_pool==NULL || pt_ch_key==NULL || ppt_ch_obj == NULL),
            (EDBR_INV_ARG),
            (("Arguments is wrong\r\n"))
            );

    h_ch_hash = pt_edb_pool->h_ch_hash;
    EDB_CHECK_RET((h_ch_hash==NULL_HANDLE), EDBR_NOT_INIT, (("CH Hash is not init\r\n")));

    i4_ret = edb_hash_get_obj(h_ch_hash, (const INT8 *)pt_ch_key, sizeof(EDB_EVENT_POOL_CH_KEY), (VOID**)(&pt_channel));
    if (i4_ret != EDBR_OK)
    {
        return i4_ret;
    }

    *ppt_ch_obj = pt_channel;
    i4_ret = EDBR_OK;
    return i4_ret;
}

INT32 edb_pool_add_ch_obj(
        EDB_EVENT_POOL_T*       pt_edb_pool, 
        EDB_EVENT_POOL_CH_T*    pt_channel)
{
    INT32                   i4_ret      = 0;
    HANDLE_T                h_ch_hash   = NULL_HANDLE;
    EDB_EVENT_POOL_CH_T*    pt_channel_find = NULL;

    EDB_CHECK_RET( 
            (pt_edb_pool==NULL || pt_channel==NULL),
            (EDBR_INV_ARG),
            (("Arguments is wrong\r\n"))
            );

    h_ch_hash = pt_edb_pool->h_ch_hash;
    EDB_CHECK_RET((h_ch_hash==NULL_HANDLE), EDBR_INV_ARG, (("CH Hash is not init\r\n")));

    i4_ret = edb_pool_get_ch_obj(pt_edb_pool, &(pt_channel->t_key), &pt_channel_find);
    EDB_CHECK_RET((pt_channel_find != NULL), EDBR_ALREADY_EXIST, (("channel already exist\r\n")));

    i4_ret = edb_hash_add_obj(h_ch_hash,(const INT8 *) (&pt_channel->t_key), 
                              sizeof(EDB_EVENT_POOL_CH_KEY), pt_channel, NULL);

    EDB_CHECK_RET( 
            (i4_ret != HASH_OK),
            (i4_ret),
            (("Can not add channel info %d\r\n",i4_ret))
            );

    return EDBR_OK;
}

INT32 edb_pool_del_ch_obj(
        EDB_EVENT_POOL_T*       pt_edb_pool, 
        EDB_EVENT_POOL_CH_T*    pt_channel)
{
    INT32                   i4_ret              = 0;
    EDB_EVENT_POOL_CH_T*    pt_channel_remove   = NULL; 
    HANDLE_T                h_ch_hash           = NULL_HANDLE;  


    EDB_CHECK_RET( 
            (pt_edb_pool==NULL || pt_channel==NULL),
            (EDBR_INV_ARG),
            (("Arguments is wrong\r\n"))
            );

    h_ch_hash = pt_edb_pool->h_ch_hash;
    EDB_CHECK_RET((h_ch_hash==NULL_HANDLE), EDBR_INV_ARG, (("CH Hash is not init\r\n")));

    i4_ret = edb_pool_get_ch_obj(pt_edb_pool,&(pt_channel->t_key), &pt_channel_remove);
    EDB_CHECK_RET( 
            (pt_channel_remove == NULL),
            (i4_ret),
            (("Can not get channel info from pool\r\n"))
            );

    /*remove this channel from hash table*/
    i4_ret  = edb_hash_remove_obj(h_ch_hash, (const INT8 *) (&(pt_channel_remove->t_key)),
                                  sizeof(EDB_EVENT_POOL_CH_KEY), NULL);

    i4_ret = edb_pool_free_single_ch_obj(pt_channel_remove);

    return i4_ret;
}



INT32    edb_pool_look_ch_obj_with_smallest_time_stamp(
        EDB_EVENT_POOL_T*       pt_edb_pool, 
        EDB_EVENT_POOL_CH_T**   ppt_ch_obj)
{
    INT32                   i4_ret      = EDBR_INFO_NOT_FOUND;
    HANDLE_T                h_ch_hash   = NULL_HANDLE;
    EDB_EVENT_POOL_CH_T*    pt_channel  = NULL;
    UINT32                  ui4_i           = 0;
    P_ELEMENT_OBJ_T         pt_elm          = NULL;
    HANDLE_TYPE_T           e_hdl_type;
    EDB_HASH_T*             pt_hash_obj     = NULL;
    UINT32                  ui4_time_stamp  = x_os_get_sys_tick();
    SIZE_T                  z_size          = 0x0;
    EDB_EVENT_POOL_CH_T*    pt_channel_aim  = NULL;

    EDB_CHECK_RET( 
            (pt_edb_pool==NULL || ppt_ch_obj == NULL),
            (EDBR_INV_ARG),
            (("Arguments is wrong\r\n"))
            );

    h_ch_hash = pt_edb_pool->h_ch_hash;
    EDB_CHECK_RET((h_ch_hash==NULL_HANDLE), EDBR_NOT_INIT, (("CH Hash is not init\r\n")));


    i4_ret = handle_get_type_obj( h_ch_hash, &e_hdl_type, (VOID**)&pt_hash_obj );
    if ( i4_ret != HR_OK || e_hdl_type != EDBT_HASH || pt_hash_obj == NULL)
    {
        return EDBR_INV_HANDLE;
    }

    for ( ui4_i=0; ui4_i<pt_hash_obj->z_table_size; ++ui4_i )
    {
        for( pt_elm = pt_hash_obj->ppt_table[ui4_i]; pt_elm != NULL; )
        {
            pt_channel = (EDB_EVENT_POOL_CH_T*)pt_elm->pv_obj;
            if (pt_channel != NULL && pt_channel->h_event_hash != NULL_HANDLE)
            {
                edb_hash_get_size(pt_channel->h_event_hash , &z_size);
            }

            if (z_size > 0x0 && pt_channel != NULL && pt_channel->ui4_time_stamp < ui4_time_stamp )
            {
                ui4_time_stamp = pt_channel->ui4_time_stamp;
                pt_channel_aim = pt_channel;
            }
            pt_elm = pt_elm->pt_next;
        }
    }

    if (pt_channel_aim != NULL)
    {
        i4_ret = EDBR_OK;
        *ppt_ch_obj = pt_channel_aim;
    }

    return i4_ret;
}


INT32    edb_pool_look_event_obj_with_farthest_start_time(
        EDB_EVENT_POOL_CH_T*        pt_ch_obj,
        EDB_POOL_EVENT_INFO_T**     ppt_event_obj_farthest)
{
    INT32                   i4_ret          = EDBR_INFO_NOT_FOUND;
    HANDLE_T                h_event_hash    = NULL_HANDLE;
    EDB_POOL_EVENT_INFO_T*  pt_event        = NULL;
    UINT32                  ui4_i           = 0;
    P_ELEMENT_OBJ_T         pt_elm          = NULL;
    HANDLE_TYPE_T           e_hdl_type;
    EDB_HASH_T*             pt_hash_obj     = NULL;
    TIME_T                  t_start_time    = NULL_TIME;

    EDB_CHECK_RET( 
            (pt_ch_obj==NULL || ppt_event_obj_farthest == NULL),
            (EDBR_INV_ARG),
            (("Arguments is wrong\r\n"))
            );

    h_event_hash = pt_ch_obj->h_event_hash;
    EDB_CHECK_RET((h_event_hash==NULL_HANDLE), EDBR_NOT_INIT, (("CH Hash is not init\r\n")));


    i4_ret = handle_get_type_obj( h_event_hash, &e_hdl_type, (VOID**)&pt_hash_obj );
    if ( i4_ret != HR_OK || e_hdl_type != EDBT_HASH || pt_hash_obj == NULL)
    {
        return EDBR_INV_HANDLE;
    }

    for ( ui4_i=0; ui4_i<pt_hash_obj->z_table_size; ++ui4_i )
    {
        for( pt_elm = pt_hash_obj->ppt_table[ui4_i]; pt_elm != NULL; )
        {
            pt_event = (EDB_POOL_EVENT_INFO_T*)pt_elm->pv_obj;

            if (pt_event != NULL && pt_event->t_start_time >  t_start_time )
            {
                t_start_time = pt_event->t_start_time;
            }
            pt_elm = pt_elm->pt_next;
        }
    }

    if (pt_event != NULL)
    {
        i4_ret = EDBR_OK;
        *ppt_event_obj_farthest = pt_event;
    }

    return i4_ret;
}





INT32 edb_pool_get_present_event_obj(
        EDB_EVENT_POOL_CH_T*    pt_channel,
        EDB_POOL_EVENT_INFO_T**      ppt_event)
{
    EDB_CHECK_RET( 
            (pt_channel==NULL || ppt_event == NULL),
            (EDBR_INV_ARG),
            (("Arguments is wrong\r\n"))
            );

    *ppt_event = pt_channel->pt_present_event;
    return EDBR_OK;
}

INT32 edb_pool_get_following_event_obj(
        EDB_EVENT_POOL_CH_T*            pt_channel,
        EDB_POOL_EVENT_INFO_T**       ppt_event)
{
    EDB_CHECK_RET( 
            (pt_channel==NULL),
            (EDBR_INV_ARG),
            (("Arguments is wrong\r\n"))
            );


    *ppt_event = pt_channel->pt_following_event;
    return EDBR_OK;
}

INT32 edb_pool_add_present_event_obj(
        EDB_EVENT_POOL_CH_T*            pt_channel,
        EDB_POOL_EVENT_INFO_T*        pt_event_obj)
{
    INT32   i4_ret = 0;
    EDB_CHECK_RET( 
            (pt_channel==NULL || pt_event_obj==NULL),
            (EDBR_INV_ARG),
            (("Arguments is wrong\r\n"))
            );

    i4_ret = edb_pool_add_event_obj(pt_channel, pt_event_obj);
    if (i4_ret == EDBR_OK)
    {
        pt_channel->pt_present_event = pt_event_obj;
        pt_channel->fg_present_event_from_pf = TRUE;

        if (pt_channel->t_key.ui2_on_id == MONITOR_ON_ID &&
            pt_channel->t_key.ui2_ts_id == MONITOR_TS_ID &&  
            pt_channel->t_key.ui2_svc_id== MONITOR_SVC_ID
           )   
        {
            x_dbg_stmt("Add present event id:0x%04x\r\n",pt_event_obj->ui2_event_id);   
        }

    }
    //DBG_INFO((_EDB_INFO"Ch id=0x%08x\tpresent_event id =0x%04x\r\n",pt_channel->ui4_channel_id, pt_channel->pt_present_event->ui2_event_id));
    return i4_ret;
}

INT32 edb_pool_add_following_event_obj(
        EDB_EVENT_POOL_CH_T*            pt_channel,
        EDB_POOL_EVENT_INFO_T*        pt_event_obj)
{
    INT32   i4_ret = EDBR_OK;
    EDB_CHECK_RET( 
            (pt_channel==NULL || pt_event_obj==NULL),
            (EDBR_INV_ARG),
            (("Arguments is wrong\r\n"))
            );
    i4_ret = edb_pool_add_event_obj(pt_channel, pt_event_obj);
    if (i4_ret == EDBR_OK)
    {
        pt_channel->pt_following_event = pt_event_obj;

        if (
                pt_channel->t_key.ui2_on_id == MONITOR_ON_ID &&
                pt_channel->t_key.ui2_ts_id == MONITOR_TS_ID &&  
                pt_channel->t_key.ui2_svc_id== MONITOR_SVC_ID
           )   
        {
            x_dbg_stmt("Add follow event id:0x%04x\r\n",pt_event_obj->ui2_event_id);   
        }
    }

    //DBG_INFO((_EDB_INFO"Ch id=0x%08x\tfollowing_event id =0x%04x\r\n",pt_channel->ui4_channel_id,pt_channel->pt_following_event->ui2_event_id));
    return i4_ret;
}

INT32 edb_pool_del_present_event_obj(
        EDB_EVENT_POOL_CH_T*    pt_channel)
{
    INT32                           i4_ret           = 0;
    EDB_POOL_EVENT_INFO_T*        pt_present_event = NULL;
    EDB_CHECK_RET( 
            (pt_channel==NULL),
            (EDBR_INV_ARG),
            (("Arguments is wrong\r\n"))
            );

    pt_present_event = pt_channel->pt_present_event;
    EDB_CHECK_RET( 
            (pt_present_event == NULL),
            (EDBR_INFO_NOT_FOUND),
            (("Can not delete present event,because present event is null\r\n"))
            );

    i4_ret = edb_pool_del_event_obj(pt_channel,pt_present_event);
    if(i4_ret == EDBR_OK)
    {
        pt_channel->pt_present_event = NULL;
        pt_channel->fg_present_event_from_pf = FALSE;
    }

    return i4_ret;
}

INT32 edb_pool_del_following_event_obj(
        EDB_EVENT_POOL_CH_T*    pt_channel)
{
    INT32                           i4_ret              = 0;
    EDB_POOL_EVENT_INFO_T*        pt_following_event  = NULL;
    EDB_CHECK_RET( 
            (pt_channel==NULL),
            (EDBR_INV_ARG),
            (("Arguments is wrong\r\n"))
            );

    pt_following_event = pt_channel->pt_following_event;
    EDB_CHECK_RET( 
            (pt_following_event == NULL),
            (EDBR_INFO_NOT_FOUND),
            (("Can not delete pt_following_event event,because pt_following_event event is null\r\n"))
            );

    i4_ret = edb_pool_del_event_obj(pt_channel,pt_following_event);
    if(i4_ret == EDBR_OK)
    {
        pt_channel->pt_following_event = NULL;
    }

    return i4_ret;

}

INT32 edb_pool_get_event_obj(
        EDB_EVENT_POOL_CH_T*            pt_channel,
        UINT16                          ui2_event_id,
        EDB_POOL_EVENT_INFO_T**         ppt_event)
{
    INT32                i4_ret       = 0;
    HANDLE_T             h_event_hash = NULL_HANDLE;

    EDB_CHECK_RET( 
            (pt_channel==NULL || ppt_event == NULL),
            (EDBR_INV_ARG),
        (("Arguments is wrong\r\n"))
    );

    h_event_hash = pt_channel->h_event_hash;
    EDB_CHECK_RET( 
        (h_event_hash==NULL_HANDLE),
        (EDBR_INFO_NOT_FOUND),
        (("Channel event hash table is null\r\n"))
    );

    i4_ret = edb_hash_get_obj(h_event_hash, (const INT8 *) (&ui2_event_id), sizeof(UINT16), (VOID**) ppt_event);
    
    return i4_ret;

}
    
INT32 edb_pool_add_event_obj(
    EDB_EVENT_POOL_CH_T*            pt_channel,
    EDB_POOL_EVENT_INFO_T*        pt_event_obj)
{
    INT32                   i4_ret             = 0;
    HANDLE_T                h_event_hash       = NULL_HANDLE;
    EDB_POOL_EVENT_INFO_T*  pt_event_obj_get   = NULL;
    SIZE_T                  z_event_num        = 0x0;
    EDB_POOL_EVENT_INFO_T*  pt_event_obj_farthest   = NULL;
    
    EDB_CHECK_RET( 
        (pt_channel==NULL || pt_event_obj == NULL),
        (EDBR_INV_ARG),
        (("Arguments is wrong\r\n"))
    );

    h_event_hash = pt_channel->h_event_hash;
    EDB_CHECK_RET( 
        (h_event_hash==NULL_HANDLE),
        (EDBR_INFO_NOT_FOUND),
        (("Channel event hash table is null\r\n"))
    );

    i4_ret = edb_pool_get_event_obj(pt_channel, pt_event_obj->ui2_event_id, &pt_event_obj_get);
    if(pt_event_obj_get != NULL) /*Delet old event with the save event id*/
    {
        #if 0
         DBG_INFO((_EDB_INFO"Event Exist,delete it  onid[0x%02x] tsid[0x%02x] svcid[0x%02x] event id[0x%08x]\n\r",
             pt_channel->t_key.ui2_on_id,
                     pt_channel->t_key.ui2_ts_id,
                     pt_channel->t_key.ui2_svc_id,
                     pt_event_obj->ui2_event_id
                     ));
        #endif
        i4_ret = edb_pool_del_event_obj(pt_channel, pt_event_obj_get);
    }

    edb_hash_get_size(h_event_hash, &z_event_num);
    if (z_event_num >= (SIZE_T)EDB_POOL_MAX_CHANNEL_EVENT)//event hash is full clean farthest one
    {
        edb_pool_look_event_obj_with_farthest_start_time(pt_channel,&pt_event_obj_farthest);
        if (pt_event_obj_farthest != NULL)
        {
            edb_pool_del_event_obj(pt_channel,pt_event_obj_farthest);
        }
    }
    
    i4_ret = edb_hash_add_obj(h_event_hash, (const INT8 *)(&(pt_event_obj->ui2_event_id)),
                sizeof(UINT16), pt_event_obj, NULL);
    

    return i4_ret;

}




INT32 edb_pool_del_event_obj(
    EDB_EVENT_POOL_CH_T*     pt_channel,
    EDB_POOL_EVENT_INFO_T*        pt_event_obj)
{
    INT32                           i4_ret       = 0;
    //EDB_EVENT_POOL_EVENT_LIST_T     t_event_list = {0};
    HANDLE_T                        h_event_hash = NULL_HANDLE;
    
    EDB_CHECK_RET( 
        (pt_channel==NULL||pt_event_obj==NULL),
        (EDBR_INV_ARG),
        (("Arguments is wrong\r\n"))
    );
    
    /*Remove from hash table*/
    h_event_hash = pt_channel->h_event_hash;
    if(h_event_hash != NULL_HANDLE)
    {
        i4_ret = edb_hash_remove_obj(h_event_hash, 
                            (const INT8 *) &(pt_event_obj->ui2_event_id), sizeof(UINT16),NULL);
    }

    if (pt_channel->pt_present_event != NULL)
    {
        if (pt_event_obj->ui2_event_id == pt_channel->pt_present_event->ui2_event_id)
        {
            pt_channel->pt_present_event = NULL;
        }
    }

    if (pt_channel->pt_following_event != NULL)
    {
        if (pt_event_obj->ui2_event_id == pt_channel->pt_following_event->ui2_event_id)
        {
            pt_channel->pt_following_event = NULL;
        }
    }
    

    EDB_CHECK_RET( 
        (i4_ret != HASH_OK),
        (i4_ret),
        (("Remove event from channle fail\r\n"))
    );

    /*Free memory*/
    if (pt_channel->pf_free_event == NULL)
    {
        DBG_ABORT( DBG_MOD_EDB );
    }
    i4_ret = edb_pool_free_single_event_obj(pt_channel->pf_free_event,pt_event_obj);
    
    return i4_ret;

}


/*

       
*/
INT32 edb_pool_lookup_event_by_time(
    EDB_EVENT_POOL_CH_T*            pt_channel,
    TIME_T                          t_time,
    EDB_POOL_EVENT_INFO_T**         ppt_event)
{
#if 0
    EDB_POOL_EVENT_INFO_T*    pt_event     = NULL;
    EDB_EVENT_POOL_EVENT_LIST_T t_event_list = {0};
    BOOL                        b_found      = FALSE;
    EDB_CHECK_RET( 
        (pt_channel==NULL),
        (EDBR_INV_ARG),
        (("Arguments is wrong\r\n"))
    );
    t_event_list = pt_channel->t_event_list;

    EDB_CHECK_RET( 
            (DLIST_IS_EMPTY(&t_event_list)),
            (EDBR_INFO_NOT_FOUND),
            (("Channel event list is null\r\n"))
    );


    DLIST_FOR_EACH(pt_event, &t_event_list, t_link)
    {
        if (pt_event->t_start_time <= t_time &&
            pt_event->t_start_time + pt_event->t_duration >= t_time)
        {
            b_found = TRUE;
            break;
        }
    }

    if (b_found == TRUE)
    {
        *ppt_event = pt_event;
        return EDBR_OK;
    }
#endif    
    return EDBR_INFO_NOT_FOUND;
}


edb_restart_fct edb_dvb_restart_fct = NULL;

extern VOID edb_pool_restart(VOID)
{
#if 0
    extern INT32 edb_dvb_restart(VOID);
    edb_dvb_restart();
#endif
	if (edb_dvb_restart_fct != NULL)
	{
		edb_dvb_restart_fct();
	}
    return;
}



INT32 edb_serialize_pool_event(EDB_POOL_EVENT_INFO_T*  pt_pool_event ,
                                UINT8*  pui1_serialized_for_flash ,
                                UINT16* pui2_event_length)
{
    SIZE_T    z_size;
    UINT8     ui1_num;
    UINT16    ui2_num;
    UINT16    ui2_event_length;
    UINT16    ui2_store_event_length; 
    UINT8*    pu1_start_address = NULL;
    UINT8*    pui1_serialized_event  = NULL;
    UINT8*    pui1_store_event_length = NULL;

    pu1_start_address = x_mem_alloc(MAX_EVENT_SIZE_FOR_FLASH);
    if(pu1_start_address == NULL)
    {
        *pui2_event_length = 0;
        return EDBR_OUT_OF_MEM;
    }
    x_memset(pu1_start_address ,0x0,MAX_EVENT_SIZE_FOR_FLASH);
    pui1_serialized_event = pu1_start_address;
    
    x_memcpy(pui1_serialized_event, &(pt_pool_event->ui2_event_id), sizeof(UINT16));
    pui1_serialized_event += sizeof(UINT16);

    pui1_store_event_length = pui1_serialized_event;
    pui1_serialized_event += sizeof(ui2_store_event_length);
    
    x_memcpy(pui1_serialized_event, &(pt_pool_event->ui1_table_id), sizeof(UINT8));
    pui1_serialized_event += sizeof(UINT8);
    x_memcpy(pui1_serialized_event, &(pt_pool_event->ui1_sect_num), sizeof(UINT8));
    pui1_serialized_event += sizeof(UINT8);
    x_memcpy(pui1_serialized_event, &(pt_pool_event->t_start_time), sizeof(TIME_T));
    pui1_serialized_event += sizeof(TIME_T);
    x_memcpy(pui1_serialized_event, &(pt_pool_event->t_duration), sizeof(TIME_T));
    pui1_serialized_event += sizeof(TIME_T);
    x_memcpy(pui1_serialized_event, &(pt_pool_event->b_caption), sizeof(BOOL));
    pui1_serialized_event += sizeof(BOOL);
    x_memcpy(pui1_serialized_event, &(pt_pool_event->b_free_ca_mode), sizeof(BOOL));
    pui1_serialized_event += sizeof(BOOL);
    x_memcpy(pui1_serialized_event, &(pt_pool_event->b_has_private), sizeof(BOOL));
    pui1_serialized_event += sizeof(BOOL);
    x_memcpy(pui1_serialized_event, &(pt_pool_event->ui1_num_rating), sizeof(UINT8));
    pui1_serialized_event += sizeof(UINT8);
    x_memcpy(pui1_serialized_event, (pt_pool_event->aui2_ca_system_id), sizeof(pt_pool_event->aui2_ca_system_id)); 
    pui1_serialized_event += sizeof(pt_pool_event->aui2_ca_system_id); 
    x_memcpy(pui1_serialized_event, &(pt_pool_event->ui1_num_event_category), sizeof(UINT8));
    pui1_serialized_event += sizeof(UINT8);
    x_memcpy(pui1_serialized_event, (pt_pool_event->aui1_event_category), sizeof(pt_pool_event->aui1_event_category)); 
    pui1_serialized_event += sizeof(pt_pool_event->aui1_event_category); 
    x_memcpy(pui1_serialized_event, &(pt_pool_event->ui1_guidance_mode), sizeof(UINT8));
    pui1_serialized_event += sizeof(UINT8);
    
    if(pt_pool_event->ps_event_title!=NULL)
    {
        z_size = x_strlen(pt_pool_event->ps_event_title);
        x_memcpy(pui1_serialized_event, &z_size, sizeof(SIZE_T));
        pui1_serialized_event += sizeof(SIZE_T);
        x_memcpy(pui1_serialized_event, pt_pool_event->ps_event_title, z_size);
        pui1_serialized_event += z_size;
    }
    else 
    {
        z_size = 0;
        x_memcpy(pui1_serialized_event, &z_size, sizeof(SIZE_T));
        pui1_serialized_event += sizeof(SIZE_T);
    }
    if(pt_pool_event->ps_event_detail!=NULL)
    {
        z_size = x_strlen(pt_pool_event->ps_event_detail);
        x_memcpy(pui1_serialized_event, &z_size, sizeof(SIZE_T));
        pui1_serialized_event += sizeof(SIZE_T); 
        x_memcpy(pui1_serialized_event, pt_pool_event->ps_event_detail, z_size);
        pui1_serialized_event += z_size;
    }
    else 
    {
        z_size = 0;
        x_memcpy(pui1_serialized_event, &z_size, sizeof(SIZE_T));
        pui1_serialized_event += sizeof(SIZE_T);
    }

    if(pt_pool_event->ps_event_guidance!=NULL)
    {
        z_size = x_strlen(pt_pool_event->ps_event_guidance);
        x_memcpy(pui1_serialized_event, &z_size, sizeof(SIZE_T));
        pui1_serialized_event += sizeof(SIZE_T); 
        x_memcpy(pui1_serialized_event, pt_pool_event->ps_event_guidance, z_size);
        pui1_serialized_event += z_size;
    }
    else 
    {
        z_size = 0;
        x_memcpy(pui1_serialized_event, &z_size, sizeof(SIZE_T));
        pui1_serialized_event += sizeof(SIZE_T);
    }

    if(pt_pool_event->pt_event_linkage_list != NULL && 
       pt_pool_event->pt_event_linkage_list->ui1_num_event_linkage !=0 && 
       pt_pool_event->pt_event_linkage_list->pt_event_linkage != NULL)
    {
        ui1_num = pt_pool_event->pt_event_linkage_list->ui1_num_event_linkage;
        x_memcpy(pui1_serialized_event, &ui1_num, sizeof(UINT8));
        pui1_serialized_event += sizeof(UINT8); 
        x_memcpy(pui1_serialized_event, pt_pool_event->pt_event_linkage_list->pt_event_linkage, sizeof(EDB_EVENT_LINKAGE_T)*ui1_num);
        pui1_serialized_event += sizeof(EDB_EVENT_LINKAGE_T)*ui1_num;
    }
    else
    {   
        ui1_num = 0;
        x_memcpy(pui1_serialized_event, &ui1_num, sizeof(UINT8));
        pui1_serialized_event += sizeof(UINT8); 
    }

    if(pt_pool_event->pt_time_shift != NULL)
    {
        x_memcpy(pui1_serialized_event, pt_pool_event->pt_time_shift, sizeof(EDB_EVENT_TIME_SHIFT));
        pui1_serialized_event += sizeof(EDB_EVENT_TIME_SHIFT);
    }
    else
    {   
        EDB_EVENT_TIME_SHIFT    t_empty_time_shift = {0}; //indicates this filed has not been set from eit
        x_memcpy(pui1_serialized_event, &t_empty_time_shift, sizeof(EDB_EVENT_TIME_SHIFT));
        pui1_serialized_event += sizeof(EDB_EVENT_TIME_SHIFT); 
    }

    if(pt_pool_event->pt_component_list != NULL && 
       pt_pool_event->pt_component_list->ui1_num != 0 && 
       pt_pool_event->pt_component_list->pt_event_component != NULL)
    {
        ui1_num = pt_pool_event->pt_component_list->ui1_num;
        x_memcpy(pui1_serialized_event, &ui1_num, sizeof(UINT8));
        pui1_serialized_event += sizeof(UINT8); 
       
        x_memcpy(pui1_serialized_event, pt_pool_event->pt_component_list->pt_event_component, sizeof(EDB_EVENT_COMPONENT)*ui1_num);
        pui1_serialized_event += sizeof(EDB_EVENT_COMPONENT)*ui1_num;

    }
    else
    {   
        ui1_num = 0;
        x_memcpy(pui1_serialized_event, &ui1_num, sizeof(UINT8));
        pui1_serialized_event += sizeof(UINT8); 
    }

    if(pt_pool_event->pt_ca_system != NULL && 
       pt_pool_event->pt_ca_system->ui2_num_ca_ids != 0 &&
       pt_pool_event->pt_ca_system->pui2_ca_ids != NULL )
    {
        ui2_num = pt_pool_event->pt_ca_system->ui2_num_ca_ids;
        x_memcpy(pui1_serialized_event, &ui2_num, sizeof(UINT16));
        pui1_serialized_event += sizeof(UINT16); 
        x_memcpy(pui1_serialized_event, pt_pool_event->pt_ca_system->pui2_ca_ids, sizeof(UINT16)*ui2_num);
        pui1_serialized_event += sizeof(UINT16)*ui2_num;
    }
    else
    {   
        ui2_num = 0;
        x_memcpy(pui1_serialized_event, &ui2_num, sizeof(UINT16));
        pui1_serialized_event += sizeof(UINT16); 
    }

    if(pt_pool_event->pt_rating_list != NULL &&
       pt_pool_event->pt_rating_list->ui1_num != 0 &&
       pt_pool_event->pt_rating_list->pt_rating != NULL)
    {
        ui1_num = pt_pool_event->pt_rating_list->ui1_num;
        x_memcpy(pui1_serialized_event, &ui1_num, sizeof(UINT8));
        pui1_serialized_event += sizeof(UINT8); 
        x_memcpy(pui1_serialized_event, pt_pool_event->pt_rating_list->pt_rating, sizeof(EDB_EVENT_RATING_T)*ui1_num);
        pui1_serialized_event += sizeof(EDB_EVENT_RATING_T)*ui1_num;
    }
    else
    {   
        ui1_num = 0;
        x_memcpy(pui1_serialized_event, &ui1_num, sizeof(UINT8));
        pui1_serialized_event += sizeof(UINT8); 
    }


    ui2_event_length = pui1_serialized_event - pu1_start_address;
    ui2_store_event_length = ui2_event_length - sizeof(pt_pool_event->ui2_event_id) - sizeof(ui2_store_event_length);
    x_memcpy(pui1_store_event_length,&ui2_store_event_length,sizeof(ui2_store_event_length));
    
    x_memcpy((pui1_serialized_for_flash), pu1_start_address, (SIZE_T)(ui2_event_length));
    *pui2_event_length = ui2_event_length;
    
    x_mem_free(pu1_start_address);
    return EDBR_OK;
}


INT32 edb_unserialize_pool_event(EDB_POOL_EVENT_INFO_T**   ppt_pool_event , UINT8* pui1_serialized_event,UINT32* pui4_pool_event_len)
{
    SIZE_T                      z_size;
    UINT8                       ui1_num;
    UINT16                      ui2_num;
    UINT16                      ui2_event_length;
    EDB_EVENT_TIME_SHIFT        t_temp_time_shift  = {0};
    EDB_EVENT_TIME_SHIFT        t_empty_time_shift = {0};//indicates this filed has not been set from eit
    UINT16                      ui2_event_id;
    INT32                       i4_ret = EDBR_OK;
    EDB_POOL_EVENT_INFO_T*      pt_pool_event = NULL;

    if (ppt_pool_event == NULL)
    {
        return EDBR_INTERNAL_ERROR;
    }    


    x_memcpy(&ui2_event_id,pui1_serialized_event,sizeof(ui2_event_id)); 
    pui1_serialized_event += sizeof(ui2_event_id);
    x_memcpy(&ui2_event_length,pui1_serialized_event,sizeof(ui2_event_length)); 
    pui1_serialized_event += sizeof(ui2_event_length);

    do
    {
        pt_pool_event = EDB_PARTITION_ALLOC(sizeof(EDB_POOL_EVENT_INFO_T));
        if(pt_pool_event == NULL)
        {   
            i4_ret = EDBR_OUT_OF_MEM;
            break;
        }
        x_memset(pt_pool_event ,0x0,sizeof(EDB_POOL_EVENT_INFO_T));

        pt_pool_event->ui2_event_id = ui2_event_id;

        x_memcpy(&(pt_pool_event->ui1_table_id),pui1_serialized_event,  sizeof(UINT8));
        pui1_serialized_event += sizeof(UINT8);
        x_memcpy( &(pt_pool_event->ui1_sect_num), pui1_serialized_event,sizeof(UINT8));
        pui1_serialized_event += sizeof(UINT8);
        x_memcpy( &(pt_pool_event->t_start_time),pui1_serialized_event, sizeof(TIME_T));
        pui1_serialized_event += sizeof(TIME_T);
        x_memcpy( &(pt_pool_event->t_duration), pui1_serialized_event,sizeof(TIME_T));
        pui1_serialized_event += sizeof(TIME_T);
        x_memcpy( &(pt_pool_event->b_caption),pui1_serialized_event, sizeof(BOOL));
        pui1_serialized_event += sizeof(BOOL);
        x_memcpy(&(pt_pool_event->b_free_ca_mode),pui1_serialized_event,  sizeof(BOOL));
        pui1_serialized_event += sizeof(BOOL);
        x_memcpy(&(pt_pool_event->b_has_private),pui1_serialized_event,  sizeof(BOOL));
        pui1_serialized_event += sizeof(BOOL);
        x_memcpy( &(pt_pool_event->ui1_num_rating), pui1_serialized_event,sizeof(UINT8));
        pui1_serialized_event += sizeof(UINT8);
        x_memcpy( (pt_pool_event->aui2_ca_system_id),pui1_serialized_event, sizeof(pt_pool_event->aui2_ca_system_id)); 
        pui1_serialized_event += sizeof(pt_pool_event->aui2_ca_system_id); 
        x_memcpy( &(pt_pool_event->ui1_num_event_category), pui1_serialized_event,sizeof(UINT8));
        pui1_serialized_event += sizeof(UINT8);
        x_memcpy( (pt_pool_event->aui1_event_category), pui1_serialized_event,sizeof(pt_pool_event->aui1_event_category)); 
        pui1_serialized_event += sizeof(pt_pool_event->aui1_event_category); 
        x_memcpy( &(pt_pool_event->ui1_guidance_mode),pui1_serialized_event, sizeof(UINT8));
        pui1_serialized_event += sizeof(UINT8);


        x_memcpy(&z_size,pui1_serialized_event, sizeof(SIZE_T));
        pui1_serialized_event += sizeof(SIZE_T);
        if(z_size > 0)
        {
            pt_pool_event->ps_event_title = EDB_PARTITION_ALLOC(z_size+1);
            if(pt_pool_event->ps_event_title == NULL)
            {   
                i4_ret = EDBR_OUT_OF_MEM;
                break;
            }
            x_memset(pt_pool_event->ps_event_title ,0x0,(z_size+1));
            x_memcpy(pt_pool_event->ps_event_title,pui1_serialized_event, z_size);
            pui1_serialized_event += z_size;
        }

        x_memcpy(&z_size,pui1_serialized_event, sizeof(SIZE_T));
        pui1_serialized_event += sizeof(SIZE_T);
        if(z_size > 0)
        {
            pt_pool_event->ps_event_detail = EDB_PARTITION_ALLOC(z_size+1);
            if(pt_pool_event->ps_event_detail == NULL)
            {   
                i4_ret = EDBR_OUT_OF_MEM;
                break;
            }
            x_memset(pt_pool_event->ps_event_detail ,0x0,(z_size+1));
            x_memcpy(pt_pool_event->ps_event_detail,pui1_serialized_event, z_size);
            pui1_serialized_event += z_size;
        }

        x_memcpy(&z_size,pui1_serialized_event, sizeof(SIZE_T));
        pui1_serialized_event += sizeof(SIZE_T);
        if(z_size > 0)
        {
            pt_pool_event->ps_event_guidance = EDB_PARTITION_ALLOC(z_size+1);
            if(pt_pool_event->ps_event_guidance == NULL)
            {   
                i4_ret = EDBR_OUT_OF_MEM;
                break;
            }
            x_memset(pt_pool_event->ps_event_guidance ,0x0,(z_size+1));
            x_memcpy(pt_pool_event->ps_event_guidance,pui1_serialized_event, z_size);
            pui1_serialized_event += z_size;
        }

        x_memcpy(&ui1_num,pui1_serialized_event, sizeof(UINT8));
        pui1_serialized_event += sizeof(UINT8); 
        if(ui1_num > 0)
        {
            pt_pool_event->pt_event_linkage_list = EDB_PARTITION_ALLOC(sizeof(EDB_EVENT_LINKAGE_LIST_T));
            if(pt_pool_event->pt_event_linkage_list == NULL)
            {   
                i4_ret = EDBR_OUT_OF_MEM;
                break;
            }
            pt_pool_event->pt_event_linkage_list->ui1_num_event_linkage = ui1_num;
            pt_pool_event->pt_event_linkage_list->pt_event_linkage = EDB_PARTITION_ALLOC(sizeof(EDB_EVENT_LINKAGE_T) * ui1_num ) ; 
            if(pt_pool_event->pt_event_linkage_list->pt_event_linkage == NULL)
            {   
                i4_ret = EDBR_OUT_OF_MEM;
                break;
            }
            x_memcpy(pt_pool_event->pt_event_linkage_list->pt_event_linkage,pui1_serialized_event, sizeof(EDB_EVENT_LINKAGE_T)*ui1_num);       
            pui1_serialized_event += sizeof(EDB_EVENT_LINKAGE_T)*ui1_num;
        }
        /*    else
              {
              pt_pool_event->pt_event_linkage_list = NULL;
              }
              */
        x_memcpy(&t_temp_time_shift,pui1_serialized_event, sizeof(EDB_EVENT_TIME_SHIFT));
        pui1_serialized_event += sizeof(EDB_EVENT_TIME_SHIFT); 

        if(x_memcmp(&t_temp_time_shift,&t_empty_time_shift,sizeof(EDB_EVENT_POOL_CH_KEY)) != 0)//indicates it is not empty ,need to sync
        {
            pt_pool_event->pt_time_shift = EDB_PARTITION_ALLOC(sizeof(EDB_EVENT_TIME_SHIFT));
            if(pt_pool_event->pt_time_shift == NULL)
            {   
                i4_ret = EDBR_OUT_OF_MEM;
                break;
            }
            x_memcpy(pt_pool_event->pt_time_shift,&t_temp_time_shift,sizeof(EDB_EVENT_TIME_SHIFT));
        }
        /*    else
              {
              pt_pool_event->pt_time_shift = NULL;
              }
              */

        x_memcpy(&ui1_num,pui1_serialized_event,sizeof(UINT8));
        pui1_serialized_event += sizeof(UINT8); 
        if(ui1_num > 0)
        {
            pt_pool_event->pt_component_list = EDB_PARTITION_ALLOC(sizeof(EDB_EVENT_COMPONENT_LIST));
            if(pt_pool_event->pt_component_list == NULL)
            {   
                i4_ret = EDBR_OUT_OF_MEM;
                break;
            }
            pt_pool_event->pt_component_list->ui1_num = ui1_num;


            pt_pool_event->pt_component_list->pt_event_component = EDB_PARTITION_ALLOC(sizeof(EDB_EVENT_COMPONENT)*ui1_num);
            if(pt_pool_event->pt_component_list->pt_event_component == NULL)
            {   
                pt_pool_event->pt_component_list->ui1_num = 0;
                i4_ret = EDBR_OUT_OF_MEM;
                break;
            }
            x_memcpy(pt_pool_event->pt_component_list->pt_event_component,pui1_serialized_event, sizeof(EDB_EVENT_COMPONENT)*ui1_num);
            pui1_serialized_event += sizeof(EDB_EVENT_COMPONENT)*ui1_num;


        }
        /*     else 
               {
               pt_pool_event->pt_component_list = NULL;
               }
               */


        x_memcpy(&ui2_num,pui1_serialized_event,sizeof(UINT16));
        pui1_serialized_event += sizeof(UINT16); 
        if(ui2_num > 0)
        {
            pt_pool_event->pt_ca_system = EDB_PARTITION_ALLOC(sizeof(EDB_EVENT_CA_SYSTEM_ID_LIST_T));
            if(pt_pool_event->pt_ca_system == NULL)
            {   
                i4_ret = EDBR_OUT_OF_MEM;
                break;
            }
            pt_pool_event->pt_ca_system->ui2_num_ca_ids = ui2_num;
            pt_pool_event->pt_ca_system->pui2_ca_ids = EDB_PARTITION_ALLOC(sizeof(UINT16)*ui2_num);
            if(pt_pool_event->pt_ca_system->pui2_ca_ids == NULL)
            {   
                i4_ret = EDBR_OUT_OF_MEM;
                break;
            }
            x_memcpy(pt_pool_event->pt_ca_system->pui2_ca_ids,pui1_serialized_event, sizeof(UINT16)*ui2_num);
            pui1_serialized_event += sizeof(UINT16)*ui2_num;
        }

        x_memcpy(&ui1_num, pui1_serialized_event,sizeof(UINT8));
        pui1_serialized_event += sizeof(UINT8); 
        if(ui1_num > 0)
        {
            pt_pool_event->pt_rating_list = EDB_PARTITION_ALLOC(sizeof(EDB_EVENT_RATING_LIST_T));
            if(pt_pool_event->pt_rating_list == NULL)
            {   
                i4_ret = EDBR_OUT_OF_MEM;
                break;
            }
            pt_pool_event->pt_rating_list->ui1_num = ui1_num;
            pt_pool_event->pt_rating_list->pt_rating = EDB_PARTITION_ALLOC(sizeof(EDB_EVENT_RATING_T)*ui1_num);
            if(pt_pool_event->pt_rating_list->pt_rating == NULL)
            {   
                i4_ret = EDBR_OUT_OF_MEM;
                break;
            }
            x_memcpy(pt_pool_event->pt_rating_list->pt_rating,pui1_serialized_event, sizeof(EDB_EVENT_RATING_T)*ui1_num);
            pui1_serialized_event += sizeof(EDB_EVENT_RATING_T)*ui1_num;
        }

        *ppt_pool_event = pt_pool_event;
    }while(0);

    *pui4_pool_event_len = sizeof(ui2_event_id) + sizeof(ui2_event_length) + ui2_event_length;
    return i4_ret;
}
