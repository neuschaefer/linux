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
#include "dbg/x_dbg.h"

#include "edb/edb_eng.h"
#include "edb/edb_slctr.h"



/*-----------------------------------------------------------------------------
  macros, defines, typedefs, enums
  ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
  data declarations
  ----------------------------------------------------------------------------*/
static BOOL                 b_edb_eng_inited      = FALSE;

/* Customized engine list */

static EDB_ENG_LIST_T       t_edb_eng_list        = {0};
static HANDLE_T             h_edb_eng_list_lock   = NULL_HANDLE;


/*-----------------------------------------------------------------------------
  functions declaraions
  ----------------------------------------------------------------------------*/

static VOID _edb_eng_free_list(VOID);

EDB_ENG_LIST_T edb_eng_get_eng_list(VOID)
{
    return t_edb_eng_list;
}

/*-----------------------------------------------------------------------------
 * Name: edb_eng_list_lock
 *
 * Description: API to lock the engine list
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
VOID edb_eng_list_lock(VOID)
{
    INT32 i4_ret;

    i4_ret = x_sema_lock( h_edb_eng_list_lock,
                          X_SEMA_OPTION_WAIT );
    if ( i4_ret != OSR_OK )
    {
        if (b_edb_eng_inited)
        {   /* should not fail when engine list inited */
            DBG_ABORT( DBG_MOD_EDB );
        }
    }
}


/*-----------------------------------------------------------------------------
 * Name: edb_eng_list_unlock
 *
 * Description: API to unlock the engine list.
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
VOID edb_eng_list_unlock(VOID)
{
    INT32 i4_ret;

    i4_ret = x_sema_unlock( h_edb_eng_list_lock );
    if ( i4_ret != OSR_OK )
    {
        if (b_edb_eng_inited)
        {   /* should not fail when engine list inited */
            DBG_ABORT( DBG_MOD_EDB );
        }
    }
}


/*-----------------------------------------------------------------------------
 * Name: _edb_eng_free_list
 *
 * Description: API to free all registered engines from the engine list.
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _edb_eng_free_list(VOID)
{
    EDB_ENG_T*  pt_eng;

    edb_eng_list_lock();

    while (!DLIST_IS_EMPTY(&t_edb_eng_list))
    {
        pt_eng = DLIST_HEAD(&t_edb_eng_list);
        DLIST_REMOVE(pt_eng, &t_edb_eng_list, t_link);
        EDB_OS_FREE((VOID *) pt_eng);
    }

    edb_eng_list_unlock();
}

/*-----------------------------------------------------------------------------
 * Name: edb_eng_list_init
 *
 * Description: API to initialize the engine list
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: EDBR_OK               The routine was successful.
 *          EDBR_ALREADY_INIT     The engine list was already inited.
 *          EDBR_INTERNAL_ERROR   Internal error happened.
 *
 ----------------------------------------------------------------------------*/
INT32 edb_eng_list_init(VOID)
{
    INT32 i4_ret;

    if (b_edb_eng_inited)
    {
        return EDBR_ALREADY_INIT;
    }

    DLIST_INIT(&t_edb_eng_list);

    i4_ret = x_sema_create( &h_edb_eng_list_lock,
                            X_SEMA_TYPE_MUTEX,
                            X_SEMA_STATE_UNLOCK );
    if ( i4_ret != OSR_OK || h_edb_eng_list_lock == NULL_HANDLE )
    {
        return EDBR_INTERNAL_ERROR;
    }

    b_edb_eng_inited = TRUE;

    return EDBR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: edb_eng_list_exit
 *
 * Description: API to clean and free the engine list and semaphore.
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
VOID edb_eng_list_exit(VOID)
{
    if (!b_edb_eng_inited)
    {
        return;
    }

    _edb_eng_free_list();
    x_sema_delete(h_edb_eng_list_lock);
    h_edb_eng_list_lock = NULL_HANDLE;

    b_edb_eng_inited = FALSE;
}

/*-----------------------------------------------------------------------------
 * Name: edb_eng_list_add
 *
 * Description: This API is called to add a new engine node to the engine
 *              list.
 *
 * Inputs:  pt_eng  Contains engine node to be added.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID edb_eng_list_add (EDB_ENG_T* pt_eng)
{
    if (!b_edb_eng_inited)
    {
        return;
    }

    DLIST_INSERT_TAIL(pt_eng, &t_edb_eng_list, t_link);
}


/*-----------------------------------------------------------------------------
 * Name: edb_find_eng_by_brdcst_type
 *
 * Description: This API searches for an engine control object with a given
 *              broadcast type.
 *
 * Inputs:  e_brdcst_type  Contains the broadcast type.
 *
 * Outputs: -
 *
 * Returns: A reference to the found engine control object or NULL when not
 *          found.
 ----------------------------------------------------------------------------*/
EDB_ENG_T* edb_find_eng_by_brdcst_type(BRDCST_TYPE_T e_brdcst_type)
{
    EDB_ENG_T* pt_eng;

    pt_eng = DLIST_HEAD(&t_edb_eng_list);

    while ((pt_eng != NULL) && 
           (pt_eng->e_brdcst_type != e_brdcst_type))
    {
        pt_eng = DLIST_NEXT(pt_eng, t_link);
    }

    return (pt_eng);
}

/*-----------------------------------------------------------------------------
 * Name: edb_eng_is_inited
 *
 * Description: API to return the initialziation status of EDB engine list.
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: TRUE        The EDB engine list had been initialized.
 *          FALSE       The EDB engine list is not initialized.
 *
 ----------------------------------------------------------------------------*/
BOOL edb_eng_is_inited(VOID)
{
    return b_edb_eng_inited;
}


INT32 edb_reg_engine (
        BRDCST_TYPE_T               e_brdcst_type,
        EDB_ENG_FCT_TBL_T*          pt_eng_fct_tbl,
        UINT16                      ui2_max_num_event_objs)
{
    INT32               i4_ret = 0;
    EDB_ENG_T*          pt_eng = NULL;

    if (pt_eng_fct_tbl == NULL /*|| ui2_max_num_event_objs == 0*/)
    {
        return EDBR_INV_ARG;
    }

    if (pt_eng_fct_tbl->pf_create_eng_obj == NULL ||
        pt_eng_fct_tbl->pf_delete_eng_obj == NULL ||
        pt_eng_fct_tbl->pf_engine_task == NULL ||
        pt_eng_fct_tbl->pf_get_event_info == NULL ||
        pt_eng_fct_tbl->pf_get_event_info_len == NULL)
    {
        return EDBR_INV_ARG;
    }

    if (!edb_is_inited())
    {
        return EDBR_NOT_INIT;
    }

    edb_eng_list_lock();

    i4_ret = EDBR_DUP_BRDCST_TYPE;

    if (edb_find_eng_by_brdcst_type(e_brdcst_type) == NULL)
    {   /* means engine not found, add a new one */

        i4_ret = EDBR_OUT_OF_MEM;

        pt_eng = (EDB_ENG_T*)EDB_OS_ALLOC(sizeof(EDB_ENG_T));
        if (pt_eng != NULL)
        {
            x_memset (((VOID*) pt_eng), 0, sizeof(EDB_ENG_T));

            pt_eng->e_brdcst_type   = e_brdcst_type;
            pt_eng->t_edb_fct_tbl = (*pt_eng_fct_tbl);
            edb_eng_list_add(pt_eng);
            i4_ret = EDBR_OK;
        } /* allocate an engine node */
    } /* check whether if there's confliction */

    edb_eng_list_unlock();

    return i4_ret;
}


INT32 edb_activate_eng_task(BRDCST_TYPE_T e_brdcst_type)
{
    INT32               i4_retry = 0;
    HANDLE_T            h_msg_q  = NULL_HANDLE;
    EDB_SLCTR_MSG_T     t_msg    = {0};

    if (!edb_is_inited())
    {
        return EDBR_NOT_INIT;
    }

    x_memset(&t_msg, 0, sizeof(EDB_SLCTR_MSG_T));

    t_msg.e_msg_type = EDB_SLCTR_MSG_ACTIVATE_ENG_TASK;
    t_msg.u_data.t_eng_task.e_brdcst_type = e_brdcst_type;


    h_msg_q = edb_get_msg_q_hdl();

    while ( ++i4_retry <= 10 )
    {
        if (x_msg_q_send(h_msg_q,
                         &t_msg,
                         sizeof(EDB_SLCTR_MSG_T),
                         0 /* highest priority */) != OSR_OK)
        {
            /* Send message fail, wait 10 ms and retry for 10 times */
            x_thread_delay( 10 );
            continue;
        }

        /*x_msg_q_delete(h_msg_q);*/
        return EDBR_OK;
    }

    /*x_msg_q_delete(h_msg_q);*/

    return EDBR_INTERNAL_ERROR;
}

INT32 x_edb_eng_unify_country_code (
        ISO_3166_COUNT_T*   pt_country_code)
{
    CHAR* pch;

    if (pt_country_code == NULL ||
        *pt_country_code == NULL)
    {
        return EDBR_INV_ARG;
    }

    pch = (CHAR*) *pt_country_code;

    while (*pch != '\x0' && (pch - *pt_country_code < ISO_3166_COUNT_LEN))
    {
        if (*pch >= 'a' && *pch <= 'z')
        {
            *pch = *pch - 'a' + 'A';
        }

        pch++;
    }

    return EDBR_OK;
}




