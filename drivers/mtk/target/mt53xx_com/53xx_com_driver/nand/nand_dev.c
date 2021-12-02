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
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: nand_dev.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file nand_dev.c
 *  nand_dev.c provide the nand device information.
 */

#include "nand_debug.h"
#include "nandhw_if.h"

static NAND_FLASH_DEV_T _arNand_DevInfo[] =
{
    { "SS K9F2808U0B",         3, 0x000073EC, 0x0000FFFF, 512,  16, 0x00000501,  0x04000,  32,  0x01000000, 0x00411888, 0x00080008},
    { "SS K9F5608U0B",         3, 0x000075EC, 0x0000FFFF, 512,  16, 0x00000501,  0x04000,  32,  0x02000000, 0x00411888, 0x00080008},
    { "SS K9F1208U0B/C",       4, 0x000076EC, 0x0000FFFF, 512,  16, 0x00000501,  0x04000,  32,  0x04000000, 0x00411888, 0x00080008},
    { "SS K9F1G08U0B/C/D",     4, 0x9500F1EC, 0xFFFFFFFF, 2048, 16, 0x00000001,  0x20000,  64,  0x08000000, 0x00433322, 0x00050005},
    { "SS K9F2G08U0B/C",       5, 0x9510DAEC, 0xFFFFFFFF, 2048, 16, 0x00000001,  0x20000,  64,  0x10000000, 0x00433322, 0x00050005},
    { "SS K9F4G08U0B/D",       5, 0x9510DCEC, 0xFFFFFFFF, 2048, 16, 0x00000001,  0x20000,  64,  0x20000000, 0x00433322, 0x00050005},
    { "SS K9K8G08U0D",         5, 0x9551D3EC, 0xFFFFFFFF, 2048, 16, 0x00000001,  0x20000,  64,  0x40000000, 0x00433322, 0x00050005},
    { "SS K9F8G08U0M",         5, 0xA610D3EC, 0xFFFFFFFF, 4096, 16, 0x00000001,  0x40000,  64,  0x40000000, 0x00433322, 0x00050005},
    { "SS K9GAG08U0E",         5, 0x7284D5EC, 0xFFFFFFFF, 8192, 26, 0x00000018,  0x100000, 128, 0x80000000, 0x00433322, 0x00050005},
    { "SS K9GAG08U0F",         5, 0x7694D5EC, 0xFFFFFFFF, 8192, 26, 0x00000018,  0x100000, 128, 0x80000000, 0x00433322, 0x00050005},
    { "SS 1Gb",                4, 0x0000F1EC, 0x0000FFFF, 2048, 16, 0x00000001,  0x20000,  64,  0x08000000, 0x00411888, 0x00080008},
    
    { "NAND128W3A2B",          3, 0x00007320, 0x0000FFFF, 512,  16, 0x00000501,  0x04000,  32,  0x01000000, 0x00411888, 0x00080008},
    { "NAND256W3A2B",          3, 0x00007520, 0x0000FFFF, 512,  16, 0x00000501,  0x04000,  32,  0x02000000, 0x00411888, 0x00080008},
    { "NAND512W3A2B/D",        4, 0x00007620, 0x0000FFFF, 512,  16, 0x00000501,  0x04000,  32,  0x04000000, 0x00411888, 0x00080008},
    { "NAND01GW3B2B",          4, 0x1D80F120, 0xFFFFFFFF, 2048, 16, 0x00000001,  0x20000,  64,  0x08000000, 0x00411888, 0x00080008},
    { "NAND01GW3B2C",          4, 0x1D00F120, 0xFFFFFFFF, 2048, 16, 0x00000001,  0x20000,  64,  0x08000000, 0x00433322, 0x00050005},
    { "NAND02GW3B2C",          5, 0x1D80DA20, 0xFFFFFFFF, 2048, 16, 0x00000001,  0x20000,  64,  0x10000000, 0x00411888, 0x00080008},
    { "NAND02GW3B2D",          5, 0x9510DA20, 0xFFFFFFFF, 2048, 16, 0x00000001,  0x20000,  64,  0x10000000, 0x00433322, 0x00050005},
    { "NAND04GW3B2B",          5, 0x9580DC20, 0xFFFFFFFF, 2048, 16, 0x00000001,  0x20000,  64,  0x20000000, 0x00411888, 0x00080008},
    { "NAND04GW3B2D",          5, 0x9510DC20, 0xFFFFFFFF, 2048, 16, 0x00000001,  0x20000,  64,  0x20000000, 0x00411888, 0x00080008},
    { "NAND08GW3B2C",          5, 0x9551D320, 0xFFFFFFFF, 2048, 16, 0x00000001,  0x20000,  64,  0x40000000, 0x00411888, 0x00080008},
    { "NAND08GW3C2B/16GW3C4B", 5, 0xA514D320, 0xFFFFFFFF, 2048, 16, 0x00000008,  0x40000,  128, 0x40000000, 0x00411888, 0x00080008},
    { "NAND08GW3F2A",          5, 0xA610D320, 0xFFFFFFFF, 4096, 16, 0x00000001,  0x40000,  64,  0x40000000, 0x00433322, 0x00050005},
    { "ST 1Gb",                4, 0x0000F120, 0x0000FFFF, 2048, 16, 0x00000001,  0x20000,  64,  0x08000000, 0x00411888, 0x00080008},
    { "ST 2Gb",                5, 0x0000DA20, 0x0000FFFF, 2048, 16, 0x00000001,  0x20000,  64,  0x10000000, 0x00411888, 0x00080008},
    { "ST 4Gb",                5, 0x0000DC20, 0x0000FFFF, 2048, 16, 0x00000001,  0x20000,  64,  0x20000000, 0x00411888, 0x00080008},
    { "ST 8Gb",                5, 0x0000D320, 0x0000FFFF, 4096, 16, 0x00000001,  0x40000,  64,  0x40000000, 0x00411888, 0x00080008},

    { "HY27US08281A",          3, 0x000073AD, 0x0000FFFF, 512,  16, 0x00000501,  0x04000,  32,  0x01000000, 0x00411888, 0x00080008},
    { "HY27US08561A",          3, 0x000075AD, 0x0000FFFF, 512,  16, 0x00000501,  0x04000,  32,  0x02000000, 0x00411888, 0x00080008},
    { "H27U518S2CTR/P",        4, 0x000076AD, 0x0000FFFF, 512,  16, 0x00000001,  0x04000,  32,  0x04000000, 0x00433322, 0x00050005},
    { "HY27UF081G2A",          4, 0x1D80F1AD, 0xFFFFFFFF, 2048, 16, 0x00000001,  0x20000,  64,  0x08000000, 0x00411888, 0x00080008},
    { "HY27U1G8F2BTR",         4, 0x1D00F1AD, 0xFFFFFFFF, 2048, 16, 0x00000001,  0x20000,  64,  0x08000000, 0x00433322, 0x00050005},
    { "HY27UF082G2B",          5, 0x9510DAAD, 0xFFFFFFFF, 2048, 16, 0x00000001,  0x20000,  64,  0x10000000, 0x00433322, 0x00050005},
    { "HY27U2G8F2CTR",         5, 0x9590DAAD, 0xFFFFFFFF, 2048, 16, 0x00000001,  0x20000,  64,  0x10000000, 0x00433322, 0x00050005},
    { "HY27U4G8F2DTR",         5, 0x9590DCAD, 0xFFFFFFFF, 2048, 16, 0x00000001,  0x20000,  64,  0x20000000, 0x00433322, 0x00050005},
    { "HY27UF084G2B",          5, 0x9510DCAD, 0xFFFFFFFF, 2048, 16, 0x00000001,  0x20000,  64,  0x20000000, 0x00433322, 0x00050005},
    { "HY27UT088G2A",          5, 0xA514D3AD, 0xFFFFFFFF, 2048, 16, 0x00000008,  0x40000,  128, 0x40000000, 0x00411888, 0x00080008},
    { "H27U8G8T2BTR",          5, 0xB614D3AD, 0xFFFFFFFF, 4096, 16, 0x00000008,  0x80000,  128, 0x40000000, 0x00433322, 0x00050005},
    { "H27UAG8T2MTR",          5, 0xB614D5AD, 0xFFFFFFFF, 4096, 16, 0x00000008,  0x80000,  128, 0x80000000, 0x00433322, 0x00050005},
    { "H27UAG8T2ATR",          5, 0x2594D5AD, 0xFFFFFFFF, 4096, 26, 0x00000018,  0x80000,  128, 0x80000000, 0x00433322, 0x00050005},
    { "H27UAG8T2BTR",          5, 0x9A94D5AD, 0xFFFFFFFF, 8192, 26, 0x00000018,  0x200000, 256, 0x80000000, 0x00433322, 0x00050005},
    { "HYNIX 1Gb",             4, 0x0000F1AD, 0x0000FFFF, 2048, 16, 0x00000001,  0x20000,  64,  0x08000000, 0x00411888, 0x00080008},
    { "HYNIX 2Gb",             5, 0x0000DAAD, 0x0000FFFF, 2048, 16, 0x00000001,  0x20000,  64,  0x10000000, 0x00411888, 0x00080008},
    { "HYNIX 4Gb",             5, 0x0000DCAD, 0x0000FFFF, 2048, 16, 0x00000001,  0x20000,  64,  0x20000000, 0x00411888, 0x00080008},
    { "HYNIX 8Gb",             5, 0x0000D3AD, 0x0000FFFF, 2048, 16, 0x00000001,  0x40000,  128, 0x40000000, 0x00411888, 0x00080008},
    
    { "MT29F1G08AAC/ABADA",    4, 0x9580F12C, 0xFFFFFFFF, 2048, 16, 0x00000008,  0x20000,  64,  0x08000000, 0x00433322, 0x00050005},
    { "MT29F2G08AAD",          5, 0x9580DA2C, 0xFFFFFFFF, 2048, 16, 0x00000001,  0x20000,  64,  0x10000000, 0x00433322, 0x00050005},
    { "MT29F2G08ABAEA",        5, 0x9590DA2C, 0xFFFFFFFF, 2048, 16, 0x00000001,  0x20000,  64,  0x10000000, 0x00433322, 0x00050005},
    { "MT29F4G08ABADA",        5, 0x9590DC2C, 0xFFFFFFFF, 2048, 16, 0x00000008,  0x20000,  64,  0x20000000, 0x00433322, 0x00050005},
    { "MT29F8G08ABABA",        5, 0x2600382C, 0xFFFFFFFF, 4096, 26, 0x00000008,  0x80000,  128, 0x40000000, 0x00433322, 0x00050005},
    { "MT29F16G08CBABA",       5, 0x4604482C, 0xFFFFFFFF, 4096, 26, 0x00000018,  0x100000, 256, 0x80000000, 0x00433322, 0x00050005},
    { "MT29F16G08CBACA",       5, 0x4A04482C, 0xFFFFFFFF, 4096, 26, 0x00000018,  0x100000, 256, 0x80000000, 0x00433322, 0x00050005},
    { "Micron 1Gb",            4, 0x0000F12C, 0x0000FFFF, 2048, 16, 0x00000001,  0x20000,  64,  0x08000000, 0x00411888, 0x00080008},
    { "Micron 2Gb",            5, 0x0000DA2C, 0x0000FFFF, 2048, 16, 0x00000001,  0x20000,  64,  0x10000000, 0x00411888, 0x00080008},
    { "Micron 16GB",           5, 0x0000D92C, 0x0000FFFF, 4096, 26, 0x00000004,  0x80000,  128, 0x80000000, 0x00411888, 0x00080008},

    { "TC58DVM92A5TAI0/J0",    4, 0x00007698, 0x0000FFFF, 512,  16, 0x00000501,  0x04000,  32,  0x04000000, 0x00433322, 0x00050005},
    { "TC58NVM9S3ETA00/I0",    4, 0x1500F098, 0xFFFFFFFF, 2048, 16, 0x00000001,  0x20000,  64,  0x04000000, 0x00433322, 0x00050005},
    { "TC58NVG0S3ETA00",       4, 0x1590D198, 0xFFFFFFFF, 2048, 16, 0x00000001,  0x20000,  64,  0x08000000, 0x00433322, 0x00050005},
    { "TC58NVG1S3ETA00",       5, 0x1590DA98, 0xFFFFFFFF, 2048, 16, 0x00000001,  0x20000,  64,  0x10000000, 0x00433322, 0x00050005},
    { "TC58NVG2S3ETA00/I0",    5, 0x1590DC98, 0xFFFFFFFF, 2048, 16, 0x00000001,  0x20000,  64,  0x20000000, 0x00433322, 0x00050005},
    { "TC58DVG2S0FTAI0",       5, 0x2690DC98, 0xFFFFFFFF, 4096, 16, 0x00000008,  0x40000,  64,  0x20000000, 0x00433322, 0x00050005},
    { "TC58DVG3S0E/FTA00",     5, 0x2690D398, 0xFFFFFFFF, 4096, 16, 0x00000008,  0x40000,  64,  0x40000000, 0x00433322, 0x00050005},
    { "TC58NVG4D2FTA00",       5, 0x3294D598, 0xFFFFFFFF, 8192, 26, 0x00000018,  0x100000, 128, 0x80000000, 0x00433322, 0x00050005},
        
    { "MX30LF1G08AM-TIB",      4, 0x1D80F1C2, 0xFFFFFFFF, 2048, 16, 0x00000001,  0x20000,  64,  0x08000000, 0x00433322, 0x00050005},
    
    { "Small 16MB",            3, 0x00007300, 0x0000FF00, 512,  16, 0x00000501,  0x04000,  32,  0x01000000, 0x00411888, 0x00080008},
    { "Small 32MB",            3, 0x00007500, 0x0000FF00, 512,  16, 0x00000501,  0x04000,  32,  0x02000000, 0x00411888, 0x00080008},
    { "Small 64MB",            4, 0x00007600, 0x0000FF00, 512,  16, 0x00000501,  0x04000,  32,  0x04000000, 0x00411888, 0x00080008},
    { "Small 128MB",           4, 0x00007900, 0x0000FF00, 512,  16, 0x00000501,  0x04000,  32,  0x08000000, 0x00411888, 0x00080008},
    { "Large 128MB",           4, 0x0000F100, 0x0000FF00, 2048, 16, 0x00000001,  0x20000,  64,  0x08000000, 0x00411888, 0x00080008},
};

//-----------------------------------------------------------------------------
/** NANDHW_GetDevCount
 */
//-----------------------------------------------------------------------------
UINT32 NANDDev_GetDevCount()
{
    UINT32 u4DevCount;

    u4DevCount = sizeof(_arNand_DevInfo) / sizeof(NAND_FLASH_DEV_T);

    return u4DevCount;
}

//-----------------------------------------------------------------------------
/** NANDHW_GetDev
 */
//-----------------------------------------------------------------------------
NAND_FLASH_DEV_T* NANDDev_GetDev(UINT32 u4Idx)
{
    UINT32 u4DevCount;

    u4DevCount = sizeof(_arNand_DevInfo) / sizeof(NAND_FLASH_DEV_T);

    if(u4Idx >= u4DevCount)
    {
        return NULL;
    }

    return (((NAND_FLASH_DEV_T*)_arNand_DevInfo) + u4Idx);
}

//-----------------------------------------------------------------------------
/** NANDDev_ShowDevList
 */
//-----------------------------------------------------------------------------
void NANDDev_ShowDevList(void)
{
    UINT32 i, u4DevCount;

    u4DevCount = sizeof(_arNand_DevInfo) / sizeof(NAND_FLASH_DEV_T);

    for(i = 0; i < u4DevCount; i++)
    {
        LOG(3,"%d: name:%s, id:0x%x, size:%d MB\n", i,
            _arNand_DevInfo[i].acName,
            _arNand_DevInfo[i].u4IDVal & _arNand_DevInfo[i].u4IDMask,
            _arNand_DevInfo[i].u4ChipSize >> 20);
    }
}

