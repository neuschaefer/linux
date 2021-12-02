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
/*-----------------------------------------------------------------------------
* $Author: dtvbm11 $
* $Date: 2012/04/27 $
* $RCSfile: nptv_avlink_cmd.c,v $
* $Revision: #1 $
*
* Description:
*         This file contains CLI implementation of NPTV AV.Link.
*---------------------------------------------------------------------------*/

#include "x_os.h"
#include "x_stl_lib.h"
#include "video_def.h"
#ifdef __MW_CLI_DEF__
#include "x_mid_cli.h"
#else
#include "x_drv_cli.h"
#endif
    
#include "x_assert.h"
#include "x_pinmux.h"
#include "x_debug.h"
    
#include "source_table.h"
#include "sv_const.h"

#include "x_pdwnc.h"
#include "hw_ckgen.h"
#include "hw_avlink.h"

/*****************************************************
                        Definitions 
 ****************************************************/
  
#ifndef CC_CLI
#define CC_CLI
#endif

#define USE_PDWNC_INT_FUNC          1

#define AVL_READ32(offset)          IO_READ32(AVLINK_IO_BASE, (offset))
#define AVL_WRITE32(offset, value)	do{IO_WRITE32(AVLINK_IO_BASE, (offset), (value));}while(0)
#define AVLINK_DBG(x...)            do{Printf("[AVLink] ");Printf(x);}while(0)

#define AVLTXNOACK		            0x0001
#define AVLRXOVERFLOW	            0x0002
#define AVLTOOLONGRXCOMMAND	        0x0004
#define AVLRXBUFFERSIZE		        17

#define AVLINK_EVENT_NAME           "avlinkevent"
#define AVLINK_MAX_BITS             100 //EN50157-2-3,p.6
#define AVLINK_MAX_BYTES            (AVLINK_MAX_BITS/8)

#define avlink_tx(x)                do{ \
                                        WriteAVL(AVL_OFFSET_TX_DATA_NEXT, (x), 0xFFFFFFFF); \
                                        WriteAVL(AVL_OFFSET_TX_EVENT, 0x10000, 0xFFFFFFFF); \
                                    } while(0)

/* note that, the BR_RDY should not be cleared until we take data away */
#define AVLINK_GET_CLR_RX_EVENT(x) \
        do {\
            CRIT_STATE_T crt; \
            crt = x_crit_start(); \
            (x) = ReadAVL(AVL_OFFSET_RX_EVENT) & AVLINK_INT_RX_EVENT_MASK; \
            AVLINK_CLR_INT_RX_EVENT((x)&(~AVLINK_BR_READY_BIT)); \
            x_crit_end(crt); \
        } while(0)

#define AVLINK_GET_CLR_TX_EVENT(x) \
        do {\
            CRIT_STATE_T crt; \
            crt = x_crit_start(); \
            (x) = ReadAVL(AVL_OFFSET_TX_EVENT) & AVLINK_INT_TX_EVENT_MASK; \
            AVLINK_CLR_INT_TX_EVENT((x)); \
            x_crit_end(crt); \
        } while(0)


#define AVLINK_SET_EVENT(ehdl,e) \
        do{ \
            VERIFY(x_ev_group_set_event(ehdl, e, X_EV_OP_OR) == OSR_OK); \
        } while(0)

/* event group flags */
#define AVLINK_EG_CTRL_EVENTS              (1<<0)
#define AVLINK_EG_RX_EVENTS                (1<<2)
#define AVLINK_EG_TX_EVENTS                (1<<3)
#define AVLINK_EG_WAKEUP_MASK              (AVLINK_EG_RX_EVENTS | \
                                            AVLINK_EG_TX_EVENTS | \
                                            AVLINK_EG_CTRL_EVENTS)

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

#define AVLINK_GET_HDR_DST(reg)             ((UINT8)(((ReadAVL(reg))>>16)&0xf))
#define AVLINK_GET_HDR_MODE(x)              ((x)&AVLINK_HDR_MODE_MASK)
#define AVLINK_GET_HDR_LEN(x)               (((x)&AVLINK_HDR_DATA_MASK)>>24)
#define AVLINK_GET_HDR_SRC_M(x)             ((UINT8)(((x)&AVLINK_HDR_SRC_MASK)>>20))
#define AVLINK_GET_HDR_DST_M(x)             ((UINT8)(((x)&AVLINK_HDR_DST_MASK)>>16))
#define AVLINK_GET_M3_APPID(x)              (((x)&AVLINK_M3_APPID_MASK)>>8)
#define AVLINK_IS_HDR_M1_DIR(x)             ((x)&AVLINK_HDR_M1_DIR_BIT)
#define AVLINK_IS_HDR_M1_PAS(x)             ((x)&AVLINK_HDR_M1_PAS_BIT)
#define AVLINK_IS_HDR_M1_NAS(x)             ((x)&AVLINK_HDR_M1_NAS_BIT)
#define AVLINK_IS_HDR_M1_DES(x)             ((x)&AVLINK_HDR_M1_DES_BIT)
#define AVLINK_IS_HDR_EOM(x)                ((x)&AVLINK_HDR_EOM_BIT)
#define AVLINK_IS_DATA_EOM(x)               ((x)&AVLINK_DATA_EOM_BIT)
#define AVLINK_IS_MODE_RDY(x)               ((x)&AVLINK_MODE_READY_BIT)
#define AVLINK_IS_HDR_RDY(x)                ((x)&AVLINK_HDR_READY_BIT)
#define AVLINK_IS_RX_ERR(x)                 ((x)&AVLINK_RX_EVET_ERR_MASKS)
#define AVLINK_IS_BR_RDY(x)                 ((x)&AVLINK_BR_READY_BIT)
#define AVLINK_CLR_BR_RDY()                 do{\
                                                if(!disableBrRdyClr)\
                                                    WriteAVL(AVL_OFFSET_RX_EVENT, 0, AVLINK_BR_READY_BIT);\
                                            }while(0)

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
    
    AVLINK_ERR_RX_UNKNOWN       = 0x10000000
};

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
    HANDLE_T    ehdl;
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


/*****************************************************
                        Variables 
 ****************************************************/
static UINT16           _uAVLStatus;
static avlink_data_t    avlink_data;
static UINT8            avlink_own_addr = 0x00; /* tv */
static INT32            disableBrRdyClr = 0;

/*****************************************************
                        Function prototypes 
 ****************************************************/
static void _vAVLinkIsr(UINT16 u2Vector);


/*****************************************************
                        Functions 
 ****************************************************/

static inline UINT32 ReadAVL(UINT32 u4Reg)
{
	return (AVL_READ32(u4Reg));
}

static inline void WriteAVL(UINT32 u4Reg, UINT32 u4Val, UINT32 dMsk)
{
  CRIT_STATE_T csState;
  UINT32 u4NewVal;

  csState = x_crit_start();
  u4NewVal = (ReadAVL(u4Reg) & (~dMsk)) | ((u4Val) & dMsk);
  AVL_WRITE32(u4Reg, u4NewVal);
  x_crit_end(csState);
}


static inline UINT8 _u1AVLinkGetDataMask(INT32 i4Bytes)
{
    switch (i4Bytes)
    {
        case 1:
            return (0x1);
        case 2:
            return (0x3);
        case 3:
            return (0x7);
        case 4:
        default:
            return (0xf);
    }
}

static inline UINT32 _u4AVLinkGetLenByDMask(UINT32 mask)
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


static void vAVL_enableINT(void)
{
	x_os_isr_fct pfnOldIsr;

#if USE_PDWNC_INT_FUNC
    VERIFY(PDWNC_RegIsr(PINTNO_AVLINK, _vAVLinkIsr, &pfnOldIsr) == PDWNC_OK);
    AVLINK_DBG("Use Pdwnc Int Func...\n");
#else
    VERIFY(x_reg_isr(VECTOR_PDWNC, _vAVLinkIsr, &pfnOldIsr) == OSR_OK);
	BIM_EnableIrq(VECTOR_PDWNC);
	EnCecInt;
#endif
 
	// TV_EVENT && RX_EVENT
	vRegWrite4BMsk(AVL_TX_EVENT, 0x00, 0xff);
	vRegWrite4BMsk(AVL_RX_EVENT, 0x00, 0xff);
	// RX_EVENT
	AVL_ENABLE_INT_OV(1);
	AVL_ENABLE_INT_BR_RDY(1);
	AVL_ENABLE_INT_HEADER_RDY(1);
	// TX_EVENT
	AVL_ENABLE_INT_UN(0);
	AVL_ENABLE_INT_LOW(0);
	AVL_ENABLE_INT_FAIL(0);
	AVL_ENABLE_INT_BS(0);
	AVL_ENABLE_INT_RB(0);
}


INT32 i4AVL_init(void)
{
    AVLINK_DBG("%s ...\n", __FUNCTION__);
	/* Set CEC TX/RX Timing */
	//CKGEN_WRITE32(REG_XTAL_CKCFG, (CKGEN_READ32(REG_XTAL_CKCFG)&0xfffffcff)|0x00000100);
	vIO32WriteFldAlign(CKGEN_XTAL_CKCFG, 1, FLD_C27M_CK);

    if (1)
    {
        /* AVlink pinmux is default set to gpio(func0), so we set it to func1 */
        ENABLE_PINMUX_TO_AVLINK();
        //vSetT8032CtlCEC(0);    //Stop T8032 Control CEC
        //u1TVLogicAddr = 0; //TV

        vRegWriteFldAlign(AVL_CKGEN, 0x00, PDN);	
        vRegWriteFldAlign(AVL_CKGEN, 0x1e, DIV_SEL);	// 3MHz 
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

    }

    AVLINK_DBG("Enabling AVLink Interrupt...\n");
    /* register ISR and enable interrupt */
    vAVL_enableINT(); // CYJ.NOTE: Disable this in the beginning of verification

    AVLINK_DBG("AVLink driver inited...\n");
	return 0;
}

static INT32 _i4AVLinkM1Send(UINT8 byte0, UINT8 byte1, UINT8 byte2)
{
    INT32 i4Ret = -1;
    UINT32 header = 0, data = 0;
        
    WriteAVL(AVL_OFFSET_CKGEN, 0x0000001E, 0xFFFFFFFF); // clock: 3M/2/0x1E = 50khz
    WriteAVL(AVL_OFFSET_TR_CONFIG, 0x00001100, 0xFFFFFFFF); // Enable AV Link Mode Tx   
    WriteAVL(AVL_OFFSET_TX_EVENT, 0x0000001F, 0xFFFFFFFF); // Enable AV Link Mode Tx_EVENT Triggers
    /* check if hw tx fail */
    if ( (ReadAVL(AVL_OFFSET_TX_STATUS) & 0x7fff) == 0x10 )
    {
        WriteAVL(AVL_OFFSET_TR_CONFIG, 0x00000100, 0xFFFFFFFF);
        WriteAVL(AVL_OFFSET_TR_CONFIG, 0x00001100, 0xFFFFFFFF);
        _uAVLStatus |= AVLTXNOACK;
        AVLINK_DBG("Tx hw fail\n");
        return i4Ret;
    }
    /* check if hw is still transmitting */
	if( ReadAVL(AVL_OFFSET_TX_EVENT) & 0x10000 )
    {
        AVLINK_DBG("TX Buf still busy\n");
		return i4Ret;
    }
    /* mode 1*/
    header |= (1&0x3);

    /* header DIR/PAS/NAS/DES */
    header |= (((byte0)<<4)&0xf0);
    WriteAVL(AVL_OFFSET_TX_HD_NEXT, header, 0xFFFFFFFF);

    /* qty */
    data |= (byte1<<8)&0xff00;
    data |= (byte2)&0xff;
    
    /* tx data out */
    avlink_tx(data);
    i4Ret = 0;
    AVLINK_DBG("TXed mode 1, HDR=0x%x, QTY=0x%x\n", header, data);

    return i4Ret;
}

static INT32 _i4AVLinkSend(INT32 i4Mode, INT32 i4Bytes, UINT8 u1Dst)
{
    INT32 i4Ret = -1;
    UINT32 header = 0, data = 0;
    INT32 i4BytesLeft = 0;
    UINT8 u1Data = 0;

    /* validate input parameters */
    if (3 < i4Mode || i4Mode < 1)
    {
        AVLINK_DBG("Invlaid mode:%d\n", i4Mode);
        return i4Ret;
    }
    if (u1Dst > 0xf)
    {
        AVLINK_DBG("Invalid dst addr:0x%x (0~0xf)\n", u1Dst);
        return i4Ret;
    }

    /* Start Tx setup */
    /***********************************************************
     * Before this, it should be noted that, this AV.link test 
     * assumes that the AV.link hw (ie. CEC hw) has been initialized
     * by CEC module. So what we need to do is to modify the clock timing
     * (offset:0x004), which is doulbed from CEC's setting. Therefore
     * we don't need to modify all the rest of the timings.
     * We will also need to modify some event registers.
     **********************************************************/
    WriteAVL(AVL_OFFSET_CKGEN, 0x0000001E, 0xFFFFFFFF); // clock: 3M/2/0x1E = 50khz
	WriteAVL(AVL_OFFSET_TR_CONFIG, 0x00001100, 0xFFFFFFFF); // Enable AV Link Mode Tx	
	WriteAVL(AVL_OFFSET_TX_EVENT, 0x0000001F, 0xFFFFFFFF); // Enable AV Link Mode Tx_EVENT Triggers
	/* check if hw tx fail */
	if ( (ReadAVL(AVL_OFFSET_TX_STATUS) & 0x7fff) == 0x10 )
	{
		WriteAVL(AVL_OFFSET_TR_CONFIG, 0x00000100, 0xFFFFFFFF);
		WriteAVL(AVL_OFFSET_TR_CONFIG, 0x00001100, 0xFFFFFFFF);
		_uAVLStatus |= AVLTXNOACK;
        AVLINK_DBG("Tx hw fail\n");
		return i4Ret;
	}
    /* check if hw is still transmitting */
	if( ReadAVL(AVL_OFFSET_TX_EVENT) & 0x10000 )
    {
        AVLINK_DBG("TX Buf still busy\n");
		return i4Ret;
    }

    /* destination addr */
	header |= ((UINT32)u1Dst << 16);
    /* mode */
    header |= (i4Mode&0x3);

    /* handle with different modes */
    switch (i4Mode)
    {
        case 1:
        {
            AVLINK_DBG("Please try m1tx cmd...\n");
            return i4Ret;
        } /* mode 1 */
        
        case 2:
        {
            if (i4Bytes)
            {
                /* command block follows */
                header |= AVLINK_CMD_ECT_BIT; //present cmd tbl
                data |= (UINT32)(i4Bytes & 0xff);
            }
            else
            {
                /* no command block.
                   EN50157-2-2 p.10,
                   This frame is used by "Initiator" to detect the 
                   presence of a destination device on the AV link.
                 */
                header |= AVLINK_HDR_EOM_BIT;
            }
            WriteAVL(AVL_OFFSET_TX_HD_NEXT, header, 0xFFFFFFFF);
            /* tx out */
            avlink_tx(data);

            AVLINK_DBG("TXed mode 2, with%s command block (0x%x)\n", 
                        i4Bytes?"":"out",
                        (UINT8)i4Bytes);

            i4Ret = 0;
            break;
        } /* mode 2 */
        
        case 3:
        {
            /* appid */
            header |= 0x300; /* appid= 011 */
            if (i4Bytes < 0 || i4Bytes > AVLINK_MAX_BYTES)
            {
                AVLINK_DBG("Invalid bytes:%d for tx, Max=%d\n", i4Bytes, AVLINK_MAX_BYTES);
                return i4Ret;
            }
            /* handle zero data-block */
            if (i4Bytes == 0)
            {
                header |= AVLINK_HDR_EOM_BIT;
                WriteAVL(AVL_OFFSET_TX_HD_NEXT, header, 0xFFFFFFFF);
                /* tx out */
                avlink_tx(data);
                i4Ret = 0;
                AVLINK_DBG("TXed frame mode 3, bytes:%d, dst:0x%x\n", i4Bytes, u1Dst);
                break;
            }
            i4BytesLeft = i4Bytes;
            /* first block */
            /* data mask */
            header |= (_u1AVLinkGetDataMask(i4Bytes) << 24);
            WriteAVL(AVL_OFFSET_TX_HD_NEXT, header, 0xFFFFFFFF);
            /* fill in data */
            while (i4BytesLeft)
            {
                data |= ((u1Data)<<((u1Data%4)*8));
                u1Data ++;
                i4BytesLeft --;
                /* 4 complete blocks */
                if (u1Data%4 == 0)
                {
                    if (i4BytesLeft == 0)
                    {
                        /* need to set EOM here */
                        header |= 0x2000;
                        WriteAVL(AVL_OFFSET_TX_HD_NEXT, header, 0xFFFFFFFF);
                    }
                    /* tx out */
                    avlink_tx(data);
                    /* wait until tx buf available */
                    while ( ReadAVL(AVL_OFFSET_TX_EVENT) & 0x10000 );
                    data = 0;
                }
            }
            /* if there are remaining bytes (ie. less than 4 bytes) */
            if (i4Bytes%4)
            {
                /* for size > 4, data mask needs to be updated */
                if (i4Bytes > 4)
                {
                    header &= 0xf0ffffff; //reset data mask bits
                    header |= (_u1AVLinkGetDataMask(i4Bytes%4) << 24);
                }
                header |= 0x2000; //EOM
                WriteAVL(AVL_OFFSET_TX_HD_NEXT, header, 0xFFFFFFFF);
                /* tx out */
                avlink_tx(data);
            }
            AVLINK_DBG("TXed frame mode 3, bytes:%d, dst:0x%x\n", i4Bytes, u1Dst);
            i4Ret = 0;

            break;
        } /* mode 3 */
        
        default:
            return i4Ret;
    }
    
    return i4Ret;
}

static void _vAVLinkIsr(UINT16 u2Vector)
{
//#if USE_PDWNC_INT_FUNC
    UINT32 u4RxMask = 0, u4TxMask = 0, u4Event = 0;
    avlink_data_t *ad = &avlink_data;

    UNUSED(u2Vector);
    /* RX Events */
    AVLINK_GET_CLR_RX_EVENT(u4RxMask);
    AVLINK_GET_CLR_TX_EVENT(u4TxMask);

    if (u4RxMask | u4TxMask)
    {
        /* disable all interrupts */
        //AVLINK_DISABLE_ALL_INT();
        ad->isr_rx_events = u4RxMask;
        /* trigger event for main task process */
        if (u4RxMask) u4Event |= AVLINK_EG_RX_EVENTS;
        if (u4TxMask) u4Event |= AVLINK_EG_TX_EVENTS;
        AVLINK_SET_EVENT(ad->ehdl, u4Event);
    }
//#else
    return ;
//#endif	
}

static void _vAVLinkShowM3Data(avlink_data_t *ad)
{
    INT32 i;

    for (i=0; i< ad->rx_bytes; i++)
    {
        Printf("Data[%d]: 0x%x\n", i, ad->u_rxdata.rx_buf[i]);
    }
}

static void _vAVLinkDumpM1QtyBits(UINT16 qty)
{
    Printf("QTY\\: 0x%04x\n", qty);
    Printf("  Bits 1~3: %d,%d,%d (RSVD)\n", qty&BIT15?1:0, qty&BIT14?1:0, qty&BIT13?1:0);    
    Printf("  Bits 4  : %d (%s)\n", qty&BIT12?1:0, qty&BIT12?"Y/C":"Non-Y/C");    
    Printf("  Bits 5  : %d (%s)\n", qty&BIT11?1:0, qty&BIT11?"RGB":"Non-RGB");    
    Printf("  Bits 6  : %d (%s)\n", qty&BIT10?1:0, qty&BIT10?"Wide screen":"Non-Wide screen");    
    Printf("  Bits 7~9: %d,%d,%d\n", qty&BIT9?1:0, qty&BIT8?1:0, qty&BIT7?1:0);
    Printf("  Bits 10 : %d (%s)\n", qty&BIT6?1:0,  qty&BIT6?"Src is VCR":"Src is not VCR");
    Printf("  Bits 11 : %d (%s)\n", qty&BIT5?1:0,  qty&BIT5?"Digital Audio Signal":"Non-Digital Audio Signal");
    Printf("  Bits 12 : %d (%s)\n", qty&BIT4?1:0,  qty&BIT4?"Audio only":"Non-Audio only");
    Printf("  Bits 13 : %d (%s)\n", qty&BIT3?1:0,  qty&BIT3?"Bi-lingual audio":"Non-Bi-lingual audio");
    Printf("  Bits 14~16:%d,%d,%d (RSVD)\n", qty&BIT2?1:0, qty&BIT1?1:0, qty&BIT0?1:0);    
}

static void _vAVLinkCompleteRx(avlink_data_t *ad)
{
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

}


static void _vAVLinkRx(avlink_data_t *ad)
{
    UINT32 u4rxHdrInfo = 0;
    UINT32 dlen = 0, data = 0;
    INT32 i;

    /* rx hdr information (receiving) */
    u4rxHdrInfo = ReadAVL(AVL_OFFSET_RX_HEADER);
    /* mode  */
    ad->rx_mode = AVLINK_GET_HDR_MODE(u4rxHdrInfo);
    ad->rx_src = AVLINK_GET_HDR_SRC_M(u4rxHdrInfo);
    ad->rx_dst = AVLINK_GET_HDR_DST_M(u4rxHdrInfo);
    
    if (ad->rx_mode == 1)
    {
        avlink_m1_bits_t *m1 = &ad->u_rxdata.m1;
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
        data = ReadAVL(AVL_OFFSET_RX_DATA);
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
            ad->u_rxdata.rx_buf[ad->rx_bytes++] = data & 0xff;
            data >>= 8;
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

static void _vAVLinkProcessIsr(avlink_data_t *ad)
{
    volatile UINT32 u4RxMask;
    //UINT32 u4TxMask;

    u4RxMask = ad->isr_rx_events;
    //u4TxMask = ad->isr_tx_events;
    
//process:
    /* check for error status first */
    if (AVLINK_IS_RX_ERR(u4RxMask))
    {
        AVLINK_DBG("RX ERR detected. ERROR bits in RX_EVENT:0x%x\n", 
                    u4RxMask&AVLINK_RX_EVET_ERR_MASKS);
        ad->rx_err_mask |= AVLINK_ERR_RX_OVERRUN;
    }
    if (AVLINK_IS_MODE_RDY(u4RxMask))
    {
        /* this int is used for verification only */
        ad->rx_mode_rdy_int ++;
    }
    /* if header is ready */
    if (AVLINK_IS_HDR_RDY(u4RxMask))
    {
        if (AVLINK_GET_HDR_DST(AVL_OFFSET_RX_HD_NEXT) == avlink_own_addr || //to me
            AVLINK_GET_HDR_DST(AVL_OFFSET_RX_HD_NEXT) == 0xf) //to all
        {
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
            /* not for me */
            AVLINK_CLR_STATUS_BIT(ad->rx_status, AVLINK_RX_HDR_READY);
        }
    }
    /* if rx buf is ready */
    if (AVLINK_IS_BR_RDY(u4RxMask))
    {
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
    return;

    
}



static void _vAVLinkReceiveTask(void *pvArgv)
{
	x_os_isr_fct pfnOldIsr;
    //INT32 i4Ret;
    UINT32 u4Event = 0;
    UINT32 u4DoExit = 0;
    avlink_data_t *ad;

    x_memset(&avlink_data, 0, sizeof(avlink_data));
    ad = &avlink_data;
    x_ev_group_create(&ad->ehdl, 
                      (const CHAR *)AVLINK_EVENT_NAME,
                      u4Event);
    //ASSERT(i4Ret == OSR_OK);
    /* register isr for avlink */
    VERIFY(PDWNC_RegIsr(PINTNO_CEC, _vAVLinkIsr, &pfnOldIsr) == PDWNC_OK);

    /* setting up RX registers */
    /* clock to 50khz (half of cec) */
    WriteAVL(AVL_OFFSET_CKGEN, 0x0000001E, 0xFFFFFFFF);
    /* reset TR_CONFIG all first */
    WriteAVL(AVL_OFFSET_TR_CONFIG, 0x00000000, 0xFFFFFFFF);
    /* tx/rx enable, mode:avlink, addr:0 */
    WriteAVL(AVL_OFFSET_TR_CONFIG, 0x00001100, 0xFFFFFFFF);
    /* rx event flag, enable all rx events */
    WriteAVL(AVL_OFFSET_RX_EVENT, 0x0000FFFF, 0xFFFFFFFF);
    
    ad->started = 1;
    do 
    {
        x_ev_group_wait_event(ad->ehdl,
                              AVLINK_EG_WAKEUP_MASK,
                              &u4Event,
                              X_EV_OP_OR_CONSUME);

        //ASSERT(i4Ret == OSR_OK);

        if (u4Event & AVLINK_EG_CTRL_EVENTS)
        {
            //todo
        }

        if ((u4Event & AVLINK_EG_RX_EVENTS) || (u4Event & AVLINK_EG_TX_EVENTS))
        {
            _vAVLinkProcessIsr(ad);
        }
        
    } while (!u4DoExit);

}


static INT32 _i4AVLinkSendCmd(INT32 i4Argc, const CHAR **szArgv)
{
    INT32 i4Mode, i4Bytes;
    UINT8 u1Dst;
    
    if (i4Argc != 4)
    {
        /* show help */
        Printf("Usage: tx dst mode bytes\n"
               "   dst: destination addr (0~0xf)\n"
               "   mode: 1/2/3\n"
               "   bytes: when mode2: cmd opcode (0=no cmd block)\n" 
               "          when mode3: data len\n");
        return 0;
    }
    
    u1Dst = (UINT8)StrToInt(szArgv[1]);
    i4Mode = StrToInt(szArgv[2]);
    i4Bytes = StrToInt(szArgv[3]);
    
    _i4AVLinkSend(i4Mode, i4Bytes, u1Dst);
    return 0;
}

static INT32 _i4AVLinkM1SendCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 byte0, byte1, byte2;
    
    if (i4Argc != 4)
    {
        /* show help */
        Printf("Usage: m1tx byte0 byte1 byte2\n"
               "   byte0: only lsb 4 bits are used: b3[DIR] b2[PAS] b1[NAS] b0[DES]\n"
               "   byte1: QTY bits8~1\n" 
               "   byte2: QTY bits16~9\n");
        return 0;
    }
    
    byte0 = StrToInt(szArgv[1])&0xf;
    byte1 = StrToInt(szArgv[2])&0xff;
    byte2 = StrToInt(szArgv[3])&0xff;

    Printf("b0:0x%x, b1:0x%x, b2:0x%x\n", byte0,byte1,byte2);
    _i4AVLinkM1Send(byte0, byte1, byte2);
    return 0;
}



static INT32 _i4AVLinkReceiveCmd(INT32 i4Argc, const CHAR **szArgv)
{
    //INT32 i4Ret;
	HANDLE_T  hThread;

    if (avlink_data.started)
    {
        Printf("AV.link already started\n");
        return 0;
    }
    Printf("Starting receiving thread for AV.link\n");
	x_thread_create(&hThread,
			"AVLINK_thread",
			2048,
			100,
			_vAVLinkReceiveTask,
			0,
			NULL);
    //ASSERT(i4Ret == OSR_OK);
    return 0;
}

static INT32 _i4AVLinkReadRegCmd(INT32 i4Argc, const CHAR **szArgv)
{
	UINT16 i;

	for(i=0; i<0xff; i+=4)
	{
		if((i&0x0f)==0)
		{
			Printf("\n%02x: ", i);
		}
		Printf("%08x ", ReadAVL(i));
	}
	return 0;
}


static INT32 _i4AVLinkWriteRegCmd(INT32 i4Argc, const CHAR **szArgv)
{
	UINT32 addr, data, mask;

	addr = StrToInt(szArgv[1]);
	data = (UINT32)StrToInt(szArgv[2]) << ((addr&0x03) << 3);
	mask = 0xff <<  ((addr&0x03) << 3);

    Printf("Writing AVL reg: addr:0x%x, data:0x%x, mask:0x%x\n", addr&0xFC, data, mask);
	WriteAVL(addr&0xFC, data, mask);
	return 0;
}

static INT32 _i4AVLinkPinMuxCmd(INT32 i4Argc, const CHAR **szArgv)
{
    Printf("Enable AVLink PinMux...\n");
    ENABLE_PINMUX_TO_AVLINK();
	return 0;
}


static INT32 _i4AVLinkInitCmd(INT32 i4Argc, const CHAR **szArgv)
{
    return i4AVL_init();
}

static INT32 _i4RegReadCmd(INT32 i4Argc, const CHAR **szArgv)
{
   UINT32 u4Addr,u4Data;

      if (i4Argc != 2)
      {
          Printf("Arg: u4Addr\n");
          return 0;
      }

      u4Addr = StrToInt(szArgv[1]);
      u4Data = IO_READ32(u4Addr, 0);
      Printf("0x%08x \n :", u4Data);
	  return 0;
}

static INT32 _i4RegWriteCmd(INT32 i4Argc, const CHAR **szArgv)
{
     UINT32 u4Addr,u4Data;

      if (i4Argc != 3)
      {
          Printf("Arg: u4Addr u4Data\n");
          return 0;
      }
      u4Addr = StrToInt(szArgv[1]);
      u4Data = StrToInt(szArgv[2]);
      IO_WRITE32(u4Addr, 0, u4Data);
	  return 0;
}

static INT32 _i4AVLinkOVStartCmd(INT32 i4Argc, const CHAR **szArgv)
{
    disableBrRdyClr = 1;
    return 0;
}

static INT32 _i4AVLinkOVEndCmd(INT32 i4Argc, const CHAR **szArgv)
{
    disableBrRdyClr = 0;
    return 0;
}

/******************************************************************************
* CLI Command Table
******************************************************************************/

// Table for AV.link command entry
CLI_EXEC_T arAVLinkCmdTbl[] = {
#ifdef CC_CLI
    {"AVLink read", "avlr",     _i4AVLinkReadRegCmd, NULL, "Read AVLink register", CLI_SUPERVISOR},
    {"AVLink write", "avlw",    _i4AVLinkWriteRegCmd, NULL, "Write AVLink register: addr data", CLI_SUPERVISOR},
    {"AVLink init", "i",        _i4AVLinkInitCmd, NULL, "Initialize AVLink registers", CLI_SUPERVISOR},
    {"AVLink Tx", "tx",         _i4AVLinkSendCmd, NULL, "Send AVLink command: header data0 data1....", CLI_SUPERVISOR},
    {"AVLink M1Tx", "m1tx",     _i4AVLinkM1SendCmd, NULL, "Send AVLink M1 cmd", CLI_SUPERVISOR},
    {"AVLink Rx", "rx",         _i4AVLinkReceiveCmd, NULL, "Receive AVLink command", CLI_SUPERVISOR},
    {"AVLink Pinmux set", "pm", _i4AVLinkPinMuxCmd, NULL, "AVLink Pinmux set", CLI_SUPERVISOR},
    {"Reg read", "r",           _i4RegReadCmd, NULL, "Register read command", CLI_SUPERVISOR},
    {"Reg read", "w",           _i4RegWriteCmd, NULL, "Register write command", CLI_SUPERVISOR},
    {"OV test start", "ovs",    _i4AVLinkOVStartCmd, NULL, "OV test start (disable clearing BR_RDY)", CLI_SUPERVISOR},
    {"OV test end", "ove",    _i4AVLinkOVEndCmd, NULL, "OV test start (enable clearing BR_RDY)", CLI_SUPERVISOR},
    //{"AVLink EnTXQ", "txq", _u1CECEnqueueTXQ, NULL, "CEC enqueue an tx mesg", CLI_SUPERVISOR},
    //{"AVLink Test", "test", _u1CECTestCommand, NULL, "CEC testing command", CLI_SUPERVISOR},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
#endif
};

 

