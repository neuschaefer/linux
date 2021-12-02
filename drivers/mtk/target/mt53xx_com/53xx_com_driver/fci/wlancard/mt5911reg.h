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
/*************************************************************************
GENERAL DESCRIPTION
********************
*/

/*
** $Log: i2220reg.h $
** Revision 1.6  2004/02/03 12:49:17Z  doublelo
** Add MCR114 define for ignore TX/RX status read-done
** Revision 1.5  2004/01/16 06:35:44Z  doublelo
** Modify copy right
** Revision 1.4  2004/01/07 12:00:50Z  doublelo
** Add MCR_MICKEY
** Revision 1.3  2004/01/02 10:01:28Z  doublelo
** Add MCR_MPTR
** Revision 1.2  2003/11/03 10:10:23Z  doublelo
** 1-bit loop back test done.
** Revision 1.1  2003/10/17 14:06:08Z  doublelo
** Initial revision
*/

#ifndef _I2220REG_H
#define _I2220REG_H

/************************************************************************
*                     C O M P I L E R   F L A G S
*************************************************************************
*/

/************************************************************************
*                          C O N S T A N T S
*************************************************************************
*/
#define MCR_MEM_MAPPED_LENGTH   0x800   /* MCR memory-mapped length */

/* IPN2220 MAC configuration register (MCR) offset definition */
/* system control */
#if 0
#define MCR_SCR                 0x0000
#define MCR_ISAR                0x0004
#define MCR_QSR0                0x0008
#define MCR_QSR1                0x000C
#define MCR_IER                 0x0010
#define MCR_TXQIER              0x0014
#define MCR_MRBS                0x0018
#define MCR_ECSR                0x0040
#define MCR_ERWCR               0x0044
#define MCR_EADR                0x0048
/*queue*/
#define MCR_DPCR                0x0100
#define MCR_TQ0SAR              0x0104
#define MCR_TQ1SAR              0x0108
#define MCR_AC0SAR              0x010C
#define MCR_AC1SAR              0x0110
#define MCR_AC2SAR              0x0114
#define MCR_AC3SAR              0x0118
#define MCR_TS0SAR              0x011C
#define MCR_TS1SAR              0x0120
#define MCR_TS2SAR              0x0124
#define MCR_TS3SAR              0x0128
#define MCR_RQSAR               0x012C
#define MCR_SEORSAR             0x0130
#define MCR_SEOTSAR             0x0134
#define MCR_QRCR                0x0138
#define MCR_TSTCR               0x0140
#define MCR_TQ0CPR              0x0144
#define MCR_TQ1CPR              0x0148
#define MCR_AC0CPR              0x014C
#define MCR_AC1CPR              0x0150
#define MCR_AC2CPR              0x0154
#define MCR_AC3CPR              0x0158
#define MCR_TS0CPR              0x015C
#define MCR_TS1CPR              0x0160
#define MCR_TS2CPR              0x0164
#define MCR_TS3CPR              0x0168
#define MCR_RQCPR               0x016C
#define MCR_SEORCAR             0x0170
#define MCR_SEOTCAR             0x0174
#define MCR_TQEMR               0x0180
#define MCR_TSTXLR              0x0184
#define MCR_DRNGR               0x0188
#define MCR_AIFSR               0x018C
#define MCR_TQCWR               0x0190
#define MCR_AC0CWR              0x0194
#define MCR_AC1CWR              0x0198
#define MCR_AC2CWR              0x019C
#define MCR_AC3CWR              0x01A0

#endif
/*Mercury Queue control registers*/
#define MCR_MRBS                0x0018
#define MCR_SCR                 0x0000
#define MCR_DPCR                0x0100
#define MCR_ISAR                0x0104
#define MCR_IER                 0x0108
#define MCR_SDRDR               0x0110   /* SDIO Read Done/Error Register */
#define MCR_CRCC                0x0114   /* crc error contorl bit*/

#if 0 // NIC_TYPE == NIC_TYPE_HPI
#define MCR_HPI_CTRL    0x7FC
#define HPI_CTRL_WAIT_SIGNAL_CTRL   BIT(15)
#define HPI_CTRL_HIF_CLK_CTRL   BIT(10)
#define HPI_CTRL_WATCHDOG   BIT(9)
#define HPI_CTRL_SW_RST BIT(8)
#define HPI_CTRL_WAIT_DRV_HIGH  BIT(2)
#define HPI_CTRL_BE BIT(1)
#define HPI_CTRL_LE (0)
#define HPI_CTRL_FIRST_HW BIT(0)
#define HPI_CTRL_FIRST_LW   (0)
#define MCR_HPI_POWER_CTRL  0x7FE

#endif

/*security engine*/
#define MCR_SKACR               0x0200
#define MCR_SKADR0              0x0204
#define MCR_SKADR1              0x0208
#define MCR_SKADR2              0x020C
#define MCR_SKADR3              0x0210
#define MCR_SKADR4              0x0214
#define MCR_SKADR5              0x0218
#define MCR_SCPR0               0x021C
#define MCR_SCPR1               0x0220
#define MCR_MICKEY0             0x0224
#define MCR_MICKEY1             0x0228
/*phyical layer control interface*/
#define MCR_PCICR               0x0240
#define MCR_PCCR                0x0244
#define MCR_PBAR                0x0248
#define MCR_PPORCR0             0x0280
#define MCR_PPORCR1             0x0284
#define MCR_PPDRCR0             0x0288
#define MCR_PPDRCR1             0x028C
#define MCR_PPDRCR2             0x0290
#define MCR_PPMCR               0x0294
#define MCR_PSCR                0x02A0
#define MCR_PSPR                0x02A4
#define MCR_LCR                 0X02A8
/*transmitter control*/
#define MCR_ACWLR               0x0300
#define MCR_MPTCR               0x0304
#define MCR_NPTCR               0x0308
#define MCR_NTTCR               0x030C
#define MCR_TXOPGBNTCR          0x0310
#define MCR_NDTCR               0x0314
#define MCR_NCSTCR              0x0318
#define MCR_ACTXOPR0            0x031C
#define MCR_ACTXOPR1            0x0320
#define MCR_TSTGPCR             0x0324
#define MCR_TSTNTCR0            0x0328
#define MCR_TSTNTCR1            0x032C
#define MCR_TRCR                0x0330
/*receiver control*/
#define MCR_DRCR                0x0360
#define MCR_RFECR               0x0364
#define MCR_RFFCR               0x0368
#define MCR_RPCR                0x036C
#define MCR_CICR                0x0370
#define MCR_CACR                0x0374
#define MCR_CATR                0x0378
#define MCR_CSACR               0x037C
#define MCR_RFBCR               0x0380
#define MCR_OMAR0               0x03A0
#define MCR_OMAR1               0x03A4
#define MCR_CBR0                0x03A8
#define MCR_CBR1                0x03AC
#define MCR_AMAR0               0x03B0
#define MCR_AMAR1               0x03B4
#define MCR_AMAR2               0x03B8
#define MCR_AMAR3               0x03BC
#define MCR_AMAR4               0x03C0
#define MCR_AMAR5               0x03C4
#define MCR_AMAR6               0x03C8
#define MCR_AMAR7               0x03CC
#define MCR_AMAR8               0x03D0
/*MAC protocol timing control*/
#define MCR_MPTR                0x03FC
#define MCR_MPDR                0x0400
#define MCR_TNTDR               0x0404
#define MCR_STDR                0x0408
#define MCR_EPDDR               0x040C
#define MCR_CRFCR0              0x0410
#define MCR_CRFCR1              0x0414
#define MCR_DTR                 0x0418
#define MCR_BCWR                0x041C
#define MCR_RFTPCR              0x0420
#define MCR_LTTR0               0x0440
#define MCR_LTTR1               0x0444
#define MCR_TTAR0               0x0448
#define MCR_TTAR1               0x044C
#define MCR_TTAR2               0x0450
/*NVA control*/
#define MCR_LNR                 0x0470
#define MCR_NRTR                0x0474
#define MCR_NSUR                0x0478
/*beacon generation*/
#define MCR_BGCR0               0x0480
#define MCR_BGCR1               0x0484
#define MCR_SWBCCR              0x0488
#define MCR_SWBCDR              0x048C
#define MCR_ARRCR               0x0490
#define MCR_AFRCR               0x0494
#define MCR_ATFCR               0x0498
#define MCR_ATBCR               0x049C

/*miscellaneous and debug port*/
#define MCR_GPIOCR              0x0050
#define MCR_PRBCR               0x0080
#define MCR_PRBSR               0x0084
#define MCR_PFWDR               0x0088
#define MCR_PRBDR               0x008C
#define MCR_BPSR                0x0090
#define MCR_CTPR1               0x0094
#define MCR_CTPR2               0x0098
#define MCR_LNUIR0              0x0500
#define MCR_LNUIR1              0x0504
#define MCR_LNUIR2              0x0508


#ifndef BIT
#define BIT(n)              ( (UINT_32)1 << (n) )
#define BITS2(m,n)          ( BIT(m) | BIT (n) )
#define BITS3(m,n,o)    ( BIT(m) | BIT (n) | BIT (o) )
#define BITS4(m,n,o,p)  ( BIT(m) | BIT (n) | BIT (o) | BIT(p) )

/* bits range: for example BITS(16,23) = 0xFF0000
 *   ==>  (BIT(m)-1)   = 0x0000FFFF     ~(BIT(m)-1)   => 0xFFFF0000
 *   ==>  (BIT(n+1)-1) = 0x00FFFFFF
 */
#define BITS(m,n)       ( ~(BIT(m)-1)  &  ((BIT(n)-1)|BIT(n)) )
#endif
/* ----- System Control ----- */
/* System Control Register (SCR) bit definitions */
#define SCR_PCI_CLKRUN_ENABLE           BIT(31)
#define SCR_ASSERT_PCI_CLKRUN           BIT(30)
#define SCR_RADIO_PD_ACTIVE_EDGE        BIT(26)
#define SCR_RADIO_PD_REQ                BIT(25)
#define SCR_GPIO_PD_N                   BIT(24)
#define SCR_SEC_CLK20M_ENABLE           BIT(20)
#define SCR_MAC_CLK20M_ENABLE           BIT(19)
#define SCR_MAC_CLK40M_ENABLE           BIT(18)
#define SCR_MAC_CLK33M_ENABLE           BIT(17)
#define SCR_PLL_ENABLE                  BIT(16)
#define SCR_CLK_ALL_EN                  BITS(16,20)
#define SCR_PAU_ENABLE                  BIT(7)
#define SCR_MAC_REGRST                  BIT(3)
#define SCR_MAC_RST                     BIT(2)
#define SCR_BB_SIORST                   BIT(1)
#define SCR_BB_MRST                     BIT(0)
#define SCR_RST_ALL                     BITS(0,3)
/* SDIO Read Done/Error Register  (SDRDER) bit definitions */
#define SDRDR_BLOCK_BYTE_COUNT          BITS(6,16)
#define SDRDR_BLOCK_BYTE_MODE           BIT(5)
#define SDRDR_CRC_ERROR                 BIT(4)
#define SDRDR_MIC_RESULT_READ_DONE      BIT(3)
#define SDRDR_RX_DATA_READ_DONE         BIT(2)
#define SDRDR_RX_STATUS_READ_DONE       BIT(1)
#define SDRDR_TX_STATUS_READ_DONE       BIT(0)

/* IRQ Status and Acknowledge Register (ISAR) bit definitions */
#define ISAR_EXTERNAL_INT               BIT(22)
#define ISAR_MAC_ABNORMAL_INT               BIT(21)
#define ISAR_PHY_CCA_INDICATE_BUSY      BIT(20)
#define ISAR_RFIFO_OVERFLOW_INT         BIT(19)
#define ISAR_PRE_TBTT_INT               BIT(18)
#define ISAR_TBTT_INT                   BIT(17)
#define ISAR_ATIM_W_TIMEUP_INT          BIT(16)
#define ISAR_SDIO_ABNORMAL_INT          BIT(9)
#define ISAR_TX_BUFFER_EMPTY_INT        BIT(8)
#define ISAR_BEACON_TX_RX_OK_INT        BIT(7)
#define ISAR_SECURITY_Q_EMPTY_INT       BIT(6)
#define ISAR_AC0_Q_EMPTYINT             BIT(5)
#define ISAR_MIC_OK_INT                 BIT(4)
#define ISAR_RX2_OK_INT                 BIT(3)
#define ISAR_RX1_OK_INT                 BIT(2)
#define ISAR_TX2_OK_INT                 BIT(1)
#define ISAR_TX1_OK_INT                 BIT(0)
/* Queue Status Register 0 (QSR0) bit definitions */
#define QSR0_ABNORMAL_Q_NO              BITS(12,15)
#define QSR0_RFD_CHECK_SUM_ERR          BIT(10)
#define QSR0_TBD_CHECK_SUM_ERR          BIT(9)
#define QSR0_TFD_CHECK_SUM_ERR          BIT(8)
#define QSR0_RX_DATA_Q_OPERATING        BIT(4)
#define QSR0_SEC_ENGINE_OP_TX_Q_EMPTY   BIT(3)
#define QSR0_SEC_ENGINE_OP_TX_DONE      BIT(2)
#define QSR0_SEC_ENGINE_OP_TX_EVENT     BITS(2,3)
#define QSR0_SEC_ENGINE_OP_RX_Q_EMPTY   BIT(1)
#define QSR0_SEC_ENGINE_OP_RX_DONE      BIT(0)
#define QSR0_SEC_ENGINE_OP_RX_EVENT     BITS(0,1)

/* Queue Status Register 1 (QSR1) bit definitions */
#define QSR1_TS3_OPERATING              BIT(31)
#define QSR1_TS2_OPERATING              BIT(30)
#define QSR1_TS1_OPERATING              BIT(29)
#define QSR1_TS0_OPERATING              BIT(28)
#define QSR1_AC3_OPERATING              BIT(27)
#define QSR1_AC2_OPERATING              BIT(26)
#define QSR1_AC1_OPERATING              BIT(25)
#define QSR1_AC0_OPERATING              BIT(24)
#define QSR1_TQ1_OPERATING              BIT(23)
#define QSR1_TQ0_OPERATING              BIT(22)
#define QSR1_TS3_QUEUE_EMPTY            BIT(21)
#define QSR1_TS3_TX_DONE                BIT(20)
#define QSR1_TS3_EVENT                  BITS(20,21)
#define QSR1_TS2_QUEUE_EMPTY            BIT(19)
#define QSR1_TS2_TX_DONE                BIT(18)
#define QSR1_TS2_EVENT                  BITS(18,19)
#define QSR1_TS1_QUEUE_EMPTY            BIT(17)
#define QSR1_TS1_TX_DONE                BIT(16)
#define QSR1_TS1_EVENT                  BITS(16,17)
#define QSR1_TS0_QUEUE_EMPTY            BIT(15)
#define QSR1_TS0_TX_DONE                BIT(14)
#define QSR1_TS0_EVENT                  BITS(14,15)
#define QSR1_AC3_QUEUE_EMPTY            BIT(13)
#define QSR1_AC3_TX_DONE                BIT(12)
#define QSR1_AC3_EVENT                  BITS(12,13)
#define QSR1_AC2_QUEUE_EMPTY            BIT(11)
#define QSR1_AC2_TX_DONE                BIT(10)
#define QSR1_AC2_EVENT                  BITS(10,11)
#define QSR1_AC1_QUEUE_EMPTY            BIT(9)
#define QSR1_AC1_TX_DONE                BIT(8)
#define QSR1_AC1_EVENT                  BITS(8,9)
#define QSR1_AC0_QUEUE_EMPTY            BIT(7)
#define QSR1_AC0_TX_DONE                BIT(6)
#define QSR1_AC0_EVENT                  BITS(6,7)
#define QSR1_TQ1_TIMEOUT                BIT(5)
#define QSR1_TQ1_QUEUE_EMPTY            BIT(4)
#define QSR1_TQ1_TX_DONE                BIT(3)
#define QSR1_TQ1_EVENT                  BITS(3,5)
#define QSR1_TQ0_TIMEOUT                BIT(2)
#define QSR1_TQ0_QUEUE_EMPTY            BIT(1)
#define QSR1_TQ0_TX_DONE                BIT(0)
#define QSR1_TQ0_EVENT                  BITS(0,2)

/* IRQ Enable Register (IER) bit definitions */
#define IER_GLOBAL_ENABLE               BIT(31)
#define IER_ABNORMAL_INT                BIT(18)
#define IER_PHY_CCA_INDICATE_BUSY       BIT(17)
#define IER_RFIFO_OVERFLOW_INT          BIT(16)
#define IER_PRE_TBTT_INT                BIT(14)
#define IER_TBTT_INT                    BIT(13)
#define IER_ATIM_W_TIMEUP_INT           BIT(12)
#define IER_TS3_TIMER_INT               BIT(11)
#define IER_TS2_TIMER_INT               BIT(10)
#define IER_TS1_TIMER_INT               BIT(9)
#define IER_TS0_TIMER_INT               BIT(8)
#define IER_RX_DATA_Q_EA_INT            BIT(6)
#define IER_RX_DATA_Q_EMPTY_INT         BIT(5)
#define IER_RX_DATA_Q_DONE_INT          BIT(4)
#define IER_SEC_Q_EMPTY_INT             BIT(3)
#define IER_SEC_Q_DONE_INT              BIT(2)
#define IER_SEC_Q_EVENT_EN              BITS(2,3)
#define IER_TX_DATA_Q_EMPTY_INT         BIT(1)
#define IER_TX_DATA_Q_DONE_INT          BIT(0)

/* Tx Queue Interrupt Enable Register (TXQIER) bit definitions */
#define TXQIER_TS3_QUEUE_EMPTY_EN       BIT(21)
#define TXQIER_TS3_TX_DONE_EN           BIT(20)
#define TXQIER_TS3_EVENT_EN             BITS(20,21)
#define TXQIER_TS2_QUEUE_EMPTY_EN       BIT(19)
#define TXQIER_TS2_TX_DONE_EN           BIT(18)
#define TXQIER_TS2_EVENT_EN             BITS(18,19)
#define TXQIER_TS1_QUEUE_EMPTY_EN       BIT(17)
#define TXQIER_TS1_TX_DONE_EN           BIT(16)
#define TXQIER_TS1_EVENT_EN             BITS(16,17)
#define TXQIER_TS0_QUEUE_EMPTY_EN       BIT(15)
#define TXQIER_TS0_TX_DONE_EN           BIT(14)
#define TXQIER_TS0_EVENT_EN             BITS(14,15)
#define TXQIER_AC3_QUEUE_EMPTY_EN       BIT(13)
#define TXQIER_AC3_TX_DONE_EN           BIT(12)
#define TXQIER_AC3_EVENT_EN             BITS(12,13)
#define TXQIER_AC2_QUEUE_EMPTY_EN       BIT(11)
#define TXQIER_AC2_TX_DONE_EN           BIT(10)
#define TXQIER_AC2_EVENT_EN             BITS(10,11)
#define TXQIER_AC1_QUEUE_EMPTY_EN       BIT(9)
#define TXQIER_AC1_TX_DONE_EN           BIT(8)
#define TXQIER_AC1_EVENT_EN             BITS(8,9)
#define TXQIER_AC0_QUEUE_EMPTY_EN       BIT(7)
#define TXQIER_AC0_TX_DONE_EN           BIT(6)
#define TXQIER_AC0_EVENT_EN             BITS(6,7)
#define TXQIER_ALL_ACQ_EVENT_EN         BITS(6,13)
#define TXQIER_TQ1_TIMEOUT_EN           BIT(5)
#define TXQIER_TQ1_QUEUE_EMPTY_EN       BIT(4)
#define TXQIER_TQ1_TX_DONE_EN           BIT(3)
#define TXQIER_TQ1_EVENT_EN             BITS(3,5)
#define TXQIER_TQ0_TIMEOUT_EN           BIT(2)
#define TXQIER_TQ0_QUEUE_EMPTY_EN       BIT(1)
#define TXQIER_TQ0_TX_DONE_EN           BIT(0)
#define TXQIER_TQ0_EVENT_EN             BITS(0,2)
#define TXQIER_ALL_TXQ_EVENT_EN         BITS(0,5)

/* Maximum Receive Buffer Size Register (MRBS) bit definitions */
#define MRBSR_MAX_RECV_BUFFER_SIZE      BITS(0,15)

/* EEPROM Control and Status Register (ECSR) bit definitions */
#define ECSR_EE_TYPE                    BITS(4,5)   /* EEPROM type */
#define ECSR_EE_TYPE_NOT_PRESENT        0               /* EEPROM not present */
#define ECSR_EE_TYPE_128_BYTES          BIT(4)     /* 128-byte EEPROM */
#define ECSR_EE_TYPE_256_BYTES          BIT(5)     /* 256-byte EEPROM */
#define ECSR_EE_TYPE_512_BYTES          BITS(4,5)   /* 512-byte EEPROM */
#define ECSR_EE_CSERR                   BIT(3)     /* EEPROM checksum error */
#define ECSR_EE_RDY                     BIT(2)     /* EEPROM access complete */
#define ECSR_EE_RECALL                  BIT(1)     /* recall operation from EEPROM */
#define ECSR_EE_SERIAL                  BIT(0)     /* EEPROM serial access mode enable */

/* EEPROM Read/Write Control Register (ERWCR) bit definitions */
#define ERWCR_EECS                      BIT(7)     /* EEPROM Chip Select */
#define ERWCR_EESK                      BIT(6)     /* EEPROM Clock */
#define ERWCR_EEDI                      BIT(5)     /* EEPROM Data Input */
#define ERWCR_EEDO                      BIT(4)     /* EEPROM Data Output */
#define ERWCR_EE_READ                   BIT(1)     /* EEPROM read access */
#define ERWCR_EE_WRITE                  BIT(0)     /* EEPROM write access */

/* EEPROM Address and Data Register (EADR) bit definitions */
#define EADR_EE_ADDR                    BITS(16,23) /* EEPROM address */
#define EADR_EE_DATA                    BITS(0,15)  /* EEPROM data */


/* ----- Queue ----- */
/* Queue Command Register (QCR) bit definitions */
#define QCR_RX_DATA_Q_STOP              BIT(25)
#define QCR_RX_DATA_Q_START             BIT(24)
#define QCR_SEC_ENGINE_OP_TX_STOP       BIT(23)
#define QCR_SEC_ENGINE_OP_TX_START      BIT(22)
#define QCR_SEC_ENGINE_OP_RX_STOP       BIT(21)
#define QCR_SEC_ENGINE_OP_RX_START      BIT(20)
#define QCR_TS3_STOP                    BIT(19)
#define QCR_TS3_START                   BIT(18)
#define QCR_TS2_STOP                    BIT(17)
#define QCR_TS2_START                   BIT(16)
#define QCR_TS1_STOP                    BIT(15)
#define QCR_TS1_START                   BIT(14)
#define QCR_TS0_STOP                    BIT(13)
#define QCR_TS0_START                   BIT(12)
#define QCR_AC3_STOP                    BIT(11)
#define QCR_AC3_START                   BIT(10)
#define QCR_AC2_STOP                    BIT(9)
#define QCR_AC2_START                   BIT(8)
#define QCR_AC1_STOP                    BIT(7)
#define QCR_AC1_START                   BIT(6)
#define QCR_AC0_STOP                    BIT(5)
#define QCR_AC0_START                   BIT(4)
#define QCR_TQ1_STOP                    BIT(3)
#define QCR_TQ1_START                   BIT(2)
#define QCR_TQ0_STOP                    BIT(1)
#define QCR_TQ0_START                   BIT(0)

/* Queue Resume Command Register (QRCR) bit definitions */

#define QRCR_RXQ_RESUME                 BIT(12)
#define QRCR_SEC_ENGINE_OP_TX_RESUME    BIT(11)
#define QRCR_SEC_ENGINE_OP_RX_RESUME    BIT(10)
#define QRCR_TS3_RESUME                 BIT(9)
#define QRCR_TS2_RESUME                 BIT(8)
#define QRCR_TS1_RESUME                 BIT(7)
#define QRCR_TS0_RESUME                 BIT(6)
#define QRCR_AC3_RESUME                 BIT(5)
#define QRCR_AC2_RESUME                 BIT(4)
#define QRCR_AC1_RESUME                 BIT(3)
#define QRCR_AC0_RESUME                 BIT(2)
#define QRCR_TQ1_RESUME                 BIT(1)
#define QRCR_TQ0_RESUME                 BIT(0)

/* TS Token Control Register (TSTCR) bit definitions */
#define TSTCR_TS3_TIMER_ENABLE          BIT(19)
#define TSTCR_TS2_TIMER_ENABLE          BIT(18)
#define TSTCR_TS1_TIMER_ENABLE          BIT(17)
#define TSTCR_TS0_TIMER_ENABLE          BIT(16)
#define TSTCR_TOKEN_P_RENEW             BIT(8)
#define TSTCR_TOKEN_P_3                 BITS(6,7)
#define TSTCR_TOKEN_P_2                 BITS(4,5)
#define TSTCR_TOKEN_P_1                 BITS(2,3)
#define TSTCR_TOKEN_P_0                 BITS(0,1)

/* TS TX Limit Register (TSTXLR) bit definitions */
#define TSTXLR_TS3_TX_LIMIT             BITS(24,31)
#define TSTXLR_TS2_TX_LIMIT             BITS(16,23)
#define TSTXLR_TS1_TX_LIMIT             BITS(8,15)
#define TSTXLR_TS0_TX_LIMIT             BITS(0,7)

/* Arbitration Inter Frame Spacing Register (AIFSR) bit definitions */
#define AIFSR_TQ_RN_ADD_1_EN            BIT(24)
#define AIFSR_AC3_RN_ADD_1_EN           BIT(23)
#define AIFSR_AC2_RN_ADD_1_EN           BIT(22)
#define AIFSR_AC1_RN_ADD_1_EN           BIT(21)
#define AIFSR_AC0_RN_ADD_1_EN           BIT(20)
#define AIFSR_AIFS_TQ                   BITS(16,19)
#define AIFSR_AIFS_3                    BITS(12,15)
#define AIFSR_AIFS_2                    BITS(8,11)
#define AIFSR_AIFS_1                    BITS(4,7)
#define AIFSR_AIFS_0                    BITS(0,3)

/* TQs Contention Window Register (TQCWR) bit definitions */
#define TQCWR_TQ_CW_MAX                 BITS(16,31)
#define TQCWR_TQ_CW_MIN                 BITS(0,7)

/* AC0 Contention Window Register (AC0CWR) bit definitions */
#define AC0CWR_AC0_CW_MAX               BITS(16,31)
#define AC0CWR_AC0_CW_MIN               BITS(0,7)

/* AC1 Contention Window Register (AC1CWR) bit definitions */
#define AC1CWR_AC1_CW_MAX               BITS(16,31)
#define AC1CWR_AC1_CW_MIN               BITS(0,7)

/* AC2 Contention Window Register (AC2CWR) bit definitions */
#define AC2CWR_AC2_CW_MAX               BITS(16,31)
#define AC2CWR_AC2_CW_MIN               BITS(0,7)

/* AC3 Contention Window Register (AC3CWR) bit definitions */
#define AC3CWR_AC3_CW_MAX               BITS(16,31)
#define AC3CWR_AC3_CW_MIN               BITS(0,7)


/* ----- Security Engine ----- */
/* Security Key Access Control Register (SKACR, offset 0x0200) */
#define SKACR_ACCESS_START              BIT(31)
#define SKACR_READ_WRITE                BIT(30)
#define SKACR_TABLE_SIZE                BITS(8,15)
#define SKACR_KEY_INDEX                 BITS(0,7)

/* Security Key Access Data Register 0 (SKADR0, offset 0x0204) */
#define SKADR0_KEY_ID                   BITS(30,31)
#define SKADR0_CHECK_ALL                BITS(27,29)
#define SKADR0_CHECK_A2                 BIT(29)
#define SKADR0_CHECK_A1                 BIT(28)
#define SKADR0_CHECK_KEY_ID             BIT(27)
#define SKADR0_CIPHER_SUITE             BITS(24,26)
#define SKADR0_KEY_MAPPING_ADDR         BITS(0,23)

/* Security Key Access Data Register 1 (SKADR0, offset 0x0208) */
#define SKADR1_KEY_MAPPING_ADDR         BITS(0,23)

/* Security Key Access Data Register 2 (SKADR0, offset 0x020C) */
#define SKADR2_KEY_VALUE                BITS(0,31)

/* Security Key Access Data Register 3 (SKADR0, offset 0x0210) */
#define SKADR3_KEY_VALUE_8_31           BITS(8,31)
#define SKADR3_KEY_VALUE_0_7            BITS(0,7)

/* Security Key Access Data Register 4 (SKADR0, offset 0x0214) */
#define SKADR4_KEY_VALUE                BITS(0,31)

/* Security Key Access Data Register 5 (SKADR0, offset 0x0218) */
#define SKADR5_KEY_VALUE_8_31           BITS(8,31)
#define SKADR5_KEY_VALUE_0_7            BITS(0,7)

/* Security Control Parameter Register 0 (SCPR0, offset 0x021C) */
#define SCPR0_FRAME_CONTROL_MASK        BITS(16,31)
#define SCPR0_SEQUENCE_CONTROL_MASK     BITS(0,15)

/* Security Control Parameter Register 1 (SCPR1, offset 0x0220) */
#define SCPR1_TKIP_IV_ENDIAN            BIT(31)
#define SCPR1_TKIP_EIV_ENDIAN           BIT(30)
#define SCPR1_QOS_CONTROL_MASK          BITS(0,15)

/* ----- Physical Layer Control Interface ----- */
/* PHY Control Interface Configuration Register (PCICR) bit definitions */
#define PCICR_CHIP_ID_VERSION           BITS(12,31) /* chip ID and version */
#define PCICR_RF_SW_CTRL                BIT(4)     /* RF software control */
#define PCICR_RF_CTRL_MODE              BITS(0,2)   /* RF control mode */

/* PAU Control Configuration register (PCCR) */
#define PCCR_BCIMODE                    BIT(0)

/* PHY BCI Access Register (PBAR) bit definitions */
#define PBAR_PHY_BCI_ACCESS_START       BIT(25)     /* PHY BCI access command/status */
#define PBAR_PHY_BCI_RW_CTRL            BIT(24)     /* PHY BCI read/write control */
#define PBAR_PHY_BCI_INDEX              BITS(16,23)  /* PHY BCI access CR register index */
#define PBDR_PHY_BCI_WR_DATA            BITS(8,15)   /* PHY BCI write data */
#define PBDR_PHY_BCI_RD_DATA            BITS(0,7)    /* PHY BCI read data */

/* PHY Power Management Control Register (PPMCR) bit definitions */
#define PPMCR_GPIO_CAL                  BIT(29)    /* GPIO for RF power control */
#define PPMCR_GPIO_RF_SHDN              BIT(28)    /* GPIO for RF power control */
#define PPMCR_GPIO_PA2_EN               BIT(22)    /* GPIO for RF 2.4G PA enable */
#define PPMCR_GPIO_PA5_EN               BIT(21)    /* GPIO for RF 5G PA enable */
#define PPMCR_GPIO_RF_TX                BIT(20)    /* GPIO for RF Tx control */
#define PPMCR_GPIO_RF_RX                BIT(19)    /* GPIO for RF Rx control */
#define PPMCR_GPIO_TR_SW                BIT(18)    /* GPIO for BB TX */
#define PPMCR_BB_TX_PE                  BIT(17)    /* GPIO for RF Tx/Rx control */
#define PPMCR_BB_RX_PE                  BIT(16)    /* GPIO for RF Tx/Rx control */

/* PHY Synthesizer Programming Register (PSPR) bit definitions */
#define PSPR_SYNTH_PROG_START           BIT(31)    /* synthesizer programming command/status */
#define PSPR_RW_CTL                     BIT(30)    /* synthesizer RW control */
#define PSPR_SYNTH_DATA                 BITS(0,23)  /* synthesizer programming data */

/* LED Configuration Register (LCR) bit definitions */
#define LCR_LED_GPIO_OUTPUT             BIT(19)    /* LED GPIO Output control */
#define LCR_LED_POLARITY                BIT(18)    /* LEDOn polarity */
#define LCR_LED_MODE                    BITS(16,17) /* LED mode */
#define LCR_LED_ON_CNT                  BITS(8,15)  /* LED On count */
#define LCR_LED_OFF_CNT                 BITS(0,7)   /* LED Off count */

/* ----- Transmitter Control ----- */
/* ATIM/CFP Window Length Register (ACWLR) bit definitions */
#define ACWLR_CFP_MAX_DURATION          BITS(16,31) /* CFP max duration */
#define ACWLR_ATIM_WINDOW               BITS(0,15)  /* ATIM window */

/* MAC Protocol Timing Control Register (MPTCR) bit definitions */
#define MPTCR_OPERATION_MODE            BITS(30,31) /* operation mode */
#define MPTCR_OPERATION_MODE_ESS_STA    0               /* Infrastructure STA */
#define MPTCR_OPERATION_MODE_IBSS       BIT(30)    /* ad hoc */
#define MPTCR_OPERATION_MODE_ESS_AP     BIT(31)    /* Infrastructure AP */
#define MPTCR_EN_COMP_NEXT_PRE_TBTT_TIME    BIT(29)
#define MPTCR_TBTT_ENABLE               BIT(27)    /* enable TBTT comparator */
#define MPTCR_DTIM_ENABLE               BIT(26)    /* enable DTIM comparator */
#define MPTCR_CFP_START_ENABLE          BIT(25)    /* enable CFPStart comparator */
#define MPTCR_TXOP_GB_ENABLE            BIT(24)    /* enable TXOP Guard Band event comparator */
#define MPTCR_EN_COMP_NEXT_TBTT_TIME    BIT(23)
#define MPTCR_EN_COMP_TXOP_GB_NEXT_TIME     BIT(22)
#define MPTCR_EN_COMP_NEXT_DTIM_TIME    BIT(21)
#define MPTCR_EN_COMP_NEXT_CFP_START_TIME   BIT(20)
#define MPTCR_EN_COMP_TS3_TIMER_NEXT_TIME   BIT(19)
#define MPTCR_EN_COMP_TS2_TIMER_NEXT_TIME   BIT(18)
#define MPTCR_EN_COMP_TS1_TIMER_NEXT_TIME   BIT(17)
#define MPTCR_EN_COMP_TS0_TIMER_NEXT_TIME   BIT(16)
#define MPTCR_PRE_TBTT_GEN_PERIOD       BITS(0,15)  /* pre-TBTT generation period */

/* Next Pre-TBTT Time Control Register (NPTCR) bit definitions */
#define NPTCR_NEXT_PRE_TBTT_TIME        BITS(0,25)  /* next pre-TBTT time */

/* Next TBTT Time Control Register (NTTCR) bit definitions */
#define NTTCR_TBTT_GEN_PERIOD           BITS(16,31) /* TBTT generation period */
#define NTTCR_NEXT_TBTT_TIME            BITS(0,15)  /* next TBTT time */

/* TXOP Guard Band Next Time Control Register (TXOPGBNTCR) bit definitions */
#define TXOPGBNTCR_TXOP_GB_NEXT_TIME    BITS(0,18)  /* next TXOP Guard Band time */

/* Next DTIM Time Control Register (NDTCR) bit definitions */
#define NDTCR_DTIM_GEN_PERIOD           BITS(16,31) /* DTIM generation period */
#define NDTCR_NEXT_DTIM_TIME            BITS(0,15)  /* next DTIM time */

/* Next CFP Start Time Control Register (NCSTCR) bit definitions */
#define NCSTCR_CFP_START_GEN_PERIOD     BITS(16,31) /* CFPStart generation period */
#define NCSTCR_NEXT_CFP_START_TIME      BITS(0,15)  /* next CFPStart time */

/* AC TXOP Limit Register 0 (ACTXOPR0) bit definitions */
#define ACTXOPR0_AC1_LIMIT              BITS(16,31) /* TXOP limit of AC1 */
#define ACTXOPR0_AC0_LIMIT              BITS(0,15)  /* TXOP limit of AC0 */

/* AC TXOP Limit Register 1 (ACTXOPR1) bit definitions */
#define ACTXOPR1_AC3_LIMIT              BITS(16,31) /* TXOP limit of AC3 */
#define ACTXOPR1_AC2_LIMIT              BITS(0,15)  /* TXOP limit of AC2 */

/* TS Timer Generation Period Control Register (TSTGPCR) bit definitions */
#define TSTGPCR_TS3_TIMER_GEN_PERIOD    BITS(24,31) /* TS3 timer generation period */
#define TSTGPCR_TS2_TIMER_GEN_PERIOD    BITS(16,23) /* TS2 timer generation period */
#define TSTGPCR_TS1_TIMER_GEN_PERIOD    BITS(8,15)  /* TS1 timer generation period */
#define TSTGPCR_TS0_TIMER_GEN_PERIOD    BITS(0,7)   /* TS0 timer generation period */

/* TS Timer Next Time Control Register 0 (TSTNTCR0) bit definitions */
#define TSTNTCR0_TS1_TIMER_NEXT_TIME    BITS(16,31) /* TS1 timer next time */
#define TSTNTCR0_TS0_TIMER_NEXT_TIME    BITS(0,15)  /* TS0 timer next time */

/* TS Timer Next Time Control Register 1 (TSTNTCR1) bit definitions */
#define TSTNTCR1_TS3_TIMER_NEXT_TIME    BITS(16,31) /* TS3 timer next time */
#define TSTNTCR1_TS2_TIMER_NEXT_TIME    BITS(0,15)  /* TS2 timer next time */

/* TXOP Release Control Register (TRCR) bit definitions */
#define TRCR_TX_TIME                    BITS(16,24)
#define TRCR_END_PACKET_TYPE            BIT(6)
#define TRCR_TX_RATE                    BITS(0,5)


/* ----- Receiver Control ----- */
/* Data Receiver Control Register (DRCR) bit definitions */
#define DRCR_ADDR_1_VALIDATION          BIT(16)    /* Addr 1 validation for data */
#define DRCR_MGT_BSSID_VALIDATION       BIT(15)    /* bc/mc mgt BSSID validation */
#define DRCR_ADDR_3_OWN_SA_FILTERING    BIT(14)    /* Addr 3 own SA filtering for bc/mc data with ToDS=0, FromDS=1 */
#define DRCR_ADDR_3_BSSID_VALIDATION    BIT(13)    /* Addr 3 BSSID validation for bc/mc data with ToDS=0, FromDS=0 */
#define DRCR_ADDR_2_BSSID_VALIDATION    BIT(12)    /* Addr 2 BSSID validation for bc/mc data with ToDS=0, FromDS=1 */
#define DRCR_RX_BROADCAST_ENABLE        BIT(11)    /* Rx bc MPDU enable bit */
#define DRCR_RX_ALL_MULTICAST_ENABLE    BIT(10)    /* Rx all mc MPDUs enable bit */
#define DRCR_RX_MULTICAST_ENABLE        BIT(9)     /* Rx multicast MPDU enable bit */
#define DRCR_PROMISCUOUS_MODE           BIT(8)     /* promiscuous mode enable bit */
#define DRCR_FCS_ERROR_FRAME_DISCARD    BIT(0)     /* Rx frame with FCS error discard control bit */

/* Receive Frame Block Configuration Register (RFBCR) bit definitions */
#define RFBCR_BB_CR_D_INDEX             BITS(24,30)   /* BB CR D index */
#define RFBCR_BB_CR_C_INDEX             BITS(16,22)   /* BB CR C index */
#define RFBCR_BB_CR_B_INDEX             BITS(8,14)    /* BB CR B index */
#define RFBCR_BB_CR_A_INDEX             BITS(0,6)     /* BB CR A index */


/* ----- MAC Protocol Timing Control ----- */
/* MAC Processing Delay Registet (MPDR) bit definitions */
#define MPDR_MAC_PRC_DELAY_2            BITS(16,23) /* MAC processing delay M2 */
#define MPDR_MAC_PRC_DELAY_1            BITS(0,7)   /* MAC processing delay M1 */

/* Transmit to Next Transmit Delay Register (TNTDR) bit definitions */
#define TNTDR_TX_PIFS_CCA               BITS(16,31)
#define TNTDR_TX_SIFS_TX                BITS(0,15)
#define TNTDR_RADIA_LONG_SLOT            0x01f40082
#define TNTDR_RADIA_SHORT_SLOT           0x01180082
#define TNTDR_MAXIM_LONG_SLOT            0x01f40082
#define TNTDR_MAXIM_SHORT_SLOT           0x01180082
#define TNTDR_RFMD_LONG_SLOT             0x01f40082
#define TNTDR_RFMD_SHORT_SLOT            0x01180082

/* Slot Time Duration Register (STDR) bit definitions */
#define STDR_SLOT_TIME_2                BITS(16,31) /* slot time with M2 */
#define STDR_SLOT_TIME_1                BITS(0,15)  /* slot time with M1 */
#define STDR_LONG_SLOT                  0x01900190
#define STDR_SHORT_SLOT                 0x00b400b4

/* EIFS and Probe Delay Duration Register (EPDDR) bit definitions */
#define EPDDR_PROBE_DELAY               BITS(16,31) /* ProbeDelay */
#define EPDDR_EIFS_TIME                 BITS(0,15)  /* EIFS */

/* Control Response Frame Control Register 0 (CRFCR0) bit definitions */
#define CRFCR0_RSP_SAME_PREAMBLE        BIT(31)
#define CRFCR0_CTRL_RSP_TX_RATE_FOR_9M  BITS(25,29)
#define CRFCR0_CTRL_RSP_TX_RATE_FOR_6M  BITS(20,24)
#define CRFCR0_CTRL_RSP_TX_RATE_FOR_11M BITS(15,19)
#define CRFCR0_CTRL_RSP_TX_RATE_FOR_5_5M    BITS(10,14)
#define CRFCR0_CTRL_RSP_TX_RATE_FOR_2M  BITS(5,9)
#define CRFCR0_CTRL_RSP_TX_RATE_FOR_1M  BITS(0,4)

/* Control Response Frame Control Register 1 (CRFCR1) bit definitions */
#define CRFCR1_CTRL_RSP_TX_RATE_FOR_54M BITS(25,29)
#define CRFCR1_CTRL_RSP_TX_RATE_FOR_48M BITS(20,24)
#define CRFCR1_CTRL_RSP_TX_RATE_FOR_36M BITS(15,19)
#define CRFCR1_CTRL_RSP_TX_RATE_FOR_24M BITS(10,14)
#define CRFCR1_CTRL_RSP_TX_RATE_FOR_18M BITS(5,9)
#define CRFCR1_CTRL_RSP_TX_RATE_FOR_12M BITS(0,4)

#define CRFCR_CTRL_RSP_TX_RATE_1M       RATE_CODE_1M
#define CRFCR_CTRL_RSP_TX_RATE_2M       RATE_CODE_2M
#define CRFCR_CTRL_RSP_TX_RATE_5_5M     RATE_CODE_5_5M
#define CRFCR_CTRL_RSP_TX_RATE_11M      RATE_CODE_11M
#define CRFCR_CTRL_RSP_TX_RATE_6M       (RATE_CODE_6M | 0x00000010)
#define CRFCR_CTRL_RSP_TX_RATE_9M       (RATE_CODE_9M | 0x00000010)
#define CRFCR_CTRL_RSP_TX_RATE_12M      (RATE_CODE_12M | 0x00000010)
#define CRFCR_CTRL_RSP_TX_RATE_18M      (RATE_CODE_18M | 0x00000010)
#define CRFCR_CTRL_RSP_TX_RATE_24M      (RATE_CODE_24M | 0x00000010)
#define CRFCR_CTRL_RSP_TX_RATE_36M      (RATE_CODE_36M | 0x00000010)
#define CRFCR_CTRL_RSP_TX_RATE_48M      (RATE_CODE_48M | 0x00000010)
#define CRFCR_CTRL_RSP_TX_RATE_54M      (RATE_CODE_54M | 0x00000010)

/* DCF Timeout Register (DTR) bit definitions */

/* Beacon Contention Window Register (BCWR) bit definitions */

/* TSF Timer Adjustment Register 0 (TTAR0) bit definitions */
#define TTAR0_TSF_TIMER_VALUE_CHANGE    BIT(1)     /* TSF timer value change command/status */
#define TTAR0_TSF_TIMER_OFFSET_ADJUST   BIT(0)     /* TSF timer offset adjust command/status */


/* ----- Network Allocation Vector (NAV) Control ----- */
/* Local NAV Register (LNR) bit definitions */
#define LNR_NAV                         BITS(0,25)  /* local NAV value */

/* NAV Reset Timeout Register (NRTR) bit definitions */
#define NRTR_CAP_RECOVERY_TIME          BITS(16,19) /* CAP recovery time for AP */
#define NRTR_RTS_NAV_RESET_TIMEOUT      BITS(0,8)   /* RTS NAV reset timeout */

/* NAV Software Update Register (NSUR) bit definitions */
#define NSUR_NAV_UPDATE                 BIT(31)    /* NAV update command and status */
#define NSUR_NAV_UPDATE_VALUE           BITS(0,25)  /* NAV updated value */


/* ----- Beacon Generation ----- */
/* Beacon Generation Control Register 0 (BGCR0) bit definitions */
#define BGCR0_TX_TIMESTAMP_OFFSET       BITS(16,23)
#define BGCR0_TRANSMISSION_POWER        BITS(8,15)
#define BGCR0_BEACON_TX_RATE            BITS(0,5)   /* beacon tx rate */

/* Beacon Generation Control Register 1 (BGCR1) bit definitions */
#define BGCR1_BC_OWN                    BIT(31)    /* BC own bit */
#define BGCR1_BEACON_SN                 BITS(16,27)
#define BGCR1_BEACON_LENGTH             BITS(8,15)
#define BGCR1_BC_LENGTH                 BITS(0,7)

/* SW Beacon Content Control Register (SWBCCR) bit definitions */
#define SWBCCR_BC_ACCESS_START          BIT(7)
#define SWBCCR_BC_RW_CTRL               BIT(6)
#define SWBCCR_BC_READ                  0
#define SWBCCR_BC_WRITE                 BIT(6)
#define SWBCCR_BC_INDEX                 BITS(0,5)


/* ----- Transmit Frame Architecture ----- */
/* Transmit Frame Control Block (TFCB) field definitions */
/* TFD Control Flag field */
#define TFCB_FLAG_EOL                   BIT(0)     /* Last TFCB indication bit */
#define TFCB_FLAG_NB                    BIT(1)     /* Null TBB pointer indication bit */
#define TFCB_FLAG_MORE                  BIT(2)     /* More Frag bit */
#define TFCB_FLAG_CFB                   BIT(3)     /* Contention Free Burst bit */
#define TFCB_FLAG_SEC                   BIT(4)     /* Security Engine Operation */
#define TFCB_FLAG_PAIRED                BIT(5)     /* Paired bit */
/* Security Operation Mode field */
#define TFCB_SEC_MODE                   BITS(0,3)
#define TFCB_SEC_MODE_NORMAL            0
#define TFCB_SEC_MODE_SEC_ENGINE        BIT(3)
#define TFCB_SEC_MODE_NONE              0x00
#define TFCB_SEC_MODE_WEP_40            0x01
#define TFCB_SEC_MODE_TKIP              0x02
#define TFCB_SEC_MODE_WRAP              0x03
#define TFCB_SEC_MODE_CCMP              0x04
#define TFCB_SEC_MODE_WEP_104           0x05
#define TFCB_SEC_MODE_2120_CCMP         0x07
#define TFCB_SEC_MODE_TKIP_MIC          0x0A
#define TFCB_SEC_MODE_ENCRYPT_AES_CHAIN 0x0C
#define TFCB_SEC_MODE_ENCRYPT_AES_SEQ   0x0D
#define TFCB_SEC_MODE_DECRYPT_AES_CHAIN 0x0E
#define TFCB_SEC_MODE_DECRYPT_AES_SEQ   0x0F
/* Initialiazation Vector (IV) field */
#define TFCB_IV_WEP_KEY_ID              BITS(30,31)   /* Key ID subfield */
#define TFCB_IV_WEP_PAD                 BITS(24,29)   /* Pad subfield */
#define TFCB_IV_WEP_IV                  BITS(0,23)    /* Initialization vector subfield */
#define TFCB_IV_TKIP_KEY_ID             BITS(30,31)
#define TFCB_IV_TKIP_EXT_IV             BIT(29)
#define TFCB_IV_TKIP_PAD                BITS(24,28)
#define TFCB_IV_TKIP_RC4_KEY_2          BITS(16,23)
#define TFCB_IV_TKIP_RC4_KEY_1          BITS(8,15)
#define TFCB_IV_TKIP_RC4_KEY_0          BITS(0,7)
#define TFCB_IV_CCMP_KEY_ID             BITS(30,31)
#define TFCB_IV_CCMP_EXT_IV             BIT(29)
#define TFCB_IV_CCMP_PAD                BITS(16,28)
#define TFCB_IV_CCMP_IV_1               BITS(8,15)
#define TFCB_IV_CCMP_IV_0               BITS(0,7)
#define TFCB_IV_WRAP_KEY_ID             BITS(30,31)
#define TFCB_IV_WRAP_PAD                BITS(28,29)
#define TFCB_IV_WRAP_REPLAY_COUNTER     BITS(0,27)
/* Extended IV field */
#define TFCB_EIV_TKIP_TSC_5             BITS(24,31)
#define TFCB_EIV_TKIP_TSC_4             BITS(16,23)
#define TFCB_EIV_TKIP_TSC_3             BITS(8,15)
#define TFCB_EIV_TKIP_TSC_2             BITS(0,7)
#define TFCB_EIV_CCMP_IV_5              BITS(24,31)
#define TFCB_EIV_CCMP_IV_4              BITS(16,23)
#define TFCB_EIV_CCMP_IV_3              BITS(8,15)
#define TFCB_EIV_CCMP_IV_2              BITS(0,7)
/* TCI Control Flag field */
#define TFCB_CTRL_RTS                   BIT(7)
#define TFCB_CTRL_CTS                   BIT(6)
#define TFCB_CTRL_QOS_CF_POLL           BIT(5)
#define TFCB_CTRL_TSF                   BIT(4)
#define TFCB_CTRL_NO_ACK                BIT(3)
#define TFCB_CTRL_DELAY                 BIT(2)
#define TFCB_CTRL_CFB                   BIT(1)
#define TFCB_CTRL_MORE                  BIT(0)
/* TID & MAC Header Control Flag field */
#define TFCB_TID                        BITS(0,3)
#define TFCB_FRAG                       BIT(7)
#define TFCB_TX_ANT                     BIT(6)
#define TFCB_MAC_HEADER_EIV_PRESENT     BIT(5)
#define TFCB_MAC_HEADER_IV_PRESENT      BIT(4)
/* Multirate Switching Control field */
#define TFCB_MULTIRATE_STEP_NUM         BITS(14,15)
#define TFCB_MULTIRATE_1_STEP_SWITCH    0
#define TFCB_MULTIRATE_2_STEP_SWITCH    BIT(14)
#define TFCB_MULTIRATE_MPDU_RETRY_LIMIT_2   BITS(7,13)
#define TFCB_MULTIRATE_MPDU_RETRY_LIMIT_1   BITS(0,6)
/* Transmission Rates field */
#define TFCB_TX_RATE_2                  BITS(8,13)
#define TFCB_TX_RATE_1                  BITS(0,5)
/* Transmit Status field */
#define TFCB_STATUS_FRAG_SWSKIP         BIT(15)
#define TFCB_STATUS_TX_RATE             BITS(8,15)
#define TFCB_STATUS_TIMEUP_ERR          BIT(4)
#define TFCB_STATUS_FRAG_SKIP_ERR       BIT(3)
#define TFCB_STATUS_RTS_RETRY_ERR       BIT(2)
#define TFCB_STATUS_MPDU_RETRY_ERR      BIT(1)
#define TFCB_STATUS_OK                  BIT(0)
#if 0
#define TFCB_STATUS_MASK                (TFCB_STATUS_OK | \
                                        TFCB_STATUS_MPDU_RETRY_ERR | \
                                        TFCB_STATUS_RTS_RETRY_ERR | \
                                        TFCB_STATUS_FRAG_SKIP_ERR | \
                                        TFCB_STATUS_TIMEUP_ERR    | \
                                        TFCB_STATUS_FRAG_SWSKIP)
#endif
#define TFCB_STATUS_MASK    (TFCB_STATUS_OK | \
                                        TFCB_STATUS_MPDU_RETRY_ERR | \
                                        TFCB_STATUS_RTS_RETRY_ERR | \
                                        TFCB_STATUS_FRAG_SKIP_ERR | \
                                        TFCB_STATUS_TIMEUP_ERR    | \
                                        TFCB_STATUS_FRAG_SWSKIP)

/* Transmit Frame Descriptor (TFD) Checksum size */
#define TFCB_TFD_SIZE                   0x0c        //TFD legth for check sum calculation
#define TFCB_TSI_SIZE                   0x10        //Transmit Status Information



/* Transmit Buffer Block (TBB) field definitions */
/* TBB Flag field */
#define TBB_FLAG_EOL                    BIT(0)     /* Last TBB indication bit */

/* Transmit Buffer Block (TBB) size definiton */
#define TBB_REVBUFFER_SIZE              32
#define TBB_TBD_SIZE                    12


/* ----- Receive Frame Architecture ----- */
/* Receive Frame Block (RFB) field definitions */
/* RFB Flag field */
#define RFB_FLAG_EOL                    BIT(0)     /* RFB EL Bit */
/* Receive Status Bits */
#define RFB_STATUS_FCS_ERR              BIT(1)
#define RFB_STATUS_OK                   BIT(0)
#define RFB_STATUS_MASK                 (RFB_STATUS_OK | RFB_STATUS_FCS_ERR)
/* Receive Security Status Bits */
#define RFB_SECSTATUS_DATA_PRIVACY_MASK     BITS(0,3)
#define RFB_SECSTATUS_FORMAT_ERR            BIT(11)
#define RFB_SECSTATUS_RXICV_ERR             BIT(10)
#define RFB_SECSTATUS_RXKEYDIS_ERR          BIT(9)
#define RFB_SECSTATUS_RXKEY_NOT_FOUND_ERR   BIT(8)
#define RFB_SECSTATUS_RXDEC_ERR_MASK        BITS(8,11)
/* Receive Frame Descriptor (RFD) size */
#define RFB_RFD_SIZE                    0x08
#define RFB_RX_BUFFER_MAC_HEADER_SIZE   32
#define RFB_RSI_SIZE                    48


/* ----- Transmission rate codes ----- */
#define RATE_CODE_RATE_MASK             0x0F
#define RATE_CODE_1M                    0x00
#define RATE_CODE_2M                    0x01
#define RATE_CODE_5_5M                  0x02
#define RATE_CODE_11M                   0x03
#define RATE_CODE_6M                    0x0B
#define RATE_CODE_9M                    0x0F
#define RATE_CODE_12M                   0x0A
#define RATE_CODE_18M                   0x0E
#define RATE_CODE_24M                   0x09
#define RATE_CODE_36M                   0x0D
#define RATE_CODE_48M                   0x08
#define RATE_CODE_54M                   0x0C

#define RATE_CODE_PREAMBLE_MASK         0x10
#define RATE_CODE_LONG_PREAMBLE         0x00
#define RATE_CODE_SHORT_PREAMBLE        0x10

#define RATE_CODE_OFDM_MASK             0x20
#define RATE_CODE_CCK                   0x00
#define RATE_CODE_OFDM                  0x20

/* CCK mode, long preamble */
#define TX_RATE_LONG_PREAMBLE_1M        (RATE_CODE_1M | RATE_CODE_LONG_PREAMBLE)
#define TX_RATE_LONG_PREAMBLE_2M        (RATE_CODE_2M | RATE_CODE_LONG_PREAMBLE)
#define TX_RATE_LONG_PREAMBLE_5_5M      (RATE_CODE_5_5M | RATE_CODE_LONG_PREAMBLE)
#define TX_RATE_LONG_PREAMBLE_11M       (RATE_CODE_11M | RATE_CODE_LONG_PREAMBLE)

/* CCK mode, short preamble */
#define TX_RATE_SHORT_PREAMBLE_2M       (RATE_CODE_2M | RATE_CODE_SHORT_PREAMBLE)
#define TX_RATE_SHORT_PREAMBLE_5_5M     (RATE_CODE_5_5M | RATE_CODE_SHORT_PREAMBLE)
#define TX_RATE_SHORT_PREAMBLE_11M      (RATE_CODE_11M | RATE_CODE_SHORT_PREAMBLE)

/* OFDM mode */
#define TX_RATE_OFDM_6M                 (RATE_CODE_6M | RATE_CODE_OFDM)
#define TX_RATE_OFDM_9M                 (RATE_CODE_9M | RATE_CODE_OFDM)
#define TX_RATE_OFDM_12M                (RATE_CODE_12M | RATE_CODE_OFDM)
#define TX_RATE_OFDM_18M                (RATE_CODE_18M | RATE_CODE_OFDM)
#define TX_RATE_OFDM_24M                (RATE_CODE_24M | RATE_CODE_OFDM)
#define TX_RATE_OFDM_36M                (RATE_CODE_36M | RATE_CODE_OFDM)
#define TX_RATE_OFDM_48M                (RATE_CODE_48M | RATE_CODE_OFDM)
#define TX_RATE_OFDM_54M                (RATE_CODE_54M | RATE_CODE_OFDM)


/* IPN2220 Baseband control register (BBCR) offset definition */
#define BBCR_CR0                0
#define BBCR_CR1                1
#define BBCR_CR2                2
#define BBCR_CR3                3
#define BBCR_CR4                4
#define BBCR_CR5                5
#define BBCR_CR6                6
#define BBCR_CR7                7
#define BBCR_CR8                8
#define BBCR_CR9                9
#define BBCR_CR10               10
#define BBCR_CR11               11
#define BBCR_CR12               12
#define BBCR_CR13               13
#define BBCR_CR14               14
#define BBCR_CR15               15
#define BBCR_CR16               16
#define BBCR_CR17               17
#define BBCR_CR18               18
#define BBCR_CR19               19
#define BBCR_CR20               20
#define BBCR_CR21               21
#define BBCR_CR22               22
#define BBCR_CR23               23
#define BBCR_CR24               24
#define BBCR_CR25               25
#define BBCR_CR26               26
#define BBCR_CR27               27
#define BBCR_CR28               28
#define BBCR_CR29               29
#define BBCR_CR30               30
#define BBCR_CR31               31
#define BBCR_CR32               32
#define BBCR_CR33               33
#define BBCR_CR34               34
#define BBCR_CR35               35
#define BBCR_CR36               36
#define BBCR_CR37               37
#define BBCR_CR38               38
#define BBCR_CR39               39
#define BBCR_CR45               45
#define BBCR_CR46               46
#define BBCR_CR80               80
#define BBCR_CR81               81
#define BBCR_CR82               82
#define BBCR_CR83               83
#define BBCR_CR84               84
#define BBCR_CR85               85
#define BBCR_CR86               86
#define BBCR_CR87               87
#define BBCR_CR88               88
#define BBCR_CR89               89
#define BBCR_CR90               90
#define BBCR_CR91               91
#define BBCR_CR92               92
#define BBCR_CR93               93
#define BBCR_CR94               94
#define BBCR_CR95               95
#define BBCR_CR96               96
#define BBCR_CR97               97
#define BBCR_CR98               98
#define BBCR_CR99               99
#define BBCR_CR100              100
#define BBCR_CR101              101
#define BBCR_CR102              102
#define BBCR_CR103              103
#define BBCR_CR104              104
#define BBCR_CR105              105
#define BBCR_CR106              106
#define BBCR_CR107              107
#define BBCR_CR108              108
#define BBCR_CR109              109
#define BBCR_CR110              110
#define BBCR_CR111              111
#define BBCR_CR112              112
#define BBCR_CR113              113
#define BBCR_CR114              114
#define BBCR_CR115              115
#define BBCR_CR116              116
#define BBCR_CR117              117
#define BBCR_CR118              118
#define BBCR_CR119              119
#define BBCR_CR120              120
#define BBCR_CR121              121
#define BBCR_CR122              122
#define BBCR_CR123              123
#define BBCR_CR124              124
#define BBCR_CR125              125
#define BBCR_CR126              126
#define BBCR_CR127              127
#define BBCR_CR128              128
#define BBCR_CR129              129

/* IPN2220 Baseband control register (BBCR) definition */
/* CR24 bit definitions */
#define CR24_RX_MODULATION_TYPE         BIT(7)
#define CR24_RX_OFDM                    BIT(7)
#define CR24_RX_CCK                     0x00
#define CR24_RX_PREAMBLE_TYPE           BIT(6)
#define CR24_RX_SHORT_PREAMBLE          BIT(6)
#define CR24_RX_LONG_PREAMBLE           0x00
#define CR24_RX_RATE                    BITS(2,5)
#define CR24_RX_RATE_1M                 (RATE_CODE_1M << 2)
#define CR24_RX_RATE_2M                 (RATE_CODE_2M << 2)
#define CR24_RX_RATE_5_5M               (RATE_CODE_5_5M << 2)
#define CR24_RX_RATE_11M                (RATE_CODE_11M << 2)
#define CR24_RX_RATE_6M                 (RATE_CODE_6M << 2)
#define CR24_RX_RATE_9M                 (RATE_CODE_9M << 2)
#define CR24_RX_RATE_12M                (RATE_CODE_12M << 2)
#define CR24_RX_RATE_18M                (RATE_CODE_18M << 2)
#define CR24_RX_RATE_24M                (RATE_CODE_24M << 2)
#define CR24_RX_RATE_36M                (RATE_CODE_36M << 2)
#define CR24_RX_RATE_48M                (RATE_CODE_48M << 2)
#define CR24_RX_RATE_54M                (RATE_CODE_54M << 2)

/* CR29 bit definitions */
#define CR29_RX_ANTENNA                 BIT(7)
#define CR29_LNA_STATUS                 BIT(5)
#define CR29_VGA_GAIN                   BITS(0,4)




/************************************************************************
*                         D A T A   T Y P E S
*************************************************************************
*/



/************************************************************************
*                             M A C R O S
*************************************************************************
*/

/************************************************************************
*                    D A T A   D E C L A R A T I O N S
*************************************************************************
*/

/************************************************************************
*                 F U N C T I O N   D E C L A R A T I O N S
*************************************************************************
*/

#endif /* _I2220REG_H */

