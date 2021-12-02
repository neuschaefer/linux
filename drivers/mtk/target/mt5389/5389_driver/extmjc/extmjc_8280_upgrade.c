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
 * $RCSfile: extmjc_8280_upgrade.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: $
 * $SWAuthor: $
 * $MD5HEX: $
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/

#ifdef DRV_SUPPORT_EXTMJC

#define FULL_ERASE_ENABLE 0
#if 1 //using I2C download
#include "extmjc_debug.h"
//#include "u_drv_cust.h"
#include "extmjc_if.h"
#include "extmjc_msg_hdlr.h"
#include "x_os.h"
#include "x_timer.h"
#include "drvcust_if.h"

#define BYTE3(arg)        (*((UINT8 *)&(arg) + 3))
#define BYTE2(arg)        (*((UINT8 *)&(arg) + 2))
#define BYTE1(arg)        (*((UINT8 *)&(arg) + 1))
#define BYTE0(arg)        (* (UINT8 *)&(arg))

/*---------------------------------------------------------------------------
 * Name
 *
 * Description      -
 * Input arguments  -
 * Output arguments -
 * Returns          -
 *---------------------------------------------------------------------------*/
static BOOL b_sf_upg_enable = FALSE;
static UINT32 u4_sf_erase_sector_bitmap=0;
static BOOL Mt8280SfWaitCmdReady(UINT8 u1Mask,UINT8 u1MaskValue,UINT32 u4MsecTimeout)
{
    INT32 u4_fail_count = 0;
    UINT8 u1_data;
	while(u4_fail_count<u4MsecTimeout) // 1 second timeout
	{
    	u1ExtmjcReadRegByI2C(0x600,&u1_data);
    	if ((u1_data & u1Mask) == u1MaskValue)
    	{
    	    return TRUE;
    	}
    	u4_fail_count++;
    	x_thread_delay(1);
    }
    return FALSE;
}

#if 0
static BOOL Mt8280SfRDSR(UINT8 *u1_rdsr)
{
	u1ExtmjcWriteRegByI2C(0x600, 0x02); //RDSR cmd
	if (Mt8280SfWaitCmdReady(0x02,0,1000) == TRUE)
	{
   	    u1ExtmjcReadRegByI2C(0x602,u1_rdsr);
   	    return TRUE;
    }
    return FALSE;
}
#endif

static BOOL Mt8280SfWRSR(UINT8 u1_wrsr)
{
	u1ExtmjcWriteRegByI2C(0x60d, 0x01); //SF WRSR cmd
	u1ExtmjcWriteRegByI2C(0x60c, u1_wrsr); //SF WRSR data
	u1ExtmjcWriteRegByI2C(0x601, 10);   //Bit count
	u1ExtmjcWriteRegByI2C(0x600, 0x04);
	return Mt8280SfWaitCmdReady(0x04,0,1000);
}

static BOOL Mt8280SfWREN(UINT32 u4MsecTimeout)
{
    	//INT32 u4_fail_count = 0;
    	//UINT8 u1_rdsr;
	u1ExtmjcWriteRegByI2C(0x60d, 0x06); //SF WREN cmd
	u1ExtmjcWriteRegByI2C(0x601, 0x08); //Bit count
	u1ExtmjcWriteRegByI2C(0x600, 0x04);
	if (Mt8280SfWaitCmdReady(0x04,0,u4MsecTimeout) == TRUE)
	{
		return TRUE;
	}

	return FALSE;
#if 0	
	while(u4_fail_count<u4MsecTimeout) // 1 second timeout
	{
	    Mt8280SfRDSR(&u1_rdsr);
	    if ((u1_rdsr & 0x02) != 0)
	    {
	        return TRUE;
	    }
    	u4_fail_count++;
    	x_thread_delay(1);
    };
    return FALSE;
#endif    	
}
static BOOL Mt8280SfBp(BOOL bBpOn) //set Block Protected
{
    	//UINT8 u1_rdsr;
    	//Mt8280SfRDSR(&u1_rdsr);
    	if (bBpOn == TRUE) // Flash Protection Enable
    	{
        	if(Mt8280SfWREN(1000)) // Flash Write Enable
    {
            		if (Mt8280SfWRSR(0x0c) == FALSE) // Set sFlash Write Status as 0
                		return FALSE;
       	         }else
        {
                return FALSE;
        }
    }
    	else // Flash Protection Disable
    {
       	if(Mt8280SfWREN(1000)) // Flash Write Enable
            	{
            		if (Mt8280SfWRSR(0x00) == FALSE) // Set sFlash Write Status as 0
                		return FALSE;
       	}else
        {
                return FALSE;
        }
    }
    return TRUE;
}
#if FULL_ERASE_ENABLE
static BOOL Mt8280SfEraseChip(UINT32 u4MsecTimeout)
{
    UINT8 u1_rdsr;

    Mt8280SfWREN(1000);// 1 second timeout
    Printf("Erasing chip\n");
    u1ExtmjcWriteRegByI2C(0x60d, 0xC7); //SF BULK ERASE cmd
	u1ExtmjcWriteRegByI2C(0x601, 0x08); //Bit count
	u1ExtmjcWriteRegByI2C(0x600, 0x04);
    INT32 u4_fail_count = 0;
	while(u4_fail_count<u4MsecTimeout) // 1 second timeout
	{
	    Mt8280SfRDSR(&u1_rdsr);
	    if ((u1_rdsr & 0x01) == 0) //WIP, Write In Progress
	    {
	        return TRUE;
	    }
    	u4_fail_count++;
    	x_thread_delay(1);
    };
    return FALSE;
}
#endif

static BOOL Mt8280SfEraseSector(UINT32 u4Address,UINT32 u4MsecTimeout)
{
#if 1
//	UINT8 u1_rdsr;
//    	INT32 u4_fail_count = 0;

    	if ((u4_sf_erase_sector_bitmap & (1<<(u4Address>>16)&0xff)) != 0)
    	{
    		Printf("already sector erased-return");
        	return TRUE; //already sector erased
    	}

    	Printf("\nErasing Sector 0x%04d\n",BYTE2(u4Address));
    	if(!Mt8280SfWREN(1000)) // Flash Write Enable
    	{
    		Printf("Flash WriteEnable Fail");
    	}

    	// Erase Bank0
    	Printf("\nErase Address: %x",u4Address);
    	u1ExtmjcWriteRegByI2C(0x60d, 0xD8); //SF Sector ERASE cmd
    	u1ExtmjcWriteRegByI2C(0x60c, BYTE2(u4Address)); //MSB address
    	u1ExtmjcWriteRegByI2C(0x60b, BYTE1(u4Address));
    	u1ExtmjcWriteRegByI2C(0x60a, BYTE0(u4Address)); //LSB address
	u1ExtmjcWriteRegByI2C(0x601, 0x20); //Bit count
	u1ExtmjcWriteRegByI2C(0x600, 0x04);
	return Mt8280SfWaitCmdReady(0x04,0,1000);
#else
    UINT8 u1_rdsr;
    INT32 u4_fail_count = 0;

    if ((u4_sf_erase_sector_bitmap & (1<<(u4Address>>16)&0xff)) != 0)
        return TRUE; //already sector erased

    Printf("Erasing Sector 0x%04d\n",BYTE2(u4Address));
    Mt8280SfWREN(1000);
    u1ExtmjcWriteRegByI2C(0x60d, 0xD8); //SF Sector ERASE cmd
    u1ExtmjcWriteRegByI2C(0x60c, BYTE2(u4Address)); //MSB address
    u1ExtmjcWriteRegByI2C(0x60b, BYTE1(u4Address));
    u1ExtmjcWriteRegByI2C(0x60a, BYTE0(u4Address)); //LSB address
	u1ExtmjcWriteRegByI2C(0x601, 32); //Bit count
	u1ExtmjcWriteRegByI2C(0x600, 0x04);
	while(u4_fail_count<u4MsecTimeout) // 1 second timeout
	{
	    Mt8280SfRDSR(&u1_rdsr);
	    if ((u1_rdsr & 0x01) == 0) //WIP, Write In Progress
	    {
	        u4_sf_erase_sector_bitmap |= (1<<((u4Address>>16)&0xff));
	        return TRUE;
	    }
    	u4_fail_count++;
    	x_thread_delay(1);
    };
    return FALSE;
#endif    	
}

#if 0
static BOOL Mt8280SfWrite(UINT32 u4Address,UINT8* pu1Data,INT32 i4Size)
{
    INT32 i4_i,i4_size;
    INT32 i4_index = 0;
    UINT8 u1_rdsr;

    Printf("Writing 0x%08x,Size=0x%08x\n",u4Address,i4Size);
    while (i4_index<i4Size)
    {
        i4_size = i4Size-i4_index;
        if (i4_size>=32)
            i4_size = 32;
        if (i4_size > 1)
        {

            u1ExtmjcWriteRegByI2C(0x615, 0x1);
    		for (i4_i = 0; i4_i < i4_size;i4_i++)
	    	{
		    	#if 0
			    if(_bExtmjcICVersion==MT8280){
				    u1ExtmjcWriteRegByI2C(0x625, pcodebuffer1[i4i+u1tmp]);
			    }
				u1ExtmjcWriteRegByI2C(0x9981, pcodebuffer1[i4i+u1tmp]);
			    {
			    #else
				u1ExtmjcWriteRegByI2C(0x625, pu1Data[i4_index+i4_i]);
			    #endif
		    }
		}
		else
		{
            u1ExtmjcWriteRegByI2C(0x615, 0x0);
            u1ExtmjcWriteRegByI2C(0x607, pu1Data[i4_index]);
		}
		u1ExtmjcWriteRegByI2C(0x606, BYTE2(u4Address));
		u1ExtmjcWriteRegByI2C(0x605, BYTE1(u4Address));
		u1ExtmjcWriteRegByI2C(0x604, BYTE0(u4Address));
		u1ExtmjcWriteRegByI2C(0x600, 0x10);
		u4Address+=i4_size;
		i4_index+=i4_size;
		do
		{
		    Mt8280SfRDSR(&u1_rdsr);
		}while(u1_rdsr & 0x01); //Write In Progress
    }
    return TRUE;
}
#endif    

INT32 d_custom_extmjc_upgrade_open(VOID)
{
#if 1
	UINT8 data;
    	if (FALSE == drv_extmjc_is_enable())
    	{
        	return (DRV_CUSTOM_EXTMJCR_OK);
    	}
    	drv_extmjc_cpu_stop(&data);
    	b_sf_upg_enable = TRUE;
	//u4_sf_erase_sector_bitmap = 0;
    	Mt8280SfBp(FALSE); // Flash Protect Disable
    	HAL_Delay_us(100*1000); // wait 100 ms
#else
    UINT8 data;
    if (FALSE == drv_extmjc_is_enable())
    {
        return (DRV_CUSTOM_EXTMJCR_OK);
    }
    drv_extmjc_cpu_stop(&data);
    b_sf_upg_enable = TRUE;
    u4_sf_erase_sector_bitmap = 0;
    Mt8280SfBp(FALSE);
#endif    
    return (DRV_CUSTOM_EXTMJCR_OK);
}


INT32 d_custom_extmjc_upgrade_erase(VOID)
{
#if 1
	if (FALSE == drv_extmjc_is_enable())
    	{
        	return (DRV_CUSTOM_EXTMJCR_OK);
    	}
    	if (b_sf_upg_enable == FALSE)
        	return (DRV_CUSTOM_EXTMJCR_REG_FAIL);
#if FULL_ERASE_ENABLE
    	Mt8280SfEraseChip(20000); //20 seconds
#else
    	Mt8280SfEraseSector(0x000000,20000); //20 seconds
#endif
#else
    if (FALSE == drv_extmjc_is_enable())
    {
        return (DRV_CUSTOM_EXTMJCR_OK);
    }
    if (b_sf_upg_enable == FALSE)
        return (DRV_CUSTOM_EXTMJCR_REG_FAIL);
#if FULL_ERASE_ENABLE
    Mt8280SfEraseChip(20000); //20 seconds
#else
    Mt8280SfEraseSector(0x000000,20000); //20 seconds
#endif
#endif
    HAL_Delay_us(800*1000); // wait 800 ms
    return (DRV_CUSTOM_EXTMJCR_OK);
}

INT32 d_custom_extmjc_upgrade_write(
    UINT32                                  ui4_offset,
    UINT8*                                  pui1_data,
    UINT32                                  ui4_data_size
    )
{
#if 1
	UINT32 u4_index;
	UINT32 u4_Actural_Address;
    	if (FALSE == drv_extmjc_is_enable())
    	{
        	return (DRV_CUSTOM_EXTMJCR_OK);
    	}

    	if (b_sf_upg_enable == FALSE)
        	return (DRV_CUSTOM_EXTMJCR_REG_FAIL);

	Printf("\n[Download Address]:  ");
	for(u4_index=0;u4_index<ui4_data_size;u4_index+=0x20)
	{
		UINT32 u4_j;
		u4_Actural_Address = u4_index+ui4_offset;
		Printf("%8x",u4_Actural_Address);
		//Printf("\nDownload Address: %x",u4_Actural_Address);
		u1ExtmjcWriteRegByI2C(0x615, 0x1);
		for(u4_j=0;u4_j<32;u4_j++)
		{
			u1ExtmjcWriteRegByI2C(0x625, pui1_data[u4_index+u4_j]);
		}
		u1ExtmjcWriteRegByI2C(0x604,BYTE0(u4_Actural_Address));
		u1ExtmjcWriteRegByI2C(0x605,BYTE1(u4_Actural_Address));
		u1ExtmjcWriteRegByI2C(0x606,BYTE2(u4_Actural_Address));
		u1ExtmjcWriteRegByI2C(0x600,0x10);
	}
    	return Mt8280SfWaitCmdReady(0x04,0,1000);
#else
    UINT32 u4_address,u4_index,u4_size;
    if (FALSE == drv_extmjc_is_enable())
    {
        return (DRV_CUSTOM_EXTMJCR_OK);
    }

    if (b_sf_upg_enable == FALSE)
        return (DRV_CUSTOM_EXTMJCR_REG_FAIL);

    u4_index = 0;
    u4_address = ui4_offset;
    while(ui4_data_size > 0)
    {

#if (FULL_ERASE_ENABLE == 0)
        //Mt8280SfEraseSector(u4_address,20000); // 20 seconds timeout
#endif
        u4_size = 0x10000-(u4_address & 0x00FFFF); //get sector remain size
        if (u4_size> ui4_data_size)
            u4_size = ui4_data_size;
#if 0        
        Mt8280SfWrite(u4_address,pui1_data+u4_index,u4_size);
#endif
        u4_index+=u4_size;
        u4_address += u4_size;
        ui4_data_size -= u4_size;
    };
    return (DRV_CUSTOM_EXTMJCR_OK);
#endif    
}

INT32 d_custom_extmjc_upgrade_close(VOID)
{
    if (FALSE == drv_extmjc_is_enable())
    {
        return (DRV_CUSTOM_EXTMJCR_OK);
    }
    b_sf_upg_enable = FALSE;
    u4_sf_erase_sector_bitmap = 0;
    Mt8280SfBp(TRUE);
    HAL_Delay_us(100*1000); // wait 100 ms
    return (DRV_CUSTOM_EXTMJCR_OK);
}

#else //using uart download
#ifdef CC_MTK_LOADER //defined(CC_5360_LOADER) ||defined(CC_5391_LOADER)

#include "x_printf.h"
#include "x_timer.h"
#include "x_gpio.h"
#include "x_timer.h"
#include "x_pinmux.h"
#include "x_hal_5381.h"
#include "drvcust_if.h"
#include "extmjc_if.h"
#include "u_drv_cust.h"

#define UART_READ32(offset)         (IO_READ32(RS232_BASE, offset))
#define UART_WRITE32(offset, val)   (IO_WRITE32(RS232_BASE, offset, val))

#define UART1_DATA1             (0x00c0)
#define UART1_WRITE_BYTE(ch)    (UART_WRITE32(UART1_DATA1, (ch & 0xffU)))
#define UART1_READ_BYTE()       (UART_READ32(UART1_DATA1) & 0xffU)

#define UART1_COMMU             (0x00c8)
#define UART1_BAUDRATE          (0x0fU << 8)
#define BAUDRATE_115200         (0 << 8)
#define BAUDRATE_921600         (3 << 8)
#define UART_SET_BAUDRATE(x)    UART_WRITE32(UART1_COMMU, (UART_READ32(UART1_COMMU) & ~UART1_BAUDRATE) | x)

#define UART1_STATUS            (0x00cc)
#define UART1_RXD_BUF_STATE     (0x1fU << 0)
#define IS_UART1_RXBUF_EMPTY()  ((UART_READ32(UART1_STATUS) & UART1_RXD_BUF_STATE) == 0)
#define UART1_TXD_BUF_STATE     (0x1fU << 8)
#define IS_UART1_TXBUF_FULL()   ((UART_READ32(UART1_STATUS) & UART1_TXD_BUF_STATE) == 0)

static UINT32 u4UartFlag = 0;
static INT32 uart1_write(UINT8 u1Ch)
{
    HAL_TIME_T rOrgTime, rNewTime, rDiffTime;

    HAL_GetTime(&rOrgTime);
    while (IS_UART1_TXBUF_FULL())
    {
        HAL_GetTime(&rNewTime);
        HAL_GetDeltaTime(&rDiffTime, &rOrgTime, &rNewTime);
        if ((rDiffTime.u4Micros > 50000) || (rDiffTime.u4Seconds > 1))
        {
            return -1;
        }
    }

    if (u4UartFlag == 2)
    {
        u4UartFlag = 1;
        printf("\n<< 0x%02x ", (UINT32)u1Ch);
    }
    else if (u4UartFlag == 1)
    {
        printf("0x%02x ", (UINT32)u1Ch);
    }

    UART1_WRITE_BYTE(u1Ch);
//    HAL_Delay_us(400);
    return 0;
}

static INT32 uart1_read(void)
{
    HAL_TIME_T rOrgTime, rNewTime, rDiffTime;
    UINT8 u1Ch;

    HAL_GetTime(&rOrgTime);
    while (IS_UART1_RXBUF_EMPTY())
    {
        HAL_GetTime(&rNewTime);
        HAL_GetDeltaTime(&rDiffTime, &rOrgTime, &rNewTime);
        if ((rDiffTime.u4Micros > 50000) || (rDiffTime.u4Seconds > 1))
        {
            return -1;
        }
    }

    u1Ch = UART1_READ_BYTE();

    if (u4UartFlag == 1)
    {
        u4UartFlag = 2;
        printf("\n>> 0x%02x ", u1Ch);
    }
    else if (u4UartFlag == 2)
    {
        printf("0x%02x ", u1Ch);
    }

    return (INT32)u1Ch;
}

static INT32 ux_8280UART_read(UINT8 u1Offset)
{
    INT32 i4Val;

    uart1_write((u1Offset | 0x80));
    do {
        i4Val = uart1_read();
    } while (!IS_UART1_RXBUF_EMPTY());
    return i4Val;
}

static void ux_8280UART_write1(UINT8 u1Offset, UINT8 u1Val)
{
    uart1_write(u1Offset);
    uart1_write(u1Val);
}
/*
static void ux_8280UART_write4(UINT8 u1Offset, UINT32 u4Val)
{
    UINT8 u1Val;

    uart1_write(u1Offset);
    u1Val = (u4Val >> 24) & 0xffU;
    uart1_write(u1Val);
    u1Val = (u4Val >> 16) & 0xffU;
    uart1_write(u1Val);
    u1Val = (u4Val >>  8) & 0xffU;
    uart1_write(u1Val);
    u1Val = (u4Val >>  0) & 0xffU;
    uart1_write(u1Val);
}
*/
static INT32 ux_8280SNOR_read(UINT8 u1Offset)
{
    INT32 i4Val;

    ux_8280UART_write1(0x2c, u1Offset);
    i4Val = ux_8280UART_read(0x2d);
    return i4Val;
}

static void ux_8280SNOR_write(UINT8 u1Offset, UINT8 u1Val)
{
    ux_8280UART_write1(0x2c, u1Offset);
    ux_8280UART_write1(0x2d, u1Val);
}

#define SEND_UART_MSG(array)                \
        for (i=0; i<sizeof(array); i++)     \
        {                                   \
            uart1_write(array[i]);          \
        }

#define READ_UART_EMPTY()                       \
        while (uart1_read() != -1) { }

static INT32 ux_8280UpgradeOpen()
{
    INT32 i;
    INT32 data;
    static UINT8 au1EchoMsg[] = {
        0x80, 0x80, 0x80, 0x80
    };
    static UINT8 au1InitMsg[] = {
        0x54, 0x8a, 0x51, 0x37, 0x56, 0x92, 0x1c, 0xb2,
        0x00, 0x13, 0x01, 0x00, 0x50, 0x64,
        0x02, 0x00, 0x20, 0x02,
        0x00, 0x00, 0x01, 0x03,
        0x01, 0x00, 0x01, 0x03,
        0x00, 0x30, 0x00, 0x20, 0x06, 0x01, 0x20, 0x02
    };
    static UINT8 au1InitMsg1[] = {
        0x2A, 0x3B,
        0x00, 0x30, 0x00, 0x20, 0x06, 0x01, 0x20, 0x02
    };

    HAL_Delay_us(4000);
    //pinmux switch to U2RX & U2TX
    data = PINMUX0_REG() & 0xfffcffff;
    data = data | 0x00010000; //U2RX
    PINMUX0_WRITE(data);

    HAL_Delay_us(4000);
    // 1. send initial strings.
    SEND_UART_MSG(au1EchoMsg);
    READ_UART_EMPTY();

    SEND_UART_MSG(au1InitMsg);
    SEND_UART_MSG(au1InitMsg1);
    READ_UART_EMPTY();
    HAL_Delay_us(4000);

    return 0;
}

static void ux_8280SetFastBaudrate(UINT32 fgFast)
{
    if (fgFast)
    {
        printf("change uart to 921600\n");
        ux_8280UART_write1(0x02, 0x03);
        HAL_Delay_us(4000);
        UART_SET_BAUDRATE(BAUDRATE_921600);
        HAL_Delay_us(4000);
    }
    else
    {
        printf("change uart to 115200\n");
        ux_8280UART_write1(0x02, 0x00);
        HAL_Delay_us(4000);
        UART_SET_BAUDRATE(BAUDRATE_115200);
        HAL_Delay_us(4000);
    }
}

static INT32 ux_8280CodeErase()
{

#if 1 //FULL_ERASE_ENABLE
    ux_8280SNOR_write(0x15, 0x03);
    ux_8280SNOR_write(0x15, 0x03);
    ux_8280UART_write1(0x00, 0x30);
#else   // 1 sector erase
    ux_8280UART_write1(0x00, 0x30);
    ux_8280UART_write1(0x20, 0x32);
    ux_8280UART_write1(0x2a, 0x3f);
#endif

    ux_8280SNOR_write(0x21, 0x40);
    //SF WREN
    ux_8280SNOR_write(0x0d, 0x06);
    ux_8280SNOR_write(0x01, 0x08);
    ux_8280SNOR_write(0x00, 0x04);
    while ((ux_8280SNOR_read(0x00) & 0xf) != 0) { HAL_Delay_us(4000); }

    //SF WRSR
    ux_8280SNOR_write(0x0d, 0x01);
    ux_8280SNOR_write(0x0c, 0x02);
    ux_8280SNOR_write(0x01, 0x10);
    ux_8280SNOR_write(0x00, 0x04);
    while ((ux_8280SNOR_read(0x00) & 0xf) != 0) { HAL_Delay_us(4000); }

    //SF WREN
    ux_8280SNOR_write(0x0d, 0x06);
    ux_8280SNOR_write(0x01, 0x08);
    ux_8280SNOR_write(0x00, 0x04);

    //SF WREN
    ux_8280SNOR_write(0x0d, 0x06);
    ux_8280SNOR_write(0x01, 0x08);
    ux_8280SNOR_write(0x00, 0x04);

    // polling ready
    Printf("\nerase polling ready\n");
    while ((ux_8280SNOR_read(0x00) & 0xf) != 0) { HAL_Delay_us(4000); }

#if FULL_ERASE_ENABLE
    ux_8280SNOR_write(0x0d, 0xc7);
    ux_8280SNOR_write(0x01, 0x08);
    ux_8280SNOR_write(0x00, 0x04);
#else   // 1 sector erase
    ux_8280SNOR_write(0x0d, 0xd8);
    ux_8280SNOR_write(0x0c, 0x00);
    ux_8280SNOR_write(0x0b, 0x00);
    ux_8280SNOR_write(0x0a, 0x00);
    ux_8280SNOR_write(0x01, 0x20);
    ux_8280SNOR_write(0x00, 0x04);
#endif

    // polling finish
    printf("\nerase polling finish\n");
    ux_8280SNOR_write(0x00, 0x02);
    while ((ux_8280SNOR_read(0x00) & 0xf) != 0) { HAL_Delay_us(4000); }
    while ((ux_8280SNOR_read(0x02) & 1) != 0)
    {
        ux_8280SNOR_write(0x00, 0x02);
        while ((ux_8280SNOR_read(0x00) & 0xf) != 0) { HAL_Delay_us(4000); }
        HAL_Delay_us(4000);
    }

    // write initial
    ux_8280SNOR_write(0x2e, 0x00);
    ux_8280SNOR_write(0x2d, 0x00);
    ux_8280SNOR_write(0x19, 0x03);
    ux_8280SNOR_write(0x15, 0x00);
    ux_8280SNOR_write(0x15, 0x03);
    ux_8280SNOR_read(0x15);
    ux_8280SNOR_read(0x27);
    ux_8280SNOR_read(0x2d);

    return 0;
}

static INT32 ux_8280CodeWrite128(UINT32 u4Offset, UCHAR *pauc_buffer, INT32 ui4_len)
{
    INT32 i;
    UINT8 u1Val;

    // initial write.
    u1Val = (u4Offset >> 16) & 0xffU;
    ux_8280SNOR_write(0x06, u1Val);
    u1Val = (u4Offset >>  8) & 0xffU;
    ux_8280SNOR_write(0x05, u1Val);
    u1Val = (u4Offset >>  0) & 0xffU;
    ux_8280SNOR_write(0x04, u1Val);

    // send data.
    for (i=0; i<ui4_len; i++)
    {
        ux_8280UART_write1(0x10, pauc_buffer[i]);
    }

    // close write.
    ux_8280SNOR_write(0x00, 0x10);

    // read uart1 empty
    READ_UART_EMPTY();
    return 0;
}

static INT32 ux_8280UpgradeClose()
{

    ux_8280UART_write1(0x01, 0x03);
    //Check write complete
    ux_8280SNOR_write(0x00, 0x02);
    while ((ux_8280SNOR_read(0x00) & 0xf) != 0) { HAL_Delay_us(4000); }
    while ((ux_8280SNOR_read(0x02) & 1) != 0)
    {
        ux_8280SNOR_write(0x00, 0x02);
        while ((ux_8280SNOR_read(0x00) & 0xf) != 0) { HAL_Delay_us(4000); }
        HAL_Delay_us(4000);
    }

    ux_8280SNOR_write(0x15, 0x00);
    ux_8280SNOR_write(0x2e, 0x10);
    ux_8280SNOR_write(0x28, 0x00);
    ux_8280SNOR_write(0x29, 0x00);
    ux_8280SNOR_write(0x2a, 0x00);
    ux_8280SNOR_write(0x2b, 0x00);
    ux_8280SNOR_read(0x2e);

    ux_8280UART_write1(0x01, 0x03);
    ux_8280SNOR_write(0x00, 0x02);
    while ((ux_8280SNOR_read(0x00) & 0xf) != 0) { HAL_Delay_us(4000); }
    while ((ux_8280SNOR_read(0x02) & 1) != 0)
    {
        ux_8280SNOR_write(0x00, 0x02);
        while ((ux_8280SNOR_read(0x00) & 0xf) != 0) { HAL_Delay_us(4000); }
        HAL_Delay_us(4000);
    }

    ux_8280UART_read(0x00);
    ux_8280UART_read(0x01);
    ux_8280UART_read(0x02);

    ux_8280SNOR_write(0x06, 0x00);
    ux_8280SNOR_write(0x05, 0x00);
    ux_8280SNOR_write(0x04, 0x00);
    ux_8280SNOR_write(0x0A, 0x00);
    ux_8280SNOR_write(0x09, 0xff);
    ux_8280SNOR_write(0x08, 0xff);
    ux_8280SNOR_write(0x2e, 0x30);
    ux_8280SNOR_write(0x2e, 0x00);
    ux_8280SNOR_write(0x15, 0x03);
    ux_8280SNOR_read(0x15);

    // read uart1 empty
    READ_UART_EMPTY();

    HAL_Delay_us(4000);
    drv_extmjc_reset();
    return 0;
}

INT32 d_custom_extmjc_upgrade_open(VOID)
{
    if (FALSE == drv_extmjc_is_enable())
    {
        return (DRV_CUSTOM_EXTMJCR_OK);
    }

    Printf("8280 firmware upgrade: Open\n");

    if (ux_8280UpgradeOpen() != 0)
    {
        Printf("\n8280 Open failed.\n");
        return DRV_CUSTOM_EXTMJCR_INV_ARG;
    }
    ux_8280SetFastBaudrate(0);
    return (DRV_CUSTOM_EXTMJCR_OK);
}
INT32 d_custom_extmjc_upgrade_erase(VOID)
{
    if (FALSE == drv_extmjc_is_enable())
    {
        return (DRV_CUSTOM_EXTMJCR_OK);
    }

    Printf("8280 firmware upgrade: Erase\n");

    if (ux_8280CodeErase() != 0)
    {
        Printf("\n8280 Erase failed.\n");
        return DRV_CUSTOM_EXTMJCR_INV_ARG;
    }
    return (DRV_CUSTOM_EXTMJCR_OK);
}
INT32 d_custom_extmjc_upgrade_write(
    UINT32                                  ui4_offset,
    UINT8*                                  pui1_data,
    UINT32                                  ui4_data_size)
{
    INT32 j,i4_cp_size,i4Ret;

    if (FALSE == drv_extmjc_is_enable())
    {
        return (DRV_CUSTOM_EXTMJCR_OK);
    }
    Printf("8280 firmware upgrade: Write\n");
    j=0;
    while (j < ui4_data_size)
    {
        i4_cp_size = 128;

        i4Ret = ux_8280CodeWrite128(j, &pui1_data[j], i4_cp_size);
        if (0 != i4Ret)
        {
            Printf("8280 write failed offset=%x size=%d.\n", j, i4_cp_size);
            return DRV_CUSTOM_EXTMJCR_INV_ARG;
        }
        else
        {
            Printf("Extmjc Write Offset=0x%08x\n",j);
        }
        j += i4_cp_size;
    }
    return (DRV_CUSTOM_EXTMJCR_OK);
}

INT32 d_custom_extmjc_upgrade_close(VOID)
{
    if (FALSE == drv_extmjc_is_enable())
    {
        return (DRV_CUSTOM_EXTMJCR_OK);
    }

    Printf("8280 firmware upgrade: Close\n");
    ux_8280SetFastBaudrate(0);
    ux_8280UpgradeClose();
    return DRV_CUSTOM_EXTMJCR_OK;
}

#else
#include "extmjc_debug.h"
#include "u_drv_cust.h"
#include "extmjc_if.h"

#include "x_gpio.h"
#include "x_timer.h"

#include "drvcust_if.h"
#include "uart_if.h"

/*-----------------------------------------------------------------------------
 * structure, constants, macro definitions
 *---------------------------------------------------------------------------*/
#define EXTMJC_UART_PORT                    1

/*-----------------------------------------------------------------------------
 * variable declarations
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * private methods declarations
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * extern methods declarations
 *---------------------------------------------------------------------------*/
#define EXTMJC_8280_UPGRADE_SNOR_WRITE(aui1_buf, ui1_offset, ui1_val) \
    aui1_buf[0] = 0x2c; \
    aui1_buf[1] = ui1_offset; \
    aui1_buf[2] = 0x2d; \
    aui1_buf[3] = ui1_val; \
    extmjc_8280_upgrade_uart_send(aui1_buf, 4);

#define EXTMJC_8280_UPGRADE_SNOR_READ(aui1_buf, ui1_offset, i4_val) \
    aui1_buf[0] = 0x2c; \
    aui1_buf[1] = ui1_offset; \
    extmjc_8280_upgrade_uart_send(aui1_buf, 2); \
    i4_val = extmjc_8280_upgrade_read(0x2d);

#define EXTMJC_8280_UPGRADE_WRITE_UINT8(aui1_buf, ui1_offset, ui1_val) \
    aui1_buf[0] = ui1_offset; \
    aui1_buf[1] = ui1_val; \
    extmjc_8280_upgrade_uart_send(aui1_buf, 2);

static VOID extmjc_8280_upgrade_uart_send(
    UINT8*                                  pui1_data,
    UINT32                                  ui4_data_size)
{
    UINT32 ui4_ttl_send_data_size;
    UINT32 ui4_send_data_size;

    ui4_ttl_send_data_size = 0;

    while (ui4_ttl_send_data_size != ui4_data_size)
    {
        ui4_send_data_size = UART_Write(EXTMJC_UART_PORT,
                                        pui1_data + ui4_ttl_send_data_size,
                                        ui4_data_size - ui4_ttl_send_data_size,
                                        TRUE);

        ui4_ttl_send_data_size += ui4_send_data_size;
    }

}


static INT32 extmjc_8280_upgrade_read(UINT8 ui1_offset)
{
    HAL_TIME_T  t_org_time, t_new_time, t_time_diff;
    INT32       i4_ret;
    UINT8       ui1_data;

    ui1_offset = ui1_offset | 0x80;
    UART_Write(EXTMJC_UART_PORT, &ui1_offset, 1, TRUE);


    HAL_GetTime(&t_org_time);

    i4_ret = -1;
    while (TRUE)
    {
        /* Get data */
        if (0 != UART_Read(EXTMJC_UART_PORT, &ui1_data, 1, FALSE))
        {
            i4_ret = ui1_data;
            HAL_GetTime(&t_org_time);
        }

        HAL_GetTime(&t_new_time);
        HAL_GetDeltaTime(&t_time_diff, &t_org_time, &t_new_time);
        if ((t_time_diff.u4Micros > 50000) || (t_time_diff.u4Seconds > 1))
        {
            break;
        }
    }

    return i4_ret;
}

static VOID extmjc_8280_upgrade_uart_rec_empty(VOID)
{
    HAL_TIME_T  t_org_time, t_new_time, t_time_diff;
    UINT8       ui1_data;

    HAL_GetTime(&t_org_time);

    while (TRUE)
    {
        /* Get data */
        if (0 != UART_Read(EXTMJC_UART_PORT, &ui1_data, 1, FALSE))
        {
            HAL_GetTime(&t_org_time);
        }

        HAL_GetTime(&t_new_time);
        HAL_GetDeltaTime(&t_time_diff, &t_org_time, &t_new_time);
        if ((t_time_diff.u4Micros > 50000) || (t_time_diff.u4Seconds > 1))
        {
            break;
        }
    }
}


/*---------------------------------------------------------------------------
 * Name
 *
 * Description      -
 * Input arguments  -
 * Output arguments -
 * Returns          -
 *---------------------------------------------------------------------------*/
INT32 d_custom_extmjc_upgrade_write128(
    UINT32                                  ui4_offset,
    UINT8*                                  pui1_data,
    UINT32                                  ui4_data_size)
{
    UINT8   aui1_buf[4];
    INT32   i;
//    INT32 i4_val;


    if (ui4_data_size > 128)
    {
        return (DRV_CUSTOM_EXTMJCR_INV_ARG);
    }
   // Printf("8280 firmware upgrade: write offset=0x%08x lengh=%d\n", ui4_offset, ui4_data_size);

    // initial write.
    EXTMJC_8280_UPGRADE_SNOR_WRITE(aui1_buf, 0x06, ((ui4_offset >> 16) & 0xff));
    EXTMJC_8280_UPGRADE_SNOR_WRITE(aui1_buf, 0x05, ((ui4_offset >> 8) & 0xff));
    EXTMJC_8280_UPGRADE_SNOR_WRITE(aui1_buf, 0x04, (ui4_offset & 0xff));

    // Send data.
    for (i = 0; i < ui4_data_size; i++)
    {
        EXTMJC_8280_UPGRADE_WRITE_UINT8(aui1_buf, 0x10, pui1_data[i]);
    }

    // close write.
    EXTMJC_8280_UPGRADE_SNOR_WRITE(aui1_buf, 0x00, 0x10);
    // read uart1 empty
    extmjc_8280_upgrade_uart_rec_empty();

    return (DRV_CUSTOM_EXTMJCR_OK);
}


/*---------------------------------------------------------------------------
 * Name
 *
 * Description      -
 * Input arguments  -
 * Output arguments -
 * Returns          -
 *---------------------------------------------------------------------------*/
INT32 d_custom_extmjc_upgrade_open(VOID)
{
    UINT8               ui1_msg_error;
    RS_232_SETUP_INFO_T t_rs232_setup_info;
    UINT8 aui1_echo_msg[] = {
        0x80, 0x80, 0x80, 0x80
    };

    UINT8 au1_init_msg[] = {
        0x54, 0x8a, 0x51, 0x37, 0x56, 0x92, 0x1c, 0xb2,
        0x00, 0x13, 0x01, 0x00, 0x50, 0x64,
        0x02, 0x00, 0x20, 0x02,
        0x00, 0x00, 0x01, 0x03,
        0x01, 0x00, 0x01, 0x03,
        0x00, 0x30, 0x00, 0x20, 0x06, 0x01, 0x20, 0x02
    };

    UINT8 au1_init_msg1[] = {
        0x2A, 0x3B,
        0x00, 0x30, 0x00, 0x20, 0x06, 0x01, 0x20, 0x02
    };
    UINT8 au1_baud_rate_msg[] = {
        0x02, 0x00
    };


    if (FALSE == drv_extmjc_is_enable())
    {
        return (DRV_CUSTOM_EXTMJCR_OK);
    }

    Printf("8280 firmware upgrade: Open\n");

    /* Change OSD to video mode */
    d_custom_extmjc_set_osd_mode(DRV_EXTMJC_OSD_MODE_NORMAL, &ui1_msg_error);

    /* Stop 8280 CPU operation */
//    d_custom_extmjc_set_dram((UINT32) 0x00492C04, (UINT32) 0x55aaaa55, &ui1_msg_error);

    /* Stop 538x MJC UART1 access */
    //extmjc_prc_rtx_set_upgrade(TRUE);

    /* Send initial strings.*/
    extmjc_8280_upgrade_uart_send(aui1_echo_msg, sizeof(aui1_echo_msg));
    extmjc_8280_upgrade_uart_rec_empty();

    extmjc_8280_upgrade_uart_send(au1_init_msg, sizeof(au1_init_msg));
    extmjc_8280_upgrade_uart_send(au1_init_msg1, sizeof(au1_init_msg1));
    extmjc_8280_upgrade_uart_rec_empty();

    /* Change uart to 921600 */
    extmjc_8280_upgrade_uart_send(au1_baud_rate_msg, sizeof(au1_baud_rate_msg));
    HAL_Delay_us(4000);
    t_rs232_setup_info.e_data_len = RS_232_DATA_LEN_8;
    t_rs232_setup_info.e_parity = RS_232_PARITY_NONE;
    t_rs232_setup_info.e_speed = RS_232_SPEED_115200;
    t_rs232_setup_info.e_stop_bit = RS_232_STOP_BIT_1;
    UART_SetParameter(EXTMJC_UART_PORT, &t_rs232_setup_info);
    HAL_Delay_us(4000);

    return (DRV_CUSTOM_EXTMJCR_OK);
}

/*---------------------------------------------------------------------------
 * Name
 *
 * Description      -
 * Input arguments  -
 * Output arguments -
 * Returns          -
 *---------------------------------------------------------------------------*/
INT32 d_custom_extmjc_upgrade_erase(VOID)
{
    UINT8 aui1_buf[4];
    INT32 i4_val;

    if (FALSE == drv_extmjc_is_enable())
    {
        return (DRV_CUSTOM_EXTMJCR_OK);
    }

    Printf("8280 firmware upgrade: Erase\n");


#if 1 //FULL_ERASE_ENABLE
    EXTMJC_8280_UPGRADE_SNOR_WRITE(aui1_buf, 0x15, 0x03);
    EXTMJC_8280_UPGRADE_SNOR_WRITE(aui1_buf, 0x15, 0x03);
    EXTMJC_8280_UPGRADE_WRITE_UINT8(aui1_buf, 0x00, 0x30);
#else   // 1 sector erase
    EXTMJC_8280_UPGRADE_WRITE_UINT8(aui1_buf, 0x00, 0x30);
    EXTMJC_8280_UPGRADE_WRITE_UINT8(aui1_buf, 0x20, 0x32);
    EXTMJC_8280_UPGRADE_WRITE_UINT8(aui1_buf, 0x2a, 0x3f);
#endif

    EXTMJC_8280_UPGRADE_SNOR_WRITE(aui1_buf, 0x21, 0x40);
    //SF WREN
    EXTMJC_8280_UPGRADE_SNOR_WRITE(aui1_buf, 0x0d, 0x06);
    EXTMJC_8280_UPGRADE_SNOR_WRITE(aui1_buf, 0x01, 0x08);
    EXTMJC_8280_UPGRADE_SNOR_WRITE(aui1_buf, 0x00, 0x04);
    while (TRUE)
    {
        EXTMJC_8280_UPGRADE_SNOR_READ(aui1_buf, 0x00, i4_val);
        if (0 == (i4_val & 0x0F))
        {
            break;
        }
        HAL_Delay_us(4000);
    }
    //SF WRSR
    EXTMJC_8280_UPGRADE_SNOR_WRITE(aui1_buf, 0x0d, 0x01);
    EXTMJC_8280_UPGRADE_SNOR_WRITE(aui1_buf, 0x0c, 0x02);
    EXTMJC_8280_UPGRADE_SNOR_WRITE(aui1_buf, 0x01, 0x10);
    EXTMJC_8280_UPGRADE_SNOR_WRITE(aui1_buf, 0x00, 0x04);
    while (TRUE)
    {
        EXTMJC_8280_UPGRADE_SNOR_READ(aui1_buf, 0x00, i4_val);
        if (0 == (i4_val & 0x0F))
        {
            break;
        }
        HAL_Delay_us(4000);
    }

    //SF WREN
    EXTMJC_8280_UPGRADE_SNOR_WRITE(aui1_buf, 0x0d, 0x06);
    EXTMJC_8280_UPGRADE_SNOR_WRITE(aui1_buf, 0x01, 0x08);
    EXTMJC_8280_UPGRADE_SNOR_WRITE(aui1_buf, 0x00, 0x04);

    //SF WREN
    EXTMJC_8280_UPGRADE_SNOR_WRITE(aui1_buf, 0x0d, 0x06);
    EXTMJC_8280_UPGRADE_SNOR_WRITE(aui1_buf, 0x01, 0x08);
    EXTMJC_8280_UPGRADE_SNOR_WRITE(aui1_buf, 0x00, 0x04);
    while (TRUE)
    {
        EXTMJC_8280_UPGRADE_SNOR_READ(aui1_buf, 0x00, i4_val);
        if (0 == (i4_val & 0x0F))
        {
            break;
        }
        HAL_Delay_us(4000);
    }

#if FULL_ERASE_ENABLE
    EXTMJC_8280_UPGRADE_SNOR_WRITE(aui1_buf, 0x0d, 0xc7);
    EXTMJC_8280_UPGRADE_SNOR_WRITE(aui1_buf, 0x01, 0x08);
    EXTMJC_8280_UPGRADE_SNOR_WRITE(aui1_buf, 0x00, 0x04);
#else   // 1 sector erase
    EXTMJC_8280_UPGRADE_SNOR_WRITE(aui1_buf, 0x0d, 0xd8);
    EXTMJC_8280_UPGRADE_SNOR_WRITE(aui1_buf, 0x0c, 0x00);
    EXTMJC_8280_UPGRADE_SNOR_WRITE(aui1_buf, 0x0b, 0x00);
    EXTMJC_8280_UPGRADE_SNOR_WRITE(aui1_buf, 0x0a, 0x00);
    EXTMJC_8280_UPGRADE_SNOR_WRITE(aui1_buf, 0x01, 0x20);
    EXTMJC_8280_UPGRADE_SNOR_WRITE(aui1_buf, 0x00, 0x04);
#endif
    // polling finish
    printf("\nerase polling finish\n");
    EXTMJC_8280_UPGRADE_SNOR_WRITE(aui1_buf, 0x00, 0x02);
    while (TRUE)
    {
        EXTMJC_8280_UPGRADE_SNOR_READ(aui1_buf, 0x00, i4_val);
        if (0 == (i4_val & 0x0F))
        {
            break;
        }
        HAL_Delay_us(4000);
    }

    while (TRUE)
    {
        EXTMJC_8280_UPGRADE_SNOR_READ(aui1_buf, 0x02, i4_val);
        if (0 == (i4_val & 1))
        {
            break;
        }

        EXTMJC_8280_UPGRADE_SNOR_WRITE(aui1_buf, 0x00, 0x02);

        while (TRUE)
        {
            EXTMJC_8280_UPGRADE_SNOR_READ(aui1_buf, 0x00, i4_val);
            if (0 == (i4_val & 0x0F))
            {
                break;
            }
            HAL_Delay_us(4000);
        }

        HAL_Delay_us(4000);
    }

    // write initial
    EXTMJC_8280_UPGRADE_SNOR_WRITE(aui1_buf, 0x2e, 0x00);
    EXTMJC_8280_UPGRADE_SNOR_WRITE(aui1_buf, 0x2d, 0x00);
    EXTMJC_8280_UPGRADE_SNOR_WRITE(aui1_buf, 0x19, 0x03);
    EXTMJC_8280_UPGRADE_SNOR_WRITE(aui1_buf, 0x15, 0x00);
    EXTMJC_8280_UPGRADE_SNOR_WRITE(aui1_buf, 0x15, 0x03);
    EXTMJC_8280_UPGRADE_SNOR_READ(aui1_buf, 0x15, i4_val);
    EXTMJC_8280_UPGRADE_SNOR_READ(aui1_buf, 0x27, i4_val);
    EXTMJC_8280_UPGRADE_SNOR_READ(aui1_buf, 0x2d, i4_val);

    return (DRV_CUSTOM_EXTMJCR_OK);
}
/*---------------------------------------------------------------------------
 * Name
 *
 * Description      -
 * Input arguments  -
 * Output arguments -
 * Returns          -
 *---------------------------------------------------------------------------*/
INT32 d_custom_extmjc_upgrade_write(
    UINT32                                  ui4_offset,
    UINT8*                                  pui1_data,
    UINT32                                  ui4_data_size)
{
    UINT32 datalen;

    if (FALSE == drv_extmjc_is_enable())
    {
        return (DRV_CUSTOM_EXTMJCR_OK);
    }
    Printf("8280 firmware upgrade: Write Offset=0x%08x Length=%d\n",ui4_offset,ui4_data_size);

    datalen = ui4_data_size;
    if (datalen>128)
    {
        Printf("Extmjc d_custom_extmjc_upgrade_write more than 128 bytes\n");
        return DRV_CUSTOM_EXTMJCR_INV_ARG;
    }
    datalen = 128;
    d_custom_extmjc_upgrade_write128(ui4_offset,pui1_data,datalen);
    return (DRV_CUSTOM_EXTMJCR_OK);
}

/*---------------------------------------------------------------------------
 * Name
 *
 * Description      -
 * Input arguments  -
 * Output arguments -
 * Returns          -
 *---------------------------------------------------------------------------*/
INT32 d_custom_extmjc_upgrade_close(VOID)
{
    RS_232_SETUP_INFO_T t_rs232_setup_info;
    UINT8 aui1_buf[4];
    INT32 i4_val;
    UINT8 au1_baud_rate_msg[] = {
        0x02, 0x00
    };

    if (FALSE == drv_extmjc_is_enable())
    {
        return (DRV_CUSTOM_EXTMJCR_OK);
    }

    Printf("8280 firmware upgrade: Close\n");

    EXTMJC_8280_UPGRADE_WRITE_UINT8(aui1_buf, 0x01, 0x03);
    //Check write complete
    EXTMJC_8280_UPGRADE_SNOR_WRITE(aui1_buf, 0x00, 0x02);
    while (TRUE)
    {
        EXTMJC_8280_UPGRADE_SNOR_READ(aui1_buf, 0x00, i4_val);
        if (0 == (i4_val & 0x0F))
        {
            break;
        }
        HAL_Delay_us(4000);
    }

    while (TRUE)
    {
        EXTMJC_8280_UPGRADE_SNOR_READ(aui1_buf, 0x02, i4_val);
        if (0 == (i4_val & 1))
        {
            break;
        }

        EXTMJC_8280_UPGRADE_SNOR_WRITE(aui1_buf, 0x00, 0x02);

        while (TRUE)
        {
            EXTMJC_8280_UPGRADE_SNOR_READ(aui1_buf, 0x00, i4_val);
            if (0 == (i4_val & 0x0F))
            {
                break;
            }
            HAL_Delay_us(4000);
        }

        HAL_Delay_us(4000);
    }

    EXTMJC_8280_UPGRADE_SNOR_WRITE(aui1_buf, 0x15, 0x00);
    EXTMJC_8280_UPGRADE_SNOR_WRITE(aui1_buf, 0x2e, 0x10);
    EXTMJC_8280_UPGRADE_SNOR_WRITE(aui1_buf, 0x28, 0x00);
    EXTMJC_8280_UPGRADE_SNOR_WRITE(aui1_buf, 0x29, 0x00);
    EXTMJC_8280_UPGRADE_SNOR_WRITE(aui1_buf, 0x2a, 0x00);
    EXTMJC_8280_UPGRADE_SNOR_WRITE(aui1_buf, 0x2b, 0x00);
    EXTMJC_8280_UPGRADE_SNOR_READ(aui1_buf, 0x2e, i4_val);

    EXTMJC_8280_UPGRADE_WRITE_UINT8(aui1_buf, 0x01, 0x03);
    EXTMJC_8280_UPGRADE_SNOR_WRITE(aui1_buf, 0x00, 0x02);
    while (TRUE)
    {
        EXTMJC_8280_UPGRADE_SNOR_READ(aui1_buf, 0x00, i4_val);
        if (0 == (i4_val & 0x0F))
        {
            break;
        }
        HAL_Delay_us(4000);
    }

    while (TRUE)
    {
        EXTMJC_8280_UPGRADE_SNOR_READ(aui1_buf, 0x02, i4_val);
        if (0 == (i4_val & 1))
        {
            break;
        }

        EXTMJC_8280_UPGRADE_SNOR_WRITE(aui1_buf, 0x00, 0x02);

        while (TRUE)
        {
            EXTMJC_8280_UPGRADE_SNOR_READ(aui1_buf, 0x00, i4_val);
            if (0 == (i4_val & 0x0F))
            {
                break;
            }
            HAL_Delay_us(4000);
        }

        HAL_Delay_us(4000);
    }


    extmjc_8280_upgrade_read(0x00);
    extmjc_8280_upgrade_read(0x01);
    extmjc_8280_upgrade_read(0x02);

    EXTMJC_8280_UPGRADE_SNOR_WRITE(aui1_buf, 0x06, 0x00);
    EXTMJC_8280_UPGRADE_SNOR_WRITE(aui1_buf, 0x05, 0x00);
    EXTMJC_8280_UPGRADE_SNOR_WRITE(aui1_buf, 0x04, 0x00);
    EXTMJC_8280_UPGRADE_SNOR_WRITE(aui1_buf, 0x0A, 0x00);
    EXTMJC_8280_UPGRADE_SNOR_WRITE(aui1_buf, 0x09, 0xff);
    EXTMJC_8280_UPGRADE_SNOR_WRITE(aui1_buf, 0x08, 0xff);
    EXTMJC_8280_UPGRADE_SNOR_WRITE(aui1_buf, 0x2e, 0x30);
    EXTMJC_8280_UPGRADE_SNOR_WRITE(aui1_buf, 0x2e, 0x00);
    EXTMJC_8280_UPGRADE_SNOR_WRITE(aui1_buf, 0x15, 0x03);
    EXTMJC_8280_UPGRADE_SNOR_READ(aui1_buf, 0x15, i4_val);

    // read uart1 empty
    extmjc_8280_upgrade_uart_rec_empty();


    /* Change uart to 115200 */
    extmjc_8280_upgrade_uart_send(au1_baud_rate_msg, sizeof(au1_baud_rate_msg));
    HAL_Delay_us(4000);
    t_rs232_setup_info.e_data_len = RS_232_DATA_LEN_8;
    t_rs232_setup_info.e_parity = RS_232_PARITY_NONE;
    t_rs232_setup_info.e_speed = RS_232_SPEED_115200;
    t_rs232_setup_info.e_stop_bit = RS_232_STOP_BIT_1;
    UART_SetParameter(EXTMJC_UART_PORT, &t_rs232_setup_info);
    HAL_Delay_us(4000);

//    extmjc_prc_rtx_set_upgrade(FALSE);

    return (DRV_CUSTOM_EXTMJCR_OK);
}
#endif
#endif
#endif /* DRV_SUPPORT_EXTMJC */

