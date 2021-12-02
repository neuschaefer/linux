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
#ifndef _UTEST_H_
#define _UTEST_H_

#include "musbdefs.h"

#if defined MUSB_DEV_USB2ETH
extern MUSB_ETHEpFifoDescriptor MUSB_aETHEpFifoDescriptors_forTest[AT_FIFOTEST_CONFIG_NUM];
#endif

typedef struct testParam_t {
    unsigned char bTxDMABurstMode;
    unsigned char bRxDMABurstMode;
    unsigned char bSrcBufMisalign;
    unsigned char bDestBufMisalign;
    unsigned char *pSrcAddr;
    unsigned char *pDestAddr;
    unsigned char bTestLenRandom;       // True: TestLen is Random, False: TestLen is Sequence
    unsigned char bTxEnd;
    unsigned char bRxEnd;
    unsigned char bInterval;                // NakLimit Timeout Test
    unsigned short wRxStallEpNum;       // RxStall Bit Test
    unsigned short wFlushFifoEpNum;      // FlushFifo Bit Test
    unsigned int dwTestCount;               // TestCount

    // USBPHY pause unit test configuration
    unsigned char bSuspendDev;                              // set TRUE for suspend USB device before pause test, else FALSE for NOT suspend USB device
    unsigned char bDataLoopbackTest;                     // set TRUE for data loopback test after pause done, else FALSE for NO data loopback test
    unsigned char bPMWakeUpEventTest;                 // set TRUE for PM wakeup event test (Auto), otherwise FALSE
    unsigned char bUSBWakeUpEventTest;               // set TRUE for USB wakeup event test (Auto), otherwise FALSE
    unsigned char bCIRQWakeUpEventTest;              // set TRUE for Intr wakeup event test (Auto), otherwise FALSE
    unsigned char bManualTest;                               // set TRUE for manual test (disconnect / connect device), otherwise FALSE
    unsigned char bRemoteWakeupEventTest;          // set TRUE for remote wakeup event test, otherwise FALSE
} testParam;

#if defined MUSB_DEV_USB2ETH
/****************************************************************************/
/* Internal Function                                                                                                             */
/****************************************************************************/
#if defined UTST_BASIC
//exc_t USB_t1501 (void *priv);
//exc_t USB_t1502 (void *priv);
#endif

#if defined UTST_STRESS

#endif

/****************************************************************************/
/* Test Function                                                                                                                 */
/****************************************************************************/
#if defined UTST_BASIC
//exc_t USB_UT_Reset(void *priv);
//exc_t USB_UT_Cpu_BulkLoopback(void *priv);
//exc_t USB_UT_Dma_BulkLoopback(void *priv);
#endif

#if defined UTST_STRESS

#endif

extern int dwceth_macAddr (unsigned int macNo,
                                                unsigned int isSet,
                                                unsigned char *mac0_p,
                                                unsigned char *mac1_p,
                                                unsigned char *mac2_p,
                                                unsigned char *mac3_p,
                                                unsigned char *mac4_p,
                                                unsigned char *mac5_p,
                                                unsigned char  macSetting);

extern void _mgc_hdrc_flush_end (hdrc_t* pCtrl, unsigned char bEnd);
extern unsigned int _mgc_packet_tx (hdrc_t* pCtrl, unsigned char bEnd, char *pBuffer, unsigned int dwTransSize);

// end of #if defined MUSB_DEV_USB2ETH

#elif defined MUSB_DEV_MASSSTORAGE

/* command block wrapper */
typedef struct bulk_cb_wrap_s {
    unsigned int Signature;                    /* contains 'USBC' */
    unsigned int Tag;                             /* unique per command id */
    unsigned int DataTransferLength;     /* size of data */
    unsigned char Flags;                       /* direction in bit 0 */
    unsigned char Lun;                          /* LUN normally 0 */
    unsigned char Length;                     /* of of the CDB */
    unsigned char opcode;
    unsigned int CDB[15];                     /* max command */
} bulk_cb_wrap_t;

#define US_BULK_CB_SIGN         0x43425355      /*spells out USBC */

extern int MSEPRequest (hdrc_t *pCtrl, bulk_cb_wrap_t *pCBW);

// end of #if defined MUSB_DEV_USB2ETH

#elif defined MUSB_DEV_7108
/****************************************************************************/
/* Internal Function                                                                                                             */
/****************************************************************************/

/****************************************************************************/
/* Test Function                                                                                                                 */
/****************************************************************************/
//exc_t USB_UT_Cpu_IsoIntrLoopback(void *priv);
//exc_t USB_UT_Dma_IsoIntrLoopback(void *priv);

#endif                  

//exc_t USB_start (void);

//exc_t APUSB_PTST_END (void *priv);

void USB_hal_delay_ms ( unsigned int msecs );

extern unsigned int MGC_SendRecvEP (hdrc_t* pCtrl, unsigned char bEnd, unsigned int bXmt,
                                    unsigned char* pBuffer, unsigned int dwLength, unsigned char bDMAMode);

extern unsigned short configure_fifo(hdrc_t* pCtrl, LocalEnd* pEnd, unsigned char bDir, 
                                    unsigned short wSize, unsigned char bDbe, unsigned short wFifoOffset);
    
extern int dwceth_nic_sendPkt(unsigned char *txData_p, unsigned int txPktLen);

extern int dwceth_nic_recvPkt(unsigned char **rxData_pp, unsigned int *rxPktLen_p);

extern int dwceth_nic_freeData (void);

extern int USB7108ControlRequest (hdrc_t *pCtrl, MUSB_DeviceRequest *pDevReq);
	
#endif
