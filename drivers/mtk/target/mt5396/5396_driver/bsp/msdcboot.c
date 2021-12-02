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
 * $RCSfile: msdcboot.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file msdcboot.c
 *  msdcboot.c provides the low level register access function of msdc device.
 */

#if defined(CC_EMMC_BOOT) && defined(CC_MTK_PRELOADER)

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_timer.h"
#include "chip_txt.h"
#include "msdcboot.h"

EXTERN void CHIP_Delay_us(UINT32 u4Micros);

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------
volatile static UINT32 _u4MsdcAccuVect = 0;
static UINT32 _u4CurMsdcclkFreq = 0;
static UINT32 _u4StartOffsetAlign = 0;
static UINT32 _u4EndOffsetAlign = 0;
static UINT32 _u4ActuralLen = 0;

static sdcard_t sdCard;
static UINT32 _u4MsdcFreq[] = {BUS_CLK_FREQ, BUS_DIV_2_CLK_FREQ, BUS_DIV_4_CLK_FREQ, BUS_DIV_8_CLK_FREQ, BUS_DIV_16_CLK_FREQ, INIT_BUS_FREQ};
static UINT32 _u4MsdcFreqIdx = 0;
static UINT32 ch = 0;
static BOOL _fgMsdcIdentyCardOK = FALSE;
BOOL _fgMsdcInit;

#define ROMMSDCTAG_0 0x2F4B544D
#define ROMMSDCTAG_1 0x2F565444
#define ROMMSDCTAG_2 0x434D4F52
#define ROMMSDCTAG_3 0x2F45444F
#define ROMMSDCTAG_4 0x4344534D
#define ROMMSDCTAG_5 0x544F4F42
#define ROMMSDCTAG_6 0x3130562F
#define ROMMSDCTAG_7 0x0030302E

const static UINT32 _u4Signature[] __attribute__ ((section ("ROMNANDTAG"))) =
{
    ROMMSDCTAG_0, ROMMSDCTAG_1, ROMMSDCTAG_2, ROMMSDCTAG_3,     // MTK/DTV/ROMCODE/
    ROMMSDCTAG_4, ROMMSDCTAG_5, ROMMSDCTAG_6, ROMMSDCTAG_7,     // MSDCBOOT/V01.00
    0,            0,            0,            0,
    0,            0,            0,            0,
    
    0,            0,            0,            0,
    0,            0,            0,            0,
    0,            0,            0,            0,
    0,            0,            0,            0
};

//-----------------------------------------------------------------------------
// static functions
//-----------------------------------------------------------------------------
void CHIP_DisplayString(const char *szString);
void CHIP_DisplayHex(UINT32);

//-----------------------------------------------------------------------------
/** CHIP_EmmcShowHex()
 */
//-----------------------------------------------------------------------------
static void CHIP_EmmcShowHex(const CHAR *pu1Str, UINT32 u4Hex)
{
    CHIP_DisplayString(pu1Str);
    CHIP_DisplayHex(u4Hex);
    CHIP_DisplayString("\n");
}

static void MsdcShowChipInfo(UINT32 *pCID)
{
    CHIP_EmmcShowHex("\nCID:", pCID[0]);
    CHIP_EmmcShowHex(":", pCID[1]);
    CHIP_EmmcShowHex(":", pCID[2]);
    CHIP_EmmcShowHex(":", pCID[3]);
}

static void MsdcSetMemory(void* pdestination, UINT32 u4Ch, UINT32 u4length)
{
    UINT32 i;
    UINT32 *pbDstBufPtr;

    pbDstBufPtr = (UINT32 *)pdestination;

    for (i = 0; i < u4length; i += 4)
    {
        *pbDstBufPtr = u4Ch;
        pbDstBufPtr++;
    }
}

static void MsdcCopyMemory(void* pdestination, void* psource, UINT32 u4length)
{
    UINT32 i;
    UINT32 *pbDstBufPtr, *pbSrcBufPtr;

    pbDstBufPtr = (UINT32 *)pdestination;
    pbSrcBufPtr = (UINT32 *)psource;

    for (i = 0; i < u4length; i += 4)
    {
        *pbDstBufPtr = *pbSrcBufPtr;
        pbDstBufPtr++;
        pbSrcBufPtr++;
    }
}

#define x_memset(dest, ch, len)   MsdcSetMemory((void*)(dest), (ch), (len))
#define x_memcpy(dest, src, len)  MsdcCopyMemory((void*)(dest), (void*)(src), (len))

static INT32 MsdcReset(void)
{
	UINT32 i;
    
    // Reset MSDC
    MSDC_SETBIT(MSDC_CFG, MSDC_CFG_RST);

    for (i = 0; i < MSDC_RST_TIMEOUT_LIMIT_COUNT; i++)
    {
        if (0 == (MSDC_READ32(MSDC_CFG) & MSDC_CFG_RST))
        {
            break;
        }
        
        CHIP_Delay_us(1000);
    }
    
    if(i == MSDC_RST_TIMEOUT_LIMIT_COUNT)
    {
        return MSDC_FAILED;        
    }
	
    return MSDC_SUCCESS;
}

static INT32 MsdcClrFifo(void)
{
    UINT32 i;
    
    // Reset FIFO
    MSDC_SETBIT(MSDC_FIFOCS, MSDC_FIFOCS_FIFOCLR);

    for (i = 0; i < MSDC_RST_TIMEOUT_LIMIT_COUNT; i++)
    {
        if (0 == (MSDC_READ32(MSDC_FIFOCS) & (MSDC_FIFOCS_FIFOCLR | MSDC_FIFOCS_TXFIFOCNT_MASK | MSDC_FIFOCS_RXFIFOCNT_MASK))) 
        {
            break;
        }
        
        CHIP_Delay_us(1000);
    }
    
    if (i == MSDC_FIFOCLR_TIMEOUT_LIMIT_COUNT)
    {
        return MSDC_FAILED;       
    }

    return MSDC_SUCCESS;
}

static void MsdcChkFifo(sd_cmd_t *pCmd)
{
    // Check if rx/tx fifo is zero
    if ((MSDC_READ32(MSDC_FIFOCS) & (MSDC_FIFOCS_TXFIFOCNT_MASK | MSDC_FIFOCS_RXFIFOCNT_MASK)) != 0)
    {
        MsdcClrFifo();
    }
}

static void MsdcClrIntr(void)
{
    // Check MSDC Interrupt vector register
    if (0 != MSDC_READ32(MSDC_INT))
    {
        // Clear MSDC Interrupt vector register
        MSDC_WRITE32(MSDC_INT, MSDC_READ32(MSDC_INT));
    } 
}

static INT32 MsdcWaitClkStable (void)
{
	UINT32 i;

    for (i = 0; i < MSDC_CLK_TIMEOUT_LIMIT_COUNT; i++)
    {
        if (0 != (MSDC_READ32(MSDC_CFG) & MSDC_CFG_CARD_CK_STABLE))
        {
            break;
        }
        
        CHIP_Delay_us(1000);
    }
    
    if (i == MSDC_CLK_TIMEOUT_LIMIT_COUNT)
    {
        return MSDC_FAILED;      
    }
	
    return MSDC_SUCCESS;
}

static INT32 MsdcInit (void) 
{
    // Reset MSDC
    MsdcReset();

    // Set PIO Mode & SD/MMC Mode 
    MSDC_SETBIT(MSDC_CFG, MSDC_CFG_PIO_MODE | MSDC_CFG_SD);

    // Disable sdio & Set bus to 1 bit mode
    MSDC_CLRBIT(SDC_CFG, SDC_CFG_SDIO | SDC_CFG_BW_MASK);

    // set clock mode (DIV mode)
    MSDC_CLRBIT(MSDC_CFG, (((UINT32)0x03) << 16));

    // Wait until clock is stable
    if (MSDC_FAILED == MsdcWaitClkStable())
    {
        return MSDC_FAILED;
    }
    
    // Set default RISC_SIZE for DWRD pio mode
    MSDC_WRITE32(MSDC_IOCON, (MSDC_READ32(MSDC_IOCON) & ~MSDC_IOCON_RISC_SIZE_MASK) | MSDC_IOCON_RISC_SIZE_DWRD);

    // Set Data timeout setting => Maximum setting
    MSDC_WRITE32(SDC_CFG, (MSDC_READ32(SDC_CFG) & ~(((UINT32)0xFF) << SDC_CFG_DTOC_SHIFT)) | (((UINT32)0xFF) << SDC_CFG_DTOC_SHIFT));
    
    return MSDC_SUCCESS;
}

static void MsdcContinueClock (INT32 i4ContinueClock)
{
    if (i4ContinueClock)
    {
       // Set clock continuous even if no command
       MSDC_SETBIT(MSDC_CFG, (((UINT32)0x01) << 1));
    }
    else
    {
       // Set clock power down if no command
       MSDC_CLRBIT(MSDC_CFG, (((UINT32)0x01) << 1));
    }
}

static INT32 MsdcSetClkfreq(UINT32 clkFreq) 
{
    UINT32 divider;
    UINT32 expFreq = 0;

    if (_u4CurMsdcclkFreq == clkFreq)
    {
        return MSDC_SUCCESS;
    }
   
    if (clkFreq > BUS_CLK_FREQ)
    {
        clkFreq = BUS_CLK_FREQ;
    }
    else if (clkFreq < LOWEST_BUS_FREQ)
    {
        clkFreq = INIT_BUS_FREQ;
    }

    for (divider = 0; divider <= 0xFF; divider++)
    {
        if (0 == divider) 
        {
            expFreq = SRC_CLK_FREQ/2;
        }
        else 
        {
            expFreq = SRC_CLK_FREQ/4/divider;
        }

        if (expFreq <= clkFreq) 
        {
            MSDC_WRITE32(MSDC_CFG, (MSDC_READ32(MSDC_CFG) & (~0x0000FF00)) | (divider << MSDC_CFG_CK_DIV_SHIFT));
            
            // Wait until clock is stable
            if (MSDC_FAILED == MsdcWaitClkStable())
            {
                return MSDC_FAILED;
            }
          
            _u4CurMsdcclkFreq = clkFreq;
            
            return MSDC_SUCCESS;
        }
    }

    return MSDC_FAILED;
}

// Return when any interrupt is matched or timeout
static INT32 MsdcWaitIntr(UINT32 vector, UINT32 timeoutCnt) 
{
    UINT32 i;
    
    // Clear Vector variable
    _u4MsdcAccuVect = 0;  

    for (i = 0; i < timeoutCnt; i++)
    {
        // Status match any bit
        if (0 != (MSDC_READ32(MSDC_INT) & vector)) 
        {
            _u4MsdcAccuVect |= MSDC_READ32(MSDC_INT);
            MSDC_WRITE32(MSDC_INT, _u4MsdcAccuVect);
            return MSDC_SUCCESS;
        }
        
        CHIP_Delay_us(1000);
    }	

    // Timeout case
    return MSDC_FAILED;
}

static void MsdcErrorHandling(void)
{
    // Reset MSDC
    MsdcReset();

    // Clear FIFO
    MsdcClrFifo();

    // Check if FIFO clean, check if rx/tx fifo is zero
    if ((MSDC_READ32(MSDC_FIFOCS) & (MSDC_FIFOCS_TXFIFOCNT_MASK | MSDC_FIFOCS_RXFIFOCNT_MASK)) != 0)
    {
        MsdcClrFifo();
    }
    
    // Clear MSDC interrupt
    MsdcClrIntr();        
}

static INT32 MsdcSendCmd (sd_cmd_t *pCmd) 
{
    UINT32 respType = RESPONSE_NO;
    UINT32 sdcCmd = 0;
    INT32 i4Ret = CMD_ERR_SUCCESS;
    UINT32 u4CmdDoneVect;
    UINT32 i;

    // Check if rx/tx fifo is zero
    MsdcChkFifo(pCmd);

    // Clear interrupt Vector
    MsdcClrIntr();

    for (i = 0; i < MSDC_WAIT_SDC_BUS_TIMEOUT_LIMIT_COUNT; i++)
    {
        if ((0 == (MSDC_READ32(SDC_STS) & (SDC_STS_SDCBUSY | SDC_STS_CMDBUSY))) && (0  == MSDC_READ32(MSDC_INT)))
        {
		    break;
        }
        
        CHIP_Delay_us(1000);
    }
    
    if (i == MSDC_WAIT_SDC_BUS_TIMEOUT_LIMIT_COUNT)
    {
        i4Ret = CMD_ERR_FAILED; 
        goto ErrorHandling;        
    }

    /* Figure out the response type */
    switch(pCmd->idx) 
    {
    case CMD0_GO_IDLE_STATE:
        respType = RESPONSE_NO;
        sdcCmd |= SDC_CMD_RSPTYPE_NO;
        break;
        
    case CMD1_MMC_SEND_OP_COND:
        respType = RESPONSE_R3;
        sdcCmd |= SDC_CMD_RSPTYPE_R3;
        break;
        
    case CMD2_ALL_SEND_CID:
        respType = RESPONSE_R2;
        sdcCmd |= SDC_CMD_RSPTYPE_R2;
        break;
        
    case CMD3_SEND_RELATIVE_ADDR:
        respType = RESPONSE_R1;
        sdcCmd |= SDC_CMD_RSPTYPE_R1;
        break;
        
    case CMD6_MMC_SWITCH:
        respType = RESPONSE_R1B;
        sdcCmd |= SDC_CMD_RSPTYPE_R1B;
        break;
        
    case CMD7_SELECT_CARD:
        respType = RESPONSE_R1;
        sdcCmd |= SDC_CMD_RSPTYPE_R1;
        break;
        
    case CMD12_STOP_TRANSMISSION:
        respType = RESPONSE_R1B;	  
        sdcCmd |= (SDC_CMD_STOP | SDC_CMD_RSPTYPE_R1B);
        break;
        
    case CMD17_READ_SINGLE_BLOCK:
        respType = RESPONSE_R1;
        sdcCmd |= (SDC_CMD_RSPTYPE_R1 | DTYPE_SINGLE_BLK | SDC_CMD_READ);
        MSDC_WRITE32(SDC_BLK_NUM, 1);
        break;
        
    case CMD18_READ_MULTIPLE_BLOCK:
        respType = RESPONSE_R1;
        sdcCmd |= (SDC_CMD_RSPTYPE_R1 | DTYPE_MULTI_BLK | SDC_CMD_READ);
        MSDC_WRITE32(SDC_BLK_NUM, pCmd->buffLen / SDHC_BLK_SIZE);
        break;
        
	case CMD24_WRITE_BLOCK:
		respType = RESPONSE_R1;
		sdcCmd |= (SDC_CMD_RSPTYPE_R1 | DTYPE_SINGLE_BLK | SDC_CMD_WRITE);
		MSDC_WRITE32(SDC_BLK_NUM, 1);
		break;
        
	case CMD25_WRITE_MULTIPLE_BLOCK:
        respType = RESPONSE_R1;
        sdcCmd |= (SDC_CMD_RSPTYPE_R1 | DTYPE_MULTI_BLK | SDC_CMD_WRITE);
        MSDC_WRITE32(SDC_BLK_NUM, pCmd->buffLen / SDHC_BLK_SIZE);
        break;
    }   

    // Set Blk Length
    sdcCmd |= ((SDHC_BLK_SIZE) << SDC_CMD_LEN_SHIFT);

    // Set SDC_CMD.CMD
    sdcCmd |= (pCmd->idx & 0x3F);

    // Set SDC Argument
    MSDC_WRITE32(SDC_ARG, pCmd->arg);

    /* Send the commands to the device */
    MSDC_WRITE32(SDC_CMD, sdcCmd);

    // Wait for command and response if existed
    u4CmdDoneVect = INT_SD_CMDRDY | INT_SD_CMDTO | INT_SD_RESP_CRCERR;

    if (MSDC_SUCCESS != MsdcWaitIntr(u4CmdDoneVect, MSDC_WAIT_CMD_TIMEOUT_LIMIT_COUNT)) 
    {
        i4Ret = CMD_ERR_WAIT_CMD_TO;
        goto ErrorHandling;
    }

    if (_u4MsdcAccuVect & INT_SD_CMDTO)
    {
        i4Ret = CMD_ERR_NO_RESP;
        goto ErrorHandling;
    }
    else if (_u4MsdcAccuVect & INT_SD_RESP_CRCERR)
    {
        i4Ret = CMD_ERR_RESP_CRCERR;
        goto ErrorHandling;
    }
    else if ((_u4MsdcAccuVect & (~(INT_SD_CMDRDY))) || (0 != MSDC_READ32(MSDC_INT)))
    {
        i4Ret = CMD_ERR_FAILED;
        goto ErrorHandling;
    }

    // Handle the response
    switch (respType) 
    {
    case RESPONSE_NO:
        return MSDC_SUCCESS;
        
    case RESPONSE_R1:
    case RESPONSE_R1B:
        pCmd->resp[0] = MSDC_READ32(SDC_RESP0);
        x_memcpy(&(sdCard.cardStatus), &pCmd->resp[0], sizeof(INT32));
        break;
        
    case RESPONSE_R2:
        pCmd->resp[0] = MSDC_READ32(SDC_RESP0);
        pCmd->resp[1] = MSDC_READ32(SDC_RESP1);
        pCmd->resp[2] = MSDC_READ32(SDC_RESP2);
        pCmd->resp[3] = MSDC_READ32(SDC_RESP3);
        x_memcpy(&(sdCard.cid), pCmd->resp, sizeof(sd_cid_t));
        MsdcShowChipInfo(pCmd->resp);
        break;
        
    case RESPONSE_R3:
        pCmd->resp[0] = MSDC_READ32(SDC_RESP0);
        x_memcpy(&(sdCard.memOcr), &pCmd->resp[0], sizeof(mem_ocr_t));
        break;
    }

    if (PIO_DATA_MODE == pCmd->dataMode) 
    {
        UINT32 u4RxFifoCnt, u4TxFifoCnt;
        UINT32 *pBufData = (UINT32 *)(pCmd->pBuff);
        UINT32 u4BufEnd = (UINT32)pBufData + (UINT32)(pCmd->buffLen);
        UINT32 u4ActualEnd = (UINT32)pBufData + _u4ActuralLen; 
        UINT32 u4RxCnt = 0;

        u4CmdDoneVect = INT_SD_DATTO | INT_SD_DATA_CRCERR;

        // Read
        if (!(sdcCmd & SDC_CMD_WRITE))
        {
            if(_u4StartOffsetAlign)
            {
                do
                {
                    // wait until fifo has enough data
                    u4RxFifoCnt = (MSDC_READ32(MSDC_FIFOCS) & MSDC_FIFOCS_RXFIFOCNT_MASK);

                    while ((sizeof(INT32) <= u4RxFifoCnt) && (_u4StartOffsetAlign > 0))
                    {
                        // Read Data
                        MSDC_READ32(MSDC_RXDATA);                
                        u4RxFifoCnt -= sizeof(INT32);
                        _u4StartOffsetAlign -= sizeof(INT32);
                    }
                    
                } while (_u4StartOffsetAlign);
            }

            while (u4ActualEnd > (UINT32)pBufData) 
            {
                // wait until fifo has enough data
                u4RxFifoCnt = (MSDC_READ32(MSDC_FIFOCS) & MSDC_FIFOCS_RXFIFOCNT_MASK);

                while ((u4ActualEnd > (UINT32)pBufData) && (sizeof(INT32) <= u4RxFifoCnt))
                {
                    // Read Data
                    *pBufData = MSDC_READ32(MSDC_RXDATA);
                     pBufData++;

                     u4RxFifoCnt -= sizeof(INT32);

                     u4RxCnt++;
                     
                     if (u4RxCnt == 128)   // Check CRC error happens or not every 128 Byte
                     {
                         // Check if done vector occurs
                         if (u4CmdDoneVect & MSDC_READ32(MSDC_INT))
                         {
                             break;
                         }
						 u4RxCnt = 0;
                     }

                }          
           }
            
           if (_u4EndOffsetAlign)
           {
                do
                {
                    // wait until fifo has enough data
                    u4RxFifoCnt = (MSDC_READ32(MSDC_FIFOCS) & MSDC_FIFOCS_RXFIFOCNT_MASK);

                    while (sizeof(INT32) <= u4RxFifoCnt && (_u4EndOffsetAlign > 0))
                    {
                        // Read Data
                        MSDC_READ32(MSDC_RXDATA);                
                        u4RxFifoCnt -= sizeof(INT32);
                        _u4EndOffsetAlign -= sizeof(INT32);
                    }
                    
                } while (_u4EndOffsetAlign);
            }

        }
		else
		{
            while (u4BufEnd > (UINT32)pBufData) 
            {
                // Check if error done vector occurs
                if (u4CmdDoneVect & (_u4MsdcAccuVect | MSDC_READ32(MSDC_INT)))
                {
                    break;            
                }

				// wait until fifo has enough space
                u4TxFifoCnt = MSDC_FIFO_LEN - (MSDC_READ32(MSDC_FIFOCS) & MSDC_FIFOCS_TXFIFOCNT_MASK);

			    if (sizeof(INT32) <= u4TxFifoCnt)
			    {
				    while ((u4BufEnd > (UINT32)pBufData) && (sizeof(INT32) <= u4TxFifoCnt))
				    {
					    // Write Data
					    MSDC_WRITE32(MSDC_TXDATA, *pBufData);
					    pBufData++;
					    u4TxFifoCnt -= sizeof(INT32);
					
					    if (u4TxFifoCnt == 0)
					    {
						    u4TxFifoCnt = MSDC_FIFO_LEN - (MSDC_READ32(MSDC_FIFOCS) & MSDC_FIFOCS_TXFIFOCNT_MASK);
					    }
				    }
			    }
                else if(sizeof(INT32) > u4TxFifoCnt)
				{
				    for (i = 0; i < MSDC_WAIT_DATA_TIMEOUT_LIMIT_COUNT; i++)
					{
					    u4TxFifoCnt = MSDC_FIFO_LEN - (MSDC_READ32(MSDC_FIFOCS) & MSDC_FIFOCS_TXFIFOCNT_MASK);
                        if(sizeof(INT32) <= u4TxFifoCnt)
						{
						    break;						        
						}                    
					}
					if (i == MSDC_WAIT_DATA_TIMEOUT_LIMIT_COUNT)
					{						            
					    i4Ret = CMD_ERR_WAIT_DATATO; 			
						goto ErrorHandling;					
					}             
				}

                // Check if done vector occurs
                else if (u4CmdDoneVect & MSDC_READ32(MSDC_INT))
                {
                    break;
                }


            }

		}

        // Wait for data complete		
        if (MSDC_SUCCESS != MsdcWaitIntr((u4CmdDoneVect | INT_SD_XFER_COMPLETE), 10*MSDC_WAIT_DATA_COMPLETE_TIMEOUT_LIMIT_COUNT)) 
        {
            i4Ret = CMD_ERR_WAIT_DATATO;
            goto ErrorHandling;
        }

        if (_u4MsdcAccuVect & INT_SD_DATTO)
        {
            i4Ret = CMD_ERR_DATTO;
            goto ErrorHandling;
        }
        else if (_u4MsdcAccuVect & INT_SD_DATA_CRCERR)
        {
            i4Ret = CMD_ERR_DATA_CRCERR;
            goto ErrorHandling;
        }
        else if ((_u4MsdcAccuVect & ~(INT_SD_XFER_COMPLETE)) || (0 != MSDC_READ32(MSDC_INT)))
        {
            i4Ret = CMD_ERR_DATA_FAILED;
            goto ErrorHandling;
        }
    }
  
    return i4Ret;
    
ErrorHandling:
    // No error handling is needed if no data transfer, so we only perform error handling for read data      
    // if (PIO_DATA_MODE == pCmd->dataMode) 
    {
        MsdcErrorHandling();
    }
    
    return i4Ret;
}

static INT32 cmd0_go_idle_state(UINT32 arg)
{
	sd_cmd_t cmd;

	x_memset(&cmd, 0, sizeof(sd_cmd_t));
	cmd.idx = CMD0_GO_IDLE_STATE;
	cmd.arg = arg;
	// cmd.dataMode = NULL_DATA_MODE;   // dataMode = 0 by x_memset

	return MsdcSendCmd(&cmd);
}

static INT32 cmd1_mmc_send_op_cond(void)
{
	sd_cmd_t cmd;

	x_memset(&cmd, 0, sizeof(sd_cmd_t));
	cmd.idx = CMD1_MMC_SEND_OP_COND;
	//cmd.dataMode = NULL_DATA_MODE;   // dataMode = 0 by x_memset
	cmd.arg = HOST_MMC_OCR;

	return MsdcSendCmd(&cmd);
}

static INT32 cmd2_all_send_cid(void)
{
	sd_cmd_t cmd;

	x_memset(&cmd, 0, sizeof(sd_cmd_t));
	cmd.idx = CMD2_ALL_SEND_CID;
	// cmd.dataMode = NULL_DATA_MODE;   // dataMode = 0 by x_memset

	return MsdcSendCmd(&cmd);
}

static INT32 cmd3_send_relative_addr(void) 
{
	sd_cmd_t cmd;

	x_memset(&cmd, 0, sizeof(sd_cmd_t));
	cmd.idx = CMD3_SEND_RELATIVE_ADDR;
	// cmd.dataMode = NULL_DATA_MODE;   // dataMode = 0 by x_memset

    if (sdCard.cardType == CARDTYPE_MEM_MMC)
    {
        sdCard.memRca = MMC_DEF_RCA;
        cmd.arg = (sdCard.memRca) << 16;
    }
            
	return MsdcSendCmd(&cmd);
}

static INT32 cmd6_mmc_set_bus_width(INT32 busWidth) 
{
	sd_cmd_t cmd;

	x_memset(&cmd, 0, sizeof(sd_cmd_t));
	cmd.idx = CMD6_MMC_SWITCH;
	// cmd.dataMode = NULL_DATA_MODE;   // dataMode = 0 by x_memset
	
   // determine Arguments according to bus selection
	if (1 == busWidth) 
	{
		cmd.arg = MMC_CMD6_ARG_1BIT_BUS;
	}
	else if (4 == busWidth) 
	{
		cmd.arg = MMC_CMD6_ARG_4BIT_BUS;
	}
	         
	return MsdcSendCmd(&cmd);
}

static INT32 cmd7_select_card(UINT32 sel) 
{
	sd_cmd_t cmd;

	x_memset(&cmd, 0, sizeof(sd_cmd_t));
	cmd.idx = CMD7_SELECT_CARD;
	// cmd.dataMode = NULL_DATA_MODE;   // dataMode = 0 by x_memset
	
	if (sel)
	{
		cmd.arg = (sdCard.memRca) << 16;
	}
	else 
	{
		cmd.arg = 0;
	}

	return MsdcSendCmd(&cmd);
}

static INT32 cmd17_read_single_block(UINT32 addr, INT8 *pBuff, UINT32 buffLen)
{
	sd_cmd_t cmd;
	UINT32 u4Tmp;

	x_memset(&cmd, 0, sizeof(sd_cmd_t));
	cmd.idx = CMD17_READ_SINGLE_BLOCK;
	cmd.dataMode = PIO_DATA_MODE;
	cmd.pBuff = pBuff;
	cmd.buffLen = buffLen;
	_u4ActuralLen = buffLen;
	
	if (sdCard.blkAddrMode == 1)
	{
        cmd.arg = addr >> 9;    // Block addressing mode
        _u4StartOffsetAlign = addr - (addr & (~0x1FF));
		//CHIP_EmmcShowHex("\nBLOCK ACCESS:", 1);
	}
	else
	{
        cmd.arg = addr;           // Byte addressing mode
        _u4StartOffsetAlign = addr - cmd.arg;
		//CHIP_EmmcShowHex("\nBLOCK ACCESS:", 0);
	}

	u4Tmp = addr + buffLen - 1;
	u4Tmp &= (~0x1FF);
	u4Tmp += 0x200;
	cmd.buffLen = u4Tmp - (addr & (~0x1FF)); 
	
	_u4EndOffsetAlign = u4Tmp - (addr + buffLen);

	//CHIP_EmmcShowHex("\n_u4ActuralLen :", _u4ActuralLen);
	//CHIP_EmmcShowHex("\n_u4StartOffsetAlign :", _u4StartOffsetAlign);
	//CHIP_EmmcShowHex("\n_u4EndOffsetAlign :", _u4EndOffsetAlign);
	//CHIP_EmmcShowHex("\ncmd.arg :", cmd.arg);
	//CHIP_EmmcShowHex("\ncmd.buffLen :", cmd.buffLen);

	
	return MsdcSendCmd(&cmd);
}

static INT32 cmd18_read_multiple_block(UINT32 addr, INT8 *pBuff, UINT32 buffLen) 
{
	sd_cmd_t cmd;
	UINT32 u4Tmp;

	x_memset(&cmd, 0, sizeof(sd_cmd_t));
	cmd.idx = CMD18_READ_MULTIPLE_BLOCK;
	cmd.dataMode = PIO_DATA_MODE;
	cmd.pBuff = pBuff;
	_u4ActuralLen = buffLen;
	
	if (sdCard.blkAddrMode == 1)
	{
        cmd.arg = addr >> 9;    // Block addressing mode
        _u4StartOffsetAlign = addr - (addr & (~0x1FF));
	}
	else
	{
        cmd.arg = addr & (~0x1FF);           // Byte addressing mode
        _u4StartOffsetAlign = addr - cmd.arg;
	}
	
	u4Tmp = addr + buffLen - 1;
	u4Tmp &= (~0x1FF);
	u4Tmp += 0x200;
	cmd.buffLen = u4Tmp - (addr & (~0x1FF)); 
	
	_u4EndOffsetAlign = u4Tmp - (addr + buffLen);
	
	return MsdcSendCmd(&cmd);
}

static INT32 cmd12_stop_transmission(void) 
{
	sd_cmd_t cmd;

	x_memset(&cmd, 0, sizeof(sd_cmd_t));
	cmd.idx = CMD12_STOP_TRANSMISSION;
	// cmd.dataMode = NULL_DATA_MODE;   // dataMode = 0 by x_memset

	return MsdcSendCmd(&cmd);
}

// Final stage of card identification flow.
static INT32 MsdcCardTypeChk(void)
{
    // CMD2
    if (CMD_ERR_SUCCESS != cmd2_all_send_cid())
    {
        return MSDC_FAILED;
    }

    // CMD3
    if (CMD_ERR_SUCCESS != cmd3_send_relative_addr())
    {
        return MSDC_FAILED;
    }
    
    return MSDC_SUCCESS;
}

static INT32 MsdcCardRescan()
{
	INT32 ret, i;

    for (i = 0; i < CMD0_RESET_LIMIT; i++)
    {
    	// CMD0 Pin1 = High
        if (MSDC_SUCCESS != cmd0_go_idle_state(0))
    	{
            return MSDC_FAILED;
    	}
    }
    	
    if (MSDC_SUCCESS != cmd1_mmc_send_op_cond())
    {
        return MSDC_FAILED;
    }

    sdCard.cardType = CARDTYPE_MEM_MMC;

    if (sdCard.memOcr.ocr & HOST_OCR)
    {
        UINT32 retry = 0;
        
        // Set new Voltage		
        while (1) 
        {
            ret = cmd1_mmc_send_op_cond();

            if (CMD_ERR_SUCCESS == ret) 
            {
                if (sdCard.memOcr.memRdy) 
                {
                    sdCard.memInitDone = 1;
                    
                    if (sdCard.memOcr.hcs)    // MMC : Sector addressing mode
                    {
                        sdCard.blkAddrMode = 1;
                    }
                    else                               // MMC :  Byte addressing mode
                    {
                        sdCard.blkAddrMode = 0;
                    }

                    if (MSDC_SUCCESS != MsdcCardTypeChk())
                    {
                        return MSDC_FAILED;
                    }
                    
                    return MSDC_SUCCESS;
                }
                else 
                {
                    retry++;
                    if (retry < MMC_CMD1_RETRY_LIMIT) 
                    {
                        CHIP_Delay_us(2000);
                        continue;
                    }
                    else 
                    {
                        return MSDC_FAILED;
                    }
                }
            }
            else 
            {
                return MSDC_FAILED;
            }
        }
    }

	return MSDC_SUCCESS;
}

static INT32 MsdcSetBusWidth (INT32 busWidth) 
{
    if (CMD_ERR_SUCCESS != cmd6_mmc_set_bus_width(busWidth))
    {
        return MSDC_FAILED;
    }

    /* Modify MSDC Register Settings */
    if (1 == busWidth)
    {
        MSDC_WRITE32(SDC_CFG, (MSDC_READ32(SDC_CFG) & ~SDC_CFG_BW_MASK) | (0 <<  SDC_CFG_BW_SHIFT));
    }
    else if (4 == busWidth)
    {
        MSDC_WRITE32(SDC_CFG, (MSDC_READ32(SDC_CFG) & ~SDC_CFG_BW_MASK) | (0x01 <<  SDC_CFG_BW_SHIFT));
    }

    return MSDC_SUCCESS;
}

static INT32 MsdcMemRead (UINT32 addr, void *pBuff, UINT32 buffLen) 
{
    INT32 i4Ret, i4RetStop;
	UINT32 u4TmpAddr;

	u4TmpAddr = addr & (~(SDHC_BLK_SIZE - 1));

	//CHIP_EmmcShowHex("\nu4TmpAddr :", u4TmpAddr);
	//CHIP_EmmcShowHex("\naddr :", addr);
	//CHIP_EmmcShowHex("\nbuffLen :", buffLen);
	//CHIP_EmmcShowHex("\npBuff :", (UINT32)pBuff);
               
    if (((addr + buffLen) - u4TmpAddr) <= SDHC_BLK_SIZE)
    {
        // CMD17 Single block read
        i4Ret = cmd17_read_single_block(addr, pBuff, buffLen);
        if (CMD_ERR_SUCCESS != i4Ret)
        {
            return i4Ret;
        }
    }
    else
    {
        // CMD18 Multiple block read
        i4Ret = cmd18_read_multiple_block(addr, pBuff, buffLen);
        if (CMD_ERR_SUCCESS != i4Ret)
        {
            i4RetStop = cmd12_stop_transmission();
            // Error Handling process : sw should manually add stop command when multi-block read Failed
            if (CMD_ERR_SUCCESS != i4RetStop)
            {
                return i4RetStop;
            }

            return i4Ret;
        }

        i4RetStop = cmd12_stop_transmission();
        if (CMD_ERR_SUCCESS != i4RetStop)
        {
            return i4RetStop;
        }
    }

    return i4Ret;
}

static INT32 MsdcEmmcBootStop (BOOL fgResetMsdc)
{    
    UINT32 i;
    
	// Issue Reset Command
	MSDC_WRITE32(SDC_ARG, 0x00000000);

	// Set SDC Argument        
	MSDC_WRITE32(SDC_CMD, 0x00001000);

	/* Send the commands to the device */
	// Boot Wait Delay
	MSDC_SETBIT(EMMC_CFG0, BTWDLY_2x32);

	// Boot Stop    
	MSDC_SETBIT(EMMC_CFG0, BTSTOP);
    
	for (i = 0; i < MSDC_WAIT_BTSTS_0_TIMEOUT_LIMIT_COUNT; i++)
	{
	    if (!(MSDC_READ32(EMMC_STS) & (BTSTS)))  
		{
		    break;
		}

		CHIP_Delay_us(1000);
	}
    
	if (i == MSDC_WAIT_BTSTS_0_TIMEOUT_LIMIT_COUNT)
	{
	    // Fetal Error
	    return MSDC_FAILED;
	}

	// Program Boot Support = 0 
	MSDC_CLRBIT(EMMC_CFG0, (BTSUP));
    
	// Clear FIFO
	MsdcClrFifo();
    
	// Clear MSC interrupt
	MsdcClrIntr();

    // Msdc Error Handling include Msdc Reset 
    if (fgResetMsdc)
	{
	    // Emmc Status W1C, so write 1 to clear old status
	    MSDC_WRITE32(EMMC_STS, MSDC_READ32(EMMC_STS));
		MsdcErrorHandling();
	}

	return MSDC_SUCCESS;
}

//---------------------------------------------------------------------------
// MSDC PinMux Switch  function
//---------------------------------------------------------------------------
static void MSDC_PinMux()
{
    UINT32 u4Value;

    ch = 1;
     
    if(ch == 0)           //MSDC Controller 0, Base Address: 0xF0012000, pinmux register d408[1:0], function 1
    {  	    
        u4Value = (*(volatile UINT32 *)(0xF000D408));
        u4Value &= (~0x03);
        u4Value |= 0x01;

        //Local Arbitor open
        (*(volatile UINT32 *)(0xF0012200)) = 0x0001001F;
	 }
	 else                 //MSDC Controller 0, Base Address: 0xF006d000, pinmux register d414[21:20]/d40c[31:30]/d40c[17], function 2
	 {
        u4Value = (*(volatile UINT32 *)(0xF000D40C));
        u4Value &= (~0xC0020000);
        u4Value |= 0x80020000;
        (*(volatile UINT32 *)(0xF000D40C)) = u4Value;

        u4Value = (*(volatile UINT32 *)(0xF000D414));
        u4Value &= (~0x300000);
        u4Value |= 0x200000;
        (*(volatile UINT32 *)(0xF000D414)) = u4Value;

        //Local Arbitor open
        (*(volatile UINT32 *)(0xF0012200)) = 0x000103E0;
	  }
	 
	  _fgMsdcIdentyCardOK = FALSE;
	  _u4MsdcFreqIdx = 0;
	  
	  MsdcEmmcBootStop(TRUE);
}

//---------------------------------------------------------------------------
// MSDC interface function
//---------------------------------------------------------------------------
static INT32 MsdcCardIdentify (void)
{    
    if (MSDC_SUCCESS != MsdcSetClkfreq(INIT_BUS_FREQ))
	{
	    return MSDC_FAILED;
	}
    
	if (MSDC_SUCCESS != MsdcCardRescan())
	{
	    return MSDC_FAILED;
	}
    
	// CMD7 Select the target card 
	if (CMD_ERR_SUCCESS != cmd7_select_card(1))
	{
	    return MSDC_FAILED;
	}
    
	// Set bus width
	if (MSDC_SUCCESS != MsdcSetBusWidth(4))
	{
	}
    
	if (MSDC_SUCCESS != MsdcSetClkfreq(_u4MsdcFreq[_u4MsdcFreqIdx]))
	{
	    return MSDC_FAILED;
	}

	return MSDC_SUCCESS;
}

//---------------------------------------------------------------------------
// MSDC interface function
//---------------------------------------------------------------------------
static INT32 MsdcSysInit (void)
{
    // Init MSDC
    if (MSDC_SUCCESS != MsdcInit())
    {
        return MSDC_FAILED;
    }

    // Initial Msdc Freq setting flag
    _u4MsdcFreqIdx = 0;

    MsdcContinueClock(0);    // Clock power down when no SD Command

    return MSDC_SUCCESS;
}

static INT32 MsdcIdentyCard (BOOL fgForceIdentCard)
{
    if (_fgMsdcIdentyCardOK && !fgForceIdentCard)
    {
        return MSDC_SUCCESS;
    }
    
    // Clear sdCard structure, let card re-identify	
    x_memset(&sdCard, 0, sizeof(sdcard_t));

    // Identify Card
    if (MSDC_SUCCESS != MsdcCardIdentify())
    {
        return MSDC_FAILED;
    }

    _fgMsdcIdentyCardOK = TRUE;
    return MSDC_SUCCESS;
}

static INT32 MsdcReadCard (UINT32 u4Addr, UINT32 *pu4Buf, UINT32 u4Length)
{
    INT32 i4Ret, i4RetryCnt = 0;

	u4Length = (u4Length + 0x03) & (~0x03);

    do
    {
        i4Ret = MsdcMemRead(u4Addr, (void*)pu4Buf, u4Length);
        
        // Protect maxium retry
        if (i4RetryCnt++ > MSDC_READ_CARD_RETRY_LIMIT)
        {
            break;
        }
        
        // Add error handling
        if (i4Ret != MSDC_SUCCESS)
        {
            if (_u4MsdcFreqIdx < (sizeof(_u4MsdcFreq) / sizeof(UINT32) - 1))
            {
                _u4MsdcFreqIdx++;
            }
            MsdcSetClkfreq(_u4MsdcFreq[_u4MsdcFreqIdx]);

            // If failed count too many, switch to 1 bit mode for read retry
            if (i4RetryCnt > MSDC_READ_CARD_RETRY_LIMIT/2)
            {
                // Set bus width
                if (MSDC_SUCCESS != MsdcSetBusWidth(1))
                {
                }
            }
        }        
    } while (i4Ret != MSDC_SUCCESS);

    if (i4Ret != MSDC_SUCCESS)
    {
        return MSDC_FAILED;
    }
    				
    return MSDC_SUCCESS;
}

static void MsdcToggleContClock (void)
{
    static INT32 i4ContClock = 1;
    
    // Continue clock & non-continue clock toggle for retry  
    if (i4ContClock)
    {
        MsdcContinueClock(0);    // Clock power down when no SD Command
    }
    else
    {
        MsdcContinueClock(1);    // Clock continues even if no SD Command
    }

    i4ContClock = !i4ContClock;
}

static INT32 MsdcIdentReadMMC (UINT32 *pu4Buf, UINT32 u4Addr, UINT32 u4Length)
{
    INT32 i4Ret, i4RetryCnt = 0;

    // Identify Card 
    i4RetryCnt = 0;
    do
    {
        if (i4RetryCnt == 0)
        {
            // Don't need force identify card in the first time
            i4Ret = MsdcIdentyCard(FALSE);
        }
        else
        {
            // Force identify if retry
            i4Ret = MsdcIdentyCard(TRUE);
        }
        
        if (i4Ret != MSDC_SUCCESS)
        {
            // Toggle Continue clock setting for retry
            MsdcToggleContClock();
        } 

        if (i4RetryCnt++ > MSDC_IDENTIFY_CARD_RETRY_LIMIT)
        {
            break;
        }
    } while (i4Ret != MSDC_SUCCESS);

    if (i4Ret != MSDC_SUCCESS)
    {
        return EMMC_IDENT_CARD_FAIL;
    }

    // Read data
    i4RetryCnt = 0;
    do
    {
        i4Ret = MsdcReadCard(u4Addr, pu4Buf, u4Length);
        
        if (i4Ret != MSDC_SUCCESS)
        {
            // Toggle Continue clock setting for retry
            MsdcToggleContClock();
            
            // If retry too many times, re-init the eMMC card
            if (i4RetryCnt >= (MSDC_READ_BOOTLDR_DATA_RETRY_LIMIT/2))
            {
                MsdcIdentyCard(TRUE);
            }
        }
        
        if (i4RetryCnt++ > MSDC_READ_BOOTLDR_DATA_RETRY_LIMIT)
        {
            break;
        }
    } while (i4Ret != MSDC_SUCCESS);

    if (i4Ret != MSDC_SUCCESS)
    {
        return EMMC_READ_CARD_FAIL;
    }    

    return EMMC_SUCCESS;
}

// This function is just protect _u4Signature/ROMCODETAG not optimized by compiler.
UINT32 MsdcDummy(UINT32 i)
{
    return _u4Signature[i];
}

INT32 CHIP_eMMCCopyToDRAM(UINT32 *pu4DstBuf, UINT32 u4ByteOffset, UINT32 u4Len)
{
    INT32 i4Ret;
    UINT32 i4RetryCnt = 0;

    if(!_fgMsdcInit)
	{
		MsdcDummy(0);
    
        MSDC_PinMux();
	
        do
        {
            i4Ret = MsdcSysInit();
            if (i4RetryCnt++ > MSDC_SYS_INIT_RETRY_LIMIT)
            {
                break;
            }		
        } while (i4Ret != MSDC_SUCCESS);

        if (i4Ret != MSDC_SUCCESS)
        {
            return i4Ret;
        }

		_fgMsdcInit = TRUE;
    }
    
	return MsdcIdentReadMMC(pu4DstBuf, u4ByteOffset, u4Len);
}

#endif  // CC_EMMC_BOOT

