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

#ifndef CLI_H
#define CLI_H

#include "x_typedef.h"
#include "x_drv_cli.h"

#ifndef CC_CLI_PERMISSION
#define CC_CLI_PERMISSION CLI_GUEST
#endif

// CLI Input Related

//#define CLI_SW_FIFO
#ifdef CLI_SW_FIFO
#define CLI_UART_INPUT_BUF_SIZE					2048			// Software FIFO Size
#define CLI_UART_INPUT_BUF_WAIT_TIME			20000			// Software FIFO wait time, micro-second
#endif

#if 0 // def CC_MINI_DRIVER
#define CLI_INPUT_BUF_ROW_NUM					3				// number of DOS key row
#define CLI_INPUT_BUF_SIZE						40				// number of character per row
#else /* CC_MINI_DRIVER */
#define CLI_INPUT_BUF_ROW_NUM					16				// number of DOS key row
#define CLI_INPUT_BUF_SIZE						256				// number of character per row
#endif /* CC_MINI_DRIVER */

#define CLI_PROMPT_STR							"DTV"			// prompt string of CLI

// ASCII key define
#define ASCII_NULL								0x00
#define ASCII_KEY_BS							0x08
#define ASCII_KEY_NL							0x0A
#define ASCII_ENTER								0x0D
#define ASCII_KEY_CR							0x0D
#define ASCII_KEY_ESC							0x1B
#define ASCII_KEY_SPACE							0x20
#define ASCII_KEY_DBL_QUOTE						0x22
#define ASCII_KEY_QUOTE							0x27
#define ASCII_KEY_DOT							0x2e
#define ASCII_KEY_DOLLAR						0x24
#define ASCII_KEY_UP							0x41
#define ASCII_KEY_DOWN							0x42
#define ASCII_KEY_RIGHT							0x43
#define ASCII_KEY_LEFT							0x44
#define ASCII_KEY_ARROW							0x5B
#define ASCII_KEY_ROOT							0x5c

#define ASCII_KEY_PRINTABLE_MIN					0x20
#define ASCII_KEY_PRINTABLE_MAX					0x7E

#define IsPrintable(c) ((((c) > ASCII_NULL) && ((c) < ASCII_KEY_SPACE)) ? 0 : 1)
#define IsSpace(c) (((c)==' ') || ((c)=='\n') || ((c)=='\t') || ((c)=='\r') || ((c)=='\a'))
#define IsDot(c) (((c) == ASCII_KEY_DOT) ? 1 : 0)
#define IsRoot(c) (((c) == ASCII_KEY_ROOT) ? 1 : 0)

// CLI Parser Related

#if 0 // def CC_MINI_DRIVER
#define CLI_MAX_ARGU							6				// maimum number of argument
#define CLI_MAX_ARGU_LEN						32				// length of each argument
#define CLI_INPUT_MAX_CMD_TBL_NUM				4				// maximum number of attach command root table
#define CLI_MANDA_CMD_TBL_IDX					0				// mandatory command root table index
#define CLI_MAX_CMD_TBL_LEVEL					8				// maximum level of command table
#else /* CC_MINI_DRIVER */
#define CLI_MAX_ARGU							32				// maimum number of argument
#define CLI_MAX_ARGU_LEN						128				// length of each argument
#define CLI_INPUT_MAX_CMD_TBL_NUM				16				// maximum number of attach command root table
#define CLI_MANDA_CMD_TBL_IDX					0				// mandatory command root table index
#define CLI_MAX_CMD_TBL_LEVEL					16				// maximum level of command table
#endif /* CC_MINI_DRIVER */

extern void CLI_CmdTblClear(void);
extern void CLI_ParserSetPromptStr(CHAR* szPtr);

// CLI Alias Related

#define CLI_ALIAS_NUM							0x10			// maximum number of alias support
#define CLI_ALIAS_SIZE							0x10			// maximum number of alias characters

#define CLI_ALIAS_CMD_STR						"alias"
#define CLI_ALIAS_CMD_ABBR_STR					"a"

extern void CLI_AliasInit(void);
extern const CHAR* CLI_AliasCompare(const CHAR* szAlias);
extern INT32 CLI_CmdAlias(INT32 i4Argc, const CHAR ** szArgv);

#endif /* CLI_H */


