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
** $Log: i2128.h $
** Revision 1.7  2004/05/12 06:13:15Z  doublelo
** Add burst MCR r/w , t/r status  port define.
** Revision 1.6  2004/02/03 12:50:48Z  doublelo
** Add TKIP_MIC define.
** Modify MIC key value
** Revision 1.5  2004/01/16 06:36:01Z  doublelo
** Modify copy right
** Revision 1.4  2004/01/07 12:01:51Z  doublelo
** Add MIC_KEY , TFCB length define.
** Revision 1.3  2004/01/02 10:02:13Z  doublelo
** Modify TFCB structure for security.Add security define.
** Revision 1.2  2003/11/20 01:14:32Z  doublelo
** Modify TFCB length field.
**
** Revision 1.1  2003/10/17 14:06:14Z  doublelo
** Initial revision
*/

#ifndef _I2128_H
#define _I2128_H

#define RATE_1M                     0x00    /* 1M */
#define RATE_2M                     0x01    /* 2M */
#define RATE_5_5M                   0x02    /* 5.5M */
#define RATE_11M                    0x03    /* 11M */
#define RATE_6M                     0x2b    /* 6M */
#define RATE_9M                     0x2f    /* 9M */
#define RATE_12M                    0x2a    /* 12M */
#define RATE_18M                    0x2e    /* 18M */
#define RATE_24M                    0x29    /* 24M */
#define RATE_36M                    0x2d    /* 36M */
#define RATE_48M                    0x28    /* 48M */
#define RATE_54M                    0x2c    /* 54M */

/* Data privacy suite code*/
#define NOTUSED                         0x00
#define WEP_40                          0x01
#define TKIP                            0x02
#define WRAP                            0x03
#define CCMP                            0x04
#define WEP_104                         0x05
#define WEP_128                         0x06
#define TKIP_ON_THE_FLY                 0x07
#define TKIP_MIC                        0x0a
#define TX_IV                           0x00000000
#define TX_EIV                          0x00000000
#define MIC_KEY0                        0x03020100
#define MIC_KEY1                        0x07060504

#define TFD_LEN                         16
#define SCI_LEN                         28
#define TCI_LEN                         32
#define MHI_LEN                         36
#define IV_LEN                          4
#define ICV_LEN                         4
#define EIV_LEN                         4
#define MIC_LEN                         8
#define TBD_LEN                         12
#define RTFCB                           0x80
/* for copy temp_TFCB to DMA used TFCB memory */
#define NonSec_TFD_LEN        4 + TFD_LEN + SCI_LEN + TCI_LEN + MHI_LEN
#define WEP_TFD_LEN           NonSec_TFD_LEN + IV_LEN
#define TKIP_TFD_LEN          NonSec_TFD_LEN + IV_LEN + EIV_LEN

#define TFCB_LEN_WITHOUT_IV             4 + TFD_LEN + SCI_LEN + TCI_LEN + MHI_LEN + TBD_LEN

#if defined(WLAN_HIF_SPI) || defined(WLAN_HIF_SDIO)
#define TX_DATA_PORT                    0x3000
#define TX_STATUS_PORT                  0x4000
#define RX_STATUS_PORT                  0x5000
#define RX_DATA_PORT                    0x6000
#endif

#ifdef WLAN_HIF_HPI
/* 5911 DATA/STAUS PORT */
#define TX_DATA_PORT                    0x600
#define TX_STATUS_PORT                  0x640
#define RX_STATUS_PORT                  0x680
#define RX_DATA_PORT                    0x6c0
#define MIC_RESULT_PORT                 0x700
#endif
/************************************************************************
*                         D A T A   T Y P E S
*************************************************************************
*/

typedef struct _WLAN_MAC_HEADER_A4_QOS_STRUC {
    UINT_16      frameCtrl;
    UINT_16      durationId;
    UINT_8       addr1[MAC_ADDRESS_LENGTH];
    UINT_8       addr2[MAC_ADDRESS_LENGTH];
    UINT_8       addr3[MAC_ADDRESS_LENGTH];
    UINT_16      seqCtrl;
    UINT_8       addr4[MAC_ADDRESS_LENGTH];
    UINT_16      qosCtrl;
} __KAL_ATTRIB_PACKED__ WLAN_MAC_HEADER_A4_QOS_STRUC, *PWLAN_MAC_HEADER_A4_QOS_STRUC;


/* Transmit Frame Control Block (TFCB) */
typedef struct _TFCB_STRUC {
    UINT_16     reserved0;
    UINT_16     txFrameLength;
    /* transmit frame descriptor (TFD) (0x00 -- 0x0f) */
//    union {
//        struct {
            UINT_8  tfdCtrlFlag;    /* 0x00: TFD control flag - EOL, NB, More, CFB, SEC, and Paired */
            UINT_8  tfdChecksum;    /* 0x01: TFD checksum */
//       } s;
//        UINT_16     tfdCtrl;        /* 0x00-0x01: TFD control field -- flag & checksum */
//    } u_tcf;
    UINT_8      reserved1[2];       /* 0x02-0x03: reserved */
    UINT_32     txPacketID;         /* 0x04-0x07: txPacketID */
    UINT_32     secModeDestAddr;    /* 0x08-0x0b: SEC mode operation result dest addr or TQ0/1 tx enable mapping */
    UINT_32     reserved2;          /* 0x0c-0x0f: reserved */
    /* security control information (0x10 -- 0x2b) */
    UINT_8      sciCtrlFlag;        /* 0x10: SCI control flag */
    UINT_8      secOpMode;          /* 0x11: security operation mode */
    UINT_8      dmaFifoHeaderLen;   /* 0x12: DMA FIFO header length */
    UINT_8      reserved3;          /* 0x13: reserved */
    UINT_32     sciIv;          /* 0x14-0x17: SCI IV */
    UINT_32     sciExtendedIv;      /* 0x18-0x1b: SCI extended IV */
    //UINT_8      encryptionKey[16];  /* 0x1c-0x2b: encryption key */
    UINT_32     key1;                 /* 0x1c-0x2b: encryption key */
    UINT_32     key2;                 /* 0x1c-0x2b: encryption key */
    UINT_32     key3;                 /* 0x1c-0x2b: encryption key */
    UINT_32     key4;                 /* 0x1c-0x2b: encryption key */
    /* transmit control information (0x2c -- 0x43) */
    UINT_8      tciCtrlFlag;        /* 0x2c: TCI control flag */
    UINT_8      rtsRetryLimit;      /* 0x2d: RTS retry limit */
    UINT_8      rtsCtsRate;         /* 0x2e: RTS/CTS rate */
    UINT_8      tidMacHdrCtrlFlag;  /* 0x2f: TID & MAC header control flag */
    UINT_16     rtsCtsDuration;     /* 0x30-0x31: RTS/CTS duration field */
    UINT_16     cfPollTxopLimit;    /* 0x32-0x33: CFPollTXOPLimit */
    UINT_16     packetTxTime1;      /* 0x34-0x35: packet tx time 1 */
    UINT_16     packetTxTime2;      /* 0x36-0x37: packet tx time 2 */
    UINT_16     minFrameExchangeTime1;  /* 0x38-0x39: min frame exchange time 1 */
    UINT_16     minFrameExchangeTime2;  /* 0x3a-0x3b: min frame exchange time 2 */
    UINT_16     multirateSwCtrl;    /* 0x3c-0x3d: multirate switching control */
    UINT_16     txRates;            /* 0x3e-0x3f: transmission rates */
    UINT_32     tQ0IdxBitMap;       /* 0x40-0x43: TQ0 Index Bit map */
    UINT_8      txPower;            /* 0x44: Tramsmission power*/
    UINT_8      reserved4[3];       /* 0x45-0x47: reserved */
    UINT_16     mpduLen;            /* 0x48-0x49: MPDU length */
    UINT_16     totalTxDataLen;     /* 0x4a-0x4b: total tx data length */
    /* MAC header information (0x4c -- 0x77) */
    UINT_8      macHeaderLen;       /* 0x4c: MAC header length */
    UINT_8      reserved5;          /* 0x4d: reserved */
    UINT_16     durationId2;        /* 0x4e-0x4f: duration/ID 2 */
    UINT_16     frameCtrl;          /* 0x50-0x51: frame control */
    UINT_16     durationId1;        /* 0x52-0x53: duration/ID 1 */
    UINT_8      macHeader[28];      /* 0x54-0x6f: MAC header field */
    //#ifdef WEP_128                      /* double add 12/25 for first security test*/
    UINT_32     IV_wep;             /* 0x70-0x73: IV */
    UINT_32     EIV_wep;
    //#endif
    //UINT_32     extendedIv;       /* 0x74-0x77: extended IV */
    UINT_8      tbbEOL;             /* 0x70:TBB EOL */
    UINT_8      tbbCheckSum;        /* 0x71:TBB checkSum*/
    UINT_16     tbbBufferLen;       /* 0x72-0x73*/
    UINT_32     reserved6[2];       /* 0x74-0x7B*/
} TFCB_STRUC, *PTFCB_STRUC;

/* Transmit Frame Status (RFB)  */
typedef struct _TFCB_STATUS_STRUC {
    /* receive frame descriptor (RFB) (0x00 -- 0x0b) */
    UINT_8      timeStamp[8];        /* 0x00-0x07: timeStamp*/
    UINT_16     ALC;                 /* 0x08-0x09: ALC */
    UINT_16     packetID;            /* 0x0a-0x0b: tx Packet ID */
    UINT_16     status;              /* 0x0c-0x0d: tx status */
    UINT_8      rtsRetryCnt;         /* 0x0e */
    UINT_8      mpduRetryCnt;        /* 0x0f */
} TFCB_STATUS_STRUC, *PTFCB_STATUS_STRUC;

/* Receive Frame Block (RFB)  */
typedef struct _RFB_STRUC {
    /* receive frame descriptor (RFB) (0x00 -- 0x0b) */
    UINT_8      hwSeqNo;        /* 0x00: RFB flags - EOL bit */
    UINT_8      reserved1[3];   /* 0x01-0x03: reserved */
    UINT_8      macHeader[32];  /* 0x04-0x23: MAC header field */
} RFB_STRUC, *PRFB_STRUC;

/* Receive Frame Status   */
typedef struct _RFB_STATUS_STRUC {
    /* receive frame descriptor (RFB) (0x00 -- 0x0b) */
    UINT_8  hwSeqNo;        /* 0x00: RFB flags - EOL bit */
    //UINT_8   reserved1[3];   /* 0x01-0x03: reserved */
    UINT_8 reserved1;
    UINT_16 rxPortByteCount;

    UINT_32  FCS;
    UINT_8   rxTimestamp[8];
    UINT_32  rxStatus;
    UINT_32  rxBytecount;
    UINT_8   bbCR52;              // BB CR 24
    UINT_8   bbCR53;              // BB CR 25
    UINT_8   bbCR54;              // BB CR 26
    UINT_8   bbCR55;              // BB CR 27
    UINT_8   bbCR89;              // BB CR 28
    UINT_8   bbCR90;              // BB CR 29
    UINT_8   bbCR91;              // BB CR 30
    UINT_8   bbCR92;              // BB CR 31
    UINT_8   bbCR62;              // BB CR 32
    UINT_8   bbCR63;              // BB CR 33
    UINT_8   bbCR93;              // BB CR 34
    UINT_8   bbCR94;              // BB CR 35
    UINT_8   bbCRA;              // BB CRA
    UINT_8   bbCRB;              // BB CRB
    UINT_8   bbCRC;              // BB CRC
    UINT_8   bbCRD;              // BB CRD 36
    UINT_32  rxSecStatus;
    UINT_32  IV;
    UINT_32  EIV;
    //UINT_8      info[48];  /* 0x04-0x23: MAC header field */
} RFB_STATUS_STRUC, *PRFB_STATUS_STRUC;

/************************************************************************
*                             M A C R O S
*************************************************************************
*/
#ifdef WLAN_HIF_SDIO
//extern INT32 SDIO_WritePort(IN UINT8 *pucBuf, IN UINT16 u2Len, IN UINT16 u2Port);
//extern INT32 SDIO_ReadPort(OUT UINT8 *pucBuf, IN UINT16 u2Len, IN UINT16 u2Port);
//extern INT32 SDIO_WriteMCR(UINT16 offset, UINT32 value);
//extern INT32 SDIO_ReadMCR(UINT16 offset, UINT32 *pValue);

/* Macros to read and write MAC control registers (MCRs) */
#define NIC_MCR_READ(_pAdapter, _register, _pValue) \
    SDIO_ReadMCR(_register, (UINT_32 *)(_pValue))

#define NIC_MCR_WRITE(_pAdapter, _register, _value) \
    SDIO_WriteMCR( _register, (UINT_32)(_value))
#endif

#ifdef WLAN_HIF_HPI
extern INT32 SDIO_WritePort(IN UINT8 *pucBuf, IN UINT16 u2Len, IN UINT16 u2Port);
extern INT32 SDIO_ReadPort(OUT UINT8 *pucBuf, IN UINT16 u2Len, IN UINT16 u2Port);
extern void hpiWriteMCR( u16 offset, u32 value);
extern void hpiReadMCR(u16 offset, u32 *pValue);

/* Macros to read and write MAC control registers (MCRs) */
#define NIC_MCR_READ(_pAdapter, _register, _pValue) \
    hpiReadMCR(_register, (u32 *)(_pValue))

#define NIC_MCR_WRITE(_pAdapter, _register, _value) \
    hpiWriteMCR( _register, (u32)(_value))
#endif

#ifdef WLAN_HIF_SPI
#include "spi.h"
__KAL_INLINE__  void NIC_MCR_WRITE(void *pAdapter, unsigned int offset, unsigned long u4McrValue)
{
    vSpiSendCmd32((SPI_FUN_WR | SPI_FUN_CONF_WLAN), offset, (unsigned int *)&(u4McrValue));
}

__KAL_INLINE__  void NIC_MCR_READ(void *pAdapter, unsigned int offset, unsigned long *pU4McrValue)
{
    vSpiSendCmd32((SPI_FUN_RD | SPI_FUN_CONF_WLAN), offset, (unsigned int *)pU4McrValue);
}

__KAL_INLINE__  void writePort(IN PUINT_16 buf_p, IN UINT_16 len, IN UINT_16 port)
{
    vSpiReadWriteData((SPI_FUN_WR | SPI_FUN_CONF_WLAN), (unsigned int)port, (unsigned char *)buf_p, (unsigned int)len);
}

__KAL_INLINE__  void readPort(OUT PUINT_16 buf_p, IN UINT_16 len, IN UINT_16 port)
{
    vSpiReadWriteData((SPI_FUN_RD | SPI_FUN_CONF_WLAN), (unsigned int)port, (unsigned char *)buf_p, (unsigned int)len);
}
#endif

/************************************************************************
*                    D A T A   D E C L A R A T I O N S
*************************************************************************
*/


/************************************************************************
*                 F U N C T I O N   D E C L A R A T I O N S
*************************************************************************
*/

#endif
