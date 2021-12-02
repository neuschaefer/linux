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
#include "ir_if.h"
#include "sif_if.h"
#include "rtc_if.h"
#include "vdp_if.h"
#include "nptv_if.h"
#include "x_hal_926.h"
#include "x_hal_5381.h"
#include "x_printf.h"
#include "x_stl_lib.h"
#include "x_pdwnc.h"
#include "pdwnc_if.h"
#include "x_os.h"
#include "source_table.h"
#ifdef __MW_CLI_DEF__
#include "x_mid_cli.h"
#else
#include "x_drv_cli.h"
#endif

#if defined(CC_MT5387) || defined(CC_MT5363)
#include "hw_ckgen.h"
#include "uart_hw_drvif.h"
#endif

//---------------------------------------------------------------------
// LOG macro related

// #define DEFINE_IS_LOG	CLI_IsLog
#define DEFINE_IS_LOG	CLI_IsLog

#include "x_debug.h"
#include "x_util.h"
#include "x_lint.h"

/*****************************************************************************/

#include "drv_common.h"
#include "drvcust_if.h"

EXTERN UINT32 IRRX_ReadSvadc(UINT32 u4Channel);
EXTERN void HAL_Delay_us(UINT32 u4Micros);
EXTERN INT32 _PDWNC_SetT8032(UINT32 u4Set);
EXTERN void PDWNC_SetupPowerDown(void); 
//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

CLIMOD_DEBUG_FUNCTIONS(PDWNC)

#if defined(CC_MT5368)||defined(CC_MT5396)||defined(CC_MT5389)
static UINT32 SetupServoADC(UINT32 u4ServoId, UINT32 u4Upper, UINT32 u4Lower )
{
	UINT32 i;
	UINT32 u4Val,u4Id;

	// disable ServoADC working.
/*
	u4Val = PDWNC_READ32(REG_RW_SRVCFG1);
	u4Val &= ~(SRVCH_EN_SRVCFG);
	PDWNC_WRITE32(REG_RW_SRVCFG1, u4Val);
*/
	vIO32WriteFldAlign(PDWNC_SRVCFG1, 0x300, FLD_SRVCH_EN); //servo8/9 must always work on mt5387
	
	// Clear ServoADC value.
/*	  u4Val = ADCOUT_CLR;
	PDWNC_WRITE32(REG_RW_SRVCLR, u4Val);
*/	  
	vIO32WriteFldAlign(PDWNC_SRVCLR, 0x1, FLD_ADCLR);
	
	// Clear ServoADC register
	for (i=0; i<6; i++)//MAX_SERVOADC_NUM
	{
		vIO32WriteFldAlign(PDWNC_WAKTOP0 + (12 * i), 0x0, FLD_CH0TOP);
		vIO32WriteFldAlign(PDWNC_WAKBTM0 + (12 * i), 0x0, FLD_CH0BTM);
		vIO32WriteFldAlign(PDWNC_CHCFG0 + (12 * i), 0x0, FLD_CH0WKC);
	}
		 vIO32WriteFldAlign(PDWNC_WAKTOP6 , 0x0, FLD_CH0TOP);
		vIO32WriteFldAlign(PDWNC_WAKBTM6 , 0x0, FLD_CH0BTM);
		vIO32WriteFldAlign(PDWNC_CHCFG6 , 0x0, FLD_CH0WKC);

		vIO32WriteFldAlign(PDWNC_WAKTOP7 , 0x0, FLD_CH0TOP);
		vIO32WriteFldAlign(PDWNC_WAKBTM7, 0x0, FLD_CH0BTM);
		vIO32WriteFldAlign(PDWNC_CHCFG7 , 0x0, FLD_CH0WKC);
	

	// Make sure enable PAD
	u4Val = PDWNC_ReadServoADCChannelValue(u4ServoId);	 
	if(u4ServoId<6)
	{
		vIO32Write4B(PDWNC_WAKTOP0 + (12 * u4ServoId), u4Upper);	   
		vIO32Write4B(PDWNC_WAKBTM0 + (12 * u4ServoId), u4Lower);		   
		u4Val = (CH_WAK_MID_EN & CH_MASK);
		vIO32Write4B(PDWNC_CHCFG0 + (12 * u4ServoId), u4Val);
	}
	else	//for servoad 6/7
	{
		u4Id = u4ServoId - 6;
		vIO32Write4B(PDWNC_WAKTOP6 + (12 * u4Id), u4Upper);	   
		vIO32Write4B(PDWNC_WAKBTM6 + (12 * u4Id), u4Lower);		   
		u4Val = (CH_WAK_MID_EN & CH_MASK);
		vIO32Write4B(PDWNC_CHCFG6 + (12 * u4Id), u4Val);
	}
	u4Val = (1 << u4ServoId);
	vIO32Write4B(PDWNC_WAKEN, SRVAD_WAKEN(u4Val));//PDWNC_WRITE32(REG_RW_PDWAKE_EN, SRVAD_WAKEN(u4Val));

	u4Val = 0;
	vIO32Write4B(PDWNC_ADOUT0 + (4 * u4ServoId), u4Val);		   
	
	u4Val = SRVAD_WAKEN(WAK_SRVADX(u4ServoId));
	
	//PDWNC_EnterPowerDown(u4Val, (UINT32)PDWNC_POWER_DOWN_UNKNOWN);
	//vIO32WriteFldAlign(PDWNC_PDCODE, PWRDWN_CODE1, FLD_PDCODE);
	//vIO32WriteFldAlign(PDWNC_PDCODE, PWRDWN_CODE2, FLD_PDCODE);
	return u4Val;
}
#else
static UINT32 SetupServoADC(UINT32 u4ServoId, UINT32 u4Upper, UINT32 u4Lower )
{
    UINT32 i;
    UINT32 u4Val;

    // disable ServoADC working.
/*
    u4Val = PDWNC_READ32(REG_RW_SRVCFG1);
    u4Val &= ~(SRVCH_EN_SRVCFG);
    PDWNC_WRITE32(REG_RW_SRVCFG1, u4Val);
*/
    vIO32WriteFldAlign(PDWNC_SRVCFG1, 0xC0, FLD_SRVCH_EN); //servo6/7 must always work on mt5387
    
    // Clear ServoADC value.
/*    u4Val = ADCOUT_CLR;
    PDWNC_WRITE32(REG_RW_SRVCLR, u4Val);
*/    
    vIO32WriteFldAlign(PDWNC_SRVCLR, 0x1, FLD_ADCLR);
    // Clear ServoADC register
    for (i=0; i<MAX_SERVOADC_NUM; i++)
    {
        u4Val = 0;
        vIO32Write4B(PDWNC_CHCFG0 + (12 * i), u4Val); // PDWNC_WRITE32(REG_RW_CHCFG0 + (12 * i), u4Val);        
        vIO32Write4B(PDWNC_WAKTOP0 + (12 * i), u4Val);// PDWNC_WRITE32(REG_RW_WAKTOP0 + (12 * i), u4Val);                
        vIO32Write4B(PDWNC_WAKBTM0  + (12 * i), u4Val); //PDWNC_WRITE32(REG_RW_WAKBTM0 + (12 * i), u4Val);                        
    }

    // Make sure enable PAD
    u4Val = PDWNC_ReadServoADCChannelValue(u4ServoId);   
    
    vIO32Write4B(PDWNC_WAKTOP0 + (12 * u4ServoId), u4Upper);       
    vIO32Write4B(PDWNC_WAKBTM0 + (12 * u4ServoId), u4Lower);           
    u4Val = (CH_WAK_MID_EN & CH_MASK);
    vIO32Write4B(PDWNC_CHCFG0 + (12 * u4ServoId), u4Val);       
    u4Val = (1 << u4ServoId);
    vIO32Write4B(PDWNC_WAKEN, SRVAD_WAKEN(u4Val));//PDWNC_WRITE32(REG_RW_PDWAKE_EN, SRVAD_WAKEN(u4Val));

    u4Val = 0;
    vIO32Write4B(PDWNC_ADOUT0 + (4 * u4ServoId), u4Val);           
    
    u4Val = SRVAD_WAKEN(WAK_SRVADX(u4ServoId));
    
    //PDWNC_EnterPowerDown(u4Val, (UINT32)PDWNC_POWER_DOWN_UNKNOWN);
    //vIO32WriteFldAlign(PDWNC_PDCODE, PWRDWN_CODE1, FLD_PDCODE);
    //vIO32WriteFldAlign(PDWNC_PDCODE, PWRDWN_CODE2, FLD_PDCODE);
    return u4Val;
}
#endif
static INT32 _CLI_PowerDown_NormalWakeup(INT32 i4Argc, const CHAR ** szArgv)
{    
    PDWNC_EnterPowerDown(WAK_IRRX, (UINT32)PDWNC_POWER_DOWN_UNKNOWN);
    return 0;    
}
static INT32 _CLI_PowerDown_PowerSavingWakeup(INT32 i4Argc, const CHAR ** szArgv)
{    
    PDWNC_EnterPowerDown(WAK_IRRX | WAK_VGA, (UINT32)PDWNC_POWER_DOWN_UNKNOWN);
    return 0;    
}
static INT32 _CLI_PowerDown_IrWakeup(INT32 i4Argc, const CHAR ** szArgv)
{    
    PDWNC_EnterPowerDown(WAK_IRRX, (UINT32)PDWNC_POWER_DOWN_UNKNOWN);
    return 0;    
}
static INT32 _CLI_PowerDown_IrServoWakeup(INT32 i4Argc, const CHAR ** szArgv)
{    
    UINT32 u4ServoId;
    UINT32 u4Lower, u4Upper;

    if(i4Argc < 4)
    {
        Printf("[ServoADC #Num] [Lower] [Upper]");
        return -1024;
    }
    
    u4ServoId = StrToInt(szArgv[1]);  
    u4Lower = StrToInt(szArgv[2]);      
    u4Upper = StrToInt(szArgv[3]);          

    PDWNC_EnterPowerDown(WAK_IRRX | SetupServoADC(u4ServoId, u4Upper, u4Lower), (UINT32)PDWNC_POWER_DOWN_UNKNOWN);
    return 0;    
}
static INT32 _CLI_PowerDown_IrServoVgaWakeup(INT32 i4Argc, const CHAR ** szArgv)
{    
    UINT32 u4ServoId;
    UINT32 u4Lower, u4Upper;

    if(i4Argc < 4)
    {
        Printf("[ServoADC #Num] [Lower] [Upper]");
        return -1024;
    }
    
    u4ServoId = StrToInt(szArgv[1]);  
    u4Lower = StrToInt(szArgv[2]);      
    u4Upper = StrToInt(szArgv[3]);          

    PDWNC_EnterPowerDown(WAK_IRRX | WAK_VGA | SetupServoADC(u4ServoId, u4Upper, u4Lower), (UINT32)PDWNC_POWER_DOWN_UNKNOWN);
    return 0;    
}
static INT32 _CLI_PowerDown_RtcWakeup(INT32 i4Argc, const CHAR ** szArgv)
{    
    PDWNC_EnterPowerDown(WAK_RTC, (UINT32)PDWNC_POWER_DOWN_UNKNOWN);
    return 0;    
}
static INT32 _CLI_PowerDown_GpioWakeup(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Val;
    UINT32 u4Polarity;

    if(i4Argc < 3)
    {
        return -1024;
    }
    
    u4Val = StrToInt(szArgv[1]);     
    u4Polarity = StrToInt(szArgv[2]);
    vIO32WriteFldAlign(PDWNC_PDIO, u4Polarity << u4Val, FLD_EXTIO_POL);//PDWNC_WRITE32(REG_RW_PWDIOCFG, u4Polarity << u4Val);    
	#if defined(CC_MT5396)
	if(u4Val>7)	//FLD_EXTIO_WAKEN_11_8 is [13:10]
		{PDWNC_EnterPowerDown(((1U<<2)<< u4Val), (UINT32)PDWNC_POWER_DOWN_UNKNOWN);}
	else
		{PDWNC_EnterPowerDown((1U << u4Val), (UINT32)PDWNC_POWER_DOWN_UNKNOWN);}
	#else	
		PDWNC_EnterPowerDown((1U << u4Val), (UINT32)PDWNC_POWER_DOWN_UNKNOWN);
	#endif
    return 0;    
}
static INT32 _CLI_PowerDown_ServoWakeup(INT32 i4Argc, const CHAR ** szArgv)
{    
    UINT32 u4ServoId;
    UINT32 u4Lower, u4Upper;

    if(i4Argc < 4)
    {
        Printf("[ServoADC #Num] [Lower] [Upper]");
        return -1024;
    }
	    u4ServoId = StrToInt(szArgv[1]);  
    u4Lower = StrToInt(szArgv[2]);      
    u4Upper = StrToInt(szArgv[3]); 
         
	PDWNC_EnterPowerDown(SetupServoADC(u4ServoId, u4Upper, u4Lower), (UINT32)PDWNC_POWER_DOWN_UNKNOWN);

    return 0;
}

#if 1
static INT32 _CLI_XdataArmReadBack(INT32 i4Argc, const CHAR ** szArgv)
{
#ifdef CC_MT5387  
	UINT32 u48032XdataStartAddr =0x4000;
#else
	UINT32 u48032XdataStartAddr =0x8000;
#endif
    UINT32 i,j=0;
	UINT8 ui1Data=0xff;
	//UINT32 u4data=0xFF;//
	UINT32 u4IntStatus=0;
	UINT32 ui4StartAddr=0;
	UINT32 ui4Length=0;
	UINT32 ui4TmpAddr=0;
	UINT8 ui1ChkSumData=0xff;
	BOOL fgChkSumRst=TRUE;
	UINT32* pu1Buf = NULL;	
		
    if(i4Argc < 3)
    {
    	Printf("armwr startaddr(0~2047) length(0~2048)\n");
        return -1024;
    }
	
    ui4StartAddr = StrToInt(szArgv[1]);
    ui4Length = StrToInt(szArgv[2]);
	
	if(ui4StartAddr >2047 || (ui4StartAddr+ui4Length) > 2048)
	{
		Printf("too big range\n");
		return 1;
	}
	
	pu1Buf = x_mem_alloc(ui4Length*4);	
	if(NULL == pu1Buf) 
	{
		Printf("Can't allocate data by x_mem_alloc\n");
		return 1;
	}	
	
    for(i = 0; i <ui4Length ; i++)
    {
        *(pu1Buf+i) = 0;
    }
	
	u4IntStatus = (UINT32)IO_READ32(PDWNC_T8032_INTEN,0);
	IO_WRITE32(PDWNC_T8032_INTEN,0,0);//clear all interrupt

    vIO32WriteFldAlign(PDWNC_UP_CFG, 0x1, FLD_XDATA_ACC);//switch xdata to ARM    
    vIO32WriteFldAlign(PDWNC_UP_ADDR, u48032XdataStartAddr+ui4StartAddr , FLD_UP_ADDR);

//read back xdata from t8032 and compare
	Printf("\nRead back to check sum\n");
    HAL_Delay_us(10000);

	ui1Data=0;
	ui4TmpAddr=u48032XdataStartAddr+ui4StartAddr;

    vIO32WriteFldAlign(PDWNC_UP_ADDR, ui4TmpAddr, FLD_UP_ADDR);

    for(i = 0; i < ui4Length; i++)
    {
		ui1Data = (UINT8)IO_READ8(PDWNC_UP_DATA,0);
        Printf("[R]addr: 0x%x=0x%02x; ",  (ui4TmpAddr+ i), (ui1Data));

		if(ui1ChkSumData != ui1Data ) 
			{fgChkSumRst = FALSE;
			*(pu1Buf+j) = ui4TmpAddr+i;
			j++;
			}
		(ui1ChkSumData==0)?(ui1ChkSumData=0xff):(ui1ChkSumData--);	
		if((i+1)%4 ==0) Printf("\n");

	  HAL_Delay_us(10000);
    }   

	
	if(fgChkSumRst)
	{
		Printf("\nRead back Compare OK\n ");
	}
	else
	{
		 Printf("\nRead back Compare NG\n ");
		 for(i=0;i<j;i++)
		 	{
			 Printf("\nFail at: 0x%x",*(pu1Buf+i));
		 	}
	}

    HAL_Delay_us(10000);
    vIO32WriteFldAlign(PDWNC_UP_CFG, 0x0, FLD_XDATA_ACC);//switch xdata to 8032  
    HAL_Delay_us(10000);
	IO_WRITE32(PDWNC_T8032_INTEN,0,u4IntStatus);//restore all interrupt
   
   	x_mem_free(pu1Buf);
   
	return 0;

}

static INT32 _CLI_XdataArmWriteRead(INT32 i4Argc, const CHAR ** szArgv)
{
#ifdef CC_MT5387  
	UINT32 u48032XdataStartAddr =0x4000;
#else
	UINT32 u48032XdataStartAddr =0x8000;
#endif
    UINT32 i,j=0;
	UINT8 ui1Data=0xff;
	UINT32 u4data=0xFF;
	UINT32 u4IntStatus=0;
	UINT32 ui4StartAddr=0;
	UINT32 ui4Length=0;
	UINT32 ui4TmpAddr=0;
	UINT8 ui1ChkSumData=0xff;
	BOOL fgChkSumRst=TRUE;
	UINT32* pu1Buf = NULL;	

    if(i4Argc < 3)
    {
    	Printf("armwr startaddr(0~2047) length(0~2048)\n");
        return -1024;
    }
	
    ui4StartAddr = StrToInt(szArgv[1]);
    ui4Length = StrToInt(szArgv[2]);

	if(ui4StartAddr >2047 || (ui4StartAddr+ui4Length) > 2048)
	{
		Printf("too big range\n");
		return 1;
	}
	
	pu1Buf = x_mem_alloc(ui4Length*4);	
	if(NULL == pu1Buf) 
	{
		Printf("Can't allocate data by x_mem_alloc\n");
		return 1;
	}	
	
    for(i = 0; i <ui4Length ; i++)
    {
        *(pu1Buf+i) = 0;
    }
	
	u4IntStatus = (UINT32)IO_READ32(PDWNC_T8032_INTEN,0);
	IO_WRITE32(PDWNC_T8032_INTEN,0,0);//clear all interrupt

    vIO32WriteFldAlign(PDWNC_UP_CFG, 0x1, FLD_XDATA_ACC);//switch xdata to ARM    
    vIO32WriteFldAlign(PDWNC_UP_ADDR, u48032XdataStartAddr+ui4StartAddr , FLD_UP_ADDR);
//write xdata
	ui4TmpAddr=u48032XdataStartAddr+ui4StartAddr ;
	Printf("Do write\n");

    for(i = 0; i < ui4Length; i++)
    {
		 IO_WRITE32(PDWNC_UP_DATA, 0,u4data);

		 Printf("[W]addr: 0x%x=0x%02x; ",  ui4TmpAddr+i, u4data);
		 (u4data ==0)?(u4data=0xff):(u4data--);	
		  
		if((i+1)%4 ==0) Printf("\n");	
	 
	  HAL_Delay_us(10000);
    }   

//read back xdata and compare
	Printf("\nRead back to check sum\n");
    HAL_Delay_us(10000);

	ui1Data=0;
	ui4TmpAddr=u48032XdataStartAddr+ui4StartAddr;

    vIO32WriteFldAlign(PDWNC_UP_ADDR, ui4TmpAddr, FLD_UP_ADDR);

    for(i = 0; i < ui4Length; i++)
    {
		ui1Data = (UINT8)IO_READ8(PDWNC_UP_DATA,0);
        Printf("[R]addr: 0x%x=0x%02x; ",  (ui4TmpAddr+ i), (ui1Data));

		if(ui1ChkSumData != ui1Data ) 
			{fgChkSumRst =FALSE;
			*(pu1Buf+j)=ui4TmpAddr+ i;
			j++;
			}
		(ui1ChkSumData==0)?(ui1ChkSumData=0xff):(ui1ChkSumData--);	
		if((i+1)%4 ==0) Printf("\n");

	  HAL_Delay_us(10000);
    }   

	
	if(fgChkSumRst)
	{
		Printf("\nWrite and Read Compare OK\n ");
	}
	else
	{
		 Printf("\nWrite and Read Compare NG\n ");
		 for(i=0;i<j;i++)
		 	{
			 Printf("\nFail at: 0x%x",*(pu1Buf+i));
		 	}
	}

    HAL_Delay_us(10000);
    vIO32WriteFldAlign(PDWNC_UP_CFG, 0x0, FLD_XDATA_ACC);//switch xdata to 8032  
    HAL_Delay_us(10000);
	IO_WRITE32(PDWNC_T8032_INTEN,0,u4IntStatus);//restore all interrupt
	
   	x_mem_free(pu1Buf);
	
	return 0;

}
	
	
static INT32 _CLI_XdataArmClear(INT32 i4Argc, const CHAR ** szArgv)
{
#ifdef CC_MT5387 
		UINT32 u48032XdataStartAddr =0x4000;
#else
		UINT32 u48032XdataStartAddr =0x8000;
#endif
		UINT32 i;
	//	UINT8 ui1Data=0xff;
		UINT32 u4data=0;
		UINT32 u4IntStatus=0;
		UINT32 ui4StartAddr=0;
		UINT32 ui4Length=0;
		//UINT32 ui4TmpAddr=0;
	//	UINT8 ui1ChkSumData=0xff;
	//	BOOL fgChkSumRst=TRUE;
	
		if(i4Argc < 3)
		{
			Printf("armwr startaddr(0~2047) length(0~2048)\n");
			return -1024;
		}
	
	
		if(ui4StartAddr >2047 ||ui4Length >2048 )
		{
			Printf("too big range\n");
		}
	
		ui4StartAddr = StrToInt(szArgv[1]);
		ui4Length = StrToInt(szArgv[2]);
		
		u4IntStatus = (UINT32)IO_READ32(PDWNC_T8032_INTEN,0);
		IO_WRITE32(PDWNC_T8032_INTEN,0,0);//clear all interrupt
	
		vIO32WriteFldAlign(PDWNC_UP_CFG, 0x1, FLD_XDATA_ACC);//switch xdata to ARM	  
		vIO32WriteFldAlign(PDWNC_UP_ADDR, u48032XdataStartAddr+ui4StartAddr , FLD_UP_ADDR);
	
		//ui4TmpAddr=u48032XdataStartAddr+ui4StartAddr ;
		Printf("Do clear\n");
	
		for(i = 0; i < ui4Length; i++)
		{
			 IO_WRITE32(PDWNC_UP_DATA, 0,u4data);
	
			 //Printf("[W]addr: 0x%x=0x%02x; ",  ui4TmpAddr+i, u4data);			  
			  
			//if((i+1)%4 ==0) Printf("\n");	
		 
		  HAL_Delay_us(10000);
		}	
	
		vIO32WriteFldAlign(PDWNC_UP_CFG, 0x0, FLD_XDATA_ACC);//switch xdata to 8032    
		vIO32WriteFldAlign(PDWNC_T8032_INTEN, 0x1, FLD_ARM_INTEN);//Enable int
		vIO32WriteFldAlign(PDWNC_ARM_INT, 0x1, FLD_ARM_INT);//trigger int	
		HAL_Delay_us(100000);
		IO_WRITE32(PDWNC_T8032_INTEN,0,u4IntStatus);//restore all interrupt
	   
	    Printf("Finished clean t8032 Xdata\n");
		
		return 0;		
	
	}


static INT32 _CLI_XdataArmWrite(INT32 i4Argc, const CHAR ** szArgv)
{
#ifdef CC_MT5387 
	UINT32 u48032XdataStartAddr =0x4000;
#else
	UINT32 u48032XdataStartAddr =0x8000;
#endif
    UINT32 i;
//	UINT8 ui1Data=0xff;
	UINT32 u4data=0xFF;
	UINT32 u4IntStatus=0;
	UINT32 ui4StartAddr=0;
	UINT32 ui4Length=0;
	UINT32 ui4TmpAddr=0;
//	UINT8 ui1ChkSumData=0xff;
//	BOOL fgChkSumRst=TRUE;

    if(i4Argc < 3)
    {
    	Printf("armwr startaddr(0~2047) length(0~2048)\n");
        return -1024;
    }


	if(ui4StartAddr >2047 ||ui4Length >2048 )
	{
		Printf("too big range\n");
	}

    ui4StartAddr = StrToInt(szArgv[1]);
    ui4Length = StrToInt(szArgv[2]);
	
	u4IntStatus = (UINT32)IO_READ32(PDWNC_T8032_INTEN,0);
	IO_WRITE32(PDWNC_T8032_INTEN,0,0);//clear all interrupt

    vIO32WriteFldAlign(PDWNC_UP_CFG, 0x1, FLD_XDATA_ACC);//switch xdata to ARM    
    vIO32WriteFldAlign(PDWNC_UP_ADDR, u48032XdataStartAddr+ui4StartAddr , FLD_UP_ADDR);

	ui4TmpAddr=u48032XdataStartAddr+ui4StartAddr ;
	Printf("Do write\n");

    for(i = 0; i < ui4Length; i++)
    {
		 IO_WRITE32(PDWNC_UP_DATA, 0,u4data);

		 Printf("[W]addr: 0x%x=0x%02x; ",  ui4TmpAddr+i, u4data);
		 (u4data ==0)?(u4data=0xff):(u4data--);	
		  
		if((i+1)%4 ==0) Printf("\n");	
	 
	  HAL_Delay_us(10000);
    }   

    vIO32WriteFldAlign(PDWNC_UP_CFG, 0x0, FLD_XDATA_ACC);//switch xdata to 8032    
	vIO32WriteFldAlign(PDWNC_T8032_INTEN, 0x1, FLD_ARM_INTEN);//Enable int
	vIO32WriteFldAlign(PDWNC_ARM_INT, 0x1, FLD_ARM_INT);//trigger int	
    HAL_Delay_us(100000);
	IO_WRITE32(PDWNC_T8032_INTEN,0,u4IntStatus);//restore all interrupt
   
	return 0;

}


static INT32 _CLI_XdataDump(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32  u4XdataLeh= 2048; 
#ifdef CC_MT5387  
	UINT32 u48032XdataStartAddr =0x4000;
#else
	UINT32 u48032XdataStartAddr =0x8000;
#endif

    UINT32 i;
    UINT8 ui1data;
	UINT32 u4IntStatus=0;
	
	u4IntStatus = (UINT32)IO_READ32(PDWNC_T8032_INTEN,0);
	IO_WRITE32(PDWNC_T8032_INTEN,0,0);//clear all interrupt

    vIO32WriteFldAlign(PDWNC_UP_CFG, 0x1, FLD_XDATA_ACC);//switch xdata to ARM    
    vIO32WriteFldAlign(PDWNC_UP_ADDR, u48032XdataStartAddr, FLD_UP_ADDR);

    for(i = 0; i < u4XdataLeh; i++)
    {
		ui1data = (UINT8)IO_READ8(PDWNC_UP_DATA, 0);
        Printf("Addr:0x%x=0x%02x;",  u48032XdataStartAddr + i, (ui1data));

		if((i+1)%4 ==0) Printf("\n");
	  HAL_Delay_us(10000);
    }   
 	
    HAL_Delay_us(10000);
    vIO32WriteFldAlign(PDWNC_UP_CFG, 0x0, FLD_XDATA_ACC);//switch xdata to 8032  
    HAL_Delay_us(10000);
	IO_WRITE32(PDWNC_T8032_INTEN,0,u4IntStatus);//restore all interrupt
   
	return 0;

}
#endif
static INT32 _CLI_InstrucdataDump(INT32 i4Argc, const CHAR ** szArgv)
{
    //UINT32 u4T8032InstrucdataStartAddr =0;
    UINT32 i;
	UINT32 u4IntStatus = 0;
	UINT32 u4Size = 0;
	UINT32* pu1Buf = NULL;
	
	volatile UINT32* flash_addrPtr1 = NULL;
	volatile UINT32* pu1Ptr = NULL;
	
	flash_addrPtr1 = (volatile UINT32*)DRVCUST_InitGet(eT8032uPOffset);

#ifdef CC_MT5363 
	u4Size = 0x8000;
#else
   	u4Size = DRVCUST_InitGet(eT8032uPSize);
#endif

	if (i4Argc == 2)
	{
		u4Size = StrToInt(szArgv[1]);			
	}
	
	Printf("#define T8032FW_SIZE %d\n",u4Size);
	
	pu1Buf = x_mem_alloc(u4Size);
	if(NULL == pu1Buf) 
	{
		Printf("Can't allocate data by x_mem_alloc\n");
		return 1;
	}
    pu1Ptr = (volatile UINT32*)pu1Buf;
	
	u4IntStatus = (UINT32)IO_READ32(PDWNC_T8032_INTEN,0);
	IO_WRITE32(PDWNC_T8032_INTEN,0,0);//disable all interrupt
#ifdef CC_MT8223
	IO_WRITE32(PDWNC_BASE,0x140,7);
#endif
//	vIO32WriteFldAlign(PDWNC_UP_CFG, 0x1, FLD_T8032_RST);
//	vIO32WriteFldAlign(PDWNC_UP_CFG, 0x0, FLD_VIR_EN);	
//	vIO32WriteFldAlign(PDWNC_UP_CFG, 0x1, FLD_XDATA_ACC);//switch xdata to ARM
	IO_WRITE32(PDWNC_BASE,0x178,0x79);

    vIO32WriteFldAlign(PDWNC_UP_ADDR, 0, FLD_UP_ADDR);

    for(i = 0; i < u4Size; i+=4)
    {
        *pu1Ptr = u4IO32Read4B(PDWNC_UP_DATA);
		if(i%16 == 0)
			Printf("\n");
		Printf("0x%08x\t",*pu1Ptr);
        pu1Ptr++;  
    }   

	if(x_memcmp((void*)flash_addrPtr1, (void*)pu1Buf, u4Size) != 0)
        {
            Printf("Load T8032 FW fail!!\n");
            //return 1;// keep vir_en = 0 && t8032_rst = 1 for T8032_ALIVE macro in pdwnc_drvif.h
        }
	else
		Printf("Load T8032 FW success\n");

/* 	Printf("const UINT8 au1T8032FW[T8032FW_SIZE]=\n{\n");
   for(i = 0; i < u4Size; i++)
    {
        Printf("0x%08x,", (*pu1Buf));
		pu1Buf ++;
		if((i+1)%16 ==0) Printf("\n");
	  //HAL_Delay_us(10000);
    } 
	Printf("}\n");
	*/

#ifdef CC_MT8223
		IO_WRITE32(PDWNC_BASE,0x140,5);
		BIM_WRITE32(0xe04, 0);//switch uart to t8032
#endif

	vIO32WriteFldAlign(PDWNC_UP_CFG, 0x0, FLD_XDATA_ACC);
    vIO32WriteFldAlign(PDWNC_UP_CFG, 0x1, FLD_VIR_EN);//switch istrunc to 8032  
	IO_WRITE32(PDWNC_T8032_INTEN,0,u4IntStatus);//restore all interrupt
	
    vIO32WriteFldAlign(PDWNC_UP_CFG, 0x0, FLD_T8032_RST);//resume mcu
#ifdef CC_MT8223
		for(i = 0; i < 0xf0000000; i++);	
		BIM_WRITE32(0xe04, 0xe2);// switch uart to arm
	
		Printf("T8032 not run!!!\n");
#endif

	x_mem_free(pu1Buf);

	return 0;

}

static INT32 _CLI_InstrucdataLoad(INT32 i4Argc, const CHAR ** szArgv)
{
    //UINT32 u4T8032InstrucdataStartAddr =0;
    UINT32 i;
	UINT32 u4IntStatus=0;
	UINT32 u4Size;	
    // Setup 8032 uP
	volatile UINT32* flash_addrPtr1 = NULL;
	volatile UINT32* pu1Ptr = NULL;
	volatile UINT32* flash_addr = NULL;
	UINT32* pu1Buf = NULL;
	
	flash_addr = (volatile UINT32*)DRVCUST_InitGet(eT8032uPOffset);
	u4Size = DRVCUST_InitGet(eT8032uPSize);
	flash_addrPtr1=flash_addr;
	
	Printf("#define T8032FW_SIZE 0x%x\n",u4Size);
	
	pu1Buf = x_mem_alloc(u4Size);
	if(NULL == pu1Buf) 
	{
		Printf("Can't allocate data by x_mem_alloc\n");
		return 1;
	}
    pu1Ptr = (volatile UINT32*)pu1Buf;
	u4IntStatus = (UINT32)IO_READ32(PDWNC_T8032_INTEN,0);
	IO_WRITE32(PDWNC_T8032_INTEN,0,0);//disable all interrupt
#ifdef CC_MT8223
	Printf("\n--------------8223>8032------------\n");
	IO_WRITE32(PDWNC_BASE,0x140,7);
#endif
	IO_WRITE32(PDWNC_BASE, 0x178, 0x79);//0xa5);// hold 8032
    vIO32WriteFldAlign(PDWNC_UP_ADDR, 0, FLD_UP_ADDR);
	 for(i = 0; i < u4Size; i+=4)
    {
        vIO32Write4B(PDWNC_UP_DATA, *flash_addrPtr1);
#ifdef SHOW_BIN
		if(i%16 == 0)
		Printf("\n");
		Printf("0x%08x",*flash_addrPtr1);
#endif
        flash_addrPtr1++;
	 }
	vIO32WriteFldAlign(PDWNC_UP_ADDR, 0, FLD_UP_ADDR); //read back to compare	
    for(i = 0; i < u4Size; i+=4)
    {
        *pu1Ptr = u4IO32Read4B(PDWNC_UP_DATA);
#ifdef SHOW_BIN
		if(i%16 == 0)
			Printf("\n");
        Printf("0x%08x,", (*pu1Ptr));
#endif
        pu1Ptr++;  
    }   
	
	if(x_memcmp((void*)flash_addr, (void*)pu1Buf, u4Size) != 0)
        {
            Printf("Load T8032 FW fail!!\n");
            //return 1;// keep vir_en = 0 && t8032_rst = 1 for T8032_ALIVE macro in pdwnc_drvif.h
        }
	else
		Printf("Load T8032 FW success\n");


	IO_WRITE8(PDWNC_BASE, 4, 6); // CLOCK
	IO_WRITE8(PDWNC_BASE, 0x178, 0x7b);//switch 8032 run on srm
	IO_WRITE8(PDWNC_BASE, 0x140, 0x5); //release load code to srm
	IO_WRITE8(PDWNC_BASE, 0, 0x80); // CLOCK

#ifdef CC_MT8223
	Printf("Register status:\n");
		Printf("0x28000 = 0x%8x\n",IO_READ32(PDWNC_BASE,0x00));		
		Printf("0x28004 = 0x%8x\n",IO_READ32(PDWNC_BASE,0x04));
		Printf("0x28140 = 0x%8x\n",IO_READ32(PDWNC_BASE,0x140));
		Printf("0x28178 = 0x%8x\n",IO_READ32(PDWNC_BASE,0x178));
		for(i = 0; i < 0xf000000; i++); //wait a short time
	BIM_WRITE32(0xe04, 0);//switch uart to t8032
#endif
	IO_WRITE32(PDWNC_T8032_INTEN,0,u4IntStatus);//restore all interrupt
	
	IO_WRITE8(PDWNC_BASE, 0x178, 0x7a);//togle
#ifdef CC_MT8223
	for(i = 0; i < 0xf0000000; i++);	
	BIM_WRITE32(0xe04, 0xe2);// switch uart to arm

	Printf("T8032 not run!!!\n");
#endif

	x_mem_free(pu1Buf);

	return 0;

}

/*
static INT32 _CLI_GetData(INT32 i4Argc, const CHAR ** szArgv)
{
	UINT8 i=0;
	UINT8* pu1Buf = x_mem_alloc(40);
        if(NULL == pu1Buf)
        {
                Printf("Can't allocate data by x_mem_alloc\n");
                return 1;
        }

	volatile UINT32* pu1Ptr = (volatile UINT32*)pu1Buf;
	UINT32* pu1PtrDst = (UINT32*)pu1Ptr; 
	UINT32* pu1PtrDstTmp = (UINT32*)pu1Ptr; 
	UINT32 auTbl[10]={0x04030201,
						 0x08070605,
						 0x0c0b0a09,
						 0x000f0e0d,
						 0x14131211,
						 0x18171615,
						 0x1c1b1a19,
						 0x101f1e1d,
						 0x24232221,
						 0x28272625};
	
	vIO32WriteFldAlign(PDWNC_T8032_INTEN, 0x1, FLD_ARM_INTEN);//Enable int
	vIO32WriteFldAlign(PDWNC_ARM_INT, 0x1, FLD_ARM_INT);//trigger int

    HAL_Delay_us(500000);

	Printf("Begin to get data form T8032\n");

	*pu1Ptr = u4IO32Read4B(PDWNC_SHREG0);
	pu1Ptr++;

	*pu1Ptr = u4IO32Read4B(PDWNC_SHREG1);
	pu1Ptr++;

	*pu1Ptr = u4IO32Read4B(PDWNC_SHREG2);
	pu1Ptr++;

	*pu1Ptr = u4IO32Read4B(PDWNC_SHREG3);
	pu1Ptr++;

	*pu1Ptr = u4IO32Read4B(PDWNC_SHREG4);
	pu1Ptr++;

	*pu1Ptr = u4IO32Read4B(PDWNC_SHREG5);
	pu1Ptr++;

	*pu1Ptr = u4IO32Read4B(PDWNC_SHREG6);
	pu1Ptr++;

	*pu1Ptr = u4IO32Read4B(PDWNC_SHREG7);
	pu1Ptr++;

	*pu1Ptr = u4IO32Read4B(PDWNC_SHREG8);
	pu1Ptr++;
	
	*pu1Ptr = u4IO32Read4B(PDWNC_SHREG9);
#if 0
	Printf("\n Muc set value\n");
	Printf("Data 0 0x%8x\n",auTbl[0]);
	Printf("Data 1 0x%8x\n",auTbl[1]);
	Printf("Data 2 0x%8x\n",auTbl[2]);
	Printf("Data 3 0x%8x\n",auTbl[3]);
	Printf("Data 4 0x%8x\n",auTbl[4]);
	Printf("Data 5 0x%8x\n",auTbl[5]);
	Printf("Data 6 0x%8x\n",auTbl[6]);
	Printf("Data 7 0x%8x\n",auTbl[7]);
	Printf("Data 8 0x%8x\n",auTbl[8]);
	Printf("Data 9 0x%8x\n",auTbl[9]);
#endif
	Printf("\nArm get value\nBYTE format:\n");
	while(i < 40)
	{
		Printf("share[%d]: 0x%2x,",i/4,*pu1Buf);
		if((i+1)%4 == 0)Printf("\n");
		i++;
		pu1Buf++;
	}

	Printf("UINT32 format:\n");
	i = 0;
	while(i < 10)
	{
		Printf("share[%d]: 0x%8x\n",i ,*pu1PtrDst);

 		i++;
		pu1PtrDst++;
	}

	i = 0;
	while(i < 10)
	{
		if(*pu1PtrDstTmp != auTbl[i])
		{
			break;
		}
		pu1PtrDstTmp++;
		i++;
	}

	if(10 <= i)
	{
		Printf("\nCompare OK\n");
	}
	else
	{
		Printf("\nCompare NG @pos %d/Tol 10 \n",i);
	}
	return 0;
}


static INT32 _CLI_SendData(INT32 i4Argc, const CHAR ** szArgv)
{
	UINT8 ui1CmdIdx=0xff;

	if( 2 == i4Argc )
	{
		ui1CmdIdx = StrToInt(szArgv[1]);
	}

	vIO32WriteFldAlign(CKGEN_PMUX2,0x1,Fld(2, 0, AC_MSKB0));//set to uart
	switch(ui1CmdIdx)
	{
		case 0:
			vIO32Write4B(PDWNC_SHREG0,0x04030201);
			Printf("Data 0 0x%x\n",0x04030201);
			break;

		case 1:
			vIO32Write4B(PDWNC_SHREG1,0x08070605);
			Printf("Data 1 0x%x\n",0x08070605);
			break;

		case 2:
			vIO32Write4B(PDWNC_SHREG2,0x0c0b0a09);
			Printf("Data 2 0x%x\n",0x0c0b0a09);
			break;
		
		case 3:
			vIO32Write4B(PDWNC_SHREG3,0x000f0e0d);
			Printf("Data 3 0x%x\n",0x000f0e0d);
			break;

		case 4:
			vIO32Write4B(PDWNC_SHREG4,0x14131211);
			Printf("Data 4 0x%x\n",0x14131211);
			break;
		
		case 5:
			vIO32Write4B(PDWNC_SHREG5,0x18171615);
			Printf("Data 5 0x%x\n",0x18171615);
			break;
		
		case 6:
			vIO32Write4B(PDWNC_SHREG6,0x1c1b1a19);
			Printf("Data 6 0x%x\n",0x1c1b1a19);
			break;
		
		case 7:
			vIO32Write4B(PDWNC_SHREG7,0x101f1e1d);
			Printf("Data 7 0x%x\n",0x101f1e1d);
			break;

		case 8:
			vIO32Write4B(PDWNC_SHREG8,0x24232221);
			Printf("Data 8 0x%x\n",0x24232221);
			break;
		
		case 9:
			vIO32Write4B(PDWNC_SHREG9,0x28272625);
			Printf("Data 9 0x%x\n",0x28272625);
			break;
		
		case 255:
			vIO32Write4B(PDWNC_SHREG0,0x04030201);
			vIO32Write4B(PDWNC_SHREG1,0x08070605);
			vIO32Write4B(PDWNC_SHREG2,0x0c0b0a09);
			vIO32Write4B(PDWNC_SHREG3,0x000f0e0d);
			vIO32Write4B(PDWNC_SHREG4,0x14131211);
			vIO32Write4B(PDWNC_SHREG5,0x18171615);
			vIO32Write4B(PDWNC_SHREG6,0x1c1b1a19);
			vIO32Write4B(PDWNC_SHREG7,0x101f1e1d);
			vIO32Write4B(PDWNC_SHREG8,0x24232221);
			vIO32Write4B(PDWNC_SHREG9,0x28272625);

			Printf("Data 0 0x%8x\n",0x04030201);
			Printf("Data 1 0x%8x\n",0x08070605);
			Printf("Data 2 0x%8x\n",0x0c0b0a09);
			Printf("Data 3 0x%8x\n",0x000f0e0d);
			Printf("Data 4 0x%8x\n",0x14131211);
			Printf("Data 5 0x%8x\n",0x18171615);
			Printf("Data 6 0x%8x\n",0x1c1b1a19);
			Printf("Data 7 0x%8x\n",0x101f1e1d);
			Printf("Data 8 0x%8x\n",0x24232221);
			Printf("Data 9 0x%8x\n",0x28272625);

			break;

		default :
		break;
	}
		
	vIO32WriteFldAlign(PDWNC_T8032_INTEN, 0x1, FLD_ARM_INTEN);//Enable int
	vIO32WriteFldAlign(PDWNC_ARM_INT, 0x1, FLD_ARM_INT);//send info to mcu
	
    HAL_Delay_us(100000);
	vIO32WriteFldAlign(CKGEN_PMUX2,0x0,Fld(2, 0, AC_MSKB0));//set to gpio 

	return 0;
}
*/
static INT32 _CLI_PowerDown_VgaWakeup(INT32 i4Argc, const CHAR ** szArgv)
{    
    UINT32 u4Size = 0;
	if (i4Argc == 2)
	{
		u4Size = StrToInt(szArgv[1]);			
		if(u4Size)
                    PDWNC_EnterPowerDown(WAK_VGA, (UINT32)PDWNC_POWER_DOWN_UNKNOWN);		
		else
		    _PdwncSetupVGA(WAK_VGA);
	}
      else
        {
        	    _PdwncSetupVGA(WAK_VGA); 
        }  
      if(u4Size)
        Printf("Test VGA Wakeup...\n");
      else
        Printf("Set PDWNC registers as VGA Wakeup...\n");
    return 0;
}
static INT32 _CLI_PowerDown_UartWakeup(INT32 i4Argc, const CHAR ** szArgv)
{    
    PDWNC_EnterPowerDown(WAK_UNOR, (UINT32)PDWNC_POWER_DOWN_UNKNOWN);
    return 0;    
}
static INT32 _CLI_PowerDown_HdmiWakeup(INT32 i4Argc, const CHAR ** szArgv)
{    
    PDWNC_EnterPowerDown(WAK_HDMI, (UINT32)PDWNC_POWER_DOWN_UNKNOWN);
    return 0;    
}
static INT32 _CLI_PowerDown_T8032Wakeup(INT32 i4Argc, const CHAR ** szArgv)
{    
    PDWNC_EnterPowerDown(WAK_T8032, (UINT32)PDWNC_POWER_DOWN_UNKNOWN);
    return 0;    
}
static INT32 _CLI_PowerDown_MaskWakeup(INT32 i4Argc, const CHAR ** szArgv)
{    
	UINT32 u4Size = 0;
	if (i4Argc == 2)
	{
		u4Size = StrToInt(szArgv[1]);			
        PDWNC_EnterPowerDown(u4Size, (UINT32)PDWNC_POWER_DOWN_UNKNOWN);		
	}
	else
    {
        Printf("downmask 'data for wtirte to 0xf0028120'\n");
        return -1024;
    }    
    return 0;    
}
/*
static INT32 _CLI_PowerDown_CecWakeup(INT32 i4Argc, const CHAR ** szArgv)
{    
    PDWNC_EnterPowerDown(WAK_CEC, (UINT32)PDWNC_POWER_DOWN_UNKNOWN);
    return 0;    
}
*/
static INT32 _CLI_Init(INT32 i4Argc, const CHAR ** szArgv)
{
    return PDWNC_Init();
}


static INT32 _CLI_ReadServo(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Val;
    UINT32 u4ServoId;
	UINT32 u4loop = 1;
    
    if(i4Argc < 2)
    {
        return -1024;
    }
     
    if(i4Argc == 2)
    { 
	    u4ServoId = StrToInt(szArgv[1]);
	    u4Val = IRRX_ReadSvadc(u4ServoId);
	    Printf("Servo %d's value: %d hex: 0x%x\n", u4ServoId, u4Val, u4Val);
    }

    if(i4Argc == 3)
    { 
	    u4ServoId = StrToInt(szArgv[1]);
		u4loop =(UINT32)StrToInt(szArgv[2]);
		
		while(0 != u4loop)
		{
			u4Val = PDWNC_ReadServoADCChannelValue(u4ServoId);
		    Printf("Loop %d,Servo %d's value: %d\n",u4loop, u4ServoId, u4Val);
			u4loop--;
		}
    }
	
    return 0;
}

#if defined(CC_MT5365) || defined(CC_MT5395) 
#define EDID_TEST_SIZE 256  
#if 0
static INT32 _CLI_TestVGA_EDID(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 i;
    UINT32 u4FailFlag = 0;
    volatile UINT32* pu4Ptr;
    
    vIO32WriteFldAlign(PDWNC_UP_CFG, 0x1, FLD_XDATA_ACC);//switch xdata to ARM
//step1
    Printf("Step1...\n");
    i = 0;
    vIO32Write4B(PDWNC_VGA_ADDR, 0);
    for(i = 0; i < EDID_TEST_SIZE; i+=4)
    {
        vIO32Write4B(PDWNC_VGA_DATA, 0x55aa55aa);
    }    
    Printf("Step2...\n");    
//step2    
    i = 0; 
    u4FailFlag = 0;
    while(i < EDID_TEST_SIZE)
    {
        vIO32Write4B(PDWNC_VGA_ADDR, i >> 2);
        *pu4Ptr = u4IO32Read4B(PDWNC_VGA_DATA);
        if((*pu4Ptr) != 0x55aa55aa)
        {
            Printf("memory compare fail at address offset %d (write: 0x55aa55aa, read: 0x%2x)\n", i, (*pu4Ptr));
            u4FailFlag = 1;
        } 
        vIO32Write4B(PDWNC_VGA_ADDR, i >> 2);
        vIO32Write4B(PDWNC_VGA_DATA, 0xaa55aa55);

        i+=4;  
    } 
    if(u4FailFlag)
    {
        Printf("Memory compare fail...\n");
        return 0;
    }
    Printf("Step3...\n");    
//step3
    i = 0;
    u4FailFlag = 0;
    while(i < EDID_TEST_SIZE)
    {
        vIO32Write4B(PDWNC_VGA_ADDR, i >> 2);
        *pu4Ptr = u4IO32Read4B(PDWNC_VGA_DATA);
        if((*pu4Ptr) != 0xaa55aa55)
        {
            Printf("memory compare fail at address offset %d (write: 0xaa55aa55, read: 0x%2x)\n", i, (*pu4Ptr));
            u4FailFlag = 1;
        } 
        vIO32Write4B(PDWNC_VGA_ADDR, i >> 2);
        vIO32Write4B(PDWNC_VGA_DATA, 0x55aa55aa);

        i+=4;  
    } 
    if(u4FailFlag)
    {
        Printf("Memory compare fail...\n");
        return 0;
    }
    
    Printf("Step4...\n");    
//step4    
    i = 0; 
    u4FailFlag = 0;
    while(i < EDID_TEST_SIZE)
    {
        vIO32Write4B(PDWNC_VGA_ADDR, i >> 2);
        *pu4Ptr = u4IO32Read4B(PDWNC_VGA_DATA);
        if((*pu4Ptr) != 0x55aa55aa)
        {
            Printf("memory compare fail at address offset %d (write: 0x55aa55aa, read: 0x%2x)\n", i, (*pu4Ptr));
            u4FailFlag = 1;
        } 
        vIO32Write4B(PDWNC_VGA_ADDR, i >> 2);
        vIO32Write4B(PDWNC_VGA_DATA, 0xaa55aa55);

        i+=4;  
    } 
    if(u4FailFlag)
    {
        Printf("Memory compare fail...\n");
        return 0;
    }
    Printf("Step5...\n");    
//step5
    i = 0;
    u4FailFlag = 0;
    while(i < EDID_TEST_SIZE)
    {
        vIO32Write4B(PDWNC_VGA_ADDR, i >> 2);
        *pu4Ptr = u4IO32Read4B(PDWNC_VGA_DATA);
        if((*pu4Ptr) != 0xaa55aa55)
        {
            Printf("memory compare fail at address offset %d (write: 0xaa55aa55, read: 0x%2x)\n", i, (*pu4Ptr));
            u4FailFlag = 1;
        } 
        vIO32Write4B(PDWNC_VGA_ADDR, i >> 2);
        vIO32Write4B(PDWNC_VGA_DATA, 0x55aa55aa);

        i+=4;  
    } 
    if(u4FailFlag)
    {
        Printf("Memory compare fail...\n");
        return 0;
    }

    Printf("Step6...\n");    
//step6    
    i = 0;
    u4FailFlag = 0;    
    vIO32Write4B(PDWNC_VGA_ADDR, 0);
    while(i < EDID_TEST_SIZE)
    {
        *pu4Ptr = u4IO32Read4B(PDWNC_VGA_DATA);
        if((*pu4Ptr) != 0x55aa55aa)
        {
            Printf("memory compare fail at address offset %d (write: 0x55aa55aa, read: 0x%2x)\n", i, (*pu4Ptr));
            u4FailFlag = 1;
        } 
        i+=4;  
    } 
    if(u4FailFlag)
    {
        Printf("Memory compare fail...\n");
    }
    Printf("March algorithm pass!\n");  
    return 0;        
}
#endif
static INT32 _CLI_UpdateVGA_EDID(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Res = 0;
    UINT32 u4tmp, i, j;
    Printf("Write 0xff ~ 0x00 to VGA sram offset address 0x00~0xff..\n");
    vIO32WriteFldAlign(PDWNC_VGA_ADDR, 0, FLD_VGA_ADDR);
    i = 0xff;
    for(j = 0; j < 256; j+=4)
    {
        u4tmp = i;
        u4tmp <<= 8;
        
        i--;
        u4tmp |= i;
        u4tmp <<= 8;
        
        i--;
        u4tmp |= i;
        u4tmp <<= 8;
        
        i--;
        u4tmp |= i;

        vIO32Write4B(PDWNC_VGA_DATA, u4tmp);

        i--;
    }

    Printf("Read back to check...compare\n");
    vIO32WriteFldAlign(PDWNC_VGA_ADDR, 0, FLD_VGA_ADDR);
    i = 0xff;
    for(j = 0; j < 256; j+=4)
    {
        u4tmp = u4IO32Read4B(PDWNC_VGA_DATA);
        Printf("Addr %d: 0x%x\n", j, u4tmp);
        if((u4tmp >> 24) != i)
        {
            u4Res = 1;
            break;
        }
        i--;
        if(((u4tmp >> 16) & 0xff) != i)
        {
            u4Res = 1;
            break;
        }
        i--;
        if(((u4tmp >> 8) & 0xff) != i)
        {
            u4Res = 1;
            break;
        }
        i--;
        if((u4tmp & 0xff) != i)
        {
            u4Res = 1;
            break;
        }
        i--;
    }
    

    if(!u4Res)
    {
        Printf("OK!\n");
    }
    else
    {
        Printf("Fail at offset address%d\n", 0xff-i);
    }
    return 0;
}
#endif

UINT32 fw_code_table[0x6000/4];
static INT32 _CLI_PrintUploadT8032FW(INT32 i4Argc, const CHAR **szArgv)
{
		Printf("Buffer address for upload up Firmware is : 0x%x\n", fw_code_table);
		Printf("(Trace32==>d.load.binary d:\\test.bin 0x%x \n)", fw_code_table);
		Printf("Usage: pdwnc.fw 0x%x \n", fw_code_table);
            return 0;
}
static INT32 _CLI_UploadT8032FW(INT32 i4Argc, const CHAR **szArgv)
{
#if 1     
	UINT32 i;
	UINT32 data;
    //volatile UINT8 u1data;

	UINT32 u4InstrucMemSize = 0x6000;
	volatile UINT32 *p = (volatile UINT32 *)(fw_code_table);//(0x32000000);
	printf("up load T8032 FW start!\n");

	if (i4Argc == 3)
	{
		p=(volatile UINT32 *)StrToInt(szArgv[1]);
		u4InstrucMemSize=StrToInt(szArgv[2]);
	}

	Printf("src add 0x%x,size %d\n",p,u4InstrucMemSize);
	
	vIO32WriteFldAlign(PDWNC_UP_CFG, 0x0, FLD_VIR_EN);
	vIO32WriteFldAlign(PDWNC_UP_CFG, 0x1, FLD_T8032_RST);	
	vIO32WriteFldAlign(PDWNC_UP_CFG, 0x1, FLD_XDATA_ACC);//switch xdata to ARM

    vIO32WriteFldAlign(PDWNC_UP_ADDR, 0, FLD_UP_ADDR);
	
    for(i=0;i<u4InstrucMemSize;i+=4)
    {
        vIO32Write4B(PDWNC_UP_DATA, *p);
        p++;
    }	

    p = (volatile UINT32 *)(fw_code_table);//(0x32000000) ;
    
	if (i4Argc == 3)
	{
		p=(volatile UINT32 *)StrToInt(szArgv[1]);
		u4InstrucMemSize=StrToInt(szArgv[2]);
	}      
	
	Printf("Current CFG 0x%x\n",IO_READ32(PDWNC_UP_CFG,0));

    vIO32WriteFldAlign(PDWNC_UP_ADDR, 0, FLD_UP_ADDR);

    for(i = 0; i < u4InstrucMemSize; i+=4)
    {
        data = u4IO32Read4B(PDWNC_UP_DATA);
		if(data != (*p))
		{
			Printf("load fail : addr %x\n", p);
			break;
		}
		p++;
    }  

	Printf("Load T8032 over!\n");


	//Set UART transparent mode
	//UART_WRITE32(REG_SER_CTL, SET_TRANS_MODE_ON);
	vIO32Write4B(PDWNC_STAB, 0xe2);//IO_WRITE32(0x20028804, 0,0xe2);
	Printf("Set UART PD transparent mode OK!\n");
/*
#ifdef CC_MT5365
//#if 1
 	u4Tmp=IO_READ32(0x2000D400, 0);
	Printf("MT5365 original pmux0 : 0x%x\n",u4Tmp);
	u4Tmp &= 0xfffffcff;
	u4Tmp |= 0x01;
	IO_WRITE32(0x2000D400, 0, u4Tmp);
MT5365	
#elif defined(CC_MT5387)
 	u4Tmp=IO_READ32(0x2000D408, 0);
	Printf("MT5387 original pmux2 : 0x%x\n",u4Tmp);
	u4Tmp &= 0xfffffff8;
	u4Tmp |= 0x01;
	IO_WRITE32(0x2000D408, 0, u4Tmp);
MT5387	
#elif defined(CC_MT5363)
 	u4Tmp=IO_READ32(0x2000D400, 0);
	Printf("MT5363 original pmux2 : 0x%x\n",u4Tmp);
	u4Tmp &= 0xff3fffff;
	IO_WRITE32(0x2000D400, 0, u4Tmp);
MT5363
#endif
*/
#if 0
	IO_WRITE32(0x20028818, 0,0x4);
	Printf("Set UART1 PD pinmux OK!\n");
 #endif
	//reset T8032  
 	//u4Tmp=IO_READ32(0x20028838, 0);
// 	u4Tmp |= 0xc0;
//	IO_WRITE32(0x20028838, 0, u4Tmp); 	

#if defined(CC_MT5365)
    vIO32Write4B(PDWNC_PINMUX, (u4IO32Read4B(PDWNC_PINMUX) & 0xff8fffff) | 0x00200000);
    Printf("Set UART1 PD pinmux OK!\n");
#elif defined(CC_MT5395)

#elif defined(CC_MT5368)
vIO32Write4B(PDWNC_PINMUX0, (u4IO32Read4B(PDWNC_PINMUX0) & 0xffff8fff) | 0x00003000);
Printf("Set VGA_SCL/SDA as UART PD function!\n");
#endif


        vIO32WriteFldAlign(PDWNC_BCR, 0x1, FLD_CLEAR_TBUF);
        vIO32WriteFldAlign(PDWNC_BCR, 0x1, FLD_CLEAR_RBUF);
	Printf("Clr UART1 buffer OK!\n");	

        vIO32Write4B(PDWNC_INTCLR, 0xffffffff);	//IO_WRITE32(0x20028048, 0, 0xffffffff); 			
	Printf("Clr all pdwnc interrupt OK!\n");		


	vIO32WriteFldAlign(PDWNC_UP_CFG, 0x0, FLD_XDATA_ACC);//switch xdata to mcu
 	vIO32WriteFldAlign(PDWNC_UP_CFG, 0x1, FLD_VIR_EN);
	Printf("Reset T8032....\n");        	
	vIO32WriteFldAlign(PDWNC_UP_CFG, 0x0, FLD_T8032_RST);	        
	return 0;
    #else
            UINT32 i;
            UINT32 data, u4Tmp;
        //volatile UINT8 u1data;
     
            UINT32 u4InstrucMemSize = 0x6000;
            volatile UINT32 *p = (volatile UINT32 *)(fw_code_table);//(0x32000000);
            printf("up load T8032 FW start!\n");
     
            if (i4Argc == 3)
            {
                    p=(volatile UINT32 *)StrToInt(szArgv[1]);
                    u4InstrucMemSize=StrToInt(szArgv[2]);
            }
     
            Printf("src add 0x%x,size %d\n",p,u4InstrucMemSize);
     
            vIO32WriteFldAlign(PDWNC_UP_CFG, 0x0, FLD_VIR_EN);
            vIO32WriteFldAlign(PDWNC_UP_CFG, 0x1, FLD_T8032_RST);
            vIO32WriteFldAlign(PDWNC_UP_CFG, 0x1, FLD_XDATA_ACC);//switch xdata to ARM
     
        vIO32WriteFldAlign(PDWNC_UP_ADDR, 0, FLD_UP_ADDR);
     
        for(i=0;i<u4InstrucMemSize;i+=4)
        {
            vIO32Write4B(PDWNC_UP_DATA, *p);
            p++;
        }
     
        p = (volatile UINT32 *)(fw_code_table);//(0x32000000) ;
     
            if (i4Argc == 3)
            {
                    p=(volatile UINT32 *)StrToInt(szArgv[1]);
                    u4InstrucMemSize=StrToInt(szArgv[2]);
            }
     
            Printf("Current CFG 0x%x\n",IO_READ32(PDWNC_UP_CFG,0));
     
        vIO32WriteFldAlign(PDWNC_UP_ADDR, 0, FLD_UP_ADDR);
     
        for(i = 0; i < u4InstrucMemSize; i+=4)
        {
            data = u4IO32Read4B(PDWNC_UP_DATA);
                    if(data != (*p))
                    {
                            Printf("load fail : addr %x\n", p);
                            break;
                    }
                    p++;
        }
     
            Printf("Load T8032 over!\n");
     
     
            //Set UART transparent mode
            //UART_WRITE32(REG_SER_CTL, SET_TRANS_MODE_ON);
            IO_WRITE32(0xf0028804, 0,0xe2);
            Printf("Set UART PD transparent mode OK!\n");
    /*
#ifdef CC_MT5365
    //#if 1
            u4Tmp=IO_READ32(0x2000D400, 0);
            Printf("MT5365 original pmux0 : 0x%x\n",u4Tmp);
            u4Tmp &= 0xfffffcff;
            u4Tmp |= 0x01;
            IO_WRITE32(0x2000D400, 0, u4Tmp);
    MT5365
#elif defined(CC_MT5387)
            u4Tmp=IO_READ32(0x2000D408, 0);
            Printf("MT5387 original pmux2 : 0x%x\n",u4Tmp);
            u4Tmp &= 0xfffffff8;
            u4Tmp |= 0x01;
            IO_WRITE32(0x2000D408, 0, u4Tmp);
    MT5387
#elif defined(CC_MT5363)
            u4Tmp=IO_READ32(0x2000D400, 0);
            Printf("MT5363 original pmux2 : 0x%x\n",u4Tmp);
            u4Tmp &= 0xff3fffff;
            IO_WRITE32(0x2000D400, 0, u4Tmp);
    MT5363
#endif
    */
            IO_WRITE32(0xf0028818, 0,0x4);
            u4Tmp=IO_READ32(0xf00280B4, 0);
            u4Tmp &= 0xff8fffff;
            u4Tmp |= 0x00200000;
            IO_WRITE32(0xf00280B4, 0, u4Tmp);
            Printf("Set UART1 PD pinmux OK!\n");
     
            //reset T8032
            u4Tmp=IO_READ32(0xf0028838, 0);
            u4Tmp |= 0xc0;
            IO_WRITE32(0xf0028838, 0, u4Tmp);
            Printf("Clr UART1 buffer OK!\n");
     
            IO_WRITE32(0xf0028048, 0, 0xffffffff);
            Printf("Clr all pdwnc interrupt OK!\n");
     
            Printf("Reset T8032....\n");
            vIO32WriteFldAlign(PDWNC_UP_CFG, 0x0, FLD_XDATA_ACC);//switch xdata to mcu
            vIO32WriteFldAlign(PDWNC_UP_CFG, 0x0, FLD_T8032_RST);
            vIO32WriteFldAlign(PDWNC_UP_CFG, 0x1, FLD_VIR_EN);
     
            return 0;

    #endif
}
static INT32 _CLI_SwitchUartPD_PORT0(INT32 i4Argc, const CHAR **szArgv)
{
	Printf("Switch UART PD to use UART PORT0!\n");
    
//check uart_pd do not be set to other port
    
#if defined(CC_MT5365)
	if(((u1IO32Read1B(PDWNC_PINMUX+2) & 0x70) == 0x20))//vga_sda/vga_scl
		{
		vIO32Write1B((PDWNC_PINMUX+2), (u1IO32Read1B(PDWNC_PINMUX+2) & 0x8f));
		}
    if(((u1IO32Read1B(CKGEN_PMUX4+1) & 0x10) == 0x10))//U1RX/U1TX (only exist in 5366)
		{
		vIO32Write1B((CKGEN_PMUX4+1), (u1IO32Read1B(CKGEN_PMUX4+1) & 0xef));
		}
    if(((u1IO32Read1B(CKGEN_PMUX1+2) & 0x03) == 0x02))//DEMOD_TSCLK
		{
		vIO32Write1B((CKGEN_PMUX1+2), (u1IO32Read1B(CKGEN_PMUX1+2) & 0xfc));
		}
    if(((u1IO32Read1B(CKGEN_PMUX3+3) & 0x30) == 0x20))//DEMOD_TSVAL
		{
		vIO32Write1B((CKGEN_PMUX3+3), (u1IO32Read1B(CKGEN_PMUX3+3) & 0xcf));
		}
	vIO32Write1B(PDWNC_PINMUX+1, ((u1IO32Read1B(PDWNC_PINMUX+1) & 0xcf)|0x10));//U0RX/U0TX   //UART0 pad as T8032 log 
#elif defined(CC_MT5395)
	if(((u1IO32Read1B(CKGEN_PMUX0) & 0x20) == 0x20))//U1RX/U1TX
		{
		vIO32Write1B(CKGEN_PMUX0, (u1IO32Read1B(CKGEN_PMUX0) & 0xdf));
		}
    if(((u1IO32Read1B(PDWNC_PINMUX+1) & 0x0c) == 0x08))//vga_sda/vga_scl
		{
		vIO32Write1B((PDWNC_PINMUX+1), (u1IO32Read1B(PDWNC_PINMUX+1) & 0xf3));
		}
	vIO32Write1B(PDWNC_PINMUX+1, ((u1IO32Read1B(PDWNC_PINMUX+1) & 0xcf)|0x10));//U0RX/U0TX  //UART0 pad as T8032 log 
#elif defined(CC_MT5363)
	if(((u1IO32Read1B(CKGEN_PMUX0+2) & 0xc0) == 0x00))//U1RX/U1TX 
		{
		vIO32Write1B((CKGEN_PMUX0+2), ((u1IO32Read1B(CKGEN_PMUX0+2) & 0x3f)|0x40));
		}
	vIO32Write1B(PDWNC_PINMUX+2, ((u1IO32Read1B(PDWNC_PINMUX+2) & 0xf8)|0x01));//U0RX/U0TX  //UART0 pad as T8032 log 
#elif defined(CC_MT5387)
	if(((u1IO32Read1B(CKGEN_PMUX0+2) & 0xc0) == 0x40))//U1RX/U1TX 
		{
		vIO32Write1B((CKGEN_PMUX0+2), (u1IO32Read1B(CKGEN_PMUX0+2) & 0x3f));
		}
	vIO32Write1B(PDWNC_PINMUX+2, ((u1IO32Read1B(PDWNC_PINMUX+2) & 0xf8)|0x01));//U0RX/U0TX	//UART0 pad as T8032 log 
#elif defined(CC_MT5389)
	if(((u4IO32Read4B(PDWNC_PINMUX0) & 0x7000) == 0x3000))//vga_sda/vga_scl
		{
		vIO32Write4B((PDWNC_PINMUX0), (u4IO32Read4B(PDWNC_PINMUX0) & 0xffff8fff));
		}
	vIO32Write4B(PDWNC_PINMUX0, ((u4IO32Read4B(PDWNC_PINMUX0) & 0xfffffcff)|0x100));//U0RX/U0TX //UART0 pad as T8032 log	 
#elif defined(CC_MT5368)
	if(((u4IO32Read4B(CKGEN_PMUX2) & 0x60000) == 0x20000))//U1RX/U1TX
		{
		vIO32Write4B(CKGEN_PMUX2, (u4IO32Read4B(CKGEN_PMUX2) & 0xfff9ffff));
		}
	if(((u4IO32Read4B(PDWNC_PINMUX0) & 0x7000) == 0x3000))//vga_sda/vga_scl
		{
		vIO32Write4B((PDWNC_PINMUX0), (u4IO32Read4B(PDWNC_PINMUX0) & 0xffff8fff));
		}
	vIO32Write4B(PDWNC_PINMUX0, ((u4IO32Read4B(PDWNC_PINMUX0) & 0xfffcffff)|0x10000));//U0RX/U0TX	//UART0 pad as T8032 log	 
#elif defined(CC_MT5396)
		if(((u4IO32Read4B(CKGEN_PMUX0) & 0xc000) == 0x4000))//U1RX/U1TX
			{
			vIO32Write4B(CKGEN_PMUX0, (u4IO32Read4B(CKGEN_PMUX0) & 0xffff3fff));
			}		
		if(((u4IO32Read4B(CKGEN_PMUX0) & 0x7) == 0x2))//GPIO45/46
			{
			vIO32Write4B(CKGEN_PMUX0, (u4IO32Read4B(CKGEN_PMUX0) & 0xfffffff8));
			}
		if(((u4IO32Read4B(PDWNC_PINMUX0) & 0x7000) == 0x3000))//vga_sda/vga_scl
			{
			vIO32Write4B((PDWNC_PINMUX0), (u4IO32Read4B(PDWNC_PINMUX0) & 0xffff8fff));
			}
		vIO32Write4B(PDWNC_PINMUX0, ((u4IO32Read4B(PDWNC_PINMUX0) & 0xfffcffff)|0x10000));//U0RX/U0TX	//UART0 pad as T8032 log	 
#endif

	return 0;
}


static INT32 _CLI_SetT8032(INT32 i4Argc, const CHAR ** szArgv)
{  
    UINT32 u4State;
    if(i4Argc < 1)
    {
        return -1024;
    }
    u4State = StrToInt(szArgv[1]);
    
    return _PDWNC_SetT8032(u4State);
        
}

extern HANDLE_T _hT8032CmdMutex;
//extern void *malloc(size_t size);
//extern void free(void *ptr);
#define MEM_TEST_SIZE 2048
static INT32 _CLI_MemRead(INT32 i4Argc, const CHAR ** szArgv)
{

    UINT32 u4SrcAddr;
    UINT32 u4Len,i ;
    UINT32 u4Idx, u4Val0, u4Val1, u4Val2, u4Val3;
    volatile UINT8* pu1Data;

    if(i4Argc != 3)
    {
        Printf("Need address & size infomation!\n");
        Printf("Ex: pdwnc.r 0x%4x 16\n",XDATA_ADDR);
        return 0;
    }
    
    u4SrcAddr = StrToInt(szArgv[1]);
    u4Len = StrToInt(szArgv[2]);
    if((u4SrcAddr < XDATA_ADDR) || (u4SrcAddr > (XDATA_ADDR + 2048)))
    {
        Printf("address range error!\n");
        Printf("Address: 0x%4x ~ 0x%4x\n",XDATA_ADDR, (XDATA_ADDR + 2048));
        return 0;
    }
    if((u4Len + u4SrcAddr) > (XDATA_ADDR + 2048))
    {
        u4Len -= ((u4Len + u4SrcAddr) - (XDATA_ADDR + 2048));
    }
    
    if(u4Len == 0)
    {
        Printf("u4Len = 0\n");
        return 0;
    }
    pu1Data = (volatile UINT8*)fw_code_table;

    for(u4Idx = 0; u4Idx < u4Len; u4Idx++)
    {
        pu1Data[u4Idx] = 0;
    }
#ifndef CC_MTK_LOADER
        x_sema_lock(_hT8032CmdMutex, X_SEMA_OPTION_WAIT); //make sure no other thread can interrupt T8032 in this critical time
#endif
#if 1    
        if(_PDWNC_SetT8032(PDWNC_T8032_SET_XDATA_ARM))
        {
            Printf("SET_XDATA_ARM fail !!\n");
            return 0;
        }
#else
        vIO32WriteFldAlign(PDWNC_UP_CFG, 0x1, FLD_XDATA_ACC);//switch xdata to ARM
#endif 
        // Copy CEC data to XData
        if(_CopyFromT8032_XDATA(u4SrcAddr, u4Len, (UINT32)pu1Data))
        {
            Printf("Get Xdata fail !!\n");
            return 0;
        }
        
 #if 1   
        if(_PDWNC_SetT8032(PDWNC_T8032_SET_XDATA_T8032))
        {
            Printf("SET_XDATA_T8032 fail !!\n");
            return 0;
        }
#endif
#ifndef CC_MTK_LOADER
        x_sema_unlock(_hT8032CmdMutex);
#endif
    for(u4Idx = 0; u4Idx < u4Len; u4Idx+=16)
    {
        u4Val0 = *((UINT32*)(pu1Data + u4Idx));
        u4Val1 = *((UINT32*)(pu1Data + u4Idx + 4));
        u4Val2 = *((UINT32*)(pu1Data + u4Idx + 8));
        u4Val3 = *((UINT32*)(pu1Data + u4Idx + 12));
        Printf("0x%08X | %08X %08X %08X %08X\n"
                , (u4SrcAddr + u4Idx)
                , u4Val0
                , u4Val1
                , u4Val2             
                , u4Val3);
    }
#if 1
    for(i = 0, u4Idx = 0; u4Idx < MEM_TEST_SIZE; u4Idx++, i++ )
    {
        if(((UINT8)i) != *(pu1Data + u4Idx))
            Printf("Compare fail at offset : %d (write: 0x%2x, read: 0x%2x)\n", u4Idx, ((UINT8)i), *(pu1Data + u4Idx));
            
    }
#endif    
    return 0;

//lint -e{818}
}



static INT32 _CLI_MemWrite(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 i;
	UINT8* pu1Buf = NULL;	
    volatile UINT8* pu1Ptr = NULL;
	
	pu1Buf = x_mem_alloc(MEM_TEST_SIZE);	
	if(NULL == pu1Buf) 
	{
		Printf("Can't allocate data by x_mem_alloc\n");
		return 1;
	}	
	pu1Ptr = (volatile UINT8*)pu1Buf;
	
    for(i = 0; i <MEM_TEST_SIZE ; i++)
    {
        *pu1Ptr = (UINT8)(i);
		pu1Ptr++;
    }
#ifndef CC_MTK_LOADER
        x_sema_lock(_hT8032CmdMutex, X_SEMA_OPTION_WAIT); //make sure no other thread can interrupt T8032 in this critical time
#endif

#if 1
        if(_PDWNC_SetT8032(PDWNC_T8032_SET_XDATA_ARM))
        {
            Printf("SET_XDATA_ARM fail !!\n");
            return 0;
        }
#else
        vIO32WriteFldAlign(PDWNC_UP_CFG, 0x1, FLD_XDATA_ACC);//switch xdata to ARM
#endif    
        // Copy CEC data to XData
        if(_CopyToT8032_XDATA(0x8000, MEM_TEST_SIZE, (UINT32)pu1Buf))
        {
            Printf("Set Xdata fail !!\n");
            return 0;
        }
#if 1
        if(_PDWNC_SetT8032(PDWNC_T8032_SET_XDATA_T8032))
        {
            Printf("SET_XDATA_T8032 fail !!\n");
            return 0;
        }
#endif
#ifndef CC_MTK_LOADER
        x_sema_unlock(_hT8032CmdMutex);
#endif
	x_mem_free(pu1Buf);

    return 0;
}
#if 0
#define INSTRUCTION_MEM_TEST_SIZE 12*1024//24*1024
UINT32 u4IdataArray[INSTRUCTION_MEM_TEST_SIZE/4];
static INT32 _CLI_InstrSramReadWriteTest(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 i;
    UINT32 u4FailFlag = 0;
    volatile UINT32* pu4Ptr;
    
    vIO32WriteFldAlign(PDWNC_UP_CFG, 0x1, FLD_T8032_RST);//Keep reset t8032
    vIO32WriteFldAlign(PDWNC_UP_CFG, 0x0, FLD_VIR_EN);//switch instruct sram to ARM    
    vIO32WriteFldAlign(PDWNC_UP_CFG, 0x1, FLD_XDATA_ACC);//switch xdata to ARM
    Printf("_CLI_InstrSramReadWriteTest...Size(%d)\n", INSTRUCTION_MEM_TEST_SIZE);
//step1
    Printf("Step1...\n");
    i = 0;
    while(i < INSTRUCTION_MEM_TEST_SIZE/4)
    {
        u4IdataArray[i] = 0x55aa55aa;
        i++;
    }

    pu4Ptr = (volatile UINT32*)u4IdataArray;
    vIO32Write4B(PDWNC_UP_ADDR, 0);
    for(i = 0; i < INSTRUCTION_MEM_TEST_SIZE/4; i++)
    {
        vIO32Write4B(PDWNC_UP_DATA, *pu4Ptr);
        pu4Ptr++;
    }    
    Printf("Step2...\n");    
//step2    
    i = 0; 
    u4FailFlag = 0;
    while(i < INSTRUCTION_MEM_TEST_SIZE)
    {
        vIO32Write4B(PDWNC_UP_ADDR, 0 + i);
        *pu4Ptr = u4IO32Read4B(PDWNC_UP_DATA);
        if((*pu4Ptr) != 0x55aa55aa)
        {
            Printf("memory compare fail at address offset %d (write: 0x55aa55aa, read: 0x%2x)\n", i, (*pu4Ptr));
            u4FailFlag = 1;
        } 
        vIO32Write4B(PDWNC_UP_ADDR, 0 + i);
        vIO32Write4B(PDWNC_UP_DATA, 0xaa55aa55);

        i+=4;  
    } 
    if(u4FailFlag)
    {
        Printf("Memory compare fail...\n");
        return 0;
    }
    Printf("Step3...\n");    
//step3
    i = 0;
    u4FailFlag = 0;
    while(i < INSTRUCTION_MEM_TEST_SIZE)
    {
        vIO32Write4B(PDWNC_UP_ADDR, 0 + i);
        *pu4Ptr = u4IO32Read4B(PDWNC_UP_DATA);
        if((*pu4Ptr) != 0xaa55aa55)
        {
            Printf("memory compare fail at address offset %d (write: 0xaa55aa55, read: 0x%2x)\n",i,(*pu4Ptr));             
            u4FailFlag = 1;
        } 
        vIO32Write4B(PDWNC_UP_ADDR, 0 + i);
        vIO32Write4B(PDWNC_UP_DATA, 0x55aa55aa);

        i+=4;  
    } 
    if(u4FailFlag)
    {
        Printf("Memory compare fail...\n");
        return 0;
    }
    
    Printf("Step4...\n");    
//step4    
    i = 0;
    u4FailFlag = 0;    
    while(i < INSTRUCTION_MEM_TEST_SIZE)
    {
        vIO32Write4B(PDWNC_UP_ADDR, 0 + i);
        *pu4Ptr = u4IO32Read4B(PDWNC_UP_DATA);
        if((*pu4Ptr) != 0x55aa55aa)
        {
            Printf("memory compare fail at address offset %d (write: 0x55aa55aa, read: 0x%2x)\n", i, (*pu4Ptr));             
            u4FailFlag = 1;
        } 
        vIO32Write4B(PDWNC_UP_ADDR, 0 + i);
        vIO32Write4B(PDWNC_UP_DATA, 0xaa55aa55);

        i+=4;  
    } 
    if(u4FailFlag)
    {
        Printf("Memory compare fail...\n");
        return 0;
    }
    
    Printf("Step5...\n");    
//step5
    i = 0;
    u4FailFlag = 0;
    while(i < INSTRUCTION_MEM_TEST_SIZE)
    {
        vIO32Write4B(PDWNC_UP_ADDR, 0 + i);
        *pu4Ptr = u4IO32Read4B(PDWNC_UP_DATA);
        if((*pu4Ptr) != 0xaa55aa55)
        {
            Printf("memory compare fail at address offset %d (write: 0xaa55aa55, read: 0x%2x)\n",i, (*pu4Ptr));             
            u4FailFlag = 1;
        } 
        vIO32Write4B(PDWNC_UP_ADDR, 0 + i);
        vIO32Write4B(PDWNC_UP_DATA, 0x55aa55aa);

        i+=4;  
    } 
    if(u4FailFlag)
    {
        Printf("Memory compare fail...\n");
        return 0;
    }
    
    Printf("Step6...\n");    
//step6    
    i = 0;
    u4FailFlag = 0;    
    vIO32Write4B(PDWNC_UP_ADDR, 0);
    while(i < INSTRUCTION_MEM_TEST_SIZE)
    {
        *pu4Ptr = u4IO32Read4B(PDWNC_UP_DATA);
        if((*pu4Ptr) != 0x55aa55aa)
        {
            Printf("memory compare fail at address offset %d (write: 0x55aa55aa, read: 0x%2x)\n",i,  (*pu4Ptr));             
            u4FailFlag = 1;
        } 
        i+=4;
    } 
    if(u4FailFlag)
    {
        Printf("Memory compare fail...\n");
    }
    Printf("March algorithm pass!!\n");
    return 0;    
    
}



static INT32 _CLI_XdataReadWriteTest(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 i;
    UINT32 u4FailFlag = 0;
    volatile UINT8* pu1Ptr;
    UINT8 u1XdataArray[MEM_TEST_SIZE];
    
    vIO32WriteFldAlign(PDWNC_UP_CFG, 0x1, FLD_XDATA_ACC);//switch xdata to ARM
    Printf("_CLI_XdataReadWriteTest...Size(%d)\n", MEM_TEST_SIZE);
//step1
    Printf("Step1...\n");
    i = 0;
    while(i < MEM_TEST_SIZE)
    {
        u1XdataArray[i] = 0xaa;
        i++;
        u1XdataArray[i] = 0x55;
        i++;
        u1XdataArray[i] = 0xaa;
        i++;
        u1XdataArray[i] = 0x55;
        i++;
    }

    pu1Ptr = (volatile UINT8*)u1XdataArray;
    vIO32Write4B(PDWNC_UP_ADDR, 0x8000);
    for(i = 0; i < MEM_TEST_SIZE; i++)
    {
        vIO32Write4B(PDWNC_UP_DATA, *pu1Ptr);
        pu1Ptr++;
    }    
    Printf("Step2...\n");    
//step2    
    i = 0; 
    u4FailFlag = 0;
    while(i < MEM_TEST_SIZE)
    {
        vIO32Write4B(PDWNC_UP_ADDR, 0x8000 + i);
        *pu1Ptr = u1IO32Read1B(PDWNC_UP_DATA);
        if((*pu1Ptr) != 0xaa)
        {
            Printf("memory compare fail at address offset %d (write: 0xaa, read: 0x%2x)\n",i,  (*pu1Ptr));
            u4FailFlag = 1;
        } 
        vIO32Write4B(PDWNC_UP_ADDR, 0x8000 + i);
        vIO32Write4B(PDWNC_UP_DATA, 0x55);

        i++;  
        
        vIO32Write4B(PDWNC_UP_ADDR, 0x8000 + i);
        *pu1Ptr = u1IO32Read1B(PDWNC_UP_DATA);
        if((*pu1Ptr) != 0x55)
        {
            Printf("memory compare fail at address offset %d (write: 0x55, read: 0x%2x)\n", i, (*pu1Ptr));             
            u4FailFlag = 1;
        } 
        vIO32Write4B(PDWNC_UP_ADDR, 0x8000 + i);
        vIO32Write4B(PDWNC_UP_DATA, 0xaa);
        
        i++;  

        vIO32Write4B(PDWNC_UP_ADDR, 0x8000 + i);
        *pu1Ptr = u1IO32Read1B(PDWNC_UP_DATA);
        if((*pu1Ptr) != 0xaa)
        {
            Printf("memory compare fail at address offset %d (write: 0xaa, read: 0x%2x)\n",i,  (*pu1Ptr));             
            u4FailFlag = 1;            
        } 
        vIO32Write4B(PDWNC_UP_ADDR, 0x8000 + i);
        vIO32Write4B(PDWNC_UP_DATA, 0x55);
        
        i++;  

        vIO32Write4B(PDWNC_UP_ADDR, 0x8000 + i);
        *pu1Ptr = u1IO32Read1B(PDWNC_UP_DATA);
        if((*pu1Ptr) != 0x55)
        {
            Printf("memory compare fail at address offset %d (write: 0x55, read: 0x%2x)\n",i,  (*pu1Ptr));             
            u4FailFlag = 1;
        } 
        vIO32Write4B(PDWNC_UP_ADDR, 0x8000 + i);
        vIO32Write4B(PDWNC_UP_DATA, 0xaa);
        
        i++;  
    } 
    if(u4FailFlag)
    {
        Printf("Memory compare fail...\n");
        return 0;
    }
    Printf("Step3...\n");    
//step3
    i = 0;
    u4FailFlag = 0;
    while(i < MEM_TEST_SIZE)
    {
        vIO32Write4B(PDWNC_UP_ADDR, 0x8000 + i);
        *pu1Ptr = u1IO32Read1B(PDWNC_UP_DATA);
        if((*pu1Ptr) != 0x55)
        {
            Printf("memory compare fail at address offset %d (write: 0x55, read: 0x%2x)\n", i, (*pu1Ptr));             
            u4FailFlag = 1;
        } 
        vIO32Write4B(PDWNC_UP_ADDR, 0x8000 + i);
        vIO32Write4B(PDWNC_UP_DATA, 0xaa);

        i++;  
        
        vIO32Write4B(PDWNC_UP_ADDR, 0x8000 + i);
        *pu1Ptr = u1IO32Read1B(PDWNC_UP_DATA);
        if((*pu1Ptr) != 0xaa)
        {
            Printf("memory compare fail at address offset %d (write: 0xaa, read: 0x%2x)\n", (*pu1Ptr));             
            u4FailFlag = 1;
        } 
        vIO32Write4B(PDWNC_UP_ADDR, 0x8000 + i);
        vIO32Write4B(PDWNC_UP_DATA, 0x55);
        
        i++;  

        vIO32Write4B(PDWNC_UP_ADDR, 0x8000 + i);
        *pu1Ptr = u1IO32Read1B(PDWNC_UP_DATA);
        if((*pu1Ptr) != 0x55)
        {
            Printf("memory compare fail at address offset %d (write: 0x55, read: 0x%2x)\n", (*pu1Ptr));             
            u4FailFlag = 1;
        } 
        vIO32Write4B(PDWNC_UP_ADDR, 0x8000 + i);
        vIO32Write4B(PDWNC_UP_DATA, 0xaa);
        
        i++;  

        vIO32Write4B(PDWNC_UP_ADDR, 0x8000 + i);
        *pu1Ptr = u1IO32Read1B(PDWNC_UP_DATA);
        if((*pu1Ptr) != 0xaa)
        {
            Printf("memory compare fail at address offset %d (write: 0xaa, read: 0x%2x)\n",i,  (*pu1Ptr));             
            u4FailFlag = 1;
        } 
        vIO32Write4B(PDWNC_UP_ADDR, 0x8000 + i);
        vIO32Write4B(PDWNC_UP_DATA, 0x55);
        
        i++;  
    } 
    if(u4FailFlag)
    {
        Printf("Memory compare fail...\n");
        return 0;
    }
    
    Printf("Step4...\n");    
//step4    
    i = 0;
    u4FailFlag = 0;    
    while(i < MEM_TEST_SIZE)
    {
        vIO32Write4B(PDWNC_UP_ADDR, 0x8000 + i);
        *pu1Ptr = u1IO32Read1B(PDWNC_UP_DATA);
        if((*pu1Ptr) != 0xaa)
        {
            Printf("memory compare fail at address offset %d (write: 0xaa, read: 0x%2x)\n", i, (*pu1Ptr));             
            u4FailFlag = 1;
        } 
        vIO32Write4B(PDWNC_UP_ADDR, 0x8000 + i);
        vIO32Write4B(PDWNC_UP_DATA, 0x55);

        i++;  
        
        vIO32Write4B(PDWNC_UP_ADDR, 0x8000 + i);
        *pu1Ptr = u1IO32Read1B(PDWNC_UP_DATA);
        if((*pu1Ptr) != 0x55)
        {
            Printf("memory compare fail at address offset %d (write: 0x55, read: 0x%2x)\n", i, (*pu1Ptr));             
            u4FailFlag = 1;
        } 
        vIO32Write4B(PDWNC_UP_ADDR, 0x8000 + i);
        vIO32Write4B(PDWNC_UP_DATA, 0xaa);
        
        i++;  

        vIO32Write4B(PDWNC_UP_ADDR, 0x8000 + i);
        *pu1Ptr = u1IO32Read1B(PDWNC_UP_DATA);
        if((*pu1Ptr) != 0xaa)
        {
            Printf("memory compare fail at address offset %d (write: 0xaa, read: 0x%2x)\n", i, (*pu1Ptr));             
            u4FailFlag = 1;
        } 
        vIO32Write4B(PDWNC_UP_ADDR, 0x8000 + i);
        vIO32Write4B(PDWNC_UP_DATA, 0x55);
        
        i++;  

        vIO32Write4B(PDWNC_UP_ADDR, 0x8000 + i);
        *pu1Ptr = u1IO32Read1B(PDWNC_UP_DATA);
        if((*pu1Ptr) != 0x55)
        {
            Printf("memory compare fail at address offset %d (write: 0x55, read: 0x%2x)\n",i,  (*pu1Ptr));             
            u4FailFlag = 1;
        } 
        vIO32Write4B(PDWNC_UP_ADDR, 0x8000 + i);
        vIO32Write4B(PDWNC_UP_DATA, 0xaa);
        
        i++;  
    } 
    if(u4FailFlag)
    {
        Printf("Memory compare fail...\n");
        return 0;
    }
    
    Printf("Step5...\n");    
//step5
    i = 0;
    u4FailFlag = 0;
    while(i < MEM_TEST_SIZE)
    {
        vIO32Write4B(PDWNC_UP_ADDR, 0x8000 + i);
        *pu1Ptr = u1IO32Read1B(PDWNC_UP_DATA);
        if((*pu1Ptr) != 0x55)
        {
            Printf("memory compare fail at address offset %d (write: 0x55, read: 0x%2x)\n",i,  (*pu1Ptr));             
            u4FailFlag = 1;
        } 
        vIO32Write4B(PDWNC_UP_ADDR, 0x8000 + i);
        vIO32Write4B(PDWNC_UP_DATA, 0xaa);

        i++;  
        
        vIO32Write4B(PDWNC_UP_ADDR, 0x8000 + i);
        *pu1Ptr = u1IO32Read1B(PDWNC_UP_DATA);
        if((*pu1Ptr) != 0xaa)
        {
            Printf("memory compare fail at address offset %d (write: 0xaa, read: 0x%2x)\n",i,  (*pu1Ptr));             
            u4FailFlag = 1;
        } 
        vIO32Write4B(PDWNC_UP_ADDR, 0x8000 + i);
        vIO32Write4B(PDWNC_UP_DATA, 0x55);
        
        i++;  

        vIO32Write4B(PDWNC_UP_ADDR, 0x8000 + i);
        *pu1Ptr = u1IO32Read1B(PDWNC_UP_DATA);
        if((*pu1Ptr) != 0x55)
        {
            Printf("memory compare fail at address offset %d (write: 0x55, read: 0x%2x)\n",i,  (*pu1Ptr));             
            u4FailFlag = 1;
        } 
        vIO32Write4B(PDWNC_UP_ADDR, 0x8000 + i);
        vIO32Write4B(PDWNC_UP_DATA, 0xaa);
        
        i++;  

        vIO32Write4B(PDWNC_UP_ADDR, 0x8000 + i);
        *pu1Ptr = u1IO32Read1B(PDWNC_UP_DATA);
        if((*pu1Ptr) != 0xaa)
        {
            Printf("memory compare fail at address offset %d (write: 0xaa, read: 0x%2x)\n", i, (*pu1Ptr));             
            u4FailFlag = 1;
        } 
        vIO32Write4B(PDWNC_UP_ADDR, 0x8000 + i);
        vIO32Write4B(PDWNC_UP_DATA, 0x55);
        
        i++;  
    } 
    if(u4FailFlag)
    {
        Printf("Memory compare fail...\n");
        return 0;
    }
    
    Printf("Step6...\n");    
//step6    
    i = 0;
    u4FailFlag = 0;    
    vIO32Write4B(PDWNC_UP_ADDR, 0x8000);
    while(i < MEM_TEST_SIZE)
    {
        *pu1Ptr = u1IO32Read1B(PDWNC_UP_DATA);
        if((*pu1Ptr) != 0xaa)
        {
            Printf("memory compare fail at address offset %d (write: 0xaa, read: 0x%2x)\n", i, (*pu1Ptr));             
            u4FailFlag = 1;
        } 
        i++;
        *pu1Ptr = u1IO32Read1B(PDWNC_UP_DATA);
        if((*pu1Ptr) != 0x55)
        {
            Printf("memory compare fail at address offset %d (write: 0x55, read: 0x%2x)\n",i,  (*pu1Ptr));             
            u4FailFlag = 1;
        } 
        i++;        
        *pu1Ptr = u1IO32Read1B(PDWNC_UP_DATA);
        if((*pu1Ptr) != 0xaa)
        {
            Printf("memory compare fail at address offset %d (write: 0xaa, read: 0x%2x)\n",i,  (*pu1Ptr));             
            u4FailFlag = 1;
        } 
        i++;        
        *pu1Ptr = u1IO32Read1B(PDWNC_UP_DATA);
        if((*pu1Ptr) != 0x55)
        {
            Printf("memory compare fail at address offset %d (write: 0x55, read: 0x%2x)\n",i,  (*pu1Ptr));             
            u4FailFlag = 1;
        } 
        i++;  
    } 
    if(u4FailFlag)
    {
        Printf("Memory compare fail...\n");
    }
    Printf("March algorithm pass!!\n");
    return 0;    
    
}
#endif


static INT32 _CLI_MemWriteByte(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4SrcAddr;
    UINT8 u1Data;

    if(i4Argc != 3)
    {
        Printf("Need address & data infomation!\n");
        Printf("Ex: pdwnc.wb 0x%4x 0xa5\n",XDATA_ADDR);
        return 0;
    }
    
    u4SrcAddr = StrToInt(szArgv[1]);
    u1Data = StrToInt(szArgv[2]);
    if((u4SrcAddr < XDATA_ADDR) || (u4SrcAddr > (XDATA_ADDR + 0x07FF)))
    {
        Printf("address range error!\n");
        Printf("Address: 0x%4x ~ 0x%4x\n",XDATA_ADDR, (XDATA_ADDR + 0x07FF));
        return 0;
    }
#ifndef CC_MTK_LOADER
        x_sema_lock(_hT8032CmdMutex, X_SEMA_OPTION_WAIT); //make sure no other thread can interrupt T8032 in this critical time
#endif
    
        if(_PDWNC_SetT8032(PDWNC_T8032_SET_XDATA_ARM))
        {
            Printf("SET_XDATA_ARM fail !!\n");
            return 0;
        }
    
        // Copy CEC data to XData
        if(_CopyToT8032_XDATA(u4SrcAddr, 1, (UINT32)(&u1Data)))
        {
            Printf("Set Xdata fail !!\n");
            return 0;
        }
        
    
        if(_PDWNC_SetT8032(PDWNC_T8032_SET_XDATA_T8032))
        {
            Printf("SET_XDATA_T8032 fail !!\n");
            return 0;
        }
    
#ifndef CC_MTK_LOADER
        x_sema_unlock(_hT8032CmdMutex);
#endif
    return 0;
}


static INT32 _CLI_MemWriteWord(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4SrcAddr;
    UINT16 u2Data;

    if(i4Argc != 3)
    {
        Printf("Need address & data infomation!\n");
        Printf("Ex: pdwnc.ww 0x%4x 0xa55a\n",XDATA_ADDR);
        return 0;
    }
    
    u4SrcAddr = StrToInt(szArgv[1]);
    u2Data = StrToInt(szArgv[2]);
    if((u4SrcAddr < XDATA_ADDR) || (u4SrcAddr > (XDATA_ADDR + 0x07FF)))
    {
        Printf("address range error!\n");
        Printf("Address: 0x%4x ~ 0x%4x\n",XDATA_ADDR, (XDATA_ADDR + 0x07FF));
        return 0;
    }
#ifndef CC_MTK_LOADER
        x_sema_lock(_hT8032CmdMutex, X_SEMA_OPTION_WAIT); //make sure no other thread can interrupt T8032 in this critical time
#endif
    
        if(_PDWNC_SetT8032(PDWNC_T8032_SET_XDATA_ARM))
        {
            Printf("SET_XDATA_ARM fail !!\n");
            return 0;
        }
#if 0    
        // Copy CEC data to XData
        if(_CopyToT8032_XDATA(u4SrcAddr, 2, (UINT32)(&u2Data)))
        {
            Printf("Set Xdata fail !!\n");
            return 0;
        }
#else
        vIO32WriteFldAlign(PDWNC_UP_ADDR, (UINT16)(u4SrcAddr), FLD_UP_ADDR);
        vIO32Write4B(PDWNC_UP_DATA, (u2Data >> 8) & 0x000000ff);
        vIO32Write4B(PDWNC_UP_DATA, u2Data & 0x000000ff);
#endif
    
        if(_PDWNC_SetT8032(PDWNC_T8032_SET_XDATA_T8032))
        {
            Printf("SET_XDATA_T8032 fail !!\n");
            return 0;
        }
    
#ifndef CC_MTK_LOADER
        x_sema_unlock(_hT8032CmdMutex);
#endif
    return 0;
}

static INT32 _CLI_MemWriteDword(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4SrcAddr;
    UINT32 u4Data;

    if(i4Argc != 3)
    {
        Printf("Need address & data infomation!\n");
        Printf("Ex: pdwnc.ww 0x%4x 0xa55a\n",XDATA_ADDR);
        return 0;
    }
    
    u4SrcAddr = StrToInt(szArgv[1]);
    u4Data = StrToInt(szArgv[2]);
    if((u4SrcAddr < XDATA_ADDR) || (u4SrcAddr > (XDATA_ADDR + 0x07FF)))
    {
        Printf("address range error!\n");
        Printf("Address: 0x%4x ~ 0x%4x\n",XDATA_ADDR, (XDATA_ADDR + 0x07FF));
        return 0;
    }
#ifndef CC_MTK_LOADER
        x_sema_lock(_hT8032CmdMutex, X_SEMA_OPTION_WAIT); //make sure no other thread can interrupt T8032 in this critical time
#endif
    
        if(_PDWNC_SetT8032(PDWNC_T8032_SET_XDATA_ARM))
        {
            Printf("SET_XDATA_ARM fail !!\n");
            return 0;
        }
    
        // Copy CEC data to XData
#if 0        
        if(_CopyToT8032_XDATA(u4SrcAddr, 4, (UINT32)(&u4Data)))
        {
            Printf("Set Xdata fail !!\n");
            return 0;
        }
#else
        vIO32WriteFldAlign(PDWNC_UP_ADDR, (UINT16)(u4SrcAddr), FLD_UP_ADDR);
        vIO32Write4B(PDWNC_UP_DATA, (u4Data >> 24) & 0x000000ff);
        vIO32Write4B(PDWNC_UP_DATA, (u4Data >> 16) & 0x000000ff);
        vIO32Write4B(PDWNC_UP_DATA, (u4Data >> 8) & 0x000000ff);
        vIO32Write4B(PDWNC_UP_DATA, u4Data & 0x000000ff);
#endif
    
        if(_PDWNC_SetT8032(PDWNC_T8032_SET_XDATA_T8032))
        {
            Printf("SET_XDATA_T8032 fail !!\n");
            return 0;
        }
    
#ifndef CC_MTK_LOADER
        x_sema_unlock(_hT8032CmdMutex);
#endif
    return 0;
}

#if 0
EXTERN RTC_FuncTbl* _RTC_GetFunc(void);
EXTERN RTC_FuncTbl* RTC_Func;
static INT32 _CLI_Stress(INT32 i4Argc, const CHAR ** szArgv)
{
    /*UINT32 u4Val;
    
    PDWNC_Init();
    u4Val = IRRX_ReadSvadc(402);
    if(u4Val < 16) // VGA wakeup stress
    {
#if 0    	
        VDP_Init();
        VdoMLInit();
        bApiVideoSetVideoSrc(0, SV_VS_VGA);
        PDWNC_EnterPowerDown(WAK_VGA, (UINT32)PDWNC_POWER_DOWN_UNKNOWN);        
#else        
        CLI_Parser("pdwnc.downservo 4 240 255");
#endif        
    }
    else if(u4Val < 32) // RTC wakeup stress
    {
*/    
        UINT64 u8Time;
  RTC_T rRtc;
 /*     if(u4IO32Read4B(PDWNC_RESRV0) == 0x53875387)
      {
          Printf("AC On!!! RCT Init...\n");
//        RTC_Init();
        vIO32Write4B(PDWNC_RESRV0, 0x0);
       }
      else*/
            RTC_Func = _RTC_GetFunc();
        RTC_GetTimeDate(&u8Time);


   RTC_UtcToRtc(&rRtc, u8Time);
    Printf("Now time is %2d/%2d/%2d %2d:%2d:%2d\n", 
    	rRtc.u1Year,
    	rRtc.u1Month,
   	rRtc.u1Day,
    	rRtc.u1Hour,
    	rRtc.u1Minute,
    	rRtc.u1Second
    	);
Printf("System is going to power down mode, and will wakeup at : \n");
HAL_Delay_us(150 * 1000);
        u8Time += 10;
   RTC_UtcToRtc(&rRtc, u8Time);
    Printf(" %2d/%2d/%2d %2d:%2d:%2d\n", 
    	rRtc.u1Year,
    	rRtc.u1Month,
   	rRtc.u1Day,
    	rRtc.u1Hour,
    	rRtc.u1Minute,
    	rRtc.u1Second
    	);
   
        RTC_SetAlertTime(u8Time);
//        PDWNC_EnterPowerDown(WAK_RTC, (UINT32)PDWNC_POWER_DOWN_UNKNOWN);
         vIO32Write4B(PDWNC_WAKEN,  WAK_RTC);//   PDWNC_WRITE32(REG_RW_PDWAKE_EN, u4PowerDownEn);   
        PDWNC_SetupPowerDown();    

    //}

    return 0;

}


static void _GpioHandler(INT32 i4Gpio, BOOL fgStatus)
{
    Printf("Gpio[%d] int val = %d\n", i4Gpio, fgStatus);
}

static INT32 _CLI_IntTest(INT32 i4Argc, const CHAR ** szArgv)
{
    
    UINT32 u4Gpio = 0;
    GPIO_TYPE rType = GPIO_TYPE_INTR_RISE;
    INT32 i4Set = 1;
    
    if(i4Argc >= 3)
    {
        u4Gpio = StrToInt(szArgv[1]);
        if(x_strcmp(szArgv[2], "rise") == 0)
        {
            rType = GPIO_TYPE_INTR_RISE;
        }
        else if(x_strcmp(szArgv[2], "fall") == 0)
        {
            rType = GPIO_TYPE_INTR_FALL;
        }
        else if(x_strcmp(szArgv[2], "both") == 0)
        {
            rType = GPIO_TYPE_INTR_BOTH;
        }        
    }
    else
    {
    }
    if(i4Argc >= 4)    	
    {
        if(x_strcmp(szArgv[3], "on") == 0)
        {
            i4Set = 1;
        }
        else
        {
            i4Set = 0;        
        }        
    }
    
    PDWNC_Init();

    GPIO_Reg(u4Gpio, rType, _GpioHandler);
    GPIO_Intrq(u4Gpio, &i4Set);

    return 0;
}
#endif
#if defined(CC_MT5368)||defined(CC_MT5396)||defined(CC_MT5389)
static void _CLI_WDT_Callback(INT32 i4Wdt)
{
    Printf("\nWDT%d Interrupt!\n", i4Wdt);
	
    HAL_Delay_us(10*1000);
	
}


static INT32 _CLI_WDTREG(INT32 i4Argc, const CHAR** aszArgv)
{
    INT32 i, i4Wdt, i4En, i4Dis;

    if (i4Argc < 2) {
		Printf("Enable/disable WDT interrupt\n");
        Printf("%s [-e -d] [wdt num]\n", aszArgv[0]);
        return 1;
    }

    i4Wdt = i4En = i4Dis = 0;
    for (i=1; i<i4Argc; i++) {
        switch(aszArgv[i][0]) {
        case '-':
            if (aszArgv[i][1]=='e') 
			{
                i4En = 1;
            } 
			else if (aszArgv[i][1]=='d') 
			{
                i4Dis = 1;
			}
            break;
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            i4Wdt = StrToInt(aszArgv[i]);
            break;
        default:
            Printf("%s [-e -d] [wdt num]\n", aszArgv[0]);
            return 1;
        }
    }
    if (!i4En && !i4Dis) 
	{
        Printf("%s [-e -d] [wdt num]\n", aszArgv[0]);
        return 1;
    }
	if(i4En)
	{
		if((WDT_ISR_Reg((UINT32)i4Wdt, _CLI_WDT_Callback)) < 0)
		{
			Printf("Reg WDT interrupt fail..\n");
			return -1;
		}
	}
	if(i4Dis)
	{
		if((WDT_ISR_Reg((UINT32)i4Wdt, NULL)) < 0)
		{
			Printf("Disable WDT interrupt fail..\n");
			return -1;
		}
	}
	return 0;
}
#endif
static INT32 _CLI_T8032CMD(INT32 i4Argc, const CHAR ** szArgv)
{
	  PDWNC_T8032_CMD_T rCmd;
    PDWNC_T8032_RESPONSE_T rResp;
    
    if((i4Argc != 7) && (i4Argc != 6))
    {	
        Printf("argc is %d\n",i4Argc);
    	Printf("pdwnc.t8032cmd Cmd subCmd data0 data1 data2 data3\n");
    	return 0;
    }	
	  rCmd.u1Cmd = StrToInt(szArgv[1]);
    rCmd.u1SubCmd = StrToInt(szArgv[2]);
    rCmd.au1Data[0] = StrToInt(szArgv[3]);
    rCmd.au1Data[1] = StrToInt(szArgv[4]);
    rCmd.au1Data[2] = StrToInt(szArgv[5]);        
	  rCmd.au1Data[3] = StrToInt(szArgv[6]);
	  PDWNC_T8032Cmd(&rCmd, &rResp);
	  Printf("Response is Ack: 0x%2x, Data0: 0x%2x, Data1: 0x%2x, Data2: 0x%2x\n", rResp.u1Ack, rResp.au1Data[0],rResp.au1Data[1],rResp.au1Data[2]);
	  return 0;
}
/*
extern UINT8 _au1CecData[];
//EXTERN INT32 _PDWNC_SetT8032(UINT32 u4State);
//EXTERN INT32 _PDWNC_TransferCECData(UINT32 u4Direction);
static INT32 _CLI_GETCEC_DATA_CMD(INT32 i4Argc, const CHAR ** szArgv)
{
    _PDWNC_GetT8302CECData();
    Printf("%s\n", (char*)_au1CecData);
    return 0;
}
*/
/******************************************************************************
* End of Declaration
******************************************************************************/

static CLI_EXEC_T _arPdwncCmdTbl[] =
{
    // LOG macro support    
	CLIMOD_DEBUG_CLIENTRY(PDWNC),
	{
		"init", "i", _CLI_Init, NULL, "", CLI_SUPERVISOR
	},
	{
		"normalstandby", "normalstandby", _CLI_PowerDown_NormalWakeup, NULL, "", CLI_SUPERVISOR
	},
	{
		"powersavingstandby", "powersavingstandby", _CLI_PowerDown_PowerSavingWakeup, NULL, "", CLI_SUPERVISOR
	},
	{
		"readservo", "rservo", _CLI_ReadServo, NULL, "", CLI_SUPERVISOR
	},	
	{
		"uploaduPFW", "fw", _CLI_UploadT8032FW, NULL, "", CLI_SUPERVISOR
	},
	{
		"uploaduPFWbufferAddr", "q", _CLI_PrintUploadT8032FW, NULL, "", CLI_SUPERVISOR
	},
#if defined(CC_MT5365) || defined(CC_MT5395)	
	{
		"updateVGAEDID", "wvga", _CLI_UpdateVGA_EDID, NULL, "", CLI_SUPERVISOR
	},	
#if 0	
	{
		"testVGAEDID", "vgawr", _CLI_TestVGA_EDID, NULL, "", CLI_SUPERVISOR
	},	
#endif	
#endif	
	{
		"SwitchUart0", "suart0", _CLI_SwitchUartPD_PORT0, NULL, "", CLI_SUPERVISOR
	},
	{
		"downir", "downir", _CLI_PowerDown_IrWakeup, NULL, "", CLI_SUPERVISOR
	},
	{
		"downirservo", "downirservo", _CLI_PowerDown_IrServoWakeup, NULL, "", CLI_SUPERVISOR
	},
	{
		"downirservovga", "downirservovga", _CLI_PowerDown_IrServoVgaWakeup, NULL, "", CLI_SUPERVISOR
	},
	{
		"downgpio", "downgpio", _CLI_PowerDown_GpioWakeup, NULL, "", CLI_SUPERVISOR
	},
	{
		"downrtc", "downrtc", _CLI_PowerDown_RtcWakeup, NULL, "", CLI_SUPERVISOR
	},
	{
		"downservo", "downservo", _CLI_PowerDown_ServoWakeup, NULL, "", CLI_SUPERVISOR
	},
	{
		"downuart", "downuart", _CLI_PowerDown_UartWakeup, NULL, "", CLI_SUPERVISOR
	},
	{
		"downvga", "downvga", _CLI_PowerDown_VgaWakeup, NULL, "", CLI_SUPERVISOR
	},
	{
		"downhdmi", "downhdmi", _CLI_PowerDown_HdmiWakeup, NULL, "", CLI_SUPERVISOR
	},	
    {
		"downhdmi", "downt8032", _CLI_PowerDown_T8032Wakeup, NULL, "", CLI_SUPERVISOR
	},	
    {
		"downmask", "downmake", _CLI_PowerDown_MaskWakeup, NULL, "", CLI_SUPERVISOR
	},	
#if defined(CC_MT5368)||defined(CC_MT5396)||defined(CC_MT5389)
	{
		"wdtreg", "wdtreg", _CLI_WDTREG, NULL, "", CLI_SUPERVISOR
	},	
#endif	

/*	{
		"downcec", "downcec", _CLI_PowerDown_CecWakeup, NULL, "", CLI_SUPERVISOR
	},		
	
	{
		"iodown", "iodown", _CLI_GpioPowerDown, NULL, "", CLI_SUPERVISOR
	},		
*/	
	{
		"sett8032", "st8032", _CLI_SetT8032, NULL, "", CLI_SUPERVISOR
	},			
	{
		"read", "r", _CLI_MemRead, NULL, "", CLI_SUPERVISOR
	},
        {
            "write", "w", _CLI_MemWrite, NULL, "", CLI_SUPERVISOR
        },  
#if 0      
        {
            "instrw", "irw", _CLI_InstrSramReadWriteTest, NULL, "", CLI_SUPERVISOR
        },  

        {
            "xdatarw", "xrw", _CLI_XdataReadWriteTest, NULL, "", CLI_SUPERVISOR
        },  
#endif        
        {
            "WriteByte", "wb", _CLI_MemWriteByte, NULL, "", CLI_SUPERVISOR
        },  
        {
            "WriteWord", "ww", _CLI_MemWriteWord, NULL, "", CLI_SUPERVISOR
        },  
        {
            "WriteDword", "wd", _CLI_MemWriteDword, NULL, "", CLI_SUPERVISOR
        },  
#if 0
	{
		"stress", "s", _CLI_Stress, NULL, "", CLI_SUPERVISOR
	},		
	{
		"inttest", "int", _CLI_IntTest, NULL, "", CLI_SUPERVISOR
	},		
#endif
	{
		"t8032cmd", "t8032cmd", _CLI_T8032CMD, NULL, "", CLI_SUPERVISOR
	},		
	
/*	
	{
		"getcec", "getcec", _CLI_GETCEC_DATA_CMD, NULL, "", CLI_SUPERVISOR
	},		
*/	
	{
		"t8032Xdatadump","dumpx",_CLI_XdataDump,NULL,"         dump current all mcu xdata",CLI_SUPERVISOR

	},
	
	{
		"ArmXdataWriteRead","armwr",_CLI_XdataArmWriteRead,NULL,"         arm write mcu xdata and read back",CLI_SUPERVISOR
	 
	},
	{
		"ArmXdataClear","armcl",_CLI_XdataArmClear,NULL,"         arm clear mcu xdata",CLI_SUPERVISOR
	 
	},
	{
		"t8032FWdump","dumpi",_CLI_InstrucdataDump,NULL,"	 dump current mcu source binary",CLI_SUPERVISOR
	 
	},
	{
		"t8032FWload","loadi",_CLI_InstrucdataLoad,NULL,"	 load current mcu source binary",CLI_SUPERVISOR
	 
	},
	
/*	
	{
		"TxToT8032","tx",_CLI_SendData,NULL,"         SentDataToT8032",CLI_SUPERVISOR
	},
	{
		"RxFromT8032","rx",_CLI_GetData,NULL,"		 GentDataFromT8032",CLI_SUPERVISOR
	},
*/	
	{
		"ArmXdataWrite","armw",_CLI_XdataArmWrite,NULL,"         arm write mcu xdata to let mcu read back",CLI_SUPERVISOR
	},
    {
        "ArmXdataRead","armr",_CLI_XdataArmReadBack,NULL,"        arm(mcu) write mcu xdata then arm read back",CLI_SUPERVISOR
    },
		
	/*
	{
		"writeReg", "wr", i4CmdWriteReg, NULL, "", CLI_SUPERVISOR
	},
	*/
	{
		NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR
	}   
};


CLI_MAIN_COMMAND_ITEM(Pdwnc)
{

    "pdwnc", "pdwnc", NULL, _arPdwncCmdTbl, "PDWNC commands", CLI_GUEST

};



