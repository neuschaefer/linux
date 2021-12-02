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

/** @file smxd_ecc.c
 *   This file implements the SM/XD slot ECC functions.
 */

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#ifdef CC_FCI_ENABLE

#include "smxd_func.h"

LINT_EXT_HEADER_BEGIN

#include "fci_debug.h"

#include "x_assert.h"

LINT_EXT_HEADER_END

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------
/// To activate ECC function.

//#define SMXD_ECC_SUPPORT

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

//-------------------------------------------------------------------------
/** _SMXD_MakeEcc256
*  This function generates 3 byte ECC for 256 byte data by Software.
*  @param  pu1EccBuf     the location where ECC should be stored.
*  @param  pu4DataBuf     given data.
*  @retval   none.
*/
//-------------------------------------------------------------------------

VOID _SMXD_MakeEcc256(UINT8 *pu1EccBuf, UINT32 *pu4DataBuf)
{

#ifdef SMXD_ECC_SUPPORT

    UINT32 i;

    UINT32 tmp;
    UINT32 uiparity = 0;
    UINT32 parityCol, ecc = 0;
    UINT32 parityCol4321 = 0;
    UINT32 parityCol4343 = 0;
    UINT32 parityCol4242 = 0;
    UINT32 parityColTot = 0;
    UINT32 *Data;
    UINT32 Xorbit = 0;

    ASSERT(pu4DataBuf != NULL);
    ASSERT(pu1EccBuf != NULL);

    Data = pu4DataBuf;

    for (i = 0; i < 8; i++)
    {
        parityCol = *Data++;

        tmp = *Data++;
        parityCol ^= tmp;
        parityCol4242 ^= tmp;
        tmp = *Data++;
        parityCol ^= tmp;
        parityCol4343 ^= tmp;
        tmp = *Data++;
        parityCol ^= tmp;
        parityCol4343 ^= tmp;
        parityCol4242 ^= tmp;
        tmp = *Data++;
        parityCol ^= tmp;
        parityCol4321 ^= tmp;
        tmp = *Data++;
        parityCol ^= tmp;
        parityCol4242 ^= tmp;
        parityCol4321 ^= tmp;
        tmp = *Data++;
        parityCol ^= tmp;
        parityCol4343 ^= tmp;
        parityCol4321 ^= tmp;
        tmp = *Data++;
        parityCol ^= tmp;
        parityCol4242 ^= tmp;
        parityCol4343 ^= tmp;
        parityCol4321 ^= tmp;

        parityColTot ^= parityCol;

        tmp = (parityCol >> 16) ^ parityCol;
        tmp = (tmp >> 8) ^ tmp;
        tmp = (tmp >> 4) ^ tmp;
        tmp = ((tmp >> 2) ^ tmp) & 0x03;
        if ((tmp == 0x01) || (tmp == 0x02))
        {
            uiparity ^= i;
            Xorbit ^= 0x01;
        }
    }

    tmp = (parityCol4321 >> 16) ^ parityCol4321;
    tmp = (tmp << 8) ^ tmp;
    tmp = (tmp >> 4) ^ tmp;
    tmp = (tmp >> 2) ^ tmp;
    ecc |= ((tmp << 1) ^ tmp) & 0x200; // p128

    tmp = (parityCol4343 >> 16) ^ parityCol4343;
    tmp = (tmp >> 8) ^ tmp;
    tmp = (tmp << 4) ^ tmp;
    tmp = (tmp << 2) ^ tmp;
    ecc |= ((tmp << 1) ^ tmp) & 0x80; // p64

    tmp = (parityCol4242 >> 16) ^ parityCol4242;
    tmp = (tmp >> 8) ^ tmp;
    tmp = (tmp << 4) ^ tmp;
    tmp = (tmp >> 2) ^ tmp;
    ecc |= ((tmp << 1) ^ tmp) & 0x20; // p32

    tmp = parityColTot & 0xFFFF0000;
    tmp = tmp >> 16;
    tmp = (tmp >> 8) ^ tmp;
    tmp = (tmp >> 4) ^ tmp;
    tmp = (tmp << 2) ^ tmp;
    ecc |= ((tmp << 1) ^ tmp) & 0x08; // p16

    tmp = parityColTot & 0xFF00FF00;
    tmp = (tmp >> 16) ^ tmp;
    tmp = (tmp >> 8);
    tmp = (tmp >> 4) ^ tmp;
    tmp = (tmp >> 2) ^ tmp;
    ecc |= ((tmp << 1) ^ tmp) & 0x02; // p8

    tmp = parityColTot & 0xF0F0F0F0;
    tmp = (tmp << 16) ^ tmp;
    tmp = (tmp >> 8) ^ tmp;
    tmp = (tmp << 2) ^ tmp;
    ecc |= ((tmp << 1) ^ tmp) & 0x800000; // p4

    tmp = parityColTot & 0xCCCCCCCC;
    tmp = (tmp << 16) ^ tmp;
    tmp = (tmp >> 8) ^ tmp;
    tmp = (tmp << 4) ^ tmp;
    tmp = (tmp >> 2);
    ecc |= ((tmp << 1) ^ tmp) & 0x200000; // p2

    tmp = parityColTot & 0xAAAAAAAA;
    tmp = (tmp << 16) ^ tmp;
    tmp = (tmp >> 8) ^ tmp;
    tmp = (tmp >> 4) ^ tmp;
    tmp = (tmp << 2) ^ tmp;
    ecc |= (tmp & 0x80000);         // p1

    ecc |= (uiparity & 0x01) << 11; // parit256_1
    ecc |= (uiparity & 0x02) << 12; // parit512_1
    ecc |= (uiparity & 0x04) << 13; // parit1024_1

    if (Xorbit)
    {
        ecc |= (ecc ^ 0x00A8AAAA) >> 1;
    }
    else
    {
        ecc |= (ecc >> 1);
    }

    ecc = ~ecc;
    *(pu1EccBuf + 2) = (UINT8)(ecc >> 16);
    *(pu1EccBuf + 1) = (UINT8)(ecc >> 8);
    *(pu1EccBuf + 0) = (UINT8)(ecc);
#else
    ASSERT(pu1EccBuf);
    UNUSED(pu4DataBuf);
    
    *(pu1EccBuf + 2) = 0xFF;
    *(pu1EccBuf + 1) = 0xFF;
    *(pu1EccBuf + 0) = 0xFF;
#endif

}

//-------------------------------------------------------------------------
/** _SMXD_ChkEcc
* This function compares two ECCs and indicates if there is an error.
*  @param  iEccdata1     		one ECC to be compared.
*  @param  iEccdata2     		the other ECC to be compared.
*  @param  pPagedata		content of data page.
*  @param  pOffset			where the error occurred.
*  @param  pCorrected		correct data.
*  @retval ECC_NO_ERROR   	Success.
*  @retval ECC_CORRECTABLE_ERROR  	Fail.
*/
//-------------------------------------------------------------------------
#if 0
SMXDECCDIFF_T _SMXD_ChkEcc(UINT8 *iEccdata1, UINT8 *iEccdata2, const UINT8 *pPagedata, UINT32 pOffset, UINT8 pCorrected)
{
    UINT32 iCompecc = 0, iEccsum = 0;

    UINT32 iFindbyte = 0;
    UINT32 iIndex;
    UINT32 nT1 = 0, nT2 = 0;
    UINT8 iNewvalue;
    UINT8 iFindbit = 0;

    UINT8 *pEcc1;
    UINT8 *pEcc2;

    ASSERT(iEccdata1 != NULL);
    ASSERT(iEccdata2 != NULL);
    ASSERT(pPagedata != NULL);

    pEcc1 = (UINT8 *)iEccdata1;
    pEcc2 = (UINT8 *)iEccdata2;

    for (iIndex = 0; iIndex < 2; iIndex++)
    {
        nT1 ^= ((( *pEcc1) >> iIndex) & 0x01);
        nT2 ^= ((( *pEcc2) >> iIndex) & 0x01);
    }

    for (iIndex = 0; iIndex < 3; iIndex++)
    {
        iCompecc |= ((UINT32)((~(*pEcc1++ )) ^ (~(*pEcc2++ ))) << (iIndex *8));
    }

    for (iIndex = 0; iIndex < 24; iIndex++)
    {
        iEccsum += ((iCompecc >> iIndex) & 0x01);
    }

    switch (iEccsum)
    {
    case 0:
        LOG(1,"RESULT : no error\n");
        return ECC_NO_ERROR;

    case 1:
        LOG(1,"RESULT : ECC 1 bit error\n");
        return ECC_ECC_ERROR;

    case 11:
        if (nT1 != nT2)
        {
            iFindbyte = ((((iCompecc >> 15) & 1) << 7) + (((iCompecc >> 13) & 1) << 6) + (((iCompecc >> 11) & 1) << 5) + (((iCompecc >> 9) & 1) << 4) + (((iCompecc >> 7) & 1) << 3) + (((iCompecc >> 5) & 1) << 2) + (((iCompecc >> 3) & 1) << 1) + ((iCompecc >> 1) & 1));

            iFindbit = (UINT8)((((iCompecc >> 23) & 1) << 2) + (((iCompecc >> 21) & 1) << 1) + ((iCompecc >> 19) & 1));

            iNewvalue = (UINT8)(pPagedata[iFindbyte] ^ (1 << iFindbit));
            LOG(1,"iCompecc = %d\n", iCompecc);
            LOG(1,"RESULT : one bit error\r\n");
            LOG(1,"byte = %d, bit = %d\r\n", iFindbyte, iFindbit);
            LOG(1,"corrupted = %x, corrected = %x\r\n", pPagedata[iFindbyte], iNewvalue);

            if (pOffset != NULL)
            {
                pOffset = iFindbyte;
            }

            if (pCorrected != NULL)
            {
                pCorrected = iNewvalue;
            }
            return ECC_CORRECTABLE_ERROR;
        }
        else
        {
            return ECC_UNCORRECTABLE_ERROR;
        }

    default:
        LOG(1,"RESULT : unrecoverable error\n");
        return ECC_UNCORRECTABLE_ERROR;
    }
}
#endif
#endif
