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
* [File]			msdc_host_hw.h
* [Version]			v1.0
* [Revision Date]	2011-05-04
* [Author]			Shunli Wang, shunli.wang@mediatek.inc, 82896, 2011-05-04
* [Description]
*	MSDC Driver Header File
* [Copyright]
*	Copyright (C) 2011 MediaTek Incorporation. All Rights Reserved.
******************************************************************************/

#ifndef _MSDC_HOST_HW_H_
#define _MSDC_HOST_HW_H_

#include "msdc_type.h"

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------
#define CONFIG_CHIP_VERSION       (unsigned int) (5396)

#define MSDC_BASE_ADDR            (unsigned int) (0xF0012000)
#define MSDC_VECTOR               (unsigned int) (28)

//---------------------------------------------------------------------------
// MSDC Register definitions
//---------------------------------------------------------------------------
#define MSDC_CH_NUM               (unsigned int) (2)
#define MSDC_CH_OFFSET            (unsigned int) (0x0005B000)

#define MSDC_CFG              (MSDC_BASE_ADDR + (ch * MSDC_CH_OFFSET) + 0x00)
#define MSDC_IOCON            (MSDC_BASE_ADDR + (ch * MSDC_CH_OFFSET) + 0x04)
#define MSDC_PS               (MSDC_BASE_ADDR + (ch * MSDC_CH_OFFSET) + 0x08)
#define MSDC_INT              (MSDC_BASE_ADDR + (ch * MSDC_CH_OFFSET) + 0x0C)
#define MSDC_INTEN            (MSDC_BASE_ADDR + (ch * MSDC_CH_OFFSET) + 0x10)
#define MSDC_FIFOCS           (MSDC_BASE_ADDR + (ch * MSDC_CH_OFFSET) + 0x14)
#define MSDC_TXDATA           (MSDC_BASE_ADDR + (ch * MSDC_CH_OFFSET) + 0x18)
#define MSDC_RXDATA           (MSDC_BASE_ADDR + (ch * MSDC_CH_OFFSET) + 0x1C)
#define SDC_CFG               (MSDC_BASE_ADDR + (ch * MSDC_CH_OFFSET) + 0x30)
#define SDC_CMD               (MSDC_BASE_ADDR + (ch * MSDC_CH_OFFSET) + 0x34)
#define SDC_ARG               (MSDC_BASE_ADDR + (ch * MSDC_CH_OFFSET) + 0x38)
#define SDC_STS               (MSDC_BASE_ADDR + (ch * MSDC_CH_OFFSET) + 0x3C)
#define SDC_RESP0             (MSDC_BASE_ADDR + (ch * MSDC_CH_OFFSET) + 0x40)
#define SDC_RESP1             (MSDC_BASE_ADDR + (ch * MSDC_CH_OFFSET) + 0x44)
#define SDC_RESP2             (MSDC_BASE_ADDR + (ch * MSDC_CH_OFFSET) + 0x48)
#define SDC_RESP3             (MSDC_BASE_ADDR + (ch * MSDC_CH_OFFSET) + 0x4C)
#define SDC_BLK_NUM           (MSDC_BASE_ADDR + (ch * MSDC_CH_OFFSET) + 0x50)
#define SDC_CSTS              (MSDC_BASE_ADDR + (ch * MSDC_CH_OFFSET) + 0x58)
#define SDC_CSTS_EN           (MSDC_BASE_ADDR + (ch * MSDC_CH_OFFSET) + 0x5C)
#define SDC_DATCRC_STS        (MSDC_BASE_ADDR + (ch * MSDC_CH_OFFSET) + 0x60)
#define EMMC_CFG0             (MSDC_BASE_ADDR + (ch * MSDC_CH_OFFSET) + 0x70)
#define EMMC_CFG1             (MSDC_BASE_ADDR + (ch * MSDC_CH_OFFSET) + 0x74)
#define EMMC_STS              (MSDC_BASE_ADDR + (ch * MSDC_CH_OFFSET) + 0x78)
#define EMMC_IOCON            (MSDC_BASE_ADDR + (ch * MSDC_CH_OFFSET) + 0x7C)
#define ACMD_RESP             (MSDC_BASE_ADDR + (ch * MSDC_CH_OFFSET) + 0x80)
#define ACMD19_TRG            (MSDC_BASE_ADDR + (ch * MSDC_CH_OFFSET) + 0x84)
#define ACMD19_STS            (MSDC_BASE_ADDR + (ch * MSDC_CH_OFFSET) + 0x88)
#define DMA_SA                (MSDC_BASE_ADDR + (ch * MSDC_CH_OFFSET) + 0x90)
#define DMA_CA                (MSDC_BASE_ADDR + (ch * MSDC_CH_OFFSET) + 0x94)
#define DMA_CTRL              (MSDC_BASE_ADDR + (ch * MSDC_CH_OFFSET) + 0x98)
#define DMA_CFG               (MSDC_BASE_ADDR + (ch * MSDC_CH_OFFSET) + 0x9C)
#define DBG_SEL               (MSDC_BASE_ADDR + (ch * MSDC_CH_OFFSET) + 0xA0)
#define DBG_OUT               (MSDC_BASE_ADDR + (ch * MSDC_CH_OFFSET) + 0xA4)
#define PATCH_BIT             (MSDC_BASE_ADDR + (ch * MSDC_CH_OFFSET) + 0xB0)
#define SD20_PAD_CTL0         (MSDC_BASE_ADDR + (ch * MSDC_CH_OFFSET) + 0xE0)
#define SD20_PAD_CTL1         (MSDC_BASE_ADDR + (ch * MSDC_CH_OFFSET) + 0xE4)
#define SD20_PAD_CTL2         (MSDC_BASE_ADDR + (ch * MSDC_CH_OFFSET) + 0xE8)
#define GPIO_DBG_OUT          (MSDC_BASE_ADDR + (ch * MSDC_CH_OFFSET) + 0xEB)
#define PAD_TUNE              (MSDC_BASE_ADDR + (ch * MSDC_CH_OFFSET) + 0xEC)
#define HW_DBG                (MSDC_BASE_ADDR + (ch * MSDC_CH_OFFSET) + 0xF8)
#define VERSION               (MSDC_BASE_ADDR + (ch * MSDC_CH_OFFSET) + 0xFC)

/* MSDC_CFG */
#define MSDC_CFG_SD                     (((unsigned int)0x01) << 0)
#define MSDC_CFG_CK_EN                  (((unsigned int)0x01) << 1)
#define MSDC_CFG_RST                    (((unsigned int)0x01) << 2)
#define MSDC_CFG_PIO_MODE               (((unsigned int)0x01) << 3)
#define MSDC_CFG_BUS_VOL_START          (((unsigned int)0x01) << 5)
#define MSDC_CFG_BUS_VOL_PASS           (((unsigned int)0x01) << 6)
#define MSDC_CFG_CARD_CK_STABLE         (((unsigned int)0x01) << 7)
#define MSDC_CFG_CK_DIV_SHIFT           (8)
#define MSDC_CFG_CK_MODE_DIVIDER        (((unsigned int)0x00) << 16)
#define MSDC_CFG_CK_MODE_DIRECT         (((unsigned int)0x01) << 16)
#define MSDC_CFG_CK_MODE_DDR            (((unsigned int)0x02) << 16)

/* MSDC_IOCON */
#define MSDC_IOCON_RISC_SIZE_MASK       (((unsigned int)0x03) << 24)
#define MSDC_IOCON_RISC_SIZE_BYTE       (((unsigned int)0x00) << 24)
#define MSDC_IOCON_RISC_SIZE_WORD       (((unsigned int)0x01) << 24)
#define MSDC_IOCON_RISC_SIZE_DWRD       (((unsigned int)0x02) << 24)

/* MSDC_INT */
#define INT_MMC_IRQ                     (((unsigned int)0x01) << 0)           
#define INT_MSDC_CDSC                   (((unsigned int)0x01) << 1) 
#define INT_SD_AUTOCMD_RDY              (((unsigned int)0x01) << 3) 
#define INT_SD_AUTOCMD_TO               (((unsigned int)0x01) << 4) 
#define INT_SD_AUTOCMD_RESP_CRCERR      (((unsigned int)0x01) << 5) 
#define INT_DMA_Q_EMPTY                 (((unsigned int)0x01) << 6) 
#define INT_SD_SDIOIRQ                  (((unsigned int)0x01) << 7) 
#define INT_SD_CMDRDY                   (((unsigned int)0x01) << 8) 
#define INT_SD_CMDTO                    (((unsigned int)0x01) << 9) 
#define INT_SD_RESP_CRCERR              (((unsigned int)0x01) << 10)
#define INT_SD_CSTA                     (((unsigned int)0x01) << 11)
#define INT_SD_XFER_COMPLETE            (((unsigned int)0x01) << 12)
#define INT_DMA_XFER_DONE               (((unsigned int)0x01) << 13)
#define INT_SD_DATTO                    (((unsigned int)0x01) << 14)
#define INT_SD_DATA_CRCERR              (((unsigned int)0x01) << 15)
#define INT_MS_RDY                      (((unsigned int)0x01) << 24)
#define INT_MS_SIF                      (((unsigned int)0x01) << 25)
#define INT_MS_TOER                     (((unsigned int)0x01) << 26)
#define INT_MS_CRCERR                   (((unsigned int)0x01) << 27)
#define INT_MS_CED                      (((unsigned int)0x01) << 28)
#define INT_MS_ERR                      (((unsigned int)0x01) << 29)
#define INT_MS_BREQ                     (((unsigned int)0x01) << 30)
#define INT_CMDNK                       (((unsigned int)0x01) << 31)

#define INT_SD_ALL                      (INT_SD_DATA_CRCERR | INT_SD_DATTO | INT_DMA_XFER_DONE | INT_SD_XFER_COMPLETE | \
                                         INT_SD_RESP_CRCERR | INT_SD_CMDTO | INT_SD_CMDRDY)

/* SDC_CFG */
#define SDC_CFG_BW_SHIFT                (16)         
#define SDC_CFG_BW_MASK                 (((unsigned int)0x03) << 16) 
#define SDC_CFG_SDIO                    (((unsigned int)0x01) << 19) 
#define SDC_CFG_INTAT_BLK_GAP           (((unsigned int)0x01) << 21) 
#define SDC_CFG_DTOC_SHIFT              (24)         

/* SDC_STS */                                        
#define SDC_STS_SDCBUSY                 (((unsigned int)0x01) << 0x0)  
#define SDC_STS_CMDBUSY                 (((unsigned int)0x01) << 0x1)  
                                 
/* SDC_CMD */                                        
#define SDC_CMD_BREAK                   (((unsigned int)0x01) << 6)  

/* Response Type */                                                     
#define SDC_CMD_RSPTYPE_NO              (((unsigned int)0x00) << 7)  
#define SDC_CMD_RSPTYPE_R1              (((unsigned int)0x01) << 7)  
#define SDC_CMD_RSPTYPE_R2              (((unsigned int)0x02) << 7)  
#define SDC_CMD_RSPTYPE_R3              (((unsigned int)0x03) << 7)  
#define SDC_CMD_RSPTYPE_R4              (((unsigned int)0x04) << 7)  
#define SDC_CMD_RSPTYPE_R5              (((unsigned int)0x01) << 7)  
#define SDC_CMD_RSPTYPE_R6              (((unsigned int)0x01) << 7)  
#define SDC_CMD_RSPTYPE_R7              (((unsigned int)0x01) << 7)  
#define SDC_CMD_RSPTYPE_R1B             (((unsigned int)0x07) << 7)  

/* Data Type */                                                     
#define DTYPE_NONE                      (((unsigned int)0x00) << 11) 
#define DTYPE_SINGLE_BLK                (((unsigned int)0x01) << 11) 
#define DTYPE_MULTI_BLK                 (((unsigned int)0x02) << 11) 
#define DTYPE_STREAM                    (((unsigned int)0x03) << 11) 
                                                     
#define SDC_CMD_READ                    (((unsigned int)0x00) << 13) 
#define SDC_CMD_WRITE                   (((unsigned int)0x01) << 13) 
                                                     
#define SDC_CMD_STOP                    (((unsigned int)0x01) << 14) 
                                                     
#define SDC_CMD_GO_IRQ                  (((unsigned int)0x01) << 15) 
                                                     
#define SDC_CMD_LEN_SHIFT               (16)         
                                                     
#define SDC_CMD_AUTO_CMD_NONE           (((unsigned int)0x0) << 28)  
#define SDC_CMD_AUTO_CMD12              (((unsigned int)0x1) << 28)  
#define SDC_CMD_ATUO_CMD23              (((unsigned int)0x2) << 28)  
#define SDC_CMD_AUTO_CMD19              (((unsigned int)0x3) << 28)  
                                                     
#define SDC_CMD_VOL_SWITCH              (((unsigned int)0x1) << 30)  
                                                     
/* MSDC_FIFOCS */                                    
#define MSDC_FIFOCS_FIFOCLR             (((unsigned int)0x1) << 31)  
#define MSDC_FIFO_LEN                   (128)        
#define MSDC_FIFOCS_TXFIFOCNT_SHIFT     (16)         
#define MSDC_FIFOCS_TXFIFOCNT_MASK      (0x00FF0000) 
#define MSDC_FIFOCS_RXFIFOCNT_SHIFT     (0)          
#define MSDC_FIFOCS_RXFIFOCNT_MASK      (0x000000FF) 

/* DMA_CTRL */
#define DMA_CTRL_BST_SHIFT              (12)
#define DMA_CTRL_BST_8                  (3 << DMA_CTRL_BST_SHIFT)
#define DMA_CTRL_BST_16                 (4 << DMA_CTRL_BST_SHIFT)
#define DMA_CTRL_BST_32                 (5 << DMA_CTRL_BST_SHIFT)
#define DMA_CTRL_BST_64                 (6 << DMA_CTRL_BST_SHIFT)
#define DMA_CTRL_START                  (((unsigned int)0x01) << 0)
#define DMA_CTRL_STOP                   (((unsigned int)0x01) << 1)
#define DMA_CTRL_RESUME                 (((unsigned int)0x01) << 2)
#define DMA_CTRL_LAST_BUF               (((unsigned int)0x01) << 10)
#define DMA_CTRL_XFER_SIZE_SHIFT        (16)
#define DMA_CTRL_DESC_MODE              (((unsigned int)0x01) << 8)

/* DMA_CFG */
#define DMA_CFG_DMA_STATUS              (((unsigned int)0x01) << 0)
#define DMA_CFG_CHKSUM                  (((unsigned int)0x01) << 1)
#define DMA_CFG_BD_CS_ERR               (((unsigned int)0x01) << 4)
#define DMA_CFG_GPD_CS_ERR              (((unsigned int)0x01) << 5)

/* EMMC_CFG0 */
#define BTSUP                           (((unsigned int)0x01) << 15)
#define BTWDLY_0x32                     (((unsigned int)0x00) << 12)
#define BTWDLY_1x32                     (((unsigned int)0x01) << 12)
#define BTWDLY_2x32                     (((unsigned int)0x02) << 12)
#define BTWDLY_7x32                     (((unsigned int)0x07) << 12)
#define BTACHKDIS                       (((unsigned int)0x01) << 3)
#define BTMOD_0                         (((unsigned int)0x00) << 2)
#define BTMOD_1                         (((unsigned int)0x01) << 2)
#define BTSTOP                          (((unsigned int)0x01) << 1)
#define BTSTART                         (((unsigned int)0x01) << 0)

/* EMMC_CFG1 */
#define BTATOC_SHIFT                    (20)
#define BTDTOC_SHIFT                    (0)

/* EMMC_STS */
#define BTDRCV                          (((unsigned int)0x01) << 6)
#define BTARCV                          (((unsigned int)0x01) << 5)
#define BTSTS                           (((unsigned int)0x01) << 4)
#define BTATO                           (((unsigned int)0x01) << 3)
#define BTDTO                           (((unsigned int)0x01) << 2)
#define BTAERR                          (((unsigned int)0x01) << 1)
#define BTDERR                          (((unsigned int)0x01) << 0)

/* PATCH_BIT */
#define R1B_DELAY_CYCLE                 (((unsigned int)0x0F) << 18)

/* 8bit Mode */
#define SUPPORT_8_BIT					(1)

/* Host OCR */
#define HOST_OCR						(0x00FF8000)
#define HOST_MMC_OCR                    (0x403C0000)
#define HOST_HCS						(1)
#define HOST_VHS						(0x1)

/* Transaction Type */
#define NULL_DATA_MODE				    (0)
#define PIO_DATA_MODE				    (1)
#define BASIC_DMA_DATA_MODE			    (2)
#define DESC_DMA_DATA_MODE			    (3)
#define ENHANCED_DMA_DATA_MODE		    (4)

/* DMA Burst */
#define DMA_BST_8						(3)
#define DMA_BST_16						(4)
#define DMA_BST_32						(5)
#define DMA_BST_64						(6)

/* DMA Data Structure */
#define BASIC_DMA_MAX_LEN				(0xFFFF)
#define GPD_MAX_LEN						(0xFFFF)
#define BD_MAX_LEN						(0xFFFF)

struct __msdc_gpd_s__
{
	unsigned int hwo	:1;
	unsigned int bdp	:1;
	unsigned int resv0	:6;
	unsigned int chksum	:8;
	unsigned int intr	:1;
	unsigned int resv1	:15;

	void *pNext;
	void *pBuff;

	unsigned int buffLen:16;
	unsigned int extLen	:8;
	unsigned int resv3	:8;

	// Enhanced DMA fields
	unsigned int arg;
	unsigned int blkNum;
	unsigned int cmd;
};
typedef struct __msdc_gpd_s__ msdc_gpd_t;

struct __msdc_bd_s__
{	
	unsigned int eol	:1;
	unsigned int resv0	:7;
	unsigned int chksum	:8;
	unsigned int resv1	:1;
	unsigned int blkPad	:1;
	unsigned int dwPad	:1;			// DWORD is 4 bytes
	unsigned int resv2	:13;

	void *pNext;
	void *pBuff;

	unsigned int buffLen:16;
	unsigned int resv3	:16;
};
typedef struct __msdc_bd_s__ msdc_bd_t;

#endif // #ifndef _MSDC_HOST_HW_H_
