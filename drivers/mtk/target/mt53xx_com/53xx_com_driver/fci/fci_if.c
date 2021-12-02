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
 * $RCSfile: fci_if.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file fci_if.c
 *  This C file implements the FCI high level functions.
 */

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#ifdef CC_FCI_ENABLE

#include "fci_if.h"

LINT_EXT_HEADER_BEGIN

#include "x_hal_5381.h"
#include "x_hal_926.h"
#include "x_typedef.h"
#include "x_assert.h"
#include "x_util.h"
#include "x_pinmux.h"
#include "x_ckgen.h"

#include "sdc_const.h"
#include "sdio_func.h"
#include "sdc_func.h"
#include "fci_type.h"
#include "fci_debug.h"
#include "fcihw_func.h"

LINT_EXT_HEADER_END

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

#define DIAG_DETECT_DEV 0

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

#define FCI_REG_LENGTH  0x1000

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
/*lint -e(950) */
//__align(16)
static FCISC_T _rFCISC;

#ifdef FCI_DIAG_TEST

UINT8 _FCIaiPattern[] = { 0x00, 0xaa, 0x5a, 0xa5, 0x55, 0xff };

static REG_TEST_T arFCIRgtList [] =
{
    /* Addr,  Type,  Len,   Mask,   fg, DfVal */
    { 0x0000, eRD_ONLY, 2, 0x0000f3ff, 1, 0x00000000 }, /* XXX */
    { 0x0004, eRD_ONLY, 4, 0xffffffff, 1, 0x00000000 }, /* XXX */
    { 0x0008, eRD_ONLY, 4, 0xffff00ff, 1, 0x00000000 }, // MT5387 FCI 0x08[0] DCE default value to 0
    { 0x000c, eRD_WR,    4, 0xf93fffe0, 1, 0x80300000 },
    { 0x0010, eRD_ONLY, 4, 0xff1fffef, 1, 0x400A0000 }, /* Boot change from 0x10 to 0x400A0000 */
    { 0x0014, eRD_WR,    4, 0xffffffff, 1, 0x3e823200 }, /* ??? 0x3e8????? */
    { 0x0018, eRD_WR,    4, 0xffffffff, 1, 0x00000000 },
    { 0x001c, eRD_ONLY, 4, 0xf3ffc037, 1, 0x00000000 },
    { 0x0020, eRD_ONLY, 4, 0xffffffff, 1, 0x00000000 }, /* change from RD_WR to RD_ONLY */
    { 0x0024, eRD_ONLY, 4, 0xffffffff, 1, 0x00000000 }, /* XXX */
    { 0x0028, eRD_ONLY, 4, 0xffffffff, 1, 0x00000000 }, /* XXX */
    { 0x002c, eRD_ONLY, 4, 0xffffffff, 1, 0x00000000 }, /* XXX */
    { 0x0030, eRD_WR,    4, 0xffffffff, 1, 0x00000000 },
    { 0x0034, eRD_WR,    4, 0xffffffff, 1, 0x00000000 },
    { 0x0038, eRD_WR,    4, 0xffffffff, 1, 0x00000000 },
    { 0x003c, eRD_WR,    2, 0x0001ffff, 1, 0x00000000 },
    { 0x0040, eRD_ONLY, 4, 0xffffffff, 0, 0x00000000 }, /* XXX, it might be no default value, checked! */
    { 0x0044, eRD_ONLY, 4, 0xffffffff, 0, 0x00000000 }, /* XXX, it might be no default value, not yet! */
    { 0x0048, eRD_ONLY, 4, 0xffffffff, 0, 0x00000000 }, /* XXX, it might be no default value, not yet! */
    { 0x0050, eRD_WR,    1, 0x000000f0, 1, 0x00000020 },
    /* XXX */
    { 0xffff, eNO_TYPE, -1, 0, 0, 0 }
};

INT32 FCI_UTIL_RegDefChk(UINT32 u4BaseAddr, REG_TEST_T *psrgtRegList);
INT32 FCI_UTIL_AllSpaceRWTest(UINT32 u4BaseAddr, UINT32 u4Length);
INT32 FCI_UTIL_RegRWTest(UINT32 u4BaseAddr, REG_TEST_T *psrgtRegList);

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------

//-------------------------------------------------------------------------
/** _Fci_AllSpaceRWTest
 *  read/write FCI register map test.
 *  @param  u4BaseAddr      register map base address.
 *  @param  u4Length          register map base length.
 *  @retval S_OK	Success
 *  @retval Others	Fail
 */
//-------------------------------------------------------------------------
static INT32 _Fci_AllSpaceRWTest(UINT32 u4BaseAddr, UINT32 u4Length)
{
    UINT32 i, u4Offset, u4Org, u4Tmp;

    u4Offset = 0x20;
    u4Org = IO_READ32(u4BaseAddr, u4Offset);

    for (i = 0; i < 6; i++)
    {
        u4Tmp = _FCIaiPattern[i];

        u4Tmp = (u4Tmp << 8) | u4Tmp;
        u4Tmp = (u4Tmp << 16) | u4Tmp;
        IO_WRITE32(u4BaseAddr, u4Offset, u4Tmp);
        u4Tmp = IO_READ32(u4BaseAddr, u4Offset);
    }

    IO_WRITE32(u4BaseAddr, u4Offset, u4Org);

    for (u4Offset = 0; u4Offset < u4Length; u4Offset += 4)
    { // DWRD is one unit.
        if (u4Offset == 0x20)
        {
            continue;
        }

        u4Org = IO_READ32(u4BaseAddr, u4Offset);

        for (i = 0; i < 6; i++)
        {
            u4Tmp = _FCIaiPattern[i];

            u4Tmp = (u4Tmp << 8) | u4Tmp;
            u4Tmp = (u4Tmp << 16) | u4Tmp;
            IO_WRITE32(u4BaseAddr, u4Offset, u4Tmp);
            u4Tmp = IO_READ32(u4BaseAddr, u4Offset);
        }
        IO_WRITE32(u4BaseAddr, u4Offset, u4Org);
    }

    return S_OK;
}
#endif

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

//-------------------------------------------------------------------------
/** FCI_PowerOn
 *  Power on FCI module (DRAM interface)
 *  @param  void
 *  @return TRUE.
 */
//-------------------------------------------------------------------------

BOOL FCI_PowerOn(void)
{
    return FCIHW_PowerOn();
}

//-------------------------------------------------------------------------
/** FCI_PowerOff
 *  Power off FCI module (DRAM interface)
 *  @param  void
 *  @return TRUE.
 */
//-------------------------------------------------------------------------

BOOL FCI_PowerOff(void)
{
    return FCIHW_PowerOff();
}

//-------------------------------------------------------------------------
/** FCI_GetSDCSlot
 *  Get SD data structure.
 *  @param  void
 *  @return A pointer to SD data structure.
 */
//-------------------------------------------------------------------------
SDC_T *FCI_GetSDCSlot(void)
{
    return (&_rFCISC.rSDCSlot);
}

//-------------------------------------------------------------------------
/** FCI_GetMSPSlot
 *  Get MS data structure.
 *  @param  void
 *  @return A pointer to MS data structure.
 */
//-------------------------------------------------------------------------
MSP_T *FCI_GetMSPSlot(void)
{
    return (&_rFCISC.rMSPSlot);
}

#ifdef CC_FCI_SMXD_SUPPORT
//-------------------------------------------------------------------------
/** FCI_GetSMXDSlot
 *  Get SMXD data structure.
 *  @param  void
 *  @return A pointer to SMXD data structure.
 */
//-------------------------------------------------------------------------
SMXD_T *FCI_GetSMXDSlot(void)
{
    return (&_rFCISC.rSMXDSlot);
}

#endif // #ifdef CC_FCI_SMXD_SUPPORT

//-------------------------------------------------------------------------
/** FCI_RegInit
 *  Initailize all FCI card h/w register.
 *  @param  void
 *  @return  0: success. -1: FCI H/W initialization fail.
 */
//-------------------------------------------------------------------------
INT32 FCI_RegInit(void)
{
    INT32 i4Ret;

    i4Ret = FCIHW_Init();

    if (i4Ret != S_OK)
    {
        return -1;
    }

    i4Ret = FCIHW_SetIsr(TRUE);

    if (i4Ret != S_OK)
    {
        return -1;
    }

    return 0;
}

//-------------------------------------------------------------------------
/** FCI_RegDeInit
 *  De-Initailize all FCI card h/w register.
 *  @param  void
 *  @return  0: success. -1: FCI H/W De-initialization fail.
 */
//-------------------------------------------------------------------------
INT32 FCI_RegDeInit(VOID)
{
    INT32 i4Ret;

    i4Ret = FCIHW_SetIsr(FALSE);

    if (i4Ret != S_OK)
    {
        return -1;
    }

    return 0;
}

//-------------------------------------------------------------------------
/** FCI_Init
 *  Initailize all FCI card.
 *  @param  void
 *  @retval   FCI_TYPE_NONE	no card.
 *  @retval   FCI_TYPE_MS	MS card.
 *  @retval   FCI_TYPE_SD	        SD card.
 *  @retval   FCI_TYPE_SMXD	SM/xD card.
 *  @retval   -1	                        init fail.
 */
//-------------------------------------------------------------------------
INT32 FCI_Init(void)
{
    INT32 i4Ret;
    INT32 i4CardType = FCI_TYPE_NONE;

#ifdef FCI_SDIO_SUPPORT
    SDC_T *prSDC;
#endif

    i4Ret = FCI_RegInit();

    if (i4Ret != S_OK)
    {
        return -1;
    }

#ifndef FCI_SDIO_SUPPORT

    LOG(1, "\n=>Start to check SDSlot.\n");

    // Setup multi-func & driving current
    FCIHW_HWConfigSetup(SDC_SLOT);

    i4Ret = SD_DetectCardRemove();

    // Setup & initialize SD card
    if(i4Ret == S_OK)
    {
        i4Ret = SD_Insert();
    }

    if (i4Ret == 0)
    {
            LOG(1, "Detect and inital SD/MMC/SDIO.\n");
            i4CardType = FCI_TYPE_SD;
    }

#if 0
    LOG(1, "\n=>Start to check MSSlot.\n");

    // Setup multi-func & driving current
    FCIHW_HWConfigSetup(MSP_SLOT);

    i4Ret = MS_DetectCardRemove();

    // Setup & initialize MS card
    if(i4Ret == S_OK)
    {
        i4Ret = MS_Insert();
    }

    if (i4Ret == S_OK)
    {
        LOG(1, "Detect and inital MS/MSPro.\n");
        i4CardType |= FCI_TYPE_MS;
    }

    if(i4CardType == FCI_TYPE_NONE)
    {
        LOG(1, "No card detect.\n");
    }
#endif

#else

    prSDC = FCI_GetSDCSlot();

    LOG(1, "=>Start to check SDIO Slot.\n");

    // Setup multi-func, driving current & pull up resistor
    FCIHW_HWConfigSetup(SDC_SLOT);

    if (SDIO_IsSDIOCard() == S_OK)
    {
        // Detect SDIO card in slot
        prSDC->u4CardType = FCI_SDIO_CARD;

        // Note : DVBH card only support 25 Mhz max clock freq
        _SDC_ChangeClock(SDC_NORM_CLOCK);

        LOG(1, "Detect and inital SDIO card.\n");

        i4CardType = FCI_TYPE_SDIO;
    }

#endif  // #ifndef FCI_SDIO_SUPPORT

    return i4CardType;
}

//-------------------------------------------------------------------------
/** FCI_Init
 *  Remove the slot device data and disable the isr.
 *  @param  void
 *  @return  S_OK  Success.
 */
//-------------------------------------------------------------------------
INT32 FCI_Stop(void)
{
    INT32 i4Ret;

    //VERIFY(FCIHW_SetIsr(FALSE) == 0);

    VERIFY(MS_Remove() == 0);
    VERIFY(SD_Remove() == 0);

#ifdef CC_FCI_SMXD_SUPPORT
    VERIFY(SMXD_Remove() == 0);
#endif

    i4Ret = FCI_RegDeInit();

    return i4Ret;
}

//-------------------------------------------------------------------------
/** FCI_Reset
 *  FCI_Reset will reset the registers of controller to the bootup state
 *  @param  void
 *  @return  S_OK  Success.
 */
//-------------------------------------------------------------------------
INT32 FCI_Reset(void)
{
    VERIFY(FCI_Stop() == 0);
    VERIFY(FCI_Init() >= 0);

    return S_OK;
}

//-------------------------------------------------------------------------
/** FCI_Diag
 *  FCI diagnositc function. Register test.
 *  @param  void
 *  @retval  S_OK       Success.
 *  @retval  Others     Fail.
 */
//-------------------------------------------------------------------------
#ifdef FCI_DIAG_TEST

INT32 FCI_UTIL_RegDefChk(UINT32 u4BaseAddr, REG_TEST_T *psrgtRegList)
{
    REG_TEST_T *psrgt;
    UINT32 u4Tmp;
    INT32 i4Ret = 0;

    ASSERT(psrgtRegList!=NULL);

    for (psrgt=psrgtRegList; psrgt->eRegType!=eNO_TYPE; psrgt++)
    {
        if (!psrgt->fgWDfV) { continue; }
        switch(psrgt->iRegLen) {
        case 1: u4Tmp = IO_READ8(u4BaseAddr, psrgt->u4Addr); break;
        case 2: u4Tmp = IO_READ16(u4BaseAddr, psrgt->u4Addr); break;
        case 4: u4Tmp = IO_READ32(u4BaseAddr, psrgt->u4Addr); break;
        default: return 1;
        }
        if ((u4Tmp & psrgt->u4Mask) !=psrgt->u4DfVal) {
            Printf("Reg ofst[0x%08x]: dftval should be [0x%x &]:0x%x but is 0x%x\n", u4BaseAddr + psrgt->u4Addr,
                    psrgt->u4DfVal, psrgt->u4Mask, u4Tmp);
            i4Ret = 1;
        }
    }
    return i4Ret;
}

INT32 FCI_UTIL_AllSpaceRWTest(UINT32 u4BaseAddr, UINT32 u4Length)
{
    UINT32 i, u4Offset, u4Org, u4Tmp;

    for (u4Offset=0; u4Offset < u4Length; u4Offset+=4) { // DWRD is one unit.
        u4Org = IO_READ32(u4BaseAddr, u4Offset);
        for (i=0; i<6; i++) {
            u4Tmp = _FCIaiPattern[i];
            u4Tmp = (u4Tmp << 8) | u4Tmp;
            u4Tmp = (u4Tmp << 16) | u4Tmp;
            IO_WRITE32(u4BaseAddr, u4Offset, u4Tmp);
            u4Tmp = IO_READ32(u4BaseAddr, u4Offset);
        }
        IO_WRITE32(u4BaseAddr, u4Offset, u4Org);
    }
    return 0;
}

INT32 FCI_UTIL_RegRWTest(UINT32 u4BaseAddr, REG_TEST_T *psrgtRegList)
{
    REG_TEST_T *psrgt;
    UINT8 u1Tmp, u1Pre, u1Org = 0;
    UINT16 u2Tmp, u2Pre, u2Org = 0;
    UINT32 u4Tmp, u4Pre, u4Org = 0;
    CHAR szBuf[16];
    INT32 i, i4Ret = 0;

    ASSERT(psrgtRegList!=NULL);
    for (psrgt=psrgtRegList; psrgt->eRegType!=eNO_TYPE; psrgt++) {
        switch(psrgt->eRegType) {
        case eRD_ONLY:
            switch(psrgt->iRegLen) {
            case 1: u1Tmp = IO_READ8(u4BaseAddr, psrgt->u4Addr); SPrintf(szBuf, "0x%02x", u1Tmp); break;
            case 2: u2Tmp = IO_READ16(u4BaseAddr, psrgt->u4Addr); SPrintf(szBuf, "0x%04x", u2Tmp); break;
            case 4: u4Tmp = IO_READ32(u4BaseAddr, psrgt->u4Addr); SPrintf(szBuf, "0x%08x", u4Tmp); break;
            default: return 1;
            } /* end of switch iRegType */
            // Printf("ROReg ofst[0x%04x]: %s(%d)\n", psrgt->u4Addr, szBuf, psrgt->iRegLen);
            break;
        case eWR_ONLY:
            for (i=0; i<6; i++) {
                u2Tmp = (u1Tmp = _FCIaiPattern[i]);
                u4Tmp = (u2Tmp = (u2Tmp << 8) | u1Tmp);
                u4Tmp = (u4Tmp << 16) | u2Tmp;
                switch(psrgt->iRegLen) {
                case 1: IO_WRITE8(u4BaseAddr, psrgt->u4Addr, u1Tmp); break;
                case 2: IO_WRITE16(u4BaseAddr, psrgt->u4Addr, u2Tmp); break;
                case 4: IO_WRITE32(u4BaseAddr, psrgt->u4Addr, u4Tmp); break;
                default: return 1;
                }
            } /* for test patter loop */
            break;
        case eRD_WR:
            switch(psrgt->iRegLen) {
            case 1: u1Org = IO_READ8(u4BaseAddr, psrgt->u4Addr); break;
            case 2: u2Org = IO_READ16(u4BaseAddr, psrgt->u4Addr); break;
            case 4: u4Org = IO_READ32(u4BaseAddr, psrgt->u4Addr); break;
            default: return 1;
            }
            for (i=0; i<6; i++) {
                u2Tmp = (u1Pre = (u1Tmp = _FCIaiPattern[i]));
                u4Tmp = (u2Pre = (u2Tmp = (u2Tmp << 8) | u1Tmp));
                u4Pre = (u4Tmp = (u4Tmp << 16) | u2Tmp);
                switch(psrgt->iRegLen) {
                case 1:
                    u1Pre = (u1Tmp = u1Tmp & psrgt->u4Mask);
                    IO_WRITE8(u4BaseAddr, psrgt->u4Addr, u1Tmp);
                    break;
                case 2:
                    u2Pre = (u2Tmp = u2Tmp & psrgt->u4Mask);
                    IO_WRITE16(u4BaseAddr, psrgt->u4Addr, u2Tmp);
                    break;
                case 4:
                    u4Pre = (u4Tmp = u4Tmp & psrgt->u4Mask);
                    IO_WRITE32(u4BaseAddr, psrgt->u4Addr, u4Tmp);
                    break;
                default: return 1;
                }
                szBuf[0] = 0;
                switch(psrgt->iRegLen) {
                case 1:
                    u1Tmp = IO_READ8(u4BaseAddr, psrgt->u4Addr);
                    if (u1Tmp!=u1Pre) { SPrintf(szBuf, "0x%02x->0x%02x", u1Pre, u1Tmp); }
                    break;
                case 2:
                    u2Tmp = IO_READ16(u4BaseAddr, psrgt->u4Addr);
                    if (u2Tmp!=u2Pre) { SPrintf(szBuf, "0x%04x->0x%04x", u2Pre, u2Tmp); }
                    break;
                case 4:
                    u4Tmp = IO_READ32(u4BaseAddr, psrgt->u4Addr);
                    if (u4Tmp!=u4Pre) { SPrintf(szBuf, "0x%08x->0x%08x", u4Pre, u4Tmp); }
                    break;
                default: return 1;
                } /* end of switch iRegType */
                if (szBuf[0]!=0) { /* read/write value different */
                    Printf("RWReg ofst[0x%08x]: r/w diff %s(%d)\n", u4BaseAddr + psrgt->u4Addr, szBuf, psrgt->iRegLen);
                    i4Ret = 1;
                }
            } /* for test patter loop */
            switch(psrgt->iRegLen) {
            case 1: IO_WRITE8(u4BaseAddr, psrgt->u4Addr, u1Org); break;
            case 2: IO_WRITE16(u4BaseAddr, psrgt->u4Addr, u2Org); break;
            case 4: IO_WRITE32(u4BaseAddr, psrgt->u4Addr, u4Org); break;
            default: return 1;
            }
            break;
        default: return 1;
        }
    } /* end of for psrgt */
    return i4Ret;
}

#endif  // #ifdef FCI_DIAG_TEST

INT32 FCI_Diag(void)
{
#ifdef FCI_DIAG_TEST
#if DIAG_DETECT_DEV

    BOOL fgLog;

    INT32 i4DevNum;
#endif

    INT32 i4Ret, i4Val;

    i4Val = 0;
    i4Ret = FCI_UTIL_RegDefChk(FCI_IO_BASE_ADDR, arFCIRgtList);
    i4Val |= i4Ret;
    Printf("Register default value check .............. %s\n", (i4Ret ? "FAIL" : "PASS"));
    i4Ret = FCI_UTIL_RegRWTest(FCI_IO_BASE_ADDR, arFCIRgtList);
    i4Val |= i4Ret;
    Printf("Register read/write test .................. %s\n", (i4Ret ? "FAIL" : "PASS"));
    i4Ret = _Fci_AllSpaceRWTest(FCI_IO_BASE_ADDR, FCI_REG_LENGTH);
    i4Val |= i4Ret;
    Printf("Memory space read/write test .............. %s\n", (i4Ret ? "FAIL" : "PASS"));

#if DIAG_DETECT_DEV

    fgLog = FCI_UTIL_SetLogFlag(FALSE);
    i4DevNum = FCI_Init();
    VERIFY(FCI_UTIL_SetLogFlag(fgLog) == fgLog);
    i4Val |= (INT32)!(i4DevNum == 2);
    Printf("FCI detect %d device ....................... %s\n", ((i4DevNum >= 0) ? i4DevNum : 0),
    ((i4DevNum != 2) ? "FAIL" : "PASS"));
#endif  // #if DIAG_DETECT_DEV

    return i4Val;
#else  // #ifdef FCI_DIAG_TEST
    return -1;
#endif  // #ifdef FCI_DIAG_TEST
}
#endif  // #ifdef CC_FCI_ENABLE
