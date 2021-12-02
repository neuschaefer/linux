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
                    include files
 ----------------------------------------------------------------------------*/
#include "dbg.h"
#include "x_linux.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/* Debug stuff. */
#ifdef ABORT
#undef ABORT
#endif

#define ABORT(_cat, _val)  DBG_ABORT ((DBG_MOD_DEBUG | _cat | _val))

#define DBG_ABRT_NO_STMT_SEMAPHORE         ((UINT32) 0x00000000)
#define DBG_ABRT_CANNOT_GET_THREAD_HANDLE  ((UINT32) 0x00000001)
#define DBG_ABRT_STATEMENT_TOO_LONG        ((UINT32) 0x00000002)
#define DBG_ABRT_CANNOT_REG_WITH_CLI       ((UINT32) 0x00000003)

/* Initial debug select settings. */
#ifndef DBG_INIT_SELECT
#define DBG_INIT_SELECT  DBG_SEL_INDIVIDUAL_COMMON
#endif

/* The maximum debug statement length is 511 characters. */
#define MAX_STMT_LEN 512

/* Debug selection */
typedef enum
{
    DBG_SEL_INDIVIDUAL = 0,
    DBG_SEL_COMMON,
    DBG_SEL_INDIVIDUAL_COMMON
}   DBG_SEL_T;

/* Thread record. */
typedef struct _THREAD_REC_T
{
    struct _THREAD_REC_T* pt_next;

    HANDLE_T  h_thread;

    UINT16  ui2_id;

    CHAR*  ps_name;
}   THREAD_REC_T;

/* Trace record flag definitions. */
#define FLAG_DATA_VALID  ((UINT8) 0x01)

/* Trace record. */
typedef struct _TRACE_REC_T
{
    HANDLE_T  h_thread;                    /* Thread id. */

    UINT32    ui4_time_stamp;              /* Time stamp. */
    UINT32    ui4_code;                    /* debug code. */

    SIZE_T  z_len;                         /* Data length. */

    UINT8  ui1_flags;

    UINT8  aui1_data [MAX_TRACE_REC_LEN];  /* Data array. */
}   TRACE_REC_T;


/*-----------------------------------------------------------------------------
 * Name: dbg_add_thread_name
 *
 * Description: This API adds a thread name to the thread record list.
 *
 * Inputs:  ps_name  References the thread name.
 *
 * Outputs: -
 *
 * Returns: DBGR_OK               Routine successful.
 *          DBGR_NOT_ENOUGH_MEM   Not enough memory to add the thread name
 *                                record.
 *          DBGR_NOT_INIT         Debug library not initialized.
 *          DBGR_NOT_ENABLED      Debug library not compiled with define
 *                                DEBUG.
 ----------------------------------------------------------------------------*/
INT32 dbg_add_thread_name (CHAR*  ps_name)
{
    INT32 i4_return;

    i4_return = DBGR_NOT_ENABLED;

    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name: dbg_add_trace_rec
 *
 * Description: This API adds a trace record to the trace buffer if the trace
 *              buffer is enabled.
 *
 * Inputs:  b_isr      This argument is set to TRUE if this API is called from
 *                     within an ISR routine, else FALSE.
 *          ui4_code   Contains the debug code.
 *          pv_data    References the data which shall be added to the trace
 *                     buffer.
 *          z_len      Contains the length of the data referenced by pv_data.
 *
 * Outputs: -
 *
 * Returns: DBGR_OK               Routine successful.
 *          DBGR_NOT_INIT         Debug library not initialized.
 *          DBGR_NO_TRACE_BUFFER  No trace buffer allocated.
 *          DBGR_NOT_ENABLED      Debug library not compiled with define
 *                                DEBUG.
 ----------------------------------------------------------------------------*/
INT32 dbg_add_trace_rec (BOOL    b_isr,
                         UINT32  ui4_code,
                         VOID*   pv_data,
                         SIZE_T  z_len)
{
    INT32 i4_return;

    i4_return = DBGR_NOT_ENABLED;

    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name: dbg_trace_init
 *
 * Description: This API initializes the Debug libraries trace buffer.
 *
 * Inputs:  ui2_num_trace_recs  Specifies the number of records in the trace
 *                              buffer.
 *
 * Outputs: -
 *
 * Returns: DBGR_OK              Routine successful.
 *          DBGR_NOT_ENOUGH_MEM  Not enough memory for the trace buffer.
 *          DBGR_ALREADY_INIT    Debug library already initialized.
 ----------------------------------------------------------------------------*/
INT32 dbg_trace_init (UINT16  ui2_num_trace_recs)
{
    INT32 i4_return;

    i4_return = DBGR_OK;

    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name: dbg_set_ctrl_stmt
 *
 * Description: This API sets the debug control statement flag 'b_stmt_ctrl'.
 *
 * Inputs:  b_ctrl  Contains the new debug control statment flag setting.
 *
 * Outputs: -
 *
 * Returns: DBGR_OK           Routine successful.
 *          DBGR_NOT_INIT     Debug library not initialized.
 *          DBGR_NOT_ENABLED  Debug library not compiled with define DEBUG.
 ----------------------------------------------------------------------------*/
INT32 dbg_set_ctrl_stmt (BOOL  b_ctrl)
{
    INT32 i4_return;

    i4_return = DBGR_NOT_ENABLED;

    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name: dbg_get_ctrl_stmt
 *
 * Description: This API returns the state of the debug control statement flag
 *              'b_stmt_ctrl'. In case the debug library is not initialized
 *              or not compiled with DEBUG set, this API will always return
 *              FALSE.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: TRUE if the flag 'b_stmt_ctrl' is set else FALSE.
 ----------------------------------------------------------------------------*/
BOOL dbg_get_ctrl_stmt (VOID)
{
    BOOL b_return;

    b_return = FALSE;

    return (b_return);
}


/*-----------------------------------------------------------------------------
 * Name: dbg_reg_ctrl_stmt_cb
 *
 * Description: This API registers an output callback function which is called
 *              when the API's '[xc]_dbg_ctrl_stmt' are executed.
 *
 * Inputs:  pf_output  References the output callback function.
 *
 * Outputs: -
 *
 * Returns: DBGR_OK           Routine successful.
 *          DBGR_NOT_INIT     Debug library not initialized.
 *          DBGR_NOT_ENABLED  Debug library not compiled with define DEBUG.
 ----------------------------------------------------------------------------*/
INT32 dbg_reg_ctrl_stmt_cb (x_dbg_output_fct  pf_output)
{
    INT32 i4_return;

    i4_return = DBGR_NOT_ENABLED;

    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name: dbg_eval_dbg_level
 *
 * Description: This API receives a software module debug level and depending
 *              on internal settings via the CLI will compute a final debug
 *              level which is returned to the aller.
 *
 * Inputs:  ui2_dbg_level  Contains the software modules debug level.
 *
 * Outputs: -
 *
 * Returns: A final calculated debug level.
 ----------------------------------------------------------------------------*/
UINT16 dbg_eval_dbg_level (UINT16  ui2_dbg_level)
{
    ui2_dbg_level = DBG_LEVEL_NONE;

    return (ui2_dbg_level);
}


/*-----------------------------------------------------------------------------
 * Name: x_dbg_dump_trace_buff
 *
 * Description: This API dumps the content of the trace buffer. Note that the
 *              trace buffer will be disabled duriung the dumping and
 *              re-enabled (if required) afterwards.
 *
 * Inputs:  pf_output  References a rotine which will handle the outputing
 *                     of the trace record.
 *
 * Outputs: -
 *
 * Returns: DBGR_OK                Routine successful.
 *          DBGR_NOT_INIT          Debug library not initialized.
 *          DBGR_NO_TRACE_BUFFER   No trace buffer allocated.
 *          DBGR_NO_OUTPUT_DEVICE  No output device available.
 *          DBGR_NOT_ENABLED       Debug library not compiled with define
 *                                 DEBUG.
 *          DBGR_DUMP_IN_PROGRESS  Trace buffer dump is in progress.
 ----------------------------------------------------------------------------*/
INT32 x_dbg_dump_trace_buff (x_dbg_output_fct  pf_output)
{
    INT32 i4_return;

    i4_return = DBGR_NOT_ENABLED;

    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name: x_dbg_flush_trace_buff
 *
 * Description: This API clears the trace buffer. Essentially, it simply resets
 *              the read and write indices.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: DBGR_OK               Routine successful.
 *          DBGR_NOT_INIT         Debug library not initialized.
 *          DBGR_NO_TRACE_BUFFER  No trace buffer allocated.
 *          DBGR_NOT_ENABLED      Debug library not compiled with define
 *                                DEBUG.
 ----------------------------------------------------------------------------*/
INT32 x_dbg_flush_trace_buff (VOID)
{
    INT32 i4_return;

    i4_return = DBGR_NOT_ENABLED;

    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name: x_dbg_get_trace_buff_ctrl
 *
 * Description: This API returns the trace buffer state.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: TRUE if the trace record buffer is enabled else FALSE.
 ----------------------------------------------------------------------------*/
BOOL x_dbg_get_trace_buff_ctrl (VOID)
{
    return (FALSE);
}


/*-----------------------------------------------------------------------------
 * Name: x_dbg_reg_trace_rec_cb
 *
 * Description: This API registers a record trace callback function. To prevent
 *              simmultanous access of the callback function pointer, a critical
 *              section start / stop is used.
 *
 * Inputs:  pf_trace_rec  References the record trace callback fucntion.
 *
 * Outputs: -
 *
 * Returns: DBGR_OK             Routine successful.
 *          DBGR_NOT_INIT       Debug library not initialized.
 *          DBGR_NOT_ENABLED    Debug library not compiled with define DEBUG
 *          DBGR_REG_CB_ACTIVE  A callback is still active.
 ----------------------------------------------------------------------------*/
INT32 x_dbg_reg_trace_rec_cb (x_dbg_trace_rec_fct  pf_trace_rec)
{
    INT32 i4_return;

    i4_return = DBGR_NOT_ENABLED;

    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name: x_dbg_reg_data_inp_cb
 *
 * Description: This API registers a data input callback function with the
 *              debug library.
 *
 * Inputs:  pf_data_inp  References the data receive callback function.
 *          pv_tag       Contains some tag value which is returned with the
 *                       callback function.
 * Outputs: -
 *
 * Returns: DBGR_OK             Routine successful.
 *          DBGR_NOT_INIT       Debug library not initialized.
 *          DBGR_NOT_ENABLED    Debug library not compiled with define DEBUG
 *          DBGR_REG_CB_ACTIVE  A callback is still active.
 ----------------------------------------------------------------------------*/
INT32 x_dbg_reg_data_inp_cb (x_dbg_data_inp_fct  pf_data_inp,
                             VOID*               pv_tag)
{
	INT32 i4_return;

    i4_return = DBGR_NOT_ENABLED;

    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name: x_dbg_set_trace_buff_ctrl
 *
 * Description: This API enables or disables the trace buffer recording. This
 *              API may be called from within an interrupt context.
 *
 * Inputs:  b_trace_ena  Set to TRUE of the trace buffer recording shall be
 *                       enabled else FALSE.
 *
 * Outputs: -
 *
 * Returns: DBGR_OK               Routine successful.
 *          DBGR_NOT_INIT         Debug library not initialized.
 *          DBGR_NO_TRACE_BUFFER  No trace buffer allocated.
 *          DBGR_NOT_ENABLED      Debug library not compiled with define
 *                                DEBUG.
 ----------------------------------------------------------------------------*/
INT32 x_dbg_set_trace_buff_ctrl (BOOL  b_trace_ena)
{
    INT32 i4_return;

    i4_return = DBGR_NOT_ENABLED;

    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name: x_dbg_ctrl_stmt
 *
 * Description: This API sends a debug statement to the output device. However,
 *              the output may be enabled / disabled via the flag\
 *              'b_stmt_ctrl'.
 *
 * Inputs:  ps_format  References the format string.
 *
 * Outputs: -
 *
 * Returns: DBGR_OK           Routine successful.
 *          DBGR_INV_ARG      ps_format contains a NULL pointer.
 *          DBGR_NOT_INIT     Debug library not initialized.
 *          DBGR_NOT_ENABLED  Debug library not compiled with define DEBUG.
 ----------------------------------------------------------------------------*/
INT32 x_dbg_ctrl_stmt (const CHAR*  ps_format, ...)
{
    INT32   i4_return;

    i4_return = DBGR_NOT_ENABLED;

    return (i4_return);
}

/*
VOID x_dbg_abort (UINT32  ui4_line_num,
                  UINT32  ui4_code)
{
    panic("\nAbort fails at:\nLine: %d, Code: %d\n", (int)ui4_line_num, (int)ui4_code);
}


EXPORT_SYMBOL(x_dbg_abort);
*/
