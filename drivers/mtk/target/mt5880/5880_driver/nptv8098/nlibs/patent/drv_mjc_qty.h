/*----------------------------------------------------------------------------*
 * No Warranty                                                                *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MTK with respect to any MTK *
 * Deliverables or any use thereof, and MTK Deliverables are provided on an   *
 * "AS IS" basis.  MTK hereby expressly disclaims all such warranties,        *
 * including any implied warranties of merchantability, non-infringement and  *
 * fitness for a particular purpose and any warranties arising out of course  *
 * of performance, course of dealing or usage of trade.  Parties further      *
 * acknowledge that Company may, either presently and/or in the future,       *
 * instruct MTK to assist it in the development and the implementation, in    *
 * accordance with Company's designs, of certain softwares relating to        *
 * Company's product(s) (the "Services").  Except as may be otherwise agreed  *
 * to in writing, no warranties of any kind, whether express or implied, are  *
 * given by MTK with respect to the Services provided, and the Services are   *
 * provided on an "AS IS" basis.  Company further acknowledges that the       *
 * Services may contain errors, that testing is important and Company is      *
 * solely responsible for fully testing the Services and/or derivatives       *
 * thereof before they are used, sublicensed or distributed.  Should there be *
 * any third party action brought against MTK, arising out of or relating to  *
 * the Services, Company agree to fully indemnify and hold MTK harmless.      *
 * If the parties mutually agree to enter into or continue a business         *
 * relationship or other arrangement, the terms and conditions set forth      *
 * hereunder shall remain effective and, unless explicitly stated otherwise,  *
 * shall prevail in the event of a conflict in the terms in any agreements    *
 * entered into between the parties.                                          *
 *----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Copyright (c) 2005, MediaTek, Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *-----------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: drv_mjc_qty.h $
 * $Revision: #1 $
 *
 *----------------------------------------------------------------------------*/

#ifndef _DRV_MJC_QTY_H_
#define _DRV_MJC_QTY_H_

/*----------------------------------------------------------------------------*
 * Header Files
 *----------------------------------------------------------------------------*/
#include "typedef.h"
#include "video_def.h"

/*----------------------------------------------------------------------------*
 * Fallback Curve
 *----------------------------------------------------------------------------*/
UINT8 arMJCQuantMap[2][MJC_FB_QUANT_TBL_NUM] =
{
    // VDO
    {
        0x00, 0x02, 0x04, 0x08, 0x0C, 0x10, 0x16, 0x1C,
        0x24, 0x2C, 0x32, 0x36, 0x3A, 0x3C, 0x3E, MAX_FBCK_LVL
    },
    // FILM
    {
        0x00, 0x02, 0x04, 0x08, 0x0C, 0x10, 0x16, 0x1C,
        0x24, 0x2C, 0x32, 0x36, 0x3A, 0x3C, 0x3E, MAX_FBCK_LVL
    }
};

UINT8 arMJCQuantMap7[MJC_FB_QUANT_TBL_NUM] =
{
    0, 0, 0, 16, 16, 16, 16, 32, 32, 32, 32, 32, 64, 64, 64, MAX_FBCK_LVL
};


UINT16 arMJCFbQuantTbl[MJC_FB_NUM][MJC_FB_QUANT_TBL_NUM] =
{
    // MJC_FB_NRMVSI
    {
        0x0000, 0x012C, 0x01FE, 0x030C, 0x04B0, 0x0744, 0x0A20, 0x0D38,								
        0x10C2, 0x1524, 0x19DA, 0x1C20, 0x1E78, 0x20D0, 0x2454, 0x27D8,								
    },
    // MJC_FB_BDRVSI
    {
        0x0000, 0x0044, 0x0088, 0x0110, 0x0198, 0x0220, 0x0330, 0x03FC,
        0x04C8, 0x05D8, 0x06A4, 0x072C, 0x07B4, 0x07F8, 0x083C, 0x1200
    },
    // MJC_FB_MVERR
    {
        0x0000, 0x023D, 0x0483, 0x0906, 0x0D89, 0x120C, 0x2655, 0x289B,
        0x2D1E, 0x31A1, 0x33DE, 0x3CE4, 0x4167, 0x43AD, 0x45EA, 0x4830
    },
    // MJC_FB_BADMOTCNT
    {
        0x0000, 0x0068, 0x00D0, 0x01A0, 0x0270, 0x0340, 0x04E0, 0x0618,
        0x0750, 0x08F0, 0x0A28, 0x0AF8, 0x0BC8, 0x0C30, 0x0C98, 0x0D00
    },
    // MJC_FB_OSDERR
    {
        0x0000, 0x0080, 0x0090, 0x00A0, 0x00B0, 0x00C0, 0x00D0, 0x00E0,
        0x00F0, 0x0100, 0x0110, 0x0120, 0x0130, 0x0140, 0x0150, 0x0160
    },
    // MJC_FB_LRGV
    {
        0x0000, 0x0012, 0x0020, 0x0024, 0x0028, 0x002C, 0x0030, 0x0034,
        0x003A, 0x0040, 0x0046, 0x004C, 0x0052, 0x0058, 0x005C, 0x0060
    },
    // MJC_FB_BEC
    {
        //0x0000, 0x003C, 0x0041, 0x0047, 0x004E, 0x0056, 0x005F, 0x0069,
        //0x0072, 0x007A, 0x0081, 0x0086, 0x008A, 0x008D, 0x008F, 0x0091
        0, 104, 208, 256, 320, 384, 512, 640, 768, 896, 1024, 1152, 1280, 1376, 1392, 1408
    },
    // MJC_FB_CUST
    {
        0x0000, 0x0068, 0x00D0, 0x01A0, 0x0270, 0x0340, 0x04E0, 0x0618,
        0x0750, 0x08F0, 0x0A28, 0x0AF8, 0x0BC8, 0x0C30, 0x0C98, 0x0D00
    }
};

#endif //#ifndef _DRV_MJC_QTY_H_
