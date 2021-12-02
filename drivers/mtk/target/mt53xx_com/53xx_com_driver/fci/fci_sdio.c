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
 * $RCSfile: fci_sdio.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file fci_sdio.c
 *  This C file implements the SDIO basic hardware protocol functions.  All calls
 *   are from XXXX.c.
 */

#ifdef CC_FCI_ENABLE

#ifdef SDIO_UINT_TEST
// MT5151 DVBH SDIO unit test.
#define SDIO_DVBH_TEST_SUPPORT
#define SDIO_IOTEST_SIZE             ((UINT32)0x20)
//#define SDIO_LOW_CLOCK              // use cpu pll slow clock to test.
#endif

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "sdc_const.h"
#include "sdio_func.h"
#include "sdc_func.h"

#ifdef SDIO_DVBH_TEST_SUPPORT
#include "dvbh_if.h"
#endif

LINT_EXT_HEADER_BEGIN

#include "fcihw_func.h"
#include "x_assert.h"
#include "x_os.h"
#include "fcihw_reg.h"

#include "x_ckgen.h"

LINT_EXT_HEADER_END

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

#define SDIO_SUPPORT

#ifdef SDIO_SUPPORT

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------
#define SDIO_CCCR_SIZE                  0x14      // bytes, do not support "Vendor Register" now.
#define SDIO_FBR_SIZE                    0x12     // bytes.
#define SDIO_FBR_OFFSET               256         // bytes.
#define SDIO_CIS_SIZE                     0x17000 // bytes.
#define SDIO_MAX_TUPLE_SIZE      257              // bytes.
#define SDIO_4BIT_BUS                  (1)
#define SDIO_1BIT_BUS                  (0)
#define SDIO_ENABLE_PC               1      // enable master power control.

#define SDIO_MAX_BLK_NUM         128        // bytes.
#define SDIO_MAX_BLK_SIZE         2048      // bytes.
#define SDIO_MAX_BYTE_SIZE       512        // Max bytes count in Byte mode.
#define SDIO_BLK_SIZE                   512 // Set block size = 512 bytes.

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------

extern INT32 _fgSdCardRemove;

//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------

static UINT8 _u1SDIOCCCR[SDIO_CCCR_SIZE];
static UINT8 _u1SDIOFBR[SDIO_FBR_SIZE];
static UINT8 _u1SDIOTUPLE[SDC_FUNC_END][SDIO_MAX_TUPLE_SIZE];
static UINT32 _u4SDIOTUPLELength[SDC_FUNC_END];
static FCI_CALLBACK _pfnSDIOHISR[SDC_FUNC_END];
static SDC_T *_prSDIO = NULL;

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------

//-------------------------------------------------------------------------
/** _Sdio_ChkResp
 *  Check SDIO cmd52 and cmd53 response R5.
 *  @param  u4Resp  R5 response value.
 *  @retval   S_OK : Success.
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
static INT32 _Sdio_ChkResp(UINT32 u4Resp)
{
    u4Resp >>= 8;

    if (u4Resp & SDIOR5_RESP_CRCERR)
    {
        LOG(1, "SDIOR5_RESP_CRCERR\n");
        return SDIOR5_RESP_CRCERR;
    }
    else if (u4Resp & SDIOR5_RESP_ILLCMD)
    {
        LOG(1, "SDIOR5_RESP_ILLCMD\n");
        return SDIOR5_RESP_ILLCMD;
    }
    else if (u4Resp & SDIOR5_RESP_GENERR)
    {
        LOG(1, "SDIOR5_RESP_GENERR\n");
        return SDIOR5_RESP_GENERR;
    }
    else if (u4Resp & SDIOR5_RESP_RFU)
    {
        LOG(1, "SDIOR5_RESP_RFU\n");
        return SDIOR5_RESP_RFU;
    }
    else if (u4Resp & SDIOR5_RESP_FUNNUM)
    {
        LOG(1, "SDIOR5_RESP_FUNNUM\n");
        return SDIOR5_RESP_FUNNUM;
    }
    else if (u4Resp & SDIOR5_RESP_OUTRANGE)
    {
        LOG(1, "SDIOR5_RESP_OUTRANGE\n");
        return SDIOR5_RESP_OUTRANGE;
    }

    return S_OK;
}

//-------------------------------------------------------------------------
/** _Sdio_SendCmd
 *  Send SDIO Cmd52 or cmd53 command.
 *  @param  u4Arg  Cmd52 or cmd53 command.
 *  @param  u4Arg  Cmd52 or cmd53 argument.
 *  @param  *pu1Data  point to address of response.
 *  @retval   S_OK : Success.
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
static INT32 _Sdio_SendCmd(UINT32 u4Cmd, UINT32 u4Arg, UINT8 *pu1Resp)
{
    INT32 i4Ret;

    UINT32 u4Val;

    i4Ret = SDCHW_SendCmd(u4Cmd, u4Arg);

    if (i4Ret)
    {
        LOG(1, "(%s,%d) SDCHW_SendCmd failed\n", __FUNCTION__, __LINE__);
        return i4Ret;
    }

    i4Ret = _SDC_CheckStatus(SD_CMD_IDX(u4Cmd));

    if (i4Ret)
    {
        LOG(1, "(%s,%d) _SDC_CheckStatus failed\n", __FUNCTION__, __LINE__);
        return i4Ret;
    }

    u4Val = SDCHW_ReadResp(1);

    // check response state.
    i4Ret = _Sdio_ChkResp(u4Val);

    if (i4Ret)
    {
        LOG(1, "(%s,%d) _Sdio_ChkResp failed\n", __FUNCTION__, __LINE__);
        return i4Ret;
    }

    if (pu1Resp != NULL)
    {
        *pu1Resp = u4Val & 0xFF;
    }

    //LOG(1, "SDIO response = 0x%08X.\n", u4Val);

    return S_OK;
}

//-------------------------------------------------------------------------
/** _Sdio_SetBusWidth
 *  Set SDIO bus to 4 bits wide.
 *  @param  VOID.
 *  @retval   S_OK : Success.
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
static INT32 _Sdio_SetBusWidth(BOOL fg4BitEnable)
{
    INT32 i4Ret;

    UINT32 u4Arg;
    UINT8 u1OldReg = 0x82;

    // set 1bit or 4 bits bus, and disable card detection.
    u1OldReg = (fg4BitEnable) ? 0x82 : 0x80;
    SDIO52_CMD(u4Arg, SDIO_WRITE, SDIO_FUNC(SDC_COMMOM), SDIO_RAW_YES,
    SDIO_REGADR(CCCR_BUSCONFIG), SDIO_WDATA(u1OldReg));

    // set new value and read back to cccr buffer.
    i4Ret = _Sdio_SendCmd(SD_CMD_52, u4Arg, &_u1SDIOCCCR[CCCR_BUSCONFIG]);

    if (S_OK != i4Ret)
    {
        return i4Ret;
    }

    SDCHW_Set4BitOn(fg4BitEnable);

    return i4Ret;
}

//-------------------------------------------------------------------------
/** _Sdio_SetPower
 *  Turn on card current may exceed 200mA.
 *  @param  VOID.
 *  @retval   S_OK : Success.
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
static INT32 _Sdio_SetPower(VOID)
{
    INT32 i4Ret;

    UINT32 u4Arg;
    UINT8 u1OldReg = _u1SDIOCCCR[CCCR_POWER];

    u1OldReg |= 0x2;

    SDIO52_CMD(u4Arg, SDIO_WRITE, SDIO_FUNC(SDC_COMMOM), SDIO_RAW_YES,
    SDIO_REGADR(CCCR_POWER), SDIO_WDATA(u1OldReg));

    // set new value and read back to cccr buffer.
    i4Ret = _Sdio_SendCmd(SD_CMD_52, u4Arg, &_u1SDIOCCCR[CCCR_POWER]);

    return i4Ret;
}

//-------------------------------------------------------------------------
/** _Sdio_Reset
 *  This function is to reset all I/O function.
 *  @param  VOID
 *  @retval   S_OK : Success.
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
static INT32 _Sdio_Reset(VOID)
{
    INT32 i4Ret;

    UINT32 u4Arg;

    SDIO52_CMD(u4Arg, SDIO_WRITE, SDIO_FUNC(SDC_COMMOM), SDIO_RAW_NO,
    SDIO_REGADR(CCCR_IOABORT), SDIO_WDATA(0x08));

    i4Ret = _Sdio_SendCmd(SD_CMD_52, u4Arg, NULL);

    return i4Ret;
}
//-------------------------------------------------------------------------
/** _Sdio_ReadCCCR
 *  Read CCCR register.
 *  @param  VOID.
 *  @retval   S_OK : Success.
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
static INT32 _Sdio_ReadCCCR(void)
{
    INT32 i4Ret;

    UINT32 u4Arg;
    UINT32 i;

    for (i = 0; i < SDIO_CCCR_SIZE; i++)
    {
        SDIO52_CMD(u4Arg, SDIO_READ, SDIO_FUNC(SDC_COMMOM), SDIO_RAW_NO,
        SDIO_REGADR(i), SDIO_WDATA(0));

        i4Ret = _Sdio_SendCmd(SD_CMD_52, u4Arg, &_u1SDIOCCCR[i]);
        if (i4Ret)
        {
            return i4Ret;
        }
    }

    LOG(1,"CCCR content:");
    SDIO_Print(_u1SDIOCCCR, SDIO_CCCR_SIZE);

    return i4Ret;
}

//-------------------------------------------------------------------------
/** _Sdio_ReadFBR
 *  Read FBR of the selected function.
 *  @param  u4FuncID        the selected function id.
 *  @retval   S_OK : Success.
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
static INT32 _Sdio_ReadFBR(UINT32 u4FuncID)
{
    INT32 i4Ret;

    UINT32 u4Arg;
    UINT32 i;

    ASSERT(u4FuncID < (UINT32)SDC_FUNC_END);

    for (i = 0; i < SDIO_FBR_SIZE; i++)
    {
        SDIO52_CMD(u4Arg, SDIO_READ, SDIO_FUNC(SDC_COMMOM), SDIO_RAW_NO,
        SDIO_REGADR((SDIO_FBR_OFFSET *u4FuncID)+i), SDIO_WDATA(0));

        i4Ret = _Sdio_SendCmd(SD_CMD_52, u4Arg, &_u1SDIOFBR[i]);
        if (i4Ret)
        {
            return i4Ret;
        }
    }

    LOG(1,"FBR content:");
    SDIO_Print(_u1SDIOFBR, SDIO_FBR_SIZE);

    return i4Ret;
}

//-------------------------------------------------------------------------
/** _Sdio_ReadCIS
 *  Read CIS tuple data on the specific CIS address.
 *  @param  u4CISAddr  specific CIS address.
 *  @param  pu1CISBuf  point to CIS buffer.
 *  @retval   > 0 : CIS length.
 *  @retval   0 : Fail.
 */
//-------------------------------------------------------------------------
static UINT32 _Sdio_ReadCIS(UINT32 u4CISAddr, UINT8 *pu1CISBuf)
{
    INT32 i4Ret;

    UINT32 u4Arg;
    UINT32 u4CisOffset = 0;
    UINT32 i;

    ASSERT(pu1CISBuf);

    while (1)
    {
        SDIO52_CMD(u4Arg, SDIO_READ, SDIO_FUNC(SDC_COMMOM), SDIO_RAW_NO,
        SDIO_REGADR(u4CISAddr + u4CisOffset), SDIO_WDATA(0));

        // get tuple code.
        i4Ret = _Sdio_SendCmd(SD_CMD_52, u4Arg, &pu1CISBuf[u4CisOffset]);

        if (i4Ret)
        {
            return 0;
        }

        // check tuple end.
        if ((pu1CISBuf[u4CisOffset] == SDIO_CISTPL_NULL)
                || (pu1CISBuf[u4CisOffset] == SDIO_CISTPL_END))
        {
            // read CIS end.
            return u4CisOffset;
        }

        SDIO52_CMD(u4Arg, SDIO_READ, SDIO_FUNC(SDC_COMMOM), SDIO_RAW_NO,
        SDIO_REGADR(u4CISAddr + u4CisOffset + 1), SDIO_WDATA(0));

        // get tuple size.
        i4Ret = _Sdio_SendCmd(SD_CMD_52, u4Arg, &pu1CISBuf[u4CisOffset + 1]);

        if (i4Ret)
        {
            return 0;
        }

        // get tuple body.
        for (i = 0; i < pu1CISBuf[u4CisOffset + 1]; i++)
        {
            SDIO52_CMD(u4Arg, SDIO_READ, SDIO_FUNC(SDC_COMMOM), SDIO_RAW_NO,
            SDIO_REGADR(u4CISAddr + u4CisOffset + 2 + i), SDIO_WDATA(0));

            i4Ret = _Sdio_SendCmd(SD_CMD_52, u4Arg, &pu1CISBuf[u4CisOffset + 2 + i]);
            if (i4Ret)
            {
                return 0;
            }
        }

        // increase offset index.
        u4CisOffset += (2 + pu1CISBuf[u4CisOffset + 1]);
    }
}

//-------------------------------------------------------------------------
/** _Sdio_HISR
 *  SDIO HISR handler. This will be called by FCI HISR when SDIO interrupt happen.
 *  @param  fgIntStatus    FCI interrupt status (NOTICE: without the highest bit MMCIS).
 *  @retval   VOID.
 */
//-------------------------------------------------------------------------
static VOID _Sdio_HISR(INT32 fgIntStatus)
{
    UINT32 u4Fid;

    for (u4Fid = (UINT32)SDIO_FUNC_1; u4Fid < (UINT32)SDC_FUNC_END; u4Fid++)
    {
        if (_pfnSDIOHISR[u4Fid])
        {
            _pfnSDIOHISR[u4Fid](fgIntStatus);
        }
    }
}

//-------------------------------------------------------------------------
/** _Sdio_TestIO
 *  get and set to/from card to test SDIO card.
 *  @param  void   data address.
 *  @retval   S_OK : Success.
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
#ifdef SDIO_UINT_TEST
static INT32 _Sdio_TestIO(VOID)
{
    UINT32 u4Buf[SDIO_IOTEST_SIZE];
    UINT32 u4Size;
    UINT32 u4Addr;
    INT32 i4Ret;

    Printf("%s start...\n", __FUNCTION__);

    // enable function 1 in CCCR.
/*
    i4Ret = SDIO_EnableIO(SDIO_FUNC_1, NULL);
    if ( i4Ret != S_OK)
    {
        LOG(1,"%s fail.\n", __FUNCTION__);
        return i4Ret;
    }
*/

    // get CIS0 by CMD53.
    for (u4Addr = 0; u4Addr<= SDIO_IOTEST_SIZE; u4Addr+=4)
    {
        for (u4Size = 4; u4Size < (SDIO_IOTEST_SIZE-1); u4Size += 4)
        {
            i4Ret = SDIO_ReadBytes((UINT32)SDC_COMMOM, u4Addr + 0x1000, u4Size,
                (UINT8 *)u4Buf, SDIO_OP_INCADR);
            if (i4Ret != S_OK)
            {
                return i4Ret;
            }

            i4Ret = x_memcmp ((const VOID*)&_u1SDIOTUPLE[0][u4Addr],
                                                (const VOID*)u4Buf, u4Size);
            if (i4Ret != S_OK)
            {
                LOG(5, "Compare Err: u4Addr = 0x%X, u4Size = %d:", u4Addr, u4Size);
                SDIO_Print((UINT8 *)u4Buf, u4Size);
                return i4Ret;
            }
        }
    }

    // get CIS1 by CMD53.
    for (u4Addr = 0; u4Addr<= SDIO_IOTEST_SIZE; u4Addr+=4)
    {
        for (u4Size = 4; u4Size <= SDIO_IOTEST_SIZE; u4Size += 4)
        {
            i4Ret = SDIO_ReadBytes((UINT32)SDC_COMMOM, u4Addr + 0x2000, u4Size,
                (UINT8 *)u4Buf, SDIO_OP_INCADR);
            if (i4Ret != S_OK)
            {
                return i4Ret;
            }

            i4Ret = x_memcmp ((const VOID*)&_u1SDIOTUPLE[1][u4Addr],
                                                (const VOID*)u4Buf, u4Size);
            if (i4Ret != S_OK)
            {
                LOG(5, "Compare Err: u4Addr = 0x%X, u4Size = %d:", u4Addr, u4Size);
                SDIO_Print((UINT8 *)u4Buf, u4Size);
                return i4Ret;
            }
        }
    }

    Printf("%s OK...\n", __FUNCTION__);

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
/** SDIO_SetInt
 *  Enable/Disable SDIO interrupt.
 *  @param  fgEnable  Enable/Disable SDIO interrupt..
 *  @retval   void
 */
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
/** SDIO_ReadCCCR
 *  Read CCCR register.
 *  @param  VOID.
 *  @retval   S_OK : Success.
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
INT32 SDIO_ReadCCCR(VOID)
{
    INT32 i4Ret;

    UINT32 u4Arg;
    UINT32 i;

    for (i = 0; i < SDIO_CCCR_SIZE; i++)
    {
        SDIO52_CMD(u4Arg, SDIO_READ, SDIO_FUNC(SDC_COMMOM), SDIO_RAW_NO,
        SDIO_REGADR(i), SDIO_WDATA(0));

        i4Ret = _Sdio_SendCmd(SD_CMD_52, u4Arg, &_u1SDIOCCCR[i]);
        if (i4Ret)
        {
            return i4Ret;
        }
    }

    LOG(4, "CCCR content:");
    SDIO_Print(_u1SDIOCCCR, SDIO_CCCR_SIZE);

    return i4Ret;
}

//-------------------------------------------------------------------------
/** SDIO_Reset
 *  This function is to reset all I/O function.
 *  @param  VOID
 *  @retval   S_OK : Success.
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
INT32 SDIO_Reset(VOID)
{
    INT32 i4Ret;

    UINT32 u4Arg;

    SDIO52_CMD(u4Arg, SDIO_WRITE, SDIO_FUNC(SDC_COMMOM), SDIO_RAW_NO,
    SDIO_REGADR(CCCR_IOABORT), SDIO_WDATA(0x08));

    i4Ret = _Sdio_SendCmd(SD_CMD_52, u4Arg, NULL);

    return i4Ret;
}

//-------------------------------------------------------------------------
/** SDIO_ShowCCCR
 *  Print CCCR register.
 *  @param  VOID.
 */
//-------------------------------------------------------------------------
VOID SDIO_ShowCCCR(VOID)
{
    UINT32  i;

    Printf("[CCCR]\n");

    for (i = 0; i < SDIO_CCCR_SIZE; i++)
    {
        Printf("0x%02X ", _u1SDIOCCCR[i]);
        if ((i+1) % 8 == 0)
            Printf("\n");
    }
}

#ifdef FCI_SDIO_SUPPORT
void SDIO_SetInt(BOOL fgEnable)
{
    SDCHW_SetSDIOInt(fgEnable);
}
#endif
//-------------------------------------------------------------------------
/** SDIO_Print
 *  print sdio array..
 *  @param  pu1Buf      user's buffer.
 *  @param  u4Length   data length.
 *  @retval   void
 */
//-------------------------------------------------------------------------
void SDIO_Print(const UINT8 *pu1Buf, UINT32 u4Length)
{
    UINT32 i;
    if ((!pu1Buf) ||(u4Length == 0))
    {
        return;
    }
    for (i = 0; i < u4Length; i++)
    {
        if (!(i % 8))
        {
            LOG(1,"\n0x%08X :", i);
        }

        LOG(1," 0x%02X,", pu1Buf[i]);
    }
    LOG(1,"\n");
}

//-------------------------------------------------------------------------
/** SDIO_ReadBytes
 *  SDIO read multi-bytes function by BYTE mode.
 *  @param  u4FuncId   specific function id.
 *  @param  u4Addr   register address.
 *  @param  u4Size   read data size.
 *  @param  u4OPcode  0: multiple byte at fix address.  1: multiple byte at incrementing address.
 *  @param  pu1Buf   point to user's data buffer.
 *  @retval   S_OK : Success.
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
INT32 SDIO_ReadBytes(UINT32 u4FuncID, UINT32 u4Addr, UINT32 u4Size, UINT8 *pu1Buf, UINT32 u4OPcode)
{
    INT32 i4Ret = S_OK;

    UINT32 u4Arg;
    UINT32 u4Count = 0;
    UINT32 u4RxSize;

    #ifdef SDIO_1BIT_MODE
    // 537x 1 bit mode h/w bug:
    // 537x will generate data in data1. This signal is the same as data0.
    SDIO_SetInt(FALSE);
    #endif

    ASSERT(pu1Buf);
    // DMA address must be double word alignment.
    ASSERT(((UINT32)pu1Buf % 4) == 0);
    ASSERT(u4Size);
    
    FCIHW_ResetSlot(SDC_SLOT);
    
    FCIHW_SetFifoRead();
    //_SDC_ChangeClock(SDC_HIGH_CLOCK);

    // open SDDR interrupt, and close SDWBF interrupt.
    SDCHW_OpenDataRdyInt();

    while (u4Count < u4Size)
    {
        u4RxSize = MIN((u4Size - u4Count), SDIO_MAX_BYTE_SIZE);

        FCIHW_DmaWriteDramEnable(pu1Buf, u4RxSize);
        SDIO53_CMD(u4Arg, SDIO_READ, SDIO_FUNC(u4FuncID), SDIO_BYTE_MODE, u4OPcode,
        SDIO_REGADR(u4Addr), SDIO_BYTE_COUNT(u4RxSize));

        i4Ret = _Sdio_SendCmd(SD_CMD_53_R | u4RxSize, u4Arg, NULL);

        if (i4Ret != S_OK)
        {
            LOG(1, "%s(%d) %s failed return %d\n", __FILE__, __LINE__, __FUNCTION__, i4Ret);
            break;
        }

        // wait read register finish.
        i4Ret = SDCHW_WaitDataRdyOrTo();

        if (i4Ret != S_OK)
        {
            LOG(1, "%s(%d) wait data rdy failed %d\n", __FILE__, __LINE__, i4Ret);
            break;
        }

        if (u4OPcode == SDIO_OP_INCADR)
        {
            u4Addr += u4RxSize;
        }
        u4Count += u4RxSize;
        pu1Buf += u4RxSize;
    }

    FCIHW_DmaDisable();
    //_SDC_ChangeClock(SDC_LOW_CLOCK);

    #ifdef SDIO_1BIT_MODE
    // 537x 1 bit mode h/w bug:
    // 537x will generate data in data1. This signal is the same as data0.
    SDIO_SetInt(TRUE);
    #endif

    return i4Ret;
}

//-------------------------------------------------------------------------
/** SDIO_ReadBlocks
 *  SDIO read multi-blocks function in block mode.
 *  @param  u4FuncId   specific function id.
 *  @param  u4Addr   register address.
 *  @param  u4BlkNs  number of read data blocks.
 *  @param  pu1Buf   point to user's data buffer.
 *  @retval   S_OK : Success.
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
INT32 SDIO_ReadBlocks(UINT32 u4FuncID, UINT32 u4Addr, UINT32 u4BlkNs, UINT8 *pu1Buf, UINT32 u4OPcode)
{
    INT32 i4Ret = S_OK;

    UINT32 u4BlkField;
    UINT32 u4Arg;
    UINT32 u4Count = 0;
    UINT32 u4RxBlks;

    #ifdef SDIO_1BIT_MODE
    // 537x 1 bit mode h/w bug:
    // 537x will generate data in data1. This signal is the same as data0.
    SDIO_SetInt(FALSE);
    #endif

    ASSERT(_prSDIO->u2BlkSz[u4FuncID] > 0);
    ASSERT(pu1Buf);

    // DMA address must be double word alignment.
    ASSERT(((UINT32)pu1Buf % 4) == 0);
    ASSERT(u4BlkNs);

    FCIHW_ResetSlot(SDC_SLOT);
    
    SDCHW_SetMultiFlag(TRUE);
    FCIHW_SetFifoRead();
    //_SDC_ChangeClock(SDC_HIGH_CLOCK);

    // open SDDR interrupt, and close SDWBF interrupt.
    SDCHW_OpenDataRdyInt();

    while (u4Count < u4BlkNs)
    {
        // FCI limit block number = 128.
        u4RxBlks = MIN((u4BlkNs - u4Count), SDIO_MAX_BLK_NUM);

        FCIHW_DmaWriteDramEnable(pu1Buf, _prSDIO->u2BlkSz[u4FuncID] *u4RxBlks);

        u4BlkField = (((u4RxBlks - 1) & 0x07f) << 12);

        SDIO53_CMD(u4Arg, SDIO_READ, SDIO_FUNC(u4FuncID), SDIO_BLOCK_MODE,
        u4OPcode, SDIO_REGADR(u4Addr), SDIO_COUNT(u4RxBlks));

        i4Ret = _Sdio_SendCmd(SD_CMD_53_MULT_R | u4BlkField | _prSDIO->u2BlkSz[u4FuncID],
        u4Arg, NULL);

        if (i4Ret != S_OK)
        {
            LOG(1, "%s(%d) %s failed return %d\n", __FILE__, __LINE__, __FUNCTION__, i4Ret);
            break;
        }

        i4Ret = SDCHW_WaitDataRdyOrTo();

        if (i4Ret != S_OK)
        {
            LOG(1, "%s(%d) wait data rdy failed %d\n", __FILE__, __LINE__, i4Ret);
            break;
        }

        if (u4OPcode == SDIO_OP_INCADR)
        {
            u4Addr += _prSDIO->u2BlkSz[u4FuncID] * u4RxBlks;
        }

        u4Count += u4RxBlks;
        pu1Buf += _prSDIO->u2BlkSz[u4FuncID] * u4RxBlks;
    }

    //_SDC_ChangeClock(SDC_LOW_CLOCK);
    FCIHW_DmaDisable();
    SDCHW_SetMultiFlag(FALSE);

    #ifdef SDIO_1BIT_MODE
    // 537x 1 bit mode h/w bug:
    // 537x will generate data in data1. This signal is the same as data0.
    SDIO_SetInt(TRUE);
    #endif

    return i4Ret;
}

//-------------------------------------------------------------------------
/** SDIO_WriteBytes
 *  SDIO write multi-bytes function in block mode.
 *  @param  u4FuncId   specific function id.
 *  @param  u4Addr   register address.
 *  @param  u4Size   read data size.
 *  @param  pu1Buf   point to user's data buffer.
 *  @param  u4OPcode  0: multiple byte at fix address.  1: multiple byte at incrementing address.
 *  @retval   S_OK : Success.
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
INT32 SDIO_WriteBytes(UINT32 u4FuncID, UINT32 u4Addr, UINT32 u4Size, UINT8 *pu1Buf, UINT32 u4OPcode)
{
    INT32 i4Ret = S_OK;

    UINT32 u4Arg;
    UINT32 u4Count = 0;
    UINT32 u4TxSize;

    #ifdef SDIO_1BIT_MODE
    // 537x 1 bit mode h/w bug:
    // 537x will generate data in data1. This signal is the same as data0.
    SDIO_SetInt(FALSE);
    #endif

    // DMA address must be double word alignment.
    ASSERT(pu1Buf);
    ASSERT(((UINT32)pu1Buf % 4) == 0);
    ASSERT(u4Size);

    FCIHW_ResetSlot(SDC_SLOT);
    
    FCIHW_SetFifoWrite();
    //_SDC_ChangeClock(SDC_HIGH_CLOCK);

    // turn off SDDR and turn on SDWBF.
    SDCHW_OpenWBFInt();

    while (u4Count < u4Size)
    {
        u4TxSize = MIN((u4Size - u4Count), SDIO_MAX_BYTE_SIZE);

        SDIO53_CMD(u4Arg, SDIO_WRITE, SDIO_FUNC(u4FuncID), SDIO_BYTE_MODE,
        u4OPcode, SDIO_REGADR(u4Addr), SDIO_BYTE_COUNT(u4TxSize));

        i4Ret = _Sdio_SendCmd(SD_CMD_53_W | u4TxSize, u4Arg, NULL);

        if (i4Ret != S_OK)
        {
            LOG(1, "%s(%d) %s failed return %d\n", __FILE__, __LINE__, __FUNCTION__, i4Ret);
            break;
        }

        SDCHW_InitSemaphore();

        FCIHW_DmaReadDramEnable(pu1Buf, u4TxSize);

        // wait write register finish.
        i4Ret = SDCHW_WaitDataIdle();

        if (i4Ret != S_OK)
        {
            LOG(1, "%s(%d) %s failed return %d\n", __FILE__, __LINE__, __FUNCTION__, i4Ret);
            break;
        }

        if (u4OPcode == SDIO_OP_INCADR)
        {
            u4Addr += u4TxSize;
        }

        u4Count += u4TxSize;
        pu1Buf += u4TxSize;
    }

    FCIHW_DmaDisable();
    //_SDC_ChangeClock(SDC_LOW_CLOCK);

    #ifdef SDIO_1BIT_MODE
    // 537x 1 bit mode h/w bug:
    // 537x will generate data in data1. This signal is the same as data0.
    SDIO_SetInt(TRUE);
    #endif

    return i4Ret;
}

//-------------------------------------------------------------------------
/** SDIO_WriteBlocks
 *  SDIO write multi-blocks function in block mode.
 *  @param  u4FuncId   specific function id.
 *  @param  u4Addr   register address.
 *  @param  u4BlkNs   number of write data blocks.
 *  @param  pu1Buf   point to user's data buffer.
 *  @retval   S_OK : Success.
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
INT32 SDIO_WriteBlocks(UINT32 u4FuncID, UINT32 u4Addr, UINT32 u4BlkNs, UINT8 *pu1Buf, UINT32 u4OPcode)
{
    INT32 i4Ret = S_OK;

    UINT32 u4BlkField;
    UINT32 u4Arg;
    UINT32 u4Count = 0;
    UINT32 u4TxBlks;
    UINT32 u4TxSize;

    #ifdef SDIO_1BIT_MODE
    // 537x 1 bit mode h/w bug:
    // 537x will generate data in data1. This signal is the same as data0.
    SDIO_SetInt(FALSE);
    #endif

    ASSERT(_prSDIO->u2BlkSz[u4FuncID] > 0);
    ASSERT(u4BlkNs);
    ASSERT(pu1Buf);

    FCIHW_ResetSlot(SDC_SLOT);
    
    SDCHW_SetMultiFlag(TRUE);
    FCIHW_SetFifoWrite();
    //_SDC_ChangeClock(SDC_HIGH_CLOCK);

    // turn off SDDR and turn on SDWBF.
    SDCHW_OpenWBFInt();

    while (u4Count < u4BlkNs)
    {
        // FCI limit block number = 128.
        u4TxBlks = ((u4BlkNs - u4Count) > SDIO_MAX_BLK_NUM)
                       ? SDIO_MAX_BLK_NUM : (u4BlkNs - u4Count);

        u4BlkField = (((u4TxBlks - 1) & 0x07f) << 12);

        SDIO53_CMD(u4Arg, SDIO_WRITE, SDIO_FUNC(u4FuncID), SDIO_BLOCK_MODE,
        u4OPcode, SDIO_REGADR(u4Addr), SDIO_COUNT(u4TxBlks));

        i4Ret = _Sdio_SendCmd(SD_CMD_53_MULT_W | u4BlkField | _prSDIO->u2BlkSz[u4FuncID],
        u4Arg, NULL);

        if (i4Ret != S_OK)
        {
            LOG(1, "%s(%d) %s failed return %d\n", __FILE__, __LINE__, __FUNCTION__, i4Ret);
            break;
        }

        // Make sure all the semaphore is in lock state.
        SDCHW_InitSemaphore();

        // Set DMA size and address.
        u4TxSize = (UINT32)(_prSDIO->u2BlkSz[u4FuncID] *u4TxBlks);
        FCIHW_DmaReadDramEnable(pu1Buf, u4TxSize);

        // wait data program finish.
        i4Ret = SDCHW_WaitDataIdle();
        if (i4Ret != S_OK)
        {
            LOG(1, "%s(%d) %s failed return %d\n", __FILE__, __LINE__, __FUNCTION__, i4Ret);
            break;
        }

        if (u4OPcode == SDIO_OP_INCADR)
        {
            u4Addr += u4TxSize;
        }

        u4Count += u4TxBlks;
        pu1Buf += u4TxSize;
    }

    //_SDC_ChangeClock(SDC_LOW_CLOCK);
    FCIHW_DmaDisable();
    SDCHW_SetMultiFlag(FALSE);

    #ifdef SDIO_1BIT_MODE
    // 537x 1 bit mode h/w bug:
    // 537x will generate data in data1. This signal is the same as data0.
    SDIO_SetInt(TRUE);
    #endif

    return i4Ret;
}

//-------------------------------------------------------------------------
/** SDIO_EnableIO
 *  Enable IO, wait IO ready, and enable IO interrupt.
 *  @param  _prSDIO  point to SD data structure.
 *  @param  u2BlkSz  block size.
 *  @retval   S_OK : Success.
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
INT32 SDIO_EnableIO(UINT32 u4FuncID, FCI_CALLBACK pfnHISR)
{
    INT32 i4Ret;

    UINT32 u4Arg;
    UINT8 u1OldReg = _u1SDIOCCCR[CCCR_IOENABLE];

    if (SDCHW_IsEmpty())
    {
        LOG(1, "SDIO card is not inserted.\n");
        return E_FAIL;
    }

    ASSERT(u4FuncID < (UINT32)SDC_FUNC_END);
    ASSERT(_prSDIO->u4State == SLOT_STATE_CMD);

    // enable IO in CCCR.
    u1OldReg |= (1 << u4FuncID);

    SDIO52_CMD(u4Arg, SDIO_WRITE, SDIO_FUNC(SDC_COMMOM), SDIO_RAW_YES,
    SDIO_REGADR(CCCR_IOENABLE), SDIO_WDATA(u1OldReg));

    // set new value and read back to fbr buffer.
    i4Ret = _Sdio_SendCmd(SD_CMD_52, u4Arg, &_u1SDIOCCCR[CCCR_IOENABLE]);

    if (i4Ret != S_OK)
    {
        return i4Ret;
    }

    // wait IO ready in CCCR.
    SDIO52_CMD(u4Arg, SDIO_READ, SDIO_FUNC(SDC_COMMOM), SDIO_RAW_NO,
    SDIO_REGADR(CCCR_IOREADY), SDIO_WDATA(0));

    // read back to fbr buffer.
    i4Ret = _Sdio_SendCmd(SD_CMD_52, u4Arg, &_u1SDIOCCCR[CCCR_IOREADY]);

    if (i4Ret != S_OK)
    {
        return i4Ret;
    }

    // IO not ready.
    if (u1OldReg != _u1SDIOCCCR[CCCR_IOREADY])
    {
        return E_FAIL;
    }

    // turn on IO interrupt in CCCR.
    u1OldReg = _u1SDIOCCCR[CCCR_INTENABLE];
    u1OldReg |= ((1 << u4FuncID) | 1);

    SDIO52_CMD(u4Arg, SDIO_WRITE, SDIO_FUNC(SDC_COMMOM), SDIO_RAW_YES,
    SDIO_REGADR(CCCR_INTENABLE), SDIO_WDATA(u1OldReg));

    // set new value and read back to fbr buffer.
    i4Ret = _Sdio_SendCmd(SD_CMD_52, u4Arg, &_u1SDIOCCCR[CCCR_INTENABLE]);

    if (i4Ret != S_OK)
    {
        return i4Ret;
    }

    _pfnSDIOHISR[u4FuncID] = pfnHISR;

    return i4Ret;
}

//-------------------------------------------------------------------------
/** SDIO_AbortIO
 *  force card to stop transmission.
 *  @param  VOID.
  *  @retval   S_OK : Success.
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
INT32 SDIO_AbortIO(UINT32 u4FuncID)
{
    INT32 i4Ret;

    UINT32 u4Arg;
    UINT8 u1OldReg = _u1SDIOCCCR[CCCR_IOABORT];

    ASSERT(u4FuncID < (UINT32)SDC_FUNC_END);
    ASSERT(_prSDIO->u4State == SLOT_STATE_TRANS);

    u1OldReg &= 0xF8;
    u1OldReg |= u4FuncID;

    SDIO52_CMD(u4Arg, SDIO_WRITE, SDIO_FUNC(SDC_COMMOM), SDIO_RAW_YES,
    SDIO_REGADR(CCCR_IOABORT), SDIO_WDATA(u1OldReg));

    // set new value and read back to cccr buffer.
    i4Ret = _Sdio_SendCmd(SD_CMD_52, u4Arg, &_u1SDIOCCCR[CCCR_IOABORT]);

    if (S_OK != i4Ret)
    {
        return i4Ret;
    }

    _prSDIO->u4State = SLOT_STATE_CMD;

    return S_OK;
}

//-------------------------------------------------------------------------
/** SDIO_SetBlkSz
 *  Set block size of selected function into CCCR or FBR.
 *  @param  u4FuncID        the selected function id.
 *  @param  u2BlkSz  block size.
 *  @retval   S_OK : Success.
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
INT32 SDIO_SetBlkSz(UINT32 u4FuncID, UINT16 u2BlkSz)
{
    INT32 i4Ret;

    UINT32 u4Arg;
    UINT8 *pu1Reg1, *pu1Reg2;
    UINT32 u4RegAddr1, u4RegAddr2;

    ASSERT(u4FuncID < (UINT32)SDC_FUNC_END);
    ASSERT((u2BlkSz > 0) && (u2BlkSz <= SDIO_MAX_BLK_SIZE));

    if (u4FuncID == (UINT32)SDC_COMMOM)
    {
        pu1Reg1 = &_u1SDIOCCCR[CCCR_BLKSIZE_1];

        pu1Reg2 = &_u1SDIOCCCR[CCCR_BLKSIZE_2];
        u4RegAddr1 = CCCR_BLKSIZE_1;
        u4RegAddr2 = CCCR_BLKSIZE_2;
    }
    else
    {
        pu1Reg1 = &_u1SDIOFBR[FBR_BLKSIZE_1];

        pu1Reg2 = &_u1SDIOFBR[FBR_BLKSIZE_2];
        u4RegAddr1 = (SDIO_FBR_OFFSET *u4FuncID)+FBR_BLKSIZE_1;
        u4RegAddr2 = (SDIO_FBR_OFFSET *u4FuncID)+FBR_BLKSIZE_2;
    }

    SDIO52_CMD(u4Arg, SDIO_WRITE, SDIO_FUNC(SDC_COMMOM), SDIO_RAW_YES,
    SDIO_REGADR(u4RegAddr1), SDIO_WDATA(u2BlkSz));

    // set new value and read back to fbr buffer.
    i4Ret = _Sdio_SendCmd(SD_CMD_52, u4Arg, pu1Reg1);

    if (i4Ret != S_OK)
    {
        return i4Ret;
    }

    SDIO52_CMD(u4Arg, SDIO_WRITE, SDIO_FUNC(SDC_COMMOM), SDIO_RAW_YES,
    SDIO_REGADR(u4RegAddr2), SDIO_WDATA(u2BlkSz >> 8));

    // set new value and read back to fbr buffer.
    i4Ret = _Sdio_SendCmd(SD_CMD_52, u4Arg, pu1Reg2);

    _prSDIO->u2BlkSz[u4FuncID] = u2BlkSz;

    return i4Ret;
}

//-------------------------------------------------------------------------
/** SDIO_GetMaxBlkSz
 *  Get max block size of selected function from CIS.
 *  @param  u4FuncID        the selected function id.
 *  @return  max block size of selected function
 */
//-------------------------------------------------------------------------
UINT32 SDIO_GetMaxBlkSz(UINT32 u4FuncID)
{
    UINT32 i;

    UINT32 offset;

    ASSERT(u4FuncID < (UINT32)SDC_FUNC_END);

    // function 0 offset is different to other function.
    offset = (u4FuncID == (UINT32)SDC_COMMOM) ? 0x03 : 0x0E;

    for (i = 0; i < SDIO_MAX_TUPLE_SIZE-(offset+1); i++)    // [Klockwork] ABR Analyze
    {
        if (_u1SDIOTUPLE[u4FuncID][i] == SDIO_CISTPL_FUNCE)
        {
            return ((_u1SDIOTUPLE[u4FuncID][i + offset])
                        | (_u1SDIOTUPLE[u4FuncID][i + offset + 1] << 8));
        }
    }

    return 0;
}

//-------------------------------------------------------------------------
/** SDIO_SetBusWidth
 *  Set 1 / 4 bit bus mode
 *  @param  fg4BitEnable : 4 bit mode
 *  @retval   S_OK : Success.
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
INT32 SDIO_SetBusWidth(BOOL fg4BitEnable)
{
    INT32 i4Ret;

    if(fg4BitEnable)
    {
        // capability check, and set 4 bits SD mode.
        if (!SDIO_CAPABLE_LSC(_u1SDIOCCCR))
        {
            // full speed card should support both 1 bit and 4 bits bus.
            ASSERT(SDIO_CAPABLE_S4MI(_u1SDIOCCCR));

            i4Ret = _Sdio_SetBusWidth(SDIO_4BIT_BUS);

            if (i4Ret)
            {
                return i4Ret;
            }

            _prSDIO->fg4BitEn = TRUE;
            SDCHW_Set4BitOn(TRUE);
            LOG(1, "4 bits mode bus.\n");
        }
        else
        {
            // low speed card, 4 bits bus is optional.
            if (SDIO_CAPABLE_4BLS(_u1SDIOCCCR))
            {
                i4Ret = _Sdio_SetBusWidth(SDIO_4BIT_BUS);

                if (i4Ret)
                {
                    return i4Ret;
                }

                _prSDIO->fg4BitEn = TRUE;
                SDCHW_Set4BitOn(TRUE);
                LOG(1, "4 bits mode bus.\n");
            }
            else
            {
                LOG(1, "The card only support 1 bit mode.\n");
            }
        }
    }
    else
    {
        i4Ret = _Sdio_SetBusWidth(SDIO_1BIT_BUS);
        if (i4Ret)
        {
            return i4Ret;
        }

        _prSDIO->fg4BitEn = FALSE;
        SDCHW_Set4BitOn(FALSE);

        LOG(1, "1 bit mode bus.\n");
    }

    return S_OK;
}

//-------------------------------------------------------------------------
/** SDIO_DetachSDIOCard
 *  Detach SDIO card.
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
INT32  SDIO_DetachSDIOCard(VOID)
{    
    _prSDIO->u4State = SLOT_STATE_EMPTY;

    return 0;
}

//-------------------------------------------------------------------------
/** SDIO_IsSDIOCard
 *  Check and initialize SDIO card.
 *  @param  u4Addr   data address.
 *  @param  pu1Buf   point to user's data buffer.
 *  @retval   S_OK : Success.
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
INT32  SDIO_IsSDIOCard(VOID)
{
    UINT32 u4Val;

    UINT32 u4Retry = 0;
    INT32 i4Ret;
    UINT32 u4Arg;
    UINT32 u4CISAddr;
    UINT32 u4Fid;
    UINT32 u4BlkSize;

    // Initially, we let card exist
    _fgSdCardRemove = FALSE;

    if (SDCHW_IsEmpty())
    {
        LOG(1, "SDIO card is not inserted.\n");
        return E_FAIL;
    }

    // Setup multi-func, driving current & pull up resistor
    FCIHW_HWConfigSetup(SDC_SLOT);

    _prSDIO = FCI_GetSDCSlot();

    FCIHW_ResetSlot(SDC_SLOT);

    SDCHW_Init();

    // wait sdio clock to be stable before change to another clock.
    x_thread_delay(100);

#ifndef SDIO_LOW_CLOCK
    _SDC_ChangeClock(SDC_NORM_CLOCK);
#else
    _SDC_ChangeClock(SDC_LOW_CLOCK);
#endif

    if (_prSDIO->u4State >= SLOT_STATE_CMD)
    {
        i4Ret = _Sdio_Reset();
        if (i4Ret)
        {
            return i4Ret;
        }
    }
    _prSDIO->u4State = SLOT_STATE_CMD;

    i4Ret = SDCHW_SendCmd(SD_CMD_5, 0);

#ifdef CC_NO_CARD_DETECT_PIN
    u4Val = 0;

    // Retry if CMD 5 failed
    while(i4Ret != S_OK)
    {
        i4Ret = SDCHW_SendCmd(SD_CMD_5, 0);

        u4Val++;

        if (u4Val >= 2)
        {
            break;
        }
    }
#endif

    if (i4Ret != S_OK)
    {
        // Card not exist
        _fgSdCardRemove = TRUE;
        _prSDIO->u4State = SLOT_STATE_EMPTY;
        LOG(1, "SDIO Card not exist\n");

        LOG(6, "%s(%d) %s return %d\n", __FILE__, __LINE__, __FUNCTION__, i4Ret);
        return i4Ret;
    }

    u4Val = SDCHW_ReadResp(1);

    LOG(1, "OCR = 0x%08x.\n", SDIO_OCR(u4Val));
    LOG(1, "MemPresent = %d.\n", SDIO_MEMPRESENT(u4Val));
    LOG(1, "NumIO = %d.\n", SDIO_NUMIO(u4Val));
    LOG(1, "IORdy = %d.\n", SDIO_IORDY(u4Val));

    // Check number of IO functions.
    if (SDIO_NUMIO(u4Val) == 0)
    {
        return E_FAIL;
    }

    u4Arg = SDIO_OCR(u4Val);

    do
    {
        if (u4Retry++ >= TRY_OCR)
        {
            LOG(1, "%s(%d) %s OCR retry timeout.\n", __FILE__, __LINE__, __FUNCTION__);
            return E_SD_R3_OCR_BUSY;
        }

        i4Ret = SDCHW_SendCmd(SD_CMD_5, u4Arg);

        if (i4Ret != S_OK)
        {
            LOG(6, "%s(%d) %s return %d.\n", __FILE__, __LINE__, __FUNCTION__, i4Ret);
            return i4Ret;
        }

        u4Val = SDCHW_ReadResp(1);

        // NOTICE: this delay is add to wait SDIO card handle OCR.
        // This delay time is an experimental value.
        if (!SDIO_IORDY(u4Val))
        {
            x_thread_delay(100);
        }
    } while (!SDIO_IORDY(u4Val));

    _prSDIO->u1NumIO = SDIO_NUMIO(u4Val);
    _prSDIO->u1MemPresent = SDIO_MEMPRESENT(u4Val);

    i4Ret = _SDC_GetRCA(FCI_SD_CARD, &_prSDIO->u2RCA);

    if (i4Ret)
    {
        return i4Ret;
    }

    LOG(1, "CMD3 Get RCA = 0x%02x.\n", _prSDIO->u2RCA);

    // Command 7.
    i4Ret = _SDC_SelectCard(_prSDIO->u2RCA);

    if (i4Ret)
    {
        return i4Ret;
    }

    i4Ret = _Sdio_ReadCCCR();

    if (i4Ret)
    {
        return i4Ret;
    }

    LOG(1, "Card capability 0x08 = 0x%X.\n", _u1SDIOCCCR[CCCR_CAPABILITY]);
    LOG(1, "capable SDC = %d.\n", SDIO_CAPABLE_SDC(_u1SDIOCCCR));
    LOG(1, "capable SMB = %d.\n", SDIO_CAPABLE_SMB(_u1SDIOCCCR));
    LOG(1, "capable SRW = %d.\n", SDIO_CAPABLE_SRW(_u1SDIOCCCR));
    LOG(1, "capable SBS = %d.\n", SDIO_CAPABLE_SBS(_u1SDIOCCCR));
    LOG(1, "capable S4MI = %d.\n", SDIO_CAPABLE_S4MI(_u1SDIOCCCR));
    LOG(1, "capable E4MI = %d.\n", SDIO_CAPABLE_E4MI(_u1SDIOCCCR));
    LOG(1, "capable LSC = %d.\n", SDIO_CAPABLE_LSC(_u1SDIOCCCR));
    LOG(1, "capable 4BLS = %d.\n", SDIO_CAPABLE_4BLS(_u1SDIOCCCR));
    LOG(1, "power SMPC = %d.\n", SDIO_POWER_SMPC(_u1SDIOCCCR));
    LOG(1, "power EMPC = %d.\n", SDIO_POWER_EMPC(_u1SDIOCCCR));
    LOG(1, "High-speed SHS = %d.\n", SDIO_HIGHSPEED_SHS(_u1SDIOCCCR));
    LOG(1, "High-speed EHS = %d.\n", SDIO_HIGHSPEED_EHS(_u1SDIOCCCR));

#ifdef SDIO_1BIT_MODE
    // set 1 bit mode
    i4Ret = SDIO_SetBusWidth(FALSE);
#else
    // set 4 bit mode
    i4Ret = SDIO_SetBusWidth(TRUE);
#endif

    if (SDIO_POWER_SMPC(_u1SDIOCCCR))
    {
        i4Ret = _Sdio_SetPower();
        if (i4Ret)
        {
            return i4Ret;
        }
    }

    // read function0 CIS.
    u4CISAddr = SDIO_CIS_ADDR(_u1SDIOCCCR[CCCR_CISPTR_3], _u1SDIOCCCR[CCCR_CISPTR_2],
    _u1SDIOCCCR[CCCR_CISPTR_1]);

    LOG(1, "FUN0 CIS addr = 0x%06X.\n", u4CISAddr);

    u4Val = _Sdio_ReadCIS(u4CISAddr, &_u1SDIOTUPLE[SDC_COMMOM][0]);

    if (u4Val == 0)
    {
        return -1;
    }

    _u4SDIOTUPLELength[SDC_COMMOM] = u4Val;

    LOG(1, "CIS common, length = %d bytes", _u4SDIOTUPLELength[SDC_COMMOM]);
    SDIO_Print(&_u1SDIOTUPLE[SDC_COMMOM][0], _u4SDIOTUPLELength[SDC_COMMOM]);

    // Get max block size of function 0 from CIS.
    u4BlkSize = SDIO_GetMaxBlkSz((UINT32)SDC_COMMOM);
    LOG(1, "FUN0 block size = 0x%04X.\n", u4BlkSize);

    // Set max block size of function 0 into CCCR.
    i4Ret = SDIO_SetBlkSz((UINT32)SDC_COMMOM, MIN(u4BlkSize, SDIO_BLK_SIZE));

    if (i4Ret)
    {
        return i4Ret;
    }

    // get function information.
    for (u4Fid = (UINT32)SDIO_FUNC_1; u4Fid <= _prSDIO->u1NumIO; u4Fid++)
    {
        // read FBR of specific function.
        i4Ret = _Sdio_ReadFBR(u4Fid);

        if (i4Ret)
        {
            return i4Ret;
        }

        // get CIS address.
        u4CISAddr = SDIO_CIS_ADDR(_u1SDIOFBR[FBR_CISPTR_3], _u1SDIOFBR[FBR_CISPTR_2],
        _u1SDIOFBR[FBR_CISPTR_1]);

        LOG(1, "FUN%d CIS addr = 0x%06X.\n", u4Fid, u4CISAddr);

        // read CIS of specific function.
        u4Val = _Sdio_ReadCIS(u4CISAddr, &_u1SDIOTUPLE[u4Fid][0]);

        if (u4Val == 0)
        {
            return -1;
        }

        if (u4Fid < SDC_FUNC_END)   // [Klockwork] ABR Analyze
        {
            _u4SDIOTUPLELength[u4Fid] = u4Val;

            LOG(1, "CIS func=%d, length = %d bytes", u4Fid, _u4SDIOTUPLELength[u4Fid]);
            SDIO_Print(&_u1SDIOTUPLE[u4Fid][0], _u4SDIOTUPLELength[u4Fid]);            
        }           

        // Get max block size of function from CIS.
        u4BlkSize = SDIO_GetMaxBlkSz(u4Fid);
        LOG(1, "FUN%d block size = 0x%04X.\n", u4Fid, u4BlkSize);

        // Set max block size of function into FBR.
        i4Ret = SDIO_SetBlkSz(u4Fid, MIN(u4BlkSize, SDIO_BLK_SIZE));
    }

    // register HISR to FCI interrupt handler.
    FCIHW_SetSDIOINTFct(_Sdio_HISR);

#ifdef SDIO_UINT_TEST
    i4Ret = _Sdio_TestIO();
    if (i4Ret)
    {
        return i4Ret;
    }

#ifdef SDIO_DVBH_TEST_SUPPORT
    i4Ret = DVBH_Init();
    if (i4Ret)
    {
        return i4Ret;
    }

    return i4Ret;
#endif  // #ifdef SDIO_DVBH_TEST_SUPPORT

#else
    return i4Ret;
#endif  // #ifdef SDIO_UINT_TEST
}
#endif  // #ifdef SDIO_SUPPORT
#endif  // #ifdef CC_FCI_ENABLE
