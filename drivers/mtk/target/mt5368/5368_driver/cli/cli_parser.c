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


#include "cli.h"

#include "x_os.h"
#include "x_stl_lib.h"
#include "x_printf.h"
#include "x_assert.h"
#include "x_lint.h"


//-----------------------------------------------------------------------------
// External variables
//-----------------------------------------------------------------------------

#ifdef CC_MTK_LOADER
#ifndef __KERNEL__
#if !defined(__GCC__)
EXTERN UINT32 Image$$DATACLI$$Base;
#else
EXTERN UINT32 __datacli_start; 
#endif
#endif /* __KERNEL__ */
#endif /* CC_MTK_LOADER */


/******************************************************************************
* local variables
******************************************************************************/
static CHAR _aszArgv[CLI_MAX_ARGU][CLI_MAX_ARGU_LEN];           // temp argument buffer
#ifndef CC_MINI_DRIVER
static CHAR _aszAliasArgv[CLI_MAX_ARGU][CLI_MAX_ARGU_LEN];      // temp alias argument buffer
#endif /* CC_MINI_DRIVER */

#ifdef CC_MTK_LOADER
static CLI_EXEC_T* _prCmdTblBuf = NULL;                         // root command table pointer
#else
static CLI_EXEC_T* _aprCmdTblBuf[CLI_INPUT_MAX_CMD_TBL_NUM];    // root command table pointer
static UINT32 _u4CmdTblNum = 0;                                 // number of root command table
#endif /* CC_MTK_LOADER */

static CLI_EXEC_T* _aprCmdTblLinkBuf[CLI_MAX_CMD_TBL_LEVEL];    // for 'cd' command
static UINT32 _u4CmdTblLinkIdx = 0;                             // for 'cd' command
static CLI_EXEC_T* _prCurrentCmdTbl = NULL;                     // current command table level, for 'cd' command

static CHAR* _szCliPromptStr = NULL;

static CLI_ACCESS_RIGHT_T _eCliMode = CC_CLI_PERMISSION;            // global permission variable

/******************************************************************************
* Function prototype
******************************************************************************/

static void _CliShowHelp(const CLI_EXEC_T * prCmdTbl);
static INT32 _CliStringParser(const CHAR *szCmdBuf, UINT32 u4ArgNum, UINT32 u4ArgLen, CHAR * const * szArgv);
static CLI_EXEC_T* _CliCmdSearch(const CHAR*  szArgv, CLI_EXEC_T * prCmdTbl);
static INT32 _CliCmdParser(INT32 i4Argc, const CHAR**  szArgv, CLI_EXEC_T * prCmdTbl, INT32 fgNoChk);
static void _CliGeneratePrompt(void);
INT32 CLI_CmdRepeat(INT32 i4Argc, const CHAR ** szArgv);
INT32 CLI_CmdChangeDirectory(INT32 i4Argc, const CHAR ** szArgv);

#ifndef CC_MTK_LOADER
static INT32 CLI_CmdList(INT32 i4Argc, const CHAR ** szArgv);
#endif

/******************************************************************************
* Variable      : cli mandatory command table
******************************************************************************/
//lint --e{64}
const CLI_EXEC_T _arMandatoryCmdTbl[] CLI_MAIN_COMMAND =
{
    {
        "cd",                       //pszCmdStr
        NULL,
        CLI_CmdChangeDirectory,     //execution function
        NULL,
        "Change current directory",
        CLI_GUEST
    },

    {
        "do",                       //pszCmdStr
        NULL,
        CLI_CmdRepeat,              //execution function
        NULL,
        "Repeat command",
        CLI_GUEST
    },

#ifndef CC_MINI_DRIVER
    {
        CLI_ALIAS_CMD_STR,          //pszCmdStr
        CLI_ALIAS_CMD_ABBR_STR,
        CLI_CmdAlias,               //execution function
        NULL,
        "Add/Show current alias",
        CLI_GUEST
    },
#endif /* CC_MINI_DRIVER */

#ifndef CC_MTK_LOADER
	{
		"ls",						//pszCmdStr
		NULL,
		CLI_CmdList,		//execution function
		NULL,
		"Recursive list all commands",
		CLI_GUEST
	},
    // last cli command record, NULL
    {
        NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR
    }
#endif /* CC_MTK_LOADER */
};


//=====================================================================

#define HELP_STR_POS            24

static BOOL _FormatHelpString(CHAR* szDst, UINT32 u4DstSize, const CHAR* szCmd,
    const CHAR* szAbbr, const CHAR* szHelp)
{
    UINT32 u4CmdLen, u4AbbrLen, u4HelpLen;
    BOOL fgOut;

    ASSERT(szCmd != NULL);

    // Check if destination buffer is big enough
    u4CmdLen = x_strlen(szCmd);
    u4AbbrLen = x_strlen(szAbbr);
    u4HelpLen = x_strlen(szHelp);
    fgOut = FALSE;
    if (szHelp != NULL)
    {
        if ((u4CmdLen + (u4AbbrLen + 4)) > HELP_STR_POS)
        {
            if ((u4CmdLen + (u4AbbrLen + (u4HelpLen + 5))) > u4DstSize)
            {
                fgOut = TRUE;
            }
        }
        else
        {
            if ((HELP_STR_POS + (u4HelpLen + 2)) > u4DstSize)
            {
                fgOut = TRUE;
            }
        }
    }
    else
    {
        if ((u4CmdLen + (u4AbbrLen + 4)) > u4DstSize)
        {
            fgOut = TRUE;
        }
    }
    if (fgOut)
    {
        return FALSE;
    }

    // Format command and abbreviation strings
    if (szAbbr != NULL)
    {
        SPrintf(szDst, "%s(%s):", szCmd, szAbbr);
    }
    else
    {
        SPrintf(szDst, "%s:", szCmd);
    }

    // Append spaces and help string
    if (szHelp != NULL)
    {
        UINT32 i;

        ASSERT(szDst!=NULL);
        for (i = x_strlen(szDst); i < HELP_STR_POS; i++)
        {
            szDst[i] = ' ';
        }
        LINT_SUPPRESS_NEXT_EXPRESSION(534);
        x_strcpy(szDst + HELP_STR_POS, szHelp);
    }

    return TRUE;
}

#ifndef CC_MTK_LOADER
#define MAX_CLI_PATH_LENGTH 64
static void _CLI_PrintCmd(const CHAR *cmdPath,CLI_EXEC_T  *cli_cmd)
{
	CHAR *pszStr;

    ASSERT(cli_cmd != NULL);
    UNUSED(cli_cmd);
	pszStr = cli_cmd->pszCmdStr;
	if (cli_cmd->pszCmdAbbrStr!=NULL)
    {
		pszStr = cli_cmd->pszCmdAbbrStr;
    }

	if (x_strlen(cmdPath)>0)
	{
		printf("%s.%s",cmdPath,pszStr);
	}
	else
	{
		printf("%s",pszStr);
	}
	if (cli_cmd->pszCmdHelpStr!=NULL)
    {
		printf("\r\t\t\t:%s",cli_cmd->pszCmdHelpStr);
    }
	printf("\n");
}

static void _CLI_RecursiveListCliCmd(const CHAR *cmdPath,CLI_EXEC_T * prCmdTbl,CLI_ACCESS_RIGHT_T access_right)
{
	UINT32 u4Idx;
	CHAR szBuffer[MAX_CLI_PATH_LENGTH+1];
	CHAR *pszStr;
	UINT32 u4Tmp1;
	
	if (prCmdTbl != NULL)
	{
		for (u4Idx = 0;prCmdTbl[u4Idx].pszCmdStr != NULL;u4Idx++) //print command first
		{
			if ((prCmdTbl[u4Idx].eAccessRight >= access_right) && 
				(prCmdTbl[u4Idx].prCmdNextLevel == NULL ))
			{
				_CLI_PrintCmd(cmdPath,&prCmdTbl[u4Idx]);
			}
		}
		for (u4Idx = 0;prCmdTbl[u4Idx].pszCmdStr != NULL;u4Idx++) //recursive into directory
		{
			if ((prCmdTbl[u4Idx].eAccessRight >= access_right) && 
				(prCmdTbl[u4Idx].prCmdNextLevel != NULL))
			{
				
				pszStr = prCmdTbl[u4Idx].pszCmdStr;
				if (prCmdTbl[u4Idx].pszCmdAbbrStr!=NULL)
                {
					pszStr = prCmdTbl[u4Idx].pszCmdAbbrStr;
                }

				if ((x_strlen(cmdPath)+x_strlen(pszStr)+1)>MAX_CLI_PATH_LENGTH) //+1 for '.'
				{
					printf("%s: %s.%s too long can't recursive\n",__FUNCTION__,cmdPath,pszStr);
					continue;
				} 
				u4Tmp1 = x_strlen(cmdPath);
				if (u4Tmp1>0)
                {
					x_sprintf(szBuffer,"%s.%s",cmdPath,pszStr);
                }
				else
                {
					x_sprintf(szBuffer,"%s",pszStr);
                }
				_CLI_RecursiveListCliCmd(szBuffer,prCmdTbl[u4Idx].prCmdNextLevel,access_right);
			}
		}
	}
	
}

static INT32 CLI_CmdList(INT32 i4Argc, const CHAR ** szArgv)
{
	const CHAR *pszStr;
	UINT32 u4Idx,u4CmdIdx;
	CLI_EXEC_T * prCmdTbl;

    UNUSED(i4Argc);
    UNUSED(szArgv);
	if (_prCurrentCmdTbl == NULL)	// root
	{
		for (u4Idx = CLI_MANDA_CMD_TBL_IDX; u4Idx < _u4CmdTblNum; u4Idx++) //print command first
		{
			for (u4CmdIdx = 0;_aprCmdTblBuf[u4Idx][u4CmdIdx].pszCmdHelpStr != NULL;u4CmdIdx++)
			{
				prCmdTbl = &(_aprCmdTblBuf[u4Idx][u4CmdIdx]);
				if ((prCmdTbl->eAccessRight >= _eCliMode) &&
					(prCmdTbl->prCmdNextLevel == NULL))
				{
					_CLI_PrintCmd("",prCmdTbl);
				}
				
			}
		}
		for (u4Idx = CLI_MANDA_CMD_TBL_IDX; u4Idx < _u4CmdTblNum; u4Idx++) //recursive into directory
		{
			for (u4CmdIdx = 0;_aprCmdTblBuf[u4Idx][u4CmdIdx].pszCmdHelpStr != NULL;u4CmdIdx++)
			{
				prCmdTbl = &(_aprCmdTblBuf[u4Idx][u4CmdIdx]);
				if ((prCmdTbl->eAccessRight >= _eCliMode) &&
					(prCmdTbl->prCmdNextLevel != NULL))
				{
					pszStr = prCmdTbl->pszCmdStr;
					if (prCmdTbl->pszCmdAbbrStr!=NULL)
                    {
						pszStr = prCmdTbl->pszCmdAbbrStr;
                    }
					_CLI_RecursiveListCliCmd(pszStr,prCmdTbl->prCmdNextLevel,_eCliMode);
				}
				
			}
		}
	}
	else
	{
		_CLI_RecursiveListCliCmd("",_prCurrentCmdTbl,_eCliMode);
	}
	return CLI_COMMAND_OK;
}
#endif
#define MAX_STR_SIZE            256 // 128

/****************************************************************************
Function        : _CliShowHelp
Description     : Show CLI command table help string
*****************************************************************************/
static void _CliShowHelp(const CLI_EXEC_T * prCmdTbl)
{
#ifndef CLI_NO_HELP
    UINT32 u4CmdIdx;
#ifdef CC_MTK_LOADER
    const CLI_EXEC_T* prCmd;
#else /* CC_MTK_LOADER */
    UINT32 u4Idx;
#endif /* CC_MTK_LOADER */
    CHAR szBuf[MAX_STR_SIZE];

#ifdef CC_MTK_LOADER
    if (prCmdTbl == NULL)   // root command table
    {
        prCmdTbl = _prCmdTblBuf;
    }

    Printf("[Help]\n");

    u4CmdIdx = 0;
    while (prCmdTbl[u4CmdIdx].pszCmdHelpStr != NULL)
    {
        prCmd = &prCmdTbl[u4CmdIdx];

        VERIFY(_FormatHelpString(szBuf, MAX_STR_SIZE, prCmd->pszCmdStr,
            prCmd->pszCmdAbbrStr, prCmd->pszCmdHelpStr));

        if ((prCmd != NULL) &&
            ((prCmd->prCmdNextLevel == NULL) || (prCmd->prCmdNextLevel->pszCmdStr != NULL)) &&
            (prCmd->eAccessRight >= _eCliMode)) {
            Printf("%s\n", szBuf);
        }

        u4CmdIdx++;
    }
#else /* CC_MTK_LOADER */
	if (prCmdTbl == NULL)	// root command table
	{
		Printf("[Help]\n");
		for (u4Idx = CLI_MANDA_CMD_TBL_IDX; u4Idx < _u4CmdTblNum; u4Idx++)
		{
			u4CmdIdx = 0;

			while (_aprCmdTblBuf[u4Idx][u4CmdIdx].pszCmdHelpStr != NULL)
			{
			    const CLI_EXEC_T* prCmd;

			    prCmd = &_aprCmdTblBuf[u4Idx][u4CmdIdx];

			    VERIFY(_FormatHelpString(szBuf, MAX_STR_SIZE, prCmd->pszCmdStr,
			        prCmd->pszCmdAbbrStr, prCmd->pszCmdHelpStr));

                if (((prCmd->prCmdNextLevel == NULL) || (prCmd->prCmdNextLevel->pszCmdStr != NULL)) &&
                    (prCmd->eAccessRight >= _eCliMode)) {
			        Printf("%s\n", szBuf);
                }

				u4CmdIdx++;
			}
		}
	}
	else
	{
		Printf("[Help]\n");

		u4CmdIdx = 0;
		while (prCmdTbl[u4CmdIdx].pszCmdHelpStr != NULL)
		{
		    const CLI_EXEC_T* prCmd;

		    prCmd = &prCmdTbl[u4CmdIdx];

		    VERIFY(_FormatHelpString(szBuf, MAX_STR_SIZE, prCmd->pszCmdStr,
		        prCmd->pszCmdAbbrStr, prCmd->pszCmdHelpStr));

            if (((prCmd->prCmdNextLevel == NULL) || (prCmd->prCmdNextLevel->pszCmdStr != NULL)) &&
                (prCmd->eAccessRight >= _eCliMode)) {
		        Printf("%s\n", szBuf);
            }

			u4CmdIdx++;
		}
	}
#endif /* CC_MTK_LOADER */
#endif /* CLI_NO_HELP */
}

/****************************************************************************
Function        : _CliStringParser
Description     : String Parser
*****************************************************************************/
static INT32 _CliStringParser(const CHAR *szCmdBuf, UINT32 u4ArgNum, UINT32 u4ArgLen, CHAR * const * szArgv)
{
    INT32 i4Argc = 0;
    CHAR cChar;
    CHAR * pcStr;
    UINT32 u4Cnt;
    INT32 u4State; /* 1 is single quote, 2, is double quota. */

    if ((szCmdBuf == NULL) || (u4ArgNum == 0) || (u4ArgLen == 0) || (szArgv == NULL))
    {
        return 0;
    }

    cChar = *szCmdBuf;

    while (cChar != ASCII_NULL)
    {
        pcStr = szArgv[i4Argc];

        // skip space
        while (!IsPrintable(cChar) || IsSpace(cChar))
        {
            cChar = *(++szCmdBuf);
        }

        // copy non-space characters
        u4Cnt = 0;
        u4State = 0;
        while (IsPrintable(cChar) && (((u4State==0) && !IsSpace(cChar)) ||
                ((u4State==1) && (cChar!=ASCII_KEY_QUOTE)) ||
                ((u4State==2) && (cChar!=ASCII_KEY_DBL_QUOTE))))
        {

            if (cChar == ASCII_NULL)    // end of string
            {
                *pcStr = ASCII_NULL;    // end of string
                break;
            }

            if (u4Cnt >= (u4ArgLen - 1))    // maximum argument length
            {
                *pcStr = ASCII_NULL;    // end of string
                break;
            }

            if ((u4State!=2) && (cChar==0x27)) {
                if (u4State==0) {
                    u4State = 1;
                    cChar = *(++szCmdBuf);
                    u4Cnt++;
                } else if (u4State==1) {
                    u4State = 0;
                }
                continue;
            } else if ((u4State!=1) && (cChar==0x22)) {
                if (u4State==0) {
                    u4State = 2;
                    cChar = *(++szCmdBuf);
                    u4Cnt++;
                } else if (u4State==2) {
                    u4State = 0;
                }
                continue;
            }

            *pcStr = cChar;
            u4Cnt++;
            pcStr++;
            cChar = *(++szCmdBuf);
        }

        if ((cChar==ASCII_KEY_QUOTE) || (cChar==ASCII_KEY_DBL_QUOTE)) {
            cChar = *(++szCmdBuf);
            u4Cnt++;
        }

        if (u4Cnt > 0)
        {
            *pcStr = ASCII_NULL;    // end of string
            i4Argc++;
        }

        if ((UINT32)i4Argc >= u4ArgNum)
        {
            break;
        }
    }

    return i4Argc;
}


/****************************************************************************
Function        : _CliCmdSearch
Description     : Search Command
*****************************************************************************/
static CLI_EXEC_T* _CliCmdSearch(const CHAR*  szArgv, CLI_EXEC_T * prCmdTbl)
{
    UINT32 u4CmdIdx;
    UINT32 u4StrLen;
    UINT32 u4CmdLen;
    UINT32 u4CmdAbbrLen;
    const CHAR *szStr;
    UINT8 ucFound = 0;

    if ((szArgv == NULL) || (prCmdTbl == NULL))
    {
        return NULL;
    }

    szStr = szArgv;
    u4StrLen = 0;
    while (!IsDot(*szStr) &&
        (*szStr != ASCII_NULL))
    {
        szStr++;
        u4StrLen++;
    }

    // search commmad from command table. If found, run command
    u4CmdIdx = 0;
    while (prCmdTbl[u4CmdIdx].pszCmdStr != NULL)
    {
        if (prCmdTbl[u4CmdIdx].pszCmdAbbrStr != NULL)
        {
            u4CmdAbbrLen = x_strlen(prCmdTbl[u4CmdIdx].pszCmdAbbrStr);

            if ((u4StrLen == u4CmdAbbrLen) &&
                (x_strncmp(prCmdTbl[u4CmdIdx].pszCmdAbbrStr, szArgv, u4StrLen) == 0))
            {
                ucFound = 1;
                break;
            }
        }

        u4CmdLen = x_strlen(prCmdTbl[u4CmdIdx].pszCmdStr);

        if ((u4StrLen == u4CmdLen) &&
            (x_strncmp(prCmdTbl[u4CmdIdx].pszCmdStr, szArgv, u4StrLen) == 0))
        {
            ucFound = 1;
            break;
        }

        u4CmdIdx++;
    }

    // execute command
    if (ucFound)
    {
        return &prCmdTbl[u4CmdIdx];
    }

    return NULL;
}


/****************************************************************************
Function        : _CliCmdParser
Description     : Command Parser
*****************************************************************************/
static INT32 _CliCmdParser(INT32 i4Argc, const CHAR**  szArgv, CLI_EXEC_T * prCmdTbl, INT32 fgNoChk)
{
    const CHAR *szStr;
    CLI_EXEC_T *pCmdTbl;

    if ((szArgv == NULL) || (prCmdTbl == NULL))
    {
        return CLI_UNKNOWN_CMD;
    }

    szStr = szArgv[0];
    while (!IsDot(*szStr) &&
        (*szStr != ASCII_NULL))
    {
        szStr++;
    }

    pCmdTbl = _CliCmdSearch(szArgv[0], prCmdTbl);

    // execute command
    if ((pCmdTbl != NULL) &&
        (fgNoChk || (pCmdTbl->eAccessRight >= _eCliMode)))
    {
        if ((pCmdTbl->prCmdNextLevel == NULL) && (pCmdTbl->pfExecFun == NULL))
        {
            ASSERT(0);
        }

        if (pCmdTbl->pfExecFun != NULL)
        {
            if ((pCmdTbl->prCmdNextLevel == NULL) ||
                (!IsDot(*szStr) && (i4Argc > 1)))       // entry can be a directory or a command
            {
                INT32 i4Return = pCmdTbl->pfExecFun(i4Argc, szArgv);

                return i4Return;
            }
        }

        if (pCmdTbl->prCmdNextLevel != NULL)
        {
            if (*szStr == ASCII_NULL)
            {
                // cli help
                _CliShowHelp(pCmdTbl->prCmdNextLevel);
                return CLI_COMMAND_OK;
            }

            szStr++;
            szArgv[0] = szStr;  // skip dot
            return _CliCmdParser(i4Argc, szArgv, pCmdTbl->prCmdNextLevel, fgNoChk); // go to next level
        }
    }

    return CLI_UNKNOWN_CMD;
}

/******************************************************************************
* Function      : _CliGeneratePrompt
* Description   : Generate CLI prompt string according to current directory
******************************************************************************/
static void _CliGeneratePrompt(void)
{
    UINT32 u4Idx;
    UINT32 u4TotalLen;
    UINT32 u4StrLen;
    CHAR* szCmd;

    _szCliPromptStr[0] = ASCII_NULL;
    u4TotalLen = 0;
    for (u4Idx = 1; u4Idx <= _u4CmdTblLinkIdx; u4Idx++)
    {
        if (_aprCmdTblLinkBuf[u4Idx]->pszCmdAbbrStr != NULL)
        {
            szCmd = _aprCmdTblLinkBuf[u4Idx]->pszCmdAbbrStr;
        }
        else
        {
            szCmd = _aprCmdTblLinkBuf[u4Idx]->pszCmdStr;
        }

        u4StrLen = x_strlen(szCmd);

        if ((u4TotalLen + u4StrLen + 1) > CLI_INPUT_BUF_SIZE)
        {
            Printf("CLI fatal error!\n");
            while(1) {}
        }

        VERIFY(x_strncat(_szCliPromptStr, ".", 2) == _szCliPromptStr);
        VERIFY(x_strncat(_szCliPromptStr, szCmd, (u4StrLen + 1)) == _szCliPromptStr);

        u4TotalLen += (u4StrLen + 1);
    }
}


/****************************************************************************
Function        : CLI_ParserSetPromptStr
Description     : Set Prompt String Pointer
*****************************************************************************/
void CLI_ParserSetPromptStr(CHAR* szPtr)
{
    _szCliPromptStr = szPtr;
}


/******************************************************************************
* Function      : CLI_CmdChangeDirectory
* Description   : CLI Command to change current command level
******************************************************************************/
INT32 CLI_CmdChangeDirectory(INT32 i4Argc, const CHAR ** szArgv)
{
    const CHAR *szStr;
    const CHAR *szCmd;
#ifndef CC_MTK_LOADER
    UINT32 u4Idx;
#endif /* CC_MTK_LOADER */
    CLI_EXEC_T* pCmdTbl = NULL;

    if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL))    // show alias
    {
        _CliShowHelp(_prCurrentCmdTbl);
        return 1;
    }

    szStr = szArgv[1];

    while (*szStr != ASCII_NULL)
    {
        if (IsDot(*szStr))
        {
            szStr++;
            if (IsDot(*szStr))  // cd ..
            {
                szStr++;
                if (_u4CmdTblLinkIdx > 0)
                {
                    _u4CmdTblLinkIdx --;

                    if (_u4CmdTblLinkIdx == 0)
                    {
                        _prCurrentCmdTbl = NULL;
                    }
                    else
                    {
                        _prCurrentCmdTbl = _aprCmdTblLinkBuf[_u4CmdTblLinkIdx]->prCmdNextLevel;
                    }

                    _CliGeneratePrompt();
                }
            }
            else
            {
                return -1;
            }
        }
        else
        if (IsRoot(*szStr))     // cd root
        {
            szStr++;

            _u4CmdTblLinkIdx = 0;
            _prCurrentCmdTbl = NULL;

            _CliGeneratePrompt();
        }
        else
        {
            szCmd = szStr;
            while (!IsDot(*szStr) &&
                (*szStr != ASCII_NULL))
            {
                szStr++;
            }

            if (_prCurrentCmdTbl == NULL)   // root
            {
#ifdef CC_MTK_LOADER
                pCmdTbl = _CliCmdSearch(szCmd, _prCmdTblBuf);
#else /* CC_MTK_LOADER */
				for (u4Idx = (CLI_MANDA_CMD_TBL_IDX + 1); u4Idx < _u4CmdTblNum; u4Idx++)
				{
					pCmdTbl = _CliCmdSearch(szCmd, _aprCmdTblBuf[u4Idx]);

					if (pCmdTbl != NULL)
					{
						break;
					}
				}
#endif /* CC_MTK_LOADER */
            }
            else
            {
                pCmdTbl = _CliCmdSearch(szCmd, _prCurrentCmdTbl);
            }

            if ((pCmdTbl != NULL) && (pCmdTbl->prCmdNextLevel != NULL) &&
                (pCmdTbl->eAccessRight >= _eCliMode))
            {
                if (_u4CmdTblLinkIdx < (CLI_MAX_CMD_TBL_LEVEL - 2))
                {
                    _u4CmdTblLinkIdx++;
                    _aprCmdTblLinkBuf[_u4CmdTblLinkIdx] = pCmdTbl;
                    _prCurrentCmdTbl = _aprCmdTblLinkBuf[_u4CmdTblLinkIdx]->prCmdNextLevel;

                    _CliGeneratePrompt();
                }
                else
                {
                    Printf("CLI fatal error!\n");
                    while(1) {}
                }
            }
            else
            {
                return -1;
            }

            if (IsDot(*szStr))
            {
                szStr++;    // skip 'dot' character
            }
        }
    }

    return 1;
//lint -e{818}
}


/******************************************************************************
* Function      : CLI_CmdChangeDirectory
* Description   : CLI Command to change current command level
******************************************************************************/
INT32 CLI_CmdRepeat(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Idx;
    UINT32 u4Dec;
    UINT32 u4LoopTotal;
    UINT32 u4LoopCount;
    UINT32 u4StringSize;
    CHAR szStringBuf[CLI_INPUT_BUF_SIZE];

    if ((i4Argc < 2) || (szArgv == NULL))   // show alias
    {
        Printf("Usage: do loop cmd\tex: do 10 read 0x200 0x10\n");
        return -1;
    }

    for (u4Idx = 2; u4Idx < (UINT32) i4Argc; u4Idx++)
    {
        if (szArgv[u4Idx] == NULL)
        {
            while (1)
            {}
        }
    }

    u4LoopTotal = StrToInt(szArgv[1]);

    if (u4LoopTotal == 0)
    {
        u4LoopTotal = ~0;
        u4Dec = 0;
    }
    else
    {
        u4Dec = 1;
    }

    // appends all string together
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

    u4LoopCount = 0;
    while (u4LoopTotal)
    {
        Printf("Loop %d\n", u4LoopCount);

        LINT_SUPPRESS_NEXT_EXPRESSION(534);
        CLI_PromptParser(szStringBuf);

        u4LoopTotal -= u4Dec;
        u4LoopCount++;
    }

    return 1;
//lint -e{818}
}

/******************************************************************************
* Function      : CLI_CmdTblClear
* Description   : cli command table clear
******************************************************************************/
void CLI_CmdTblClear(void)
{
#ifdef CC_MTK_LOADER
#ifndef __KERNEL__
#if !defined(__GCC__)
    _prCmdTblBuf = (CLI_EXEC_T *) &Image$$DATACLI$$Base;
#else
    _prCmdTblBuf = (CLI_EXEC_T *) &__datacli_start; 
#endif
#else /* __KERNEL__ */
    _prCmdTblBuf = (CLI_EXEC_T *) NULL; // XXX fix me
#endif /* __KERNEL__ */
#else
    UINT32 u4TblIdx;

    for (u4TblIdx = 0; u4TblIdx < CLI_INPUT_MAX_CMD_TBL_NUM; u4TblIdx++)
    {
        _aprCmdTblBuf[u4TblIdx] = NULL;
    }

    _u4CmdTblNum = 0;
    VERIFY( CLI_CmdTblAttach((CLI_EXEC_T *)_arMandatoryCmdTbl) != -1);

    _aprCmdTblLinkBuf[0] = NULL;
    _u4CmdTblLinkIdx = 0;
    _prCurrentCmdTbl = NULL;
#endif
}


#ifndef CC_MTK_LOADER
/******************************************************************************
* Function      : CLI_CmdTblAttach(CLI_EXEC_T* pTbl, UINT32 u4Len)
* Description   : cli command table attach
******************************************************************************/
INT32 CLI_CmdTblAttach(CLI_EXEC_T* pTbl)
{
    if (_u4CmdTblNum >= CLI_INPUT_MAX_CMD_TBL_NUM)
    {
        return -1;
    }

    if (pTbl == NULL)
    {
        return -1;
    }

    _aprCmdTblBuf[_u4CmdTblNum] = pTbl;
    _u4CmdTblNum++;

    return 0;
}
#endif /* CC_MTK_LOADER */

/****************************************************************************
Function        : CLI_Parser
Description     : CLI Parer main function
*****************************************************************************/
INT32 CLI_PromptParser(const CHAR *szCmdBuf)
{
    INT32 i4ReturnValue;
    INT32 i4Argc;
    UINT32 u4Argc;
    UINT32 u4InCmdLen;
    UINT32 u4CmdLen;
    UINT32 u4CmdAbbrLen;
    CHAR* aszTemp[CLI_MAX_ARGU];
#ifndef CC_MINI_DRIVER
    const CHAR *pszAliasString;
#endif /* CC_MINI_DRIVER */

    if (szCmdBuf == NULL)
    {
        return CLI_UNKNOWN_CMD;
    }

    // prepare argument
    for (u4InCmdLen = 0; u4InCmdLen < CLI_MAX_ARGU; u4InCmdLen++)
    {
        aszTemp[u4InCmdLen] = _aszArgv[u4InCmdLen];
    }

    i4Argc = _CliStringParser(szCmdBuf, CLI_MAX_ARGU, CLI_MAX_ARGU_LEN, aszTemp);
    u4Argc = (UINT32) i4Argc;

    if (u4Argc > 0)
    {
        // alias handle, replace alias with corresponding string
        u4InCmdLen = x_strlen(aszTemp[0]);
        u4CmdLen = x_strlen(CLI_ALIAS_CMD_STR);
        u4CmdAbbrLen = x_strlen(CLI_ALIAS_CMD_ABBR_STR);

        if (!(((u4InCmdLen == u4CmdLen) && (x_strncmp(aszTemp[0], CLI_ALIAS_CMD_STR, u4InCmdLen) == 0)) ||
            ((u4InCmdLen == u4CmdAbbrLen) && (x_strncmp(aszTemp[0], CLI_ALIAS_CMD_ABBR_STR, u4InCmdLen) == 0))))
        {
            UINT32 u4Idx;
            u4Idx = 0;
            while (u4Idx < u4Argc)
            {
#ifndef CC_MINI_DRIVER
                pszAliasString = CLI_AliasCompare(aszTemp[u4Idx]);

                if (pszAliasString)
                {
                    UINT32 u4AliasIdx;
                    UINT32 u4AliasArgc;
                    CHAR* aszAliasTemp[CLI_MAX_ARGU];

                    // prepare alias argument
                    for (u4AliasIdx = 0; u4AliasIdx < CLI_MAX_ARGU; u4AliasIdx++)
                    {
                        aszAliasTemp[u4AliasIdx] = _aszAliasArgv[u4AliasIdx];
                    }

                    u4AliasArgc = (UINT32) _CliStringParser(pszAliasString, CLI_MAX_ARGU, CLI_MAX_ARGU_LEN, aszAliasTemp);

                    // too many argument after alias parser
                    if (u4AliasArgc > (CLI_MAX_ARGU - (u4Argc + 1)))
                    {
                        u4AliasArgc = CLI_MAX_ARGU - (u4Argc + 1);
                    }

                    // move back string argument after alias
                    for (u4AliasIdx = (u4Argc - 1); u4AliasIdx > u4Idx; u4AliasIdx--)
                    {
                        VERIFY(x_strncpy(aszTemp[u4AliasIdx + (u4AliasArgc - 1)],
                                aszTemp[u4AliasIdx],
                                CLI_MAX_ARGU_LEN) == aszTemp[u4AliasIdx + (u4AliasArgc - 1)]);
                    }

                    // copy alias string arguemnt
                    for (u4AliasIdx = 0; u4AliasIdx < u4AliasArgc; u4AliasIdx++)
                    {
                        VERIFY(x_strncpy(aszTemp[u4Idx+u4AliasIdx],
                                aszAliasTemp[u4AliasIdx],
                                CLI_MAX_ARGU_LEN) == aszTemp[u4Idx+u4AliasIdx]);
                    }

                    u4Argc += (u4AliasArgc - 1);
                    u4Idx--;
                }
#endif /* CC_MINI_DRIVER */

                u4Idx++;
            }
        }

        i4Argc = (INT32) u4Argc;

        // command parsing, for all other user command table
        if (i4Argc > 0)
        {
            if (_prCurrentCmdTbl == NULL)   // root
            {
#ifdef CC_MTK_LOADER
                i4ReturnValue = _CliCmdParser(i4Argc, (const CHAR**)aszTemp, _prCmdTblBuf, 0);
                if (i4ReturnValue != CLI_UNKNOWN_CMD)
                {
                    return i4ReturnValue;
                }
#else /* CC_MTK_LOADER */
				UINT32 u4Idx;

				for (u4Idx = (CLI_MANDA_CMD_TBL_IDX + 1); u4Idx < _u4CmdTblNum; u4Idx++)
				{
					i4ReturnValue = _CliCmdParser(i4Argc, (const CHAR**)aszTemp, _aprCmdTblBuf[u4Idx], 0);
					if (i4ReturnValue != CLI_UNKNOWN_CMD)
					{
						return i4ReturnValue;
					}
				}
#endif /* CC_MTK_LOADER */
            }
            else                            // sub-directory
            {
                i4ReturnValue = _CliCmdParser(i4Argc, (const CHAR**)aszTemp, _prCurrentCmdTbl, 0);
                if (i4ReturnValue != CLI_UNKNOWN_CMD)
                {
                    return i4ReturnValue;
                }
            }
        }

        // command parsing, mandatory table
        i4ReturnValue = _CliCmdParser(i4Argc, (const CHAR**)aszTemp, (CLI_EXEC_T *)_arMandatoryCmdTbl, 0);
        if (i4ReturnValue != CLI_UNKNOWN_CMD)
        {
            return i4ReturnValue;
        }
    }

    // cli help
    _CliShowHelp(_prCurrentCmdTbl);

    return CLI_UNKNOWN_CMD;
}

/****************************************************************************
Function        : CLI_Parser
Description     : CLI Parer main function
*****************************************************************************/
INT32 CLI_Parser(const CHAR *szCmdBuf)
{
    INT32 i4ReturnValue;
    INT32 i4Argc;
    UINT32 u4Argc;
    UINT32 u4InCmdLen;
    UINT32 u4CmdLen;
    UINT32 u4CmdAbbrLen;
    CHAR* aszTemp[CLI_MAX_ARGU];
#ifndef CC_MINI_DRIVER
    const CHAR *pszAliasString;
#endif /* CC_MINI_DRIVER */

    if (szCmdBuf == NULL)
    {
        return CLI_UNKNOWN_CMD;
    }

    // prepare argument
    for (u4InCmdLen = 0; u4InCmdLen < CLI_MAX_ARGU; u4InCmdLen++)
    {
        aszTemp[u4InCmdLen] = _aszArgv[u4InCmdLen];
    }

    i4Argc = _CliStringParser(szCmdBuf, CLI_MAX_ARGU, CLI_MAX_ARGU_LEN, aszTemp);
    u4Argc = (UINT32) i4Argc;

    if (u4Argc > 0)
    {
        // alias handle, replace alias with corresponding string
        u4InCmdLen = x_strlen(aszTemp[0]);
        u4CmdLen = x_strlen(CLI_ALIAS_CMD_STR);
        u4CmdAbbrLen = x_strlen(CLI_ALIAS_CMD_ABBR_STR);

        if (!(((u4InCmdLen == u4CmdLen) && (x_strncmp(aszTemp[0], CLI_ALIAS_CMD_STR, u4InCmdLen) == 0)) ||
            ((u4InCmdLen == u4CmdAbbrLen) && (x_strncmp(aszTemp[0], CLI_ALIAS_CMD_ABBR_STR, u4InCmdLen) == 0))))
        {
            UINT32 u4Idx;
            u4Idx = 0;
            while (u4Idx < u4Argc)
            {
#ifndef CC_MINI_DRIVER
                pszAliasString = CLI_AliasCompare(aszTemp[u4Idx]);

                if (pszAliasString)
                {
                    UINT32 u4AliasIdx;
                    UINT32 u4AliasArgc;
                    CHAR* aszAliasTemp[CLI_MAX_ARGU];

                    // prepare alias argument
                    for (u4AliasIdx = 0; u4AliasIdx < CLI_MAX_ARGU; u4AliasIdx++)
                    {
                        aszAliasTemp[u4AliasIdx] = _aszAliasArgv[u4AliasIdx];
                    }

                    u4AliasArgc = (UINT32) _CliStringParser(pszAliasString, CLI_MAX_ARGU, CLI_MAX_ARGU_LEN, aszAliasTemp);

                    // too many argument after alias parser
                    if (u4AliasArgc > (CLI_MAX_ARGU - (u4Argc + 1)))
                    {
                        u4AliasArgc = CLI_MAX_ARGU - (u4Argc + 1);
                    }

                    // move back string argument after alias
                    for (u4AliasIdx = (u4Argc - 1); u4AliasIdx > u4Idx; u4AliasIdx--)
                    {
                        VERIFY(x_strncpy(aszTemp[u4AliasIdx + (u4AliasArgc - 1)],
                                aszTemp[u4AliasIdx],
                                CLI_MAX_ARGU_LEN) == aszTemp[u4AliasIdx + (u4AliasArgc - 1)]);
                    }

                    // copy alias string arguemnt
                    for (u4AliasIdx = 0; u4AliasIdx < u4AliasArgc; u4AliasIdx++)
                    {
                        VERIFY(x_strncpy(aszTemp[u4Idx+u4AliasIdx],
                                aszAliasTemp[u4AliasIdx],
                                CLI_MAX_ARGU_LEN) == aszTemp[u4Idx+u4AliasIdx]);
                    }

                    u4Argc += (u4AliasArgc - 1);
                    u4Idx--;
                }
#endif /* CC_MINI_DRIVER */

                u4Idx++;
            }
        }

        i4Argc = (INT32) u4Argc;

        // command parsing, mandatory table
        i4ReturnValue = _CliCmdParser(i4Argc, (const CHAR**)aszTemp, (CLI_EXEC_T *)_arMandatoryCmdTbl, 1);
        if (i4ReturnValue != CLI_UNKNOWN_CMD)
        {
            return i4ReturnValue;
        }

        // command parsing, for all other user command table
        if (i4Argc > 0)
        {
            if (_prCurrentCmdTbl == NULL)   // root
            {
#ifdef CC_MTK_LOADER
                i4ReturnValue = _CliCmdParser(i4Argc, (const CHAR**)aszTemp, _prCmdTblBuf, 1);
                if (i4ReturnValue != CLI_UNKNOWN_CMD)
                {
                    return i4ReturnValue;
                }
#else /* CC_MTK_LOADER */
				UINT32 u4Idx;

				for (u4Idx = (CLI_MANDA_CMD_TBL_IDX + 1); u4Idx < _u4CmdTblNum; u4Idx++)
				{
					i4ReturnValue = _CliCmdParser(i4Argc, (const CHAR**)aszTemp, _aprCmdTblBuf[u4Idx], 1);
					if (i4ReturnValue != CLI_UNKNOWN_CMD)
					{
						return i4ReturnValue;
					}
				}
#endif /* CC_MTK_LOADER */
            }
            else                            // sub-directory
            {
                i4ReturnValue = _CliCmdParser(i4Argc, (const CHAR**)aszTemp, _prCurrentCmdTbl, 1);
                if (i4ReturnValue != CLI_UNKNOWN_CMD)
                {
                    return i4ReturnValue;
                }
            }
        }
    }

    // cli help
    _CliShowHelp(_prCurrentCmdTbl);

    return CLI_UNKNOWN_CMD;
}

#ifndef CC_MTK_LOADER
//lint -e530 -e718
INT32 CLI_Parser_Arg(const CHAR* szCmd, ...)
{
    CHAR szBuf[256];
    VA_LIST t_ap;
//  static INT32 _i4Cnt;

    VA_START(t_ap, szCmd);
    LINT_SUPPRESS_NEXT_EXPRESSION(534);
    x_vsprintf(szBuf, szCmd, t_ap);
    VA_END(t_ap);

//  Printf("CLI_Parser_Arg %s\n",  szBuf);
    return CLI_Parser(szBuf);
}
//lint -restore
#endif /* CC_MTK_LOADER */

INT32 CLI_SetCliMode(CLI_ACCESS_RIGHT_T eCliAccess)
{
    _eCliMode = eCliAccess;
    return 0;
}
