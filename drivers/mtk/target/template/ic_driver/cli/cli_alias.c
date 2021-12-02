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

#ifndef CC_5391_LOADER

#include "cli.h"

#include "x_stl_lib.h"
#include "x_printf.h"
#include "x_assert.h"


/******************************************************************************
* local variables
******************************************************************************/
static CHAR _aszCliAliasBuf[CLI_ALIAS_NUM][CLI_ALIAS_SIZE];
static CHAR _aszCliAliasString[CLI_ALIAS_NUM][CLI_INPUT_BUF_SIZE];


/******************************************************************************
* Function		: CLI_AliasInit
* Description	: cli alias table clear
******************************************************************************/
void CLI_AliasInit(void)
{
	UINT32 u4Idx;

	for (u4Idx = 0; u4Idx < CLI_ALIAS_NUM; u4Idx++)
	{
		_aszCliAliasBuf[u4Idx][0] = ASCII_NULL;
		_aszCliAliasString[u4Idx][0] = ASCII_NULL;
	}
}

/******************************************************************************
* Function		: CLI_AliasAttach
* Description	: add/delete/replace alias table
******************************************************************************/
INT32 CLI_AliasAttach(const CHAR* szAlias, const CHAR* szString)
{
	UINT32 u4AliasSize;
	UINT32 u4CommandSize;
	UINT32 u4StringSize;
	UINT32 u4Idx;

	if ((szAlias == NULL) || (szString == NULL))
	{
		return -1;
	}
	
	u4CommandSize = x_strlen(szAlias);
	u4StringSize = x_strlen(szString);

	if ((u4CommandSize >= CLI_ALIAS_SIZE) || (u4StringSize >= CLI_INPUT_BUF_SIZE))
	{
		return -1;
	}

	// replace or delete
	for (u4Idx = 0; u4Idx < CLI_ALIAS_NUM; u4Idx++)
	{
		u4AliasSize = x_strlen(_aszCliAliasBuf[u4Idx]);

		if ((u4CommandSize == u4AliasSize) &&
			(x_strncmp(szAlias, _aszCliAliasBuf[u4Idx], u4AliasSize) == 0))
		{
			if (u4StringSize == 0)	// delete
			{
				_aszCliAliasBuf[u4Idx][0] = ASCII_NULL;
				_aszCliAliasString[u4Idx][0] = ASCII_NULL;
			}
			else
			{
				VERIFY(x_memset(_aszCliAliasString[u4Idx], ASCII_NULL, CLI_INPUT_BUF_SIZE) == _aszCliAliasString[u4Idx]);				
				VERIFY(x_strncpy(_aszCliAliasString[u4Idx], szString, u4StringSize) == _aszCliAliasString[u4Idx]);
			}

			return 1;
		}		
	}

	// add new item
	for (u4Idx = 0; u4Idx < CLI_ALIAS_NUM; u4Idx++)
	{
		if (_aszCliAliasBuf[u4Idx][0] == ASCII_NULL)
		{
			VERIFY(x_strncpy(_aszCliAliasBuf[u4Idx], szAlias, u4CommandSize) == _aszCliAliasBuf[u4Idx]);
			VERIFY(x_strncpy(_aszCliAliasString[u4Idx], szString, u4StringSize) == _aszCliAliasString[u4Idx]);

			return 1;
		}
	}

	return -1;
}


/******************************************************************************
* Function		: CLI_AliasCompare
* Description	: search alias table
******************************************************************************/
const CHAR* CLI_AliasCompare(const CHAR* szAlias)
{
	UINT32 u4Idx;
	UINT32 u4CommandSize;
	UINT32 u4AliasSize;

	if (szAlias)
	{
		u4CommandSize = x_strlen(szAlias);

		for (u4Idx = 0; u4Idx < CLI_ALIAS_NUM; u4Idx++)
		{
			u4AliasSize = x_strlen(_aszCliAliasBuf[u4Idx]);

			if ((u4CommandSize == u4AliasSize) &&
				(x_strncmp(szAlias, _aszCliAliasBuf[u4Idx], u4AliasSize) == 0))
			{
				return _aszCliAliasString[u4Idx];
			}		
		}
	}

	return NULL;
}


/******************************************************************************
* Function		: CLI_CmdAlias
* Description	: CLI Command to add/delete/replace alias table 
******************************************************************************/
INT32 CLI_CmdAlias(INT32 i4Argc, const CHAR ** szArgv)
{
	UINT32 u4Idx;
	UINT32 u4AliasSize;
	UINT32 u4StringSize;
	CHAR szStringBuf[CLI_INPUT_BUF_SIZE];
		
	if ((i4Argc == 1) || (szArgv == NULL) || (szArgv[1] == NULL))	// show alias
	{
		for (u4Idx = 0; u4Idx < CLI_ALIAS_NUM; u4Idx++)
		{
			if (_aszCliAliasBuf[u4Idx][0] != ASCII_NULL)
			{
				Printf("%s:\t%s\n", _aszCliAliasBuf[u4Idx], _aszCliAliasString[u4Idx]);
			}
		}
	
		return 1;
	}

	// check alias length
	u4AliasSize = x_strlen(szArgv[1]);
	if (u4AliasSize >= CLI_ALIAS_SIZE)
	{
		return -1;
	}

	// check alias string length, appends all string together
	u4StringSize = 0;
	szStringBuf[0] = ASCII_NULL;
	for (u4Idx = 2; u4Idx < (UINT32) i4Argc; u4Idx++)
	{
		u4StringSize += x_strlen(szArgv[u4Idx]) + 1;

		if (u4StringSize >= CLI_INPUT_BUF_SIZE)
		{
			return -1;
		}

		VERIFY(x_strncat(szStringBuf, szArgv[u4Idx], (x_strlen(szArgv[u4Idx]) + 1)) == szStringBuf);
		VERIFY(x_strncat(szStringBuf, " ", 2) == szStringBuf);
	}

	return CLI_AliasAttach(szArgv[1], szStringBuf);
//lint -e{818}
}


#endif /* CC_5391_LOADER */

