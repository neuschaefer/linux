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
/********************************************************************************************
* LEGAL DISCLAIMER
*
* (Header of MediaTek Software/Firmware Release or Documentation)
*
 *     BY OPENING OR USING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES 
 *     THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE") RECEIVED 
 *     FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON AN "AS-IS" BASIS 
 *     ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, EXPRESS OR IMPLIED, 
 *     INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR 
 *     A PARTICULAR PURPOSE OR NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY 
 *     WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, 
 *     INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK 
 *     ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
 *     NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S SPECIFICATION 
 *     OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
 *     BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE LIABILITY WITH 
 *     RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION, 
 *     TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE 
 *     FEES OR SERVICE CHARGE PAID BY BUYER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE. 
*
 *     THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE WITH THE LAWS 
 *     OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF LAWS PRINCIPLES.  
 ************************************************************************************************/
/*****************************************************************************
 *
 * Filename:
 * ---------
 *   $Workfile: ether_reg.h $
 *
 * Project:
 * --------
 *
 *
 * Description:
 * ------------
 *   Ethernet LAN register header file
 *
 * Author:
 * -------
 *   HsHuang
 *
 * Last changed:
 * -------------
 * $Author:
 *
 * $Modtime:
 *
 * $Revision:
****************************************************************************/

#ifndef _ETHER_REG_H_
#define _ETHER_REG_H_

#include "x_hal_5381.h"

// *********************************************************************
// Ethernet Registers offset
// *********************************************************************
//#define MAC_REG_OFFSET  (ETHERNET_BASE + 0x0000)  //0x20032000    
//#define MMC_REG_OFFSET  (ETHERNET_BASE + 0x0100)  //0x20032100
//#define DMA_REG_OFFSET  (ETHERNET_BASE + 0x0200)  //0x20032200
#define ETHERNET_REG_OFFSET      	(ETHERNET_BASE + 0x000)
//#define CKGEN_REG_OFFSET      	    (CKGEN_BASE + 0x000)//0x2000d000

// *********************************************************************
// Ethernet Registers map
// *********************************************************************
#ifdef CC_MT5368

#define RW_PHY_CTRL0   	0x00                  
  #define RW_DATA				((UINT32)0xFFFF << 16)  
  #define RW_DATA_SHIFT			(16)  
  #define RW_OK            		((UINT32)0x01 << 15)  
  #define RD_CMD           		((UINT32)0x01 << 14)  
  #define WT_CMD         		((UINT32)0x01 << 13)   
  #define PHY_REG        		((UINT32)0x1F << 8)  
  #define PHY_ADDR       		((UINT32)0x1F << 0)   
  #define PHY_REG_SHIFT        	(8)  
  #define PHY_ADDR_SHIFT      	(0)   

#define RW_PHY_CTRL1   	0x04                  
  #define AUTO_POLL_DIS			((UINT32)0x01 << 31)  
  #define PHY_ADDR_AUTO      	((UINT32)0x1F << 24)  
  #define PHY_ADDR_AUTO_SHIFT 	(24)  
  #define SWH_CK_PWN           	((UINT32)0x01 << 23)  
  #define PHY_PORT_SEL         	((UINT32)0x01 << 22)   
  #define EXTER_MAC_SEL       	((UINT32)0x01 << 21)  
  #define INTER_PYH_PD          ((UINT32)0x01 << 19)   
  #define RGMII_PHY          	((UINT32)0x01 << 17)   
  #define REV_MII		       	((UINT32)0x01 << 16)   
  #define TXC_CHECK_EN          ((UINT32)0x01 << 14)   
  #define FORCE_FC_TX          	((UINT32)0x01 << 13)   
  #define FORCE_FC_RX          	((UINT32)0x01 << 12)   
  #define FORCE_DUPLEX          ((UINT32)0x01 << 11)   
  #define FORCE_DUPLEX_HALF     ((UINT32)0x00 << 11)  
  #define FORCE_DUPLEX_FULL     ((UINT32)0x01 << 11)  
  #define FORCE_SPEED          	((UINT32)0x03 << 9)   
    #define FORCE_SPEED_10M    	((UINT32)0x00 << 9)  
  	#define FORCE_SPEED_100M    ((UINT32)0x01 << 9)  
  	#define FORCE_SPEED_1000M   ((UINT32)0x02 << 9) 
  #define AN_EN					((UINT32)0x01 << 8)   
  #define MI_DIS		      	((UINT32)0x01 << 7)   
  #define FC_TX_ST	          	((UINT32)0x01 << 6)   
  #define RX_RX_ST	          	((UINT32)0x01 << 5)   
  #define DULPLEX_ST          	((UINT32)0x01 << 4)   
  #define SPEED_ST	          	((UINT32)0x03 << 2)   
  	#define SPEED_ST_10M        ((UINT32)0x00 << 2)  
  	#define SPEED_ST_100M       ((UINT32)0x01 << 2)  
  	#define SPEED_ST_1000M      ((UINT32)0x02 << 2) 
  #define TXC_ST		      	((UINT32)0x01 << 1)   
  #define LINK_ST          		((UINT32)0x01 << 0)   

#define RW_MAC_CFG   	0x08                  
  #define NIC_PD     			((UINT32)0x01 << 31)  
  #define WOL	            	((UINT32)0x01 << 30)  
  #define NIC_PD_READY      	((UINT32)0x01 << 29)  
  #define RXDV_WAKEUP_EN		((UINT32)0x01 << 28)   
  #define TXPART_WAKEUP_EN		((UINT32)0x01 << 27)  
  #define TX_CKS_EN        		((UINT32)0x01 << 26)   
  #define RX_CKS_EN        		((UINT32)0x01 << 25)   
  #define ACPT_CKS_ERR   		((UINT32)0x01 << 24)   
  #define IST_EN		   		((UINT32)0x01 << 23)   
  #define VLAN_STRIPPING		((UINT32)0x01 << 22)   
  #define ACPT_CRC_ERR   		((UINT32)0x01 << 21)   
  #define CRC_STRIPPING  		((UINT32)0x01 << 20)   
  #define TX_AUTO_PAD           ((UINT32)0x01 << 19)
  #define ACPT_LONG_PKT 		((UINT32)0x01 << 18)   
  #define MAX_LEN	   			((UINT32)0x03 << 16)   
  	#define MAX_LEN_1518		((UINT32)0x00 << 16)  
  	#define MAX_LEN_1522		((UINT32)0x01 << 16)  
  	#define MAX_LEN_1536		((UINT32)0x02 << 16)  
  #define IPG			   		((UINT32)0x1F << 10)   
  #define DO_NOT_SKIP   		((UINT32)0x01 << 9)   
  #define FAST_RETRY   			((UINT32)0x01 << 8)   
  #define TX_VLAN_TAG_AUTO_PARSE   	((UINT32)0x01 << 0)   

#define RW_FC_CFG   		0x0C                  
  #define SEND_PAUSE_TH     	((UINT32)0xFFF << 16)  
  #define SEND_PAUSE_TH_2K     	((UINT32)0x800 << 16)  
  #define COLCNT_CLR_MODE		((UINT32)0x01 << 9)  
  #define UC_PAUSE_DIS  		((UINT32)0x01 << 8)   
  #define BP_ENABLE   			((UINT32)0x01 << 7)   
  #define REV   				((UINT32)0x01 << 6)   
  #define MAX_BP_COL_EN   		((UINT32)0x01 << 5)   
  #define MAX_BP_COL_CNT   		((UINT32)0x1F << 0)   

#define RW_ARL_CFG   	0x10                  
  #define PRI_TAG_FILTER     	((UINT32)0x01 << 6)  
  #define VLAN_UTAG_FILTER		((UINT32)0x01 << 5)  
  #define MISC_MODE  			((UINT32)0x01 << 4)   
  #define MY_MAC_ONLY   		((UINT32)0x01 << 3)   
  #define CPU_LEARN_DIS			((UINT32)0x01 << 2)   
  #define REV_MC_FILTER   		((UINT32)0x01 << 1)   
  #define HASH_ALG		   		((UINT32)0x01 << 0)   

#define RW_MY_MAC_H   	0x14                  
#define RW_MY_MAC_L  	0x18                  

#define RW_HASH_CTRL   	0x1C                  
  #define HASH_TABLE_BIST_START      ((UINT32)0x01 << 31)  
  #define HASH_TABLE_BIST_DONE     	((UINT32)0x01 << 17)  
  #define HASH_TABLE_BIST_OK		((UINT32)0x01 << 16)  
  #define COMMAND_START	  			((UINT32)0x01 << 14)   
  #define HASH_ACCESS_COMMAND		((UINT32)0x01 << 13)   
  #define HASH_BIT_DATA				((UINT32)0x01 << 12)   
  #define HASH_BIT_ADDRESS   		((UINT32)0x1F << 0)   

#define RW_VLAN_CTRL   	0x20                  
  #define MY_VID0_3_EN			     	((UINT32)0x0F << 0)  
  #define MY_VID3_EN                    ((UINT32)0x01 << 3)
  #define MY_VID2_EN                    ((UINT32)0x01 << 2)
  #define MY_VID1_EN                    ((UINT32)0x01 << 1)
  #define MY_VID0_EN                    ((UINT32)0x01 << 0)

#define RW_VLAN_ID_0_1	0x24                 
  #define MY_VID1			     		((UINT32)0xFFF << 16)  
  #define MY_VID0			     		((UINT32)0xFFF << 0)  

#define RW_VLAN_ID_2_3	0x28                 
  #define MY_VID3			     		((UINT32)0xFFF << 16)  
  #define MY_VID2			     		((UINT32)0xFFF << 0)  

#define RW_DUMMY        0x2C
  #define FPGA_MODE             ((UINT32)0x01 << 31)
  #define WT_CLR_MIB            ((UINT32)0x01 << 3)
  #define NO_COL_PIN            ((UINT32)0x01 << 2)
  #define TX_RX_OD_RDY          ((UINT32)0x01 << 1)
  #define MDIO_CMD_DONE         ((UINT32)0x01 << 0)

#define RW_DMA_CFG   	0x30                  
  #define RX_OFFSET_2B_DIS  	((UINT32)0x01 << 16)
//  #define TX_INTR_WDLE  		((UINT32)0x01 << 9)   
//  #define RX_INTR_WDLE 		 	((UINT32)0x01 << 8)   
  #define TX_POLL_PERIOD		((UINT32)0x03 << 6)  
  #define TX_POLL_EN  			((UINT32)0x01 << 5)   
  #define TX_SUSPEND   			((UINT32)0x01 << 4)   
  #define RX_POLL_PERIOD		((UINT32)0x03 << 2)  
  #define RX_POLL_EN  			((UINT32)0x01 << 1)   
  #define RX_SUSPEND   			((UINT32)0x01 << 0)   

#define RW_TX_DMA_CTRL   	0x34                  
  #define TX_EN                 ((UINT32)0x01 << 3) // RO, TX DMA Enable
  #define TX_RESUME				((UINT32)0x01 << 2)
  #define TX_STOP  				((UINT32)0x01 << 1)  
  #define TX_START  			((UINT32)0x01 << 0)  

#define RW_RX_DMA_CTRL   	0x38                  
  #define RX_EN                 ((UINT32)0x01 << 3) // RO, RX DMA Enable
  #define RX_RESUME				((UINT32)0x01 << 2)  
  #define RX_STOP  				((UINT32)0x01 << 1)  
  #define RX_START              ((UINT32)0x01 << 0)  

#define RW_TX_DPTR  		0x3C                  
  #define TXSD  				((UINT32)0xFFFFFFF << 4)  

#define RW_RX_DPTR  		0x40                  
  #define RXSD  				((UINT32)0xFFFFFFF << 4)  

#define RW_TX_BASE_ADDR		0x44                 
  #define TX_BASE				((UINT32)0xFFFFFFF << 4)  

#define RW_RX_BASE_ADDR		0x48                 
  #define RX_BASE				((UINT32)0xFFFFFFF << 4)  

#define RW_DLY_INT_CFG		0x50 
  #define RX_PCODE              ((UINT32)0x01 << 10) // Rx 802.3az LPI Code interrupt
  #define TX_SKIP				((UINT32)0x01 << 9)  
  #define TNTC					((UINT32)0x01 << 8)  
  #define TNQE					((UINT32)0x01 << 7)  
  #define FNRC					((UINT32)0x01 << 6)  
  #define FNQF					((UINT32)0x01 << 5)  
  #define MAGIC_PKT_REC			((UINT32)0x01 << 4)  
  #define MIB_COUNTER_TH		((UINT32)0x01 << 3)  
  #define PORT_STATUS_CFG		((UINT32)0x01 << 2)  
  #define RX_FIFO_FULL			((UINT32)0x01 << 1)  
  #define TX_FIFO_UNDER_RUN		((UINT32)0x01 << 0)  // removed in mt5395 and mt5396 ?

#define RW_INT_MASK			0x54               
  #define INT_MASK					((UINT32)0x3FF << 0)  
  #define RX_PCODE_MASK             ((UINT32)0x01 << 10) // Rx 802.3az LPI Code interrupt
  #define TX_SKIP_INT_MASK		 	((UINT32)0x01 << 9)  
  #define TNTC_INT_MASK				((UINT32)0x01 << 8)  
  #define TNQE_INT_MASK				((UINT32)0x01 << 7)  
  #define FNRC_INT_MASK				((UINT32)0x01 << 6)  
  #define FNQF_INT_MASK				((UINT32)0x01 << 5)  
  #define MAGIC_PAK_REC_INT_MASK	((UINT32)0x01 << 4)  
  #define MIB_COUNTER_TH_MASK		((UINT32)0x01 << 3)  
  #define PORT_STATUS_CFG_MASK		((UINT32)0x01 << 2)  
  #define RX_FIFO_FULL_MASK			((UINT32)0x01 << 1)  
  #define TX_FIFO_UNDER_RUN_MASK	((UINT32)0x01 << 0)  

#define RW_TEST0			0x58               
  #define ALWAYS_TXC_OUT			((UINT32)0x01 << 16)  
  #define TX_SKEW					((UINT32)0x3F << 8)  
  #define RX_SKEW					((UINT32)0x3F << 0)  

#define RW_TEST1			0x5C               
  #define RESTART_HASH_MBIST		((UINT32)0x01 << 31)  
  #define DBG_SEL					((UINT32)0x03 << 29)  
  #define HANDSHAKE_MODE			((UINT32)0x01 << 28)  
//  #define NO_PATCH_READ_C_BIT_TX	((UINT32)0x01 << 27)  
//  #define NO_PATCH_READ_C_BIT_RX	((UINT32)0x01 << 26)  
  #define NO_PATCH_PRE_FETCH_RX     ((UINT32)0x01 << 26)
  #define INT_LB_MII				((UINT32)0x01 << 18)  
  #define EXT_LB_MII                ((UINT32)0x01 << 17)
  #define SIM_MODE                  ((UINT32)0x01 << 16)
  #define RX_FIFO_FREE_BYTE			((UINT32)0x1FFF << 0)  


#define RW_EXTEND_CFG		0x60               
  #define SEND_PAUSE_RLS			((UINT32)0xFFF << 16)  
  #define SEND_PAUSE_RLS_1K			((UINT32)0x400 << 16)  

#define RW_ETHPHY			0x6C               
  #define ADC_IN_MUX_EN				((UINT32)0x01 << 8)  
//  #define SWC_MII_MODE				((UINT32)0x01 << 7)  
//  #define EXT_MDC_MODE				((UINT32)0x01 << 6)  
  #define AFE_TEST_MODE				((UINT32)0x01 << 5)  
  #define AFE_TEST_OE				((UINT32)0x01 << 4)  
//  #define MII_PAD_OE				((UINT32)0x01 << 3)  
  #define FRC_SMI_ENB				((UINT32)0x01 << 2)  
  #define PIFCKLBYP					((UINT32)0x01 << 1)  
  #define PLLCLKBYP					((UINT32)0x01 << 0)  

#define RW_DMA_STATUS		0x70               
  #define TN_BIT					((UINT32)0x01 << 16)  
  #define TN_E_BIT					((UINT32)0x01 << 15)  
  #define TN_L_BIT					((UINT32)0x01 << 14)  
  #define TN_I_BIT					((UINT32)0x01 << 13)  
  #define TN_VTG_BIT				((UINT32)0x01 << 12)  
  #define TN_DMA_STS				((UINT32)0x0F << 8)  
  #define FN_C_BIT					((UINT32)0x01 << 6)  
  #define FN_E_BIT					((UINT32)0x01 << 5)  
  #define FN_DMA_STS				((UINT32)0x1F << 0)  

#define RW_BURST_TYPE		0x74               
  #define TX_BURST					((UINT32)0x07 << 4) 
  	  #define TX_BURST_4			((UINT32)0x03 << 4) 
  	  #define TX_BURST_8			((UINT32)0x05 << 4) 
  	  #define TX_BURST_16			((UINT32)0x07 << 4) 
  #define RX_BURST					((UINT32)0x01 << 0)  
  	  #define RX_BURST_4			((UINT32)0x03 << 0) 
  	  #define RX_BURST_8			((UINT32)0x05 << 0) 
  	  #define RX_BURST_16			((UINT32)0x07 << 0) 

#define EEE_CTRL            0x78 
  #define LPI_MODE_EB ((UINT32)0x01 << 0) 
  
#define EEE_SLEEP_TIMER 0x7c

#define EEE_WAKEUP_TIMER 0x80

#define ETHPHY_CONFIG1 0x84
  #define PLL_MODE                  ((UINT32)0x01 << 5)
  #define CHIP_SMI_ADDR             ((UINT32)0x1F << 0)
//#define RW_INTERNAL_PHY_CTRL 0x84
//  #define INTERNAL_PHY_ADDRESS_MASK    ((UINT32)0x3c)      
//    #define INTERNAL_PHY_ADDRESS_POS    (6)      
    
#define ETHPHY_CONFIG2          0x88    
  #define PART_NUM                  ((UINT32)0x1F << 16)
  #define REV_CODE                  ((UINT32)0x3FF << 0)

#define RW_ETHPHY_MONITOR       0x8C
  #define EPHY_DBG_FLAG             ((UINT32)0xFFFFFFFF << 0)

#define RW_ETHPHY_CLOCK_CONTROL 0x90

#define RW_ETHSYS_CONFIG        0x94
  #define INT_PHY_SEL               ((UINT32)0x01 << 3)
  #define SWC_MII_MODE              ((UINT32)0x01 << 2)
  #define EXT_MDC_MODE              ((UINT32)0x01 << 1)
  #define MII_PAD_OE                ((UINT32)0x01 << 0)
  
#define RW_MAC_MODE_CONFIG      0x98
  #define RMII_SRC_SEL              ((UINT32)0x01 << 1)
    #define RMII_SRC_SEL_INTERNAL   ((UINT32)0x00 << 1)
    #define RMII_SRC_SEL_EXTERNAL   ((UINT32)0x01 << 1)
  #define BIG_ENDIAN				((UINT32)0x01 << 0)
  
#define RW_SW_RESET_CONTROL     0x9c
  #define PHY_RSTN					((UINT32)0x01 << 4)   
  #define MRST						((UINT32)0x01 << 3)  
  #define NRST						((UINT32)0x01 << 2)  
  #define HRST						((UINT32)0x01 << 1) 
  #define DMA_RESET				    ((UINT32)0x01 << 0) 

#define RO_MAC_DEBUG1           0xA0

#define RO_MAC_DEBUG2           0xA4

#define RO_MAC_DEBUG3           0xA8

#define MAC_CLOCK_CONFIG        0xac
  #define GTXC_OUT_INV              ((UINT32)0x01 << 21)
  #define RMII_CLK_INV              ((UINT32)0x01 << 20)
  #define TXCLK_OUT_INV				((UINT32)0x01 << 19)  
  #define RXCLK_OUT_INV				((UINT32)0x01 << 18)  
  #define TXCLK_IN_INV				((UINT32)0x01 << 17)  
  #define RXCLK_IN_INV				((UINT32)0x01 << 16)
  #define SW_MAC_DIV_EN             ((UINT32)0x01 << 14)
  #define SW_MAC_DIV                ((UINT32)0x01 << 12)
  #define MDC_INV					((UINT32)0x01 << 9)
  #define MDC_NEG_LAT				((UINT32)0x01 << 8)  
  #define MDC_DIV					((UINT32)0xFF << 0)  
    #define MDC_CLK_DIV_10			((UINT32)0x0A << 0)


#else //Other chip
#define RW_PHY_CTRL0   	0x00                  
  #define RW_DATA				((UINT32)0xFFFF << 16)  
  #define RW_DATA_SHIFT			(16)  
  #define RW_OK            		((UINT32)0x01 << 15)  
  #define RD_CMD           		((UINT32)0x01 << 14)  
  #define WT_CMD         		((UINT32)0x01 << 13)   
  #define PHY_REG        		((UINT32)0x1F << 8)  
  #define PHY_ADDR       		((UINT32)0x1F << 0)   
  #define PHY_REG_SHIFT        	(8)  
  #define PHY_ADDR_SHIFT      	(0)   

#define RW_PHY_CTRL1   	0x04                  
  #define AUTO_POLL_DIS			((UINT32)0x01 << 31)  
  #define PHY_ADDR_AUTO      	((UINT32)0x1F << 24)  
  #define PHY_ADDR_AUTO_SHIFT 	(24)  
  #define SWH_CK_PWN           	((UINT32)0x01 << 23)  
  #define PHY_PORT_SEL         	((UINT32)0x01 << 22)   
  #define EXTER_MAC_SEL       	((UINT32)0x01 << 21)  
  #define INTER_PYH_PD          ((UINT32)0x01 << 19)   
  #define RGMII_PHY          	((UINT32)0x01 << 17)   
  #define REV_MII		       	((UINT32)0x01 << 16)   
  #define TXC_CHECK_EN          ((UINT32)0x01 << 14)   
  #define FORCE_FC_TX          	((UINT32)0x01 << 13)   
  #define FORCE_FC_RX          	((UINT32)0x01 << 12)   
  #define FORCE_DUPLEX          ((UINT32)0x01 << 11)   
  #define FORCE_DUPLEX_HALF     ((UINT32)0x00 << 11)  
  #define FORCE_DUPLEX_FULL     ((UINT32)0x01 << 11)  
  #define FORCE_SPEED          	((UINT32)0x03 << 9)   
    #define FORCE_SPEED_10M    	((UINT32)0x00 << 9)  
  	#define FORCE_SPEED_100M    ((UINT32)0x01 << 9)  
  	#define FORCE_SPEED_1000M   ((UINT32)0x02 << 9) 
  #define AN_EN					((UINT32)0x01 << 8)   
  #define MI_DIS		      	((UINT32)0x01 << 7)   
  #define FC_TX_ST	          	((UINT32)0x01 << 6)   
  #define RX_RX_ST	          	((UINT32)0x01 << 5)   
  #define DULPLEX_ST          	((UINT32)0x01 << 4)   
  #define SPEED_ST	          	((UINT32)0x03 << 2)   
  	#define SPEED_ST_10M        ((UINT32)0x00 << 2)  
  	#define SPEED_ST_100M       ((UINT32)0x01 << 2)  
  	#define SPEED_ST_1000M      ((UINT32)0x02 << 2) 
  #define TXC_ST		      	((UINT32)0x01 << 1)   
  #define LINK_ST          		((UINT32)0x01 << 0)   

#define RW_MAC_CFG   	0x08                  
  #define NIC_PD     			((UINT32)0x01 << 31)  
  #define WOL	            	((UINT32)0x01 << 30)  
  #define NIC_PD_READY      	((UINT32)0x01 << 29)  
  #define RXDV_WAKEUP_EN		((UINT32)0x01 << 28)   
  #define TXPART_WAKEUP_EN		((UINT32)0x01 << 27)  
  #define TX_CKS_EN        		((UINT32)0x01 << 26)   
  #define RX_CKS_EN        		((UINT32)0x01 << 25)   
  #define ACPT_CKS_ERR   		((UINT32)0x01 << 24)   
  #define IST_EN		   		((UINT32)0x01 << 23)   
  #define VLAN_STRIPPING		((UINT32)0x01 << 22)   
  #define ACPT_CRC_ERR   		((UINT32)0x01 << 21)   
  #define CRC_STRIPPING  		((UINT32)0x01 << 20)   
  #define ACPT_LONG_PKT 		((UINT32)0x01 << 18)   
  #define MAX_LEN	   			((UINT32)0x03 << 16)   
  	#define MAX_LEN_1518		((UINT32)0x00 << 16)  
  	#define MAX_LEN_1522		((UINT32)0x01 << 16)  
  	#define MAX_LEN_1536		((UINT32)0x02 << 16)  
  #define IPG			   		((UINT32)0x1F << 10)   
  #define DO_NOT_SKIP   		((UINT32)0x01 << 9)   
  #define FAST_RETRY   			((UINT32)0x01 << 8)   
  #define TX_VLAN_TAG_AUTO_PARSE   	((UINT32)0x01 << 0)   

#define RW_FC_CFG   		0x0C                  
  #define SEND_PAUSE_TH     	((UINT32)0xFFF << 16)  
  #define SEND_PAUSE_TH_2K     	((UINT32)0x800 << 16)  
  #define COLCNT_CLR_MODE		((UINT32)0x01 << 9)  
  #define UC_PAUSE_DIS  		((UINT32)0x01 << 8)   
  #define BP_ENABLE   			((UINT32)0x01 << 7)   
  #define REV   				((UINT32)0x01 << 6)   
  #define MAX_BP_COL_EN   		((UINT32)0x01 << 5)   
  #define MAX_BP_COL_CNT   		((UINT32)0x1F << 0)   

#define RW_ARL_CFG   	0x10                  
  #define PRI_TAG_FILTER     	((UINT32)0x01 << 6)  
  #define VLAN_UTAG_FILTER		((UINT32)0x01 << 5)  
  #define MISC_MODE  			((UINT32)0x01 << 4)   
  #define MY_MAC_ONLY   		((UINT32)0x01 << 3)   
  #define CPU_LEARN_DIS			((UINT32)0x01 << 2)   
  #define REV_MC_FILTER   		((UINT32)0x01 << 1)   
  #define HASH_ALG		   		((UINT32)0x01 << 0)   

#define RW_MY_MAC_H   	0x14                  
#define RW_MY_MAC_L  	0x18                  

#define RW_HASH_CTRL   	0x1C             
  #define HASH_TABLE_BIST_START      ((UINT32)0x01 << 31)  
  #define HASH_TABLE_BIST_DONE     	((UINT32)0x01 << 17)  
  #define HASH_TABLE_BIST_OK		((UINT32)0x01 << 16)  
  #define COMMAND_START	  			((UINT32)0x01 << 14)   
  #define HASH_ACCESS_COMMAND		((UINT32)0x01 << 13)   
  #define HASH_BIT_DATA				((UINT32)0x01 << 12)   
  #define HASH_BIT_ADDRESS   		((UINT32)0x1F << 0)   

#define RW_VLAN_CTRL   	0x20                  
  #define MY_VID0_3_EN			     	((UINT32)0x0F << 0)  

#define RW_VLAN_ID_0_1	0x24                 
  #define MY_VID1			     		((UINT32)0xFFF << 16)  
  #define MY_VID0			     		((UINT32)0xFFF << 0)  

#define RW_VLAN_ID_2_3	0x28                 
  #define MY_VID3			     		((UINT32)0xFFF << 16)  
  #define MY_VID2			     		((UINT32)0xFFF << 0)  

#define RW_DMA_CFG   	0x30                  
  #define RX_OFFSET_2B_DIS  	((UINT32)0x01 << 16)
  #define TX_INTR_WDLE  		((UINT32)0x01 << 9)   
  #define RX_INTR_WDLE 		 	((UINT32)0x01 << 8)   
  #define TX_POLL_PERIOD		((UINT32)0x03 << 6)  
  #define TX_POLL_EN  			((UINT32)0x01 << 5)   
  #define TX_SUSPEND   			((UINT32)0x01 << 4)   
  #define RX_POLL_PERIOD		((UINT32)0x03 << 2)  
  #define RX_POLL_EN  			((UINT32)0x01 << 1)   
  #define RX_SUSPEND   			((UINT32)0x01 << 0)   

#define RW_TX_DMA_CTRL   	0x34                  
  #define TX_EN  				((UINT32)0x01 << 0)  

#define RW_RX_DMA_CTRL   	0x38                  
  #define RX_EN  				((UINT32)0x01 << 0)  

#define RW_TX_DPTR  		0x3C                  
  #define TXSD  				((UINT32)0xFFFFFFF << 4)  

#define RW_RX_DPTR  		0x40                  
  #define RXSD  				((UINT32)0xFFFFFFF << 4)  

#define RW_TX_BASE_ADDR		0x44                 
  #define TX_BASE				((UINT32)0xFFFFFFF << 4)  

#define RW_RX_BASE_ADDR		0x48                 
  #define RX_BASE				((UINT32)0xFFFFFFF << 4)  

#define RW_DLY_INT_CFG		0x50 
  #define TX_SKIP				((UINT32)0x01 << 9)  
  #define TNTC					((UINT32)0x01 << 8)  
  #define TNQE					((UINT32)0x01 << 7)  
  #define FNRC					((UINT32)0x01 << 6)  
  #define FNQF					((UINT32)0x01 << 5)  
  #define MAGIC_PKT_REC			((UINT32)0x01 << 4)  
  #define MIB_COUNTER_TH		((UINT32)0x01 << 3)  
  #define PORT_STATUS_CFG		((UINT32)0x01 << 2)  
  #define RX_FIFO_FULL			((UINT32)0x01 << 1)  
  #define TX_FIFO_UNDER_RUN		((UINT32)0x01 << 0)  

#define RW_INT_MASK			0x54               
  #define INT_MASK					((UINT32)0x1FF << 0)  
  #define TX_SKIP_INT_MASK		 	((UINT32)0x01 << 9)  
  #define TNTC_INT_MASK				((UINT32)0x01 << 8)  
  #define TNQE_INT_MASK				((UINT32)0x01 << 7)  
  #define FNRC_INT_MASK				((UINT32)0x01 << 6)  
  #define FNQF_INT_MASK				((UINT32)0x01 << 5)  
  #define MAGIC_PAK_REC_INT_MASK	((UINT32)0x01 << 4)  
  #define MIB_COUNTER_TH_MASK		((UINT32)0x01 << 3)  
  #define PORT_STATUS_CFG_MASK		((UINT32)0x01 << 2)  
  #define RX_FIFO_FULL_MASK			((UINT32)0x01 << 1)  
  #define TX_FIFO_UNDER_RUN_MASK	((UINT32)0x01 << 0)  

#define RW_TEST0			0x58               
  #define ALWAYS_TXC_OUT			((UINT32)0x01 << 16)  
  #define TX_SKEW					((UINT32)0x3F << 8)  
  #define RX_SKEW					((UINT32)0x3F << 0)  

#define RW_TEST1			0x5C               
  #define RESTART_HASH_MBIST		((UINT32)0x01 << 31)  
  #define DBG_SEL					((UINT32)0x03 << 29)  
  #define HANDSHAKE_MODE			((UINT32)0x01 << 28)  
  #define NO_PATCH_READ_C_BIT_TX	((UINT32)0x01 << 27)  
  #define NO_PATCH_READ_C_BIT_RX	((UINT32)0x01 << 26)  
  #define INT_LB_MII				((UINT32)0x01 << 18)  
  #define RX_FIFO_FREE_BYTE			((UINT32)0x1FFF << 0)  


#define RW_EXTEND_CFG		0x60               
  #define SEND_PAUSE_RLS			((UINT32)0xFFF << 16)  
  #define SEND_PAUSE_RLS_1K			((UINT32)0x400 << 16)  

#define RW_ETHPHY			0x6C      
  #define CFG_INT_ETHPHY			((UINT32)0x01 << 9)  
  #define ADC_IN_MUX_EN				((UINT32)0x01 << 8)  
  #define SWC_MII_MODE				((UINT32)0x01 << 7)  
  #define EXT_MDC_MODE				((UINT32)0x01 << 6)  
  #define AFE_TEST_MODE				((UINT32)0x01 << 5)  
  #define AFE_TEST_OE				((UINT32)0x01 << 4)  
  #define MII_PAD_OE				((UINT32)0x01 << 3)  
  #define FRC_SMI_ENB				((UINT32)0x01 << 2)  
  #define PIFCKLBYP					((UINT32)0x01 << 1)  
  #define PLLCLKBYP					((UINT32)0x01 << 0)  

#define RW_DMA_STATUS		0x70               
  #define TN_BIT					((UINT32)0x01 << 16)  
  #define TN_E_BIT					((UINT32)0x01 << 15)  
  #define TN_L_BIT					((UINT32)0x01 << 14)  
  #define TN_I_BIT					((UINT32)0x01 << 13)  
  #define TN_VTG_BIT				((UINT32)0x01 << 12)  
  #define TN_DMA_STS				((UINT32)0x0F << 8)  
  #define FN_C_BIT					((UINT32)0x01 << 6)  
  #define FN_E_BIT					((UINT32)0x01 << 5)  
  #define FN_DMA_STS				((UINT32)0x1F << 0)  

#define RW_BURST_TYPE		0x74               
  #define TX_BURST					((UINT32)0x07 << 4) 
  	  #define TX_BURST_4			((UINT32)0x03 << 4) 
  	  #define TX_BURST_8			((UINT32)0x05 << 4) 
  	  #define TX_BURST_16			((UINT32)0x07 << 4) 
  #define RX_BURST					((UINT32)0x01 << 0)  
  	  #define RX_BURST_4			((UINT32)0x03 << 0) 
  	  #define RX_BURST_8			((UINT32)0x05 << 0) 
  	  #define RX_BURST_16			((UINT32)0x07 << 0) 

#define RW_ARB_CTRL		0x80              

#define RW_ARB_MON0		0x84             

#define RW_ARB_MON1		0x88

#define RW_ARB_MON2		0x8C

#define RW_EXT_CFG			0x90               
  #define BIG_ENDIAN				((UINT32)0x01 << 31)  
  #define DRAM_PRIORITY				((UINT32)0x01 << 30)  
   	#define DRAM_PRIORITY_HIGH		((UINT32)0x01 << 30)  
  #define PDMAC_SEL					((UINT32)0x01 << 27)
    #define PDMAC_SEL_PDWNC			((UINT32)0x01 << 27)
  #define TEST_CLK					((UINT32)0x01 << 26) 
    #define TEST_CLK_27M			((UINT32)0x01 << 26)
  #define RMII_SRC_SEL_EXT_PAD		((UINT32)0x01 << 26)    
  #define COL_SEL					((UINT32)0x01 << 25) 
  	#define COL_SEL_EXTERNAL		((UINT32)0x01 << 25) // For MII use
	#define COL_SEL_INTERNAL		((UINT32)0x00 << 25)  //For RMII use 
  #define RMII_CLK_SEL				((UINT32)0x01 << 24) 
  #define TCPIP_RST					((UINT32)0x01 << 23) 
  #define MRST						((UINT32)0x01 << 22)  
  #define NRST						((UINT32)0x01 << 21)  
  #define HRST						((UINT32)0x01 << 20) 
  #define RMII_CLK_INV				((UINT32)0x01 << 19) 
  #define DMA_RESET				    ((UINT32)0x01 << 18) 
  #define PB_NIC_SEL				((UINT32)0x3F << 11)  
  	 #define PB_NIC_SEL_20H			((UINT32)0x20 << 11) 
  #define CFG_NIC_REVMII			((UINT32)0x1F << 10)  
  #define EFUSE_RGMII				((UINT32)0x01 << 9)  
  #define EFUSE_FEPHY				((UINT32)0x01 << 8) 
  #define MII_INTER_COL             ((UINT32)0x01 << 7)  //MII internal COL
  #define RMII_MODE					((UINT32)0x01 << 6) 
  #define CLK27_SEL					((UINT32)0x01 << 5)  
  #define US_20						((UINT32)0x07 << 2)  
  #define RX_PREREAD_EN				((UINT32)0x01 << 1)  
  #define TX_PREREAD_EN				((UINT32)0x01 << 0)  

#define RW_DELSEL			0x98               
  #define TX_DELSEL					((UINT32)0x03 << 2)  
  #define RX_DELSEL					((UINT32)0x03 << 0)  

#define RW_CLK_CFG			0x9C               
  #define TXCLK_OUT_INV				((UINT32)0x01 << 31)  
  #define RXCLK_OUT_INV				((UINT32)0x01 << 30)  
  #define TXCLK_IN_INV				((UINT32)0x01 << 29)  
  #define RXCLK_IN_INV				((UINT32)0x01 << 28)
  #define MAC_SEL					((UINT32)0x03 << 26)
  	#define MAC_CLK_25M				((UINT32)0x00 << 26)
  	#define MAC_CLK_RXCLK			((UINT32)0x01 << 26)
  	#define MAC_CLK_27M				((UINT32)0x02 << 26)
  	#define MAC_CLK_TXCLK			((UINT32)0x03 << 26)
  #define MDC_SEL					((UINT32)0x03 << 24)
  	#define MDC_CLK_25M				((UINT32)0x00 << 24)
  	#define MDC_CLK_RXCLK			((UINT32)0x01 << 24)
  	#define MDC_CLK_27M				((UINT32)0x02 << 24)
  	#define MDC_CLK_TXCLK			((UINT32)0x03 << 24)
  #define CLK25_SEL					((UINT32)0x03 << 22)
  	#define CLK25_CLK_25M			((UINT32)0x00 << 22)
  	#define CLK25_CLK_RXCLK			((UINT32)0x01 << 22)
  	#define CLK25_CLK_27M			((UINT32)0x02 << 22)
  	#define CLK25_CLK_TXCLK			((UINT32)0x03 << 22)
  #define MAC_DIV					((UINT32)0x01 << 16)
    #define MAC_CLK_DIV_10			((UINT32)0x00 << 16)
  	#define MAC_CLK_DIV_1			((UINT32)0x01 << 16)
  #define MDC_INV					((UINT32)0x01 << 12)  
  #define MDC_STOP					((UINT32)0x01 << 8)  
  #define MDC_DIV					((UINT32)0xFF << 0)  
    #define MDC_CLK_DIV_10			((UINT32)0x0A << 0)

#define  RW_INTERNAL_PHY_CTRL 0x88
  #define INTERNAL_PHY_ADDRESS_MASK    ((UINT32)0xF8)      
    #define INTERNAL_PHY_ADDRESS_POS    (3)      
  #define INTERNAL_PHY_ENABLE_MDC_MDIO          ((UINT32)0x01 << 0)
  #define INTERNAL_PHY_ENABLE_MII_PIN          ((UINT32)0x01 << 1)
      
#define  RW_INTERNAL_PHY_CTRL1 0x80
  #define SOFT_RESET_DISABLE                   ((UINT32)0x01 << 1) 
    
    
    
#endif


#define RW_C_RXOKPKT		0x100

#define RW_C_RXOKBYTE		0x104

#define RW_C_RXRUNT			0x108

#define RW_C_RXLONG			0x10C

#define RW_C_RXDROP			0x110

#define RW_C_RXCRC			0x114

#define RW_C_RXARLDROP		0x118

#define RW_C_RXVLANDROP		0x11C

#define RW_C_RXCSERR		0x120

#define RW_C_RXPAUSE		0x124

#define RW_C_TXOKPKT		0x128

#define RW_C_TXOKBYTE		0x12C

#define RW_C_TXPAUSE		0x130
  
#define RW_C_TXRETRY    	0x134      

#define RW_C_TXSKIP	    	0x138       
  
#define RW_MY_MAC1_H		0x140
  #define ADDR_EN           ((UINT32)0x01 << 31)   /* address filter module is enabled */
 
#define RW_MY_MAC1_L		0x144

#define RW_CHKSUM_ADDR   			0x00     //	UDP/TCP/IP Checksum Start Address										
  #define CHECKSUM_SADR_MASK 	 	((UINT32)0xFFFFFFFF)	  //Checksum Start Address (Byte Address)

#define RW_CHKSUM_LENGTH  			0x04
  #define CHECKSUM_LENGTH_MASK 	 	((UINT32)0xFFFFFFFF)	  // Checksum Length (Byte)										

#define RW_CHKSUM_CTRL     			0x08
  #define  CHKSUM_INT 				((UINT32)0x01 << 31) 
  #define  OUT_INV					((UINT32)0x01 << 7)  // 1's complement of UDP/TCP/IP Checksum Result Data
  #define  OUT_REV					((UINT32)0x01 << 6)  //Reverse Byte of Checksum Result Data
  #define  BYTE_REV         		((UINT32)0x01 << 5)  //Reverse Byte of Checksum Input Data (Word)
  #define  DW_REV	      			((UINT32)0x01 << 4)  //Reverse Byte of Checksum Input Data (Double Word)	
  #define  CHKSUM_INT_CLR			((UINT32)0x01 << 2)  //Checksum Interrupt clear
  #define  INTR_EN					((UINT32)0x01 << 1) //Checksum Interrupt Enable
  #define  CHECKSUM_EN				((UINT32)0x01 << 0)  //Checksum Enable. When Checksum is finish, this bit will be clear to 0.	

#define RW_CHKSUM_RESULT     		0x0C
  #define  CHECKSUM_RESULT_MASK		((UINT32)0x0FFFF)  //Checksum Result Data Mask

#define RW_CHKSUM_DMA_CTRL     		0x10
  #define  CHECKSUM_DMA_IDLE_TIME_MASK	((UINT32)0xFFFF0000)  //Checksum DMA Idle Time (dram clock cycle)
  #define  CHECKSUM_DMA_ALE_NUM_MASK	((UINT32)0x0000FFFF)  //Checksum Burst Length (64 bytes). If these registers set to 0, burst length is CHECKSUM_LENGTH.

#if 1 //RX Check SUm
#define RW_RX_CHKSUM_ADDR   			0x14     //	RX UDP/TCP/IP Checksum Start Address										
  #define RX_CHECKSUM_SADR_MASK 	 	((UINT32)0xFFFFFFFF)	  //Checksum Start Address (Byte Address)

#define RW_RX_CHKSUM_LENGTH  			0x18
  #define RX_CHECKSUM_LENGTH_MASK 	 	((UINT32)0xFFFFFFFF)	  // Checksum Length (Byte)										

#define RW_RX_CHKSUM_CTRL     			0x1c
  #define  RX_CHKSUM_INT 				((UINT32)0x01 << 31) 
  #define  RX_OUT_INV					((UINT32)0x01 << 7)  // 1's complement of UDP/TCP/IP Checksum Result Data
  #define  RX_OUT_REV					((UINT32)0x01 << 6)  //Reverse Byte of Checksum Result Data
  #define  RX_BYTE_REV         		    ((UINT32)0x01 << 5)  //Reverse Byte of Checksum Input Data (Word)
  #define  RX_DW_REV	      			((UINT32)0x01 << 4)  //Reverse Byte of Checksum Input Data (Double Word)	
  #define  RX_CHKSUM_INT_CLR			((UINT32)0x01 << 2)  //Checksum Interrupt clear
  #define  RX_INTR_EN					((UINT32)0x01 << 1) //Checksum Interrupt Enable
  #define  RX_CHECKSUM_EN				((UINT32)0x01 << 0)  //Checksum Enable. When Checksum is finish, this bit will be clear to 0.	

#define RW_RX_CHKSUM_RESULT     		0x20
  #define  RX_CHECKSUM_RESULT_MASK		((UINT32)0x0FFFF)  //Checksum Result Data Mask


#endif

// *********************************************************************
// Ethernet Init setting
// *********************************************************************
enum MacInitReg
{
  MAC_CFG_INIT        	= (UINT32)(RX_CKS_EN | ACPT_CKS_ERR | CRC_STRIPPING | MAX_LEN_1522 | IPG),   
#if (MT5368_EMU)
  MAC_CLK_INIT			= (UINT32)( MDC_CLK_DIV_10),
  MAC_EXT_INIT			= (UINT32)( MRST | NRST | HRST | DMA_RESET),
  
  MAC_EXT_INIT_RMII		= (UINT32)(MRST | NRST | HRST ),

  PDWNC_MAC_EXT_INIT	= (UINT32)(MRST | NRST | HRST | DMA_RESET),

  PDWNC_MAC_EXT_INIT_RMII	= (UINT32)(MRST | NRST | HRST),

  PDWNC_MAC_EXT_CFG		= (UINT32)(MRST | NRST | HRST ),

  PDWNC_MAC_EXT_CFG_RMII	= (UINT32)(MRST | NRST | HRST),
  
#else

  MAC_CLK_INIT			= (UINT32)(MAC_CLK_27M | MDC_CLK_27M | CLK25_CLK_27M | MAC_CLK_DIV_1 | MDC_STOP | MDC_CLK_DIV_10),
	
  MAC_EXT_INIT			= (UINT32)(DRAM_PRIORITY_HIGH | COL_SEL_EXTERNAL | TCPIP_RST | MRST | NRST | HRST | DMA_RESET| PB_NIC_SEL_20H),

  MAC_EXT_INIT_RMII		= (UINT32)(DRAM_PRIORITY_HIGH | COL_SEL_INTERNAL | TCPIP_RST | MRST | NRST | HRST | RMII_CLK_INV | PB_NIC_SEL_20H | RMII_MODE),

  PDWNC_MAC_EXT_INIT	= (UINT32)(DRAM_PRIORITY_HIGH | COL_SEL_EXTERNAL | TCPIP_RST | MRST | NRST | HRST | DMA_RESET| PB_NIC_SEL_20H),

  PDWNC_MAC_EXT_INIT_RMII	= (UINT32)(DRAM_PRIORITY_HIGH | COL_SEL_INTERNAL | TCPIP_RST | MRST | NRST | HRST | RMII_CLK_INV |PB_NIC_SEL_20H | RMII_MODE),

  PDWNC_MAC_EXT_CFG		= (UINT32)(DRAM_PRIORITY_HIGH | PDMAC_SEL_PDWNC | COL_SEL_EXTERNAL | TCPIP_RST | MRST | NRST | HRST | PB_NIC_SEL_20H),

  PDWNC_MAC_EXT_CFG_RMII	= (UINT32)(DRAM_PRIORITY_HIGH | PDMAC_SEL_PDWNC | COL_SEL_INTERNAL | TCPIP_RST | MRST | NRST | HRST | RMII_CLK_INV | PB_NIC_SEL_20H | RMII_MODE),
#endif  
	
  

  MAC_FILTER_INIT       = (UINT32)(0),                               

  MAC_FLOWCTRL_INIT  	= (UINT32)(SEND_PAUSE_TH_2K | UC_PAUSE_DIS | BP_ENABLE ),                                      	

  PHY_CTRL_INIT			= (UINT32)(FORCE_FC_TX | FORCE_FC_RX | FORCE_DUPLEX | FORCE_SPEED_100M | AN_EN),

  DMA_BUSMODE_INIT      = (UINT32)(TX_BURST_16 | RX_BURST_16),               	// Bus Mode

  DMA_OPMODE_INIT      	= (UINT32)(RX_OFFSET_2B_DIS | TX_SUSPEND | RX_SUSPEND),       // Operation Mode  

  DMA_RX_INT_MASK       = (UINT32)(FNRC | RX_FIFO_FULL | FNQF),
                                     
  DMA_TX_INT_MASK       = (UINT32)(TNTC | TX_FIFO_UNDER_RUN),

  DMA_INT_ENABLE        = (UINT32)(TNTC | FNRC | FNQF | RX_FIFO_FULL | TX_FIFO_UNDER_RUN),         

  DMA_INT_MASK          = (UINT32)(TNTC | FNRC | FNQF | RX_FIFO_FULL | TX_FIFO_UNDER_RUN),

  DMA_INT_ENABLE_ALL   	= (UINT32)(TX_SKIP | TNTC | TNQE | FNRC | FNQF |MAGIC_PKT_REC |MIB_COUNTER_TH |PORT_STATUS_CFG |RX_FIFO_FULL |TX_FIFO_UNDER_RUN),

  DMA_INT_CLEAR_ALL     = (UINT32)(TX_SKIP | TNTC | TNQE | FNRC | FNQF |MAGIC_PKT_REC |MIB_COUNTER_TH |PORT_STATUS_CFG |RX_FIFO_FULL |TX_FIFO_UNDER_RUN),
    
};

#define  MAC_EXT_CFG	(BIG_ENDIAN | DRAM_PRIORITY | MRST | NRST | PRST | HRST | PB_NIC_SEL_20H)

#define  MAC_CLK_CFG	(CLK_27M | MAC_CLK_DIV_1 | MDC_CLK_DIV_10) 	

// *********************************************************************
// Ethernet Descriptor Registers map
// *********************************************************************
  #define RX_OWN          ((UINT32)0x01 << 31)  
  
// *********************************************************************
// Ethernet Macros
// *********************************************************************
#define vWriteMAC(dAddr, dVal)  IO_WRITE32((ETHERNET_REG_OFFSET), (dAddr), (dVal))
#define dReadMAC(dAddr)         IO_READ32((ETHERNET_REG_OFFSET), (dAddr))
#define SetBitMAC(Reg, Bit)     vWriteMAC(Reg, (dReadMAC(Reg) & (~(Bit))) | (Bit))
#define ClrBitMAC(Reg, Bit)     vWriteMAC(Reg, dReadMAC(Reg) & (~(Bit)))

//#define vWriteCkGen(dAddr, dVal)  IO_WRITE32((CKGEN_REG_OFFSET), (dAddr), (dVal))
//#define vWriteMACPDWNC(dAddr, dVal)  IO_WRITE32((ETHERNET_PDWNC_REG_OFFSET), (dAddr), (dVal))
//#define dReadMACPDWNC(dAddr)         IO_READ32((ETHERNET_PDWNC_REG_OFFSET), (dAddr))
//#define SetBitMACPDWNC(Reg, Bit)     vWriteMACPDWNC(Reg, (dReadMACPDWNC(Reg) & (~Bit)) | (Bit))
//#define ClrBitMACPDWNC(Reg, Bit)     vWriteMACPDWNC(Reg, dReadMACPDWNC(Reg) & (~(Bit)))


#endif // _ETHER_REG_H_
