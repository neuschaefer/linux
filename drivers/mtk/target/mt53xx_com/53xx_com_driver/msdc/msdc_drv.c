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
 *---------------------------------------------------------------------------*/
/******************************************************************************
* [File]			msdc_drv.c
* [Version]			v1.0
* [Revision Date]	2011-05-04
* [Author]			Shunli Wang, shunli.wang@mediatek.inc, 82896, 2011-05-04
* [Description]
*	MSDC Driver Source File
* [Copyright]
*	Copyright (C) 2011 MediaTek Incorporation. All Rights Reserved.
******************************************************************************/

#include "x_typedef.h"
#include "x_printf.h"
#include "x_hal_arm.h"
#include "x_timer.h"
#include "x_os.h"
#include "x_bim.h"

#include "msdc_debug.h"
#include "msdc_host_hw.h"
#include "msdc_slave_hw.h"
#include "msdc_drv.h"

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------
volatile static UINT32 _u4MsdcAccuVect = 0;
	
//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------
extern UINT32 _u4MsdcFreq[];
extern sdcard_t sdCard;
extern sdhost_t sdHost;
extern UINT32 ch;

//---------------------------------------------------------------------------
// MSDC related function
//---------------------------------------------------------------------------

static VOID MsdcShowChipInfo(UINT32 *pCID)
{	
    Printf("\nCID:%08X:%08X:%08X:%08X", pCID[0], pCID[1], pCID[2], pCID[3]);
	Printf("\nMID:%02X", pCID[3]>>24);
	Printf("\nDevice Type: ");
	if(((pCID[3]>>16)&0x3) == 0x00)
	{
        Printf("Card(removable)");
	}
	else if(((pCID[3]>>16)&0x3) == 0x01)
	{
        Printf("BGA(Discret embedded)");
	}
	else if(((pCID[3]>>16)&0x3) == 0x02)
	{
        Printf("POP");
	}
	else
	{
        Printf("Unknown");
	}
    Printf("\nOID:%02X", pCID[3]>>8);
	Printf("\nPNM: %c%c%c%c%c%c", pCID[3]&0xFF, (pCID[2]>>24)&0xFF, (pCID[2]>>16)&0xFF,  
		                         (pCID[2]>>8)&0xFF, pCID[2]&0xFF, pCID[1]>>24);
	Printf("\nPRV:%02X", (pCID[1]>>16)&0xFF);
	Printf("\nPSN:%08X", (pCID[0]>>16)|(pCID[1]<<16));
	Printf("\nMDT:%02X\n", pCID[0]>>8);
    
}
INT32 MsdcReset(void)
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
        
        HAL_Delay_us(1000);
    }
    
    if (i == MSDC_RST_TIMEOUT_LIMIT_COUNT)
    {
        return MSDC_FAILED;        
    }
	
    return MSDC_SUCCESS;
}

INT32 MsdcClrFifo(void)
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
        
        HAL_Delay_us(1000);
    }
    
    if (i == MSDC_FIFOCLR_TIMEOUT_LIMIT_COUNT)
    {
        return MSDC_FAILED;       
    }

    return MSDC_SUCCESS;
}

VOID MsdcChkFifo(sd_cmd_t *pCmd)
{
    // Check if rx/tx fifo is zero
    if ((MSDC_READ32(MSDC_FIFOCS) & (MSDC_FIFOCS_TXFIFOCNT_MASK | MSDC_FIFOCS_RXFIFOCNT_MASK)) != 0)
    {
        LOG(0, "CMD%d : FiFo not 0, FIFOCS:0x%08X!!\n", pCmd->idx, MSDC_READ32(MSDC_FIFOCS));
        MsdcClrFifo();
    }
}

VOID MsdcClrIntr(VOID)
{
    // Check MSDC Interrupt vector register
    if (0 != MSDC_READ32(MSDC_INT))
    {
        //LOG(0, "MSDC INT(0x%08X) not 0:0x%08X!!\n", MSDC_INT, MSDC_READ32(MSDC_INT));

        // Clear MSDC Interrupt vector register
        MSDC_WRITE32(MSDC_INT, MSDC_READ32(MSDC_INT));
    }
}

INT32 MsdcWaitClkStable (VOID)
{
	UINT32 i;

    for (i = 0; i < MSDC_CLK_TIMEOUT_LIMIT_COUNT; i++)
    {
        if (0 != (MSDC_READ32(MSDC_CFG) & MSDC_CFG_CARD_CK_STABLE))
        {
            break;
        }
        
        HAL_Delay_us(1000);
    }
    
    if (i == MSDC_CLK_TIMEOUT_LIMIT_COUNT)
    {
        LOG(0, "WaitClkStable Failed!\n");
        return MSDC_FAILED;
    }
	
    return MSDC_SUCCESS;
}

INT32 MsdcInit (VOID) 
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

VOID MsdcInitHostConfig(VOID)
{
    x_memset(&sdHost, 0, sizeof(sdhost_t));

    sdHost.accuVect = 0;
    sdHost.blkLen = SDHC_BLK_SIZE;
    sdHost.busWidth = 4;
    sdHost.dataMode = PIO_DATA_MODE;
	sdHost.maxClock = 48;
	sdHost.curClock = _u4MsdcFreq[SD_DEFAULT_NORM_CLOCK_INDEX];
    sdHost.dmaBstSize = DMA_BST_64;
    sdHost.maxBasicDmaLen = BASIC_DMA_MAX_LEN;
    sdHost.polling = 1;
}

VOID MsdcContinueClock (UINT32 i4ContinueClock)
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

INT32 MsdcSetClkFreq(UINT32 clkFreq) 
{
    UINT32 sdClkSel;
    UINT32 expFreq = 0;

	if (clkFreq >= 51)
	{
        sdClkSel = 2; 
		expFreq = 54;
	}
	else if (clkFreq >= 45)
	{
        sdClkSel = 3; 
		expFreq = 48;
	}
	else if (clkFreq >= 42)
	{
        sdClkSel = 4; 
		expFreq = 43;
	}
	else if (clkFreq >= 38)
	{
        sdClkSel = 8; 
		expFreq = 40;
	}
	else if (clkFreq >= 32)
	{
        sdClkSel = 5;
		expFreq = 36;
	}
	else if(clkFreq >= 29)
	{
        sdClkSel = 7;
		expFreq = 30;
	}
	else if (clkFreq >= 26)
	{
        sdClkSel = 0; 
		expFreq = 27;
	}
	else if (clkFreq >= 21)
	{
        sdClkSel = 9; 
		expFreq = 24;
	}
	else if (clkFreq >= 16)
	{
        sdClkSel = 6;
		expFreq = 18;
	}
	else if (clkFreq >= 1)
	{
        sdClkSel = 1;
		expFreq = 13;
	}
	else
	{
        sdClkSel = 0; 
		expFreq = 0;
	}

	if (sdHost.curClock == expFreq)
	{
        return MSDC_SUCCESS;
	}

	MSDC_WRITE32(0xF000D2F8, sdClkSel);

    // set clock mode (DIV mode)
    MSDC_CLRBIT(MSDC_CFG, (((UINT32)0x03) << 16));
    if (expFreq >= 1)
    {
        MSDC_SETBIT(MSDC_CFG, (((UINT32)0x01) << 16));
		
    }
    else
    {
        MSDC_CLRBIT(MSDC_CFG, (((UINT32)0xFF) << 8));
		MSDC_SETBIT(MSDC_CFG, (((UINT32)0x11) << 8));
    }
	
	// Wait until clock is stable
	if (MSDC_FAILED == MsdcWaitClkStable())
	{
        LOG(0, "Set Bus Clock as %d(MHz) Failed!\n", expFreq);
		return MSDC_FAILED;
	}

	LOG(0, "Set Bus Clock as %d(MHz) Success!\n", expFreq);	
	sdHost.curClock= expFreq;

    return MSDC_SUCCESS;

}

// Return when any interrupt is matched or timeout
INT32 MsdcWaitIntr (UINT32 vector, UINT32 timeoutCnt) 
{
    UINT32 i;
    
    // Clear Vector variable
    _u4MsdcAccuVect = 0;  

    if(sdHost.polling == 1)
    {
	    for (i = 0; i < timeoutCnt; i++)
        {
            // Status match any bit
            if (0 != (MSDC_READ32(MSDC_INT) & vector)) 
            {
                _u4MsdcAccuVect |= MSDC_READ32(MSDC_INT);
                MSDC_WRITE32(MSDC_INT, _u4MsdcAccuVect);
                return MSDC_SUCCESS;
            }
        
            HAL_Delay_us(1000);
        }
    }
	else
	{
        // Wait ISR
	}

    // Timeout case
    return MSDC_FAILED;
}

VOID MsdcErrorHandling(VOID)
{
    // Reset MSDC
    MsdcReset();

    // Clear FIFO
    MsdcClrFifo();

    // Check if FIFO clean, check if rx/tx fifo is zero
    if ((MSDC_READ32(MSDC_FIFOCS) & (MSDC_FIFOCS_TXFIFOCNT_MASK | MSDC_FIFOCS_RXFIFOCNT_MASK)) != 0)
    {
        LOG(0, "MsdcErrorHandling : FiFo not 0, FIFOCS:0x%08X!!\n", MSDC_READ32(MSDC_FIFOCS));
        MsdcClrFifo();
    }
    
    // Clear MSDC interrupt
    MsdcClrIntr();        
}

VOID MsdcSetupCmd(sd_cmd_t *pCmd, UINT32 *pu4respType, UINT32 *pu4sdcCmd)
{
    /* Figure out the response type */
    switch (pCmd->idx) 
    {
    case CMD0_GO_IDLE_STATE:
        *pu4respType = RESPONSE_NO;
        (*pu4sdcCmd) |= SDC_CMD_RSPTYPE_NO;
        break;
        
    case CMD1_MMC_SEND_OP_COND:
        *pu4respType = RESPONSE_R3;
        (*pu4sdcCmd) |= SDC_CMD_RSPTYPE_R3;
        break;
        
    case CMD2_ALL_SEND_CID:
        *pu4respType = RESPONSE_R2;
        (*pu4sdcCmd) |= SDC_CMD_RSPTYPE_R2;
        break;
        
    case CMD3_SEND_RELATIVE_ADDR:
        *pu4respType = RESPONSE_R1;
        (*pu4sdcCmd) |= SDC_CMD_RSPTYPE_R1;
        break;
        
    case CMD6_MMC_SWITCH:
        *pu4respType = RESPONSE_R1B;
        (*pu4sdcCmd) |= SDC_CMD_RSPTYPE_R1B;
        break;
        
    case CMD7_SELECT_CARD:
        *pu4respType = RESPONSE_R1;
        (*pu4sdcCmd) |= SDC_CMD_RSPTYPE_R1;
        break;
        
    case CMD12_STOP_TRANSMISSION:
        *pu4respType = RESPONSE_R1B;	  
        (*pu4sdcCmd) |= (SDC_CMD_STOP | SDC_CMD_RSPTYPE_R1B);
        break;
        
    case CMD17_READ_SINGLE_BLOCK:
        *pu4respType = RESPONSE_R1;
        (*pu4sdcCmd) |= (SDC_CMD_RSPTYPE_R1 | DTYPE_SINGLE_BLK | SDC_CMD_READ);
        MSDC_WRITE32(SDC_BLK_NUM, 1);
        break;
        
    case CMD18_READ_MULTIPLE_BLOCK:
        *pu4respType = RESPONSE_R1;
        (*pu4sdcCmd) |= (SDC_CMD_RSPTYPE_R1 | DTYPE_MULTI_BLK | SDC_CMD_READ);
        MSDC_WRITE32(SDC_BLK_NUM, pCmd->buffLen / SDHC_BLK_SIZE);
        break;
        
    case CMD24_WRITE_BLOCK:
    	*pu4respType = RESPONSE_R1;
    	(*pu4sdcCmd) |= (SDC_CMD_RSPTYPE_R1 | DTYPE_SINGLE_BLK | SDC_CMD_WRITE);
    	MSDC_WRITE32(SDC_BLK_NUM, 1);
    	break;
        
    case CMD25_WRITE_MULTIPLE_BLOCK:
        *pu4respType = RESPONSE_R1;
        (*pu4sdcCmd) |= (SDC_CMD_RSPTYPE_R1 | DTYPE_MULTI_BLK | SDC_CMD_WRITE);
        MSDC_WRITE32(SDC_BLK_NUM, pCmd->buffLen / SDHC_BLK_SIZE);
        break;
        
    case CMD35_ERASE_WR_BLK_START:
    case CMD36_ERASE_WR_BLK_END:
        *pu4respType = RESPONSE_R1;
        (*pu4sdcCmd) |= SDC_CMD_RSPTYPE_R1;
        break;
        
    case CMD38_ERASE:
        *pu4respType = RESPONSE_R1B;
        (*pu4sdcCmd) |= SDC_CMD_RSPTYPE_R1B;
        break;	
    }   

    // Set Blk Length
    (*pu4sdcCmd) |= ((SDHC_BLK_SIZE) << SDC_CMD_LEN_SHIFT);

    // Set SDC_CMD.CMD
    (*pu4sdcCmd) |= (pCmd->idx & 0x3F);

    // Set SDC Argument
    MSDC_WRITE32(SDC_ARG, pCmd->arg);

    /* Send the commands to the device */
    MSDC_WRITE32(SDC_CMD, (*pu4sdcCmd));    
    
}

INT32 MsdcSendCmd (sd_cmd_t *pCmd) 
{
    UINT32 i;
    UINT32 respType = RESPONSE_NO;
    UINT32 sdcCmd = 0;
    INT32 i4Ret = CMD_ERR_SUCCESS;
    UINT32 u4CmdDoneVect;

    // Check if rx/tx fifo is zero
    MsdcChkFifo(pCmd);

    // Clear interrupt Vector
    MsdcClrIntr(); 
    
    //LOG(0, "MsdcSendCmd : CMD%d!!\n", pCmd->idx);  

    for (i = 0; i < MSDC_WAIT_SDC_BUS_TIMEOUT_LIMIT_COUNT; i++)
    {
        if ((0 == (MSDC_READ32(SDC_STS) & (SDC_STS_SDCBUSY | SDC_STS_CMDBUSY))) && (0x00  == MSDC_READ32(MSDC_INT)))
        {
		    break;
        }
        
        HAL_Delay_us(1000);
    }
    
    if (i == MSDC_WAIT_SDC_BUS_TIMEOUT_LIMIT_COUNT)
    {
        i4Ret = CMD_ERR_FAILED; 
        goto ErrorHandling;        
    }

    MsdcSetupCmd(pCmd, &respType, &sdcCmd);

    // Wait for command and response if existed
    u4CmdDoneVect = INT_SD_CMDRDY | INT_SD_CMDTO | INT_SD_RESP_CRCERR;

    if (MSDC_SUCCESS != MsdcWaitIntr(u4CmdDoneVect, MSDC_WAIT_CMD_TIMEOUT_LIMIT_COUNT)) 
    {
        LOG(0, "Failed to send CMD/RESP, DoneVect = 0x%x.\n", u4CmdDoneVect);
        i4Ret = CMD_ERR_WAIT_CMD_TO;
        goto ErrorHandling;
    }

    if (_u4MsdcAccuVect & INT_SD_CMDTO)
    {
        LOG(0, "CMD%d ARG 0x%08X - CMD Timeout (AccuVect 0x%08X INTR 0x%08X).\n", pCmd->idx, pCmd->arg, _u4MsdcAccuVect, MSDC_READ32(MSDC_INT));
        i4Ret = CMD_ERR_NO_RESP;
        goto ErrorHandling;
    }
    
    if (_u4MsdcAccuVect & INT_SD_RESP_CRCERR)
    {
        LOG(0, "CMD%d ARG 0x%08X - CMD CRC Error (AccuVect 0x%08X INTR 0x%08X).\n", pCmd->idx, pCmd->arg, _u4MsdcAccuVect, MSDC_READ32(MSDC_INT));
        i4Ret = CMD_ERR_RESP_CRCERR;
        goto ErrorHandling;
    }

    if ((_u4MsdcAccuVect & (~(INT_SD_CMDRDY))) || (0 != MSDC_READ32(MSDC_INT)))
    {
        LOG(0, "CMD%d ARG 0x%08X - UnExpect status (AccuVect 0x%08X INTR 0x%08X).\n", pCmd->idx, pCmd->arg, _u4MsdcAccuVect, MSDC_READ32(MSDC_INT));
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
        x_memcpy(&(sdCard.cardStatus), &pCmd->resp[0], sizeof(int));
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
        UINT32 u4BufLen = (UINT32)(pCmd->buffLen);
        UINT32 u4BufEnd = (UINT32)pBufData + u4BufLen;
        UINT32 u4RxCnt = 0;

        u4CmdDoneVect = INT_SD_DATTO | INT_SD_DATA_CRCERR;

        // Read
        if (!(sdcCmd & SDC_CMD_WRITE))
        {
            while (u4BufLen) 
            {   
                // wait until fifo has enough data
                u4RxFifoCnt = (MSDC_READ32(MSDC_FIFOCS) & MSDC_FIFOCS_RXFIFOCNT_MASK);

                while ((u4BufEnd > (UINT32)pBufData) && (sizeof(int) <= u4RxFifoCnt))
                {
                    // Read Data
                    *pBufData = MSDC_READ32(MSDC_RXDATA);
                     pBufData++;

                     u4RxFifoCnt -= sizeof(int);
					 u4BufLen -= sizeof(int);

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
        }
		else
		{
            while (u4BufEnd > (UINT32)pBufData) 
            {
                // Check if error done vector occurs
                if (u4CmdDoneVect & (_u4MsdcAccuVect | MSDC_READ32(MSDC_INT)))
                {
                    LOG(0, "DoneVect:0x%08X, accuVect:0x%08X, INTR:0x%08X\n", u4CmdDoneVect, _u4MsdcAccuVect, MSDC_READ32(MSDC_INT));
                    break;            
                }

				// wait until fifo has enough space               
                while (1)
                {
                    if((MSDC_READ32(MSDC_FIFOCS) & MSDC_FIFOCS_TXFIFOCNT_MASK) == 0)
                    {
                        break;        
                    }    
                }
                
                u4TxFifoCnt = MSDC_FIFO_LEN;

			    if (sizeof(int) <= u4TxFifoCnt)
			    {
				    while ((u4BufEnd > (UINT32)pBufData) && (sizeof(int) <= u4TxFifoCnt))
				    {
					    // Write Data
					    MSDC_WRITE32(MSDC_TXDATA, *pBufData);
					    pBufData++;
					    u4TxFifoCnt -= sizeof(int);
				    }
			    }
            }

		}

        // Wait for data complete		
        if (MSDC_SUCCESS != MsdcWaitIntr((u4CmdDoneVect | INT_SD_XFER_COMPLETE), 10*MSDC_WAIT_DATA_COMPLETE_TIMEOUT_LIMIT_COUNT)) 
        {
            LOG(0, "Wait Intr timeout (AccuVect 0x%08X INTR 0x%08X).\n", _u4MsdcAccuVect, MSDC_READ32(MSDC_INT));
            i4Ret = CMD_ERR_WAIT_DATATO;
            goto ErrorHandling;
        }

        if (_u4MsdcAccuVect & INT_SD_DATTO)
        {
            LOG(0, "CMD%d ARG 0x%08X - Data Timeout (AccuVect 0x%08X INTR 0x%08X).\n", pCmd->idx, pCmd->arg, _u4MsdcAccuVect, MSDC_READ32(MSDC_INT));
            i4Ret = CMD_ERR_DATTO;
            goto ErrorHandling;
        }
        else if (_u4MsdcAccuVect & INT_SD_DATA_CRCERR)
        {
            LOG(0, "CMD%d ARG 0x%08X - Data CRC Error (AccuVect 0x%08X INTR 0x%08X).\n", pCmd->idx, pCmd->arg, _u4MsdcAccuVect, MSDC_READ32(MSDC_INT));
            i4Ret = CMD_ERR_DATA_CRCERR;
            goto ErrorHandling;
        }
        else if ((_u4MsdcAccuVect & ~(INT_SD_XFER_COMPLETE)) || (0 != MSDC_READ32(MSDC_INT)))
        {
            LOG(0, "UnExpect status (AccuVect 0x%08X INTR 0x%08X).\n", _u4MsdcAccuVect, MSDC_READ32(MSDC_INT));
            i4Ret = CMD_ERR_DATA_FAILED;
            goto ErrorHandling;
        }
    }
	else if(BASIC_DMA_DATA_MODE == pCmd->dataMode)
	{
        UINT32 *pBufData = (UINT32 *)(pCmd->pBuff);
        UINT32 u4BufLen = (UINT32)(pCmd->buffLen);
        UINT32 u4AccLen = 0;

		//printf("DMA Mode: pBufData = %08X, Length = %08X\r\n", (UINT32)pBufData, u4BufLen);

        while (u4AccLen < u4BufLen) 
		{
            MSDC_WRITE32(DMA_SA, (UINT32)pBufData + u4AccLen);
			
            if (u4BufLen - u4AccLen <= BASIC_DMA_MAX_LEN) 
			{
				//printf("->Last: AccLen = %08X\r\n", u4AccLen);
                MSDC_WRITE32(DMA_CTRL, ((u4BufLen - u4AccLen) << DMA_CTRL_XFER_SIZE_SHIFT) | (DMA_BST_64 << DMA_CTRL_BST_SHIFT) | DMA_CTRL_LAST_BUF | DMA_CTRL_START);

                // Wait for sd xfer complete
                if (MSDC_SUCCESS != MsdcWaitIntr(INT_SD_XFER_COMPLETE | INT_DMA_XFER_DONE, 10*MSDC_WAIT_DATA_COMPLETE_TIMEOUT_LIMIT_COUNT)) 
				{
                    LOG(0, "%s: Failed to send/receive data (AccuVect 0x%08X INTR 0x%08X). %s line %d\r\n", __FUNCTION__, _u4MsdcAccuVect, MSDC_READ32(MSDC_INT), __FILE__, __LINE__);
                    return MSDC_FAILED;
                }
                if ((_u4MsdcAccuVect & ~(INT_SD_XFER_COMPLETE | INT_DMA_XFER_DONE)) || (0 != MSDC_READ32(MSDC_INT))) 
				{
                    LOG(0, "%s: Unexpected status (AccuVect 0x%08X INTR 0x%08X). %s line %d\r\n", __FUNCTION__, _u4MsdcAccuVect, MSDC_READ32(MSDC_INT), __FILE__, __LINE__);
                    return MSDC_FAILED;
                }

                u4AccLen += u4BufLen - u4AccLen;
            }
            else 
			{
				//printf("->AccLen = %08X\r\n", u4AccLen);
                MSDC_WRITE32(DMA_CTRL, (BASIC_DMA_MAX_LEN << DMA_CTRL_XFER_SIZE_SHIFT) | (DMA_BST_64 << DMA_CTRL_BST_SHIFT) | DMA_CTRL_START);

                if (MSDC_SUCCESS != MsdcWaitIntr(INT_DMA_XFER_DONE, 10*MSDC_WAIT_DATA_COMPLETE_TIMEOUT_LIMIT_COUNT)) 
				{
                    LOG(0, "%s: Failed to send/receive data (AccuVect 0x%08X INTR 0x%08X). %s line %d\r\n", __FUNCTION__, _u4MsdcAccuVect, MSDC_READ32(MSDC_INT), __FILE__, __LINE__);
                    return MSDC_FAILED;
                }

                // Check DMA status
                if (0 != (MSDC_READ32(DMA_CFG) & (DMA_CFG_DMA_STATUS | DMA_CFG_BD_CS_ERR | DMA_CFG_GPD_CS_ERR))) 
				{
                    LOG(0, "%s: Incorrect DMA status. DMA_CFG: 0x%08X\r\n", __FUNCTION__, MSDC_READ32(DMA_CFG));
                    return MSDC_FAILED;
                }

                if ((_u4MsdcAccuVect & ~(INT_DMA_XFER_DONE)) || (0 != MSDC_READ32(MSDC_INT))) 
				{
                    LOG(0, "%s: Unexpected status (AccuVect 0x%08X INTR 0x%08X). %s line %d\r\n", __FUNCTION__, _u4MsdcAccuVect, MSDC_READ32(MSDC_INT), __FILE__, __LINE__);
                    return MSDC_FAILED;
                }
                u4AccLen += BASIC_DMA_MAX_LEN;
            }
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

/* Command Set */
INT32 cmd0_go_idle_state(UINT32 arg)
{
	sd_cmd_t cmd;

	x_memset(&cmd, 0, sizeof(sd_cmd_t));
	cmd.idx = CMD0_GO_IDLE_STATE;
	cmd.arg = arg;
	cmd.dataMode = NULL_DATA_MODE;

	return MsdcSendCmd(&cmd);
}

INT32 cmd1_mmc_send_op_cond(VOID)
{
	sd_cmd_t cmd;

	x_memset(&cmd, 0, sizeof(sd_cmd_t));
	cmd.idx = CMD1_MMC_SEND_OP_COND;
	cmd.dataMode = NULL_DATA_MODE;
	cmd.arg = HOST_MMC_OCR;

	return MsdcSendCmd(&cmd);
}

INT32 cmd2_all_send_cid(VOID)
{
	sd_cmd_t cmd;

	x_memset(&cmd, 0, sizeof(sd_cmd_t));
	cmd.idx = CMD2_ALL_SEND_CID;
	cmd.dataMode = NULL_DATA_MODE;

	return MsdcSendCmd(&cmd);
}

INT32 cmd3_send_relative_addr(VOID) 
{
	sd_cmd_t cmd;

	x_memset(&cmd, 0, sizeof(sd_cmd_t));
	cmd.idx = CMD3_SEND_RELATIVE_ADDR;
	cmd.dataMode = NULL_DATA_MODE;

    if (sdCard.cardType == CARDTYPE_MEM_MMC)
    {
        sdCard.memRca = MMC_DEF_RCA;
        cmd.arg = (sdCard.memRca) << 16;
    }
            
	return MsdcSendCmd(&cmd);
}

INT32 cmd6_mmc_set_bus_width(INT32 busWidth) 
{
	sd_cmd_t cmd;

	x_memset(&cmd, 0, sizeof(sd_cmd_t));
	cmd.idx = CMD6_MMC_SWITCH;
	cmd.dataMode = NULL_DATA_MODE;
	
   // determine Arguments according to bus selection
	if (1 == busWidth) 
	{
		cmd.arg = MMC_CMD6_ARG_1BIT_BUS;
	}
	else if (4 == busWidth) 
	{
		cmd.arg = MMC_CMD6_ARG_4BIT_BUS;
	}
	else if (8 == busWidth)
	{
		cmd.arg = MMC_CMD6_ARG_8BIT_BUS;
	}
	         
	return MsdcSendCmd(&cmd);
}

INT32 cmd7_select_card(UINT32 sel) 
{
	sd_cmd_t cmd;

	x_memset(&cmd, 0, sizeof(sd_cmd_t));
	cmd.idx = CMD7_SELECT_CARD;
	cmd.dataMode = NULL_DATA_MODE;
	
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

INT32 cmd17_read_single_block(UINT32 addr, UINT8 *pBuff, UINT32 buffLen)
{
	sd_cmd_t cmd;

	x_memset(&cmd, 0, sizeof(sd_cmd_t));
	cmd.idx = CMD17_READ_SINGLE_BLOCK;
	cmd.dataMode = sdHost.dataMode;
	cmd.pBuff = pBuff;
	
	if (sdCard.blkAddrMode == 1)
	{
      cmd.arg = addr >> 9;      // Block addressing mode
	}
	else
	{
      cmd.arg = addr;           // Byte addressing mode
	}
	
	cmd.buffLen = buffLen;
	
	return MsdcSendCmd(&cmd);
}

INT32 cmd18_read_multiple_block(UINT32 addr, UINT8 *pBuff, UINT32 buffLen) 
{
	sd_cmd_t cmd;

	x_memset(&cmd, 0, sizeof(sd_cmd_t));
	cmd.idx = CMD18_READ_MULTIPLE_BLOCK;
	cmd.dataMode = sdHost.dataMode;
	cmd.pBuff = pBuff;
	
	if (sdCard.blkAddrMode == 1)
	{
      cmd.arg = addr >> 9;    // Block addressing mode
	}
	else
	{
      cmd.arg = addr;           // Byte addressing mode
	}
	
	cmd.buffLen = buffLen;
	
	return MsdcSendCmd(&cmd);
}

INT32 cmd24_write_single_block(UINT32 addr, VOID *pBuff, UINT32 buffLen)
{
	sd_cmd_t cmd;

	x_memset(&cmd, 0, sizeof(sd_cmd_t));
	cmd.idx = CMD24_WRITE_BLOCK;
	cmd.dataMode = sdHost.dataMode;
	cmd.pBuff = pBuff;
	cmd.buffLen = buffLen;
	
	if (sdCard.blkAddrMode == 1)
	{
      cmd.arg = addr >> 9;  // Block addressing mode
	}
	else
	{
      cmd.arg = addr;       // Byte addressing mode
	}
	
	return MsdcSendCmd(&cmd);

}

INT32 cmd25_write_multiple_block(UINT32 addr, VOID *pBuff, UINT32 buffLen)
{
	sd_cmd_t cmd;

	x_memset(&cmd, 0, sizeof(sd_cmd_t));
	cmd.idx = CMD25_WRITE_MULTIPLE_BLOCK;
	cmd.dataMode = sdHost.dataMode;
	cmd.pBuff = pBuff;
	cmd.buffLen = buffLen;

	if (sdCard.blkAddrMode == 1)
	{
      cmd.arg = addr >> 9;    // Block addressing mode
	}
	else
	{
      cmd.arg = addr;           // Byte addressing mode
	}
	
	return MsdcSendCmd(&cmd);
}

INT32 cmd35_erase_wr_blk_start(UINT32 addr)
{
	sd_cmd_t cmd;

	x_memset(&cmd, 0, sizeof(sd_cmd_t));
	cmd.idx = CMD35_ERASE_WR_BLK_START;
	cmd.dataMode = NULL_DATA_MODE;

	if (sdCard.blkAddrMode == 1)
	{
      cmd.arg = addr >> 9;    // Block addressing mode
	}
	else
	{
      cmd.arg = addr;           // Byte addressing mode
	}
	
	return MsdcSendCmd(&cmd);

}

INT32 cmd36_erase_wr_blk_end(UINT32 addr)
{
	sd_cmd_t cmd;

	x_memset(&cmd, 0, sizeof(sd_cmd_t));
	cmd.idx = CMD36_ERASE_WR_BLK_END;
	cmd.dataMode = NULL_DATA_MODE;

	if (sdCard.blkAddrMode == 1)
	{
      cmd.arg = addr >> 9;    // Block addressing mode
	}
	else
	{
      cmd.arg = addr;           // Byte addressing mode
	}
	
	return MsdcSendCmd(&cmd);
}

INT32 cmd38_erase(VOID)
{
	sd_cmd_t cmd;

	x_memset(&cmd, 0, sizeof(sd_cmd_t));
	cmd.idx = CMD38_ERASE;
	cmd.dataMode = NULL_DATA_MODE;
	
	return MsdcSendCmd(&cmd);
}

INT32 cmd12_stop_transmission(VOID) 
{
	sd_cmd_t cmd;

	x_memset(&cmd, 0, sizeof(sd_cmd_t));
	cmd.idx = CMD12_STOP_TRANSMISSION;
	cmd.dataMode = NULL_DATA_MODE;

	return MsdcSendCmd(&cmd);
}

// Final stage of card identification flow.
INT32 MsdcCardTypeChk(VOID)
{
    // CMD2
    if (CMD_ERR_SUCCESS != cmd2_all_send_cid())
    {
        LOG(0, "%s: Failed to send CMD2. %s line %d.\n", __FUNCTION__, __FILE__, __LINE__);
        return MSDC_FAILED;
    }

    // CMD3
    if (CMD_ERR_SUCCESS != cmd3_send_relative_addr()) 
    {
        LOG(0, "%s: Failed to send CMD3. %s line %d.\n", __FUNCTION__, __FILE__, __LINE__);
        return MSDC_FAILED;
    }
    
    return MSDC_SUCCESS;
}

INT32 MsdcCardRescan() 
{
	INT32 ret, i;

    for (i = 0; i < CMD0_RESET_LIMIT; i++)
    {
    	// CMD0 Pin1 = High
        if (MSDC_SUCCESS != cmd0_go_idle_state(0x00)) 
    	{
          LOG(0, "%s: Failed to send GO_IDLE_STATE cmd in %s line %d\n", __FUNCTION__, __FILE__, __LINE__);
          return MSDC_FAILED;
    	}
    }
    	
    if (MSDC_SUCCESS != cmd1_mmc_send_op_cond())
    {
        LOG(0, "%s: Failed to send CMD1 %s line %d\n", __FUNCTION__, __FILE__, __LINE__);
        return MSDC_FAILED;
    }

    sdCard.cardType = CARDTYPE_MEM_MMC;

    if (sdCard.memOcr.ocr & HOST_OCR) 
    {
        unsigned int retry = 0;
        
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
                    else                      // MMC : Byte addressing mode
                    {
                        sdCard.blkAddrMode = 0;
                    }

                    if (MSDC_SUCCESS != MsdcCardTypeChk()) 
                    {
                        LOG(0, "%s: Failed to check the card type. %s line %d\n", __FUNCTION__, __FILE__, __LINE__);
                        return MSDC_FAILED;
                    }
                    
                    return MSDC_SUCCESS;
                }
                else 
                {
                    retry++;
                    if (retry < MMC_CMD1_RETRY_LIMIT) 
                    {
                        HAL_Delay_us(2000);
                        continue;
                    }
                    else 
                    {
                        LOG(0, "Failed to init the card, Retry = %d.\n", retry);
                        return MSDC_FAILED;
                    }
                }
            }
            else 
            {
                LOG(0, "Failed to send CMD1.\n");
                return MSDC_FAILED;
            }
        }
    }

	return MSDC_SUCCESS;
}

INT32 MsdcSetBusWidth (UINT32 busWidth) 
{
    if (CMD_ERR_SUCCESS != cmd6_mmc_set_bus_width(busWidth)) 
    {
        LOG(0, "Set card bus width to %d Failed!\n", busWidth);
        return MSDC_FAILED;
    }

    /* Modify MSDC Register Settings */
    if (1 == busWidth) 
    {
        MSDC_WRITE32(SDC_CFG, (MSDC_READ32(SDC_CFG) & ~SDC_CFG_BW_MASK) | (0x00 <<  SDC_CFG_BW_SHIFT));
    }
    else if (4 == busWidth) 
    {
        MSDC_WRITE32(SDC_CFG, (MSDC_READ32(SDC_CFG) & ~SDC_CFG_BW_MASK) | (0x01 <<  SDC_CFG_BW_SHIFT));
    }
	else if(8 == busWidth)
	{
        MSDC_WRITE32(SDC_CFG, (MSDC_READ32(SDC_CFG) & ~SDC_CFG_BW_MASK) | (0x02 <<  SDC_CFG_BW_SHIFT));
	}

    return MSDC_SUCCESS;
}

INT32 MsdcMemRead (UINT32 addr, VOID *pBuff, UINT32 buffLen) 
{
    INT32 i4Ret, i4RetStop;
    UINT32 u4Tmp;

	if(sdHost.dataMode > 1)
	{
        HalDmaBufferStart((void*)pBuff, buffLen, HAL_DMA_FROM_DEVICE);
	}
    
    u4Tmp = addr - (addr & (~(SDHC_BLK_SIZE - 1))) + buffLen;

	//Printf("MsdcMemRead : addr: %08X, bufflen: %08X\n", u4Tmp, buffLen);

    if (u4Tmp <= SDHC_BLK_SIZE) 
    {
        // CMD17 Single block read
        i4Ret = cmd17_read_single_block(addr, pBuff, buffLen);
        if (CMD_ERR_SUCCESS != i4Ret) 
        {
            LOG(0, "%s Failed to read single block with address 0x%08X buffLen 0x%08X\n", __FUNCTION__, addr, buffLen);
            return i4Ret;
        }
    }
    else 
    {
        // CMD18 Multiple block read
        i4Ret = cmd18_read_multiple_block(addr, pBuff, buffLen);
        if (CMD_ERR_SUCCESS != i4Ret) 
        {
            LOG(0, "Failed to read multiple blocks with address 0x%08X buffLen 0x%08X\n", addr, buffLen);

            i4RetStop = cmd12_stop_transmission();
            // Error Handling process : sw should manually add stop command when multi-block read Failed
            if (CMD_ERR_SUCCESS != i4RetStop) 
            {
                LOG(0, "%s Failed to send auto stop\n", __FUNCTION__);
                return i4RetStop;
            }

            return i4Ret;
        }

        i4RetStop = cmd12_stop_transmission();
        if (CMD_ERR_SUCCESS != i4RetStop) 
        {
            LOG(0, "%s Failed to send auto stop\n", __FUNCTION__);
            return i4RetStop;
        }
    }

	if(sdHost.dataMode > 1)
	{
        HalDmaBufferDone((void*)pBuff, buffLen, HAL_DMA_FROM_DEVICE);
	}

    return i4Ret;
}

INT32 MsdcMemWrite(UINT32 addr, VOID *pBuff, UINT32 buffLen)
{
    INT32 i4Ret, i4RetStop;

	if(sdHost.dataMode > 1)
	{
        HalDmaBufferStart((void*)pBuff, buffLen, HAL_DMA_TO_DEVICE);
	}

    if (buffLen <= SDHC_BLK_SIZE) 
    {
        // CMD24 Single block write
        i4Ret = cmd24_write_single_block(addr, pBuff, buffLen);
        if (CMD_ERR_SUCCESS != i4Ret) 
        {
            LOG(0, "%s Failed to write single block with address 0x%08X buffLen 0x%08X\n", __FUNCTION__, addr, buffLen);
            return i4Ret;
        }
    }
    else 
    {
        // CMD25 Multiple block write
        i4Ret = cmd25_write_multiple_block(addr, pBuff, buffLen);
        if (CMD_ERR_SUCCESS != i4Ret) 
        {
            LOG(0, "Failed to write multiple blocks with address 0x%08X buffLen 0x%08X\n", addr, buffLen);

            i4RetStop = cmd12_stop_transmission();
            // Error Handling process : sw should manually add stop command when multi-block read Failed
            if (CMD_ERR_SUCCESS != i4RetStop) 
            {
                LOG(0, "%s Failed to send auto stop\n", __FUNCTION__);
                return i4RetStop;
            }

            return i4Ret;
        }

        i4RetStop = cmd12_stop_transmission();
        if (CMD_ERR_SUCCESS != i4RetStop) 
        {
            LOG(0, "%s Failed to send auto stop\n", __FUNCTION__);
            return i4RetStop;
        }
    }

	if(sdHost.dataMode > 1)
	{
        HalDmaBufferDone((void*)pBuff, buffLen, HAL_DMA_TO_DEVICE);
	}

    return i4Ret;
}

INT32 MsdcMemErase(UINT32 addr, UINT32 buffLen)
{
	INT32 i4Ret;

	i4Ret = cmd35_erase_wr_blk_start(addr);
	if (CMD_ERR_SUCCESS != i4Ret)
	{
		LOG(0, "%s Failed to set erase start address\n", __FUNCTION__);
		return i4Ret;
	}

	i4Ret = cmd36_erase_wr_blk_end(addr + buffLen);
	if (CMD_ERR_SUCCESS != i4Ret)
	{
		LOG(0, "%s Failed to set erase end address\n", __FUNCTION__);
		return i4Ret;
	}

	i4Ret = cmd38_erase();
	if (CMD_ERR_SUCCESS != i4Ret)
	{
		LOG(0, "%s Failed to erase card\n", __FUNCTION__);
		return i4Ret;
	}
	
	return i4Ret;
}

INT32 MsdcEmmcBootStop (BOOL fgResetMsdc)
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

		HAL_Delay_us(1000);
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
