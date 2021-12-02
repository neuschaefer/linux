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
 * $RCSfile: rec_util_utils.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X/DTV_X_HQ_int/1 $
 * $SWAuthor: Lianming Lin $
 * $MD5HEX: d052cebe5bec682f2354c25548053f2d $
 *
 * Description: 
 *         This file implements ring buffer  APIs.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "inc/x_common.h"
#include "inc/x_mm_common.h"
#include "handle/u_handle.h"
#include "handle/x_handle.h"
#include "handle/handle.h"
#include "util/x_lnk_list.h"
#include "os/inc/x_os.h"
#include "dbg/dbg.h"
#include "file_mngr/x_fm.h"
#include "file_mngr/x_fm_mfw.h"
#include "rec_util/_rec_util.h"
#include "rec_util/rec_util_utils.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define REC_UTIL_MAX_STMT_LEN            512

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
static CHAR        aui1_rec_util_dbg_stmt[REC_UTIL_MAX_STMT_LEN];   /* Debug statement. */
static HANDLE_T    h_rec_util_output_sema;                          /* References the output semaphore. */
static UINT16      ui2_rec_util_dbg_level;


/*-----------------------------------------------------------------------------
                functions declaraions
 ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                Internal functions implementations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                 External functions implementations
 ------------------------------------------------------------------------------*/
VOID rec_util_utils_init(VOID)
{
    /* Create the required semaphores. */
    if (x_sema_create(&h_rec_util_output_sema,
                      X_SEMA_TYPE_MUTEX, 
                      X_SEMA_STATE_UNLOCK) != OSR_OK)
    {
        REC_UTIL_ABORT(REC_UTIL_DBG_ABRT_CANNOT_CREATE_SEMAPHORE);
    }
}

/*-----------------------------------------------------------------------------
 * Name: rec_util_set_dbg_level
 *
 * Description: This API changes the setting of debug level to new one.
 *
 * Inputs: ui2_level        The new setting of debug level.  
 *
 * Outputs: -
 *
 * Returns: TRUE            Routine successful.
 *          FALSE           Routine failed.
 ----------------------------------------------------------------------------*/ 
BOOL rec_util_set_dbg_level(UINT16 ui2_level)
{
    ui2_rec_util_dbg_level = ui2_level;    
    return TRUE;
}

/*-----------------------------------------------------------------------------
 * Name: rec_util_get_dbg_level
 *
 * Description: This API returns the current setting of debug level.
 *
 * Inputs: -  
 *
 * Outputs: -
 *
 * Returns: The current setting of debug level.
 ----------------------------------------------------------------------------*/ 
UINT16 rec_util_get_dbg_level(VOID)
{
    return ui2_rec_util_dbg_level;    
}

/*-----------------------------------------------------------------------------
 * Name:  rec_util_dbg_ctrl_stmt_api
 *
 * Description: This function prints rec_util api debug info. 
 *
 * Inputs:  ps_format 
 *
 * Outputs: NULL  
 *
 * Returns: REC_UTILR_OK,             Success. 
 *          REC_UTILR_INTERNAL_ERROR: Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-04-26 : initial
 ----------------------------------------------------------------------------*/
INT32 rec_util_dbg_ctrl_stmt_api(const CHAR*  ps_format, ...)
{
    INT32           i4_return;
    VA_LIST         arg_list;

    i4_return = REC_UTILR_INV_ARG;
    if (ps_format != NULL)
    {
        i4_return = REC_UTILR_OK;
        
        VA_START(arg_list, ps_format);
        
        /* Wait for other debug statement to be finished. */
        x_sema_lock(h_rec_util_output_sema, X_SEMA_OPTION_WAIT);
        
        x_memset(aui1_rec_util_dbg_stmt, 0, REC_UTIL_MAX_STMT_LEN);
        /* Create debug statement string and send it to output device. */
        x_vsnprintf(aui1_rec_util_dbg_stmt, REC_UTIL_MAX_STMT_LEN - 1, ps_format, arg_list);
        DBG_API(("{REC_UTIL} API: %s", aui1_rec_util_dbg_stmt));
        
        x_sema_unlock(h_rec_util_output_sema);
        
        VA_END(arg_list);
    }
    else
    {
        
    }

    return (i4_return);
}

#ifdef _REC_UTIL_UT_SUPPORT
 
/*-----------------------------------------------------------------------------
 * Name:  rec_util_ut_info
 *
 * Description: This function prints rec_util api debug info. 
 *
 * Inputs:  ps_format 
 *
 * Outputs: NULL  
 *
 * Returns: REC_UTILR_OK,             Success. 
 *          REC_UTILR_INTERNAL_ERROR: Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-04-26 : initial
 ----------------------------------------------------------------------------*/
INT32 rec_util_ut_info(const CHAR*  ps_format, ...)
{
    INT32           i4_return;
    VA_LIST         arg_list;

    i4_return = REC_UTILR_INV_ARG;
    if (ps_format != NULL)
    {
        i4_return = REC_UTILR_OK;
        
        VA_START(arg_list, ps_format);
        
        /* Wait for other debug statement to be finished. */
        x_sema_lock(h_rec_util_output_sema, X_SEMA_OPTION_WAIT);
        
        x_memset(aui1_rec_util_dbg_stmt, 0, REC_UTIL_MAX_STMT_LEN);
        /* Create debug statement string and send it to output device. */
        x_vsnprintf(aui1_rec_util_dbg_stmt, REC_UTIL_MAX_STMT_LEN - 1, ps_format, arg_list);
        x_dbg_stmt("{REC_UTIL} UT INFO: %s", aui1_rec_util_dbg_stmt);
        x_sema_unlock(h_rec_util_output_sema);
        
        VA_END(arg_list);
    }
    else
    {
        
    }

    return (i4_return);
}
#endif

/*-----------------------------------------------------------------------------
 * Name:  rec_util_dbg_ctrl_stmt_err
 *
 * Description: This function prints rec_util error debug info. 
 *
 * Inputs:  ps_format 
 *
 * Outputs: NULL  
 *
 * Returns: REC_UTILR_OK,             Success. 
 *          REC_UTILR_INTERNAL_ERROR: Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-04-26 : initial
 ----------------------------------------------------------------------------*/
INT32 rec_util_dbg_ctrl_stmt_err(const CHAR*  ps_format, ...)
{
    INT32           i4_return;
    VA_LIST         arg_list;


    i4_return = REC_UTILR_INV_ARG;
    if (ps_format != NULL)
    {
        i4_return = REC_UTILR_OK;
        
        VA_START(arg_list, ps_format);
        
        /* Wait for other debug statement to be finished. */
        x_sema_lock(h_rec_util_output_sema, X_SEMA_OPTION_WAIT);
        
        x_memset(aui1_rec_util_dbg_stmt, 0, REC_UTIL_MAX_STMT_LEN);
        /* Create debug statement string and send it to output device. */
        x_vsnprintf(aui1_rec_util_dbg_stmt, 
                   REC_UTIL_MAX_STMT_LEN - 1, 
                   ps_format, 
                   arg_list);
        DBG_ERROR(("{REC_UTIL} ERR: %s", aui1_rec_util_dbg_stmt));
        
        x_sema_unlock(h_rec_util_output_sema);
        
        VA_END(arg_list);
    }
    else
    {
        
    }

    return (i4_return);
}

/*-----------------------------------------------------------------------------
 * Name:  rec_util_dbg_ctrl_stmt_info
 *
 * Description: This function prints rec_util info debug info. 
 *
 * Inputs:  ps_format 
 *
 * Outputs: NULL  
 *
 * Returns: REC_UTILR_OK,             Success. 
 *          REC_UTILR_INTERNAL_ERROR: Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-04-26 : initial
 ----------------------------------------------------------------------------*/
INT32 rec_util_dbg_ctrl_stmt_info(const CHAR*  ps_format, ...)
{
    INT32           i4_return;
    VA_LIST         arg_list;


    i4_return = REC_UTILR_INV_ARG;
    if (ps_format != NULL)
    {
        i4_return = REC_UTILR_OK;
        
        VA_START(arg_list, ps_format);
        
        /* Wait for other debug statement to be finished. */
        x_sema_lock(h_rec_util_output_sema, X_SEMA_OPTION_WAIT);
        
        x_memset(aui1_rec_util_dbg_stmt, 0, REC_UTIL_MAX_STMT_LEN);
        /* Create debug statement string and send it to output device. */
        x_vsnprintf(aui1_rec_util_dbg_stmt, REC_UTIL_MAX_STMT_LEN - 1, ps_format, arg_list);
        DBG_INFO(("{REC_UTIL} INFO: %s", aui1_rec_util_dbg_stmt));
    
        x_sema_unlock(h_rec_util_output_sema);
        
        VA_END(arg_list);
    }
    else
    {
        
    }

    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name:  rec_util_get_small_multi_num
 *
 * Description: This function calculates smallest common multiple number
 *
 * Inputs:  
 *       ui4_m:   number m
 *       ui4_n:   number n
 *
 * Outputs: NULL
 *
 * Returns: smallest common multiple number
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-05-07 : initial
 ------------------------------------------------------------------------------*/
UINT32 rec_util_get_smallest_multi_num(UINT32 ui4_m, UINT32 ui4_n)
{
    UINT32     ui4_temp;
    UINT32     ui4_i;
    UINT32     ui4_j;

    ui4_i = ui4_m;
    ui4_j = ui4_n;
    
    if (0 == ui4_j)
    {
        return 0;
    }
    
    ui4_temp = ui4_i % ui4_j;
    while (ui4_temp != 0)
    {
        ui4_i = ui4_j;
        ui4_j = ui4_temp;
        ui4_temp = ui4_i % ui4_j;
    }

    return (ui4_m * ui4_n / ui4_j);
    
}

/*-----------------------------------------------------------------------------
 * Name:  rec_util_align_write_sz
 *
 * Description: This function calculates the least alignment write length
 *              and remainder
 *
 * Inputs:  
 *       ui4_write_sz:   write data length
 *       ui4_align_sz:   aligment
 *
 * Outputs: 
 *       pui4_new_sz: alignment write length
 *       pui4_remainder_sz: remainder length
 *
 * Returns: NULL
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-05-07 : initial
 ------------------------------------------------------------------------------*/
VOID rec_util_align_write_sz(UINT32    ui4_write_sz,
                             UINT32    ui4_align_sz,
                             UINT32*   pui4_new_sz,
                             UINT32*   pui4_remainder_sz)
{
    if ((0 == ui4_align_sz) || (NULL == pui4_new_sz) || (NULL == pui4_remainder_sz))
    {
        REC_UTIL_ABORT(REC_UTIL_DBG_ABRT_CANNOT_HAPPEN);
        return;
    }
    
    if (0 == (ui4_write_sz % ui4_align_sz))
    {
        *pui4_new_sz = ui4_write_sz;
        *pui4_remainder_sz = 0;
    }
    else 
    {
        *pui4_remainder_sz = ui4_write_sz % ui4_align_sz;
        *pui4_new_sz = ui4_write_sz + (ui4_align_sz - *pui4_remainder_sz);
    }
    
    return;
}

/*-----------------------------------------------------------------------------
 * Name:  rec_util_div_u64
 *
 * Description: This function simulates 64bit divide function
 *
 * Inputs:  
 *       ui8_dividend:   dividend 
 *       ui8_divider:    divider
 *
 * Outputs: 
 *       pui8_remainder: remainder
 *
 * Returns: quotient
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-05-07 : initial
 ------------------------------------------------------------------------------*/

UINT64 rec_util_div_u64(UINT64 ui8_dividend, UINT64 ui8_divider, UINT64* pui8_remainder)
{
    #ifdef __KERNEL__

    return u8Div6432(ui8_dividend, ui8_divider, pui8_remainder);
    
    #else

    UINT64 ui8_temp = 0;

    if (ui8_divider)
    {
        ui8_temp = ui8_dividend / ui8_divider;
    }
    
    if (pui8_remainder)
    {
        *pui8_remainder = ui8_dividend - (ui8_temp * ui8_divider);
    }
    return (ui8_temp);
    
    #endif
}

/*-----------------------------------------------------------------------------
 * Name:  _rec_util_rque_free_fct
 *
 * Description: This function free ring queue object resource
 *
 * Inputs:  
 *       h_handle :ring queue handle
 *       e_type :  ring queue handle type
 *       pv_obj :  ring queue object pointer
 *       h_handle :ring queue object tag
 *       b_req_handle :free is allowed or not flag
 *
 * Outputs: NULL  
 *
 * Returns: TRUE,  Success. 
 *          FALSE: Fail to free handle
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-05-14 : initial
 ------------------------------------------------------------------------------*/
static BOOL _rec_util_rque_free_fct(HANDLE_T       h_handle,
                                    HANDLE_TYPE_T  e_type,
                                    VOID*          pv_obj,
                                    VOID*          pv_tag,
                                    BOOL           b_req_handle)
{
    REC_UTIL_RING_QUEUE_CTRL_T*      pt_rque_ctrl;

    if ((TRUE == b_req_handle) || (NULL == pv_obj))
    {
        REC_UTIL_DBG_ERR(("_rec_util_rque_free_fct:invalid parameters!\r\n"));
        return FALSE;
    }
    
    pt_rque_ctrl = (REC_UTIL_RING_QUEUE_CTRL_T*)pv_obj;
    if (pt_rque_ctrl != NULL)
    {
        switch (pt_rque_ctrl->e_mem_mode)
        {
        case REC_UTIL_RQUE_MEM_MODE_NORMAL:
            if (pt_rque_ctrl->pui1_start_addr != NULL)
            {
                x_mem_free(pt_rque_ctrl->pui1_start_addr);
                pt_rque_ctrl->pui1_start_addr = NULL;
            }
            break;
            
        case REC_UTIL_RQUE_MEM_MODE_SHARE:
            if (pt_rque_ctrl->pui1_start_addr != NULL)
            {
                x_munmap_share_mem (pt_rque_ctrl->pui1_start_addr, 
                                    (SIZE_T) pt_rque_ctrl->ui4_buf_len);
                pt_rque_ctrl->pui1_start_addr = NULL;
            }
            if (pt_rque_ctrl->pui1_start_addr_phy != NULL)
            {
                x_share_mem_free (pt_rque_ctrl->pui1_start_addr_phy,
                                  (SIZE_T) pt_rque_ctrl->ui4_buf_len);
                pt_rque_ctrl->pui1_start_addr_phy = NULL;
            }
            break;
            
        default:
            break;
        }        
        
        if (pt_rque_ctrl->h_lock != NULL_HANDLE)
        {
            x_sema_delete(pt_rque_ctrl->h_lock);
            pt_rque_ctrl->h_lock = NULL_HANDLE;
        }
        
        x_mem_free(pt_rque_ctrl);
        pt_rque_ctrl = NULL;
    }

    return TRUE;
} 

INT32 rec_util_rque_create(REC_UTIL_RQUE_MEM_MODE_T  e_mem_mode,
                           UINT32                    ui4_entry_sz,
                           UINT32                    ui4_entry_cnt,
                           UINT32         ui4_padding_cnt,
                           HANDLE_T*      ph_handle)
{
    REC_UTIL_RING_QUEUE_CTRL_T*  pt_rque_ctrl = NULL;
    INT32                        i4_ret;   
    
    /*1.check parameters*/
    if (NULL == ph_handle || ui4_entry_cnt < 2)
    {
        return REC_UTILR_RQUE_ERR;
    }

    do
    {
        /*1. allocate a que ctrl*/
        pt_rque_ctrl = (REC_UTIL_RING_QUEUE_CTRL_T*)
                       x_mem_alloc(sizeof(REC_UTIL_RING_QUEUE_CTRL_T));
        if (NULL == pt_rque_ctrl)
        {
            break;
        }
        x_memset(pt_rque_ctrl, 0, sizeof(REC_UTIL_RING_QUEUE_CTRL_T));
        
        pt_rque_ctrl->e_mem_mode            = e_mem_mode;
        pt_rque_ctrl->ui4_entry_sz          = ui4_entry_sz;
        pt_rque_ctrl->ui4_total_entry_cnt   = ui4_entry_cnt + ui4_padding_cnt;
        pt_rque_ctrl->ui4_avail_entry_cnt   = 0;
        pt_rque_ctrl->ui4_padding_entry_cnt = ui4_padding_cnt;

        /*2. allocate queue buffer*/
        pt_rque_ctrl->ui4_buf_len     = (ui4_entry_sz) * (pt_rque_ctrl->ui4_total_entry_cnt);
        
        i4_ret = REC_UTILR_OK;
        switch (e_mem_mode)
        {
        case REC_UTIL_RQUE_MEM_MODE_NORMAL:
        {
            pt_rque_ctrl->pui1_start_addr = (UINT8*) x_mem_alloc(pt_rque_ctrl->ui4_buf_len);
            if (NULL == pt_rque_ctrl->pui1_start_addr)
            {
                i4_ret = REC_UTILR_OUT_OF_MEMORY;
                break;
            }
            pt_rque_ctrl->pui1_start_addr_phy = pt_rque_ctrl->pui1_start_addr;
            break;
        }
        case REC_UTIL_RQUE_MEM_MODE_SHARE:
        {
            pt_rque_ctrl->pui1_start_addr_phy = (UINT8 *) x_share_mem_alloc (pt_rque_ctrl->ui4_buf_len);
            if (NULL == pt_rque_ctrl->pui1_start_addr_phy)
            {
                i4_ret = REC_UTILR_OUT_OF_MEMORY;
                break;
            }
            
            pt_rque_ctrl->pui1_start_addr = x_mmap_share_mem (pt_rque_ctrl->pui1_start_addr_phy,
                                                              pt_rque_ctrl->ui4_buf_len);
            if (NULL == pt_rque_ctrl->pui1_start_addr)
            {
                i4_ret = REC_UTILR_OTHER_MODULE_FAIL;
                break;
            }
            break;
        }    
        default:
            REC_UTIL_ABORT(REC_UTIL_DBG_ABRT_CANNOT_HAPPEN);            
        }
        
        if (i4_ret != REC_UTILR_OK)
        {
            break;
        }
        
        x_memset(pt_rque_ctrl->pui1_start_addr, 0, pt_rque_ctrl->ui4_buf_len);

        /*3.create a lock*/
        if (x_sema_create(&(pt_rque_ctrl->h_lock),
                          X_SEMA_TYPE_BINARY, 
                          X_SEMA_STATE_UNLOCK) != OSR_OK)
        {
            REC_UTIL_ABORT(REC_UTIL_DBG_ABRT_CANNOT_CREATE_SEMAPHORE);
        }

        /*4.allocate handle*/
        if (HR_OK != handle_alloc(RU_HT_RING_QUEUE, 
                                  (VOID*)pt_rque_ctrl,
                                  NULL, 
                                  _rec_util_rque_free_fct, 
                                  ph_handle))
        {
            break;
        }
        
        pt_rque_ctrl->b_init = TRUE;
        
        return REC_UTILR_RQUE_OK; 
        
    } while (0);

    /*abnormal handle*/
    if (pt_rque_ctrl != NULL)
    {
        switch (e_mem_mode)
        {
        case REC_UTIL_RQUE_MEM_MODE_NORMAL:
            if (pt_rque_ctrl->pui1_start_addr != NULL)
            {
                x_mem_free(pt_rque_ctrl->pui1_start_addr);
                pt_rque_ctrl->pui1_start_addr = NULL;
            }
            break;
            
        case REC_UTIL_RQUE_MEM_MODE_SHARE:
            if (pt_rque_ctrl->pui1_start_addr != NULL)
            {
                x_munmap_share_mem (pt_rque_ctrl->pui1_start_addr, 
                                    (SIZE_T) pt_rque_ctrl->ui4_buf_len);
                pt_rque_ctrl->pui1_start_addr = NULL;
            }
            if (pt_rque_ctrl->pui1_start_addr_phy != NULL)
            {
                x_share_mem_free (pt_rque_ctrl->pui1_start_addr_phy,
                                  (SIZE_T) pt_rque_ctrl->ui4_buf_len);
                pt_rque_ctrl->pui1_start_addr_phy = NULL;
            }
            break;
            
        default:
            break;
        }
        
        if (pt_rque_ctrl->h_lock != NULL_HANDLE)
        {
            x_sema_delete(pt_rque_ctrl->h_lock);
            pt_rque_ctrl->h_lock = NULL_HANDLE;
        }
        
        x_mem_free(pt_rque_ctrl);
        pt_rque_ctrl = NULL;
    }
    
    return REC_UTILR_RQUE_ERR;  
}
INT32 rec_util_rque_get_info(HANDLE_T       h_handle,
                             UINT8**        ppui1_start_addr_phy,
                             UINT8**        ppui1_end_addr_phy,
                             UINT32*        pui4_entry_sz)
{
    INT32                           i4_ret;
    HANDLE_TYPE_T                   e_handle_type;
    VOID*                           pv_obj = NULL;
    REC_UTIL_RING_QUEUE_CTRL_T*     pt_rque_ctrl;

    i4_ret = handle_get_type_obj(h_handle, &e_handle_type, &pv_obj);
    if ((HR_OK != i4_ret) || (RU_HT_RING_QUEUE != e_handle_type) || (NULL == pv_obj))
    {
        REC_UTIL_DBG_ERR(("rec_util_rque_get_info: invalid handle!\r\n"));
        return REC_UTILR_RQUE_INV_ARG;
    }
    
    pt_rque_ctrl = (REC_UTIL_RING_QUEUE_CTRL_T*)pv_obj;
    if (!pt_rque_ctrl->b_init)
    {
        REC_UTIL_DBG_ERR(("rec_util_rque_get_info: not init!\r\n"));
        return REC_UTILR_RQUE_ERR;
    }
    
    REC_UTIL_LOCK(pt_rque_ctrl->h_lock);
    if (ppui1_start_addr_phy != NULL)
    {
        *ppui1_start_addr_phy = pt_rque_ctrl->pui1_start_addr_phy;
    }
    if (ppui1_end_addr_phy != NULL)
    {
        *ppui1_end_addr_phy = pt_rque_ctrl->pui1_start_addr_phy + pt_rque_ctrl->ui4_buf_len;
    }
    if (pui4_entry_sz != NULL)
    {
        *pui4_entry_sz   = pt_rque_ctrl->ui4_entry_sz;
    }
    REC_UTIL_UNLOCK(pt_rque_ctrl->h_lock);
    
    return REC_UTILR_RQUE_OK;
}

INT32 rec_util_rque_get_status(HANDLE_T       h_handle,
                                      UINT32*        pui4_read_start_idx,
                                      UINT32*        pui4_read_end_idx,
                                      UINT32*        pui4_avail_entry_cnt,
                                      UINT32*        pui4_total_cnt)
{
    INT32                           i4_ret;
    HANDLE_TYPE_T                   e_handle_type;
    VOID*                           pv_obj = NULL;
    REC_UTIL_RING_QUEUE_CTRL_T*     pt_rque_ctrl;
    UINT32                          ui4_avail_entry_cnt;

    i4_ret = handle_get_type_obj(h_handle, &e_handle_type, &pv_obj);
    if ((HR_OK != i4_ret) || (RU_HT_RING_QUEUE != e_handle_type) || (NULL == pv_obj))
    {
        REC_UTIL_DBG_ERR(("rec_util_rque_get_status: invalid handle!\r\n"));
        return REC_UTILR_RQUE_INV_ARG;
    }
    
    pt_rque_ctrl = (REC_UTIL_RING_QUEUE_CTRL_T*)pv_obj;
    if (!pt_rque_ctrl->b_init)
    {
        REC_UTIL_DBG_ERR(("rec_util_rque_get_status: not init!\r\n"));
        return REC_UTILR_RQUE_ERR;
    }
    
    REC_UTIL_LOCK(pt_rque_ctrl->h_lock);
    if (pui4_read_start_idx != NULL)
    {
        *pui4_read_start_idx      = pt_rque_ctrl->ui4_read_idx;
    }

    ui4_avail_entry_cnt = pt_rque_ctrl->ui4_avail_entry_cnt;
    
    if (pui4_avail_entry_cnt != NULL)
    {
        *pui4_avail_entry_cnt = ui4_avail_entry_cnt;
    }

    if (ui4_avail_entry_cnt != 0)
    {
        if (pui4_read_end_idx != NULL)
        {
            *pui4_read_end_idx = (pt_rque_ctrl->ui4_read_idx + ui4_avail_entry_cnt - 1) 
                                  % pt_rque_ctrl->ui4_total_entry_cnt;
        }
    }
    else
    {
        if (NULL != pui4_read_end_idx)
        {
            *pui4_read_end_idx   = (UINT32)0xFFFFFFFF;
        }
        if (NULL != pui4_read_start_idx)
        {
            *pui4_read_start_idx   = (UINT32)0xFFFFFFFF;
        }
    }
    
    if (pui4_total_cnt != NULL)
    {
        *pui4_total_cnt = pt_rque_ctrl->ui4_total_entry_cnt;
    }
    
    REC_UTIL_UNLOCK(pt_rque_ctrl->h_lock);
    
    return REC_UTILR_RQUE_OK;
}

BOOL rec_util_rque_is_full(HANDLE_T       h_handle)
{
    INT32                           i4_ret;
    HANDLE_TYPE_T                   e_handle_type;
    VOID*                           pv_obj = NULL;
    REC_UTIL_RING_QUEUE_CTRL_T*     pt_rque_ctrl;
    BOOL                            b_full;
     
    i4_ret = handle_get_type_obj(h_handle, &e_handle_type, &pv_obj);
    if ((HR_OK != i4_ret) || (RU_HT_RING_QUEUE != e_handle_type) || (NULL == pv_obj))
    {
        REC_UTIL_DBG_ERR(("rec_util_rque_is_full: invalid handle!\r\n"));
        return FALSE;
    }
    
    pt_rque_ctrl = (REC_UTIL_RING_QUEUE_CTRL_T*)pv_obj;
    if (!pt_rque_ctrl->b_init)
    {
        REC_UTIL_DBG_ERR(("rec_util_rque_is_full: not init!\r\n"));
        return FALSE;
    }
    
    REC_UTIL_LOCK(pt_rque_ctrl->h_lock);
    
    if ((pt_rque_ctrl->ui4_avail_entry_cnt + pt_rque_ctrl->ui4_padding_entry_cnt) 
        == pt_rque_ctrl->ui4_total_entry_cnt)
    {
        b_full = TRUE;
    }
    else
    {
        b_full = FALSE;
    }
    REC_UTIL_UNLOCK(pt_rque_ctrl->h_lock);
    
    return b_full;
}

BOOL rec_util_rque_is_empty(HANDLE_T       h_handle)
{
    INT32                           i4_ret;
    HANDLE_TYPE_T                   e_handle_type;
    VOID*                           pv_obj = NULL;
    REC_UTIL_RING_QUEUE_CTRL_T*     pt_rque_ctrl;
    BOOL                            b_empty;
     
    i4_ret = handle_get_type_obj(h_handle, &e_handle_type, &pv_obj);
    if ((HR_OK != i4_ret) || (RU_HT_RING_QUEUE != e_handle_type) || (NULL == pv_obj))
    {
        REC_UTIL_DBG_ERR(("rec_util_rque_is_empty: invalid handle!\r\n"));
        return TRUE;
    }
    
    pt_rque_ctrl = (REC_UTIL_RING_QUEUE_CTRL_T*)pv_obj;
    if (!pt_rque_ctrl->b_init)
    {
        REC_UTIL_DBG_ERR(("rec_util_rque_is_empty: not init!\r\n"));
        return TRUE;
    }
    
    REC_UTIL_LOCK(pt_rque_ctrl->h_lock);
    if (pt_rque_ctrl->ui4_avail_entry_cnt == 0)
    {
        b_empty = TRUE;
    }
    else
    {
        b_empty = FALSE;
    }
    REC_UTIL_UNLOCK(pt_rque_ctrl->h_lock);
    
    return b_empty;
}

INT32 rec_util_rque_write(HANDLE_T       h_handle,
                          UINT8*         pui1_data,
                          UINT32         ui4_data_sz)
{
    INT32                           i4_ret;
    HANDLE_TYPE_T                   e_handle_type;
    VOID*                           pv_obj = NULL;
    REC_UTIL_RING_QUEUE_CTRL_T*     pt_rque_ctrl;
    UINT8*                          pui1_write_ptr = NULL;
    
    if (NULL == pui1_data)
    {
        REC_UTIL_DBG_ERR(("rec_util_rque_write: pui1_data is null!\r\n"));
        return REC_UTILR_RQUE_ERR;
    }
    
    i4_ret = handle_get_type_obj(h_handle, &e_handle_type, &pv_obj);
    if ((HR_OK != i4_ret) || (RU_HT_RING_QUEUE != e_handle_type) || (NULL == pv_obj))
    {
        REC_UTIL_DBG_ERR(("rec_util_rque_write: invalid handle!\r\n"));
        return REC_UTILR_RQUE_ERR;
    }
    
    pt_rque_ctrl = (REC_UTIL_RING_QUEUE_CTRL_T*)pv_obj;
    if (!pt_rque_ctrl->b_init)
    {
        REC_UTIL_DBG_ERR(("rec_util_rque_write: not init!\r\n"));
        return REC_UTILR_RQUE_ERR;
    }
    
    REC_UTIL_LOCK(pt_rque_ctrl->h_lock);
    i4_ret =  REC_UTILR_RQUE_OK;
    do 
    {
        
        /*if it is full can not in*/
        if ((pt_rque_ctrl->ui4_avail_entry_cnt + pt_rque_ctrl->ui4_padding_entry_cnt) 
             == pt_rque_ctrl->ui4_total_entry_cnt)
        {
            i4_ret = REC_UTILR_RQUE_FULL;
            break;
        }
        
        if (ui4_data_sz != pt_rque_ctrl->ui4_entry_sz)
        {
            i4_ret =  REC_UTILR_RQUE_ERR;
            break;
        }

        pui1_write_ptr = pt_rque_ctrl->pui1_start_addr 
                        + pt_rque_ctrl->ui4_write_idx * pt_rque_ctrl->ui4_entry_sz;
        x_memcpy(pui1_write_ptr, pui1_data, ui4_data_sz);
        pt_rque_ctrl->ui4_write_idx = (pt_rque_ctrl->ui4_write_idx + 1) 
                                       % (pt_rque_ctrl->ui4_total_entry_cnt);
        ++(pt_rque_ctrl->ui4_avail_entry_cnt);  
    }while(0);
    REC_UTIL_UNLOCK(pt_rque_ctrl->h_lock);

    return i4_ret;
    
}
INT32 rec_util_rque_read(HANDLE_T       h_handle,
                         UINT8*         pui1_buf,
                         UINT32*        pui4_buf_sz)
{
    INT32                           i4_ret;
    HANDLE_TYPE_T                   e_handle_type;
    VOID*                           pv_obj = NULL;
    REC_UTIL_RING_QUEUE_CTRL_T*     pt_rque_ctrl;
    UINT8*                          pui1_read_ptr = NULL;

    i4_ret = handle_get_type_obj(h_handle, &e_handle_type, &pv_obj);
    if ((HR_OK != i4_ret) || (RU_HT_RING_QUEUE != e_handle_type) || (NULL == pv_obj))
    {
        REC_UTIL_DBG_ERR(("rec_util_rque_read: invalid handle!\r\n"));
        return REC_UTILR_RQUE_ERR;
    }
    
    pt_rque_ctrl = (REC_UTIL_RING_QUEUE_CTRL_T*)pv_obj;
    if (!pt_rque_ctrl->b_init)
    {
        REC_UTIL_DBG_ERR(("rec_util_rque_read: not init!\r\n"));
        return REC_UTILR_RQUE_ERR;
    }
    
    REC_UTIL_LOCK(pt_rque_ctrl->h_lock);
    i4_ret =  REC_UTILR_RQUE_OK;
    do 
    {
        /*if it is empty can not out*/
        if (pt_rque_ctrl->ui4_avail_entry_cnt == 0)
        {
            i4_ret = REC_UTILR_RQUE_EMPTY;
            break;
        }
        
        if (pui4_buf_sz != NULL)
        {
            if (*pui4_buf_sz < pt_rque_ctrl->ui4_entry_sz)
            {
                i4_ret =  REC_UTILR_RQUE_ERR;
                break;
            }
            *pui4_buf_sz = pt_rque_ctrl->ui4_entry_sz;
        }
        
        pui1_read_ptr = pt_rque_ctrl->pui1_start_addr 
                        + pt_rque_ctrl->ui4_read_idx * pt_rque_ctrl->ui4_entry_sz;
        if (pui1_buf != NULL)
        {
            x_memcpy(pui1_buf, pui1_read_ptr, pt_rque_ctrl->ui4_entry_sz);
            
        }
        pt_rque_ctrl->ui4_read_idx = (pt_rque_ctrl->ui4_read_idx + 1) 
                                      % (pt_rque_ctrl->ui4_total_entry_cnt);
        --(pt_rque_ctrl->ui4_avail_entry_cnt);                              
    }while (0);
    REC_UTIL_UNLOCK(pt_rque_ctrl->h_lock);

    return i4_ret;
}

INT32 rec_util_rque_copy_entry(HANDLE_T       h_handle,
                              UINT32         ui4_idx,
                              UINT8*         pui1_buf,
                              UINT32*        pui4_buf_sz)
{
    INT32                           i4_ret;
    HANDLE_TYPE_T                   e_handle_type;
    VOID*                           pv_obj = NULL;
    REC_UTIL_RING_QUEUE_CTRL_T*     pt_rque_ctrl;
    UINT8*                          pui1_read_ptr = NULL;

    if (NULL == pui1_buf)
    {
        REC_UTIL_DBG_ERR(("rec_util_rque_copy_entry: pui1_buf is null!\r\n"));
        return REC_UTILR_RQUE_ERR;
    }
    
    i4_ret = handle_get_type_obj(h_handle, &e_handle_type, &pv_obj);
    if ((HR_OK != i4_ret) || (RU_HT_RING_QUEUE != e_handle_type) || (NULL == pv_obj))
    {
        REC_UTIL_DBG_ERR(("rec_util_rque_copy_entry: invalid handle!\r\n"));
        return REC_UTILR_RQUE_ERR;
    }
    
    pt_rque_ctrl = (REC_UTIL_RING_QUEUE_CTRL_T*)pv_obj;
    if (!pt_rque_ctrl->b_init)
    {
        REC_UTIL_DBG_ERR(("rec_util_rque_copy_entry: not init!\r\n"));
        return REC_UTILR_RQUE_ERR;
    }
    
    REC_UTIL_LOCK(pt_rque_ctrl->h_lock);
    i4_ret =  REC_UTILR_RQUE_OK;
    do 
    {
        if (pt_rque_ctrl->ui4_avail_entry_cnt == 0)
        {
            i4_ret = REC_UTILR_RQUE_EMPTY;
            break;
        }
        
        if (*pui4_buf_sz < pt_rque_ctrl->ui4_entry_sz)
        {
            i4_ret =  REC_UTILR_RQUE_ERR;
            break;
        }
        
        if (pt_rque_ctrl->ui4_write_idx > pt_rque_ctrl->ui4_read_idx)
        {
            if ((ui4_idx < pt_rque_ctrl->ui4_read_idx) 
                 || (ui4_idx >= pt_rque_ctrl->ui4_write_idx))
            {
                i4_ret =  REC_UTILR_RQUE_INV_ARG;
                REC_UTIL_DBG_ERR(("rec_util_rque_copy_entry:\r\n"
                                  "ui4_write_idx = %u\r\n"
                                  "ui4_read_idx  = %u\r\n"
                                  "ui4_idx       = %u\r\n",
                                  pt_rque_ctrl->ui4_write_idx,
                                  pt_rque_ctrl->ui4_read_idx,
                                  ui4_idx));
                break;
            }
        }
        else
        {
            if ((ui4_idx >= pt_rque_ctrl->ui4_write_idx) 
                 && (ui4_idx < pt_rque_ctrl->ui4_read_idx))
            {
                i4_ret =  REC_UTILR_RQUE_INV_ARG;
                REC_UTIL_DBG_ERR(("rec_util_rque_copy_entry:\r\n"
                                  "ui4_write_idx = %u\r\n"
                                  "ui4_read_idx  = %u\r\n"
                                  "ui4_idx       = %u\r\n",
                                  pt_rque_ctrl->ui4_write_idx,
                                  pt_rque_ctrl->ui4_read_idx,
                                  ui4_idx));
                break;
            }
        }
        
        pui1_read_ptr = pt_rque_ctrl->pui1_start_addr 
                        + ui4_idx * pt_rque_ctrl->ui4_entry_sz;
        x_memcpy(pui1_buf, pui1_read_ptr, pt_rque_ctrl->ui4_entry_sz);
        *pui4_buf_sz = pt_rque_ctrl->ui4_entry_sz;
        
    }while (0);
    REC_UTIL_UNLOCK(pt_rque_ctrl->h_lock);

    return i4_ret;
}

extern INT32 rec_util_rque_get_entry(HANDLE_T       h_handle,
                                         UINT32     ui4_idx,
                                         UINT8**    ppui1_buf)
{
    INT32                           i4_ret;
    HANDLE_TYPE_T                   e_handle_type;
    VOID*                           pv_obj = NULL;
    REC_UTIL_RING_QUEUE_CTRL_T*     pt_rque_ctrl;
    UINT8*                          pui1_read_ptr = NULL;

    if (NULL == ppui1_buf)
    {
        REC_UTIL_DBG_ERR(("rec_util_rque_get_entry: pui1_buf is null!\r\n"));
        return REC_UTILR_RQUE_ERR;
    }
    
    i4_ret = handle_get_type_obj(h_handle, &e_handle_type, &pv_obj);
    if ((HR_OK != i4_ret) || (RU_HT_RING_QUEUE != e_handle_type) || (NULL == pv_obj))
    {
        REC_UTIL_DBG_ERR(("rec_util_rque_get_entry: invalid handle!\r\n"));
        return REC_UTILR_RQUE_ERR;
    }
    
    pt_rque_ctrl = (REC_UTIL_RING_QUEUE_CTRL_T*)pv_obj;
    if (!pt_rque_ctrl->b_init)
    {
        REC_UTIL_DBG_ERR(("rec_util_rque_get_entry: not init!\r\n"));
        return REC_UTILR_RQUE_ERR;
    }
    
    REC_UTIL_LOCK(pt_rque_ctrl->h_lock);
    i4_ret =  REC_UTILR_RQUE_OK;
    do 
    {
        if (pt_rque_ctrl->ui4_avail_entry_cnt == 0)
        {
            i4_ret = REC_UTILR_RQUE_EMPTY;
            break;
        }
        
        if (pt_rque_ctrl->ui4_write_idx > pt_rque_ctrl->ui4_read_idx)
        {
            if ((ui4_idx < pt_rque_ctrl->ui4_read_idx) 
                 || (ui4_idx >= pt_rque_ctrl->ui4_write_idx))
            {
                i4_ret =  REC_UTILR_RQUE_INV_ARG;
                REC_UTIL_DBG_ERR(("rec_util_rque_get_entry:\r\n"
                                  "ui4_write_idx = %u\r\n"
                                  "ui4_read_idx  = %u\r\n"
                                  "ui4_idx       = %u\r\n",
                                  pt_rque_ctrl->ui4_write_idx,
                                  pt_rque_ctrl->ui4_read_idx,
                                  ui4_idx));
                break;
            }
        }
        else
        {
            if ((ui4_idx >= pt_rque_ctrl->ui4_write_idx) 
                 && (ui4_idx < pt_rque_ctrl->ui4_read_idx))
            {
                i4_ret =  REC_UTILR_RQUE_INV_ARG;
                REC_UTIL_DBG_ERR(("rec_util_rque_get_entry:\r\n"
                                  "ui4_write_idx = %u\r\n"
                                  "ui4_read_idx  = %u\r\n"
                                  "ui4_idx       = %u\r\n",
                                  pt_rque_ctrl->ui4_write_idx,
                                  pt_rque_ctrl->ui4_read_idx,
                                  ui4_idx));
                break;
            }
        }
        
        pui1_read_ptr = pt_rque_ctrl->pui1_start_addr 
                        + ui4_idx * pt_rque_ctrl->ui4_entry_sz;
        *ppui1_buf = pui1_read_ptr;
        
    }while (0);
    REC_UTIL_UNLOCK(pt_rque_ctrl->h_lock);

    return i4_ret;

}

INT32 rec_util_rque_delete(HANDLE_T       h_handle)
{
    INT32          i4_ret;
    
    i4_ret =  handle_free(h_handle, FALSE);
    if (i4_ret != HR_OK)
    {
        REC_UTIL_DBG_ERR(("rec_util_rque_delete:free_handle err(%d)!\r\n",
                          i4_ret));
        return REC_UTILR_RQUE_ERR;
    }
    else
    {
        return REC_UTILR_RQUE_OK;
    }
}

BOOL rec_util_rque_chk_index_in_range(HANDLE_T   h_handle,
                                      UINT32     ui4_idx)
{
    INT32                           i4_ret;
    BOOL                            b_ret;
    HANDLE_TYPE_T                   e_handle_type;
    VOID*                           pv_obj = NULL;
    REC_UTIL_RING_QUEUE_CTRL_T*     pt_rque_ctrl;
    
    
    i4_ret = handle_get_type_obj(h_handle, &e_handle_type, &pv_obj);
    if ((HR_OK != i4_ret) || (RU_HT_RING_QUEUE != e_handle_type) || (NULL == pv_obj))
    {
        REC_UTIL_DBG_ERR(("rec_util_rque_chk_index_in_range: invalid handle!\r\n"));
        return FALSE;
    }
    
    pt_rque_ctrl = (REC_UTIL_RING_QUEUE_CTRL_T*)pv_obj;
    if (!pt_rque_ctrl->b_init)
    {
        REC_UTIL_DBG_ERR(("rec_util_rque_chk_index_in_range: not init!\r\n"));
        return FALSE;
    }
    
    REC_UTIL_LOCK(pt_rque_ctrl->h_lock);
    do
    {
    
        if (pt_rque_ctrl->ui4_avail_entry_cnt == 0)
        {
            REC_UTIL_DBG_ERR(("rec_util_rque_chk_index_in_range: is empty!\r\n"));
            b_ret =  FALSE;
            break;
        }
        
        if (pt_rque_ctrl->ui4_write_idx > pt_rque_ctrl->ui4_read_idx)
        {
            if((pt_rque_ctrl->ui4_read_idx > ui4_idx) ||
                (pt_rque_ctrl->ui4_write_idx <= ui4_idx))
            {
                b_ret = FALSE;
                break;
            }
            else
            {
                b_ret = TRUE;
                break;
            }
        }
        else if (pt_rque_ctrl->ui4_write_idx < pt_rque_ctrl->ui4_read_idx)
        {
            if ((pt_rque_ctrl->ui4_read_idx > ui4_idx) &&
                (pt_rque_ctrl->ui4_write_idx <= ui4_idx))
            {
                b_ret = FALSE;
                break;
            }
            else
            {
                b_ret = TRUE;
                break;
            }
        }
        else
        {
            REC_UTIL_DBG_ERR(("rec_util_rque_chk_index_in_range: is empty!\r\n"));
            b_ret = FALSE;
            break;
        }
    }while(0);
    
    REC_UTIL_UNLOCK(pt_rque_ctrl->h_lock);

    return b_ret;
}

INT32 rec_util_alloc_fm_cache(  REC_UTIL_FM_CACHE_T*  pt_fm_cache, UINT32 ui4_size)
{
    UINT32 ui4_capacity;
    if(NULL == pt_fm_cache)
    {
        return REC_UTILR_INV_ARG;
    }

    ui4_capacity = (UINT32)REC_DB_ALIGN_ADDR_FORWARD(ui4_size, 512);
    pt_fm_cache->pv_start = x_mem_alloc(ui4_capacity + 32);
    if (NULL == pt_fm_cache->pv_start)
    {
        return REC_UTILR_OUT_OF_MEMORY;
    }
    x_memset(pt_fm_cache->pv_start, 0, ui4_capacity+32);
    
    pt_fm_cache->pv_align = REC_DB_ALIGN_ADDR_FORWARD(pt_fm_cache->pv_start, 32);
    pt_fm_cache->ui4_cache_sz   = ui4_capacity;
    pt_fm_cache->ui4_data_sz    = 0;

    return REC_UTILR_OK;
}

VOID  rec_util_free_fm_cache(REC_UTIL_FM_CACHE_T*  pt_fm_cache)
{
    if (NULL == pt_fm_cache)
    {
        return;
    }
    
    if (NULL != pt_fm_cache->pv_start)
    {
        x_mem_free(pt_fm_cache->pv_start);
        pt_fm_cache->pv_start = NULL;
        pt_fm_cache->pv_align = NULL;
    }
 
    pt_fm_cache->ui4_cache_sz = 0;
    pt_fm_cache->ui4_data_sz  = 0;

    return;
}

INT32 rec_util_cli_write_with_cache(HANDLE_T                h_file,
                                    REC_UTIL_FM_CACHE_T*    pt_fm_cache,
                                    BOOL                    b_finish_write,
                                    const VOID*             pv_data,
                                    UINT32                  ui4_count,
                                    UINT32*                 pui4_write)
{
    INT32       i4_ret;
    UINT32      ui4_left_sz;
    UINT32      ui4_copy_sz;
    UINT8*      pui1_data_src;
    UINT32      ui4_write_sz;
    
    if( NULL == pt_fm_cache || (NULL == pv_data && ui4_count > 0)||
        NULL == pui4_write || NULL_HANDLE == h_file)
    {
        x_dbg_stmt("_rec_util_cli_write_with_cache:invalid param.\r\n");
        return REC_UTILR_INV_ARG;
    }

    pui1_data_src = (UINT8*)pv_data;
    ui4_left_sz = ui4_count;
    *pui4_write = 0;

    while(ui4_left_sz > 0)
    {
        ui4_copy_sz = ((pt_fm_cache->ui4_cache_sz - pt_fm_cache->ui4_data_sz) < 
                        ui4_left_sz) ?
                        (pt_fm_cache->ui4_cache_sz - pt_fm_cache->ui4_data_sz) :
                        ui4_left_sz;
        
        if(NULL == x_memcpy((VOID*)((UINT8*)pt_fm_cache->pv_align + pt_fm_cache->ui4_data_sz),
                                    pui1_data_src, ui4_copy_sz))
        {
            x_dbg_stmt("_rec_util_cli_write_with_cache:copy to cache error.\r\n");
            return REC_UTILR_INTERNAL_ERROR;
        }
                    
        ui4_left_sz                 -= ui4_copy_sz;
        pui1_data_src               += ui4_copy_sz;
        pt_fm_cache->ui4_data_sz    += ui4_copy_sz;

        /*cache is full, write to file and clear cache*/
        if(pt_fm_cache->ui4_cache_sz == pt_fm_cache->ui4_data_sz)
        {
            i4_ret = x_fm_mfw_write(h_file, pt_fm_cache->pv_align, 
                                pt_fm_cache->ui4_cache_sz, 
                                &ui4_write_sz);
            if(FMR_OK != i4_ret || pt_fm_cache->ui4_cache_sz != ui4_write_sz)
            {
                x_dbg_stmt("_rec_util_cli_write_with_cache:write fail.\r\n");
                return REC_UTILR_INTERNAL_ERROR;
            }
            
            *pui4_write += pt_fm_cache->ui4_cache_sz;
            pt_fm_cache->ui4_data_sz = 0;       
        }
                 
    }

    /*it's the last output, so write to file and clear cache*/
    if(b_finish_write && pt_fm_cache->ui4_data_sz > 0)
    {
        i4_ret = x_fm_mfw_write(h_file, pt_fm_cache->pv_align, 
                            pt_fm_cache->ui4_cache_sz, 
                            &ui4_write_sz);
        if(FMR_OK != i4_ret || pt_fm_cache->ui4_cache_sz != ui4_write_sz)
        {
            x_dbg_stmt("_rec_util_cli_write_with_cache:write fail.\r\n");
            return REC_UTILR_INTERNAL_ERROR;
        }
        
        *pui4_write += pt_fm_cache->ui4_data_sz;
        pt_fm_cache->ui4_data_sz = 0; 
    }
    return REC_UTILR_OK;
}


