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
 * @file HDMI customization file: Two hdmi ports on PCB MT5363M1V1
 *
 * @author josh.pan@mediatek.com
 */
#include "general.h"

#include "x_gpio.h"
#include "x_printf.h"

#include "hw_vdoin.h"
#include "hw_hdmi.h"

#include "drv_hdmi.h"
#include "drv_video.h"
#include "drv_adcset.h"

#include "sv_const.h"
#include "source_table.h"
#include "sif_if.h"

#define Chip_I2C_ChannelId  2		//It CANNOT be changed
#define I2C_TIMING	0x100
#define I2C_AddrNum 				1
#define Switch_Slave_Addr	0xc0

#define SUPPORT_F3 0
#define SUPPORT_WRITE_TO_MTP 1
#define SUPPORT_MTK_EDID 1
//Change the following definition for gpio customization
#define HDMI_PORT_LARGEST HDMI_SWITCH_4


//extern UINT16 SIF_Write(UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8 u1WordAddr,
 //                UINT8 *pu1Buf, UINT16 u2ByteCnt);
//extern UINT16 SIF_Read(UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8 u1WordAddr,
  //              UINT8 *pu1Buf, UINT16 u2ByteCnt);
#if SUPPORT_MTK_EDID
//extern CODE UINT8 HDMIEDID_CEC_192k_DeepColor12B_P0[256]; 
CODE UINT8 HDMIEDID[256] =
{
    0x00, 0xFF, 0xFF, 0xFF,  0xFF, 0xFF, 0xFF, 0x00,   0x36, 0x8B, 0x01, 0x00,  0x01, 0x01, 0x01, 0x01,
    0x01, 0x0F, 0x01, 0x03,  0x80, 0x3C, 0x22, 0x78,   0x0A, 0x0D, 0xC9, 0xA0,  0x57, 0x47, 0x98, 0x27,
    0x12, 0x48, 0x4C, 0xBF,  0xEF, 0x00, 0x01, 0x01,   0x01, 0x01, 0x01, 0x01,  0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01,  0x01, 0x01, 0x01, 0x1D,   0x00, 0x72, 0x51, 0xD0,  0x1E, 0x20, 0x6E, 0x28,
    0x55, 0x00, 0xC4, 0x8E,  0x21, 0x00, 0x00, 0x1E,   0x01, 0x1D, 0x80, 0x18,  0x71, 0x1C, 0x16, 0x20,
    0x58, 0x2C, 0x25, 0x00,  0xC4, 0x8E, 0x21, 0x00,   0x00, 0x9E, 0x00, 0x00,  0x00, 0xFC, 0x00, 0x4D,
    0x54, 0x4B, 0x20, 0x4C,  0x43, 0x44, 0x54, 0x56,   0x0A, 0x20, 0x20, 0x20,  0x00, 0x00, 0x00, 0xFD,
    0x00, 0x31, 0x4C, 0x0F,  0x50, 0x0E, 0x00, 0x0A,   0x20, 0x20, 0x20, 0x20,  0x20, 0x20, 0x01, 0x56,
    0x02, 0x03, 0x23, 0x74,  0x4B, 0x84, 0x10, 0x1F,   0x05, 0x13, 0x14, 0x01,  0x02, 0x11, 0x06, 0x15,
    0x26, 0x09, 0x7F, 0x03,  0x11, 0x7F, 0x18, 0x83,   0x01, 0x00, 0x00, 0x67,  0x03, 0x0C, 0x00, 0x10,
    0x00, 0xB8, 0x2D, 0x01,  0x1D, 0x00, 0xBC, 0x52,   0xD0, 0x1E, 0x20, 0xB8,  0x28, 0x55, 0x40, 0xC4,
    0x8E, 0x21, 0x00, 0x00,  0x1E, 0x01, 0x1D, 0x80,   0xD0, 0x72, 0x1C, 0x16,  0x20, 0x10, 0x2C, 0x25,
    0x80, 0xC4, 0x8E, 0x21,  0x00, 0x00, 0x9E, 0x8C,   0x0A, 0xD0, 0x8A, 0x20,  0xE0, 0x2D, 0x10, 0x10,
    0x3E, 0x96, 0x00, 0x13,  0x8E, 0x21, 0x00, 0x00,   0x18, 0x8C, 0x0A, 0xD0,  0x90, 0x20, 0x40, 0x31,
    0x20, 0x0C, 0x40, 0x55,  0x00, 0x13, 0x8E, 0x21,   0x00, 0x00, 0x18, 0x00,  0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x52
};

#endif
UINT8 bNXPCurPage=0xff;
// ------------------read NXP switch function--------------------
//-------wAddr: high byte is page num
//-------          low byte is reg address
//-------return: reg value

UINT8 bReadNXP19998(UINT16 wAddr)
{
	UINT8 bReturn,bPage,bReg;
	CRIT_STATE_T rCritState;

 
	bPage=(UINT8)((wAddr>>8)&0x00ff);

	bReg=(UINT8)(wAddr&0x00ff);

	rCritState = x_crit_start();
	if(bNXPCurPage!=bPage)
	{
		//SIF_Write(0x80,0xc0,0xff,&bPage,1);
		//SIF_X_Read(Chip_I2C_ChannelId,I2C_TIMING, Switch_Slave_Addr, I2C_AddrNum , 0xff, &bPage, 1);
		SIF_X_Write(Chip_I2C_ChannelId, I2C_TIMING, Switch_Slave_Addr, I2C_AddrNum ,0xff, &bPage, 1);
		bNXPCurPage=bPage;
	}
	//SIF_Read(0x80,0xc0,bReg,&bReturn,1);
	SIF_X_Read(Chip_I2C_ChannelId,I2C_TIMING, Switch_Slave_Addr, I2C_AddrNum , bReg, &bReturn, 1);
	x_crit_end(rCritState);
	return bReturn;
}
// ------------------write NXP switch function--------------------
//-------wAddr: high byte is page num
//-------          low byte is reg address
//-------value:  the data be ready to write
//-------return: none

void vWriteNXP19998(UINT16 wAddr,UINT8 bValue)
{
	UINT8 bPage,bReg;
	
	CRIT_STATE_T rCritState;
	bPage=(UINT8)((wAddr>>8)&0x00ff);
	bReg=(UINT8)(wAddr&0x00ff);
	rCritState = x_crit_start();
	if(bNXPCurPage!=bPage)
	{
		//SIF_Write(0x80,0xc0,0xff,&bPage,1);
		SIF_X_Write(Chip_I2C_ChannelId, I2C_TIMING, Switch_Slave_Addr, I2C_AddrNum ,0xff, &bPage, 1);
		bNXPCurPage=bPage;
	}

	//SIF_Write(0x80,0xc0,bReg,&bValue,1);
	SIF_X_Write(Chip_I2C_ChannelId, I2C_TIMING, Switch_Slave_Addr, I2C_AddrNum ,bReg, &bValue, 1);
	x_crit_end(rCritState);
}
//====================//
UINT8 bGetPHYAddrOffset(UINT8 *bPtrEDID)
{
	UINT8 i;
	UINT8 bOffset=0;
	
	for (i = 128 ; i < 256 ; i++)
		{
			if ((bPtrEDID[i] == 0x3) && \
				(bPtrEDID[i+1] == 0xc) && \
				 (bPtrEDID[i+2] == 0x0))
			{
				bOffset = i+3-128;
			}			

		}
	if(bOffset==0)
	{
		printf("\n can not find the physical address \n");
	}
	return bOffset;
}
UINT8 bGetCheckSum(UINT8 *bPtrEDID,UINT8 bPort)
{
	UINT8 u1Checksum;
	UINT8 u1Buf=0;
	 u1Checksum = (bPtrEDID[255] + 0x10)%256;
 	switch(bPort)
	{
	case HDMI_SWITCH_1:
		return bPtrEDID[255];
	case HDMI_SWITCH_2:
		if (u1Checksum >= 0x20)
		{
	    	u1Buf = u1Checksum - 0x20;
		}
		else
		{
	    	u1Buf = u1Checksum + (0x100 - 0x20);
		}
		return u1Buf;
	case HDMI_SWITCH_3:
		if (u1Checksum >= 0x30)
		{
	    	u1Buf = u1Checksum - 0x30;
		}
		else
		{
	    	u1Buf = u1Checksum + (0x100 - 0x30);
		}
		return u1Buf;
	case HDMI_SWITCH_4:
		if (u1Checksum >= 0x40)
		{
	    	u1Buf = u1Checksum - 0x40;
		}
		else
		{
	    	u1Buf = u1Checksum + (0x100 - 0x40);
		}
		return u1Buf;
 	}
}
#if 0
void vHDMI_NXP_SET_HPD(UINT8 bCase,UINT16 wDelay)
{
	UINT8 bData;
	if(wDelay>=135)
	{
		wDelay-=135;
		
	}
	else
	{
		wDelay=0;
	}
	bData=(wDelay/25);
	switch(bCase)
	{
		case 1:
			vWriteNXP19998(0x2192,0x10);
			break;
		case 2:
			vWriteNXP19998(0x2192,0x11);
			vWriteNXP19998(0x2193,bData);
			break;
		case 3:
			vWriteNXP19998(0x2192,0x18);
			break;
		case 4:
			vWriteNXP19998(0x2192,0x1a);
			vWriteNXP19998(0x2193,bData);
			break;
		case 5:
			vWriteNXP19998(0x2192,0x19);
			vWriteNXP19998(0x2193,bData);
			break;
		case 6:
			vWriteNXP19998(0x2192,0x1b);
			vWriteNXP19998(0x2193,bData);
			break;
		case 7:
			vWriteNXP19998(0x2192,0x1c);
			vWriteNXP19998(0x2193,bData);
			break;
		case 8:
			vWriteNXP19998(0x2192,0x38);
			vWriteNXP19998(0x2193,bData);
			break;
		default:
			break;
	}
}

//====================//

void vHDMI_NXP_Set_Termination(UINT8 bCase)
{
	switch(bCase)
	{
		case 1:
			vWriteNXP19998(0x3000,0x70);
			break;
		case 2:
			vWriteNXP19998(0x3000,0x71);
			break;
		case 3:
			vWriteNXP19998(0x3000,0x78);
			break;
		case 4:
			vWriteNXP19998(0x3000,0x7a);
			break;
		case 5:
			vWriteNXP19998(0x3000,0x79);
			break;
		case 6:
			vWriteNXP19998(0x3000,0x7b);
			break;
		default:
			break;
	}
}
#endif

//====================//


void vHDMI_NXP_RAM_Init()
{
	UINT8 i;
	UINT8 bData;
	printf("Load HDMI internal EDID.................\n ");
	#if !SUPPORT_F3
	vWriteNXP19998(0x0002,0x0f);  // set HPD is manual mode, and all port HPD is high.
	vWriteNXP19998(0x0003,0x00);  // set HPD is manual mode, and all port HPD is high.
	#else
	vWriteNXP19998(0x0002,0x8f);  // set HPD is auto mode
	vWriteNXP19998(0x0003,0x8f);  // set RT is AUTO mode
	#endif
	#if SUPPORT_MTK_EDID
	i=0;
	while(i<128)
	{

		bData=HDMIEDID[i];
		vWriteNXP19998(0x2000+i,bData);
		i++;
	}
	printf("finish first 128 byte \n");

	i=0;
	while(i<128)
	{
		
		
		bData=HDMIEDID[i+128];
		vWriteNXP19998(0x2100+i,bData);
		i++;
	}
	printf("finish second 128 byte \n");
	#endif

	
	vWriteNXP19998(0x2180,0x7f);
	vWriteNXP19998(0x2181,bGetPHYAddrOffset(HDMIEDID));
	// port1 physical address
	vWriteNXP19998(0x2182,0x10);
	vWriteNXP19998(0x2183,0);
	// port2 physical address
	vWriteNXP19998(0x2184,0x20);
	vWriteNXP19998(0x2185,0);
	// port3 physical address
	vWriteNXP19998(0x2186,0x30);
	vWriteNXP19998(0x2187,0);
	// port4 physical address
	vWriteNXP19998(0x2188,0x40);
	vWriteNXP19998(0x2189,0);
	// port1 checksum
	vWriteNXP19998(0x218c,HDMIEDID[255]);
	// port2 checksum
	
	vWriteNXP19998(0x218d,bGetCheckSum(HDMIEDID,2));
	// port3 checksum
	vWriteNXP19998(0x218e,bGetCheckSum(HDMIEDID,3));
	// port4 checksum
	vWriteNXP19998(0x218f,bGetCheckSum(HDMIEDID,4));



	vWriteNXP19998(0x2191,0x15);
	vWriteNXP19998(0x2192,0x38);
	vWriteNXP19998(0x2193,0x22);
	vWriteNXP19998(0x2194,0x00);
	vWriteNXP19998(0x2195,0x30);
	printf("finish 3 stage \n");
	

	
	// page30
	vWriteNXP19998(0x3000,0x68);  
	vWriteNXP19998(0x3001,0x00);
	vWriteNXP19998(0x3002,0x00);
	vWriteNXP19998(0x3003,0x00);
	//for(i=0;i<7;i++)
	//{
	//	vWriteNXP19998(0x0009+i,0xff);
	//}

}
//====================//

void vHDMI_NXP_EDID_Init()
{
	//UINT8 i;
	#if SUPPORT_WRITE_TO_MTP
	UINT8 bData=0;
	#endif
	//vHDMIStopFw();
	vHDMI_NXP_RAM_Init();

	//page00
	
	// start writing data to E-MTP
	#if SUPPORT_WRITE_TO_MTP
	vWriteNXP19998(0x00f0,0x02);
	do
	{
		bData=bReadNXP19998(0x000a);
	}while((bData&8)==0);
	printf("finish 4 stage \n");
	
	// start writing data to C-MTP
	vWriteNXP19998(0x00f3,0);
	vWriteNXP19998(0x00f1,8);
	do
	{
		bData=bReadNXP19998(0x000f);
	}while((bData&1)==0);
	printf("finish 5 stage \n");
	
	vWriteNXP19998(0x000f,1);
	vWriteNXP19998(0x00f3,2);
	vWriteNXP19998(0x00f1,8);
	do
	{
		bData=bReadNXP19998(0x000f);
	}while((bData&1)==0);
	printf("finish 6 stage \n");
	//vHDMIResumeFw();
	#endif
}

//====================//
void vHDMISwitchDDCBusCtrl(UINT8 bSwitch,UINT8 fgOnOff)
{
	UINT8 bTemp;
	UINT8 bTemp2;
	if(bSwitch>HDMI_PORT_LARGEST)
	{
		return;
	}
	else
	{
		bTemp=bReadNXP19998(0x2180);
		switch(bSwitch)
		{
			case HDMI_SWITCH_1:
				bTemp2=0;
				break;
			case HDMI_SWITCH_2:
				bTemp2=1;
				break;
			case HDMI_SWITCH_3:
				bTemp2=2;
				break;
			case HDMI_SWITCH_4
				bTemp2=3;
				break;
			default:
				bTemp2=0;
				break;
		}
		if(fgOnOff)
		{
			//vWriteNXP19998(0x2180,bTemp|((UINT8)1<<(bSwitch-HDMI_SWITCH_1)));
			vWriteNXP19998(0x2180,bTemp|((UINT8)1<<(bTemp2)));
			vWriteNXP19998(0x0006,0x00);
		}
		else
		{
			//vWriteNXP19998(0x2180,bTemp&(~((UINT8)1<<(bSwitch-HDMI_SWITCH_1))));
			vWriteNXP19998(0x2180,bTemp&(~((UINT8)1<<(bTemp2))));
			vWriteNXP19998(0x0006,0x04);
		}
	}
}
//====================//

void vHDMISwitchCLKTerm(UINT8 bSwitch,UINT8 fgOnOff)
{
	#if !SUPPORT_F3
	UINT8 bTemp;
	UINT8 bTemp2;
	if(bSwitch>HDMI_PORT_LARGEST)
	{
		return;
	}
	else
	{
		bTemp=bReadNXP19998(0x0003);
		switch(bSwitch)
		{
			case HDMI_SWITCH_1:
				bTemp2=0;
				break;
			case HDMI_SWITCH_2:
				bTemp2=1;
				break;
			case HDMI_SWITCH_3:
				bTemp2=2;
				break;
			case HDMI_SWITCH_4:
				bTemp2=3;
				break;
			default:
				bTemp2=0;
				break;
		}
		if(fgOnOff)
		{
			
			//vWriteNXP19998(0x0003,bTemp|((UINT8)1<<(bSwitch-HDMI_SWITCH_1)));
			vWriteNXP19998(0x0003,bTemp|((UINT8)1<<(bTemp2)));
		}
		else
		{
			//vWriteNXP19998(0x0003,bTemp&(~((UINT8)1<<(bSwitch-HDMI_SWITCH_1))));
			vWriteNXP19998(0x0003,bTemp&(~((UINT8)1<<(bTemp2))));
		}
	}
	#else
	return;
	#endif
}
//====================//

void vHDMIHPDHigh(UINT8 bSwitch)
{
	#if !SUPPORT_F3
	UINT8 bTemp;
	UINT8 bTemp2;
   if(bSwitch>HDMI_PORT_LARGEST)
    {
   		return;
    }
   else
   	{
		bTemp=bReadNXP19998(0x0002);
		switch(bSwitch)
		{
			case HDMI_SWITCH_1:
				bTemp2=0;
				break;
			case HDMI_SWITCH_2:
				bTemp2=1;
				break;
			case HDMI_SWITCH_3:
				bTemp2=2;
				break;
			case HDMI_SWITCH_4
				bTemp2=3;
				break;
			default:
				bTemp2=0;
				break;
		}
		//vWriteNXP19998(0x0002,bTemp|((UINT8)1<<(bSwitch-HDMI_SWITCH_1)));
		vWriteNXP19998(0x0002,bTemp|((UINT8)1<<(bTemp2)));
   	}
   #else
	return;
   #endif
}

//====================//
void vHDMIHPDLow(UINT8 bSwitch)
{
	#if !SUPPORT_F3
	UINT8 bTemp;
	UINT8 bTemp2;
   if(bSwitch>HDMI_PORT_LARGEST)
    {
   		return;
    }
   else
   	{
		bTemp=bReadNXP19998(0x0002);
		switch(bSwitch)
		{
			case HDMI_SWITCH_1:
				bTemp2=0;
				break;
			case HDMI_SWITCH_2:
				bTemp2=1;
				break;
			case HDMI_SWITCH_3:
				bTemp2=2;
				break;
			case HDMI_SWITCH_4
				bTemp2=3;
				break;
			default:
				bTemp2=0;
				break;
		}
		//vWriteNXP19998(0x0002,bTemp&(~((UINT8)1<<(bSwitch-HDMI_SWITCH_1))));
		vWriteNXP19998(0x0002,bTemp&(~((UINT8)1<<(bTemp2))));
   	}
	#else
	return;
   #endif
}

//====================//
UINT8 bHDMIPort5VStatus(UINT8 port)
{
	UINT8 bData;
	// write page addr
	//bData=0;
	//SIF_Write(0x80,0xc0,0xff,&bData,1);
	// read POWER5V status
	//SIF_Read(0x80,0xc0,0x20,&bData,1);
	bData=bReadNXP19998(0x0020);
	switch(port)
	{
		case HDMI_SWITCH_1:
			 return ((bData&1)>0?1:0);
		case HDMI_SWITCH_2:
			 return ((bData&2)>0?1:0);
		case HDMI_SWITCH_3:
			 return ((bData&4)>0?1:0);
		case HDMI_SWITCH_4:
			 return ((bData&8)>0?1:0);
		default:
		     return 0;
	}
}

//====================//
void vHDMISwitchInit(void)
{    
	//printf(".................vHDMI_NXP_RAM_Init \n");
	//vHDMI_NXP_RAM_Init();
}

//====================//
void vHDMISetSwitch(UINT8 bSwitch)
{
   UINT8 i=0;
   if(bSwitch>HDMI_PORT_LARGEST)
   	{
   		return;
   	}
   else
   	{
   
	if (_bHDMICurrSwitch != bSwitch)
	{	
		_bHDMICurrSwitch = bSwitch;
		
		//SIF_Write(0x80,0xc0,0xff,&i,1);
		switch (bSwitch)
		{
			case HDMI_SWITCH_1:
				i=0;
				break;
			case HDMI_SWITCH_2:
				i=1;
				break;
			case HDMI_SWITCH_3:
				i=2;
				break;
			case HDMI_SWITCH_4
				i=3;
				break;
			default:
				i=0;
				break;
		}
		//i=(bSwitch-HDMI_SWITCH_1);
		//SIF_Write(0x80,0xc0,0x01,&i,1);
		vWriteNXP19998(0x0001,i);
		#if MUTE_TEMP
		vHDMIVideoOutOff();
		#endif
		_bHDMIState = HDMI_STATE_INIT;
	}
   	}
}


