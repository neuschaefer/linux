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
#include "drv_common.h"

#include "x_serial.h"
#include "x_printf.h"
#include "x_stl_lib.h"
#include "x_assert.h"
#include "x_lint.h"

#ifndef CC_5391_LOADER
#include "x_os.h"
#else /* CC_5391_LOADER */
extern VOID* x_memcpy (VOID*        pv_to,
                       const VOID*  pv_from,
                       SIZE_T       z_len);
#endif /* CC_5391_LOADER */

//-----------------------------------------------------------------------------
// Macros
//-----------------------------------------------------------------------------

#define PrintChar(x)        outbyte(x)


//-----------------------------------------------------------------------------
// External variables
//-----------------------------------------------------------------------------

EXTERN const CLI_EXEC_T _arDefaultCmdTbl[];
EXTERN const CLI_EXEC_T _arNullCmdTbl[];


/******************************************************************************
* local variables
******************************************************************************/
// CLI Input Character Buffer
static CHAR _aszCliInputBuf[CLI_INPUT_BUF_ROW_NUM][CLI_INPUT_BUF_SIZE];
static UINT32 _u4CliInputBufRowIdx = 0;
static UINT32 _u4CliInputBufRefRowIdx = 0;
static UINT32 _u4CliInputBufIdx = 0;

// CLI Prompt String
#ifndef CC_MINI_DRIVER
static const CHAR* _szMainPrompt = CLI_PROMPT_STR;
#endif /* CC_MINI_DRIVER */
static CHAR _szCliPromptStr[CLI_INPUT_BUF_SIZE];


/******************************************************************************
* Function prototype
******************************************************************************/

#ifdef CLI_SW_FIFO
// CLI Software Buffer
static CHAR _aszCliUartSwBuf[CLI_UART_INPUT_BUF_SIZE];
static UINT32 _u4CliUartSwBufRead = 0;
static UINT32 _u4CliUartSwBufWrite = 0;

/******************************************************************************
* Function prototype
******************************************************************************/
static CHAR _CliGetCharFromBuffer(void);

/******************************************************************************
* Function		: _CliGetCharFromBuffer(void)
* Description	: cli input fucntion
* Parameter		: None
* Return		: UINT8
******************************************************************************/
static CHAR _CliGetCharFromBuffer(void)
{
	UINT32 ui4ReturnIdx;
	ui4ReturnIdx = _u4CliUartSwBufRead;

	if (_u4CliUartSwBufRead != _u4CliUartSwBufWrite)	// Get Char From Buffer
	{
		_u4CliUartSwBufRead = (_u4CliUartSwBufRead + 1) % CLI_UART_INPUT_BUF_SIZE;
		return _aszCliUartSwBuf[ui4ReturnIdx];
	}
	else										// Get Char From UART (H/W)
	{
		_aszCliUartSwBuf[_u4CliUartSwBufWrite] = SerPollGetChar();	// 1st Char
		_u4CliUartSwBufWrite = (_u4CliUartSwBufWrite + 1) % CLI_UART_INPUT_BUF_SIZE;

		while (SerGetCharTimeout(&_aszCliUartSwBuf[_u4CliUartSwBufWrite], CLI_UART_INPUT_BUF_WAIT_TIME) == 1)	// Get all pending Char
		{
			_u4CliUartSwBufWrite = (_u4CliUartSwBufWrite + 1) % CLI_UART_INPUT_BUF_SIZE;

			if (_u4CliUartSwBufWrite == _u4CliUartSwBufRead)	// Buffer Full
			{
				break;
			}
		}

		_u4CliUartSwBufRead = (_u4CliUartSwBufRead + 1) % CLI_UART_INPUT_BUF_SIZE;
		return _aszCliUartSwBuf[ui4ReturnIdx];
	}
}
#endif


/******************************************************************************
* Function		: CLI_Input(void)
* Description	: cli input fucntion
* Parameter		: None
* Return		: None
******************************************************************************/
void CLI_Input(void* pvArg)
{
	INT32 i4ReturnValue;
	UINT32 u4TempIdx = 0;
	CHAR cChar = 0;

	if (pvArg != NULL)
	{
		while (1)
		{}
	}

	for (u4TempIdx = 0; u4TempIdx < CLI_INPUT_BUF_ROW_NUM; u4TempIdx++)
	{
		_aszCliInputBuf[u4TempIdx][0] = ASCII_NULL;
	}

#ifndef __KERNEL__
	PrintChar(ASCII_KEY_CR);
	PrintChar(ASCII_KEY_NL);
#endif /* __KERNEL__ */
#ifdef CC_MINI_DRIVER
	Printf("%s%s>", CLI_PROMPT_STR, _szCliPromptStr);
#else /* CC_MINI_DRIVER */
	Printf("%s%s>", _szMainPrompt, _szCliPromptStr);
#endif /* CC_MINI_DRIVER */

	while (1)
	{
#ifdef CLI_SW_FIFO
		cChar = _CliGetCharFromBuffer();
#else
		cChar = (CHAR) SerPollGetChar();
#endif

		switch (cChar)
		{
#ifndef __KERNEL__
		case ASCII_ENTER:
#else
        case ASCII_KEY_NL:
#endif
			_aszCliInputBuf[_u4CliInputBufRowIdx][_u4CliInputBufIdx] = ASCII_NULL;
			_u4CliInputBufIdx = (_u4CliInputBufIdx + 1) % CLI_INPUT_BUF_SIZE;

#ifndef __KERNEL__
			PrintChar(ASCII_KEY_CR);
			PrintChar(ASCII_KEY_NL);
#endif /* __KERNEL__ */

			// disable UART INT
//			SerIsrDisable();

			i4ReturnValue = CLI_PromptParser(_aszCliInputBuf[_u4CliInputBufRowIdx]);
			if ((i4ReturnValue < CLI_COMMAND_OK) && (i4ReturnValue != CLI_UNKNOWN_CMD))
			{
				Printf("CLI Command Return Value (%d)\n", i4ReturnValue);
			}

			// enable UART INT
//			SerIsrEnable();

			// reset cli buffer
			if (_aszCliInputBuf[_u4CliInputBufRowIdx][0] != ASCII_NULL)
			{
				_u4CliInputBufRowIdx = (_u4CliInputBufRowIdx + 1) % CLI_INPUT_BUF_ROW_NUM;
				_aszCliInputBuf[_u4CliInputBufRowIdx][0] = ASCII_NULL;
			}
			_u4CliInputBufIdx = 0;

			_u4CliInputBufRefRowIdx = _u4CliInputBufRowIdx;

#ifndef __KERNEL__
			PrintChar(ASCII_KEY_CR);
			PrintChar(ASCII_KEY_NL);
#endif /* __KERNEL__ */
#ifdef CC_MINI_DRIVER
	        Printf("%s%s>", CLI_PROMPT_STR, _szCliPromptStr);
#else /* CC_MINI_DRIVER */
			Printf("%s%s>", _szMainPrompt, _szCliPromptStr);
#endif /* CC_MINI_DRIVER */
			break;

		case ASCII_KEY_BS:

			if (_u4CliInputBufIdx > 0)
			{
				PrintChar(ASCII_KEY_BS);
				PrintChar(ASCII_KEY_SPACE);
				PrintChar(ASCII_KEY_BS);

				_u4CliInputBufIdx --;
			}

			break;

		case ASCII_KEY_ESC:

#ifdef CLI_SW_FIFO
			cChar = _CliGetCharFromBuffer();
#else
			cChar = (CHAR) SerPollGetChar();
#endif

			if (cChar == ASCII_KEY_ARROW)
			{

#ifdef CLI_SW_FIFO
				cChar = _CliGetCharFromBuffer();
#else
				cChar = (CHAR) SerPollGetChar();
#endif

				switch (cChar)
				{
				case ASCII_KEY_UP:

					// clear current row
					while (_u4CliInputBufIdx > 0)
					{
						PrintChar(ASCII_KEY_BS);
						PrintChar(ASCII_KEY_SPACE);
						PrintChar(ASCII_KEY_BS);

						_u4CliInputBufIdx--;
					}
					_aszCliInputBuf[_u4CliInputBufRowIdx][0] = ASCII_NULL;

					// load upper reference row
					_u4CliInputBufRefRowIdx = (_u4CliInputBufRefRowIdx - 1) % CLI_INPUT_BUF_ROW_NUM;
					VERIFY(x_memcpy(&_aszCliInputBuf[_u4CliInputBufRowIdx][0],
						&_aszCliInputBuf[_u4CliInputBufRefRowIdx][0],
						CLI_INPUT_BUF_SIZE) == &_aszCliInputBuf[_u4CliInputBufRowIdx][0]);

					_u4CliInputBufIdx = 0;
					while (_aszCliInputBuf[_u4CliInputBufRowIdx][_u4CliInputBufIdx] != ASCII_NULL)
					{
						PrintChar(_aszCliInputBuf[_u4CliInputBufRowIdx][_u4CliInputBufIdx]);
						_u4CliInputBufIdx++;
					}

					break;

				case ASCII_KEY_DOWN:

					// clear current line
					while (_u4CliInputBufIdx > 0)
					{
						PrintChar(ASCII_KEY_BS);
						PrintChar(ASCII_KEY_SPACE);
						PrintChar(ASCII_KEY_BS);

						_u4CliInputBufIdx--;
					}
					_aszCliInputBuf[_u4CliInputBufRowIdx][0] = ASCII_NULL;

					// load lower reference row
					_u4CliInputBufRefRowIdx = (_u4CliInputBufRefRowIdx + 1) % CLI_INPUT_BUF_ROW_NUM;
					VERIFY(x_memcpy(&_aszCliInputBuf[_u4CliInputBufRowIdx][0],
						&_aszCliInputBuf[_u4CliInputBufRefRowIdx][0],
						CLI_INPUT_BUF_SIZE) == &_aszCliInputBuf[_u4CliInputBufRowIdx][0]);

					_u4CliInputBufIdx = 0;
					while (_aszCliInputBuf[_u4CliInputBufRowIdx][_u4CliInputBufIdx] != ASCII_NULL)
					{
						PrintChar(_aszCliInputBuf[_u4CliInputBufRowIdx][_u4CliInputBufIdx]);
						_u4CliInputBufIdx++;
					}

					break;

				case ASCII_KEY_LEFT:

					if (_u4CliInputBufIdx > 0)
					{
						PrintChar(ASCII_KEY_BS);
						PrintChar(ASCII_KEY_SPACE);
						PrintChar(ASCII_KEY_BS);

						_u4CliInputBufIdx --;
					}

					break;

				case ASCII_KEY_RIGHT:
				default:
					break;
				}
			}

			break;

		default:

			if ((cChar >= ASCII_KEY_PRINTABLE_MIN) &&
				(cChar <= ASCII_KEY_PRINTABLE_MAX))
			{
				_aszCliInputBuf[_u4CliInputBufRowIdx][_u4CliInputBufIdx] = cChar;
				_u4CliInputBufIdx = (_u4CliInputBufIdx + 1) % CLI_INPUT_BUF_SIZE;
#ifndef __KERNEL__
				PrintChar(cChar);
#endif /* __KERNEL__ */
			}

			break;
		}
	}
//lint -e{818}
}

/******************************************************************************
* Function		: CLI_Init(void)
* Description	: cli input initialization
* Parameter		: None
* Return		: None
******************************************************************************/
void CLI_Init(void)
{
#ifndef CC_5391_LOADER
    HANDLE_T hThread1;
#endif /* CC_5391_LOADER */

	_u4CliInputBufIdx = 0;

	CLI_CmdTblClear();	// Initialize Command Table
#ifndef CC_MINI_DRIVER
	CLI_AliasInit();		// Initialize Alias Table
#endif /* CC_MINI_DRIVER */

	_szCliPromptStr[0] = ASCII_NULL;

#ifdef CLI_SW_FIFO
	_u4CliUartSwBufRead = 0;
	_u4CliUartSwBufWrite = 0;
#endif

	CLI_ParserSetPromptStr(_szCliPromptStr);

#ifndef CC_5391_LOADER
    // Create CLI thread
    if (x_thread_create(&hThread1, CLI_THREAD_NAME, CLI_STACK_SIZE, CLI_THREAD1_PRIORITY,
        CLI_Input, 0, NULL) != OSR_OK)
    {
        LINT_SAVE_AND_DISABLE
		Printf("\nError: Create CLI thread fail\n",
            _arNullCmdTbl, _arDefaultCmdTbl);
        LINT_RESTORE

	    // Hold the system
	    while (1) {}
    }

	// install default command table
    LINT_SUPPRESS_NEXT_EXPRESSION(534);
	CLI_CmdTblAttach(CLI_GetDefaultCmdTbl());
#endif /* CC_5391_LOADER */

    LINT_SAVE_AND_DISABLE
    Printf("", _arNullCmdTbl, _arDefaultCmdTbl);
    LINT_RESTORE
}


#ifndef CC_MINI_DRIVER
void CLI_SetPrompt(const CHAR* szPrompt)
{
    if (szPrompt == NULL)
    {
        _szMainPrompt = CLI_PROMPT_STR;
    }
    else
    {
        _szMainPrompt = szPrompt;
    }
}
#endif /* CC_MINI_DRIVER */

