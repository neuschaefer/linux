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
/*-----------------------------------------------------------------------------
 * $RCSfile: prc_debug.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/1 $
 * $SWAuthor: Felisa Hsiao $
 * $MD5HEX: 64193ff7da6a0138286803b58acba32b $
 *
 * Description: 
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "inc/x_common.h"
#include "os/inc/os.h"
#include "hash/hash_api.h"
#include "dbg/def_dbg_level_mw.h"
#include "dbg/x_dbg.h"
#include "prc_common.h"

static HANDLE_T h_time_measure_hash = NULL_HANDLE;
static HANDLE_T h_time_measure_sema = NULL_HANDLE;
static BOOL     b_init              = FALSE;
static UINT32   ui4_tick_ms         = 0;
static UINT16   ui2_pm_dbg_level    = DBG_LEVEL_NONE;
HANDLE_T        h_pm_dbg_test_hdl   = NULL_HANDLE;
/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
typedef struct _TIME_RECORD_T
{
    CHAR*    ps_func_name;
    UINT32   ui4_ticks;
} TIME_RECORD_T; 

/*-----------------------------------------------------------------------------
                    functions declaraions
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: pmi_dbg_init
 *
 * Description: This API initialize pm debug module.
 *
 * Inputs: -  
 *
 * Outputs: -
 *
 * Returns: The current setting of debug level.
 ----------------------------------------------------------------------------*/ 
void pmi_dbg_init(VOID)
{
    ui2_pm_dbg_level = DBG_INIT_LEVEL_MW_PRC_MNGR;    
} 
 
/*-----------------------------------------------------------------------------
 * Name: pmi_dbg_get_level
 *
 * Description: This API returns the current setting of debug level.
 *
 * Inputs: -  
 *
 * Outputs: -
 *
 * Returns: The current setting of debug level.
 ----------------------------------------------------------------------------*/ 
UINT16 pmi_dbg_get_level(VOID)
{    
    return ui2_pm_dbg_level;  
}


/*-----------------------------------------------------------------------------
 * Name: pmi_dbg_set_level
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
BOOL pmi_dbg_set_level(UINT16 ui2_level)
{
    ui2_pm_dbg_level = ui2_level;    
    return TRUE;
} 

/*-----------------------------------------------------------------------------
 * Name: time_record_create
 * Description: 
 * Inputs:  
 * Outputs: 
 * Returns: 
 ----------------------------------------------------------------------------*/
static TIME_RECORD_T* time_record_create(
                const CHAR*                 ps_func_name, 
                const UINT32                ui4_ticks
                )
{
    TIME_RECORD_T* pt_record = NULL;
    
    pt_record = (TIME_RECORD_T*)x_mem_alloc(
                    sizeof(TIME_RECORD_T)
                    );
    
    if (pt_record == NULL) {
        x_dbg_stmt("[TIME_MEASURE]create a TIME_RECORD_T failed\n");
        PM_ABORT(DBG_CAT_INV_OP, PM_DBG_ABRT_INVALID_OPEN_COND);
    }
    else
    {
        pt_record->ps_func_name = x_mem_alloc(x_strlen(ps_func_name)+1);
        if (NULL != pt_record->ps_func_name)
        {
            x_strcpy(pt_record->ps_func_name, ps_func_name);
            pt_record->ui4_ticks = ui4_ticks;
        }
    }
    
    return pt_record;  
}
/*-----------------------------------------------------------------------------
 * Name: mem_rec_destroy
 * Description: 
 * Inputs:  
 * Outputs: 
 * Returns: 
 ----------------------------------------------------------------------------*/
static VOID time_record_destroy(TIME_RECORD_T* pt_record)
{
    if (pt_record == NULL) {
        x_dbg_stmt("[TIME_MEASURE]destroy a TIME_RECORD_T failed, cann't be NULL\n");
        PM_ABORT(DBG_CAT_INV_OP, PM_DBG_ABRT_INVALID_OPEN_COND);
    }
    else
    {
        x_mem_free(pt_record->ps_func_name);
        x_mem_free(pt_record);
    }
}
/*-----------------------------------------------------------------------------
 * Name: _time_measure_rec_free_fct
 * Description: 
 * Inputs:  
 * Outputs: 
 * Returns: 
 ----------------------------------------------------------------------------*/
static VOID _time_measure_rec_free_fct(
                    VOID*                       pv_obj
                    )
{
    time_record_destroy((TIME_RECORD_T*)pv_obj);
}
/*-----------------------------------------------------------------------------
 * Name: time_measure_init
 * Description: 
 * Inputs:  
 * Outputs: 
 * Returns: 
 ----------------------------------------------------------------------------*/
VOID pmi_dbg_time_measure_init(
                    VOID
                    )
{
    
    INT32 i4_ret;
    
    if (b_init == TRUE) {
        return;
    }
    
    i4_ret = hash_create(
                8, 
                &h_time_measure_hash
                );
                
    if (i4_ret != HASH_OK) {
        x_dbg_stmt("[TIME_MEASURE]create hash for time measurement failed ret = %d\n", i4_ret);
        PM_ABORT(DBG_CAT_INV_OP, PM_DBG_ABRT_INVALID_OPEN_COND);
    }
           
    i4_ret = x_sema_create(
                        &h_time_measure_sema,
                        X_SEMA_TYPE_BINARY,
                        X_SEMA_STATE_UNLOCK
                        );

    if (i4_ret != OSR_OK) {
        x_dbg_stmt("[TIME_MEASURE]create semaphore for time measurement failed ret = %d\n", i4_ret);
        PM_ABORT(DBG_CAT_INV_OP, PM_DBG_ABRT_INVALID_OPEN_COND);
    }
        
    ui4_tick_ms = x_os_drv_get_tick_period();
    
    b_init = TRUE;           
}
/*-----------------------------------------------------------------------------
 * Name: mm_deinit
 * Description: 
 * Inputs:  
 * Outputs: 
 * Returns: 
 ----------------------------------------------------------------------------*/
VOID pmi_dbg_time_measure_deinit(
                    VOID
                    )
{
    if (b_init == FALSE) return;
    x_dbg_stmt("[TIME_MEASURE]deinit time measurement\n");
    hash_delete(h_time_measure_hash, _time_measure_rec_free_fct);
    x_sema_delete(h_time_measure_sema);
}
/*-----------------------------------------------------------------------------
 * Name: pmi_dbg_time_measure_begin
 * Description: 
 * Inputs:  
 * Outputs: 
 * Returns: 
 ----------------------------------------------------------------------------*/
VOID pmi_dbg_time_measure_begin(
                    const CHAR*                 ps_func_name
                    )
{
    TIME_RECORD_T* pt_new_rec     = NULL;
    TIME_RECORD_T* pt_old_rec     = NULL;
    UINT32         ui4_crnt_ticks = 0;
    INT32          i4_ret;
    
    ui4_crnt_ticks = os_get_sys_tick();
    
    if (b_init == FALSE) {
        pmi_dbg_time_measure_init();
    }
    
    x_sema_lock(h_time_measure_sema, X_SEMA_OPTION_WAIT);
    
    pt_new_rec = time_record_create(
                    ps_func_name,
                    ui4_crnt_ticks
                    );
    
    i4_ret = hash_add_obj(
                    h_time_measure_hash,
                    (INT8*)(ps_func_name),
                    x_strlen(ps_func_name)+1,
                    (VOID*)pt_new_rec,
                    (VOID**)(&pt_old_rec)
                    );
    
    if (i4_ret != HASH_OK) {
        x_dbg_stmt("[TIME_MEASURE]Internal error, add record failed (ret = %d, func:%s)\n", 
                    i4_ret,
                    ps_func_name);
    } else {
        x_dbg_stmt("%s begin at %d ms\n", 
                    ps_func_name,
                    ui4_crnt_ticks * ui4_tick_ms
                    );
    }
    
    if (pt_old_rec != NULL) {
        x_dbg_stmt("%s no end (begin at %d ms)\n", 
                    pt_old_rec->ps_func_name,
                    pt_old_rec->ui4_ticks * ui4_tick_ms
                    );
        time_record_destroy(pt_old_rec);
    }
    
    x_sema_unlock(h_time_measure_sema);
}
/*-----------------------------------------------------------------------------
 * Name: pmi_dbg_time_measure_end
 * Description: 
 * Inputs:  
 * Outputs: 
 * Returns: 
 ----------------------------------------------------------------------------*/
VOID pmi_dbg_time_measure_end(
                    const CHAR*                 ps_func_name
                    )
{   
    TIME_RECORD_T* pt_record = NULL;
    UINT32         ui4_crnt_ticks;
    INT32          i4_ret;
    
    ui4_crnt_ticks = os_get_sys_tick();
    
    if (b_init == FALSE) {
        pmi_dbg_time_measure_init();
    }
    
    x_sema_lock(h_time_measure_sema, X_SEMA_OPTION_WAIT);
    
    i4_ret = hash_remove_obj(
                    h_time_measure_hash,
                    (INT8*)(ps_func_name),
                    x_strlen(ps_func_name)+1,
                    (VOID**)(&pt_record)
                    );
    
    if (i4_ret == HASH_NOT_EXIST){
        x_dbg_stmt("%s end at %d ms(no begin time)\n", 
                    ps_func_name,
                    ui4_crnt_ticks * ui4_tick_ms
                    );
    } else {
        if (pt_record == NULL) {
            x_dbg_stmt("[TIME_MEASURE]Oooooooh, time measure record is NULL!!!!\n"); 
        } else {
            x_dbg_stmt("%s end at %d ms(needs %d ms)\n", 
                    pt_record->ps_func_name,
                    ui4_crnt_ticks * ui4_tick_ms,
                    (ui4_crnt_ticks - pt_record->ui4_ticks) * ui4_tick_ms
                    );
            time_record_destroy(pt_record);
        }
    }
    x_sema_unlock(h_time_measure_sema);
}


BOOL pm_dbg_hdl_allocate_nfy_fct (HANDLE_T       h_handle,
                                HANDLE_TYPE_T  e_type,
                                VOID*          pv_obj,
                                VOID*          pv_tag,
                                BOOL           b_req_handle)
{
    return TRUE;
}


