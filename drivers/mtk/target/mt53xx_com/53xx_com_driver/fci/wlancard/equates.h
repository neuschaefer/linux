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
** $Log: equates.h $
** Revision 1.5  2004/06/23 06:48:10Z  jiun
** Revision 1.3  2004/05/03 10:45:45Z  jiun
** Revision 1.2  2004/03/31 06:35:35Z  jiun
** Revision 1.1  2004/03/08 13:52:33Z  jiun
** Initial revision
** Revision 1.5  2004/02/27 03:54:27Z  jayshen
** Remove redundant code
** Revision 1.4  2004/02/05 09:29:50Z  jayshen
** Modify the definition value of NUM_TX_QUEUES
** Revision 1.3  2004/01/16 05:30:47Z  jayshen
** 1. Replace "2220" with "2128".
** 2. Change copyright from "2002-2003" to "2003-2004".
** Revision 1.2  2003/12/18 08:34:26Z  kchuang
** remove redundant code
** Revision 1.1  2003/11/19 12:27:10Z  jayshen
** Initial revision
*/


#ifndef _EQUATES_H
#define _EQUATES_H

/************************************************************************
*                     C O M P I L E R   F L A G S
*************************************************************************
*/

/************************************************************************
*                          C O N S T A N T S
*************************************************************************
*/

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


/* MAC address */
#define DEFAULT_MAC_ADDR        {0x00, 0x08, 0x22, 0xFE, 0x00, 0x01}
#define NULL_MAC_ADDR           {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
#define BROADCAST_MAC_ADDR      {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}

/* Chip ID and revision */
#define IPN2128                 0x21280000
#define IPN2126                 0x21260000

#define CHIP_ID_VERSION_MASK    BITS(8,31) /* mask for chip ID and version */
#define CHIP_ID_MASK    BITS(16,31)                 /* mask for chip ID */
#define FPGA_VERSION_MASK       BITS(0,15)  /* mask for FPGA emulation platform version */

#define IPN2128_FPGA            0x01            // FPGA version driver set 1
#define IPN2128_BCI_P           0x00            // BCI parallel interface set 1

/* Adapter Types Supported */
#define IPN2128_MINI_PCI        0x00
#define IPN2128_CARDBUS         0x01

/* Supported RF chipsets */
#define RF_RFMD                 0x00000001  /* RF MicroDevice */
#define RF_MAXIM                0x00000002  /* Maxim */
#define RF_AIROHA               0x00000003  /* Airoha */


#define MAC_ADDRESS_LENGTH              6
#define MAC_ADDR_LEN MAC_ADDRESS_LENGTH

/* Ethernet Frame Sizes */
//#define ETHERNET_ADDRESS_LENGTH         6
#define ETHERNET_HEADER_SIZE            14
#define MINIMUM_ETHERNET_PACKET_SIZE    60
#define MAXIMUM_ETHERNET_PACKET_SIZE    1514

/* IEEE 802.11 WLAN Frame Sizes */
#define WLAN_MAC_HEADER_SIZE            24      /* Address 4 excluded */
#define MINIMUM_WLAN_PACKET_SIZE        16
#define MAXIMUM_WLAN_PACKET_SIZE        4096    // including encryption needed

/* PS-Poll frame MAC header size */
#define WLAN_PSPOLL_MAC_HEADER_SIZE     16

#define FRAME_BODY_MAX_LEN              2304    /* maximum length of MPDU */

#define WLAN_MAC_FCS_LEN                4   /* length of FCS field in MAC frame */
#define WLAN_MAC_IV_LEN                 4   /* length of IV field in encrypted MAC frame */
#define WLAN_MAC_ICV_LEN                4   /* length of ICV field in encrypted MAC frame */
#define WLAN_MAC_EXT_IV_LEN             4   /* length of Extented ICV field in encrypted MAC frame */
#define WLAN_MAC_MIC_LEN                8   /* length of MIC field in encrypted MAC frame */
#define WLAN_MAC_REPLAY_CTR_LEN         4   /* length of Replay Counter field in encrypted MAC frame */
#define WLAN_MAC_CKIP_MIC_LEN           4   /* length of CKIP MIC */
#define WLAN_MAC_CKIP_SEQ_LEN           4   /* length of CKIP SEQ */

#define CCM_IB_FLAGS                    0x59
#define CCM_CTR_FLAGS                   0x01

//#define TCB_BUFFER_SIZE                 0XE0 // 224
#define COALESCE_BUFFER_SIZE            2048    /* 4k bytes */
//#define ETH_MAX_COPY_LENGTH             0x80 // 128

// Make receive area 1536 for 16 bit alignment.
//#define RCB_BUFFER_SIZE       MAXIMUM_ETHERNET_PACKET_SIZE
//#define RCB_BUFFER_SIZE                 1520 // 0x5F0

/* Wireless LAN packet buffer size (padding for 16-byte alignment) */
#define WLAN_PACKET_BUFFER_SIZE         (((MAXIMUM_WLAN_PACKET_SIZE + 15) >> 4) << 4)

/* transmit buffer size (padding for 16-byte alignment) */
#define TX_BUFFER_SIZE                  WLAN_PACKET_BUFFER_SIZE

/* receive buffer size (padding for 16-byte alignment) */
#define RX_BUFFER_SIZE                  WLAN_PACKET_BUFFER_SIZE

//- Area reserved for all Non Transmit command blocks
//#define MAX_NON_TX_CB_AREA              512

/* IEEE 802.11 defined time constant */
#define TU                          1024  /* Time uint: 1024 microseconds */

/*  Ndis/Adapter driver constants */
#define MAX_PHYS_DESC               16

#define NUM_TX_QUEUES               4

/* System wide Equates */
//#define MAX_NUMBER_OF_EISA_SLOTS    15
//#define MAX_NUMBER_OF_PCI_SLOTS     15

#define  MAX_ARRAY_SEND_PACKETS                 8

/* Number of RFBs - minimum, default, and maximum */
#define NIC_MIN_NUM_RFBS                    4
#define NIC_DEF_NUM_RFBS                    20
#define NIC_MAX_NUM_RFBS                    1024

/* Maximum number of remote STA records */
#define MAX_NUM_REMOTE_STA_RECORDS          16

/* Maximum number of supported physical layer network subtypes */
#define MAX_NUM_SUPPORTED_NETWORK_TYPE      3

/* Maximum number of scan results */
#define MAX_NUM_SCAN_RESULTS        32

#ifndef FALSE
#define FALSE       0
#define TRUE        1
#endif

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


#endif  /* _EQUATES_H */

