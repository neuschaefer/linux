/******************************************************************
 * Copyright 2005 Mentor Graphics Corporation
 *
 * This file is part of the Inventra Controller Driver for Linux.
 * 
 * The Inventra Controller Driver for Linux is free software; you 
 * can redistribute it and/or modify it under the terms of the GNU 
 * General Public License version 2 as published by the Free Software 
 * Foundation.
 * 
 * The Inventra Controller Driver for Linux is distributed in 
 * the hope that it will be useful, but WITHOUT ANY WARRANTY; 
 * without even the implied warranty of MERCHANTABILITY or 
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public 
 * License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with The Inventra Controller Driver for Linux ; if not, 
 * write to the Free Software Foundation, Inc., 59 Temple Place, 
 * Suite 330, Boston, MA  02111-1307  USA
 * 
 * ANY DOWNLOAD, USE, REPRODUCTION, MODIFICATION OR DISTRIBUTION 
 * OF THIS DRIVER INDICATES YOUR COMPLETE AND UNCONDITIONAL ACCEPTANCE 
 * OF THOSE TERMS.THIS DRIVER IS PROVIDED "AS IS" AND MENTOR GRAPHICS 
 * MAKES NO WARRANTIES, EXPRESS OR IMPLIED, RELATED TO THIS DRIVER.  
 * MENTOR GRAPHICS SPECIFICALLY DISCLAIMS ALL IMPLIED WARRANTIES 
 * OF MERCHANTABILITY; FITNESS FOR A PARTICULAR PURPOSE AND 
 * NON-INFRINGEMENT.  MENTOR GRAPHICS DOES NOT PROVIDE SUPPORT 
 * SERVICES OR UPDATES FOR THIS DRIVER, EVEN IF YOU ARE A MENTOR 
 * GRAPHICS SUPPORT CUSTOMER. 
 ******************************************************************/

#ifndef __MUSB_MUSBDEFS_H__
#define __MUSB_MUSBDEFS_H__

#include "dt_debug.h"

#if 0 //!PORTING
#include "base/llist.h"
#endif
#include "mu_hdrdf.h"
#include "debug.h"


/****************************** USB CONSTANTS ********************************/
#ifndef MUSB_QMU
#ifndef MGC_HSDMA_MIN_DMA_LEN
#define MGC_HSDMA_MIN_DMA_LEN	0
#endif
#endif

/****************************** DEBUG CONSTANTS *******************************/

#if MUSB_DEBUG > 0
static int MGC_DebugLevel=MUSB_DEBUG;
static int MGC_DebugDisable=0;
#endif

/****************************** CONSTANTS *************************************/

/******************************  ********************************/

#define CACHE_ENABLE 0 // mhzhang

#if CACHE_ENABLE
#define APUSB_MPU_CACHE_INVALIDATE_AP(base,size) MPU_CACHE_INVALIDATE_AP(base,size)
#define APUSB_MPU_CACHE_FLUSH_INV_AP(base,size) MPU_CACHE_FLUSH_INV_AP(base,size)
#define APUSB_MPU_CACHE_INV_ALL_AP() MPU_CACHE_INV_ALL_AP()
#define APUSB_MPU_CACHE_FLUSH_ALL_AP() MPU_CACHE_FLUSH_ALL_AP()
#define APUSB_MPU_CACHE_FLUSH_INV_ALL_AP() MPU_CACHE_FLUSH_INV_ALL_AP()
#else
#define APUSB_MPU_CACHE_INVALIDATE_AP(base,size)
#define APUSB_MPU_CACHE_FLUSH_INV_AP(base,size)
#define APUSB_MPU_CACHE_INV_ALL_AP()
#define APUSB_MPU_CACHE_FLUSH_ALL_AP()
#define APUSB_MPU_CACHE_FLUSH_INV_ALL_AP()
#endif

/******************************  ********************************/

#if MUSB_DEBUG > 0
#define STATIC
#else
#define STATIC static
#endif

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#ifndef MUSB_C_NUM_EPS
#define MUSB_C_NUM_EPS ((unsigned char)16)
#endif

#ifndef MGC_END0_FIFOSIZE
#if defined MUSB_HIGHSPEED
#define MGC_END0_FIFOSIZE 64
#define MUSB_HS_DEVICE
#undef MUSB_FS_DEVICE
#elif defined MUSB_FULLSPEED
#define MGC_END0_FIFOSIZE 8
#define MUSB_FS_DEVICE
#undef MUSB_HS_DEVICE
#endif
#endif

#if PORTING
#define MUSB_DEV_7108
#endif

#if defined MUSB_DEV_USB2ETH
#define MUSB_TX_NULL_LENGTH_PACKET    
#define MUSB_RX_NULL_LENGTH_PACKET  
#define MAXBUFSIZE 2048
#define MUSB_LOOPBACK_DELAY_MS 1
#elif defined MUSB_DEV_MASSSTORAGE
#undef MUSB_TX_NULL_LENGTH_PACKET
#undef MUSB_RX_NULL_LENGTH_PACKET
#define MAXBUFSIZE 65536
#define MUSB_LOOPBACK_DELAY_MS 1
#elif defined MUSB_DEV_7108
#undef MUSB_TX_NULL_LENGTH_PACKET
#define MUSB_RX_NULL_LENGTH_PACKET  
#define MAXBUFSIZE 4096
#define MUSB_LOOPBACK_DELAY_MS 50
#endif


// EP0 Transaction State
#define MGC_END0_SETUP                    0x10       // setup stage
#define MGC_END0_DATA                     0x20       // data stage
#define MGC_END0_STATUS                  0x40       // handshake stage
#define MGC_END0_FINISH                   0x80      // END0 enumeration done
// EP0 Data Stage
#define MGC_END0_DSTAGE_DATAIN     0x21
#define MGC_END0_DSTAGE_DATAOUT   0x22
#define MGC_END0_DSTAGE_NODATA    0x23

#if defined MUSB_DEV_USB2ETH
// ETH EP0 Transaction State
#define MGC_ETHEND0_SETUP                 0x10
#define MGC_ETHEND0_DATA                  0x20
#define MGC_ETHEND0_STATUS               0x40       // handshake stage
#define MGC_ETHEND0_FINISH                0x80      // END0 enumeration done
// ETH EP0 Data Stage
#define MGC_ETHEND0_DSTAGE_DATAIN      0x21
#define MGC_ETHEND0_DSTAGE_DATAOUT   0x22
#define MGC_ETHEND0_DSTAGE_NODATA     0x23

#elif defined MUSB_DEV_MASSSTORAGE
// MS EP Transaction State
#define MGC_MSEND_REQUEST                0x10
#define MGC_MSEND_RESPONSE              0x20
#define MGC_MSEND_STATUS                  0x40       // handshake stage
#define MGC_MSEND_FINISH                   0x80      // END0 enumeration done

#elif defined MUSB_DEV_7108
#define MGC_7108END0_SETUP                0x10
#define MGC_7108END0_DATA                 0x20
#define MGC_7108END0_STATUS              0x40       // handshake stage
#define MGC_7108END0_FINISH               0x80      // END0 enumeration done
// ETH EP0 Data Stage
#define MGC_7108END0_DSTAGE_DATAIN      0x21
#define MGC_7108END0_DSTAGE_DATAOUT   0x22
#define MGC_7108END0_DSTAGE_NODATA     0x23
#endif

/******************************AHB MONITOR*********************************/
#define APMCU_CIRQ_BASE                 0x80060000
#define APMCU_MON_MISAR                 (APMCU_CIRQ_BASE + 0x12E0)
#define APMCU_MON_MIMKR                 (APMCU_CIRQ_BASE + 0x12E4)
#define APMCU_MON_MIMCR                 (APMCU_CIRQ_BASE + 0x12E8)
#define APMCU_MON_MIMSR                 (APMCU_CIRQ_BASE + 0x12EC)
#define APMCU_MON_MISTR                 (APMCU_CIRQ_BASE + 0x12F0)
#define APMCU_INT0_MASK                 0x00000001
#define APMCU_INT1_MASK                 0x00000002
#define APMCU_INT2_MASK                 0x00000004

#define APMCU_AHBMON_BASE               0x800F0000
#define APMCU_AHBMON_MCENCTL    (APMCU_AHBMON_BASE + 0x0000)
#define MON_OP_OFFSET                   0
#define MON_MODE_OFFSET                 4
#define HIT_START_MASK                  0x00010000
#define HIT_RESUME_MASK                 0x00020000
#define HIT_PAUSE_MASK                  0x00040000
#define APPERF_OP_OFFSET                16

#define APMCU_AHBMON_MTMR0L             (APMCU_AHBMON_BASE + 0x0004)
#define APMCU_AHBMON_MTMR0H             (APMCU_AHBMON_BASE + 0x0008)
#define APMCU_AHBMON_RSVD               (APMCU_AHBMON_BASE + 0x000C)
#define APMCU_AHBMON_MTMR1L             (APMCU_AHBMON_BASE + 0x0010)
#define APMCU_AHBMON_MTMR1H             (APMCU_AHBMON_BASE + 0x0014)

// For APMCU, i = {0,1,2}; 0: APS0_AHB, 1: APD0_AHB, 2: API_AHB
#define APMCU_MON_MCTLR(i)              (APMCU_AHBMON_BASE + (i) * 0x0100 + 0x0100)
#define MON_MASTER_OFFSET               0
#define MON_MASTER_LENGTH               4
#define MON_TRANS_OFFSET                14
#define MON_TRANS_LENGTH                2
#define MON_MASTER_RPT_OFFSET   16
#define MON_MASTER_RPT_LENGTH   4
#define MON_STAT_OFFSET                 31
#define MON_STAT_LENGTH                 1

// Access Sensor Registers
#define APMCU_MON_MADDR(i)              (APMCU_AHBMON_BASE + (i) * 0x0100 + 0x0104)
#define APMCU_MON_MADDMR(i)             (APMCU_AHBMON_BASE + (i) * 0x0100 + 0x0108)
#define APMCU_MON_SADDR(i)              (APMCU_AHBMON_BASE + (i) * 0x0100 + 0x010C)
#define APMCU_MON_SDATR(i)              (APMCU_AHBMON_BASE + (i) * 0x0100 + 0x0110)
#define APMCU_MON_SINFR(i)              (APMCU_AHBMON_BASE + (i) * 0x0100 + 0x0114)
#define MON_HTRANS_OFFSET               0
#define MON_HTRANS_LENGTH               2
#define MON_HSIZE_OFFSET                2
#define MON_HSIZE_LENGTH                2
#define MON_HWRITE_OFFSET               4
#define MON_HWRITE_LENGTH               1
#define MON_HBURST_OFFSET               5
#define MON_HBURST_LENGTH               3
// Traffic Monitor Registers
#define APMCU_MON_MARBNRL(i)    (APMCU_AHBMON_BASE + (i) * 0x0100 + 0x0120)
#define APMCU_MON_MARBNRH(i)    (APMCU_AHBMON_BASE + (i) * 0x0100 + 0x0124)
#define APMCU_MON_MNSNRL(i)             (APMCU_AHBMON_BASE + (i) * 0x0100 + 0x0128)
#define APMCU_MON_MNSNRH(i)             (APMCU_AHBMON_BASE + (i) * 0x0100 + 0x012C)
#define APMCU_MON_MRDYNRL(i)    (APMCU_AHBMON_BASE + (i) * 0x0100 + 0x0130)
#define APMCU_MON_MRDYNRH(i)    (APMCU_AHBMON_BASE + (i) * 0x0100 + 0x0134)
#define APMCU_MON_MTTRNCRL(i)   (APMCU_AHBMON_BASE + (i) * 0x0100 + 0x0138)
#define APMCU_MON_MTTRNCRH(i)   (APMCU_AHBMON_BASE + (i) * 0x0100 + 0x013C)
#define APMCU_MON_MTDATACRL(i)  (APMCU_AHBMON_BASE + (i) * 0x0100 + 0x0140)
#define APMCU_MON_MTDATACRH(i)  (APMCU_AHBMON_BASE + (i) * 0x0100 + 0x0144)
#define APMCU_MON_MTDEVCRL(i)   (APMCU_AHBMON_BASE + (i) * 0x0100 + 0x0148)
#define APMCU_MON_MTDEVCRH(i)   (APMCU_AHBMON_BASE + (i) * 0x0100 + 0x014C)
#define APMCU_MON_MMTRNCR(i)    (APMCU_AHBMON_BASE + (i) * 0x0100 + 0x0150)
#define APMCU_MON_MMDEVCR(i)    (APMCU_AHBMON_BASE + (i) * 0x0100 + 0x0154)

// APMCU Performance Counter Register
// Only access-able from APMCU
#define APMCU_AP_ICR_ACCRL              (APMCU_AHBMON_BASE + 0x0800)
#define APMCU_AP_ICR_ACCRH              (APMCU_AHBMON_BASE + 0x0804)
#define APMCU_AP_ICR_HITRL              (APMCU_AHBMON_BASE + 0x0808)
#define APMCU_AP_ICR_HITRH              (APMCU_AHBMON_BASE + 0x080C)
#define APMCU_AP_DCR_ACCRL              (APMCU_AHBMON_BASE + 0x0810)
#define APMCU_AP_DCR_ACCRH              (APMCU_AHBMON_BASE + 0x0814)
#define APMCU_AP_DCR_HITRL              (APMCU_AHBMON_BASE + 0x0818)
#define APMCU_AP_DCR_HITRH              (APMCU_AHBMON_BASE + 0x081C)
#define APMCU_AP_DCW_ACCRL              (APMCU_AHBMON_BASE + 0x0820)
#define APMCU_AP_DCW_ACCRH              (APMCU_AHBMON_BASE + 0x0824)
#define APMCU_AP_DCW_HITRL              (APMCU_AHBMON_BASE + 0x0828)
#define APMCU_AP_DCW_HITRH              (APMCU_AHBMON_BASE + 0x082C)

/****************************** DMA TYPES **********************************/

 #include "dma.h"

#ifndef MGC_HSDMA_CHANNELS
#define MGC_HSDMA_CHANNELS 8
#endif 

/****************************** Ep Configuration ********************************/

/** The End point descriptor */
typedef struct MUSB_EpFifoDescriptor_t {
    unsigned char bType; 	/* 0 for autoconfig, CNTR, ISOC, BULK, INTR */
    unsigned char bDir; 	/* 0 for autoconfig, INOUT, IN, OUT */
    unsigned short wSize;	/* 0 for autoconfig, or the size */
    unsigned char bDbe; 	/* Double buffering: 0 disabled, 1 enabled */
} MUSB_EpFifoDescriptor;

#define MUSB_EPD_AUTOCONFIG	0

#define MUSB_EPD_T_CNTRL	      1
#define MUSB_EPD_T_ISOC		2
#define MUSB_EPD_T_BULK		3
#define MUSB_EPD_T_INTR		4

#define MUSB_EPD_D_INOUT	0
#define MUSB_EPD_D_TX		1
#define MUSB_EPD_D_RX		2

#define ETH_FIFO_CONFIG_NUM 2
#define ETH_FIFO_CONFIG_SB_INDEX 0
#define ETH_FIFO_CONFIG_DB_INDEX 1

#define USB7108_FIFO_CONFIG_NUM 2
#define USB7108_FIFO_CONFIG_SB_INDEX 0
#define USB7108_FIFO_CONFIG_DB_INDEX 1

#if defined MUSB_HS_DEVICE
#define AT_FIFOTEST_CONFIG_NUM 8
#elif defined MUSB_FS_DEVICE
#define AT_FIFOTEST_CONFIG_NUM 51
#endif

typedef struct MUSB_ETHEpFifoDescriptor_t {
    unsigned short wFifoStartAddr;     /* Start Address of FIFO - 0 ~ 4864*/
    MUSB_EpFifoDescriptor sEpFifoDescriptor[MUSB_C_NUM_EPS];
} MUSB_ETHEpFifoDescriptor;

// 7109 Endpoint Configuration 5R4T
#define MT7108_TX_EPNUM  4
#define MT7108_RX_EPNUM  5

#define MUSB_C_DYNFIFO_DEF

/******************************** TYPES *************************************/

/**
 * localEnd.
 */
typedef struct _LocalEnd_t
{
//    spinlock_t Lock;
    unsigned char bEnd;                         /* ep number */
    unsigned int dwOffset; 		            /* offset int the current request */
    unsigned int dwRequestSize;             /* request size */
//    unsigned int dwIsoPacket;
//    unsigned int dwWaitFrame;
    unsigned char bRetries;	
    unsigned short wFifoAdd;
    unsigned char bDmaMode;                 /* MGC_DmaMode - DMA Mode and DMA1 Burst Mode */
    unsigned char bNakLimitError;           /* set if NakLimit timeout is assert */
    MGC_DmaChannel* pDmaChannel; 

#ifdef MUSB_CONFIG_PROC_FS
//    unsigned long dwTotalTxBytes;
//    unsigned long dwTotalRxBytes;
//    unsigned long dwTotalTxPackets;
//    unsigned long dwTotalRxPackets;
//    unsigned long dwErrorTxPackets;
//    unsigned long dwErrorRxPackets;
//    unsigned long dwMissedTxPackets;
//    unsigned long dwMissedRxPackets;
#endif

    unsigned int dwMaxPacketSizeTx;
    unsigned int dwMaxPacketSizeRx;
    unsigned char bTxInterval;
    unsigned char bRxInterval;
    unsigned char bTransferType;
    unsigned char bIsSharedFifo;	   
//    unsigned char bRemoteAddress;
//    unsigned char bRemoteEnd;	
//    unsigned char bTrafficType;
    unsigned char bIsClaimed;               /* only for isoc and int traffic */    
    unsigned char bIsTx;
//    unsigned char bIsReady;	
//    unsigned char bStalled;                 /* the ep has been halted */	

    /* Buffer Remainder Data while Tx Transfer Payload > 1 MaxTransferSize*/
    unsigned int dwTxRemainderDataLen;
    unsigned char aTxRemainderData[MAXBUFSIZE];
    unsigned char bZeroPacket;             /* TRUE if (sendLen%MaxPktSize == 0), and should send NULL packet */ 

    /* Record Buffer Address for moving the last short packet in Rx DMA */
    unsigned char *pRxBufAddr;
    unsigned int dwRxRecvSize;

    volatile unsigned char bRxDmaDone;
    volatile unsigned char bTxDmaDone;
    
} LocalEnd;

/**
 * hdrc_t.
 */
typedef struct _hdrc_t
{
    MGC_DmaController* pDmaController; 

    int nBabbleCount;

    LocalEnd aLocalEnd[MUSB_C_NUM_EPS];
	
    unsigned char bEndCount;                    // excluding EP0
    unsigned char bIgnoreDisconnect;        /* during bus resets I got fake disconnects */

    unsigned char bEnd0Stage;                  /* end0 stage while in host or device mode */
    unsigned char bEnd0DataStage;
    unsigned char bHighSpeedReset;
    unsigned char bIsDoubleBuffer;
    unsigned short wFifoAdd;

#if defined MUSB_DEV_USB2ETH
    unsigned char bETHEnd0Stage;
    unsigned char bETHEnd0DataStage;
    unsigned char bETHTxEnd;
    unsigned char bETHRxEnd;

#elif defined MUSB_DEV_MASSSTORAGE
    unsigned char bMSEndStage;
    unsigned int bMSEndResCnt;                 /* The remainder bytes of response data in MS enumeration step */

    unsigned char bMSTxEnd;
    unsigned char bMSRxEnd;
    unsigned char aMSTxDataBuffer[8192];
    unsigned char aMSRxDataBuffer[8192];

#elif defined MUSB_DEV_7108
    volatile unsigned char b7108End0Stage;
    volatile unsigned char b7108End0DataStage;

    unsigned char b7108BulkTxEnd;
    unsigned char b7108BulkRxEnd;
    unsigned char b7108IntrTxEnd;
    unsigned char b7108IntrRxEnd;
    unsigned char b7108IsoTxEnd;
    unsigned char b7108IsoRxEnd;

    unsigned char bEP0DisPingTest;

#endif

    #if 0 //!PORTING
    llist_t             BsrPool;
    llist_t             BsrList;
    #endif

    int                     nEnumStep;
    unsigned short     wSetAddress;
    unsigned int        SetupTxDataIndex;
    unsigned int        SetupTxDataCount;
    unsigned char      SetupTxDataBuffer[256];
    unsigned int        SetupRxDataIndex;
    unsigned int        SetupRxDataCount;
    unsigned char      SetupRxDataBuffer[256];

    volatile unsigned char bRemoteWakeup;
    unsigned char bPrintIsrMsg;

    /* Ixia mode test (call stareth_nic_freeData after bTxDmaDone) */
    unsigned char bIxiaTest;

} hdrc_t;

void MGC_HdrcConfigureEps(hdrc_t *pCtrl);
void MGC_InitLocalEndPoints(hdrc_t *pCtrl);
void AT_HdrcConfigureEps (hdrc_t *pCtrl, MUSB_ETHEpFifoDescriptor *pFifoDescriptor) ;

#endif	/* multiple inclusion protection */
