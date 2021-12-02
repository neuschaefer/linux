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
 * $RCSfile: efuse.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: scramble given data buffer
 *---------------------------------------------------------------------------*/
 /*
#include "os/inc/x_os.h"
#include "dbg/x_dbg.h"
*/
#include "x_os.h"
#include "x_dbg.h"
#include "x_dlm.h"

/* This file provided by Joshua_Huang. */
/* Robert: Changed
 * 1. To support varient length
 * 2. To support CBC mode (original mode is ECB mode)
 * 
*/
UINT32 const _au4Efunc[24] = {
    0, 15, 14, 13, 12, 11,
    12, 11, 10, 9, 8, 7,
    8, 7, 6, 5, 4, 3, 
    4, 3, 2, 1, 0, 15,
};

static UINT32 _Efuse_Efunc(UINT16 u2Data)
{
    INT32 i;
    UINT32 u4Val;

    u4Val = 0;
    for (i=0; i<24; i++)
    {
        if ((u2Data >> _au4Efunc[i]) & 1)
        {
            u4Val |= (1 << i);
        }
    }
    return u4Val;
}

UINT32 const _aSBox[4][64] = {
    {
        13,  6,  0, 10, 14,  3, 11,  5, /*  0~ 7 */
        7,  1,  9,  4,  2,  8, 12, 15, /*  8~15 */
        0,  5, 10,  3,  7,  9, 12, 15, /* 16~23 */
        11,  2,  6, 13,  8, 14,  1,  4, /* 24~31 */
        8,  3,  5,  9, 11, 12,  6, 10, /* 32~39 */
        1, 13,  2, 14,  4,  7, 15,  0, /* 40~47 */
        7,  0,  9,  5, 12,  6, 10,  3, /* 48~55 */
        8, 11, 15,  2,  1, 13,  4, 14, /* 56~63 */
    },
    {
        9, 14,  5,  0, 12,  7,  6, 11,
        2,  4, 15,  3,  1, 10,  8, 13,
        15,  2,  3,  9,  6, 12,  8,  5,
        1, 13,  4, 10, 11,  7, 14,  0,
        5, 11,  9,  6, 10,  1,  0, 12,
        14,  8,  2, 15,  7,  4, 13,  3,
        10,  5,  0, 12, 13,  2,  7,  9,
        4,  3, 11,  6, 14,  8,  1, 15,
    },
    {
        12,  2,  5, 11,  3, 14, 15,  4,
        7,  8,  9,  6,  0, 13, 10,  1,
        7, 14, 10,  5, 12,  2,  1, 11,
        0,  3, 15,  8,  9,  4,  6, 13,
        1, 15, 11, 12, 14,  5,  8,  2,
        10,  6,  4,  3,  9,  0,  7, 13,
        8,  2,  4, 11,  7, 12, 13,  1,
        5, 15,  3,  6, 14,  9,  0, 10,
    },
    {
        14,  2,  7, 12, 11,  5,  4,  9,
        8, 13,  1, 10,  6,  0, 15,  3,
        4,  8,  2,  5, 14,  3,  1, 15,
        13,  7, 11, 12,  0,  9,  6, 10,
        9, 14, 10,  1, 12,  2,  7,  4,
        3,  0, 15,  6,  5, 11,  8, 13,
        15,  4, 12, 11,  5,  8,  2,  1, 
        10,  9,  6,  0,  3, 14, 13,  7,
    },
};

static UINT16 _Efuse_SBox(UINT32 u4Data)
{
    INT32 i;
    UINT16 u2Val;

    u2Val = 0;
    for (i=0; i<4; i++)
    {
        u2Val |= (_aSBox[i][((u4Data >> (i*6)) & 0x3f)] << (i*4));
    }

    return u2Val;
}

UINT32 const _au4PSwap[16] = {
    12, 8, 5, 0, 10, 2, 14, 7, 4, 9, 1, 13, 3, 6, 11, 15,
};

static UINT16 _Efuse_PSwap(UINT16 u2Data)
{
    UINT16 u2Val;
    INT32 i;

    u2Val = 0;
    for (i=0; i<16; i++)
    {
        if (u2Data & (1 << i))
        {
            u2Val |= (1 << _au4PSwap[i]);
        }
    }
    return u2Val;
}

static UINT16 _Efuse_FofX(UINT16 u2Data, UINT16 u2Key)
{
    UINT32 u4EData, u4EKey;
    UINT16 u2Val;

    u4EData = _Efuse_Efunc(u2Data);
    u4EKey = _Efuse_Efunc(u2Key);
    u2Val = _Efuse_SBox((u4EData ^ u4EKey));
    return _Efuse_PSwap(u2Val);
}

#if 0 /* ORIGINAL_CODE_FROM_Joshua_Huang */
static UINT32 _Efuse_SoftEncrypt(UINT32 u4Data, UINT32 u4Key)
{
    UINT16 u2DataL, u2DataR, u2KeyL, u2KeyR, u2ValR, u2ValL, u2Tmp;
    UINT32 u4Val;

    u2DataL = (UINT16)(u4Data >> 16);
    u2DataR = (UINT16)(u4Data & 0x0ffff);
    u2KeyL = (UINT16)(u4Key >> 16);
    u2KeyR = (UINT16)(u4Key & 0x0ffff);

    // Swap? IC use Swap Key XXX
    u2Tmp = u2KeyL; u2KeyL = u2KeyR; u2KeyR = u2Tmp;

    u2ValR = u2DataL ^ (_Efuse_FofX(u2DataR, u2KeyR));
    u2ValL = u2DataR ^ (_Efuse_FofX(u2ValR, u2KeyL));
    u4Val = ((UINT32)u2ValL << 16) | (UINT32)u2ValR;
    return u4Val;
}

static UINT32 _Efuse_SoftDecrypt(UINT32 u4Data, UINT32 u4Key)
{
    UINT16 u2DataL, u2DataR, u2KeyL, u2KeyR, u2ValR, u2ValL, u2Tmp;
    UINT32 u4Val;

    u2DataL = (UINT16)(u4Data >> 16);
    u2DataR = (UINT16)(u4Data & 0x0ffff);
    u2KeyL = (UINT16)(u4Key >> 16);
    u2KeyR = (UINT16)(u4Key & 0x0ffff);

    // Swap? IC use Swap Key XXX
    u2Tmp = u2KeyL; u2KeyL = u2KeyR; u2KeyR = u2Tmp;

    u2ValR = u2DataL ^ (_Efuse_FofX(u2DataR, u2KeyL));
    u2ValL = u2DataR ^ (_Efuse_FofX(u2ValR, u2KeyR));
    u4Val = ((UINT32)u2ValL << 16) | (UINT32)u2ValR;
    return u4Val;
}
#endif


/*-----------------------------------------------------------------------------
 * Name: dlm_encrypt_efuse
 *
 * Description: 
 *
 * Inputs:  
 * Outputs: 
 * Returns: 
 ----------------------------------------------------------------------------*/
VOID dlm_encrypt_efuse(UCHAR *puac_buf, UINT32 ui4_buf_size, UINT32 u4Key, UINT32 *pui4_iv)
{
    UINT16 u2DataL, u2DataR, u2KeyL, u2KeyR, u2ValR, u2ValL;
    UINT32 u4Val;

    UINT32 u4Data;
    UCHAR  *puac_tmp = puac_buf;
    INT32  i4_size = (INT32) sizeof(UINT32);
    UCHAR  uc_xor = 0x3d;

    u2KeyL = (UINT16)(u4Key >> 16);
    u2KeyR = (UINT16)(u4Key & 0x0ffff);

    /* Media Tek Inc. DTV Internal cypher algo */
    while (ui4_buf_size  >= sizeof(UINT32))
    {
        /* 
            in embedded system, most chip need align with 4 bytes, the damned ARM also has same problem,
            the following code is to prevent BUS error when puac_tmp is not start in align address.
            >>>> u4Data = *((UINT32 *)puac_tmp); world is not so perfect
        */
        u4Data = (UINT32) (*(puac_tmp+3) * 256*256*256 + *(puac_tmp+2) * 256 * 256 + *(puac_tmp+1) * 256 + *(puac_tmp));

        u4Data ^= *pui4_iv;

        u2DataL = (UINT16)(u4Data >> 16);
        u2DataR = (UINT16)(u4Data & 0x0ffff);

        u2ValR = u2DataL ^ (_Efuse_FofX(u2DataR, u2KeyR));
        u2ValL = u2DataR ^ (_Efuse_FofX(u2ValR, u2KeyL));
        u4Val = ((UINT32)u2ValL << 16) | (UINT32)u2ValR;
        x_memcpy(puac_tmp, (CHAR *) &u4Val, 4); 
        puac_tmp += i4_size; 
        ui4_buf_size -= i4_size;
        *pui4_iv = u4Val;
    }

    /* Remainder, simply use xor to scramble it */
    while (ui4_buf_size)
    {
        *puac_tmp++ ^= uc_xor;
        ui4_buf_size --;
    }    
}


/*-----------------------------------------------------------------------------
 * Name: dlm_decrypt_efuse
 *
 * Description: 
 *
 * Inputs:  
 * Outputs: 
 * Returns: 
 ----------------------------------------------------------------------------*/
VOID dlm_decrypt_efuse(UCHAR *puac_buf, UINT32 ui4_buf_size, UINT32 u4Key, UINT32 *pui4_iv)
{
    UINT16 u2DataL, u2DataR, u2KeyL, u2KeyR, u2ValR, u2ValL;
    UINT32 u4Val;

    UINT32 u4Data;
    UCHAR  *puac_tmp = puac_buf;
    INT32  i4_size = (INT32) sizeof(UINT32);
    UCHAR  uc_xor = 0x3d;

    u2KeyL = (UINT16)(u4Key >> 16);
    u2KeyR = (UINT16)(u4Key & 0x0ffff);

    /* Media Tek Inc. DTV Internal cypher algo */
    while (ui4_buf_size >= sizeof(UINT32))
    {
        /* 
            in embedded system, most chip need align with 4 bytes, the damned ARM also has same problem,
            the following code is to prevent BUS error when puac_tmp is not start in align address.
            >>>> u4Data = *((UINT32 *)puac_tmp); world is not so perfect
        */
        u4Data = (UINT32) (*(puac_tmp+3) * 256*256*256 + *(puac_tmp+2) * 256 * 256 + *(puac_tmp+1) * 256 + *(puac_tmp));

        u2DataL = (UINT16)(u4Data >> 16);
        u2DataR = (UINT16)(u4Data & 0x0ffff);

        u2ValR = u2DataL ^ (_Efuse_FofX(u2DataR, u2KeyL));
        u2ValL = u2DataR ^ (_Efuse_FofX(u2ValR, u2KeyR));
        u4Val = ((UINT32)u2ValL << 16) | (UINT32)u2ValR;

        u4Val ^= *pui4_iv;
        x_memcpy(puac_tmp, (CHAR *) &u4Val, 4); 
        puac_tmp += i4_size; 
        ui4_buf_size -= i4_size;
        *pui4_iv = u4Data;
    }

    /* Remainder, simply use xor to scramble it */
    while (ui4_buf_size)
    {
        *puac_tmp++ ^= uc_xor;
        ui4_buf_size --;
    }        
}

