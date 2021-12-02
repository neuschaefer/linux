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
 * @file HDMI customization file
 *
 *
 * @author chiachi_chiang@mtk.com.tw
 * @author darren_weng@mtk.com.tw
 * @author daniel_hsiao@mtk.com.tw
 */
//Change the following definition for gpio customization



#include "x_gpio.h"
#include "x_printf.h"

#include "hw_vdoin.h"
#include "hw_hdmi.h"
#include "drv_hdmi.h"
#include "drv_video.h"
#include "sv_const.h"
#include "source_table.h"
#include "sif_if.h"
#include "x_timer.h"

#define HDMI_PORT_LARGEST HDMI_SWITCH_4

/*Main chip I2C define*/
#define Chip_I2C_ChannelId  0		//It CANNOT be changed
#define I2C_TIMING	0x100
#define I2C_AddrNum 				0
//#define Switch_Slave_Addr	0xAE
UINT8 Switch_Slave_Addr;

/*
@para: 1 is success , 0 is fail   
if I2C_AddrNum=1, I2C will send out wordaddr
if I2C_AddrNum=0, I2C will not  send out wordaddr
*/
UINT8 u1SwitchI2CRead(UINT8 u1SlaveAddr, UINT32 u1WordAddr,UINT8 *pu1Buffer, UINT16 u2ByteCount)
{
  
    INT32 i4ReturnValue;
    i4ReturnValue = SIF_X_Read(Chip_I2C_ChannelId,I2C_TIMING, u1SlaveAddr, I2C_AddrNum , u1WordAddr, pu1Buffer, u2ByteCount);

    if (i4ReturnValue>0)
    {
		return 1;
    }
    else
    {
	  return 0;
    }

}
/*
@para: 1 is success , 0 is fail   
*/ 
UINT8 u1SwitchI2CWrite(UINT8 u1SlaveAddr, UINT32 u1WordAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount)
{
    INT32 i4ReturnValue;
   
    i4ReturnValue = SIF_X_Write(Chip_I2C_ChannelId, I2C_TIMING, u1SlaveAddr, I2C_AddrNum ,u1WordAddr, (UINT8 *)pu1Buffer, u2ByteCount);

    if (i4ReturnValue > 0)
    {

    		return 1;
    }
    else
    {
       //LOG(1,"Sil9187 write fail\n");
        return 0;
    }
}

static void vSwitchI2CInit(void)
{
    	UINT8 data[2];
    	if(u1SwitchI2CRead(0xAE,0,data,1))
    	{
    			Switch_Slave_Addr=0xAE;
    			data[0]=0x3f;
    			printf("2410 A....\n");
    	}	
    	else
    	{
    			Switch_Slave_Addr=0xAC;
    			data[0]=0x1f;
    			printf("2410 ....\n");
    	}
    	
    	data[1]=0xc2;
  	u1SwitchI2CWrite(Switch_Slave_Addr,0,data,2);
  	u1SwitchI2CRead(Switch_Slave_Addr,0,data,2);
  	printf("data[0]=%x data[1]=%x\n",data[0],data[1]);
}

void vHDMIHPDHigh(UINT8 bSwitch)
{
    	UINT8 data[2];
    	
  	 u1SwitchI2CRead(Switch_Slave_Addr,0,data,2);  	 
	switch(bSwitch) 
	{
		case HDMI_SWITCH_1:
			  	 data[0]=data[0]&0xfe;
			  	 data[0]=data[0]|0x01;
				break;
		case HDMI_SWITCH_2:
				data[0]=data[0]&0xfd;
			  	data[0]=data[0]|0x02;
				break;
		case HDMI_SWITCH_3:
				data[0]=data[0]&0xfb;
			  	data[0]=data[0]|0x04;
				break;
		case HDMI_SWITCH_4: // SIDE
				data[0]=data[0]&0xf7;
			  	data[0]=data[0]|0x08;
				break;
		default:
				break;
				
	}	
	u1SwitchI2CWrite(Switch_Slave_Addr,0,data,2);
}

void vHDMIHPDLow(UINT8 bSwitch)
{
    	UINT8 data[2];
    	
  	 u1SwitchI2CRead(Switch_Slave_Addr,0,data,2);  	 
	switch(bSwitch) 
	{
		case HDMI_SWITCH_1:
			  	 data[0]=data[0]&0xfe;
				break;
		case HDMI_SWITCH_2:
				data[0]=data[0]&0xfd;
				break;
		case HDMI_SWITCH_3:
				data[0]=data[0]&0xfb;
				break;
		case HDMI_SWITCH_4: // SIDE
				data[0]=data[0]&0xf7;
				break;
		default:
				break;
				
	}	
	u1SwitchI2CWrite(Switch_Slave_Addr,0,data,2);
}

//====================//
UINT8 bHDMIPort5VStatus(UINT8 port)
{

UINT8 data[2];
	if(u1SwitchI2CRead(Switch_Slave_Addr,0,data,2))
	{
		//if(data[1]&0x2==0x2)
		{
			switch(port)
			{
			       case HDMI_SWITCH_1: 
			       	return (data[1]&0x4)>>2;
			       case HDMI_SWITCH_2: 
			       	return (data[1]&0x8)>>3;
			       case HDMI_SWITCH_3: 
			       	return (data[1]&0x10)>>4;
			       case HDMI_SWITCH_4: 
			       	return (data[1]&0x20)>>5;          
				default: 
					return 0;
			}
			//u1SwitchI2CWrite(Switch_Slave_Addr,0,);	
		}		
	}
	else
	{
		return 0;
	}

}

//====================//
void vHDMISwitchInit(void)
{    
	vSwitchI2CInit();
}

//====================//
void vHDMISetSwitch(UINT8 bSwitch)
{
    UINT8 i = 0;
    
    UINT8 data[2];
   u1SwitchI2CRead(Switch_Slave_Addr,0,data,2);
    	
	if (_bHDMICurrSwitch != bSwitch)
	{	
		_bHDMICurrSwitch = bSwitch;
		
		vHDMIHPDLow(bSwitch);
		data[0]=data[0]&0x3f;

		switch(bSwitch)
		{				
			case HDMI_SWITCH_1:				
				data[0]=data[0]|0x0;
				vRegWriteFldAlign(HDMI_DDC_PAD,0x0,HDMI_DDC_MUX_MASK);  //DDC pad select
				break;			
			case HDMI_SWITCH_2:
				data[0]=data[0]|0x40;
				vRegWriteFldAlign(HDMI_DDC_PAD,0x1,HDMI_DDC_MUX_MASK);
				break;
			case HDMI_SWITCH_3:		
				data[0]=data[0]|0x80;
				vRegWriteFldAlign(HDMI_DDC_PAD,0x2,HDMI_DDC_MUX_MASK);  //DDC pad select
				break;			
			case HDMI_SWITCH_4:
				data[0]=data[0]|0xc0;
				vRegWriteFldAlign(HDMI_DDC_PAD,0x3,HDMI_DDC_MUX_MASK);
				break;
				
			default: /* disable all */
				//vRegWriteFldAlign(ANA_INTF_2,0x0,RG_HDMI_SW_EN);
				vRegWriteFldAlign(HDMI_DDC_PAD,0x0,HDMI_DDC_MUX_MASK);
				break;
			 
		}
		u1SwitchI2CWrite(Switch_Slave_Addr,0,data,2);
		
		// HPD high for unselected ports, support CEC feature.
		for ( i = HDMI_SWITCH_1 ; i <= HDMI_PORT_LARGEST ; i++) 
		{
			if (i != bSwitch)
			{
				vHDMIHPDHigh(i);
			}
		}
		
		#if MUTE_TEMP
		vHDMIVideoOutOff();
		#endif
		
		_bHDMIState = HDMI_STATE_INIT;
	}
}
