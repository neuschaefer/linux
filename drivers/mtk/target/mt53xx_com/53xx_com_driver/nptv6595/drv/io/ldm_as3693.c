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

/** @file ldm_as3693.c
 *  ldm_as3693.c provides local dimming control functions for AS3693 \n
 *  device.
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "ldm_if.h"
#include "ldm_reg.h"
#include "x_bim.h"
#include "hw_sw.h"
#include "drv_lcdim.h"
#include "nptv_debug.h"
//#include "hw_vdoin.h"
#include "x_hal_io.h"
#include "sv_const.h"


extern UINT8 _dimming_value[];
UINT8 u1AS3693_Count;
extern UINT32 u4HBlockNum;
extern UINT32 u4VBlockNum;
extern BOOL _fgLdmISR;


/*
*@brief SPI ISR 
*/
static void _vDrvSPIISR(UINT16 u2Vector)
{
	UINT8 u1IntOn;
	//static UINT32 u4ISRCount = 0;
	
	ASSERT(u2Vector == VECTOR_LED);
	
	// Input Vsync Rising
	u1IntOn = IO32ReadFldAlign(LED_ISR_2, SPI_INTR);
	
	if (u1IntOn)
	{		 
		// Interrupt Happened, Routine Begin...
		//SPI Interrupt Routine Here
        if(u1AS3693_Count == 0)
			return;
		else if(u1AS3693_Count ==1)	
		 {
		   _LDMHW_AS3693SPI(LDM_AS3693_ADDR1, LDM_AS3693_CURREG1+16, _dimming_value+16, 16);
		 }
		else if(u1AS3693_Count ==2)
		 {
		   _LDMHW_AS3693SPI(LDM_AS3693_ADDR2, LDM_AS3693_CURREG1, _dimming_value+32, 16);
		 }
        else 
         {
           _LDMHW_AS3693SPI(LDM_AS3693_ADDR2, LDM_AS3693_CURREG1+16, _dimming_value+48, 16);
         }

		u1AS3693_Count ++ ;
		if(u1AS3693_Count%4 == 0)
		    u1AS3693_Count = 0;
		
		// Routine End...
		vIO32WriteFldAlign(LED_ISR_2, 0, SPI_INTR); // Routine Done, Clear ISR
	}
}

void vDrvSPIEnableISR(UINT8 bFlagEn)
{
    vIO32WriteFldAlign(LED_ISR_1, 1, SPI_INTR_EN);     // Enable SPI ISR
    LOG(7, "= SPI ISR Enable =\n");
}

static void _vDrvSPIInitISR(void)
{

    static UINT8 u1SPIIsrInitialized = SV_FALSE;
    x_os_isr_fct pfnOldIsr;

    if (!u1SPIIsrInitialized)
    {
        // Add Semaphore
        // VERIFY(x_sema_create(&_hSPIIsrSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
    
        if (x_reg_isr(VECTOR_LED, _vDrvSPIISR, &pfnOldIsr) != OSR_OK)     // Register ISR
        {
            LOG(7, "= SPI Register ISR Error! =\n");
        }
        else
        {
            LOG(7, "= SPI Register ISR OK! =\n");
        }
    
        u1SPIIsrInitialized = SV_TRUE;

        vDrvSPIEnableISR(SV_ON);        
    }
}


void vDrvLDMHW_LightAS3693(void)
{
    static UINT8 au1cmd[2+16*2];
    UINT8 i;

    // set dimming value;
    for (i=0; i<16; i++)
    {
        au1cmd[i*2] = 0xff;
        au1cmd[i*2+1] = 0x00;
    }
	_LDMHW_WaitReady();
    _LDMHW_AS3693SPI(LDM_AS3693_ADDR1, LDM_AS3693_CURREG1, au1cmd, 32);
	_LDMHW_WaitReady();
    _LDMHW_AS3693SPI(LDM_AS3693_ADDR2, LDM_AS3693_CURREG1, au1cmd, 32);
    _LDMHW_WaitReady();

    // light on AS3693 PWM
    au1cmd[0] = 0xff;
    au1cmd[1] = 0xff;
    _LDMHW_AS3693SPI(LDM_AS3693_ADDR1, LDM_AS3693_CONREG1, au1cmd, 2);
	_LDMHW_WaitReady();
    _LDMHW_AS3693SPI(LDM_AS3693_ADDR2, LDM_AS3693_CONREG1, au1cmd, 2);
    _LDMHW_WaitReady();

	_vDrvSPIInitISR();     //initial SPI ISR
	
}


void vDrvLDMHW_TurnOffAS3693(void)
{
    static UINT8 au1cmd[2+16*2];
    UINT8 i;

	_fgLdmISR = SV_OFF;   //disable SW Mode interrupt

    // set dimming value;
    for (i=0; i<16; i++)
    {
        au1cmd[i*2] = 0x00;
        au1cmd[i*2+1] = 0x00;
    }
    _LDMHW_WaitReady();    
    _LDMHW_AS3693SPI(LDM_AS3693_ADDR1, LDM_AS3693_CURREG1, au1cmd, 32);
    _LDMHW_WaitReady();
	_LDMHW_AS3693SPI(LDM_AS3693_ADDR2, LDM_AS3693_CURREG1, au1cmd, 32);
    _LDMHW_WaitReady();

	//turn off AS3693 PWM
    au1cmd[0] = 0x00;
    au1cmd[1] = 0x00;

    _LDMHW_AS3693SPI(LDM_AS3693_ADDR1, LDM_AS3693_CONREG1, au1cmd, 2);
    _LDMHW_WaitReady();
	_LDMHW_AS3693SPI(LDM_AS3693_ADDR2, LDM_AS3693_CONREG1, au1cmd, 2);
    
}



/** 
 * @brief Send SPI command to AS3693
 *      The timing setting must be initialized.
 * 
 * @param au1Data the data sent to AS3693
 * @param u4Len the length of data , must satisfy: u4Len <= AS3693_MAXLEN
 * @return the status
 */
void _LDMHW_AS3693SPI(UINT8 u1Addr, UINT8 u1Reg, UINT8* au1Data, UINT32 u4Len)
{
    static UINT32 AS3693_MAXLEN=32;
    
    UINT32 i;
	
	if(u4Len > AS3693_MAXLEN)
	  {
	     LOG(0, "Error:AS3693 datalen exceed range\n");
		 return;
	  }
  
	  vIO32WriteFldAlign(LED_NOB, u4Len+2, MDATA_LEN);    //reset H&V datalen
	  vIO32WriteFldAlign(LED_NOB,  1, NDATA_LEN);  

      vIO32Write4B(LED_WADR, 0);   // set WADR, 0x290
      vIO32Write4B(LED_DATA, u1Addr);
      vIO32Write4B(LED_DATA, (u1Reg<<1));
      for (i=0; i<u4Len; i++)
        {
            // write byte to SPI SRAM, 0x298
            vIO32Write4B(LED_DATA, (UINT32)(*au1Data));
            au1Data++;
        }
	  
      //vIO32Write4BMsk(LED_CON, 1, 1);   // trigger SW_TRIG, 0x2b8
      vIO32WriteFldAlign(LED_CON, 1, SW_TRIG);  // trigger SW_TRIG, 0x2b8[0:0]=1    
		
    return;  
    
}



void DRVCUST_LDMHW_DataGenAS3693(void)
{
 #ifndef CC_MTK_LOADER 
	 UINT32 i;
	 UINT32 u4DataLen;
	
	 static UINT8 buffer_value[256*2];
	  
	 u4DataLen = u4HBlockNum * u4VBlockNum;     //total block number from panel table
	
	 vDrvGetLcDimDimmingResult(buffer_value); //move local dimming data to buffer_dimming_value[512]
    
	for(i=0; i<u4DataLen; i++)
	  {
		 _dimming_value[i*2] = buffer_value[i];
		 _dimming_value[i*2+1] = 0;
		   	 
	   }

	 if(u1AS3693_Count != 0)
	 	u1AS3693_Count = 0;

	   _LDMHW_AS3693SPI(LDM_AS3693_ADDR1, LDM_AS3693_CURREG1, _dimming_value, 16);   //send 1st AS3693 data
	   u1AS3693_Count++;

 #endif   
	 return;
	 
}


