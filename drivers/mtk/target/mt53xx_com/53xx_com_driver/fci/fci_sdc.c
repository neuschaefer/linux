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
 * $RCSfile: fci_sdc.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file fci_sdc.c
 *  This C file implements the SD basic hardware protocol functions.  All calls
 *   are from sd_if.c.
 */

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#ifdef CC_FCI_ENABLE

#include "sdc_const.h"
#include "sdc_func.h"
#include "sdio_func.h"
#include "fcihw_reg.h"

LINT_EXT_HEADER_BEGIN

#include "x_clksrc.h"
#include "x_assert.h"
#include "x_os.h"

LINT_EXT_HEADER_END

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------
//#define CC_HIGH_SPEED_MODE_ENABLE

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

extern UINT32 _u4SdDetectCardType;
BOOL  _fgHighSpeedEnable = TRUE;
BOOL  _fgFCI4BitBusEnable = TRUE;

//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------

static UINT32 _u4SDCardType = FCI_SD_CARD; ///< SD 1.x or SD 2.0 card.

UINT32 _u4SdHighClock = 45;        // about 50 Khz
UINT32 _u4SdNormalClock = 27;
UINT32 _u4SdLowClock = 0;            // 421.875 Khz for identify device

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------

//-------------------------------------------------------------------------
/** _Sdc_IsSDCard
 *  Check if the card is SD card.
 *  @param  VOID
 *  @retval   S_OK : Success.
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
static INT32 _Sdc_IsSDCard(VOID)
{
    UINT32 u4Val = 0;

    UINT32 u4Arg = 0;

    INT32 i4Ret, i4Retry = 0;

    /* 5 Reset to avoid initial clock. */
    i4Ret = _SDC_Reset();
    i4Ret = _SDC_Reset();
    i4Ret = _SDC_Reset();
    i4Ret = _SDC_Reset();
    i4Ret = _SDC_Reset();

    // initial SD 2.0 card.
    if (_u4SdDetectCardType == FCI_SDHC_CARD &&
        _SDC_SendIfCondCmd() == S_OK)
    {
        u4Arg = SDC_OCR_HD_DEFAULT;

        LOG(7, "SD 2.0 card.\n");
        _u4SDCardType = FCI_SDHC_CARD;
    }
    else
    {
        LOG(7, "SD 1.x card.\n");

        u4Arg = SDC_OCR_SD_DEFAULT;
        i4Ret = _SDC_Reset();
        _u4SDCardType = FCI_SD_CARD;
    }

    do
    {
        if (i4Retry++ >= TRY_OCR)
        {
            LOG(7, "%s(%d) %s return %d retry:%d u4Val:0x%08x\n", __FILE__, __LINE__, __FUNCTION__,
                E_SD_R3_OCR_BUSY, i4Retry, u4Val);
            return E_SD_R3_OCR_BUSY;
        }

        i4Ret = _SDC_SendCmd55(SDC_RCA_DEFAULT);

        if (i4Ret != S_OK)
        {
            LOG(7, "%s(%d) %s return %d\n", __FILE__, __LINE__, __FUNCTION__, i4Ret);
            return i4Ret;
        }

        i4Ret = SDCHW_SendCmd(SD_CMD_41_SD, u4Arg);

        if (i4Ret != S_OK)
        {
            LOG(7, "%s(%d) %s return %d\n", __FILE__, __LINE__, __FUNCTION__, i4Ret);
            return i4Ret;
        }

        u4Val = SDCHW_ReadResp(1);

        // NOTICE: this delay is add to wait SD card handle OCR.
        // This delay time is an experimental value.
        if (!(u4Val & SDC_OCR_BUSY))
        {
            x_thread_delay(100);
        }
    } while (!(u4Val & SDC_OCR_BUSY));

    LOG(7, "%s(%d) %s return %d retry:%d u4Val:0x%08x\n", __FILE__, __LINE__, __FUNCTION__, 0,
        i4Retry, u4Val);

    return S_OK;
}

//-------------------------------------------------------------------------
/** _Sdc_IsMMCCard
 *  Check if the card is MMC.
 *  @param  VOID
 *  @retval   S_OK : Success.
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
static INT32 _Sdc_IsMMCCard(VOID)
{
    UINT32 u4Val = 0;

    INT32 i4Ret, i4Retry = 0;

    /* SDC Reset Cmd. */
    i4Ret = _SDC_Reset();
    i4Ret = _SDC_Reset();
    i4Ret = _SDC_Reset();
    /* 3 Reset to avoid initial clock. */

    do
    {
        if (i4Retry++ >= TRY_OCR)
        {
            LOG(1, "%s(%d) %s return %d\n", __FILE__, __LINE__, __FUNCTION__, E_SD_R3_OCR_BUSY);
            return E_SD_R3_OCR_BUSY;
        }

        i4Ret = SDCHW_SendCmd(SD_CMD_1, SDC_OCR_SD_DEFAULT);
        UNUSED(i4Ret);

        u4Val = SDCHW_ReadResp(1);

        // NOTICE: this delay is add to wait MMC card handle OCR.
        // This delay time is an experimental value.
        if (!(u4Val & SDC_OCR_BUSY))
        {
            x_thread_delay(100);
        }
    } while (!(u4Val & SDC_OCR_BUSY));

    return S_OK;
}

//-------------------------------------------------------------------------
/** _Sdc_GetCID
 *  asks any card to send the CID numbers on the CMD line.
 *  @param  pu4CID  point to CID buffer.
 *  @retval   S_OK : Success.
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
static INT32 _Sdc_GetCID(UINT32 *pu4CID)
{
    INT32 i4Ret;

    ASSERT(pu4CID != NULL);
    i4Ret = SDCHW_SendCmd(SD_CMD_2, SDC_NO_ARG);

    if (i4Ret != S_OK)
    {
        return i4Ret;
    }

    pu4CID[0] = SDCHW_ReadResp(4);
    pu4CID[1] = SDCHW_ReadResp(3);
    pu4CID[2] = SDCHW_ReadResp(2);
    pu4CID[3] = SDCHW_ReadResp(1);
    return S_OK;
}

//-------------------------------------------------------------------------
/** _Sdc_GetCSD
 *  Send CSD (card specific data) on the CMD line.
 *  @param  u2RCA  relative card address.
 *  @param  pu4CSD  point to CSD buffer.
 *  @retval   S_OK : Success.
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
static INT32 _Sdc_GetCSD(UINT16 u2RCA, UINT32 *pu4CSD)
{
    INT32 i4Ret;

    ASSERT(pu4CSD != NULL);
    i4Ret = SDCHW_SendCmd(SD_CMD_9, (UINT32)u2RCA << 16);

    if (i4Ret != S_OK)
    {
        return i4Ret;
    }

    pu4CSD[0] = SDCHW_ReadResp(4);
    pu4CSD[1] = SDCHW_ReadResp(3);
    pu4CSD[2] = SDCHW_ReadResp(2);
    pu4CSD[3] = SDCHW_ReadResp(1);
    return S_OK;
}

//-------------------------------------------------------------------------
/** _Sdc_SetDSR
 *  set default DSR (driver stage register).
 *  @param  VOID.
 *  @retval   S_OK : Success.
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
static INT32 _Sdc_SetDSR(VOID)
{
    INT32 i4Ret;

    i4Ret = SDCHW_SendCmd(SD_CMD_4, (UINT32)SD_DEF_DSR << 16);
    return i4Ret;
}

#ifdef CC_HIGH_SPEED_MODE_ENABLE
//-------------------------------------------------------------------------
/** _Sdc_SendCmd6
 *  send switch commands. Only use at SD card.
 *  @param  pbBuf  SD or MMC type.
 *  @param  dwArg  relative card address.
 *  @retval   S_OK : Success.
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
static INT32 _Sdc_SendCmd6(BYTE *pbBuf, UINT32 dwArg)
{
    INT32  i4Ret;

    // Read 64 bytes = 512 bits
    i4Ret = SDCHW_SendCmd(SD_CMD_6 | 64, dwArg);

    if (i4Ret != S_OK)
    {
        LOG(7, "Send Cmd 6 failed 1 = 0x%x.\n", i4Ret);
        return i4Ret;
    }

    i4Ret = _SDC_CheckStatus(SD_CMD_IDX(SD_CMD_6));

    if (i4Ret != S_OK)
    {
        LOG(7, "Send Cmd 6 failed 2 = 0x%x.\n", i4Ret);
        return i4Ret;
    }

    // Read 16 word = 64 byts = 512 bits
    i4Ret = SDCHW_ReadDataReg((UINT32 *)((UINT32)pbBuf), 16);

    if (i4Ret != S_OK)
    {
        LOG(7, "Send Cmd 6 failed 3 = 0x%x.\n", i4Ret);
        return i4Ret;
    }

    return i4Ret;
}

//-------------------------------------------------------------------------
/** _Sdc_SDHC_HighSpeed
 *  Switch Speed procedure : switch SDHC card into High speed mode.
 *  @retval   S_OK : Success.
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
static INT32 _Sdc_SDHC_HighSpeed(BOOL bHighSpeed)
{
    INT32  i4Ret;
    BYTE pbBuf[64] = {0};
    UINT32 u4SpeedSetting;
    BYTE bCheckByte;
    
    if (bHighSpeed)
    {
        bCheckByte = 0x01;
        u4SpeedSetting = SDC_CMD6_HIGH_SPEED;
    }
    else
    {
        bCheckByte = 0x00;    
        u4SpeedSetting = SDC_CMD6_DEFAULT_SPEED;
    }

    if ((i4Ret = _Sdc_SendCmd6((BYTE*)pbBuf, SDC_CMD6_CHECK_FUNC | SDC_CMD6_HIGH_SPEED)) != S_OK)
    {
        LOG(1, "_Sdc_SDHC_HighSpeed e1. \n");
        return i4Ret;
    }

    if(*((UINT16*)(pbBuf+0)) != 0x00) // Bit 511 ~ 496 ~= 0 means No Error
    {
        if(pbBuf[13] & 0x02)      // Check bit 401 : bit 1 = 1 means HIGH speed function is supported
        {
              if(pbBuf[16] == 0x01)  // Check bit 379 ~ 376 : 1 means the function can be switched
              {
                  if ((i4Ret = _Sdc_SendCmd6((BYTE*)pbBuf, SDC_CMD6_SWITCH_FUNC|u4SpeedSetting)) != S_OK)
                  {
                      LOG(1, "_Sdc_SDHC_HighSpeed e2. \n");
                      return i4Ret;
                  }

                  if(pbBuf[16] == bCheckByte)  // check if the function switch ok
                  {
                      LOG(1, "SDHC Switch Speed OK. \n");
                      return S_OK;
                  }
                  else
                  {
                      LOG(1, "SDHC Switch Speed Failed e3. \n");
                      return E_FAIL;
                  }
              }
        }
    }

    return E_FAIL;
}

//-------------------------------------------------------------------------
/** _Sdc_MMC_HighSpeed
 *  Switch Speed procedure : switch MMC card into High speed mode (max to 52 Mhz for MMC 4.0).
 *  @retval   S_OK : Success.
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
static INT32 _Sdc_MMC_HighSpeed(BOOL bHighSpeed)
{
    INT32 i4Ret;
    UINT32 u4SpeedSetting;

    if (bHighSpeed)
    {
        u4SpeedSetting = MMC_CMD6_ARG_NORM_SPEED;
    }
    else
    {
        u4SpeedSetting = MMC_CMD6_ARG_HIGH_SPEED;
    }

    // Send cmd 6 to change EXT_CSD - byte 183th BUS_WIDTH field.
    i4Ret = SDCHW_SendCmd(SD_CMD_6_MMC, u4SpeedSetting);

    if (i4Ret != S_OK)
    {
        LOG(1, "%s(%d) %s CMD_6_MMC failed to set High Speed Mode. Ret:0x%08x\n", __FILE__, __LINE__,
            __FUNCTION__, i4Ret);
        return i4Ret;
    }
    else
    {
        LOG(6, "%s(%d) %s CMD_6_MMC success to set High Speed Mode.\n", __FILE__, __LINE__,
            __FUNCTION__);
    }

    i4Ret = _SDC_CheckStatus(SD_CMD_IDX(SD_CMD_6_MMC));

    if (i4Ret != S_OK)
    {
        LOG(1, "%s(%d) %s CheckStatus failed %d\n", __FILE__, __LINE__, __FUNCTION__, i4Ret);
        return i4Ret;
    }

    return S_OK;
}
#endif  // #ifdef CC_HIGH_SPEED_MODE_ENABLE

//-------------------------------------------------------------------------
/** _Sdc_GetAddrCID
 *  addressed card sends its card identification on CMD line.
 *  @param  u4CardType  SD or MMC type.
 *  @param  u2RCA  relative card address.
 *  @param  pu4CID  point to CID buffer.
 *  @retval   S_OK : Success.
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
static INT32 _Sdc_GetAddrCID(UINT32 u4CardType, UINT16 u2RCA, UINT32 *pu4CID)
{
    INT32 i4Ret;

    ASSERT(pu4CID != NULL);

    if (u4CardType == FCI_SD_CARD)
    {
        i4Ret = SDCHW_SendCmd(SD_CMD_10, (UINT32)u2RCA << 16);
        if (i4Ret != S_OK)
        {
            return i4Ret;
        }
    }
    else if (u4CardType == FCI_MMC_CARD)
    {
        i4Ret = SDCHW_SendCmd(MMC_CMD_10, (UINT32)u2RCA << 16);
        if (i4Ret != S_OK)
        {
            return i4Ret;
        }
    }
    else
    {
        return E_INVALID_CARD;
    }

    pu4CID[0] = SDCHW_ReadResp(4);
    pu4CID[1] = SDCHW_ReadResp(3);
    pu4CID[2] = SDCHW_ReadResp(2);
    pu4CID[3] = SDCHW_ReadResp(1);
    return S_OK;
}

//-------------------------------------------------------------------------
/** _Sdc_SetBlkSz
 *  Set block size.
 *  @param  u2BlkSz  block size.
 *  @retval   S_OK : Success.
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
static INT32 _Sdc_SetBlkSz(UINT16 u2BlkSz)
{
    INT32 i4Ret;

    if (u2BlkSz > SDC_MAX_BKLENGTH)
    {
        return E_INVALID_BLOCK_SIZE;
    }

    i4Ret = SDCHW_SendCmd(SD_CMD_16, u2BlkSz);

    if (i4Ret != S_OK)
    {
        return i4Ret;
    }

    i4Ret = _SDC_CheckStatus(SD_CMD_IDX(SD_CMD_16));

    if (i4Ret != S_OK)
    {
        return i4Ret;
    }

    return S_OK;
}

//-------------------------------------------------------------------------
/** _Sdc_GetSDSCR
 *  Get SD card configuration register.
 *  @param  prSDC  point to SD data structure.
 *  @retval   S_OK : Success.
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
static INT32 _Sdc_GetSDSCR(SDC_T *prSDC)
{
    INT32 i4Ret;

    ASSERT(prSDC != NULL);

    i4Ret = _Sdc_SetBlkSz(8);

    if (i4Ret != S_OK)
    {
        LOG(1, "%s(%d) GetSDSCR failed return %d\n", __FILE__, __LINE__, i4Ret);
        return i4Ret;
    }

    prSDC->u2BlkSz[SDC_COMMOM] = 8;

    i4Ret = _SDC_SendCmd55(prSDC->u2RCA);

    if (i4Ret != S_OK)
    {
        LOG(1, "%s(%d) GetSDSCR failed return %d\n", __FILE__, __LINE__, i4Ret);
        return i4Ret;
    }

    i4Ret = SDCHW_SendCmd(SD_ACMD_51 | 8, SD_CMD_NO_ARG);

    if (i4Ret != S_OK)
    {
        LOG(1, "%s(%d) GetSDSCR failed return %d\n", __FILE__, __LINE__, i4Ret);
        return i4Ret;
    }

    i4Ret = _SDC_CheckStatus(SD_CMD_IDX(SD_ACMD_51));

    if (i4Ret != S_OK)
    {
        LOG(1, "%s(%d) GetSDSCR failed return %d\n", __FILE__, __LINE__, i4Ret);
        return i4Ret;
    }

    i4Ret = SDCHW_ReadDataReg(prSDC->au4SCR, 2);

    if (i4Ret != S_OK)
    {
        LOG(1, "%s(%d) GetSDSCR failed return %d\n", __FILE__, __LINE__, i4Ret);
        return i4Ret;
    }

    return S_OK;
}

//-------------------------------------------------------------------------
/** _Sdc_StopTrans
 *  force card to stop transmission.
 *  @param  VOID.
  *  @retval   S_OK : Success.
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
static INT32 _Sdc_StopTrans(VOID)
{
    INT32 i4Ret;

    i4Ret = SDCHW_SendCmd(SD_CMD_12, SDC_NO_ARG);

    if (i4Ret != S_OK)
    {
        return i4Ret;
    }

    i4Ret = _SDC_CheckStatus(SD_CMD_IDX(SD_CMD_12));

    if (i4Ret != S_OK)
    {
        return i4Ret;
    }

    return S_OK;
}

//-------------------------------------------------------------------------
/** _Sdc_GetStatus
 *  Get card status.
 *  @param  u2RCA   relative card address.
 *  @param  pu4Tmp   point to status buffer.
 *  @retval   S_OK : Success.
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
static INT32 _Sdc_GetStatus(UINT16 u2RCA, UINT32 *pu4Tmp)
{
    INT32 i4Ret;

    ASSERT(pu4Tmp != NULL);
    i4Ret = SDCHW_SendCmd(SD_CMD_13, (UINT32)u2RCA << 16);

    if (i4Ret != S_OK)
    {
        return i4Ret;
    }

    *pu4Tmp = SDCHW_ReadResp(1);
    return S_OK;
}

//-------------------------------------------------------------------------
/** _Sdc_ReadSingleBlk
 *  SD read single block function.
 *  @param  u4Addr   data address.
 *  @param  pu1Buf   point to user's data buffer.
 *  @retval   S_OK : Success.
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------

static INT32 _Sdc_ReadSingleBlk(UINT32 u4Addr, UINT8 *pu1Buf)
{
    INT32 i4Ret;

    SDC_T *prSDC;

    prSDC = FCI_GetSDCSlot();
    FCIHW_SetFifoRead();
    FCIHW_DmaWriteDramEnable(pu1Buf, prSDC->u2BlkSz[SDC_COMMOM]);
    //_SDC_ChangeClock(SDC_HIGH_CLOCK);

    // open SDDR interrupt, and close SDWBF interrupt.
    SDCHW_OpenDataRdyInt();

    i4Ret = SDCHW_SendCmd(SD_CMD_17 | prSDC->u2BlkSz[SDC_COMMOM], u4Addr);

    if (i4Ret != S_OK)
    {
        LOG(1, "%s(%d) %s failed return %d\n", __FILE__, __LINE__, __FUNCTION__, i4Ret);
        goto RdSingleRet;
    }

    i4Ret = _SDC_CheckStatus(SD_CMD_IDX(SD_CMD_17));

    if (i4Ret != S_OK)
    {
        LOG(1, "%s(%d) %s failed return %d\n", __FILE__, __LINE__, __FUNCTION__, i4Ret);
        goto RdSingleRet;
    }

    // wait SD Data line ready interrupt.
    i4Ret = SDCHW_WaitDataRdyOrTo();

    if (i4Ret != S_OK)
    {
        LOG(1, "%s(%d) %s failed return %d\n", __FILE__, __LINE__, __FUNCTION__, i4Ret);
        goto RdSingleRet;
    }

    RdSingleRet:
    FCIHW_DmaDisable();

    //_SDC_ChangeClock(SDC_LOW_CLOCK);
    return i4Ret;
}

//-------------------------------------------------------------------------
/** _Sdc_ReadMultiBlk
 *  SD read multi-block function.
 *  @param  u4Addr   data address.
 *  @param  pu1Buf   point to user's data buffer.
 *  @param  u4BlkNs   read data block number.
 *  @retval   S_OK : Success.
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
static INT32 _Sdc_ReadMultiBlk(UINT32 u4Addr, UINT8 *pu1Buf, UINT32 u4BlkNs)
{
    INT32 i4Ret;

    INT32 i4Ret1;
    //INT32 i4Tmp;

    SDC_T *prSDC;
    UINT32 u4BlkField;

    prSDC = FCI_GetSDCSlot();
    SDCHW_SetMultiFlag(TRUE);
    FCIHW_SetFifoRead();
    FCIHW_DmaWriteDramEnable(pu1Buf, prSDC->u2BlkSz[SDC_COMMOM] *u4BlkNs);
    //_SDC_ChangeClock(SDC_HIGH_CLOCK);

    // open SDDR interrupt, and close SDWBF interrupt.
    SDCHW_OpenDataRdyInt();

    ASSERT(((u4BlkNs - 1) & 0xffffff80) == 0);
    u4BlkField = (((u4BlkNs - 1) & 0x07f) << 12);

    i4Ret = SDCHW_SendCmd(SD_CMD_18 | u4BlkField | prSDC->u2BlkSz[SDC_COMMOM], u4Addr);

    if (i4Ret != S_OK)
    {
        LOG(1, "%s(%d) SendCmd failed %d\n", __FILE__, __LINE__, i4Ret);
        goto RdMultiRet;
    }

    i4Ret = _SDC_CheckStatus(SD_CMD_IDX(SD_CMD_18));

    if (i4Ret != S_OK)
    {
        LOG(1, "%s(%d) check status failed %d\n", __FILE__, __LINE__, i4Ret);
        goto RdMultiRet;
    }

    i4Ret = SDCHW_WaitDataRdyOrTo();

    if (i4Ret != S_OK)
    {
        LOG(1, "%s(%d) wait data rdy failed %d\n", __FILE__, __LINE__, i4Ret);
        goto RdMultiRet;
    }

    RdMultiRet:
    //_SDC_ChangeClock(SDC_LOW_CLOCK);

    i4Ret1 = _Sdc_StopTrans();

    if (i4Ret1 != S_OK)
    {
        LOG(1, "%s(%d) stop trans failed %d\n", __FILE__, __LINE__, i4Ret1);
    }

    //i4Ret = _Sdc_GetStatus(prSDC->u2RCA, &u4Tmp);
    //LOG(1, "%s(%d) status=0x%08X.\n", __FILE__, __LINE__, u4Tmp);

    FCIHW_DmaDisable();
    SDCHW_SetMultiFlag(FALSE);
    return i4Ret;
}

//-------------------------------------------------------------------------
/** _Sdc_WriteSingleBlk
 *  SD write single block function.
 *  @param  u4Addr   data address.
 *  @param  pu1Buf   point to user's data buffer.
 *  @retval   S_OK : Success.
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
static INT32 _Sdc_WriteSingleBlk(UINT32 u4Addr, UINT8 *pu1Buf)
{
    INT32 i4Ret;

    SDC_T *prSDC;
    UINT32 u4Tmp;
    UINT32 u4Count = 0;

    prSDC = FCI_GetSDCSlot();
    FCIHW_SetFifoWrite();
    //_SDC_ChangeClock(SDC_HIGH_CLOCK);

    // turn off SDDR and turn on SDWBF.
    SDCHW_OpenWBFInt();

    i4Ret = SDCHW_SendCmd(SD_CMD_24 | prSDC->u2BlkSz[SDC_COMMOM], u4Addr);

    if (i4Ret != S_OK)
    {
        LOG(1, "%s(%d) %s failed return %d\n", __FILE__, __LINE__, __FUNCTION__, i4Ret);
        goto WrSingleRet;
    }

    i4Ret = _SDC_CheckStatus(SD_CMD_IDX(SD_CMD_24));

    if (i4Ret != S_OK)
    {
        LOG(1, "%s(%d) %s failed return %d\n", __FILE__, __LINE__, __FUNCTION__, i4Ret);
        goto WrSingleRet;
    }

    // Make sure all the semaphore is in lock state.
    SDCHW_InitSemaphore();

    FCIHW_DmaReadDramEnable(pu1Buf, prSDC->u2BlkSz[SDC_COMMOM]);

    i4Ret = SDCHW_WaitDataIdle();

    if (i4Ret != S_OK)
    {
        LOG(1, "%s(%d) %s failed return %d\n", __FILE__, __LINE__, __FUNCTION__, i4Ret);
        goto WrSingleRet;
    }

    // wait program finish.
    do
    {
        i4Ret = _Sdc_GetStatus(prSDC->u2RCA, &u4Tmp);

        if (i4Ret != S_OK)
        {
            LOG(1, "%s(%d) %s failed return %d\n", __FILE__, __LINE__, __FUNCTION__, i4Ret);
            goto WrSingleRet;
        }
        u4Count++;
    } while ((u4Tmp != 0x900) && (u4Count < 100));

    i4Ret = (u4Tmp != 0x900) ? E_FAIL : S_OK;

    WrSingleRet:
    FCIHW_DmaDisable();

    //_SDC_ChangeClock(SDC_LOW_CLOCK);

    return i4Ret;
}

//-------------------------------------------------------------------------
/** _Sdc_WriteMultiBlk
 *  SD write multi-block function.
 *  @param  u4Addr   data address.
 *  @param  pu1Buf   point to user's data buffer.
 *  @param  u4BlkNs   write data block number.
 *  @retval   S_OK : Success.
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
static INT32 _Sdc_WriteMultiBlk(UINT32 u4Addr, UINT8 *pu1Buf, UINT32 u4BlkNs)
{
    INT32 i4Ret;

    SDC_T *prSDC;
    UINT32 u4Tmp, u4BlkField;
    UINT32 u4Count = 0;

    prSDC = FCI_GetSDCSlot();
    SDCHW_SetMultiFlag(TRUE);
    FCIHW_SetFifoWrite();
    //_SDC_ChangeClock(SDC_HIGH_CLOCK);

    // turn off SDDR and turn on SDWBF.
    SDCHW_OpenWBFInt();

    ASSERT(((u4BlkNs - 1) & 0xffffff80) == 0);
    u4BlkField = (((u4BlkNs - 1) & 0x07f) << 12);

    i4Ret = SDCHW_SendCmd(SD_CMD_25 | u4BlkField | prSDC->u2BlkSz[SDC_COMMOM], u4Addr);

    if (i4Ret != S_OK)
    {
        LOG(1, "%s(%d) %s failed return %d\n", __FILE__, __LINE__, __FUNCTION__, i4Ret);
        goto WrMultiRet;
    }

    i4Ret = _SDC_CheckStatus(SD_CMD_IDX(SD_CMD_25));

    if (i4Ret != S_OK)
    {
        LOG(1, "%s(%d) %s failed return %d\n", __FILE__, __LINE__, __FUNCTION__, i4Ret);
        goto WrMultiRet;
    }

    // Make sure all the semaphore is in lock state.
    SDCHW_InitSemaphore();

    // Set DMA size and address.
    u4Tmp = (UINT32)(prSDC->u2BlkSz[SDC_COMMOM] *u4BlkNs);
    FCIHW_DmaReadDramEnable(pu1Buf, u4Tmp);

    // wait data program finish.
    i4Ret = SDCHW_WaitDataIdle();
    if (i4Ret != S_OK)
    {
       LOG(1, "%s(%d) %s failed return %d\n", __FILE__, __LINE__, __FUNCTION__, i4Ret);
       goto WrMultiRet;
    }

    i4Ret = _Sdc_StopTrans();

    if (i4Ret != S_OK)
    {
        LOG(1, "%s(%d) %s failed return %d\n", __FILE__, __LINE__, __FUNCTION__, i4Ret);
        goto WrMultiRet;
    }

    // wait program finish.
    do
    {
        i4Ret = _Sdc_GetStatus(prSDC->u2RCA, &u4Tmp);

        if (i4Ret != S_OK)
        {
            LOG(1, "%s(%d) %s failed return %d\n", __FILE__, __LINE__, __FUNCTION__, i4Ret);
            goto WrMultiRet;
        }
        u4Count++;
    } while ((u4Tmp != 0x900) && (u4Count < 100));

    i4Ret = (u4Tmp != 0x900) ? E_FAIL : S_OK;

    WrMultiRet:
    FCIHW_DmaDisable();

    SDCHW_SetMultiFlag(FALSE);
    //_SDC_ChangeClock(SDC_LOW_CLOCK);

    return i4Ret;
}

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

//-------------------------------------------------------------------------
/** _SDC_Reset
 *  reset card.
 *  @param  VOID
 *  @retval   S_OK : Success.
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
INT32 _SDC_Reset(VOID)
{
    INT32 i4Ret;

    i4Ret = SDCHW_SendCmd(SD_CMD_0, SD_CMD_NO_ARG);

    if (i4Ret)
    {
        LOG(7, "SDC Slot reset failed.\n");
    }

    x_thread_delay(10);
    return i4Ret;
}

//-------------------------------------------------------------------------
/** _Sdc_SendIfCondCmd
*    CMD8 (Send Interface Condition Command) is defined to initialize SD Memory Cards compliant to the
*    Physical Specification Version 2.00. CMD8 is valid when the card is in Idle state. This command has two
*    functions.
*    ¡E Voltage check:
*        Checks whether the card can operate on the host supply voltage.
*    ¡E Enabling expansion of existing command and response:
*        Reviving CMD8 enables to expand new functionality to some existing commands by redefining
*        previously reserved bits. ACMD41 is expanded to support initialization of High Capacity SD
*        Memory Cards.
 *  @param  pu4CID  point to CID buffer.
 *  @retval   S_OK : Success.
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
INT32 _SDC_SendIfCondCmd(void)
{
    INT32 i4Ret;

    UINT32 u4Val;

    i4Ret = SDCHW_SendCmd(SD_CMD_8, SDC_HVS_DEFAULT);

    if (i4Ret != S_OK)
    {
        return i4Ret;
    }

    u4Val = SDCHW_ReadResp(1);

    if (u4Val == SDC_HVS_DEFAULT)
    {
        return S_OK;
    }
    else
    {
        return E_FAIL;
    }
}

//-------------------------------------------------------------------------
/** _SDC_SendCmd55
 *  send application specific commands. Only use at SD card.
 *  @param  pu2RCA  point to RCA buffer.
 *  @retval   S_OK : Success.
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
INT32 _SDC_SendCmd55(UINT16 u2RCA)
{
    UINT32 u4Val;

    INT32 i4Ret;

    i4Ret = SDCHW_SendCmd(SD_CMD_55, u2RCA << 16);

    if (i4Ret != S_OK)
    {
        LOG(6, "%s(%d) %s return %d\n", __FILE__, __LINE__, __FUNCTION__, i4Ret);
        return i4Ret;
    }

    i4Ret = _SDC_CheckStatus(SD_CMD_IDX(SD_CMD_55));

    if (i4Ret != S_OK)
    {
        LOG(1, "%s(%d) %s return %d\n", __FILE__, __LINE__, __FUNCTION__, i4Ret);
        return i4Ret;
    }

    u4Val = SDCHW_ReadResp(1);

    if (!(u4Val & R1_APP_CMD_5))
    {
        LOG(1, "%s(%d) %s return %d\n", __FILE__, __LINE__, __FUNCTION__, E_SD_APPCMD_FAILED);
        return E_SD_APPCMD_FAILED;
    }

    return S_OK;
}

//-------------------------------------------------------------------------
/** _SDC_GetRCA
 *  Get RCA (relative card address) on the CMD line.
 *  @param  u4CardType  SD or MMC.
 *  @param  pu2RCA  point to RCA buffer.
 *  @retval   S_OK : Success.
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
INT32 _SDC_GetRCA(UINT32 u4CardType, UINT16 *pu2RCA)
{
    INT32 i4Ret;

    UINT32 u4Val;

    ASSERT(pu2RCA != NULL);

    if (u4CardType == FCI_SD_CARD)
    {
        i4Ret = SDCHW_SendCmd(SD_CMD_3_SD, SD_CMD_NO_ARG);
        if (i4Ret)
        {
            return i4Ret;
        }
    }
    else if (u4CardType == FCI_MMC_CARD)
    {
        i4Ret = SDCHW_SendCmd(MMC_CMD_3, SD_CMD_NO_ARG);
        if (i4Ret)
        {
            return i4Ret;
        }
    }
    else
    {
        return E_INVALID_CARD;
    }

    u4Val = SDCHW_ReadResp(1);
    *pu2RCA = (UINT16)(u4Val >> 16);
    return S_OK;
}

//-------------------------------------------------------------------------
/** _SDC_SelectCard
 *  Select the card with specific RCA.
 *  @param  u2RCA  relative card address.
 *  @retval   S_OK : Success.
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
INT32 _SDC_SelectCard(UINT16 u2RCA)
{
    INT32 i4Ret;

    i4Ret = SDCHW_SendCmd(SD_CMD_7, (UINT32)u2RCA << 16);

    if (i4Ret != S_OK)
    {
        LOG(1, "%s(%d) %s return %d\n", __FILE__, __LINE__, __FUNCTION__, i4Ret);
        return i4Ret;
    }

    i4Ret = _SDC_CheckStatus(SD_CMD_IDX(SD_CMD_7));

    if (i4Ret != S_OK)
    {
        LOG(1, "%s(%d) %s return %d\n", __FILE__, __LINE__, __FUNCTION__, i4Ret);
        return i4Ret;
    }

    return S_OK;
}

//-------------------------------------------------------------------------
/** _SDC_CheckStatus
 *  check card response
 *  @param  pu2RCA  point to RCA buffer.
 *  @retval   S_OK : Success.
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
INT32 _SDC_CheckStatus(UINT32 u4CmdIdx)
{
    UINT32 u4Val;

    INT32 i4Count;

    u4Val = SDCHW_CmdIdx();
    LOG(9, "RegIdx is 0x%02x CmdIdx is 0x%02x\n", u4Val, u4CmdIdx);
    i4Count = 0;

    do
    {
        u4Val = SDCHW_CmdIdx();

        i4Count++;
        if (i4Count > 5)
        {
            return E_STATUS;
        }
    } while (u4Val != u4CmdIdx);

    u4Val = SDCHW_ReadResp(1);

    if ((u4Val & SDC_CSTA_MASK) == 0)
    {
        return S_OK;
    }
    /* NOTICE: THIS IS VERY TRICKY.... !!!!!
         if next sector is out of range, this bit will on.
        so, let's ignore this bit. XXX */
    if ((u4Val & SDC_CSTA_MASK) == SDC_OUT_OF_RANGE)
    {
        return S_OK;
    }

    LOG(1, "SDC Check status failed. u4Val:0x%08x\n", u4Val);

    if (u4Val & SDC_CARD_IS_LOCKED)
    {
        return E_CARD_IS_LOCKED;
    }

    return E_STATUS;
}

//-------------------------------------------------------------------------
/** _SDC_ChangeClock
 *  Change SD card clock.
 *  @param  fgClock   TRUE: use high clock. FALSE: use low clock.
 *  @return   VOID
 */
//-------------------------------------------------------------------------
VOID _SDC_ChangeClock(INT32 i4Clock)
{
    // Set clock according to your selection
    if(i4Clock == SDC_HIGH_CLOCK)
    {
        // about 50 Khz
        FCIHW_ChangeSDCClock(_u4SdHighClock);
    }
    else if(i4Clock == SDC_NORM_CLOCK)
    {
        // about 25 Khz
        FCIHW_ChangeSDCClock(_u4SdNormalClock);
    }
    else if(i4Clock == SDC_LOW_CLOCK)
    {
        // 421.875 Khz for identify device
        FCIHW_ChangeSDCClock(_u4SdLowClock);    
    }
}

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------
//-------------------------------------------------------------------------
/** SD_SwitchHighSpeed
 *  SD & MMC switch speed function
 *  @param  bHighSpeed   TRUE : high speed, FALSE : Normal speed
 *  @retval   1 : failed
 *  @retval   0 : successfully.
 */
//-------------------------------------------------------------------------

INT32 SD_SwitchHighSpeed(BOOL bHighSpeed)
{
#ifdef CC_HIGH_SPEED_MODE_ENABLE
    INT32 i4Ret = 0;    
    SDC_T *prSDC;
  
    prSDC = FCI_GetSDCSlot();
    
    if(_u4SDCardType == FCI_SDHC_CARD)
    {
        i4Ret = _Sdc_SDHC_HighSpeed(bHighSpeed);

        if (i4Ret != S_OK)
        {
            LOG(1, "SDHC switch speed failed at %s(%d)\n", __FILE__, __LINE__);
        }
        else
        {
            if (bHighSpeed)
            {
                LOG(1, "SDHC Card in High Speed Mode.\n");
            }
            else
            {
                LOG(1, "SDHC Card in Normal Speed Mode.\n");
            }
        }
    }
    else if (_u4SDCardType == FCI_MMC_CARD)
    {
        // Check MMC SpecVers 4.0, 4.1 or 4.2 => support 4 bit, 8 bit mode
        if (((prSDC->au4CSD[3] >> 26) & 0x0F) == 4)
        {
            // Set high speed mode (52 Mhz max)
            i4Ret = _Sdc_MMC_HighSpeed(bHighSpeed);

            if (i4Ret != S_OK)
            {
                LOG(1, "MMC Card switch high speed failed at %s(%d)\n", __FILE__, __LINE__);
            }
            else
            {
                if (bHighSpeed)
                {
                    LOG(1, "MMC Card in High Speed Mode.\n");
                }
                else
                {
                    LOG(1, "MMC Card in Normal Speed Mode.\n");
                }                
            }
        }
        else
        {
            LOG(1, "MMC Card not support High Speed Mode.\n");
        }
    }

    return i4Ret;
#else
    return 0;
#endif
}

//-------------------------------------------------------------------------
/** SDC_Init
 *  SD detect and attach.
 *  @param  prSDC   point to SD data structure.
 *  @retval   1 : Device is failed to detect.
 *  @retval   0 : Device is attached successfully.
 */
//-------------------------------------------------------------------------
INT32 SDC_Init(SDC_T *prSDC)
{
    INT32 i4Ret;

    UINT32 u4BlkLen;
    UINT32 u4BlkNr;
    UINT32 u4Mult;
    INT32 i4Retry = 0;

#ifdef CC_MT5360B
    UINT32 u4Val;
#endif

    ASSERT(prSDC != NULL);
    SDCHW_Init();

    _SDC_ChangeClock(SDC_LOW_CLOCK);

    // reset data structure.
    x_memset((VOID *)prSDC, 0, sizeof(SDC_T));

/*
#ifdef CC_MT5360B   // [5360B] one bit for control SD DI direct input from PAD
    u4Val = FCI_READ32(RW_FCI_INTR_CTRL_REG);
    u4Val &= ~(INTR_SDDI_DIRECT_IN);    // Set 0 : let SDDI direct input from PAD
    FCI_WRITE32(RW_FCI_INTR_CTRL_REG, u4Val);
#endif
*/

    while (i4Retry++ < 3)
    {
        if ((_u4SdDetectCardType == FCI_SDHC_CARD || _u4SdDetectCardType == FCI_SD_CARD) &&
            _Sdc_IsSDCard() == S_OK)
        {
            // Detect SD card in slot.
            prSDC->u4State = SLOT_STATE_INIT;

            prSDC->u4CardType = FCI_SD_CARD;

            _SDC_ChangeClock(SDC_NORM_CLOCK);
            break;
        }
        else if (_u4SdDetectCardType == FCI_MMC_CARD &&
                 _Sdc_IsMMCCard() == S_OK)
        {
            // Detect MMC card in slot.
            prSDC->u4State = SLOT_STATE_INIT;

            prSDC->u4CardType = FCI_MMC_CARD;
            _u4SDCardType = FCI_MMC_CARD;
            break;
        }
    }

    if ((prSDC->u4CardType != FCI_SD_CARD) && (prSDC->u4CardType != FCI_MMC_CARD))
    {
        LOG(1, "SDCInit failed at %s(%d)\n", __FILE__, __LINE__);
        /* What is in slot? */
        prSDC->u4State = SLOT_STATE_UNKNOWN;
        prSDC->u4CardType = FCI_UNKNOWN_CARD;
        /* then do nothing. */
        return 1;
    }

    LOG(7, "CID size is %d\n", sizeof(prSDC->au4CID));

    i4Ret = _Sdc_GetCID(prSDC->au4CID);

    if (i4Ret != S_OK)
    {
        LOG(1, "SDCInit failed at %s(%d)\n", __FILE__, __LINE__);
        /* What is in slot? */
        prSDC->u4State = SLOT_STATE_UNKNOWN;
        prSDC->u4CardType = FCI_UNKNOWN_CARD;
        /* then do nothing. */
        return 1;
    }

    /* SD is different from MMC on RCA. */
    i4Ret = _SDC_GetRCA(prSDC->u4CardType, &prSDC->u2RCA);

    if (i4Ret != S_OK)
    {
        LOG(1, "SDCInit failed at %s(%d)\n", __FILE__, __LINE__);
        /* What is in slot? */
        prSDC->u4State = SLOT_STATE_UNKNOWN;
        prSDC->u4CardType = FCI_UNKNOWN_CARD;
        /* then do nothing. */
        return 1;
    }

    LOG(7, "SD Slot RCA is 0x%04x\n", prSDC->u2RCA);

    LOG(7, "CSD size is %d\n", sizeof(prSDC->au4CSD));
    i4Ret = _Sdc_GetCSD(prSDC->u2RCA, prSDC->au4CSD);

    if (i4Ret != S_OK)
    {
        LOG(1, "SDCInit failed at %s(%d)\n", __FILE__, __LINE__);
        /* What is in slot? */
        prSDC->u4State = SLOT_STATE_UNKNOWN;
        prSDC->u4CardType = FCI_UNKNOWN_CARD;
        /* then do nothing. */
        return 1;
    }

    if (_u4SDCardType == FCI_SDHC_CARD)
    {
        /*
        C_SIZE: Physical Layer Specification Version 2.00 page 109.
        The user data area capacity (not include the
        protected area) is calculated from C_SIZE as follows:
        memory capacity = (C_SIZE+1) * 512K byte
        As the maximum capacity of the Physical Layer Specification Version 2.00 is 32 GB,
        the upper 6 bits of this field shall be set to 0.
        */
        u4BlkNr = (SDHC_CSD_CSIZE(prSDC->au4CSD) + 1);

        u4BlkLen = 1 << (SD_CSD_READBLLEN(prSDC->au4CSD));
        LOG(7, "block size = 0x%08X.\n", u4BlkLen);
        prSDC->u4CardSize = u4BlkNr * 1024;
        LOG(7, "card size/block = 0x%08X.\n", prSDC->u4CardSize);
    }
    else
    {
        // Notice: MMC, SD are compatible in the following field:
        // C_SIZE_MULT  [49:47]
        // C_SIZE   [73:62]
        // READ_BL_LEN   [83:80]
        //
        // card capacity = BLOCKNR * BLOCK_LEN bytes.
        //
        // MULT = 2^(C_SIZE_MULT+2)
        // BLOCKNR = (C_SIZE + 1)*MULT
        // BLOCK_LEN =  2^ READ_BL_LEN
        u4Mult = 1 << (SD_CSD_CSIZEMULT(prSDC->au4CSD) + 2);

        u4BlkNr = (SD_CSD_CSIZE(prSDC->au4CSD) + 1) * u4Mult;
        u4BlkLen = 1 << (SD_CSD_READBLLEN(prSDC->au4CSD));
        LOG(7, "block size = 0x%08X.\n", u4BlkLen);
        prSDC->u4CardSize = (u4BlkNr * u4BlkLen)/512;
        LOG(7, "card size/block = 0x%08X.\n", u4BlkNr);
    }

    if (SD_CSD_SPEED(prSDC->au4CSD) == 0x32)
    {
        LOG(7, "speed = 25 MHz.\n");
    }
    else if (SD_CSD_SPEED(prSDC->au4CSD) == 0x5A)
    {
        LOG(7, "speed = 50 MHz.\n");
    }
    else
    {
        LOG(7, "speed = 0x%X.\n", SD_CSD_SPEED(prSDC->au4CSD));
    }

    // Notice: MMC, SD are compatible in field: DSR_IMP [76:76]
    if (SD_CSD_DSRIMP(prSDC->au4CSD))
    {
        i4Ret = _Sdc_SetDSR();
        if (i4Ret != S_OK)
        {
            LOG(1, "SDCInit failed at %s(%d)\n", __FILE__, __LINE__);
            /* What is in slot? */
            prSDC->u4State = SLOT_STATE_UNKNOWN;
            prSDC->u4CardType = FCI_UNKNOWN_CARD;
            /* then do nothing. */
            return 1;
        }
    }

    i4Ret = _Sdc_GetAddrCID(prSDC->u4CardType, prSDC->u2RCA, prSDC->au4CID);

    if (i4Ret != S_OK)
    {
        LOG(1, "SDCInit failed at %s(%d)\n", __FILE__, __LINE__);
        /* What is in slot? */
        prSDC->u4State = SLOT_STATE_UNKNOWN;
        prSDC->u4CardType = FCI_UNKNOWN_CARD;
        /* then do nothing. */
        return 1;
    }

    i4Ret = _SDC_SelectCard(prSDC->u2RCA);

    if (i4Ret == E_CARD_IS_LOCKED)
    {
        /* what to do if locked? */
        ASSERT(0);
    }

    if (prSDC->u4CardType == FCI_SD_CARD)
    {
#ifdef CC_HIGH_SPEED_MODE_ENABLE
        // Swtich to high speed mode if SD-HC card
        if(_u4SDCardType == FCI_SDHC_CARD && _fgHighSpeedEnable)
        {
            i4Ret = _Sdc_SDHC_HighSpeed(TRUE);

            if (i4Ret != S_OK)
            {
                LOG(1, "SDHC switch high speed failed at %s(%d)\n", __FILE__, __LINE__);
            }
            else
            {
                LOG(1, "SDHC Card in High Speed Mode.\n");
            }
        }
#endif  // #ifdef CC_HIGH_SPEED_MODE_ENABLE

        i4Ret = _Sdc_GetSDSCR(prSDC);

        if (i4Ret != S_OK)
        {
            LOG(1, "SDCInit failed at %s(%d)\n", __FILE__, __LINE__);
            /* What is in slot? */
            prSDC->u4State = SLOT_STATE_UNKNOWN;
            prSDC->u4CardType = FCI_UNKNOWN_CARD;
            /* then do nothing. */
            return 1;
        }

        // log card information.
        LOG(7, "SCR_STRUCTURE=%d.\n", SD_SCR_STRUCTURE(prSDC->au4SCR));
        LOG(7, "SCR_SPEC=%d.\n", SD_SCR_SPEC(prSDC->au4SCR));
        LOG(7, "SCR_BUS_WIDTH=%d.\n", SD_SCR_BUS_WIDTHS(prSDC->au4SCR));
        LOG(7, "SCR_SECURITY=%d.\n", SD_SCR_SECURITY(prSDC->au4SCR));

        if (_fgFCI4BitBusEnable)
        {
            if ((SD_SCR_BUS_WIDTHS(prSDC->au4SCR) & 0x04) != 0)
            {
                i4Ret = SDC_SetSDBusWidth(TRUE);

                if (i4Ret != S_OK)
                {
                    LOG(1, "SDCInit (SetSDBusWidth) failed at %s(%d)\n", __FILE__, __LINE__);
                    /* What is in slot? */
                    prSDC->u4State = SLOT_STATE_UNKNOWN;
                    prSDC->u4CardType = FCI_UNKNOWN_CARD;
                    /* then do nothing. */
                    return 1;
                }
                prSDC->fg4BitEn = TRUE;
                LOG(1, "SD 4 bit bus mode.\n");
            }
        }
        else
        {
            i4Ret = SDC_SetSDBusWidth(FALSE);
        
            if (i4Ret != S_OK)
            {
                LOG(1, "SDCInit (SetSDBusWidth) failed at %s(%d)\n", __FILE__, __LINE__);
            }
            
            LOG(1, "SD 1 bit bus mode.\n");
        }

        // initialize SD product name string 5 bytes.
        prSDC->u1PNM[0] = SD_CID_PNM_B1(prSDC->au4CID);
        prSDC->u1PNM[1] = SD_CID_PNM_B2(prSDC->au4CID);
        prSDC->u1PNM[2] = SD_CID_PNM_B3(prSDC->au4CID);
        prSDC->u1PNM[3] = SD_CID_PNM_B4(prSDC->au4CID);
        prSDC->u1PNM[4] = SD_CID_PNM_B5(prSDC->au4CID);
        prSDC->u1PNM[5] = 0;
        prSDC->u1PNM[6] = 0;
    }
    else
    {
        LOG(7, "prSDC->au4CSD[3] = 0x%X\n",prSDC->au4CSD[3]);
        LOG(7, "prSDC->au4CSD[2] = 0x%X\n",prSDC->au4CSD[2]);
        LOG(7, "prSDC->au4CSD[1] = 0x%X\n",prSDC->au4CSD[1]);
        LOG(7, "prSDC->au4CSD[0] = 0x%X\n",prSDC->au4CSD[0]);

        // Check MMC SpecVers 4.0, 4.1 or 4.2 => support 4 bit, 8 bit mode
        if (((prSDC->au4CSD[3] >> 26) & 0x0F) == 4)
        {
            if (_fgFCI4BitBusEnable)
            {
                // Set bus width to 4 bit mode
                i4Ret = SDC_SetMMCBusWidth(TRUE);

                if (i4Ret != S_OK)
                {
                    LOG(1, "SDCInit (SetMMCBusWidth) failed at %s(%d)\n", __FILE__, __LINE__);
                }
                else
                {
                    prSDC->fg4BitEn = TRUE;
                    LOG(1, "MMC 4 bit bus mode.\n");
                }
            }
            else
            {
                i4Ret = SDC_SetMMCBusWidth(FALSE);
            
                if (i4Ret != S_OK)
                {
                    LOG(1, "SDCInit (SetMMCBusWidth) failed at %s(%d)\n", __FILE__, __LINE__);
                }
                
                LOG(1, "MMC 1 bit bus mode.\n");
            }
            
#ifdef CC_HIGH_SPEED_MODE_ENABLE
            if (_fgHighSpeedEnable)
            {
                // Set high speed mode (52 Mhz max)
                i4Ret = _Sdc_MMC_HighSpeed(TRUE);

                if (i4Ret != S_OK)
                {
                    LOG(1, "MMC Card switch high speed failed at %s(%d)\n", __FILE__, __LINE__);
                }
                else
                {
                    LOG(1, "MMC Card in High Speed Mode.\n");
                }
            }
#endif  // #ifdef CC_HIGH_SPEED_MODE_ENABLE
        }

        // initialize MMC product name string 6 bytes.
        prSDC->u1PNM[0] = MMC_CID_PNM_B1(prSDC->au4CID);

        prSDC->u1PNM[1] = MMC_CID_PNM_B2(prSDC->au4CID);
        prSDC->u1PNM[2] = MMC_CID_PNM_B3(prSDC->au4CID);
        prSDC->u1PNM[3] = MMC_CID_PNM_B4(prSDC->au4CID);
        prSDC->u1PNM[4] = MMC_CID_PNM_B5(prSDC->au4CID);
        prSDC->u1PNM[5] = MMC_CID_PNM_B6(prSDC->au4CID);
        prSDC->u1PNM[6] = 0;
    }

    LOG(7, "PNM = %s.\n", prSDC->u1PNM);

    i4Ret = _Sdc_SetBlkSz(512);

    if (i4Ret != S_OK)
    {
        LOG(1, "SDCInit failed at %s(%d)\n", __FILE__, __LINE__);
        /* What is in slot? */
        prSDC->u4State = SLOT_STATE_UNKNOWN;
        prSDC->u4CardType = FCI_UNKNOWN_CARD;
        /* then do nothing. */
        return 1;
    }

    prSDC->u2BlkSz[SDC_COMMOM] = 512;

    LOG(7, "SDCInit OK Card:%d (SD:%d MMC:%d) (%s)\n", prSDC->u4CardType, FCI_SD_CARD, FCI_MMC_CARD,
        prSDC->fg4BitEn ? "Parallel" : "Serial");

    if(_u4SDCardType == FCI_SDHC_CARD)
    {
        // Set clock around 50 mhz for SDHC card
        _SDC_ChangeClock(SDC_HIGH_CLOCK);

        // Set clock around 25 mhz for SD card  => MT5391 only support 45 Mhz
        // _SDC_ChangeClock(SDC_NORM_CLOCK);
    }
    else
    {
        // Set clock around 25 mhz for SD card
        _SDC_ChangeClock(SDC_NORM_CLOCK);
    }

    UNUSED(u4BlkLen);

    return 0;
}

//-------------------------------------------------------------------------
/** SDC_SetSDBusWidth
 *  Set SD bus to 4 bits wide.
 *  @param  fg4BitEnable : 4 bit mode
 *  @retval   S_OK : Success.
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
INT32 SDC_SetSDBusWidth(BOOL fg4BitEnable)
{
    SDC_T *prSDC;
    INT32 i4Ret;
    INT32 i4Width = 0x02;

    if (_fgFCI4BitBusEnable == FALSE && fg4BitEnable == TRUE)
    {       
        LOG(1, "Error !! 4 Bit Bus Mode Disabled.\n");
        LOG(1, "Enable 4 Bit Bus Mode First.\n");
    }
    
    prSDC = FCI_GetSDCSlot();

    i4Ret = _SDC_SendCmd55(prSDC->u2RCA);

    if (i4Ret != S_OK)
    {
        LOG(1, "%s(%d) %s SendCmd55 failed %d\n", __FILE__, __LINE__, __FUNCTION__, i4Ret);
        return i4Ret;
    }

    // Set 4-bit mode or 1-bit mode
    if(fg4BitEnable)
    {
        i4Width = 0x02; // 4-bit mode
    }
    else
    {
        i4Width = 0x00; // 1-bit mode
    }

    i4Ret = SDCHW_SendCmd(SD_ACMD_6, (UINT32)i4Width);

    if (i4Ret != S_OK)
    {
        LOG(1, "%s(%d) %s SendACmd6 failed to set Width enable. Ret:0x%08x\n", __FILE__, __LINE__,
            __FUNCTION__, i4Ret);
        return i4Ret;
    }
    else
    {
        LOG(6, "%s(%d) %s SendACmd6 success to set Width enable.\n", __FILE__, __LINE__,
            __FUNCTION__);
    }

    i4Ret = _SDC_CheckStatus(SD_CMD_IDX(SD_ACMD_6));

    if (i4Ret != S_OK)
    {
        LOG(1, "%s(%d) %s CheckStatus failed %d\n", __FILE__, __LINE__, __FUNCTION__, i4Ret);
        return i4Ret;
    }

    SDCHW_Set4BitOn(fg4BitEnable);

    prSDC->fg4BitEn = fg4BitEnable;

    return S_OK;
}

//-------------------------------------------------------------------------
/** SDC_SetMMCBusWidth
 *  Set MMC bus to 4 bits wide.
 *  @param  fg4BitEnable : 4 bit mode
 *  @retval   S_OK : Success.
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
INT32 SDC_SetMMCBusWidth(BOOL fg4BitEnable)
{
    SDC_T *prSDC;
    INT32 i4Ret;
    UINT32 u4Arg;

    if (_fgFCI4BitBusEnable == FALSE && fg4BitEnable == TRUE)
    {       
        LOG(1, "Error !! 4 Bit Bus Mode Disabled.\n");
        LOG(1, "Enable 4 Bit Bus Mode First.\n");
    }
    
    prSDC = FCI_GetSDCSlot();

    // determine Arguments according to bus selection
    if (fg4BitEnable)
    {
        u4Arg = MMC_CMD6_ARG_4BIT_BUS;
    }
    else
    {
        u4Arg = MMC_CMD6_ARG_1BIT_BUS;
    }

    // Send cmd 6 to change EXT_CSD - byte 183th BUS_WIDTH field.
    i4Ret = SDCHW_SendCmd(SD_CMD_6_MMC, u4Arg);

    if (i4Ret != S_OK)
    {
        LOG(1, "%s(%d) %s CMD_6_MMC failed to set Width enable. Ret:0x%08x\n", __FILE__, __LINE__,
            __FUNCTION__, i4Ret);
        return i4Ret;
    }
    else
    {
        LOG(6, "%s(%d) %s CMD_6_MMC success to set Width enable.\n", __FILE__, __LINE__,
            __FUNCTION__);
    }

    i4Ret = _SDC_CheckStatus(SD_CMD_IDX(SD_CMD_6_MMC));

    if (i4Ret != S_OK)
    {
        LOG(1, "%s(%d) %s CheckStatus failed %d\n", __FILE__, __LINE__, __FUNCTION__, i4Ret);
        return i4Ret;
    }

    SDCHW_Set4BitOn(fg4BitEnable);

    prSDC->fg4BitEn = fg4BitEnable;

    return S_OK;
}

//-------------------------------------------------------------------------
/** SDC_GetExtCSD
 *  Send Extended CSD (card specific data) on the Data line.
 *  @param  u2RCA  relative card address.
 *  @param  pu4CSD  point to CSD buffer.
 *  @retval   S_OK : Success.
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
INT32 SDC_GetExtCSD(UINT32 *pu4ExtCSD)
{
    INT32 i4Ret;

    ASSERT(pu4ExtCSD != NULL);

    // Read 512 bytes
    i4Ret = SDCHW_SendCmd(SD_CMD_8_MMC | 512, SDC_NO_ARG);

    if (i4Ret != S_OK)
    {
        LOG(7, "%s(%d) GetExtCSD failed 1 = 0x%x.\n", __FUNCTION__, __LINE__,  i4Ret);
        return i4Ret;
    }

    i4Ret = _SDC_CheckStatus(SD_CMD_IDX(SD_CMD_8_MMC));

    if (i4Ret != S_OK)
    {
        LOG(7, "%s(%d) GetExtCSD failed 2 = 0x%x.\n", __FUNCTION__, __LINE__,  i4Ret);
        return i4Ret;
    }

    // Read 128 words = 512 bytes
    i4Ret = SDCHW_ReadDataReg((UINT32 *)pu4ExtCSD, 128);

    if (i4Ret != S_OK)
    {
        LOG(7, "%s(%d) GetExtCSD failed 3 = 0x%x.\n", __FUNCTION__, __LINE__,  i4Ret);
        return i4Ret;
    }

    return i4Ret;
}

//-------------------------------------------------------------------------
/** SDC_ReadBlks
 *  SD read block function.
 *  @param  u4Addr   data address.
 *  @param  pu1Buf   point to user's data buffer.
 *  @param  u4BlkNs   read data block number.
 *  @retval   S_OK : Success.
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
INT32 SDC_ReadBlks(UINT32 u4Addr, UINT8 *pu1Buf, UINT32 u4BlkNs)
{
    UINT32 i;
    INT32 i4Ret = S_OK;

    SDC_T *prSDC;

    prSDC = FCI_GetSDCSlot();

    if (_u4SDCardType == FCI_SDHC_CARD)
    {
        /*
        Data address is in byte units in a Standard Capacity SD Memory Card
        and in block (512 Byte) units in a High Capacity SD Memory Card
        */
        u4Addr /= 0x200;
    }

    if (u4BlkNs == 1)
    {
        return _Sdc_ReadSingleBlk(u4Addr, pu1Buf);
    }
    else if (u4BlkNs)
    {
        if (prSDC->fgSingleBlockAccess)
        {
            for(i = 0; i < u4BlkNs; i++)
            {
                i4Ret = _Sdc_ReadSingleBlk(u4Addr, pu1Buf);

                if (_u4SDCardType == FCI_SDHC_CARD)
                {
                    u4Addr += prSDC->u2BlkSz[SDC_COMMOM] / 0x200;   // Sector base
                }
                else
                {
                    u4Addr += prSDC->u2BlkSz[SDC_COMMOM];               // Byte base
                }

                pu1Buf = (UINT8 *)((UINT32)pu1Buf + (UINT32)(prSDC->u2BlkSz[SDC_COMMOM]));

                if(i4Ret != S_OK)
                {
                    break;
                }
            }

            return i4Ret;
        }
        else
        {
            return _Sdc_ReadMultiBlk(u4Addr, pu1Buf, u4BlkNs);
        }
    }

    return E_INVALID_ARG;
}

//-------------------------------------------------------------------------
/** SDC_WriteBlks
 *  SD write block function.
 *  @param  u4Addr   data address.
 *  @param  pu1Buf   point to user's data buffer.
 *  @param  u4BlkNs   write data block number.
 *  @retval   S_OK : Success.
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
INT32 SDC_WriteBlks(UINT32 u4Addr, UINT8 *pu1Buf, UINT32 u4BlkNs)
{
    UINT32 i;
    INT32 i4Ret = S_OK;

    SDC_T *prSDC;

    prSDC = FCI_GetSDCSlot();

    if (_u4SDCardType == FCI_SDHC_CARD)
    {
        /*
        Data address is in byte units in a Standard Capacity SD Memory Card
        and in block (512 Byte) units in a High Capacity SD Memory Card
        */
        u4Addr /= 0x200;
    }

    if (u4BlkNs == 1)
    {
        return _Sdc_WriteSingleBlk(u4Addr, pu1Buf);
    }
    else if (u4BlkNs)
    {
        if (prSDC->fgSingleBlockAccess)
        {
            for(i = 0; i < u4BlkNs; i++)
            {
                i4Ret = _Sdc_WriteSingleBlk(u4Addr, pu1Buf);

                if (_u4SDCardType == FCI_SDHC_CARD)
                {
                    u4Addr += prSDC->u2BlkSz[SDC_COMMOM] / 0x200;   // Sector base
                }
                else
                {
                    u4Addr += prSDC->u2BlkSz[SDC_COMMOM];               // Byte base
                }

                pu1Buf = (UINT8 *)((UINT32)pu1Buf + (UINT32)(prSDC->u2BlkSz[SDC_COMMOM]));

                if(i4Ret != S_OK)
                {
                    break;
                }
            }

            return i4Ret;
        }
        else
        {
            return _Sdc_WriteMultiBlk(u4Addr, pu1Buf, u4BlkNs);
        }
    }

    return E_INVALID_ARG;
}

//-------------------------------------------------------------------------
/** SDC_GetStatus
 *  Get card status.
 *  @param  u2RCA   relative card address.
 *  @param  pu4Tmp   point to status buffer.
 *  @retval   S_OK : Success.
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
INT32 SDC_GetStatus(UINT16 u2RCA, UINT32 *pu4Tmp)
{
    INT32 i4Ret;

    ASSERT(pu4Tmp != NULL);
    i4Ret = SDCHW_SendCmd(SD_CMD_13, (UINT32)u2RCA << 16);

    if (i4Ret != S_OK)
    {
        return i4Ret;
    }

    *pu4Tmp = SDCHW_ReadResp(1);
    return S_OK;
}

//-------------------------------------------------------------------------
/** SDC_DisableCD
 *  Disable Card Detect for SD host
 *  @param  u2RCA   SD Card RCA.
 *  @retval   S_OK : Success.
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------

INT32 SDC_DisableCD(UINT16 u2RCA)
{
    INT32 i4Ret;

    i4Ret = _SDC_SendCmd55(u2RCA);

    if (i4Ret!=S_OK)
    {
        LOG(1, "%s(%d) GetSDSCR failed return %d\n", __FILE__, __LINE__, i4Ret);
        return i4Ret;
    }

    i4Ret = SDCHW_SendCmd(SD_ACMD_42, 0);

    if (i4Ret!=S_OK)
    {
        LOG(1, "%s(%d) Disable CD failed return %d\n", __FILE__, __LINE__, i4Ret);
        return i4Ret;
    }

    i4Ret = _SDC_CheckStatus(SD_CMD_IDX(SD_ACMD_42));

    if (i4Ret!=S_OK)
    {
        LOG(1, "%s(%d) GetSDSCR failed return %d\n", __FILE__, __LINE__, i4Ret);
        return i4Ret;
    }

    return S_OK;
}

#endif
