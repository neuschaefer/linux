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
 * $RCSfile: cli_input.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/15 $
 * $SWAuthor: Clear Case Administrator $
 * $MD5HEX: 96b0a89975193784c9eddfef11dec3ef $
 *
 * Description:
 *         This program will handle input from UART to CLI console.
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "os/inc/x_os.h"
#include "inc/common.h"
#include "inc/x_common.h"
#include "dbg/dbg.h"
#include "dbg/x_dbg.h"
#include "dbg/x_dbg_drv.h"
#include "dt/x_dt.h"
#include "time_msrt/x_time_msrt.h"
#include "cli/x_cli.h"
#include "cli/cli.h"
#include "cli/_cli.h"
#include "res_mngr/drv/u_irrc_btn_def.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define MAX_STMT_ROW                            ((UINT32) 256)
#define MAX_STMT_LEN                            ((UINT32) 512)
#define MAX_CLI_ENABLE_PASSWD_TRY               ((UINT32)   3)
#define SET_UART_TO_NORMAL_MODE_CMD_1           "basic.stop"
#define SET_UART_TO_NORMAL_MODE_ABBR_CMD_1      "b.stop"
#define SET_UART_TO_NORMAL_MODE_CMD_2           "0.st"

/* CLI debug output control */
typedef enum
{
    CLI_WITH_DBG_STMT = 0,
    CLI_DISCARD_DBG_STMT,
    CLI_BUFFER_DBG_STMT
}   CLI_DBG_CTRL;

/* CLI operation mode */
typedef enum
{
    CLI_ENABLED = 0,
    CLI_ENABLING,
    CLI_DISABLED
}   CLI_OP_MODE;

typedef enum
{
    CLI_STATE_SUPERVISOR = 0,
    CLI_STATE_TRANSITION,
    CLI_STATE_GUEST
}   CLI_STATE;

/* UI Hot Keys registration structure */
typedef struct _CLI_UI_HOTKEY_T
{
    struct _CLI_UI_HOTKEY_T*    pt_next;
    UINT16                      ui2_hotkeys;
    x_cli_ui_hotkey_fct         pf_hotkey_cb;
    VOID*                       pv_tag;
}   CLI_UI_HOTKEY_T;

#ifdef LINUX_TURNKEY_SOLUTION
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
static UINT32 _generate_random(UINT32 x, UINT32 y)
{    
    time_t t;
    srand((UINT32) time(&t));
    return (rand()%(y - x + 1) + x);
}
#endif

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
#if defined(CLI_SUPPORT) || defined(CLI_STRESS_MODE)
static HANDLE_T         h_cli_thread;
static HANDLE_T         h_cli_msgq;
static HANDLE_T         h_cli_ctrl_b_msgq;
static BOOL             b_cli_init = FALSE;
static UINT32           ui4_cli_cmd_buf_row_idx;
static UINT32           ui4_cli_cmd_buf_ref_row_idx;
static UINT32           ui4_cli_cmd_buf_idx;
static UINT32           ui4_ctrl_stmt_buf_idx;
static CHAR             s_cli_prompt_str[CLI_CMD_BUF_SIZE];
static CHAR             s_cli_cmd_buf[CLI_CMD_BUF_ROW_NUM][CLI_CMD_BUF_SIZE];
static CHAR             s_ctrl_stmt_buf[MAX_STMT_ROW][MAX_STMT_LEN];
static CLI_DBG_CTRL     e_cli_dbg_ctrl;
static CLI_OP_MODE      e_cli_enabled;

static CLI_STATE        e_cli_state = CLI_STATE_GUEST;


static CLI_LOG_STRUCTURE_T      at_log_list[MAX_LOG_LIST_NUMBER];
static UINT32					ui4_log_num_cnt;
static char*			ps_log_err_prompt[] = 
						{"t,","d,","f,","l,","c)"};
#endif

static CLI_UI_HOTKEY_T* pt_hotkey_list_head = NULL;

/*-----------------------------------------------------------------------------
                    function declarations
 ----------------------------------------------------------------------------*/
#if defined(CLI_SUPPORT) || defined(CLI_STRESS_MODE)
static VOID _dump_ctrl_stmt_buf(VOID);

static VOID _char_rec_nfy_fct(UINT8 ui1_data);

static VOID _ctrl_stmt_output(const CHAR* ps_stmt);

static VOID _cli_input_thread(VOID* pv_arg);

static BOOL _cli_parse_dbg_level(const CHAR* ps_dbg_level,
                            UINT16*      pui2_dbg_level);


static BOOL  _cli_get_bool_level(UINT8        ui1_argc,
                            	UINT32      ui4_log_level);


/*-----------------------------------------------------------------------------
 * Name: _show_ctrl_stmt_buf
 *
 * Description: This API dumps the content of control statement buffer.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _dump_ctrl_stmt_buf(VOID)
{
    UINT32  ui4_idx;

    x_dbg_stmt("\n\r");

    for (ui4_idx = 0; ui4_idx < ui4_ctrl_stmt_buf_idx; ui4_idx++)
    {
        x_dbg_stmt("%s", &s_ctrl_stmt_buf[ui4_idx]);
    }

    ui4_ctrl_stmt_buf_idx = 0;
    x_dbg_stmt("%s", s_cli_prompt_str);
}


/*-----------------------------------------------------------------------------
 * Name: _char_rec_nfy_fct
 *
 * Description: This notify function is called by dbg library for any character
 *              reception from UART.
 *
 * Inputs:  ui1_data        The received character.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _char_rec_nfy_fct(UINT8 ui1_data)
{
    /* Enable or disable CLI */
    if (e_cli_state == CLI_STATE_GUEST)
    {
        if (ui1_data == ASCII_KEY_CTRL_C || ui1_data == ASCII_KEY_CTRL_X || ui1_data == (UINT8)'!')
        {
            e_cli_state = CLI_STATE_TRANSITION;
        }
    }
    else if (e_cli_state == CLI_STATE_SUPERVISOR)
    {
        if (ui1_data == ASCII_KEY_CTRL_C || ui1_data == ASCII_KEY_CTRL_X || ui1_data == (UINT8)'!')
        {
            e_cli_state = CLI_STATE_GUEST;
			cli_set_access_right(CLI_MODE_GUEST);
            x_dbg_stmt("\n\r===========\n\r");
            x_dbg_stmt("CLI is OFF\n\r");
            x_dbg_stmt("===========\n\r");
            return;
        }
    }

    switch (ui1_data)
    {
        case ASCII_KEY_CTRL_B: /* Break "do" command or a loop operation */
            x_msg_q_send(h_cli_ctrl_b_msgq,
                         &ui1_data,
                         sizeof(UINT8),
                         CLI_MSGQ_DEFAULT_PRIORITY);
                         
            x_msg_q_send(h_cli_msgq,
                         &ui1_data,
                         sizeof(UINT8),
                         CLI_MSGQ_DEFAULT_PRIORITY);                         
            break;

        case ASCII_KEY_CTRL_D: /* Switch among different debug modes */
            e_cli_dbg_ctrl = (CLI_DBG_CTRL)((e_cli_dbg_ctrl + 1) % (CLI_BUFFER_DBG_STMT + 1));

            x_dbg_stmt("\n\r===================================\n\r");
            switch (e_cli_dbg_ctrl)
            {
                case CLI_WITH_DBG_STMT:
                    x_dbg_stmt("Output: CLI + DBG stmt\n\r");
                    break;

                case CLI_DISCARD_DBG_STMT:
                    x_dbg_stmt("Output: CLI only\n\r");
                    break;

                case CLI_BUFFER_DBG_STMT:
                    x_dbg_stmt("Output: CLI only (buffer DBG stmt)\n\r");
                    break;

                default:
                    break;
            }
            x_dbg_stmt("===================================\n\r\n\r");
            x_dbg_stmt("%s", s_cli_prompt_str);

            if ((ui4_ctrl_stmt_buf_idx > 0) &&
                (e_cli_dbg_ctrl != CLI_BUFFER_DBG_STMT))
            {
                _dump_ctrl_stmt_buf();
            }
            break;

        case ASCII_KEY_CTRL_L: /* Dump content of control statement buffer */
            _dump_ctrl_stmt_buf();
            break;

        case ASCII_KEY_CTRL_W: /* Print mandatory table commands */
            cli_parser_list_mandatory_tbl_cmd();
            x_dbg_stmt("\n\r%s", s_cli_prompt_str);
            break;
            
        default:
            x_msg_q_send(h_cli_msgq,
                         &ui1_data,
                         sizeof(UINT8),
                         CLI_MSGQ_DEFAULT_PRIORITY);
            break;
    }
}


/*-----------------------------------------------------------------------------
 * Name: _ctrl_stmt_output
 *
 * Description: This notify function is called by dbg library when API
 *              "[xc]_dbg_ctrl_stmt" are executed.
 *
 * Inputs:  ps_stmt        The control statement output.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _ctrl_stmt_output(const CHAR* ps_stmt)
{
    switch (e_cli_dbg_ctrl)
    {
        case CLI_WITH_DBG_STMT:
            x_dbg_stmt("%s", ps_stmt);
            break;

        case CLI_BUFFER_DBG_STMT:
            x_strcpy((CHAR*)(&s_ctrl_stmt_buf[ui4_ctrl_stmt_buf_idx++]), ps_stmt);

            if (ui4_ctrl_stmt_buf_idx == MAX_STMT_ROW)
            {
                _dump_ctrl_stmt_buf();
            }
            else
            {
                ui4_ctrl_stmt_buf_idx %= MAX_STMT_ROW;
            }
            break;

        case CLI_DISCARD_DBG_STMT:
        default:
            break;
    }
}


/*-----------------------------------------------------------------------------
 * Name: _cli_input_thread
 *
 * Description: This is CLI input thread for handling input from UART.
 *
 * Inputs:  pv_arg   Input argument
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _cli_input_thread(VOID* pv_arg)
{
    INT32       i4_return;
    CHAR        c_char;
    UINT32      ui4_idx;
    BOOL        b_recv_ascii_key_cr = FALSE;

    /* Check arguments */
    if (pv_arg != NULL)
    {
        CLI_ABORT(1);
    }

    /* Initialize input buffers */
    for (ui4_idx = 0; ui4_idx < CLI_CMD_BUF_ROW_NUM; ui4_idx++)
    {
        s_cli_cmd_buf[ui4_idx][0] = ASCII_NULL;
    }

    /* Display prompt string */
    x_dbg_stmt("%s", s_cli_prompt_str);

    while (1)
    {
        if (e_cli_state == CLI_STATE_TRANSITION)
        {
            for (ui4_idx = 0; ui4_idx < MAX_CLI_ENABLE_PASSWD_TRY; ui4_idx++)
            {
                if (x_cli_passwd(CLI_PASSWD_EVEN) == CLIR_OK)
                {
                    e_cli_state = CLI_STATE_SUPERVISOR;
					cli_set_access_right(CLI_MODE_SUPERVISOR);
                    x_dbg_stmt("\n\r===========\n\r");
                    x_dbg_stmt("CLI is ON\n\r");
                    x_dbg_stmt("===========\n\r");
                    x_dbg_stmt("%s", s_cli_prompt_str);
                    break;
                }
                x_dbg_stmt("\n\r");

                /* Reset command buffer indexes */
                ui4_cli_cmd_buf_row_idx = 0;
                ui4_cli_cmd_buf_idx = 0;
            }

            if (ui4_idx == MAX_CLI_ENABLE_PASSWD_TRY)
            {
                x_dbg_stmt("Oops! you are having a trouble, try again...\n\r\n\r");
                e_cli_state = CLI_STATE_GUEST;
			    cli_set_access_right(CLI_MODE_GUEST);
            }
        }

        c_char = cli_get_char();
        
        if ((UINT8)c_char == ASCII_KEY_CR)
        {
            b_recv_ascii_key_cr = TRUE;
        }
        else if ((UINT8)c_char == ASCII_KEY_LF)                                    
        {            
            if (b_recv_ascii_key_cr)
            {
                /* Handle CR-LF terminating characters */ 
                b_recv_ascii_key_cr = FALSE;
                continue;
            }
            else
            {                                        
                b_recv_ascii_key_cr = FALSE;
            }
        }            
                            
        switch (c_char)
        {
            case ASCII_KEY_CR:
            case ASCII_KEY_LF:
                s_cli_cmd_buf[ui4_cli_cmd_buf_row_idx][ui4_cli_cmd_buf_idx] = ASCII_NULL;
                x_dbg_stmt("\n\r");

                /* Parse input CLI command */
                i4_return = cli_parser(s_cli_cmd_buf[ui4_cli_cmd_buf_row_idx]);

                if (i4_return == CLIR_CMD_NOT_FOUND)
                {
                    x_dbg_stmt("Invalid command\n\r");
                }
                else if (i4_return == CLIR_DIR_NOT_FOUND)
                {
                    x_dbg_stmt("Invalid directory\n\r");
                }
                else if (i4_return < CLIR_OK)
                {
                    x_dbg_stmt("Command execution error [%d]\n\r", i4_return);
                }

                /* Scroll to next row of CLI commad buffer */
                if (ui4_cli_cmd_buf_idx > 0)
                {
                    ui4_cli_cmd_buf_row_idx = (ui4_cli_cmd_buf_row_idx + 1) & (CLI_CMD_BUF_ROW_NUM - 1);
                    s_cli_cmd_buf[ui4_cli_cmd_buf_row_idx][0] = ASCII_NULL;
                }

                ui4_cli_cmd_buf_idx = 0;
                ui4_cli_cmd_buf_ref_row_idx = ui4_cli_cmd_buf_row_idx;
                
                if (e_cli_enabled == CLI_ENABLED)
                {                    
                    x_dbg_stmt("\n\r%s", s_cli_prompt_str);
                }                    
                break;

            case ASCII_KEY_BS:
                b_recv_ascii_key_cr = FALSE;
                if (ui4_cli_cmd_buf_idx > 0)
                {
                    x_dbg_stmt("%c%c%c", ASCII_KEY_BS, ASCII_KEY_SPACE, ASCII_KEY_BS);
                    ui4_cli_cmd_buf_idx--;
                }
                break;

            case ASCII_KEY_ESC:
                b_recv_ascii_key_cr = FALSE;
                c_char = cli_get_char();
                if ((UINT8)c_char == ASCII_KEY_ARROW)
                {
                    c_char = cli_get_char();
                    switch (c_char)
                    {
                        case ASCII_KEY_UP:
                            /* Clear terminal display */
                            while (ui4_cli_cmd_buf_idx > 0)
                            {
                                x_dbg_stmt("%c%c%c", ASCII_KEY_BS, ASCII_KEY_SPACE, ASCII_KEY_BS);
                                ui4_cli_cmd_buf_idx--;
                            }
                            s_cli_cmd_buf[ui4_cli_cmd_buf_row_idx][0] = ASCII_NULL;

                            /* Load previous reference row */
                            do
                            {
                                ui4_cli_cmd_buf_ref_row_idx = (ui4_cli_cmd_buf_ref_row_idx - 1) & (CLI_CMD_BUF_ROW_NUM - 1);
                                if (ui4_cli_cmd_buf_ref_row_idx == ui4_cli_cmd_buf_row_idx)
                                {
                                    break;
                                }
                            } while (s_cli_cmd_buf[ui4_cli_cmd_buf_ref_row_idx][0] == ASCII_NULL);

                            x_memcpy(&s_cli_cmd_buf[ui4_cli_cmd_buf_row_idx][0],
                                     &s_cli_cmd_buf[ui4_cli_cmd_buf_ref_row_idx][0],
                                     CLI_CMD_BUF_SIZE);

                            /* Show the row content */
                            ui4_cli_cmd_buf_idx = 0;
                            while (s_cli_cmd_buf[ui4_cli_cmd_buf_row_idx][ui4_cli_cmd_buf_idx] != ASCII_NULL)
                            {
                                x_dbg_stmt("%c", s_cli_cmd_buf[ui4_cli_cmd_buf_row_idx][ui4_cli_cmd_buf_idx]);
                                ui4_cli_cmd_buf_idx++;
                            }
                            break;

                        case ASCII_KEY_DOWN:
                            /* Clear terminal display */
                            while (ui4_cli_cmd_buf_idx > 0)
                            {
                                x_dbg_stmt("%c%c%c", ASCII_KEY_BS, ASCII_KEY_SPACE, ASCII_KEY_BS);
                                ui4_cli_cmd_buf_idx--;
                            }
                            s_cli_cmd_buf[ui4_cli_cmd_buf_row_idx][0] = ASCII_NULL;

                            /* Load next reference row */
                            do
                            {
                                ui4_cli_cmd_buf_ref_row_idx = (ui4_cli_cmd_buf_ref_row_idx + 1) & (CLI_CMD_BUF_ROW_NUM - 1);
                                if (ui4_cli_cmd_buf_ref_row_idx == ui4_cli_cmd_buf_row_idx)
                                {
                                    break;
                                }
                            } while (s_cli_cmd_buf[ui4_cli_cmd_buf_ref_row_idx][0] == ASCII_NULL);

                            x_memcpy(&s_cli_cmd_buf[ui4_cli_cmd_buf_row_idx][0],
                                     &s_cli_cmd_buf[ui4_cli_cmd_buf_ref_row_idx][0],
                                     CLI_CMD_BUF_SIZE);

                            /* Show the row content */
                            ui4_cli_cmd_buf_idx = 0;
                            while (s_cli_cmd_buf[ui4_cli_cmd_buf_row_idx][ui4_cli_cmd_buf_idx] != ASCII_NULL)
                            {
                                x_dbg_stmt("%c", s_cli_cmd_buf[ui4_cli_cmd_buf_row_idx][ui4_cli_cmd_buf_idx]);
                                ui4_cli_cmd_buf_idx++;
                            }
                            break;

                        case ASCII_KEY_LEFT:
                            if (ui4_cli_cmd_buf_idx > 0)
                            {
                                x_dbg_stmt("%c%c%c", ASCII_KEY_BS, ASCII_KEY_SPACE, ASCII_KEY_BS);
                                ui4_cli_cmd_buf_idx--;
                            }
                            break;

                        case ASCII_KEY_RIGHT:
                        default:
                            break;
                    }
                }
                break;

            default:
                b_recv_ascii_key_cr = FALSE;
                if (((UINT8)c_char >= ASCII_KEY_PRINTABLE_MIN) &&
                    ((UINT8)c_char <= ASCII_KEY_PRINTABLE_MAX))
                {
                    if (ui4_cli_cmd_buf_idx == CLI_CMD_BUF_SIZE)
                    {
                        /* Command buffer overflow */
                        continue;
                    }
                    else
                    {
                        /* Store input character into command buffer */
                        s_cli_cmd_buf[ui4_cli_cmd_buf_row_idx][ui4_cli_cmd_buf_idx++] = c_char;
#ifdef LINUX_TURNKEY_SOLUTION
                        /* Linux do not need to log the input char, the "tty_man" will do it. */
#else
                        x_dbg_stmt("%c", c_char);
#endif
                    }
                }
                break;
        }
    }
}

/*-----------------------------------------------------------------------------
 * Name: _cli_parse_dbg_level
 *
 * Description: This API parses a CLI command for debug level.
 *
 * Inputs:  ps_dbg_level        Points to the dbg level string.
 *          
 * Outputs: pui2_dbg_lvl    The value of debug level after parsing.
 *
 * Returns: TRUE                 Routine successful.
 *          FALSE            One or more invalid arguments.
 ----------------------------------------------------------------------------*/


static BOOL _cli_parse_dbg_level(const CHAR* ps_dbg_level,
                            UINT16*      pui2_dbg_level)
{
	BOOL			b_invalid_arg = FALSE;
	UINT8			ui1_get_num = 0;
	UINT8			ui1_str_lth	= 0;
	BOOL			b_show_log = TRUE;
	UINT8			ui1_idx_num =0;
	
	/* Check arguments */
	if ((ps_dbg_level == NULL) ||
		(pui2_dbg_level == NULL))
	{
		b_invalid_arg = TRUE;
        return b_invalid_arg;
	}

	*pui2_dbg_level = 0;

    if (((x_strchr(ps_dbg_level, 'n') != NULL) ||
         (x_strchr(ps_dbg_level, 'N') != NULL)) &&
        (x_strlen(ps_dbg_level) > 1))
    {
    	b_invalid_arg = TRUE;
        return b_invalid_arg;
    }

	ui1_str_lth	= x_strlen(ps_dbg_level);
	if(ui1_str_lth == 1)
	{
        switch (*ps_dbg_level)
        {
            case 'e':
            case 'E':
				ui1_get_num	= 0;
                break;

            case 'a':
            case 'A':
				ui1_get_num	= 6;
                break;

            case 'i':
            case 'I':
                ui1_get_num	= 3;
                break;
            case 'n':
            case 'N':  
            case '0':
				b_show_log	= FALSE;
				break;
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
				ui1_get_num	= *ps_dbg_level-'0' ;
				ui1_get_num--;
                break;
            default:
                b_invalid_arg = TRUE;
                break;
        }
	}
	else if (ui1_str_lth == 2)
	{
		if((x_strcmp(ps_dbg_level,"ei") == 0) ||
			(x_strcmp(ps_dbg_level,"ie") == 0))
		{
			ui1_get_num	= 3;
		}
		else
		{
			b_invalid_arg = TRUE;
		}
	}
	else if (ui1_str_lth == 3)
	{
		if (x_strcmp(ps_dbg_level, CLI_ALL_STR) == 0)
	    {
	        ui1_get_num	= 6;
	    }
		else
		{
			b_invalid_arg = TRUE;
		}
	}
	else
	{
		b_invalid_arg = TRUE;
	}

	if(b_invalid_arg)
	{
	}
	else
	{
		if(b_show_log)
		{
			for(ui1_idx_num = 0;ui1_idx_num <= ui1_get_num; ui1_idx_num++)
			{
				*pui2_dbg_level	|= (1<<ui1_idx_num );
			}
		}
		else
		{
			*pui2_dbg_level = 0;
		}
	} 
	return b_invalid_arg;
}

/*-----------------------------------------------------------------------------
 * Name: _cli_get_bool_level
 *
 * Description: This API parses  a UINT32 number's specify bit
 * Inputs:  ui1_argc        which bit to parse
 *          ui4_log_level        the UINT32 number
 *		
 *          
 * Outputs: 
 *
 * Returns: TRUE                 this bit is 1
 *          FALSE           this bit is 0.
 ----------------------------------------------------------------------------*/
static BOOL  _cli_get_bool_level(UINT8        ui1_argc,
                            	UINT32      ui4_log_level)
{
	if(ui4_log_level & (1<<ui1_argc))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

#endif


/*-----------------------------------------------------------------------------
 * Name: cli_get_char
 *
 * Description: This API waits for the first available character.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: ASCII_NULL      The UART queue is empty.
 *          Others          The first available character from UART queue.
 ----------------------------------------------------------------------------*/
CHAR cli_get_char(VOID)
{
#if defined(CLI_SUPPORT) || defined(CLI_STRESS_MODE)
    INT32       i4_return;
    UINT16      ui2_idx;
    CHAR        c_char;
    SIZE_T      z_size = sizeof(CHAR);

    i4_return = x_msg_q_receive(&ui2_idx,
                                &c_char,
                                &z_size,
                                &h_cli_msgq,
                                1,
                                X_MSGQ_OPTION_WAIT);

    if (i4_return != OSR_OK)
    {
        c_char = ASCII_NULL;
    }

    return c_char;
#else
    return ASCII_NULL;
#endif
}


/*-----------------------------------------------------------------------------
 * Name: cli_get_char_timeout
 *
 * Description: This API waits for the first available character within
 *              specific time period.
 *
 * Inputs:  ui4_time    The waiting time period in ms.
 *
 * Outputs: -
 *
 * Returns: ASCII_NULL      The UART queue is empty.
 *          Others          The first available character from UART queue.
 ----------------------------------------------------------------------------*/
CHAR cli_get_char_timeout(UINT32 ui4_time)
{
#if defined(CLI_SUPPORT) || defined(CLI_STRESS_MODE)
    INT32       i4_return;
    UINT16      ui2_idx;
    CHAR        c_char;
    SIZE_T      z_size = sizeof(CHAR);

    i4_return = x_msg_q_receive_timeout(&ui2_idx,
                                        &c_char,
                                        &z_size,
                                        &h_cli_msgq,
                                        1,
                                        ui4_time);

    if (i4_return != OSR_OK)
    {
        c_char = ASCII_NULL;
    }

    return c_char;
#else
    return ASCII_NULL;
#endif
}


/*-----------------------------------------------------------------------------
 * Name: cli_get_string
 *
 * Description: This API waits for the first available string.
 *
 * Inputs:  b_show_srt      Indicate if star signs are shown when character
 *                          input.
 *
 * Outputs: ps_str_buf      The first available string from UART queue.
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID cli_get_string(CHAR* ps_str_buf,
                    BOOL  b_show_star)
{
#if defined(CLI_SUPPORT) || defined(CLI_STRESS_MODE)
    CHAR        c_char;
    UINT32      ui4_idx = 0;

    do
    {
        c_char = cli_get_char();
        if (((UINT8)c_char != ASCII_KEY_CR) &&
            ((UINT8)c_char != ASCII_KEY_LF) &&
            ((UINT8)c_char != ASCII_NULL))
        {
            if ((UINT8)c_char == ASCII_KEY_ESC)
            {
                c_char = cli_get_char();
                if ((UINT8)c_char == ASCII_KEY_LEFT)
                {
                    if (ui4_idx > 0)
                    {
                        x_dbg_stmt("%c%c%c", ASCII_KEY_BS, ASCII_KEY_SPACE, ASCII_KEY_BS);
                        ui4_idx--;
                    }
                }
            }
            else if ((UINT8)c_char == ASCII_KEY_BS)
            {
                if (ui4_idx > 0)
                {
                    x_dbg_stmt("%c%c%c", ASCII_KEY_BS, ASCII_KEY_SPACE, ASCII_KEY_BS);
                    ui4_idx--;
                }
            }
            else
            {
                if (((UINT8)c_char >= ASCII_KEY_PRINTABLE_MIN) &&
                    ((UINT8)c_char <= ASCII_KEY_PRINTABLE_MAX))
                {
                    if (ui4_idx == CLI_CMD_BUF_SIZE)
                    {
                        continue;
                    }

                    ps_str_buf[ui4_idx++] = c_char;
                    x_dbg_stmt("*");
                }
            }
        }
    } while (((UINT8)c_char != ASCII_KEY_CR) && ((UINT8)c_char != ASCII_KEY_LF));

    ps_str_buf[ui4_idx] = ASCII_NULL;
#else
    ps_str_buf[0] = ASCII_NULL;
#endif
}


/*-----------------------------------------------------------------------------
 * Name: cli_get_prompt_str_buf
 *
 * Description: This API returns the address of CLI prompt string buffer.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: The address of CLI prompt string buffer.
 ----------------------------------------------------------------------------*/
CHAR* cli_get_prompt_str_buf(VOID)
{
#if defined(CLI_SUPPORT) || defined(CLI_STRESS_MODE)
    return s_cli_prompt_str;
#else
    return NULL;
#endif
}


/*-----------------------------------------------------------------------------
 * Name: cli_get_ctrl_b_msgq_hdl
 *
 * Description: This API gets CLI message queue handle.
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: CLI message queue handle.
 ----------------------------------------------------------------------------*/
HANDLE_T* cli_get_ctrl_b_msgq_hdl(VOID)
{
#if defined(CLI_SUPPORT) || defined(CLI_STRESS_MODE)
    return &h_cli_ctrl_b_msgq;
#else
    return NULL;
#endif
}


/*-----------------------------------------------------------------------------
 * Name: cli_is_inited
 *
 * Description: This API returns the initialization status of CLI component.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: TRUE        The CLI has already been initialized.
 *          FALSE       The CLI has not been initialized.
 ----------------------------------------------------------------------------*/
BOOL cli_is_inited(VOID)
{
#if defined(CLI_SUPPORT) || defined(CLI_STRESS_MODE)
    return b_cli_init;
#else
    return FALSE;
#endif
}


/*-----------------------------------------------------------------------------
 * Name: cli_init
 *
 * Description: CLI initialization function.
 *
 * Inputs:  pt_thread_descr      References a thread descriptor structure.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                     Routine successful.
 *          CLIR_ALREADY_INIT           The CLI has already been initialized.
 ----------------------------------------------------------------------------*/
INT32 cli_init(THREAD_DESCR_T* pt_thread_descr)
{
#if defined(CLI_SUPPORT) || defined(CLI_STRESS_MODE)
    THREAD_DESCR_T      t_thread_descr;

    if (b_cli_init)
    {
        return CLIR_ALREADY_INIT;
    }

    /* Initialize internal parameters */
    ui4_cli_cmd_buf_row_idx = 0;
    ui4_cli_cmd_buf_ref_row_idx = 0;
    ui4_cli_cmd_buf_idx = 0;
    ui4_ctrl_stmt_buf_idx = 0;
    e_cli_dbg_ctrl = CLI_WITH_DBG_STMT;
    e_cli_enabled = CLI_ENABLED; 

    /* Initialize default command tables and set up prompt string */
    if (cli_parser_clear_cmd_tbl() != CLIR_OK) /* Mandatory table will be loaded here */
    {
        CLI_ABORT(1);
    }

    /* Initialize alias table */
    if (cli_alias_init() != CLIR_OK)
    {
        CLI_ABORT(1);
    }

    /* Register character reception callback and control statement output callback to DBG library */
    x_dbg_reg_data_rec(_char_rec_nfy_fct);
    dbg_reg_ctrl_stmt_cb(_ctrl_stmt_output);

    /* Initialize thread descriptor structure */
    if (pt_thread_descr == NULL)
    {
        t_thread_descr.ui1_priority = CLI_THREAD_DEFAULT_PRIORITY;
        t_thread_descr.ui2_num_msgs = CLI_NUM_OF_MSGS;
        t_thread_descr.z_stack_size = CLI_THREAD_DEFAULT_STACK_SIZE;
    }
    else
    {
        t_thread_descr = (*pt_thread_descr);

        /* Convert default thread values to actual ones if needed */
        if (t_thread_descr.ui1_priority == DEFAULT_PRIORITY)
        {
            t_thread_descr.ui1_priority = CLI_THREAD_DEFAULT_PRIORITY;
        }

        if (t_thread_descr.ui2_num_msgs == DEFAULT_NUM_MSGS)
        {
            t_thread_descr.ui2_num_msgs = CLI_NUM_OF_MSGS;
        }

        if (t_thread_descr.z_stack_size == DEFAULT_STACK_SIZE)
        {
            t_thread_descr.z_stack_size = CLI_THREAD_DEFAULT_STACK_SIZE;
        }
    }

    /* Create the required message queues */
    if (x_msg_q_create(&h_cli_ctrl_b_msgq,
                       "cli_ctrl_b_msgq",
                       sizeof(UINT8),
                       32) != OSR_OK)
    {
        CLI_ABORT(1);
    }

    if (x_msg_q_create(&h_cli_msgq,
                       CLI_MSGQ_NAME,
                       sizeof(UINT8),
                       t_thread_descr.ui2_num_msgs) != OSR_OK)
    {
        CLI_ABORT(1);
    }

    /* Create CLI thread */
    if (x_thread_create(&h_cli_thread,
                        CLI_THREAD_NAME,
                        t_thread_descr.z_stack_size,
                        t_thread_descr.ui1_priority,
                        _cli_input_thread,
                        0,
                        NULL) != OSR_OK)
    {
        CLI_ABORT(1);
    }

    b_cli_init = TRUE;

    return CLIR_OK;
#else
    return CLIR_OK;
#endif
}

/*-----------------------------------------------------------------------------
 * Name: cli_get_module_fuc
 *
 * Description: get module index from log table,and then executive the regist function
 *
 * Inputs:  i4_argc     Number of arguments.
 *          pps_argv    Points to the argument array.    
 *
 * Outputs:-
 *
 * Returns: CLIR_OK                 		Routine successful.
 *          CLIR_INV_CMD_USAGE      	Invalid command usage.
 *		CLIR_MODULE_UNREGIST      Unregist module
 *		CLIR_NOT_ENABLED     		CLI is not enabled.
 ----------------------------------------------------------------------------*/

INT32 cli_get_module_fuc(INT32 i4_argc, const CHAR** pps_argv)
{
#if defined(CLI_SUPPORT) || defined(CLI_STRESS_MODE)

	UINT32	ui4_idx;
	//UINT32	ui4_idx_prnf;
	UINT32	ui4_idx_cn;
	UINT32  ui4_name_len;
	UINT32  ui4_cur_name_size;
	UINT32	ui4_idx_name;
	UINT32	ui4_lenth;
	const CHAR* ps_arg	= NULL;
	const CHAR*	ps_diy_arg = NULL;
	
	CHAR*	ps_argv[CLI_MAX_ARG_NUM];
	CHAR     s_argv[CLI_MAX_ARG_NUM][CLI_MAX_ARG_LEN];

	 /* Init argument buffer */
    for (ui4_idx = 0; ui4_idx < CLI_MAX_ARG_NUM; ui4_idx++)
    {
        ps_argv[ui4_idx] 	= s_argv[ui4_idx];
        s_argv[ui4_idx][0] 	= 0;
    }
	
	/* Check arguments */
	if(pps_argv[1] == NULL )
	{
		return CLIR_INV_ARG;
	}

	ui4_name_len	= x_strlen(pps_argv[1]);
	
	for(ui4_idx = 0; ui4_idx < ui4_log_num_cnt; ui4_idx++)
	{
		/* do the regist function */
		ui4_cur_name_size	= x_strlen(at_log_list[ui4_idx].s_module_name);
		if((ui4_cur_name_size == ui4_name_len)
			&&(x_strncmp(at_log_list[ui4_idx].s_module_name, pps_argv[1], ui4_name_len) == 0))
		{
			at_log_list[ui4_idx].pfExecFun(i4_argc, pps_argv);

			if(i4_argc >= 3)
			{
				for (ui4_idx_name = 0; (ui4_idx_name + 2) < i4_argc; ui4_idx_name++)
				{
					/* parse argc*/
					ps_arg		= pps_argv[ui4_idx_name + 2];
					ui4_lenth		= x_strlen(ps_arg);
		
					if(ui4_lenth >= 3 && (x_strncmp(ps_arg, "y=", 2)== 0))
					{
						ps_diy_arg = ps_arg + 2;
						break;
					} 
				}
			}

			ui4_idx_cn = cli_parser_cmd(ps_diy_arg, ps_argv);
						
			/* do the callback function */
			if(at_log_list[ui4_idx].pfCallbackFun != NULL)
			{
				at_log_list[ui4_idx].pfCallbackFun(ui4_idx_cn,(const char**)ps_argv);
			}
			
			return CLIR_OK;
		}
	}

	return CLIR_MODULE_UNREGIST;
#else
    return CLIR_NOT_ENABLED;
#endif
}


/*-----------------------------------------------------------------------------
 * Name: x_cli_attach_cmd_tbl
 *
 * Description: This API attaches a command table to CLI.
 *
 * Inputs:  pt_tbl          The Command table to be attached.
 *          e_category      The category that the command table belongs to.
 *          ui8_group_mask  The group(s) that the command table belongs to.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_NOT_INIT           The CLI has not been initialized.
 *          CLIR_INV_ARG            One or more invalid arguments.
 *          CLIR_CMD_TBL_FULL       Command table is full.
 *          CLIR_GROUP_TBL_FULL     Module-to-group table is full.
 *          CLIR_NOT_ENABLED        CLI is not enabled.
 ----------------------------------------------------------------------------*/
INT32 x_cli_attach_cmd_tbl(CLI_EXEC_T* pt_tbl,
                           CLI_CAT_T   e_category,
                           UINT64      ui8_group_mask)
{
#if defined(CLI_SUPPORT) || defined(CLI_STRESS_MODE)
    if (!b_cli_init)
    {
        return CLIR_NOT_INIT;
    }

    /* Check arguments */
    if (pt_tbl == NULL)
    {
        return CLIR_INV_ARG;
    }

    return (cli_parser_attach_cmd_tbl(pt_tbl, e_category, ui8_group_mask));
#else
    return CLIR_NOT_ENABLED;
#endif
}


/*-----------------------------------------------------------------------------
 * Name: x_cli_attach_alias
 *
 * Description: This API adds, deletes, or raplaces an alias element in alias
 *              table.
 *
 * Inputs:  ps_alias    Contains the alias to attach.
 *          ps_cmd      Contains the command string corresponding to the alias.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_NOT_INIT           The CLI has not been initialized.
 *          CLIR_INV_ARG            One or more invalid arguments.
 *          CLIR_ALIAS_TOO_LONG     Alias is too long.
 *          CLIR_CMD_TOO_LONG       CLI command is too long.
 *          CLIR_ALIAS_TBL_FULL     CLI alias table is full.
 *          CLIR_CMD_EXEC_ERROR     CLI command execution failed.
 *          CLIR_NOT_ENABLED        CLI is not enabled.
 ----------------------------------------------------------------------------*/
INT32 x_cli_attach_alias(const CHAR* ps_alias,
                         const CHAR* ps_cmd)
{
#if defined(CLI_SUPPORT) || defined(CLI_STRESS_MODE)
    if (!b_cli_init)
    {
        return CLIR_NOT_INIT;
    }

    /* Check arguments */
    if ((ps_alias == NULL) ||
        (ps_cmd == NULL))
    {
        return CLIR_INV_ARG;
    }

    return (cli_alias_attach(ps_alias, ps_cmd));
#else
    return CLIR_NOT_ENABLED;
#endif
}


/*-----------------------------------------------------------------------------
 * Name: x_cli_register_log_command
 *
 * Description: This API adds, or raplaces an log element in log
 *              table.
 *
 * Inputs:  ps_module_name    Contains the module  name to register.
 *             pv_tag      Contains the custom tag.
 *             pf_exec_func   	point to the executive function
 *		   pf_callback_func point to the callback function
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_NOT_INIT           The CLI has not been initialized.
 *          CLIR_INV_ARG            One or more invalid arguments.
 *          CLIR_MODULE_NAME_TOO_LONG     module name is too long.
 *          CLIR_LOG_TBL_FULL     CLI log table is full.
 *          CLIR_NOT_ENABLED        CLI is not enabled.
 ----------------------------------------------------------------------------*/
INT32 x_cli_register_log_command(const CHAR* ps_module_name, 
                                VOID* pv_tag,
                                x_cli_exec_fct pf_exec_func,
                                x_cli_exec_fct pf_callback_func)
{
#if defined(CLI_SUPPORT) || defined(CLI_STRESS_MODE)
	UINT32 ui4_module_name_len;
	UINT32 ui4_cur_name_len;
	UINT32 ui4_idx;

	//CLI_LOG_STRUCTURE_T*  ps_log_tbl = NULL;
	
	if (!b_cli_init)
    {
        return CLIR_NOT_INIT;
    }

    /* Check arguments */
    if ((ps_module_name == NULL) ||
        (pf_exec_func == NULL) )
    {
        return CLIR_INV_ARG;
    }

	/* Check module name length */
    ui4_module_name_len	= x_strlen(ps_module_name);
	if(ui4_module_name_len > MODULE_NAME_MAX_LENTH)
	{
		return CLIR_MODULE_NAME_TOO_LONG;
	}
		
	/*if exited, Replace existed log */
	for(ui4_idx = 0; ui4_idx < ui4_log_num_cnt; ui4_idx++)
	{
		ui4_cur_name_len	= x_strlen(at_log_list[ui4_idx].s_module_name);
		if((ui4_cur_name_len == ui4_module_name_len)
			&& (x_strncmp(at_log_list[ui4_idx].s_module_name, ps_module_name, ui4_module_name_len) == 0))
		{
				at_log_list[ui4_idx].pv_tag			= pv_tag;
				at_log_list[ui4_idx].pfExecFun		= pf_exec_func;
				at_log_list[ui4_idx].pfCallbackFun	= pf_callback_func;
				return CLIR_OK;
		}
	}

	/* Check log table's capacity */
	if(ui4_log_num_cnt >= MAX_LOG_LIST_NUMBER)
	{
		return CLIR_LOG_TBL_FULL;
	}
	
	ui4_cur_name_len	= x_strlen(at_log_list[ui4_log_num_cnt].s_module_name);
	
	x_strncpy(at_log_list[ui4_log_num_cnt].s_module_name, ps_module_name, ui4_module_name_len);

	ui4_cur_name_len	= x_strlen(at_log_list[ui4_log_num_cnt].s_module_name);
	
	at_log_list[ui4_log_num_cnt].pv_tag			= pv_tag;
	at_log_list[ui4_log_num_cnt].pfExecFun		= pf_exec_func;
	at_log_list[ui4_log_num_cnt].pfCallbackFun	= pf_callback_func;
	ui4_log_num_cnt++;
	
	return CLIR_OK;
#else
    return CLIR_NOT_ENABLED;
#endif
}

/*-----------------------------------------------------------------------------
 * Name: x_cli_parser
 *
 * Description: This API parses CLI command and performs corresponding
 *              operation.
 *
 *
 * Inputs:  ps_cmd      Contain the command to parse.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_NOT_INIT           The CLI has not been initialized.
 *          CLIR_INV_ARG            One or more invalid arguments.
 *          CLIR_UNKNOWN_CMD        Unknown CLI command.
 *          CLIR_CMD_NOT_FOUND      CLI command not found.
 *          CLIR_DIR_NOT_FOUND      CLI directory not found.
 *          CLIR_NOT_ENABLED        CLI is not enabled.
 ----------------------------------------------------------------------------*/
INT32 x_cli_parser(const CHAR* ps_cmd)
{
#if defined(CLI_SUPPORT) || defined(CLI_STRESS_MODE)
    if (!b_cli_init)
    {
        return CLIR_NOT_INIT;
    }

    return (cli_parser(ps_cmd));
#else
    return CLIR_NOT_ENABLED;
#endif
}


/*-----------------------------------------------------------------------------
 * Name: x_cli_parser_arg
 *
 * Description: This API parses CLI command and performs corresponding
 *              operation. The variable-length argument lists are supported.
 *
 * Inputs:  ps_cmd   Contain the format.
 *          ...      Variable argument list.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_NOT_INIT           The CLI has not been initialized.
 *          CLIR_INV_ARG            One or more invalid arguments.
 *          CLIR_UNKNOWN_CMD        Unknown CLI command.
 *          CLIR_CMD_NOT_FOUND      CLI command not found.
 *          CLIR_DIR_NOT_FOUND      CLI directory not found.
 *          CLIR_NOT_ENABLED        CLI is not enabled.
 ----------------------------------------------------------------------------*/
INT32 x_cli_parser_arg(const CHAR* ps_cmd, ...)
{
#if defined(CLI_SUPPORT) || defined(CLI_STRESS_MODE)
    CHAR        s_buf[CLI_CMD_BUF_SIZE];
    VA_LIST     t_ap;

    if (!b_cli_init)
    {
        return CLIR_NOT_INIT;
    }

    VA_START(t_ap, ps_cmd);
    x_vsprintf(s_buf, ps_cmd, t_ap);
    VA_END(t_ap);

    return (cli_parser(s_buf));
#else
    return CLIR_NOT_ENABLED;
#endif
}


/*-----------------------------------------------------------------------------
 * Name: x_cli_passwd
 *
 * Description: This API generates a password according to current system
 *              date and time, prompt for user input, then verify its
 *              correctness.
 *
 * Inputs:  t_password          The password calculation method.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_NOT_INIT           The CLI has not been initialized.
 *          CLIR_CMD_EXEC_ERROR     Input password is incorrect.
 *          CLIR_NOT_ENABLED        CLI is not enabled.
 ----------------------------------------------------------------------------*/
INT32 x_cli_passwd(CLI_PASSWD_T t_password)
{
#if defined(CLI_SUPPORT) || defined(CLI_STRESS_MODE)
    DTG_T       t_dtg;
    CHAR        s_inp_passwd[CLI_CMD_BUF_SIZE];
    UINT32      ui4_passwd = 0;

    if (!b_cli_init)
    {
        return CLIR_NOT_INIT;
    }

    /* Calculate password */
#ifdef LINUX_TURNKEY_SOLUTION
    t_dtg.ui2_yr  = (UINT16)_generate_random(2003, 9000); 
    t_dtg.ui1_mo  = (UINT8)_generate_random(1, 12); /* 1~12 */    
    t_dtg.ui1_day = (UINT8)_generate_random(1,31); /* 1~31*/    
    t_dtg.ui1_dow = (UINT8)_generate_random(0,6); /* 0~6 */    
    t_dtg.ui1_hr  = (UINT8)_generate_random(0,23); /* 0~23 */    
    t_dtg.ui1_min = (UINT8)_generate_random(0,59); /* 0 ~ 59 */    
    t_dtg.ui1_sec = (UINT8)_generate_random(0,61); /* 0~61 */
#else    
    if (x_dt_utc_sec_to_dtg(x_dt_get_utc(NULL, NULL), &t_dtg) == DTR_OK)
#endif        
    {
        x_dbg_stmt("%02d/%02d/%04d %02d:%02d:%02d-\n\r",
                   t_dtg.ui1_mo, t_dtg.ui1_day, t_dtg.ui2_yr,
                   t_dtg.ui1_hr, t_dtg.ui1_min, t_dtg.ui1_sec);
                           
        if (t_dtg.ui2_yr > 9999)
        {
            t_dtg.ui2_yr = (UINT16)(t_dtg.ui2_yr/10);
        }   
                
        switch (t_password)
        {
            case CLI_PASSWD_ODD:
                ui4_passwd |= (t_dtg.ui1_mo/10) << 24;
                ui4_passwd |= (t_dtg.ui1_day/10) << 20;
                ui4_passwd |= ((UINT8)(t_dtg.ui2_yr/100%100/10)) << 16;
                ui4_passwd |= ((UINT8)(t_dtg.ui2_yr%100/10)) << 12;                
                ui4_passwd |= (t_dtg.ui1_hr/10) << 8;
                ui4_passwd |= (t_dtg.ui1_min/10) << 4;
                ui4_passwd |= t_dtg.ui1_sec/10;                          
                break;
                
            case CLI_PASSWD_EVEN:
                ui4_passwd |= (t_dtg.ui1_mo%10) << 28; /* Repeat the first digit in purpose */
                ui4_passwd |= (t_dtg.ui1_mo%10) << 24;
                ui4_passwd |= (t_dtg.ui1_day%10) << 20;
                ui4_passwd |= ((UINT8)(t_dtg.ui2_yr/100%10)) << 16;
                ui4_passwd |= ((UINT8)(t_dtg.ui2_yr%10)) << 12;                
                ui4_passwd |= (t_dtg.ui1_hr%10) << 8;
                ui4_passwd |= (t_dtg.ui1_min%10) << 4;
                ui4_passwd |= t_dtg.ui1_sec%10;                       
                break;
                
            case CLI_PASSWD_TWO_DIGITS_SUM_DEC:
                ui4_passwd |= ((t_dtg.ui1_mo/10 + t_dtg.ui1_mo%10)%10) << 24;
                ui4_passwd |= ((t_dtg.ui1_day/10 + t_dtg.ui1_day%10)%10) << 20;    
                ui4_passwd |= ((UINT8)((t_dtg.ui2_yr/100%100/10 + t_dtg.ui2_yr/100%10)%10)) << 16;
                ui4_passwd |= ((UINT8)((t_dtg.ui2_yr%100/10 + t_dtg.ui2_yr%10)%10)) << 12;
                ui4_passwd |= ((t_dtg.ui1_hr/10 + t_dtg.ui1_hr%10)%10) << 8;
                ui4_passwd |= ((t_dtg.ui1_min/10 + t_dtg.ui1_min%10)%10) << 4;
                ui4_passwd |= (t_dtg.ui1_sec/10 + t_dtg.ui1_sec%10)%10;
                break;
                
            case CLI_PASSWD_TWO_DIGITS_SUM_HEX:
                ui4_passwd |= (t_dtg.ui1_mo/10 + t_dtg.ui1_mo%16) << 24;
                ui4_passwd |= (t_dtg.ui1_day/10 + t_dtg.ui1_day%16) << 20;    
                ui4_passwd |= ((UINT8)((t_dtg.ui2_yr/100%100/10 + t_dtg.ui2_yr/100%10)%16)) << 16;
                ui4_passwd |= ((UINT8)((t_dtg.ui2_yr%100/10 + t_dtg.ui2_yr%10)%16)) << 12;
                ui4_passwd |= (t_dtg.ui1_hr/10 + t_dtg.ui1_hr%16) << 8;
                ui4_passwd |= (t_dtg.ui1_min/10 + t_dtg.ui1_min%16) << 4;
                ui4_passwd |= t_dtg.ui1_sec/10 + t_dtg.ui1_sec%16;
                break;                
        }

        x_dbg_stmt("Password: ");
        cli_get_string(s_inp_passwd, TRUE);
    }

    /* Check user's input password */
    if (((UINT32)(x_strtoull(s_inp_passwd, NULL, 16))) != ui4_passwd)
    {
        x_dbg_stmt("\n\rAccess denied!\n\r");
        return CLIR_CMD_EXEC_ERROR;
    }

    return CLIR_OK;
#else
    return CLIR_NOT_ENABLED;
#endif
}


/*-----------------------------------------------------------------------------
 * Name: x_cli_get_char_timeout
 *
 * Description: This API waits for the first available character within
 *              specific time period.
 *
 * Inputs:  ui4_time    The waiting time period in ms.
 *
 * Outputs: -
 *
 * Returns: ASCII_NULL      The UART queue is empty.
 *          Others          The first available character from UART queue.
 ----------------------------------------------------------------------------*/
CHAR x_cli_get_char_timeout(UINT32 ui4_time)
{
#if defined(CLI_SUPPORT) || defined(CLI_STRESS_MODE)
    return (cli_get_char_timeout(ui4_time));
#else
    return ((CHAR)ASCII_NULL);
#endif    
}


/*-----------------------------------------------------------------------------
 * Name: x_cli_parse_dbg_level
 *
 * Description: This API parses a CLI command for debug level.
 *
 * Inputs:  i4_argc         Number of arguments.
 *          pps_argv        Points to the argument array.
 *
 * Outputs: pui2_dbg_lvl    The value of debug level after parsing.
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_NOT_INIT           The CLI has not been initialized.
 *          CLIR_INV_ARG            One or more invalid arguments.
 *          CLIR_NOT_ENABLED        CLI is not enabled.
 ----------------------------------------------------------------------------*/
INT32 x_cli_parse_dbg_level(INT32        i4_argc,
                            const CHAR** pps_argv,
                            UINT16*      pui2_dbg_level)
{
#if defined(CLI_SUPPORT) || defined(CLI_STRESS_MODE)
    //const CHAR*     ps_dbg_level;
    BOOL            b_invalid_arg = FALSE;

    /* xxx.xxx [level] */

    if (!b_cli_init)
    {
        return CLIR_NOT_INIT;
    }

    /* Check arguments */
    if ((i4_argc < 2) ||
        (pps_argv == NULL) ||
        (pui2_dbg_level == NULL))
    {
	    x_dbg_stmt("sdl [level]\n\r");
	    x_dbg_stmt("[level]:0~7, n=0,e=1, a=7, i=4, all =7\n\r");
        return CLIR_INV_ARG;
    }

	b_invalid_arg	= _cli_parse_dbg_level(pps_argv[1], pui2_dbg_level);

    if (b_invalid_arg)
    {
    	*pui2_dbg_level	= 0;
        x_dbg_stmt("sdl [level]\n\r");
	    x_dbg_stmt("[level]:0~7, n=0,e=1, a=7, i=4, all =7\n\r");
		x_dbg_stmt("sorry, set dbg level failed \r\n");
		return CLIR_INV_ARG;
    }
    else
    {
    	x_dbg_stmt("set dbg level successful \r\n");
    }
	x_cli_show_dbg_level(*pui2_dbg_level);
	return CLIR_OK;
#else
    return CLIR_NOT_ENABLED;
#endif
}


/*-----------------------------------------------------------------------------
 * Name: x_cli_show_dbg_level
 *
 * Description: This API display debug level in text format.
 *
 * Inputs:  ui2_dbg_lvl          The value of debug level to display.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_NOT_INIT           The CLI has not been initialized.
 *          CLIR_NOT_ENABLED        CLI is not enabled.
 ----------------------------------------------------------------------------*/
INT32 x_cli_show_dbg_level(UINT16 ui2_dbg_level)
{
#if defined(CLI_SUPPORT) || defined(CLI_STRESS_MODE)

    UINT8   ui1_i 	= 0;
    UINT8   ui1_dbg_lvl = 0;
    if (!b_cli_init)
    {
        return CLIR_NOT_INIT;
    }

    x_dbg_stmt("Debug level = ");

    if (ui2_dbg_level & DBG_LEVEL_ERROR)
    {
        x_dbg_stmt("e");
    }

    if (ui2_dbg_level & DBG_LEVEL_API)
    {
        x_dbg_stmt("a");
    }

    if (ui2_dbg_level & DBG_LEVEL_INFO)
    {
        x_dbg_stmt("i");
    }

	if (ui2_dbg_level == 0)
	{
		x_dbg_stmt("%s", CLI_NONE_STR);
	}

	for (ui1_i = 0; ui1_i < 16; ui1_i++)
	{
		if(!_cli_get_bool_level( ui1_i , ui2_dbg_level))
		{
			ui1_dbg_lvl	= ui1_i ;
			x_dbg_stmt("(%d)", ui1_dbg_lvl);
			break;
		}
	}
    x_dbg_stmt("\n\r");

    return CLIR_OK;
#else
    return CLIR_NOT_ENABLED;
#endif
}


/*-----------------------------------------------------------------------------
 * Name: x_cli_parse_log_level
 *
 * Description: This API parses a log command with dbg level part and extral level part.
 *
 * Inputs:  i4_argc         Number of arguments.
 *          pps_argv        Points to the argument array.
 *		
 *          
 * Outputs: pui4_log_level    The value of log level after parsing.
 *	
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_NOT_INIT           The CLI has not been initialized.
 *          CLIR_NOT_ENABLED     CLI is not enabled.
 * 		CLIR_INV_ARG		      Invalid arguments	
 ----------------------------------------------------------------------------*/

INT32 x_cli_parse_log_level(INT32        i4_argc, 
                         		const CHAR** pps_argv,
                            	UINT32*      pui4_log_level)
{
#if defined(CLI_SUPPORT) || defined(CLI_STRESS_MODE)

	UINT32			ui4_lenth;
	UINT32			ui4_idx;
	UINT32			ui4_idx_num;
	const CHAR* 	ps_charargc;
	UINT32			ui4_set_num	= 1;
	UINT8			ui1_get_num = 0;
	
	if (!b_cli_init)
    {
        return CLIR_NOT_INIT;
    }

    /* Check arguments */
    if ((pps_argv == NULL) ||
        (pui4_log_level == NULL))
    {
        return CLIR_INV_ARG;
    }

	/* Check if need parse the argc*/
	if (i4_argc > 2)
	{
		for (ui4_idx = 0; (ui4_idx + 2) < i4_argc; ui4_idx++)
		{
			/* parse argc*/
			ps_charargc		= pps_argv[ui4_idx + 2];
			ui4_lenth		= x_strlen(ps_charargc);

			if ((*ps_charargc == '-')&&(ui4_lenth == 2))
			{
				ps_charargc++;
				switch(*ps_charargc)
				{
					/*time*/
					case 't':
					case 'T':
						ui4_set_num	= (UINT32)(1<< 16);
						break;
					/*file*/
					case 'd':
					case 'D':
						ui4_set_num	= (UINT32)(1<< 17);
						break;
					/*func*/
					case 'f':
					case 'F':
						ui4_set_num	= (UINT32)(1<< 18);
						break;
					/*line*/
					case 'l':
					case 'L':
						ui4_set_num	= (UINT32)(1<< 19);
						break;
					/*custom*/
					case 'c':
					case 'C':
						ui4_set_num	= (UINT32)(1<< 20);
						break;
					default:
						ui4_set_num	= 0;
						break;		
				}

				ui4_set_num	= ~ui4_set_num;
				*pui4_log_level	&= ui4_set_num;
			}
			else if ((*ps_charargc == '+')&&(ui4_lenth == 2))
			{
				ps_charargc++;
				switch(*ps_charargc)
				{
					/*time*/
					case 't':
					case 'T':
						ui4_set_num	= (UINT32)(1<< 16);
						break;
					/*file*/
					case 'd':
					case 'D':
						ui4_set_num	= (UINT32)(1<< 17);
						break;
					/*func*/
					case 'f':
					case 'F':
						ui4_set_num	= (UINT32)(1<< 18);
						break;
					/*line*/
					case 'l':
					case 'L':
						ui4_set_num	= (UINT32)(1<< 19);
						break;
					/*custom*/
					case 'c':
					case 'C':
						ui4_set_num	= (UINT32)(1<< 20);
						break;
					default:
						ui4_set_num	= 0;
						break;		
				}
				*pui4_log_level	|= ui4_set_num;
			}
			else if (ui4_lenth == 1)
			{			
				switch (*ps_charargc )
				{
					case '0':
						*pui4_log_level	= (*pui4_log_level>>16)<<16;
						break;
		            case '1':
		            case '2':
		            case '3':
		            case '4':
		            case '5':
		            case '6':
		            case '7':
						*pui4_log_level	= (*pui4_log_level>>16)<<16;
						ui1_get_num		= *ps_charargc-'0';
						for(ui4_idx_num = 1;ui4_idx_num <= ui1_get_num; ui4_idx_num++)
						{
							*pui4_log_level	|= (1<<(ui4_idx_num - 1));
						}
						break;
					default:
						break;
				}
			}
			else
			{
				/*now do nothing*/
			}

		}
	}
	//x_dbg_stmt("module debug level is %d ",*pui4_log_level);
	/* show now log level*/
	x_cli_show_log_level(pps_argv[1],*pui4_log_level);
	return CLIR_OK;
#else
    return CLIR_NOT_ENABLED;
#endif

}


INT32 x_cli_show_log_level(const CHAR* ps_module_name, 
								UINT32 ui4_log_level)
{
#if defined(CLI_SUPPORT) || defined(CLI_STRESS_MODE)
	UINT8			ui1_idx;
	UINT8			ui1_dbg_lvl = 0;

	
	for (ui1_idx = 0; ui1_idx < 16; ui1_idx++)
	{
		if(!_cli_get_bool_level( ui1_idx , ui4_log_level))
		{
			ui1_dbg_lvl	= ui1_idx ;
			break;
		}
	}
	
	x_dbg_stmt("%s module debug level is %u (", ps_module_name, ui1_dbg_lvl);

	for (ui1_idx = 0; ui1_idx < 5; ui1_idx++)
	{
		if(_cli_get_bool_level((ui1_idx + 16), ui4_log_level))
		{
			x_dbg_stmt("+%s", ps_log_err_prompt[ui1_idx]);
		}
		else
		{
			x_dbg_stmt("-%s", ps_log_err_prompt[ui1_idx]);
		}
	}

	x_dbg_stmt("\r\n");
	
	return CLIR_OK;
#else
    return CLIR_NOT_ENABLED;
#endif

}

/*-----------------------------------------------------------------------------
 * Name: x_cli_parse_tms_level
 *
 * Description: This API parses a CLI command for time measurement level.
 *
 * Inputs:  i4_argc         Number of arguments.
 *          pps_argv        Points to the argument array.
 *
 * Outputs: pui2_tms_lvl    The value of time measurement level after parsing.
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_NOT_INIT           The CLI has not been initialized.
 *          CLIR_CMD_EXEC_ERROR     Input password is incorrect.
 *          CLIR_NOT_ENABLED        CLI is not enabled.
 ----------------------------------------------------------------------------*/
INT32 x_cli_parse_tms_level(INT32        i4_argc,
                            const CHAR** pps_argv,
                            UINT16*      pui2_tms_level)
{
#if defined(CLI_SUPPORT) || defined(CLI_STRESS_MODE)
    const CHAR*   ps_tms_level;

    /* xxx.xxx [level] */

    if (!b_cli_init)
    {
        return CLIR_NOT_INIT;
    }

    /* Check arguments */
    if ((i4_argc < 2) ||
        (pps_argv == NULL) ||
        (pui2_tms_level == NULL))
    {
        return CLIR_INV_ARG;
    }

    *pui2_tms_level = 0;
    ps_tms_level = pps_argv[1];

    while (*ps_tms_level != 0)
    {
        switch (*ps_tms_level)
        {
            case 'r':
            case 'R':
                *pui2_tms_level |= TMS_LEVEL_REAL_TIME;
                break;

            case 'o':
            case 'O':
                *pui2_tms_level |= TMS_LEVEL_OFF_LINE;
                break;

            default:
                break;
        }

        ps_tms_level++;
    }

    return CLIR_OK;
#else
    return CLIR_NOT_ENABLED;
#endif
}


/*-----------------------------------------------------------------------------
 * Name: x_cli_show_tms_level
 *
 * Description: This API display time measurement level in text format.
 *
 * Inputs:  ui2_tms_lvl          The value of time measurement level to
 *                               display.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_NOT_INIT           The CLI has not been initialized.
 *          CLIR_NOT_ENABLED        CLI is not enabled.
 ----------------------------------------------------------------------------*/
INT32 x_cli_show_tms_level(UINT16 ui2_tms_level)
{
#if defined(CLI_SUPPORT) || defined(CLI_STRESS_MODE)
    BOOL    b_valid_tms_level = FALSE;

    if (!b_cli_init)
    {
        return CLIR_NOT_INIT;
    }

    x_dbg_stmt("TMS level = ");

    if (ui2_tms_level & TMS_LEVEL_REAL_TIME)
    {
        x_dbg_stmt("r");
        b_valid_tms_level = TRUE;
    }

    if (ui2_tms_level & TMS_LEVEL_OFF_LINE)
    {
        x_dbg_stmt("o");
        b_valid_tms_level = TRUE;
    }

    if (b_valid_tms_level == FALSE)
    {
        x_dbg_stmt("%s", CLI_NONE_STR);
    }

    x_dbg_stmt("\n\r");

    return CLIR_OK;
#else
    return CLIR_NOT_ENABLED;
#endif
}


/*-----------------------------------------------------------------------------
 * Name: x_cli_ui_hotkey_register
 *
 * Description: This API is used to register callback functions for modules 
 *              which have some features needed to be toggled from UI.
 *
 * Inputs:  ui2_hotkeys         The value of hotkeys to identify callbacks.
 *
 *          pf_hotkey_cb        (*x_cli_ui_hotkey_fct) type callback.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_ARG            Invalid arguments.
 *          CLIR_OUT_OF_MEM         Out of memory.
 *          CLIR_DUP_KEY            The input hotkeys is duplicated.
 ----------------------------------------------------------------------------*/
INT32 x_cli_ui_hotkey_register (UINT16              ui2_hotkeys,
                                x_cli_ui_hotkey_fct pf_hotkey_cb,
                                VOID*               pv_tag)
{
    CLI_UI_HOTKEY_T*    pt_hotkey = pt_hotkey_list_head;

    if (pf_hotkey_cb == NULL)
    {
        return CLIR_INV_ARG;
    }

    while (pt_hotkey)
    {
        if (ui2_hotkeys == pt_hotkey->ui2_hotkeys)
        {
            return CLIR_DUP_KEY;
        }

        pt_hotkey = pt_hotkey->pt_next;
    }

    pt_hotkey = (CLI_UI_HOTKEY_T*) x_mem_alloc(sizeof(CLI_UI_HOTKEY_T));
    if (pt_hotkey == NULL)
    {
        return CLIR_OUT_OF_MEM;
    }

    pt_hotkey->ui2_hotkeys  = ui2_hotkeys;
    pt_hotkey->pf_hotkey_cb = pf_hotkey_cb;
    pt_hotkey->pv_tag       = pv_tag;

    pt_hotkey->pt_next = pt_hotkey_list_head;
    pt_hotkey_list_head = pt_hotkey;

    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: x_cli_ui_send_hotkeys
 *
 * Description: This API is used to trigger the hotkey callback functions.
 *
 * Inputs:  pui4_keys           Contains a hotkey array.
 *          ui2_key_len         The length of the hotkey array.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_NOT_INIT           The CLI sub-module has not been initialized.
 *          CLIR_INV_ARG            Invalid arguments.
 *          CLIR_NOT_ENABLED        CLI sub-module is not enabled.
 ----------------------------------------------------------------------------*/
INT32 x_cli_ui_send_hotkeys (UINT32*    pui4_keys,
                             UINT16     ui2_key_len)
{

#define MAX_HOTKEY_LEN  (4)

    CLI_UI_HOTKEY_T*    pt_hotkey;
    UINT16              ui2_keys = 0;
    UINT16              ui2_idx;

    if (pt_hotkey_list_head == NULL)
    {
        return CLIR_NOT_INIT;
    }

    if (pui4_keys == NULL || ui2_key_len != MAX_HOTKEY_LEN)
    {
        return CLIR_INV_ARG;
    }

    for (ui2_idx = 0; ui2_idx < MAX_HOTKEY_LEN; ui2_idx++)
    {
        /* map 4 digits to an UINT16 (16 bits) variable */
        if (pui4_keys[ui2_idx] >= BTN_DIGIT_0 &&
            pui4_keys[ui2_idx] <= BTN_DIGIT_9)
        {
            ui2_keys |= ((pui4_keys[ui2_idx] - BTN_DIGIT_0) & 0x0f) << (4*(3-ui2_idx));
        }
        else
        {
            return CLIR_INV_ARG;
        }
    }

    pt_hotkey = pt_hotkey_list_head;
    while (pt_hotkey)
    {
        if (ui2_keys == pt_hotkey->ui2_hotkeys)
        {
            if (pt_hotkey->pf_hotkey_cb)
            {
                pt_hotkey->pf_hotkey_cb(ui2_keys, pt_hotkey->pv_tag);
            }
            return CLIR_OK;
        }

        pt_hotkey = pt_hotkey->pt_next;
    }

    return CLIR_NOT_ENABLED;
}


