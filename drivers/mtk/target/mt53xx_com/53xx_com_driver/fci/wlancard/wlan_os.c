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
/*
** $Log: wlan_os.c $
** Revision 1.15 jiun
**
*/

/*! \file wlan_os.c
    \brief subroutines to support Linux OS

    This is a set of subroutines.
*/

/************************************************************************
*                     C O M P I L E R   F L A G S
*************************************************************************
*/

#ifdef CC_FCI_ENABLE

#define UNUSED(x)               (void)x

#define CHK_TX_PKT_ID 1
#define POLLING_ISAR 0
#define INCREASE
//#undef INCREASE
#define SCANLEN
#define MRBS 1700  // Maxim RFB size
//#define STRESS_TEST_DISPLAY
//#define FIXED_TX_PATTERN
#define SDIO_ENHANCE    0

#define __LITTLE_ENDIAN__       0
#define __BIG_ENDIAN__          1
#ifndef __CPU_ENDIAN__
#define __CPU_ENDIAN__          __LITTLE_ENDIAN__
#endif

// Max Liao, wlan card porting, 2006/03/02.
#define KERN_ALERT

//#define WLAN_CLOCK_TEST // for different clock test

/************************************************************************
*                E X T E R N A L   R E F E R E N C E S
*************************************************************************
*/

#include "wlan_kal.h"
#include "basic.h"
#include "equates.h"
#include "mt5911hw.h"
#include "mt5911reg.h"

// Max Liao, wlan card porting, 2006/03/02.
#include "sdio_func.h"
#include "x_typedef.h"
#include "x_assert.h"
#include "x_ckgen.h"
#include "x_os.h"
/************************************************************************
*              F U N C T I O N   D E C L A R A T I O N S
*************************************************************************
*/
VOID do_isr_tasklet(IN UINT_32 obj);

/************************************************************************
*                          C O N S T A N T S
*************************************************************************
*/
#if POLLING_ISAR
    #ifdef MT5901IRQ
    #undef MT5901IRQ
    #endif
    #define MT5901IRQ 0
#endif

#ifdef FIXED_TX_PATTERN
#define TX_PATTERN 0xAA
#endif

/************************************************************************
*                        P U B L I C   D A T A
*************************************************************************
*/
UINT_32 u4LoopBackRx1OkCount = 0, u4LoopBackRx2OkCount = 0;
UINT_32 u4LoopBackTx1OkCount = 0, u4LoopBackTx2OkCount = 0;
UINT_32 u4LoopBackTxPacketCount= 0, u4LoopBackRxPacketCount= 0;
UINT_32 u4PayLoadLen = 0;
UINT_32 u4TestCount = 0;
PTFCB_STRUC pTfcb;
PTFCB_STATUS_STRUC  pTfcbStatus;
PRFB_STRUC  pRfb;
PRFB_STATUS_STRUC   pRfbStatus;
UINT_8 u1SecurityMode = 0;

#ifdef WLAN_HIF_HPI
UINT_32 x_membase;
#endif

// Max Liao, wlan card porting, 2006/03/02.
//#ifdef WLAN_HIF_SDIO
#if 0
volatile struct mmc_slot *x_slot;
#endif

#ifdef MODULE_LICENSE
MODULE_LICENSE("MediaTek License Agreement");
#endif

#if !POLLING_ISAR
/* Tasklet mechanism is like buttom-half in Linux. We just want to
 * send a signal to OS for interrupt defer processing. All resources
 * are NOT allowed reentry, so txPacket, ISR-DPC and ioctl must avoid preempty.
 */
// Max Liao, wlan card porting, 2006/03/02.
//DECLARE_TASKLET (wlan_hw_tasklet, do_isr_tasklet, 0);
#endif

/************************************************************************
*              F U N C T I O N   D E C L A R A T I O N S
*************************************************************************
*/

/*----------------------------------------------------------------*/
/*!
* \brief  The registered driver interrupt handler, also called top half in Linux.
*
* Simply acknowledge the interrupt and scheduled the bottom-half tasklet to
* continue with further processing of the interrupt.
* For some platform this ISR might handle all interrupt context within this function
* without spawning a new TASK as a deferred procedure call. Therefore our interrupt
* handler have a compiler option to decide whether to invoke the bottom half directly
* or defer the execution to tasklet.
*
* \param  irq             irq number, don't care here
* \param  regs            registers, don't care here
* \param  dev_id          pointer to i2220mgmt_t, the device's private structure
* \return none
* \sa     do_isr_tasklet
*/
/*----------------------------------------------------------------*/
#if !POLLING_ISAR
int mt5901hw_interrupt(void *dev_id)
{
    UINT_32 isr = 0;
    UINT_32 scr = 0;  // [Klockwork] UNINIT.STACK.MUST Analyze

    KDBG_ENTER;

    /* Read the SCR to check if the global interrupt mask bit is enabled. */
    NIC_MCR_READ(NULL, MCR_IER, &scr);

    /* Read Interrupt Status Register */
    NIC_MCR_READ(NULL, MCR_ISAR, &isr);

    //CON_MSG("i5326hw_interrupt, ISAR:0x%08X\n", isr);
    /* Strip off the disabled interrupts */
    //isr &= priv_p->interruptEnable;

    /* We want to process the interrupt if the output from our interrupt line
     * is high, and our interrupt is not masked.  If our interrupt line
     * is already masked, then we must be currently processing interrupts.
     */
    // [Klockwork] UNINIT.STACK.MUST Analyze
    if ((scr & IER_GLOBAL_ENABLE) && isr) {
        //nicDisableInterrupt(priv_p);
        NIC_MCR_WRITE(NULL, MCR_IER, 0x0020032f);

        /* Save interrupt status */
        //priv_p->interruptStatus = isr;
        // priv_p->interruptStatus |= (isr & priv_p->interruptEnable);
        KDBG_EXIT;
        return TRUE;
    }

    KDBG_EXIT;
    return FALSE;
}

/*----------------------------------------------------------------*/
/*!
* \brief
*
* \param
* \return
*/
/*----------------------------------------------------------------*/
//Mark for lint free, static void wlan_hw_interrupt(int irq, void *dev_id, struct pt_regs *regs)
static void wlan_hw_interrupt(int irq, void *dev_id)
{
    /* Check if this interrupt is ours. If it is true, issue the event to defer process. */
    if ( mt5901hw_interrupt(dev_id)) {
    #ifdef CONFIG_M5249
        #if HOSEA1
        /* Clear Interrupt of GPIO3 */
        *((volatile unsigned long *)(0x80000000+0xc0))=0x00000808;
        #elif HOSEA2
        /* Clear Interrupt of GPIO4 */
        *((volatile unsigned long *)(0x80000000+0xc0))=0x00001010;
        #endif
    #endif

    // Max Liao, wlan card porting, 2006/03/02.
    // activate ISR handler.
    do_isr_tasklet(0);
		/*
        wlan_hw_tasklet.data = (unsigned long) dev_id;
        tasklet_hi_schedule(&wlan_hw_tasklet);
        //tasklet_schedule(&wlan_hw_tasklet);
		*/
    }
}
#endif

#ifdef WLAN_HIF_SPI
void spiInit(PVOID pAdapter)
{
    // 1) Mode Select
    //unsigned char ucByte = (SPI_MS_LE | SPI_MS_MODE_8);
    //vSpiWriteBuffSync(&ucByte, 1);
    vSpiSetOpMode((SPI_MS_BE | SPI_MS_MODE_8));
}
#endif

void vInitMCR(void)
{
    unsigned long mcrValue = 0x00;    // [Klockwork] UNINIT.STACK.MUST Analyze
#ifdef WLAN_HIF_HPI
    unsigned short hpiCrValue;
    u32 addr = x_membase + MCR_HPI_CTRL;
#endif

    CON_MSG("TestMCR\n");

    /* init hif specific part */
#ifdef WLAN_HIF_HPI
    // 1) write 0x11
    hpiCrValue = (HPI_CTRL_SW_RST | HPI_CTRL_FIRST_HW);
    writew(hpiCrValue, addr);
    CON_MSG("W MCR_HPI_CTRL = 0x%04X\n", hpiCrValue);

    // 2) write 0x10
    hpiCrValue = HPI_CTRL_SW_RST;
    writew(hpiCrValue, addr);
    CON_MSG("W MCR_HPI_CTRL = 0x%04X\n", hpiCrValue);

    #if 0 //__CPU_ENDIAN__ == __BIG_ENDIAN__
        // 3)
        mcrValue = HPI_CTRL_BE;
        writew(mcrValue, addr);
        //CON_MSG("W MCR_HPI_CTRL = 0x%04X\n", mcrValue);
    #endif
    hpiCrValue = readw(addr);
    CON_MSG("R MCR_HPI_CTRL = 0x%04X\n", hpiCrValue);

    addr = x_membase + MCR_HPI_POWER_CTRL;
    hpiCrValue = readw(addr);
    CON_MSG("R MCR_HPI_POWER_CTRL = 0x%04X\n", hpiCrValue);

#endif

#ifdef WLAN_HIF_SPI
    spiInit(NULL);
#endif

    KDBG_LOG_INIT(KDBG_INFO, "initial mac Enable Clock\n");
    // initial mac Enable Clock
    NIC_MCR_WRITE(NULL, 0x00, 0x000F0000);
    kal_udelay(1);

    KDBG_LOG_INIT(KDBG_INFO, "MAC/BB Reset\n");
    // MAC/BB Reset
    NIC_MCR_WRITE(NULL, 0x00, 0x000F000F);
    kal_udelay(1);

    KDBG_LOG_INIT(KDBG_INFO, "Take effect in Normal Mode\n");
    // Take effect in Normal Mode
    NIC_MCR_WRITE(NULL, 0x00, 0x000F0003);

    kal_udelay(1);
    NIC_MCR_WRITE(NULL, MCR_PRBCR, 0x00000D0F);
    //NIC_MCR_READ(NULL, MCR_PRBCR, &mcrValue);
    //CON_MSG("MCR_PRBCR :0x%08X\n", mcrValue);

    // [Klockwork] UNINIT.STACK.MUST Analyze
    NIC_MCR_READ(NULL, MCR_SCR, &mcrValue);
    CON_MSG("R, MCR_SCR :0x%08X\n", mcrValue);

    NIC_MCR_READ(NULL, 0x20, &mcrValue);
    CON_MSG("R, 0x20 chip id :0x%08X\n", mcrValue);

    KDBG_LOG_INIT(KDBG_INFO, "Maximum Receive Buffer Size\n");
    // Maximum Receive Buffer Size
    NIC_MCR_WRITE(NULL, MCR_MRBS, 1900);
    NIC_MCR_READ(NULL, MCR_MRBS, &mcrValue);
    CON_MSG("R, MCR_MRBS, 0x%08X\n", mcrValue);

    //NIC_MCR_READ(NULL, 0x20, &mcrValue);
    //CON_MSG("R, 0x20 chip id :0x%08X\n", mcrValue);

#if 0
    //NIC_MCR_READ(NULL, 0x7FE, &mcrValue);
    mcrValue = readw(x_membase + 0x7FE);
    CON_MSG("R WatchDog 0x7FE, 0x%08X\n", mcrValue);
#endif

    // TSTCR
    NIC_MCR_WRITE(NULL, 0x140, 0x000000e4);
    NIC_MCR_READ(NULL, 0x140, &mcrValue);
    KDBG_LOG_INIT(KDBG_INFO, "R 0x140, 0x%08X\n", mcrValue);

    // AIFSR
    NIC_MCR_WRITE(NULL, 0x18c, 0x00011111);
    NIC_MCR_READ(NULL, 0x18c, &mcrValue);
    KDBG_LOG_INIT(KDBG_INFO, "R 0x18c, 0x%08X\n", mcrValue);

    // TQCWR
    NIC_MCR_WRITE(NULL, 0x18c, 0x00011111);
    NIC_MCR_READ(NULL, 0x18c, &mcrValue);
    KDBG_LOG_INIT(KDBG_INFO, "R 0x18c, 0x%08X\n", mcrValue);

    // AC0CWR
    NIC_MCR_WRITE(NULL, 0x194, 0x00030001);
    NIC_MCR_READ(NULL, 0x194, &mcrValue);
    KDBG_LOG_INIT(KDBG_INFO, "R 0x194, 0x%08X\n", mcrValue);

    // AC1CWR
    NIC_MCR_WRITE(NULL, 0x198, 0x00030001);
    NIC_MCR_READ(NULL, 0x198, &mcrValue);
    KDBG_LOG_INIT(KDBG_INFO, "R 0x198, 0x%08X\n", mcrValue);

    // AC2CWR
    NIC_MCR_WRITE(NULL, 0x19c, 0x00030001);
    NIC_MCR_READ(NULL, 0x19c, &mcrValue);
    KDBG_LOG_INIT(KDBG_INFO, "R 0x19c, 0x%08X\n", mcrValue);

    // AC3CWR
    NIC_MCR_WRITE(NULL, 0x1a0, 0x00030001);
    NIC_MCR_READ(NULL, 0x1a0, &mcrValue);
    KDBG_LOG_INIT(KDBG_INFO, "R 0x1a0, 0x%08X\n", mcrValue);

    // PPORCR0
    NIC_MCR_WRITE(NULL, 0x280, 0x00000012);
    NIC_MCR_READ(NULL, 0x280, &mcrValue);
    KDBG_LOG_INIT(KDBG_INFO, "R 0x280, 0x%08X\n", mcrValue);

    // PPORCR1
    NIC_MCR_WRITE(NULL, 0x284, 0x00520052);
    NIC_MCR_READ(NULL, 0x284, &mcrValue);
    KDBG_LOG_INIT(KDBG_INFO, "R 0x284, 0x%08X\n", mcrValue);

    // PPDRCR0
    NIC_MCR_WRITE(NULL, 0x288, 0x00780000);
    NIC_MCR_READ(NULL, 0x288, &mcrValue);
    KDBG_LOG_INIT(KDBG_INFO, "R 0x288, 0x%08X\n", mcrValue);

    // PPDRCR1
    NIC_MCR_WRITE(NULL, 0x28C, 0x00500078);
    NIC_MCR_READ(NULL, 0x28C, &mcrValue);
    KDBG_LOG_INIT(KDBG_INFO, "R 0x28C, 0x%08X\n", mcrValue);

    // PPDRCR2
    NIC_MCR_WRITE(NULL, 0x290, 0x00000078);
    NIC_MCR_READ(NULL, 0x290, &mcrValue);
    KDBG_LOG_INIT(KDBG_INFO, "R 0x290, 0x%08X\n", mcrValue);

    // PPDRCR3
    NIC_MCR_WRITE(NULL, 0x294, 0x50000000);
    NIC_MCR_READ(NULL, 0x294, &mcrValue);
    KDBG_LOG_INIT(KDBG_INFO, "R 0x294, 0x%08X\n", mcrValue);

    // MPDR
    NIC_MCR_WRITE(NULL, 0x400, 0x00703070);
    NIC_MCR_READ(NULL, 0x400, &mcrValue);
    KDBG_LOG_INIT(KDBG_INFO, "R 0x400, 0x%08X\n", mcrValue);

    // EPDR
    NIC_MCR_WRITE(NULL, 0x40c, 0x03e80070);
    NIC_MCR_READ(NULL, 0x40c, &mcrValue);
    KDBG_LOG_INIT(KDBG_INFO, "R 0x40c, 0x%08X\n", mcrValue);

    // CRFCR0 ACK Rate
    NIC_MCR_WRITE(NULL, 0x410, 0x39ce739c);
    NIC_MCR_READ(NULL, 0x410, &mcrValue);
    KDBG_LOG_INIT(KDBG_INFO, "R 0x410, 0x%08X\n", mcrValue);

    // CRFCR1 ACK Rate
    NIC_MCR_WRITE(NULL, 0x414, 0x39ce739c);
    NIC_MCR_READ(NULL, 0x414, &mcrValue);
    KDBG_LOG_INIT(KDBG_INFO, "R 0x414, 0x%08X\n", mcrValue);

    // DTR
    NIC_MCR_WRITE(NULL, 0x418, 0x90b0b92c);
    NIC_MCR_READ(NULL, 0x418, &mcrValue);
    KDBG_LOG_INIT(KDBG_INFO, "R 0x418, 0x%08X\n", mcrValue);

    // NRTR
    NIC_MCR_WRITE(NULL, 0x474, 0x00020010);
    NIC_MCR_READ(NULL, 0x474, &mcrValue);
    KDBG_LOG_INIT(KDBG_INFO, "R 0x474, 0x%08X\n", mcrValue);

    CON_MSG("TestMCR OK\n");
}

/*_________________________________________________________________________
**  calcUINT_8Sum
**
**  descriptions: Calculate sum of UINT_8
**  parameters: addr_p, starting address of UINT_8
**              len, byte count for byte sum calculation
**  return: sum of the input UINT_8 stream
**  note:
**__________________________________________________________________________*/
UINT_8
calcUINT_8Sum(PUINT_8 addr_p, UINT_8 len )
{
    UINT_8 temp8 = 0x00,i;

    for ( i = 0; i < len; i++ ) {
        temp8 += *(addr_p + i);
    }
    return temp8;
}

VOID vAdjustTfcb(PTFCB_STRUC hwTfcb_p)
{
}

/*_________________________________________________________________________
**  txTFCBInit
**
**  descriptions: Make a complete TFCB structure.
**  parameters: tfcb_p:      point to TFCB struture
**              payloadLen:  payload length, bytes.
**  return: True or False
**  note:
**__________________________________________________________________________*/
BOOLEAN txTFCBInit(PTFCB_STRUC tfcb_p, UINT_16 payloadLen, UINT_8 securityMode,
                UINT_8 MACaddr, UINT_8 DesAddr, UINT_8 Rate )
{
    PWLAN_MAC_HEADER_A4_QOS_STRUC pMacHeader;
    UINT_16   i,padLen = 0;
    PINT_8   temp8_p ;
    int      tfcbTotalLen;
    PTFCB_STRUC  tfcb_temp_p;
    PINT_32  temp32_p;
//    PUINT_16  temp16_p;
    static  UINT_16 seqNo = 0;

    UINT_16  ofdmTxRate[12]={RATE_1M,RATE_2M,RATE_5_5M,RATE_11M,RATE_6M,RATE_9M,
                        RATE_12M,RATE_18M,RATE_24M,RATE_36M,RATE_48M,RATE_54M};

    UNUSED(temp32_p);

    /* allocate memory for TFCB */
    tfcb_temp_p = (PTFCB_STRUC)kal_malloc_sz(
                    2000 + 4, KAL_MALLOC_FLAG);                 /* add 4-bytes for safe margin */
    if (NULL == tfcb_temp_p) {
        CON_MSG("Failed to allocate tfcb_temp_p memory.\n");
        return FALSE;
    }

    memset(tfcb_temp_p , 0, 2000 + 4);

    switch(securityMode)
    {
       case WEP_40:
       case WEP_104:
       case WEP_128:
            tfcbTotalLen =  payloadLen + ( (4-(payloadLen%4))%4 ) +  80 + 32 + 12 + 4  ; // 4-byte boudary,IV
            break;
       case CCMP:
       case TKIP_ON_THE_FLY:
            tfcbTotalLen =  payloadLen + ( (4-(payloadLen%4))%4 ) +  80 + 32 + 12 + 8 ; // IV, EIV
            break;
       case TKIP_MIC:
            padLen = Rate;
            tfcbTotalLen =  payloadLen + ( (4-(payloadLen%4))%4 ) +  80 + 32 + 12  ;
            break;
       default:
            //Non security
            tfcbTotalLen =  payloadLen + ( (4-(payloadLen%4))%4 ) +  80 + 32 + 12; // 4-byte boudary
            break;
    }

    /*TFD*/
    //tfcb_temp_p->u_tcf.s.tfdChecksum = 0x00;
    tfcb_temp_p->tfdChecksum = 0x00;
    tfcb_temp_p->txFrameLength = tfcbTotalLen;

    tfcb_temp_p->txFrameLength |= 0x2000 ; //AC0 Que.

    if(securityMode == TKIP_MIC){
        //tfcb_temp_p->u_tcf.s.tfdCtrlFlag = 0x10;  /* SEC  engine enable */
        tfcb_temp_p->tfdCtrlFlag = 0x10;  /* SEC  engine enable */
    }else {
        //tfcb_temp_p->u_tcf.s.tfdCtrlFlag = 0x00;
        tfcb_temp_p->tfdCtrlFlag = 0x00;
    }

    if (payloadLen == 0){
        tfcb_temp_p->tfdCtrlFlag |= 0x02;
    }

    tfcb_temp_p->txPacketID = (UINT_16)seqNo ;      /* Use seqNo as packetID */

    /*SCI*/
    tfcb_temp_p->key1 = 0x11111111;
    tfcb_temp_p->key2 = 0x11111111;
    tfcb_temp_p->key3 = 0x11111111;
    tfcb_temp_p->key4 = 0x11111111;

    if ( (securityMode == WEP_128) || (securityMode == WEP_104)
         || (securityMode == WEP_40) ){
        if (securityMode == WEP_104) {
            tfcb_temp_p->secOpMode = 0x05 ; /* WEP 104 */
            tfcb_temp_p->sciIv     = 0x40000000;     /* set SCI IV ,key ID 1*/
            tfcb_temp_p->IV_wep    = 0x40000000;     /* IV, key ID 1 */
        }else if (securityMode == WEP_128){
            tfcb_temp_p->secOpMode =0x06 ; /* WEP 128 */
            tfcb_temp_p->sciIv     = 0x80000000;  /* set SCI IV, key ID2  */
            tfcb_temp_p->IV_wep    = 0x80000000;     /* IV, key ID2 */
        }else{
            tfcb_temp_p->secOpMode =0x01 ; /* WEP 40 */
            tfcb_temp_p->sciIv     = 0x00000000;  /* set SCI IV */
            tfcb_temp_p->IV_wep    = 0x00000000;     /* IV */
        }
        tfcb_temp_p->dmaFifoHeaderLen = 0x12; /* WEP */
        tfcb_temp_p->mpduLen = (UINT_16)payloadLen + 24 + 4 + IV_LEN  +  ICV_LEN;

    }else if ( securityMode == TKIP_ON_THE_FLY){
        tfcb_temp_p->secOpMode =0x07 ; /* tkip on the fly*/
        tfcb_temp_p->dmaFifoHeaderLen = 0x13;
        tfcb_temp_p->mpduLen = (UINT_16)payloadLen + 24 + 4
                                 + IV_LEN  + EIV_LEN +ICV_LEN + MIC_LEN;
        tfcb_temp_p->sciIv  = MIC_KEY0;             /* set mic key 0 */
        tfcb_temp_p->sciExtendedIv  = MIC_KEY1;     /* set mic key 1  */
        tfcb_temp_p->IV_wep  = 0xA0000000;          /* set IV, key ID2  */
        tfcb_temp_p->EIV_wep = 0x22222222;               /* set wep EIV    */
    }else if ( securityMode == CCMP){
        tfcb_temp_p->secOpMode =0x04 ; /*CCMP*/
        tfcb_temp_p->dmaFifoHeaderLen = 0x13;
        tfcb_temp_p->mpduLen = (UINT_16)payloadLen + 24 + 4
                                 + IV_LEN  + EIV_LEN + MIC_LEN ;
        tfcb_temp_p->sciIv   = 0xE0000000;
        tfcb_temp_p->sciExtendedIv  = 0x22222222;
        tfcb_temp_p->IV_wep  = 0xE0000000;            /* IV, key ID3 */;
        tfcb_temp_p->EIV_wep = 0x22222222;               /* set wep EIV    */
    }else if ( securityMode == TKIP_MIC){
        tfcb_temp_p->secOpMode =0x0A ; /* tkip on the fly*/
        tfcb_temp_p->dmaFifoHeaderLen = 0x11;
        tfcb_temp_p->mpduLen = (UINT_16)payloadLen + 24 + 4 ;
                                  //+ICV_LEN + MIC_LEN;
        tfcb_temp_p->sciIv  = MIC_KEY0;             /* set mic key 0 */
        tfcb_temp_p->sciExtendedIv  = MIC_KEY1;     /* set mic key 1  */
        tfcb_temp_p->key1 = MIC_KEY0;
        tfcb_temp_p->key2 = MIC_KEY1;
    }else {
        tfcb_temp_p->dmaFifoHeaderLen = 0x11; /* No security */
        tfcb_temp_p->mpduLen = (UINT_16)payloadLen + 24 +4 ;
    }

    /*TCI*/
    if (((Rate-1) < (sizeof(ofdmTxRate) / sizeof(UINT_16))) && ((Rate-1) >= 0))  // [Klockwork] ABR Analyze
    {
        tfcb_temp_p->txRates = ofdmTxRate[Rate-1];
    }
    tfcb_temp_p->txPower = 0x50;

    tfcb_temp_p->totalTxDataLen = (UINT_16)payloadLen;
     //tfcb_temp_p->multirateSwCtrl = 3;

     /* MHI*/
    tfcb_temp_p->macHeaderLen = 24;

    // ONLY FOR LOOP-BACK TEST
    tfcb_temp_p->durationId2 = (UINT_16)payloadLen ;
    tfcb_temp_p->reserved5 = 0x00;
    tfcb_temp_p->reserved5 = 0xff - calcUINT_8Sum(&tfcb_temp_p->macHeaderLen, 4);

#if 1
    pMacHeader = (PWLAN_MAC_HEADER_A4_QOS_STRUC)&tfcb_temp_p->frameCtrl;

    pMacHeader->frameCtrl = 0x08;

    if( (securityMode > 0) && (securityMode < 10)){
        pMacHeader->frameCtrl |= BIT(14);       //WEP , security
    }
    //tfcb_temp_p->duration_ID1;
    //A1
    pMacHeader->addr1[0] = 0x00 ;
    pMacHeader->addr1[1] = 0x08 ;
    pMacHeader->addr1[2] = 0x22 ;
    pMacHeader->addr1[3] = 0x00 ;
    pMacHeader->addr1[4] = 0x00 ;
    pMacHeader->addr1[5] = DesAddr;

    //A2
    pMacHeader->addr2[0] = 0x00 ;
    pMacHeader->addr2[1] = 0x08 ;
    pMacHeader->addr2[2] = 0x22 ;
    pMacHeader->addr2[3] = 0x00 ;
    pMacHeader->addr2[4] = 0x00 ;
    pMacHeader->addr2[5] = MACaddr;

    //A2
    pMacHeader->addr3[0] = 0x45 ;
    pMacHeader->addr3[1] = 0x45 ;
    pMacHeader->addr3[2] = 0x45 ;
    pMacHeader->addr3[3] = 0x45 ;
    pMacHeader->addr3[4] = 0x45 ;
    pMacHeader->addr3[5] = 0x45;

    //temp16_p = (PUINT_16)(&(pLocalTfcb->macHeader[18]) );
    //temp16_p[0] = ( seqNo << 4 );
    pMacHeader->seqCtrl = seqNo << 4;
#else
    tfcb_temp_p->frameCtrl = 0x08;       //MAC_FRAME_TYPE_DATA
    if( (securityMode > 0) && (securityMode < 10)){
        tfcb_temp_p->frameCtrl |= BIT(14);       //WEP , security
    }
    //tfcb_temp_p->duration_ID1;

    #if 1
    //A1
    tfcb_temp_p->macHeader[0] = 0x00 ;
    tfcb_temp_p->macHeader[1] = 0x08 ;
    tfcb_temp_p->macHeader[2] = 0x22 ;
    tfcb_temp_p->macHeader[3] = 0x00 ;
    tfcb_temp_p->macHeader[4] = 0x00 ;
    tfcb_temp_p->macHeader[5] = DesAddr;
    #else
    tfcb_temp_p->macHeader[0] = 0x08 ;
    tfcb_temp_p->macHeader[1] = 0x00 ;
    tfcb_temp_p->macHeader[2] = 0x00 ;
    tfcb_temp_p->macHeader[3] = 0x22 ;
    tfcb_temp_p->macHeader[4] = 0x02 ;
    tfcb_temp_p->macHeader[5] = 0x00;
    #endif
    //A2
    tfcb_temp_p->macHeader[6] = 0x00 ;
    tfcb_temp_p->macHeader[7] = 0x08 ;
    tfcb_temp_p->macHeader[8] = 0x22 ;
    tfcb_temp_p->macHeader[9] = 0x00 ;
    tfcb_temp_p->macHeader[10] = 0x00 ;
    tfcb_temp_p->macHeader[11] = MACaddr;
    for (i=0; i<5; i++) {
        //tfcb_temp_p->macHeader[i+1] = 0x77;        //A1
        //tfcb_temp_p->macHeader[i+7] = 0x66;   //A2
        tfcb_temp_p->macHeader[i+13] = 0x45;   //A3
    }

    temp16_p = (PUINT_16)(&(tfcb_temp_p->macHeader[18]) );
    temp16_p[0] = ( seqNo << 4 );
#endif

    /*tbb*/
    tfcb_temp_p->tbbCheckSum  = 0x00;
    tfcb_temp_p->tbbEOL       = 0x01;

    // Max Liao, wlan card porting, 2006/03/02.
    tfcb_temp_p->tbbBufferLen = (UINT_16)payloadLen;
    // !!!FOR ENDIAN SAFE!!!
    //tfcb_temp_p->tbbBufferLen = host2chip_16((UINT_16)payloadLen);

    tfcb_temp_p->reserved6[0] = 0x11111111;
    tfcb_temp_p->reserved6[1] = 0x11111111;   //end of tfcb

    temp8_p = (PINT_8)((PUINT_8 )tfcb_temp_p + sizeof (TFCB_STRUC));
    for (i = 0 ; i < payloadLen; i++ ){
    #ifdef FIXED_TX_PATTERN
        temp8_p[i] = TX_PATTERN;
    #else
        temp8_p[i] = (UINT_8) i;
    #endif
    }

    if (securityMode == TKIP_MIC){
        temp8_p[payloadLen - 4 - padLen - 1 ] = 0x5a;
        for (i= (payloadLen - 4 - padLen); i < payloadLen; i++){
            temp8_p[i]  = 0x00;
        }
    }

    switch(securityMode)
    {
        case WEP_40:
        case WEP_104:
        case WEP_128:
                memcpy (tfcb_p,tfcb_temp_p, WEP_TFD_LEN);
                temp8_p = (PINT_8)((PUINT_8)tfcb_p + WEP_TFD_LEN);
                memcpy (temp8_p,&(tfcb_temp_p->tbbEOL), TBB_TBD_SIZE+payloadLen);
            break;
        case CCMP:
        case TKIP_ON_THE_FLY:
            memcpy (tfcb_p,tfcb_temp_p, TKIP_TFD_LEN + TBB_TBD_SIZE+payloadLen);
            break;
        default:
            //Non security
            memcpy (tfcb_p, tfcb_temp_p, NonSec_TFD_LEN);
            temp8_p = (PINT_8)((PUINT_8)tfcb_p + NonSec_TFD_LEN);
            memcpy (temp8_p, &(tfcb_temp_p->tbbEOL), TBB_TBD_SIZE + payloadLen);
            break;
    }

    //SDFreeMemorytfcb_temp_p;
    kal_free(tfcb_temp_p);

    temp32_p = (PINT_32)((PUINT_32 ) (tfcb_p));
    #ifdef DUMP_TFCB
    //temp32_p = (PUINT_32 ) (tfcb_p);
    DbgPrintZo(SDCARD_ZONE_INIT, (TEXT("  The #%04d  TFCB content : \n"),seqNo));
    DbgPrintZo(1, (TEXT("\n========================\n")));
    for (i =0 ; i< (tfcbTotalLen+4)/4 ; i++){
        DbgPrintZo(SDCARD_ZONE_INIT, (TEXT("0x%04X : 0x%08X \n"),i*4,temp32_p[i]));
    }
    DbgPrintZo(1, (TEXT("\n========================\n")));
    #endif

    #ifdef SDRAM_SIMULATION_PATTERN
      DbgPrintZo( SDCARD_ZONE_INIT,(TEXT("//The #0x%04X  TFCB content\n"), seqNo));
      temp16_p = (PUINT_16) (tfcb_p);
      for (i = 0 ; i < (tfcbTotalLen / 2) + 1 ; i++){
          DbgPrintZo( SDCARD_ZONE_INIT,(TEXT("write     0x0600    0x%04X\n"), temp16_p[i+1]));

      }
    #endif

    seqNo++;

    return TRUE;
}

BOOLEAN
txOKProcess( UINT_32 intStatus)
{
    int                   j,loopCount;
    static UINT_16        packetID = 0;
#ifdef DUMP_TX_STATUS
    UINT_16               i;
#endif

    loopCount = 0;
    if ( intStatus & ISAR_TX1_OK_INT){
        CON_MSG("ISAR_TX1_OK_INT\n");
        loopCount++;
        u4LoopBackTx1OkCount++;
    }

    if (intStatus & ISAR_TX2_OK_INT){
        CON_MSG("ISAR_TX2_OK_INT\n");
        loopCount++;
        u4LoopBackTx2OkCount++;
    }

    for (j= 0 ; j< loopCount ; j++){
        /* get Tx_staus */
        //CON_MSG("READ TX_STATUS\n");
        memset(pTfcbStatus, 0, sizeof(TFCB_STATUS_STRUC));
        SDIO_ReadPort((PUINT_8)pTfcbStatus, sizeof(TFCB_STATUS_STRUC), TX_STATUS_PORT);

#if defined(WLAN_HIF_SDIO) && !SDIO_ENHANCE
        // set READ DONE.
        NIC_MCR_WRITE(adapter_p, MCR_SDRDR, BIT(0));
#endif

        //HEX_DUMP_BODY(pTfcbStatus, pTfcbStatus, sizeof(TFCB_STATUS_STRUC));

#if __CPU_ENDIAN__ == __BIG_ENDIAN__
        pTfcbStatus->status = chip2host_16(pTfcbStatus->status);
        pTfcbStatus->packetID = chip2host_16(pTfcbStatus->packetID);
#endif

        #if CHK_TX_PKT_ID
        if (pTfcbStatus->status != BIT(0)){
           CON_MSG("No tx-done, tx status:0x%04X \n", pTfcbStatus->status);
           HEX_DUMP_BODY(pTfcbStatus, pTfcbStatus, sizeof(TFCB_STATUS_STRUC));
        }

        if (pTfcbStatus->packetID != packetID){
            CON_MSG("ID mismatch, pktID: 0x%X  ,tx staus pktID: 0x%X \n", packetID, pTfcbStatus->packetID);
            HEX_DUMP_BODY(pTfcbStatus, pTfcbStatus, sizeof(TFCB_STATUS_STRUC));
        }else{
            //CON_MSG("tx status pktID: 0x%X \n", pTfcbStatus->packetID);
        }
        packetID++;
        #endif
    }
    return TRUE;

}

/*_________________________________________________________________________
**  parseRxPacket
**
**  descriptions: parse received rate, preamble type, source MAC Address.
**                RSSI, noise floor..etc
**  parameters: pClientInfo: point to gloabal variable struture
**  return: True or False
**  note:
**__________________________________________________________________________*/
BOOLEAN parseRxPacket(void)
{
    UINT_8    temp8,rxrate,preambletype,rssi,noiseFloor;
    UINT_8    LQ[6] = {0};  // [Klockwork] UNINIT.STACK.ARRAY.MUST Analyze

    UNUSED(rxrate);
    UNUSED(preambletype);
    UNUSED(rssi);
    UNUSED(LQ[0]);
    UNUSED(noiseFloor);

    temp8 =  pRfbStatus->bbCR63;
    temp8 &= 0xc0;
    switch(temp8){
      case 0x00:
      	    rxrate = 0;       //in unit of 0.5M bps
      	    break;
          case 0x40:
      	    rxrate = 1;       //in unit of 0.5M bps
      	    break;
          case 0x80:
      	    rxrate = 2;       //in unit of 0.5M bps
      	    break;
          case 0xc0:
      	    rxrate = 3;       //in unit of 0.5M bps
      	    break;
      default:
      	rxrate = pRfbStatus->bbCR63;
      	break;
      }
    temp8 =  pRfbStatus->bbCR63;
    temp8 &= 0x10;
    if (temp8 == 0x10) {
        preambletype = 1;         //Long preamble
    } else {
        preambletype = 0;         //Short preamble
    }

    rssi = pRfbStatus->bbCR62;           //BB vga & LNA mode,CR62
    noiseFloor = pRfbStatus->bbCRA;     // BBCR 56, noise floor
    LQ[5] = pRfbStatus->bbCR89 ;                               // LQ
    LQ[4] = pRfbStatus->bbCR90 ;                               // LQ
    LQ[3] = pRfbStatus->bbCR91 ;                               // LQ
    LQ[2] = pRfbStatus->bbCR92 ;                               // LQ
    LQ[1] = pRfbStatus->bbCR93 ;                               // LQ
    LQ[0] = pRfbStatus->bbCR94 ;                               // LQ

    return TRUE;
}

BOOLEAN
rxOKProcess(UINT_32 intStatus)
{
    int                   j,loopCount;
    UINT_32 rfbByteCount, busByteCount, i;
    PUINT_8 pu8Payload;
    //BOOLEAN rDataErrorFlag = FALSE;

    loopCount = 0;

    if ( intStatus & ISAR_RX1_OK_INT){
        loopCount++;
        u4LoopBackRx1OkCount++;
    }

    if (intStatus & ISAR_RX2_OK_INT){
        loopCount++;
        u4LoopBackRx2OkCount++;
    }

    for (j= 0; j < loopCount; j++){
        u4LoopBackRxPacketCount++;
        if( (u4LoopBackRxPacketCount > u4TestCount) &&
             (u4LoopBackTxPacketCount == 0 ) ){ /* finish Rx test */
            return TRUE;
        }

        /* get RFB_staus */
        KDBG_LOG_RX(KDBG_DEBUG, "READ RFB_STATUS\n");
        memset(pRfbStatus, 0, sizeof(RFB_STATUS_STRUC));
        SDIO_ReadPort((PUINT_8)pRfbStatus, sizeof(RFB_STATUS_STRUC), RX_STATUS_PORT);

#if defined(WLAN_HIF_SDIO) && !SDIO_ENHANCE
        // set READ DONE.
        NIC_MCR_WRITE(adapter_p, MCR_SDRDR, BIT(1));
#endif

        // RFB_STATUS_STRUC
        //HEX_DUMP_BODY(pRfbStatus, pRfbStatus, sizeof(RFB_STATUS_STRUC));

#if __CPU_ENDIAN__ == __BIG_ENDIAN__
        pRfbStatus->rxPortByteCount = chip2host_16(pRfbStatus->rxPortByteCount);
        pRfbStatus->rxStatus = chip2host_32(pRfbStatus->rxStatus);
        pRfbStatus->rxBytecount = chip2host_32(pRfbStatus->rxBytecount);
        pRfbStatus->rxSecStatus = chip2host_32(pRfbStatus->rxSecStatus);
#endif

        //temp32_p = (PUINT_32 ) pRfbStatus;
        //rfbByteCount = chip2host_32(temp32_p[5]); /* 4 bytes for seq No */  //12/18 modify
        rfbByteCount = pRfbStatus->rxBytecount;
        //temp32 = chip2host_32(temp32_p[4]) ;           /* Rx status */

        //if ( (temp32 & BIT(0))){
        //if (pRfbStatus->rxStatus & BIT(0)){
        if (pRfbStatus->rxSecStatus & BIT(31)){
            // Rx Done
            #ifdef STRESS_TEST_DISPLAY
            if ((u4LoopBackRxPacketCount % 100) == 0){
                CON_MSG("Rx counter : 0x%08X  RFB ID : 0x%02X Status= RX Done\n",
                    u4LoopBackRxPacketCount, pRfbStatus->hwSeqNo);
            }
            #else
            //CON_MSG("Rx counter : 0x%08X  RfbSeq: 0x%02X, RX Done\n",
            //    u4LoopBackRxPacketCount, pRfbStatus->hwSeqNo);
            #endif
        //}else if (temp32 & BIT(1)){
        }
        else
        {
            CON_MSG("No Rx Done (SecStatus)!\n");
            if (pRfbStatus->rxStatus & BIT(1)){
                // FCS Error
                CON_MSG("RfbSeq: 0x%02X, RX FCS Err\n",
                    pRfbStatus->hwSeqNo);
            }
        }

        if (pRfbStatus->rxSecStatus & BITS(8, 12)){
            CON_MSG("Sec Status Error:0x%08X!\n", pRfbStatus->rxSecStatus);
            if (pRfbStatus->rxSecStatus & BIT(8))
                CON_MSG("RxKeyNotFountErr!\n");
            if (pRfbStatus->rxSecStatus & BIT(9))
                CON_MSG("RxKeyDisableErr!\n");
            if (pRfbStatus->rxSecStatus & BIT(10))
                CON_MSG("RxICVErr!\n");
            if (pRfbStatus->rxSecStatus & BIT(11))
                CON_MSG("Sec FormatErr!\n");

            HEX_DUMP_BODY(pRfbStatus, pRfbStatus, sizeof(RFB_STATUS_STRUC));
            //return FALSE;
        }else{
             #ifdef STRESS_TEST_DISPLAY
                if ((u4LoopBackRxPacketCount % 100) == 0){
                    //CON_MSG("Sec Status  :0x%X\n", pRfbStatus->rxSecStatus);
                }
             #else
             //CON_MSG("//Sec Status  :0x%08X\n", temp32);
             #endif
        }

        if (pRfbStatus->rxSecStatus & BITS(0, 3)){
            rfbByteCount -=  8 ;  //security done,WEP mode
            if( (pRfbStatus->rxSecStatus & BITS(0, 3)) == 0x00000004){
                rfbByteCount -=  8 ;  //CCMP mode
            }
            if((pRfbStatus->rxSecStatus & BITS(0, 3)) == 0x00000007){
                rfbByteCount -=  12 ;  //TKIP mode
            }
        }

        //busByteCount = rfbByteCount + 4 + 4; // HW seq + FCS
        busByteCount = pRfbStatus->rxPortByteCount;
        busByteCount += (4 - (busByteCount % 4)) % 4;

        //CON_MSG("Port Count:%d, Rx Byte Count:%ld, Bus Byte Count:%ld\n",
        //    pRfbStatus->rxPortByteCount, pRfbStatus->rxBytecount, busByteCount);

        memset(pRfb, 0, 2004);
        if (pRfbStatus->rxPortByteCount & 0x03)
        {
            do{
                unsigned long mcrValue;
                NIC_MCR_READ(NULL, 0x20, &mcrValue);
            }while(0);
            CON_MSG("Port Count:%d, Rx Byte Count:%ld, Bus Byte Count:%ld\n",
                pRfbStatus->rxPortByteCount, pRfbStatus->rxBytecount, busByteCount);
            HEX_DUMP_BODY(pRfbStatus, pRfbStatus, sizeof(RFB_STATUS_STRUC));
        }

        if ((busByteCount > MRBS + 32 + 4) || (busByteCount < 24 )
            || (rfbByteCount > MRBS + 32) || (rfbByteCount < 24 )){  /* worong bytecount */
            do{
                unsigned long mcrValue;
                NIC_MCR_READ(NULL, 0x20, &mcrValue);
            }while(0);

            /* byte count too large. Error*/
            CON_MSG("rx byte Count Error ! => bus:0x%08lx, rfb:0x%08X\n", busByteCount, rfbByteCount);
            HEX_DUMP_BODY(pRfbStatus, pRfbStatus, sizeof(RFB_STATUS_STRUC));

            if ((busByteCount > MRBS + 32 + 4) || (busByteCount < 24) || (busByteCount % 4 != 0))
            {
                busByteCount = 100;   // make a fixed bytecount to follow rx
                CON_MSG("rx move busByteCount:%ld\n", busByteCount);
            }
            else
                CON_MSG("rx move busByteCount:%ld\n", busByteCount);

             /* get RFB buffer */
            SDIO_ReadPort((PUINT_8)pRfb, busByteCount, RX_DATA_PORT);
            HEX_DUMP_BODY(pRfb, pRfb, busByteCount);
            return FALSE;
        }
        else
        {
            /* get RFB buffer */
            KDBG_LOG_RX(KDBG_DEBUG, "READ RFB\n");
            SDIO_ReadPort((PUINT_8)pRfb, busByteCount, RX_DATA_PORT);
            //HEX_DUMP_BODY(pRfb, pRfb, busByteCount);
        }

#if defined(WLAN_HIF_SDIO) && !SDIO_ENHANCE
        // set READ DONE.
        NIC_MCR_WRITE(adapter_p, MCR_SDRDR, BIT(2));
#endif

        if (pRfbStatus->hwSeqNo !=  pRfb->hwSeqNo){
            CON_MSG("Rx status/RFB ID mismatch . status ID :0x%x     RFB ID:0x%x \n",
                pRfbStatus->hwSeqNo, pRfb->hwSeqNo);
            u4TestCount = 1;
            return FALSE;
        }

        // compare Rx Payload
        pu8Payload = (PUINT_8 )pRfb + sizeof (RFB_STRUC);

        for (i = 0 ; i < rfbByteCount - 32; i++ ){
        //for (i = 0 ; i < rfbByteCount - 4; i++ ){
#ifdef FIXED_TX_PATTERN
            if (pu8Payload[i] != TX_PATTERN)
#else
            if (pu8Payload[i] != (UINT_8) i)
#endif
            {
                CON_MSG("Rx Data Error\n");
                HEX_DUMP_BODY(pRfb, pRfb, busByteCount);
                u4TestCount = 1;
                return FALSE;
            }
            else
            {
                //CON_MSG("Rx Data OK\n");
            }
        }

        /*write Rx buffer done here  ,for david sim*/
        //IPC_2128_MCR_Write(pDevice,MCR_SDRDR,  SDRDR_RX_DATA_READ_DONE);
        #ifdef STRESS_TEST_DISPLAY
        if((u4LoopBackRxPacketCount % 100) == 0){
               /* parsing RFB */
                parseRxPacket();
            }
        #else
           /* parsing RFB */
            parseRxPacket();
        #endif
    }
    return TRUE;
}

BOOLEAN
txEmptyProcess(void)
{
    UINT_16 payLoadLen = 0;

    if ( u4LoopBackTxPacketCount < (u4TestCount-1))
    {
        u4LoopBackTxPacketCount++;

#ifdef INCREASE
        if (u4PayLoadLen == 1700){
            u4PayLoadLen = 32;
        }else{
            u4PayLoadLen++;
        }
#else
        if (u4PayLoadLen == 32){
            u4PayLoadLen = 1700;
        }else{
            u4PayLoadLen--;
        }
#endif
        payLoadLen = (UINT_16)u4PayLoadLen;

        //pDevice->testPayloadLen = (rand() % 1531 )+1;
        txTFCBInit(pTfcb, payLoadLen, u1SecurityMode, 1, 2, 3);
        vAdjustTfcb(pTfcb);
        CON_MSG("Tx Empty, Tx New %d bytes\n", payLoadLen);
        //HEX_DUMP_BODY(pTfcb, pTfcb, payLoadLen + ( (4-(payLoadLen% 4)) % 4) + TFCB_LEN_WITHOUT_IV);
        SDIO_WritePort((PUINT_8)pTfcb, ((payLoadLen + 3) & ~3) + TFCB_LEN_WITHOUT_IV, TX_DATA_PORT);

#ifdef STRESS_TEST_DISPLAY
        if((u4LoopBackTxPacketCount % 100) == 0){
            DbgPrintZo(SDCARD_ZONE_INIT, (TEXT("//Tx packet %d send to SD bus done!  \n"),
                                                u4LoopBackTxPacketCount));
        }
#else
        DbgPrintZo(SDCARD_ZONE_INIT, (TEXT("//Tx packet %d send to SD bus done!  \n"),
                           pDevice->LoopBackTxPacketCount ));
#endif
    }
    return TRUE;
}

/*----------------------------------------------------------------*/
/*!
* \brief  The core of deferred Interrupt Service Routine for every kind of
*         interrupts interested to this device. It is also called bottom half
*         in Linux.
*
* Depends on ISR (Interrupt Status Register) and TDMA/RDMA status registers,
* different routines are further invoked to handle the required interrupted
* service.
*
* \param  obj  an cookie by Linux ISR dispatching routine, to get our
*              pointer to i2220mgmt_t, the device's private structure
* \return none
*/
/*----------------------------------------------------------------*/
VOID
do_isr_tasklet(IN UINT_32 obj)
{
    UINT_32 intStatus = 0, mcrValue = 0;  // [Klockwork] UNINIT.STACK.MUST Analyze
    //void *adapter_p, *pDevice;
    //UINT32 i;
    UINT_32 u4CheckInt;
    //PUINT_32 pTemp32;
#ifdef WLAN_HIF_SPI
    PVOID pDevice;
#endif

    UNUSED(u4CheckInt);

    KDBG_ENTER;

    NIC_MCR_READ(NULL, MCR_ISAR, &intStatus);

#if POLLING_ISAR
    NIC_MCR_WRITE(pDevice, MCR_IER, 0x0020032f);
//    nicDisableInterrupt(adapter_p);
#endif

    NIC_MCR_WRITE(pDevice, MCR_ISAR, intStatus);

    if( u1SecurityMode == TKIP_MIC){
        u4CheckInt = 0x20031f;
        intStatus = (intStatus & 0x00000010);   //use MIC OK only
    }else{
        u4CheckInt = 0x20032f;
    }

    if (intStatus > 0){   /* ISAR trigger  */
        CON_MSG("MCR_ISAR : 0x%08X \n", intStatus);
        if (intStatus & ISAR_SDIO_ABNORMAL_INT){
#if 0 // DBG // Jiun say(0511):
            intStatus &= ~ISAR_SDIO_ABNORMAL_INT;
#endif
            NIC_MCR_READ(NULL, 0x0118, &mcrValue);
            CON_MSG("ISAR_SDIO_ABNORMAL_INT!, MCR_SDSR(0x118) : [0x%08X] \n",
                mcrValue);
        }

        if (intStatus & ISAR_MAC_ABNORMAL_INT){
    #if 0 // DBG // Jiun say(0511):
            intStatus &= ~ISAR_MAC_ABNORMAL_INT;
    #endif
            NIC_MCR_READ(pDevice, 0x0008, &mcrValue);
            CON_MSG("ISAR_MAC_ABNORMAL_INT!, MCR_MAISR(QSR0,0x008) : [0x%08X] \n",
                mcrValue);
            u4LoopBackTx1OkCount = 0xffffffff;
            u4LoopBackTx1OkCount = 0xffffffff;
            u4LoopBackRx1OkCount = 0xffffffff;
            return;
        }

        if (intStatus & ISAR_RFIFO_OVERFLOW_INT){
            NIC_MCR_READ(pDevice, MCR_RFFCR, &mcrValue);
            CON_MSG("ISAR_RFIFO_OVERFLOW_INT!, %lu\n", mcrValue);
#if 0 // DBG // Jiun say(0511): skip checking in normal mode
            intStatus &= ~ISAR_RFIFO_OVERFLOW_INT;
            KDBG_LOG_INT(KDBG_WARN, "ISAR:RFIFO overflow interrupt\n");
    #if 0
            NIC_MCR_READ(adapter_p, MCR_RFECR, &mcrValue);
            KDBG_LOG_INT(KDBG_NOTICE,"Rx FCS error count: %lu\n", mcrValue);
            NIC_MCR_READ(adapter_p, MCR_RFFCR, &mcrValue);
            KDBG_LOG_INT(KDBG_WARN,"Rx FIFO overflow count: %lu\n", mcrValue);
            NIC_MCR_READ(adapter_p, MCR_RPCR, &mcrValue);
            KDBG_LOG_INT(KDBG_NOTICE,"Rx PKT count: %lu\n", mcrValue);
            NIC_MCR_READ(adapter_p, MCR_QSR0, &mcrValue);
            KDBG_LOG_INT(KDBG_WARN,"QSR0:: 0x%08X\n", mcrValue);
             /* dump arbitor state machine  */
            for (i = 0 ; i< 16 ; i++){
                NIC_MCR_WRITE(adapter_p,0x0080, 0x0300+i);      // enable probe
                NIC_MCR_READ(adapter_p,0x0084,&mcrValue);
                KDBG_LOG_INT(KDBG_DEBUG,"Arbitor state addr %lu = 0x%08X \n",i, mcrValue );
            }
            KDBG_LOG_INT(KDBG_DEBUG, "\n========================\n");
    #endif
#endif
        }

        if (intStatus & ISAR_RX1_OK_INT){
            CON_MSG("ISAR_RX1_OK_INT\n");
            rxOKProcess(intStatus);
            intStatus &= ~ISAR_RX1_OK_INT;
        }

        if (intStatus & ISAR_RX2_OK_INT){
            CON_MSG("ISAR_RX2_OK_INT\n");
            rxOKProcess(intStatus);
            intStatus &= ~ISAR_RX2_OK_INT;
        }

        if (intStatus & ISAR_BEACON_TX_RX_OK_INT){
#if 0 // DBG // Jiun say(0511):
            intStatus &= ~ISAR_BEACON_TX_RX_OK_INT;
#endif
        }

        if (intStatus & ( ISAR_TX1_OK_INT | ISAR_TX2_OK_INT)){
            txOKProcess(intStatus);

            if(intStatus &  ISAR_TX1_OK_INT){
                txEmptyProcess();
            }

            if(intStatus &  ISAR_TX2_OK_INT){
                txEmptyProcess();
            }
        }

        if (intStatus & ISAR_MIC_OK_INT){
            //micProcess();
        }

        if ( ((u4LoopBackTx1OkCount + u4LoopBackTx2OkCount ) >= u4TestCount) ){
            return ;
        }

    }
    else
    {
        NIC_MCR_READ(NULL, 0x20, &mcrValue);
        KDBG_LOG_MGT(KDBG_WARN, "R chip id: 0x%08X\n", mcrValue);
    }

    if (u4LoopBackTxPacketCount % 1000 == 0) {
        CON_MSG("DO_ISR_TASK end, Tx Test Count:0x%08X\n\n", u4LoopBackTxPacketCount);
    }

    /* enable Global mask */
    mcrValue = ((u1SecurityMode ) == TKIP_MIC) ? 0x8020035f : 0x8020032f;
    NIC_MCR_WRITE(pDevice, MCR_IER, mcrValue);

    KDBG_EXIT;
}

/**
 * Loop Back Test
 *
 *
 */
 BOOLEAN vTestLoopBack(void)
 {
    UINT_32 u4Finish, u4MaxTfcbSize;
    UINT_32 u4Temp32 = 0, u4i = 0;

#ifdef WLAN_HIF_SPI
    PVOID pClientInfo;
#endif

    UNUSED(u4Finish);
    UNUSED(u4i);

    CON_MSG("TestLoopBack\n");

    u1SecurityMode = NOTUSED;
    u4TestCount = 0x99999999;
    u4MaxTfcbSize = 2000;

    /* allocate memory for TFCB */
    pTfcb = (PTFCB_STRUC)kal_malloc_sz(u4MaxTfcbSize + 4, KAL_MALLOC_FLAG);
    if (NULL == pTfcb) {
        CON_MSG("Failed to allocate tfcb memory.\n");
        return FALSE;
    }
    memset(pTfcb , 0, u4MaxTfcbSize + 4);

    /* allocate memory for  TFCB STATUS */
    pTfcbStatus = (PTFCB_STATUS_STRUC)kal_malloc_sz(sizeof(TFCB_STATUS_STRUC), KAL_MALLOC_FLAG);
    if (NULL == pTfcbStatus) {
        CON_MSG("Failed to allocate tfcb memory.\n");
        return FALSE;
    }
    memset(pTfcbStatus , 0, sizeof(TFCB_STATUS_STRUC));

    /* allocate memory for RFB */
    pRfb = (PRFB_STRUC)kal_malloc_sz(u4MaxTfcbSize + 4, KAL_MALLOC_FLAG);
    //add 4-bytes for boundary
    if (NULL == pRfb) {
        CON_MSG("Failed to allocate rfb memory.\n");
        return FALSE;
    }
    memset(pRfb , 0, u4MaxTfcbSize + 4);

    /* allocate memory for RFB STATUS */
    pRfbStatus = (PRFB_STATUS_STRUC)kal_malloc_sz(sizeof(RFB_STATUS_STRUC), KAL_MALLOC_FLAG);
    if (NULL == pRfbStatus ) {
        CON_MSG("Failed to allocate rfb status memory.\n");
        return FALSE;
    }
    memset(pRfbStatus , 0, sizeof(RFB_STATUS_STRUC));

#ifndef INCREASE
    u4PayLoadLen = 1700;
#else
    u4PayLoadLen = 32;
#endif

    vInitMCR();

    /* Enable loopback */
    NIC_MCR_WRITE(pClientInfo, MCR_PRBDR, BIT(10));

    NIC_MCR_WRITE(pClientInfo, MCR_OMAR0, 0x01010100);
    NIC_MCR_WRITE(pClientInfo, MCR_OMAR1, 0x00000101);

     /*  AUTO Start AC0 (TX)  queue */
    NIC_MCR_WRITE(pClientInfo, MCR_DPCR, BIT(26));

     /*  Start RX  queue */
    NIC_MCR_WRITE(pClientInfo, MCR_DPCR, BIT(24));

#if !POLLING_ISAR
    /* Enable 2128 interrupt */
    u4Temp32 = 0x802503ff;
    NIC_MCR_WRITE(pClientInfo, MCR_IER, u4Temp32);
    NIC_MCR_READ(pClientInfo, MCR_IER, &u4Temp32);
    CON_MSG("MCR_IER : 0x%08X\n", u4Temp32);
#endif

    /* no TFCB,TBB check sum */
    NIC_MCR_WRITE(pClientInfo, MCR_PRBDR, BITS(10, 11));

    NIC_MCR_READ(pClientInfo, MCR_SCR, &u4Temp32);
    CON_MSG("MCR_SCR :0x%08X\n", u4Temp32);

    NIC_MCR_READ(pClientInfo, MCR_PRBDR, &u4Temp32);
    CON_MSG("MCR_PRBDR :0x%08X\n", u4Temp32);

#if defined(WLAN_HIF_SPI) || defined(WLAN_HIF_SDIO)
    NIC_MCR_READ(pClientInfo, 0x12c, &u4Temp32);
    CON_MSG("0x12c :0x%08X\n", u4Temp32);
#elif defined (WLAN_HIF_HPI)
#endif

    NIC_MCR_READ(pClientInfo, MCR_IER, &u4Temp32);
    CON_MSG("MCR_IER :0x%08X\n", u4Temp32);

    /* Make first TFCB */
    txTFCBInit(pTfcb, u4PayLoadLen,  u1SecurityMode, 1, 2, 3);
    //CON_MSG("txTFCBInit\n");

    /*
    CON_MSG("sizeof TFCB %ld, payload len:%d\n", sizeof(TFCB_STRUC),  u2PayloadLen);
    HEX_DUMP_BODY(pTfcb, pTfcb, u2PayloadLen + ( (4-(u2PayloadLen % 4)) % 4)
        + TFCB_LEN_WITHOUT_IV);
    */
    vAdjustTfcb(pTfcb);

    //CON_MSG("Tx First %d bytes, port:0x%x\n", u2PayloadLen, TX_DATA_PORT);

    SDIO_WritePort((PUINT_8)pTfcb, ((u4PayLoadLen + 3) & ~3) + TFCB_LEN_WITHOUT_IV, TX_DATA_PORT);

    HEX_DUMP_BODY(pTfcb, pTfcb, ((u4PayLoadLen + 3) & ~3) + TFCB_LEN_WITHOUT_IV);

    u4Finish = 0 ;
    u4i = 0;
#if POLLING_ISAR
    while (u4Finish == 0)
    {
    #ifdef WLAN_HIF_SDIO
        if (x_slot == NULL)
            break;
    #endif
        do_isr_tasklet(0);
        u4i++;

        if ((u4LoopBackTx1OkCount + u4LoopBackTx2OkCount) >= u4TestCount){
            u4Finish = 1 ;
        }
    }

    CON_MSG("rx1:0x%08X, rx2:0x%08X, tx1:0x%08X, tx2:0x%08X\n",
        u4LoopBackRx1OkCount,
        u4LoopBackRx2OkCount,
        u4LoopBackTx1OkCount,
        u4LoopBackTx2OkCount);

    #if 1
    kal_free(pTfcb);
    kal_free(pTfcbStatus);
    kal_free(pRfb);
    kal_free(pRfbStatus);
    #endif
#endif // Polling ISAR

    return TRUE;
}


#ifdef WLAN_HIF_HPI
/*----------------------------------------------------------------*/
/*!
* \brief  SRAM probe function. This function initializes the device. Actually it is this place,
* the per instance of IPN2126 NIC card get started execution.
*
* \return  zero when success, negative as failure
*/
static int wlan_probe(void)
{
    int result;
#ifdef WLAN_HIF_HPI
    UINT32 membase = 0;
#endif

    /*-------------------------------------------
      *  Modify CPU/Platform specific control registers
      *-------------------------------------------
      */
#ifdef CONFIG_ARCH_S3C2440A
    do {
    #include <asm/arch/clocks.h>

        unsigned long fclk = elfin_get_cpu_clk();
        unsigned long pclk = elfin_get_bus_clk(GET_PCLK);
        unsigned long hclk = elfin_get_bus_clk(GET_HCLK);
        unsigned long clkratio = elfin_get_clk_ratio();

        // Jiun say(0907): use Bank5, enable wait, 16-bit
        BWSCON &= ~(BWSCON_ST5 | BWSCON_WS5 | BWSCON_DW5);
        BWSCON |= BWSCON_WS5 | BWSCON_DW(5, BWSCON_DW_16); // 0x22511112UL;
        //BWSCON |= BWSCON_ST5 | BWSCON_WS5 | BWSCON_DW(5, BWSCON_DW_8);
        BANKCON5 = BANKCON_Tacs4 | BANKCON_Tcos4 | BANKCON_Tacc14 | \
            BANKCON_Toch0 | BANKCON_Tcah4 | BANKCON_Tacp2 | BANKCON_PMC1; // 0x0000FF30UL
        //MPLLCON = (0x3C << 12) | (4 << 4) | 2;
        printk("fclk: %lu, pclk: %lu, hclk: %lu, clkratio: 0x%08X\n", fclk, pclk, hclk, clkratio);
        printk("BWSCON: 0x%08X\n", BWSCON);
        printk("BANKCON5: 0x%08X\n", BANKCON5);
        printk("BANKCON3: 0x%08X\n", BANKCON3);
        printk("MPLLCON: 0x%08X\n", MPLLCON);
        printk("CLKDIVN: 0x%08X\n", CLKDIVN);

        elfin_set_clk_ratio(0x00010606);
        clkratio = elfin_get_clk_ratio();
        printk("fclk: %lu, pclk: %lu, hclk: %lu, clkratio: 0x%08X\n", fclk, pclk, hclk, clkratio);
        printk("BWSCON: 0x%08X\n", BWSCON);
        printk("BANKCON5: 0x%08X\n", BANKCON5);
        printk("BANKCON3: 0x%08X\n", BANKCON3);
        printk("MPLLCON: 0x%08X\n", MPLLCON);
        printk("CLKDIVN: 0x%08X\n", CLKDIVN);
        printk("UCON0: 0x%08X\n", UCON0);
    }while(0);
#endif

#ifdef WLAN_HIF_HPI
    /*-------------------------------------------
     *  reserve I/O memory, probe chipID
     *-------------------------------------------
     */
    if ((result = check_mem_region(MEM_MAPPED_ADDR, MCR_MEM_MAPPED_LENGTH)) < 0){
        printk(KERN_ALERT "IPN2126 check_mem_region fail\n");
        goto fail;
    }

    if (!kal_request_mem_region(MEM_MAPPED_ADDR, MCR_MEM_MAPPED_LENGTH, "IPN2126STA")) {
        printk(KERN_ALERT "IPN2126 request_mem_region fail \n");
        result = -EIO;
        goto fail;
    }
    membase = (u32)ioremap(MEM_MAPPED_ADDR, MCR_MEM_MAPPED_LENGTH);
    //printk(KERN_ALERT "IPN2126 remapped I/O base: [0x%08X] \n", membase);

    x_membase = membase;
#endif

    /* Shouldn't actually hook up the IRQ until we
     * _know_ things are alright.  A test routine would help.
     */
#if !POLLING_ISAR && !defined(WLAN_HIF_SDIO)
    #ifdef CONFIG_M5249
    /* Enable interrupts for GIPO4 H-->L */
    *((volatile unsigned long *)(0x80000000 + 0x00C4)) |= 0x00001000;
    /* Enable interrupt level for GIPO4 - VEC35 */
    *((volatile unsigned long *)(0x80000000 + 0x0150)) |= 0x00040000;
    #endif

    if (request_irq(MT5901IRQ, wlan_hw_interrupt, SA_INTERRUPT, "mt5911 loopback test", NULL)){
        printk("MT5911 request_irq(%d) failed\n", MT5901IRQ);
        result = -EIO;
        goto fail;
    }else{
        CON_MSG("use irq#%d\n", MT5901IRQ);
    }
#else
    /*
        NdisMInitializeTimer(&priv->pollingISARTimer,
            NULL, (PNDIS_TIMER_FUNCTION)do_isr_polling, (PVOID)priv);
        NdisMSetPeriodicTimer(&priv->pollingISARTimer, 10);
    */
#endif // POLLING_ISAR

    result = 0;
    goto done;

#ifdef WLAN_HIF_HPI
fail:
    kal_release_mem_region(MEM_MAPPED_ADDR, MCR_MEM_MAPPED_LENGTH);
#endif

 done:
    return result;
}
#endif  // #ifdef WLAN_HIF_HPI

static void wlan_remove(void)
{

#if !POLLING_ISAR && !defined(WLAN_HIF_SDIO)
    free_irq(MT5901IRQ, NULL);
#endif

    if (pTfcb != NULL){
        kal_free(pTfcb);
        kal_free(pTfcbStatus);
        kal_free(pRfb);
        kal_free(pRfbStatus);
    }

#ifdef WLAN_HIF_HPI
    iounmap((void *) x_membase);
    kal_release_mem_region(MEM_MAPPED_ADDR, MCR_MEM_MAPPED_LENGTH);
#endif
}

// Max Liao, wlan card porting, 2006/03/02.
//#ifdef WLAN_HIF_SDIO
#if 0
/*----------------------------------------------------------------*/
/*!
* \brief    Call back function for card insertion handling. Start to probe card and
*               do the loopback test. The sdio bus driver should initialize card well
*               before call this function.
*
* \param
* \return
*/
/*----------------------------------------------------------------*/
void sdio_notify_add(struct mmc_slot *slot)
{
    int result  = 0;

    printk("SDIO notify card added!\n");
    x_slot = slot;
    result = slot->set_block_size(slot, 1, SDIO_BLKSIZE);

    if (result < 0){
        printk("WLAN: init SDIO Card, set block size fail, use byte mode\n");
    }else{
        printk("WLAN: init SDIO Card, set block size:[%d]\n", SDIO_BLKSIZE);
    }

    if (wlan_probe() < 0) {
        printk(KERN_ALERT "Failed to initialize SDIO card!\n");
        wlan_remove();
    }

#if !POLLING_ISAR
    slot->set_isr(slot, wlan_hw_interrupt, NULL);
#endif

    printk(KERN_ALERT "vTestLoopBack\n");
    if (FALSE == vTestLoopBack()){
        wlan_remove();
    }

    return;
}

/*----------------------------------------------------------------*/
/*!
* \brief
*
* \param
* \return
*/
/*----------------------------------------------------------------*/
void sdio_notify_remove(struct mmc_slot *slot)
{
    printk("SDIO notify card removed!\n");
    x_slot = NULL;

#if !POLLING_ISAR
    slot->set_isr(slot, NULL, NULL);
#endif

    wlan_remove();
    return;
}

static struct mmc_notifier wlan_notifier = {
    add:    sdio_notify_add,
    remove: sdio_notify_remove,
};
#endif


/*----------------------------------------------------------------*/
/*!
* \brief  Driver entry point when the driver is configured as a Linux Module.
*
* Module initialization routine, called once at module load time, by the user-
* level modutils application: insmod or modprobe.
* In particular the invokation of \a pci_register_driver with the \a i2220_pci_drv_id
* will tell the PCI controller that when it detects our own PCI device, calls it
* via the \a probe call back function, from where every interesting stuffs of
* our driver started to life.
*
* \returns zero when success, non-zero for failure, module will be unloaded.
*/
/*----------------------------------------------------------------*/

int init_module(void)
{
    // We use printk only here and in cleanup_module, so that the module
    // loading/unloading will always get alerted no matter what the KDBG_
    // debugging setting is.
    INIT_KDBG_VARS(gate,15);
    INIT_KDBG_VARS(prtk,7);
    CON_MSG("Loading... %s\n", "MT5911 LoopBack Test");

#ifdef WLAN_HIF_SDIO
    /* register call back functions to handle card insertion and removal events */
    // Max Liao, wlan card porting, 2006/03/02.
    //register_mmc_user(&wlan_notifier);
    if (FALSE == vTestLoopBack())
    {
        wlan_remove();
        return -1;
    }

#else

    #ifdef WLAN_HIF_HPI
    if (wlan_probe() < 0){
        wlan_remove();
        return -EIO;
    }
    #endif

    if (FALSE == vTestLoopBack()){
        wlan_remove();
        return -EIO;
    }
#endif
    return 0;
}

/*----------------------------------------------------------------*/
/*!
* \brief  Driver exit point when the driver as a Linux Module is removed.
*
* Called at module unload time, by the user level modutils application: rmmod.
* This is our last chance to clean up after ourselves.
*
* \returns none
*/
/*----------------------------------------------------------------*/
void cleanup_module(void)
{
// Max Liao, wlan card porting, 2006/03/02.
#ifdef WLAN_HIF_SDIO
    //unregister_mmc_user(&wlan_notifier);
    wlan_remove();
#endif
    printk(KERN_ALERT "Unload .... %s\n", "MT5911 LoopBack Test");

    return;
}


/// Wlan interrupt Binary semaphore.
static HANDLE_T _hWlanIntSema;

//-------------------------------------------------------------------------
/** _Wlan_HISR
 *  Wlan HISR function. This will be called by FCI HISR.
 *  @param  fgStatus    FCI interrupt status.
 *  @retval   void.
 */
//-------------------------------------------------------------------------
static void _Wlan_HISR(INT32 fgStatus)
{
    // unlock the specific type semaphore.
    VERIFY(OSR_OK == x_sema_unlock(_hWlanIntSema));
}

//-------------------------------------------------------------------------
/** WLAN_Main
 *  Test wpd sdio wirless lan card entry function.
 *  @param  fg4Bits : true => 4 bit access mode, false => 1 bit access mode.
 *  @retval   void.
 */
//-------------------------------------------------------------------------
void WLAN_Main(BOOL  fg4Bits)
{
    INT32 i4Ret;

#ifdef WLAN_CLOCK_TEST
    UINT32 u4Count = 0;
    INT32 i4ClockIndex, i4ClockIndexLimit;
    UINT32 u4ClockSel[] = {54,45,40,33,30,27,25,13,9,6,1,0};

    i4ClockIndex = 0;
    i4ClockIndexLimit = sizeof(u4ClockSel) / sizeof(UINT32);
#endif

    i4Ret = SDIO_IsSDIOCard();
    if ( i4Ret != S_OK)
    {
        printf("(%s):SDIO_IsSDIOCard fail.\n", __FUNCTION__);
        return;
    }

    // enable function 1 in CCCR.
    i4Ret = SDIO_EnableIO(1, _Wlan_HISR);
    if ( i4Ret != S_OK)
    {
        printf("(%s):SDIO_EnableIO fail.\n", __FUNCTION__);
        return;
    }

    if(fg4Bits)
    {
        i4Ret = SDIO_SetBusWidth(TRUE);
    }
    else
    {
        i4Ret = SDIO_SetBusWidth(FALSE);
    }

    // Set higher clock
    //MT5360 only can run to 40 mhz, so mark it. _SDC_ChangeClock(SDC_HIGH_CLOCK);

    VERIFY(OSR_OK == x_sema_create(&_hWlanIntSema, X_SEMA_TYPE_BINARY,
                X_SEMA_STATE_LOCK));

    // clean data structure.
    cleanup_module();

    // start module initialize and test.
    init_module();

    while (1)
    {
        // wait interrupt.
        VERIFY(OSR_OK == x_sema_lock(_hWlanIntSema, X_SEMA_OPTION_WAIT));

        wlan_hw_interrupt(0, NULL);

#ifdef WLAN_CLOCK_TEST
        u4Count++;

        if (u4Count > 1000)
        {
            i4ClockIndex++;

            if (i4ClockIndex >= i4ClockIndexLimit)
            {
                i4ClockIndex = 0;
            }

            CLKSRC_ChangeSDCClock(u4ClockSel[i4ClockIndex]);

            Printf("@@@@@ Change Clock to %d Mhz (index = %d)@@@@@ \n", u4ClockSel[i4ClockIndex], i4ClockIndex);

            u4Count = 0;
        }
#endif        // #ifdef WLAN_CLOCK_TEST     
    }
}
#endif
