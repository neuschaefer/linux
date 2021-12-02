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
//#include "hw_bim.h"
//#include "drv_kr.h"
//#include "hw_reg.h"
//#include "custom_def.h"

//#include "hw_rs232.h"
#if SIF_DRV_SUPPORT

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

#define CC_SIF_MULTIPLE_DATA_BYTE_ACCESS

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
#define  SIF_OK    (0)

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
data SIF_BUS_STATUS_T SifObj;
UINT8 SIF_ADRESS[2];

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

// Wait tirgger bit becomes 0
static INT8 _SIF_WaitHwDone(void)
{
    UINT8 u1Reg;
    u1Reg = TRI;

    while (u1Reg & TRI)
    {
        u1Reg = u1IO32Read1B(PDWNC_SM0CTL1);
    }

    return SIF_OK;
}

/*
static SIF_BUS_STATUS_T _SIF_CheckBus (void)
{
    UINT8 u1Reg;
    SIF_BUS_STATUS_T tRet;

    u1Reg = u1IO32Read1B(PDWNC_SM0CTL0);
    tRet.SIF_BUS_SDA = (UINT8) ((u1Reg & SDA_STATE_MASK) ? 1 : 0);
    tRet.SIF_BUS_SCL = (UINT8) ((u1Reg & SCL_STATE_MASK) ? 1 : 0);

    return tRet;
};
*/

// Send one start bit
static INT8 _SIF_PutStartBit(void)
{
    vRegWriteFldAlign(PDWNC_SM0CTL1, MODE_START, FLD_SIF_MODE);
    vRegWriteFldAlign(PDWNC_SM0CTL1, 1, FLD_TRI);
    return _SIF_WaitHwDone();
}

// Send one stop bit
static INT8 _SIF_PutStopBit(void)
{
    vRegWriteFldAlign(PDWNC_SM0CTL1, MODE_STOP, FLD_SIF_MODE);
    vRegWriteFldAlign(PDWNC_SM0CTL1, 1, FLD_TRI);
    return _SIF_WaitHwDone();

}


// Send one data byte
// Return: 1 success, 0 fail, <0 error.
static INT8 _SIF_PutByte(UINT8 u1Data)
{
    INT8 iRet = 0;
    // fill data port
    vRegWriteFldAlign(PDWNC_SM0D0, u1Data, FLD_DATA0);

    // fill control word
    vRegWriteFldAlign(PDWNC_SM0CTL1, 0, FLD_PGLEN);
    vRegWriteFldAlign(PDWNC_SM0CTL1, MODE_WR_DATA, FLD_SIF_MODE);    
    vRegWriteFldAlign(PDWNC_SM0CTL1, 1, FLD_TRI);

    // wait for hardware
    iRet = _SIF_WaitHwDone();
    if(iRet != SIF_OK)
    {
#if UART_DBG        
        LogS("SIF put byte: sema time out ");
#endif
        return iRet;
    }
    return ((SIF_ACK() & 0x01) ? 1 : 0);
}

#ifdef CC_SIF_MULTIPLE_DATA_BYTE_ACCESS
// Read multiple data bytes
// Return: how many bytes were read, 0 fail
static INT8 _SIF_GetMultipleBytes(UINT8 *pu1Data, INT8 iBytes)
{
//    UINT8 u4Reg, u4Msk;
    INT8 iRet, i, j, iSplitBytes, iTotalLoops;
    //UINT32 u4D0,u4D1;//

    iRet = 0;
     //    iTotalLoops = (iBytes + SIF_PAGE_SIZE - 1) / SIF_PAGE_SIZE;
   iTotalLoops = (iBytes + SIF_PAGE_SIZE - 1) >> SIF_PAGE_SIZE_POW;

    for (i = 0; i < iTotalLoops; i++)
    {
        // Last <= SIF_PAGE_SIZE bytes
        if (i == (iTotalLoops - 1))
        {
//            iSplitBytes = iBytes % SIF_PAGE_SIZE;
            iSplitBytes = iBytes & (SIF_PAGE_SIZE - 1);            
        }
        else
        {
            iSplitBytes = (INT8) SIF_PAGE_SIZE;
        }

        if (iSplitBytes == 0)
        {
            iSplitBytes = (INT8) SIF_PAGE_SIZE;
        }

        if (i == (iTotalLoops - 1))
        {
            vRegWriteFldAlign(PDWNC_SM0CTL1, iSplitBytes - 1, FLD_PGLEN);
            vRegWriteFldAlign(PDWNC_SM0CTL1, MODE_RD_DATA_LAST, FLD_SIF_MODE);    
            vRegWriteFldAlign(PDWNC_SM0CTL1, 1, FLD_TRI);
        }
        else
        {
            vRegWriteFldAlign(PDWNC_SM0CTL1, iSplitBytes - 1, FLD_PGLEN);
            vRegWriteFldAlign(PDWNC_SM0CTL1, MODE_RD_DATA, FLD_SIF_MODE);    
            vRegWriteFldAlign(PDWNC_SM0CTL1, 1, FLD_TRI);
        }
        // fill control word
        // wait for hardware
        j = _SIF_WaitHwDone();
        if(j != SIF_OK)
        {
#if UART_DBG            
            LogS("SIF get mutiple byte: sema time out ");
#endif
            return j;
        }
#if 0
        u4D0 = u4RegRead4B(PDWNC_SM0D0);//SIF_READ32(RW_SM0_D0_REG);
        u4D1 = u4RegRead4B(PDWNC_SM0D0);//_SIF_READ32(RW_SM0_D1_REG);

        // save to buffer
        for (j = 0; j < ((iSplitBytes > 4) ? 4 : iSplitBytes); j++)
        {
            *pu1Data = (UINT8) (u4D0 & 0xFF);
            u4D0 >>= 8;
            pu1Data++;
            iRet++;
        }

        if (iSplitBytes > 4)
        {
            for (j = 0; j < (iSplitBytes - 4); j++)
            {
                *pu1Data = (UINT8) (u4D1 & 0xFF);
                u4D1 >>= 8;
                pu1Data++;
                iRet++;
            }
        }
#else
        for (j = 0; j < iSplitBytes; j++)
        {
            *(pu1Data + j) = u1RegRead1B(PDWNC_SM0D0 + j);
        }
        pu1Data += iSplitBytes;
        iRet += iSplitBytes;                  
#endif
    }

    return iRet;
}
#else // CC_SIF_MULTIPLE_DATA_BYTE_ACCESS
// Get one data byte from SIF
static INT8 _SIF_GetByte(UINT8 *pu1Data, BIT fgLast)
{
    //UINT32 u4Reg, u4Msk;
    INT8 iRet = 0;
    // fill control word
    vRegWriteFldAlign(PDWNC_SM0CTL1, 0, FLD_PGLEN);
    if (fgLast == TRUE) //last byte
    {
        vRegWriteFldAlign(PDWNC_SM0CTL1, MODE_RD_DATA_LAST, FLD_SIF_MODE);    
    }
    else
    {
        vRegWriteFldAlign(PDWNC_SM0CTL1, MODE_RD_DATA, FLD_SIF_MODE);    
    }
    vRegWriteFldAlign(PDWNC_SM0CTL1, 1, FLD_TRI);

    // wait for hardware
    iRet = _SIF_WaitHwDone();
    if(iRet != SIF_OK)
    {
#if UART_DBG
        LogS("SIF get byte: sema time out ");
#endif
        return iRet;
    }

    *pu1Data = u1RegRead1B(PDWNC_SM0D0);
    return iRet;
}
#endif // CC_SIF_MULTIPLE_DATA_BYTE_ACCESS


#ifdef CC_SIF_MULTIPLE_DATA_BYTE_ACCESS
// Send multiple data bytes
// Return: how many bytes were written, 0 fail, <1 error
static INT8 _SIF_PutMultipleBytes(UINT8 *pu1Data, INT8 iBytes)
{
#if 1    
#else
    UINT32 u4D0, u4D1; 
#endif
    INT8 iRet, i, j, iSplitBytes, iTotalLoops;

    iRet = 0;
//    iTotalLoops = (iBytes + SIF_PAGE_SIZE - 1) / SIF_PAGE_SIZE;
    iTotalLoops = (iBytes + SIF_PAGE_SIZE - 1) >> SIF_PAGE_SIZE_POW;

    for (i = 0; i < iTotalLoops; i++)
    {
        // Last <= SIF_PAGE_SIZE bytes
        if (i == (iTotalLoops - 1))
        {
//            iSplitBytes = iBytes % SIF_PAGE_SIZE;
            iSplitBytes = iBytes & (SIF_PAGE_SIZE - 1);
        }
        else
        {
            iSplitBytes = (INT8) SIF_PAGE_SIZE;
        }

        if (iSplitBytes == 0)
        {
            iSplitBytes = (INT8) SIF_PAGE_SIZE;
        }
#if 1
        // setup data bytes
        for (j = 0; j < iSplitBytes; j++)
        {
            vIO32Write1B(PDWNC_SM0D0 + j, *(pu1Data + j));
        }
        pu1Data += iSplitBytes;
#else
        u4D0 = 0;
        u4D1 = 0;

        // setup data bytes
        for (j = 0; j < ((iSplitBytes > 4) ? 4 : iSplitBytes); j++)
        {
            u4D0 |= ((*(pu1Data + j) << (8 * j)) & (0xFF << (8 * j)));
        }

        if (iSplitBytes > 4)
        {
            for (j = 0; j < (iSplitBytes - 4); j++)
            {
                u4D1 |= ((*(pu1Data + 4 + j) << (8 * j)) & (0xFF << (8 * j)));
            }
        }

        pu1Data += iSplitBytes;

        // fill data port
        vIO32Write4B(PDWNC_SM0D0, u4D0);
        vIO32Write4B(PDWNC_SM0D1, u4D1);
#endif

        // fill control word
        vRegWriteFldAlign(PDWNC_SM0CTL1, iSplitBytes - 1, FLD_PGLEN);
        vRegWriteFldAlign(PDWNC_SM0CTL1, MODE_WR_DATA, FLD_SIF_MODE);    
        vRegWriteFldAlign(PDWNC_SM0CTL1, 1, FLD_TRI);

        j = 0;
        // wait for hardware
        j = _SIF_WaitHwDone();
        if(j != SIF_OK)
        {
#if UART_DBG            
            LogS("SIF put mutiple byte time out ");
#endif
            return j;
        }

        j = SIF_ACK();
        j &= 0xFF;

        // count how many acks
        switch (j)
        {
            case 0x00:
                j = 0;
                break;

            case 0x01:
                j = 1;
                break;

            case 0x03:
                j = 2;
                break;

            case 0x07:
                j = 3;
                break;

            case 0x0F:
                j = 4;
                break;

            case 0x1F:
                j = 5;
                break;

            case 0x3F:
                j = 6;
                break;

            case 0x7F:
                j = 7;
                break;

            case 0xFF:
                j = 8;
                break;

            default:            	  
                LogSB("SIF put mutiple byte: Abnormal device ack sequence", j);            	
                j = 0;
                // ASSERT(0);
        }

        iRet += j;

        if (j == 0)
        {
            return iRet;
        }
    }

    return iRet;
}
#endif // CC_SIF_MULTIPLE_DATA_BYTE_ACCESS


/** _SIF_OptRead
 *  Read data with options
 *  @param  u2ClkDiv    Parameter of divider to divide 27Mhz for SCL.
 *  @param  u1DevAddr   Serial interface device address.
 *  @param  pu1DataBuf  Pointer to user's buffer.
 *  @param  u4DataLen   Number of bytes to read.
 *  @param  bus_ctrl    Options for read
 *  @retval > 0         Actual number of bytes read.
 *  @retval = 0         Device acked
 *  @retval < 0         Error
 */
static INT8 _SIF_OptRead(UINT8 u1DevAddr, UINT8 *pu1DataBuf, UINT8 u1DataLen,
                          SIF_BUS_CONTROL_T bus_ctrl)
{
#ifndef CC_SIF_MULTIPLE_DATA_BYTE_ACCESS
    bit fgLastByte = FALSE;
#endif
    INT8 iCnt = 0, iRet = 0;

    if (!(bus_ctrl & SIF_CTL_NO_START))
    {
        iRet = _SIF_PutStartBit();
        if (iRet != SIF_OK)                 // start
        {
#if UART_DBG
           LogSB("_SIF_OptRead: Start Bit error", iRet);
#endif
           return iRet;
        }
        if (_SIF_PutByte(u1DevAddr) != 1)   // dev addr
        {
#if UART_DBG            
            LogSB("_SIF_OptRead: dev addr does not ack", u1DevAddr);
#endif
            // stop
            if (!(bus_ctrl & SIF_CTL_NO_STOP))
            {
                iRet = _SIF_PutStopBit();
                if ( iRet != SIF_OK)               
                {
#if UART_DBG                    
                    LogSB("_SIF_OptRead: Stop Bit error", iRet);
#endif
                    return iRet;
                }
            }

            return SIF_ERROR_NODEV;
        }
    }


    // data
#ifdef CC_SIF_MULTIPLE_DATA_BYTE_ACCESS
    iCnt = _SIF_GetMultipleBytes(pu1DataBuf, (INT8) u1DataLen);
    if (iCnt != (INT8) u1DataLen)
    {
#if UART_DBG        
        LogSB("_SIF_OptRead: only byte were read", iCnt);
#endif
    }
#else // CC_SIF_MULTIPLE_DATA_BYTE_ACCESS
    while (iCnt < u1DataLen)
    {
        if (iCnt == (u1DataLen - 1))
        {
            fgLastByte = TRUE;
        }
        iRet = _SIF_GetByte((pu1DataBuf + iCnt), fgLastByte);
        if (iRet != SIF_OK)
        {
#if UART_DBG            
            LogSB("_SIF_OptRead: terminated at byte due to no ACK",iCnt );
#endif
            break;
        }
        iCnt += 1;
    }
#endif // CC_SIF_MULTIPLE_DATA_BYTE_ACCESS

    // stop
    if (!(bus_ctrl & SIF_CTL_NO_STOP))
    {
        iRet = _SIF_PutStopBit();
        if (iRet != SIF_OK)               
        {
#if UART_DBG            
            LogSB("_SIF_OptRead: Stop Bit error", iRet);
#endif
            return iRet;
        }
    }

    return iCnt;
}


/** _SIF_OptWrite
 *  Write data with options
 *  @param  u2ClkDiv    Parameter of divider to divide 27Mhz for SCL.
 *  @param  u1DevAddr   Serial interface device address.
 *  @param  pu1DataBuf  Pointer to user's buffer.
 *  @param  u4DataLen   Number of bytes to read.
 *  @param  bus_ctrl    Options for read
 *  @retval > 0         Actual number of bytes (sub-address + data) written.
 *  @retval = 0         Device acked
 *  @retval < 0         Error
 */
static INT8 _SIF_OptWrite(UINT8 u1DevAddr, UINT8 *pu1DataBuf, UINT8 u1DataLen,
                           SIF_BUS_CONTROL_T bus_ctrl)
{
    INT8 iCnt = 0, iRet = 0;

    if (!(bus_ctrl & SIF_CTL_NO_START))
    {
        iRet = _SIF_PutStartBit();
        if (iRet != SIF_OK)                 // start
        {
#if UART_DBG
            LogSB("_SIF_OptWrite: Start Bit error", iRet);
#endif
            return iRet;
        }

        if (_SIF_PutByte(u1DevAddr) != 1)   // dev addr
        {
#if UART_DBG            
            LogSB("_SIF_OptWrite: dev addr does not ack", u1DevAddr);
#endif
            // stop
            if (!(bus_ctrl & SIF_CTL_NO_STOP))
            {
                iRet = _SIF_PutStopBit();
                if (iRet != SIF_OK)                
                {
#if UART_DBG                    
                    LogSB("_SIF_OptWrite: Stop Bit error", iRet);
#endif
                    return iRet;
                }
            }

            return SIF_ERROR_NODEV;
        }
    }

#ifdef CC_SIF_MULTIPLE_DATA_BYTE_ACCESS
    // data
    iCnt = _SIF_PutMultipleBytes(pu1DataBuf, (INT8) u1DataLen);
    if (iCnt != (INT8) u1DataLen)
    {
#if UART_DBG         
        LogSB("_SIF_OptWrite: only byte were written", iCnt);
#endif
    }
#else // CC_SIF_MULTIPLE_DATA_BYTE_ACCESS
    // data
    for (iCnt = 0; iCnt < u1DataLen; iCnt++)
    {
        if (_SIF_PutByte(*(pu1DataBuf + iCnt)) != 1)
        {
#if UART_DBG            
            LogSB("_SIF_OptWrite: terminated at byte due to no ACK.", iCnt);
#endif
            break;
        }
    }
#endif // CC_SIF_MULTIPLE_DATA_BYTE_ACCESS

    // stop
    if (!(bus_ctrl & SIF_CTL_NO_STOP))
    {
        iRet = _SIF_PutStopBit();
        if (iRet != SIF_OK)               
        {
#if UART_DBG
            LogSB("_SIF_OptWrite: Stop Bit error", iRet);
#endif
            return iRet;
        }
    }

    return iCnt;
}

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Interface(exported) functions
//-----------------------------------------------------------------------------
#if SIF_PDWNC_TEST
UINT8 SIF_RDATA[SIF_BUFFER_SIZE],SIF_WDATA[SIF_BUFFER_SIZE];
SIF_REQ_OBJ pSifObjTest;
void PDDataSet(UINT8 u1data_write)
{
    UINT16 i;
    #if UART_DBG
    LogS("PDDataSet u1data_write=");
    LogB(u1data_write);
    #endif
    for(i=0;i<SIF_BUFFER_SIZE;i++)
        {
        SIF_WDATA[i]=u1data_write;
        }
    
}
#endif

void vSIF_Init(void)
{
    if(SifObj.fgInit != 1)
    {
        MSetSifSclStrech(SIF_SCL_STRECH);
        MSetSifODrain(SIF_ODRAIN);
        MSetSifPinmux();
        SifObj.SIF_BUS_LOCK = 0;
        SifObj.fgInit = 1;
    }
    return;
}
#if SIF_PDWNC_TEST
UINT8 PD_SIF_Write_Test(UINT8 pdu1WordAddr1st,UINT8 pdu1WordAddr2nd,UINT8 pdbDataLen,UINT8 pddata)//need disable eeprom wp before test
{
        
    UINT8 u1val;    
    pSifObjTest.u1DevAddr=EEPROM_DEV_ADDR;
    pSifObjTest.u1WordAddrNum=EEPROM_SUB_ADDR_SIZE;
    pSifObjTest.u1DevClkDiv=EEPROM_CLK_DIV;
    pSifObjTest.u1WordAddr1st=pdu1WordAddr1st; 
    pSifObjTest.u1WordAddr2nd=pdu1WordAddr2nd; 
    pSifObjTest.bDataLen=pdbDataLen; //pdbDataLen should lest than SIF_BUFFER_SIZE
   
    PDDataSet(pddata);
    pSifObjTest.pDataBuf=SIF_WDATA;
    u1val = bSIF_Write(&pSifObjTest);
    #if UART_DBG
    LogS("bSIF_Write result");
    LogB(u1val);
    #endif
    return (u1val != pdbDataLen);//0:PASS;1:FAIL
}
UINT8 PD_SIF_Read_Test(UINT8 pdu1WordAddr1st,UINT8 pdu1WordAddr2nd,UINT8 pdbDataLen)
{
    
    UINT8 u1val;
    #if UART_DBG
    UINT8 i;
    #endif
    pSifObjTest.u1DevAddr=EEPROM_DEV_ADDR;
    pSifObjTest.u1WordAddrNum=EEPROM_SUB_ADDR_SIZE;
    pSifObjTest.u1DevClkDiv=EEPROM_CLK_DIV;
    pSifObjTest.u1WordAddr1st=pdu1WordAddr1st; 
    pSifObjTest.u1WordAddr2nd=pdu1WordAddr2nd; 
    pSifObjTest.bDataLen=pdbDataLen; //pdbDataLen should lest than SIF_BUFFER_SIZE  
    pSifObjTest.pDataBuf=SIF_RDATA;
    u1val=bSIF_Read(&pSifObjTest);
    #if UART_DBG
    LogS("bSIF_Read data:");
    for(i=0;i<u1val;i++)
        {
        LogB(SIF_RDATA[i]);        
        }
    LogS("bSIF_Read result");
    LogB(u1val);
    #endif
    return (u1val != pdbDataLen);//0:PASS;1:FAIL
}

#endif

static bit _bLOCK_SIF_BUS(void)
{
    MDisableINT();
    if(SifObj.SIF_BUS_LOCK == 1)
    {
#if UART_DBG
        LogS("Bus lock");
#endif
        return 0;
    }
    SifObj.SIF_BUS_LOCK = 1;
    MEnableINT();
    return 1;
}

static void _bUnLOCK_SIF_BUS(void)
{
    MDisableINT();
    SifObj.SIF_BUS_LOCK = 0;
    MEnableINT();
}


INT8 bSIF_Write(SIF_REQ_OBJ* pSifObj)
{
    INT8 iRet = SIF_ERROR_PARAM;
    if(!_bLOCK_SIF_BUS())
    {
#if UART_DBG
                LogS("Sif bus locked");
#endif
        return 0;
    }
#if UART_DBG  
    LogSB("pSifObj->bDataLen ",pSifObj->bDataLen);
#endif
    //Due to performance issue, we don't accept too much transfer per request... 
    //DataLen should lest than SIF_BUFFER_SIZE 
    if(((pSifObj->bDataLen) > SIF_BUFFER_SIZE) || ((pSifObj->bDataLen) == 0))//sif_lili
    {
#if UART_DBG
        LogS("payload is too much");
#endif
        _bUnLOCK_SIF_BUS();
        return 0;
    }
    
    vSIF_Init();
    MSetSifSclClkDIV(pSifObj->u1DevClkDiv);
    SifObj.SIF_BUS_SCL = RegReadFldAlign(PDWNC_SM0CTL0, FLD_SCL_STATE);
    SifObj.SIF_BUS_SDA = RegReadFldAlign(PDWNC_SM0CTL0, FLD_SDA_STATE);
    if ((SifObj.SIF_BUS_SCL == 0) || (SifObj.SIF_BUS_SDA == 0))
    {
        iRet = _SIF_PutStopBit();
        if (iRet != SIF_OK)               
        {
#if UART_DBG
            LogS(" Err: stop bit");
#endif
        }
        SifObj.SIF_BUS_SCL = RegReadFldAlign(PDWNC_SM0CTL0, FLD_SCL_STATE);
        SifObj.SIF_BUS_SDA = RegReadFldAlign(PDWNC_SM0CTL0, FLD_SDA_STATE);
        if ((SifObj.SIF_BUS_SCL == 0) || (SifObj.SIF_BUS_SDA == 0)) // still bus busy...
        {
#if UART_DBG
            LogS(" Err: SIF bus busy");
#endif
            iRet = SIF_ERROR_BUS_BUSY;                             // let's return error
        }
    }

    // put sub address and data
    if ((SifObj.SIF_BUS_SCL == 1) && (SifObj.SIF_BUS_SDA == 1))     // bus free
    {
        // sub address
          if  (pSifObj->u1WordAddrNum)  
          {
              if  (pSifObj->u1WordAddrNum == 1)
                {
                SIF_ADRESS[0]=pSifObj->u1WordAddr1st;
                }
                else if(pSifObj->u1WordAddrNum == 2)
                {
                SIF_ADRESS[0]=pSifObj->u1WordAddr1st;
                SIF_ADRESS[1]=pSifObj->u1WordAddr2nd;
                }
              iRet = _SIF_OptWrite(pSifObj->u1DevAddr,SIF_ADRESS, pSifObj->u1WordAddrNum, SIF_CTL_NO_STOP);//        //&(pSifObj->u1WordAddr1st)
              if (iRet != (INT8) (pSifObj->u1WordAddrNum))
              {
                  iRet = _SIF_PutStopBit();
                  if (iRet != SIF_OK) 
                  {
                    #if UART_DBG
                        LogS(" Stop Bit error");
                    #endif                      
                      //can't return,because of after callback. 
                  }
                  iRet = SIF_ERROR_SUBADDR;
              }
              else
              {
                  iRet = _SIF_OptWrite(pSifObj->u1DevAddr, pSifObj->pDataBuf, pSifObj->bDataLen, SIF_CTL_NO_START);
                  if (iRet != (INT8) (pSifObj->bDataLen))
                  {
                      iRet = SIF_ERROR_DATA;
                  }
              }
          }
          else
          {
              iRet = _SIF_OptWrite(pSifObj->u1DevAddr, pSifObj->pDataBuf, pSifObj->bDataLen,  SIF_CTL_NORMAL);
              if (iRet != (INT8) (pSifObj->bDataLen))
              {
                  iRet = SIF_ERROR_DATA;
              }
          }

    }
    _bUnLOCK_SIF_BUS();
    return iRet;

}

INT8 bSIF_Read(SIF_REQ_OBJ* pSifObj)
{
    INT8 iRet = SIF_ERROR_BUS_BUSY;    

    if(!_bLOCK_SIF_BUS())
    {
#if UART_DBG
                LogS("Sif bus locked");
#endif
        return 0;
    }
 #if UART_DBG  
    LogSB("pSifObj->bDataLen ",pSifObj->bDataLen);
 #endif
    //Due to performance issue, we don't accept too much transfer per request... 
    //DataLen should lest than SIF_BUFFER_SIZE 
    if(((pSifObj->bDataLen) > SIF_BUFFER_SIZE) || ((pSifObj->bDataLen) == 0))
    {
#if UART_DBG
        LogS("payload is too much");
#endif
        _bUnLOCK_SIF_BUS();
        return 0;
    }

    vSIF_Init();
    MSetSifSclClkDIV(pSifObj->u1DevClkDiv);    
    // check bus status
    SifObj.SIF_BUS_SCL = RegReadFldAlign(PDWNC_SM0CTL0, FLD_SCL_STATE);
    SifObj.SIF_BUS_SDA = RegReadFldAlign(PDWNC_SM0CTL0, FLD_SDA_STATE);

    if ((SifObj.SIF_BUS_SCL == 0) || (SifObj.SIF_BUS_SDA == 0))
    {
        iRet = _SIF_PutStopBit();
        if (iRet != SIF_OK)               
        {
#if UART_DBG
            LogSB("_SIF_DrvRawRead: Stop Bit error", iRet);
#endif
            //can't return,because of after callback.
        }
        SifObj.SIF_BUS_SCL = RegReadFldAlign(PDWNC_SM0CTL0, FLD_SCL_STATE);
        SifObj.SIF_BUS_SDA = RegReadFldAlign(PDWNC_SM0CTL0, FLD_SDA_STATE);
        if ((SifObj.SIF_BUS_SCL == 0) || (SifObj.SIF_BUS_SDA == 0)) // still bus busy...
        {
#if UART_DBG
            LogSB("SIF_ERROR_BUS_BUSY", iRet);
#endif
            iRet = SIF_ERROR_BUS_BUSY;                             // let's return error
        }
    }
    if ((SifObj.SIF_BUS_SCL == 1) && (SifObj.SIF_BUS_SDA == 1))     // bus free
    {
        if (pSifObj->u1WordAddrNum)                                             // sub-addr
        {
            if  (pSifObj->u1WordAddrNum == 1)
            {
            SIF_ADRESS[0]=pSifObj->u1WordAddr1st;
            }
            else if(pSifObj->u1WordAddrNum == 2)
            {
            SIF_ADRESS[0]=pSifObj->u1WordAddr1st;
            SIF_ADRESS[1]=pSifObj->u1WordAddr2nd;
            }
            iRet = _SIF_OptWrite(pSifObj->u1DevAddr,SIF_ADRESS, pSifObj->u1WordAddrNum, SIF_CTL_NO_STOP);// &(pSifObj->u1WordAddr1st)
            if (iRet == (INT8)(pSifObj->u1WordAddrNum))
            {
                iRet = _SIF_OptRead((pSifObj->u1DevAddr) | 0x1, pSifObj->pDataBuf, pSifObj->bDataLen, SIF_CTL_NORMAL);
                if (pSifObj->bDataLen != (UINT8) iRet)
                {
                    iRet = SIF_ERROR_DATA;
                }
            }
            else                                                    // status != SIF_OK
            {
                iRet = _SIF_PutStopBit();
                if (iRet!= SIF_OK)               
                {
#if UART_DBG
                    LogSB("_SIF_DrvRawRead: Stop Bit error", iRet);
                    //can't return,because of after callback.
#endif
                }
                iRet = SIF_ERROR_SUBADDR;
            }
        }
        else                                                        // No sub-addr
        {
            iRet = _SIF_OptRead((pSifObj->u1DevAddr) | 0x1, pSifObj->pDataBuf, pSifObj->bDataLen, SIF_CTL_NORMAL);
              if (pSifObj->bDataLen != (UINT8) iRet)
            {
                iRet = SIF_ERROR_DATA;
            }
        }

    }
    _bUnLOCK_SIF_BUS();
    return iRet;
}

#endif
