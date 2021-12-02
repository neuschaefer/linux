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
#ifndef _UTEST_UNIUSB_H
#define _UTEST_UNIUSB_H

//#include "dt_debug.h"


#if 0 //!PORTING
#include <bsp/cisr.h>
#include <musbdefs.h>
#include <ddr_hdrc.h>
#endif

#if 1//PORTING //fill in the correct configuration
#define TX_FIFO_NUM	 5     /* including ep0 */
#define RX_FIFO_NUM 5     /* including ep0 */
#define DMA_CHANNEL_NUM 4
#define CONFIG_DATA 0x1f   /* MPRxE(1),MPTxE(1),BigEndian(0),HBRxe(1),HBTxE(1),DynFIFOSizing(1),SoftConE(1),UTMIDataWidth(1) */
#define USB_FIFO_START_ADDRESS  64
#define NUM_BSR_ITEMS   16
#define USB_RAM_SIZE    6144
#if 1//ben
#define USB_BUF_SIZE    65536
#else
#define USB_BUF_SIZE    64
#endif
#define USB_EP0_MAXP   64

#define USB_IRQ_NO 15

#define	M_REG_FIFOBYTECNT	 0x690
#endif

#define MGC_FIFO_CNT    MGC_Write32
//#define MUSB_MemCopy    x_memcpy

//#define GLOBAL_FADDR
#define SEPERATED_FADDR

typedef enum 
{
	USB_TX = 0,
	USB_RX
} USB_DIR;

typedef enum
{
	USB_CTRL = 0,
	USB_BULK = 2,
	USB_INTR = 3,
	USB_ISO = 1
}USB_TRANSFER_TYPE;

typedef enum
{
    #if !PORTING
    USB_SPEED_HIGH = 0,
    USB_SPEED_FULL
    #else
    DT_USB_SPEED_HIGH = 0,
    DT_USB_SPEED_FULL
    #endif
}USB_SPEED;

typedef enum
{
    USB_EP0_SETUP = 0,
    USB_EP0_DATA,
    USB_EP0_STATUS
}USB_EP0_STATE;

typedef enum
{
    USB_EP0_DATAIN = 0,
    USB_EP0_DATAOUT,
    USB_EP0_NODATA
}USB_EP0_DATASTAGE;

typedef struct {
    unsigned char *txBuf;
    unsigned char *rxBuf;
    unsigned char setupBuf[8];
    USB_EP0_STATE state;
    USB_EP0_DATASTAGE data_stage;
    unsigned char complete;
    unsigned int txActual;
    unsigned int txCount;
    unsigned int rxActual;
    unsigned int rxCount;
    unsigned address;
}USB_EP0_REQ;

typedef struct {
    unsigned int channel;
    unsigned int burstmode;
    unsigned int ep_num;
    unsigned int dma_mode;
    unsigned int dma_reqmode;
    USB_DIR dir;
    unsigned char used;
}USB_DMA_REQ;

typedef struct {
    USB_TRANSFER_TYPE transfer_type;
    unsigned int fifoaddr;
    unsigned int fifosz;
    unsigned int maxp;
    USB_DIR dir;
    unsigned char shared;
    unsigned char enabled;
    unsigned short csr;
    int dpb;
}USB_EP_SETTING;

typedef struct 
{
    unsigned char tmp;
    unsigned char *buf;
    unsigned int actual;
    unsigned int count;
    unsigned int curr;
    //unsigned int current;
    unsigned char  complete;
    unsigned char isDma;
    unsigned int dmaChannel;
    unsigned char nextDma;
    unsigned char nextReq;
    unsigned char nextTx;
    unsigned int transferCount;
} USB_REQ;

typedef struct{
    USB_SPEED speed;
    USB_EP_SETTING ep_setting[TX_FIFO_NUM + RX_FIFO_NUM -1];
} USB_TEST_SETTING;

typedef struct
{
    void*           pData;
    #if 0 //!PORTING
    E_HDRC_NOTIFY   Cause;
    #endif
    unsigned int    nPipe;
    unsigned int    Status;
    unsigned char bIntrUsbValue;
    unsigned int dwDmaIntrValue;
    unsigned short wIntrTxValue;
    unsigned short wIntrRxValue;
    unsigned int wIntrQMUValue;
    #if 0 //!PORTING
    llist_t link;
    #endif
} BsrItem;


#ifndef UTEST_UNIUSB_C
extern void USB_hal_delay_ms ( unsigned int msecs );
extern void MGC_ResetPort(void);
extern void resetUSB(void);
#if 0
#if 0 //!PORTING
extern cisr_status_t HdrcIsrReset(unsigned int irq_no, void* pParam);
#else
extern irqreturn_t HdrcIsrReset(int irq_no, void* pParam);
#endif
#endif
extern int disableIntr(void);
extern int enableUsbIntr(void);
extern unsigned int getFIFOsz(unsigned int FIFOlen, int isDPB);
extern void SetAddress(int addr);
extern void SetHubAddress(int ep_num, int addr, USB_DIR dir);
//extern int EP0ControlRequest(PMUSB_DeviceRequest pDevReq);
extern unsigned char ProcHostDefaultEnd(unsigned int dwCount);
extern void QMU_HdrcLoadFifo(unsigned char nEnd, unsigned int dwCount, const unsigned char* pSource);
extern void QMU_HdrcUnloadFifo(unsigned char nEnd, unsigned int dwCount, unsigned char* pDest);
#endif


#ifdef UTEST_UNIUSB_C
#define AUTOEXT
#else 
#define AUTOEXT  extern
#endif
AUTOEXT volatile unsigned char bError;
AUTOEXT volatile unsigned char bStall;
AUTOEXT volatile int isConnIntr;
AUTOEXT volatile int isDisconIntr;
AUTOEXT volatile int countSOFIntr;
AUTOEXT volatile int isSOFIntr;
AUTOEXT volatile int isReset;
AUTOEXT volatile int isResumeIntr;

AUTOEXT volatile unsigned short wSetAddress;
AUTOEXT volatile unsigned char bTxInterval;
AUTOEXT volatile unsigned char bRxInterval;
AUTOEXT volatile unsigned char bEnd0Stage;
AUTOEXT volatile unsigned char bEnd0DataStage;
AUTOEXT volatile unsigned int SetupRxDataCount;
AUTOEXT volatile unsigned int SetupTxDataCount;
AUTOEXT volatile unsigned int SetupTxDataLen;
AUTOEXT volatile unsigned int SetupRxDataLen;
AUTOEXT volatile unsigned int SetupTxDataIndex;
AUTOEXT volatile unsigned int SetupRxDataIndex;
AUTOEXT volatile unsigned int dwMaxPacketSizeRx;
AUTOEXT volatile unsigned int dwMaxPacketSizeTx;
AUTOEXT volatile unsigned short wAssignAddress;
AUTOEXT volatile unsigned char bEndpoint;
AUTOEXT volatile unsigned char bDMAChannel;
AUTOEXT unsigned char SetupRxDataBuffer[65535];
AUTOEXT unsigned char SetupTxDataBuffer[65535];
AUTOEXT volatile unsigned int nBabble;
AUTOEXT volatile unsigned int nIncompRx;
AUTOEXT volatile unsigned int nPIDError;
AUTOEXT volatile unsigned int nDataError;
AUTOEXT volatile unsigned char bNonStop;
AUTOEXT volatile unsigned char bRetries;
AUTOEXT volatile unsigned int dwEP0MaxP;
AUTOEXT volatile unsigned char bStop;
AUTOEXT volatile unsigned char isSuspend;
AUTOEXT volatile unsigned char bSession;
#undef AUTOEXT
//static int maxp_list[] = {8, 16, 32, 64, 128, 256, 512, 1023, 1024, 2048, 3072, 4096, 65536};
#if 0 //!PORTING
static long _b[ ( CISR_MAX_SIZE + ( sizeof( long ) - 1 ) ) / sizeof( long ) ];
static cisr_t* hdrc_cisr = (cisr_t*)_b;
static long _b2[ ( CISR_MAX_SIZE + ( sizeof( long ) - 1 ) ) / sizeof( long ) ];
static cisr_t* hqmu_cisr = (cisr_t*)_b2;
volatile static hdrc_t hdrc;
volatile static hdrc_t hqmu;
static BsrItem BsrItemPool[NUM_BSR_ITEMS];
static BsrItem QMUBsrItemPool[NUM_BSR_ITEMS];

cisr_status_t HdrcIsrReset();
#endif
void resetUSB(void);
int disableIntr(void);


#endif
