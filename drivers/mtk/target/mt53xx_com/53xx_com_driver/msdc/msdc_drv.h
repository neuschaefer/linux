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

#include "msdc_host_hw.h"
#include "msdc_slave_hw.h"

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

#define MSDC_CLOCK_SELECTION_STRING    54, 48, 43, 40, 36, 30, 27, 24, 18, 13, 0
#define SD_DEFAULT_HIGH_CLOCK_INDEX 1
#define SD_DEFAULT_NORM_CLOCK_INDEX 7
#define SD_DEFAULT_INIT_CLOCK_INDEX 10

#define CMD0_RESET_LIMIT	            (3)
#define MMC_CMD1_RETRY_LIMIT            (1500)
#define SDHC_BLK_SIZE		            (512)
#define MMC_DEF_RCA                      0x0001

//---------------------------------------------------------------------------
// Basic definitions
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

#define EMMC_READ_CARD_FAIL                                 1
#define EMMC_MSDC_INIT_FAIL                                 2
#define EMMC_IDENT_CARD_FAIL                                3
#define EMMC_SUCCESS                                        0


//---------------------------------------------------------------------------
// MSDC error retry limitation
//---------------------------------------------------------------------------

#define MSDC_LOAD_MMC_IMAGE_RETRY_LIMIT               10
#define MSDC_SYS_INIT_RETRY_LIMIT                      3
#define MSDC_IDENTIFY_CARD_RETRY_LIMIT                10
#define MSDC_READ_CARD_RETRY_LIMIT                     6
#define MSDC_READ_BOOTLDR_DATA_RETRY_LIMIT             8


//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

#define MSDC_WRITE32(addr, value)      (*(volatile unsigned int *)(addr)) = (value)
#define MSDC_READ32(addr)              (*(volatile unsigned int *)(addr))

#define MSDC_SETBIT(addr, dBit)        MSDC_WRITE32(addr, MSDC_READ32(addr) | (dBit))
#define MSDC_CLRBIT(addr, dBit)        MSDC_WRITE32(addr, MSDC_READ32(addr) & (~(dBit)))
	
#define RESPONSE_NO			          (0)
#define RESPONSE_R1			          (1)
#define RESPONSE_R2			          (2)
#define RESPONSE_R3			          (3)
#define RESPONSE_R4			          (4)
#define RESPONSE_R5			          (5)
#define RESPONSE_R6			          (6)
#define RESPONSE_R7			          (7)
#define RESPONSE_R1B		          (8)

#define MSDC_SUCCESS                  (int)(0)
#define MSDC_FAILED                   (int)(-1)

#define CMD_ERR_SUCCESS               (MSDC_SUCCESS)
#define CMD_ERR_FAILED                (MSDC_FAILED)

#define CMD_ERR_NO_RESP               (0x01 << 0)
#define CMD_ERR_RESP_CRCERR           (0x01 << 1)
#define CMD_ERR_WAIT_CMD_TO           (0x01 << 2)

#define CMD_ERR_DATTO                 (0x01 << 3)
#define CMD_ERR_DATA_CRCERR           (0x01 << 4)
#define CMD_ERR_WAIT_DATATO           (0x01 << 5)
#define CMD_ERR_DATA_FAILED           (0x01 << 6)

typedef struct __sd_cmd_t__ 
{
	UINT8 idx;
	UINT32 arg;
	UINT32 resp[4];		// Maximum Length Reserved
	UINT32 autoStopResp;	
	UINT32 dataMode;
	UINT32 buffLen;
	VOID *pBuff;				// Point to data buffer in PIO and basic DMA mode
} sd_cmd_t;

typedef struct __msdc_intr_s__ 
{
	// Interrupt Counter
	UINT32 mmcIrqCnt;
	UINT32 msdcCdscCnt;
	UINT32 sdAutoCmdRdyCnt;
	UINT32 sdAutoCmdToCnt;
	UINT32 sdAutoCmdRespCrcErrCnt;
	UINT32 dmaQEmptyCnt;
	UINT32 sdSdioIrqCnt;
	UINT32 sdCmdRdyCnt;
	UINT32 sdCmdToCnt;
	UINT32 sdRespCrcErrCnt;
	UINT32 sdCstaCnt;
	UINT32 sdXferCompleteCnt;
	UINT32 dmaXferDoneCnt;
	UINT32 sdDatToCnt;
	UINT32 sdDataCrcErrCnt;
	UINT32 msRdyCnt;
	UINT32 msSifCnt;
	UINT32 msToerCnt;
	UINT32 msCrcErrCnt;
	UINT32 msCedCnt;
	UINT32 msErrCnt;
	UINT32 msBreqCnt;
	UINT32 msCmdNkCnt;

} msdc_intr_t;

typedef struct __sdhost_s__ 
{
    UINT32 busWidth;
    UINT32 blkLen;

    sd_status_t sdStatus;

    // Select polling mode or interrupt mode 
    UINT32 polling;   

    // Host Data Mode 
    UINT32 dataMode;  

	// Host Bus Clock
	UINT32 maxClock;
	UINT32 curClock;

    // DMA Burst Size 
    UINT32 dmaBstSize;
    // Configurable Maximum Basic DMA Len     
    UINT32 maxBasicDmaLen;    

    // Accumulated interrupt vector
    volatile UINT32 accuVect;

    // Accumulated interrupt Counter
    msdc_intr_t intrSta;

    // I/O OCR
    UINT32 ioOcr;

    // byte unit
    UINT32 dwSizePerDma; 
} sdhost_t;

//---------------------------------------------------------------------------
// export function declaration
//---------------------------------------------------------------------------
extern INT32 MsdcInit (VOID);
extern VOID MsdcInitHostConfig(VOID);
extern VOID MsdcContinueClock (UINT32 i4ContinueClock);
extern INT32 MsdcSetClkFreq (UINT32 clkFreq);
extern INT32 MsdcSetBusWidth (UINT32 busWidth); 
extern INT32 MsdcCardRescan (VOID);
extern INT32 MsdcMemRead (UINT32 addr, VOID *pBuff, UINT32 buffLen);
extern INT32 MsdcMemWrite(UINT32 addr, VOID *pBuff, UINT32 buffLen);
extern INT32 MsdcMemErase(UINT32 addr, UINT32 buffLen);
extern VOID MsdcErrorHandling(VOID);
extern INT32 MsdcEmmcBootStop(BOOL fgResetMsdc);

extern INT32 cmd7_select_card (UINT32 sel);

#endif // #ifndef _MSDC_DRV_H_
