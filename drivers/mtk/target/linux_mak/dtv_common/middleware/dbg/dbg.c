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
 * $RCSfile: dbg.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This file contains the implementation of the debug library. Note
 *         that many API's are only enabled if this module is compiled with
 *         the define DEBUG set.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/


#include "dbg/dbg.h"
#include "dbg/def_dbg_level_mw.h"
#include "cli/x_cli.h"
#include "inc/x_common.h"
#include "os/inc/os.h"


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

/* Some CLI definitions. */
#define CLI_DBG_MODULE       "dbg"
#define CLI_DBG_MODULE_HELP  "DBG module commands"

#define CLI_SET_DBG_LEVEL  CLI_SET_DBG_LVL_STR
#define CLI_GET_DBG_LEVEL  CLI_GET_DBG_LVL_STR
#define CLI_SET_DBG_SEL    "sds"
#define CLI_GET_DBG_SEL    "gds"

#define CLI_SET_DBG_LEVEL_HELP  CLI_SET_DBG_LVL_HELP_STR
#define CLI_GET_DBG_LEVEL_HELP  CLI_GET_DBG_LVL_HELP_STR
#define CLI_SET_DBG_SEL_HELP    "Set debug select state (i = individual, c = common, b = individual + common)"
#define CLI_GET_DBG_SEL_HELP    "Get debug select state (i = individual, c = common, b = individual + common)"

#define CLI_NUM_ARGS_DBG_SEL  ((INT32) 2)
#define CLI_ARG_IDX_DBG_SEL   1

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
                    data declarations
 ----------------------------------------------------------------------------*/

static BOOL b_output_init = FALSE;         /* Tracks if the debug output has been initialized. */

static HANDLE_T h_output_sema;             /* References the output semaphore. */

static CHAR s_out_stmt   [MAX_STMT_LEN];   /* Statement adjusted for output. */
static CHAR s_dbg_stmt   [MAX_STMT_LEN];   /* Debug statement. */

/* The following declarations are only required */
/* if DEBUG has been defined.                   */
#ifdef DEBUG
static x_dbg_data_inp_fct  pf_dbg_data_inp;
static x_dbg_output_fct    pf_dbg_output;


static BOOL b_trace_init  = FALSE;         /* Tracks if the debug trace component has been initialized. */

static BOOL b_trace_ctrl;                  /* Controls trace buffer recording. */
static BOOL b_trace_dump;                  /* Tracks if the trace buffer is being dumped. */

static TRACE_REC_T*  pt_trace_head;        /* Trace record buffer. */
static THREAD_REC_T* pt_thread_head;       /* Thread record list. */

static x_dbg_trace_rec_fct pf_dbg_trace_rec;

static UINT16 ui2_thread_id;               /* Keeps track of the thread id. */

static UINT16 ui2_rd_idx;                  /* Read index of the trace record buffer. */
static UINT16 ui2_wr_idx;                  /* Write index of the trace record buffer. */
static UINT16 ui2_num_recs;                /* Number of records in the trace record buffer. */

static CHAR s_trace_stmt [MAX_STMT_LEN];   /* Trace record statment. */

static VOID* pv_dbg_data_inp_tag;          /* Stores the data receive notify tag. */
#endif

/* The following declarations are only required */
/* if DEBUG or CLI_SUPPORT has been defined.    */
#if defined (DEBUG) || defined (CLI_SUPPORT)
static UINT16 ui2_common_dbg_level;        /* Common or central debug level. */
static BOOL b_stmt_ctrl;                   /* Controls statement output via "[xc]_dbg_ctrl_stmt". */
static DBG_SEL_T e_dbg_sel;                /* Debug selection. */
#endif


/*-----------------------------------------------------------------------------
                    functions declaraions
 ----------------------------------------------------------------------------*/

static VOID put_trace_stmt (x_dbg_output_fct  pf_output,
                            const CHAR*       ps_stmt);

#ifdef DEBUG
static VOID add_trace_rec (HANDLE_T h_thread,
                           UINT32   ui4_code,
                           VOID*    pv_data,
                           SIZE_T   z_len);
static THREAD_REC_T* find_thread_rec (HANDLE_T h_thread);
static VOID put_trace_hdr (x_dbg_output_fct  pf_output);
static VOID put_thread_info (x_dbg_output_fct  pf_output);
static VOID put_trace_buffer (x_dbg_output_fct  pf_output);

static VOID drv_data_rec (UINT8  ui1_data);
#endif

/* The following is CLI related stuff. */
#ifdef CLI_SUPPORT
static INT32 cli_get_dbg_level (INT32         i4_argc,
                                const CHAR**  pps_argv);
static INT32 cli_set_dbg_level (INT32         i4_argc,
                                const CHAR**  pps_argv);
static INT32 cli_get_dbg_sel (INT32         i4_argc,
                              const CHAR**  pps_argv);
static INT32 cli_set_dbg_sel (INT32         i4_argc,
                              const CHAR**  pps_argv);


static CLI_EXEC_T at_cli_dbg_cmd [] =
{
    {CLI_GET_DBG_LEVEL, NULL, cli_get_dbg_level, NULL, CLI_GET_DBG_LEVEL_HELP, CLI_SUPERVISOR},
    {CLI_SET_DBG_LEVEL, NULL, cli_set_dbg_level, NULL, CLI_SET_DBG_LEVEL_HELP, CLI_SUPERVISOR},
    {CLI_GET_DBG_SEL,   NULL, cli_get_dbg_sel,   NULL, CLI_GET_DBG_SEL_HELP,   CLI_SUPERVISOR},
    {CLI_SET_DBG_SEL,   NULL, cli_set_dbg_sel,   NULL, CLI_SET_DBG_SEL_HELP,   CLI_SUPERVISOR},
    END_OF_CLI_CMD_TBL
};

static CLI_EXEC_T at_cli_dbg [] =
{
    {CLI_DBG_MODULE, NULL, NULL, &(at_cli_dbg_cmd [0]), CLI_DBG_MODULE_HELP, CLI_SUPERVISOR},
    END_OF_CLI_CMD_TBL
};
#endif


/*-----------------------------------------------------------------------------
 * Name: put_trace_stmt
 *
 * Description: This API sends a trace buffer dump statement to the output
 *              device or the callback routine. This API will also convert the
 *              line terminating sequence from '\n' to '\n\r'. Note that this
 *              API will only create an output string of MAX_STMT_LEN length.
 *
 * Inputs:  pf_output  References a rotine which will handle the outputing
 *                     of the trace record.
 *          ps_stmt    References the statement.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID put_trace_stmt (x_dbg_output_fct  pf_output,
                            const CHAR*       ps_stmt)
{
    CHAR*  ps_out;
    SIZE_T z_len;


    /* Convert all '\n' to '\n\r' line termination sequences. */
    /* Note that we alreday reserve space for the final '\0'  */
    /* termination character.                                 */
    ps_out = &(s_out_stmt [0]);
    z_len  = MAX_STMT_LEN - 1;

    while (((*ps_stmt) != '\0') &&
           (z_len > 0))
    {
        if (((*(ps_out++)) = (*(ps_stmt++))) == '\n')
        {
            z_len--;

            /* We must reserve space for the final '\0' character. */
            if (z_len > 0)
            {
                (*(ps_out++)) = '\r';
            }
        }

        z_len--;
    }

    (*(ps_out++)) = '\0';

    /* And output the final statement */
    if (pf_output == NULL)
    {
        x_dbg_put_stmt ((const CHAR*) &(s_out_stmt [0]));
    }
    else
    {
        pf_output ((const CHAR*) &(s_out_stmt [0]));
    }
}


#ifdef DEBUG
/*-----------------------------------------------------------------------------
 * Name: add_trace_rec
 *
 * Description: This API adds a record to the trace buffer and increments the
 *              write index. This routine assumes that the value in argument
 *              z_len does not exceed the value of MAX_TRACE_REC_LEN. This API
 *              is fully re-entrant and uses critical section statments for
 *              protection.
 *
 *
 * Inputs:  h_thread  Contains the handle to the thread.
 *          ui4_code  Contains the debug code.
 *          pv_data   References the debug data.
 *          z_len     Contains the length of the debug data.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID add_trace_rec (HANDLE_T h_thread,
                           UINT32   ui4_code,
                           VOID*    pv_data,
                           SIZE_T   z_len)
{
    UINT16       ui2_idx;
    TRACE_REC_T* pt_trace_rec;
    CRIT_STATE_T t_state;


    /* Get next write index, set data as invalid    */
    /* and update write and read index. This must   */
    /* be done within a critical section to protect */
    /* for simmultanous access.                     */
    t_state = x_crit_start ();

    ui2_idx = ui2_wr_idx++;
    if (ui2_wr_idx >= ui2_num_recs)
    {
        ui2_wr_idx = 0;
    }

    if (ui2_wr_idx == ui2_rd_idx)
    {
        /* Write index caught up with read index */
        /* which now must also be updated.       */
        ui2_rd_idx++;
        if (ui2_rd_idx >= ui2_num_recs)
        {
            ui2_rd_idx = 0;
        }
    }

    pt_trace_head [ui2_idx].ui1_flags &= ~(FLAG_DATA_VALID);

    x_crit_end (t_state);

    pt_trace_rec = &(pt_trace_head [ui2_idx]);

    /* And now set the trace record. */
    pt_trace_rec->h_thread       = h_thread;
    pt_trace_rec->ui4_time_stamp = os_get_sys_tick ();
    pt_trace_rec->ui4_code       = ui4_code;

    if ((pt_trace_rec->z_len = z_len) > 0)
    {
        x_memcpy (((VOID*) &(pt_trace_rec->aui1_data [0])), pv_data, z_len);
    }

    /* And make the record valid */
    pt_trace_head [ui2_idx].ui1_flags |= FLAG_DATA_VALID;
}


/*-----------------------------------------------------------------------------
 * Name: find_thread_rec
 *
 * Description: This searches for a thread record given a thread handle. If a
 *              thread record with matching thread handle has been found this
 *              API will return a reference to this record else a NULL pointer.
 *
 * Inputs:  h_thread  Contains a thread handle.
 *
 * Outputs: -
 *
 * Returns: A reference to the thread record or a NULL pointer.
 ----------------------------------------------------------------------------*/
static THREAD_REC_T* find_thread_rec (HANDLE_T h_thread)
{
    CRIT_STATE_T  t_state;
    THREAD_REC_T* pt_thread_rec;


    /* Cycle through all stored threads. */
    t_state = x_crit_start ();

    pt_thread_rec = pt_thread_head;

    x_crit_end (t_state);

    while ((pt_thread_rec != NULL)                &&
           (pt_thread_rec->h_thread != h_thread))
    {
        pt_thread_rec = pt_thread_rec->pt_next;
    }

    return (pt_thread_rec);
}


/*-----------------------------------------------------------------------------
 * Name: put_trace_hdr
 *
 * Description: This API simply outputs a trace header.
 *
 * Inputs:  pf_output  References a rotine which will handle the outputing
 *                     of the trace record.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID put_trace_hdr (x_dbg_output_fct  pf_output)
{
    put_trace_stmt (pf_output, "\n");
    put_trace_stmt (pf_output, "TRACE BUFFER DUMP\n");
    put_trace_stmt (pf_output, "=================\n\n");
}


/*-----------------------------------------------------------------------------
 * Name: put_thread_info
 *
 * Description: This API dumps the thread information.
 *
 * Inputs:  pf_output  References a rotine which will handle the outputing
 *                     of the trace record.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID put_thread_info (x_dbg_output_fct  pf_output)
{
    CRIT_STATE_T  t_state;
    THREAD_REC_T* pt_thread_rec;


    /* Output header. */
    put_trace_stmt (pf_output, "\n");
    put_trace_stmt (pf_output, "Thread list\n");
    put_trace_stmt (pf_output, "===========\n\n");
    put_trace_stmt (pf_output, "Thread       Name\n");
    put_trace_stmt (pf_output, "-------------------------------------------------------------\n");

    /* Cycle through all stored threads. */
    t_state = x_crit_start ();

    pt_thread_rec = pt_thread_head;

    x_crit_end (t_state);

    while (pt_thread_rec != NULL)
    {
        x_sprintf (&(s_trace_stmt [0]), "  %3d        %s\n",
                   pt_thread_rec->ui2_id,
                   pt_thread_rec->ps_name);

        put_trace_stmt (pf_output, ((const CHAR*) &(s_trace_stmt [0])));

        pt_thread_rec = pt_thread_rec->pt_next;
    }

    put_trace_stmt (pf_output, "\n");
}


/*-----------------------------------------------------------------------------
 * Name: put_trace_buff
 *
 * Description: This API dumps the trace buffer.
 *
 * Inputs:  pf_output  References a rotine which will handle the outputing
 *                     of the trace record.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID put_trace_buffer (x_dbg_output_fct  pf_output)
{
    UINT16        ui2_rec_idx;
    UINT16        ui2_data_idx;
    UINT16        ui2_last_rec_idx;
    TRACE_REC_T*  pt_trace_rec;
    CRIT_STATE_T  t_state;
    THREAD_REC_T* pt_thread_rec;


    /* Output header. */
    put_trace_stmt (pf_output, "\n");
    put_trace_stmt (pf_output, "Trace buffer\n");
    put_trace_stmt (pf_output, "============\n\n");
    put_trace_stmt (pf_output, "Time stamp   Thread       Debug code   Data\n");
    put_trace_stmt (pf_output, "---------------------------------------------------------------------------------------------------\n");

    t_state = x_crit_start ();

    ui2_rec_idx      = ui2_rd_idx;
    ui2_last_rec_idx = ui2_wr_idx;

    x_crit_end (t_state);

    /* And now dump the records. */
    while (ui2_rec_idx != ui2_last_rec_idx)
    {
        pt_trace_rec = &(pt_trace_head [ui2_rec_idx]);

        /* Only process records which have valid data. */
        if ((pt_trace_rec->ui1_flags & FLAG_DATA_VALID) != 0)
        {
            x_sprintf (&(s_trace_stmt [0]), " %09d   ", pt_trace_rec->ui4_time_stamp);

            put_trace_stmt (pf_output, ((const CHAR*) &(s_trace_stmt [0])));

            if (pt_trace_rec->h_thread != NULL_HANDLE)
            {
                /* Get the thread id if possible. */
                if ((pt_thread_rec = find_thread_rec (pt_trace_rec->h_thread)) != NULL)
                {
                    x_sprintf (&(s_trace_stmt [0]), "  %3d     ", pt_thread_rec->ui2_id);
                }
                else
                {
                    x_sprintf (&(s_trace_stmt [0]), "0x%08x", pt_trace_rec->h_thread);
                }

                put_trace_stmt (pf_output, ((const CHAR*) &(s_trace_stmt [0])));
            }
            else
            {
                /* This record was added as interrupt context. */
                put_trace_stmt (pf_output, "  ISR     ");
            }

            /* Output debug code. */
            x_sprintf (&(s_trace_stmt [0]), "   0x%08x   ", pt_trace_rec->ui4_code);
            put_trace_stmt (pf_output, ((const CHAR*) &(s_trace_stmt [0])));

            /* Output data. */
            ui2_data_idx = 0;
            while (ui2_data_idx < pt_trace_rec->z_len)
            {
                x_sprintf (&(s_trace_stmt [0]), "0x%02x ", pt_trace_rec->aui1_data [ui2_data_idx]);

                put_trace_stmt (pf_output, ((const CHAR*) &(s_trace_stmt [0])));

                ui2_data_idx++;
            }

            put_trace_stmt (pf_output, "\n");
        }

        /* And update the record index. */
        ui2_rec_idx++;
        if (ui2_rec_idx >= ui2_num_recs)
        {
            ui2_rec_idx = 0;
        }
    }

    put_trace_stmt (pf_output, "\n");
}


/*-----------------------------------------------------------------------------
 * Name: drv_data_rec
 *
 * Description: This API is called when the debug port receives a character
 *              and sends it to the registerd input data callback function.
 *
 * Inputs:  ui1_data  Contains the received data Byte.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID drv_data_rec (UINT8  ui1_data)
{
	VOID*               pv_data_inp_tag;
	CRIT_STATE_T        t_state;
	x_dbg_data_inp_fct  pf_data_inp;


	t_state = x_crit_start ();

	if (pf_dbg_data_inp != NULL)
	{
		pf_data_inp      = pf_dbg_data_inp;
		pv_data_inp_tag  = pv_dbg_data_inp_tag;

		x_crit_end (t_state);

		(*pf_data_inp) (ui1_data, pv_data_inp_tag);
	}
	else
	{
		x_crit_end (t_state);
	}
}
#endif


#ifdef CLI_SUPPORT
/*-----------------------------------------------------------------------------
 * Name: cli_get_dbg_level
 *
 * Description: This function is called by the CLI and handles the debug level
 *              get command.
 *
 * Inputs:  i4_argc   Contains the number of arguments.
 *          pps_argv  References a list of arguments.
 *
 * Outputs: -
 *
 * Returns: Always CLIR_OK.
 ----------------------------------------------------------------------------*/
static INT32 cli_get_dbg_level (INT32         i4_argc,
                                const CHAR**  pps_argv)
{
    x_cli_show_dbg_level (ui2_common_dbg_level);

    return (CLIR_OK);
}


/*-----------------------------------------------------------------------------
 * Name: cli_set_dbg_level
 *
 * Description: This function is called by the CLI and handles the debug level
 *              set command.
 *
 * Inputs:  i4_argc   Contains the number of arguments.
 *          pps_argv  References a list of arguments.
 *
 * Outputs: -
 *
 * Returns: Always CLIR_OK.
 ----------------------------------------------------------------------------*/
static INT32 cli_set_dbg_level (INT32         i4_argc,
                                const CHAR**  pps_argv)
{
    UINT16 ui2_dbg_level;


    if (x_cli_parse_dbg_level (i4_argc, pps_argv, &ui2_dbg_level) == CLIR_OK)
    {
        ui2_common_dbg_level = ui2_dbg_level;
    }

    return (CLIR_OK);
}


/*-----------------------------------------------------------------------------
 * Name: cli_get_dbg_sel
 *
 * Description: This function is called by the CLI and handles the debug select
 *              get command.
 *
 * Inputs:  i4_argc   Contains the number of arguments.
 *          pps_argv  References a list of arguments.
 *
 * Outputs: -
 *
 * Returns: Always CLIR_OK.
 ----------------------------------------------------------------------------*/
static INT32 cli_get_dbg_sel (INT32         i4_argc,
                              const CHAR**  pps_argv)
{
    x_dbg_stmt ("Debug select: ");

    switch (e_dbg_sel)
    {
        case DBG_SEL_INDIVIDUAL:
            x_dbg_stmt ("i\n");
            break;

        case DBG_SEL_COMMON:
            x_dbg_stmt ("c\n");
            break;

        case DBG_SEL_INDIVIDUAL_COMMON:
            x_dbg_stmt ("b\n");
            break;

        default:
            x_dbg_stmt ("unknown\n");
            break;
    }

    return (CLIR_OK);
}


/*-----------------------------------------------------------------------------
 * Name: cli_set_dbg_sel
 *
 * Description: This function is called by the CLI and handles the debug select
 *              set command.
 *
 * Inputs:  i4_argc   Contains the number of arguments.
 *          pps_argv  References a list of arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK       Successful
 *          CLIR_INV_ARG  One or more invalid arguments
 ----------------------------------------------------------------------------*/
static INT32 cli_set_dbg_sel (INT32         i4_argc,
                              const CHAR**  pps_argv)
{
    BOOL        b_dbg_sel_set;
    INT32       i4_return;
    const CHAR* ps_dbg_sel;


    i4_return = CLIR_INV_ARG;

    if ((i4_argc >= CLI_NUM_ARGS_DBG_SEL)                        &&
        (pps_argv != NULL)                                       &&
        ((ps_dbg_sel = pps_argv [CLI_ARG_IDX_DBG_SEL]) != NULL))
    {
        i4_return = CLIR_OK;

        b_dbg_sel_set = FALSE;

        if (x_strlen (ps_dbg_sel) == 1)
        {
            switch (*ps_dbg_sel)
            {
                case 'I':
                case 'i':
                    b_dbg_sel_set = TRUE;

                    e_dbg_sel = DBG_SEL_INDIVIDUAL;
                    break;

                case 'C':
                case 'c':
                    b_dbg_sel_set = TRUE;

                    e_dbg_sel = DBG_SEL_COMMON;
                    break;

                case 'B':
                case 'b':
                    b_dbg_sel_set = TRUE;

                    e_dbg_sel = DBG_SEL_INDIVIDUAL_COMMON;
                    break;

                default:
                    break;
            }
        }

        /* Output error message if not successful. */
        if (! (b_dbg_sel_set))
        {
            x_dbg_stmt ("Invalid debug selection. %s\n", CLI_SET_DBG_SEL_HELP);
        }
    }

    return (i4_return);
}
#endif


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

#ifdef DEBUG
    SIZE_T        z_len;
    CRIT_STATE_T  t_state;
    THREAD_REC_T* pt_thread_rec;


    i4_return = DBGR_NOT_INIT;

    if (b_trace_init)
    {
        i4_return = DBGR_NOT_ENOUGH_MEM;

        /* Size of structure plus the zero terminating character. */
        z_len = sizeof (THREAD_REC_T) + 1;

        if (ps_name != NULL)
        {
            z_len += x_strlen (ps_name);
        }

        /* Allocate space for the thread record. */
        if ((pt_thread_rec = (THREAD_REC_T*) x_mem_alloc (z_len)) != NULL)
        {
            i4_return = DBGR_OK;

            /* Set the thread record. */
            x_memset (((VOID*) pt_thread_rec), 0, z_len);

            if (x_thread_self (&pt_thread_rec->h_thread) != OSR_OK)
            {
                ABORT (DBG_CAT_THREAD, DBG_ABRT_CANNOT_GET_THREAD_HANDLE);
            }

            /* Copy thread name. */
            pt_thread_rec->ps_name = (CHAR*)((UINT8*) pt_thread_rec) + sizeof (THREAD_REC_T);
            (pt_thread_rec->ps_name) [0] = '\0';

            if (ps_name != NULL)
            {
                x_strcpy (&((pt_thread_rec->ps_name) [0]), ps_name);
            }

            /* get thread id and link record into list. */
            t_state = x_crit_start ();

            /* Don't forget to increment the thread id. */
            pt_thread_rec->ui2_id = ui2_thread_id++;

            pt_thread_rec->pt_next = pt_thread_head;
            pt_thread_head         = pt_thread_rec;

            x_crit_end (t_state);
        }
    }
#else
    i4_return = DBGR_NOT_ENABLED;
#endif

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

#ifdef DEBUG
    HANDLE_T            h_thread;
    CRIT_STATE_T        t_state;
    x_dbg_trace_rec_fct pf_trace_rec;


    i4_return = DBGR_NOT_INIT;

    if (b_trace_init)
    {
        i4_return = DBGR_NO_TRACE_BUFFER;

        if (pt_trace_head != NULL)
        {
            i4_return = DBGR_OK;

            /* Adjust record length */
            if (pv_data == NULL)
            {
                z_len = 0;
            }

            if (z_len > MAX_TRACE_REC_LEN)
            {
                z_len = MAX_TRACE_REC_LEN;
            }

            /* Grab the callback inside a critical */
            /* section because it may be updated.  */
            t_state = x_crit_start ();

            pf_trace_rec = pf_dbg_trace_rec;

            x_crit_end (t_state);

            /* Let the callback function decide if the trace */
            /* buffer shall be enabled.                      */
            if ((! (b_trace_ctrl))      &&
                (pf_trace_rec != NULL))
            {
                pf_trace_rec (FALSE, ui4_code, ((const VOID*) pv_data), z_len);
            }

            if (b_trace_ctrl)
            {
                h_thread = NULL_HANDLE;

                if (! (b_isr))
                {
                    x_thread_self (&h_thread);
                }

                add_trace_rec (h_thread, ui4_code, pv_data, z_len);
            }

            /* Let the callback function decide if the trace */
            /* buffer shall be disabled.                     */
            if ((b_trace_ctrl)          &&
                (pf_trace_rec != NULL))
            {
                pf_trace_rec (TRUE, ui4_code, ((const VOID*) pv_data), z_len);
            }
        }
    }
#else
    i4_return = DBGR_NOT_ENABLED;
#endif

    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name: dbg_output_init
 *
 * Description: This API initializes the output of the debug library which
 *              allows debug statements and the dumping of trace buffers. Note
 *              that trace buffers can also be dumped if the debug output has
 *              not been initialized.
 *
 * Inputs:  ps_output_name  Specifies the output device for debug statements.
 *          e_output_type   Indicates the output information referenced by
 *                          pv_setup_info.
 *          pv_output_info  References the output information.
 *
 * Outputs: -
 *
 * Returns: DBGR_OK                 Routine successful.
 *          DBGR_OPEN_FAIL          The open operation failed for some reason.
 *          DBGR_ALREADY_INIT       Debug library output already initialized.
 *          DBGR_INV_OUTPUT_DEVICE  Unknown output device.
 ----------------------------------------------------------------------------*/
INT32 dbg_output_init (const CHAR*        ps_output_name,
                       DBG_OUTPUT_TYPE_T  e_output_type,
                       const VOID*        pv_output_info)
{
    INT32 i4_return;


    i4_return = DBGR_ALREADY_INIT;

    if (! (b_output_init))
    {
        /* Create the required semaphores. */
        if (x_sema_create (&h_output_sema, X_SEMA_TYPE_MUTEX, X_SEMA_STATE_UNLOCK) != OSR_OK)
        {
            ABORT (DBG_CAT_INIT, DBG_ABRT_NO_STMT_SEMAPHORE);
        }

        /* Open the output device. */
#ifndef EXT_DBG_DEV
		i4_return = x_dbg_open_output (ps_output_name, e_output_type, pv_output_info);
#else
		i4_return = x_dbg_open_output ("", DBG_OUTPUT_TYPE_SERIAL_PORT, NULL);
#endif

		/* Initialize the callback functions. */
#ifdef DEBUG
		pf_dbg_data_inp = NULL;
        pf_dbg_output   = NULL;
#endif

#if defined (DEBUG) || defined (CLI_SUPPORT)
        /* Initialize the debug statement control flag. */
        b_stmt_ctrl = TRUE;
#endif

#if defined (DEBUG) || defined (CLI_SUPPORT)
        /* Initialize the CLI debug control variables. */
        ui2_common_dbg_level = DBG_INIT_LEVEL_MW_DBG;
        e_dbg_sel            = DBG_INIT_SELECT;
#endif

#ifdef CLI_SUPPORT
        /* And register the Debug library with the CLI module. */
        if (x_cli_attach_cmd_tbl (&(at_cli_dbg [0]), CLI_CAT_MW, CLI_GRP_NONE) != CLIR_OK)
        {
            ABORT (DBG_CAT_INIT, DBG_ABRT_CANNOT_REG_WITH_CLI);
        }
#endif

        /* And signal that the init function has been performed. */
        b_output_init = TRUE;
    }

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


#ifdef DEBUG
    i4_return = DBGR_ALREADY_INIT;

    if (! (b_trace_init))
    {
        /* Initialize variables. */
        pt_trace_head  = NULL;
        pt_thread_head = NULL;

        ui2_thread_id = 0;

        ui2_rd_idx   = 0;
        ui2_wr_idx   = 0;
        ui2_num_recs = ui2_num_trace_recs;

        pf_dbg_trace_rec = NULL;

        b_trace_ctrl = FALSE;
        b_trace_dump = FALSE;

        /* Only do this if the trace buffer shall be enabled. */
        if (ui2_num_trace_recs > 0)
        {
            i4_return = DBGR_NOT_ENOUGH_MEM;

            if ((pt_trace_head = (TRACE_REC_T*) x_mem_alloc (ui2_num_trace_recs * sizeof (TRACE_REC_T))) != NULL)
            {
                i4_return = DBGR_OK;

                x_memset (((VOID*) pt_trace_head), 0, (ui2_num_trace_recs * sizeof (TRACE_REC_T)));
            }
        }
        else
        {
            i4_return = DBGR_OK;
        }

        /* And signal that the init function has been performed. */
        b_trace_init = TRUE;
    }
#else
    i4_return = DBGR_OK;
#endif

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

#if defined (DEBUG) || defined (CLI_SUPPORT)
    CRIT_STATE_T t_state;


    i4_return = DBGR_NOT_INIT;

    if (b_output_init)
    {
        i4_return = DBGR_OK;

        t_state = x_crit_start ();

        b_stmt_ctrl = b_ctrl;

        x_crit_end (t_state);
    }
#else
    i4_return = DBGR_NOT_ENABLED;
#endif

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

#if defined (DEBUG) || defined (CLI_SUPPORT)
    CRIT_STATE_T t_state;


    b_return = FALSE;

    if (b_output_init)
    {
        t_state = x_crit_start ();

        b_return = b_stmt_ctrl;

        x_crit_end (t_state);
    }
#else
    b_return = FALSE;
#endif

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

#ifdef DEBUG
    CRIT_STATE_T t_state;


    i4_return = DBGR_NOT_INIT;

    if (b_output_init)
    {
        i4_return = DBGR_OK;

        t_state = x_crit_start ();

        pf_dbg_output = pf_output;

        x_crit_end (t_state);
    }
#else
    i4_return = DBGR_NOT_ENABLED;
#endif

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
#if defined (DEBUG) || defined (CLI_SUPPORT)
    if (b_output_init)
    {
        switch (e_dbg_sel)
        {
            case DBG_SEL_INDIVIDUAL:
                break;

            case DBG_SEL_COMMON:
                ui2_dbg_level = ui2_common_dbg_level;
                break;

            case DBG_SEL_INDIVIDUAL_COMMON:
                ui2_dbg_level |= ui2_common_dbg_level;
                break;

            default:
                ui2_dbg_level = DBG_LEVEL_NONE;
                break;
        }
    }
    else
    {
        ui2_dbg_level = DBG_LEVEL_NONE;
    }
#else
    ui2_dbg_level = DBG_LEVEL_NONE;
#endif

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

#ifdef DEBUG
    BOOL         b_curr_trace_ctrl;
    CRIT_STATE_T t_state;


    i4_return = DBGR_NOT_INIT;

    if (b_trace_init)
    {
        i4_return = DBGR_NO_TRACE_BUFFER;

        if (pt_trace_head != NULL)
        {
            i4_return = DBGR_NO_OUTPUT_DEVICE;

            if ((b_output_init)      ||
                (pf_output != NULL))
            {
                i4_return = DBGR_DUMP_IN_PROGRESS;

                t_state = x_crit_start ();

                if (! (b_trace_dump))
                {
                    b_trace_dump = TRUE;

                    x_crit_end (t_state);

                    i4_return = DBGR_OK;

                    /* Disable the trace buffer but remember the current control state. */
                    b_curr_trace_ctrl = b_trace_ctrl;
                    b_trace_ctrl = FALSE;

                    put_trace_stmt (pf_output, "\n\n");

                    put_trace_hdr (pf_output);
                    put_thread_info (pf_output);
                    put_trace_buffer (pf_output);

                    put_trace_stmt (pf_output, "\n\n");

                    /* And re-enable the trace buffer if required. */
                    b_trace_ctrl = b_curr_trace_ctrl;

                    b_trace_dump = FALSE;
                }
                else
                {
                    /* Don't forget to enable the interrupts. */
                    x_crit_end (t_state);
                }
            }
        }
    }
#else
    i4_return = DBGR_NOT_ENABLED;
#endif

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

#ifdef DEBUG
    CRIT_STATE_T t_state;


    i4_return = DBGR_NOT_INIT;

    if (b_trace_init)
    {
        i4_return = DBGR_NO_TRACE_BUFFER;

        if (pt_trace_head != NULL)
        {
            i4_return = DBGR_OK;

            t_state = x_crit_start ();

            ui2_rd_idx = 0;
            ui2_wr_idx = 0;

            x_crit_end (t_state);
        }
    }
#else
    i4_return = DBGR_NOT_ENABLED;
#endif

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
#ifdef DEBUG
    return (b_trace_ctrl);
#else
    return (FALSE);
#endif
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

#ifdef DEBUG
    CRIT_STATE_T t_state;


    i4_return = DBGR_NOT_INIT;

    if (b_trace_init)
    {
		i4_return = DBGR_REG_CB_ACTIVE;

        t_state = x_crit_start ();

		if ((pf_dbg_trace_rec == NULL)  ||
			(pf_trace_rec     == NULL))
		{
			i4_return = DBGR_OK;

			pf_dbg_trace_rec = pf_trace_rec;
		}

        x_crit_end (t_state);
    }
#else
    i4_return = DBGR_NOT_ENABLED;
#endif

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

#ifdef DEBUG
	CRIT_STATE_T t_state;


	i4_return = DBGR_NOT_INIT;

	if (b_output_init)
	{
		i4_return = DBGR_REG_CB_ACTIVE;

		t_state = x_crit_start ();

		/* Only allow the setting of a new data receive callback function if there */
		/* is no other one set or if argument "pf_data" contains a NULL value.     */
		if ((pf_dbg_data_inp == NULL)  ||
			(pf_data_inp     == NULL))
		{
			i4_return = DBGR_OK;

			pf_dbg_data_inp     = pf_data_inp;
			pv_dbg_data_inp_tag = pv_tag;

			if (pf_dbg_data_inp != NULL)
			{
				x_dbg_reg_data_rec (drv_data_rec);
			}
			else
			{
				x_dbg_reg_data_rec (NULL);
			}
		}

		x_crit_end (t_state);
	}
#else
    i4_return = DBGR_NOT_ENABLED;
#endif

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


#ifdef DEBUG
    i4_return = DBGR_NOT_INIT;

    if (b_trace_init)
    {
        i4_return = DBGR_NO_TRACE_BUFFER;

        if (pt_trace_head != NULL)
        {
            i4_return = DBGR_OK;

            b_trace_ctrl = b_trace_ena;
        }
    }
#else
    i4_return = DBGR_NOT_ENABLED;
#endif

    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name: x_dbg_stmt
 *
 * Description: This API sends a debug statement to the output device.
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
INT32 x_dbg_stmt (const CHAR*  ps_format, ...)
{
    INT32   i4_return;
    VA_LIST arg_list;


    i4_return = DBGR_NOT_INIT;

    if (b_output_init)
    {
        i4_return = DBGR_INV_ARG;

        if (ps_format != NULL)
        {
            i4_return = DBGR_OK;

            /* Bug-26  01/05/2005  ffr. Don't use the old API "x_va_start". Use the */
            /* new macro VA_START.                                                  */
            VA_START (arg_list, ps_format);

            /* Wait for other debug statement to be finished. */
            x_sema_lock (h_output_sema, X_SEMA_OPTION_WAIT);

            /* Create debug statement string and send it to output device. */
            x_vsnprintf (s_dbg_stmt, MAX_STMT_LEN, ps_format, arg_list);

            put_trace_stmt (NULL, ((const CHAR*) &(s_dbg_stmt [0])));

            x_sema_unlock (h_output_sema);

            VA_END (arg_list);
        }
    }

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

#if defined (DEBUG) || defined (CLI_SUPPORT)
    VA_LIST          arg_list;
    CRIT_STATE_T     t_state;
#if defined (DEBUG)
    x_dbg_output_fct pf_output;
#endif

    i4_return = DBGR_NOT_INIT;

    if (b_output_init)
    {
        i4_return = DBGR_INV_ARG;

        if (ps_format != NULL)
        {
            i4_return = DBGR_OK;

            /* Check the flag 'b_stmt-ctrl' inside a critical section and also load */
            /* the debug control output function within the critical section.       */
            t_state = x_crit_start ();

            if (b_stmt_ctrl)
            {
#if defined (DEBUG)
                pf_output = pf_dbg_output;
#endif
                x_crit_end (t_state);

                /* Bug-26  01/05/2005  ffr. Don't use the old API "x_va_start". Use the */
                /* new macro VA_START.                                                  */
                VA_START (arg_list, ps_format);

                /* Wait for other debug statement to be finished. */
                x_sema_lock (h_output_sema, X_SEMA_OPTION_WAIT);

                /* Create debug statement string and send it to output device. */
                x_vsnprintf (s_dbg_stmt, MAX_STMT_LEN, ps_format, arg_list);
#if defined (DEBUG)
                put_trace_stmt (pf_output, ((const CHAR*) &(s_dbg_stmt [0])));
#elif defined (CLI_SUPPORT)
				put_trace_stmt (NULL, ((const CHAR*) &(s_dbg_stmt [0])));
#endif
                x_sema_unlock (h_output_sema);

                VA_END (arg_list);
            }
            else
            {
                /* Don't forget to enable the critical section again. */
                x_crit_end (t_state);
            }
        }
    }
#else
    i4_return = DBGR_NOT_ENABLED;
#endif

    return (i4_return);
}

/*-----------------------------------------------------------------------------
 * Name: x_dbg_check_lvl
 *
 * Description: This API check the dbg level ,and output special msg.
 *
 * Inputs:  ui4_dbg_lvl  References now module levle
 *             ui4_stmt_lvl References to current dbg level
 *             ps_file        References to file name
 *             ps_func      References to function name
 *             ui2_line      References to line number
 *             ps_time      References to current time 
 *             ps_time      References to current data
 * Outputs: -
 *
 * Returns: TRUE           need to output dbg message
 *          FALSE      no need to output dbg message
 ----------------------------------------------------------------------------*/
BOOL x_dbg_check_lvl(UINT32 ui4_dbg_lvl, 
                        UINT32 ui4_stmt_lvl,
                        const CHAR    *ps_file,
                        const CHAR    *ps_func,
                        UINT16  ui2_line,
                        CHAR   *ps_time,
                        CHAR   *ps_date)
{
    if(ui4_dbg_lvl & ui4_stmt_lvl)                                   
    {                                                            
        if (ui4_dbg_lvl & (1<<20))                                
        {                                                        
            x_dbg_ctrl_stmt("[%s]", ps_file);                   
            x_dbg_ctrl_stmt("[%d]\n\r", ui2_line);               
        }                                                        
        else                                                     
        {                                                       
            if(ui4_dbg_lvl & (1<<16))                                  
            {                                                      
                x_dbg_ctrl_stmt("[%s %s] ",  ps_time, ps_date);  
            }                                                      
            if(ui4_dbg_lvl & (1<<17))                                
            {                                                    
                x_dbg_ctrl_stmt("[%s]", ps_file);                
            }                                                      
            if(ui4_dbg_lvl & (1<<18))                                  
            {                                                      
                x_dbg_ctrl_stmt("[%s]", ps_func);              
            }                                                      
            if(ui4_dbg_lvl & (1<<19))                                  
            {                                                      
                x_dbg_ctrl_stmt("[%d]", ui2_line);                 
            } 
            return TRUE;
        }                                                      
    }
    return FALSE;
}
/*-----------------------------------------------------------------------------
 * Name: c_dbg_stmt
 *
 * Description: This API sends a debug statement to the output device.
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
INT32 c_dbg_stmt (const CHAR*  ps_format, ...)
{
    INT32   i4_return;
    VA_LIST arg_list;


    i4_return = DBGR_NOT_INIT;

    if (b_output_init)
    {
        i4_return = DBGR_INV_ARG;

        if (ps_format != NULL)
        {
            i4_return = DBGR_OK;

            /* Bug-26  01/05/2005  ffr. Don't use the old API "x_va_start". Use the */
            /* new macro VA_START.                                                  */
            VA_START (arg_list, ps_format);

            /* Wait for other debug statement to be finished. */
            x_sema_lock (h_output_sema, X_SEMA_OPTION_WAIT);

            /* Create debug statement string and send it to output device. */
            x_vsnprintf (s_dbg_stmt, MAX_STMT_LEN, ps_format, arg_list);

            put_trace_stmt (NULL, ((const CHAR*) &(s_dbg_stmt [0])));

            x_sema_unlock (h_output_sema);

            VA_END (arg_list);
        }
    }

    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name: c_dbg_ctrl_stmt
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
INT32 c_dbg_ctrl_stmt (const CHAR*  ps_format, ...)
{
    INT32   i4_return;

#if defined (DEBUG) || defined (CLI_SUPPORT)
    VA_LIST          arg_list;
    CRIT_STATE_T     t_state;
#if defined (DEBUG)
    x_dbg_output_fct pf_output;
#endif

    i4_return = DBGR_NOT_INIT;

    if (b_output_init)
    {
        i4_return = DBGR_INV_ARG;

        if (ps_format != NULL)
        {
            i4_return = DBGR_OK;

            /* Check the flag 'b_stmt-ctrl' inside a critical section and also load */
            /* the debug control output function within the critical section.       */
            t_state = x_crit_start ();

            if (b_stmt_ctrl)
            {
#if defined (DEBUG)
                pf_output = pf_dbg_output;
#endif
                x_crit_end (t_state);

                /* Bug-26  01/05/2005  ffr. Don't use the old API "x_va_start". Use the */
                /* new macro VA_START.                                                  */
                VA_START (arg_list, ps_format);

                /* Wait for other debug statement to be finished. */
                x_sema_lock (h_output_sema, X_SEMA_OPTION_WAIT);

                /* Create debug statement string and send it to output device. */
                x_vsnprintf (s_dbg_stmt, MAX_STMT_LEN, ps_format, arg_list);
#if defined (DEBUG)
                put_trace_stmt (pf_output, ((const CHAR*) &(s_dbg_stmt [0])));
#elif defined (CLI_SUPPORT)
				put_trace_stmt (NULL, ((const CHAR*) &(s_dbg_stmt [0])));
#endif
                x_sema_unlock (h_output_sema);

                VA_END (arg_list);
            }
            else
            {
                /* Don't forget to enable the critical section again. */
                x_crit_end (t_state);
            }
        }
    }
#else
    i4_return = DBGR_NOT_ENABLED;
#endif

    return (i4_return);
}

/*-----------------------------------------------------------------------------
 * Name: c_dbg_check_lvl
 *
 * Description: This API check the dbg level ,and output special msg.
 *
 * Inputs:  ui4_dbg_lvl  References now module levle
 *             ui4_stmt_lvl References to current dbg level
 *             ps_file        References to file name
 *             ps_func      References to function name
 *             ui2_line      References to line number
 *             ps_time      References to current time 
 *             ps_time      References to current data
 * Outputs: -
 *
 * Returns: TRUE           need to output dbg message
 *          FALSE      no need to output dbg message
 ----------------------------------------------------------------------------*/
BOOL c_dbg_check_lvl(UINT32 ui4_dbg_lvl, 
                        UINT32 ui4_stmt_lvl,
                        const CHAR    *ps_file,
                        const CHAR    *ps_func,
                        UINT16  ui2_line,
                        CHAR   *ps_time,
                        CHAR   *ps_date)
{        
    return x_dbg_check_lvl(ui4_dbg_lvl, 
                        ui4_stmt_lvl,
                        ps_file,
                        ps_func,
                        ui2_line,
                        ps_time,
                        ps_date);
}


