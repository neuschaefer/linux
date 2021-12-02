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
/**
 * AV.link functions.
 *
 * @author sarick.jiang@mediatek.com
 */

//#include "custom_def.h"
#ifdef ENABLE_AVLINK
#include "general.h"
//#include "hw_bim.h"
#include "hw_avlink.h"
//#include "hw_reg.h"
//#include "hw_cec.h"



/*****************************************************
                        Function prototypes 
 ****************************************************/
static UINT32 ReadAVL(UINT32 u4RegOffset);
static void WriteAVL(UINT32 u4RegOffset, UINT32 u4Val, UINT32 dMsk);
void vAVLinkSendTest(void);


/*****************************************************
                        Defines
 ****************************************************/
#define AVLINK_HDR_M1_DES_BIT               (1<<4)
#define AVLINK_HDR_M1_NAS_BIT               (1<<5)
#define AVLINK_HDR_M1_PAS_BIT               (1<<6)
#define AVLINK_HDR_M1_DIR_BIT               (1<<7)
#define AVLINK_CMD_ECT_BIT                  (1<<13)
#define AVLINK_HDR_EOM_BIT                  (1<<15)
#define AVLINK_DATA_EOM_BIT                 (1<<13)
#define AVLINK_BR_READY_BIT                 (1<<16)
#define AVLINK_RX_OVERRUN_BIT               (1<<20)
#define AVLINK_MODE_READY_BIT               (1<<21)
#define AVLINK_HDR_READY_BIT                (1<<22)

#define AVLINK_INT_RX_EVENT_MASK            (0xff0000) /* 0x54 */
#define AVLINK_INT_TX_EVENT_MASK            (0x1f0000) /* 0x54 */
#define AVLINK_HDR_MODE_MASK                0x3
#define AVLINK_HDR_DATA_MASK                0xf000000
#define AVLINK_HDR_SRC_MASK                 0xf00000
#define AVLINK_HDR_DST_MASK                 0xf0000
#define AVLINK_M3_APPID_MASK                0x700

#define AVLINK_RX_EVET_ERR_MASKS            (AVLINK_RX_OVERRUN_BIT)

#define AVL_READ32(offset)                  (u4RegRead4B(AVLINK_BASE+offset))
#define AVL_WRITE32(offset, value)	        (vRegWrite4B(AVLINK_BASE+offset, (value)))
//#define AVLINK_DBG(x...)                    do{Printf("[AVLink] ");Printf(x);}while(0)

#define AVLINK_CLR_INT_RX_EVENT_ALL()       do{WriteAVL(AVL_OFFSET_RX_EVENT, 0, AVLINK_INT_RX_EVENT_MASK);}while(0)
#define AVLINK_CLR_INT_RX_EVENT(x)          do{WriteAVL(AVL_OFFSET_RX_EVENT, 0, AVLINK_INT_RX_EVENT_MASK&(x));}while(0)
#define AVLINK_CLR_INT_TX_EVENT(x)          do{WriteAVL(AVL_OFFSET_TX_EVENT, 0, AVLINK_INT_TX_EVENT_MASK&(x));}while(0)
#define AVLINK_GET_HDR_DST(reg)             ((UINT8)(((ReadAVL(reg))>>16)&0xf))
#define AVLINK_GET_HDR_MODE(x)              ((x)&AVLINK_HDR_MODE_MASK)
#define AVLINK_GET_HDR_LEN(x)               (((x)&AVLINK_HDR_DATA_MASK)>>24)
#define AVLINK_GET_HDR_SRC_M(x)             ((UINT8)(((x)&AVLINK_HDR_SRC_MASK)>>20))
#define AVLINK_GET_HDR_DST_M(x)             ((UINT8)(((x)&AVLINK_HDR_DST_MASK)>>16))
#define AVLINK_GET_M3_APPID(x)              (((x)&AVLINK_M3_APPID_MASK)>>8)
#define AVLINK_IS_HDR_M1_DIR(x)             ((x>>7 )& 0x1)
#define AVLINK_IS_HDR_M1_PAS(x)             ((x>>6) & 0x1)
#define AVLINK_IS_HDR_M1_NAS(x)             ((x>>5) & 0x1)
#define AVLINK_IS_HDR_M1_DES(x)             ((x>>4) & 0x1)
#define AVLINK_IS_HDR_EOM(x)                ((x>>15) & 0x1)
#define AVLINK_IS_DATA_EOM(x)               ((x>>13) & 0x1)
#define AVLINK_IS_MODE_RDY(x)               ((x>>21) & 0x1)
#define AVLINK_IS_HDR_RDY(x)                ((x>>22) & 0x1)
#define AVLINK_IS_RX_ERR(x)                 ((x>>20) & 0x1)
#define AVLINK_IS_BR_RDY(x)                 ((x>>16) & 0x1)
#define AVLINK_CLR_BR_RDY()                 do{\
                                                    WriteAVL(AVL_OFFSET_RX_EVENT, 0, AVLINK_BR_READY_BIT);\
                                            }while(0)

/* note that, the BR_RDY should not be cleared until we take data away */
#define AVLINK_GET_CLR_RX_EVENT(x) \
            do {\
                (x) = ReadAVL(AVL_OFFSET_RX_EVENT) & AVLINK_INT_RX_EVENT_MASK; \
                AVLINK_CLR_INT_RX_EVENT((x)&(~AVLINK_BR_READY_BIT)); \
            } while(0)
    
#define AVLINK_GET_CLR_TX_EVENT(x) \
            do {\
                (x) = ReadAVL(AVL_OFFSET_TX_EVENT) & AVLINK_INT_TX_EVENT_MASK; \
                AVLINK_CLR_INT_TX_EVENT((x)); \
            } while(0)

#define AVLINK_ENABLE_RX_INT()              do{WriteAVL(AVL_OFFSET_RX_EVENT, 0x000000ff, 0x000000ff);}while(0)
#define AVLINK_ENABLE_TX_INT()              do{WriteAVL(AVL_OFFSET_TX_EVENT, 0x0000001f, 0x0000001f);}while(0)
#define AVLINK_DISABLE_RX_INT()             do{WriteAVL(AVL_OFFSET_RX_EVENT, 0x00000000, 0x000000ff);}while(0)
#define AVLINK_DISABLE_TX_INT()             do{WriteAVL(AVL_OFFSET_TX_EVENT, 0x00000000, 0x0000001f);}while(0)
#define AVLINK_ENABLE_ALL_INT()             do{AVLINK_ENABLE_RX_INT();AVLINK_ENABLE_TX_INT();}while(0)
#define AVLINK_DISABLE_ALL_INT()            do{AVLINK_DISABLE_RX_INT();AVLINK_DISABLE_TX_INT();}while(0)
#define AVLINK_SET_STATUS(x,bit)            do{(x)|=bit;}while(0) 
#define AVLINK_IS_STATUS(x,bit)             ((x)&bit)
#define AVLINK_CLR_STATUS_BIT(x,bit)        do{(x) &= ~(bit);}while(0);
#define AVLINK_CLR_STATUS_ALL(x)            do{(x)=0;}while(0)
#define AVLINK_CLR_INT_RX_EVENT_ALL()       do{WriteAVL(AVL_OFFSET_RX_EVENT, 0, AVLINK_INT_RX_EVENT_MASK);}while(0)
#define AVLINK_CLR_INT_RX_EVENT(x)          do{WriteAVL(AVL_OFFSET_RX_EVENT, 0, AVLINK_INT_RX_EVENT_MASK&(x));}while(0)
#define AVLINK_CLR_INT_TX_EVENT(x)          do{WriteAVL(AVL_OFFSET_TX_EVENT, 0, AVLINK_INT_TX_EVENT_MASK&(x));}while(0)


typedef struct _avlink_m1_bits_s
{
    UINT8       dir;
    UINT8       pas;
    UINT8       nas;
    UINT8       des;
    UINT16      qty;
} avlink_m1_bits_t;


/* avlink control block */
typedef struct _avlink_data_s
{
    //HANDLE_T    ehdl;
    UINT32      started;
    UINT32      isr_rx_events;
    UINT32      isr_tx_events;
    /* statistics follows */
    UINT8       stats_start; // used for memset
    /* RX data */
    UINT8       rx_src;
    UINT8       rx_dst;
    UINT8       rx_m3_app_id;
    UINT32      rx_status;
    UINT32      rx_bytes;
    UINT32      rx_mode;
    union
    {
        avlink_m1_bits_t    m1;
        UINT8               rx_buf[32];
    }u_rxdata;
    /* rx check list for ic verification */
    UINT32      rx_err_mask;
    UINT32      rx_mode_rdy_int;
    UINT32      rx_hdr_rdy_int;
    UINT32      rx_br_rdy_int;
} avlink_data_t;

enum
{
    AVLINK_RX_MODE_READY    = 0x0001,
    AVLINK_RX_HDR_READY     = 0x0002,
    AVLINK_RX_INCOMPLETE    = 0x0004
};

enum
{
    AVLINK_ERR_RX_MISS_HDR      = 0x0001,
    AVLINK_ERR_RX_MISS_H_EOM    = 0x0002,
    AVLINK_ERR_RX_MISS_D_EOM    = 0x0004,
    AVLINK_ERR_RX_OVERRUN       = 0x0008,
    AVLINK_ERR_RX_UNKNOWN       = 0x1000
};

/*****************************************************
                        Variables 
 ****************************************************/
 #ifdef CC_MT8223
UINT8 xdata u1AvlinkAddr        _at_ 0xd400;
 #else
UINT8 xdata u1AvlinkAddr        _at_ 0xfc00;
 #endif
static avlink_data_t            avl_data;

/*****************************************************
                        Functions
 ****************************************************/
static UINT32 ReadAVL(UINT32 u4RegOffset)
{
    return (AVL_READ32(u4RegOffset));
}

static void WriteAVL(UINT32 u4RegOffset, UINT32 u4Val, UINT32 dMsk)
{
  //CRIT_STATE_T csState;
  UINT32 u4NewVal;

  //csState = x_crit_start();
  u4NewVal = (ReadAVL(u4RegOffset) & (~dMsk)) | ((u4Val) & dMsk);
  AVL_WRITE32(u4RegOffset, u4NewVal);
  //x_crit_end(csState);
}

static UINT32 _u4AVLinkGetLenByDMask(UINT32 mask)
{
    switch (mask)
    {
        case 0x1:
            return 1;
        case 0x3:
            return 2;
        case 0x7:
            return 3;
        case 0xf:
            return 4;
        default:
            return 0;
    }
}

static void _vAVLinkShowM3Data(avlink_data_t *ad)
{
    INT32 i;

    LogS("M3 Data Content:");

    for (i=0; i< ad->rx_bytes; i++)
    {
        LogB(ad->u_rxdata.rx_buf[i]);
    }
    /* send back for test */
    //vAVLinkSendTest();
}


static void _vAVLinkCompleteRx(avlink_data_t *ad)
{
#ifdef T8032_AVLINK
    LogS("Complete AV.link RX:\n");
    LogS("---------------------\n");
    LogSD("Mode:", ad->rx_mode);
    LogSB("Src", ad->rx_src);
    LogSB("Dst", ad->rx_dst);
    if (ad->rx_mode == 1)
    {
        LogSB("DIR\\:", ad->u_rxdata.m1.dir);
        LogSB("PAS\\:", ad->u_rxdata.m1.pas);
        LogSB("NAS\\:", ad->u_rxdata.m1.nas);
        LogSB("DES\\:", ad->u_rxdata.m1.des);
        //_vAVLinkDumpM1QtyBits(ad->u_rxdata.m1.qty);
    }
    else if (ad->rx_mode == 2)
    {
        LogSB("Opcode:", ad->u_rxdata.rx_buf[0]);
    }
    else if (ad->rx_mode == 3)
    {
        LogSB("M3 App Id:", ad->rx_m3_app_id);
        LogSD("App Bytes:", ad->rx_bytes);
        _vAVLinkShowM3Data(ad);
    }
    /* print INT stats */
    LogSD("rx_mode_rdy_int:", ad->rx_mode_rdy_int);
    LogSD("rx_hdr_rdy_int:", ad->rx_hdr_rdy_int);
    LogSD("rx_br_rdy_int:", ad->rx_br_rdy_int);
    LogSD("rx_err_mask:", ad->rx_err_mask);
    /* reset all stats */
    memset(&ad->stats_start, 0, 
             sizeof(avlink_data_t)-((UINT32)&ad->stats_start - (UINT32)ad));

#else /* Non T8032_AVLINK */
    /* showing statistics */
    Printf("Complete AV.link RX:\n");
    Printf("---------------------\n");
    Printf("Mode: %d\n", ad->rx_mode);
    Printf("Src: 0x%x\n", ad->rx_src);
    Printf("Dst: 0x%x\n", ad->rx_dst);
    if (ad->rx_mode == 1)
    {
        Printf("DIR\\: 0x%x\n", ad->u_rxdata.m1.dir);
        Printf("PAS\\: 0x%x\n", ad->u_rxdata.m1.pas);
        Printf("NAS\\: 0x%x\n", ad->u_rxdata.m1.nas);
        Printf("DES\\: 0x%x\n", ad->u_rxdata.m1.des);
        _vAVLinkDumpM1QtyBits(ad->u_rxdata.m1.qty);
    }
    else if (ad->rx_mode == 2)
    {
        Printf("Opcode:0x%x\n", ad->u_rxdata.rx_buf[0]);
    }
    else if (ad->rx_mode == 3)
    {
        Printf("M3 App Id: 0x%x\n", ad->rx_m3_app_id);
        Printf("App Bytes: %d\n", ad->rx_bytes);
        _vAVLinkShowM3Data(ad);
    }
    /* print INT stats */
    Printf("rx_mode_rdy_int: %d\n", ad->rx_mode_rdy_int);
    Printf("rx_hdr_rdy_int: %d\n", ad->rx_hdr_rdy_int);
    Printf("rx_br_rdy_int: %d\n", ad->rx_br_rdy_int);
    Printf("rx_err_mask: 0x%x\n", ad->rx_err_mask);


    /* reset all stats */
    x_memset(&ad->stats_start, 0, 
             sizeof(avlink_data_t)-((UINT32)&ad->stats_start - (UINT32)ad));
#endif /* T8032_AVLINK */
}



static void vAVL_enableINT(void)
{
	vEnableSysInt(AVLINK_INT_EN);
 
	// TV_EVENT && RX_EVENT
	vRegWrite4BMsk(AVL_TX_EVENT, 0x00, 0xff);
	vRegWrite4BMsk(AVL_RX_EVENT, 0x00, 0xff);
	// RX_EVENT
	//AVL_ENABLE_INT_OV(1);
	//AVL_ENABLE_INT_BR_RDY(1);
	//AVL_ENABLE_INT_HEADER_RDY(1);
	vRegWrite4BMsk(AVL_RX_EVENT, 0xff, 0xff);
	// TX_EVENT
	//AVL_ENABLE_INT_UN(0);
	//AVL_ENABLE_INT_LOW(0);
	//AVL_ENABLE_INT_FAIL(0);
	//AVL_ENABLE_INT_BS(0);
	//AVL_ENABLE_INT_RB(0);
	vRegWrite4BMsk(AVL_TX_EVENT, 0x1f, 0xff);
    }

static void _vAVLinkRx(avlink_data_t *ad)
{
    UINT32 u4rxHdrInfo = 0;
    UINT32 dlen = 0;
    UINT32 data1 = 0;
    UINT32 i;

#ifdef T8032_AVLINK
    LogS("Processing AVLink RX...\n");
#endif
    /* rx hdr information (receiving) */
    u4rxHdrInfo = ReadAVL(AVL_OFFSET_RX_HEADER);
    LogSD("Rx Hdr Info", u4rxHdrInfo);
    /* mode  */
    ad->rx_mode = AVLINK_GET_HDR_MODE(u4rxHdrInfo);
    ad->rx_src = AVLINK_GET_HDR_SRC_M(u4rxHdrInfo);
    ad->rx_dst = AVLINK_GET_HDR_DST_M(u4rxHdrInfo);
    
    if (ad->rx_mode == 1)
    {
        avlink_m1_bits_t *m1 = &ad->u_rxdata.m1;
        LogS("RX Mode 1");
        m1->dir = AVLINK_IS_HDR_M1_DIR(u4rxHdrInfo)? 1:0;
        m1->pas = AVLINK_IS_HDR_M1_PAS(u4rxHdrInfo)? 1:0;
        m1->nas = AVLINK_IS_HDR_M1_NAS(u4rxHdrInfo)? 1:0;
        m1->des = AVLINK_IS_HDR_M1_DES(u4rxHdrInfo)? 1:0;
        //todo qty bits
        m1->qty = (UINT16)(ReadAVL(AVL_OFFSET_RX_DATA)&0xff);
        ad->rx_br_rdy_int ++;
        AVLINK_CLR_STATUS_ALL(ad->rx_status);
        AVLINK_CLR_BR_RDY();
        _vAVLinkCompleteRx(ad);
    }
    else if (ad->rx_mode == 2)
    {
        LogS("RX Mode 2");
        if (AVLINK_IS_HDR_EOM(u4rxHdrInfo))
            ad->rx_bytes = 0;
        else
            ad->rx_bytes = 1;
        if (ad->rx_bytes && !AVLINK_IS_DATA_EOM(u4rxHdrInfo))
        {
            /* for current implementation, the mode 2 cmd block
               should have only one block, so the D_EOM should
               be set, except it has no cmd block */
            ad->rx_err_mask |= AVLINK_ERR_RX_MISS_D_EOM;
        }
        /* command block */
        ad->u_rxdata.rx_buf[0] = (UINT8)(ReadAVL(AVL_OFFSET_RX_DATA)&0xf);
        ad->rx_br_rdy_int ++;
        AVLINK_CLR_STATUS_ALL(ad->rx_status);
        AVLINK_CLR_BR_RDY();
        _vAVLinkCompleteRx(ad);
    }
    else if (ad->rx_mode == 3)
    {
        UINT32 d_eom;
        LogS("RX Mode 3");

        ad->rx_br_rdy_int ++;
        if (AVLINK_IS_STATUS(ad->rx_status, AVLINK_RX_HDR_READY))
        {
            if (AVLINK_IS_STATUS(ad->rx_status, AVLINK_RX_INCOMPLETE))
            {
                /* data-eom may be lost, so we got both hdr_rdy and rx_incomplete on */
                ad->rx_err_mask |= AVLINK_ERR_RX_MISS_D_EOM;
            }
            AVLINK_CLR_STATUS_BIT(ad->rx_status, AVLINK_RX_HDR_READY);
            AVLINK_SET_STATUS(ad->rx_status, AVLINK_RX_INCOMPLETE);
        }
        
        if (!AVLINK_IS_STATUS(ad->rx_status, AVLINK_RX_INCOMPLETE))
        {
            ad->rx_err_mask |= AVLINK_ERR_RX_MISS_HDR;
            AVLINK_CLR_BR_RDY();
            _vAVLinkCompleteRx(ad);
            return;
        }
        dlen = AVLINK_GET_HDR_LEN(u4rxHdrInfo);
        dlen = _u4AVLinkGetLenByDMask(dlen);
        data1 = ReadAVL(AVL_OFFSET_RX_DATA);
        d_eom = AVLINK_IS_DATA_EOM(u4rxHdrInfo);
        ad->rx_m3_app_id = (UINT8)AVLINK_GET_M3_APPID(u4rxHdrInfo);
        
        if (AVLINK_IS_HDR_EOM(u4rxHdrInfo))
        {
            /* no blocks follows */
            AVLINK_CLR_BR_RDY();
            _vAVLinkCompleteRx(ad);
            return;
            
        }
        for (i=0; i<dlen; i++)
        {
            ad->u_rxdata.rx_buf[ad->rx_bytes] = data1 & 0xff;
            ad->rx_bytes ++;
            data1 >>= 8;
        }
        AVLINK_CLR_BR_RDY();
        if (d_eom)
        {
            _vAVLinkCompleteRx(ad);
        }
    } 
    else
    {
        //unknown mode
        ad->rx_err_mask |= AVLINK_ERR_RX_UNKNOWN;
        AVLINK_CLR_BR_RDY();
        _vAVLinkCompleteRx(ad);
    }
    //notify main task
}

#define MODE_RDY Fld(1, 21, AC_MSKB2) //21

#define IS_AVL_INT_HEADER_RDY() (RegReadFldAlign(AVL_RX_EVENT, HEADER_RDY))
#define IS_AVL_INT_MODE_RDY() (RegReadFldAlign(AVL_RX_EVENT, MODE_RDY))
#define IS_AVL_INT_BR_RDY() (RegReadFldAlign(AVL_RX_EVENT, BR_RDY))

static void _vAVLinkProcessIsr(void)
{
    UINT32 u4RxMask;
    avlink_data_t *ad = &avl_data;
    UINT32 u4TxMask;

#ifdef T8032_AVLINK
    LogS("Process AVLink ISR...\n");
#endif

    AVLINK_GET_CLR_RX_EVENT(u4RxMask);
    AVLINK_GET_CLR_TX_EVENT(u4TxMask);
    LogS("RX_Event="); LogD(u4RxMask);
    LogS("TX_Event="); LogD(u4TxMask); LogS("\n");
    //u4TxMask = ad->isr_tx_events;

//process rx_event:
    if (u4RxMask != 0)
    {
        /* check for error status first */
        if (AVLINK_IS_RX_ERR(u4RxMask))
        {
#ifdef T8032_AVLINK
            LogS("RX ERR detected. ERROR bits in RX_EVENT: "); 
            LogD(u4RxMask&AVLINK_RX_EVET_ERR_MASKS);
#else
            AVLINK_DBG("RX ERR detected. ERROR bits in RX_EVENT:0x%x\n", 
                        u4RxMask&AVLINK_RX_EVET_ERR_MASKS);
#endif
            ad->rx_err_mask |= AVLINK_ERR_RX_OVERRUN;
        }
        if (AVLINK_IS_MODE_RDY(u4RxMask) != 0)
        {
            /* this int is used for verification only */
            ad->rx_mode_rdy_int ++;
        }
        /* if header is ready */
        if (AVLINK_IS_HDR_RDY(u4RxMask))
        {
            LogS("HDR RDY");
            if (AVLINK_GET_HDR_DST(AVL_OFFSET_RX_HD_NEXT) == u1AvlinkAddr || //to me
                AVLINK_GET_HDR_DST(AVL_OFFSET_RX_HD_NEXT) == 0xf) //to all
            {
                LogS("HDR RDY: To Me");
                /* exception handling */
                if (AVLINK_IS_STATUS(ad->rx_status, AVLINK_RX_HDR_READY))
                {
                    /* missed something? */
                    ad->rx_err_mask |= AVLINK_ERR_RX_MISS_H_EOM;
                }

                AVLINK_SET_STATUS(ad->rx_status, AVLINK_RX_HDR_READY);
                ad->rx_hdr_rdy_int ++;
            }
            else
            {
                LogS("HDR RDY: Not To Me");
                /* not for me */
                AVLINK_CLR_STATUS_BIT(ad->rx_status, AVLINK_RX_HDR_READY);
            }
        }
        /* if rx buf is ready */
        if (AVLINK_IS_BR_RDY(u4RxMask))
        {
            LogS("BR RDY");
            _vAVLinkRx(ad);
        }


        /* if still incoming INTs */
        /* NOTE: From testing, we found that, the 
           xxx_RDY flags cannot be cleared unless the BR_RDY bit
           is cleared first!.
           So we don't use loop checking here. */
        /*
        AVLINK_GET_CLR_RX_EVENT(u4RxMask);
        if (u4RxMask)
        {
            goto process;
        }
        */

        ad->isr_rx_events = 0;
        ad->isr_tx_events = 0;
        /* enable interrupt again */
        //AVLINK_ENABLE_ALL_INT();
    }

    if (u4TxMask)
    {
        LogS("Got TX Event:");
        LogD(u4TxMask);
    }
    
    return;

    
}


void _vAVLinkIsr(void)
{
    _vAVLinkProcessIsr();
}

void vAVLinkEnablePinMux(void)
{
    /* PinMux register: 0x280b4
       AVLink at: bit 8~9
       pinmux value: function1 (1)
    */
    vRegWriteFldAlign(0x0b4, 0x1, AVL_PINMUX_BITMASKS);
}

void vAVLinkSendTest(void)
{
    UINT32 header = 0;
    UINT32 send_data = 0x4;
    INT32 i;
    volatile UINT32 status = 0;

    LogS("Avlink Tx Test ----> ");
    /* Send whatever a mode 3 appid:3 cmd to dst: 2 */
    header |= 0x10000; //dst = 1
    header |= 0x3; //mode
    header |= 0x300; //appid
    header |= 0x01000000; //data len =1
    header |= 0x2000; //Data EOM
    WriteAVL(AVL_OFFSET_TX_HD_NEXT, header, 0xFFFFFFFF);
    
    WriteAVL(AVL_OFFSET_TX_DATA_NEXT, send_data, 0xFFFFFFFF);
    WriteAVL(AVL_OFFSET_TX_EVENT, 0x10000, 0xFFFFFFFF);

    /* polling for tx event status */
    for (i=0; i<100000; i++)
    {
        status = ReadAVL(AVL_OFFSET_TX_EVENT);
        if (status != 0)
        {
            LogS("TX Event status");
            LogD(status);
            break;
            WriteAVL(AVL_OFFSET_TX_EVENT, 0x1f, 0xffffffff);
        }
    }
}

INT32 i4AVL_init(void)
{
#ifdef T8032_AVLINK
    LogS("Initializing AV.link\n");
#endif
    /* Setting up global settings */
    u1AvlinkAddr = 0; // TV

    /* Enable PinMux for AVLink */
    vAVLinkEnablePinMux();
    /* Set AVLink TX/RX Timing */
    vRegWriteFldAlign(AVL_CKGEN, 0x00, PDN);
    vRegWrite4B(AVL_CKGEN, 0x0000001e); // 3MHZ 
    // CYJ.NOTE TX_EN, RX_EN: disable it
    vRegWrite4B(AVL_TR_CONFIG, 0x00000000); //avlink mode

    vRegWrite4B(AVL_RX_T_START_R, 0x0189015b);
    vRegWrite4B(AVL_RX_T_START_F, 0x01db01ae);
    vRegWrite4B(AVL_RX_T_DATA, 0x006e00c8);
    vRegWrite4B(AVL_RX_T_ACK, 0x00000096);
    vRegWrite4B(AVL_RX_T_ERROR, 0x01680212);
    vRegWrite4B(AVL_TX_T_START, 0x01c20172);
    vRegWrite4B(AVL_TX_T_DATA_R, 0x003c0096);
    vRegWrite4B(AVL_TX_T_DATA_F, 0x00f000f0);

    vRegWriteFldAlign(AVL_TX_ARB, 6, BCNT_NEW_INIT);
    vRegWriteFldAlign(AVL_TX_ARB, 9, BCNT_NEW_MSG);
    vRegWriteFldAlign(AVL_TX_ARB, 4, BCNT_RETRANSMIT);
    vRegWriteFldAlign(AVL_TX_ARB, 0, MAX_RETRANSMIT);
    /* for tx timing, avlink is still at TR_TEST */
    //vRegWriteFldAlign(AVL_TX_ARB, 0x19, TX_INPUT_DELAY);
    vRegWriteFldAlign(AVL_TR_TEST, 0x19, AVL_TX_INPUT_DELAY);
    AVL_FLOW_CONTROL_ACK(1);

    /* device addr */
    vRegWrite4B(AVL_TR_CONFIG, 0x0ff01100 | ((UINT32)u1AvlinkAddr << 16));
#ifdef T8032_AVLINK
    LogS("Enabling AV.link interrupt\n");
#endif
    /* Enable Interrupt */
    vAVL_enableINT();

    return 0;
}


#endif /* ENABLE_AVLINK */

