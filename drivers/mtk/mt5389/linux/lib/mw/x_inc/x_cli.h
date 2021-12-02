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
 * $RCSfile: x_cli.h,v $ x_cli_h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/4 $
 * $SWAuthor: Alec Lu $
 * $MD5HEX: f561d156232286bf00fd87f76b0f5314 $
 *
 * Description:
 *         This header file contains CLI related definitions, which are
 *         exported.
 *---------------------------------------------------------------------------*/
 
 
/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_CLI
  *  @{  
  */
/*----------------------------------------------------------------------------*/

#ifndef _X_CLI_H_
#define _X_CLI_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "u_cli.h"

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
/*------------------------------------------------------------------*/
/*! @brief  This API attaches a command table to CLI root.
  *  @param [in]   pt_tbl                    References to the command table to be attached.
  *  @param [in]   e_category            References to the category that the command table belongs to.
  *  @param [in]   ui8_group_mask    References to the group(s) that the command table belongs to.
  *  @retval      -CLIR_OK                      Routine successful.
  *  @retval      -CLIR_NOT_INIT            The CLI has not been initialized.
  *  @retval      -CLIR_INV_ARG             One or more invalid arguments.
  *  @retval      -CLIR_CMD_TBL_FULL    Command table is full.
  */
/*------------------------------------------------------------------*/
extern INT32 x_cli_attach_cmd_tbl(CLI_EXEC_T* pt_tbl, 
                                  CLI_CAT_T   e_category,
                                  UINT64      ui8_group_mask);


/*------------------------------------------------------------------*/
/*! @brief  This API adds, deletes, or raplaces an alias element from alias table
  *  @param [in]   ps_alias           Contains the alias to attach.            
  *  @param [in]   ps_cmd           Contains the command string corresponding to the alias. 
  *  @retval      -CLIR_OK                          Routine successful.
  *  @retval      -CLIR_NOT_INIT                The CLI has not been initialized.
  *  @retval      -CLIR_INV_ARG                 One or more invalid arguments.
  *  @retval      -CLIR_ALIAS_TOO_LONG    Alias is too long.
  *  @retval      -CLIR_CMD_TOO_LONG      CLI command is too long.
  *  @retval      -CLIR_ALIAS_TBL_FULL      CLI alias table is full.
  *  @retval      -CLIR_CMD_EXEC_ERROR  CLI command execution failed.  
  */
/*------------------------------------------------------------------*/                                  
extern INT32 x_cli_attach_alias(const CHAR* ps_alias, 
                                const CHAR* ps_cmd);


/*-----------------------------------------------------------------------------*/
/*!@brief   This API adds, or raplaces an log element in log table.
 *  @param [in] 	ps_module_name	Contains the module  name to register.
 *  @param [in] 	pv_tag      		Contains the custom tag.
 *  @param [in]	pf_exec_func   	point to the executive function
 *  @param [in]	pf_callback_func 	point to the callback function
 *  @retval 		-CLIR_OK                 		Routine successful.
 *  @retval		-CLIR_NOT_INIT           	The CLI has not been initialized.
 *  @retval		-CLIR_INV_ARG            	One or more invalid arguments.
 *  @retval		-CLIR_MODULE_NAME_TOO_LONG     module name is too long.
 *  @retval		-CLIR_LOG_TBL_FULL     	CLI log table is full.
 *  @retval		-CLIR_NOT_ENABLED        	CLI is not enabled.
 */
/* ----------------------------------------------------------------------------*/

extern INT32 x_cli_register_log_command(const CHAR* ps_module_name, 
                                VOID* pv_tag,
                                x_cli_exec_fct pf_exec_func,
                                x_cli_exec_fct pf_callback_func);

                                
/*------------------------------------------------------------------*/
/*! @brief  This API parses CLI command and performs corresponding operation.
  *  @param [in]   ps_cmd           Contain the command to parse. 
  *  @retval      -CLIR_OK                          Routine successful.
  *  @retval      -CLIR_NOT_INIT                The CLI has not been initialized.
  *  @retval      -CLIR_INV_ARG                 One or more invalid arguments.
  *  @retval      -CLIR_UNKNOWN_CMD       Unknown CLI command.
  */
/*------------------------------------------------------------------*/
extern INT32 x_cli_parser(const CHAR* ps_cmd);


/*------------------------------------------------------------------*/
/*! @brief  This API parses CLI command and performs corresponding operation. The variable-length argument lists are supported.
  *  @param [in]   ps_cmd          Contain the format.   
  *  @param [in]   ...                  Variable argument list.
  *  @retval      -CLIR_OK                          Routine successful.
  *  @retval      -CLIR_NOT_INIT                The CLI has not been initialized.
  *  @retval      -CLIR_INV_ARG                 One or more invalid arguments.
  *  @retval      -CLIR_UNKNOWN_CMD       Unknown CLI command.
  */
/*------------------------------------------------------------------*/
extern INT32 x_cli_parser_arg(const CHAR* ps_cmd, ...);



/*------------------------------------------------------------------*/
/*! @brief  This API generates a password according to current system date and time.
  *  @param [in]   t_password          Contain the password calculation method.
  *  @retval      -CLIR_OK                          Routine successful.
  *  @retval      -CLIR_CMD_EXEC_ERROR  Input password is incorrect.
  */
/*------------------------------------------------------------------*/
extern INT32 x_cli_passwd(CLI_PASSWD_T t_password);



/*------------------------------------------------------------------*/
/*! @brief  This API waits for the first available character within specific time period.
  *  @param [in]   ui4_time          Contain the waiting time period in ms.
  *  @retval      -ASCII_NULL   The UART queue is empty.
  *  @retval      -Others           The first available character from UART queue.
  */
/*------------------------------------------------------------------*/
extern CHAR x_cli_get_char_timeout(UINT32 ui4_time);



/*------------------------------------------------------------------*/
/*! @brief   This API parses a CLI command for debug level.
  *  @param [in]   i4_argc              Contain the number of arguments.
  *  @param [in]   pps_argv           Contain the points to the argument array.
  *  @param [out] pui2_dbg_level   Contain the value of debug level after parsing.
  *  @retval      -CLIR_OK                          Routine successful.
  *  @retval      -CLIR_CMD_EXEC_ERROR  CLI command execution failed.  
  */
/*------------------------------------------------------------------*/
extern INT32 x_cli_parse_dbg_level(INT32        i4_argc, 
                                   const CHAR** pps_argv,
                                   UINT16*      pui2_dbg_level);
                                   

/*------------------------------------------------------------------*/
/*! @brief   This API display debug level in text format.
  *  @param [in]   ui2_dbg_level     Contain the value of debug level to display.
  *  @retval      -CLIR_OK                          Routine successful.
  */
/*------------------------------------------------------------------*/
extern INT32 x_cli_show_dbg_level(UINT16 ui2_dbg_level);


/*------------------------------------------------------------------*/
/*! @brief   This API parses a log command
  *  @param [in]   i4_argc              Contain the number of arguments.
  *  @param [in]   pps_argv           Contain the points to the argument array.
  *  @param [out] pui4_log_level    Contain the value of log level after parsing.
  *  @retval      -CLIR_OK                          Routine successful.
  *  @retval 	   -CLIR_NOT_INIT           The CLI has not been initialized.
  *  @retval      -CLIR_NOT_ENABLED     CLI is not enabled.
  * 	@retval 	   -CLIR_INV_ARG		      Invalid arguments
  */
/*------------------------------------------------------------------*/
extern INT32 x_cli_parse_log_level(INT32        i4_argc, 
                                   const CHAR** pps_argv,
                                   UINT32*      pui4_log_level);


/*------------------------------------------------------------------*/
/*! @brief   This API display debug level in text format.
  *  @param [in]	ps_module_name	  Contain the module name
  *  @param [in]   ui4_log_level     Contain the value of log level to display.
  *  @retval      -CLIR_OK                          Routine successful.
  */
/*------------------------------------------------------------------*/
extern INT32 x_cli_show_log_level(const CHAR* ps_module_name, 
								UINT32 ui4_log_level);


/*------------------------------------------------------------------*/
/*! @brief   This API parses a CLI command for time measurement level.
  *  @param [in]   i4_argc              Contain the number of arguments.
  *  @param [in]   pps_argv           Contain the points to the argument array.
  *  @param [out] pui2_tms_level   Contain the value of time measurement level after parsing.
  *  @retval      -CLIR_OK                          Routine successful.
  *  @retval      -CLIR_NOT_INIT                The CLI has not been initialized.
  *  @retval      -CLIR_CMD_EXEC_ERROR  Input password is incorrect.
  *  @retval      -CLIR_NOT_ENABLED         CLI is not enabled.
  */
/*------------------------------------------------------------------*/
extern INT32 x_cli_parse_tms_level(INT32        i4_argc, 
                                   const CHAR** pps_argv,
                                   UINT16*      pui2_tms_level);




/*------------------------------------------------------------------*/
/*! @brief   This API display time measurement level in text format.
  *  @param [in]   ui2_tms_level     Contain the value of time measurement level to display.
  *  @retval      -CLIR_OK                          Routine successful.
  *  @retval      -CLIR_NOT_INIT                The CLI has not been initialized.
  *  @retval      -CLIR_NOT_ENABLED         CLI is not enabled.
  */
/*------------------------------------------------------------------*/                                   
extern INT32 x_cli_show_tms_level(UINT16 ui2_tms_level);



/*------------------------------------------------------------------*/
/*! @brief   This API is used to register callback functions for modules which have some features needed to be toggled from UI.
  *  @param [in]   ui2_hotkeys      Contain the value of hotkeys to identify callbacks.
  *  @param [in]   pf_hotkey_cb     Contain (*x_cli_ui_hotkey_fct) type callback.
  *  @param [in]   pv_tag               Contain the tag of callback function.
  *  @retval      -CLIR_OK                  Routine successful.
  *  @retval      -CLIR_DUP_KEY          The input hotkeys is duplicated.
  *  @retval      -CLIR_OUT_OF_MEM   Out of memory.
  *  @retval      -CLIR_INV_ARG         Invalid arguments.
  */
/*------------------------------------------------------------------*/              
extern INT32 x_cli_ui_hotkey_register (UINT16               ui2_hotkeys,
                                       x_cli_ui_hotkey_fct  pf_hotkey_cb,
                                       VOID*                pv_tag);


/*------------------------------------------------------------------*/
/*! @brief   This API is used to trigger the hotkey callback functions.
  *  @param [in]   pui4_keys     Contains a hotkey array.
  *  @param [in]   ui2_key_len  Contain the length of the hotkey array.
  *  @retval      -CLIR_OK                      Routine successful.
  *  @retval      -CLIR_NOT_INIT            The CLI sub-module has not been initialized.
  *  @retval      -CLIR_INV_ARG             Invalid arguments.
  *  @retval      -CLIR_NOT_ENABLED     CLI sub-module is not enabled.
  */
/*------------------------------------------------------------------*/ 
extern INT32 x_cli_ui_send_hotkeys (UINT32* pui4_keys,
                                    UINT16  ui2_key_len);

#endif /* _X_CLI_H_ */

/*----------------------------------------------------------------------------*/
/*! @} */                                                 /* end of groupMW_CLI */
/*----------------------------------------------------------------------------*/


