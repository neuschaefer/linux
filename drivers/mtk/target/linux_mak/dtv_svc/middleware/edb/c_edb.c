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
#include "edb/x_edb.h"
#include "edb/c_edb.h"


/*-----------------------------------------------------------------------------
  macros, defines, typedefs, enums
  ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
  data declarations
  ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
  functions declaraions
  ----------------------------------------------------------------------------*/
extern INT32 c_edb_create (
        const CHAR*          ps_edb_name,
        EDB_COMMAND_T*       pt_cmd,
        VOID*                pv_nfy_tag,
        x_edb_nfy_fct        pf_nfy,
        HANDLE_T*            ph_edb_client)
{
    return x_edb_create(ps_edb_name,pt_cmd,pv_nfy_tag,pf_nfy,ph_edb_client);
}

extern INT32 c_edb_set_listener (
        HANDLE_T             h_edb_client,
        VOID*                pv_nfy_tag,
        x_edb_nfy_fct        pf_nfy)
{
    return x_edb_set_listener(h_edb_client,pv_nfy_tag,pf_nfy);
}

extern INT32 c_edb_set (
        HANDLE_T            h_edb_client,
        EDB_COMMAND_T*      pt_set_cmd)
{
    return x_edb_set(h_edb_client,pt_set_cmd);
}

extern INT32 c_edb_get_info_len (
        HANDLE_T            h_edb_client,
        EDB_KEY_TYPE_T      e_key_type,
        VOID*               pv_key_info,
        SIZE_T*             pz_event_info_len)
{
    return x_edb_get_info_len(h_edb_client,e_key_type,pv_key_info,pz_event_info_len);
}

extern INT32 c_edb_get_info (
        HANDLE_T            h_edb_client,
        EDB_KEY_TYPE_T      e_key_type,
        VOID*               pv_key_info,
        SIZE_T*             pz_event_info_len,
        VOID*               pv_event_info)
{
    return x_edb_get_info(h_edb_client,e_key_type,pv_key_info,pz_event_info_len,pv_event_info);
}

VOID c_edb_pool_read_lock(VOID)
{
    x_edb_pool_lock_rd();
}


VOID c_edb_pool_read_unlock(VOID)
{
    x_edb_pool_unlock_rd();
}


