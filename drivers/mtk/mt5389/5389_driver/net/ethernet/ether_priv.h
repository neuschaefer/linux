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
 *   $Workfile: ether_priv.h $
 *
 * Project:
 * --------
 *
 *
 * Description:
 * ------------
 *   Ethernet private structure header file
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

#ifndef _ETHER_PRIV_H_
#define _ETHER_PRIV_H_

/****************************************************************************
** Config definitions
****************************************************************************/
#define CONFIG_ETHER_TX_INT_MODE    0   /* wait event from irq service for transmission */
#define CONFIG_ETHER_CRC_NORMAL     1   /* apply normal poly parameter for CRC calculation */
#define CONFIG_ETHER_DRV_RW_MAC     0   /* r/w MAC address in flash at driver layer */
#define CONFIG_ETHER_USE_OS_NATIVE  0   /* apply OS native APIs */
#define CONFIG_ETHER_TX_NO_COPY     1   /* no packet copy for tx */
#define CONFIG_ETHER_HW_CHKSUM      0   /* apply hw to calculate checksum,need to set HW_CHKSUM_ON_THE_FLY 1 in chksum.h*/
#define CONFIG_ETHER_USE_SAM_INSTEAD_EV  0   /* use sam insteand of event*/
#define ENABLE_REALTIME_THROUGHPUT_COUNT 0
#define SLT_TEST_ETHERNET_EN  0     
//*********************************************************************
// MAC RX Descriptors
//*********************************************************************
#define OWNED_BY_DMA	0
#define OWNED_BY_CPU	1

typedef union MacRxDescControl
{
        UINT32 u4Val;
        struct
        {
	    UINT32   u2BufSize      		: 16;   	// Buffer size
	    UINT32   fgL4ChecksumErr   		: 1;    	// TCP/UDP checksum error 
	    UINT32   fgIPChecksumErr   		: 1;    	// IP checksum error 
	    UINT32   u1ProtocolSel   		: 2;    	// Protocol select IP/TCP/UPD checksum select
	    UINT32   fgVlanPacket        	: 1;    	// indicate VLAN tag packet  
	    UINT32   fgMyMacAddr  			: 1;   		// Receive packet match My MAC addr.
	    UINT32   fgHitHash       		: 1;    	// Receive packet hit hash table
	    UINT32   fgRsvMultiAddr   		: 1;    	// Receive packet is reserved multicast addr.
	    UINT32   fgCrcErr          		: 1;   		// Receive packet is CRC error
	    UINT32   fgOversizeErr       	: 1;    	// Receive packet is oversize error.  
	    UINT32   u1Reserved       		: 2;    	// Reserved
	    UINT32   fgLastSeg          	: 1;   		// Rx - Last segment of the frame 
	    UINT32   fgFirstSeg           	: 1;    	// Rx - First segment of the frame  
	    UINT32   fgEndOfRing            : 1;    	// End of descriptors ring   
	    UINT32   fgOwn           		: 1;    	// Descriptor is owned by CPU engine   
        }Bits;
} MAC_RX_DESC_CTRL_T, *PMAC_RX_DESC_CTRL_T;

typedef struct MacRxDescVLAN
{
    UINT32   u2VlanID     		: 12;   	// VLAN tag ID
    UINT32   fgCFI    			: 1;   		// VLAN tag CFI  
    UINT32   u1Priority        	: 3;    	// VLAN tag priority
    UINT32   u2EPID       		: 16;    	// VLAN tag EPID
} MAC_RX_DESC_VLAN_T, *PMAC_RX_DESC_VLAN_T;

typedef struct MacRxDescriptor
{
#ifdef CC_MT5389
    MAC_RX_DESC_CTRL_T  	rRxCtrl;   
    UINT32   u4BufAddr;     
#else
    UINT32   u4BufAddr; 
    MAC_RX_DESC_CTRL_T  	rRxCtrl;   
#endif    
    MAC_RX_DESC_VLAN_T   rRxVlan;                 
    UINT32   u4Reserved;                     
} MAC_RX_DESC_T, *PMAC_RX_DESC_T;

//*********************************************************************
// MAC TX Descriptors
//*********************************************************************
typedef union MacTxDescControl
{
        UINT32 u4Val;
        struct
        {
	    UINT32   u2BufSize      		: 16;   	// Buffer size
	    UINT32   u1Reserved    			: 6;   		// Reserved   
	    UINT32   fgVTG        			: 1;    	// Tx packet is VLAN tagged
	    UINT32   fgTCO       			: 1;    	// TCP checksum generation offload
	    UINT32   fgUCO         			: 1;    	// UDP checksum generation offload   
	    UINT32   fgICO         			: 1;   		// IP checksum generation offload
	    UINT32   fgINSV         		: 1;    	// insert VLAN tag
	    UINT32   fgTxIntOn       		: 1;    	// interrupt enable after sending out packet
	    UINT32   fgLastSeg          	: 1;   		// Tx - Last segment of the frame 
	    UINT32   fgFirstSeg           	: 1;    	// Tx - First segment of the frame  
	    UINT32   fgEndOfRing            : 1;    	// End of descriptors ring   
	    UINT32   fgOwn           		: 1;    	// Descriptor is owned by CPU engine   
        }Bits;
} MAC_TX_DESC_CTRL_T, *PMAC_TX_DESC_CTRL_T;

typedef struct MacTxDescVLAN
{
    UINT32   u2VlanID     		: 12;   	// VLAN tag ID
    UINT32   fgCFI    			: 1;   		// VLAN tag CFI  
    UINT32   u1Priority        	: 3;    	// VLAN tag priority
    UINT32   u2EPID       		: 16;    	// VLAN tag EPID
} MAC_TX_DESC_VLAN_T, *PMAC_TX_DESC_VLAN_T;

typedef struct MacTxDescriptor
{
#ifdef CC_MT5389
    MAC_TX_DESC_CTRL_T 	rTxCtrl;    
    UINT32   u4BufAddr;  
#else	
    UINT32   u4BufAddr;  
    MAC_TX_DESC_CTRL_T 	rTxCtrl;    
#endif    
    MAC_TX_DESC_VLAN_T   rTxVlan;               
    UINT32   u4Reserved;           
} MAC_TX_DESC_T, *PMAC_TX_DESC_T;


/* Ethernet rx packet list structure */
typedef struct _ETHER_RX_LIST_T_
{
    UINT8                       *pucPktBuf;
    struct _ETHER_RX_LIST_T_    *prNext;
} ETHER_RX_LIST_T;

/* Ethernet tx packet list structure */
typedef struct _ETHER_TX_LIST_T_
{
    PKT_BUFF_T                  *prPktBuf;
    struct _ETHER_TX_LIST_T_    *prNext;
} ETHER_TX_LIST_T;


//*********************************************************************
// MAC TX/RX statistics 
//*********************************************************************
typedef struct MacTxRxStat
{
    UINT64 u8RxFrame;                  // number of received frame
    UINT64 u8RxOctet;                  // number of received octet
    UINT64 u8RxPreOctet;               // number of previous received octet
    UINT32 u4RxThroughput;			   // Rx current thourghput(octec/sec)(u8RxOctet - u8RxPreOctet)
    UINT32 u4RxFrameSrv;               // number of received frame can be serviced each time
    UINT32 u4RxOctetSrv;               // number of received octet can be serviced each time
    UINT32 u4RxDrop;                   // Dropped packets
    UINT32 u4RxErr;                    // Total RX error count
    UINT32 u4RxCrcErr;                 // CRC error count 
    UINT32 u4RxLateColErr;             // Late collision error count 
    UINT32 u4RxLenErr;                 // Length error count
    UINT32 u4RxOverErr;                // Overflow error count
    UINT32 u4RxDribErr;                // Dribbled error count

    UINT32 u4TxFrame;                  // number of transmitted frame
    UINT32 u4TxOctet;                  // number of transmitted octet
    UINT32 u4TxFrameSrv;               // number of tx frame can be serviced each time
    UINT32 u4TxOctetSrv;               // number of tx octet can be serviced each time
    UINT32 u4TxErr;                    // Total TX error count
    UINT32 u4TxColCnt;                 // Collision count
    UINT32 u4TxCarErr;                 // No and lost carrier error count
    UINT32 u4TxExcessDefer;            // Excessive deferral of over 24288 bit time
    UINT32 u4TxExcessColErr;           // Tx excessive collision error count
    UINT32 u4TxLateColErr;             // Tx late collision error count
} MAC_STAT_T, *PMAC_STAT_T;


//*********************************************************************
// MAC MII status  
//*********************************************************************
typedef struct MacMiiStatus
{
    UINT32   fgFullDup           : 1;    // Fill duplex or half duplex
    UINT32   fgSpeed1000         : 1;    // 1000Mbit speed or not, reserved
    UINT32   fgSpeed100          : 1;    // 100 or 10 Mbit speed
    UINT32   fgLinkUp            : 1;    // Link ip or link off
    UINT32   fgLpNwAble          : 1;    // Link partner support auto-negotiation
} MAC_MII_T, *PMAC_MII_T;


typedef struct MacAddr
{
    MAC_ADDRESS_T   rMacAddr;            // MAC address
    BOOL            fgValidInFlash;      // MAC in flash is valid or not
} MAC_ADDR_T;


typedef struct EthBitRate
{
    UINT32      u4RxBitRate;        /* Ethernet driver rx bit rate */
    UINT32      u4PrevRxOctet;      /* Previous number of bytes received */
    UINT32      u4TxBitRate;        /* Ethernet driver tx bit rate */
    UINT32      u4PrevTxOctet;      /* Previous number of bytes transmitted */
    HAL_TIME_T  rTime_Start;
    HAL_TIME_T  rTime_End;
    HAL_TIME_T  rTime_Delta;
} ETH_BIT_RATE_T;


//*********************************************************************
// Ethernet MAC Struct
//*********************************************************************
typedef struct MacStruct
{
    /* tx related parameters */
    UINT32 u4TxIdx;                 /* TX descriptors index */
    UINT32 u4TxCompIdx;             /* Index of tx descriptor that is sent completed */
    UINT32 *pu4TxDescBuf;           /* TX descriptors buffer address */
    UINT32 *pu4TxPktList;           /* TX packet list memory address */
    LIST_HEAD_T rTxPktList;         /* TX descriptor list */
    //HANDLE_T        hTxListSema;  /* TX list semaphore handler */
    PMAC_TX_DESC_T  prTxDesc;       /* TX descriptor struct */
	
    /* rx related parameters */
    UINT32 u4RxIdx;                 /* RX descriptors index */
    UINT32 u4RxDescNoBufIdx;        /* Index of rx descriptor without packet buffer */
    UINT32 u4RxNoBufCnt;            /* no RX buffer count */
    UINT32 u4RxPktHdrSize;          /* RX packet buffer header size */
    UINT32 *pu4RxDescBuf;           /* RX descriptors buffer address */
    UINT32 *pu4RxPktBuf;            /* RX packet buffer address */
    LIST_HEAD_T rRxPktList;         /* RX packet buffer list */
    HANDLE_T        hRxListSema;    /* RX list semaphore handler */
    PMAC_RX_DESC_T  prRxDesc;       /* RX descriptors struct */
    	
    /* others */
    UINT32 u4PhyAddr;               /* PHY device address on MII interface */
    UINT32 u4IrqStatus;             /* Interrupt status */
    UINT32 u4McListNum;             /* multicast list number, max. is 15 */
    UINT32 u4DevStatus;             /* MAC device status */
    HANDLE_T       hDevStatusSema;  /* MAC device status semaphore handler */
    ETH_BIT_RATE_T rEthBitRate;     /* record current tx/rx bit rate */ 
    MAC_MII_T      rMii;            /* MII status */ 
    MAC_STAT_T     rStat;           /* TX/RX process statistics */
    EV_GRP_EVENT_T u8TaskEvt;
} ETHER_ADAPTER_T, *PETHER_ADAPTER_T;

#endif // _ETHER_PRIV_H_
