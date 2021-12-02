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
#include <linux/delay.h>
#include <asm/delay.h>
#include <linux/highmem.h>
#include <linux/pci.h>
#include <linux/dma-mapping.h>

#include <linux/platform_device.h>
#include <linux/mmc/host.h>
#include <linux/mmc/card.h>
#include <linux/mmc/mmc.h>
#include <linux/mmc/sd.h>
#include <asm/scatterlist.h>
#include <linux/jiffies.h>
#include <linux/proc_fs.h>
#include <linux/module.h>
#include <linux/random.h>
#include <mach/irqs.h>

#include "msdc_type.h"
#include "msdc_host_hw.h"
#include "msdc_drv.h"

#define DRIVER_NAME "MTK_MSDC"
#define DRIVER_VERSION "0.10"

//#define BUGMAIL "Mediatek. Inc"

#define PRINTF_DEBUG_ENABLE

#if defined(CONFIG_MT53XX_MAP_CHANNELB_DRAM) || defined(CONFIG_MT53XX_USE_CHANNELB_DRAM)
#define MTK_MSDC_DMA_MASK 0x1fffffff
#else
#define MTK_MSDC_DMA_MASK 0xffffffff
#endif

#ifdef PRINTF_DEBUG_ENABLE

static unsigned int msdc_log_en = MSG_LVL_ERR;

#define MSG(n, f, x...) if ((n) < msdc_log_en){ printk(KERN_ERR ": <%s> " f, __func__,## x); }
#define UTIL_Printf   MSG

#else

#define MSG(lvl, formatStr, args...)
#define UTIL_Printf(formatStr, args...)

#endif

#define SD_BUF_ALLOC_IN_SATA_BLTIN
#ifdef SD_BUF_ALLOC_IN_SATA_BLTIN
//extern u8 *sd_dma_buf;	      /* DMA buffer address (virtual)*/
u8 sd_dma_buf[MSDC_DMA_BUFFER_SIZE];
#endif

#ifdef CC_SDMMC_SUPPORT
int ch = 0;
#else
int ch = 1;
#endif
int _bMSDCCardExist = 0;
struct sdhci_host *msdc_host = NULL;
volatile static unsigned int _u4MsdcAccuVect = 0x00;
static unsigned int _u4CurMsdcclkFreq = 0x0;
static unsigned int _u4BlkBuf[SDHC_BLK_SIZE>>2];
static unsigned int msdc_isr_en = 1;
static struct completion comp;

struct mmc *emmc_dev = NULL;



int MsdcReset(void)
{
#if 0
    unsigned int i;

	// Reset MSDC    
    MSDC_SETBIT(MSDC_CFG, MSDC_CFG_RST);

    for(i = 0; i<MSDC_RST_TIMEOUT_LIMIT_COUNT; i++)
    {
        if (0 == (MSDC_READ32(MSDC_CFG) & MSDC_CFG_RST))
        {
            break;
        }
        
        //HAL_Delay_us(1000);
        udelay(1000);
    }
    if(i == MSDC_RST_TIMEOUT_LIMIT_COUNT)
    {
        return MSDC_FAILED;        
    }
#else

    unsigned long u4JiffSt = 0, u4JiffEnd = 0, u4Time;
    unsigned int u4Val;

	// Reset MSDC    
    MSDC_SETBIT(MSDC_CFG, MSDC_CFG_RST);

    u4JiffSt = jiffies;
	while(1)
	{
        u4Val = (MSDC_READ32(MSDC_CFG) & MSDC_CFG_RST);
		if(u4Val == 0)
		{
            break;
		}
		else
		{
            u4JiffEnd = jiffies;
			u4Time = (u4JiffEnd - u4JiffSt) * 1000/HZ;

			if(u4Time > 20)
			{
                MSG(MSG_LVL_WARN, "Wait HOST Controller Stable Timeout!\r\n");
				return MSDC_FAILED;
			}
		}
	}
#endif

	return MSDC_SUCCESS;

}

int MsdcClrFifo(void)
{
#if 0
    unsigned int i;
    // Reset FIFO
    MSDC_SETBIT(MSDC_FIFOCS, MSDC_FIFOCS_FIFOCLR);

    for(i = 0; i<MSDC_RST_TIMEOUT_LIMIT_COUNT; i++)
    {
        if (0 == (MSDC_READ32(MSDC_FIFOCS) & (MSDC_FIFOCS_FIFOCLR | MSDC_FIFOCS_TXFIFOCNT_MASK | MSDC_FIFOCS_RXFIFOCNT_MASK))) 
        {
            break;
        }
        
        //HAL_Delay_us(1000);
        udelay(1000);
    }
    if(i == MSDC_FIFOCLR_TIMEOUT_LIMIT_COUNT)
    {
        return MSDC_FAILED;       
    }
#else

    unsigned long u4JiffSt = 0, u4JiffEnd = 0, u4Time;
    unsigned int u4Val;

    // Reset FIFO
    MSDC_SETBIT(MSDC_FIFOCS, MSDC_FIFOCS_FIFOCLR);

    u4JiffSt = jiffies;
	while(1)
	{
        u4Val = (MSDC_READ32(MSDC_FIFOCS) & (MSDC_FIFOCS_FIFOCLR | MSDC_FIFOCS_TXFIFOCNT_MASK | MSDC_FIFOCS_RXFIFOCNT_MASK));
		if(u4Val == 0)
		{
            break;
		}
		else
		{
            u4JiffEnd = jiffies;
			u4Time = (u4JiffEnd - u4JiffSt) * 1000/HZ;

			if(u4Time > 20)
			{
                MSG(MSG_LVL_WARN, "Wait HOST Controller FIFO Clear Timeout!\r\n");
				return MSDC_FAILED;
			}
		}
	}
#endif

    return MSDC_SUCCESS;
}

void MsdcChkFifo(void)
{
    // Check if rx/tx fifo is zero
    if ((MSDC_READ32(MSDC_FIFOCS) & (MSDC_FIFOCS_TXFIFOCNT_MASK | MSDC_FIFOCS_RXFIFOCNT_MASK)) != 0)
    {
        //printf("FiFo not 0, FIFOCS:0x%08X !!\r\n", MSDC_READ32(MSDC_FIFOCS));
        MsdcClrFifo();
    }
}

void MsdcClrIntr(void)
{
    // Check MSDC Interrupt vector register
    if  (0x00  != MSDC_READ32(MSDC_INT))
    {
        //printf("MSDC INT(0x%08X) not 0:0x%08X !!\r\n", MSDC_INT, MSDC_READ32(MSDC_INT));

        // Clear MSDC Interrupt vector register
        MSDC_WRITE32(MSDC_INT, MSDC_READ32(MSDC_INT));
    } 
}

int MsdcWaitClkStable (void)
{
#if 0
	unsigned int i;

    for(i = 0; i<MSDC_CLK_TIMEOUT_LIMIT_COUNT; i++)
    {
        if (0 != (MSDC_READ32(MSDC_CFG) & MSDC_CFG_CARD_CK_STABLE))
        {
            break;
        }
        
        //HAL_Delay_us(1000);
        udelay(1000);
    }
	  if(i == MSDC_CLK_TIMEOUT_LIMIT_COUNT)
	  {
        MSG(MSG_LVL_ERR, "WaitClkStable Failed !\r\n");
        return MSDC_FAILED;      
	  }
#else

    unsigned long u4JiffSt = 0, u4JiffEnd = 0, u4Time;
    unsigned int u4Val;

    u4JiffSt = jiffies;
	while(1)
	{
        u4Val = (MSDC_READ32(MSDC_CFG) & MSDC_CFG_CARD_CK_STABLE);
		if(u4Val != 0)
		{
            break;
		}
		else
		{
            u4JiffEnd = jiffies;
			u4Time = (u4JiffEnd - u4JiffSt) * 1000/HZ;

			if(u4Time > 20)
			{
                MSG(MSG_LVL_WARN, "Wait HOST Controller Clock Stable Timeout!\r\n");
				return MSDC_FAILED;
			}
		}
	}
#endif
    return MSDC_SUCCESS;
}

static irqreturn_t MsdcHandleIsr(int irq, void *dev_id)
{
	_u4MsdcAccuVect = MSDC_READ32(MSDC_INT);

	MSG(MSG_LVL_INFO, "[MSDC]%08X!\r\n", _u4MsdcAccuVect);

	MSDC_WRITE32(MSDC_INT, _u4MsdcAccuVect);

	complete(&comp);

    return IRQ_HANDLED;
}

void MsdcSetIsr(unsigned int u4Mask, struct sdhci_host *host)
{
    if (msdc_isr_en)
    {
        if(host->irq == VECTOR_MSDC)
        {
            MSDC_SETBIT(IRQ_EN_REG, (0x1<<MSDC0_EN_SHIFT));
        }
		else if(host->irq == VECTOR_MSDC2)
		{
		    MSDC_SETBIT(IRQ_EN_REG, (0x1<<MISC2_EN_SHIFT));
            MSDC_SETBIT(MSDC1_EN_REG, (0x1<<MSDC1_EN_SHIFT));
		}
        MSDC_WRITE32(MSDC_INTEN, u4Mask);
        enable_irq(host->irq);       

		init_completion(&comp);
    }
}

static int MsdcRegIsr(struct sdhci_host *host)
{
    MsdcClrIntr();  // Clear interrupt, read clear

    if (request_irq(host->irq, MsdcHandleIsr, IRQF_SHARED, DRIVER_NAME, host) != 0)
    {
        printk(KERN_ERR "Request MSDC1 IRQ fail!\n");
        return -1;
    }
    
    MsdcClrIntr();  // Clear interrupt, read clear
    disable_irq(host->irq);
    //disable_irq(30);
    return 0;

}

void MSDC_PinMux(void)
{    
    unsigned int u4Value;
	        
	if (ch == 0)  //MSDC Controller 0, Base Address: 0xF0012000, pinmux register d408[1:0], function 1   
	{  	    	      
	    u4Value = (*(volatile unsigned int *)(0xF000D408));    	      
		u4Value &= (~0x03);    	      
		u4Value |= 0x01; 
		(*(volatile unsigned int *)(0xF000D408)) = u4Value;
    
		//Local Arbitor open	      
		(*(volatile unsigned int *)(0xF0012200)) = 0x0001001F;
	}
	else  //MSDC Controller 1, Base Address: 0xF006d000, pinmux register d414[21:20]/d40c[31:30]/d40c[17], function 2	 
	{	  	  
	    u4Value = (*(volatile unsigned int *)(0xF000D40C));    	      
		u4Value &= (~0xC0020000);    	      
		u4Value |= 0x80020000;    	      
		(*(volatile unsigned int *)(0xF000D40C)) = u4Value;	  		      
		u4Value = (*(volatile unsigned int *)(0xF000D414));    	      
		u4Value &= (~0x300000);    	      
		u4Value |= 0x200000;    	      
		(*(volatile unsigned int *)(0xF000D414)) = u4Value;	      

		//Local Arbitor open	      
		(*(volatile unsigned int *)(0xF0012200)) = 0x000103E0; 	  		  
	}	 	  

}


int MsdcInit (void) 
{
    // Pinmux Change
	MSDC_PinMux();
    // Reset MSDC
    MsdcReset();

    // Set PIO Mode & SD/MMC Mode 
    //MSDC_SETBIT(MSDC_CFG, MSDC_CFG_PIO_MODE | MSDC_CFG_SD);
    MSDC_SETBIT(MSDC_CFG, MSDC_CFG_SD);
	// Set DMA Mode 
	MSDC_CLRBIT(MSDC_CFG, MSDC_CFG_PIO_MODE);

    // Disable sdio & Set bus to 1 bit mode
    MSDC_CLRBIT(SDC_CFG, SDC_CFG_SDIO | SDC_CFG_BW_MASK);

    // set clock mode (DIV mode)
    MSDC_CLRBIT(MSDC_CFG, (((unsigned int)0x03) << 16));

    // Wait until clock is stable
    if (MSDC_FAILED == MsdcWaitClkStable())
    {
        return MSDC_FAILED;
    }
    
    // Set default RISC_SIZE for DWRD pio mode
    MSDC_WRITE32(MSDC_IOCON, (MSDC_READ32(MSDC_IOCON) & ~MSDC_IOCON_RISC_SIZE_MASK) | MSDC_IOCON_RISC_SIZE_DWRD);

    // Set Data timeout setting => Maximum setting
    MSDC_WRITE32(SDC_CFG, (MSDC_READ32(SDC_CFG) & ~(((unsigned int)0xFF) << SDC_CFG_DTOC_SHIFT)) | (((unsigned int)0xFF) << SDC_CFG_DTOC_SHIFT));


    // Timing Tuning

	//MSDC_CLRBIT(SD20_PAD_CTL0, ((unsigned int)0x03));
	//MSDC_SETBIT(SD20_PAD_CTL0, ((unsigned int)0x01)<<1);

	//MSDC_CLRBIT(SD20_PAD_CTL0, ((unsigned int)0x03));
	//MSDC_SETBIT(SD20_PAD_CTL0, ((unsigned int)0x01)<<1);

	//MSDC_CLRBIT(SD20_PAD_CTL0, ((unsigned int)0x03));
	//MSDC_SETBIT(SD20_PAD_CTL0, ((unsigned int)0x01)<<1);
	MSDC_SETBIT(MSDC_IOCON, ((unsigned int)0x03)<<1);

    return MSDC_SUCCESS;
}


void MsdcContinueClock (int i4ContinueClock)
{
    if (i4ContinueClock)
    {
       // Set clock continuous even if no command
       MSDC_SETBIT(MSDC_CFG, (((unsigned int)0x01) << 1));
    }
    else
    {
       // Set clock power down if no command
       MSDC_CLRBIT(MSDC_CFG, (((unsigned int)0x01) << 1));
    }
}

int MsdcWaitHostIdle (void) 
{
#if 0
    unsigned int i;
    for(i=0;i<MSDC_WAIT_SDC_BUS_TIMEOUT_LIMIT_COUNT;i++)
    {
        if ((0 == (MSDC_READ32(SDC_STS) & (SDC_STS_SDCBUSY | SDC_STS_CMDBUSY))) && (0x00  == MSDC_READ32(MSDC_INT)))
        {
		    break;
        }
        //HAL_Delay_us(1000);
        udelay(1000);
    }
    if(i == MSDC_WAIT_SDC_BUS_TIMEOUT_LIMIT_COUNT)
    {
        return MSDC_FAILED;       
    }
#else
    unsigned long u4JiffSt = 0, u4JiffEnd = 0, u4Time;

    u4JiffSt = jiffies;
	while(1)
	{
        if ((0 == (MSDC_READ32(SDC_STS) & (SDC_STS_SDCBUSY | SDC_STS_CMDBUSY))) && (0x00  == MSDC_READ32(MSDC_INT)))
        {
		    break;
        }
		else
		{
            u4JiffEnd = jiffies;
			u4Time = (u4JiffEnd - u4JiffSt) * 1000/HZ;

			if(u4Time > 400)
			{
                MSG(MSG_LVL_WARN, "Wait HOST Controller Idle Timeout!\r\n");
				return MSDC_FAILED;
			}
		}
	}
#endif

    return MSDC_SUCCESS;

}

// Return when any interrupt is matched or timeout
int MsdcWaitIntr (unsigned int vector, unsigned int timeoutCnt) 
{
    unsigned int i, u4Ret;
    
    // Clear Vector variable
    _u4MsdcAccuVect = 0; 

    if(!msdc_isr_en)
    {
#if 0
        for(i = 0; i<timeoutCnt; i++)
        {
            // Status match any bit
            if (0 != (MSDC_READ32(MSDC_INT) & vector)) 
            {
                _u4MsdcAccuVect |= MSDC_READ32(MSDC_INT);
                printk(KERN_ERR "->%08X\r\n", MSDC_READ32(MSDC_INT));
                printk(KERN_ERR "->%08X, %08X, %08X, %08X, %08X\r\n", MSDC_READ32(MSDC_INTEN), MSDC_READ32(0xF0008034), MSDC_READ32(0xF0008030), MSDC_READ32(0xF0008084), MSDC_READ32(0xF0008080));
                MSDC_WRITE32(MSDC_INT, _u4MsdcAccuVect);
                return MSDC_SUCCESS;
            }
        
            //HAL_Delay_us(1000);
            udelay(1000);
        }
#else
        unsigned long u4JiffSt = 0, u4JiffEnd = 0, u4Time;
        unsigned int u4Val;

        u4JiffSt = jiffies;
	    while(1)
	    {
            u4Val = (MSDC_READ32(MSDC_INT) & vector);
		    if(u4Val != 0)
		    {
                _u4MsdcAccuVect |= MSDC_READ32(MSDC_INT);
                MSDC_WRITE32(MSDC_INT, _u4MsdcAccuVect);
                return MSDC_SUCCESS;

		    }
		    else
		    {
                u4JiffEnd = jiffies;
			    u4Time = (u4JiffEnd - u4JiffSt) * 1000/HZ;

			    if(u4Time > timeoutCnt)
			    {
                    MSG(MSG_LVL_WARN, "Wait HOST Controller Interrupt Timeout!\r\n");
				    return MSDC_FAILED;
			    }
		    }
	    }
#endif
    }
	else
	{            
	    u4Ret = wait_for_completion_timeout(&comp, timeoutCnt);

		if(u4Ret == 0)
		{
            MSG(MSG_LVL_WARN, "Wait HOST Controller Interrupt Timeout!\r\n");
			return MSDC_FAILED;
		}
		else
		{
			return MSDC_SUCCESS;
		}
  	}

    // Timeout case
    return MSDC_FAILED;
}

void MsdcSetupCmd(struct mmc_command *cmd, struct mmc_data *data)
{  
    unsigned int u4sdcCmd = 0;

    /* Figure out the response type */
    switch(cmd->opcode) 
    {
        case MMC_GO_IDLE_STATE:
            u4sdcCmd |= SDC_CMD_RSPTYPE_NO;
            break;
        case MMC_SEND_OP_COND:
            u4sdcCmd |= SDC_CMD_RSPTYPE_R3;
            break;
        case MMC_ALL_SEND_CID:
            u4sdcCmd |= SDC_CMD_RSPTYPE_R2;
            break;
        case MMC_SET_RELATIVE_ADDR:
            u4sdcCmd |= SDC_CMD_RSPTYPE_R1;
            break;
        case MMC_SWITCH:
#if 0
            u4sdcCmd |= SDC_CMD_RSPTYPE_R1B;
#else
            u4sdcCmd |= ((mmc_resp_type(cmd) == MMC_RSP_R1)?(SDC_CMD_RSPTYPE_R1):(SDC_CMD_RSPTYPE_R1B));
            if(data)
            {
	            u4sdcCmd |= (DTYPE_SINGLE_BLK | SDC_CMD_READ);
				MSDC_WRITE32(SDC_BLK_NUM, 1);
            }
#endif
            break;
        case MMC_SELECT_CARD:
            u4sdcCmd |= SDC_CMD_RSPTYPE_R1;
            break;
		case MMC_SEND_EXT_CSD:
#if 0
			//if(mmc->version & MMC_VERSION_MMC) {
				//printf("mh, to send MMC CMD8\n"); 				
			u4sdcCmd |= (SDC_CMD_RSPTYPE_R1 | DTYPE_SINGLE_BLK | SDC_CMD_READ); 
			MSDC_WRITE32(SDC_BLK_NUM, 1);	
			//}
			//else{ // for SD or unkown card				
				//u4sdcCmd |= SDC_CMD_RSPTYPE_R7;
			//}
#else
            u4sdcCmd |= ((mmc_resp_type(cmd) == MMC_RSP_R1)?(SDC_CMD_RSPTYPE_R1):(SDC_CMD_RSPTYPE_R7));
            if(data)
            {
	            u4sdcCmd |= (DTYPE_SINGLE_BLK | SDC_CMD_READ);
	            MSDC_WRITE32(SDC_BLK_NUM, 1);
            }
#endif
			break;	
        case MMC_SEND_CSD:
		case MMC_SEND_CID:
			u4sdcCmd |= SDC_CMD_RSPTYPE_R2;
			break;
        case MMC_STOP_TRANSMISSION: 
            u4sdcCmd |= (SDC_CMD_STOP | SDC_CMD_RSPTYPE_R1B);
            break;
		case MMC_SEND_STATUS:
			u4sdcCmd |= SDC_CMD_RSPTYPE_R1;
			break;
		case MMC_SET_BLOCKLEN:
            u4sdcCmd |= SDC_CMD_RSPTYPE_R1;		
			break;
        case MMC_READ_SINGLE_BLOCK:
            u4sdcCmd |= (SDC_CMD_RSPTYPE_R1 | DTYPE_SINGLE_BLK | SDC_CMD_READ);
            MSDC_WRITE32(SDC_BLK_NUM, 1);
            break;
        case MMC_READ_MULTIPLE_BLOCK:
            u4sdcCmd |= (SDC_CMD_RSPTYPE_R1 | DTYPE_MULTI_BLK | SDC_CMD_READ);
			if(data)
			{
                MSDC_WRITE32(SDC_BLK_NUM, data->blocks);
			}
			else
			{
                MSDC_WRITE32(SDC_BLK_NUM, 0x0);
			}
            break;
		case MMC_WRITE_BLOCK:
			u4sdcCmd |= (SDC_CMD_RSPTYPE_R1 | DTYPE_SINGLE_BLK | SDC_CMD_WRITE);
			MSDC_WRITE32(SDC_BLK_NUM, 1);
			break;
		case MMC_WRITE_MULTIPLE_BLOCK:
            u4sdcCmd |= (SDC_CMD_RSPTYPE_R1 | DTYPE_MULTI_BLK | SDC_CMD_WRITE);
			 //For MSDC tunning
			//MSG(MSG_LVL_INFO, "Start Fine Tunning!\n");
			//MSDC_WRITE32(PAD_TUNE, 0x00001F00);
			if(data)
			{
                MSDC_WRITE32(SDC_BLK_NUM, data->blocks);
			}
			else
			{
                MSDC_WRITE32(SDC_BLK_NUM, 0x0);
			}
            break;
        case SD_APP_OP_COND:
            u4sdcCmd |= SDC_CMD_RSPTYPE_R3;
            break;
		case SD_APP_SEND_SCR:
			u4sdcCmd |= (DTYPE_SINGLE_BLK | SDC_CMD_READ | SDC_CMD_RSPTYPE_R1);
			MSDC_WRITE32(SDC_BLK_NUM, 1);
			break;
        case MMC_APP_CMD:
            u4sdcCmd |= SDC_CMD_RSPTYPE_R1;
            break;

    }   


    // Set Blk Length
    if(cmd->opcode == SD_APP_SEND_SCR)
    {
        u4sdcCmd |= (0x8 << SDC_CMD_LEN_SHIFT);    
    }
	else if((cmd->opcode == MMC_SWITCH) && (mmc_resp_type(cmd) == MMC_RSP_R1) && (data))
	{
        u4sdcCmd |= (0x40 << SDC_CMD_LEN_SHIFT);    
	}
	else
	{
        u4sdcCmd |= ((SDHC_BLK_SIZE) << SDC_CMD_LEN_SHIFT);
	}
    //(*pu4sdcCmd) |= ((data->blocksize) << SDC_CMD_LEN_SHIFT);

    // Set SDC_CMD.CMD
    u4sdcCmd |= (cmd->opcode & 0x3F);

	if(data && data->stop)
	{
		u4sdcCmd |= SDC_CMD_AUTO_CMD12;
	}

    MSG(MSG_LVL_INFO, "CMD:%08X, ARGU:%08X\n", u4sdcCmd, cmd->arg);

    // Set SDC Argument
    MSDC_WRITE32(SDC_ARG, cmd->arg);

    /* Send the commands to the device */
    MSDC_WRITE32(SDC_CMD, u4sdcCmd);   
    
}


void MsdcHandleResp(struct mmc_command *cmd)
{
    // Handle the response
    switch (mmc_resp_type(cmd)) 
    {
        case MMC_RSP_NONE:
            //printf("%s: CMD%d CMD 0x%08X ARG 0x%08X RESPONSE_NO\r\n", __FUNCTION__, cmd->cmdidx, *pu4sdcCmd, cmd->cmdarg);
            return MSDC_SUCCESS;            
        case MMC_RSP_R1:
        case MMC_RSP_R1B:
            cmd->resp[0] = MSDC_READ32(SDC_RESP0);
            //printf( "%s: CMD%d CMD 0x%08X ARG 0x%08X RESPONSE_R1/R1B/R5/R6/R6 0x%08X\r\n", __FUNCTION__, cmd->cmdidx, *pu4sdcCmd, cmd->cmdarg, cmd->response[0]);
            break;
        case MMC_RSP_R2:
            cmd->resp[0] = MSDC_READ32(SDC_RESP3);
            cmd->resp[1] = MSDC_READ32(SDC_RESP2);
            cmd->resp[2] = MSDC_READ32(SDC_RESP1);
            cmd->resp[3] = MSDC_READ32(SDC_RESP0);
            //printf( "%s: CMD%d CMD 0x%08X ARG 0x%08X RESPONSE_R2 0x%08X 0x%08X 0x%08X 0x%08X\r\n", __FUNCTION__, cmd->cmdidx, *pu4sdcCmd, cmd->cmdarg, cmd->response[0], cmd->response[1], cmd->response[2], cmd->response[3]);
			break;
        case MMC_RSP_R3:
            cmd->resp[0] = MSDC_READ32(SDC_RESP0);
            //printf( "%s: CMD%d CMD 0x%08X ARG 0x%08X RESPONSE_R3/R4 0x%08X\r\n", __FUNCTION__, cmd->cmdidx, *pu4sdcCmd, cmd->cmdarg, cmd->response[0]);
            break;

    }

}


void MsdcErrorHandling(void)
{
    // Reset MSDC
    MsdcReset();

    // Clear FIFO
    MsdcClrFifo();

    // Check if FIFO clean, check if rx/tx fifo is zero
    if ((MSDC_READ32(MSDC_FIFOCS) & (MSDC_FIFOCS_TXFIFOCNT_MASK | MSDC_FIFOCS_RXFIFOCNT_MASK)) != 0)
    {
        //printf("MsdcErrorHandling : FiFo not 0, FIFOCS:0x%08X !!\r\n", MSDC_READ32(MSDC_FIFOCS));
        MsdcClrFifo();
    }
    
    // Clear MSDC interrupt
    MsdcClrIntr();        
}


static inline void sdhci_init_sg(struct sdhci_host *host, struct mmc_data *data);
static inline int sdhci_next_sg(struct sdhci_host *host);
static inline char *sdhci_sg_to_buffer(struct sdhci_host *host);

int MsdcSendCmd (struct sdhci_host *host, struct mmc_command *cmd, struct mmc_data *data) 
{
    unsigned int respType = MMC_RSP_NONE;
    unsigned int sdcCmd = 0;
    int i4Ret = CMD_ERR_SUCCESS;
    unsigned int u4CmdDoneVect;
    unsigned int i;

    // Check if rx/tx fifo is zero
    MsdcChkFifo();

    // Clear interrupt Vector
    MsdcClrIntr(); 
    
    //MSG(MSG_LVL_INFO, "MsdcSendCmd : CMD%d!!\r\n", cmd->opcode);  
#if 0
    for(i=0;i<MSDC_WAIT_SDC_BUS_TIMEOUT_LIMIT_COUNT;i++)
    {
        if ((0 == (MSDC_READ32(SDC_STS) & (SDC_STS_SDCBUSY | SDC_STS_CMDBUSY))) && (0x00  == MSDC_READ32(MSDC_INT)))
        {
		    break;
        }
        //HAL_Delay_us(1000);
        udelay(1000);
    }
    if(i == MSDC_WAIT_SDC_BUS_TIMEOUT_LIMIT_COUNT)
    {
        i4Ret = CMD_ERR_FAILED;
        goto ErrorHandling;        
    }
#endif

	if(MSDC_FAILED == MsdcWaitHostIdle())
	{
        i4Ret = CMD_ERR_FAILED;
        goto ErrorHandling; 
	}

	MSG(MSG_LVL_INFO, "------>CMD%d ARGU%08X\r\n", cmd->opcode, cmd->arg);

    MsdcSetupCmd(cmd, data);

    // Wait for command and response if existed
    u4CmdDoneVect = INT_SD_CMDRDY | INT_SD_CMDTO | INT_SD_RESP_CRCERR;

    if (MSDC_SUCCESS != MsdcWaitIntr(u4CmdDoneVect, MSDC_WAIT_CMD_TIMEOUT_LIMIT_COUNT)) 
    {
        MSG(MSG_LVL_ERR, "Failed to send CMD/RESP, DoneVect = 0x%x.\r\n", u4CmdDoneVect);
        i4Ret = CMD_ERR_FAILED;
        //WRITEMEM(0x70008104, MSDC_WAIT_CMD_TIMEOUT_ERROR);
        goto ErrorHandling;
    }

	//printf("Interrupt = %08X\n", _u4MsdcAccuVect);

    if (_u4MsdcAccuVect & INT_SD_CMDTO)
    {
        MSG(MSG_LVL_ERR, "CMD%d ARG 0x%08X - CMD Timeout (AccuVect 0x%08X INTR 0x%08X).\r\n", cmd->opcode, cmd->arg, _u4MsdcAccuVect, MSDC_READ32(MSDC_INT));
        i4Ret = CMD_ERR_WAIT_CMD_TO;
        //WRITEMEM(0x70008104, MSDC_CMD_TIMEOUT_ERROR);
        goto ErrorHandling;
    }
    else if (_u4MsdcAccuVect & INT_SD_RESP_CRCERR)
    {
        MSG(MSG_LVL_ERR, "CMD%d ARG 0x%08X - CMD CRC Error (AccuVect 0x%08X INTR 0x%08X).\r\n", cmd->opcode, cmd->arg, _u4MsdcAccuVect, MSDC_READ32(MSDC_INT));
        i4Ret = CMD_ERR_FAILED;
        //WRITEMEM(0x70008104, MSDC_CMD_CRC_ERROR);
        goto ErrorHandling;
    }
    else if ((_u4MsdcAccuVect & (~(INT_SD_CMDRDY))) || (0 != MSDC_READ32(MSDC_INT)))
    {
        MSG(MSG_LVL_ERR, "CMD%d ARG 0x%08X - UnExpect status (AccuVect 0x%08X INTR 0x%08X).\r\n", cmd->opcode, cmd->arg, _u4MsdcAccuVect, MSDC_READ32(MSDC_INT));
        i4Ret = CMD_ERR_FAILED;
        //WRITEMEM(0x70008104, MSDC_CMD_INT_ERROR);        
        goto ErrorHandling;
    }

    // Handle the response
    MsdcHandleResp(cmd);

    if(data)
    {
        unsigned int u4RxFifoCnt, u4TxFifoCnt;
        //unsigned int *pBufData;
        unsigned int u4BufLen = (unsigned int)((data->blocks)*(data->blksz));
        //unsigned int u4BufEnd = (unsigned int)pBufData + u4BufLen;
		//struct mmc_data *data = host->mrq->cmd->data;
		unsigned char *buffer;
		unsigned int i, fifo = 4;
		unsigned char fifo_buf[4] = {0x0};
        unsigned int u4RxCnt = 0;

        // Initialize sg for data transfer
		sdhci_init_sg(host, data);
        
		/*
		 * Handle excessive data.
		 */
		if (host->num_sg == 0)
			return;

		buffer = sdhci_sg_to_buffer(host) + host->offset;
	    if(PIO_DATA_MODE == host->data_mode)
	    {
			if (data->stop)
			{
			   u4CmdDoneVect = INT_SD_DATTO | INT_SD_DATA_CRCERR | 
										INT_SD_AUTOCMD_TO | INT_SD_AUTOCMD_RESP_CRCERR; 		 
			}
			else
			{
			   u4CmdDoneVect = INT_SD_DATTO | INT_SD_DATA_CRCERR;
			}

	      
	        // Read
	        if (data->flags & MMC_DATA_READ)
	        {
	            while (u4BufLen) 
	            {   
	                // wait until fifo has enough data
	                u4RxFifoCnt = (MSDC_READ32(MSDC_FIFOCS) & MSDC_FIFOCS_RXFIFOCNT_MASK);

	                while ((u4BufLen) && (sizeof(int) <= u4RxFifoCnt))
	                {                                         
	                     // Read Data
	                     *((unsigned int *)fifo_buf) = MSDC_READ32(MSDC_RXDATA);

						 //MSG(MSG_LVL_INFO, "%08X ", *((unsigned int *)fifo_buf));    

	                     u4RxFifoCnt -= sizeof(int);
	                     u4RxCnt += sizeof(int);

					     for (i = 0; i < fifo; i++) 
					     {
						     *buffer = fifo_buf[i];
						     buffer++;
						     host->offset++;
						     host->remain--;
					
						     data->bytes_xfered++;
					
						    /*
						      * End of scatter list entry?
						      */
						     if (host->remain == 0) 
						     {
							    /*
							     * Get next entry. Check if last.
							     */
							    if (!sdhci_next_sg(host)) 
							    {
								    break;
							    }
					
							    buffer = sdhci_sg_to_buffer(host);
						     }
					     }

	                     
	                     if(u4RxCnt == SDHC_BLK_SIZE)
	                     {
	                         // Check CRC error happens by every 512 Byte
	                         // Check if done vector occurs
	                         if (u4CmdDoneVect & MSDC_READ32(MSDC_INT))
	                         {
								 MSG(MSG_LVL_ERR, "Read Error Break !!\r\n");
	                             break;
	                         }
	                                         
							 u4RxCnt = 0; 
							 u4BufLen -= SDHC_BLK_SIZE;  
	                     }
	                                                
	                }                   
	            }         
	        }
			else
			{
	            while (u4BufLen) 
	            {
	                // Check if error done vector occurs
	                if (u4CmdDoneVect & (_u4MsdcAccuVect | MSDC_READ32(MSDC_INT)))
	                {
	                    MSG(MSG_LVL_INFO, "DoneVect:0x%08X, accuVect:0x%08X,  INTR:0x%08X\r\n", u4CmdDoneVect, _u4MsdcAccuVect, MSDC_READ32(MSDC_INT));
	                    break;            
	                }

					// wait until fifo has enough space               
	                while(1)
	                {
	                    if((MSDC_READ32(MSDC_FIFOCS) & MSDC_FIFOCS_TXFIFOCNT_MASK) == 0)
	                    {
	                        break;        
	                    }    
	                }
	                
	                u4TxFifoCnt = MSDC_FIFO_LEN;

				    if (sizeof(int) <= u4TxFifoCnt)
				    {
					    while ((u4BufLen) && (sizeof(int) <= u4TxFifoCnt))
					    {

						    // gather byte data into fifo_buf		   
						    for (i = 0; i < fifo; i++) 
						    {
						        fifo_buf[i] = *buffer;
							    buffer++;
							    host->offset++;
							    host->remain--;
						
							    data->bytes_xfered++;
						
							    /*
							      * End of scatter list entry?
							      */
							    if (host->remain == 0) 
							    {
								    /*
								     * Get next entry. Check if last.
								     */
								    if (!sdhci_next_sg(host)) 
								    {
									    break;
								    } 
						
								    buffer = sdhci_sg_to_buffer(host);
							    }
						    }

						    // Write Data
						    MSDC_WRITE32(MSDC_TXDATA, *((unsigned int *)(fifo_buf)));
						    u4TxFifoCnt -= sizeof(int);
							u4BufLen -= sizeof(int);						

					    }
				    }
	            }

			}

	        // Wait for data complete		
	        if (MSDC_SUCCESS != MsdcWaitIntr((u4CmdDoneVect | INT_SD_XFER_COMPLETE), 10*MSDC_WAIT_DATA_COMPLETE_TIMEOUT_LIMIT_COUNT)) 
	        {
	            MSG(MSG_LVL_ERR, "Wait Intr timeout (AccuVect 0x%08X INTR 0x%08X).\r\n", _u4MsdcAccuVect, MSDC_READ32(MSDC_INT));
	            i4Ret = CMD_ERR_WAIT_DATATO;
	            //WRITEMEM(0x70008104, MSDC_WAIT_DATA_TIMEOUT_ERROR);
	            goto ErrorHandling;
	        }

	        if (_u4MsdcAccuVect & INT_SD_DATTO)
	        {
	            MSG(MSG_LVL_ERR, "CMD%d ARG 0x%08X - Data Timeout (AccuVect 0x%08X INTR 0x%08X).\r\n", cmd->opcode, cmd->arg, _u4MsdcAccuVect, MSDC_READ32(MSDC_INT));
	            i4Ret = CMD_ERR_DATTO;
	            //WRITEMEM(0x70008104, MSDC_DATA_TIMEOUT_ERROR);
	            goto ErrorHandling;
	        }
	        else if (_u4MsdcAccuVect & INT_SD_DATA_CRCERR)
	        {
	            MSG(MSG_LVL_ERR, "CMD%d ARG 0x%08X - Data CRC Error (AccuVect 0x%08X INTR 0x%08X).\r\n", cmd->opcode, cmd->arg, _u4MsdcAccuVect, MSDC_READ32(MSDC_INT));
	            i4Ret = CMD_ERR_DATA_CRCERR;
	            //WRITEMEM(0x70008104, MSDC_DATA_CRC_ERROR);
	            goto ErrorHandling;
	        }
	        else if ((_u4MsdcAccuVect & ~(INT_SD_XFER_COMPLETE | ((data->stop)?(INT_SD_AUTOCMD_RDY):0))) || (0 != MSDC_READ32(MSDC_INT)))
	        {
	            MSG(MSG_LVL_ERR, "UnExpect status (AccuVect 0x%08X INTR 0x%08X).\r\n", _u4MsdcAccuVect, MSDC_READ32(MSDC_INT));
	            i4Ret = CMD_ERR_DATA_FAILED;
	            //WRITEMEM(0x70008104, MSDC_DATA_INT_ERROR);
	            goto ErrorHandling;
	        }
	    }
		else if(BASIC_DMA_DATA_MODE == host->data_mode)
		{
			unsigned int u4ActuralLen, u4PhyAddr;
			while(u4BufLen)
			{	
				/*
				  * End of scatter list entry?
				  */
				if (host->remain == 0) 
				{
					/*
						 * Get next entry. Check if last.
					   */
					if (!sdhci_next_sg(host)) 
					{
						break;
					}
					
				}  
				MSG(MSG_LVL_INFO, "#1: %d\r\n", host->remain);

				buffer = sdhci_sg_to_buffer(host) + host->offset;

				u4PhyAddr = dma_map_single(NULL, buffer, host->remain, (data->flags & MMC_DATA_READ)?(DMA_FROM_DEVICE):(DMA_TO_DEVICE));

				if (u4PhyAddr >= 0x20000000)
				    printk("!!!!MTK MSDC: DMA to ChB %p\n", buffer);
				MSDC_WRITE32(DMA_SA, u4PhyAddr);

				if((host->num_sg == 1) && ((host->remain) <= BASIC_DMA_MAX_LEN))
				{
					u4ActuralLen = host->remain;
					MSG(MSG_LVL_INFO, "->Last: AccLen = %08X\r\n", u4ActuralLen);
	                MSDC_WRITE32(DMA_CTRL, ((u4ActuralLen) << DMA_CTRL_XFER_SIZE_SHIFT) | (DMA_BST_64 << DMA_CTRL_BST_SHIFT) | DMA_CTRL_LAST_BUF | DMA_CTRL_START);

	                // Wait for sd xfer complete
	                if (MSDC_SUCCESS != MsdcWaitIntr(INT_SD_XFER_COMPLETE | INT_DMA_XFER_DONE | INT_SD_AUTOCMD_RDY, 10*MSDC_WAIT_DATA_COMPLETE_TIMEOUT_LIMIT_COUNT)) {
	                    MSG(MSG_LVL_ERR, "%s: Failed to send/receive data (AccuVect 0x%08X INTR 0x%08X). %s line %d\r\n", __FUNCTION__, _u4MsdcAccuVect, MSDC_READ32(MSDC_INT), __FILE__, __LINE__);
	                    return MSDC_FAILED;
	                }
	                if ((_u4MsdcAccuVect & ~(INT_SD_XFER_COMPLETE | INT_DMA_XFER_DONE | INT_SD_AUTOCMD_RDY)) || (0 != MSDC_READ32(MSDC_INT))) {
	                    MSG(MSG_LVL_ERR, "%s: Unexpected status (AccuVect 0x%08X INTR 0x%08X). %s line %d\r\n", __FUNCTION__, _u4MsdcAccuVect, MSDC_READ32(MSDC_INT), __FILE__, __LINE__);
	                    return MSDC_FAILED;
	                }


				}
				else
				{
					u4ActuralLen = ((host->remain) <= BASIC_DMA_MAX_LEN)?(host->remain):(BASIC_DMA_MAX_LEN);
					MSG(MSG_LVL_INFO, "->AccLen = %08X\r\n", u4ActuralLen);
	                MSDC_WRITE32(DMA_CTRL, (u4ActuralLen << DMA_CTRL_XFER_SIZE_SHIFT) | (DMA_BST_64 << DMA_CTRL_BST_SHIFT) | DMA_CTRL_START);

	                if (MSDC_SUCCESS != MsdcWaitIntr(INT_DMA_XFER_DONE, 10*MSDC_WAIT_DATA_COMPLETE_TIMEOUT_LIMIT_COUNT)) {
	                    MSG(MSG_LVL_ERR, "%s: Failed to send/receive data (AccuVect 0x%08X INTR 0x%08X). %s line %d\r\n", __FUNCTION__, _u4MsdcAccuVect, MSDC_READ32(MSDC_INT), __FILE__, __LINE__);
	                    return MSDC_FAILED;
	                }

					//x_thread_delay(200);        //add by shunli 2010.11.24 for DMA_CFG=0x00000001 when test DescSingleByte

	                // Check DMA status
	                if (0 != (MSDC_READ32(DMA_CFG) & (DMA_CFG_DMA_STATUS | DMA_CFG_BD_CS_ERR | DMA_CFG_GPD_CS_ERR))) {
	                    MSG(MSG_LVL_ERR, "%s: Incorrect DMA status. DMA_CFG: 0x%08X\r\n", __FUNCTION__, MSDC_READ32(DMA_CFG));
	                    return MSDC_FAILED;
	                }

	                if ((_u4MsdcAccuVect & ~(INT_DMA_XFER_DONE)) || (0 != MSDC_READ32(MSDC_INT))) {
	                    MSG(MSG_LVL_ERR, "%s: Unexpected status (AccuVect 0x%08X INTR 0x%08X). %s line %d\r\n", __FUNCTION__, _u4MsdcAccuVect, MSDC_READ32(MSDC_INT), __FILE__, __LINE__);
	                    return MSDC_FAILED;
	                }

				}

				dma_unmap_single(NULL, u4PhyAddr, host->remain, (data->flags & MMC_DATA_READ)?(DMA_FROM_DEVICE):(DMA_TO_DEVICE));

				u4BufLen -= u4ActuralLen;
				host->offset += u4ActuralLen;
				host->remain -= u4ActuralLen;
				data->bytes_xfered += u4ActuralLen;

				MSG(MSG_LVL_INFO, "#2: %d, %d, %d, %d\r\n", u4BufLen, host->offset, host->remain, data->bytes_xfered);
				
			}

		}
    }

    if (data && (data->flags & MMC_DATA_READ))
    {
		//MSG(MSG_LVL_INFO, "Close Fine Tunning!\n");
		//MSDC_WRITE32(PAD_TUNE, 0x00000000);

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

int MsdcSetClkfreq(unsigned int clkFreq) 
{
    unsigned int sdClkSel;
    unsigned int expFreq = 0;

	if(clkFreq>=51000000)
	{
        sdClkSel = 2; 
		expFreq = 54000000;
	}
	else if(clkFreq>=45000000)
	{
        sdClkSel = 3; 
		expFreq = 48000000;
	}
	else if(clkFreq>=42000000)
	{
        sdClkSel = 4; 
		expFreq = 43200000;
	}
	else if(clkFreq>=38000000)
	{
        sdClkSel = 8; 
		expFreq = 40500000;
	}
	else if(clkFreq>=32000000)
	{
        sdClkSel = 5;
		expFreq = 36000000;
	}
	else if(clkFreq>=26000000)
	{
        sdClkSel = 0; 
		expFreq = 27000000;
	}
	else if(clkFreq>=21000000)
	{
        sdClkSel = 9; 
		expFreq = 24000000;
	}
	else if(clkFreq>=16000000)
	{
        sdClkSel = 6;
		expFreq = 18000000;
	}
	else if(clkFreq>=1000000)
	{
        sdClkSel = 1;
		expFreq = 13500000;
	}
	else
	{
        sdClkSel = 0; 
		expFreq = 400000;
	}

	if(_u4CurMsdcclkFreq == expFreq)
	{
        return MSDC_SUCCESS;
	}

	MSDC_WRITE32(0xF000D2F8, sdClkSel);

    // set clock mode (DIV mode)
    MSDC_CLRBIT(MSDC_CFG, (((unsigned int)0x03) << 16));
    if(clkFreq>=1000000)
    {
        MSDC_SETBIT(MSDC_CFG, (((unsigned int)0x01) << 16));
		
    }
    else
    {
        MSDC_CLRBIT(MSDC_CFG, (((unsigned int)0xFF) << 8));  
		MSDC_SETBIT(MSDC_CFG, (((unsigned int)0x11) << 8));
    }
	
	// Wait until clock is stable
	if (MSDC_FAILED == MsdcWaitClkStable())
	{
		return MSDC_FAILED;
	}
	
	//printf("ClkFreq : %d, Expect Frequency : %d Hz\r\n", clkFreq, expFreq);			
	_u4CurMsdcclkFreq = clkFreq;

    return MSDC_SUCCESS;
}

int MsdcSetBusWidth (int busWidth) 
{ 
    MSG(MSG_LVL_INFO, "Set Bus Width:%d\n", (1<<busWidth));

	/* Modify MSDC Register Settings */    
	if (MMC_BUS_WIDTH_1 == busWidth)     
	{        
	    MSDC_WRITE32(SDC_CFG, (MSDC_READ32(SDC_CFG) & ~SDC_CFG_BW_MASK) | (0x00 <<  SDC_CFG_BW_SHIFT));    
	}    
	else if (MMC_BUS_WIDTH_4 == busWidth)     
	{        
	    MSDC_WRITE32(SDC_CFG, (MSDC_READ32(SDC_CFG) & ~SDC_CFG_BW_MASK) | (0x01 <<  SDC_CFG_BW_SHIFT));    
	}    
	else if (MMC_BUS_WIDTH_8 == busWidth)     
	{        
	    MSDC_WRITE32(SDC_CFG, (MSDC_READ32(SDC_CFG) & ~SDC_CFG_BW_MASK) | (0x02 <<  SDC_CFG_BW_SHIFT));    
	}   


	return MSDC_SUCCESS;
}


void MsdcSetIos(struct mmc_host *mmc, struct mmc_ios *ios)
{
    unsigned int clock = ios->clock;
	unsigned int busWidth = ios->bus_width;

	MsdcSetClkfreq(clock);

	MsdcSetBusWidth(busWidth);

}

#if 0

int MsdcEmmcBootStop (uint fgResetMsdc)
{    
    UINT32 i;    
	// Step 5 :        

	// Issue Reset Command    
	MSDC_WRITE32(SDC_ARG, 0x00000000);    

	// Set SDC Argument        
	MSDC_WRITE32(SDC_CMD, 0x00001000);    

	/* Send the commands to the device */        
	// Boot Wait Delay    
	MSDC_SETBIT(EMMC_CFG0, BTWDLY_2x32);           

	// Boot Stop    
	MSDC_SETBIT(EMMC_CFG0, BTSTOP);
	  
	for(i = 0;i<MSDC_WAIT_BTSTS_0_TIMEOUT_LIMIT_COUNT;i++)    
	{        
	    if (!(MSDC_READ32(EMMC_STS) & (BTSTS)))        
		{            
		    break;        
		}        

		//HAL_Delay_us(1000);   
		udelay(1000);
	}	  
	if(i==MSDC_WAIT_BTSTS_0_TIMEOUT_LIMIT_COUNT)	  
	{        
	    // Fetal Error        
	    return MSDC_FAILED;    	  
	}       

	// Step 6 :    
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


void MSDC_PinMux(uint u4Ch)
{    
    UINT32 u4Value;
	
	ch = u4Ch;         
	if(ch == 0)  //MSDC Controller 0, Base Address: 0xF0012000, pinmux register d408[1:0], function 1   
	{  	    	      
	    u4Value = (*(volatile unsigned int *)(0xF000D408));    	      
		u4Value &= (~0x03);    	      
		u4Value |= 0x01;    	      

		//(*(volatile unsigned int *)(0xF000D408)) = (u4Value);	     
		//Local Arbitor open	      
		(*(volatile uint *)(0xF0012200)) = (0x0001001F);	 
	}	 

	else  //MSDC Controller 1, Base Address: 0xF006d000, pinmux register d414[21:20]/d40c[31:30]/d40c[17], function 2	 
	{	  	  
	    u4Value = (*(volatile unsigned int *)(0xF000D40C));    	      
		u4Value &= (~0xC0020000);    	      
		u4Value |= 0x80020000;    	      
		(*(volatile unsigned int *)(0xF000D40C)) = (u4Value);	  		      
		u4Value = (*(volatile unsigned int *)(0xF000D414));    	      
		u4Value &= (~0x300000);    	      
		u4Value |= 0x200000;    	      
		(*(volatile unsigned int *)(0xF000D414)) = (u4Value);	      

		//Local Arbitor open	      
		(*(volatile uint *)(0xF0012200)) = (0x000103E0); 	  		  
	}	 	  

	MsdcEmmcBootStop(1);

}

#endif


/*
 * Scatter/gather functions
 */

static inline void sdhci_init_sg(struct sdhci_host *host, struct mmc_data *data)
{
	/*
	 * Get info. about SG list from data structure.
	 */
	host->cur_sg = data->sg;
	host->num_sg = data->sg_len;

	host->offset = 0;
	host->remain = host->cur_sg->length;
}

static inline int sdhci_next_sg(struct sdhci_host *host)
{
	/*
	 * Skip to next SG entry.
	 */
	host->cur_sg++;
	host->num_sg--;

	/*
	 * Any entries left?
	 */
	if (host->num_sg > 0) 
	{
		host->offset = 0;
		host->remain = host->cur_sg->length;
	}

	return host->num_sg;
}

static inline char *sdhci_sg_to_buffer(struct sdhci_host *host)
{
    return sg_virt(host->cur_sg);
}

static void sdhci_write_pio(struct sdhci_host *host)
{
	struct mmc_data *data = host->mrq->cmd->data;
	char *buffer;
	int i, fifo = 4;
	char fifo_buf[4];

	/*
	 * Check that we aren't being called after the
	 * entire buffer has been transfered.
	 */	
   if (host->num_sg == 0)
		return;

	buffer = sdhci_sg_to_buffer(host) + host->offset;
}


static inline void sdhci_sg_to_dma(struct sdhci_host *host, struct mmc_data *data)
{
    unsigned int len, i;
    struct scatterlist *sg;
    char *dmabuf = host->dma_buf;
    char *sgbuf;
  
    sg = data->sg;
    len = data->sg_len;

    for (i = 0; i < len; i++) 
    {		
        sgbuf = sg_virt(&sg[i]);
        memcpy(dmabuf, sgbuf, sg[i].length);
        dmabuf += sg[i].length;
    }

    host->dma_req_adr = host->dma_buf_adr;
}

static inline void sdhci_dma_to_sg(struct sdhci_host *host, struct mmc_data *data)
{
    unsigned int len, i,j;
    struct scatterlist *sg;
    char *dmabuf = host->dma_buf;
    char *sgbuf;

    sg = data->sg;
    len = data->sg_len;

    for (i = 0; i < len; i++) 
    {
        sgbuf = sg_virt(&sg[i]);
        memcpy(sgbuf, dmabuf, sg[i].length);
        dmabuf += sg[i].length;
    }
}


void sdhci_send_command(struct sdhci_host *host, struct mmc_command *cmd)
{
    unsigned int u4Cmd = 0x00;
    unsigned int u4BlkField;
    int i4Ret;      
    struct mmc_data *data = cmd->data;
   
    MSG(MSG_LVL_INFO, "Start - Sending cmd (%d), flags = 0x%08x\n", cmd->opcode, cmd->flags);

    // Save current command
    memcpy(&(host->cur_cmd), cmd, sizeof(struct mmc_command));
    
   /*
    * Send the command
    */
	i4Ret = MsdcSendCmd (host, cmd, data);   

	if(i4Ret)
    {
        cmd->error = -EIO;     
    }

end:

    MSG(MSG_LVL_INFO, "Finish - Sent cmd (%d), error: %d, i4Ret: 0x%x\n", cmd->opcode, cmd->error, i4Ret); 
}


/*****************************************************************************\
 *                                                                           *
 * MMC callbacks                                                             *
 *                                                                           *
\*****************************************************************************/

static void sdhci_request_end(struct sdhci_host *host, struct mmc_request *mrq)
{

    MSG(MSG_LVL_INFO, "%s: req done (CMD%u): %d/%d/%d: %08x %08x %08x %08x\n",
    	 mmc_hostname(host->mmc), mrq->cmd->opcode, mrq->cmd->error,
    	 mrq->data ? mrq->data->error : 0,
    	 mrq->stop ? mrq->stop->error : 0,
    	 mrq->cmd->resp[0], mrq->cmd->resp[1], mrq->cmd->resp[2], mrq->cmd->resp[3]);

    if (mrq->data)
    {
       if (host->hispeedcard) // High Speed Mode
       {
           MSG(MSG_LVL_INFO, "LBA : 0x%08X,  Len : %d bytes\n", (mrq->cmd->arg), mrq->data->bytes_xfered);
       }
       else                    // Default Speed Mode
       {
           MSG(MSG_LVL_INFO, "LBA : 0x%08X,  Len : %d bytes\n", (mrq->cmd->arg) >> 9, mrq->data->bytes_xfered);
       }
    }

    // Save current finished command
    memcpy(&(host->pre_cmd), mrq->cmd, sizeof(struct mmc_command));


    host->mrq = NULL;
    
    /*
     * MMC layer might call back into the driver so first unlock.
     */
    //spin_unlock(&host->lock);
    mmc_request_done(host->mmc, mrq);
    //spin_lock(&host->lock);
}

static void sdhci_request(struct mmc_host *mmc, struct mmc_request *mrq)
{
    struct sdhci_host *host = mmc_priv(mmc);
    struct mmc_command *cmd;

    /*
     * Disable tasklets to avoid a deadlock.
     */

    //spin_lock_bh(&host->lock);
    if(down_interruptible(&host->msdc_sema))
    {
        MSG(MSG_LVL_INFO, "Can not get Sema...\m");
        return;
    }

    cmd = mrq->cmd;

    host->mrq = mrq; 

    if (cmd->data)
    {        
        if (host->hispeedcard) // High Speed Mode
        {
            MSG(MSG_LVL_INFO, "(H)Request : cmd (%d), arg (0x%x), cmd->data = 0x%08x, LBA : 0x%X,  Len : 0x%X\n", cmd->opcode, cmd->arg, (unsigned int)(cmd->data), (cmd->arg), (unsigned int)(cmd->data->blksz * cmd->data->blocks));
        }
        else                    // Default Speed Mode
        {
            MSG(MSG_LVL_INFO, "(N)Request : cmd (%d), arg (0x%x), cmd->data = 0x%08x, LBA : 0x%X,  Len : 0x%X\n", cmd->opcode, cmd->arg, (unsigned int)(cmd->data), (cmd->arg) >> 9, (unsigned int)(cmd->data->blksz * cmd->data->blocks));
        }        
    }    
    
    MSG(MSG_LVL_INFO, "Request : cmd (%d), arg (0x%x)\n", cmd->opcode, cmd->arg);
    /*
     * Check that there is actually a card in the slot.
     */
    if (!(host->flags & SDHCI_FCARD_PRESENT) || (host->forceRemove)) 
    {
      MSG(MSG_LVL_INFO, "Check there is acturally a card\n");
      cmd->error = -ENOMEDIUM;
      goto done;
    }

	cmd->error = 0;

    
    sdhci_send_command(host, cmd);
    
    /*
     * If this is a data transfer the request
     * will be finished after the data has
     * transfered.
     */
    if (cmd->data && (cmd->error == 0)) 
    {     
        // Perform PIO mode access.

		MSG(MSG_LVL_INFO, "Ending data transfer (%d bytes)\n", cmd->data->bytes_xfered);

        //spin_unlock_bh(&host->lock);
        up(&host->msdc_sema);
		sdhci_request_end(host, host->mrq);

        return;
    }

done:

    // Reset FCI if error
    if (cmd->error)
    {
        MSG(MSG_LVL_INFO, "Request Error\n");
    }
  
    MSG(MSG_LVL_INFO, "Request with No Data return\n");

	//spin_unlock_bh(&host->lock);
	up(&host->msdc_sema);
    sdhci_request_end(host, mrq);

}


static void sdhci_reset_variables(struct sdhci_host *host)
{
    // Reset variables
    host->hispeedcard = false;
    host->sd_clk_minus = 0;
    host->forceRemove = 0;
    host->fgPowerSwitchOn = 1;
    
#ifdef USE_SDCR_SEMA
    host->sdcr_sema = 0;
#endif
}

static int sdhci_card_exist(void)
{
    unsigned int u4Val;

	u4Val = (*(volatile unsigned int *)(0xF000D544));

	if((u4Val>>19)&0x1)
	{
	    MSG(MSG_LVL_ERR, "Card not Insert!\n");
		return 0;
	}

	MSG(MSG_LVL_ERR, "Card Insert!\n");
    return 1;

}

static void sdhci_tasklet_card(unsigned long param)
{    
    struct sdhci_host *host = (struct sdhci_host *)param;
    int delay = -1;       
	
    //spin_lock(&host->lock);   
	
    _bMSDCCardExist = sdhci_card_exist();
	
	// If user remove card, clear force remove counter    
	if (!_bMSDCCardExist)
	{        
        host->forceRemoveCnt = 0;    
	}   
	
	if (_bMSDCCardExist && !(host->forceRemove) )     
	{        
	    if (!(host->flags & SDHCI_FCARD_PRESENT))         
		{            
		    // Prevent glitch            
		    mdelay(10);  
			MSG(MSG_LVL_INFO, "Card inserted\n");
			
			host->flags |= SDHCI_FCARD_PRESENT;            
			// Reset variables            
			sdhci_reset_variables(host);
			
			MsdcInit();           
			delay = 0;        
		 }    
	}
	else if (host->flags & SDHCI_FCARD_PRESENT)     
	{        
	    MSG(MSG_LVL_INFO, "Card removed\n");        
		host->flags &= ~SDHCI_FCARD_PRESENT;        

		// Reset variables        
		sdhci_reset_variables(host);        
		if (host->mrq)         
		{            
		    MSG(MSG_LVL_ERR, "%s: Card removed during transfer!\n", mmc_hostname(host->mmc));            
			MsdcInit();            
			host->mrq->cmd->error = -ENOMEDIUM;            
			//tasklet_schedule(&host->finish_tasklet);        
		}         

		delay = 0;    
	}   

	/*  Unlock first since we might get a call back.  */    
	
	//spin_unlock(&host->lock);    

	if (delay != -1)         
	    mmc_detect_change(host->mmc, msecs_to_jiffies(delay));    

	mod_timer(&host->card_detect_timer, jiffies + HZ/3);

}

static void sdhci_free_mmc(struct device *dev)
{
  struct mmc_host *mmc;
  struct sdhci_host *host;

  mmc = dev_get_drvdata(dev);
  if (!mmc)
    return;

  host = mmc_priv(mmc);
  BUG_ON(host == NULL);

  del_timer_sync(&host->card_detect_timer);

  mmc_free_host(mmc);

  dev_set_drvdata(dev, NULL);
}



/*
 * Allocate/free DMA port and buffer
 */

static void __devinit sdhci_request_dma(struct sdhci_host *host)
{
  /*
   * We need to allocate a special buffer in
   * order for ISA to be able to DMA to it.
   */
#ifdef SD_BUF_ALLOC_IN_SATA_BLTIN
  host->dma_buf = (u8 *)sd_dma_buf;
#else
  #ifdef DMA_BUF_BY_KMALLOC
  host->dma_buf = kmalloc(MSDC_DMA_BUFFER_SIZE,
    GFP_NOIO | GFP_DMA | __GFP_REPEAT | __GFP_NOWARN);
  #else
  host->dma_buf = (u8 *)(__get_free_pages(GFP_NOIO | GFP_DMA | __GFP_REPEAT | __GFP_NOWARN, MSDC_DMA_BUFFER_SIZE_ORDER));
  #endif
#endif  //  #ifdef SD_BUF_ALLOC_IN_SATA_BLTIN

  if (!host->dma_buf)
  {
    printk("mmc : can't alloc dma buffer !!\n");
    goto free;
  }

  /*
   * Translate the address to a physical address.
   */
  host->dma_buf_adr = virt_to_phys(host->dma_buf);

  /*
   * FCI DMA must be aligned on a 4 byte basis.
   */
  if ((host->dma_buf_adr & 0x3) != 0)
    goto kfree;

  MSG(MSG_LVL_INFO, "dma_buf = 0x%08x, dma_buf_adr = 0x%08x\n", (unsigned int)(host->dma_buf), (unsigned int)(host->dma_buf_adr));

  return;

kfree:
  /*
   * If we've gotten here then there is some kind of alignment bug
   */
  BUG_ON(1);

  dma_unmap_single(mmc_dev(host->mmc), host->dma_buf_adr,
    MSDC_DMA_BUFFER_SIZE, DMA_BIDIRECTIONAL);
  host->dma_buf_adr = 0;

  kfree(host->dma_buf);
  host->dma_buf = NULL;

free:

  printk(KERN_WARNING DRIVER_NAME ": Unable to allocate DMA."
    "Falling back on FIFO.\n");
}

static void sdhci_release_dma(struct sdhci_host *host)
{
  if (host->dma_buf_adr) 
  {
    dma_unmap_single(mmc_dev(host->mmc), host->dma_buf_adr,
      MSDC_DMA_BUFFER_SIZE, DMA_BIDIRECTIONAL);
  }

#ifdef SD_BUF_ALLOC_IN_SATA_BLTIN  
  // free memory in sata_bltin/sata_bltin.c
  // free_pages((unsigned long)(sd_dma_buf), FCI_DMA_BUFFER_SIZE_ORDER);
#else
  #ifdef DMA_BUF_BY_KMALLOC
  kfree(host->dma_buf);
  #else
  free_pages((unsigned long)(host->dma_buf), MSDC_DMA_BUFFER_SIZE_ORDER);
  #endif
#endif  // #ifdef SD_BUF_ALLOC_IN_SATA_BLTIN

  host->dma_buf = NULL;
  host->dma_buf_adr = 0;
}


/*
 * Allocate/free IRQ.
 */

static int __devinit sdhci_request_irq(struct sdhci_host *host, int irq)
{
    int ret;

    /*
    * Set up tasklets. Must be done before requesting interrupt.
    */

   //tasklet_init(&host->card_tasklet, sdhci_tasklet_card,
    	//(unsigned long)host);
    //tasklet_init(&host->pio_tasklet, sdhci_transfer_pio,
      //(unsigned long)host);
    //tasklet_init(&host->finish_tasklet, sdhci_tasklet_finish,
      //(unsigned long)host);

    /*
    * Allocate interrupt.
    */

    //ret = request_irq(irq, sdhci_irq, IRQF_SHARED, DRIVER_NAME, host);
    //if (ret)
        //return ret;

    //host->irq = irq;

    return 0;
}

static void sdhci_release_irq(struct sdhci_host *host)
{
    if (!host->irq)
        return;

    free_irq(host->irq, host);

    host->irq = 0;

    tasklet_kill(&host->card_tasklet);
    tasklet_kill(&host->pio_tasklet);
    tasklet_kill(&host->finish_tasklet);
}


/*
 * Helper function to reset detection ignore
 */

static void sdhci_reset_ignore(unsigned long data)
{
    struct sdhci_host *host = (struct sdhci_host *)data;

    BUG_ON(host == NULL);

    //spin_lock_bh(&host->lock);

    /*
    * Card status might have changed during the
    * blackout.
    */
    tasklet_schedule(&host->card_tasklet);

    //spin_unlock_bh(&host->lock);
}


/*
 * Add a set_ios hook which is called when the SDHCI driver
 * is called to change parameters such as clock or card width.
 */
static void sdhci_set_ios(struct mmc_host *mmc, struct mmc_ios *ios)
{
    struct sdhci_host *host = mmc_priv(mmc); 

    MSG(MSG_LVL_INFO, "ios->power_mode = 0x%x, ios->clock =%d, ios->bus_width = 0x%x\n", ios->power_mode, ios->clock, ios->bus_width);
  
    //spin_lock_bh(&host->lock);


    host->clk = ios->clock;
    host->bus_width = ios->bus_width;

	MsdcSetIos(mmc, ios);

    //spin_unlock_bh(&host->lock);
}
static int sdhci_get_ro(struct mmc_host *mmc)
{
    return 0;
}

static int sdhci_get_cd(struct mmc_host *mmc)
{
#ifdef CC_SDMMC_SUPPORT
    if(sdhci_card_exist())
	    return 1;

	return 0;
#else
	return 1;
#endif
}


static const struct mmc_host_ops sdhci_ops = {
   .request  = sdhci_request,
   .set_ios  = sdhci_set_ios,
   .get_ro   = sdhci_get_ro,
   .get_cd   = sdhci_get_cd,
};

/*****************************************************************************\
 *                                                                           *
 * Suspend/resume                                                            *
 *                                                                           *
\*****************************************************************************/

#define sdhci_platform_suspend NULL
#define sdhci_platform_resume NULL

/*****************************************************************************\
 *                                                                           *
 * Device probing/removal                                                    *
 *                                                                           *
\*****************************************************************************/

/*
 * Allocate/free MMC structure.
 */

static struct sdhci_msdc_pdata sdhci_msdc_pdata = {
  /* This is currently here to avoid a number of if (host->pdata)
   * checks. Any zero fields to ensure reaonable defaults are picked. */
};

static int __devinit sdhci_probe(struct platform_device *pdev)
{
    struct sdhci_host *host = NULL;
    struct mmc_host *mmc = NULL;
    int ret = 0;

    MSG(MSG_LVL_INFO, "\n");

    // Initialize fci hardware
    MsdcInit();

    /*
     * Allocate MMC structure.
     */
    mmc = mmc_alloc_host(sizeof(struct sdhci_host), &pdev->dev);
    if (!mmc) 
    {
      ret = -ENOMEM;
      goto probe_out;
    }
  
    host = mmc_priv(mmc);
    host->mmc = mmc;
    host->pdev  = pdev;
    host->pdata = pdev->dev.platform_data;
    host->chip_id = CONFIG_CHIP_VERSION;
    host->base = (MSDC_BASE_ADDR + ch*MSDC_CH_OFFSET);
	//host->data_mode = PIO_DATA_MODE;
	host->data_mode = BASIC_DMA_DATA_MODE;
	host->irq = ch?(VECTOR_MSDC2):(VECTOR_MSDC);
    
    // Reset variables
    sdhci_reset_variables(host);
    
    // Default card exists
    if(ch == 0)
    {
        host->flags &= ~SDHCI_FCARD_PRESENT;
    }
	else
	{
        host->flags |= SDHCI_FCARD_PRESENT;
	}
       
    if (!host->pdata) 
    {
      pdev->dev.platform_data = &sdhci_msdc_pdata;
      host->pdata = &sdhci_msdc_pdata;
    }
     
    // Save host pointer for futher useage
    msdc_host = host;

    /*
     * Set host parameters.
     */
    mmc->ops = &sdhci_ops;
    mmc->f_min = 397000;
    mmc->f_max = 48000000;
    
    mmc->ocr_avail = MMC_VDD_30_31 | MMC_VDD_31_32 | MMC_VDD_32_33 | MMC_VDD_33_34 | MMC_VDD_34_35 | MMC_VDD_35_36;
    mmc->caps = MMC_CAP_4_BIT_DATA |MMC_CAP_8_BIT_DATA| MMC_CAP_SD_HIGHSPEED | MMC_CAP_MMC_HIGHSPEED;
    
    spin_lock_init(&host->lock);

    /*
     * Maximum number of segments. Worst case is one sector per segment
     * so this will be 64kB/512.
     */
    mmc->max_hw_segs = 128;
    mmc->max_phys_segs = 128;
    
    /*	 * Maximum request size. Also limited by 64KiB buffer.	 */
    mmc->max_req_size = 128 * SDHC_BLK_SIZE;

    /*	 
     * Maximum segment size. Could be one segment with the maximum number	 
     * of bytes.	 
     */	

    mmc->max_seg_size = mmc->max_req_size;	

    /*	 
     * Maximum block size. We have 12 bits (= 4095) but have to subtract	 
     * space for CRC. So the maximum is 4095 - 4*2 = 4087.	 
     */	

    mmc->max_blk_size = 512;	

    /*	 
     * Maximum block count. There is no real limit so the maximum	 
     * request size will be the only restriction.	 
     */	

    mmc->max_blk_count = mmc->max_req_size;
    
    dev_set_drvdata(&(pdev->dev), mmc);
    
    /*
     * Allocate DMA.
     */
    sdhci_request_dma(host);

    /* Setup irq function */   
    if (sdhci_request_irq(host, MSDC_VECTOR))
    {
      dev_err(&pdev->dev, "failed to request sdhci interrupt.\n");
      ret = -ENOENT;
      goto probe_out;
    }

    if(msdc_isr_en == 1)
    {
	    MsdcRegIsr(host);
	    MsdcSetIsr(INT_SD_ALL, host);
	    init_MUTEX(&host->msdc_sema);
    }
    
    /* We get spurious interrupts even when we have set the IMSK
     * register to ignore everything, so use disable_irq() to make
     * ensure we don't lock the system with un-serviceable requests. */
    // disable_irq(host->irq);
    
    mmc_add_host(mmc);
    
    mmc = dev_get_drvdata(&(pdev->dev));
    
    /*
     * Allow device to initialise itself properly.
     */
    mdelay(5);
    
    printk(KERN_INFO "%s:", mmc_hostname(mmc));
    if (host->chip_id != 0)
      printk(KERN_ERR " %d", (int)host->chip_id);
    printk(KERN_ERR " at 0x%x irq %d buf 0x%x\n", (int)host->base, (int)host->irq, (u32)(host->dma_buf));
    /*
    * Set up timers
    */
    if (ch == 0)
    {
        init_timer(&host->card_detect_timer);
        host->card_detect_timer.data = (unsigned long)host;
        host->card_detect_timer.function = sdhci_tasklet_card;
        host->card_detect_timer.expires = jiffies + 2 * HZ;
        add_timer(&host->card_detect_timer);
    }
	
    return 0;
    
probe_out:
    return ret;
}


/*****************************************************************************\
 *                                                                           *
 * Driver init/exit                                                          *
 *                                                                           *
\*****************************************************************************/

/*
 * Release all resources for the host.
 */

static void __devexit sdhci_shutdown(struct device *dev)
{
  struct mmc_host *mmc = dev_get_drvdata(dev);
  struct sdhci_host *host;

  if (!mmc)
    return;

  host = mmc_priv(mmc);

  mmc_remove_host(mmc);

  sdhci_release_dma(host);

  sdhci_release_irq(host);

  sdhci_free_mmc(dev);
}


static int __devexit sdhci_remove(struct platform_device *dev)
{
   MSG(MSG_LVL_INFO, "\n");
   
  sdhci_shutdown(&dev->dev);

  return 0;
}


static struct platform_device *sdhci_device;

static struct platform_driver sdhci_driver = {
    .probe    = sdhci_probe,
    .remove    = __devexit_p(sdhci_remove),

    .suspend  = sdhci_platform_suspend,
    .resume    = sdhci_platform_resume,
    .driver    = {
        .name  = DRIVER_NAME,		
        .owner = THIS_MODULE,
  },
};


static int __init sdhci_drv_init(void)
{
    int result;
    
    MSG(MSG_LVL_INFO, "initialize..##.\n");

    result = platform_driver_register(&sdhci_driver);
     
    if (result < 0)
    {
        MSG(MSG_LVL_ERR, "platform_driver_register failed !!\n");
        return result;
    }

    sdhci_device = platform_device_alloc(DRIVER_NAME, -1);

    if (!sdhci_device) 
    {
        MSG(MSG_LVL_ERR, "platform_device_alloc failed !!\n");
        platform_driver_unregister(&sdhci_driver);
        return -ENOMEM;
    }

    sdhci_device->dev.coherent_dma_mask = MTK_MSDC_DMA_MASK;
    sdhci_device->dev.dma_mask = &sdhci_device->dev.coherent_dma_mask;

    result = platform_device_add(sdhci_device);
     
    if (result) 
    {
         MSG(MSG_LVL_ERR, "platform_device_add failed !!\n");
         platform_device_put(sdhci_device);
         platform_driver_unregister(&sdhci_driver);
         return result;
    }

    MSG(MSG_LVL_ERR, "initialize finish..\n");

    return 0;
}

static void __exit sdhci_drv_exit(void)
{
    MSG(MSG_LVL_INFO, "\n");

    platform_device_unregister(sdhci_device);

    platform_driver_unregister(&sdhci_driver);
}


module_init(sdhci_drv_init);
module_exit(sdhci_drv_exit);

MODULE_AUTHOR("Shunli Wang");
MODULE_DESCRIPTION("MTK Secure Digital Host Controller Interface driver");
MODULE_VERSION(DRIVER_VERSION);
MODULE_LICENSE("GPL");



