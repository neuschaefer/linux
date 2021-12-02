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
 *
 * Description:
 *         The XMODEM protocol
 *
 *---------------------------------------------------------------------------*/

#include "x_os.h"
#include "x_timer.h"
#include "x_serial.h"
#include "x_crc16.h"
#include "xmodem.h"

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

// Protocal characters
#define SOH                             0x01
#define STX                             0x02
#define EOT                             0x04
#define ACK                             0x06
#define NAK                             0x15
#define CAN                             0x18
#define CTRLZ                           0x1A

#define MAX_RETRANS                     30

#define MAX_RETRY                       20

#define FLUSH_INPUT_DELAY               100         // 100 ms

// 1024 for XModem 1k + 3 head chars + 2 crc + nul
#define BUF_SIZE                        1030

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------


/*----------------------------------------------------------------------------
 * Function:
 *
 * Description:
 *
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: -
 *---------------------------------------------------------------------------*/
static INT32 _InByte(UINT16 u2Timeout)
{
    HAL_TIME_T dt, t0, t1;
    UINT32 u4Diff;

    HAL_GetTime(&t0);

    while (1)
    {
        UCHAR uc;

        if (SerInByte(&uc))
        {
            return (INT32)uc;
        }

        HAL_GetTime(&t1);
        HAL_GetDeltaTime(&dt, &t0, &t1);
        u4Diff = dt.u4Seconds * 1000 + dt.u4Micros / 1000;
        if (u4Diff >= (UINT32)u2Timeout)
        {
            break;
        }
    }

    return -1;
}


/*----------------------------------------------------------------------------
 * Function:
 *
 * Description:
 *
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: -
 *---------------------------------------------------------------------------*/
static void _OutByte(INT32 c)
{
    SerPollPutChar((UINT8)c);
}


/*----------------------------------------------------------------------------
 * Function:
 *
 * Description:
 *
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: -
 *---------------------------------------------------------------------------*/
static BOOL _Check(BOOL fgCrc, const UCHAR* pucBuf, INT32 i4Size)
{
	if (fgCrc)
	{
		UINT16 u2Crc = crc16_ccitt(pucBuf, i4Size);
		UINT16 u2Tcrc = (pucBuf[i4Size] << 8) + pucBuf[i4Size + 1];
		if (u2Crc == u2Tcrc)
		{
			return TRUE;
		}
	}
	else
	{
		INT32 i;
		UCHAR cks;

		cks = 0;
		for (i = 0; i < i4Size; ++i)
		{
			cks += pucBuf[i];
		}
		if (cks == pucBuf[i4Size])
		{
		    return TRUE;
		}
	}

	return FALSE;
}


/*----------------------------------------------------------------------------
 * Function:
 *
 * Description:
 *
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: -
 *---------------------------------------------------------------------------*/
static void _FlushInput(void)
{
    while (_InByte(FLUSH_INPUT_DELAY) >= 0)
    {
    }
}

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------


/*----------------------------------------------------------------------------
 * Function:
 *
 * Description:
 *
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: -
 *---------------------------------------------------------------------------*/
INT32 XMODEM_Receive(UCHAR* pucDest, INT32 i4DestSize)
{
	static UCHAR _aucXbuff[BUF_SIZE];
	UCHAR ucTry;
	UINT8 u1PacketNum;
	INT32 i, c, i4Len, i4BufSize, i4Retry, i4Retrans;
	BOOL fgCrc = FALSE;

    ucTry = 'C';
    u1PacketNum = 1;
    i4Len = 0;
    i4Retrans = MAX_RETRANS;

	while (1)
	{
	    UCHAR* p;

		for ( i4Retry = 0; i4Retry < MAX_RETRY; ++i4Retry)
		{
			if (ucTry != 0)
			{
			    _OutByte(ucTry);
			}

			if ((c = _InByte(2000)) >= 0)
			{
				switch (c)
				{
				case SOH:
					i4BufSize = 128;
					goto start_recv;

				case STX:
					i4BufSize = 1024;
					goto start_recv;

				case EOT:
					_FlushInput();
					_OutByte(ACK);
					return i4Len; /* normal end */

				case CAN:
					if ((c = _InByte(1000)) == CAN)
					{
						_FlushInput();
						_OutByte(ACK);
						return XERR_CANCEL;
					}
					break;

				default:
					break;
				}
			}
		}
		if (ucTry == 'C')
		{
		    ucTry = NAK;
		    continue;
		}
		_FlushInput();
		_OutByte(CAN);
		_OutByte(CAN);
		_OutByte(CAN);
		return XERR_SYNC;

start_recv:

		if (ucTry == 'C')
		{
		    fgCrc = TRUE;
		}

		ucTry = 0;
		p = _aucXbuff;
		*p++ = c;

		for (i = 0;  i < (i4BufSize + (fgCrc ? 1 : 0 ) + 3); i++)
		{
			if ((c = _InByte(2000)) < 0)
			{
			    goto reject;
			}
			*p++ = c;
		}

		if ((_aucXbuff[1] == (UCHAR)(~_aucXbuff[2])) &&
		    (_aucXbuff[1] == u1PacketNum || _aucXbuff[1] == (UCHAR)u1PacketNum - 1)
		    && _Check(fgCrc, &_aucXbuff[3], i4BufSize))
	    {
			if (_aucXbuff[1] == u1PacketNum)
			{
				INT32 i4Count = i4DestSize - i4Len;

				if (i4Count > i4BufSize)
				{
				    i4Count = i4BufSize;
				}
				if (i4Count > 0)
				{
					x_memcpy (&pucDest[i4Len], &_aucXbuff[3], i4Count);
					i4Len += i4Count;
				}

				++u1PacketNum;
				i4Retrans = MAX_RETRANS + 1;
			}
			if (--i4Retrans <= 0)
			{
				_FlushInput();
				_OutByte(CAN);
				_OutByte(CAN);
				_OutByte(CAN);

				return XERR_TOO_MANY_RETRY;
			}

			_OutByte(ACK);
			continue;
		}

reject:

		_FlushInput();
		_OutByte(NAK);
	}

	// Never goes here
}


/*----------------------------------------------------------------------------
 * Function:
 *
 * Description:
 *
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: -
 *---------------------------------------------------------------------------*/
const CHAR* XMODEM_GetErrorString(INT32 i4Code)
{
    const CHAR* szErr = "Unknown error";

    switch (i4Code)
    {
    case XERR_CANCEL:
        szErr = "Canceled by remote";
        break;

    case XERR_SYNC:
        szErr = "Sync error";
        break;

    case XERR_TOO_MANY_RETRY:
        szErr = "Too many retry";
        break;

    default:
        break;
    }

    return szErr;
}

