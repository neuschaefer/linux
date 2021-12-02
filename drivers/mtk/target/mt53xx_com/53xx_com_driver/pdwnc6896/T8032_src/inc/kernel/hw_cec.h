
#ifndef _HW_HDMICEC_H_
#define _HW_HDMICEC_H_

#define CEC_PIN_MUX	0x00b4
	#define CEC_MUX Fld(2, 0, AC_MSKB0) 
//-----------------------------------------------
#define TR_CONFIG 0x0500
 #define TR_CONFIG_0 0x0500
 #define TR_CONFIG_1 0x0501
 #define TR_CONFIG_2 0x0502
 #define TR_CONFIG_3 0x0503
 	#define BYPASS Fld(1, 28, AC_MSKB3) //28
  	#define DEVICE_ADDR3 Fld(4, 24, AC_MSKB3) //27:24
	#define DEVICE_ADDR2 Fld(4, 20, AC_MSKB2) //23:20
	#define DEVICE_ADDR1 Fld(4, 16, AC_MSKB2) //19:16
	#define TX_EN Fld(1, 12, AC_MSKB1) //12
	#define RX_EN Fld(1, 8, AC_MSKB1) //8
	#define CEC_ENABLE Fld(1, 0, AC_MSKB0) //0
#define CKGEN 0x0504
 #define CKGEN_0 0x0504
 #define CKGEN_1 0x0505
 #define CKGEN_2 0x0506
 #define CKGEN_3 0x0507
	#define PDN Fld(1, 16, AC_MSKB2) //16
	#define DIV_SEL Fld(16, 0, AC_FULLW10) //15:0
#define RX_T_START_R 0x0508
 #define RX_T_START_R_0 0x0508
 #define RX_T_START_R_1 0x0509
 #define RX_T_START_R_2 0x050a
 #define RX_T_START_R_3 0x050b
	#define RX_TIMER_START_R_MAX Fld(11, 16, AC_MSKW32) //26:16
	#define RX_TIMER_START_R_MIN Fld(11, 0, AC_MSKW10) //10:0
#define RX_T_START_F 0x050c
 #define RX_T_START_F_0 0x050c
 #define RX_T_START_F_1 0x050d
 #define RX_T_START_F_2 0x050e
 #define RX_T_START_F_3 0x050f
	#define RX_TIMER_START_F_MAX Fld(11, 16, AC_MSKW32) //26:16
	#define RX_TIMER_START_F_MIN Fld(11, 0, AC_MSKW10) //10:0
#define RX_T_DATA 0x0510
 #define RX_T_DATA_0 0x0510
 #define RX_T_DATA_1 0x0511
 #define RX_T_DATA_2 0x0512
 #define RX_T_DATA_3 0x0513
	#define RX_TIMER_DATA_SAMPLE Fld(11, 16, AC_MSKW32) //26:16
	#define RX_TIMER_DATA_F_MIN Fld(11, 0, AC_MSKW10) //10:0
#define RX_T_ACK 0x0514
 #define RX_T_ACK_0 0x0514
 #define RX_T_ACK_1 0x0515
 #define RX_T_ACK_2 0x0516
 #define RX_T_ACK_3 0x0517
	#define RX_TIMER_ACK_R Fld(11, 0, AC_MSKW10) //10:0
#define RX_T_ERROR 0x0518
 #define RX_T_ERROR_0 0x0518
 #define RX_T_ERROR_1 0x0519
 #define RX_T_ERROR_2 0x051a
 #define RX_T_ERROR_3 0x051b
	#define RX_TIMER_ERROR_D Fld(11, 16, AC_MSKW32) //26:16
	#define RX_TIMER_ERROR_S Fld(11, 0, AC_MSKW10) //10:0
#define TX_T_START 0x051c
 #define TX_T_START_0 0x051c
 #define TX_T_START_1 0x051d
 #define TX_T_START_2 0x051e
 #define TX_T_START_3 0x051f
	#define TX_TIMER_START_F Fld(11, 16, AC_MSKW32) //26:16
	#define TX_TIMER_START_R Fld(11, 0, AC_MSKW10) //10:0
#define TX_T_DATA_R 0x0520
 #define TX_T_DATA_R_0 0x0520
 #define TX_T_DATA_R_1 0x0521
 #define TX_T_DATA_R_2 0x0522
 #define TX_T_DATA_R_3 0x0523
	#define TX_TIMER_BIT1_R Fld(11, 16, AC_MSKW32) //26:16
	#define TX_TIMER_BIT0_R Fld(11, 0, AC_MSKW10) //10:0
#define TX_T_DATA_F 0x0524
 #define TX_T_DATA_F_0 0x0524
 #define TX_T_DATA_F_1 0x0525
 #define TX_T_DATA_F_2 0x0526
 #define TX_T_DATA_F_3 0x0527
	#define TX_TIMER_DATA_N Fld(11, 16, AC_MSKW32) //26:16
	#define TX_TIMER_DATA_F Fld(11, 0, AC_MSKW10) //10:0
#define TX_ARB 0x0528
 #define TX_ARB_0 0x0528
 #define TX_ARB_1 0x0529
 #define TX_ARB_2 0x052a
 #define TX_ARB_3 0x052b
 	#define TX_INPUT_DELAY Fld(5, 25, AC_MSKB3) //29:25
   	#define RX_CHK_DST Fld(1, 24, AC_MSKB3) //24
  	#define TX_LEAVE_FAIL Fld(1, 17, AC_MSKB2) //17
 	#define TX_NEW_RETRY_CNT Fld(1, 16, AC_MSKB2) //16
	#define MAX_RETRANSMIT Fld(4, 12, AC_MSKB1) //15:12
	#define BCNT_RETRANSMIT Fld(4, 8, AC_MSKB1) //11:8
	#define BCNT_NEW_MSG Fld(4, 4, AC_MSKB0) //7:4
	#define BCNT_NEW_INIT Fld(4, 0, AC_MSKB0) // 3:0

#ifdef MT5387_CEC
/*
5387 CEC only
*/
 #define RX_HEADER 0x0540
 #define RX_HEADER_0 0x0540
 #define RX_HEADER_1 0x0541
 #define RX_HEADER_2 0x0542
 #define RX_HEADER_3 0x0543
	#define RXED_M3_DATA_MASK Fld(4, 24, AC_MSKB3) //27:24
	#define RXED_SRC Fld(4, 20, AC_MSKB2) //23:20
	#define RXED_DST Fld(4, 16, AC_MSKB2) //19:16
	#define RXED_H_EOM Fld(1, 15, AC_MSKB1) //15
	#define RXED_D_EOM Fld(1, 13, AC_MSKB1) //13

#define RX_DATA 0x0544
 #define RX_DATA_0 0x0544
 #define RX_DATA_1 0x0545
 #define RX_DATA_2 0x0546
 #define RX_DATA_3 0x0547
	#define RXED_DATA Fld(32, 0, AC_FULLDW) //31:0

#define RX_HD_NEXT 0x0548
 #define RX_HD_NEXT_0 0x0548
 #define RX_HD_NEXT_1 0x0549
 #define RX_HD_NEXT_2 0x054a
 #define RX_HD_NEXT_3 0x054b
	#define RXING_M3_DATA_MASK Fld(4, 24, AC_MSKB3) //27:24
	#define RXING_SRC Fld(4, 20, AC_MSKB2) //23:20
	#define RXING_DST Fld(4, 16, AC_MSKB2) //19:16
	#define RXING_H_EOM Fld(1, 15, AC_MSKB1) //15
	#define RXING_H_ACK Fld(1, 14, AC_MSKB1) //14
	#define RXING_D_EOM Fld(1, 13, AC_MSKB1) //13
	#define RXING_D_ACK Fld(1, 12, AC_MSKB1) //12

#define RX_DATA_NEXT 0x054c
 #define RX_DATA_NEXT_0 0x054c
 #define RX_DATA_NEXT_1 0x054d
 #define RX_DATA_NEXT_2 0x054e
 #define RX_DATA_NEXT_3 0x054f
	#define RXING_DATA Fld(32, 0, AC_FULLDW) //31:0

#define RX_EVENT 0x0554
 #define RX_EVENT_0 0x0554
 #define RX_EVENT_1 0x0555
 #define RX_EVENT_2 0x0556
 #define RX_EVENT_3 0x0557
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
	#define I_EN_SB Fld(1, 1, AC_MSKB0) //1
	#define I_EN_BR Fld(1, 0, AC_MSKB0) //0

#define RX_FAIL 0x0568
 #define RX_FAIL_0 0x0568
 #define RX_FAIL_1 0x0569
 #define RX_FAIL_2 0x056a
 #define RX_FAIL_3 0x056b
	#define ERR_ONCE Fld(1, 4, AC_MSKB0) //4
	#define ERR_H Fld(1, 0, AC_MSKB0) //0

#define RX_STATUS 0x056c
 #define RX_STATUS_0 0x056c
 #define RX_STATUS_1 0x056d
 #define RX_STATUS_2 0x056e
 #define RX_STATUS_3 0x056f
	#define RX_BIT_COUNTER Fld(4, 28, AC_MSKB3) //31:28
	#define RX_TIMER Fld(11, 16, AC_MSKW32) //26:16
	#define RX_FSM Fld(7, 0, AC_MSKB0) //6:0

#define TX_HD_NEXT 0x0580
 #define TX_HD_NEXT_0 0x0580
 #define TX_HD_NEXT_1 0x0581
 #define TX_HD_NEXT_2 0x0582
 #define TX_HD_NEXT_3 0x0583
	#define WTX_M3_DATA_MASK Fld(4, 24, AC_MSKB3) //27:24
	#define WTX_SRC Fld(4, 20, AC_MSKB2) //23:20
	#define WTX_DST Fld(4, 16, AC_MSKB2) //19:16
	#define WTX_H_EOM Fld(1, 15, AC_MSKB1) //15
	#define WTX_D_EOM Fld(1, 13, AC_MSKB1) //13
#define TX_DATA_NEXT 0x0584
 #define TX_DATA_NEXT_0 0x0584
 #define TX_DATA_NEXT_1 0x0585
 #define TX_DATA_NEXT_2 0x0586
 #define TX_DATA_NEXT_3 0x0587
	#define WTX_DATA Fld(32, 0, AC_FULLDW) //31:0
#define TX_HEADER 0x0588
 #define TX_HEADER_0 0x0588
 #define TX_HEADER_1 0x0589
 #define TX_HEADER_2 0x058a
 #define TX_HEADER_3 0x058b
	#define TXING_M3_DATA_SENT Fld(4, 28, AC_MSKB3) //31:28
	#define TXING_M3_DATA_MASK Fld(4, 24, AC_MSKB3) //27:24
	#define TXING_SRC Fld(4, 20, AC_MSKB2) //23:20
	#define TXING_DST Fld(4, 16, AC_MSKB2) //19:16
	#define TXING_H_EOM Fld(1, 15, AC_MSKB1) //15
	#define TXING_H_ACK Fld(1, 14, AC_MSKB1) //14
	#define TXING_D_EOM Fld(1, 13, AC_MSKB1) //13
	#define TXING_D_ACK Fld(1, 12, AC_MSKB1) //12
#define TX_DATA 0x058c
 #define TX_DATA_0 0x058c
 #define TX_DATA_1 0x058d
 #define TX_DATA_2 0x058e
 #define TX_DATA_3 0x058f
	#define TXING_DATA Fld(32, 0, AC_FULLDW) //31:0
#define TX_EVENT 0x0594
 #define TX_EVENT_0 0x0594
 #define TX_EVENT_1 0x0595
 #define TX_EVENT_2 0x0596
 #define TX_EVENT_3 0x0597
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
#define TX_FAIL 0x05a8
 #define TX_FAIL_0 0x05a8
 #define TX_FAIL_1 0x05a9
 #define TX_FAIL_2 0x05aa
 #define TX_FAIL_3 0x05ab
	#define RETX_MAX Fld(1, 28, AC_MSKB3) //28
	#define DATA Fld(1, 24, AC_MSKB3) //24
	#define HEADER_ACK Fld(1, 17, AC_MSKB2) //17
	#define HEADER Fld(1, 16, AC_MSKB2) //16
	#define SOURCE Fld(1, 12, AC_MSKB1) //12
	#define MODE Fld(1, 4, AC_MSKB0) //4
#define TX_STATUS 0x05ac
 #define TX_STATUS_0 0x05ac
 #define TX_STATUS_1 0x05ad
 #define TX_STATUS_2 0x05ae
 #define TX_STATUS_3 0x05af
	#define TX_BIT_COUNTER Fld(4, 28, AC_MSKB3) //31:28
	#define TX_TIMER Fld(11, 16, AC_MSKW32) //26:16
	#define TX_FSM Fld(9, 0, AC_MSKW10) //8:0
#define TR_TEST 0x05bc
 #define TR_TEST_0 0x05bc
 #define TR_TEST_1 0x05bd
 #define TR_TEST_2 0x05be
 #define TR_TEST_3 0x05bf
	#define TX_EOM_EN Fld(1, 15, AC_MSKB1) //15
	#define BAK Fld(15, 0, AC_FULLW10) //14:0

#else
/*
5363, 5301A, 5365, 5395, and later.
*/
 #define RX_HEADER 0x052c
 #define RX_HEADER_0 0x052c
 #define RX_HEADER_1 0x052d
 #define RX_HEADER_2 0x052e
 #define RX_HEADER_3 0x052f
	#define RXED_M3_DATA_MASK Fld(4, 24, AC_MSKB3) //27:24
	#define RXED_SRC Fld(4, 20, AC_MSKB2) //23:20
	#define RXED_DST Fld(4, 16, AC_MSKB2) //19:16
	#define RXED_H_EOM Fld(1, 15, AC_MSKB1) //15
	#define RXED_D_EOM Fld(1, 13, AC_MSKB1) //13

 #define RX_DATA 0x0530
 #define RX_DATA_0 0x0530
 #define RX_DATA_1 0x0531
 #define RX_DATA_2 0x0532
 #define RX_DATA_3 0x0533
	#define RXED_DATA Fld(32, 0, AC_FULLDW) //31:0

 #define RX_HD_NEXT 0x0534
 #define RX_HD_NEXT_0 0x0534
 #define RX_HD_NEXT_1 0x0535
 #define RX_HD_NEXT_2 0x0536
 #define RX_HD_NEXT_3 0x0537
	#define RXING_M3_DATA_MASK Fld(4, 24, AC_MSKB3) //27:24
	#define RXING_SRC Fld(4, 20, AC_MSKB2) //23:20
	#define RXING_DST Fld(4, 16, AC_MSKB2) //19:16
	#define RXING_H_EOM Fld(1, 15, AC_MSKB1) //15
	#define RXING_H_ACK Fld(1, 14, AC_MSKB1) //14
	#define RXING_D_EOM Fld(1, 13, AC_MSKB1) //13
	#define RXING_D_ACK Fld(1, 12, AC_MSKB1) //12

 #define RX_DATA_NEXT 0x0538
 #define RX_DATA_NEXT_0 0x0538
 #define RX_DATA_NEXT_1 0x0539
 #define RX_DATA_NEXT_2 0x053a
 #define RX_DATA_NEXT_3 0x053b
	#define RXING_DATA Fld(32, 0, AC_FULLDW) //31:0

 #define RX_EVENT 0x0540
 #define RX_EVENT_0 0x0540
 #define RX_EVENT_1 0x0541
 #define RX_EVENT_2 0x0542
 #define RX_EVENT_3 0x0543
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

 #define RX_FAIL 0x0544
 #define RX_FAIL_0 0x0544
 #define RX_FAIL_1 0x0545
 #define RX_FAIL_2 0x0546
 #define RX_FAIL_3 0x0547
	#define ERR_ONCE Fld(1, 4, AC_MSKB0) // 4
	#define ERR_H Fld(1, 0, AC_MSKB0) //0

 #define RX_STATUS 0x0548
 #define RX_STATUS_0 0x0548
 #define RX_STATUS_1 0x0549
 #define RX_STATUS_2 0x054a
 #define RX_STATUS_3 0x054b
	#define RX_BIT_COUNTER Fld(4, 28, AC_MSKB3) //31:28
	#define RX_TIMER Fld(11, 16, AC_MSKW32) //26:16
	#define RX_FSM Fld(7, 0, AC_MSKB0) //6:0

 #define TX_HD_NEXT 0x054c
 #define TX_HD_NEXT_0 0x054c
 #define TX_HD_NEXT_1 0x054d
 #define TX_HD_NEXT_2 0x054e
 #define TX_HD_NEXT_3 0x054f
 	#define WTX_M3_DATA_MASK Fld(4, 24, AC_MSKB3) //27:24
	#define WTX_SRC Fld(4, 20, AC_MSKB2) //23:20
	#define WTX_DST Fld(4, 16, AC_MSKB2) //19:16
	#define WTX_H_EOM Fld(1, 15, AC_MSKB1) //15
	#define WTX_D_EOM Fld(1, 13, AC_MSKB1) //13

 #define TX_DATA_NEXT 0x0550
 #define TX_DATA_NEXT_0 0x0550
 #define TX_DATA_NEXT_1 0x0551
 #define TX_DATA_NEXT_2 0x0552
 #define TX_DATA_NEXT_3 0x0553
 	#define WTX_DATA Fld(32, 0, AC_FULLDW) //31:0

 #define TX_HEADER 0x0554
 #define TX_HEADER_0 0x0554
 #define TX_HEADER_1 0x0555
 #define TX_HEADER_2 0x0556
 #define TX_HEADER_3 0x0557
	#define TXING_M3_DATA_SENT Fld(4, 28, AC_MSKB3) //31:28
	#define TXING_M3_DATA_MASK Fld(4, 24, AC_MSKB3) //27:24
	#define TXING_SRC Fld(4, 20, AC_MSKB2) //23:20
	#define TXING_DST Fld(4, 16, AC_MSKB2) //19:16
	#define TXING_H_EOM Fld(1, 15, AC_MSKB1) //15
	#define TXING_H_ACK Fld(1, 14, AC_MSKB1) //14
	#define TXING_D_EOM Fld(1, 13, AC_MSKB1) //13
	#define TXING_D_ACK Fld(1, 12, AC_MSKB1) //12

 #define TX_DATA 0x0558
 #define TX_DATA_0 0x0558
 #define TX_DATA_1 0x0559
 #define TX_DATA_2 0x055a
 #define TX_DATA_3 0x055b
 	#define TXING_DATA Fld(32, 0, AC_FULLDW) //31:0

 #define TX_EVENT 0x0560
 #define TX_EVENT_0 0x0560
 #define TX_EVENT_1 0x0561
 #define TX_EVENT_2 0x0562
 #define TX_EVENT_3 0x0563
 	#define TX_D_FINISH Fld(1, 21, AC_MSKB2) //21
	#define UN Fld(1, 20, AC_MSKB2) //20
	#define FAIL Fld(1, 19, AC_MSKB2) //19
	#define LOW Fld(1, 18, AC_MSKB2) //18
	#define TX_DATA_FINISH Fld(1, 17, AC_MSKB2) //17
	#define RB_RDY Fld(1, 16, AC_MSKB2) //16
	#define TX_DATA_FIN_E Fld(1, 5, AC_MSKB0) //5
	#define I_EN_UN Fld(1, 4, AC_MSKB0) //4
	#define I_EN_FAIL Fld(1, 3, AC_MSKB0) //3
	#define I_EN_LOW Fld(1, 2, AC_MSKB0) // 2  
	#define I_EN_BS Fld(1, 1, AC_MSKB0) // 1
	#define I_EN_RB Fld(1, 0, AC_MSKB0) //0

 #define TX_FAIL 0x0564
 #define TX_FAIL_0 0x0564
 #define TX_FAIL_1 0x0565
 #define TX_FAIL_2 0x0566
 #define TX_FAIL_3 0x0567
 	#define RETX_MAX Fld(1, 28, AC_MSKB3) //28
	#define DATA Fld(1, 24, AC_MSKB3) //24
	#define HEADER_ACK Fld(1, 17, AC_MSKB2) //17
	#define HEADER Fld(1, 16, AC_MSKB2) //16
	#define SOURCE Fld(1, 12, AC_MSKB1) //12
	#define MODE Fld(1, 4, AC_MSKB0)

 #define TX_STATUS 0x0568
 #define TX_STATUS_0 0x0568
 #define TX_STATUS_1 0x0569
 #define TX_STATUS_2 0x056a
 #define TX_STATUS_3 0x056b
 	#define TX_BIT_COUNTER Fld(4, 22, AC_MSKW10) //25:22
	#define TX_TIMER Fld(11, 10, AC_MSKW32) //20:10
	#define TX_FSM Fld(9, 0, AC_MSKW10) //8:0

 #define TR_TEST 0x056c
 #define TR_TEST_0 0x056c
 #define TR_TEST_1 0x056d
 #define TR_TEST_2 0x056e
 #define TR_TEST_3 0x056f
 	#define TX_EOM_EN Fld(1, 15, AC_MSKB1) //15
	#define BAK Fld(15, 0, AC_FULLW10) //14:0

#endif
#endif

