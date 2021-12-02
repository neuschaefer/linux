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
 * $RCSfile: fci_msp.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file fci_msp.c
 *  This C file implements the MS basic hardware protocol functions.  All calls
 *   are from ms_if.c.
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
#include "x_clksrc.h"

LINT_EXT_HEADER_END

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

#define BOOT_BLOCK_RECOVERY             0

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------
#define MAX_MSP_REG_NUM                 (0x20)
#define MAX_MSP_RETRY                       (1)

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------
#define REVERSE_UINT16(x)               ((x) = _Msp_ReverseUINT16(x))
#define REVERSE_UINT32(x)               ((x) = _Msp_ReverseUINT32(x))

#ifdef NDEBUG
#define MSP_DumpReg(szFile, Line)
#else
#define MSP_DumpReg(szFile, Line) //FCIHW_DumpReg(szFile, Line)
#endif

//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------

extern BOOL  _fgFCI4BitBusEnable;

//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------

static MSP_T *_prFCIMSP;

#if BOOT_BLOCK_RECOVERY

//__align(16)

static MS_BATR_T rMsBootAtr;

//__align(16)

static UINT8 au1Extra[0x30];
#endif

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------

//-------------------------------------------------------------------------
/** _Msp_ReverseUINT16
 *  Change little(big) endian to big(little) endian.
 *  @param  u2Val    16 bits little(big) endian value.
 *  @return  16 bits big(little) endian value.
 */
//-------------------------------------------------------------------------

static UINT16 _Msp_ReverseUINT16(UINT16 u2Val)
{
    return ((u2Val >> 8) | ((u2Val & 0xFF) << 8));
}

//-------------------------------------------------------------------------
/** _Msp_ReverseUINT32
 *  Change little(big) endian to big(little) endian.
 *  @param  u4Val    32 bits little(big) endian value.
 *  @return  32 bits big(little) endian value.
 */
//-------------------------------------------------------------------------
static UINT32 _Msp_ReverseUINT32(UINT32 u4Val)
{
    return (((u4Val >> 24) & 0x0ff) | ((u4Val >> 8) & 0x0ff00) | ((u4Val << 8) & 0x0ff0000)
                | ((u4Val << 24) & 0xff000000));
}

/*****************************************************************************
 * TPC functions
 */
//-------------------------------------------------------------------------
/** _Msp_TpcReadPageData
 *  TPC functions:
 *  Send TPC 0x02 = READ_PAGE_DATA/MSPRO_READ_LONG_DATA command to card.
 *  @param  u4PgSz   read page size.
 *  @param  pu1Buf    point to read data buffer.
 *  @retval  S_OK  Success.
 *  @retval  Others  Fail.
 */
//-------------------------------------------------------------------------
static INT32 _Msp_TpcReadPageData(UINT32 u4PgSz, UINT8 *pu1Buf)
{
    INT32 i4Ret;

    INT32 i4Retry = 0;

    MSP_DumpReg(__FILE__, __LINE__);
    LOG(9, "=====> Call _Msp_TpcReadPageData(prMSP, PgSz:0x%02x, pu1Buf:0x%02x)\n", u4PgSz, pu1Buf);

    do
    {
        i4Retry++;

        FCIHW_DmaWriteDramEnable(pu1Buf, u4PgSz);
        VERIFY(MSPHW_SetTpc(NULL, u4PgSz, TPC_READ_PAGE_DATA) == 0);

        // MS card do not generate SIT interrupt at last page read.
        if (_prFCIMSP->fg4BitEn && (!_prFCIMSP->fgLastpage))
        {
            i4Ret = MSPHW_WaitStatusBit('S');
        }
        else
        {
            // serial mode do not have SIT interrupt.
            i4Ret = MSPHW_WaitStatusBit('R');
        }

        if ((i4Ret == E_CMD_RSP_CRC_ERR) || (i4Ret == E_NO_RESPONSE))
        {
            LOG(1, "%s(%d) _Msp_TpcReadPageData fail =%d.\n", __FUNCTION__, __LINE__, i4Ret);
        }
        FCIHW_DmaDisable();
        if (i4Ret != S_OK)
        {
        	x_thread_delay(5);
        }

    // retry when E_CMD_TIMEOUT.
    } while ((i4Ret == E_CMD_TIMEOUT) && (i4Retry < MAX_MSP_RETRY));

    LOG(9, "<===== Call _Msp_TpcReadPageData(prMSP, u4PgSz:0x%02x, pu1Buf:0x%02x) return %d\n",
    u4PgSz, pu1Buf,
    i4Ret);

    return i4Ret;
}

//-------------------------------------------------------------------------
/** _Msp_TpcReadReg
 *  TPC functions:
 *  Send TPC 0x04= READ_REG command to card.
 *  @param  pu1Reg   point to read register buffer.
 *  @param  u4RdSz   read register size.
 *  @retval  S_OK  Success.
 *  @retval  Others  Fail.
 */
//-------------------------------------------------------------------------
static INT32 _Msp_TpcReadReg(UINT8 *pu1Reg, UINT32 u4RdSz)
{
    INT32 i4Ret;

    INT32 i4Retry = 0;

    MSP_DumpReg(__FILE__, __LINE__);

    do
    {
        i4Retry++;

        // TPC_READ_REG has no arguement.
        VERIFY(MSPHW_SetTpc(NULL, u4RdSz, TPC_READ_REG) == 0);

        i4Ret = MSPHW_WaitStatusBit('R');

        if (i4Ret != S_OK)
        {
        	x_thread_delay(5);
        }

    } while ((i4Ret != S_OK) && (i4Retry < MAX_MSP_RETRY));

    MSPHW_ReadRegData(pu1Reg, u4RdSz);

    return S_OK;
}

//-------------------------------------------------------------------------
/** _Msp_TpcGetInt
 *  TPC functions: Send TPC 0x07 = GET_INT command to card.
 *  @param  pu1Reg   Get INT register value.
 *  @retval  S_OK  Success.
 *  @retval  Others  Fail.
 */
//-------------------------------------------------------------------------
static INT32 _Msp_TpcGetInt(UINT8 *pu1Reg)
{
    INT32 i4Ret;

    INT32 i4Retry = 0;

    do
    {
        i4Retry++;

        VERIFY(MSPHW_SetTpc(NULL, 1, TPC_GET_INT) == 0);

        i4Ret = MSPHW_WaitStatusBit('R');
        if (i4Ret != S_OK)
        {
        	x_thread_delay(5);
        }

    } while ((i4Ret != S_OK) && (i4Retry < MAX_MSP_RETRY));

    MSPHW_ReadRegData(pu1Reg, 1);
    return S_OK;
}

//-------------------------------------------------------------------------
/** _Msp_TpcSetRWRegAdrs
 *  TPC functions: Send TPC 0x08 = SET_RW_REG_ADDRS command to card.
 *  @param  u4RdAddr   Set starting address by READ_REG TPC.
 *  @param  u4RdSz   Set the number of bytes by READ_REG TPC.
 *  @param  u4WrAddr   Set starting address by WRITE_REG TPC.
 *  @param  u4WrSz   Set the number of bytes by WRITE_REG TPC.
 *  @retval  S_OK  Success.
 *  @retval  Others  Fail.
 */
//-------------------------------------------------------------------------
static INT32 _Msp_TpcSetRWRegAdrs(UINT32 u4RdAddr, UINT32 u4RdSz, UINT32 u4WrAddr, UINT32 u4WrSz)
{
    INT32 i4Ret;

    UINT32 u4Data;
    INT32 i4Retry = 0;

    MSP_DumpReg(__FILE__, __LINE__);
    LOG(9, "=====> Call _Msp_TpcSetRWRegAdrs(RdAdr:0x%02x, RdSz:0x%02x,"
               "WrAdr:0x%02x, WrSz:0x%02x\n", u4RdAddr, u4RdSz,
    u4WrAddr, u4WrSz);

    u4Data = (((u4RdAddr & 0x0ff) << 0) | ((u4RdSz & 0x0ff) << 8) | ((u4WrAddr & 0x0ff) << 16)
                  | ((u4WrSz & 0x0ff) << 24));

    do
    {
        i4Retry++;

        // TPC_SET_RW_REG_ADRS always with argument 4 byte.
        VERIFY(MSPHW_SetTpc(&u4Data, 4, TPC_SET_RW_REG_ADRS) == 0);

        i4Ret = MSPHW_WaitStatusBit('R');

        if (i4Ret != S_OK)
        {
        	x_thread_delay(5);
        }

    } while ((i4Ret != S_OK) && (i4Retry < MAX_MSP_RETRY));

    LOG(9, "<===== Call _Msp_TpcSetRWRegAdrs(RdAdr:0x%02x, RdSz:0x%02x,"
               "WrAdr:0x%02x, WrSz:0x%02x)\n", u4RdAddr, u4RdSz,
    u4WrAddr, u4WrSz);

    return i4Ret;
}

//-------------------------------------------------------------------------
/** _Msp_TpcExSetCmd
 *  TPC functions: Send TPC 0x09= MSPRO_EX_SET_CMD command to card.
 *  @param  u4Cmd   command code.
 *  @param  u4DataCnt   data count.
 *  @param  u4DataAddr   data address.
  *  @retval  S_OK  Success.
 *  @retval  Others  Fail.
 */
//-------------------------------------------------------------------------
static INT32 _Msp_TpcExSetCmd(UINT32 u4Cmd, UINT32 u4DataCnt, UINT32 u4DataAddr)
{
    UINT32 au4Data[2];

    INT32 i4Retry = 0;
    INT32 i4Ret;

    // Command and parameter setting.
    au4Data[0] = (u4Cmd & 0x0ff) | (u4DataCnt & 0x0ff00) | ((u4DataCnt & 0x0ff) << 16)
                     | (u4DataAddr & 0xff000000);
    au4Data[1] = (((u4DataAddr >> 16) & 0x0ff) | (u4DataAddr & 0x0ff00)
                      | ((u4DataAddr & 0x0ff) << 16));

    do
    {
        i4Retry++;

        VERIFY(MSPHW_SetTpc(au4Data, 7, TPC_EX_SET_CMD) == 0);

        i4Ret = MSPHW_WaitStatusBit('S');

        if (i4Ret != S_OK)
        {
        	x_thread_delay(5);
        }

    } while ((i4Ret != S_OK) && (i4Retry < MAX_MSP_RETRY));

    return i4Ret;
}

//-------------------------------------------------------------------------
/** _Msp_TpcWriteReg
 *  TPC functions: Send TPC 0x0B = WRITE_REG command to card.
 *  @param  pu1Reg      point to const write register buffer.
 *  @param  u4WrSz      write register size.
 *  @retval  S_OK  Success.
 *  @retval  Others  Fail.
 */
//-------------------------------------------------------------------------
static INT32 _Msp_TpcWriteReg(const UINT8 *pu1Reg, UINT32 u4WrSz)
{
    UINT32 au4Data[(MAX_MSP_REG_NUM /sizeof(UINT32))];

    INT32 i4Retry = 0;
    INT32 i4Ret;
    UINT32 i;

    MSP_DumpReg(__FILE__, __LINE__);

    ASSERT(u4WrSz <= MAX_MSP_REG_NUM);
    ASSERT(pu1Reg != NULL);

    LOG(9, "=====> Call _Msp_TpcWriteReg(u4WrSz:0x%02x)\n", u4WrSz);

    for (i = 0; i < (MAX_MSP_REG_NUM /sizeof(UINT32)); i++)
    {
        au4Data[i] = 0;
    }

    for (i = 0; (UINT32)i < u4WrSz; i++)
    {
        if ((i & 0x3) == 0)
        {
            au4Data[i >> 2] = pu1Reg[i];
        }
        else
        {
            au4Data[i >> 2] |= (pu1Reg[i] << ((i & 0x3) *8));
        }
    }

    do
    {
        i4Retry++;

        VERIFY(MSPHW_SetTpc(au4Data, u4WrSz, TPC_WRITE_REG) == 0);
        i4Ret = MSPHW_WaitStatusBit('R');

        if (i4Ret != S_OK)
        {
        	x_thread_delay(5);
        }

    } while ((i4Ret != S_OK) && (i4Retry < MAX_MSP_RETRY));

    LOG(9, "<===== Call _Msp_TpcWriteReg(u4WrSz:0x%02x) %d\n", u4WrSz, i4Ret);
    return i4Ret;
}

//-------------------------------------------------------------------------
/** _Msp_TpcWritePageData
 *  TPC functions:
 *  Send TPC 0x0D = WRITE_PAGE_DATA command to card.
 *  @param  u4PgSz   write page size.
 *  @param  pu1Buf    point to read data buffer.
 *  @retval  S_OK  Success.
 *  @retval  Others  Fail.
 */
//-------------------------------------------------------------------------
static INT32 _Msp_TpcWritePageData(UINT32 u4PgSz, UINT8 *pu1Buf)
{
    INT32 i4Ret;

    INT32 i4Retry = 0;

    MSP_DumpReg(__FILE__, __LINE__);

    do
    {
        i4Retry++;

        VERIFY(MSPHW_SetTpc(NULL, u4PgSz, TPC_WRITE_PAGE_DATA) == 0);

        FCIHW_DmaReadDramEnable(pu1Buf, u4PgSz);

        i4Ret = MSPHW_WaitStatusBit('S');

        FCIHW_DmaDisable();

        if (i4Ret != S_OK)
        {
        	x_thread_delay(5);
        }

    } while ((i4Ret != S_OK) && (i4Retry < MAX_MSP_RETRY));

    return i4Ret;
}

//-------------------------------------------------------------------------
/** _Msp_TpcSetCmd
 *  Send TPC 0x0E = SET_CMD command to card.
 *  @param  u4Cmd   Command control code (Memory access cmd and function command).
 *  @retval  S_OK  Success.
 *  @retval  Others  Fail.
 */
//-------------------------------------------------------------------------
static INT32 _Msp_TpcSetCmd(UINT32 u4Cmd)
{
    INT32 i4Ret;

    INT32 i4Retry = 0;

    MSP_DumpReg(__FILE__, __LINE__);

    do
    {
        i4Retry++;

        // TPC_SET_CMD always with argument 1 byte.
        VERIFY(MSPHW_SetTpc(&u4Cmd, 1, TPC_SET_CMD) == 0);

        i4Ret = MSPHW_WaitStatusBit('S');

        if (i4Ret != S_OK)
        {
        	x_thread_delay(5);
        }
    } while ((i4Ret != S_OK) && (i4Retry < MAX_MSP_RETRY));

    return i4Ret;
}

/*****************************************************************************
 * MSP protocol Integration static functions
 */

//-------------------------------------------------------------------------
/** _Msp_ReadStatusReg
 *  read MS status register.
 *  @param  u4RdAddr  status register offset.
 *  @param  u4RdSz   status register length.
 *  @retval  S_OK  Success.
 *  @retval  Others  Fail.
 */
//-------------------------------------------------------------------------
static INT32 _Msp_ReadStatusReg(UINT32 u4RdAddr, UINT32 u4RdSz)
{
    INT32 i4Ret;

    ASSERT(_prFCIMSP != NULL);
    i4Ret = _Msp_TpcSetRWRegAdrs(u4RdAddr, u4RdSz, u4RdAddr, u4RdSz);

    if (i4Ret != S_OK)
    {
        return i4Ret;
    }

    i4Ret = _Msp_TpcReadReg(&(_prFCIMSP->au1MsReg[u4RdAddr]), u4RdSz);

    return i4Ret;
}

//-------------------------------------------------------------------------
/** _Msp_WriteParaReg
 *  write MS parameter register.
 *  @param  u4WrAddr  parameter register offset.
 *  @param  u4WrSz   parameter register length.
 *  @retval  S_OK  Success.
 *  @retval  Others  Fail.
 */
//-------------------------------------------------------------------------
static INT32 _Msp_WriteParaReg(UINT32 u4WrAddr, UINT32 u4WrSz)
{
    INT32 i4Ret;
    INT32 i4Retry = 8;

    ASSERT(_prFCIMSP != NULL);

    while (i4Retry)
    {
        i4Ret = _Msp_TpcSetRWRegAdrs(0, 0, u4WrAddr, u4WrSz);

        if (i4Ret != S_OK)
        {
            i4Retry--;
            continue;
        }

        i4Ret = _Msp_TpcWriteReg(&(_prFCIMSP->au1MsReg[u4WrAddr]), u4WrSz);

        if (i4Ret != S_OK)
        {
            i4Retry--;
            continue;
        }

        break;
    }

    if (i4Retry == 0)
    {
        LOG(1,"%s,%d) Failed !!",__FUNCTION__,__LINE__);
    }

    return i4Ret;
}

//-------------------------------------------------------------------------
/** _Msp_SwitchParallel
 *  change to parallel interface.
 *  @param  VOID
 *  @retval  S_OK  Success.
 *  @retval  Others  Fail.
 */
//-------------------------------------------------------------------------
static INT32 _Msp_SwitchParallel(VOID)
{
    INT32 i4Ret;

    ASSERT(_prFCIMSP != NULL);

    if (!_fgFCI4BitBusEnable)
    {
        LOG(1, "Error !! 4 Bit Bus Mode Disabled.\n");
        LOG(1, "Enable 4 Bit Bus Mode First.\n");
    }

    if ((_prFCIMSP->fg4BitEn == 0) &&
        (_prFCIMSP->fgSupportParallel))
    {
        // Set it's as serial mode.
        _prFCIMSP->fg4BitEn = FALSE;

        // Reset to serial mode.
        FCIHW_ResetSlot(MSP_SLOT);

        // Try to modify REG_SYSTEM_PARAM to 0x88.
        if (_prFCIMSP->u4CardType == FCI_MS_CARD)
        {
            // MS set 0x88.
            _prFCIMSP->au1MsReg[REG_SYSTEM_PARAM] = (SYSP_ON | SYSP_PAM);
        }
        else
        {
            // MSPRO set 0.
            _prFCIMSP->au1MsReg[REG_SYSTEM_PARAM] = 0;
        }

        i4Ret = _Msp_WriteParaReg(REG_SYSTEM_PARAM, 1);

        if (i4Ret != S_OK)
        {
            return i4Ret;
        }

        // Set it as parallel mode.
        _prFCIMSP->fg4BitEn = TRUE;

        // Reset to parallel mode.
        FCIHW_ResetSlot(MSP_SLOT);

        //if (_prFCIMSP->u4CardType == FCI_MSPRO_CARD)
        //{// MS Card don't change the clock. <= Error, MS also works in 40 Mhz when parallel mode
        FCIHW_ChangeMSPClock(40);
        //}

        // Move to _Msp_InitMsCard & _Msp_InitMSProCard
        // _prFCIMSP->fgSupportParallel = TRUE;

        LOG(6, "MSP Slot switch to Parallel mode.\n");
    }

    return S_OK;
}

//-------------------------------------------------------------------------
/** _Msp_SwitchSerial
 *  change to serial interface.
 *  @param VOID
 *  @retval  S_OK  Success.
 *  @retval  Others  Fail.
 */
//-------------------------------------------------------------------------
static INT32 _Msp_SwitchSerial(VOID)
{
    INT32 i4Ret;

    ASSERT(_prFCIMSP != NULL);

    if (_prFCIMSP->fg4BitEn)
    {
        // Set it as parallel mode.
        _prFCIMSP->fg4BitEn = TRUE;

        // Reset to parallel mode.
        FCIHW_ResetSlot(MSP_SLOT);

        // Try to modify REG_SYSTEM_PARAM to 0x80, then it's serial mode.
        // Both for MSP & MS card
        _prFCIMSP->au1MsReg[REG_SYSTEM_PARAM] = SYSP_ON;

        i4Ret = _Msp_WriteParaReg(REG_SYSTEM_PARAM, 1);

        if (i4Ret != S_OK)
        {
            return i4Ret;
        }

        // Set it's as serial mode.
        _prFCIMSP->fg4BitEn = FALSE;

        // Reset to serial mode.
        FCIHW_ResetSlot(MSP_SLOT);

        // Change to 20 Mhz clock.
        FCIHW_ChangeMSPClock(20);

        LOG(6, "MSP Slot switch to Serial mode.\n");
    }

    return S_OK;
}

//-------------------------------------------------------------------------
/** _Msp_MSSetupBootAttr
 *  Check attribute and endian transformation.
 *  @param  prMsBat        point to MS attribute.
 *  @retval  S_OK  Success.
 *  @retval  Others  Fail.
 */
//-------------------------------------------------------------------------
static INT32 _Msp_MSSetupBootAttr(MS_BATR_T *prMsBat)
{
    UINT32 i;

    UINT16 *pu2;

    ASSERT(prMsBat != NULL);
    LOG(7, "size of (MS_BATR_T) is %d\n", sizeof(MS_BATR_T));

    REVERSE_UINT16(prMsBat->u2BlockId);
    REVERSE_UINT16(prMsBat->u2Format);

    // Check Header: Block ID, and Format reserved.
    if ((prMsBat->u2BlockId != 0x0001) || (prMsBat->u2Format != 0x0100))
    {
        LOG(1, "MS BootBlock error id:0x%x format:0x%x\n", prMsBat->u2BlockId, prMsBat->u2Format);
        return E_FAIL;
    }

    REVERSE_UINT32(prMsBat->u4DisBlkStartAddr);
    REVERSE_UINT32(prMsBat->u4DisBlkSz);
    REVERSE_UINT32(prMsBat->u4CisIdiStartAddr);
    REVERSE_UINT32(prMsBat->u4CisIdiSz);

    if ((prMsBat->u1MSClass != 0x01) || (prMsBat->u1FmtUVal2 != 0x02))
    {
        LOG(1, "MS BootBlock error class:0x%x val2:0x%x\n", prMsBat->u1MSClass,
        prMsBat->u1FmtUVal2);
        return E_FAIL;
    }

    REVERSE_UINT16(prMsBat->u2BlkSz);
    REVERSE_UINT16(prMsBat->u2BlkNumber);
    REVERSE_UINT16(prMsBat->u2EffBlkNumber);
    REVERSE_UINT16(prMsBat->u2PageSz);
    REVERSE_UINT16(prMsBat->u2MemoryManu);
    REVERSE_UINT16(prMsBat->u2MemoryDev);
    REVERSE_UINT16(prMsBat->u2TotalMB);
    REVERSE_UINT16(prMsBat->u2CtrlNumber);
    REVERSE_UINT16(prMsBat->u2CtrlFunction);

    pu2 = ((UINT16 *)(&(prMsBat->u2InfoBlkNo)));

    for (i = 0; i < 256; i++)
    {
        LINT_SUPPRESS_NEXT_EXPRESSION(661);

        LINT_SUPPRESS_NEXT_EXPRESSION(662);
        REVERSE_UINT16(pu2[i]);
    }

    pu2 = ((UINT16 *)(prMsBat->au1FirmVer));

    for (i = 0; i < ((sizeof(prMsBat->au1FirmVer) + sizeof(prMsBat->au1ModelName)) >> 1); i++)
    {
        LINT_SUPPRESS_NEXT_EXPRESSION(661);

        LINT_SUPPRESS_NEXT_EXPRESSION(662);
        REVERSE_UINT16(pu2[i]);
    }

    return S_OK;
}

//-------------------------------------------------------------------------
/** _Msp_MsproReadAttribute
 *  Read MSpro attribute information area.
 *  @param  pu1Buf      point to buffer address.
 *  @retval  S_OK  Success.
 *  @retval  Others  Fail.
 */
//-------------------------------------------------------------------------
static INT32 _Msp_MsproReadAttribute(UINT8 *pu1Buf)
{
    INT32 i4Ret;

    ASSERT(pu1Buf != NULL);
    ASSERT(_prFCIMSP != NULL);

    /* Read 1 pages. */

#if 1
    // Jethro 2007/11/15, only read 1st page
    // i4Ret = _Msp_TpcExSetCmd(CMD_MSPRO_READ_ATRB, 2, 0);
    i4Ret = _Msp_TpcExSetCmd(CMD_MSPRO_READ_ATRB, 1, 0);

    if (i4Ret != S_OK)
    {
        LOG(1, "MSPHW_TpcExSetCmd read atrb-1 failed\n");
        return i4Ret;
    }
#else
    // Use SetTpcCmd also work, FYI
    UINT32 u4WrAddr, u4WrSz;

    u4WrAddr = REG_MSPRO_DATACNT1;
    u4WrSz = ((REG_MSPRO_DATAADR0 - REG_MSPRO_DATACNT1) + 1);

    i4Ret = _Msp_TpcSetRWRegAdrs(0, 0, u4WrAddr, u4WrSz);

    if (i4Ret != S_OK)
    {
        LOG(1, "%s(%d) _Msp_MsproReadAttribute failed ret:%d\n", __FILE__, __LINE__, i4Ret);
        return i4Ret;
    }

    _prFCIMSP->au1MsReg[REG_MSPRO_DATACNT1] = 0x00;
    _prFCIMSP->au1MsReg[REG_MSPRO_DATACNT0] = 0x01;
    _prFCIMSP->au1MsReg[REG_MSPRO_DATAADR3] = 0x00;
    _prFCIMSP->au1MsReg[REG_MSPRO_DATAADR2] = 0x00;
    _prFCIMSP->au1MsReg[REG_MSPRO_DATAADR1] = 0x00;
    _prFCIMSP->au1MsReg[REG_MSPRO_DATAADR0] = 0x00;

    i4Ret = _Msp_TpcWriteReg(&(_prFCIMSP->au1MsReg[u4WrAddr]), u4WrSz);

    if (i4Ret != S_OK)
    {
        LOG(1, "%s(%d) _Msp_MsproReadAttribute - Write Reg failed ret:%d\n", __FILE__, __LINE__, i4Ret);
        return i4Ret;
    }

    x_thread_delay(1);

    i4Ret = _Msp_TpcSetCmd(CMD_MSPRO_READ_ATRB);

    if (i4Ret != S_OK)
    {
        LOG(1, "_Msp_TpcSetCmd read atrb-1 failed\n");
        return i4Ret;
    }
#endif

    /* GET_INT in Serial, read CmdIdx in Parallel */
    if (_prFCIMSP->fg4BitEn)
    {
        _prFCIMSP->au1MsReg[REG_INT] = MSPHW_ReadParallelSif();
    }
    else
    {
        i4Ret = _Msp_TpcGetInt(&(_prFCIMSP->au1MsReg[REG_INT]));
        if (i4Ret != S_OK)
        {
            LOG(1, "%s(%d) GetInt failed ret:%d\n", __FILE__, __LINE__, i4Ret);
            return i4Ret;
        }
    }

    LOG(9, "%s,%d) REG_INT:0x%02x\n", __FUNCTION__, __LINE__, _prFCIMSP->au1MsReg[REG_INT]);

    i4Ret = _Msp_TpcReadPageData(MS_PAGE_SIZE, pu1Buf);

    if (i4Ret != S_OK)
    {
        LOG(1, "MSPHW_TpcReadLongData on atrb-1 failed\n");
        return i4Ret;
    }

    /* Jethro, Don't need to read the second page
    i4Ret = _Msp_TpcReadPageData(MS_PAGE_SIZE, pu1Buf + MS_PAGE_SIZE);

    if (i4Ret != S_OK)
    {
        LOG(1, "MSPHW_TpcReadLongData on atrb-2 failed\n");
        return i4Ret;
    }
    */

    /* GET_INT in Serial, read CmdIdx in Parallel */
    if (_prFCIMSP->fg4BitEn)
    {
        _prFCIMSP->au1MsReg[REG_INT] = MSPHW_ReadParallelSif();
    }
    else
    {
        i4Ret = _Msp_TpcGetInt(&(_prFCIMSP->au1MsReg[REG_INT]));
        if (i4Ret != S_OK)
        {
            LOG(1, "%s(%d) GetInt failed ret:%d\n", __FILE__, __LINE__, i4Ret);
            return i4Ret;
        }
    }

    LOG(9, "%s,%d) REG_INT:0x%02x\n", __FUNCTION__, __LINE__, _prFCIMSP->au1MsReg[REG_INT]);

    return S_OK;
}

//-------------------------------------------------------------------------
/** _Msp_MSProSetupAttr
 *  Check attribure and endian transformation.
 *  @param  prAtr  point to mspro attribute information area.
 *  @retval  S_OK  Success.
 *  @retval  Others  Fail.
 */
//-------------------------------------------------------------------------
static INT32 _Msp_MSProSetupAttr(MSPRO_ATR_T *prAtr)
{
    INT32 i;

    ASSERT(prAtr != NULL);
    REVERSE_UINT16(prAtr->u2Signature);
    REVERSE_UINT16(prAtr->u2Version);

    if (prAtr->u2Signature != 0xA5C3)
    {
        LOG(1, "MSPro AttrSign is not 0xA5C3, it is 0x%04x\n", prAtr->u2Signature);
        return E_FAIL;
    }

    prAtr->prSysInfo = NULL;
    prAtr->pu1ModelName = NULL;

    for (i = 0; i < 12; i++)
    {
        REVERSE_UINT32(prAtr->arDevInfoEntry[i].u4Addr);

        REVERSE_UINT32(prAtr->arDevInfoEntry[i].u4Size);
        if (prAtr->arDevInfoEntry[i].u1DevId == 0x10)
        {
            prAtr->prSysInfo =
                (MSPRO_SYSINFO_T *)(void *)(((UINT8 *)prAtr) + prAtr->arDevInfoEntry[i].u4Addr);
        }
        else if (prAtr->arDevInfoEntry[i].u1DevId == 0x15)
        {
            prAtr->pu1ModelName = (((UINT8 *)prAtr) + prAtr->arDevInfoEntry[i].u4Addr);
        }
    }

    if ((prAtr->prSysInfo == NULL) || (prAtr->pu1ModelName == NULL))
    {
        LOG(1, "No SysInfo:0x%08x or No ModelName:0x%08x\n", (UINT32)prAtr->prSysInfo,
        (UINT32)prAtr->pu1ModelName);
        return E_FAIL;
    }

    ASSERT(((UINT32)prAtr->prSysInfo) >= ((UINT32)prAtr + 400));
    ASSERT(((UINT32)prAtr->prSysInfo) <= ((UINT32)prAtr + 2048));

    REVERSE_UINT16(prAtr->prSysInfo->u2BlkSz);
    REVERSE_UINT16(prAtr->prSysInfo->u2TotalBlks);
    REVERSE_UINT16(prAtr->prSysInfo->u2UsrAreaBlks);
    REVERSE_UINT16(prAtr->prSysInfo->u2PageSz);
    // Jethro, 2007/11/20.
    // In ARM11, compiler will use STRH for the follwing operation.
    // But prSysInfo->u2Year is byte 1 & 2 in 4 byte alignment segment.
    // It will cause memory access violation. So mark it for safe.
    // REVERSE_UINT16(prAtr->prSysInfo->u2Year);
    REVERSE_UINT32(prAtr->prSysInfo->u4SerialNumber);
    REVERSE_UINT16(prAtr->prSysInfo->u2MemoryMaker);
    REVERSE_UINT16(prAtr->prSysInfo->u2MemoryModel);
    REVERSE_UINT16(prAtr->prSysInfo->u2CtrlNumber);
    REVERSE_UINT16(prAtr->prSysInfo->u2CtrlFunction);
    REVERSE_UINT16(prAtr->prSysInfo->u2StartSector);
    REVERSE_UINT16(prAtr->prSysInfo->u2UnitSz);
    REVERSE_UINT16(prAtr->prSysInfo->u2CtrlCode);

    return S_OK;
}

/*****************************************************************************
 * Main Operation functions
 */

//-------------------------------------------------------------------------
/** _Msp_InitMSProCard
 *  Initialize MSpro card.
 *  @param  VOID
 *  @retval  S_OK  Success.
 *  @retval  Others  Fail.
 */
//-------------------------------------------------------------------------
static INT32 _Msp_InitMSProCard(VOID)
{
    INT32 i4Ret;

    ASSERT(_prFCIMSP != NULL);

    if (_prFCIMSP->u4CardType != FCI_MSPRO_CARD)
    {
        return E_FAIL;
    }
    // wait ms card to stable.
    x_thread_delay(500);

    // Read Attribute and Information Block.
    LOG(7, "uATR.rMspAtr size is %d\n", sizeof(_prFCIMSP->uATR.rMsproAtr));
    i4Ret = _Msp_MsproReadAttribute((UINT8 *)&_prFCIMSP->uATR);

    if (i4Ret != S_OK)
    {
        LOG(1, "Mspro read attribute failed.\n");
        return i4Ret;
    }

    // Check attribute and endian transformation.
    i4Ret = _Msp_MSProSetupAttr(&_prFCIMSP->uATR.rMsproAtr);

    if (i4Ret != S_OK)
    {
        LOG(1, "Mspro setup attribute failed.\n");
        return i4Ret;
    }

    if (_prFCIMSP->uATR.rMsproAtr.prSysInfo->u1InterfaceType == 1)
    {
        if (_fgFCI4BitBusEnable)
        {
            // Support Parallel mode
            _prFCIMSP->fgSupportParallel = TRUE;

            // Switch the MSPro card to Parallel Mode.
            i4Ret = _Msp_SwitchParallel();
            if (i4Ret != S_OK)
            {
                LOG(1, "MSPro card switch Parallel mode failed.\n");
                return i4Ret;
            }
        }
    }

    return S_OK;
}

//-------------------------------------------------------------------------
/** _Msp_MsReadBootblock
 *  Boot Block Search Process Flow. Follow MS spec v1.42-00, Fig A.5.1.
 *  @param  pu1Buf  point to buffer address.
 *  @retval  S_OK  Success.
 *  @retval  Others  Fail.
 */
//-------------------------------------------------------------------------
static INT32 _Msp_MsReadBootblock(UINT8 *pu1Buf)
{
    INT32 i4Ret;

    UINT32 i;
    UINT8 au1Extra[32];

    ASSERT((_prFCIMSP != NULL) && (pu1Buf != NULL));

    // check first 12 block, page 0 for boot block.
    for (i = 0; i < 12; i++)
    {
        // 1st block, page 0.
        i4Ret = MSP_MsReadSinglePage(i, 0, pu1Buf, au1Extra, CMD_SINGLE_ACC);

        if (i4Ret != S_OK)
        {
            continue;
        }

        // Follow MS spec v1.42-00, Fig A.5.1 Boot Block Search Process Flow.
        if ((au1Extra[0]&OWFG_BLOCK_STT)
                && !(au1Extra[1]&MGFG_SYSTEM) && (pu1Buf[0] == 0x00) && (pu1Buf[1] == 0x01))
        {
            LOG(6, "System block is block [%d] - 0x%02x 0x%02x 0x%02x 0x%02x\n", i, au1Extra[0],
            au1Extra[1], au1Extra[2], au1Extra[3]);

            // 1st block, page 1.
            i4Ret = MSP_MsReadSinglePage(i, 1, pu1Buf + MS_PAGE_SIZE, au1Extra, CMD_SINGLE_ACC);

            if (i4Ret != S_OK)
            {
                LOG(1, "%s(%d) MsReadBootblock failed\n", __FILE__, __LINE__);
                return i4Ret;
            }

            // 1st block, page 2.
            i4Ret = MSP_MsReadSinglePage(i, 2, pu1Buf + (2 *MS_PAGE_SIZE), au1Extra,
            CMD_SINGLE_ACC);

            if (i4Ret != S_OK)
            {
                LOG(1, "%s(%d) MsReadBootblock failed\n", __FILE__, __LINE__);
                return i4Ret;
            }
            return S_OK;
        }
    }

    LOG(1, "MsReadBootblock extra data error\n");
    return E_FAIL;
}

//-------------------------------------------------------------------------
/** _Msp_InitMsCard
 *  Initialize MS card.
 *  @param  VOID
 *  @retval  S_OK  Success.
 *  @retval  Others  Fail.
 */
//-------------------------------------------------------------------------
static INT32 _Msp_InitMsCard(VOID)
{
    INT32 i4Ret;

    ASSERT(_prFCIMSP != NULL);

    if (_prFCIMSP->u4CardType != FCI_MS_CARD)
    {
        return E_FAIL;
    }

    // wait ms card to stable.
    x_thread_delay(500);

    // Read Bootblock.
    i4Ret = _Msp_MsReadBootblock((UINT8 *)&_prFCIMSP->uATR);

    if (i4Ret != S_OK)
    {
        LOG(1, "MS read boot block failed\n");
        return i4Ret;
    }

    // Parsing the Bootblock information.
    i4Ret = _Msp_MSSetupBootAttr(&_prFCIMSP->uATR.rMsBootAtr);

    if (i4Ret != S_OK)
    {
        LOG(1, "MS setup attribute failed.\n");
        return i4Ret;
    }

    // Check Device type: 0 = MS, 1 = ROM, 2 = ROM2, 3 = ROM3. Other: reserved.
    if ((_prFCIMSP->uATR.rMsBootAtr.u1DevType == 1) || (_prFCIMSP->uATR.rMsBootAtr.u1DevType == 2)
            || (_prFCIMSP->uATR.rMsBootAtr.u1DevType == 3))
    {
        _prFCIMSP->fgWriteProtected = TRUE;
    }
    else if (_prFCIMSP->uATR.rMsBootAtr.u1DevType != 0)
    {
        LOG(1, "MS type:0x%02x error\n", _prFCIMSP->uATR.rMsBootAtr.u1DevType);
        return E_FAIL;
    }

    // Check Interface type: 0: Serial, 1: Serial&Parallel, other: reserved.
    if (_prFCIMSP->uATR.rMsBootAtr.u1InterfaceType == 1)
    {
        if (_fgFCI4BitBusEnable)
        {
            // Support Parallel mode
            _prFCIMSP->fgSupportParallel = TRUE;

            i4Ret = _Msp_SwitchParallel();
            if (i4Ret != S_OK)
            {
                LOG(1, "MS card switch Parallel mode failed.\n");
                return i4Ret;
            }
        }
    }
    else if (_prFCIMSP->uATR.rMsBootAtr.u1InterfaceType != 0)
    {
        LOG(1, "MS Interface:0x%02x error\n", _prFCIMSP->uATR.rMsBootAtr.u1InterfaceType);
        return E_FAIL;
    }

    // Set the MS LPTable as un-initial.
    _prFCIMSP->fgL2PInit = FALSE;
    return S_OK;
}

//-------------------------------------------------------------------------
/** _Msp_CardDetect
 *  Read MS status, type, category, class, register.
 *  @param  VOID.
 *  @retval   card type number: Success.
 *  @retval   E_FAIL : Fail.
 */
//-------------------------------------------------------------------------
static INT32 _Msp_CardDetect(VOID)
{
    INT32 i4Ret;

    UINT8 u1Status, u1Type, u1Category, u1Class;

    ASSERT(_prFCIMSP != NULL);

    // Set to default MS 20 Mhz clock.
    FCIHW_ChangeMSPClock(20);

    LOG(9, "=====>Enter CardDetect\n");

    // Get Status register to identify card type, category, class.
    i4Ret = _Msp_ReadStatusReg(REG_STATUS_0, 6);

    if (i4Ret != S_OK)
    {
#if 0
        // Switch to serial mode. and then read again.
        i4Ret = _Msp_SwitchSerial();

        if (i4Ret != S_OK)
        {
            return i4Ret;
        }

        // use serial interface to read status register again.
        i4Ret = _Msp_ReadStatusReg(REG_STATUS_0, 6);
        if (i4Ret != S_OK)
        {
            LOG(1, "ReadReg(%d, %d) failed return %d\n", REG_STATUS_0, 6, i4Ret);
            return i4Ret;
        }
#else
        return i4Ret;
#endif
    }

    u1Status = _prFCIMSP->au1MsReg[REG_STATUS_0];
    u1Type = _prFCIMSP->au1MsReg[REG_TYPE];
    u1Category = _prFCIMSP->au1MsReg[REG_CATEGORY];
    u1Class = _prFCIMSP->au1MsReg[REG_CLASS];

    _prFCIMSP->u4CardType = FCI_UNKNOWN_CARD;
    LOG(6, "Status:0x%02x Type:0x%02x Category:0x%02x Class:0x%02x\n", u1Status, u1Type, u1Category,
    u1Class);

    switch (u1Type)
    {
    case 0x00:
        if ((u1Category == 0x00) && (u1Class == 0x00))
        {
            _prFCIMSP->fgWriteProtected = ((u1Status&ST0_WP) ? TRUE : FALSE);
            _prFCIMSP->u4CardType = FCI_MS_CARD;
        }
        else if ((u1Category == 0x00) && (u1Class >= 0x01) && (u1Class <= 0x03))
        {
            _prFCIMSP->fgWriteProtected = TRUE;
            _prFCIMSP->u4CardType = FCI_MS_CARD;
        }
        break;

    case 0x01:
        if ((u1Category == 0x00) && (u1Class == 0x00))
        {
            _prFCIMSP->fgWriteProtected = ((u1Status&ST0_WP) ? TRUE : FALSE);
            _prFCIMSP->u4CardType = FCI_MSPRO_CARD;
        }
        else if ((u1Category == 0x00) && (u1Class >= 0x01) && (u1Class <= 0x03))
        {
            _prFCIMSP->fgWriteProtected = TRUE;
            _prFCIMSP->u4CardType = FCI_MSPRO_CARD;
        }
        break;

    case 0xff:
        if ((u1Category == 0xff) && ((u1Class >= 0x01) && (u1Class <= 0x03)))
        {
            _prFCIMSP->fgWriteProtected = TRUE;
            _prFCIMSP->u4CardType = FCI_MS_CARD;
        }
        else if ((u1Category == 0xff) && (u1Class == 0xff))
        {
            _prFCIMSP->fgWriteProtected = ((u1Status&ST0_WP) ? TRUE : FALSE);
            _prFCIMSP->u4CardType = FCI_MS_CARD;
        }
        break;

    default:
        break;
    }

    LOG(9, "=====>Endof CardDetect\n");
    return (INT32)_prFCIMSP->u4CardType;
}

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

/*****************************************************************************
 * CLI Export functions
 */

//-------------------------------------------------------------------------
/** _Msp_CardDetect
 *  CLI card detect function.
 *  @param  VOID.
 *  @retval  S_OK  Success.
 *  @retval  Others  Fail.
 */
//-------------------------------------------------------------------------
INT32 _MSP_Detect(VOID)
{
    INT32 i4Ret;

    i4Ret = _Msp_CardDetect();
    UNUSED(i4Ret);
    LOG(1, "Detect return %d\n", i4Ret);
    return S_OK;
}

//-------------------------------------------------------------------------
/** _MSP_SetRWReg
 *  CLI set R/W register function.
 *  @param  VOID.
 *  @retval  S_OK  Success.
 *  @retval  Others  Fail.
 */
//-------------------------------------------------------------------------
INT32 _MSP_SetRWReg(VOID)
{
    INT32 i4Ret;

    UINT32 u4RdAddr, u4RdSz, u4WrAddr, u4WrSz;

    u4RdAddr = REG_INT; // REG_OVERWRITE_FLAG;
    // u4RdSz = (REG_RESERVED_0 - REG_OVERWRITE_FLAG + 1);
    u4RdSz = ((REG_RESERVED_0 - REG_INT) + 1);
    u4WrAddr = REG_SYSTEM_PARAM;
    u4WrSz = ((REG_PAGE_ADDR - REG_SYSTEM_PARAM) + 1);
    i4Ret = _Msp_TpcSetRWRegAdrs(u4RdAddr, u4RdSz, u4WrAddr, u4WrSz);

    LOG(1, "u4RdAddr = 0x%x, u4RdSz = 0x%0x\n", u4RdAddr, u4RdSz);
    LOG(1, "u4WrAddr = 0x%x, u4WrSz = 0x%0x\n", u4WrAddr, u4WrSz);
    LOG(1, "SetRWRegAdrs return %d\n", i4Ret);

    return i4Ret;
}

//-------------------------------------------------------------------------
/** _MSP_ReadReg
 *  CLI read register function.
 *  @param  VOID.
 *  @retval  S_OK  Success.
 *  @retval  Others  Fail.
 */
//-------------------------------------------------------------------------
INT32 _MSP_ReadReg(VOID)
{
    UINT32 u4RdAddr, u4RdSz, i;

    INT32 i4Ret;
    MSP_T *prMSP;

    prMSP = FCI_GetMSPSlot();
    u4RdAddr = REG_INT; // REG_OVERWRITE_FLAG;
    // u4RdSz = (REG_RESERVED_0 - REG_OVERWRITE_FLAG + 1);
    u4RdSz = ((REG_RESERVED_0 - REG_INT) + 1);
    i4Ret = _Msp_TpcReadReg(&(prMSP->au1MsReg[u4RdAddr]), u4RdSz);

    LOG(1, "ReadReg return %d\n", i4Ret);

    if (i4Ret != S_OK)
    {
        return i4Ret;
    }

    for (i = u4RdAddr; i < (u4RdAddr + u4RdSz); i++)
    {
        LOG(1, "Reg[0x%02x]=0x%02x\n", i, prMSP->au1MsReg[i]);
    }

    return S_OK;
}

//-------------------------------------------------------------------------
/** _MSP_WriteReg
 *  CLI write register function.
 *  @param  VOID.
 *  @retval  S_OK  Success.
 *  @retval  Others  Fail.
 */
//-------------------------------------------------------------------------
INT32 _MSP_WriteReg(VOID)
{
    UINT32 u4WrAddr, u4WrSz;

    INT32 i4Ret;
    MSP_T *prMSP;

    prMSP = FCI_GetMSPSlot();

    prMSP->au1MsReg[REG_SYSTEM_PARAM] = (SYSP_ON);
    prMSP->au1MsReg[REG_BLOCK_ADDR_2] = 0;
    prMSP->au1MsReg[REG_BLOCK_ADDR_1] = 0;
    prMSP->au1MsReg[REG_BLOCK_ADDR_0] = 0;
    prMSP->au1MsReg[REG_CMD_PARAM] = CMD_SINGLE_ACC;
    prMSP->au1MsReg[REG_PAGE_ADDR] = 0;

    u4WrAddr = REG_SYSTEM_PARAM;
    u4WrSz = ((REG_PAGE_ADDR - REG_SYSTEM_PARAM) + 1);
    i4Ret = _Msp_TpcWriteReg(&(prMSP->au1MsReg[u4WrAddr]), u4WrSz);

    LOG(1, "WriteReg return %d\n", i4Ret);
    return i4Ret;
}

//-------------------------------------------------------------------------
/** _MSP_MSSetCmd
 *  CLI set command function.
 *  @param  szStr command string.
 *  @retval  S_OK  Success.
 *  @retval  Others  Fail.
 */
//-------------------------------------------------------------------------
INT32 _MSP_MSSetCmd(const CHAR *szStr)
{
    INT32 i4Ret;

    UINT32 u4Cmd;

    ASSERT(szStr != NULL);

    switch (szStr[0])
    {
    case 'R':
    case 'r':
        if ((szStr[2] == 'A') || (szStr[2] == 'a'))
        {
            u4Cmd = CMD_BLOCK_READ;
        }
        else
        {
            u4Cmd = CMD_RESET;
        }
        break;

    case 'W':
    case 'w':
        u4Cmd = CMD_BLOCK_WRITE;
        break;

    case 'E':
    case 'e':
        if ((szStr[1] == 'R') || (szStr[2] == 'r'))
        {
            u4Cmd = CMD_BLOCK_ERASE;
        }
        else
        {
            u4Cmd = CMD_BLOCK_END;
        }
        break;

    case 'S':
    case 's':
        if ((szStr[1] == 'L') || (szStr[2] == 'l'))
        {
            u4Cmd = CMD_SLEEP;
        }
        else
        {
            u4Cmd = CMD_FLASH_STOP;
        }
        break;

    case 'C':
    case 'c':
        u4Cmd = CMD_CLEAR_BUF;
        break;

    default:
        LOG(1, "read/write/end/erase/stop/sleep/clear/reset only\n");
        return E_FAIL;
    }

    i4Ret = _Msp_TpcSetCmd(u4Cmd);
    return i4Ret;
}

//-------------------------------------------------------------------------
/** _MSP_MSPSetCmd
 *  CLI set command function.
 *  @param  szStr command string.
 *  @retval  S_OK  Success.
 *  @retval  Others  Fail.
 */
//-------------------------------------------------------------------------
INT32 _MSP_MSPSetCmd(const CHAR *szStr)
{
    INT32 i4Ret;

    UINT32 u4Cmd;

    ASSERT(szStr != NULL);

    switch (szStr[0])
    {
    case 'R':
    case 'r':
        u4Cmd = CMD_MSPRO_READ_DATA;
        break;

    case 'W':
    case 'w':
        u4Cmd = CMD_MSPRO_WRITE_DATA;
        break;

    case 'E':
    case 'e':
        u4Cmd = CMD_MSPRO_ERASE;
        break;

    case 'S':
    case 's':
        if ((szStr[1] == 'L') || (szStr[2] == 'l'))
        {
            u4Cmd = CMD_MSPRO_SLEEP;
        }
        else
        {
            u4Cmd = CMD_MSPRO_STOP;
        }
        break;

    case 'C':
    case 'c':
        u4Cmd = CMD_CLEAR_BUF;
        break;

    case 'G':
    case 'g':
        u4Cmd = CMD_MSPRO_GET_IBD;
        break;

    default:
        LOG(1, "read/write/erase/sleep/stop/clear/getibd only\n");
        return E_FAIL;
    }

    i4Ret = _Msp_TpcSetCmd(u4Cmd);
    return i4Ret;
}

//-------------------------------------------------------------------------
/** _MSP_GetInt
 *  CLI get INT register function.
 *  @param  VOID.
 *  @retval  S_OK  Success.
 *  @retval  Others  Fail.
 */
//-------------------------------------------------------------------------
INT32 _MSP_GetInt(VOID)
{
    INT32 i4Ret = 0;

    MSP_T *prMSP;

    prMSP = FCI_GetMSPSlot();

    // Serial interface: Get INT result by TPC.
    if (prMSP->fg4BitEn == 0)
    {
        i4Ret = _Msp_TpcGetInt(&(prMSP->au1MsReg[REG_INT]));

        if (i4Ret != S_OK)
        {
            LOG(1, "%s(%d) GetInt failed ret:%d\n", __FILE__, __LINE__, i4Ret);
            return i4Ret;
        }
        LOG(1, "Serial REG_INT:0x%02x\n", prMSP->au1MsReg[REG_INT]);
    }
    else
    {
        // Parallel interface check by H/W INT DATA[3:0].
        prMSP->au1MsReg[REG_INT] = MSPHW_ReadParallelSif();
        LOG(1, "Parallel INT:0x%02x\n", prMSP->au1MsReg[REG_INT]);
    }

    return i4Ret;
}

//-------------------------------------------------------------------------
/** _MSP_ReadPage
 *  CLI read page data function.
 *  @param  u4Block     block number.
 *  @param  u4PgNo     page number.
 *  @param  pu4Result  point to result buffer.
 *  @param  u4BufPtr    data buffer address.
 *  @retval  S_OK  Success.
 *  @retval  Others  Fail.
 */
//-------------------------------------------------------------------------
INT32 _MSP_ReadPage(UINT32 u4Block, UINT32 u4PgNo, UINT32 *pu4Result, UINT32 u4BufPtr)
{
    UINT8 au1Extra[0x10];

    INT32 i4Ret, i;

    for (i = 0; i < 9; i++)
    {
        au1Extra[i] = 0x55;
    }

    if (u4BufPtr == 0)
    {
        i4Ret = MSP_MsReadSinglePage(u4Block, u4PgNo, NULL, au1Extra, CMD_EXTRA_ACC);
    }
    else
    {
        i4Ret = MSP_MsReadSinglePage(u4Block, u4PgNo, (UINT8 *)u4BufPtr, au1Extra, CMD_SINGLE_ACC);
    }

    LOG(9, "ReadPage(Blk:%d, Pg:%d) return %d\n", u4Block, u4PgNo, i4Ret);

    if (pu4Result)
    {
        *pu4Result = 0;
        for (i = 0; i < 4; i++)
        {
            *pu4Result = ((*pu4Result << 8) | au1Extra[i]);
        }
    }

    return i4Ret;
}

//-------------------------------------------------------------------------
/** _MSP_WritePage
 *  CLI write page data function.
 *  @param  u4Block     block number.
 *  @param  u4PgNo     page number.
 *  @param  pu4Result  point to result buffer.
 *  @param  u4BufPtr    data buffer address.
 *  @retval  S_OK  Success.
 *  @retval  Others  Fail.
 */
//-------------------------------------------------------------------------
INT32 _MSP_WritePage(UINT32 u4Block, UINT32 u4PgNo, UINT32 u4Extra, UINT32 u4BufPtr)
{
    UINT8 au1Extra[0x10];

    INT32 i4Ret, i;

    i4Ret = 1;

    for (i = 0; i < 4; i++)
    {
        au1Extra[i] = (u4Extra >> (8 *i)) & 0x0ff;
    }

    for (i = 4; i < 9; i++)
    {
        au1Extra[i] = 0x0ff;
    }

    if (u4BufPtr == 0)
    {
        i4Ret = MSP_MsWriteSinglePage(u4Block, u4PgNo, NULL, au1Extra, CMD_OVERWRITE);
    }
    else
    {
        i4Ret = MSP_MsWriteSinglePage(u4Block, u4PgNo, (UINT8 *)u4BufPtr, au1Extra, CMD_SINGLE_ACC);
    }

    LOG(9, "WritePage(Blk:%d, Pg:%d) return %d\n", u4Block, u4PgNo, i4Ret);
    return i4Ret;
}

//-------------------------------------------------------------------------
/** MS_MsproReadAttribute
*  MSPro card read attribute information
*  @param  pu1Buf pointer of buffer.
*  @return  the result of read attribute.
*/
//-------------------------------------------------------------------------

INT32 _MSP_MsproReadAttribute(UINT8 *pu1Buf)
{
    return _Msp_MsproReadAttribute(pu1Buf);
}

#if BOOT_BLOCK_RECOVERY

//-------------------------------------------------------------------------
/** _MSP_BootWrite
 *  CLI set boot record to default function.
 *  @param  VOID.
 *  @retval  S_OK  Success.
 *  @retval  Others  Fail.
 */
//-------------------------------------------------------------------------

INT32 _MSP_BootWrite(VOID)
{
    MSP_T *prMSP;

    MS_BATR_T *prMsBat;
    UINT16 *pu2;
    INT32 i, i4Ret;

    rMsBootAtr.u2BlockId = 1;
    rMsBootAtr.u2Format = 0x0100;
    rMsBootAtr.au1HdrReserved1[0] = 0; /* XXX */
    rMsBootAtr.u1DataEntry = 0x2;
    rMsBootAtr.au1HdrReserved2[0] = 0; /* XXX */
    rMsBootAtr.u4DisBlkStartAddr = 0;
    rMsBootAtr.u4DisBlkSz = 0x0a;
    rMsBootAtr.u1DisBlkId = 0x01;
    rMsBootAtr.au1SysReserved1[0] = 0; /* XXX */
    rMsBootAtr.u4CisIdiStartAddr = 0x0200;
    rMsBootAtr.u4CisIdiSz = 0x0200;
    rMsBootAtr.u1CisIdiId = 0x0a;
    rMsBootAtr.au1SysReserved2[0] = 0; /* XXX */
    rMsBootAtr.au1SysReserved3[0] = 0; /* XXX */
    rMsBootAtr.u1MSClass = 0x01;
    rMsBootAtr.u1FmtUVal2 = 0x02;
    rMsBootAtr.u2BlkSz = 0x10;
    rMsBootAtr.u2BlkNumber = 0x2000;
    rMsBootAtr.u2EffBlkNumber = 0x1f00;
    rMsBootAtr.u2PageSz = 0x0200;
    rMsBootAtr.u1ExtraSz = 0x10;
    rMsBootAtr.u1FmtUVal1 = 0x1;
    rMsBootAtr.au1AssemblyDate[0] = 0x24;
    rMsBootAtr.au1AssemblyDate[1] = 0x07;
    rMsBootAtr.au1AssemblyDate[2] = 0xd4;
    rMsBootAtr.au1AssemblyDate[3] = 0x09;
    rMsBootAtr.au1AssemblyDate[4] = 0x08;
    rMsBootAtr.au1AssemblyDate[5] = 0x0c;
    rMsBootAtr.au1AssemblyDate[6] = 0x2b;
    rMsBootAtr.au1AssemblyDate[7] = 0x38;
    rMsBootAtr.u1FmtUVal0 = 0x0;
    rMsBootAtr.au1ManuArea[0] = 0xa6;
    rMsBootAtr.au1ManuArea[1] = 0x52;
    rMsBootAtr.au1ManuArea[2] = 0xb3;
    rMsBootAtr.u1AssemblyManuCode = 0x1;
    rMsBootAtr.au1AssemblyModel[0] = 0x1;
    rMsBootAtr.au1AssemblyModel[1] = 0x28;
    rMsBootAtr.au1AssemblyModel[2] = 0x0;
    rMsBootAtr.u2MemoryManu = 0x0;
    rMsBootAtr.u2MemoryDev = 0x0;
    rMsBootAtr.u2TotalMB = 0x80;
    rMsBootAtr.u1FmtUVal1_1 = 0x1;
    rMsBootAtr.u1FmtUVal1_2 = 0x1;
    rMsBootAtr.u1VCC = 0x21;
    rMsBootAtr.u1VPP = 0x21;
    rMsBootAtr.u2CtrlNumber = 0xf0;
    rMsBootAtr.u2CtrlFunction = 0x1001;
    rMsBootAtr.au1Reserved[0] = 0; /* XXX */
    rMsBootAtr.u1InterfaceType = 0x1;
    rMsBootAtr.u2FmtUVal0_1 = 0x0;
    rMsBootAtr.u1FormatType = 0x1;
    rMsBootAtr.u1Application = 0x0;
    rMsBootAtr.u1DevType = 0x0;
    rMsBootAtr.au1Reserved1[0] = 0; /* XXX */
    rMsBootAtr.u1FmtUVala = 0xa;
    rMsBootAtr.u1FmtUVal1_3 = 0x1;
    rMsBootAtr.au1Reserved2[0] = 0; /* XXX */
    rMsBootAtr.u2InfoBlkNo = 2;
    i = 0;
    rMsBootAtr.au2InitDefectBlkNo[0] = 0x3fe;
    rMsBootAtr.au2InitDefectBlkNo[1] = 0x3ff;
    rMsBootAtr.au2InitDefectBlkNo[2] = 0x1ffe;
    rMsBootAtr.au2InitDefectBlkNo[3] = 0x1fff;

    for (i = 4; i < 255; i++)
        rMsBootAtr.au2InitDefectBlkNo[i] = 0xffff;

    rMsBootAtr.au1CisDev[0] = 0x01;
    rMsBootAtr.au1CisDev[1] = 0x04;
    rMsBootAtr.au1CisDev[2] = 0xdf;
    rMsBootAtr.au1CisDev[3] = 0x4a;
    rMsBootAtr.au1CisDev[4] = 0x01;
    rMsBootAtr.au1CisDev[5] = 0xff;
    rMsBootAtr.au1CisDevOc[0] = 0x1c;
    rMsBootAtr.au1CisDevOc[1] = 0x04;
    rMsBootAtr.au1CisDevOc[2] = 0x02;
    rMsBootAtr.au1CisDevOc[3] = 0x4a;
    rMsBootAtr.au1CisDevOc[4] = 0x01;
    rMsBootAtr.au1CisDevOc[5] = 0xff;
    rMsBootAtr.au1CisJedec_C[0] = 0x18;
    rMsBootAtr.au1CisJedec_C[1] = 0x02;
    rMsBootAtr.au1CisJedec_C[2] = 0xdf;
    rMsBootAtr.au1CisJedec_C[3] = 0x01;
    rMsBootAtr.au1CisManfid[0] = 0x20;
    rMsBootAtr.au1CisManfid[1] = 0x04;
    rMsBootAtr.au1CisManfid[2] = 0xf1;
    rMsBootAtr.au1CisManfid[3] = 0x00;
    rMsBootAtr.au1CisManfid[4] = 0x00;
    rMsBootAtr.au1CisManfid[5] = 0x00;
    rMsBootAtr.au1CisVers_1[0x00] = 0x15;
    rMsBootAtr.au1CisVers_1[0x01] = 0x1e;
    rMsBootAtr.au1CisVers_1[0x02] = 0x04;
    rMsBootAtr.au1CisVers_1[0x03] = 0x01;
    rMsBootAtr.au1CisVers_1[0x04] = 0x53;
    rMsBootAtr.au1CisVers_1[0x05] = 0x4f;
    rMsBootAtr.au1CisVers_1[0x06] = 0x4e;
    rMsBootAtr.au1CisVers_1[0x07] = 0x59;
    rMsBootAtr.au1CisVers_1[0x08] = 0x00;
    rMsBootAtr.au1CisVers_1[0x09] = 0x4d;
    rMsBootAtr.au1CisVers_1[0x0a] = 0x45;
    rMsBootAtr.au1CisVers_1[0x0b] = 0x4d;
    rMsBootAtr.au1CisVers_1[0x0c] = 0x4f;
    rMsBootAtr.au1CisVers_1[0x0d] = 0x52;
    rMsBootAtr.au1CisVers_1[0x0e] = 0x59;
    rMsBootAtr.au1CisVers_1[0x0f] = 0x53;
    rMsBootAtr.au1CisVers_1[0x00] = 0x54;
    rMsBootAtr.au1CisVers_1[0x01] = 0x49;
    rMsBootAtr.au1CisVers_1[0x02] = 0x43;
    rMsBootAtr.au1CisVers_1[0x03] = 0x4b;
    rMsBootAtr.au1CisVers_1[0x04] = 0x28;
    rMsBootAtr.au1CisVers_1[0x05] = 0x31;
    rMsBootAtr.au1CisVers_1[0x06] = 0x32;
    rMsBootAtr.au1CisVers_1[0x07] = 0x38;
    rMsBootAtr.au1CisVers_1[0x08] = 0x4d;
    rMsBootAtr.au1CisVers_1[0x09] = 0x29;
    rMsBootAtr.au1CisVers_1[0x0a] = 0x00;
    rMsBootAtr.au1CisVers_1[0x0b] = 0x31;
    rMsBootAtr.au1CisVers_1[0x0c] = 0x2e;
    rMsBootAtr.au1CisVers_1[0x0d] = 0x30;
    rMsBootAtr.au1CisVers_1[0x0e] = 0x00;
    rMsBootAtr.au1CisVers_1[0x0f] = 0xff;
    rMsBootAtr.au1CisFuncCid[0] = 0x21;
    rMsBootAtr.au1CisFuncCid[1] = 0x02;
    rMsBootAtr.au1CisFuncCid[2] = 0x01;
    rMsBootAtr.au1CisFuncCid[3] = 0x01;
    rMsBootAtr.au1CisFuncCe[0] = 0x22;
    rMsBootAtr.au1CisFuncCe[1] = 0x2;
    rMsBootAtr.au1CisFuncCe[2] = 0x1;
    rMsBootAtr.au1CisFuncCe[3] = 0x1;
    rMsBootAtr.au1CisFuncCe2[0] = 0x22;
    rMsBootAtr.au1CisFuncCe2[1] = 0x3;
    rMsBootAtr.au1CisFuncCe2[2] = 0x2;
    rMsBootAtr.au1CisFuncCe2[3] = 0xc;
    rMsBootAtr.au1CisFuncCe2[4] = 0xf;
    rMsBootAtr.au1CisConfig[0] = 0x1a;
    rMsBootAtr.au1CisConfig[1] = 0x05;
    rMsBootAtr.au1CisConfig[2] = 0x01;
    rMsBootAtr.au1CisConfig[3] = 0x03;
    rMsBootAtr.au1CisConfig[4] = 0x00;
    rMsBootAtr.au1CisConfig[5] = 0x02;
    rMsBootAtr.au1CisConfig[6] = 0x0f;
    rMsBootAtr.au1CisSomething[0] = 0x1b;
    rMsBootAtr.au1CisSomething[1] = 0x08;
    rMsBootAtr.au1CisSomething[2] = 0xc0;
    rMsBootAtr.au1CisSomething[3] = 0xc0;
    rMsBootAtr.au1CisSomething[4] = 0xa1;
    rMsBootAtr.au1CisSomething[5] = 0x01;
    rMsBootAtr.au1CisSomething[6] = 0x55;
    rMsBootAtr.au1CisSomething[7] = 0x08;
    rMsBootAtr.au1CisSomething[8] = 0x00;
    rMsBootAtr.au1CisSomething[9] = 0x20;
    rMsBootAtr.au1CisSomething[10] = 0x1b;
    rMsBootAtr.au1CisSomething[11] = 0x06;
    rMsBootAtr.au1CisSomething[12] = 0x00;
    rMsBootAtr.au1CisSomething[13] = 0x01;
    rMsBootAtr.au1CisSomething[14] = 0x21;
    rMsBootAtr.au1CisSomething[15] = 0xb5;
    rMsBootAtr.au1CisSomething[16] = 0x1e;
    rMsBootAtr.au1CisSomething[17] = 0x0e;
    rMsBootAtr.au1CisSomething[18] = 0x1b;
    rMsBootAtr.au1CisSomething[19] = 0x0a;
    rMsBootAtr.au1CisSomething[20] = 0xc1;
    rMsBootAtr.au1CisSomething[21] = 0x41;
    rMsBootAtr.au1CisSomething[22] = 0x99;
    rMsBootAtr.au1CisSomething[23] = 0x01;
    rMsBootAtr.au1CisSomething[24] = 0x55;
    rMsBootAtr.au1CisSomething[25] = 0x64;
    rMsBootAtr.au1CisSomething[26] = 0xf0;
    rMsBootAtr.au1CisSomething[27] = 0xff;
    rMsBootAtr.au1CisSomething[28] = 0xff;
    rMsBootAtr.au1CisSomething[29] = 0x20;
    rMsBootAtr.au1CisSomething[30] = 0x1b;
    rMsBootAtr.au1CisSomething[31] = 0x41;
    rMsBootAtr.au1CisSomething[32] = 0x06;
    rMsBootAtr.au1CisSomething[33] = 0x01;
    rMsBootAtr.au1CisSomething[34] = 0x21;
    rMsBootAtr.au1CisSomething[35] = 0xb5;
    rMsBootAtr.au1CisSomething[36] = 0x1e;
    rMsBootAtr.au1CisSomething[37] = 0x4d;
    rMsBootAtr.au1CisSomething[38] = 0x1b;
    rMsBootAtr.au1CisSomething[39] = 0x0f;
    rMsBootAtr.au1CisSomething[40] = 0xc2;
    rMsBootAtr.au1CisSomething[41] = 0x41;
    rMsBootAtr.au1CisSomething[42] = 0x99;
    rMsBootAtr.au1CisSomething[43] = 0x01;
    rMsBootAtr.au1CisSomething[44] = 0x55;
    rMsBootAtr.au1CisSomething[45] = 0xea;
    rMsBootAtr.au1CisSomething[46] = 0x61;
    rMsBootAtr.au1CisSomething[47] = 0xf0;
    rMsBootAtr.au1CisSomething[48] = 0x01;
    rMsBootAtr.au1CisSomething[49] = 0x07;
    rMsBootAtr.au1CisSomething[50] = 0xf6;
    rMsBootAtr.au1CisSomething[51] = 0x03;
    rMsBootAtr.au1CisSomething[52] = 0x01;
    rMsBootAtr.au1CisSomething[53] = 0xee;
    rMsBootAtr.au1CisSomething[54] = 0x20;
    rMsBootAtr.au1CisSomething[55] = 0x1b;
    rMsBootAtr.au1CisSomething[56] = 0x06;
    rMsBootAtr.au1CisSomething[57] = 0x02;
    rMsBootAtr.au1CisSomething[58] = 0x01;
    rMsBootAtr.au1CisSomething[59] = 0x21;
    rMsBootAtr.au1CisSomething[60] = 0xb5;
    rMsBootAtr.au1CisSomething[61] = 0x1e;
    rMsBootAtr.au1CisSomething[62] = 0x4d;
    rMsBootAtr.au1CisSomething[63] = 0x1b;
    rMsBootAtr.au1CisSomething[64] = 0x0f;
    rMsBootAtr.au1CisSomething[65] = 0xc3;
    rMsBootAtr.au1CisSomething[66] = 0x41;
    rMsBootAtr.au1CisSomething[67] = 0x99;
    rMsBootAtr.au1CisSomething[68] = 0x01;
    rMsBootAtr.au1CisSomething[69] = 0x55;
    rMsBootAtr.au1CisSomething[70] = 0xea;
    rMsBootAtr.au1CisSomething[71] = 0x61;
    rMsBootAtr.au1CisSomething[72] = 0x70;
    rMsBootAtr.au1CisSomething[73] = 0x01;
    rMsBootAtr.au1CisSomething[74] = 0x07;
    rMsBootAtr.au1CisSomething[75] = 0x76;
    rMsBootAtr.au1CisSomething[76] = 0x03;
    rMsBootAtr.au1CisSomething[77] = 0x01;
    rMsBootAtr.au1CisSomething[78] = 0xee;
    rMsBootAtr.au1CisSomething[79] = 0x20;
    rMsBootAtr.au1CisSomething[80] = 0x1b;
    rMsBootAtr.au1CisSomething[81] = 0x06;
    rMsBootAtr.au1CisSomething[82] = 0x03;
    rMsBootAtr.au1CisSomething[83] = 0x01;
    rMsBootAtr.au1CisSomething[84] = 0x21;
    rMsBootAtr.au1CisSomething[85] = 0xb5;
    rMsBootAtr.au1CisSomething[86] = 0x1e;
    rMsBootAtr.au1CisSomething[87] = 0x4d;
    rMsBootAtr.au1CisSomething[88] = 0x14;
    rMsBootAtr.au1CisSomething[89] = 0x00;
    rMsBootAtr.u1CisEnd = 0xff;
    rMsBootAtr.au1CisReserved[0] = 0;
    rMsBootAtr.u2Config = 0x848a;
    rMsBootAtr.u2Cylinders = 0x03df;
    rMsBootAtr.u2Reserved1 = 0;
    rMsBootAtr.u2Headers = 0x0010;
    rMsBootAtr.u2BytesInTrack = 0;
    rMsBootAtr.u2BytesInSectors = 0x0200;
    rMsBootAtr.u2SectorsInTrack = 0x0010;
    rMsBootAtr.u2MSWSectors = 0x03;
    rMsBootAtr.u2LSWSectors = 0xdf00;
    rMsBootAtr.u2Reserved2 = 0;
    rMsBootAtr.au1SerialNumber[0] = 0x20;
    rMsBootAtr.au1SerialNumber[1] = 0x20;
    rMsBootAtr.au1SerialNumber[2] = 0x20;
    rMsBootAtr.au1SerialNumber[3] = 0x20;
    rMsBootAtr.au1SerialNumber[4] = 0x20;
    rMsBootAtr.au1SerialNumber[5] = 0x20;
    rMsBootAtr.au1SerialNumber[6] = 0x20;
    rMsBootAtr.au1SerialNumber[7] = 0x20;
    rMsBootAtr.au1SerialNumber[8] = 0x20;
    rMsBootAtr.au1SerialNumber[9] = 0x20;
    rMsBootAtr.au1SerialNumber[10] = 0x20;
    rMsBootAtr.au1SerialNumber[11] = 0x20;
    rMsBootAtr.au1SerialNumber[12] = 0x20;
    rMsBootAtr.au1SerialNumber[13] = 0x20;
    rMsBootAtr.au1SerialNumber[14] = 0x20;
    rMsBootAtr.au1SerialNumber[15] = 0x20;
    rMsBootAtr.au1SerialNumber[16] = 0x20;
    rMsBootAtr.au1SerialNumber[17] = 0x20;
    rMsBootAtr.au1SerialNumber[18] = 0x20;
    rMsBootAtr.au1SerialNumber[19] = 0x20;
    rMsBootAtr.u2BufType = 0x02;
    rMsBootAtr.u2BufSz = 0x02;
    rMsBootAtr.u2LongCmdEcc = 0x04;
    rMsBootAtr.au1FirmVer[0] = 0x53;
    rMsBootAtr.au1FirmVer[1] = 0x4f;
    rMsBootAtr.au1FirmVer[2] = 0x4e;
    rMsBootAtr.au1FirmVer[3] = 0x59;
    rMsBootAtr.au1FirmVer[4] = 0x31;
    rMsBootAtr.au1FirmVer[5] = 0x2e;
    rMsBootAtr.au1FirmVer[6] = 0x30;
    rMsBootAtr.au1FirmVer[7] = 0x30;
    rMsBootAtr.au1ModelName[0] = 0x4d;
    rMsBootAtr.au1ModelName[1] = 0x45;
    rMsBootAtr.au1ModelName[2] = 0x4d;
    rMsBootAtr.au1ModelName[3] = 0x4f;
    rMsBootAtr.au1ModelName[4] = 0x52;
    rMsBootAtr.au1ModelName[5] = 0x59;
    rMsBootAtr.au1ModelName[6] = 0x53;
    rMsBootAtr.au1ModelName[7] = 0x54;
    rMsBootAtr.au1ModelName[8] = 0x49;
    rMsBootAtr.au1ModelName[9] = 0x43;
    rMsBootAtr.au1ModelName[10] = 0x4b;
    rMsBootAtr.au1ModelName[11] = 0x20;
    rMsBootAtr.au1ModelName[12] = 0x31;
    rMsBootAtr.au1ModelName[13] = 0x32;
    rMsBootAtr.au1ModelName[14] = 0x38;
    rMsBootAtr.au1ModelName[15] = 0x4d;
    rMsBootAtr.au1ModelName[16] = 0x20;
    rMsBootAtr.au1ModelName[17] = 0x31;
    rMsBootAtr.au1ModelName[18] = 0x36;
    rMsBootAtr.au1ModelName[19] = 0x4b;
    rMsBootAtr.au1ModelName[20] = 0x20;
    rMsBootAtr.au1ModelName[21] = 0x20;
    rMsBootAtr.au1ModelName[22] = 0x20;
    rMsBootAtr.au1ModelName[23] = 0x20;
    rMsBootAtr.au1ModelName[24] = 0x20;
    rMsBootAtr.au1ModelName[25] = 0x20;
    rMsBootAtr.au1ModelName[26] = 0x20;
    rMsBootAtr.au1ModelName[27] = 0x20;
    rMsBootAtr.au1ModelName[28] = 0x20;
    rMsBootAtr.au1ModelName[29] = 0x20;
    rMsBootAtr.au1ModelName[30] = 0x20;
    rMsBootAtr.au1ModelName[31] = 0x20;
    rMsBootAtr.au1ModelName[32] = 0x20;
    rMsBootAtr.au1ModelName[33] = 0x20;
    rMsBootAtr.au1ModelName[34] = 0x20;
    rMsBootAtr.au1ModelName[35] = 0x20;
    rMsBootAtr.au1ModelName[36] = 0x20;
    rMsBootAtr.au1ModelName[37] = 0x20;
    rMsBootAtr.au1ModelName[38] = 0x20;
    rMsBootAtr.au1ModelName[39] = 0x00;
    rMsBootAtr.u2Reserved3 = 0x1;
    rMsBootAtr.u2DualSup = 0;
    rMsBootAtr.u2DmaSup = 0x200;
    rMsBootAtr.u2Reserved4 = 0x0;
    rMsBootAtr.u2PioMode = 0x100;
    rMsBootAtr.u2DmaMode = 0;
    rMsBootAtr.u2FieldValid = 0x1;
    rMsBootAtr.u2CurCylinders = 0x03df;
    rMsBootAtr.u2CurHeads = 0x10;
    rMsBootAtr.u2CurSectorsInTrack = 0x10;
    rMsBootAtr.u4CurTotalSectors = 0x0003df00;
    rMsBootAtr.u2MultiSectorSetting = 0x0100;
    rMsBootAtr.u4TotalUserSectors = 0x0003df00;
    rMsBootAtr.u2SingleDma = 0;
    rMsBootAtr.u2MultiDma = 0;
    rMsBootAtr.au1Reserved3[0] = 0; /* XXX */

    prMsBat = &rMsBootAtr;
    REVERSE_UINT16(prMsBat->u2BlockId);
    REVERSE_UINT16(prMsBat->u2Format);
    REVERSE_UINT32(prMsBat->u4DisBlkStartAddr);
    REVERSE_UINT32(prMsBat->u4DisBlkSz);
    REVERSE_UINT32(prMsBat->u4CisIdiStartAddr);
    REVERSE_UINT32(prMsBat->u4CisIdiSz);
    REVERSE_UINT16(prMsBat->u2BlkSz);
    REVERSE_UINT16(prMsBat->u2BlkNumber);
    REVERSE_UINT16(prMsBat->u2EffBlkNumber);
    REVERSE_UINT16(prMsBat->u2PageSz);
    REVERSE_UINT16(prMsBat->u2MemoryManu);
    REVERSE_UINT16(prMsBat->u2MemoryDev);
    REVERSE_UINT16(prMsBat->u2TotalMB);
    REVERSE_UINT16(prMsBat->u2CtrlNumber);
    REVERSE_UINT16(prMsBat->u2CtrlFunction);
    pu2 = ((UINT16 *)(&(prMsBat->u2InfoBlkNo)));

    for (i = 0; i < 256; i++)
    {
        REVERSE_UINT16(pu2[i]);
    }

    pu2 = ((UINT16 *)(&(prMsBat->au1FirmVer)));

    for (i = 0; i < ((sizeof(prMsBat->au1FirmVer) + sizeof(prMsBat->au1ModelName)) >> 1); i++)
    {
        REVERSE_UINT16(pu2[i]);
    }

    for (i = 0; i < 0x30; i++)
        au1Extra[i] = 0xff;

    au1Extra[0] = 0xc0;
    au1Extra[1] = 0xfb;
    au1Extra[9] = 0xc0;
    au1Extra[10] = 0xfb;
    au1Extra[18] = 0xc0;
    au1Extra[19] = 0xfb;

    prMSP = FCI_GetMSPSlot();
    i4Ret = MSP_MsWriteSinglePage(0, 0, ((UINT8 *)&rMsBootAtr), au1Extra, CMD_SINGLE_ACC);
    LOG(1, "WriteSingle(0, 0) return %d\n", i4Ret);

    if (i4Ret != S_OK)
    {
        return i4Ret;
    }

    i4Ret = MSP_MsWriteSinglePage(0, 1, ((UINT8 *)&rMsBootAtr) + 512, au1Extra, CMD_SINGLE_ACC);
    LOG(1, "WriteSingle(0, 1) return %d\n", i4Ret);

    if (i4Ret != S_OK)
    {
        return i4Ret;
    }

    i4Ret = MSP_MsWriteSinglePage(0, 2, ((UINT8 *)&rMsBootAtr) + 1024, au1Extra, CMD_SINGLE_ACC);
    LOG(1, "WriteSingle(0, 2) return %d\n", i4Ret);

    if (i4Ret != S_OK)
    {
        return i4Ret;
    }

    i4Ret = MSP_MsWriteSinglePage(1, 0, ((UINT8 *)&rMsBootAtr), au1Extra, CMD_SINGLE_ACC);
    LOG(1, "WriteSingle(1, 0) return %d\n", i4Ret);

    if (i4Ret != S_OK)
    {
        return i4Ret;
    }

    i4Ret = MSP_MsWriteSinglePage(1, 1, ((UINT8 *)&rMsBootAtr) + 512, au1Extra, CMD_SINGLE_ACC);
    LOG(1, "WriteSingle(1, 1) return %d\n", i4Ret);

    if (i4Ret != S_OK)
    {
        return i4Ret;
    }

    i4Ret = MSP_MsWriteSinglePage(1, 2, ((UINT8 *)&rMsBootAtr) + 1024, au1Extra, CMD_SINGLE_ACC);
    LOG(1, "WriteSingle(1, 2) return %d\n", i4Ret);

    return i4Ret;
}

INT32 _MSP_BootRead(VOID)
{
    MSP_T *prMSP;
    INT32 i4Ret;

    prMSP = FCI_GetMSPSlot();
    i4Ret = MSP_MsReadSinglePage(0, 0, ((UINT8 *)&rMsBootAtr), au1Extra, CMD_SINGLE_ACC);
    LOG(1, "ReadSingle(0, 0) return %d\n", i4Ret);

    if (i4Ret != S_OK)
    {
        return i4Ret;
    }

    i4Ret = MSP_MsReadSinglePage(0, 1, ((UINT8 *)&rMsBootAtr) + 512, au1Extra, CMD_SINGLE_ACC);
    LOG(1, "ReadSingle(0, 1) return %d\n", i4Ret);

    if (i4Ret != S_OK)
    {
        return i4Ret;
    }

    i4Ret = MSP_MsReadSinglePage(0, 2, ((UINT8 *)&rMsBootAtr) + 1024, au1Extra, CMD_SINGLE_ACC);
    LOG(1, "ReadSingle(0, 2) return %d\n", i4Ret);

    if (i4Ret != S_OK)
    {
        return i4Ret;
    }

    i4Ret = MSP_MsReadSinglePage(1, 0, ((UINT8 *)&rMsBootAtr), au1Extra, CMD_SINGLE_ACC);
    LOG(1, "ReadSingle(1, 0) return %d\n", i4Ret);

    if (i4Ret != S_OK)
    {
        return i4Ret;
    }

    i4Ret = MSP_MsReadSinglePage(1, 1, ((UINT8 *)&rMsBootAtr) + 512, au1Extra, CMD_SINGLE_ACC);
    LOG(1, "ReadSingle(1, 1) return %d\n", i4Ret);

    if (i4Ret != S_OK)
    {
        return i4Ret;
    }

    i4Ret = MSP_MsReadSinglePage(1, 2, ((UINT8 *)&rMsBootAtr) + 1024, au1Extra, CMD_SINGLE_ACC);
    LOG(1, "ReadSingle(1, 2) return %d\n", i4Ret);

    return i4Ret;
}

#else

//-------------------------------------------------------------------------
/** _MSP_BootWrite
 *  CLI set boot record to default function.
 *  @param  VOID.
 *  @retval  S_OK  Success.
 *  @retval  Others  Fail.
 */
//-------------------------------------------------------------------------

INT32 _MSP_BootWrite(VOID)
{
    LOG(1, "Disable BootWrite function\n");

    return S_OK;
}
#endif

//-------------------------------------------------------------------------
/** _MSP_ReadBlock
 *  CLI read block function.
 *  @param  u4Block                block number.
 *  @param  u4ExtraMemBuf    extra memory address.
 *  @param  u4DataMemBuf     data memory address.
 *  @retval  S_OK  Success.
 *  @retval  Others  Fail.
 */
//-------------------------------------------------------------------------

INT32 _MSP_ReadBlock(UINT32 u4Block, UINT32 u4ExtraMemBuf, UINT32 u4DataMemBuf)
{
    INT32 i4Ret;

    UINT32 u4BlkNum;
    MSP_T *prMSP;

    prMSP = FCI_GetMSPSlot();
    // Calculate total page number in one block.
    u4BlkNum = 2 * (prMSP->uATR.rMsBootAtr.u2BlkSz);
    i4Ret = MSP_MsReadBlock(u4Block, 0, u4BlkNum, (UINT8 *)u4DataMemBuf, (UINT8 *)u4ExtraMemBuf);
    return i4Ret;
}

//-------------------------------------------------------------------------
/** _MSP_WriteBlock
 *  CLI write block function.
 *  @param  u4Block                block number.
 *  @param  u4ExtraMemBuf    extra memory address.
 *  @param  u4DataMemBuf     data memory address.
 *  @retval  S_OK  Success.
 *  @retval  Others  Fail.
 */
//-------------------------------------------------------------------------
INT32 _MSP_WriteBlock(UINT32 u4Block, UINT32 u4ExtraMemBuf, UINT32 u4DataMemBuf)
{
    INT32 i4Ret;

    UINT32 u4BlkNum;
    MSP_T *prMSP;

    prMSP = FCI_GetMSPSlot();
    // Calculate total page number in one block.
    u4BlkNum = 2 * (prMSP->uATR.rMsBootAtr.u2BlkSz);
    i4Ret = MSP_MsWriteBlock(u4Block, 0, u4BlkNum, (UINT8 *)u4DataMemBuf, (UINT8 *)u4ExtraMemBuf);
    return i4Ret;
}

//-------------------------------------------------------------------------
/** _MSP_EraseBlock
 *  CLI erase block function.
 *  @param  u4Block                block number.
 *  @retval  S_OK  Success.
 *  @retval  Others  Fail.
 */
//-------------------------------------------------------------------------
INT32 _MSP_EraseBlock(UINT32 u4Block)
{
    return MSP_MsEraseBlock(u4Block);
}

//-------------------------------------------------------------------------
/** _MSP_SetParallel
 *  CLI set to parallel interface function.
 *  @param  VOID.
 *  @retval  S_OK  Success.
 *  @retval  Others  Fail.
 */
//-------------------------------------------------------------------------
INT32 _MSP_SetParallel(VOID)
{
    return _Msp_SwitchParallel();
}

//-------------------------------------------------------------------------
/** _MSP_SetSerial
 *  CLI set to serial interface function.
 *  @param  VOID.
 *  @retval  S_OK  Success.
 *  @retval  Others  Fail.
 */
//-------------------------------------------------------------------------
INT32 _MSP_SetSerial(VOID)
{
    return _Msp_SwitchSerial();
}

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

//-------------------------------------------------------------------------
/** MSP_MsReadSinglePage
 *  Read MS single page = 512 bytes.
 *  @param  u4BlkNo     block number
 *  @param  u4PgNo      page number
 *  @param  pu1Buf       point to buffer address.
 *  @param  pu1Extra    point to extra data register.
 *  @param  u4Mode     access mode.
 *  @retval  S_OK  Success.
 *  @retval  Others  Fail.
 */
//-------------------------------------------------------------------------
INT32 MSP_MsReadSinglePage(UINT32 u4BlkNo, UINT32 u4PgNo, UINT8 *pu1Buf, UINT8 *pu1Extra,
                           UINT32 u4Mode)
{
    INT32 i4Ret, i;

    UINT32 u4RdAddr, u4RdSz, u4WrAddr, u4WrSz;

    ASSERT(_prFCIMSP != NULL);

    if (_prFCIMSP->fg4BitEn)
    {
        ASSERT((u4PgNo) < (2 *(_prFCIMSP->uATR.rMsBootAtr.u2BlkSz)));

        ASSERT((u4BlkNo) < (_prFCIMSP->uATR.rMsBootAtr.u2BlkNumber));

        /* Parallel mode, read from start register to the end one. */
        u4RdAddr = REG_INT;
        u4RdSz = ((REG_RESERVED_0 - REG_INT) + 1);
    }
    else
    {
        /* Serial mode, only read the extra data. */
        u4RdAddr = REG_OVERWRITE_FLAG;
        u4RdSz = ((REG_RESERVED_0 - REG_OVERWRITE_FLAG) + 1);
    }

    u4WrAddr = REG_SYSTEM_PARAM;
    u4WrSz = ((REG_PAGE_ADDR - REG_SYSTEM_PARAM) + 1);

    i4Ret = _Msp_TpcSetRWRegAdrs(u4RdAddr, u4RdSz, u4WrAddr, u4WrSz);

    if (i4Ret != S_OK)
    {
        LOG(1, "%s(%d) SetRWReg failed ret:%d\n", __FILE__, __LINE__, i4Ret);
        return i4Ret;
    }

    _prFCIMSP->au1MsReg[REG_SYSTEM_PARAM] = (SYSP_ON | ((_prFCIMSP->fg4BitEn) ? (SYSP_PAM) : 0));
    _prFCIMSP->au1MsReg[REG_BLOCK_ADDR_2] = (u4BlkNo >> 16) & 0x0ff;
    _prFCIMSP->au1MsReg[REG_BLOCK_ADDR_1] = (u4BlkNo >> 8) & 0x0ff;
    _prFCIMSP->au1MsReg[REG_BLOCK_ADDR_0] = (u4BlkNo >> 0) & 0x0ff;
    _prFCIMSP->au1MsReg[REG_CMD_PARAM] = (u4Mode & 0x0ff);
    _prFCIMSP->au1MsReg[REG_PAGE_ADDR] = (u4PgNo & 0x0ff);

    i4Ret = _Msp_TpcWriteReg(&(_prFCIMSP->au1MsReg[u4WrAddr]), u4WrSz);

    if (i4Ret != S_OK)
    {
        LOG(1, "%s(%d) Write Reg failed ret:%d\n", __FILE__, __LINE__, i4Ret);
        return i4Ret;
    }

    i4Ret = _Msp_TpcSetCmd(CMD_BLOCK_READ);

    if (i4Ret != S_OK)
    {
        LOG(1, "%s(%d) SetCmd BLOCK_READ failed ret:%d\n", __FILE__, __LINE__, i4Ret);
        return i4Ret;
    }

    // Serial interface: Get INT result by TPC.
    if (_prFCIMSP->fg4BitEn == 0)
    {
        i4Ret = _Msp_TpcGetInt(&(_prFCIMSP->au1MsReg[REG_INT]));

        if (i4Ret != S_OK)
        {
            LOG(1, "%s(%d) GetInt failed ret:%d\n", __FILE__, __LINE__, i4Ret);
            return i4Ret;
        }
        LOG(9, "%s(%d) Serial REG_INT:0x%02x\n", __FUNCTION__, __LINE__,
        _prFCIMSP->au1MsReg[REG_INT]);
    }
    else
    {
        // Parallel interface check by H/W INT DATA[3:0].
        _prFCIMSP->au1MsReg[REG_INT] = MSPHW_ReadParallelSif();
        LOG(9, "%s(%d) Parallel INT:0x%02x\n", __FUNCTION__, __LINE__,
        _prFCIMSP->au1MsReg[REG_INT]);
    }

    // read all status register.
    i4Ret = _Msp_TpcReadReg(&(_prFCIMSP->au1MsReg[u4RdAddr]), u4RdSz);

    if (i4Ret != S_OK)
    {
        LOG(1, "%s(%d) Read Reg failed ret:%d\n", __FILE__, __LINE__, i4Ret);
        return i4Ret;
    }

    // CMD can not be executed.
    if (_prFCIMSP->au1MsReg[REG_INT] & INT_CMDNK)
    {
        LOG(1, "(%d,%d) REG_INT:0x%02x with INT_CMDNK:0x%02x\n", u4BlkNo, u4PgNo,
        _prFCIMSP->au1MsReg[REG_INT], INT_CMDNK);
        return E_FAIL;
    }

    // CMD  error termination.
    if (_prFCIMSP->au1MsReg[REG_INT] & INT_ERR)
    {
        LOG(7, "(%d,%d) REG_INT:0x%02x\n", u4BlkNo, u4PgNo, _prFCIMSP->au1MsReg[REG_INT]);

        // read status register 0 and status register 1.
        i4Ret = _Msp_ReadStatusReg(REG_STATUS_0, 2);

        if (i4Ret != S_OK)
        {
            LOG(1, "Read Status register failed return %d.\n", i4Ret);
            return i4Ret;
        }

        LOG(7, "Error - Status0:0x%02x Status1:0x%02x\n", _prFCIMSP->au1MsReg[REG_STATUS_0],
        _prFCIMSP->au1MsReg[REG_STATUS_1]);

        // Max Liao, add ST1_UCDT: ECC error in Non-overwrite area data  , 2006/02/09.
        //if (_prFCIMSP->au1MsReg[REG_STATUS_1]&(ST1_UCDT | ST1_UCEX | ST1_UCFG))
        if (_prFCIMSP->au1MsReg[REG_STATUS_1]&(ST1_EXER | ST1_UCDT | ST1_UCEX | ST1_UCFG))
        {
            LOG(7, "(%d,%d) Uncorrectable error\n", u4BlkNo, u4PgNo);
            return E_FAIL;
        }
        else
        {
            LOG(7, "(%d,%d) Correctable!!!\n", u4BlkNo, u4PgNo);
        }
    }

    // CMD not normal end.
    if (!(_prFCIMSP->au1MsReg[REG_INT]&INT_CED))
    {
        LOG(1, "(%d,%d) REG_INT:0x%02x without INT_CED:0x%02x\n", u4BlkNo, u4PgNo,
        _prFCIMSP->au1MsReg[REG_INT], INT_CED);
        return E_FAIL;
    }

    if (((u4Mode & 0x0ff) == CMD_SINGLE_ACC) && (!(_prFCIMSP->au1MsReg[REG_INT]&INT_BREQ)))
    {
        LOG(1, "(%d,%d) REG_INT:0x%02x without INT_BREQ:0x%02x\n", u4BlkNo, u4PgNo,
        _prFCIMSP->au1MsReg[REG_INT], INT_BREQ);
        return E_FAIL;
    }

    if ((u4Mode & 0x0ff) == CMD_SINGLE_ACC)
    {
        // Send ReadPageData Tpc to trigger DMA access flash memory.
        i4Ret = _Msp_TpcReadPageData(MS_PAGE_SIZE, pu1Buf);
        if (i4Ret != S_OK)
        {
            LOG(1, "%s(%d) ReadPageData failed ret:%d\n", __FILE__, __LINE__, i4Ret);
            return i4Ret;
        }
    }

    // read extra data register.
    ASSERT(pu1Extra != NULL);

    for (i = 0; i < 9; i++)
    {
        pu1Extra[i] = _prFCIMSP->au1MsReg[REG_OVERWRITE_FLAG + i];
    }

    return S_OK;
}

//-------------------------------------------------------------------------
/** MSP_MsWriteSinglePage
 *  Write MS single page = 512 bytes.
 *  @param  u4BlkNo     block number
 *  @param  u4PgNo      page number
 *  @param  pu1Buf       point to buffer address.
 *  @param  pu1Extra    point to extra data register.
 *  @param  u4Mode     access mode.
 *  @retval  S_OK  Success.
 *  @retval  Others  Fail.
 */
//-------------------------------------------------------------------------
INT32 MSP_MsWriteSinglePage(UINT32 u4BlkNo, UINT32 u4PgNo, UINT8 *pu1Buf, const UINT8 *pu1Extra,
                            UINT32 u4Mode)
{
    INT32 i4Ret, i;

    UINT32 u4RdAddr, u4RdSz, u4WrAddr, u4WrSz;

    ASSERT(_prFCIMSP != NULL);
    ASSERT(pu1Extra != NULL);

#if BOOT_BLOCK_RECOVERY
#else /* without RECOVERY, must do write checking. */

    ASSERT((u4PgNo) < (2 *(_prFCIMSP->uATR.rMsBootAtr.u2BlkSz)));
    ASSERT((u4BlkNo) < (_prFCIMSP->uATR.rMsBootAtr.u2BlkNumber));
#endif

    if (_prFCIMSP->fg4BitEn)
    {
        u4RdAddr = REG_INT;
        u4RdSz = ((REG_STATUS_1 - REG_INT) + 1);
    }
    else
    {
        u4RdAddr = REG_STATUS_0;
        u4RdSz = ((REG_STATUS_1 - REG_STATUS_0) + 1);
    }

    u4WrAddr = REG_SYSTEM_PARAM;
    u4WrSz = ((REG_RESERVED_0 - REG_SYSTEM_PARAM) + 1);

    /* (1) */
    i4Ret = _Msp_TpcSetRWRegAdrs(u4RdAddr, u4RdSz, u4WrAddr, u4WrSz);

    if (i4Ret != S_OK)
    {
        LOG(1, "%s(%d) SetRWReg failed ret:%d\n", __FILE__, __LINE__, i4Ret);
        return i4Ret;
    }

    _prFCIMSP->au1MsReg[REG_SYSTEM_PARAM] = (SYSP_ON | ((_prFCIMSP->fg4BitEn) ? (SYSP_PAM) : 0));
    _prFCIMSP->au1MsReg[REG_BLOCK_ADDR_2] = (u4BlkNo >> 16) & 0x0ff;
    _prFCIMSP->au1MsReg[REG_BLOCK_ADDR_1] = (u4BlkNo >> 8) & 0x0ff;
    _prFCIMSP->au1MsReg[REG_BLOCK_ADDR_0] = (u4BlkNo >> 0) & 0x0ff;
    _prFCIMSP->au1MsReg[REG_CMD_PARAM] = (u4Mode & 0x0ff);
    _prFCIMSP->au1MsReg[REG_PAGE_ADDR] = (u4PgNo & 0x0ff);

    for (i = 0; i < 9; i++)
    {
        _prFCIMSP->au1MsReg[REG_OVERWRITE_FLAG + i] = pu1Extra[i];
    }

    /* (2) */
    i4Ret = _Msp_TpcWriteReg(&(_prFCIMSP->au1MsReg[u4WrAddr]), u4WrSz);

    if (i4Ret != S_OK)
    {
        LOG(1, "%s(%d) Write Reg failed ret:%d\n", __FILE__, __LINE__, i4Ret);
        return i4Ret;
    }

    /* (3)/(4) */
    i4Ret = _Msp_TpcSetCmd(CMD_BLOCK_WRITE);

    if (i4Ret != S_OK)
    {
        LOG(1, "%s(%d) SetCmd BLOCK_WRITE failed ret:%d\n", __FILE__, __LINE__, i4Ret);
        return i4Ret;
    }

    /* (5) */
    if (_prFCIMSP->fg4BitEn == 0)
    {
        i4Ret = _Msp_TpcGetInt(&(_prFCIMSP->au1MsReg[REG_INT]));

        if (i4Ret != S_OK)
        {
            LOG(1, "%s(%d) GetInt failed ret:%d\n", __FILE__, __LINE__, i4Ret);
            return i4Ret;
        }
        LOG(9, "1. REG_INT:0x%02x\n", _prFCIMSP->au1MsReg[REG_INT]);
    }
    else
    {
        i4Ret = _Msp_TpcReadReg(&(_prFCIMSP->au1MsReg[u4RdAddr]), u4RdSz);

        if (i4Ret != S_OK)
        {
            LOG(1, "%s(%d) GetInt failed ret:%d\n", __FILE__, __LINE__, i4Ret);
            return i4Ret;
        }
        LOG(9, "2. REG_INT:0x%02x\n", _prFCIMSP->au1MsReg[REG_INT]);
    }

    if (_prFCIMSP->au1MsReg[REG_INT] & INT_CMDNK)
    {
        LOG(1, "(%d,%d) REG_INT:0x%02x failed with INT_CMDNK:0x%02x\n", u4BlkNo, u4PgNo,
        _prFCIMSP->au1MsReg[REG_INT], INT_CMDNK);
        return E_FAIL;
    }

    if ((u4Mode & 0x0ff) == CMD_SINGLE_ACC)
    {
        /* Must INT_BREQ on by Spec. */
        ASSERT(_prFCIMSP->au1MsReg[REG_INT] & INT_BREQ);

        /* (6)/(7) Send WritePageData Tpc to trigger DMA access flash memory. */
        i4Ret = _Msp_TpcWritePageData(MS_PAGE_SIZE, pu1Buf);

        if (i4Ret != S_OK)
        {
            LOG(1, "%s(%d) WritePageData failed ret:%d\n", __FILE__, __LINE__, i4Ret);
            return i4Ret;
        }

        /* (8) */
        if (_prFCIMSP->fg4BitEn == 0)
        {
            i4Ret = _Msp_TpcGetInt(&(_prFCIMSP->au1MsReg[REG_INT]));

            if (i4Ret != S_OK)
            {
                LOG(1, "%s(%d) GetInt failed ret:%d\n", __FILE__, __LINE__, i4Ret);
                return i4Ret;
            }
            LOG(9, "3. REG_INT:0x%02x\n", _prFCIMSP->au1MsReg[REG_INT]);
        }
        else
        {
            i4Ret = _Msp_TpcReadReg(&(_prFCIMSP->au1MsReg[u4RdAddr]), u4RdSz);

            if (i4Ret != S_OK)
            {
                LOG(1, "%s(%d) GetInt failed ret:%d\n", __FILE__, __LINE__, i4Ret);
                return i4Ret;
            }
            LOG(9, "4. REG_INT:0x%02x\n", _prFCIMSP->au1MsReg[REG_INT]);
        }

        if (_prFCIMSP->au1MsReg[REG_INT]&(INT_CMDNK | INT_ERR))
        {
            LOG(1, "(%d,%d) REG_INT:0x%02x failed with ERR|CMDNK:0x%02x\n", u4BlkNo, u4PgNo,
            _prFCIMSP->au1MsReg[REG_INT], INT_CMDNK | INT_ERR);
            return E_FAIL;
        }
        ASSERT(_prFCIMSP->au1MsReg[REG_INT] & INT_CED);
    }

    return S_OK;
}

//-------------------------------------------------------------------------
/** MSP_MsEraseBlock
 *  MS erase block function.
 *  @param  u4BlkNo   physical block number.
 *  @retval  S_OK  Success.
 *  @retval  Others  Fail.
 */
//-------------------------------------------------------------------------
INT32 MSP_MsEraseBlock(UINT32 u4BlkNo)
{
    INT32 i4Ret;

    UINT32 u4RdAddr, u4RdSz, u4WrAddr, u4WrSz;
    UINT32 u1RetryCnt = 0;

    ASSERT(_prFCIMSP != NULL);
    ASSERT((u4BlkNo) < (_prFCIMSP->uATR.rMsBootAtr.u2BlkNumber));

    u4RdAddr = REG_INT;
    u4RdSz = ((REG_STATUS_1 - REG_INT) + 1);
    u4WrAddr = REG_SYSTEM_PARAM;
    u4WrSz = ((REG_BLOCK_ADDR_0 - REG_SYSTEM_PARAM) + 1);

    /* (1) */
    i4Ret = _Msp_TpcSetRWRegAdrs(u4RdAddr, u4RdSz, u4WrAddr, u4WrSz);

    if (i4Ret != S_OK)
    {
        LOG(1, "%s(%d) SetRWReg failed ret:%d\n", __FILE__, __LINE__, i4Ret);
        return i4Ret;
    }

    _prFCIMSP->au1MsReg[REG_SYSTEM_PARAM] = (SYSP_ON | ((_prFCIMSP->fg4BitEn) ? (SYSP_PAM) : 0));
    _prFCIMSP->au1MsReg[REG_BLOCK_ADDR_2] = (u4BlkNo >> 16) & 0x0ff;
    _prFCIMSP->au1MsReg[REG_BLOCK_ADDR_1] = (u4BlkNo >> 8) & 0x0ff;
    _prFCIMSP->au1MsReg[REG_BLOCK_ADDR_0] = (u4BlkNo >> 0) & 0x0ff;

    /* (2) */
    i4Ret = _Msp_TpcWriteReg(&(_prFCIMSP->au1MsReg[u4WrAddr]), u4WrSz);

    if (i4Ret != S_OK)
    {
        LOG(1, "%s(%d) Write Reg failed ret:%d\n", __FILE__, __LINE__, i4Ret);
        return i4Ret;
    }

    /* (3)/(4) */
    i4Ret = _Msp_TpcSetCmd(CMD_BLOCK_ERASE);

    if (i4Ret != S_OK)
    {
        LOG(1, "%s(%d) SetCmd BLOCK_ERASE failed ret:%d\n", __FILE__, __LINE__, i4Ret);
        return i4Ret;
    }

Retry:
    /* (5) */
    if (_prFCIMSP->fg4BitEn == 0)
    {
        i4Ret = _Msp_TpcGetInt(&(_prFCIMSP->au1MsReg[REG_INT]));

        if (i4Ret != S_OK)
        {
            LOG(1, "%s(%d) GetInt failed ret:%d\n", __FILE__, __LINE__, i4Ret);
            return i4Ret;
        }
        LOG(9, "1. REG_INT:0x%02x\n", _prFCIMSP->au1MsReg[REG_INT]);
    }
    else
    {
        i4Ret = _Msp_TpcReadReg(&(_prFCIMSP->au1MsReg[u4RdAddr]), u4RdSz);

        if (i4Ret != S_OK)
        {
            LOG(1, "%s(%d) GetInt failed ret:%d\n", __FILE__, __LINE__, i4Ret);
            return i4Ret;
        }
        LOG(9, "2. REG_INT:0x%02x\n", _prFCIMSP->au1MsReg[REG_INT]);
    }

    if (_prFCIMSP->au1MsReg[REG_INT]&(INT_CMDNK | INT_ERR))
    {
        LOG(1, "Erase (%d) REG_INT:0x%02x failed with ERR|CMDNK:0x%02x\n", u4BlkNo,
        _prFCIMSP->au1MsReg[REG_INT], INT_CMDNK | INT_ERR);
        return E_FAIL;
    }

    // Jethro MS-128MB 1-bit mode MS card will take more time to rise CED.
    if (!(_prFCIMSP->au1MsReg[REG_INT] & INT_CED) && u1RetryCnt < 10)
    {
        x_thread_delay(1);
        u1RetryCnt++;
        goto Retry;
    }

    ASSERT(_prFCIMSP->au1MsReg[REG_INT] & INT_CED);
    return S_OK;
}

#define MT8222_MS_READ_FUNC

//-------------------------------------------------------------------------
/** MSP_MsReadBlock
 *  MS read pages and extra data function.
 *  @param  u4BlkNo         block number.
 *  @param  u4PgNo          begin page number.
 *  @param  u4PgNum       read page number.
 *  @param  pu1Buf           point to buffer address.
 *  @param  pu1Extra        point to extra buffer address.
 *  @retval  S_OK  Success.
 *  @retval  Others  Fail.
 */
//-------------------------------------------------------------------------
INT32 MSP_MsReadBlock(UINT32 u4BlkNo, UINT32 u4PgNo, UINT32 u4PgNum, UINT8 *pu1Buf, UINT8 *pu1Extra)
{
    INT32 i4Ret;

    UINT32 u4RdAddr, u4RdSz, u4WrAddr, u4WrSz, i;
    UINT32 u4OrgPgNum = u4PgNum, u4RdPg = 0;
    UINT32 u4GetIntRetry;

#ifdef MT8222_MS_READ_FUNC
    //* From 8222 driver, but it not work in FDC MS-128 MB
    UINT32 u4PagePerBlk;
    BOOL  fgEnd;
#endif  // #ifdef MT8222_MS_READ_FUNC

    ASSERT(_prFCIMSP != NULL);
    ASSERT((u4PgNo + u4PgNum) <= (2 *(_prFCIMSP->uATR.rMsBootAtr.u2BlkSz)));
    ASSERT((u4BlkNo) < (_prFCIMSP->uATR.rMsBootAtr.u2BlkNumber));

#ifdef MT8222_MS_READ_FUNC
    u4PagePerBlk = (_prFCIMSP->uATR.rMsBootAtr.u2BlkSz * 1024) / (_prFCIMSP->uATR.rMsBootAtr.u2PageSz);

    if ((u4PagePerBlk == 32) && (u4PgNo + u4PgNum == 32))
    {
        fgEnd = TRUE;
    }
    else if ((u4PagePerBlk == 16) && (u4PgNo + u4PgNum == 16))
    {
        fgEnd = TRUE;
    }
    else
    {
        fgEnd = FALSE;
    }
#endif  // #ifdef MT8222_MS_READ_FUNC

    /* Check the DefectBlkNo array. */
    for (i = 0; _prFCIMSP->uATR.rMsBootAtr.au2InitDefectBlkNo[i] != 0xffff; i++)
    {
        if (_prFCIMSP->uATR.rMsBootAtr.au2InitDefectBlkNo[i] == (UINT16)u4BlkNo)
        {
            LOG(1, "Try to read BlkNo:0x%04x in defect table.\n", u4BlkNo);
            return E_FAIL;
        }
    }

    LOG(7, "%s (Blk:%d, Pg:%d, Num:%d)\n", __FUNCTION__, u4BlkNo, u4PgNo, u4PgNum);

    if (_prFCIMSP->fg4BitEn)
    {
        /* Parallel mode, read from start register to the end one. */
        u4RdAddr = REG_INT;
        u4RdSz = ((REG_RESERVED_0 - REG_INT) + 1);
    }
    else
    {
        /* Serial mode, only read the extra data. */
        u4RdAddr = REG_OVERWRITE_FLAG;
        u4RdSz = ((REG_RESERVED_0 - REG_OVERWRITE_FLAG) + 1);
    }

    u4WrAddr = REG_SYSTEM_PARAM;
    u4WrSz = ((REG_PAGE_ADDR - REG_SYSTEM_PARAM) + 1);

    i = 0;

    while (1)
    {
        /* (1) */
        i4Ret = _Msp_TpcSetRWRegAdrs(u4RdAddr, u4RdSz, u4WrAddr, u4WrSz);

        if (i4Ret != S_OK)
        {
            LOG(1, "%s(%d) SetRWReg failed ret:%d\n", __FILE__, __LINE__, i4Ret);
            return i4Ret;
        }

        _prFCIMSP->au1MsReg[REG_SYSTEM_PARAM] = (SYSP_ON | ((_prFCIMSP->fg4BitEn)
                                                                ? (SYSP_PAM) : 0));
        _prFCIMSP->au1MsReg[REG_BLOCK_ADDR_2] = (u4BlkNo >> 16) & 0x0ff;
        _prFCIMSP->au1MsReg[REG_BLOCK_ADDR_1] = (u4BlkNo >> 8) & 0x0ff;
        _prFCIMSP->au1MsReg[REG_BLOCK_ADDR_0] = (u4BlkNo >> 0) & 0x0ff;
        _prFCIMSP->au1MsReg[REG_CMD_PARAM] = CMD_BLOCK_ACC;
        _prFCIMSP->au1MsReg[REG_PAGE_ADDR] = (u4PgNo & 0x0ff);

        /* (2) */
        i4Ret = _Msp_TpcWriteReg(&(_prFCIMSP->au1MsReg[u4WrAddr]), u4WrSz);

        if (i4Ret != S_OK)
        {
            LOG(1, "%s(%d) Write Reg failed ret:%d\n", __FILE__, __LINE__, i4Ret);
            return i4Ret;
        }

        /* (3)/(4) */
        i4Ret = _Msp_TpcSetCmd(CMD_BLOCK_READ);
        if (i4Ret != S_OK)
        {
            LOG(1, "%s(%d) (Blk:%d, Pg:%d) SetCmd BLOCK_READ failed ret:%d\n", __FILE__, __LINE__,
            u4BlkNo, u4PgNo, i4Ret);

            i++;
            // Send Reset.
            i4Ret = _Msp_TpcSetCmd(CMD_RESET);

            if (_prFCIMSP->fg4BitEn)
            {
                // Whenever do CMD_RESET, must set parallel again.
                i4Ret = _Msp_SwitchParallel();
                if (i4Ret != S_OK)
                {
                    LOG(1, "%s(%d) SwitchParallel failed ret:%d\n", __FILE__, __LINE__, i4Ret);
                    return i4Ret;
                }
            }
            if (i > 6)
            {
                return E_FAIL;
            }
        }
        else
        {
            break;
        }
    }

    while (1)
     {
        u4GetIntRetry = 8;

        while(1)  // [20090321] 8 times retry to GET Int (Some times Get INT result will be 0x00)
        {
            /* (5) */
            // Serial interface: Get INT result by TPC.
            if (_prFCIMSP->fg4BitEn == 0)
            {
                i4Ret = _Msp_TpcGetInt(&(_prFCIMSP->au1MsReg[REG_INT]));

                if (i4Ret != S_OK)
                {
                    LOG(1, "%s(%d) GetInt failed ret:%d\n", __FILE__, __LINE__, i4Ret);
                    return i4Ret;
                }
                LOG(9, "%s(%d) Serial REG_INT:0x%02x\n", __FUNCTION__, __LINE__,
                _prFCIMSP->au1MsReg[REG_INT]);
            }
            else
            {
                // Parallel interface check by H/W INT DATA[3:0].
                _prFCIMSP->au1MsReg[REG_INT] = MSPHW_ReadParallelSif();
                LOG(9, "%s(%d) Parallel INT:0x%02x\n", __FUNCTION__, __LINE__,
                _prFCIMSP->au1MsReg[REG_INT]);
            }

    #ifdef MT8222_MS_READ_FUNC
            if (_prFCIMSP->au1MsReg[REG_INT] == INT_CMDNK)
            {
                LOG(1, "%s (%d) (%d,%d) REG_INT:0x%02x with INT_CMDNK:0x%02x u4RdPg:%d\n", __FUNCTION__, __LINE__, u4BlkNo, u4PgNo,
                _prFCIMSP->au1MsReg[REG_INT], INT_CMDNK, u4RdPg);
                return E_FAIL;  // Error termination. goto Step (13)
            }
            else if ((_prFCIMSP->au1MsReg[REG_INT]  == INT_BREQ) ||                 // Data OK
                        (_prFCIMSP->au1MsReg[REG_INT]  == (INT_ERR | INT_BREQ))) // Correctable Error
            {
                i4Ret = 0;  // Buffer full, break to read Page data
                break;
            }
            else if ((_prFCIMSP->au1MsReg[REG_INT]  == (INT_CED | INT_BREQ)))   // Last page
            {
                i4Ret = 0;  // Last Page. goto Step (11)
                goto ReadLastPage;
            }
            else if (_prFCIMSP->au1MsReg[REG_INT] == (INT_CED | INT_BREQ | INT_ERR))
            {
                LOG(1, "(%d,%d) REG_INT:0x%02x\n", u4BlkNo, u4PgNo, _prFCIMSP->au1MsReg[REG_INT]);

                i4Ret = _Msp_ReadStatusReg(REG_STATUS_0, 2);

                if (i4Ret != S_OK)
                {
                    LOG(1, "Read Status register failed return %d.\n", i4Ret);
                    return i4Ret;
                }

                LOG(1, "Error - Status0:0x%02x Status1:0x%02x\n", _prFCIMSP->au1MsReg[REG_STATUS_0],
                _prFCIMSP->au1MsReg[REG_STATUS_1]);

                if (_prFCIMSP->au1MsReg[REG_STATUS_1]&(ST1_UCDT | ST1_UCEX | ST1_UCFG))
                {
                    LOG(1, "(%d,%d) Uncorrectable error\n", u4BlkNo, u4PgNo);
                    return E_FAIL;
                }
                LOG(1, "(%d,%d) Correctable!!!\n", u4BlkNo, u4PgNo);

                goto ReadLastPage;
            }

#else
            // =================================================
            if ((_prFCIMSP->au1MsReg[REG_INT]&(
            INT_CED | INT_BREQ | INT_CMDNK)) == (INT_CED | INT_BREQ | INT_CMDNK))
            {
                LOG(1, "%s (%d) (Blk:%d, Pg:%d) - INT:0x%08x Num:%d RdPg:%d\n", __FUNCTION__, __LINE__, u4BlkNo, u4PgNo,
                _prFCIMSP->au1MsReg[REG_INT], u4PgNum, u4RdPg);
                i4Ret = 1; /* XXX Buggy??? */

                goto ReadLastPage;
            }

            if (_prFCIMSP->au1MsReg[REG_INT] & INT_CMDNK)
            {
                LOG(1, "%s (%d) (%d,%d) REG_INT:0x%02x with INT_CMDNK:0x%02x u4RdPg:%d\n", __FUNCTION__, __LINE__, u4BlkNo, u4PgNo,
                _prFCIMSP->au1MsReg[REG_INT], INT_CMDNK, u4RdPg);
                return E_FAIL;  // Error termination. goto Step (13)
            }

            if ((_prFCIMSP->au1MsReg[REG_INT]&(
            INT_CED | INT_BREQ | INT_ERR)) == (INT_CED | INT_BREQ | INT_ERR))
            {
                LOG(1, "%s (%d) (%d,%d) REG_INT:0x%02x with INT_ERR:0x%02x u4RdPg:%d\n", __FUNCTION__, __LINE__, u4BlkNo, u4PgNo,
                _prFCIMSP->au1MsReg[REG_INT], INT_CMDNK, u4RdPg);
                i4Ret = 1;  // Flash Read Error occurred, Error termination. goto Step (11)
                goto ReadLastPage;
            }

            if ((_prFCIMSP->au1MsReg[REG_INT]&(INT_CED | INT_BREQ)) == (INT_CED | INT_BREQ))
            {
                i4Ret = 0;  // Last Page. goto Step (11)
                goto ReadLastPage;
            }

            if (_prFCIMSP->au1MsReg[REG_INT] & INT_ERR)
            {
                LOG(1, "(%d,%d) REG_INT:0x%02x\n", u4BlkNo, u4PgNo, _prFCIMSP->au1MsReg[REG_INT]);

                i4Ret = _Msp_ReadStatusReg(REG_STATUS_0, 2);

                if (i4Ret != S_OK)
                {
                    LOG(1, "Read Status register failed return %d.\n", i4Ret);
                    return i4Ret;
                }

                LOG(1, "Error - Status0:0x%02x Status1:0x%02x\n", _prFCIMSP->au1MsReg[REG_STATUS_0],
                _prFCIMSP->au1MsReg[REG_STATUS_1]);

                if (_prFCIMSP->au1MsReg[REG_STATUS_1]&(ST1_UCDT | ST1_UCEX | ST1_UCFG))
                {
                    LOG(1, "(%d,%d) Uncorrectable error\n", u4BlkNo, u4PgNo);
                    return E_FAIL;
                }
                LOG(1, "(%d,%d) Correctable!!!\n", u4BlkNo, u4PgNo);

                break;
            }
#endif  // MT8222_MS_READ_FUNC

            if (--u4GetIntRetry == 0)
            {
              LOG(1, "%s (%d) (%d,%d) 8 Retry Failed\n", __FUNCTION__, __LINE__, u4BlkNo, u4PgNo);
              return E_FAIL;
            }
        }

        LOG(7, "%s (Blk:%d, Pg:%d) - INT:0x%08x Num:%d RdPg:%d\n", __FUNCTION__, u4BlkNo, u4PgNo,
        _prFCIMSP->au1MsReg[REG_INT], u4PgNum, u4RdPg);

        ASSERT(u4PgNum != 0);
        /* (6) */
        if (u4PgNum > 0)
        {
            u4PgNum--;
        }

        /* (7) */
#ifdef MT8222_MS_READ_FUNC
        if (u4PgNum == 0 && !fgEnd)
#else
        if (u4PgNum == 0)
#endif
        {
            /* (8)/(4) */
            i4Ret = _Msp_TpcSetCmd(CMD_BLOCK_END);

            if (i4Ret != S_OK)
            {
                LOG(1, "%s(%d) SetCmd BLOCK_END failed ret:%d\n", __FILE__, __LINE__, i4Ret);
                return i4Ret;
            }

            /* [20090231] This read reg will let Step 5 failed to read correct REG_INT
            if (_prFCIMSP->fg4BitEn == 0)
            {
                i4Ret = _Msp_TpcGetInt(&(_prFCIMSP->au1MsReg[REG_INT]));

                if (i4Ret != S_OK)
                {
                    LOG(1, "%s(%d) GetInt failed ret:%d\n", __FILE__, __LINE__, i4Ret);
                    return i4Ret;
                }
                LOG(9, "%s(%d) Serial REG_INT:0x%02x\n", __FUNCTION__, __LINE__,
                _prFCIMSP->au1MsReg[REG_INT]);
            }

            i4Ret = _Msp_TpcReadReg(&(_prFCIMSP->au1MsReg[u4RdAddr]), u4RdSz);

            if (i4Ret != S_OK)
            {
                LOG(1, "%s(%d) Read Reg failed ret:%d\n", __FILE__, __LINE__, i4Ret);
                return i4Ret;
            }
            */

            continue;
        }

#ifdef MT8222_MS_READ_FUNC
        if (u4PgNum != 0) // last page will be read later => don't read here
#endif
        {
            /* (9) READ_REG */
            i4Ret = _Msp_TpcReadReg(&(_prFCIMSP->au1MsReg[u4RdAddr]), u4RdSz);

            if (i4Ret != S_OK)
            {
                LOG(1, "%s(%d) Read Reg failed ret:%d\n", __FILE__, __LINE__, i4Ret);
                return i4Ret;
            }

            ASSERT(pu1Buf != NULL);
            /* (10) */
            i4Ret = _Msp_TpcReadPageData(MS_PAGE_SIZE, pu1Buf + (MS_PAGE_SIZE *u4RdPg));

            if (i4Ret != S_OK)
            {
                LOG(1, "%s(%d) ReadPageData failed ret:%d\n", __FILE__, __LINE__, i4Ret);
                return i4Ret;
            }

            ASSERT(pu1Extra != NULL);
            /* Assign Extra Attribute */
            for (i = 0; i < 9; i++)
            {
                pu1Extra[i + (9 *u4RdPg)] = _prFCIMSP->au1MsReg[REG_OVERWRITE_FLAG + i];
            }
            u4RdPg++;
        }
    }

ReadLastPage:
    /* (11) READ_REG */
    i4Ret = _Msp_TpcReadReg(&(_prFCIMSP->au1MsReg[u4RdAddr]), u4RdSz);

    if (i4Ret != S_OK)
    {
        LOG(1, "%s(%d) Read Reg failed ret:%d\n", __FILE__, __LINE__, i4Ret);
        return i4Ret;
    }

    ASSERT(pu1Buf != NULL);

    /* (12) READ_PAGE_DATA: MS card do not generate SIT interrupt */
    _prFCIMSP->fgLastpage = TRUE;

    i4Ret = _Msp_TpcReadPageData(MS_PAGE_SIZE, pu1Buf + (MS_PAGE_SIZE *u4RdPg));

    _prFCIMSP->fgLastpage = FALSE;

    if (i4Ret != S_OK)
    {
        LOG(1, "%s(%d) ReadPageData failed ret:%d\n", __FILE__, __LINE__, i4Ret);
        return i4Ret;
    }

     ASSERT(pu1Extra != NULL);
    /* Assign Extra Attribute */
    for (i = 0; i < 9; i++)
    {
        pu1Extra[i + (9 *u4RdPg)] = _prFCIMSP->au1MsReg[REG_OVERWRITE_FLAG + i];
    }

    u4RdPg++;

    UNUSED(u4OrgPgNum);

    if (i4Ret == 0)
    {
        ASSERT(u4RdPg == u4OrgPgNum);
    }

    return i4Ret;
}
    
//-------------------------------------------------------------------------
/** MSP_MsWriteBlock
 *  MS write pages and extra data function.
 *  @param  u4BlkNo         block number.
 *  @param  u4PgNo          begin page number.
 *  @param  u4PgNum       write page number.
 *  @param  pu1Buf           point to buffer address.
 *  @param  pu1Extra        point to extra buffer address.
 *  @retval  S_OK  Success.
 *  @retval  Others  Fail.
 */
//-------------------------------------------------------------------------
INT32 MSP_MsWriteBlock(UINT32 u4BlkNo, UINT32 u4PgNo, UINT32 u4PgNum, UINT8 *pu1Buf,
                       const UINT8 *pu1Extra)
{
    INT32 i4Ret, i4LoopCnt;

    UINT32 u4RdAddr, u4RdSz, u4WrAddr, u4WrSz, i;
    UINT32 u4OrgPgNum = u4PgNum, u4WrPg = 0;

    ASSERT(_prFCIMSP != NULL);
    ASSERT(pu1Extra != NULL);
    ASSERT((u4PgNo + u4PgNum) <= (2 *(_prFCIMSP->uATR.rMsBootAtr.u2BlkSz)));
    ASSERT((u4BlkNo) < (_prFCIMSP->uATR.rMsBootAtr.u2BlkNumber));

#if BOOT_BLOCK_RECOVERY
#else /* without RECOVERY, must do write checking. */
    /* Check the DefectBlkNo array. */

    for (i = 0; _prFCIMSP->uATR.rMsBootAtr.au2InitDefectBlkNo[i] != 0xffff; i++)
    {
        if (_prFCIMSP->uATR.rMsBootAtr.au2InitDefectBlkNo[i] == (UINT16)u4BlkNo)
        {
            LOG(1, "Try to read BlkNo:0x%04x in defect table.\n", u4BlkNo);
            return E_FAIL;
        }
    }
#endif

    LOG(7, "%s (Blk:%d, Pg:%d, Num:%d)\n", __FUNCTION__, u4BlkNo, u4PgNo, u4PgNum);

    if (_prFCIMSP->fg4BitEn)
    {
        u4RdAddr = REG_INT;
        u4RdSz = ((REG_STATUS_1 - REG_INT) + 1);
    }
    else
    {
        u4RdAddr = REG_STATUS_0;
        u4RdSz = ((REG_STATUS_1 - REG_STATUS_0) + 1);
    }

    u4WrAddr = REG_SYSTEM_PARAM;
    // u4WrSz = (REG_PAGE_ADDR - REG_SYSTEM_PARAM + 1);
    u4WrSz = ((REG_RESERVED_0 - REG_SYSTEM_PARAM) + 1);

    /* (1) */
    i4Ret = _Msp_TpcSetRWRegAdrs(u4RdAddr, u4RdSz, u4WrAddr, u4WrSz);

    if (i4Ret != S_OK)
    {
        LOG(1, "%s(%d) SetRWReg failed ret:%d\n", __FILE__, __LINE__, i4Ret);
        return i4Ret;
    }

    _prFCIMSP->au1MsReg[REG_SYSTEM_PARAM] = (SYSP_ON | ((_prFCIMSP->fg4BitEn) ? (SYSP_PAM) : 0));
    _prFCIMSP->au1MsReg[REG_BLOCK_ADDR_2] = (u4BlkNo >> 16) & 0x0ff;
    _prFCIMSP->au1MsReg[REG_BLOCK_ADDR_1] = (u4BlkNo >> 8) & 0x0ff;
    _prFCIMSP->au1MsReg[REG_BLOCK_ADDR_0] = (u4BlkNo >> 0) & 0x0ff;
    _prFCIMSP->au1MsReg[REG_CMD_PARAM] = CMD_BLOCK_ACC;
    _prFCIMSP->au1MsReg[REG_PAGE_ADDR] = (u4PgNo & 0x0ff);

    for (i = 0; i < 9; i++)
    {
        _prFCIMSP->au1MsReg[REG_OVERWRITE_FLAG + i] = pu1Extra[i];
    }

    /* (2) */
    i4Ret = _Msp_TpcWriteReg(&(_prFCIMSP->au1MsReg[u4WrAddr]), u4WrSz);

    if (i4Ret != S_OK)
    {
        LOG(1, "%s(%d) Write Reg failed ret:%d\n", __FILE__, __LINE__, i4Ret);
        return i4Ret;
    }

    u4WrAddr = REG_MANAGEMENT_FLAG;
    u4WrSz = 1; // (REG_RESERVED_0 - REG_MANAGEMENT_FLAG + 1);

    /* (3) */
    i4Ret = _Msp_TpcSetRWRegAdrs(u4RdAddr, u4RdSz, u4WrAddr, u4WrSz);

    if (i4Ret != S_OK)
    {
        LOG(1, "%s(%d) SetRWReg failed ret:%d\n", __FILE__, __LINE__, i4Ret);
        return i4Ret;
    }

    /* (4)/(5) */
    i4Ret = _Msp_TpcSetCmd(CMD_BLOCK_WRITE);

    if (i4Ret != S_OK)
    {
        LOG(1, "%s(%d) SetCmd BLOCK_WRITE failed ret:%d\n", __FILE__, __LINE__, i4Ret);
        return i4Ret;
    }

    i4LoopCnt = 0;

    while (1)
    {
        /* (6) */
        if (_prFCIMSP->fg4BitEn == 0)
        {
            i4Ret = _Msp_TpcGetInt(&(_prFCIMSP->au1MsReg[REG_INT]));

            if (i4Ret != S_OK)
            {
                LOG(1, "%s(%d) GetInt failed ret:%d\n", __FILE__, __LINE__, i4Ret);
                return i4Ret;
            }
            LOG(9, "%s(%d) Serial REG_INT:0x%02x\n", __FUNCTION__, __LINE__,
            _prFCIMSP->au1MsReg[REG_INT]);
        }
        else
        {
            i4Ret = _Msp_TpcReadReg(&(_prFCIMSP->au1MsReg[u4RdAddr]), u4RdSz);

            if (i4Ret != S_OK)
            {
                LOG(1, "%s(%d) Read Reg failed ret:%d\n", __FILE__, __LINE__, i4Ret);
                return i4Ret;
            }
            LOG(9, "%s(%d) Parallel REG_INT:0x%02x\n", __FUNCTION__, __LINE__,
            _prFCIMSP->au1MsReg[REG_INT]);
        }

        if (_prFCIMSP->au1MsReg[REG_INT]&(INT_CMDNK | INT_ERR))
        {
            return E_FAIL;
        }

        if (_prFCIMSP->au1MsReg[REG_INT] & INT_CED)
        {
            /* (12) write end */
            break;
        }

        if ((_prFCIMSP->au1MsReg[REG_INT]&INT_BREQ) == 0)
        {
            LOG(1, "%s(%d) REG_INT:0x%02x\n", __FUNCTION__, __LINE__, _prFCIMSP->au1MsReg[REG_INT]);

            i4LoopCnt++;

            if (i4LoopCnt < 512)
            { /* WHY 512?? XXX fix me. */
                continue;
            }
            else
            {
                LOG(1, "%s(%d) GetInt failed 0x%02x\n", __FILE__, __LINE__,
                _prFCIMSP->au1MsReg[REG_INT]);
            }
            break;
        }

        /* (7) */
        if (u4PgNum == 0)
        {
            /* (8)/(5) */
            i4Ret = _Msp_TpcSetCmd(CMD_BLOCK_END);

            if (i4Ret != S_OK)
            {
                LOG(1, "%s(%d) SetCmd BLOCK_END failed ret:%d\n", __FILE__, __LINE__, i4Ret);
                return i4Ret;
            }

            if (_prFCIMSP->fg4BitEn == 0)
            {
                i4Ret = _Msp_TpcGetInt(&(_prFCIMSP->au1MsReg[REG_INT]));

                if (i4Ret != S_OK)
                {
                    LOG(1, "%s(%d) GetInt failed ret:%d\n", __FILE__, __LINE__, i4Ret);
                    return i4Ret;
                }
                LOG(9, "%s(%d) Serial REG_INT:0x%02x\n", __FUNCTION__, __LINE__,
                _prFCIMSP->au1MsReg[REG_INT]);
            }

            i4Ret = _Msp_TpcReadReg(&(_prFCIMSP->au1MsReg[u4RdAddr]), u4RdSz);

            if (i4Ret != S_OK)
            {
                LOG(1, "%s(%d) Read Reg failed ret:%d\n", __FILE__, __LINE__, i4Ret);
                return i4Ret;
            }
            continue;
        }

        for (i = 0; i < 9; i++)
        {
            _prFCIMSP->au1MsReg[REG_OVERWRITE_FLAG + i] = pu1Extra[i + (9 *u4WrPg)];
        }

#if 0 /* XXX Why write register failed? Bug? */
        /* (10) */

        i4Ret = _Msp_TpcWriteReg(&(_prFCIMSP->au1MsReg[u4WrAddr]), u4WrSz);

        //i4Ret = _Msp_TpcWriteReg(_prFCIMSP, u4WrAddr, u4WrSz);
        if (i4Ret != S_OK)
        {
            LOG(1, "%s(%d) Write Reg failed ret:%d\n", __FILE__, __LINE__, i4Ret);
            return i4Ret;
        }
#endif

        ASSERT(pu1Buf != NULL);
        /* (11) */
        i4Ret = _Msp_TpcWritePageData(MS_PAGE_SIZE, pu1Buf + (MS_PAGE_SIZE *u4WrPg));

        if (i4Ret != S_OK)
        {
            LOG(1, "%s(%d) WritePageData failed ret:%d\n", __FILE__, __LINE__, i4Ret);
            return i4Ret;
        }

        ASSERT(u4PgNum > 0);

        /* (9) */
        u4WrPg++;
        if (u4PgNum > 0)
        {
            u4PgNum--;
        }
    }

    UNUSED(u4OrgPgNum);
    ASSERT(u4WrPg == u4OrgPgNum);

    return S_OK;
}

/*****************************************************************************
 * MSPRO card Integration functions
 */

//-------------------------------------------------------------------------
/** MSP_MsproReadBlks
 *  MSpro read pages data function.
 *  @param  u4Lba      begin block number.
 *  @param  pu1Buf     point to buffer address.
 *  @param  u4BlkNs   read block number.
 *  @retval  S_OK  Success.
 *  @retval  Others  Fail.
 */
//-------------------------------------------------------------------------
INT32 MSP_MsproReadBlks(UINT32 u4Lba, UINT8 *pu1Buf, UINT32 u4BlkNs)
{
    INT32 i4Ret;
    UINT32 u4RdPg;
    UINT32 u4GetIntRetry;
    
    ASSERT(_prFCIMSP != NULL);
    ASSERT(pu1Buf != NULL);

    i4Ret = _Msp_TpcExSetCmd(CMD_MSPRO_READ_DATA, u4BlkNs, u4Lba);

    if (i4Ret != S_OK)
    {
        LOG(1, "MSPHW_TpcExSetCmd read_data failed\n");
        return i4Ret;
    }

    // FCI controller cannot help to write and dma at one TPC, must use loop.
    u4RdPg = 0;

    while (1)
    {
        u4GetIntRetry = 8;
        
        while(1)
        {
            // GET_INT in Serial, read CmdIdx in Parallel.
            if (_prFCIMSP->fg4BitEn)
            {
                _prFCIMSP->au1MsReg[REG_INT] = MSPHW_ReadParallelSif();
            }
            else
            {
                i4Ret = _Msp_TpcGetInt(&(_prFCIMSP->au1MsReg[REG_INT]));
                if (i4Ret != S_OK)
                {
                    LOG(1, "%s(%d) GetInt failed ret:%d\n", __FUNCTION__, __LINE__, i4Ret);
                    return i4Ret;
                }
            }

            LOG(9, "%s,%d) REG_INT:0x%02x\n", __FUNCTION__, __LINE__, _prFCIMSP->au1MsReg[REG_INT]);

            if (_prFCIMSP->au1MsReg[REG_INT] & INT_CMDNK)
            {
                LOG(1, "%s (%d,%d) REG_INT:0x%02x with INT_CMDNK:0x%02x\n", __FUNCTION__, u4Lba,
                u4BlkNs, _prFCIMSP->au1MsReg[REG_INT], INT_CMDNK);
                return E_FAIL;
            }

            // check if data read finish.
            if ((_prFCIMSP->au1MsReg[REG_INT]&INT_CED) && !(_prFCIMSP->au1MsReg[REG_INT]&INT_BREQ))
            {
                ASSERT(u4RdPg == u4BlkNs);
                return S_OK;
            }

            if (_prFCIMSP->au1MsReg[REG_INT] & INT_BREQ)
            {
                // MSP card Buffer full => trigger FCI to read data from buffer of MSP card
                break;
            }
            
            if (--u4GetIntRetry == 0)
            {
                LOG(1, "%s (%d) (%d,%d) 8 Retry Failed, REG_INT:0x%02x\n", __FUNCTION__, __LINE__, u4Lba,
                u4BlkNs, _prFCIMSP->au1MsReg[REG_INT]);
                return E_FAIL;
            }
        }
        
        i4Ret = _Msp_TpcReadPageData(MS_PAGE_SIZE, pu1Buf + (u4RdPg *MS_PAGE_SIZE));

        if (i4Ret != S_OK)
        {
            LOG(1, "%s(%d) MSPHW_TpcReadPageData failed ret:%d\n", __FUNCTION__, __LINE__, i4Ret);
            return i4Ret;
        }
        u4RdPg++;
    }
}

//-------------------------------------------------------------------------
/** MSP_MsproWriteBlks
 *  MSpro write pages data function.
 *  @param  u4Lba      begin block number.
 *  @param  pu1Buf     point to buffer address.
 *  @param  u4BlkNs   write block number.
 *  @retval  S_OK  Success.
 *  @retval  Others  Fail.
 */
//-------------------------------------------------------------------------
INT32 MSP_MsproWriteBlks(UINT32 u4Lba, UINT8 *pu1Buf, UINT32 u4BlkNs)
{
    INT32 i4Ret;

    UINT32 u4WrPg;

    ASSERT(_prFCIMSP != NULL);
    ASSERT(pu1Buf != NULL);

    i4Ret = _Msp_TpcExSetCmd(CMD_MSPRO_WRITE_DATA, u4BlkNs, u4Lba);

    if (i4Ret != S_OK)
    {
        LOG(1, "MSPHW_TpcExSetCmd write_data failed\n");
        return i4Ret;
    }

    // FCI controller cannot help to write and dma at one TPC, must use loop.
    u4WrPg = 0;

    while (1)
    {
        // GET_INT in Serial, read CmdIdx in Parallel.
        if (_prFCIMSP->fg4BitEn)
        {
            _prFCIMSP->au1MsReg[REG_INT] = MSPHW_ReadParallelSif();
        }
        else
        {
            i4Ret = _Msp_TpcGetInt(&(_prFCIMSP->au1MsReg[REG_INT]));
            if (i4Ret != S_OK)
            {
                LOG(1, "%s(%d) GetInt failed ret:%d\n", __FILE__, __LINE__, i4Ret);
                return i4Ret;
            }
        }

        LOG(9, "%s,%d) REG_INT:0x%02x\n", __FUNCTION__, __LINE__, _prFCIMSP->au1MsReg[REG_INT]);

        // command error.
        if (_prFCIMSP->au1MsReg[REG_INT] & INT_CMDNK)
        {
            LOG(1, "%s (%d,%d) REG_INT:0x%02x with INT_CMDNK.\n", __FUNCTION__, u4Lba,
            u4BlkNs, _prFCIMSP->au1MsReg[REG_INT]);
            return E_FAIL;
        }

        // command error or data error occur.
        if (_prFCIMSP->au1MsReg[REG_INT] & INT_ERR)
        {
            LOG(1, "%s (%d,%d) REG_INT:0x%02x with INT_ERR.\n", __FUNCTION__, u4Lba,
            u4BlkNs, _prFCIMSP->au1MsReg[REG_INT]);
            return E_FAIL;
        }

        // check if data write finish.
        if (_prFCIMSP->au1MsReg[REG_INT] == INT_CED)
        {
            ASSERT(u4WrPg == u4BlkNs);
            return S_OK;
        }

        i4Ret = _Msp_TpcWritePageData(MS_PAGE_SIZE, pu1Buf + (u4WrPg *MS_PAGE_SIZE));
        u4WrPg++;
        if (i4Ret != S_OK)
        {
            LOG(1, "%s(%d) MSPHW_TpcWritePageData failed ret:%d\n", __FUNCTION__, __LINE__, i4Ret);
            return i4Ret;
        }
    }
}

/*****************************************************************************
 * Export functions
 */

//-------------------------------------------------------------------------
/** MSP_Init
 *  MS card detect and attach.
 *  @param  prMSP   point to MS data structure.
 *  @retval   1 : Device is failed to detect.
 *  @retval   0 : Device is attached successfully.
 */
//-------------------------------------------------------------------------
INT32 MSP_Init(MSP_T *prMSP)
{
    INT32 i4Ret;

    ASSERT(prMSP != NULL);

    _prFCIMSP = prMSP;

    // initialize smxd data structure.
    VERIFY(NULL != x_memset(_prFCIMSP, 0, sizeof(MSP_T)));

    MSPHW_Init();
    i4Ret = _Msp_CardDetect();

    if (i4Ret == FCI_MSPRO_CARD)
    {
        i4Ret = _Msp_InitMSProCard();
    }
    else if (i4Ret == FCI_MS_CARD)
    {
        i4Ret = _Msp_InitMsCard();
    }
    else
    {
        i4Ret = 1; /* Error. */
    }

    if (i4Ret != S_OK)
    {
        LOG(1, "MSPInit failed at %s(%d) Ret:%d\n", __FILE__, __LINE__, i4Ret);
        /* What is in slot? */
        _prFCIMSP->u4State = SLOT_STATE_UNKNOWN;
        _prFCIMSP->u4CardType = FCI_UNKNOWN_CARD;
    }

    if (_prFCIMSP->u4CardType != FCI_UNKNOWN_CARD)
    {
        LOG(1,
            "MSP_Init Card:%d (MS:%d MSPro:%d) %s %s %s\n",
        _prFCIMSP->u4CardType,
        FCI_MS_CARD,
        FCI_MSPRO_CARD,
        (_prFCIMSP->fgWriteProtected ? "(ReadOnly)" : "(Writable)"),
        (_prFCIMSP->fgSupportParallel ? "(Parallel/Serial)" : "(Serial Only)"),
        (_prFCIMSP->fg4BitEn ? "(Parallel Mode)" : "(Serial Mode)"));
    }
    else
    {
        LOG(1, "MSP_Init Card: None\n");
    }

    return i4Ret;
}

//-------------------------------------------------------------------------
/** MSP_Stop
 *  MS card de-attach.
 *  @param  VOID.
 *  @retval   1 : Device is failed to detect.
 *  @retval   0 : Device is attached successfully.
 */
//-------------------------------------------------------------------------
INT32 MSP_Stop(VOID)
{
    INT32 i4Ret = S_OK;

    // If no card is detected, return OK.
    if((_prFCIMSP == NULL) || (_prFCIMSP->u4CardType == FCI_UNKNOWN_CARD))
    {
        return S_OK;
    }

    // If parallel mode, switch back to serial mode.
    if(_prFCIMSP->fg4BitEn)
    {
        i4Ret = _Msp_SwitchSerial();
    }

    _prFCIMSP = NULL;

    return i4Ret;
}

/*****************************************************************************
 * CLI Export functions
 */


#if BOOT_BLOCK_RECOVERY

__align(16) static MS_BATR_T   rMsBootAtr;
__align(16) static UINT8 au1Extra[0x30];

INT32 MSP_BootWrite(VOID)
{
    MS_BATR_T *prMsBat;
    UINT16 *pu2;
    INT32 i, i4Ret;

    rMsBootAtr.u2BlockId = 1;
    rMsBootAtr.u2Format = 0x0100;
    rMsBootAtr.au1HdrReserved1[0] = 0; /* XXX */
    rMsBootAtr.u1DataEntry = 0x2;
    rMsBootAtr.au1HdrReserved2[0] = 0; /* XXX */
    rMsBootAtr.u4DisBlkStartAddr = 0;
    rMsBootAtr.u4DisBlkSz = 0x0a;
    rMsBootAtr.u1DisBlkId = 0x01;
    rMsBootAtr.au1SysReserved1[0] = 0; /* XXX */
    rMsBootAtr.u4CisIdiStartAddr = 0x0200;
    rMsBootAtr.u4CisIdiSz = 0x0200;
    rMsBootAtr.u1CisIdiId = 0x0a;
    rMsBootAtr.au1SysReserved2[0] = 0; /* XXX */
    rMsBootAtr.au1SysReserved3[0] = 0; /* XXX */
    rMsBootAtr.u1MSClass = 0x01;
    rMsBootAtr.u1FmtUVal2 = 0x02;
    rMsBootAtr.u2BlkSz = 0x10;
    rMsBootAtr.u2BlkNumber = 0x2000;
    rMsBootAtr.u2EffBlkNumber = 0x1f00;
    rMsBootAtr.u2PageSz = 0x0200;
    rMsBootAtr.u1ExtraSz = 0x10;
    rMsBootAtr.u1FmtUVal1 = 0x1;
    rMsBootAtr.au1AssemblyDate[0] = 0x24; rMsBootAtr.au1AssemblyDate[1] = 0x07;
    rMsBootAtr.au1AssemblyDate[2] = 0xd4; rMsBootAtr.au1AssemblyDate[3] = 0x09;
    rMsBootAtr.au1AssemblyDate[4] = 0x08; rMsBootAtr.au1AssemblyDate[5] = 0x0c;
    rMsBootAtr.au1AssemblyDate[6] = 0x2b; rMsBootAtr.au1AssemblyDate[7] = 0x38;
    rMsBootAtr.u1FmtUVal0 = 0x0;
    rMsBootAtr.au1ManuArea[0] = 0xa6; rMsBootAtr.au1ManuArea[1] = 0x52;
    rMsBootAtr.au1ManuArea[2] = 0xb3;
    rMsBootAtr.u1AssemblyManuCode = 0x1;
    rMsBootAtr.au1AssemblyModel[0] = 0x1;
    rMsBootAtr.au1AssemblyModel[1] = 0x28;
    rMsBootAtr.au1AssemblyModel[2] = 0x0;
    rMsBootAtr.u2MemoryManu = 0x0;
    rMsBootAtr.u2MemoryDev = 0x0;
    rMsBootAtr.u2TotalMB = 0x80;
    rMsBootAtr.u1FmtUVal1_1 = 0x1;
    rMsBootAtr.u1FmtUVal1_2 = 0x1;
    rMsBootAtr.u1VCC = 0x21;
    rMsBootAtr.u1VPP = 0x21;
    rMsBootAtr.u2CtrlNumber = 0xf0;
    rMsBootAtr.u2CtrlFunction = 0x1001;
    rMsBootAtr.au1Reserved[0] = 0; /* XXX */
    rMsBootAtr.u1InterfaceType = 0x1;
    rMsBootAtr.u2FmtUVal0_1 = 0x0;
    rMsBootAtr.u1FormatType = 0x1;
    rMsBootAtr.u1Application = 0x0;
    rMsBootAtr.u1DevType = 0x0;
    rMsBootAtr.au1Reserved1[0] = 0; /* XXX */
    rMsBootAtr.u1FmtUVala = 0xa;
    rMsBootAtr.u1FmtUVal1_3 = 0x1;
    rMsBootAtr.au1Reserved2[0] = 0; /* XXX */
    rMsBootAtr.u2InfoBlkNo = 2;
    i = 0;
    rMsBootAtr.au2InitDefectBlkNo[0] = 0x3fe;
    rMsBootAtr.au2InitDefectBlkNo[1] = 0x3ff;
    rMsBootAtr.au2InitDefectBlkNo[2] = 0x1ffe;
    rMsBootAtr.au2InitDefectBlkNo[3] = 0x1fff;
    for (i=4; i<255; i++) rMsBootAtr.au2InitDefectBlkNo[i] = 0xffff;
    rMsBootAtr.au1CisDev[0] = 0x01; rMsBootAtr.au1CisDev[1] = 0x04;
    rMsBootAtr.au1CisDev[2] = 0xdf; rMsBootAtr.au1CisDev[3] = 0x4a;
    rMsBootAtr.au1CisDev[4] = 0x01; rMsBootAtr.au1CisDev[5] = 0xff;
    rMsBootAtr.au1CisDevOc[0] = 0x1c; rMsBootAtr.au1CisDevOc[1] = 0x04;
    rMsBootAtr.au1CisDevOc[2] = 0x02; rMsBootAtr.au1CisDevOc[3] = 0x4a;
    rMsBootAtr.au1CisDevOc[4] = 0x01; rMsBootAtr.au1CisDevOc[5] = 0xff;
    rMsBootAtr.au1CisJedec_C[0] = 0x18; rMsBootAtr.au1CisJedec_C[1] = 0x02;
    rMsBootAtr.au1CisJedec_C[2] = 0xdf; rMsBootAtr.au1CisJedec_C[3] = 0x01;
    rMsBootAtr.au1CisManfid[0] = 0x20; rMsBootAtr.au1CisManfid[1] = 0x04;
    rMsBootAtr.au1CisManfid[2] = 0xf1; rMsBootAtr.au1CisManfid[3] = 0x00;
    rMsBootAtr.au1CisManfid[4] = 0x00; rMsBootAtr.au1CisManfid[5] = 0x00;
    rMsBootAtr.au1CisVers_1[0x00] = 0x15; rMsBootAtr.au1CisVers_1[0x01] = 0x1e;
    rMsBootAtr.au1CisVers_1[0x02] = 0x04; rMsBootAtr.au1CisVers_1[0x03] = 0x01;
    rMsBootAtr.au1CisVers_1[0x04] = 0x53; rMsBootAtr.au1CisVers_1[0x05] = 0x4f;
    rMsBootAtr.au1CisVers_1[0x06] = 0x4e; rMsBootAtr.au1CisVers_1[0x07] = 0x59;
    rMsBootAtr.au1CisVers_1[0x08] = 0x00; rMsBootAtr.au1CisVers_1[0x09] = 0x4d;
    rMsBootAtr.au1CisVers_1[0x0a] = 0x45; rMsBootAtr.au1CisVers_1[0x0b] = 0x4d;
    rMsBootAtr.au1CisVers_1[0x0c] = 0x4f; rMsBootAtr.au1CisVers_1[0x0d] = 0x52;
    rMsBootAtr.au1CisVers_1[0x0e] = 0x59; rMsBootAtr.au1CisVers_1[0x0f] = 0x53;
    rMsBootAtr.au1CisVers_1[0x00] = 0x54; rMsBootAtr.au1CisVers_1[0x01] = 0x49;
    rMsBootAtr.au1CisVers_1[0x02] = 0x43; rMsBootAtr.au1CisVers_1[0x03] = 0x4b;
    rMsBootAtr.au1CisVers_1[0x04] = 0x28; rMsBootAtr.au1CisVers_1[0x05] = 0x31;
    rMsBootAtr.au1CisVers_1[0x06] = 0x32; rMsBootAtr.au1CisVers_1[0x07] = 0x38;
    rMsBootAtr.au1CisVers_1[0x08] = 0x4d; rMsBootAtr.au1CisVers_1[0x09] = 0x29;
    rMsBootAtr.au1CisVers_1[0x0a] = 0x00; rMsBootAtr.au1CisVers_1[0x0b] = 0x31;
    rMsBootAtr.au1CisVers_1[0x0c] = 0x2e; rMsBootAtr.au1CisVers_1[0x0d] = 0x30;
    rMsBootAtr.au1CisVers_1[0x0e] = 0x00; rMsBootAtr.au1CisVers_1[0x0f] = 0xff;
    rMsBootAtr.au1CisFuncCid[0] = 0x21; rMsBootAtr.au1CisFuncCid[1] = 0x02;
    rMsBootAtr.au1CisFuncCid[2] = 0x01; rMsBootAtr.au1CisFuncCid[3] = 0x01;
    rMsBootAtr.au1CisFuncCe[0] = 0x22; rMsBootAtr.au1CisFuncCe[1] = 0x2;
    rMsBootAtr.au1CisFuncCe[2] = 0x1; rMsBootAtr.au1CisFuncCe[3] = 0x1;
    rMsBootAtr.au1CisFuncCe2[0] = 0x22; rMsBootAtr.au1CisFuncCe2[1] = 0x3;
    rMsBootAtr.au1CisFuncCe2[2] = 0x2; rMsBootAtr.au1CisFuncCe2[3] = 0xc;
    rMsBootAtr.au1CisFuncCe2[4] = 0xf;
    rMsBootAtr.au1CisConfig[0] = 0x1a; rMsBootAtr.au1CisConfig[1] = 0x05;
    rMsBootAtr.au1CisConfig[2] = 0x01; rMsBootAtr.au1CisConfig[3] = 0x03;
    rMsBootAtr.au1CisConfig[4] = 0x00; rMsBootAtr.au1CisConfig[5] = 0x02;
    rMsBootAtr.au1CisConfig[6] = 0x0f;
    rMsBootAtr.au1CisSomething[0] = 0x1b; rMsBootAtr.au1CisSomething[1] = 0x08;
    rMsBootAtr.au1CisSomething[2] = 0xc0; rMsBootAtr.au1CisSomething[3] = 0xc0;
    rMsBootAtr.au1CisSomething[4] = 0xa1; rMsBootAtr.au1CisSomething[5] = 0x01;
    rMsBootAtr.au1CisSomething[6] = 0x55; rMsBootAtr.au1CisSomething[7] = 0x08;
    rMsBootAtr.au1CisSomething[8] = 0x00; rMsBootAtr.au1CisSomething[9] = 0x20;
    rMsBootAtr.au1CisSomething[10] = 0x1b; rMsBootAtr.au1CisSomething[11] = 0x06;
    rMsBootAtr.au1CisSomething[12] = 0x00; rMsBootAtr.au1CisSomething[13] = 0x01;
    rMsBootAtr.au1CisSomething[14] = 0x21; rMsBootAtr.au1CisSomething[15] = 0xb5;
    rMsBootAtr.au1CisSomething[16] = 0x1e; rMsBootAtr.au1CisSomething[17] = 0x0e;
    rMsBootAtr.au1CisSomething[18] = 0x1b; rMsBootAtr.au1CisSomething[19] = 0x0a;
    rMsBootAtr.au1CisSomething[20] = 0xc1; rMsBootAtr.au1CisSomething[21] = 0x41;
    rMsBootAtr.au1CisSomething[22] = 0x99; rMsBootAtr.au1CisSomething[23] = 0x01;
    rMsBootAtr.au1CisSomething[24] = 0x55; rMsBootAtr.au1CisSomething[25] = 0x64;
    rMsBootAtr.au1CisSomething[26] = 0xf0; rMsBootAtr.au1CisSomething[27] = 0xff;
    rMsBootAtr.au1CisSomething[28] = 0xff; rMsBootAtr.au1CisSomething[29] = 0x20;
    rMsBootAtr.au1CisSomething[30] = 0x1b; rMsBootAtr.au1CisSomething[31] = 0x41;
    rMsBootAtr.au1CisSomething[32] = 0x06; rMsBootAtr.au1CisSomething[33] = 0x01;
    rMsBootAtr.au1CisSomething[34] = 0x21; rMsBootAtr.au1CisSomething[35] = 0xb5;
    rMsBootAtr.au1CisSomething[36] = 0x1e; rMsBootAtr.au1CisSomething[37] = 0x4d;
    rMsBootAtr.au1CisSomething[38] = 0x1b; rMsBootAtr.au1CisSomething[39] = 0x0f;
    rMsBootAtr.au1CisSomething[40] = 0xc2; rMsBootAtr.au1CisSomething[41] = 0x41;
    rMsBootAtr.au1CisSomething[42] = 0x99; rMsBootAtr.au1CisSomething[43] = 0x01;
    rMsBootAtr.au1CisSomething[44] = 0x55; rMsBootAtr.au1CisSomething[45] = 0xea;
    rMsBootAtr.au1CisSomething[46] = 0x61; rMsBootAtr.au1CisSomething[47] = 0xf0;
    rMsBootAtr.au1CisSomething[48] = 0x01; rMsBootAtr.au1CisSomething[49] = 0x07;
    rMsBootAtr.au1CisSomething[50] = 0xf6; rMsBootAtr.au1CisSomething[51] = 0x03;
    rMsBootAtr.au1CisSomething[52] = 0x01; rMsBootAtr.au1CisSomething[53] = 0xee;
    rMsBootAtr.au1CisSomething[54] = 0x20; rMsBootAtr.au1CisSomething[55] = 0x1b;
    rMsBootAtr.au1CisSomething[56] = 0x06; rMsBootAtr.au1CisSomething[57] = 0x02;
    rMsBootAtr.au1CisSomething[58] = 0x01; rMsBootAtr.au1CisSomething[59] = 0x21;
    rMsBootAtr.au1CisSomething[60] = 0xb5; rMsBootAtr.au1CisSomething[61] = 0x1e;
    rMsBootAtr.au1CisSomething[62] = 0x4d; rMsBootAtr.au1CisSomething[63] = 0x1b;
    rMsBootAtr.au1CisSomething[64] = 0x0f; rMsBootAtr.au1CisSomething[65] = 0xc3;
    rMsBootAtr.au1CisSomething[66] = 0x41; rMsBootAtr.au1CisSomething[67] = 0x99;
    rMsBootAtr.au1CisSomething[68] = 0x01; rMsBootAtr.au1CisSomething[69] = 0x55;
    rMsBootAtr.au1CisSomething[70] = 0xea; rMsBootAtr.au1CisSomething[71] = 0x61;
    rMsBootAtr.au1CisSomething[72] = 0x70; rMsBootAtr.au1CisSomething[73] = 0x01;
    rMsBootAtr.au1CisSomething[74] = 0x07; rMsBootAtr.au1CisSomething[75] = 0x76;
    rMsBootAtr.au1CisSomething[76] = 0x03; rMsBootAtr.au1CisSomething[77] = 0x01;
    rMsBootAtr.au1CisSomething[78] = 0xee; rMsBootAtr.au1CisSomething[79] = 0x20;
    rMsBootAtr.au1CisSomething[80] = 0x1b; rMsBootAtr.au1CisSomething[81] = 0x06;
    rMsBootAtr.au1CisSomething[82] = 0x03; rMsBootAtr.au1CisSomething[83] = 0x01;
    rMsBootAtr.au1CisSomething[84] = 0x21; rMsBootAtr.au1CisSomething[85] = 0xb5;
    rMsBootAtr.au1CisSomething[86] = 0x1e; rMsBootAtr.au1CisSomething[87] = 0x4d;
    rMsBootAtr.au1CisSomething[88] = 0x14; rMsBootAtr.au1CisSomething[89] = 0x00;
    rMsBootAtr.u1CisEnd = 0xff;
    rMsBootAtr.au1CisReserved[0] = 0;
    rMsBootAtr.u2Config = 0x848a;
    rMsBootAtr.u2Cylinders = 0x03df;
    rMsBootAtr.u2Reserved1 = 0;
    rMsBootAtr.u2Headers = 0x0010;
    rMsBootAtr.u2BytesInTrack = 0;
    rMsBootAtr.u2BytesInSectors = 0x0200;
    rMsBootAtr.u2SectorsInTrack = 0x0010;
    rMsBootAtr.u2MSWSectors = 0x03;
    rMsBootAtr.u2LSWSectors = 0xdf00;
    rMsBootAtr.u2Reserved2 = 0;
    rMsBootAtr.au1SerialNumber[0] = 0x20; rMsBootAtr.au1SerialNumber[1] = 0x20;
    rMsBootAtr.au1SerialNumber[2] = 0x20; rMsBootAtr.au1SerialNumber[3] = 0x20;
    rMsBootAtr.au1SerialNumber[4] = 0x20; rMsBootAtr.au1SerialNumber[5] = 0x20;
    rMsBootAtr.au1SerialNumber[6] = 0x20; rMsBootAtr.au1SerialNumber[7] = 0x20;
    rMsBootAtr.au1SerialNumber[8] = 0x20; rMsBootAtr.au1SerialNumber[9] = 0x20;
    rMsBootAtr.au1SerialNumber[10] = 0x20; rMsBootAtr.au1SerialNumber[11] = 0x20;
    rMsBootAtr.au1SerialNumber[12] = 0x20; rMsBootAtr.au1SerialNumber[13] = 0x20;
    rMsBootAtr.au1SerialNumber[14] = 0x20; rMsBootAtr.au1SerialNumber[15] = 0x20;
    rMsBootAtr.au1SerialNumber[16] = 0x20; rMsBootAtr.au1SerialNumber[17] = 0x20;
    rMsBootAtr.au1SerialNumber[18] = 0x20; rMsBootAtr.au1SerialNumber[19] = 0x20;
    rMsBootAtr.u2BufType = 0x02;
    rMsBootAtr.u2BufSz = 0x02;
    rMsBootAtr.u2LongCmdEcc = 0x04;
    rMsBootAtr.au1FirmVer[0] = 0x53; rMsBootAtr.au1FirmVer[1] = 0x4f;
    rMsBootAtr.au1FirmVer[2] = 0x4e; rMsBootAtr.au1FirmVer[3] = 0x59;
    rMsBootAtr.au1FirmVer[4] = 0x31; rMsBootAtr.au1FirmVer[5] = 0x2e;
    rMsBootAtr.au1FirmVer[6] = 0x30; rMsBootAtr.au1FirmVer[7] = 0x30;
    rMsBootAtr.au1ModelName[0] = 0x4d; rMsBootAtr.au1ModelName[1] = 0x45;
    rMsBootAtr.au1ModelName[2] = 0x4d; rMsBootAtr.au1ModelName[3] = 0x4f;
    rMsBootAtr.au1ModelName[4] = 0x52; rMsBootAtr.au1ModelName[5] = 0x59;
    rMsBootAtr.au1ModelName[6] = 0x53; rMsBootAtr.au1ModelName[7] = 0x54;
    rMsBootAtr.au1ModelName[8] = 0x49; rMsBootAtr.au1ModelName[9] = 0x43;
    rMsBootAtr.au1ModelName[10] = 0x4b; rMsBootAtr.au1ModelName[11] = 0x20;
    rMsBootAtr.au1ModelName[12] = 0x31; rMsBootAtr.au1ModelName[13] = 0x32;
    rMsBootAtr.au1ModelName[14] = 0x38; rMsBootAtr.au1ModelName[15] = 0x4d;
    rMsBootAtr.au1ModelName[16] = 0x20; rMsBootAtr.au1ModelName[17] = 0x31;
    rMsBootAtr.au1ModelName[18] = 0x36; rMsBootAtr.au1ModelName[19] = 0x4b;
    rMsBootAtr.au1ModelName[20] = 0x20; rMsBootAtr.au1ModelName[21] = 0x20;
    rMsBootAtr.au1ModelName[22] = 0x20; rMsBootAtr.au1ModelName[23] = 0x20;
    rMsBootAtr.au1ModelName[24] = 0x20; rMsBootAtr.au1ModelName[25] = 0x20;
    rMsBootAtr.au1ModelName[26] = 0x20; rMsBootAtr.au1ModelName[27] = 0x20;
    rMsBootAtr.au1ModelName[28] = 0x20; rMsBootAtr.au1ModelName[29] = 0x20;
    rMsBootAtr.au1ModelName[30] = 0x20; rMsBootAtr.au1ModelName[31] = 0x20;
    rMsBootAtr.au1ModelName[32] = 0x20; rMsBootAtr.au1ModelName[33] = 0x20;
    rMsBootAtr.au1ModelName[34] = 0x20; rMsBootAtr.au1ModelName[35] = 0x20;
    rMsBootAtr.au1ModelName[36] = 0x20; rMsBootAtr.au1ModelName[37] = 0x20;
    rMsBootAtr.au1ModelName[38] = 0x20; rMsBootAtr.au1ModelName[39] = 0x00;
    rMsBootAtr.u2Reserved3 = 0x1;
    rMsBootAtr.u2DualSup = 0;
    rMsBootAtr.u2DmaSup = 0x200;
    rMsBootAtr.u2Reserved4 = 0x0;
    rMsBootAtr.u2PioMode = 0x100;
    rMsBootAtr.u2DmaMode = 0;
    rMsBootAtr.u2FieldValid = 0x1;
    rMsBootAtr.u2CurCylinders = 0x03df;
    rMsBootAtr.u2CurHeads = 0x10;
    rMsBootAtr.u2CurSectorsInTrack = 0x10;
    rMsBootAtr.u4CurTotalSectors = 0x0003df00;
    rMsBootAtr.u2MultiSectorSetting = 0x0100;
    rMsBootAtr.u4TotalUserSectors = 0x0003df00;
    rMsBootAtr.u2SingleDma = 0;
    rMsBootAtr.u2MultiDma = 0;
    rMsBootAtr.au1Reserved3[0] = 0; /* XXX */

    prMsBat = &rMsBootAtr;
    REVERSE_UINT16(prMsBat->u2BlockId);
    REVERSE_UINT16(prMsBat->u2Format);
    REVERSE_UINT32(prMsBat->u4DisBlkStartAddr);
    REVERSE_UINT32(prMsBat->u4DisBlkSz);
    REVERSE_UINT32(prMsBat->u4CisIdiStartAddr);
    REVERSE_UINT32(prMsBat->u4CisIdiSz);
    REVERSE_UINT16(prMsBat->u2BlkSz);
    REVERSE_UINT16(prMsBat->u2BlkNumber);
    REVERSE_UINT16(prMsBat->u2EffBlkNumber);
    REVERSE_UINT16(prMsBat->u2PageSz);
    REVERSE_UINT16(prMsBat->u2MemoryManu);
    REVERSE_UINT16(prMsBat->u2MemoryDev);
    REVERSE_UINT16(prMsBat->u2TotalMB);
    REVERSE_UINT16(prMsBat->u2CtrlNumber);
    REVERSE_UINT16(prMsBat->u2CtrlFunction);
    pu2 = ((UINT16 *)(& (prMsBat->u2InfoBlkNo)));
    for (i=0; i<256; i++) {
        REVERSE_UINT16(pu2[i]);
    }

    pu2 = ((UINT16 *)(& (prMsBat->au1FirmVer)));
    for (i=0; i<((sizeof(prMsBat->au1FirmVer)+sizeof(prMsBat->au1ModelName)) >> 1); i++) {
        REVERSE_UINT16(pu2[i]);
    }

    for (i=0; i<0x30; i++) au1Extra[i] = 0xff;

    au1Extra[0] = 0xc0;
    au1Extra[1] = 0xfb;
    au1Extra[9] = 0xc0;
    au1Extra[10] = 0xfb;
    au1Extra[18] = 0xc0;
    au1Extra[19] = 0xfb;

    i4Ret = MSP_MsWriteSinglePage(0, 0, ((UINT8 *)&rMsBootAtr), au1Extra, CMD_SINGLE_ACC);
    LOG(1, "WriteSingle(0, 0) return %d\n", i4Ret);
    if (i4Ret) return i4Ret;
    i4Ret = MSP_MsWriteSinglePage(0, 1, ((UINT8 *)&rMsBootAtr)+512, au1Extra, CMD_SINGLE_ACC);
    LOG(1, "WriteSingle(0, 1) return %d\n", i4Ret);
    if (i4Ret) return i4Ret;
    i4Ret = MSP_MsWriteSinglePage(0, 2, ((UINT8 *)&rMsBootAtr)+1024, au1Extra, CMD_SINGLE_ACC);
    LOG(1, "WriteSingle(0, 2) return %d\n", i4Ret);
    if (i4Ret) return i4Ret;
    i4Ret = MSP_MsWriteSinglePage(1, 0, ((UINT8 *)&rMsBootAtr), au1Extra, CMD_SINGLE_ACC);
    LOG(1, "WriteSingle(1, 0) return %d\n", i4Ret);
    if (i4Ret) return i4Ret;
    i4Ret = MSP_MsWriteSinglePage(1, 1, ((UINT8 *)&rMsBootAtr)+512, au1Extra, CMD_SINGLE_ACC);
    LOG(1, "WriteSingle(1, 1) return %d\n", i4Ret);
    if (i4Ret) return i4Ret;
    i4Ret = MSP_MsWriteSinglePage(1, 2, ((UINT8 *)&rMsBootAtr)+1024, au1Extra, CMD_SINGLE_ACC);
    LOG(1, "WriteSingle(1, 2) return %d\n", i4Ret);
    if (i4Ret) return i4Ret;

    return i4Ret;
}

#else

INT32 MSP_BootWrite(VOID)
{
    LOG(1, "Disable BootWrite function\n");

    return 0;
}

#endif

#endif
