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

/*****************************************************************************
 *
 * Filename:
 * ---------
 *   $Workfile: Services.c $ 
 *
 * Project:
 * --------
 *   MT5387
 *
 * Description:
 * ------------
 *   T8032 main loop
 *   
 * Author:
 * -------
 *   David Hsieh
 *
 * Last changed:
 * ------------- 
 * $Author: dtvbm11 $ 
 *
 * $Modtime: 04/05/27 10:47p $    
 *
 * $Revision: #1 $
****************************************************************************/
//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "general.h"
#include "hw_bim.h"
#include "drv_kr.h"
#include "hw_reg.h"
//#include "hw_rs232.h"
#include "custom_def.h"

#if EEPROM_DRV_SUPPORT

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
#define EEP_SIF_MSB_PAGE_MASK (0xff)
#define EEP_SIF_PAGE_MASK (0xff)
#define _u1EEPROMPage SIF_PAGE_SIZE

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Prototypes
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
EEPROM_DRV_OBJ _EepromObj;
#if (RW_REG_INTERFACE&&SIF_PDWNC_TEST)
UINT8 xdata u1pdsiffg _at_ 0xF800;
UINT8 xdata u1pdsiflen _at_ 0xF801;
UINT8 xdata u1pdsifdata _at_ 0xF802;
#endif
//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
static bit _bSetEepromInUse(void)
{
    MDisableINT();
    if(_EepromObj.fgInUse == 1)
    {
        MEnableINT();
        return 1;
    }
    _EepromObj.fgInUse = 1;
    MEnableINT();
    return 0;
}

static void _EnableWP(void)
{
    if(_EepromObj.u1WP_Gpio_polarity == 0)
    {
        ClrGPIOOUT(_EepromObj.u1WP_Gpio);
    }
    else
    {
        SetGPIOOUT(_EepromObj.u1WP_Gpio);
    }
}

static void _DisableWP(void)
{
    if(_EepromObj.u1WP_Gpio_polarity == 0)
    {
        SetGPIOOUT(_EepromObj.u1WP_Gpio);
    }
    else
    {
        ClrGPIOOUT(_EepromObj.u1WP_Gpio);
    }
}


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------
#define MClrEepromInUse() (_EepromObj.fgInUse = 0) 

#if SIF_PDWNC_TEST
UINT8 SIF_EEP_RDATA[SIF_BUFFER_SIZE],SIF_EEP_WDATA[SIF_BUFFER_SIZE]; 
void PDEepDataSet(UINT8 u1data_write)
{
    UINT16 i;
    #if UART_DBG
    LogS("PDEepDataSet u1data_write=");
    LogB(u1data_write);
    #endif
    for(i=0;i<SIF_BUFFER_SIZE;i++)
    {
    SIF_EEP_WDATA[i]=u1data_write;
    }
    
}
#endif

//-----------------------------------------------------------------------------
// Interface(exported) functions
//-----------------------------------------------------------------------------
/*
UINT8 *pGetEEPROM_Buf(void)
{
    return (UINT8*)(EEPROM_BUFFER + EEPROM_SUB_ADDR_SIZE);
}
*/
void vEEPROM_Init(void)
{
    if(_EepromObj.fgEepInit != 1)
    {
    _EepromObj.fgInUse = 0;
    _EepromObj.fgEepInit = 1;
    //change wp pinmux
    MSetSifEepromWpPinmux();//set opcrtl3 as gpio(WP)
    }
    _EepromObj.SifReqObj.u1DevAddr = EEPROM_DEV_ADDR;
    _EepromObj.SifReqObj.u1DevClkDiv = EEPROM_CLK_DIV;
    _EepromObj.SifReqObj.u1WordAddrNum = EEPROM_SUB_ADDR_SIZE;    
    _EepromObj.u1WP_Gpio = EEPROM_WP_GPIO_MASK;    
    _EepromObj.u1WP_Gpio_polarity= EEPROM_WP_GPIO_POLARITY;
    SetGPIO_AS_OUTPUT(_EepromObj.u1WP_Gpio);
    _EnableWP();// set wp polarity   
    
}
#if SIF_PDWNC_TEST
UINT8 PD_SIF_EEP_Write_Test(UINT8 pdu1WordAddr1st,UINT8 pdu1WordAddr2nd,UINT8 pdbDataLen,UINT8 pddata)
{
        
    UINT8 u1val;    
    UINT16 u2WordAddr;   
    PDEepDataSet(pddata);
    u2WordAddr = ((UINT16)pdu1WordAddr1st<<8)|((UINT16)pdu1WordAddr2nd);
    #if UART_DBG
    LogS("u2WordAddr");
    LogW(u2WordAddr);
    #endif
    u1val = bEEPROM_Write(u2WordAddr,pdbDataLen,SIF_EEP_WDATA);//pdbDataLen should lest than SIF_BUFFER_SIZE 
    #if RW_REG_INTERFACE
        u1pdsiffg=0;
    #endif
    return u1val;//0:PASS;1:FAIL
}
UINT8 PD_SIF_EEP_Read_Test(UINT8 pdu1WordAddr1st,UINT8 pdu1WordAddr2nd,UINT8 pdbDataLen)
{
    
    UINT8 u1val;
   #if UART_DBG
    UINT8 i;
    #endif 
    UINT16 u2WordAddr;
    u2WordAddr = ((UINT16)pdu1WordAddr1st<<8)|((UINT16)pdu1WordAddr2nd);
    #if UART_DBG
    LogS("u2WordAddr");
    LogW(u2WordAddr);
    #endif
    u1val = bEEPROM_Read(u2WordAddr,pdbDataLen,SIF_EEP_RDATA);//pdbDataLen should lest than SIF_BUFFER_SIZE 
    #if UART_DBG
    LogS("PD_SIF_EEP_Read_Test data:");
    for(i=0;i<pdbDataLen;i++)
        {
        LogB(SIF_EEP_RDATA[i]);        
        }
    #endif
    #if RW_REG_INTERFACE
        u1pdsiffg=0;
    #endif
    return u1val;//0:PASS;1:FAIL
}

#endif

//UINT16 for address range to 32767
INT8 bEEPROM_Write(UINT16 u2Offset, UINT8 bLen, UINT8* p1Buf)
{
    UINT8 u1Retry, u1Index = 0;
    INT8 iRet;
    
    vEEPROM_Init();
    
    if(_bSetEepromInUse())
    {
#if UART_DBG
       LogS("EEPROM is accessing!");
#endif
                MClrEepromInUse();
                return 1;
    }
    _DisableWP();

    while(u1Index < bLen)
    {

        if ( _EepromObj.SifReqObj.u1WordAddrNum == 1)
        {
            _EepromObj.SifReqObj.u1WordAddr1st = u2Offset & EEP_SIF_PAGE_MASK;   
           
           
           /// Device address LSB [b3, b2, b1] are from Data u2Offset MSB [A10,A9,A8].//((UINT8)((((u2Offset) >> 8) <<1) & 0x0E))
            
            // upper device address [b7 ~ b4] is fixed.
            _EepromObj.SifReqObj.u1DevAddr = ((UINT8)(((u2Offset) >> 7) & 0x0E))|(_EepromObj.SifReqObj.u1DevAddr);
           
            // EEPROM should align the boundary length of 8 bytes when do page write.
            _EepromObj.SifReqObj.bDataLen = _u1EEPROMPage - ((_EepromObj.SifReqObj.u1WordAddr1st) & (_u1EEPROMPage-1));
        }
        else if( _EepromObj.SifReqObj.u1WordAddrNum == 2)
        {
            _EepromObj.SifReqObj.u1WordAddr1st = (u2Offset >> 8) & EEP_SIF_MSB_PAGE_MASK;
            _EepromObj.SifReqObj.u1WordAddr2nd = u2Offset & EEP_SIF_PAGE_MASK;
            _EepromObj.SifReqObj.bDataLen = _u1EEPROMPage - ((_EepromObj.SifReqObj.u1WordAddr2nd) & (_u1EEPROMPage-1));            
        }
        _EepromObj.SifReqObj.pDataBuf = p1Buf;
        _EepromObj.SifReqObj.bDataLen = (UINT8)MIN((bLen-u1Index), _EepromObj.SifReqObj.bDataLen); 

        iRet = bSIF_Write(&(_EepromObj.SifReqObj));
        if(iRet > 0)
        {
            iRet = (UINT8)iRet;
        }
        else
        {
            iRet = 0;
        }
            
        if (iRet !=  _EepromObj.SifReqObj.bDataLen)
       {
           u1Retry++;
           #if UART_DBG
           LogSW( "[EEPROM] Write WARN at 0x%x", u2Offset);
           #endif
           // This is redundency disable write protection pin,
           // but it is useful when ARM or 8032 have bad operation on this pin
           _DisableWP();
           if (u1Retry > 10)
           {
                _EnableWP();
                MClrEepromInUse();
               return 1;
           }
       }
       else
       {
           u1Retry = 0;
           // exclude 2 bytes of word address.
       }
       vKrDelay10ms(2);
       
       u1Index += iRet;
       u2Offset += iRet;
       p1Buf += iRet;
    }
    _EnableWP();
    MClrEepromInUse();
    return 0;
}

INT8 bEEPROM_Read(UINT16 u2Offset, UINT8 bLen, UINT8* p1Buf)
{
    UINT8 u1Index = 0;
    INT8 iRet;
    vEEPROM_Init();
    if(_bSetEepromInUse())
    {
        #if UART_DBG
        LogS("EEPROM is accessing!");
        #endif
        MClrEepromInUse();
        return 1;
    }
    
    u1Index = 0;
    
    while (u1Index < bLen)
    {
        if ( _EepromObj.SifReqObj.u1WordAddrNum == 1)
        {
            _EepromObj.SifReqObj.u1WordAddr1st = u2Offset & EEP_SIF_PAGE_MASK;   
            
             /// Device address LSB [b3, b2, b1] are from Data u2Offset MSB [A10,A9,A8].//((UINT8)((((u2Offset) >> 8) <<1) & 0x0E))
            
            // upper device address [b7 ~ b4] is fixed.
            _EepromObj.SifReqObj.u1DevAddr = ((UINT8)(((u2Offset) >> 7) & 0x0E))|(_EepromObj.SifReqObj.u1DevAddr);
           // EEPROM should align the boundary length of 8 bytes when do page write.
            _EepromObj.SifReqObj.bDataLen = _u1EEPROMPage - ((_EepromObj.SifReqObj.u1WordAddr1st) & (_u1EEPROMPage-1));
        }
        else if( _EepromObj.SifReqObj.u1WordAddrNum == 2)
        {
            _EepromObj.SifReqObj.u1WordAddr1st = (u2Offset >> 8) & EEP_SIF_MSB_PAGE_MASK;
            _EepromObj.SifReqObj.u1WordAddr2nd = u2Offset & EEP_SIF_PAGE_MASK;
            _EepromObj.SifReqObj.bDataLen = _u1EEPROMPage - ((_EepromObj.SifReqObj.u1WordAddr2nd) & (_u1EEPROMPage-1));            
        }
        
        _EepromObj.SifReqObj.pDataBuf = p1Buf;
        _EepromObj.SifReqObj.bDataLen = (UINT8)MIN((bLen-u1Index), _EepromObj.SifReqObj.bDataLen); 
        iRet = bSIF_Read(&(_EepromObj.SifReqObj));
        if(iRet > 0)
        {
            iRet = (UINT8)iRet;
        }
        else
        {
            iRet = 0;
        }
        if (iRet !=  _EepromObj.SifReqObj.bDataLen)
        {
            #if UART_DBG
            LogSW( "[EEPROM] Read WARN at 0x%x", u2Offset);
            #endif
            MClrEepromInUse();
            return 1;
        }
        u1Index += iRet;
        u2Offset += iRet;
        p1Buf += iRet;
    }
    MClrEepromInUse();
    
    return 0;
}

#endif
