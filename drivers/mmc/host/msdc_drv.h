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
* [File]			msdc_drv.h
* [Version]			v1.0
* [Revision Date]	2011-05-04
* [Author]			Shunli Wang, shunli.wang@mediatek.inc, 82896, 2011-05-04
* [Description]
*	MSDC Driver Header File
* [Copyright]
*	Copyright (C) 2011 MediaTek Incorporation. All Rights Reserved.
******************************************************************************/

#ifndef _MSDC_DRV_H_
#define _MSDC_DRV_H_

#include "msdc_type.h"
#define CC_SDMMC_SUPPORT

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------
#define SDHC_BLK_SIZE		  (512)
#define MMC_DEF_RCA           0x0001
#define SD_CARD_DETECT_GPIO   (((*(volatile unsigned int *)(0xF000D544))>>20)&(0x1));
#define MSDC_CLK                      54, 48, 43, 40, 36, 37, 24, 18, 13, 0

//---------------------------------------------------------------------------
// LOG Macro Definitions
//---------------------------------------------------------------------------
#define MSG_LVL_OFF						(0)
#define MSG_LVL_FATAL					(1)
#define MSG_LVL_ERR						(2)
#define MSG_LVL_WARN					(3)
#define MSG_LVL_TITLE					(4)
#define MSG_LVL_INFO					(5)
#define MSG_LVL_CMD						(6)
#define MSG_LVL_DBG						(7)
#define MSG_LVL_TRACE					(8)
#define PRINTF_DEBUG_ENABLE

#ifdef PRINTF_DEBUG_ENABLE
static unsigned int msdc_log_en = MSG_LVL_OFF;
#define MSG(n, f, x...) if ((n) < msdc_log_en){ printk(KERN_ERR ": <%s> " f, __func__,## x); }
#else
#define MSG(lvl, formatStr, args...)
#endif

//---------------------------------------------------------------------------
// Wait Time Macro definitions
//---------------------------------------------------------------------------
#define MSDC_RST_TIMEOUT_LIMIT_COUNT                        5         
#define MSDC_CLK_TIMEOUT_LIMIT_COUNT                        5 
#define MSDC_FIFOCLR_TIMEOUT_LIMIT_COUNT                    5   

#define MSDC_WAIT_SDC_BUS_TIMEOUT_LIMIT_COUNT               400                 
#define MSDC_WAIT_CMD_TIMEOUT_LIMIT_COUNT                   200                     
#define MSDC_WAIT_DATA_TIMEOUT_LIMIT_COUNT                  200              
#define MSDC_WAIT_DATA_COMPLETE_TIMEOUT_LIMIT_COUNT         200           

//---------------------------------------------------------------------------
// MSDC Interrupt Macro definitions
//---------------------------------------------------------------------------
#define IRQ_EN_REG                                          (0xF0008034)
#define MSDC0_EN_SHIFT                                      (28)
#define MISC2_EN_SHIFT                                      (30)
#define MISC2_EN_REG                                        (0xF0008084)
#define MSDC1_EN_SHIFT                                      (4)

//---------------------------------------------------------------------------
// I/O Macro definitions
//---------------------------------------------------------------------------
#define MSDC_WRITE32(addr, value)      (*(volatile unsigned int *)(addr)) = (value)
#define MSDC_READ32(addr)              (*(volatile unsigned int *)(addr))

#define MSDC_SETBIT(addr, dBit)        MSDC_WRITE32(addr, MSDC_READ32(addr) | (dBit))
#define MSDC_CLRBIT(addr, dBit)        MSDC_WRITE32(addr, MSDC_READ32(addr) & (~(dBit)))

//---------------------------------------------------------------------------
// Command & Response Macro definitions
//---------------------------------------------------------------------------
#define RESPONSE_NO			          (0)
#define RESPONSE_R1			          (1)
#define RESPONSE_R2			          (2)
#define RESPONSE_R3			          (3)
#define RESPONSE_R4			          (4)
#define RESPONSE_R5			          (5)
#define RESPONSE_R6			          (6)
#define RESPONSE_R7			          (7)
#define RESPONSE_R1B		          (8)

//---------------------------------------------------------------------------
// Error Flag Macro definitions
//---------------------------------------------------------------------------
#define MSDC_SUCCESS                  (int)(0)
#define MSDC_FAILED                   (int)(-1)

#define ERR_HOST_BUSY                 (0x01 << 0)

#define ERR_CMD_FAILED                (0x01 << 1)
#define ERR_CMD_NO_INT                (0x01 << 2)
#define ERR_CMD_RESP_TIMEOUT          (0x01 << 3)
#define ERR_CMD_RESP_CRCERR           (0x01 << 4)
#define ERR_AUTOCMD_RESP_CRCERR       (0x01 << 5)
#define ERR_AUTOCMD_RESP_TIMEOUT      (0x01 << 6)
#define ERR_CMD_UNEXPECT_INT          (0x01 << 7)

#define ERR_DAT_FAILED                (0x01 << 8)
#define ERR_DAT_NO_INT                (0x01 << 9)
#define ERR_DAT_CRCERR                (0x01 << 10)   
#define ERR_DAT_TIMEOUT               (0x01 << 11)
#define ERR_DAT_UNEXPECT_INT          (0x01 << 12) 

#define ERR_DMA_STATUS_FAILED         (0x01 << 16)

//---------------------------------------------------------------------------
// DMA Macro definitions
//---------------------------------------------------------------------------
#if defined(CONFIG_MT53XX_MAP_CHANNELB_DRAM) || defined(CONFIG_MT53XX_USE_CHANNELB_DRAM)
#define MTK_MSDC_DMA_MASK 0x1fffffff
#else
#define MTK_MSDC_DMA_MASK 0xffffffff
#endif


/**
* Host definition
*/
struct sdhci_msdc_pdata {
   unsigned int    no_wprotect : 1;
   unsigned int    no_detect : 1;
   unsigned int    wprotect_invert : 1;
   unsigned int    detect_invert : 1;   /* set => detect active high. */
   unsigned int    use_dma : 1;

   unsigned int    gpio_detect;
   unsigned int    gpio_wprotect;
   unsigned long   ocr_avail;
   void            (*set_power)(unsigned char power_mode,
                                unsigned short vdd);
 };

struct sdhci_host
{
	struct mmc_host *mmc;		/* MMC structure */
	struct platform_device	*pdev;
	void *pdata;	/* Platform specific data, device core doesn't touch it */
	
	spinlock_t lock;		/* Mutex */

	int flags;		/* Driver states */

#ifdef CC_SDMMC_SUPPORT
	int maxWorkClk;
	int maxWorkClkIndex;
#endif

#define SDHCI_FCARD_PRESENT	(1<<0)		/* Card is present */

	struct mmc_request *mrq;		/* Current request */
	struct mmc_command cur_cmd;
	struct mmc_command pre_cmd;

	struct scatterlist *cur_sg;		/* Current SG entry */
	unsigned int num_sg;		/* Number of entries left */
	unsigned int offset;		/* Offset into current entry */
	unsigned int remain;		/* Data left in curren entry */

	msdc_gpd_t *gpd;				  /* pointer to gpd array */		
	msdc_bd_t  *bd; 				  /* pointer to bd array */ 	   
	dma_addr_t gpd_addr; 		/* the physical address of gpd array */ 	   
	dma_addr_t bd_addr;			/* the physical address of bd array */
	
	unsigned char clk;		/* Current clock speed */
	unsigned char bus_width;	/* Current bus width */
	int hispeedcard;          /* HiSpeed card or not */
 
	int chip_id;	/* ID of controller */
	int base;		/* I/O port base */
	int irq;		    /* Interrupt irq number*/	
	int dma;		/* DMA channel */
	int data_mode;    /* data transfer mode */
	struct semaphore msdc_sema;       /* semaphore */

#ifdef CC_SDMMC_SUPPORT
	struct tasklet_struct card_tasklet;	/* Tasklet structures */
	struct tasklet_struct finish_tasklet;	
	struct timer_list card_detect_timer;	/* card detection timer */
#endif
};


//---------------------------------------------------------------------------
// export function declaration
//---------------------------------------------------------------------------

#endif // #ifndef _MSDC_DRV_H_
