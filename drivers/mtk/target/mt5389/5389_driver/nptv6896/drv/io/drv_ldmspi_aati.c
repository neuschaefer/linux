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
 * $RCSfile: nandhw_reg.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file ldm.c
 *  ldm.c provides local dimming control functions\n
 *  device.
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_typedef.h"
#include "sv_const.h"
#include "nptv_debug.h"
#include "panel.h"
#include "hw_ldmspi.h"
#include "drv_ldmspi.h"
#include "drv_lcdim.h"
#include "drv_display.h"
#include "x_printf.h"
#include "hw_sw.h"
#include "drv_pwm.h"
#include "pe_if.h"

UINT16 u2AatiFaultState[16] =          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
UINT16 u2AatiBadBlock = 0x0;            // use 16bit to record bad block status
UINT16 u2AatiDriverOrder[256];
UINT32 _u4AatiFaultBlkIdx = 0;

UINT8 u1AATIDelayOrder[16] = {0, 0, 1, 2, 2, 3, 4, 4, 4, 4, 3, 2, 2, 1, 0, 0};

extern UINT8 u1SpiCmdArray[MAX_SPI_CMD_SIZE];
extern UINT8 u1ReadFlg;

extern PANEL_LOCAL_DIMMING_ATTRIBUTE_T *pParam;

//====================================================================================
//                        Function Prototype For Command Set
//====================================================================================
sLdmSpiCommandSet sAatiSpiFunc = 
{
	vDrvAatiBatchReadCmd,            //  void (*vSpiBatchReadCmd)(void);
	vDrvAatiBatchWriteCmd,           //  void (*vSpiBatchWriteCmd)(void);
    vDrvAatiLightUpCmd,              //  void (*vSpiLightUpCmd)(void);
	vDrvAatiLogOffCmd,               //	void (*vSpiLogOffCmd)(void);
	vDrvAatiRepairCmd,	            //	void (*vSpiRepairCmd)(void);	
	vDrvAatiRead1BCmd,	            //	void (*vSpiRead1BCmd)(UINT8);
	vDrvAatiWrite1BCmd               //	void (*vSpiWrite1BCmd)(UINT8, UINT8);	
};

//====================================================================================
//                     AATI Spec Implementation (General Behavior)
//====================================================================================

void vDrvAatiRead1BCmd(UINT8 u1Addr)
{
    UINT8 u1ReadBack;

    vIO32WriteFldAlign(LED_CON, 2, OP_MODE);  
    vIO32WriteFldAlign(LED_NOB2, 2, RBYTE_LEN);     
    vIO32WriteFldAlign(LED_NOB2, 1, RBACK_LEN);         

    u1SpiCmdArray[0] = (UINT8)((LDM_AAT2439A_DEVICE_ADDR << 1)| LDM_AAT2439A_READ_IDX);
    u1SpiCmdArray[1] = u1Addr;

    vDrvLcdimSpiSendDataSWMode(u1SpiCmdArray, 2);    

    if(vDrvLcdimSpiWaitReady(10000000)==SV_FAIL)
    {
        Printf("[SPI] Reading Address %d Fail\n", u1Addr);

        vIO32WriteFldAlign(LED_CON, 1, OP_MODE);          

        return;
    }
    
    u1ReadBack = IO32ReadFldAlign(LED_RBUF, READ_BUF0);    
    
    Printf("Current Reading Address %d, Readback = 0x%x\n", u1Addr, u1ReadBack);

    vIO32WriteFldAlign(LED_CON, 1, OP_MODE);  
}



void vDrvAatiWrite1BCmd(UINT8 u1Addr, UINT8 u1Data)
{
    // 0. set header (device id and register address)
    u1SpiCmdArray[0] = (UINT8)((LDM_AAT2439A_DEVICE_ADDR << 1)| LDM_AAT2439A_WRITE_IDX);
    u1SpiCmdArray[1] = u1Addr;    
    u1SpiCmdArray[2] = u1Data;

    vDrvLcdimSpiSendDataSWMode(u1SpiCmdArray, 3);   
}

void vDrvAatiSetFaultState(UINT8 u1BlkIdx, UINT8 u1FaultStatus);
BOOL _bDrvAatiReadCmd(void)
{
    UINT8 u1ReadBack;
    
    if(u1ReadFlg == SV_TRUE)
    {
        if(vDrvLcdimSpiWaitReady(10000000)==SV_FAIL)
        {
            LOG(4, "[SPI] Read Procedure Fail, Wait For Next Vsync\n");

            return SV_FAIL;
        }
        
        u1ReadBack = IO32ReadFldAlign(LED_RBUF, READ_BUF0);    

        // AATI spec for fault handling.
        vDrvAatiSetFaultState(_u4AatiFaultBlkIdx, u1ReadBack);

        ++_u4AatiFaultBlkIdx;
        
        _u4AatiFaultBlkIdx %= 4;
    
        vIO32WriteFldAlign(LED_CON, 1, OP_MODE);  
        
        u1ReadFlg = SV_FALSE;       
    }   

    return SV_SUCCESS;            
}

void vDrvAatiBatchReadCmd(void)
{
    // if hw is sending data, avoid r/w command to SRAM
    if(u1DrvLcdimSpiRWProtect() == SV_FALSE)
    {
        return;
    }    

    if(_bDrvAatiReadCmd() == SV_FAIL)
    {
        return;
    }
    
    vIO32WriteFldAlign(LED_CON, 2, OP_MODE);  
    vIO32WriteFldAlign(LED_NOB2, 2, RBACK_LEN);         

    u1SpiCmdArray[0] = (UINT8)((LDM_AAT2439A_DEVICE_ADDR << 1)| LDM_AAT2439A_READ_IDX);
    u1SpiCmdArray[1] = (UINT8)(LDM_AAT2439A_Fault_Status + _u4AatiFaultBlkIdx);   

    vDrvLcdimSpiSendDataHWMode(u1SpiCmdArray, 2);        

    // set flag, read in next vsync
    u1ReadFlg = SV_TRUE;
}

void vDrvAatiBatchWriteCmd(void)
{
#ifndef CC_MTK_LOADER      
    UINT16 u2DataLen = 2 + u2DrvLcdimSpiGetBlockNum()*2;
    UINT16 i;    
    UINT8 u1DimGain = vDrvLcdimSpiGetGlobalDimming();
    UINT16 u2TempBuf;    
    static UINT8 u1Buffer[256];

    // previous step is read function
    // ( note : spi read command will take effect in next vsync, hence need to be handled in write command section )
    if(_bDrvAatiReadCmd() == SV_FAIL)
    {
        return;
    }
    
    vDrvGetLcDimDimmingResult(u1Buffer);

    // 0. set header (device id and register address)
    u1SpiCmdArray[0] = (UINT8)((LDM_AAT2439A_DEVICE_ADDR << 1)| LDM_AAT2439A_WRITE_IDX);
    u1SpiCmdArray[1] = (UINT8)LDM_AAT2439A_PWM_DUTY1_LB;    

    // 1. set data body (16 bit per block, but each block use 9 bit to represent data actually)
    for(i=0; i < u2DrvLcdimSpiGetBlockNum(); i++)
    {
        if(!pParam->u1ScanningOnoff)
        {
            u1SpiCmdArray[2+i*2] =   (((u2IO32Read2B(ADAP_LCDIM_SPI_SEP) >> u2AatiDriverOrder[i]) & 0x1) == 1) ? 0 : (UINT8)((UINT32)((UINT32)u1Buffer[i]*(UINT32)u1DimGain)>>8);
            u1SpiCmdArray[2+i*2+1] = 0;

        }
        else
        {
            // when scanning supported, means pwm invert function is enabled, hence need to re-invert data using software
            u2TempBuf = 0x1ff - (UINT16)(((UINT32)((UINT32)u1Buffer[i]*(UINT32)u1DimGain)>>8) << 1);
            u1SpiCmdArray[2+i*2] = (((u2IO32Read2B(ADAP_LCDIM_SPI_SEP) >> u2AatiDriverOrder[i]) & 0x1) == 1) ? 0xFF : (UINT8)(u2TempBuf & 0xff);      
            u1SpiCmdArray[2+i*2+1] = (((u2IO32Read2B(ADAP_LCDIM_SPI_SEP) >> u2AatiDriverOrder[i]) & 0x1) == 1) ? 1 : (UINT8)((u2TempBuf>>8) & 0xff);
        }
    }        
    vDrvLcdimSpiSendDataHWMode(u1SpiCmdArray, u2DataLen);        
#endif      
}

void vDrvAatiLightUpCmd(void)
{
    //-------------------------------------
    // Customization Part For Different Panel Design
    //-------------------------------------
    UINT16 u2DataLen = 2 + u2DrvLcdimSpiGetBlockNum()*2;
    UINT16 i;
    UINT16 u2InitLevel = 0x3FE;
    UINT16 u2RegulatorTurnOff = 0x0;
//    UINT32 u4FeedbackTurnOff = 0x0;
    UINT32 u4OpenShortTurnOff = 0x0;

    vDrvLcdimSpiSetLoopAllOnOff(SV_OFF);    
    
    vIO32WriteFldAlign(LED_CON, 1, OP_MODE);  

    //-------------------------------------
    // 0. DOT setting
    //-------------------------------------    
    // 0.0. set header (device id and register address)
    u1SpiCmdArray[0] = (UINT8)((LDM_AAT2439A_DEVICE_ADDR << 1)| LDM_AAT2439A_WRITE_IDX);
    u1SpiCmdArray[1] = (UINT8)LDM_AAT2439A_CTRL3;  
    u1SpiCmdArray[2] = 0x00;
    
    vDrvLcdimSpiSendDataSWMode(u1SpiCmdArray, 3);   

    u1SpiCmdArray[0] = (UINT8)((LDM_AAT2439A_DEVICE_ADDR << 1)| LDM_AAT2439A_WRITE_IDX);
    u1SpiCmdArray[1] = (UINT8)LDM_AAT2439A_DOT;  
    u1SpiCmdArray[2] = 0xF0;
    
    vDrvLcdimSpiSendDataSWMode(u1SpiCmdArray, 3);      
    //-------------------------------------
    // 1. turn on all current sink enable
    //-------------------------------------    
    // 1.0. set header (device id and register address)
    u1SpiCmdArray[0] = (UINT8)((LDM_AAT2439A_DEVICE_ADDR << 1)| LDM_AAT2439A_WRITE_IDX);
    u1SpiCmdArray[1] = (UINT8)LDM_AAT2439A_CTRL1; 

    // 1.1. set data body
    for(i = 0; i < u2DrvLcdimSpiGetBlockNum(); i++)
    {
        u2RegulatorTurnOff |= (1 << i);
    }

    u1SpiCmdArray[2] = u2RegulatorTurnOff & 0xFF;
    u1SpiCmdArray[3] = (u2RegulatorTurnOff >> 8) & 0xFF;
    
    vDrvLcdimSpiSendDataSWMode(u1SpiCmdArray, 4);   
  
    //-------------------------------------
    // 2. turn on open / short detection
    //-------------------------------------    
    // 2.1. set header (device id and register address)
    u1SpiCmdArray[0] = (UINT8)((LDM_AAT2439A_DEVICE_ADDR << 1)| LDM_AAT2439A_WRITE_IDX);
    u1SpiCmdArray[1] = (UINT8)LDM_AAT2439A_OTSL;     
    u1SpiCmdArray[2] = 0xC8;                      // Short LED threshold 9V, clear fault flag
    vDrvLcdimSpiSendDataSWMode(u1SpiCmdArray, 3);   
    
    u1SpiCmdArray[0] = (UINT8)((LDM_AAT2439A_DEVICE_ADDR << 1)| LDM_AAT2439A_WRITE_IDX);
    u1SpiCmdArray[1] = (UINT8)LDM_AAT2439A_FAULT_TRIGGER;    

    for(i = 0; i < u2DrvLcdimSpiGetBlockNum(); i++)
    {
        u4OpenShortTurnOff |= (3 << i);
    }    
    u1SpiCmdArray[2] = u4OpenShortTurnOff & 0xFF;
    u1SpiCmdArray[3] = (u4OpenShortTurnOff >> 8) & 0xFF;
    u1SpiCmdArray[4] = (u4OpenShortTurnOff >> 16) & 0xFF;
    u1SpiCmdArray[5] = (u4OpenShortTurnOff >> 24) & 0xFF;
    vDrvLcdimSpiSendDataSWMode(u1SpiCmdArray, 6);

    vDrvAatiSetScanning();

    //-------------------------------------
    // 2. set all dimming to full
    //-------------------------------------    

    // 2.0. set header (device id and register address)
    u1SpiCmdArray[0] = (UINT8)((LDM_AAT2439A_DEVICE_ADDR << 1)| LDM_AAT2439A_WRITE_IDX);
    u1SpiCmdArray[1] = (UINT8)LDM_AAT2439A_PWM_DUTY1_LB;  

    // 2.1. set data body (16 bit per block)
    for(i=0; i < u2DrvLcdimSpiGetBlockNum(); i++)
    {

        if(pParam->u1ScanningOnoff)        
        {
//            u1SpiCmdArray[2+i*2] = (UINT8)(((UINT16)(0xFF - u1InitLevel) << 1) & 0xFF);
//            u1SpiCmdArray[2+i*2+1] = (UINT8)(((UINT16)(0xFF - u1InitLevel) << 1) >> 8);
              u1SpiCmdArray[2+i*2] = (UINT8)(((UINT16)(0x3FF - u2InitLevel) << 1) & 0xFF);
              u1SpiCmdArray[2+i*2+1] = (UINT8)(((UINT16)(0x3FF - u2InitLevel) << 1) >> 8);

        }
        else
        {
            u1SpiCmdArray[2+i*2] = u2InitLevel & 0xFF;
            u1SpiCmdArray[2+i*2+1] = (u2InitLevel>>8) & 0x03;
        }
    }        

    vDrvLcdimSpiSendDataSWMode(u1SpiCmdArray, u2DataLen);   

    vDrvLcdimSpiSetLoopAllOnOff(SV_ON);

    LOG(0, "u2RegulatorTurnOff =%d\n", u2RegulatorTurnOff);
    LOG(0, "u4OpenShortTurnOff =%d\n", u4OpenShortTurnOff);

}

void vDrvAatiLogOffCmd(void)
{
    UINT16 u2DataLen = 2 + u2DrvLcdimSpiGetBlockNum()*2;
    UINT16 i;

    if(u2DataLen > MAX_SPI_CMD_SIZE)
    {
        u2DataLen = MAX_SPI_CMD_SIZE;    // fix klockwork
    }

    vDrvLcdimSpiSetLoopAllOnOff(SV_OFF);    

    vIO32WriteFldAlign(LED_CON, 1, OP_MODE);  
    
    // 1. set all block dimming to 0
    // 1.0. set header (device id and register address)
    u1SpiCmdArray[0] = (UINT8)((LDM_AAT2439A_DEVICE_ADDR << 1)| LDM_AAT2439A_WRITE_IDX);
    u1SpiCmdArray[1] = (UINT8)LDM_AAT2439A_PWM_DUTY1_LB;  

    // 1.1. set data body (16 bit per block)
    for(i=1; i < u2DrvLcdimSpiGetBlockNum(); i++)
    {
        u1SpiCmdArray[2+i*2] = 0;
        u1SpiCmdArray[2+i*2+1] = 0;
    }        

    vDrvLcdimSpiSendDataSWMode(u1SpiCmdArray, u2DataLen); 

    // 2. turn off current regulators    
    // 2.0. set header (device id and register address)
    u1SpiCmdArray[0] = (UINT8)((LDM_AAT2439A_DEVICE_ADDR << 1)| LDM_AAT2439A_WRITE_IDX);
    u1SpiCmdArray[1] = (UINT8)LDM_AAT2439A_CTRL1;  

    // 2.1. set data body
    u1SpiCmdArray[2] = 0x00;
    u1SpiCmdArray[3] = 0x00;
    
    vDrvLcdimSpiSendDataSWMode(u1SpiCmdArray, 4); 

    LOG(0, "SPI Init [LogOff]\n");
}

//====================================================================================
//                 AATI Spec Implementation (Customized Behavior)
//====================================================================================

void vDrvAatiSetFaultState(UINT8 u1BlkIdx, UINT8 u1FaultStatus)
{
    UINT8 u1Blk = 0;

    if(u1BlkIdx > 3)
    {
        u1BlkIdx = 3;     // fix klockwork
    }    
    
    for(u1Blk = 0; u1Blk < 4; u1Blk++)
    {
        if(((u1FaultStatus>>(u1Blk*2)) & 0x3) == E_SPI_SHORT_STATUS)
        {
            if(u2AatiFaultState[u1BlkIdx*4+u1Blk] == E_SPI_NORMAL_STATUS)
            {
                u2AatiFaultState[u1BlkIdx*4+u1Blk] = E_SPI_UNCERTAIN_SHORT_STATUS;
            }
            else
            {
                u2AatiFaultState[u1BlkIdx*4+u1Blk] += 1;
            }
        }
        else
        {
            u2AatiFaultState[u1BlkIdx*4+u1Blk] = ((u1FaultStatus>>(u1Blk*2)) & 0x3);
        }

        if((u2AatiFaultState[u1BlkIdx*4+u1Blk] >= E_SPI_SHORT_TOLERANCE) || (u2AatiFaultState[u1BlkIdx*4+u1Blk] == E_SPI_OPEN_STATUS))
        {
            u2AatiBadBlock |= (1 << (u1BlkIdx*4+u1Blk));
        }
    }
}

UINT8 u1DrvAatiGetFaultState(void)
{
    UINT8 u1Blk = 0;

    // return open status first
    for(u1Blk = 0; u1Blk < u2DrvLcdimSpiGetBlockNum(); u1Blk++)
    {
        if(u1Blk > 15)
        {
            u1Blk = 15;     // fix klockwork
        }
        
        if(u2AatiFaultState[u1Blk] == E_SPI_OPEN_STATUS)
        {
            return E_SPI_OPEN_STATUS;
        }
    }

    // if any block short over than 60 frame, mark it as real short
    for(u1Blk = 0; u1Blk < u2DrvLcdimSpiGetBlockNum(); u1Blk++)
    {      
        if(u1Blk > 15)
        {
            u1Blk = 15;     // fix klockwork
        }
        
        if(u2AatiFaultState[u1Blk] >= E_SPI_SHORT_TOLERANCE)
        {
            LOG(0, "[%d] SPI u2FaultState = %d\n", u1Blk, u2AatiFaultState[u1Blk]);               
            return E_SPI_SHORT_STATUS;
        }
    }    

    // mark uncertain short block
    for(u1Blk = 0; u1Blk < u2DrvLcdimSpiGetBlockNum(); u1Blk++)
    {
        if(u1Blk > 15)
        {
            u1Blk = 15;     // fix klockwork
        }
        
        if((u2AatiFaultState[u1Blk] >= E_SPI_UNCERTAIN_SHORT_STATUS) && (u2AatiFaultState[u1Blk] < E_SPI_SHORT_TOLERANCE))
        {
            return E_SPI_UNCERTAIN_SHORT_STATUS;
        }
    }    

    return E_SPI_NORMAL_STATUS;
}

void vDrvAatiRepairCmd(void)
{
    UINT8 i;
    UINT16 u2RepairMask = 0x0;
    UINT8 u1FaultState = u1DrvAatiGetFaultState();

    for(i = 0; i < u2DrvLcdimSpiGetBlockNum(); i++)
    {
        u2RepairMask |= (1 << i);
    }

    u2AatiBadBlock &= u2RepairMask;

    if((u1FaultState == E_SPI_OPEN_STATUS)||(u1FaultState == E_SPI_SHORT_STATUS))
    {
        u1SpiCmdArray[0] = (UINT8)((LDM_AAT2439A_DEVICE_ADDR << 1)| LDM_AAT2439A_WRITE_IDX);
        u1SpiCmdArray[1] = (UINT8)LDM_AAT2439A_CTRL1; 

        u1SpiCmdArray[2] = (UINT8)(u2AatiBadBlock & 0xFF);
        u1SpiCmdArray[3] = (UINT8)((u2AatiBadBlock >> 8) & 0xFF);

        LOG(0, "Repair Open/Short 0x%x 0x%x\n", u1SpiCmdArray[2], u1SpiCmdArray[3]);
    
        vDrvLcdimSpiSendDataHWMode(u1SpiCmdArray, 4); 
        vDrvLcdimSpiWaitReady(1000000);            
    }

    if(u1FaultState == E_SPI_UNCERTAIN_SHORT_STATUS)
    {
        // uncertain state, bypass
    }
}
/*
UINT16 u2DrvGetTotalBadBlocks(void)
{
    UINT16 i, u2BadCount = 0;
    for(i = 0; i < u2DrvLcdimSpiGetBlockNum(); i++)
    {
        if(((u2AatiBadBlock >> i) & 0x1) == 1)
        {
            u2BadCount++;
        }
    }
    return u2BadCount;
}
*/
// For Delay Order Customization
void vDrvAatiSetDelayOrder(UINT8 *u1CustDelayOrder)
{
    UINT8 i;
    
    for(i = 0; i < 16; i++)
    {
        u1AATIDelayOrder[i] = u1CustDelayOrder[i];
    }    
}

// Customization Part For Customer Spec (5 Dimming Area)
void vDrvAatiSetScanning(void)
{
    static UINT16 u2AatiDelayTime[3][5] = {
                                    {128, 248, 312, 370, 468},      // for scanning model
                                    {222, 279, 414, 485,  60},      // for non-scanning model
                                    {468, 370, 312, 248, 128}       // for upside-down scanning model                                
                                  };

    UINT16 u2DataLen = 2 + u2DrvLcdimSpiGetBlockNum()*2;
    UINT16 i;
        
    //-------------------------------------
    // 0. Set Scanning Backlight
    //-------------------------------------        
    // hsync (pwm) clk set to 30kHz
    // 256 (pwm resolution) x 120 (frame rate) = 30800
    // vDrvSetPWM(SrcPWM2, (BSP_GetDomainClock(SRC_BUS_CLK)/255) / 30720, 0x80, 0xFF); 
    // set dimming resolution
    // 20110525 todo : move scanning backlight setting to customization part
    if(pParam->u1ScanningOnoff)
    {
        vDrvSetPWM(HSYNC_SRC, (BSP_GetDomainClock(SRC_BUS_CLK)/255) / 63000, 0x80, 0xFF); 

        // 0.1. set scanning parameters
        u1SpiCmdArray[0] = (UINT8)((LDM_AAT2439A_DEVICE_ADDR << 1)| LDM_AAT2439A_WRITE_IDX);
        u1SpiCmdArray[1] = (UINT8)LDM_AAT2439A_CTRL3;

        u1SpiCmdArray[2] = 0x20;    // 0x10;  // For Scan Backlight Function, Set PWM invert
 

        vDrvLcdimSpiSendDataSWMode(u1SpiCmdArray, 3);
                                    
        // 0.2 set scanning delay
        u2DataLen = 2 + u2DrvLcdimSpiGetBlockNum()*2;
        
        u1SpiCmdArray[0] = (UINT8)((LDM_AAT2439A_DEVICE_ADDR << 1)| LDM_AAT2439A_WRITE_IDX);
        u1SpiCmdArray[1] = (UINT8)LDM_AAT2439A_PWM_DELAY1_LB; 

        // u1AatiDelayOrder need to be set in customization part (for different panel such as 16/10/8 block case)
        for(i=0; i < u2DrvLcdimSpiGetBlockNum(); i++)
        {
            if(i > 15)
            {
                i = 15;
            }

            
            if(u1AATIDelayOrder[i] > 5)
            {
                u1AATIDelayOrder[i] = 5;
            }

            if(pParam->u1ScanningTblIdx > 2)
            {
                pParam->u1ScanningTblIdx = 2;
            }
                
            u1SpiCmdArray[2 + i*2] =  (UINT8)(u2AatiDelayTime[pParam->u1ScanningTblIdx][u1AATIDelayOrder[i]]&0xFF);  // delay time LSB of channel[i]
            u1SpiCmdArray[2 + i*2 + 1] = (UINT8)((u2AatiDelayTime[pParam->u1ScanningTblIdx][u1AATIDelayOrder[i]]>>8)&0xFF);                                                           // delay time MSB of channel[i]     
        }
        vDrvLcdimSpiSendDataSWMode(u1SpiCmdArray, u2DataLen);   
    }    
    else
    {
        u1SpiCmdArray[0] = (UINT8)((LDM_AAT2439A_DEVICE_ADDR << 1)| LDM_AAT2439A_WRITE_IDX);
        u1SpiCmdArray[1] = (UINT8)LDM_AAT2439A_PWM_DUTY1_LB;         
        u1SpiCmdArray[2] = 0xFF;

        vDrvLcdimSpiSendDataSWMode(u1SpiCmdArray, 3);   
        LOG(0, "Not Support Scanning\n");
    }     
}

