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
* [File]			msdc_if.c
* [Version]			v1.0
* [Revision Date]	2010-08-18
* [Author]			Jethro Chang, jethro.chang@mediatek.inc, 82896, 2010-08-18
* [Description]
*	BootROM C code
* [Copyright]
*	Copyright (C) 2007 MediaTek Incorporation. All Rights Reserved.
******************************************************************************/

#include "x_typedef.h"

#include "msdc_debug.h"
#include "msdc_host_hw.h"
#include "msdc_slave_hw.h"
#include "msdc_drv.h"
#include "msdc_if.h"

sdcard_t sdCard;
sdhost_t sdHost;

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Definition
//---------------------------------------------------------------------------

UINT32 _u4MsdcFreq[] = {MSDC_CLOCK_SELECTION_STRING};
UINT32 _u4MsdcFreqIdx = SD_DEFAULT_NORM_CLOCK_INDEX;
BOOL _fgMsdcIdentyCardOK = FALSE;
UINT32 ch = 0;
UINT32 _pu4BlkBuf[SDHC_BLK_SIZE>>2];


//---------------------------------------------------------------------------
// MSDC PinMux Switch  function
//---------------------------------------------------------------------------
VOID MSDC_PinMux(UINT32 u4Ch)
{
    UINT32 u4Value;

    ch = u4Ch;
     
    if (ch == 0)  //MSDC Controller 0, Base Address: 0xF0012000, pinmux register d408[1:0], function 1
    {  	    
        u4Value = (*(volatile UINT32 *)(0xF000D408));
        u4Value &= (~0x03);
        u4Value |= 0x01;

        //Local Arbitor open
        (*(volatile UINT32 *)(0xF0012200)) = (0x0001001F);
    }
	else  //MSDC Controller 1, Base Address: 0xF006d000, pinmux register d414[21:20]/d40c[31:30]/d40c[17], function 2
	{
        u4Value = (*(volatile UINT32 *)(0xF000D40C));
        u4Value &= (~0xC0020000);
        u4Value |= 0x80020000;
        (*(volatile UINT32 *)(0xF000D40C)) = (u4Value);

        u4Value = (*(volatile UINT32 *)(0xF000D414));
        u4Value &= (~0x300000);
        u4Value |= 0x200000;
        (*(volatile UINT32 *)(0xF000D414)) = (u4Value);

        //Local Arbitor open
        (*(volatile UINT32 *)(0xF0012200)) = (0x000103E0);	
    }
	 
    _u4MsdcFreqIdx = SD_DEFAULT_NORM_CLOCK_INDEX;
	_fgMsdcIdentyCardOK = FALSE;
	  
    MsdcEmmcBootStop(TRUE);
}

//---------------------------------------------------------------------------
// MSDC interface function
//---------------------------------------------------------------------------
INT32 MsdcCardIdentify (VOID)
{    
    if (MSDC_SUCCESS != MsdcSetClkFreq(_u4MsdcFreq[SD_DEFAULT_INIT_CLOCK_INDEX]))
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
	if (MSDC_SUCCESS != MsdcSetBusWidth(sdHost.busWidth))
	{
	    return MSDC_FAILED;
	}

	if (MSDC_SUCCESS != MsdcSetClkFreq(_u4MsdcFreq[SD_DEFAULT_NORM_CLOCK_INDEX]))
	{
	    return MSDC_FAILED;
	}

    return MSDC_SUCCESS;
}


//---------------------------------------------------------------------------
// MSDC interface function
//---------------------------------------------------------------------------
INT32 MsdcSysInit (VOID)
{
	MsdcInitHostConfig();

    // Init MSDC
    if (MSDC_SUCCESS != MsdcInit())
    {
        return MSDC_FAILED;
    }
   
    // Initial Msdc Freq setting flag
    MsdcContinueClock(0);    // Clock power down when no SD Command
   
    return MSDC_SUCCESS;
}

INT32 MsdcSetDataMode(UINT32 fgDataMode)
{
    sdHost.dataMode = fgDataMode;
    if(fgDataMode > PIO_DATA_MODE)
    {
        // Set Host DMA Mode
		MSDC_CLRBIT(MSDC_CFG, MSDC_CFG_PIO_MODE);
    }
	else
	{
        // Set Host PIO Mode
		MSDC_SETBIT(MSDC_CFG, MSDC_CFG_PIO_MODE);
	}

	return MSDC_SUCCESS; 
}

INT32 MsdcSetDataBusWidth(UINT32 busWidth)
{
    sdHost.busWidth = busWidth;

	if(MSDC_SUCCESS != MsdcSetBusWidth(sdHost.busWidth))
	{
        LOG(0, "Set Data Bus Width to %dbit Mode Failed\n", busWidth);
		return MSDC_FAILED;
	}

	return MSDC_SUCCESS; 
}

INT32 MsdcSetBusClock(UINT32 busClock)
{
    return MsdcSetClkFreq(busClock);
}

INT32 MsdcSetContinueClock(UINT32 fgContinueClock)
{
    MsdcContinueClock(fgContinueClock);

    return MSDC_SUCCESS;
}

INT32 MsdcPrintConfigInfo(VOID)
{
    LOG(0, "\n**********MSDC HOST CONFIG INFORMATION**********\n");
	LOG(0, "Block Length: %08X(Fixed)\n", sdHost.blkLen);
	LOG(0, "BUS Width: %d\n", sdHost.busWidth);
	
	if(sdHost.dataMode == PIO_DATA_MODE)
	{
	    LOG(0, "Data Mode: PIO\n");
	}
	else if(sdHost.dataMode == BASIC_DMA_DATA_MODE)
	{
	    LOG(0, "Data Mode: Basic DMA\n");
	}
	else if(sdHost.dataMode == DESC_DMA_DATA_MODE)
	{
	    LOG(0, "Data Mode: Descriptor DMA\n");
	}
	else if(sdHost.dataMode == ENHANCED_DMA_DATA_MODE)
	{
	    LOG(0, "Data Mode: Enhanced DMA\n");
	}

    if(sdHost.dataMode > PIO_DATA_MODE)
    {
	    LOG(0, "DMA Burst Size: %d(Bytes)\n", 0x1<<sdHost.dmaBstSize);
    }
	
	LOG(0, "MAX DMA XFER SIZE: %04X\n", sdHost.maxBasicDmaLen);

	LOG(0, "MAX Clock Frequency: %d(MHz)\n", sdHost.maxClock);
	LOG(0, "Current Clock Frequency: %d(MHz)\n", sdHost.curClock);

	if(sdHost.polling == 1)
	{
	    LOG(0, "Interrupt Handle: Polling\n");
	}
	else if(sdHost.polling == 0)
	{
	    LOG(0, "Interrupt Handle: ISR\n");
	}

	return MSDC_SUCCESS; 
	
}

INT32 MsdcIdentyCard (BOOL fgForceIdentCard)
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

INT32 MsdcReadCard (UINT32 u4Addr, UINT32 *pu4Buf, UINT32 u4Length)
{
    INT32 i4Ret;
    UINT32 u4OffsetInBlk, u4BlkOffset, u4RestSize = u4Length;
    UINT32 u4MemPtr = (UINT32)pu4Buf;
    
    u4OffsetInBlk = u4Addr & (SDHC_BLK_SIZE - 1);
    u4BlkOffset = u4Addr - u4OffsetInBlk;

	//Printf("Read Address %08X, Length: %08X, u4OffsetInBlk: %08X, u4BlkOffset: %08X.\n", u4Addr, u4Length, u4OffsetInBlk, u4BlkOffset);
    
    // Handle offset non-block-size-aligned case
    if (u4OffsetInBlk)
    {
        UINT32 u4OpSize;
        i4Ret = MsdcMemRead(u4BlkOffset, (void *)_pu4BlkBuf, SDHC_BLK_SIZE);
        if (i4Ret != MSDC_SUCCESS)
        {
            LOG(0, "MsdcWriteCard: Read In Process #1 Failed!\n");
            return i4Ret;
        }
        
        if (u4RestSize >= (SDHC_BLK_SIZE - u4OffsetInBlk))
        {
            u4OpSize = SDHC_BLK_SIZE - u4OffsetInBlk;                
        } 
        else
        {
            u4OpSize = u4RestSize;
        }

        x_memcpy((void *)u4MemPtr, (void *)((UINT8 *)_pu4BlkBuf + u4OffsetInBlk), u4OpSize); 
        
        u4RestSize  -= u4OpSize;
        u4MemPtr    += u4OpSize;
        u4BlkOffset += SDHC_BLK_SIZE;
    }     

    // Handle offset & size block-size-aligned case
    if (u4RestSize >= SDHC_BLK_SIZE)
    {
        UINT32 u4ByteCnt = u4RestSize & (~(SDHC_BLK_SIZE - 1));
        
        i4Ret = MsdcMemRead(u4BlkOffset, (void*)u4MemPtr, u4ByteCnt); 
        if (i4Ret != MSDC_SUCCESS)
        {
            LOG(0, "MsdcWriteCard: Read In Process #2 Failed!\n");
            return i4Ret;
        } 
        
        u4RestSize  -= u4ByteCnt;
        u4MemPtr    += u4ByteCnt;
        u4BlkOffset += u4ByteCnt;
    }  
    
    // Handle size none-block-size-aligned case
    if (u4RestSize)
    {
        i4Ret = MsdcMemRead(u4BlkOffset, (void*)_pu4BlkBuf, SDHC_BLK_SIZE); 
        if (i4Ret != MSDC_SUCCESS)
        {
            LOG(0, "MsdcWriteCard: Read In Process #3 Failed!\n");
            return i4Ret;      
        } 
        
        x_memcpy((void *)u4MemPtr, (void *)_pu4BlkBuf, u4RestSize);
        
    }
	
    return MSDC_SUCCESS;

}

INT32 MsdcWriteCard(UINT32 *u4SrcAddr, UINT32 u4ByteOffset, UINT32 u4Len)
{
    INT32 i4Ret;
    UINT32 u4OffsetInBlk, u4BlkOffset, u4RestSize = u4Len;
    UINT32 u4MemPtr = (UINT32)u4SrcAddr;

	Printf("Write Address %08X, Length: %08X.\n", u4ByteOffset, u4Len);
    
    u4OffsetInBlk = u4ByteOffset & (SDHC_BLK_SIZE - 1);
    u4BlkOffset = u4ByteOffset - u4OffsetInBlk;
    
    // Handle offset non-block-size-aligned case
    if (u4OffsetInBlk)
    {
        UINT32 u4OpSize;
        i4Ret = MsdcMemRead(u4BlkOffset, (void *)_pu4BlkBuf, SDHC_BLK_SIZE);
        if (i4Ret != MSDC_SUCCESS)
        {
            LOG(0, "MsdcWriteCard: Read In Write Process #1 Failed\n");
            return i4Ret;
        }
        
        if (u4RestSize >= (SDHC_BLK_SIZE - u4OffsetInBlk))
        {
            u4OpSize = SDHC_BLK_SIZE - u4OffsetInBlk;                
        } 
        else
        {
            u4OpSize = u4RestSize;
        }

        x_memcpy((void *)((UINT8 *)_pu4BlkBuf + u4OffsetInBlk), (void *)u4SrcAddr, u4OpSize); 
        
        i4Ret = MsdcMemWrite(u4BlkOffset, (void*)_pu4BlkBuf, SDHC_BLK_SIZE); 
        if (i4Ret != MSDC_SUCCESS)
        {
            LOG(0, "MsdcWriteCard: Write In Write Process #1 Failed\n");
            return i4Ret;
        } 
        
        u4RestSize  -= u4OpSize;
        u4MemPtr    += u4OpSize;
        u4BlkOffset += SDHC_BLK_SIZE;
    }     

    // Handle offset & size block-size-aligned case
    if (u4RestSize >= SDHC_BLK_SIZE)
    {
        UINT32 u4ByteCnt = u4RestSize & (~(SDHC_BLK_SIZE - 1));
        
        i4Ret = MsdcMemWrite(u4BlkOffset, (void*)u4SrcAddr, u4ByteCnt); 
        if (i4Ret != MSDC_SUCCESS)
        {
            LOG(0, "MsdcWriteCard: Write In Write Process #2 Failed\n");
            return i4Ret;
        } 
        
        u4RestSize  -= u4ByteCnt;
        u4MemPtr    += u4ByteCnt;
        u4BlkOffset += u4ByteCnt;
    }  
    
    // Handle size none-block-size-aligned case
    if (u4RestSize)
    {
        i4Ret = MsdcMemRead(u4BlkOffset, (void*)_pu4BlkBuf, SDHC_BLK_SIZE); 
        if (i4Ret != MSDC_SUCCESS)
        {
            LOG(0, "MsdcWriteCard: Read In Write Process #3 Failed\n");
            return i4Ret;      
        } 
        
        x_memcpy((void *)_pu4BlkBuf, (void *)u4MemPtr, u4RestSize);

        i4Ret = MsdcMemWrite(u4BlkOffset, (void*)_pu4BlkBuf, SDHC_BLK_SIZE);         
        if (i4Ret != MSDC_SUCCESS)
        {
            LOG(0, "MsdcWriteCard: Write In Write Process #3 Failed\n");
            return i4Ret;       
        } 
        
    }
	
    return MSDC_SUCCESS;
}

INT32 MsdcEraseCard(UINT32 u4ByteOffset, UINT32 u4Len)
{
    UINT32 addr, buffLen;
    
    addr = u4ByteOffset & (~(SDHC_BLK_SIZE - 1));
    buffLen = ((u4ByteOffset + u4Len) & (~(SDHC_BLK_SIZE - 1))) + SDHC_BLK_SIZE - addr;
    
    return MsdcMemErase(addr, buffLen);
}

VOID MsdcToggleContClock (VOID)
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

INT32 MsdcIdentReadMMC (UINT32 *pu4Buf, UINT32 u4Addr, UINT32 u4Length)
{
    INT32 i4Ret;
    INT32 i4RetryCnt = 0;

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
        LOG(0, "# 2\n");
        return EMMC_IDENT_CARD_FAIL;
    }

    // Read data
    i4RetryCnt = 0;
    do
    {
        i4Ret = MsdcReadCard(u4Addr, pu4Buf, u4Length);
        if (i4Ret != MSDC_SUCCESS)
        {
            LOG(0, "# 3\n");	
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

INT32 eMMC2DRAM(UINT32 u4Addr, UINT32 *pSrcOrDes, UINT32 u4Length, UINT32 u4Type)
{
    INT32 i4Ret;
    UINT32 i4RetryCnt = 0;
	static UINT32 fgInited = 0;

    if(!fgInited)
	{
        MSDC_PinMux(1);
	
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
            LOG(0, "eMMC2DRAM SysInit Failed!\n");
			return i4Ret;
        }

		fgInited = 1;
    }

    i4Ret = MsdcIdentyCard (FALSE);
	if(i4Ret != MSDC_SUCCESS)
	{
        LOG(0, "eMMC2DRAM Identy Card Failed!\n");
		return i4Ret;
	}

	if(u4Type == 0)
	{
	    return MsdcReadCard(u4Addr, pSrcOrDes, u4Length);
	}
	else
	{
        return MsdcWriteCard(pSrcOrDes, u4Addr, u4Length);    
	}

}



