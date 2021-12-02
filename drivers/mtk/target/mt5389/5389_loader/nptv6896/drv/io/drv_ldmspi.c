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

extern sLdmSpiCommandSet sAmsSpiFunc;
extern sLdmSpiCommandSet sLgdSpiFunc;

sLdmSpiCommandSet *sSpiFunc = NULL;

PANEL_LOCAL_DIMMING_ATTRIBUTE_T *pParam = NULL;

UINT8 u1SpiCmdArray[MAX_SPI_CMD_SIZE];

UINT16 _u2LcdimBlockNum = 0;
UINT8 u1ReadFlg = SV_FALSE;
//====================================================================================
//                     Interface For System / MW Integration
//====================================================================================
void SPI_NON_DEF_CMD_0_PARAM(void){}                            // for non-defined command in specific IC
void SPI_NON_DEF_CMD_1_PARAM(UINT8 u1Param1){}                  // for non-defined command in specific IC
void SPI_NON_DEF_CMD_2_PARAM(UINT8 u1Param1, UINT8 u1Param2){}  // for non-defined command in specific IC

void vDrvLcdimSpiSetLoopAllOnOff(UINT8 u1OnOff)
{
    vIO32WriteFldAlign(LED_CON, u1OnOff, HW_TRIG_EN);  
    vIO32WriteFldAlign(ADAP_LCDIM_SPI, u1OnOff, SPI_AUTO_VSYNC_EN);
}

void vDrvLcdimSpiSetGlobalDimming(UINT16 u2Duty, UINT8 u1Freq)
{
	vIO32WriteFldAlign(ADAP_LCDIM_SPI,(UINT8)u2Duty,LCDIM_DUTY);    

    // For Scanning Backlight Ref HSync
    if ((u1Freq== 25) || (u1Freq == 50) || (u1Freq == 100))
    {
        vDrvSetPWM(HSYNC_SRC, (BSP_GetDomainClock(SRC_BUS_CLK)/255) / 52000 , 0x80, 0xFF); 
    }
    else
    {
        vDrvSetPWM(HSYNC_SRC, (BSP_GetDomainClock(SRC_BUS_CLK)/255) / 63000, 0x80, 0xFF); 
    }
}

UINT8 vDrvLcdimSpiGetGlobalDimming(void)
{
    return IO32ReadFldAlign(ADAP_LCDIM_SPI, LCDIM_DUTY);
}

void vDrvLcdimSpiSetBlockNum(void)
{   
    _u2LcdimBlockNum = pParam->u1BlockNumM * pParam->u1BlockNumN;    
}

UINT16 u2DrvLcdimSpiGetBlockNum(void)
{
    if(_u2LcdimBlockNum > ((MAX_SPI_CMD_SIZE >> 1) - 1))
    {
        _u2LcdimBlockNum = (MAX_SPI_CMD_SIZE >> 1) - 1; // fix klockwork
    }
    
    return _u2LcdimBlockNum;
}

void vDrvLcdimSpiGetSystemInfo(void)
{
    LOG(0, "SPI Auto Vsync = %d\n", IO32ReadFldAlign(LED_CON, HW_TRIG_EN));
    LOG(0, "SPI SW Loop Enable = %d\n", IO32ReadFldAlign(ADAP_LCDIM_SPI, SPI_AUTO_VSYNC_EN));
    LOG(0, "SPI Protocol Index = %d\n", pParam->u1ProtocolIndex);
}

//====================================================================================
//                             General SPI Function
//====================================================================================
void vDrvLcdimSpiSetAttr(void)
{
    UINT16  u2ClkDiv;                       // CLK setting
    UINT16  Tvh, Twait;                     // VSync setting
    UINT8   Tch;
    UINT8   Tcld,Tclg;                      // CS setting

    //-------------------------------------------------------
    // 0. set sw / hw mode
    //-------------------------------------------------------    
    vIO32WriteFldAlign(LED_CON, !pParam->u1SWModeEn, SRAM_HW_MODE);
    vIO32WriteFldAlign(LED_CON, !pParam->u1SWModeEn, HW_TRIG_EN);   

    //-------------------------------------------------------
    // 1. set clock
    //-------------------------------------------------------    
    u2ClkDiv =  ((LDM_BUS_CLOCK*1000000/(pParam->u4OutputClock))-2)>>1;     // 20110221 todo : define LDM_BUSCLK
    
    vIO32WriteFldAlign(LED_CKCFG, u2ClkDiv, CLK_DIV);       //set clock divide value
    vIO32WriteFldAlign(LED_CKCFG, pParam->u1CPOL, CPOL);        //set clock polarity
    vIO32WriteFldAlign(LED_CKCFG, pParam->u1CPHA, CPHA);        //set clock phase    

    //-------------------------------------------------------
    // 2. set vsync
    //-------------------------------------------------------    
    Tvh     = (pParam->u1VSyncHighTime) * LDM_BUS_CLOCK ;
    Twait   = (pParam->u1WaitTime) * LDM_BUS_CLOCK ;

    vIO32WriteFldAlign(LED_VSACC, Tvh, VSYNC_HOLD_LATENCY);     //set Vsync high time
    vIO32WriteFldAlign(LED_VSACC, Twait, WAIT_LATENCY);         //set wait time from Vsync low to CS low

    //-------------------------------------------------------
    // 3. set cs                                                
    //-------------------------------------------------------    
    Tch =    LDM_BUS_CLOCK*(pParam->u2CSHighTime)/1000;
    Tcld =  (LDM_BUS_CLOCK*(pParam->u1Tcld)+500)/1000;        
    Tclg =  (LDM_BUS_CLOCK*(pParam->u1Tclg)+500)/1000;        

    vIO32WriteFldAlign(LED_CSACC2, Tch, CS_HOLD_LATENCY);	    //set CS high time
    vIO32WriteFldAlign(LED_CSACC, Tcld, CS_LEAD_LATENCY);	    //set setup time from CS low to CLK edge
    vIO32WriteFldAlign(LED_CSACC, Tclg, CS_LAG_LATENCY);	    //set hold time from CLK edge to CS high
    vIO32WriteFldAlign(LED_CSACC, 2, CS_LEAD_LATENCY);	        //set setup time from CS low to CLK edge
    vIO32WriteFldAlign(LED_CSACC, 2, CS_LAG_LATENCY);	        //set hold time from CLK edge to CS high


    vIO32WriteFldAlign(LED_CSCFG, pParam->u1HheadSingleEn, MHEAD_SINGLE_EN); 
    vIO32WriteFldAlign(LED_CSCFG, pParam->u1HdataSingleEn, MDATA_SINGLE_EN); 
    vIO32WriteFldAlign(LED_CSCFG, pParam->u1HfootSingleEn, MFOOT_SINGLE_EN);     
    vIO32WriteFldAlign(LED_CSCFG, pParam->u1VheadSingleEn, NHEAD_SINGLE_EN);
    vIO32WriteFldAlign(LED_CSCFG, pParam->u1VdataSingleEn, NDATA_SINGLE_EN);
    vIO32WriteFldAlign(LED_CSCFG, pParam->u1VfootSingleEn, NFOOT_SINGLE_EN);    

    //-------------------------------------------------------
    // 4. set frame rate setting
    //-------------------------------------------------------    
    vIO32WriteFldAlign(LED_CON, 1, SW_SET_DATA);    // write buffered setting into register

    vIO32WriteFldAlign(LED_VSCFG, 0, FRAME_RATE);             //set frame rate 
    vIO32WriteFldAlign(LED_VSCFG, 0, VSYNC_LATENCY);           //set vsync latency    

    vIO32WriteFldAlign(LED_CON, 0, SW_SET_DATA);       
    //-------------------------------------------------------
    // 5. misc default setting
    //-------------------------------------------------------
    vIO32WriteFldAlign(LED_FMT, 0, LSB_MODE);       //set LSB mode : (0) msb first  (1) lsb first
    vIO32WriteFldAlign(LED_FMT, 0, SPI_RGB_MODE);       //set RGB mode : (0) white mode (1) rgb repeat mode
    vIO32WriteFldAlign(LED_FMT, 0, LED_TYPE);       //set LED type : (0) default no special mode

    vIO32WriteFldAlign(LED_CON, 1, SW_SET_DATA);    // write buffered setting into register
    vIO32WriteFldAlign(LED_CON, 0, SW_SET_DATA);   

    if((pParam->u1SWModeEn == SV_FALSE)&&(pParam->u1ProtocolIndex == E_SPI_LGD_PROTOCOL))
    {
        vIO32WriteFldAlign(LED_NOB, pParam->u1BlockNumM*pParam->u1BlockNumN, MDATA_LEN);  
        vIO32WriteFldAlign(LED_NOB, 1, NDATA_LEN);          

        vIO32WriteFldAlign(LED_NOB, 2, NHEAD_LEN);  
        vIO32WriteFldAlign(LED_NOB, 1, NFOOT_LEN);  

        vIO32WriteFldAlign(LED_NHEAD, 0xAA, NHEAD_0);  
        vIO32WriteFldAlign(LED_NHEAD, 0x03, NHEAD_1); 

        vIO32WriteFldAlign(LED_FMT, 0x01, CUSTOM_FMT); 
    }
}

UINT8 u1DrvLcdimSpiRWProtect(void)
{
    UINT16 u1Raddr;

    u1Raddr = IO32ReadFldAlign(LED_RADR, SRAM_RADR);

    if(u1Raddr != 0)  // ISR congestion case, discarding this round.
    {
        LOG(4,"[SPI] Read/Write Conflict, Current Read Addr = %d\n", u1Raddr);
        return SV_FALSE;
    }    

    return SV_TRUE;
}

BOOL bDrvLcdimSpiSWInit(void)
{
    // 1. get panel attribute from panel table (defined in panel_table_cust.c)
    if (PANEL_GetLocalDimmingParam(&pParam) == SV_FALSE)
    {
        LOG(0, "[SPI] Fail To Retrieve Local Dimming Panel Attribute, SPI Initialization Fail\n");
        return SV_FAIL;
    }      
    // 2. set block number   
    vDrvLcdimSpiSetBlockNum();    

    // 4. choose command set
    if(pParam->u1ProtocolIndex == E_SPI_AMS_PROTOCOL)
    {
        sSpiFunc = &sAmsSpiFunc;
    }
    else if (pParam->u1ProtocolIndex == E_SPI_LGD_PROTOCOL)
    {
        sSpiFunc = &sLgdSpiFunc;
    }
    else
    {
        LOG(0, "[SPI] Protocol Not Support, SPI Initialization Fail.\n");
        return SV_FAIL;        
    }    

  	// 4. get panel order
#ifndef CC_MTK_LOADER        	
    // DRVCUST_GetLcdimPanelOrder(u2AMSDriverOrder, u2DrvLcdimSpiGetBlockNum());
#endif //CC_MTK_LOADER   

    return SV_SUCCESS;    
}

BOOL bDrvLcdimSpiHWInit(void)
{
    // 3. set panel attributes
    vDrvLcdimSpiSetAttr();
    
    sSpiFunc->vSpiLightUpCmd();    

    LOG(0, "[SPI] Init Success, Block H = %d, Block V = %d, Support Scanning = %d\n", pParam->u1BlockNumN, pParam->u1BlockNumM, pParam->u1ScanningOnoff);     

    return SV_SUCCESS;    
}

BOOL bDrvLcdimSpiInit(void)
{   
    return (bDrvLcdimSpiSWInit() & bDrvLcdimSpiHWInit());
}

UINT32 vDrvLcdimSpiWaitReady(UINT32 u4WaitCount)
{
    UINT32 i;

    for (i=0; i<u4WaitCount; i++)
    {
        if (IO32ReadFldAlign(LED_CON,LED_IDLE)==SV_TRUE)
            return SV_SUCCESS;
    }
    return SV_FAIL;
}

void vDrvLcdimSpiSendDataSWMode(UINT8 *u1DataArray, UINT16 u2Datalen)
{
    UINT32 i;
      
    vIO32WriteFldAlign(LED_NOB, u2Datalen, MDATA_LEN);  //set horizontal data len of Bytes
    vIO32WriteFldAlign(LED_NOB, 1, NDATA_LEN);          //set vertical data len of Bytes

    vIO32Write4B(LED_WADR, 0);                          //clean write address to 0

    for (i=0; i<u2Datalen; i++)
    {  
        // address will increase by 1 after every write to address LED_DATA
        vIO32Write4B(LED_DATA, u1DataArray[i]);
    }

    vIO32WriteFldAlign(LED_CON, 1, SW_TRIG);  // trigger SW_TRIG, 0x2b8[0:0]=1    

    vDrvLcdimSpiWaitReady(1000000);
}

void vDrvLcdimSpiSendDataHWMode(UINT8 *u1DataArray, UINT16 u2Datalen)
{
    UINT32 i;

    // avoid writing SRAM when SPI is sending data
    if(u1DrvLcdimSpiRWProtect() == SV_FALSE)
    {
        LOG(4,"[SPI Read] Wait Fail in Last Stage\n");
        return;
    }
        
    vIO32WriteFldAlign(LED_NOB, u2Datalen, MDATA_LEN);  //set horizontal data len of Bytes
    vIO32WriteFldAlign(LED_NOB, 1, NDATA_LEN);          //set vertical data len of Bytes

    vIO32Write4B(LED_WADR, 0);                          //clean write address to 0
    
    for (i=0; i<u2Datalen; i++)
    {  
        // address will increase by 1 after every write to address LED_DATA
        vIO32Write4B(LED_DATA, u1DataArray[i]);
    }
            
    vIO32Write4B(LED_RADR, 0);                          //clean read address to 0    
}

//====================================================================================
//                             SPI Command Interface
//====================================================================================
void vDrvLcdimSpi1BRead(UINT8 u1Addr)
{
    sSpiFunc->vSpiRead1BCmd(u1Addr);
}

void vDrvLcdimSpi1BWrite(UINT8 u1Addr, UINT8 u1Data)
{
    sSpiFunc->vSpiWrite1BCmd(u1Addr, u1Data);
}

void vDrvLcdimSpiCommandGen(UINT8 u1CommandSet)
{
    switch(u1CommandSet)
    {
        case E_SPI_PAGE_WRITE:             
            sSpiFunc->vSpiBatchWriteCmd();
            break;
        case E_SPI_READ_FAULT:             
            sSpiFunc->vSpiBatchReadCmd();
            break;                
        case E_SPI_LIGHT_UP:               
            sSpiFunc->vSpiLightUpCmd();
            break;
        case E_SPI_LOG_OFF:                
            sSpiFunc->vSpiLogOffCmd();                
            break;                
        case E_SPI_FAULT_REPAIR:
            sSpiFunc->vSpiRepairCmd();
            break;
        default:
            LOG(0, "Command Not Support\n");                
            break;
    }
}

