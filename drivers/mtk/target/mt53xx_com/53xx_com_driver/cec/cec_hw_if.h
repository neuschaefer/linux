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
#ifndef CEC_HW_IF
#define CEC_HW_IF

#define TR_CONFIG 0x8500
 #define TR_CONFIG_0 0x8500
 #define TR_CONFIG_1 0x8501
 #define TR_CONFIG_2 0x8502
 #define TR_CONFIG_3 0x8503
 	#define BYPASS Fld(1, 28, AC_MSKB3) //28
  	#define DEVICE_ADDR3 Fld(4, 24, AC_MSKB3) //27:24
	#define DEVICE_ADDR2 Fld(4, 20, AC_MSKB2) //23:20
	#define DEVICE_ADDR1 Fld(4, 16, AC_MSKB2) //19:16
	#define TX_EN Fld(1, 12, AC_MSKB1) //12
	#define RX_EN Fld(1, 8, AC_MSKB1) //8
#if !defined(CC_MT5387) && !defined(CC_MT5363)
        #define TX_INT_EN1 Fld(1,7,AC_MSKB0)//[7:7]
        #define TX_INT_EN0 Fld(1,6,AC_MSKB0)//[6:6]
        #define RX_INT_EN1 Fld(1,5,AC_MSKB0)//[5:5]
        #define RX_INT_EN0 Fld(1,4,AC_MSKB0)//[4:4]
#endif
	#define CEC_ENABLE Fld(1, 0, AC_MSKB0) //0
#define CKGEN 0x8504
 #define CKGEN_0 0x8504
 #define CKGEN_1 0x8505
 #define CKGEN_2 0x8506
 #define CKGEN_3 0x8507
	#define PDN Fld(1, 16, AC_MSKB2) //16
	#define DIV_SEL Fld(16, 0, AC_FULLW10) //15:0
#define RX_T_START_R 0x8508
 #define RX_T_START_R_0 0x8508
 #define RX_T_START_R_1 0x8509
 #define RX_T_START_R_2 0x850a
 #define RX_T_START_R_3 0x850b
	#define RX_TIMER_START_R_MAX Fld(11, 16, AC_MSKW32) //26:16
	#define RX_TIMER_START_R_MIN Fld(11, 0, AC_MSKW10) //10:0
#define RX_T_START_F 0x850c
 #define RX_T_START_F_0 0x850c
 #define RX_T_START_F_1 0x850d
 #define RX_T_START_F_2 0x850e
 #define RX_T_START_F_3 0x850f
	#define RX_TIMER_START_F_MAX Fld(11, 16, AC_MSKW32) //26:16
	#define RX_TIMER_START_F_MIN Fld(11, 0, AC_MSKW10) //10:0
#define RX_T_DATA 0x8510
 #define RX_T_DATA_0 0x8510
 #define RX_T_DATA_1 0x8511
 #define RX_T_DATA_2 0x8512
 #define RX_T_DATA_3 0x8513
	#define RX_TIMER_DATA_SAMPLE Fld(11, 16, AC_MSKW32) //26:16
	#define RX_TIMER_DATA_F_MIN Fld(11, 0, AC_MSKW10) //10:0
#define RX_T_ACK 0x8514
 #define RX_T_ACK_0 0x8514
 #define RX_T_ACK_1 0x8515
 #define RX_T_ACK_2 0x8516
 #define RX_T_ACK_3 0x8517
	#define RX_TIMER_ACK_R Fld(11, 0, AC_MSKW10) //10:0
#define RX_T_ERROR 0x8518
 #define RX_T_ERROR_0 0x8518
 #define RX_T_ERROR_1 0x8519
 #define RX_T_ERROR_2 0x851a
 #define RX_T_ERROR_3 0x851b
	#define RX_TIMER_ERROR_D Fld(11, 16, AC_MSKW32) //26:16
	#define RX_TIMER_ERROR_S Fld(11, 0, AC_MSKW10) //10:0
#define TX_T_START 0x851c
 #define TX_T_START_0 0x851c
 #define TX_T_START_1 0x851d
 #define TX_T_START_2 0x851e
 #define TX_T_START_3 0x851f
	#define TX_TIMER_START_F Fld(11, 16, AC_MSKW32) //26:16
	#define TX_TIMER_START_R Fld(11, 0, AC_MSKW10) //10:0
#define TX_T_DATA_R 0x8520
 #define TX_T_DATA_R_0 0x8520
 #define TX_T_DATA_R_1 0x8521
 #define TX_T_DATA_R_2 0x8522
 #define TX_T_DATA_R_3 0x8523
	#define TX_TIMER_BIT1_R Fld(11, 16, AC_MSKW32) //26:16
	#define TX_TIMER_BIT0_R Fld(11, 0, AC_MSKW10) //10:0
#define TX_T_DATA_F 0x8524
 #define TX_T_DATA_F_0 0x8524
 #define TX_T_DATA_F_1 0x8525
 #define TX_T_DATA_F_2 0x8526
 #define TX_T_DATA_F_3 0x8527
	#define TX_TIMER_DATA_N Fld(11, 16, AC_MSKW32) //26:16
	#define TX_TIMER_DATA_F Fld(11, 0, AC_MSKW10) //10:0
#define TX_ARB 0x8528
 #define TX_ARB_0 0x8528
 #define TX_ARB_1 0x8529
 #define TX_ARB_2 0x852a
 #define TX_ARB_3 0x852b
 	#define TX_INPUT_DELAY Fld(5, 25, AC_MSKB3) //29:25
   	#define RX_CHK_DST Fld(1, 24, AC_MSKB3) //24
  	#define TX_LEAVE_FAIL Fld(1, 17, AC_MSKB2) //17
 	#define TX_NEW_RETRY_CNT Fld(1, 16, AC_MSKB2) //16
	#define MAX_RETRANSMIT Fld(4, 12, AC_MSKB1) //15:12
	#define BCNT_RETRANSMIT Fld(4, 8, AC_MSKB1) //11:8
	#define BCNT_NEW_MSG Fld(4, 4, AC_MSKB0) //7:4
	#define BCNT_NEW_INIT Fld(4, 0, AC_MSKB0) //3:0

// 5387 and others (5301A, 5363, 5365, 5395.. later.)
#if defined(CC_MT5387) && !defined(CC_MT5363) && !defined(CC_MT5301A)
/* 5387 only */

 #define RX_HEADER 0x8540
 #define RX_HEADER_0 0x8540
 #define RX_HEADER_1 0x8541
 #define RX_HEADER_2 0x8542
 #define RX_HEADER_3 0x8543
	#define RXED_M3_DATA_MASK Fld(4, 24, AC_MSKB3) //27:24
	#define RXED_SRC Fld(4, 20, AC_MSKB2) //23:20
	#define RXED_DST Fld(4, 16, AC_MSKB2) //19:16
	#define RXED_H_EOM Fld(1, 15, AC_MSKB1) //15
	#define RXED_D_EOM Fld(1, 13, AC_MSKB1) //13

 #define RX_DATA 0x8544
 #define RX_DATA_0 0x8544
 #define RX_DATA_1 0x8545
 #define RX_DATA_2 0x8546
 #define RX_DATA_3 0x8547
	#define RXED_DATA Fld(32, 0, AC_FULLDW) //31:0

 #define RX_HD_NEXT 0x8548
 #define RX_HD_NEXT_0 0x8548
 #define RX_HD_NEXT_1 0x8549
 #define RX_HD_NEXT_2 0x854a
 #define RX_HD_NEXT_3 0x854b
	#define RXING_M3_DATA_MASK Fld(4, 24, AC_MSKB3) //27:24
	#define RXING_SRC Fld(4, 20, AC_MSKB2) //23:20
	#define RXING_DST Fld(4, 16, AC_MSKB2) //19:16
	#define RXING_H_EOM Fld(1, 15, AC_MSKB1) //15
	#define RXING_H_ACK Fld(1, 14, AC_MSKB1) //14
	#define RXING_D_EOM Fld(1, 13, AC_MSKB1) //13
	#define RXING_D_ACK Fld(1, 12, AC_MSKB1) //12

 #define RX_DATA_NEXT 0x854c
 #define RX_DATA_NEXT_0 0x854c
 #define RX_DATA_NEXT_1 0x854d
 #define RX_DATA_NEXT_2 0x854e
 #define RX_DATA_NEXT_3 0x854f
	#define RXING_DATA Fld(32, 0, AC_FULLDW) //31:0

 #define RX_EVENT 0x8554
 #define RX_EVENT_0 0x8554
 #define RX_EVENT_1 0x8555
 #define RX_EVENT_2 0x8556
 #define RX_EVENT_3 0x8557
	#define DATA_RDY Fld(1, 23, AC_MSKB2) //23
	#define HEADER_RDY Fld(1, 22, AC_MSKB2) //22
	#define OV Fld(1, 20, AC_MSKB2) //20
	#define BR_SB_RDY Fld(1, 18, AC_MSKB2) //18
	#define SB_RDY Fld(1, 17, AC_MSKB2) //17
	#define BR_RDY Fld(1, 16, AC_MSKB2) //16
	#define I_EN_DATA Fld(1, 7, AC_MSKB0) //7
	#define I_EN_HEADER Fld(1, 6, AC_MSKB0) //6
	#define I_EN_OV Fld(1, 4, AC_MSKB0) //4
	#define I_EN_PULSE Fld(1, 3, AC_MSKB0) //3
	#define I_EN_BR_SB Fld(1, 2, AC_MSKB0) //2
	#define I_EN_SB  Fld(1, 1, AC_MSKB0) //1
	#define I_EN_BR Fld(1, 0, AC_MSKB0) //0

 #define RX_FAIL 0x8568
 #define RX_FAIL_0 0x8568
 #define RX_FAIL_1 0x8569
 #define RX_FAIL_2 0x856a
 #define RX_FAIL_3 0x856b
	#define ERR_ONCE Fld(1, 4, AC_MSKB0) //4
	#define ERR_H Fld(1, 0, AC_MSKB0) //0

 #define RX_STATUS 0x856c
 #define RX_STATUS_0 0x856c
 #define RX_STATUS_1 0x856d
 #define RX_STATUS_2 0x856e
 #define RX_STATUS_3 0x856f
	#define RX_BIT_COUNTER Fld(4, 28, AC_MSKB3) //31:28
	#define RX_TIMER Fld(11, 16, AC_MSKW32) //26:16
	#define RX_FSM Fld(7, 0, AC_MSKB0) //6:0

#define TX_HD_NEXT 0x8580
 #define TX_HD_NEXT_0 0x8580
 #define TX_HD_NEXT_1 0x8581
 #define TX_HD_NEXT_2 0x8582
 #define TX_HD_NEXT_3 0x8583
	#define WTX_M3_DATA_MASK Fld(4, 24, AC_MSKB3) //27:24
	#define WTX_SRC Fld(4, 20, AC_MSKB2) //23:20
	#define WTX_DST Fld(4, 16, AC_MSKB2) //19:16
	#define WTX_H_EOM Fld(1, 15, AC_MSKB1) //15
	#define WTX_D_EOM Fld(1, 13, AC_MSKB1) //13
#define TX_DATA_NEXT 0x8584
 #define TX_DATA_NEXT_0 0x8584
 #define TX_DATA_NEXT_1 0x8585
 #define TX_DATA_NEXT_2 0x8586
 #define TX_DATA_NEXT_3 0x8587
	#define WTX_DATA Fld(32, 0, AC_FULLDW) //31:0
#define TX_HEADER 0x8588
 #define TX_HEADER_0 0x8588
 #define TX_HEADER_1 0x8589
 #define TX_HEADER_2 0x858a
 #define TX_HEADER_3 0x858b
	#define TXING_M3_DATA_SENT Fld(4, 28, AC_MSKB3) //31:28
	#define TXING_M3_DATA_MASK Fld(4, 24, AC_MSKB3) //27:24
	#define TXING_SRC Fld(4, 20, AC_MSKB2) //23:20
	#define TXING_DST Fld(4, 16, AC_MSKB2) //19:16
	#define TXING_H_EOM Fld(1, 15, AC_MSKB1) //15
	#define TXING_H_ACK Fld(1, 14, AC_MSKB1) //14
	#define TXING_D_EOM Fld(1, 13, AC_MSKB1) //13
	#define TXING_D_ACK Fld(1, 12, AC_MSKB1) //12
#define TX_DATA 0x858c
 #define TX_DATA_0 0x858c
 #define TX_DATA_1 0x858d
 #define TX_DATA_2 0x858e
 #define TX_DATA_3 0x858f
	#define TXING_DATA Fld(32, 0, AC_FULLDW) //31:0
#define TX_EVENT 0x8594
 #define TX_EVENT_0 0x8594
 #define TX_EVENT_1 0x8595
 #define TX_EVENT_2 0x8596
 #define TX_EVENT_3 0x8597
 	#define TX_D_FINISH Fld(1, 21, AC_MSKB2) //21
	#define UN Fld(1, 20, AC_MSKB2) //20
	#define FAIL Fld(1, 19, AC_MSKB2) //19
	#define LOW Fld(1, 18, AC_MSKB2) //18
	#define TX_DATA_FINISH Fld(1, 17, AC_MSKB2) //17
	#define RB_RDY Fld(1, 16, AC_MSKB2) //16
	#define TX_DATA_FIN_E Fld(1, 5, AC_MSKB0) //5
	#define I_EN_UN Fld(1, 4, AC_MSKB0) //4
	#define I_EN_FAIL Fld(1, 3, AC_MSKB0) //3
	#define I_EN_LOW Fld(1, 2, AC_MSKB0) //2
	#define I_EN_BS Fld(1, 1, AC_MSKB0) //1
	#define I_EN_RB Fld(1, 0, AC_MSKB0) //0
#define TX_FAIL 0x85a8
 #define TX_FAIL_0 0x85a8
 #define TX_FAIL_1 0x85a9
 #define TX_FAIL_2 0x85aa
 #define TX_FAIL_3 0x85ab
	#define RETX_MAX Fld(1, 28, AC_MSKB3) //28
	#define DATA Fld(1, 24, AC_MSKB3) //24
	#define HEADER_ACK Fld(1, 17, AC_MSKB2) //17
	#define HEADER Fld(1, 16, AC_MSKB2) //16
	#define SOURCE Fld(1, 12, AC_MSKB1) //12
	#define MODE Fld(1, 4, AC_MSKB0) //4
#define TX_STATUS 0x85ac
 #define TX_STATUS_0 0x85ac
 #define TX_STATUS_1 0x85ad
 #define TX_STATUS_2 0x85ae
 #define TX_STATUS_3 0x85af
	#define TX_BIT_COUNTER Fld(4, 28, AC_MSKB3) //31:28
	#define TX_TIMER Fld(11, 16, AC_MSKW32) //26:16
	#define TX_FSM Fld(9, 0, AC_MSKW10) //8:0
#define TR_TEST 0x85bc
 #define TR_TEST_0 0x85bc
 #define TR_TEST_1 0x85bd
 #define TR_TEST_2 0x85be
 #define TR_TEST_3 0x85bf
	#define TX_EOM_EN Fld(1, 15, AC_MSKB1) //15
	#define BAK Fld(15, 0, AC_FULLW10) //14:0

#else
/* others (5301A, 5363, 5365, 5395.. later.) */

 #define RX_HEADER 0x852c
 #define RX_HEADER_0 0x852c
 #define RX_HEADER_1 0x852d
 #define RX_HEADER_2 0x852e
 #define RX_HEADER_3 0x852f
	#define RXED_M3_DATA_MASK Fld(4, 24, AC_MSKB3) //27:24
	#define RXED_SRC Fld(4, 20, AC_MSKB2) //23:20
	#define RXED_DST Fld(4, 16, AC_MSKB2) //19:16
	#define RXED_H_EOM Fld(1, 15, AC_MSKB1) //15
	#define RXED_D_EOM Fld(1, 13, AC_MSKB1) //13

 #define RX_DATA 0x8530
 #define RX_DATA_0 0x8530
 #define RX_DATA_1 0x8531
 #define RX_DATA_2 0x8532
 #define RX_DATA_3 0x8533
	#define RXED_DATA Fld(32, 0, AC_FULLDW) //31:0

 #define RX_HD_NEXT 0x8534
 #define RX_HD_NEXT_0 0x8534
 #define RX_HD_NEXT_1 0x8535
 #define RX_HD_NEXT_2 0x8536
 #define RX_HD_NEXT_3 0x8537
	#define RXING_M3_DATA_MASK Fld(4, 24, AC_MSKB3) //27:24
	#define RXING_SRC Fld(4, 20, AC_MSKB2) //23:20
	#define RXING_DST Fld(4, 16, AC_MSKB2) //19:16
	#define RXING_H_EOM Fld(1, 15, AC_MSKB1) //15
	#define RXING_H_ACK Fld(1, 14, AC_MSKB1) //14
	#define RXING_D_EOM Fld(1, 13, AC_MSKB1) //13
	#define RXING_D_ACK Fld(1, 12, AC_MSKB1) //12

 #define RX_DATA_NEXT 0x8538
 #define RX_DATA_NEXT_0 0x8538
 #define RX_DATA_NEXT_1 0x8539
 #define RX_DATA_NEXT_2 0x853a
 #define RX_DATA_NEXT_3 0x853b
	#define RXING_DATA Fld(32, 0, AC_FULLDW) //31:0

 #define RX_EVENT 0x8540
 #define RX_EVENT_0 0x8540
 #define RX_EVENT_1 0x8541
 #define RX_EVENT_2 0x8542
 #define RX_EVENT_3 0x8543
	#define DATA_RDY Fld(1, 23, AC_MSKB2) //23
	#define HEADER_RDY Fld(1, 22, AC_MSKB2) //22
	#define OV Fld(1, 20, AC_MSKB2) //20
	#define BR_SB_RDY Fld(1, 18, AC_MSKB2) //18
	#define SB_RDY Fld(1, 17, AC_MSKB2) //17
	#define BR_RDY Fld(1, 16, AC_MSKB2) //16
	#define I_EN_DATA Fld(1, 7, AC_MSKB0) //7
	#define I_EN_HEADER Fld(1, 6, AC_MSKB0) //6
	#define I_EN_OV Fld(1, 4, AC_MSKB0) //4
	#define I_EN_PULSE Fld(1, 3, AC_MSKB0) //3
	#define I_EN_BR_SB Fld(1, 2, AC_MSKB0) //2
	#define I_EN_SB  Fld(1, 1, AC_MSKB0) //1
	#define I_EN_BR Fld(1, 0, AC_MSKB0) //0

 #define RX_FAIL 0x8544
 #define RX_FAIL_0 0x8544
 #define RX_FAIL_1 0x8545
 #define RX_FAIL_2 0x8546
 #define RX_FAIL_3 0x8547
	#define ERR_ONCE Fld(1, 4, AC_MSKB0) // 4
	#define ERR_H Fld(1, 0, AC_MSKB0) //0

 #define RX_STATUS 0x8548
 #define RX_STATUS_0 0x8548
 #define RX_STATUS_1 0x8549
 #define RX_STATUS_2 0x854a
 #define RX_STATUS_3 0x854b
	#define RX_BIT_COUNTER Fld(4, 28, AC_MSKB3) //31:28
	#define RX_TIMER Fld(11, 16, AC_MSKW32) //26:16
	#define RX_FSM Fld(7, 0, AC_MSKB0) //6:0

 #define TX_HD_NEXT 0x854c
 #define TX_HD_NEXT_0 0x854c
 #define TX_HD_NEXT_1 0x854d
 #define TX_HD_NEXT_2 0x854e
 #define TX_HD_NEXT_3 0x854f
 	#define WTX_M3_DATA_MASK Fld(4, 24, AC_MSKB3) //27:24
	#define WTX_SRC Fld(4, 20, AC_MSKB2) //23:20
	#define WTX_DST Fld(4, 16, AC_MSKB2) //19:16
	#define WTX_H_EOM Fld(1, 15, AC_MSKB1) //15
	#define WTX_D_EOM Fld(1, 13, AC_MSKB1) //13

 #define TX_DATA_NEXT 0x8550
 #define TX_DATA_NEXT_0 0x8550
 #define TX_DATA_NEXT_1 0x8551
 #define TX_DATA_NEXT_2 0x8552
 #define TX_DATA_NEXT_3 0x8553
 	#define WTX_DATA Fld(32, 0, AC_FULLDW) //31:0

 #define TX_HEADER 0x8554
 #define TX_HEADER_0 0x8554
 #define TX_HEADER_1 0x8555
 #define TX_HEADER_2 0x8556
 #define TX_HEADER_3 0x8557
	#define TXING_M3_DATA_SENT Fld(4, 28, AC_MSKB3) //31:28
	#define TXING_M3_DATA_MASK Fld(4, 24, AC_MSKB3) //27:24
	#define TXING_SRC Fld(4, 20, AC_MSKB2) //23:20
	#define TXING_DST Fld(4, 16, AC_MSKB2) //19:16
	#define TXING_H_EOM Fld(1, 15, AC_MSKB1) //15
	#define TXING_H_ACK Fld(1, 14, AC_MSKB1) //14
	#define TXING_D_EOM Fld(1, 13, AC_MSKB1) //13
	#define TXING_D_ACK Fld(1, 12, AC_MSKB1) //12

 #define TX_DATA 0x8558
 #define TX_DATA_0 0x8558
 #define TX_DATA_1 0x8559
 #define TX_DATA_2 0x855a
 #define TX_DATA_3 0x855b
 	#define TXING_DATA Fld(32, 0, AC_FULLDW) //31:0

 #define TX_EVENT 0x8560
 #define TX_EVENT_0 0x8560
 #define TX_EVENT_1 0x8561
 #define TX_EVENT_2 0x8562
 #define TX_EVENT_3 0x8563
 	#define TX_D_FINISH Fld(1, 21, AC_MSKB2) //21
	#define UN Fld(1, 20, AC_MSKB2) //20
	#define FAIL Fld(1, 19, AC_MSKB2) //19
	#define LOW Fld(1, 18, AC_MSKB2) //18
	#define TX_DATA_FINISH Fld(1, 17, AC_MSKB2) //17
	#define RB_RDY Fld(1, 16, AC_MSKB2) //16
	#define TX_DATA_FIN_E Fld(1, 5, AC_MSKB0) //5
	#define I_EN_UN Fld(1, 4, AC_MSKB0) //4
	#define I_EN_FAIL Fld(1, 3, AC_MSKB0) //3
	#define I_EN_LOW Fld(1, 2, AC_MSKB0) //2
	#define I_EN_BS Fld(1, 1, AC_MSKB0) //1
	#define I_EN_RB Fld(1, 0, AC_MSKB0) //0

 #define TX_FAIL 0x8564
 #define TX_FAIL_0 0x8564
 #define TX_FAIL_1 0x8565
 #define TX_FAIL_2 0x8566
 #define TX_FAIL_3 0x8567
 	#define RETX_MAX Fld(1, 28, AC_MSKB3) //28
	#define DATA Fld(1, 24, AC_MSKB3) //24
	#define HEADER_ACK Fld(1, 17, AC_MSKB2) //17
	#define HEADER Fld(1, 16, AC_MSKB2) //16
	#define SOURCE Fld(1, 12, AC_MSKB1) //12
	#define MODE Fld(1, 4, AC_MSKB0)

 #define TX_STATUS 0x8568
 #define TX_STATUS_0 0x8568
 #define TX_STATUS_1 0x8569
 #define TX_STATUS_2 0x856a
 #define TX_STATUS_3 0x856b
 	#define TX_BIT_COUNTER Fld(4, 22, AC_MSKW10) //25:22
	#define TX_TIMER Fld(11, 10, AC_MSKW32) //20:10
	#define TX_FSM Fld(9, 0, AC_MSKW10) //8:0

 #define TR_TEST 0x856c
 #define TR_TEST_0 0x856c
 #define TR_TEST_1 0x856d
 #define TR_TEST_2 0x856e
 #define TR_TEST_3 0x856f
 	#define TX_EOM_EN Fld(1, 15, AC_MSKB1) //15
	#define BAK Fld(15, 0, AC_FULLW10) //14:0

#endif 

#endif /* CEC_HW_IF */
