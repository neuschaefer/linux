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

/** @file ms_l2p.c
 *  This C file implements the MS logical to physical transfer functions.
 */

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#ifdef CC_FCI_ENABLE

#include "fcihw_func.h"

#include "msp_const.h"
#include "msp_func.h"

#include "x_lint.h"

LINT_EXT_HEADER_BEGIN

#include "fci_if.h"
#include "fci_type.h"
#include "fci_debug.h"

#include "x_assert.h"
#include "x_typedef.h"
#include "x_os.h"

LINT_EXT_HEADER_END

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------

/*****************************************************************************
 * Debug Macro definitions
 */

#if 0
#define MSP_MsEraseBlock(a, b)                  \
        LOG(1, "=====> %s(%d) %s call MsEraseBlock(%d)\n", __FILE__, __LINE__, __FUNCTION__, b);
#define MSP_MsWriteBlock(a, b, p, n, m, e)      \
        LOG(1, "=====> %s(%d) %s call MsWriteBlock(%d, %d)\n", __FILE__, __LINE__, __FUNCTION__, b, p);
#define MSP_MsWriteSinglePage(a, b, c, d, e, m) \
        LOG(1, "=====> %s(%d) %s call MsWritePage(%d)\n", __FILE__, __LINE__, __FUNCTION__, b);
#endif

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
/*lint -e(950) */

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------

//-------------------------------------------------------------------------
/** _L2P_InitSeg
 *  Initialize MS data structure.
 *  @param  prMSP   point to MS data structure address.
 *  @retval VOID
 */
//-------------------------------------------------------------------------
static VOID _L2P_InitSeg(MSP_T *prMSP)
{
    INT32 i, j;

    ASSERT(prMSP != NULL);

    // check initial flag.
    if (prMSP->fgL2PInit)
    {
        return;
    }

    // Set logical to physical mapping table to default value.
    x_memset(prMSP->au4LPTable, 0xff, sizeof(prMSP->au4LPTable));

    // initial Segment information table.
    for (i = 0; i < MAX_SEG_NUMBER; i++)
    {
        prMSP->aSegInfo[i].fgInit = FALSE;

        prMSP->aSegInfo[i].i4FreeBlks = 0;
        for (j = 0; j < PHYBLKS_PERSEG; j++)
        {
            prMSP->aSegInfo[i].au4FreeTable[j] = 0xffff;
        }
    }

    // set initial flag.
    prMSP->fgL2PInit = TRUE;
}

//-------------------------------------------------------------------------
/** _L2P_Lba2Seg
 *  MS calculate LBA to segment number.
 *  @param  prMSP           point to MS data structure.
 *  @param  u4Lba           logical block address.
 *  @retval   Segment number.
 *  @retval   -1          Fail.
 */
//-------------------------------------------------------------------------
static INT32 _L2P_Lba2Seg(MSP_T *prMSP, UINT32 u4Lba)
{
    INT32 i4SegNo, i4LogBlk;

    ASSERT(prMSP != NULL);

    // Initial the SegInfo.
    if (!prMSP->fgL2PInit)
    {
        _L2P_InitSeg(prMSP);
    }

    // Transfer from LBA address to Logic Block number.
    if (prMSP->uATR.rMsBootAtr.u2BlkSz == 0x10)
    {
        // 16K
        i4LogBlk = (INT32)(u4Lba >> 5); // 512B * 2^5 = 16KB.
    }
    else if (prMSP->uATR.rMsBootAtr.u2BlkSz == 0x08)
    {
        // 8K
        i4LogBlk = (INT32)(u4Lba >> 4); // 512B * 2^4 = 8KB.
    }
    else
    {
        return -1;
    }

    // Check whether the Logic Block number more than Physical blocks.
    if (i4LogBlk >= (INT32)(prMSP->uATR.rMsBootAtr.u2EffBlkNumber - 2))
    {
        return -1;
    }

    // Find the Segment number.
    i4SegNo = 0;

    if (i4LogBlk < FIRSTSEG_LOGICBLK)
    {
        return i4SegNo;
    }

    i4LogBlk -= FIRSTSEG_LOGICBLK;
    i4SegNo++;

    while (i4LogBlk >= MAX_LOGICBLK_INSEG)
    {
        i4LogBlk -= MAX_LOGICBLK_INSEG;
        i4SegNo++;
    }

    // Max MS card is 128 MB = 16 segments.
    ASSERT(i4SegNo < MAX_SEG_NUMBER);

    if (i4SegNo >= MAX_SEG_NUMBER)
    {
        return -1;
    }

    return i4SegNo;
}

//-------------------------------------------------------------------------
/** _L2P_BuildSegInfo
 *  MS calculate LBA to segment number.
 *  @param  prMSP           point to MS data structure.
 *  @param  i4SegNo        segment number.
 *  @retval   S_OK            Success.
 *  @retval   Others          Fail.
 */
//-------------------------------------------------------------------------
static INT32 _L2P_BuildSegInfo(MSP_T *prMSP, INT32 i4SegNo)
{
    INT32 i4Ret, i4PhyBlkNo, i4LogicAddr, fgUsePhy;

    INT32 i, i4MaxSegNo, i4LogicStart, i4LogicEnd;
    UINT8 au1Extra[0x10];
    UINT8 au1Extra2[0x10];

    ASSERT(prMSP != NULL);

    ASSERT(i4SegNo >= 0);

    if (prMSP->aSegInfo[i4SegNo].fgInit)
    {
        return S_OK;
    }

    // Transfer from LBA address to Logic Block number.
    if (prMSP->uATR.rMsBootAtr.u2BlkSz == 0x10)
    {
        // 16K, Segment size = 8 MB.
        i4MaxSegNo = (prMSP->uATR.rMsBootAtr.u2TotalMB >> 3);
    }
    else if (prMSP->uATR.rMsBootAtr.u2BlkSz == 0x08)
    {
        // 8K, Segment size = 4 MB.
        i4MaxSegNo = (prMSP->uATR.rMsBootAtr.u2TotalMB >> 2);
    }
    else
    {
        return E_FAIL;
    }

    i4LogicStart = LOGICBLK_START(i4SegNo);
    i4LogicEnd = LOGICBLK_START(i4SegNo + 1) - 1;

    // Browse all 512 physical blocks in this segment.
    for (i4PhyBlkNo = i4SegNo *PHYBLKS_PERSEG;
        i4PhyBlkNo < ((i4SegNo + 1) *PHYBLKS_PERSEG); i4PhyBlkNo++)
    {
        i4Ret = S_OK;

        // Check DefectBlkNo array in page 1.
        for (i = 0; prMSP->uATR.rMsBootAtr.au2InitDefectBlkNo[i] != 0xffff; i++)
        {
            LOG(9, "BlkNo:0x%08x DefectBlk:0x%08x\n", i4PhyBlkNo,
                prMSP->uATR.rMsBootAtr.au2InitDefectBlkNo[i]);
            if (prMSP->uATR.rMsBootAtr.au2InitDefectBlkNo[i] == (UINT16)i4PhyBlkNo)
            {
                LOG(7, "Try to read BlkNo:0x%04x in defect table.\n", i4PhyBlkNo);

                i4Ret = E_FAIL;
                break;
            }
        }

        if (i4Ret != S_OK)
        {
            continue;
        }

        // Read Extra Data Area on page 0 in block i4PhyBlkNo.
        i4Ret = MSP_MsReadSinglePage((UINT32)i4PhyBlkNo, 0, NULL, au1Extra, CMD_EXTRA_ACC);

        if (i4Ret != S_OK)
        {
            LOG(7, "%s,%d) BlkNo:%d read extra failed\n", __FUNCTION__, __LINE__, i4PhyBlkNo);
            /* XXX Write BS of Extra as 0 */
            continue;
        }

        // Check transformation table bit.
        if ((i4SegNo == i4MaxSegNo) && !(au1Extra[EXTRA_MANAGEMENT_FLAG] & MGFG_TRANS_TBL))
        {
            i4Ret = MSP_MsEraseBlock((UINT32)i4PhyBlkNo);
            if (i4Ret != S_OK)
            {
                LOG(1, "%s,%d) BlkNo:%d erase failed\n", __FUNCTION__, __LINE__, i4PhyBlkNo);
                return i4Ret;
            }
        }

        // check block status is NG (access-protected).
        if ((au1Extra[EXTRA_OVERWRITE_FLAG] & OWFG_BLOCK_STT) == BLKSTT_NG)
        {
            LOG(7, "%s,%d) BlkNo:%d Overwrite:0x%02x\n", __FUNCTION__, __LINE__, i4PhyBlkNo,
                au1Extra[EXTRA_OVERWRITE_FLAG]);
            continue;
        }

        // check page status is ok.
        if ((au1Extra[EXTRA_OVERWRITE_FLAG] & OWFG_PAGE_STT) != PAGESTT_OK)
        {
            LOG(7, "%s,%d) BlkNo:%d Overwrite:0x%02x\n", __FUNCTION__, __LINE__, i4PhyBlkNo,
                au1Extra[EXTRA_OVERWRITE_FLAG]);
            /* XXX Write BS of Extra as 0 */
            continue;
        }

        // Get logical address.
        i4LogicAddr = ((au1Extra[EXTRA_LOGIC_ADDR_1] << 8) | au1Extra[EXTRA_LOGIC_ADDR_0]);

        // logical block is empty or out of boundary.
        if ((i4LogicAddr == 0x0ffff) || (i4LogicAddr < i4LogicStart) || (i4LogicAddr > i4LogicEnd))
        {

#if 0
            // Erase block.

            i4Ret = MSP_MsEraseBlock(i4PhyBlkNo);

            if (i4Ret != S_OK)
            {
                LOG(1, "%s,%d) BlkNo:%d erase failed\n", __FUNCTION__, __LINE__, i4PhyBlkNo);
                return i4Ret;
            }
#endif

            // Register block i4PhyBlkNo on FreeTable[n].

            ASSERT(i4SegNo >= 0);
            ASSERT(prMSP->aSegInfo[i4SegNo].i4FreeBlks >= 0);
            prMSP->aSegInfo[i4SegNo].au4FreeTable[prMSP->aSegInfo[i4SegNo].i4FreeBlks] = i4PhyBlkNo;
            // n <- n+1.
            prMSP->aSegInfo[i4SegNo].i4FreeBlks++;
            continue;
        }

        ASSERT(i4LogicAddr >= 0);

        // Register logical block to physical block mapping table.
        if (prMSP->au4LPTable[i4LogicAddr] == 0x0ffff)
        {
            // LPTable[d] <- X.
            prMSP->au4LPTable[i4LogicAddr] = i4PhyBlkNo;
            continue;
        }

        i4Ret = MSP_MsReadSinglePage(prMSP->au4LPTable[i4LogicAddr], 0, NULL, au1Extra2,
                                     CMD_EXTRA_ACC);

        if (i4Ret != S_OK)
        {
            LOG(7, "%s,%d) BlkNo:%d read extra failed\n", __FUNCTION__, __LINE__,
                prMSP->au4LPTable[i4LogicAddr]);

            // LPTable[d] <- X.
            prMSP->au4LPTable[i4LogicAddr] = i4PhyBlkNo;
            continue;
        }

        // Handle power loss recovery. If two physical blocks have the same logical address.
        if ((au1Extra[EXTRA_OVERWRITE_FLAG]
                 & OWFG_UPDATE_STT) == (au1Extra2[EXTRA_OVERWRITE_FLAG] & OWFG_UPDATE_STT))
        {
            // Update status is the same.
            if (prMSP->au4LPTable[i4LogicAddr] > i4PhyBlkNo)
            {
                // Give up X.
                fgUsePhy = FALSE;
            }
            else
            {
                // Use X.
                fgUsePhy = TRUE;
            }
        }
        else
        {
            // Update status is different.
            if ((au1Extra[EXTRA_OVERWRITE_FLAG] & OWFG_UPDATE_STT) == 0)
            {
                // Use X.
                fgUsePhy = TRUE;
            }
            else
            {
                // Give up X.
                fgUsePhy = FALSE;
            }
        }

        if (fgUsePhy)
        {
            i4Ret = MSP_MsEraseBlock(prMSP->au4LPTable[i4LogicAddr]);

            if (i4Ret != S_OK)
            {
                LOG(1, "%s,%d) BlkNo:%d erase failed\n", __FUNCTION__, __LINE__,
                    prMSP->au4LPTable[i4LogicAddr]);
                return i4Ret;
            }

            ASSERT(i4SegNo >= 0);
            ASSERT(prMSP->aSegInfo[i4SegNo].i4FreeBlks >= 0);
            /* Register block i4PhyBlkNo on FreeTable[n] */
            prMSP->aSegInfo[i4SegNo].au4FreeTable[prMSP->aSegInfo[i4SegNo].i4FreeBlks]
                = prMSP->au4LPTable[i4LogicAddr];
            prMSP->au4LPTable[i4LogicAddr] = i4PhyBlkNo;
        }
        else
        {
            i4Ret = MSP_MsEraseBlock((UINT32)i4PhyBlkNo);

            if (i4Ret != S_OK)
            {
                LOG(1, "%s,%d) BlkNo:%d erase failed\n", __FUNCTION__, __LINE__, i4PhyBlkNo);
                return i4Ret;
            }

            ASSERT(i4SegNo >= 0);
            ASSERT(prMSP->aSegInfo[i4SegNo].i4FreeBlks >= 0);
            /* Register block i4PhyBlkNo on FreeTable[n] */
            prMSP->aSegInfo[i4SegNo].au4FreeTable[prMSP->aSegInfo[i4SegNo].i4FreeBlks] = i4PhyBlkNo;
        }

        ASSERT(i4SegNo >= 0);
        /* n <- n+1 */
        prMSP->aSegInfo[i4SegNo].i4FreeBlks++;
    }

    LOG(7, "Build Seg %d successfully.\n", i4SegNo);
    return S_OK;
}

//-------------------------------------------------------------------------
/** _L2P_ConfirmationSegInfo
 *  Handle no physical address mapping in this segment.
 *  Assign physical block from au4FreeTable[] to this logical block.
 *  @param  prMSP           point to MS data structure.
 *  @param  i4SegNo        segment number.
 *  @retval   S_OK            Success.
 *  @retval   Others          Fail.
 */
//-------------------------------------------------------------------------
static INT32 _L2P_ConfirmationSegInfo(MSP_T *prMSP, INT32 i4SegNo)
{
    INT32 i4Ret, i4PhyBlkNo, i4LogicAddr;

    INT32 i4MaxSegNo, i4LogicStart, i4LogicEnd;
    UINT8 au1Extra[0x10];

    ASSERT(prMSP != NULL);
    ASSERT(i4SegNo >= 0);

    if (prMSP->aSegInfo[i4SegNo].fgInit)
    {
        return S_OK;
    }

    // Transfer from LBA address to Logic Block number.
    if (prMSP->uATR.rMsBootAtr.u2BlkSz == 0x10)
    {
        // 16K
        i4MaxSegNo = (prMSP->uATR.rMsBootAtr.u2TotalMB >> 3);
    }
    else if (prMSP->uATR.rMsBootAtr.u2BlkSz == 0x08)
    {
        // 8K
        i4MaxSegNo = (prMSP->uATR.rMsBootAtr.u2TotalMB >> 2);
    }
    else
    {
        ASSERT(0);

        LINT_SUPPRESS_NEXT_EXPRESSION(527);
        return E_FAIL;
    }

    // Is Last segment and whether transferring into write-protected status?
    if (prMSP->aSegInfo[i4SegNo].i4FreeBlks < ((i4SegNo == i4MaxSegNo) ? 2 : 1))
    {
        prMSP->fgWriteProtected = TRUE;
        return S_OK;
    }

    i4LogicStart = LOGICBLK_START(i4SegNo);
    i4LogicEnd = LOGICBLK_START(i4SegNo + 1) - 1;

    // Handle no physical address mapping in this segment.
    for (i4LogicAddr = i4LogicStart; i4LogicAddr <= i4LogicEnd; i4LogicAddr++)
    {
        ASSERT(i4LogicAddr >= 0);

        // Handle no mapping logical block.
        if (prMSP->au4LPTable[i4LogicAddr] == 0xffff)
        {
            ASSERT(i4SegNo >= 0);

            ASSERT(prMSP->aSegInfo[i4SegNo].i4FreeBlks > 0);

            // Get new physical block from au4FreeTable[].
            prMSP->aSegInfo[i4SegNo].i4FreeBlks--;

            i4PhyBlkNo = prMSP->aSegInfo[i4SegNo].au4FreeTable[prMSP->aSegInfo[i4SegNo].i4FreeBlks];

            // Get original extra data and prepare to update its logical address.
            i4Ret = MSP_MsReadSinglePage((UINT32)i4PhyBlkNo, 0, NULL, au1Extra, CMD_EXTRA_ACC);

            if (i4Ret != S_OK)
            {
                LOG(1, "%s,%d) BlkNo:%d read extra failed\n", __FUNCTION__, __LINE__,
                    prMSP->au4LPTable[i4LogicAddr]);
                return i4Ret;
            }

            // Write logical block address.
            au1Extra[EXTRA_LOGIC_ADDR_1] = (((UINT32)i4LogicAddr >> 8) & 0x0ff);
            au1Extra[EXTRA_LOGIC_ADDR_0] = (i4LogicAddr & 0x0ff);

            i4Ret = MSP_MsWriteSinglePage((UINT32)i4PhyBlkNo, 0, NULL, au1Extra, CMD_EXTRA_ACC);

            if (i4Ret != S_OK)
            {
                LOG(1, "%s,%d) BlkNo:%d write extra failed\n", __FUNCTION__, __LINE__,
                    prMSP->au4LPTable[i4LogicAddr]);
                return i4Ret;
            }

            // Assign new physical address from au4FreeTable[] to mapping table.
            prMSP->au4LPTable[i4LogicAddr] = i4PhyBlkNo;

            // Clear previous physical address in au4FreeTable[].
            prMSP->aSegInfo[i4SegNo].au4FreeTable[prMSP->aSegInfo[i4SegNo].i4FreeBlks] = 0x0ffff;

            // Is Last segment and whether transferring into write-protected status?
            if (prMSP->aSegInfo[i4SegNo].i4FreeBlks < ((i4SegNo == i4MaxSegNo) ? 2 : 1))
            {
                // no further write the card any more.
                prMSP->fgWriteProtected = TRUE;
            }
        }
    }

    ASSERT(i4SegNo >= 0);
    prMSP->aSegInfo[i4SegNo].fgInit = TRUE;

    LOG(7, "Confirm Seg %d successfully.\n", i4SegNo);
    return S_OK;
}

//-------------------------------------------------------------------------
/** _L2P_Lba2BlkAndPage
 *  Calculate LBA to physical block number and page number.
 *  @param  prMSP           point to MS data structure.
 *  @param  i4SegNo        segment number.
 *  @retval   S_OK            Success.
 *  @retval   Others          Fail.
 */
//-------------------------------------------------------------------------
static INT32 _L2P_Lba2BlkAndPage(const MSP_T *prMSP, UINT32 u4Lba, INT32 *pi4Blk, INT32 *pi4PgNo)
{
    INT32 i4PhyBlk, i4PgNo, i4LogBlk;

    ASSERT(prMSP != NULL);
    ASSERT(pi4Blk != NULL);
    ASSERT(pi4PgNo != NULL);

    // Transfer from LBA address to Logic Block number.
    if (prMSP->uATR.rMsBootAtr.u2BlkSz == 0x10)
    {
        // 16K
        i4LogBlk = (u4Lba >> 5); // 512B * 2^5 = 16KB
        i4PgNo = (u4Lba & 0x1f);
    }
    else if (prMSP->uATR.rMsBootAtr.u2BlkSz == 0x08)
    {
        // 8K
        i4LogBlk = (u4Lba >> 4); // 512B * 2^4 = 8KB
        i4PgNo = (u4Lba & 0x0f);
    }
    else
    {
        return E_FAIL;
    }

    i4PhyBlk = prMSP->au4LPTable[i4LogBlk];

    // No mapping physical address... !!!
    if (i4PhyBlk == 0x0ffff)
    {
        LOG(1, "%s(%d) LogBlk:0x%08x return -1 because no in LPTable.\n", __FUNCTION__, __LINE__,
            i4PhyBlk);
        return E_FAIL;
    }

    // physical block number and page number is mapping to LBA.
    *pi4Blk = i4PhyBlk;
    *pi4PgNo = i4PgNo;

    return S_OK;
}

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

//-------------------------------------------------------------------------
/** MSP_MsL2PRead
 *  MS logical to physical read function.
 *  @param  u4Lba           logical block address.
 *  @param  pu1MemPtr   point to user's buffer.
 *  @param  u4SecLen      read sector length.
 *  @retval   S_OK            Success.
 *  @retval   Others          Fail.
 */
//-------------------------------------------------------------------------
INT32 MSP_MsL2PRead(UINT32 u4Lba, UINT8 *pu1MemPtr, UINT32 u4SecLen)
{
    INT32 i4SegNo, i4NewSegNo, i4Ret, i4BlkNo, i4PgNo, i4PgInBlk, i4PgNum, i4RestPg, i4ReadPg;

    MSP_T *prMSP;
    UINT32 u4Ptr;

    prMSP = FCI_GetMSPSlot();

    // Calculate segment number.
    i4SegNo = _L2P_Lba2Seg(prMSP, u4Lba);

    if (i4SegNo < 0)
    {
        LOG(1, "%s(%d) i4SegNo %d\n", __FUNCTION__, __LINE__, i4SegNo);
        return E_FAIL;
    }

    i4Ret = _L2P_BuildSegInfo(prMSP, i4SegNo);

    if (i4Ret != S_OK)
    {
        LOG(1, "%s(%d) return %d\n", __FUNCTION__, __LINE__, i4Ret);
        return i4Ret;
    }

    i4Ret = _L2P_ConfirmationSegInfo(prMSP, i4SegNo);

    if (i4Ret != S_OK)
    {
        LOG(1, "%s(%d) return %d\n", __FUNCTION__, __LINE__, i4Ret);
        return i4Ret;
    }

    if (!prMSP->pu1Extra)
    {
        // 4 bytes alignment.
        u4Ptr = (UINT32)x_mem_alloc((MAX_MS_EXTRA_NUM *MAX_PG_IN_BLK) + 4);
        ASSERT(u4Ptr);
        if (u4Ptr & 0x3)
        {
            u4Ptr = ((u4Ptr + 4) & ~((UINT32)0x3));
        }
       prMSP->pu1Extra = (UINT8 *)u4Ptr;
    }

    /* Transfer from LBA address to Logic Block number. */
    i4PgInBlk = (prMSP->uATR.rMsBootAtr.u2BlkSz == 0x10) ? 32 : 16;
    i4RestPg = (INT32)u4SecLen;
    i4ReadPg = 0;

    while (1)
    {
        i4Ret = _L2P_Lba2BlkAndPage(prMSP, u4Lba, &i4BlkNo, &i4PgNo);

        if (i4Ret != S_OK)
        {
            LOG(1, "%s(%d) Seg:%d return %d\n", __FUNCTION__, __LINE__, i4SegNo, i4Ret);
            return i4Ret;
        }

        i4PgNum = (i4PgInBlk - i4PgNo);

        if (i4PgNum > i4RestPg)
        {
            i4PgNum = i4RestPg;
        }

        ASSERT(pu1MemPtr != NULL);
        i4Ret = MSP_MsReadBlock((UINT32)i4BlkNo, (UINT32)i4PgNo, (UINT32)i4PgNum,
                                pu1MemPtr + (i4ReadPg *0x200), prMSP->pu1Extra);

        if (i4Ret != S_OK)
        {
            LOG(1, "%s(%d) return %d\n", __FUNCTION__, __LINE__, i4Ret);

            i4Ret = _MSP_MSSetCmd("reset");
            i4Ret = _MSP_MSSetCmd("reset");
            return i4Ret;
        }

        i4RestPg -= i4PgNum;

        if (i4RestPg == 0)
        {
            break;
        }

        ASSERT(i4RestPg >= 0);
        i4ReadPg += i4PgNum;
        ASSERT(((UINT32)i4ReadPg) <= u4SecLen);
        u4Lba += (UINT32)i4PgNum;

        i4NewSegNo = _L2P_Lba2Seg(prMSP, u4Lba);

        if (i4NewSegNo < 0)
        {
            LOG(1, "Lba:0x%08x SectLen:0x%08x Size:0x%08x\n", u4Lba, u4SecLen,
                (MS_DEV_TOTALBYTE(prMSP->uATR.rMsBootAtr) >> 9));
            return E_FAIL;
        }
        if (i4NewSegNo != i4SegNo)
        {
            i4Ret = _L2P_BuildSegInfo(prMSP, i4NewSegNo);

            if (i4Ret != S_OK)
            {
                LOG(1, "%s(%d) return %d\n", __FUNCTION__, __LINE__, i4Ret);
                return i4Ret;
            }

            i4Ret = _L2P_ConfirmationSegInfo(prMSP, i4NewSegNo);

            if (i4Ret != S_OK)
            {
                LOG(1, "%s(%d) return %d\n", __FUNCTION__, __LINE__, i4Ret);
                return i4Ret;
            }
            i4SegNo = i4NewSegNo;
        }
    }

    return S_OK;
}

//-------------------------------------------------------------------------
/** MSP_MsL2PWrite
 *  MS logical to physical write function.
 *  @param  u4Lba           logical block address.
 *  @param  pu1MemPtr   point to user's buffer.
 *  @param  u4SecLen      write sector length.
 *  @retval   S_OK            Success.
 *  @retval   Others          Fail.
 */
//-------------------------------------------------------------------------
INT32 MSP_MsL2PWrite(UINT32 u4Lba, const UINT8 *pu1MemPtr, UINT32 u4SecLen)
{
    INT32 i4SegNo, i4NewSegNo, i4Ret, i4BlkNo, i4PgNo, i4PgInBlk, i4PgNum, i4RestPg, i4WritePg;

    MSP_T *prMSP;
    UINT32 u4Ptr;

    prMSP = FCI_GetMSPSlot();
    i4SegNo = _L2P_Lba2Seg(prMSP, u4Lba);

    if (i4SegNo < 0)
    {
        LOG(1, "%s(%d) i4SegNo %d\n", __FUNCTION__, __LINE__, i4SegNo);
        return E_FAIL;
    }

    i4Ret = _L2P_BuildSegInfo(prMSP, i4SegNo);

    if (i4Ret != S_OK)
    {
        LOG(1, "%s(%d) return %d\n", __FUNCTION__, __LINE__, i4Ret);
        return i4Ret;
    }

    i4Ret = _L2P_ConfirmationSegInfo(prMSP, i4SegNo);

    if (i4Ret != S_OK)
    {
        LOG(1, "%s(%d) return %d\n", __FUNCTION__, __LINE__, i4Ret);
        return i4Ret;
    }

    if (!prMSP->pu1MsMemBuf)
    {
        // Maximum 16KB of MS card write buffer.
        // 4 bytes alignment.
        u4Ptr = (UINT32)x_mem_alloc(0x4000 + 4);
        ASSERT(u4Ptr);
        if (u4Ptr & 0x3)
        {
            u4Ptr = ((u4Ptr + 4) & ~((UINT32)0x3));
        }
       prMSP->pu1MsMemBuf = (UINT8 *)u4Ptr;
    }

    if (!prMSP->pu1Extra)
    {
        // 4 bytes alignment.
        u4Ptr = (UINT32)x_mem_alloc((MAX_MS_EXTRA_NUM *MAX_PG_IN_BLK) + 4);
        ASSERT(u4Ptr);
        if (u4Ptr & 0x3)
        {
            u4Ptr = ((u4Ptr + 4) & ~((UINT32)0x3));
        }
       prMSP->pu1Extra = (UINT8 *)u4Ptr;
    }

    /* Transfer from LBA address to Logic Block number. */
    i4PgInBlk = (prMSP->uATR.rMsBootAtr.u2BlkSz == 0x10) ? 32 : 16;
    i4RestPg = (INT32)u4SecLen;
    i4WritePg = 0;

    while (1)
    {
        i4Ret = _L2P_Lba2BlkAndPage(prMSP, u4Lba, &i4BlkNo, &i4PgNo);

        if (i4Ret != S_OK)
        {
            LOG(1, "%s(%d) return %d\n", __FUNCTION__, __LINE__, i4Ret);
            return i4Ret;
        }

        i4PgNum = (i4PgInBlk - i4PgNo);

        if (i4PgNum > i4RestPg)
        {
            i4PgNum = i4RestPg;
        }

        if (i4PgNum == i4PgInBlk)
        {
            /* XXX Read extra _au1MsMemBuf is exhausted, need another function to read whole block Extra only. */
            i4Ret = MSP_MsReadBlock((UINT32)i4BlkNo, 0, (UINT32)i4PgInBlk, prMSP->pu1MsMemBuf, prMSP->pu1Extra);

            if (i4Ret != S_OK)
            {
                LOG(1, "%s(%d) return %d\n", __FUNCTION__, __LINE__, i4Ret);
                return i4Ret;
            }

            i4Ret = MSP_MsEraseBlock((UINT32)i4BlkNo);

            if (i4Ret != S_OK)
            {
                LOG(1, "%s(%d) return %d\n", __FUNCTION__, __LINE__, i4Ret);
                return i4Ret;
            }

            ASSERT(pu1MemPtr != NULL);
            i4Ret = MSP_MsWriteBlock((UINT32)i4BlkNo, 0,
                (UINT32)i4PgInBlk, (UINT8 *)((INT32)pu1MemPtr + (i4WritePg *0x200)), prMSP->pu1Extra);
            if (i4Ret != S_OK)
            {
                LOG(1, "%s(%d) return %d\n", __FUNCTION__, __LINE__, i4Ret);
                return i4Ret;
            }
        }
        else
        {
            i4Ret = MSP_MsReadBlock((UINT32)i4BlkNo, 0,
                (UINT32)i4PgInBlk, prMSP->pu1MsMemBuf, prMSP->pu1Extra);

            if (i4Ret != S_OK)
            {
                LOG(1, "%s(%d) return %d\n", __FUNCTION__, __LINE__, i4Ret);
                return i4Ret;
            }

            i4Ret = MSP_MsEraseBlock((UINT32)i4BlkNo);

            if (i4Ret != S_OK)
            {
                LOG(1, "%s(%d) return %d\n", __FUNCTION__, __LINE__, i4Ret);
                return i4Ret;
            }

            ASSERT(pu1MemPtr != NULL);
            x_memcpy(prMSP->pu1MsMemBuf + (i4PgNo *0x200), pu1MemPtr + (i4WritePg *0x200), (UINT32)(i4PgNum *0x200));
            i4Ret = MSP_MsWriteBlock((UINT32)i4BlkNo, 0,
                (UINT32)i4PgInBlk, prMSP->pu1MsMemBuf, prMSP->pu1Extra);
            if (i4Ret != S_OK)
            {
                LOG(1, "%s(%d) return %d\n", __FUNCTION__, __LINE__, i4Ret);
                return i4Ret;
            }
        }

        i4RestPg -= i4PgNum;

        if (i4RestPg == 0)
        {
            break;
        }

        ASSERT(i4RestPg >= 0);
        i4WritePg += i4PgNum;
        ASSERT((UINT32)i4WritePg <= u4SecLen);
        u4Lba += (UINT32)i4PgNum;

        i4NewSegNo = _L2P_Lba2Seg(prMSP, u4Lba);

        if (i4NewSegNo < 0)
        {
            LOG(1, "Lba:0x%08x SectLen:0x%08x Size:0x%08x\n", u4Lba, u4SecLen, (MS_DEV_TOTALBYTE(prMSP->uATR.rMsBootAtr) >> 9));
            return E_FAIL;
        }
        if (i4NewSegNo != i4SegNo)
        {
            i4Ret = _L2P_BuildSegInfo(prMSP, i4NewSegNo);

            if (i4Ret != S_OK)
            {
                LOG(1, "%s(%d) return %d\n", __FUNCTION__, __LINE__, i4Ret);
                return i4Ret;
            }

            i4Ret = _L2P_ConfirmationSegInfo(prMSP, i4NewSegNo);

            if (i4Ret != S_OK)
            {
                LOG(1, "%s(%d) return %d\n", __FUNCTION__, __LINE__, i4Ret);
                return i4Ret;
            }
            i4SegNo = i4NewSegNo;
        }
    }

    return S_OK;
}
#endif
