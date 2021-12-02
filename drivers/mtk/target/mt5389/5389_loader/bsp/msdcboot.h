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
 * $RCSfile: msdcboot.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file msdcboot.h
 *  msdcboot.h The MSDC register constant macros
 */
 
#ifndef _MSDCBOOT_H_
#define _MSDCBOOT_H_

//---------------------------------------------------------------------------
// MSDC Register definitions
//---------------------------------------------------------------------------
#define MSDC_BASE_ADDR            (unsigned int) (0xF0012000)

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
#define MSDC_CFG_SD                     (((UINT32)0x01) << 0)
#define MSDC_CFG_CK_EN                  (((UINT32)0x01) << 1)
#define MSDC_CFG_RST                    (((UINT32)0x01) << 2)
#define MSDC_CFG_PIO_MODE               (((UINT32)0x01) << 3)
#define MSDC_CFG_BUS_VOL_START          (((UINT32)0x01) << 5)
#define MSDC_CFG_BUS_VOL_PASS           (((UINT32)0x01) << 6)
#define MSDC_CFG_CARD_CK_STABLE         (((UINT32)0x01) << 7)
#define MSDC_CFG_CK_DIV_SHIFT           (8)
#define MSDC_CFG_CK_MODE_DIVIDER        (((UINT32)0x00) << 16)
#define MSDC_CFG_CK_MODE_DIRECT         (((UINT32)0x01) << 16)
#define MSDC_CFG_CK_MODE_DDR            (((UINT32)0x02) << 16)

/* MSDC_IOCON */
#define MSDC_IOCON_RISC_SIZE_MASK       (((UINT32)0x03) << 24)
#define MSDC_IOCON_RISC_SIZE_BYTE       (((UINT32)0x00) << 24)
#define MSDC_IOCON_RISC_SIZE_WORD       (((UINT32)0x01) << 24)
#define MSDC_IOCON_RISC_SIZE_DWRD       (((UINT32)0x02) << 24)

/* MSDC_INT */
#define INT_MMC_IRQ                     (((UINT32)0x01) << 0)           
#define INT_MSDC_CDSC                   (((UINT32)0x01) << 1) 
#define INT_SD_AUTOCMD_RDY              (((UINT32)0x01) << 3) 
#define INT_SD_AUTOCMD_TO               (((UINT32)0x01) << 4) 
#define INT_SD_AUTOCMD_RESP_CRCERR      (((UINT32)0x01) << 5) 
#define INT_DMA_Q_EMPTY                 (((UINT32)0x01) << 6) 
#define INT_SD_SDIOIRQ                  (((UINT32)0x01) << 7) 
#define INT_SD_CMDRDY                   (((UINT32)0x01) << 8) 
#define INT_SD_CMDTO                    (((UINT32)0x01) << 9) 
#define INT_SD_RESP_CRCERR              (((UINT32)0x01) << 10)
#define INT_SD_CSTA                     (((UINT32)0x01) << 11)
#define INT_SD_XFER_COMPLETE            (((UINT32)0x01) << 12)
#define INT_DMA_XFER_DONE               (((UINT32)0x01) << 13)
#define INT_SD_DATTO                    (((UINT32)0x01) << 14)
#define INT_SD_DATA_CRCERR              (((UINT32)0x01) << 15)
#define INT_MS_RDY                      (((UINT32)0x01) << 24)
#define INT_MS_SIF                      (((UINT32)0x01) << 25)
#define INT_MS_TOER                     (((UINT32)0x01) << 26)
#define INT_MS_CRCERR                   (((UINT32)0x01) << 27)
#define INT_MS_CED                      (((UINT32)0x01) << 28)
#define INT_MS_ERR                      (((UINT32)0x01) << 29)
#define INT_MS_BREQ                     (((UINT32)0x01) << 30)
#define INT_CMDNK                       (((UINT32)0x01) << 31)

/* SDC_CFG */
#define SDC_CFG_BW_SHIFT                (16)         
#define SDC_CFG_BW_MASK                 (((UINT32)0x03) << 16) 
#define SDC_CFG_SDIO                    (((UINT32)0x01) << 19) 
#define SDC_CFG_INTAT_BLK_GAP           (((UINT32)0x01) << 21) 
#define SDC_CFG_DTOC_SHIFT              (24)         

/* SDC_STS */                                        
#define SDC_STS_SDCBUSY                 (((UINT32)0x01) << 0x0)  
#define SDC_STS_CMDBUSY                 (((UINT32)0x01) << 0x1)  
                                 
/* SDC_CMD */                                        
#define SDC_CMD_BREAK                   (((UINT32)0x01) << 6)  
                                                     
#define SDC_CMD_RSPTYPE_NO              (((UINT32)0x00) << 7)  
#define SDC_CMD_RSPTYPE_R1              (((UINT32)0x01) << 7)  
#define SDC_CMD_RSPTYPE_R2              (((UINT32)0x02) << 7)  
#define SDC_CMD_RSPTYPE_R3              (((UINT32)0x03) << 7)  
#define SDC_CMD_RSPTYPE_R4              (((UINT32)0x04) << 7)  
#define SDC_CMD_RSPTYPE_R5              (((UINT32)0x01) << 7)  
#define SDC_CMD_RSPTYPE_R6              (((UINT32)0x01) << 7)  
#define SDC_CMD_RSPTYPE_R7              (((UINT32)0x01) << 7)  
#define SDC_CMD_RSPTYPE_R1B             (((UINT32)0x07) << 7)  
                                                     
#define DTYPE_NONE                      (((UINT32)0x00) << 11) 
#define DTYPE_SINGLE_BLK                (((UINT32)0x01) << 11) 
#define DTYPE_MULTI_BLK                 (((UINT32)0x02) << 11) 
#define DTYPE_STREAM                    (((UINT32)0x03) << 11) 
                                                     
#define SDC_CMD_READ                    (((UINT32)0x00) << 13) 
#define SDC_CMD_WRITE                   (((UINT32)0x01) << 13) 
                                                     
#define SDC_CMD_STOP                    (((UINT32)0x01) << 14) 
                                                     
#define SDC_CMD_GO_IRQ                  (((UINT32)0x01) << 15) 
                                                     
#define SDC_CMD_LEN_SHIFT               (16)         
                                                     
#define SDC_CMD_AUTO_CMD_NONE           (((UINT32)0x0) << 28)  
#define SDC_CMD_AUTO_CMD12              (((UINT32)0x1) << 28)  
#define SDC_CMD_ATUO_CMD23              (((UINT32)0x2) << 28)  
#define SDC_CMD_AUTO_CMD19              (((UINT32)0x3) << 28)  
                                                     
#define SDC_CMD_VOL_SWITCH              (((UINT32)0x1) << 30)  
                                                     
/* MSDC_FIFOCS */                                    
#define MSDC_FIFOCS_FIFOCLR             (((UINT32)0x1) << 31)  
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
#define DMA_CTRL_START                  (((UINT32)0x01) << 0)
#define DMA_CTRL_STOP                   (((UINT32)0x01) << 1)
#define DMA_CTRL_RESUME                 (((UINT32)0x01) << 2)
#define DMA_CTRL_LAST_BUF               (((UINT32)0x01) << 10)
#define DMA_CTRL_XFER_SIZE_SHIFT        (16)
#define DMA_CTRL_DESC_MODE              (((UINT32)0x01) << 8)

/* DMA_CFG */
#define DMA_CFG_DMA_STATUS              (((UINT32)0x01) << 0)
#define DMA_CFG_CHKSUM                  (((UINT32)0x01) << 1)
#define DMA_CFG_BD_CS_ERR               (((UINT32)0x01) << 4)
#define DMA_CFG_GPD_CS_ERR              (((UINT32)0x01) << 5)

/* EMMC_CFG0 */
#define BTSUP                           (((UINT32)0x01) << 15)
#define BTWDLY_0x32                     (((UINT32)0x00) << 12)
#define BTWDLY_1x32                     (((UINT32)0x01) << 12)
#define BTWDLY_2x32                     (((UINT32)0x02) << 12)
#define BTWDLY_7x32                     (((UINT32)0x07) << 12)
#define BTACHKDIS                       (((UINT32)0x01) << 3)
#define BTMOD_0                         (((UINT32)0x00) << 2)
#define BTMOD_1                         (((UINT32)0x01) << 2)
#define BTSTOP                          (((UINT32)0x01) << 1)
#define BTSTART                         (((UINT32)0x01) << 0)

/* EMMC_CFG1 */
#define BTATOC_SHIFT                     20
#define BTDTOC_SHIFT                     0

/* EMMC_STS */
#define BTDRCV                          (((UINT32)0x01) << 6)
#define BTARCV                          (((UINT32)0x01) << 5)
#define BTSTS                           (((UINT32)0x01) << 4)
#define BTATO                           (((UINT32)0x01) << 3)
#define BTDTO                           (((UINT32)0x01) << 2)
#define BTAERR                          (((UINT32)0x01) << 1)
#define BTDERR                          (((UINT32)0x01) << 0)

/* PATCH_BIT */
#define R1B_DELAY_CYCLE                 (((UINT32)0x0F) << 18)

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

#define HOST_OCR						(0x00FF8000)
#define HOST_MMC_OCR                    (0x403C0000)
#define HOST_HCS						(1)
#define HOST_VHS						(0x1)

#define INIT_BUS_FREQ					(400 * 1000)
#define BUS_DIV_16_CLK_FREQ 	        (84375 * 10)             // 0.84375Mhz, maximum clock setting is 27/32
#define BUS_DIV_8_CLK_FREQ	  	        (16875 * 100)            // 1.6875Mhz, maximum clock setting is 27/16
#define BUS_DIV_4_CLK_FREQ	  	        (3375 * 1000)            // 3.375Mhz, maximum clock setting is 27/8
#define BUS_DIV_2_CLK_FREQ	  	        (675 * 10 * 1000)        // 6.75 Mhz, maximum clock setting is 27/4
#define BUS_CLK_FREQ					(135 * 100 * 1000)       // 13.5 Mhz, maximum clock setting is 27/2
#define SRC_CLK_FREQ					(27 * 1000 * 1000)       // OSC = 27 Mhz
#define LOWEST_BUS_FREQ					(SRC_CLK_FREQ / (4 * 255))


#define CMD0_RESET_LIMIT	            (3)
#define MMC_CMD1_RETRY_LIMIT            (1500)
#define SDHC_BLK_SIZE		            (512)
#define MMC_DEF_RCA                      0x0001

//---------------------------------------------------------------------------
// Basic definitions
//---------------------------------------------------------------------------

//#define MIN(n, m)						(((n) > (m))?(m):(n))
//#define MAX(n, m)						(((n) > (m))?(n):(m))

#define MSG_LVL_OFF						(0)
#define MSG_LVL_FATAL					(1)
#define MSG_LVL_ERR						(2)
#define MSG_LVL_WARN					(3)
#define MSG_LVL_TITLE					(4)
#define MSG_LVL_INFO					(5)
#define MSG_LVL_CMD						(6)
#define MSG_LVL_DBG						(7)
#define MSG_LVL_TRACE					(8)

//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------

#define MSDC_RST_TIMEOUT_LIMIT_COUNT                        5         
#define MSDC_CLK_TIMEOUT_LIMIT_COUNT                        5 
#define MSDC_FIFOCLR_TIMEOUT_LIMIT_COUNT                    5   

#define MSDC_WAIT_SDC_BUS_TIMEOUT_LIMIT_COUNT               400                 
#define MSDC_WAIT_CMD_TIMEOUT_LIMIT_COUNT                   200                     
#define MSDC_WAIT_DATA_TIMEOUT_LIMIT_COUNT                  5000              
#define MSDC_WAIT_DATA_COMPLETE_TIMEOUT_LIMIT_COUNT         1000              

#define MSDC_WAIT_BTSTS_1_TIMEOUT_LIMIT_COUNT               20                     
#define MSDC_WAIT_BTSTS_0_TIMEOUT_LIMIT_COUNT               1000                   
#define MSDC_WAIT_EMMC_ACK_TIMEOUT_LIMIT_COUNT              60                 
#define MSDC_WAIT_EMMC_DATA_TIMEOUT_LIMIT_COUNT             1100 


//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

#define MSDC_WRITE32(addr, value)      (*(volatile unsigned int *)(addr)) = (value)
#define MSDC_READ32(addr)              (*(volatile unsigned int *)(addr))

#define MSDC_SETBIT(addr, dBit)        MSDC_WRITE32(addr, MSDC_READ32(addr) | (dBit))
#define MSDC_CLRBIT(addr, dBit)        MSDC_WRITE32(addr, MSDC_READ32(addr) & (~(dBit)))
	
#define RESPONSE_NO			(0)
#define RESPONSE_R1			(1)
#define RESPONSE_R2			(2)
#define RESPONSE_R3			(3)
#define RESPONSE_R4			(4)
#define RESPONSE_R5			(5)
#define RESPONSE_R6			(6)
#define RESPONSE_R7			(7)
#define RESPONSE_R1B		        (8)

#define CMD_ERR_SUCCESS               (MSDC_SUCCESS)
#define CMD_ERR_FAILED                (MSDC_FAILED)

#define CMD_ERR_NO_RESP               (0x01 << 0)
#define CMD_ERR_RESP_CRCERR           (0x01 << 1)
#define CMD_ERR_WAIT_CMD_TO           (0x01 << 2)

#define CMD_ERR_DATTO                 (0x01 << 3)
#define CMD_ERR_DATA_CRCERR           (0x01 << 4)
#define CMD_ERR_WAIT_DATATO           (0x01 << 5)
#define CMD_ERR_DATA_FAILED           (0x01 << 6)

#define DMA_BST_8						(3)
#define DMA_BST_16						(4)
#define DMA_BST_32						(5)
#define DMA_BST_64						(6)

#define ACMD41_BUS_WIDTH_1				(0x00)
#define ACMD41_BUS_WIDTH_4				(0x02)
#define ACMD41_BUS_WIDTH_8				(0xFF)	//TODO: UNDEFINED!!

// MMC CMD6 Argument :
// (1) Bus Width Selection :
// Access bits = 0x03 (Write byte), Index = 0xB7 = 183, Value = 0(1bit), 1(4bits), 2(8bits)
#define MMC_CMD6_ARG_1BIT_BUS  0x03B70000
#define MMC_CMD6_ARG_4BIT_BUS  0x03B70100
#define MMC_CMD6_ARG_8BIT_BUS  0x03B70200

// (2) High SPeed Mode Selection :
// Access bits = 0x03 (Write byte), Index = 0xB9 = 185, Value = 0(26 Mhz max), 1(52 Mhz max)
#define MMC_CMD6_ARG_NORM_SPEED   0x03B90000
#define MMC_CMD6_ARG_HIGH_SPEED   0x03B90100

#define BASIC_DMA_MAX_LEN			(0xFFFF)
#define GPD_MAX_LEN					(0xFFFF)
#define BD_MAX_LEN					(0xFFFF)

#define CMD_TYPE_NORMAL				0
#define CMD_TYPE_ACMD				1

#define NULL_DATA_MODE				0
#define PIO_DATA_MODE				1
#define BASIC_DMA_DATA_MODE			2
#define DESC_DMA_DATA_MODE			3
#define ENHANCED_DMA_DATA_MODE		4

typedef struct __sd_cmd_t__ 
{
	unsigned char idx;
	unsigned int arg;
	unsigned int resp[4];		// Maximum Length Reserved
	unsigned int autoStopResp;	
	unsigned int dataMode;
	unsigned int buffLen;
	void *pBuff;				// Point to data buffer in PIO and basic DMA mode
} sd_cmd_t;

#define CARDTYPE_NOT_SD				0
#define CARDTYPE_MEM_STD			1
#define CARDTYPE_MEM_HC				2
#define CARDTYPE_MEM_MMC			3
#define CARDTYPE_IO					4
#define CARDTYPE_COMBO_STD			5
#define CARDTYPE_COMBO_HC			6

#define VOLTAGE_NORMAL_RANGE		0x1
#define VOLTAGE_LOW_RANGE			0x2

struct __sd_cid_s__ {
	unsigned long long resv0			:1;
	unsigned long long crc				:7;
	unsigned long long mdt				:12;
	unsigned long long resv1			:4;
	unsigned long long psn				:32;
	unsigned long long prv				:8;

	unsigned long long pnm				:40;

	unsigned long long oid				:16;
	unsigned long long mid				:8;
};

typedef struct __sd_cid_s__  sd_cid_t;
  
struct __csd_10_s__ {
	unsigned int resv0					:1;
	unsigned int crc					:7;
	unsigned int resv1					:2;
	unsigned int fileFmt				:2;
	unsigned int tmpWrProtect			:1;
	unsigned int permWrProtect			:1;
	unsigned int copy					:1;
	unsigned int fileFmtGrp				:1;

	unsigned int resv2					:5;
	unsigned int wrBlPartial			:1;
	unsigned int wrBlLen				:4;
	unsigned int r2wFact				:3;
	unsigned int resv3					:2;
	unsigned int wpGrpEnable			:1;

	unsigned long long wpGrpsize		:7;
	unsigned long long sectorSize		:7;
	unsigned long long eraseBlkEn		:1;
	unsigned long long cSizeMult		:3;
	unsigned long long vddWCurrMax		:3;
	unsigned long long vddWCurrMin		:3;
	unsigned long long vddRCurrMax		:3;
	unsigned long long vddRCurrMin		:3;
	unsigned long long cSize			:12;
	unsigned long long resv4			:2;
	unsigned long long dsrImp			:1;
	unsigned long long rdBlkMisAlign	:1;
	unsigned long long wrBlkMisAlign	:1;
	unsigned long long rdBlPartial		:1;
	unsigned long long rdBlkLen			:4;
	unsigned long long ccc				:12;
};

typedef struct __csd_10_s__  csd_10_t;

struct __csd_20_s__ {
	unsigned int resv0					:1;
	unsigned int crc					:7;

	unsigned int resv1					:2;
	unsigned int fileFmt				:2;
	unsigned int tmpWrProtect			:1;
	unsigned int permWrProtect			:1;
	unsigned int copy					:1;
	unsigned int fileFmtGrp				:1;
	unsigned int resv2					:5;
	unsigned int wrBlPartial			:1;
	unsigned int wrBlLen				:4;
	unsigned int r2wFactor				:3;
	unsigned int resv3					:2;
	unsigned int wpGrpEnable			:1;

	unsigned long long wpGrpSize		:7;
	unsigned long long sectorSize		:7;
	unsigned long long eraseBlkLen		:1;
	unsigned long long resv4			:1;
	unsigned long long cSize			:22;
	unsigned long long resv5			:6;
	unsigned long long dsrImp			:1;
	unsigned long long rdBlkMisAlign	:1;
	unsigned long long wrBlkMisAlign	:1;
	unsigned long long rdBlPartial		:1;
	unsigned long long rdBlLen			:4;
	unsigned long long ccc				:12;

};

typedef struct __csd_20_s__  csd_20_t;

/*
struct __sd_csd_s__ {

	union {
		csd_10_t v10;
		csd_20_t v20;
	} csdSub;

	unsigned int tranSpeed				:8;
	unsigned int nsac					:8;
	unsigned int taac					:8;
	unsigned int resv0					:6;
	unsigned int csdStructure			:2;
}  __attribute ((packed));

typedef struct __sd_csd_s__  sd_csd_t;
*/
  
struct __card_status_s__ {
	unsigned int resv0					:3;
	unsigned int akeSeqErr				:1;
	unsigned int resv1					:1;
	unsigned int appCmd					:1;
	unsigned int resv2					:2;
	unsigned int rdyForData				:1;
	unsigned int currStat				:4;
	unsigned int eraseReset				:1;
	unsigned int cardEccDisabled		:1;
	unsigned int wpEraseSkip			:1;
	unsigned int csdOverwrite			:1;
	unsigned int resv3					:2;
	unsigned int error					:1;
	unsigned int ccError				:1;
	unsigned int cardEccFailed			:1;
	unsigned int illegalCmd				:1;
	unsigned int comCrcErr				:1;
	unsigned int lockUnlockFail			:1;
	unsigned int cardIsLocked			:1;
	unsigned int wpViolation			:1;
	unsigned int eraseParam				:1;
	unsigned int eraseSeqErr			:1;
	unsigned int blkLenErr				:1;
	unsigned int addressErr				:1;
	unsigned int outOfRange				:1;
};

typedef struct __card_status_s__  card_status_t;

struct __mem_ocr_s__ {
	unsigned int ocr					:24;
	unsigned int resv0					:6;
	unsigned int hcs					:1;
	unsigned int memRdy					:1;
};

typedef struct __mem_ocr_s__  mem_ocr_t;

struct __io_ocr_s__ {
	unsigned int ocr					:24;
	unsigned int resv0					:3;
	unsigned int memPresent				:1;
	unsigned int nof					:3;
	unsigned int ioRdy					:1;
};

typedef struct __io_ocr_s__  io_ocr_t;

struct __io_r5_s__ {
	unsigned int rwData					:8;
	unsigned int respFlag				:8;
	unsigned int resv					:16;
} __attribute ((packed));

typedef struct __io_r5_s__  io_r5_t;

struct __sd_scr_s__ {
	unsigned int manufactureResv;
	unsigned int resv0					:16;
	unsigned int busWidth				:4;
	unsigned int security				:3;
	unsigned int dsAfterErase			:1;
	unsigned int spec					:4;
	unsigned int scrStructure			:4;
};

typedef struct __sd_scr_s__  sd_scr_t;

#define CSD_LENGTH				(4)

typedef struct __sd_card_s__ {

	unsigned int cardType;
	unsigned int blkAddrMode;

//  Common Card Definitions
	sd_cid_t cid;

	unsigned int memInitDone;		// Flag for Memory initialized status

// Card Status
	card_status_t cardStatus;

//	Memory Card Definitions
	unsigned int memRca;
	mem_ocr_t memOcr;	
} sdcard_t;

struct __sd_status_s__ {
	unsigned int resv0[12];				// 384 bits

	unsigned long long resv1			:16;
	unsigned long long eraseOffset		:2;
	unsigned long long eraseTimeout		:6;
	unsigned long long eraseSize		:16;
	unsigned long long resv2			:4;
	unsigned long long auSize			:4;
	unsigned long long performanceMove	:8;
	unsigned long long speedClass		:8;

	unsigned int sizeOfProtArea;

	unsigned int sdCardType				:16;
	unsigned int resv3					:13;
	unsigned int securedMode			:1;
	unsigned int dataBusWidth			:2;
};

typedef struct __sd_status_s__ sd_status_t;

/* SDC_CMD */
/* Standard SD 2.0 Commands						Type	Arguments			Response	*/
/* Class 0 */
#define CMD0_GO_IDLE_STATE				0	/*	bc										*/
#define CMD1_MMC_SEND_OP_COND			1	/*	bcr		[23:0] OCR			R3			*/
#define CMD2_ALL_SEND_CID				2	/*	bcr							R2			*/
#define CMD3_SEND_RELATIVE_ADDR			3	/*	bcr							R6			*/
#define CMD4_SET_DSR					4	/*	bc		[31:16] DSR						*/
#define CMD6_MMC_SWITCH				    6	/*	ac		[1:0] Bus width		R1B			*/
#define CMD7_SELECT_CARD				7	/*	ac		[31:16] RCA			R1b			*/
#define CMD8_SEND_IF_COND				8	/*	bcr		[11:8] VHS			R7			*/
#define CMD8_MMC_SEND_EXT_CSD		    8	/*	adtc		[31:0] stuff bits			R1		*/
#define CMD9_SEND_CSD					9	/*	ac		[31:16] RCA			R2			*/
#define CMD10_SEND_CID					10	/*	ac		[31:16] RCA			R2			*/
#define CMD12_STOP_TRANSMISSION			12	/*	ac							R1b			*/
#define CMD13_SEND_STATUS				13	/*	ac		[31:16] RCA			R1b			*/
#define CMD15_GO_INACTIVE_STATE			15	/*	ac		[31:16] RCA						*/
	
/* Class 2 */
#define CMD16_SET_BLOCKLEN				16	/*	ac		[31:0] blk len		R1			*/
#define CMD17_READ_SINGLE_BLOCK			17	/*	adtc	[31:0] data addr.	R1			*/
#define CMD18_READ_MULTIPLE_BLOCK		18	/*	adtc	[31:0] data addr.	R1			*/

/* Class 4 */
#define	CMD24_WRITE_BLOCK				24	/*	adtc	[31:0] data addr.	R1			*/
#define CMD25_WRITE_MULTIPLE_BLOCK		25	/*	adtc	[31:0] data addr.	R1			*/
#define CMD27_PROGRAM_CSD				27	/*	adtc						R1			*/

/* Class 6 */
#define CMD28_SET_WRITE_PROT			28	/*	ac		[31:0] data addr.	R1b			*/
#define CMD29_CLR_WRITE_PROT			29	/*	ac		[31:0] data addr.	R1b			*/
#define CMD30_SEND_WRITE_PROT			30	/*	adtc	[31:0] prot addr.	R1			*/

/* Class 5 */
#define CMD32_ERASE_WR_BLK_START		32	/*	ac		[31:0] data addr.	R1			*/
#define CMD33_ERASE_WR_BLK_END			33	/*	ac		[31:0] data addr.	R1			*/
#define CMD38_ERASE						38	/*	ac							R1b			*/

/* Class 7 */
#define CMD42_LOCK_UNLOCK				42	/*	ac		[31:0] Reserved		R1			*/

/* Class 8 */
#define CMD55_APP_CMD					55	/*	ac		[31:16] RCA			R1			*/
#define CMD56_GEN_CMD					56	/*	adtc	[0] RD/WR			R1			*/

/* Application Specific Cmds */
#define ACMD6_SET_BUS_WIDTH				6	/*	ac		[1:0] Bus width		R1			*/
#define ACMD13_SD_STATUS				13	/*	adtc						R1			*/
#define ACMD22_SEND_NUM_WR_BLOCKS		22	/*	adtc						R1			*/
#define ACMD23_SET_WR_BLK_ERASE_COUNT	23	/*	ac		[22:0] Blk num.		R1			*/
#define ACMD41_SD_SEND_OP_COND			41	/*	bcr		[23:0] OCR			R3			*/
#define ACMD42_SET_CLR_CARD_DETECT		42	/*	ac		[0] Set cd			R1			*/
#define ACMD51_SEND_SCR					51	/*	adtc						R1			*/

/* IO Card Commands */
#define CMD5_IO_SEND_OP_COND			5	/*	ac		[24:0] OCR			R4			*/
#define CMD52_IO_RW_DIRECT				52	/*	ac							R5			*/
#define CMD53_IO_RW_EXTENDED			53	/*	ac							R5			*/

/* Checksum Option for fill_chksum */
#define CHKSUM_OPT_NONE			(0x00)
#define CHKSUM_OPT_GPD			(0x01)
#define CHKSUM_OPT_BD			(0x02)
#define CHKSUM_OPT_ERR_IN_GPD	(0x04)
#define CHKSUM_OPT_ERR_IN_BD	(0x08)

typedef struct __msdc_intr_s__ {

	// Interrupt Counter
	unsigned int mmcIrqCnt;
	unsigned int msdcCdscCnt;
	unsigned int sdAutoCmdRdyCnt;
	unsigned int sdAutoCmdToCnt;
	unsigned int sdAutoCmdRespCrcErrCnt;
	unsigned int dmaQEmptyCnt;
	unsigned int sdSdioIrqCnt;
	unsigned int sdCmdRdyCnt;
	unsigned int sdCmdToCnt;
	unsigned int sdRespCrcErrCnt;
	unsigned int sdCstaCnt;
	unsigned int sdXferCompleteCnt;
	unsigned int dmaXferDoneCnt;
	unsigned int sdDatToCnt;
	unsigned int sdDataCrcErrCnt;
	unsigned int msRdyCnt;
	unsigned int msSifCnt;
	unsigned int msToerCnt;
	unsigned int msCrcErrCnt;
	unsigned int msCedCnt;
	unsigned int msErrCnt;
	unsigned int msBreqCnt;
	unsigned int msCmdNkCnt;

} msdc_intr_t;

typedef struct __sdhost_s__ {

	// Accumulated interrupt vector
	volatile unsigned int accuVect;
} sdhost_t;

//---------------------------------------------------------------------------
// Basic definitions
//---------------------------------------------------------------------------

#define MSDC_SUCCESS								   (0)
#define MSDC_FAILED									   (-1)

//---------------------------------------------------------------------------
// MSDC error code definition
//---------------------------------------------------------------------------

#define MSDC_INIT_ERROR                               0x80
#define MSDC_IDENTIFY_CARD_ERROR                      0x81
#define MSDC_READ_BOOTLDR_DATA_ERROR                  0x82

#define MSDC_RST_TIMEOUT_ERROR                        0xA0
#define MSDC_CLK_TIMEOUT_ERROR                        0xA1
#define MSDC_FIFOCLR_TIMEOUT_ERROR                    0xA2

#define MSDC_RESCAN_CARD_FAILED                       0xA3
#define MSDC_WAIT_SDC_BUS_TIMEOUT_ERROR               0xA4
#define MSDC_WAIT_CMD_TIMEOUT_ERROR                   0xA5
#define MSDC_CMD_TIMEOUT_ERROR                        0xA6
#define MSDC_CMD_CRC_ERROR                            0xA7
#define MSDC_CMD_INT_ERROR                            0xA8
#define MSDC_WAIT_DATA_TIMEOUT_ERROR                  0xA9
#define MSDC_DATA_TIMEOUT_ERROR                       0xAA
#define MSDC_DATA_CRC_ERROR                           0xAB
#define MSDC_DATA_INT_ERROR                           0xAC
#define MSDC_READ_CARD_FAILED                         0xAD

#define MSDC_CMD0_FAILED                              0xC0
#define MSDC_CMD1_FAILED                              0xC1
#define MSDC_CMD2_FAILED                              0xC2
#define MSDC_CMD3_FAILED                              0xC3
#define MSDC_CMD6_FAILED                              0xC6
#define MSDC_CMD7_FAILED                              0xC7
#define MSDC_CMD12_FAILED                             0xD2
#define MSDC_CMD17_FAILED                             0xD7
#define MSDC_CMD18_FAILED                             0xD8

#define EMMC_READ_CARD_FAIL                           1
#define EMMC_MSDC_INIT_FAIL                           2
#define EMMC_IDENT_CARD_FAIL                          3
#define EMMC_SUCCESS                                  0

//---------------------------------------------------------------------------
// MSDC error retry limitation
//---------------------------------------------------------------------------
#define MSDC_SYS_INIT_RETRY_LIMIT                      0
#define MSDC_IDENTIFY_CARD_RETRY_LIMIT                 0
#define MSDC_READ_CARD_RETRY_LIMIT                     0
#define MSDC_READ_BOOTLDR_DATA_RETRY_LIMIT             0

#endif // _MSDCBOOT_H_

