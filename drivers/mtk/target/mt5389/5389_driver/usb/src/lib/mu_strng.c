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
/*
 * Micro-string library
 * $Revision: #1 $
 */

#ifdef MUSB_DIAG
#include "mu_strng.h"

static const CHAR MGC_CapOffset = 'a' - 'A';

static CHAR MGC_CharCompare(CHAR c1, CHAR c2, uint8_t bCaseSensitive)
{
    CHAR cc1 = c1;

    CHAR cc2 = c2;

    if (!bCaseSensitive)
    {
        if ((c1 >= 'a') && (c1 <= 'z'))
        {
            cc1 -= MGC_CapOffset;
        }
        if ((c2 >= 'a') && (c2 <= 'z'))
        {
            cc2 -= MGC_CapOffset;
        }
    }

    return (cc1 - cc2);
}

/*
* implementation
*/
CHAR MUSB_StringCompare(const CHAR *pString1, const CHAR *pString2, uint8_t bCaseSensitive)
{
    CHAR bResult = 0;

    const CHAR *p1 = pString1;
    const CHAR *p2 = pString2;

    /* sanity */
    if (!pString1 || !pString2)
    {
        return 1;
    }

    /* compare CHARacters until mismatch or at least one string's end */
    while ((0 == bResult) && *p1 && *p2)
    {
        bResult = MGC_CharCompare(*p1, *p2, bCaseSensitive);

        p1++;
        p2++;
    }

    if (0 == bResult)
    {
        /* strings matched until one's end; indicate different-sized strings */
        if (*p1 && !*p2)
        {
            /* first string is longer */
            bResult = 1;
        }
        else if (!*p1 && *p2)
        {
            /* second string is longer */
            bResult = -1;
        }
    }

    return bResult;
}

/*
* implementation
*/
uint8_t MUSB_StringConcat(CHAR *pResult, uint16_t wResultLength, const CHAR *pConcat)
{
    const CHAR *pAppend = pConcat;

    uint16_t wIndex = 0;
    uint8_t bResult = FALSE;

    /* sanity */
    if (!pResult || !pConcat)
    {
        return bResult;
    }

    while (pResult[wIndex] && (wIndex < wResultLength))
    {
        wIndex++;
    }

    if (wIndex < wResultLength)
    {
        while (*pAppend && (wIndex < wResultLength))
        {
            pResult[wIndex++ ] = *pAppend;
            pAppend++;
        }
        if (wIndex < wResultLength)
        {
            pResult[wIndex] = (CHAR)0;
            bResult = TRUE;
        }
        else
        {
            pResult[wIndex - 1] = (CHAR)0;
        }
    }

    return bResult;
}

/*
* implementation
*/
const CHAR *MUSB_StringFind(const CHAR *pString, const CHAR *pPattern, uint8_t bCaseSensitive)
{
    const CHAR *pResult = NULL;

    const CHAR *pSearch = pString;
    const CHAR *pFind = pPattern;
    int32_t dwStringLength = MUSB_StringLength(pString);
    uint16_t wPatternLength = MUSB_StringLength(pPattern);
    uint16_t wFoundLength = 0;

    /* sanity */
    if (!pString || !pPattern)
    {
        return NULL;
    }

    /* find first candidate and prune if length impossible */
    while ((dwStringLength >= (int32_t)wPatternLength) && (wFoundLength < wPatternLength))
    {
        wFoundLength = 0;

        while ((dwStringLength >= (int32_t)
                                      wPatternLength) && (0 != MGC_CharCompare(*pSearch, *pFind,
                                                                               bCaseSensitive)))
        {
            pSearch++;
            dwStringLength--;
        }

        pResult = pSearch;

        if (dwStringLength >= (int32_t)wPatternLength)
        {
            while ((wFoundLength < wPatternLength) && (0 == MGC_CharCompare(
                                                                pSearch[wFoundLength],
                                                                pPattern[wFoundLength],
                                                                bCaseSensitive)))
            {
                wFoundLength++;
            }
        }
        dwStringLength--;
    }

    return (wFoundLength == wPatternLength) ? pResult : NULL;
}

/*
* implementation
*/
uint8_t MUSB_Stringize(CHAR *pResult, uint16_t wResultLength, uint32_t dwNumber, uint8_t bBase,
                       uint8_t bJustification)
{
    uint8_t bDigit;

    uint16_t wIndex;
    uint16_t wCount;
    uint16_t wZeroCount = 0;
    uint8_t bOk = TRUE;
    uint32_t dwValue = dwNumber;

    if (!pResult || !wResultLength || (wResultLength <= bJustification))
    {
        bOk = FALSE;
    }

    switch (bBase)
    {
    case 2:
    case 8:
    case 10:
    case 16:
        break;

    default:
        bOk = FALSE;
    }

    if (bOk)
    {
        if (!dwNumber)
        {
            /* trivial case */
            for (wIndex = 0; wIndex < MUSB_MAX(bJustification, 1); wIndex++)
            {
                pResult[wIndex] = '0';
            }
            pResult[wIndex] = (CHAR)0;
        }
        else
        {
            /* count digits */
            wCount = 0;

            while (dwValue)
            {
                dwValue /= bBase;
                wCount++;
            }

            /* justify now if needed */
            if (bJustification && (bJustification >= wCount))
            {
                wZeroCount = bJustification - wCount;
                for (wIndex = 0; wIndex < wZeroCount; wIndex++)
                {
                    pResult[wIndex] = '0';
                }
            }

            /* fail if overflow would result */
            if ((wCount + wZeroCount) >= wResultLength)
            {
                bOk = FALSE;
            }
            else
            {
                /* do the real job */
                wIndex = wCount + wZeroCount;

                pResult[wIndex] = (CHAR)0;
                dwValue = dwNumber;
                while (dwValue)
                {
                    bDigit = (uint8_t)(dwValue %bBase);

                    if (bDigit > 9)
                    {
                        pResult[--wIndex] = ('A' + bDigit) - 10;
                    }
                    else
                    {
                        pResult[--wIndex] = '0' + bDigit;
                    }
                    dwValue /= bBase;
                }
            }
        }
    }

    return bOk;
}

/*
* implementation
*/
uint16_t MUSB_StringLength(const CHAR *pString)
{
    uint16_t wResult = 0;

    const CHAR *p = pString;

    if (p)
    {
        while (*p && (wResult < 0xffff))
        {
            p++;
            wResult++;
        }
    }

    return wResult;
}

static uint8_t MGC_GetDigitValue(const CHAR bDigit, uint8_t *pbResult, uint8_t bBase)
{
    uint8_t bResult = TRUE;

    uint8_t bValue = bDigit - '0';

    if (bDigit < '0')
    {
        bResult = FALSE;
    }

    if (16 == bBase)
    {
        if (bDigit >= 'a')
        {
            bValue = (bDigit - 'a') + 10;
        }
        else if (bDigit >= 'A')
        {
            bValue = (bDigit - 'A') + 10;
        }
    }

    if (bValue >= bBase)
    {
        bResult = FALSE;
    }

    *pbResult = bValue;
    return bResult;
}

/*
* implementation
*/
int32_t MUSB_StringParse(const CHAR *pString, const CHAR **pEnd, uint8_t bBase)
{
    const CHAR *pTest;

    uint8_t bOk;
    uint8_t bDigit;
    uint8_t bIsNegative = FALSE;
    const CHAR *pDigit = pString;
    int32_t dwResult = 0;
    uint8_t bRealBase = bBase;

    /* sanity */
    if (!pString)
    {
        return 0;
    }

    if (pEnd)
    {
        *pEnd = pString;
    }

    if ('-' == *pDigit)
    {
        bIsNegative = TRUE;
        pDigit++;
    }

    if (!bRealBase)
    {
        pTest = MUSB_StringFind(pDigit, "0x", FALSE);
        if (pTest == pDigit)
        {
            bRealBase = 16;
            pDigit += 2;
        }
        else if ('0' == *pDigit)
        {
            bRealBase = 8;
            pDigit++;
        }
        else if ('%' == *pDigit)
        {
            bRealBase = 2;
            pDigit++;
        }
        else
        {
            bRealBase = 10;
        }
    }

    bOk = MGC_GetDigitValue(*pDigit, &bDigit, bRealBase);

    while (bOk)
    {
        dwResult *= bRealBase;

        dwResult += bDigit;
        bOk = MGC_GetDigitValue(*++pDigit, &bDigit, bRealBase);
    }

    if (pEnd)
    {
        *pEnd = pDigit;
    }

    return bIsNegative ? (0 - dwResult) : dwResult;
}
#endif
