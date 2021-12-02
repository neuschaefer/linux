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
#include "ldm_if.h"
#include "ldm_reg.h"
#include "panel.h"
#include "nptv_debug.h"

#include "x_hal_926.h"
#include "x_hal_5381.h"
#include "x_printf.h"
#include "x_stl_lib.h"
#include "hw_vdoin.h"
#include "hw_ycproc.h"
#include "sv_const.h"
#include "x_ckgen.h"
#include "hw_sw.h"
#include "x_bim.h"


#define LDM_MAX_TIMEOUT 10000
#define LDM_MAX_DIMMING_SIZE    256
BOOL _fgLdmISR;

UINT8 _dimming_value[LDM_MAX_DIMMING_SIZE*2];
UINT32 u4HBlockNum,u4VBlockNum;

UINT8 bLdmPanelType;

/**********************************************************************************/
/******************************   SPI driver porting  ****** ******************************/
/**********************************************************************************/

#define LDM_BUS_CLOCK 144  //unit is MHz , SPI bus_clock = 144MHz

extern UINT32 PANEL_GetPixelClk60Hz(void);
extern UINT32 PANEL_GetVTotal60Hz(void);
extern UINT32 PANEL_GetHTotal60Hz(void);
extern UINT8 vDrvGetLCDFreq(void); 


UINT8 vDrvLDMHW_setPinmux(UINT8 u1Select)
{
   if(u1Select >= SPI_PINMUX_MAX)
   	{
   	  Printf("[LDM]Pinmux option is out of range\n");
	  return SV_FALSE;
   	}

   switch (u1Select) 
   	{
       case SPI_PINMUX_OPTION1:
             #ifdef CC_MT5365 
				// LDM_VSYNC
				CKGEN_MASK32(REG_PINMUX_SEL1, 0x00030000, 0x00030000);
				// LDM_CLK
				CKGEN_MASK32(REG_PINMUX_SEL3, 0x30000000, 0x30000000);
				// LDM_DOUT , LDM_CS
				CKGEN_MASK32(REG_PINMUX_SEL4, 0x0C000000, 0x0C000000);
             #endif
             #ifdef CC_MT5395
				// bit24: LDM_CS, bit25: LDM_DO/LDM_CLK, bit27: LDM_VSYNC
			    CKGEN_MASK32(REG_PINMUX_SEL1, 0x0B000000, 0x0B000000);
             #endif
	         break;
			 
	   case SPI_PINMUX_OPTION2:
            #ifdef CC_MT5365
              // PINMUX5[11] = 1: LDM_VSYNC, LDM_CLK, LDM_DOUT, LDM_CS
              CKGEN_MASK32(REG_PINMUX_SEL5, 0x0800, 0x0800);
			   break;
            #endif
            #ifdef CC_MT5395
              Printf("[LDM]MT5395 not support this Pinmux type\n");
			  return SV_FALSE;
            #endif
           
       default:
              Printf("[LDM]Pinmux type is Not support yet\n");
			  return SV_FALSE;
	   	
   	}

   return SV_TRUE;
   
}

void vDrvLDMHW_setControl(PANEL_LOCAL_DIMMING_ATTRIBUTE_T *pParam)
{
  if((pParam->u1BitMode==1)||(pParam->u1BitMode==2))
  	{
  	 vIO32WriteFldAlign(LED_CON, LDM_SRAM_DEPTH_16BIT, SRAM_FOLD_MODE); //set SRAM fold Mode
  	}
  else
    {
     vIO32WriteFldAlign(LED_CON, LDM_SRAM_DEPTH_8BIT, SRAM_FOLD_MODE);  //set SRAM fold Mode
  	}
  	
  vIO32WriteFldAlign(LED_CON, LDM_SW_TIRG_EN_FLAG, SW_TRIG_EN); //enable  Trigger by SW instead of Vsync input
  vIO32WriteFldAlign(LED_CON, LDM_SW_SET_DATA_FLAG, SW_SET_DATA); //SW set data
  
}

void vDrvLDMHW_setClock(PANEL_LOCAL_DIMMING_ATTRIBUTE_T *pParam)
{
  UINT16 u2ClkDiv;

  u2ClkDiv =  ((LDM_BUS_CLOCK*1000000/(pParam->u4OutputClock))-2)>>1; 

  vIO32WriteFldAlign(LED_CKCFG, u2ClkDiv, LDM_CLK_DIV);  //set clock divide value

  vIO32WriteFldAlign(LED_CKCFG, pParam->u1CPOL, CPOL);  //set clock polarity

  vIO32WriteFldAlign(LED_CKCFG, pParam->u1CPHA, CPHA); //set clock phase
  
}

void vDrvLDMHW_setVsync(PANEL_LOCAL_DIMMING_ATTRIBUTE_T *pParam)
{
  UINT16 Tvh, Twait;

  Tvh = (pParam->u1VSyncHighTime) * LDM_BUS_CLOCK ;
  Twait = (pParam->u1WaitTime) * LDM_BUS_CLOCK ;

  vIO32WriteFldAlign(LED_VSACC, Tvh, VSYNC_HOLD_LATENCY);  //set Vsync high time
  vIO32WriteFldAlign(LED_VSACC, Twait, WAIT_LATENCY);      //set wait time from Vsync low to CS low
}

void vDrvLDMHW_setCS(PANEL_LOCAL_DIMMING_ATTRIBUTE_T *pParam)
{
   UINT16 Tch;  //CS high time
   UINT8 Tcld,Tclg;

   Tch = LDM_BUS_CLOCK*(pParam->u2CSHighTime)/1000;
   Tcld = (LDM_BUS_CLOCK*(pParam->u1Tcld)+500)/1000;
   Tclg = (LDM_BUS_CLOCK*(pParam->u1Tclg)+500)/1000;
   
   vIO32WriteFldAlign(LED_CSACC, Tch, CS_HOLD_LATENCY);	 //set CS high time
   vIO32WriteFldAlign(LED_CSACC, Tcld, CS_LEAD_LATENCY);	 //set setup time from CS low to CLK edge
   vIO32WriteFldAlign(LED_CSACC, Tclg, CS_LAG_LATENCY);	 //set hold time from CLK edge to CS high

   vIO32WriteFldAlign(LED_CSACC, pParam->u1HdataSingleTrans, MDATA_SINGLE_EN); 
   vIO32WriteFldAlign(LED_CSACC, pParam->u1VdataSingleTrans, NDATA_SINGLE_EN);

}

void vDrvLDMHW_setDataLen(PANEL_LOCAL_DIMMING_ATTRIBUTE_T *pParam)
{
  UINT16 HDataLen,VDataLen;

  HDataLen = pParam->u1BlockNumM;
  VDataLen = pParam->u1BlockNumN;

  vIO32WriteFldAlign(LED_NOB, HDataLen, MDATA_LEN);  //set horizontal data len of Bytes
  vIO32WriteFldAlign(LED_NOB, VDataLen, NDATA_LEN);  //set vertical data len of Bytes
 
}

UINT32 vDrvLDMHW_getDataLen(void)
{
    UINT32 u4DataLen;

    u4DataLen = u4IO32Read4B(LED_NOB);
    u4DataLen = ((u4DataLen >>20) & 0x3ff) * ((u4DataLen >>4) & 0x3ff); 

	return u4DataLen; 
}

void vDrvLDMHW_setLSBMode(PANEL_LOCAL_DIMMING_ATTRIBUTE_T *pParam)
{
   vIO32WriteFldAlign(LED_FMT, pParam->u1LSBMode, LSB_MODE);  //set LSB mode
}

UINT8 vDrvLDMHW_getLSBMode(void)
{
   UINT8 bLSBMode;
   
   bLSBMode = IO32ReadFldAlign(LED_FMT,LSB_MODE);
   
   return bLSBMode;
}

void vDrvLDMHW_setRGBMode(PANEL_LOCAL_DIMMING_ATTRIBUTE_T *pParam)
{
   vIO32WriteFldAlign(LED_FMT, pParam->u1RGBMode, SPI_RGB_MODE);  //set RGB mode
}

void vDrvLDMHW_setBITMode(PANEL_LOCAL_DIMMING_ATTRIBUTE_T *pParam)
{
   vIO32WriteFldAlign(LED_FMT, pParam->u1BitMode, BIT_MODE);  //set Bit mode
}

void vDrvLDMHW_setLEDType(PANEL_LOCAL_DIMMING_ATTRIBUTE_T *pParam)
{
   vIO32WriteFldAlign(LED_FMT, pParam->u1LEDType, LED_TYPE);  //set LED type
}


void vDrvLDMHW_setFrameRate(PANEL_LOCAL_DIMMING_ATTRIBUTE_T *pParam)
{
   
   UINT8 u1PanelFreq,u1InputFreq;
   UINT8 u1FrameRate,u1FrameRateMode;
   UINT32 u4VsyncLat;


   u1InputFreq =vDrvGetLCDFreq();  //SPI input frame rate

   #ifdef DRV_SUPPORT_EXTMJC
   u1PanelFreq =u1InputFreq*2;  //connect extern MJC
   #else
   u1PanelFreq =u1InputFreq; 
   #endif
   
   LOG(0,"SPI output frame rate=%d\n",u1PanelFreq);

   u1FrameRateMode =(u1PanelFreq>=u1InputFreq)?0:1;
   if(u1FrameRateMode)
   	{
   	   u1FrameRate = u1InputFreq/u1PanelFreq - 1;
   	}
   	else
    {
       u1FrameRate = u1PanelFreq/u1InputFreq - 1;
   	}
   u4VsyncLat = LDM_BUS_CLOCK*1000000/u1PanelFreq;
	
   //LOG(0,"frame rate =%d\n",u1FrameRate);
   //LOG(0,"frame rate mode=%d\n",u1FrameRateMode);
   //LOG(0,"Vsync latency=%d\n",u4VsyncLat);

   vIO32WriteFldAlign(LED_VSCFG, u1FrameRateMode, FRAME_RATE_MODE);  //set frame rate mode
   vIO32WriteFldAlign(LED_VSCFG, u1FrameRate, FRAME_RATE);         //set frame rate 
   vIO32WriteFldAlign(LED_VSCFG, u4VsyncLat, VSYNC_LATENCY);      //set vsync latency
   
}



/** 
 * @brief Wait until LDM SPI ready
 * 
 * @return the status
 * @retval LDM_ERR_OK
 * @retval LDM_ERR_TIMEOUT
 */
UINT32 _LDMHW_WaitReady(void)
{
    UINT32 i;

    for (i=0; i<LDM_MAX_TIMEOUT; i++)
    {
       // if (u4IO32Read4B(LED_CON) & (1<<5))
       //     return LDM_ERR_OK;

		if(IO32ReadFldAlign(LED_CON,LED_IDLE)== SV_TRUE)
			 return LDM_ERR_OK;
		
    }
    return LDM_ERR_TIMEOUT;
}



void vDrvLDMHWInit(UINT8 u1Pinmux, UINT8 u1PanelType)
{
     
    PANEL_LOCAL_DIMMING_ATTRIBUTE_T *pParam;

	_fgLdmISR = SV_OFF;   //disable SW Mode interrupt

    if(u1PanelType >= LDM_PANEL_MAX)
    {
        Printf("[LDM]panel type is out of range\n");
	    return;
    }

	if(u1Pinmux >= SPI_PINMUX_MAX)
   	{
   	  Printf("[LDM]Pinmux option is out of range\n");
	  return;
   	}
    

    // Get Local Dimming information here  	
    if (PANEL_GetLocalDimmingParam(&pParam) == SV_FALSE)
    {
      Printf("panel not support SPI\n");
      // not support local dimming
      return;
    }        

   if(pParam != NULL)
   	{
   	   u4HBlockNum = pParam->u1BlockNumM;
	   u4VBlockNum = pParam->u1BlockNumN;

	   vDrvLDMHW_setControl(pParam);           //set operation control register
   	   vDrvLDMHW_setClock(pParam);            // set CKCFG param
       vDrvLDMHW_setVsync(pParam);           // set VSCFG param
       vDrvLDMHW_setCS(pParam);            //set CS param
       vDrvLDMHW_setFrameRate(pParam);      //set frame rate & Vsync latency
	
       vDrvLDMHW_setDataLen(pParam);       //set H&V data len
       vDrvLDMHW_setLSBMode(pParam);       //set LSB mode
       vDrvLDMHW_setRGBMode(pParam);      //set RGB mode
       vDrvLDMHW_setBITMode(pParam);      //set Bit mode
	   vDrvLDMHW_setLEDType(pParam);      //set LED type
   	}
   else
   	{  
   	   Printf("error:pParam is Null\n");
       return;
    }

   if(vDrvLDMHW_setPinmux(u1Pinmux) == SV_FALSE)	//pinmux initial
	{
	 Printf("Pinmux initial error!\n");
	 return;
	}

   switch(u1PanelType)
   	{
   	  case LDM_PANEL_AMS:
	  	vDrvLDMHW_LightAS3693();
	  	   break;
	  case LDM_PANEL_IWATT:
	  	vDrvLDMHW_iWattInit();
		   break;

	  case LDM_PANEL_LGD:
	  	vDrvLDMHW_LightLGD();
		   break;

	  case LDM_PANEL_SCT:
	  	vDrvLDMHW_LightSCT();
		   break;

	  default:
	  	   break;
   	}


	_fgLdmISR = SV_ON;  //enable SW Mode interrupt

    LOG(0,"[LDM]SPI initial is OK\n");
	
}


void vDrvLDMHW_TurnOffSPI(UINT8 u1PanelType)
{
   if(u1PanelType >= LDM_PANEL_MAX)
    {
        Printf("[LDM]SPI turn off error:panel type is out of range\n");
	    return;
    }

    switch(u1PanelType)
   	{
   	  case LDM_PANEL_AMS:
	  	vDrvLDMHW_TurnOffAS3693();
	  	   break;
	  case LDM_PANEL_IWATT:
	  	vDrvLDMHW_TurnOffiWatt();
		   break;

	  case LDM_PANEL_LGD:
	  	vDrvLDMHW_TurnOffLGD();
		   break;

	  case LDM_PANEL_SCT:
	  	vDrvLDMHW_TurnOffSCT();
		   break;

	  default:
	  	   break;
   	}
}





