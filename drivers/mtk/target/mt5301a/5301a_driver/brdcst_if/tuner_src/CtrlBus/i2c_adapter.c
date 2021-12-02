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
 * $RCSfile: i2c_adapter.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file i2c_adapter.c
 *  Implement underlying I2C control bus interface.
 */

#include "ctrl_bus.h"
#include "sif_if.h"
#include "PI_Def_dbg.h"

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#ifndef cMAX_I2C_LEN
#define cMAX_I2C_LEN            8
#endif

#define  DEMO_REG_PAGE_NUM  0xFF
#define cI2C_TIMING             270

#define cMAX_HC_WAIT_LOOP       6
#define cHC_WAIT_TIME_INTERVAL  10 // ms

#define fcRISC_DIRECT_RW        1

#define DEMOD_REG_uP_CMDR_CFG  (UINT16) 0x07
#define DEMOD_REG_RISC_WDATA   (UINT16) 0x4B0
#define DEMOD_REG_RISC_CTRL    (UINT16) 0x4B4
#define DEMOD_REG_RISC_RDATA   (UINT16) 0x4B8
#define DEMOD_REG_RISC_STAT    (UINT16) 0x4BC


#define cpRISC_CTRL_DL_EN       (7 + (3 * 8))
#define cpRISC_CTRL_WR_EN       (6 + (3 * 8))
#define cpRISC_CTRL_RD_EN       (5 + (3 * 8))

#define cpRISC_CTRL_B3_EN       (3 + (2 * 8))
#define cpRISC_CTRL_B2_EN       (2 + (2 * 8))
#define cpRISC_CTRL_B1_EN       (1 + (2 * 8))
#define cpRISC_CTRL_B0_EN       (0 + (2 * 8))


/*--------- RISC_STAT 0x4BC -------------------------------------------*/
#define cwRISC_STAT_WSTAT       2
#define cpRISC_STAT_WSTAT       0
#define cmRISC_STAT_WSTAT       mcSET_MASKS(RISC_STAT_WSTAT)

#define cpRISC_STAT_WSTAT_RDY   (0 + cpRISC_STAT_WSTAT)
#define cpRISC_STAT_WSTAT_ERR   (1 + cpRISC_STAT_WSTAT)
#define cRISC_STAT_WSTAT_RDY    0
#define cRISC_STAT_WSTAT_BUSY   1
#define cRISC_STAT_WSTAT_ERR    2

#define cwRISC_STAT_RSTAT       2
#define cpRISC_STAT_RSTAT       2
#define cmRISC_STAT_RSTAT       mcSET_MASKS(RISC_STAT_RSTAT)

#define cpRISC_STAT_RSTAT_RDY   (0 + cpRISC_STAT_RSTAT)
#define cpRISC_STAT_RSTAT_ATTN  (1 + cpRISC_STAT_RSTAT)
#define cRISC_STAT_RSTAT_RDY    0
#define cRISC_STAT_RSTAT_BUSY   1
#define cRISC_STAT_RSTAT_ATTN   2
#define cRISC_STAT_RSTAT_ERR    3

#define cwRISC_STAT_DSTAT       1
#define cpRISC_STAT_DSTAT       4
#define cmRISC_STAT_DSTAT       mcSET_MASKS(RISC_STAT_DSTAT)

#define cpRISC_STAT_CMD_VAL     6

#define cpRISC_STAT_WIRQ        (0 + (1 * 8))
#define cpRISC_STAT_RIRQ        (1 + (1 * 8))
#define cpRISC_STAT_UPIRQ       (2 + (1 * 8))


#define cpRISC_STAT_DSTAT_RDY   (0 + cpRISC_STAT_DSTAT)
#define cRISC_STAT_DSTAT_RDY    0
#define cRISC_STAT_DSTAT_BUSY   1


/* For DEMOD_REG_uP_CMDR_CFG */
#define cpDEMOD_UP_CMD_RX             7
#define cpDEMOD_UP_CMD_RX_IE          6
#define cpDEMOD_UP_RSP_TX             5
#define cpDEMOD_UP_RSP_TX_IE          4


#define mcSET_RISC_ACT(enb)                                         \
{                                                                   \
    mcSET_BIT(u4RegSetting, cpRISC_CTRL_##enb##_EN);                \
    mcRISC_Reg(pRiscCtx->pu4RegBase, DEMOD_REG_RISC_CTRL) = u4RegSetting;     \
}

#if fcCONV_RISC_ENDIAN
#define mcRISC_CTRL_MASK(u2CurByteCount, u1ByteOfst)    \
    mcRISC_BYTE_MASK(u2CurByteCount, u1ByteOfst, cpRISC_CTRL_B3_EN)
#else
#define mcRISC_CTRL_MASK(u2CurByteCount, u1ByteOfst)    \
    mcRISC_BYTE_MASK(u2CurByteCount, u1ByteOfst, cpRISC_CTRL_B0_EN)
#endif

#define mcSET_RISC_CTRL(enb)                                            \
{                                                                       \
/*    mcSET_BIT(u4RegSetting, u1ByteOfst + cpRISC_CTRL_B0_EN); */       \
    mcSET_MASK(u4RegSetting, mcRISC_CTRL_MASK(u2CurByteCount, u1ByteOfst)); \
    mcSET_RISC_ACT(enb);                                                \
}

#define mcRISC_RD_OK(u4RegSetting, stat)    \
    (mcGET_FIELD_CMB(u4RegSetting, RISC_STAT_##stat) == cRISC_STAT_##stat##_ATTN)
#define mcRISC_WR_OK(u4RegSetting, stat)    \
    (mcGET_FIELD_CMB(u4RegSetting, RISC_STAT_##stat) == cRISC_STAT_##stat##_RDY)

#define mcCHK_RISC_STS(ChkAct, stat)                                    \
{                                                                       \
    for (ii = 0; ii < cMAX_HC_WAIT_LOOP; ii ++)                         \
    {                                                                   \
        u4RegSetting = mcRISC_Reg(pRiscCtx->pu4RegBase, DEMOD_REG_RISC_STAT);     \
        if (mcRISC_##ChkAct##_OK(u4RegSetting, stat))                   \
            break;                                                      \
        mcDELAY_MS(cHC_WAIT_TIME_INTERVAL);                             \
    }                                                                   \
    if (!mcRISC_##ChkAct##_OK(u4RegSetting, stat))                      \
    {                                   /* Read / Write Fail */         \
    /* release semaphore */                                             \
        /*mcSEMA_UNLOCK_DBG(psDemodCtx->hRiscIfLock);*/                 \
        return 0;                                                       \
    }                                                                   \
}

#define mcRISC_DATA_READ(pu1Buffer)                                 \
{                                                                   \
    u4RegValue = mcRISC_Reg(pRiscCtx->pu4RegBase, DEMOD_REG_RISC_RDATA);     \
    mcRISC_ENDIAN_READ(pu1Buffer, u1ByteOfst, u2CurByteCount);      \
}

#define mcRISC_DATA_WRITE(pu1Buffer, u1ByteOfst)                    \
{                                                                   \
    mcRISC_ENDIAN_WRITE(pu1Buffer, u1ByteOfst, u2CurByteCount);     \
    mcRISC_Reg(pRiscCtx->pu4RegBase, DEMOD_REG_RISC_WDATA) = u4RegValue;     \
}

#define mcRISC_DATA_WRITE_ALGN(pu1Buffer)   \
    (mcRISC_Reg(pRiscCtx->pu4RegBase, DEMOD_REG_RISC_WDATA) = *((UINT32 *) (pu1Buffer)))


STATIC   HANDLE_T         hICtrlBus_RiscIfLock;       // Semaphore


INT32 ICtrlBus_Init(VOID)
{
    if (mcSEMA_CREATE(&hICtrlBus_RiscIfLock, X_SEMA_STATE_LOCK) != OSR_OK)
    {
        mcSHOW_HW_MSG(("%s: mcSEMA_CREATE(hICtrlBus_RiscIfLock) Fail!\n", __FUNCTION__));
        return (DRVAPI_TUNER_ERROR);
    }
    mcSEMA_UNLOCK_DBG(hICtrlBus_RiscIfLock);
    
    I2cPinmuxInit();
    
    return DRVAPI_TUNER_OK;
}

	
VOID I2cPinmuxInit(VOID)
{

#if defined (CC_MT5391)
    #define cCKGEN_REG_PMUX1    0x404
    #define cwPMUX1_TUNERI2C    2
    #define cpPMUX1_TUNERI2C    0
    #define cmPMUX1_TUNERI2C    mcSET_MASKS(PMUX1_TUNERI2C)

    UINT32 u4RegValue;
    
    /** pin mux setting
    */
    //! MX1[1:0] = 2'b01: Tuner i2c [Normal] 
    mcGET_SHOW_REG(CKGEN_BASE, cCKGEN_REG_PMUX1, u4RegValue);
    mcSET_FIELD_CMB(u4RegValue, 0x01, PMUX1_TUNERI2C );
    mcSET_SHOW_REG(CKGEN_BASE, cCKGEN_REG_PMUX1, u4RegValue);

#elif defined (CC_MT5363) /* For MT5363 */
    #define cCKGEN_REG_PMUX2    0x408
    #define cwPMUX2_TUNERI2C    2
    #define cpPMUX2_TUNERI2C    26
    #define cmPMUX2_TUNERI2C    mcSET_MASKS(PMUX2_TUNERI2C)

    UINT32 u4RegValue;
    
    /** pin mux setting
    */
    //! MX2[1:0] = 2'b00: Tuner i2c [Normal] 
    mcGET_SHOW_REG(CKGEN_BASE, cCKGEN_REG_PMUX2, u4RegValue);
    mcSET_FIELD_CMB(u4RegValue, 0x00, PMUX2_TUNERI2C );
    mcSET_SHOW_REG(CKGEN_BASE, cCKGEN_REG_PMUX2, u4RegValue);

#elif defined (CC_MT5387)&& !(defined CC_MT8223) /* For MT5387, MT5388 */
    #define cCKGEN_REG_PMUX2    0x408
    #define cwPMUX2_TUNERI2C    2
    #define cpPMUX2_TUNERI2C    16
    #define cmPMUX2_TUNERI2C    mcSET_MASKS(PMUX2_TUNERI2C)

    UINT32 u4RegValue;
    
    /** pin mux setting
    */
    //! MX2[1:0] = 2'b01: Tuner i2c [Normal] 
    mcGET_SHOW_REG(CKGEN_BASE, cCKGEN_REG_PMUX2, u4RegValue);
    mcSET_FIELD_CMB(u4RegValue, 0x01, PMUX2_TUNERI2C );
    mcSET_SHOW_REG(CKGEN_BASE, cCKGEN_REG_PMUX2, u4RegValue);

#endif

}

UINT8 ICtrlBus_I2cTunerWrite(
    UINT16 u2ClkDiv,
    UINT8 i2cAddr,
    UINT8 *data,
    UINT16 len
    )
{
    if (len > cMAX_I2C_LEN)
        mcSHOW_HW_MSG(("%d exceed max I2C length! (%d)\n", data, len));

    if (SIF_TunerWriteNoSubAddr(u2ClkDiv, i2cAddr, data, len) == 0)
    {
        mcSHOW_HW_MSG(("SIF_TunerWriteNoSubAddr failed\n"));
        return 1;
    }

    return 0;                               // TRUE
}

UINT8 ICtrlBus_I2cTunerRead(
    UINT16 u2ClkDiv,
    UINT8 i2cAddr,
    UINT8 *data,
    UINT16 len
    )
{
    if (SIF_TunerReadNoSubAddr(u2ClkDiv, i2cAddr, data, len) == 0)
    {
        mcSHOW_HW_MSG(("SIF_TunerReadNoSubAddr failed\n"));
        return 1;
    }
    return 0;
}
//-------------------------------------------------------------------------
/** ICtrlBus_I2cDemodWrite
 *  Write demod api :writer regisiter of demod by external I2C.
 *  @param  u2ClkDiv 	     Parameter of divider to divide 27Mhz for SCL.
 *  @param  u1DevAddr     Serial interface device address.
 *  @param  regAddr	     Demod regisiter address.
 *  @param  data 	            Pointer to user's data.
 *  @param  len 	            Number of byte to write.
 *  @retval   1		            Write success
 *  @retval   0		            Write fail
 */
//-------------------------------------------------------------------------
UINT8 ICtrlBus_I2cDemodWrite(
    UINT16 u2ClkDiv,
    UINT8 i2cAddr,
    UINT8 regAddr,
    UINT8 *data,
    UINT16 len
    )
{
    if (len > cMAX_I2C_LEN)
        mcSHOW_HW_MSG(("%d exceed max I2C length! (%d)\n", len, cMAX_I2C_LEN));
        
    if (SIF_X_Write(SIF_BUS_TUNER_I2C, u2ClkDiv, i2cAddr, 1, regAddr, data, len) == 0)
    {
        mcSHOW_HW_MSG(("ICtrlBus_I2cDemodWrite: %02X-%02X!\n", i2cAddr, regAddr));
        return 1;
    }
    return 0;                               // TRUE
}
//-------------------------------------------------------------------------
/** ICtrlBus_I2cDemodRead
 *  Read demod api :Read regisiter of demod by external I2C.
 *  @param  u2ClkDiv 	     Parameter of divider to divide 27Mhz for SCL.
 *  @param  u1DevAddr     Serial interface device address.
 *  @param  regAddr	     Demod regisiter address.
 *  @param  data 	            Pointer to user's data.
 *  @param  len 	            Number of byte to read. 
 *  @retval   1		            Write success
 *  @retval   0		            Write fail
 */
//-------------------------------------------------------------------------
UINT8 ICtrlBus_I2cDemodRead(
    UINT16 u2ClkDiv,
    UINT8 i2cAddr,
    UINT8 regAddr,
    UINT8 *data,
    UINT16 len
    )
{
    if (SIF_X_Read(SIF_BUS_TUNER_I2C, u2ClkDiv, i2cAddr, 1, regAddr, data, len) == 0)
    {
        mcSHOW_HW_MSG(("ICtrlBus_I2cDemodRead: %02X-%02X!\n", i2cAddr, regAddr));
        return 1;
    }
    return 0;
}

STATIC UINT16 ICtrlBus_RiscRead(PARAM_RISC_T *pRiscCtx, UINT16 u2RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount)
{
UINT16  u2CurByteCount;
UINT8   ii, u1ByteOfst;
UINT32  u4RegSetting, u4RegValue;
     
    // wait semaphore
    //mcSEMA_LOCK_DBG(psDemodCtx->hRiscIfLock);

    #if fcRISC_DIRECT_RW

    mcCALC_RISC_ADDR(u2RegAddr, u2ByteCount);
    u2RegAddr = (u4RegSetting << cRISC_WORD_WID);

    mcRISC_Reg(pRiscCtx->pu4RegBase, DEMOD_REG_RISC_CTRL) = mcRISC_CTRL_MASK(u2CurByteCount, u1ByteOfst);

    u4RegValue = mcRISC_Reg(pRiscCtx->pu4RegBase, u2RegAddr);
    mcRISC_ENDIAN_READ(pu1Buffer, u1ByteOfst, u2CurByteCount);

    #else

    mcCALC_RISC_ADDR(u2RegAddr, u2ByteCount);

    mcSET_RISC_CTRL(RD);
    mcCHK_RISC_STS(RD, RSTAT);
    mcRISC_DATA_READ(pu1Buffer);

    #endif

// release semaphore
    //mcSEMA_UNLOCK_DBG(psDemodCtx->hRiscIfLock);

    return u2CurByteCount;
}

STATIC UINT16  ICtrlBus_RiscWrite(PARAM_RISC_T *pRiscCtx, UINT16 u2RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount)
{
UINT8   ii, u1ByteOfst;
UINT16  u2CurByteCount;
UINT32  u4RegSetting, u4RegValue;

// wait semaphore
    //mcSEMA_LOCK_DBG(psDemodCtx->hRiscIfLock);

    #if fcRISC_DIRECT_RW

    mcCALC_RISC_ADDR(u2RegAddr, u2ByteCount);
    u2RegAddr = (u4RegSetting << cRISC_WORD_WID);

    mcRISC_Reg(pRiscCtx->pu4RegBase, DEMOD_REG_RISC_CTRL) = mcRISC_CTRL_MASK(u2CurByteCount, u1ByteOfst);

    mcRISC_ENDIAN_WRITE(pu1Buffer, u1ByteOfst, u2CurByteCount);
    mcRISC_Reg(pRiscCtx->pu4RegBase, u2RegAddr) = u4RegValue;

    #else

    mcCALC_RISC_ADDR(u2RegAddr, u2ByteCount);
    mcRISC_DATA_WRITE(pu1Buffer, u1ByteOfst);

    mcSET_RISC_CTRL(WR);
    mcCHK_RISC_STS(WR, WSTAT);

    #endif

// release semaphore
    //mcSEMA_UNLOCK_DBG(psDemodCtx->hRiscIfLock);

    return u2CurByteCount;
}

UINT8 ICtrlBus_RiscDemWrite(PARAM_RISC_T *pRiscCtx, UINT16 u2RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount)
{
UINT8 u1RetSts = 0;
UINT16  u2CurByteCount;

    // wait semaphore
    mcSEMA_LOCK_DBG(hICtrlBus_RiscIfLock);
        
    while (u2ByteCount)
    {
        if ((u2CurByteCount = ICtrlBus_RiscWrite(pRiscCtx, u2RegAddr, pu1Buffer, u2ByteCount)) == 0)
        {
            // release semaphore
            mcSEMA_UNLOCK_DBG(hICtrlBus_RiscIfLock);        
            return 1;            
        }
        
        u2ByteCount -= u2CurByteCount;
        pu1Buffer += u2CurByteCount;
        u2RegAddr += u2CurByteCount;
    }

    // release semaphore
    mcSEMA_UNLOCK_DBG(hICtrlBus_RiscIfLock);      
    
    return u1RetSts;
}

UINT8 ICtrlBus_RiscDemRead(PARAM_RISC_T *pRiscCtx, UINT16 u2RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount)
{
UINT8 u1Addr;
UINT8 u1RetSts = 0;
UINT16  u2CurByteCount;


      // wait semaphore
         mcSEMA_LOCK_DBG(hICtrlBus_RiscIfLock);

        if (u2RegAddr == DEMOD_REG_uP_CMDR_CFG)
        {
        UINT32  u4RegSetting;


            u4RegSetting = mcRISC_Reg(pRiscCtx->pu4RegBase, DEMOD_REG_RISC_STAT);
            u1Addr = 0;
            if (mcTEST_BIT(u4RegSetting, cpRISC_STAT_CMD_VAL))
                mcSET_BIT(u1Addr, cpDEMOD_UP_CMD_RX);
            if (mcTEST_BIT(u4RegSetting, cpRISC_STAT_UPIRQ))
            {
                mcSET_BIT(u1Addr, cpDEMOD_UP_RSP_TX);
            // Clear Int Status
                mcRISC_Reg(pRiscCtx->pu4RegBase, DEMOD_REG_RISC_STAT) = mcBIT(cpRISC_STAT_UPIRQ);
            }
            *pu1Buffer = u1Addr;

        }
        else
        {
            while (u2ByteCount)
            {
               if ((u2CurByteCount = ICtrlBus_RiscRead(pRiscCtx, u2RegAddr, pu1Buffer, u2ByteCount)) == 0)
               {
                    // release semaphore
                    mcSEMA_UNLOCK_DBG(hICtrlBus_RiscIfLock);                           
                    return 1;
               }
                u2ByteCount -= u2CurByteCount;
                pu1Buffer += u2CurByteCount;
                u2RegAddr += u2CurByteCount;
            }
        }

    // release semaphore
        mcSEMA_UNLOCK_DBG(hICtrlBus_RiscIfLock);    
    
    return u1RetSts;
	
}

UINT8 ICtrlBus_Risc_HostWriteCmd(PARAM_RISC_T *pRiscCtx, UINT16 u2RegAddr, UINT8 *pu1Buffer, UINT8 u1ParaCnt, UINT8 u1ParaOfst)
{
    #if fcCONV_RISC_ENDIAN
    UINT8   ii;
    UINT32  u4RegValue;
    #endif

    // wait semaphore
        mcSEMA_LOCK_DBG(hICtrlBus_RiscIfLock);

    #if fcCONV_RISC_ENDIAN
        mcRISC_ENDIAN_WRITE(pu1Buffer, 0, cRISC_WORD_LEN);
        mcRISC_Reg(pRiscCtx->pu4RegBase, u2RegAddr) = u4RegValue;
        mcRISC_ENDIAN_WRITE(pu1Buffer + cRISC_WORD_LEN, 0, cRISC_WORD_LEN);
        mcRISC_Reg(pRiscCtx->pu4RegBase, u2RegAddr + cRISC_WORD_LEN) = u4RegValue;
    #else
        mcRISC_Reg(pRiscCtx->pu4RegBase, u2RegAddr) = ((UINT32 *) pu1Buffer)[0];
        mcRISC_Reg(pRiscCtx->pu4RegBase, u2RegAddr + cRISC_WORD_LEN) = ((UINT32 *) pu1Buffer)[1];
    #endif

    // release semaphore
       mcSEMA_UNLOCK_DBG(hICtrlBus_RiscIfLock);

        return u1ParaCnt;
}

UINT8 ICtrlBus_Risc_HostReadRsp(PARAM_RISC_T *pRiscCtx, UINT16 u2RegAddr, UINT8 *pu1Buffer, UINT8 u1ParaCnt, UINT8 u1ParaOfst)
{
    #if fcCONV_RISC_ENDIAN
    UINT8   ii;
    UINT32  u4RegValue;
    #endif

    // wait semaphore
        mcSEMA_LOCK_DBG(hICtrlBus_RiscIfLock);

    #if fcCONV_RISC_ENDIAN
        u4RegValue = mcRISC_Reg(pRiscCtx->pu4RegBase, u2RegAddr);
        mcRISC_ENDIAN_READ(pu1Buffer, 0, cRISC_WORD_LEN);
        u4RegValue = mcRISC_Reg(pRiscCtx->pu4RegBase, u2RegAddr + cRISC_WORD_LEN);
        mcRISC_ENDIAN_READ(pu1Buffer + cRISC_WORD_LEN, 0, cRISC_WORD_LEN);
    #else
        ((UINT32 *) pu1Buffer)[0] = mcRISC_Reg(pRiscCtx->pu4RegBase, u2RegAddr);
        ((UINT32 *) pu1Buffer)[1] = mcRISC_Reg(pRiscCtx->pu4RegBase, u2RegAddr + cRISC_WORD_LEN);
    #endif

    // release semaphore
        mcSEMA_UNLOCK_DBG(hICtrlBus_RiscIfLock);

        return u1ParaCnt;	
}

UINT8 ICtrlBus_Risc_LoadCode(PARAM_RISC_T *pRiscCtx, UINT8 u1RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount)
{

//DEMOD_CTX_T *psDemodCtx = (DEMOD_CTX_T *) psI2cCtx;

    UINT8   ii, u1RetSts = 0, u1AlignChk = 0;
    UINT16  u2CurByteCount;
    UINT32  u4RegSetting;
    UINT32  u4RegValue;
    STATIC  UINT8 u1AlignWarn = 0;

    //if (psDemodCtx == NULL)
    //    return 1;

    // wait semaphore
        mcSEMA_LOCK_DBG(hICtrlBus_RiscIfLock);

        mcCALC_RISC_ADDR_BASE(u1RegAddr);
        mcSET_BIT(u4RegSetting, cpRISC_CTRL_B0_EN);
        mcSET_RISC_ACT(DL);
        mcCALC_RISC_ADDR_CNT(u2ByteCount, 0);
        if ((u2CurByteCount != cRISC_WORD_LEN)
         || ((((UINT32) (pu1Buffer)) & mcMASK(cRISC_WORD_WID)) != 0))
        {
            if (!u1AlignWarn)
                mcSHOW_DBG_MSG(("\n ****** RISC_Write not aligned! (%d, 0x%X) ******\n",
                                u2CurByteCount, (UINT32) (pu1Buffer)));
            u1AlignWarn = 1;
            u1AlignChk = 1;
        //    u1RetSts = 1;
        //    break;
        }
      
       while (u2ByteCount)
        {
        
            mcCALC_RISC_ADDR_CNT(u2ByteCount, 0);

            if (!u1AlignChk)
                mcRISC_DATA_WRITE_ALGN(pu1Buffer);
            else
                mcRISC_DATA_WRITE(pu1Buffer, 0);
            
            mcCHK_RISC_STS(WR, DSTAT);
         
            if (u2CurByteCount == 0)
            {
                u1RetSts = 1;
                break;
            }
            
            u2ByteCount -= u2CurByteCount;
            pu1Buffer += u2CurByteCount;
        }
        mcRISC_Reg(pRiscCtx->pu4RegBase, DEMOD_REG_RISC_CTRL) = 0;   // Disable download mode

    // release semaphore
        mcSEMA_UNLOCK_DBG(hICtrlBus_RiscIfLock);

        return u1RetSts;
        
}
//-------------------------------------------------------------------------
/** ICtrlBus_SetPage_ExtI2c
 *  Set demod page api :set external demod page.
 *  @param  psI2cCtx 	     Pointer of I2C ctx.
 *  @param  u1I2cAddr       Demod I2C address.
 *  @param  regAddr	     Demod regisiter address.
 *  @retval   0		            Set page success
 *  @retval   1	                   Set page fail
 */
//-------------------------------------------------------------------------
UINT8 ICtrlBus_SetPage_ExtI2c(VOID *psI2cCtx, UINT8 u1I2cAddr, UINT16 u2RegAddr)
{
static UINT8 u1PrePageAddr = 0xFF;
UINT8 u1PageAddr;
UINT8 u1RetSts = 0;

    u1PageAddr = mcHIGH_BYTE(u2RegAddr);        
    if (u1PageAddr != u1PrePageAddr)
    {
        if (ICtrlBus_I2cDemodWrite(cI2C_TIMING, u1I2cAddr, DEMO_REG_PAGE_NUM, &u1PageAddr, 1 ))	   
	 {
    	     u1RetSts = 1;
    	 }
        u1PrePageAddr = u1PageAddr;
    }
        return u1RetSts;
}
//-------------------------------------------------------------------------
/** ICtrlBus_I2cDemRead_ExtI2c
 *  Read external demod api :read external demod.
 *  @param  psI2cCtx 	     Pointer of I2C ctx.
 *  @param  u1I2cAddr       Demod I2C address.
 *  @param  regAddr	     Demod regisiter address.
 *  @param  pu1Buffer        Pointer to user's data.
 *  @param  u2ByteCount   Number of byte to read
 *  @retval   0		            Read success
 *  @retval   1	                   Read fail
 */
//-------------------------------------------------------------------------

UINT8 ICtrlBus_I2cDemRead_ExtI2c(VOID *psI2cCtx, UINT8 u1I2cAddr, UINT16 u2RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount)
{

UINT8 u1RegAddr;
UINT8 u1RetSts = 0;

    /* Set Page */
    u1RetSts = ICtrlBus_SetPage_ExtI2c(psI2cCtx, u1I2cAddr, u2RegAddr);

    /* Read Reg */
    u1RegAddr = mcLOW_BYTE(u2RegAddr); 
	

    if (ICtrlBus_I2cDemodRead(cI2C_TIMING, u1I2cAddr, u1RegAddr, pu1Buffer, u2ByteCount))        
    {            
        u1RetSts = 1;        
    }        

    return u1RetSts;       
}

//-------------------------------------------------------------------------
/** ICtrlBus_I2cDemWrite_ExtI2c
 *  Write external demod api :Write external demod.
 *  @param  psI2cCtx 	     Pointer of I2C ctx.
 *  @param  u1I2cAddr       Demod I2C address.
 *  @param  regAddr	     Demod regisiter address.
 *  @param  pu1Buffer        Pointer to user's data.
 *  @param  u2ByteCount   Number of byte to write
 *  @retval   0		            Write success
 *  @retval   1	                   Write fail
 */
//-------------------------------------------------------------------------

UINT8 ICtrlBus_I2cDemWrite_ExtI2c(VOID *psI2cCtx, UINT8 u1I2cAddr, UINT16 u2RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount)
{
UINT8 u1RegAddr;
UINT8 u1RetSts = 0;

    /* Set Page*/
    u1RetSts = ICtrlBus_SetPage_ExtI2c(psI2cCtx, u1I2cAddr, u2RegAddr);
       	
    /* Write Reg*/
    u1RegAddr = mcLOW_BYTE(u2RegAddr); 

    if(ICtrlBus_I2cDemodWrite(cI2C_TIMING, u1I2cAddr, u1RegAddr, pu1Buffer, u2ByteCount)) 
    {            
        u1RetSts = 1;        
    }
    return u1RetSts;
}
