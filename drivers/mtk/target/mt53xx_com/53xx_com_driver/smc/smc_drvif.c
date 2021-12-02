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
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: smc_drvif.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file smc_drvif.c
 *  smc_drvif.c is the Interfaces of SmartCard.
 */

#include "x_os.h"
#include "x_bim.h"
#include "x_assert.h"
#include "x_printf.h"
#include "x_pinmux.h"
#include "x_ckgen.h"
#include "smc_debug.h"
#include "smc_hw.h"
#include "smc_drvif.h"
#include "smc_apdu.h"
#include "drvcust_if.h"

#ifdef CC_TRUSTZONE_SUPPORT
#include "tz_if.h"
#endif

/****************************************************************************
** Local definitions
****************************************************************************/
BOOL bTestAtrErr = FALSE;
BOOL bTestTxTide = FALSE;
BOOL bTestTxRxErr = FALSE;
BOOL bTestWTime = FALSE;

// #define ACCESS_SMC_REG_IN_TRUSTZONE
/****************************************************************************
** Local structures and enumerations
****************************************************************************/
#if defined(CC_MT5365) || defined(CC_MT5395)
#define SMC_FIFO_SIZE  512
#else
#define SMC_FIFO_SIZE  16
#endif

// #define SMC_PROFILING 
#ifdef SMC_PROFILING
#include "x_timer.h"
#endif


//#define VECTOR_SMARTCARD 27         // Smart Card Interrupt		//5391
/****************************************************************************
** Local function prototypes
****************************************************************************/
static void _SmcIsr(UINT16 u2Vector);
static void _SmcRegisterIsr(void);
static BOOL _SmcWaitMsg(UINT32 u4MsgType, UINT32 u4Timeout);
static INT32 _SmcSendMsg(UINT32 u4MsgType);
static INT32 _SmcEmuSendMsg(UINT32 u4MsgType);
static void _SmcRxHandler(UINT16 u2SimInt);
static void _SmcCmdHandler(void);
static void _SmcTxHandler(void);


/****************************************************************************
** Local variable
****************************************************************************/
static BOOL _bSmcInit = FALSE;
static HANDLE_T _hSmcMsgQ = NULL;

static HANDLE_T _hSmcOpMutex = NULL;

static SMART_CARD gtSMC;

static UINT16 _u2Tout = 0x983;
static UINT16 _u2CWT = 43;
static UINT16 _u2BWT = 15371;
static UINT8  _ucToutFactor=1;

static UINT8 _uResetCnt;
static UINT8 _aPtsData[4];

//static BOOL _bCheckPts = TRUE;

/*Maybe changed when the unit of the HW TOUT counter is changed!!*/
const UINT8  _aClkStopTimeTable[3][2]={{0,1}, {3,7}, {6,15}};

// ISO 7816-3 : Table 7
const INT16 FI_TABLE[16] = {372, 372, 558,  744, 1116, 1488, 1860, -1,
                             -1, 512, 768, 1024, 1536, 2048,   -1, -1};
// ISO 7816-3 : Table 8
const INT16 DI_TABLE[16] = { -1,   1,   2,    4,    8,   16,   32, -1,
                             12,  20,  -1,   -1,   -1,   -1,   -1, -1};
// ISO 7816-3 : Table 9
const INT16 I_TABLE[4]   = { 25,  50,   -1,   -1};

#ifdef SMC_PROFILING
static HAL_TIME_T _rActivateTime;
static HAL_TIME_T _rATR1stByteTime;
static HAL_TIME_T _rATRLastByteTime;
static HAL_TIME_T _rATRToutTime;
#endif

/****************************************************************************
** Exported definition
****************************************************************************/
HANDLE_T hSmcEmuMsgQ = NULL_HANDLE;
#ifdef CC_FPGA  
BOOL fgSmcFpgaEmuMode = FALSE;
#endif

/****************************************************************************
** Local functions
****************************************************************************/
static void _SmcSetPinmux(void)
{
#ifdef CC_MT5365
    if (DRVCUST_InitGet(eFlagDDRQfp))
    {
        // Pinmux for QFP
        UINT32 u4Val;
        u4Val = CKGEN_READ32(0x400);
        // pmux0[27:26] = 3
        u4Val |= 0x0C000000;
        CKGEN_WRITE32(0x400, u4Val);
    
        u4Val = CKGEN_READ32(0x414);
        // pmux5[28:26] = 4
        u4Val &= ~0x1C000000;
        u4Val |= 0x10000000;    
        CKGEN_WRITE32(0x414, u4Val);    
    }
    else
    {
        // Pinmux for BGA    
        UINT32 u4Val;
        u4Val = CKGEN_READ32(0x400);
        // pmux0[19:16] = 5
        u4Val &= ~0x000F0000;
        u4Val |= 0x00050000;    
        CKGEN_WRITE32(0x400, u4Val);    
    
        u4Val = CKGEN_READ32(0x404);
        // pmux1[23:20] = 5
        u4Val &= ~0x00F00000;
        u4Val |= 0x00500000;    
        CKGEN_WRITE32(0x404, u4Val);  
    
        u4Val = CKGEN_READ32(0x410);
        // pmux4[11:8] = 5; pmux4[23:20] = 5
        u4Val &= ~0x00F00F00;
        u4Val |= 0x00500500;    
        CKGEN_WRITE32(0x410, u4Val);
    
        u4Val = CKGEN_READ32(0x40C);
        // pmux3[2:0] = 5; pmux3[18:16] = 5
        u4Val &= ~0x00070007;
        u4Val |= 0x00050005;    
        CKGEN_WRITE32(0x40C, u4Val);       
    }
#endif
#ifdef CC_MT5395
    BSP_PinSet(PIN_SD_CMD, PINMUX_FUNCTION2);
#endif

#ifdef CC_MT5396
#if 1
    BSP_PinSet(PIN_SD_CMD, PINMUX_FUNCTION2);
    BSP_PinSet(PIN_SD_CLK, PINMUX_FUNCTION2);
	BSP_PinSet(PIN_SD_D0, PINMUX_FUNCTION2);
	BSP_PinSet(PIN_SD_D1, PINMUX_FUNCTION2);
	BSP_PinSet(PIN_SD_D2, PINMUX_FUNCTION2);
	BSP_PinSet(PIN_SD_D3, PINMUX_FUNCTION2);
#else
    BSP_PinSet(PIN_CI_TSDATA0, PINMUX_FUNCTION2);
    BSP_PinSet(PIN_PVR_TSCLK, PINMUX_FUNCTION2);
	BSP_PinSet(PIN_PVR_TSVAL, PINMUX_FUNCTION2);
	BSP_PinSet(PIN_PVR_TSSYNC, PINMUX_FUNCTION2);
	BSP_PinSet(PIN_PVR_TSDATA0, PINMUX_FUNCTION2);
	BSP_PinSet(PIN_PVR_TSDATA1, PINMUX_FUNCTION2);
#endif
#endif

#ifdef CC_MT5392B
     //ONDA_00_O funciton 1
    BSP_PinSet(ONDA_00_O, PINMUX_FUNCTION1);
#endif
}

static void _SmcRegisterIsr(void)
{
    x_os_isr_fct pfnOldIsr;

    VERIFY(x_reg_isr(VECTOR_SMARTCARD, _SmcIsr, &pfnOldIsr) == OSR_OK);
}


static void _SmcIsr(UINT16 u2Vector)
{
    UINT16 u2IntSta;

    if (u2Vector != VECTOR_SMARTCARD)
    {
        return;
    }

    //get interrupt status
    u2IntSta = SMC_READ16(REG_SMC_STA);
    SMC_INFO_PRINT("INT=0x%x, IRQ_ENABLE=0x%x\n", u2IntSta, SMC_READ16(REG_SMC_IRQEN));

    if(u2IntSta & STA_TXTIDE)
    {
        SMC_INFO_PRINT("INT: STA_TXTIDE\n");
        VERIFY(_SmcEmuSendMsg((UINT32)EMU_MSG_TXTIDE) == SMCR_OK);

        SmcDisableInt(IRQ_TXTIDE);
        _SmcTxHandler();
    }

    if(u2IntSta & STA_RXTIDE)
    {
        SMC_INFO_PRINT("INT: STA_RXTIDE\n");
        VERIFY(_SmcEmuSendMsg((UINT32)EMU_MSG_RXTIDE) == SMCR_OK);

        _SmcRxHandler(u2IntSta);
    }

    if(u2IntSta & STA_OVRUN)
    {
        SMC_INFO_PRINT("INT: STA_OVRUN\n");
        VERIFY(_SmcEmuSendMsg((UINT32)EMU_MSG_OVRUN) == SMCR_OK);

        gtSMC.bRcvErr = TRUE;
    }

    if(u2IntSta & STA_TOUT)
    {
        SMC_INFO_PRINT("INT: STA_TOUT\n");
        VERIFY(_SmcEmuSendMsg((UINT32)EMU_MSG_TOUT) == SMCR_OK);

        if(gtSMC.eState == STA_PROCESS_ATR)
        {
            _SmcRxHandler(u2IntSta);
        }

        if( gtSMC.eState == STA_PROCESS_PTS)
        {
            SmcSetResult(gtSMC,CMD_RES_PTS_ERR);
        }

        if (gtSMC.eState == STA_PROCESS_CMD)
        {
            if(gtSMC.bRcvErr == TRUE)
            {
                SmcSetResult(gtSMC,CMD_RES_RX_ERR);
            }
            else
            {
                switch(gtSMC.eCmdSta)
                {
                case CMD_STA_CHAR_RECV:
                    VERIFY(_SmcSendMsg((UINT32)MSG_CHAR_RECV_END) == SMCR_OK);
                    break;                	
                case CMD_STA_PROCESS_CLK:
                    SmcSetResult(gtSMC,CMD_RES_CLK_PROC);
                    break;
                case CMD_STA_STOP_CLK:
                    SmcIdle(gtSMC.bClkStopLvl);
                    break;
                default: /*normal command case*/
                    SmcSetResult(gtSMC,CMD_RES_TOUT);
                    break;
                }
            }
        }

        if (gtSMC.eState == STA_SERIOUS_ERR)
        {
            SmcSetResult(gtSMC,CMD_RES_TX_ERR);
        }
        SmcDisableAllInt();
    }

    if(u2IntSta & STA_TXERR)
    {
        SMC_INFO_PRINT("INT: STA_TXERR\n");
        VERIFY(_SmcEmuSendMsg((UINT32)EMU_MSG_TXERR) == SMCR_OK);

        if (gtSMC.eState == STA_PROCESS_CMD)
        {
            gtSMC.eState = STA_SERIOUS_ERR;
            SMC_WRITE16(REG_SMC_IRQEN, IRQ_TOUT);
        }
        else
        {
            SMC_INFO_PRINT("SMC reject\n");
            SmcReject();
        }
    }

    if(u2IntSta & STA_ATRERR)
    {
        SMC_INFO_PRINT("INT: STA_ATRERR\n");
        VERIFY(_SmcEmuSendMsg((UINT32)EMU_MSG_ATRERR) == SMCR_OK);

        SmcDisableAllInt();
        SmcReject();
        SmcSetResult(gtSMC, CMD_RES_NO_CARD);
        VERIFY(_SmcSendMsg((UINT32)MSG_ATR_END) == SMCR_OK);
    }

    if(u2IntSta & STA_SMCOFF)
    {
        SMC_INFO_PRINT("INT: STA_SMCOFF\n");
        VERIFY(_SmcEmuSendMsg((UINT32)EMU_MSG_SIMOFF) == SMCR_OK);

        VERIFY(_SmcSendMsg((UINT32)MSG_DEACT_END) == SMCR_OK);
    }

    if(u2IntSta & STA_T0END)
    {
        SMC_INFO_PRINT("INT: STA_T0END\n");
        VERIFY(_SmcEmuSendMsg((UINT32)EMU_MSG_T0END) == SMCR_OK);

        if(gtSMC.eState == STA_PROCESS_CMD)
        {
            _SmcCmdHandler();
            SmcDisableAllInt();
        }
    }

    if(u2IntSta & STA_RXERR)
    {
        SMC_INFO_PRINT("INT: STA_RXERR\n");
        VERIFY(_SmcEmuSendMsg((UINT32)EMU_MSG_RXERR) == SMCR_OK);

        gtSMC.bRcvErr = TRUE;
        SMC_WRITE16(REG_SMC_IRQEN, IRQ_TOUT);
    }

    if(u2IntSta & STA_T1END)
    {
        SMC_INFO_PRINT("INT: STA_T1END\n");
        VERIFY(_SmcEmuSendMsg((UINT32)EMU_MSG_T1END) == SMCR_OK);

        if(gtSMC.eCmdSta == CMD_STA_T1_SEND)
        {
            SMC_INFO_PRINT("MSG_T1_SEND_END\n");

            // Switch T=1 controller to receiving data
            SmcSetIns(0);
            VERIFY(_SmcSendMsg((UINT32)MSG_T1_SEND_END) == SMCR_OK);

            // In T1 mode, always change to rcv mode after send
            gtSMC.eCmdSta = CMD_STA_T1_RECV;
        }
        else if(gtSMC.eCmdSta == CMD_STA_T1_RECV)
        {
            SMC_INFO_PRINT("MSG_T1_RECV_END\n");

            _SmcCmdHandler();
        }
    }

    if(u2IntSta & STA_EDCERR)
    {
        SMC_INFO_PRINT("INT: STA_EDCERR\n");
        VERIFY(_SmcEmuSendMsg((UINT32)EMU_MSG_EDCERR) == SMCR_OK);

        gtSMC.bRcvErr = TRUE;
    }

    // Clear SMC interrupt
    VERIFY(BIM_ClearIrq(VECTOR_SMARTCARD));
}


static BOOL _SmcWaitMsg(UINT32 u4MsgWait, UINT32 u4WaitTime)
{
    UINT32 u4Msg;
    UINT16 u2MsgIdx = 0;
    SIZE_T zMsgSize = sizeof(u4Msg);

    while (TRUE)
    {
        if((x_msg_q_receive_timeout(&u2MsgIdx,
                            &u4Msg,
                            &zMsgSize,
                            &_hSmcMsgQ,
                            1,
                            u4WaitTime)) == OSR_OK)
        {
            SMC_DEBUG_PRINT("Got msg =0x%x\n", u4Msg);
            if (u4Msg == u4MsgWait)
            {
                break;
            }
            else
            {
                SMC_DEBUG_PRINT("Oh! Expect msg = 0x%x\n", u4MsgWait);                
            }
        }
        else
        {
            SMC_DEBUG_PRINT("Wait for message, Timeout!\n");
            return FALSE;
        }
    }

    return TRUE;
}


static INT32 _SmcSendMsg(UINT32 u4MsgType)
{ 
    VERIFY(x_msg_q_send(_hSmcMsgQ,
                        &u4MsgType,
                        sizeof(u4MsgType),
                        100) == OSR_OK);

    return SMCR_OK;
}


static INT32 _SmcEmuSendMsg(UINT32 u4MsgType)
{
#ifdef CC_FPGA   
    if(hSmcEmuMsgQ != NULL_HANDLE && fgSmcFpgaEmuMode)
    {
        VERIFY(x_msg_q_send(hSmcEmuMsgQ,
                            &u4MsgType,
                            sizeof(u4MsgType),
                            1) == OSR_OK);
    }
#endif
    return SMCR_OK;
}


static void _SmcTxHandler(void)
{
    CRIT_STATE_T rCrit;

    switch(gtSMC.eCmdSta)
    {
        case CMD_STA_CHAR_SEND:
    	     SMC_DEBUG_PRINT("_SmcTxHandler TXTIDE at %dth byte!!\n", gtSMC.u2TxIdx);         
//            VERIFY(_SmcSendMsg((UINT32)MSG_CHAR_SEND_TIDE) == SMCR_OK);
            break;
        default:
    	     SMC_ERROR_PRINT("_SmcTxHandler TXTIDE but received unknown message!\n");         	
            break;
    }
    
    while(1)
    {
        rCrit = x_crit_start();        
        if((gtSMC.u2TxIdx < gtSMC.u2TxSize) && (SMC_READ16(REG_SMC_COUNT) < SMC_FIFO_SIZE))
        {
            SMC_WRITE8(REG_SMC_DATA, *(gtSMC.pucTxBuf+gtSMC.u2TxIdx));
            gtSMC.u2TxIdx++;
            SMC_INFO_PRINT( "Send(%x)\n", (UINT32)*(gtSMC.pucTxBuf+gtSMC.u2TxIdx));                
        }
        else
        {
            // Notify end of block sending complete
            if(gtSMC.u2TxIdx >= gtSMC.u2TxSize)
            {
                VERIFY(_SmcSendMsg((UINT32)MSG_CHAR_SEND_END) == SMCR_OK);           
            }
            else
            {
                SmcSetTxTide(1);             
                SmcEnableInt(IRQ_TXTIDE);                               
            }
            break;
        }
        x_crit_end(rCrit);
        
        SMC_INFO_PRINT( "Send(%x)\n", (UINT32)*(gtSMC.pucTxBuf+gtSMC.u2TxIdx));        
        
    }    
}


static void _SmcRxHandler(UINT16 u2SimInt)
{
    UINT8   TS;
    UINT8   index;

    switch(gtSMC.eState)
    {
    case STA_WAIT_FOR_ATR:
        gtSMC.eState = STA_PROCESS_ATR;
        TS = SMC_READ8(REG_SMC_DATA);
        SMC_INFO_PRINT("TS = %02X\n", TS);

#ifdef SMC_PROFILING
        HAL_GetTime(&_rATR1stByteTime);
#endif

        if ((TS == 0x3F) || (TS == 0x3B))
        {
            SmcSetResult(gtSMC, CMD_RES_SUCCESS);
            SMC_WRITE16(REG_SMC_CNF, SMC_READ16(REG_SMC_CNF) |CNF_TXACK|CNF_RXACK|CNF_TOUT);

            SmcSetRxTide(12);
            SmcSetRxRetry(7);
            SmcSetTxRetry(7);
            gtSMC.u2RcvLen = 0;

            gtSMC.pucRcvBuf[gtSMC.u2RcvLen++] = TS;
            SMC_WRITE16(REG_SMC_IRQEN, IRQEN_Normal);
        }
        else
        {
            SMC_INFO_PRINT("ATR Err, TS = %02X\n", TS);
            _uResetCnt++;  //Change format
            if (_uResetCnt>1)
            {
                SMC_INFO_PRINT("Reset > 1\n");
                _uResetCnt = 0;
                SmcSetResult(gtSMC, CMD_RES_CARD_ERR);
            }
            else
            {
                SmcSetResult(gtSMC, CMD_RES_ATR_FMT_ERR);
            }

            // Error data format, Wait for other ATR bytes
            SMC_WRITE16(REG_SMC_CNF, SMC_READ16(REG_SMC_CNF) |CNF_TXACK|CNF_RXACK|CNF_TOUT);

            SmcSetRxTide(12);
            SmcSetRxRetry(7);
            SmcSetTxRetry(7);
            gtSMC.u2RcvLen = 0;
            SMC_WRITE16(REG_SMC_IRQEN, IRQ_TOUT|IRQ_RXTIDE);
        }
        break;

    case STA_PROCESS_ATR:
        while(1)
        {
            if (SmcGetFifoCnt())
            {
                if(gtSMC.u2RcvLen >= 256)
                {
                    SMC_ERROR_PRINT("gtSmc receive buffer overflow!\n");                
                    break;
                }
                gtSMC.pucRcvBuf[gtSMC.u2RcvLen] = (UINT8)SMC_READ8(REG_SMC_DATA);
                gtSMC.u2RcvLen++;
#ifdef SMC_PROFILING
                HAL_GetTime(&_rATRLastByteTime);
#endif                      
            }
            else
            {
                if (u2SimInt & STA_TOUT)
                {
#ifdef SMC_PROFILING
                    HAL_GetTime(&_rATRToutTime);
#endif                   
                    VERIFY(_SmcSendMsg((UINT32)MSG_ATR_END) == SMCR_OK);
                }
                break;
            }
        }
        break;

    case STA_PROCESS_PTS:
        index = 0;

        SMC_INFO_PRINT("PTS count = %x\n", SmcGetFifoCnt());

        while(index < 4)
        {
            _aPtsData[index++] = (UINT8)SMC_READ8(REG_SMC_DATA);

            if (SmcGetFifoCnt()==0)
            {
                VERIFY(_SmcSendMsg((UINT32)MSG_PTS_END) == SMCR_OK);
                break;
            }
        }

        SMC_INFO_PRINT("PTS=(%x,%x,%x)\n", _aPtsData[0], _aPtsData[1], _aPtsData[2]);

        SmcDisableAllInt();
        break;

    case STA_PROCESS_CMD:
        //Set CWT according to ATR result
        _u2Tout = _u2CWT;
        SmcSetTout(_u2Tout);    	
        if(SmcGetFifoCnt())
        {
            *(gtSMC.pucRxBuf+gtSMC.u2RcvLen) = (UINT8)SMC_READ8(REG_SMC_DATA);
            SMC_INFO_PRINT("Receive(%x)\n",*(gtSMC.pucRxBuf+gtSMC.u2RcvLen));
            gtSMC.u2RcvLen++;
        }
        else
        {
    	     SMC_ERROR_PRINT("_SmcRxHandler RXTIDE but no data!\n");          
        }
        break;

    case STA_SERIOUS_ERR:
        break;

    case STA_POWER_OFF:
        break;

    case STA_WAIT_REJECT_DONE:
        break;

    default:
        break;
    }
}

static void _SmcCmdHandler(void)
{
    while(SmcGetFifoCnt())
    {
        *(gtSMC.pucRxBuf+gtSMC.u2RcvLen) = (UINT8)SMC_READ8(REG_SMC_DATA);
        SMC_INFO_PRINT("Receive(%x)\n",*(gtSMC.pucRxBuf+gtSMC.u2RcvLen));
        gtSMC.u2RcvLen++;
    }

    if(gtSMC.eCmdSta == CMD_STA_T1_RECV)
    {
        VERIFY(_SmcSendMsg((UINT32)MSG_T1_RECV_END) == SMCR_OK);
    }
    else
    {
        VERIFY(_SmcSendMsg((UINT32)MSG_CMD_END) == SMCR_OK);
    }

    return;
}


static BOOL _SmcProcPps(const UINT8 *TxBuffaddr, UINT8 Txlength)
{
    UINT8    index;
    UINT16   tmp;

    ASSERT(TxBuffaddr != NULL);

    SmcFlushFifo();

    SMC_INFO_PRINT("_SmcProcPts, Txlength =0x%x\n", Txlength);
    Txlength = Txlength>4?4:Txlength;
    for(index=0; index<Txlength; index++)
    {
       _aPtsData[index]=0;
       tmp = *(TxBuffaddr+index);
       SMC_WRITE8(REG_SMC_DATA, tmp);
    }

    gtSMC.eState = STA_PROCESS_PTS;

    SmcSetRxTide(Txlength-1);
    SMC_WRITE16(REG_SMC_IRQEN,IRQEN_Normal);

    UNUSED(_SmcWaitMsg((UINT32)MSG_PTS_END, 5000));

    if ((gtSMC.bRcvErr == TRUE) || (gtSMC.eCmdRes == CMD_RES_PTS_ERR))
    {
        gtSMC.bRcvErr = FALSE;
        return FALSE;
    }

    for(index=0; index<Txlength; index++)
    {
        if (_aPtsData[index]!=*(TxBuffaddr+index))
        {
            return FALSE;
        }
    }

    gtSMC.eState = STA_PROCESS_CMD;

    SMC_INFO_PRINT("_SmcProcPts ok \n");

    return TRUE;
}

/****************************************************************************
** Global functions
****************************************************************************/
void SmcReject(void)
{
   SmcDisableAllInt();
   gtSMC.eState = STA_POWER_OFF;
   SMC_WRITE16(REG_SMC_IRQEN, IRQ_SMCOFF);
   SMC_WRITE16(REG_SMC_CON,SMC_READ16(REG_SMC_CON)& ~SMCON);
   SmcFlushFifo();
}


BOOL SmcResetNoATR(void)
{
    //Only enable SMC interrupt

    gtSMC.eState = STA_WAIT_FOR_ATR;
    _uResetCnt = 0;

    //Deactivate the SMC card
    SmcReset();

    UNUSED(_SmcWaitMsg((UINT32)MSG_RST_READY, 1000));

    if (gtSMC.eCmdRes == CMD_RES_SUCCESS)
    {
       return TRUE;
    }
    else
    {
       return FALSE;
    }
}

void SmcInitialize(SMC_DATA_FMT eDataFmt)
{
    UINT16 Conf;

    // Clock is 13/4 = 3.25 M
   // SMC_WRITE16(REG_SMC_BRR, (372<<2)|BRR_SMCCLK_DIV2);

    Conf = (eDataFmt == FMT_DIRECT) ? CNF_DIRECT : CNF_INDIRECT;

    if(bTestTxRxErr)
    {
        Conf ^= CNF_ODD;    // Invert this parity bit
    }

    Conf |= CNF_SMCSEL;
    SMC_INFO_PRINT("Config = 0x%x\n", Conf);
    SMC_WRITE16(REG_SMC_CNF, Conf);

    SmcSetRxRetry(1);
    SmcSetTxRetry(1);

    //Set the ATRTout as 9600etu
    SmcSetTout(_u2Tout);

    // reset interrupts, flush rx, tx fifo
    SmcFlushFifo();

    //Set the txfifo and rxfifo tide mark
    SmcSetRxTide(0);

    //Set activation time out duration
    if(bTestAtrErr)
    {
        SMC_WRITE16(REG_SMC_ATIME, 0x10);
    }
    else
    {
        SMC_WRITE16(REG_SMC_ATIME, 0xAFC7);
    }

    //Set charcter to character waiting time
    if(bTestWTime)
    {
        SMC_WRITE16(REG_SMC_WTIME, 0);
    }
    else
    {
        SMC_WRITE16(REG_SMC_WTIME, 0x983);
    }

    gtSMC.eState = STA_WAIT_FOR_ATR;
    gtSMC.bRcvErr = FALSE;

    //Enable Interrupt
    SMC_WRITE16(REG_SMC_IRQEN, IRQEN_Normal & ~IRQ_RXERR);

#ifdef SMC_PROFILING
    HAL_GetTime(&_rActivateTime);
#endif

    //activate the SMC card, and activate the SMCCLK
    SmcActive();

    SMC_INFO_PRINT("SMC Active\n");
}

/*-----  ISO7816-3: 6.6 8 ---*/
//#define UndefinedMode 0
#define  NegotiableMode 1
#define  SpecificMode  2
#define TYPE_MODE UINT16
BOOL SmcProcATR(void)
{
    UINT8 i, u1Idx = 0;

    // Refer to ISO 7816-3 for the variables naming
    UINT8 TS, T0, Y1, K, TA1, FI, DI, TB1, II, PI1, TC1, N=0, TD1, Y2, T;
    UINT8 TA2, TB2, PI2, TC2, TD2 = 0;
    INT32 Fi=372, Di=1, P=5, I=50, WI=10;  const INT32 Fd=372, Dd=1;
    UINT32 u4WTime;
    BOOL bFiDiValid = TRUE;
    BOOL bCanChangeMode = FALSE;
    BOOL bT[16];    // To record supported T
    TYPE_MODE ModeOfOperation = SpecificMode;

    UNUSED(TD2);

    // ATR data is invalid, reset and give up
    if (gtSMC.bRcvErr == TRUE)
    {
       gtSMC.bRcvErr = FALSE;
       return FALSE;
    }

    // Clear bT
    for(i=0; i<16; i++)
    {
        bT[i] = FALSE;
    }

    // TS
    TS = gtSMC.pucRcvBuf[u1Idx++];

    // T0
    T0 = gtSMC.pucRcvBuf[u1Idx++];
    Y1 = T0 & 0xF0;
    K  = T0 & 0x0F;

    // TA(1)
    if (Y1 & TA_MASK)
    {
        TA1 = gtSMC.pucRcvBuf[u1Idx++];
        FI = TA1 >> 4;
        DI = TA1 & 0x0F;

        SMC_INFO_PRINT("TA1 = 0x%x\n", TA1);

        if ((FI_TABLE[FI] == -1) || (DI_TABLE[DI] == -1))
        {
            SMC_INFO_PRINT("TA1 is invalid!\n");
            bFiDiValid = FALSE;
        }
        else
        {
            Fi = FI_TABLE[FI];
            Di = DI_TABLE[DI];
        }        
    }

    // TB(1)
    if (Y1 & TB_MASK)
    {
        TB1 = gtSMC.pucRcvBuf[u1Idx++];
        II  = (TB1 & (BIT5|BIT6)) >> 5;
        PI1 = TB1 & (BIT0|BIT1|BIT2|BIT3|BIT4);

        SMC_INFO_PRINT("TB1 = 0x%x\n",TB1);

        // Programming parameter I
        if(I_TABLE[II] != -1)
        {
            I = I_TABLE[II];
        }
        else
        {
            SMC_INFO_PRINT("II is invalid!\n");
        }

        // Program parameter P
        if(PI1==0)
        {
            // Use VCC instead of Vpp (6.5.4)
            // So ??
        }
        else if ((PI1 >= 5) && (PI1 <= 25))
        {
            P = PI1;
        }
        else
        {
            SMC_INFO_PRINT("PI1 is invalid!\n");
        }        
    }

    // TC(1)
    if (Y1 & TC_MASK)
    {
        TC1 = gtSMC.pucRcvBuf[u1Idx++];
        N   = TC1;
        SMC_INFO_PRINT("TC1 = 0x%x\n", TC1);
    }

    // TD(1)
    if (Y1 & TD_MASK)
    {
        TD1 = gtSMC.pucRcvBuf[u1Idx++];
        Y2  = TD1 & 0xF0;
        T   = TD1 & 0x0F;
        bT[T] = TRUE;   // Supported T

        SMC_INFO_PRINT( "TD1 = 0x%x\n",TD1);
        SMC_INFO_PRINT( "T = %d\n", T);

        if(Y2 & TA_MASK)
        {
            // In Specific Mode
            TA2 = gtSMC.pucRcvBuf[u1Idx++];

            SMC_INFO_PRINT( "TA2 = 0x%x\n", TA2);

            bCanChangeMode = ((TA2 & BIT7) == 0) ? TRUE : FALSE;
            bFiDiValid = ((TA2 & BIT4) == 0) ? TRUE : FALSE;

            T = TA2 & (BIT0|BIT1|BIT2|BIT3);

            SMC_INFO_PRINT( "T = %d\n", T);

            // We can trigger warm reset to go to negotiable mode (6.6.2)
            // Should we? When?
            //SmcWarmRst();
        }
        else
        {
            // In Negotiable Mode
            SMC_INFO_PRINT( "==> Negotiable Mode\n");            //TD1 exits , but TD1[TA_MASK] =0, TA2 not exist --> Negotiable Mode (6.6.1)
            ModeOfOperation = NegotiableMode;
        }

        if (Y2 & TB_MASK)
        {
            TB2 = gtSMC.pucRcvBuf[u1Idx++];

            SMC_INFO_PRINT( "TB2 = 0x%x\n", TB2);

            PI2 = TB2;
            if ((PI2 >= 50) && (PI2 <= 250))
            {
                P = PI2 / 10;   // PI2 is in decivolts
            }
        }

        if (Y2 & TC_MASK)
        {
            TC2 = gtSMC.pucRcvBuf[u1Idx++];

            SMC_INFO_PRINT( "TC2 = 0x%x\n", TC2);

            WI  = TC2;
        }

        if (Y2 & TD_MASK)
        {
            TD2 = gtSMC.pucRcvBuf[u1Idx++];

            SMC_INFO_PRINT( "TD2 = 0x%x\n", TD2);
        }
    }
    else
    {
        // TD(1) not exist ==> only support T=0 (7816-3: 6.4.3.1)
        bT[0] = TRUE;
        T = 0;
        SMC_INFO_PRINT( "T = %d\n", T);

         // In Negotiable Mode
        SMC_INFO_PRINT( "==> Negotiable Mode\n");            //TD1 not exits , surely TA2 will not exist --> Negotiable Mode (6.6.1)
        ModeOfOperation = NegotiableMode;
        Printf("<<<Negotiable Mode>>>\n");
    }

    // Final process
    if(bT[15] == FALSE)
    {
        P = 5;
        I = 50;
    }
    u4WTime = (UINT32)((960 * WI * Di) / 4);

if( ModeOfOperation != NegotiableMode)
{
#if 1
    if ((Fi / Di) > 511) // SIM_BRR can only handle till 511 ETU
    {
        if(!SmcSendPps(T, 0x11))
        {
            Printf("Fail to send PPS!\n");
            return FALSE;
        }
    }
    else
    {
        SmcSetEtu(Fi/Di);
    }
#else
    if(!SmcSendPps(T, 0x11))    // Force to F=312, D=12
    {
        Printf("Fail to send PPS!\n");
        return FALSE;
    }
#endif
}
else            //NegotiableMode
{
#if 1
        SmcSetEtu(Fd/Dd);   //// Force to Fd=372, Dd=1
        Printf("<<<Force to Set Fd/Dd>>>\n");
#else
///// only in NegotiableMode,can sendpps?
		 if(!SmcSendPps(T, 0x11))    // Force to F=312, D=12
    {
        SMC_INFO_PRINT("Fail to send PPS!\n");
        return FALSE;
    }
	 SMC_INFO_PRINT("sucess to send PPS!\n");
#endif
		
}

    gtSMC.eState = STA_PROCESS_CMD;

// Not support T=0 control
//    SmcEnableT0Ctrl();

    UNUSED(P);
    UNUSED(u4WTime);
    UNUSED(bCanChangeMode);
    UNUSED(bFiDiValid);
    UNUSED(TS);
    UNUSED(I);
    UNUSED(K);
    UNUSED(N);

    return TRUE;
}


void SmcInitDrv(void)
{
    if(!_bSmcInit)
    {
        _SmcSetPinmux();

        _SmcRegisterIsr();
        VERIFY(x_msg_q_create(&_hSmcMsgQ, "SmcDrvCmdQ", sizeof(UINT32), 1024) == OSR_OK);

        // Emulation
        VERIFY(x_msg_q_create(&hSmcEmuMsgQ, "SmcEmuMsgQ", sizeof(UINT32), 1024) == OSR_OK);

        VERIFY(x_sema_create(&_hSmcOpMutex, X_SEMA_TYPE_MUTEX, X_SEMA_STATE_UNLOCK) == OSR_OK);
    }

    _bSmcInit = TRUE;
}


UINT16 SmcT0Cmd(UINT8 *txData,
                UINT16 txSize,
                UINT8 *pucRxBuf,
                UINT16 *rcvSize,
                UINT8 *Error)
{
    UINT16 SW, INS, expSize;
    INT32 i;

    ASSERT((txData != NULL) && (pucRxBuf != NULL) && (rcvSize != NULL) && (Error != NULL));

    expSize = *rcvSize;

/*
    SmcDisableAllInt();

    if (gtSMC.bClkStop == TRUE)
    {
        SmcActiveClk();
    }

    // What is this for?
    if ((gtSMC.eSpeed != SPEED_372) &&
        ((SMC_READ16(REG_SMC_CON)) & CSTOP))
    {
        SmcSetTout(_aClkStopTimeTable[gtSMC.eSpeed][0]*_ucToutFactor);
        gtSMC.eCmdSta = CMD_STA_PROCESS_CLK;
        SMC_WRITE16(REG_SMC_IRQEN,IRQ_TOUT);
        VERIFY(_SmcWaitMsg((UINT32)MSG_CLK_PROC));
    }
*/

    SmcSetTout(_u2Tout);
    SmcFlushFifo();
    *Error = FALSE;

    gtSMC.pucTxBuf = txData;
    gtSMC.u2TxSize = txSize;
    gtSMC.pucRxBuf = pucRxBuf;
    gtSMC.u2RcvLen = 0;

    //open T0 controller
    SMC_WRITE16(REG_SMC_CNF, SMC_READ16(REG_SMC_CNF) & ~CNF_T1EN);
    SMC_WRITE16(REG_SMC_CNF, SMC_READ16(REG_SMC_CNF) | CNF_T0EN);

    if(txSize==5)   // P3 => Length of data expected for response
    {
        SmcSetTxTide(1);
        SmcSetRxTide( (expSize>10)? 10 : 1 );

        SMC_WRITE16(REG_SMC_IRQEN, IRQEN_CMD_T0_Normal);

        for (i=0;i<5;i++)
        {
            SMC_WRITE8(REG_SMC_DATA, txData[i]);
        }
    }
    else    // P3 => Length of data for command
    {
        SmcSetTxTide(1);
        SmcSetRxTide(0);

        SMC_WRITE16(REG_SMC_IRQEN, IRQEN_CMD_T0_Normal);
        SmcEnableInt((IRQ_TXTIDE | IRQ_TOUT));

        for(i=0;i<7;i++)
        {
            SMC_WRITE8(REG_SMC_DATA, txData[i]);
        }
    }

    SmcSetP3(*(txData+4));
    INS = (UINT16)*(txData+1);
    gtSMC.eCmdSta = CMD_STA_WAIT_END;

    if (txSize>5)
    {
        INS |= INSD; // T=0 controller send data
    }

    // Trigger T=0 controller and start to send data
    SmcSetIns(INS);

    UNUSED(_SmcWaitMsg((UINT32)MSG_CMD_END, 3000));

    *rcvSize = gtSMC.u2RcvLen;
    SMC_INFO_PRINT( "rcvSize = 0x%x\n",*rcvSize);

    if (gtSMC.eCmdRes == CMD_RES_SUCCESS)
    {
        SW = SmcReadSw();
        SMC_INFO_PRINT( "SW=%x\n",SW);
        if(gtSMC.bClkStop == TRUE)
        {
           SmcDisableAllInt();

           SmcSetTout(_aClkStopTimeTable[gtSMC.eSpeed][1]*_ucToutFactor);
           gtSMC.eCmdSta = CMD_STA_STOP_CLK;
           SMC_WRITE16(REG_SMC_IRQEN,IRQ_TOUT);
        }
        return SW;
    }
    else
    {
        gtSMC.bRcvErr = FALSE;
        *Error = TRUE;
        return 0;
    }
}


UINT16 SmcT1Cmd(UINT8 *txData,
               UINT16 txSize,
               UINT8 *pucRxBuf,
               UINT16 *rcvSize,
               UINT8 *Error)
{
    INT32 i;
    UINT16 u2INS, SW;

    ASSERT((txData != NULL) && (pucRxBuf != NULL) && (rcvSize != NULL) && (Error != NULL));

    gtSMC.pucTxBuf = txData;
    gtSMC.u2TxSize = txSize;
    gtSMC.pucRxBuf = pucRxBuf;
    gtSMC.u2RcvLen = 0;
    gtSMC.u2TxIdx = 0;

    // Set interrupt types
    SMC_WRITE16(REG_SMC_IRQEN, IRQEN_CMD_T1_Normal);

    if(bTestTxTide)
    {
        SmcEnableInt(IRQ_TXTIDE);
    }

    // Turn on T=1 hardware protolcol controller
    SMC_WRITE16(REG_SMC_CNF, SMC_READ16(REG_SMC_CNF) & ~CNF_T0EN);
    SMC_WRITE16(REG_SMC_CNF, SMC_READ16(REG_SMC_CNF) | CNF_T1EN);

    // Clear Fifo
    SmcFlushFifo();

    // Write data into fifo
    for(i=0; i<txSize; i++)
    {
        SMC_WRITE8(REG_SMC_DATA,  txData[i]);
        gtSMC.u2TxIdx ++;
    }

    // Set transmission size
    SmcSetP3(*(txData+2));

    SmcSetRxTide(10);

    gtSMC.eCmdSta = CMD_STA_T1_SEND;

    u2INS = txSize|INSD;

    // trigger data transmit
    SMC_WRITE16(REG_SMC_INS, u2INS);

    UNUSED(_SmcWaitMsg((UINT32)MSG_T1_SEND_END, 3000));

    //VERIFY(_SmcWaitMsg((UINT32)MSG_T1_RECV_END));
    UNUSED(_SmcWaitMsg((UINT32)MSG_T1_RECV_END, 3000));

    *rcvSize = gtSMC.u2RcvLen ;

    *Error = gtSMC.bRcvErr;
#if 0
    if (*Error == TRUE)
    {
        return SMCR_FAIL;
    }
    else
    {
        return SMCR_OK;
    }
#else
    SW = SmcReadSw();
    return SW;
#endif
}

UINT16 _SMC_SendBlock(UINT8 *txData,
               UINT32 txSize,
               UINT8 *Error)
{
    UINT16 u2INS;

    CRIT_STATE_T rCrit;

    ASSERT((txData != NULL) && (Error != NULL));

    // Use S/W to control T=1 protocol
    SMC_WRITE16(REG_SMC_CNF, SMC_READ16(REG_SMC_CNF) & ~(CNF_T0EN | CNF_T1EN));

    // Set BGT to 22 ETU
    SMC_WRITE16(REG_SMC_GTIME, 22);

    //Set BWT according to ATR result
    _u2Tout = _u2BWT;
    SmcSetTout(_u2Tout);
    
    gtSMC.pucTxBuf = txData;
    gtSMC.u2TxSize = (UINT16)txSize;
    gtSMC.u2TxIdx = 0;

    gtSMC.eState = STA_PROCESS_CMD;   
    gtSMC.eCmdSta = CMD_STA_CHAR_SEND;
    
    // Set interrupt types
    SMC_WRITE16(REG_SMC_IRQEN, IRQEN_Normal);

    u2INS = SMC_READ16(REG_SMC_CNF);
    SMC_WRITE16(REG_SMC_CNF, u2INS & ~(CNF_TXACK | CNF_RXACK));

    // Clear Fifo
    SmcFlushFifo();

    SmcSetTxTide(0); 
    while((gtSMC.u2TxIdx < gtSMC.u2TxSize) && (SMC_READ16(REG_SMC_COUNT) < SMC_FIFO_SIZE))
    {
        SMC_WRITE8(REG_SMC_DATA, *(gtSMC.pucTxBuf+gtSMC.u2TxIdx));
        gtSMC.u2TxIdx++;
        SMC_INFO_PRINT( "Send(%x)\n", (UINT32)*(gtSMC.pucTxBuf+gtSMC.u2TxIdx));           
    }      
    rCrit = x_crit_start();
    if(gtSMC.u2TxIdx < gtSMC.u2TxSize)
    {
        SmcSetTxTide(1);     
        SmcEnableInt(IRQ_TXTIDE);     
        x_crit_end(rCrit);    
        // wait until last CHAR_SEND interrupt unlock sema
        UNUSED(_SmcWaitMsg((UINT32)MSG_CHAR_SEND_END, 1000));           
    } 
    else 
    {
        x_crit_end(rCrit);    
    }    

    SMC_INFO_PRINT( "Send end at %d!!\n", gtSMC.u2TxIdx);      
    
    *Error = 0;

    return 0;
}

UINT16 _SMC_ReceiveBlock(UINT8 *pucRxBuf,
               UINT32 *rcvSize,
               UINT8 *Error)
{
    ASSERT((pucRxBuf != NULL) && (rcvSize != NULL) && (Error != NULL));

    //Set BWT according to ATR result
    _u2Tout = _u2BWT;
    SmcSetTout(_u2Tout);
    
    gtSMC.pucRxBuf = pucRxBuf;
    gtSMC.u2RcvLen = 0;
    gtSMC.u2TxIdx = 0;

    gtSMC.eState = STA_PROCESS_CMD;   
    gtSMC.eCmdSta = CMD_STA_CHAR_RECV;
    SmcSetRxTide(0);
    
    UNUSED(_SmcWaitMsg((UINT32)MSG_CHAR_RECV_END, 1000));

    *rcvSize = (UINT32)gtSMC.u2RcvLen ;

    *Error = gtSMC.bRcvErr;

    return 0;
}

void _SMC_SetBWT(UINT16 u2BWT)
{
    _u2BWT = u2BWT;
}

void _SMC_SetCWT(UINT16 u2CWT)
{
    _u2CWT = u2CWT;
}

// The function requests the driver to deactivate SMC
static void SmcPowerOff(void)  //Validate
{
   SmcDisableAllInt();
   //DRVPDN_Disable(DRVPDN_CON1,DRVPDN_CON1_SMC,PDN_SMC); /*FIXME*/
   SmcReject();
   //DRVPDN_Enable(DRVPDN_CON1,DRVPDN_CON1_SMC,PDN_SMC);
}

// The function is used to reset SMC
INT32 SmcResetCard(UINT8 *pucATR, UINT32 u4BufLen, UINT32 *u4ATRLen)
{
    UINT32 u4Idx, u4Len;

    ASSERT((pucATR != NULL) && (u4ATRLen != NULL));

    *u4ATRLen=0;        //5391

    SmcDisableAllInt();

    _uResetCnt = 0;
    _u2Tout = 0x983;

    gtSMC.bClkStop = FALSE;
    gtSMC.eSpeed = SPEED_372;
    gtSMC.eState = STA_WAIT_FOR_ATR;

    // B-CAS use direct mode by default
    gtSMC.eDataFmt = FMT_DIRECT;
    gtSMC.eCmdRes = CMD_RES_NOT_READY;

    SmcReset();
    UNUSED(_SmcWaitMsg((UINT32)MSG_DEACT_END, 1000));

    SmcInitialize(gtSMC.eDataFmt);
    UNUSED(_SmcWaitMsg((UINT32)MSG_ATR_END, 12000));

    switch(gtSMC.eCmdRes)
    {
    case CMD_RES_NOT_READY:
        return SMCR_NO_CARD;

    case CMD_RES_CARD_ERR:
        SmcReset();
        UNUSED(_SmcWaitMsg((UINT32)MSG_DEACT_END, 1000));

        // Change power format and try again
        SmcInitialize(gtSMC.eDataFmt);
        UNUSED(_SmcWaitMsg((UINT32)MSG_ATR_END, 12000));

        if (gtSMC.eCmdRes == CMD_RES_CARD_ERR)
        {
            return SMCR_CARD_ERROR;
        }
        break;
    case CMD_RES_ATR_FMT_ERR:
        // Change ATR data fomat
        if (gtSMC.eDataFmt == FMT_DIRECT)
        {
            gtSMC.eDataFmt = FMT_INDIRECT;
        }
        else
        {
            gtSMC.eDataFmt = FMT_DIRECT;
        }

        SmcReset();
        UNUSED(_SmcWaitMsg((UINT32)MSG_DEACT_END, 1000));

        // Change power format and try again
        SmcInitialize(gtSMC.eDataFmt);
        UNUSED(_SmcWaitMsg((UINT32)MSG_ATR_END, 12000));
        //lint -fallthrough
    case CMD_RES_SUCCESS:
        u4Idx=0;
        while(1)
        {
            if (!SmcProcATR())
            {
                u4Idx++;
                if (u4Idx ==2)
                {
                    //_bCheckPts = FALSE;
                }
                else if (u4Idx > 2)
                {
                    //_bCheckPts = FALSE;
                    SmcPowerOff();
                    return SMCR_CARD_ERROR;
                }
                UNUSED(SmcResetNoATR());
            }
            else
            {
                if (pucATR != NULL)
                {
                    u4Len = (gtSMC.u2RcvLen > u4BufLen) ? u4BufLen : gtSMC.u2RcvLen;
                    for (u4Idx = 0; u4Idx < u4Len; u4Idx ++)
                    {
                        pucATR[u4Idx] = gtSMC.pucRcvBuf[u4Idx];
                    }
                    *u4ATRLen = u4Idx;
                }
                return SMCR_OK;
            }
        }
    default:
        SmcPowerOff();
        return SMCR_CARD_ERROR;
    }

    return SMCR_OK;
}

// The function is used to reset SMC
INT32 SmcDeactCard(void)
{
    SmcDisableAllInt();

    SmcReset();
    UNUSED(_SmcWaitMsg((UINT32)MSG_DEACT_END, 1000));

    return SMCR_OK;
}

// The function is used to reset SMC
INT32 SmcActCard(UINT8 *pucATR, UINT32 u4BufLen, UINT32 *u4ATRLen)
{
    UINT32 u4Idx, u4Len;

    ASSERT((pucATR != NULL) && (u4ATRLen != NULL));

    *u4ATRLen=0;        //5391

    // Reset ns counter
    _SMC_ResetNs();

//    SmcDisableAllInt();

    _uResetCnt = 0;
    _u2Tout = 0x983;

    gtSMC.bClkStop = FALSE;
    gtSMC.eSpeed = SPEED_372;
    gtSMC.eState = STA_WAIT_FOR_ATR;
    // B-CAS use direct mode    
    gtSMC.eDataFmt = FMT_DIRECT;
    gtSMC.eCmdRes = CMD_RES_NOT_READY;

//    SmcReset();
//    UNUSED(_SmcWaitMsg((UINT32)MSG_DEACT_END));

    SmcInitialize(gtSMC.eDataFmt);
    UNUSED(_SmcWaitMsg((UINT32)MSG_ATR_END, 1500));

#ifdef SMC_PROFILING
    {
        HAL_TIME_T rDeltaTime;
        HAL_GetDeltaTime(&rDeltaTime, &_rATR1stByteTime, &_rATRToutTime);        
        Printf("ATR Tout Interval: %d sec %d micro\n", rDeltaTime.u4Seconds, rDeltaTime.u4Micros);
        HAL_GetDeltaTime(&rDeltaTime, &_rATR1stByteTime, &_rATRLastByteTime);                
        Printf("ATR 1st-Last Byte Interval: %d sec %d micro\n", rDeltaTime.u4Seconds, rDeltaTime.u4Micros);        
    }
#endif

    switch(gtSMC.eCmdRes)
    {
    case CMD_RES_NOT_READY:
        return SMCR_NO_CARD;

    case CMD_RES_CARD_ERR:
        SmcReset();
        UNUSED(_SmcWaitMsg((UINT32)MSG_DEACT_END, 1000));

        // Change power format and try again
        SmcInitialize(gtSMC.eDataFmt);
        UNUSED(_SmcWaitMsg((UINT32)MSG_ATR_END, 1500));

        if (gtSMC.eCmdRes == CMD_RES_CARD_ERR)
        {
            return SMCR_CARD_ERROR;
        }
        break;
    case CMD_RES_ATR_FMT_ERR:
        // Change ATR data fomat
        if (gtSMC.eDataFmt == FMT_DIRECT)
        {
            gtSMC.eDataFmt = FMT_INDIRECT;
        }
        else
        {
            gtSMC.eDataFmt = FMT_DIRECT;
        }

        SmcReset();
        UNUSED(_SmcWaitMsg((UINT32)MSG_DEACT_END, 1000));

        // Change power format and try again
        SmcInitialize(gtSMC.eDataFmt);
        UNUSED(_SmcWaitMsg((UINT32)MSG_ATR_END, 1500));
        //lint -fallthrough
    case CMD_RES_SUCCESS:
        u4Idx=0;
        while(1)
        {
            if (!SmcProcATR())
            {
                u4Idx++;
                if (u4Idx ==2)
                {
                    //_bCheckPts = FALSE;
                }
                else if (u4Idx > 2)
                {
                    //_bCheckPts = FALSE;
                    SmcPowerOff();
                    return SMCR_CARD_ERROR;
                }
                UNUSED(SmcResetNoATR());
            }
            else
            {
                if (pucATR != NULL)
                {
                    u4Len = (gtSMC.u2RcvLen > u4BufLen) ? u4BufLen : gtSMC.u2RcvLen;
                    for (u4Idx = 0; u4Idx < u4Len; u4Idx ++)
                    {
                        pucATR[u4Idx] = gtSMC.pucRcvBuf[u4Idx];
                    }
                    *u4ATRLen = u4Idx;
                }                
                return SMCR_OK;
            }
        }
    default:
        SmcPowerOff();
        return SMCR_CARD_ERROR;
    }

    return SMCR_OK;
}

BOOL SmcResetModule(void)
{
    UINT8   pucATR[ATR_BUF_LEN];
    UINT32  i, u4ATRLen;
    for(i = 0; i < ATR_BUF_LEN; i++)
    {
        pucATR[i] = 0;
    }
    UNUSED(SmcResetCard(pucATR, ATR_BUF_LEN, &u4ATRLen));
    u4ATRLen = u4ATRLen>256?256:u4ATRLen;
    Printf("ATR = ");
    for(i=0; i<u4ATRLen; i++)
    {
        Printf("%02X ", pucATR[i]);
    }
    Printf("\n");

    return TRUE;
}


BOOL SmcSendPps(UINT8 u1T, UINT8 u1FD)
{
    UINT8 ucPps[6];

    ucPps[0] = 0xFF;
    ucPps[1] = 0x10 | u1T;  // Only PPS1
    ucPps[2] = u1FD;
    ucPps[3] = ucPps[0] ^ ucPps[1] ^ ucPps[2];
    return _SmcProcPps(ucPps,4);
}

INT32  _SMC_SendMsg(UINT32 u4MsgType)
{
   return _SmcSendMsg(u4MsgType);
}

INT32 _SMC_Reactivate (void)
{
    static UINT8   pucATR[256];
    UINT32  i, u4ATRLen;
    INT32 i4Ret = SMCR_OK;
    
    SmcDeactCard();
    
    i4Ret = SmcActCard(pucATR, 256, &u4ATRLen);

    SMC_DEBUG_PRINT("ATR = ");
    for(i=0; i<u4ATRLen; i++)
    {
        SMC_DEBUG_PRINT( "%02X ", pucATR[i]);
    }
    SMC_DEBUG_PRINT( "\n");
    
    return i4Ret;
}

INT32 _SMC_Op_Lock(void)
{
    return x_sema_lock(_hSmcOpMutex, X_SEMA_OPTION_WAIT);
}

INT32 _SMC_Op_Unlock(void)
{
    return x_sema_unlock(_hSmcOpMutex);
}

#if 1  //debug		//5391
#define LOG_RW  1
UINT8 SMC_READ8(UINT32 addr)
{
   UINT8 u1var;
#if defined(CC_TRUSTZONE_SUPPORT) && defined(ACCESS_SMC_REG_IN_TRUSTZONE)
    TZ_IO_READ8(SMART_CARD_BASE, (addr), &u1var);
#else
   u1var=IO_READ8(SMART_CARD_BASE, (addr));
#endif

#ifdef LOG_RW
   SMC_INFO_PRINT("SMC_READ8(%xh)=0x%02X\n",addr,u1var);
#endif
   return u1var;
}
UINT16 SMC_READ16(UINT32 addr)
{
   UINT16 u2var;
#if defined(CC_TRUSTZONE_SUPPORT) && defined(ACCESS_SMC_REG_IN_TRUSTZONE)
    TZ_IO_READ16(SMART_CARD_BASE, (addr), &u2var);
#else
   u2var=IO_READ16(SMART_CARD_BASE, (addr));
#endif

#ifdef LOG_RW
    SMC_INFO_PRINT("SMC_READ16(%xh)=0x%04X\n",addr,u2var);
#endif
   return u2var;
}
void SMC_WRITE8(UINT32 addr, UINT8 val)
{
#ifdef LOG_RW
    SMC_INFO_PRINT("SMC_WRITE8(%xh)=0x%02X\n",addr,val);
#endif
#if defined(CC_TRUSTZONE_SUPPORT) && defined(ACCESS_SMC_REG_IN_TRUSTZONE)
    TZ_IO_WRITE8(SMART_CARD_BASE, (addr), val);
#else
    IO_WRITE8(SMART_CARD_BASE, (addr), (val));
#endif
}
void SMC_WRITE16(UINT32 addr, UINT16 val)
{
#ifdef LOG_RW
    SMC_INFO_PRINT("SMC_WRITE16(%xh)=0x%04X\n",addr,val);
#endif
#if defined(CC_TRUSTZONE_SUPPORT) && defined(ACCESS_SMC_REG_IN_TRUSTZONE)
    TZ_IO_WRITE16(SMART_CARD_BASE, (addr), val);
#else
    IO_WRITE16(SMART_CARD_BASE, (addr), (val));
#endif
}
#endif
