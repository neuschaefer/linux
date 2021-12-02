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

/** @file ldm_iwatt.c
 *  ldm_iwatt.c provides local dimming control functions for iWatt\n
 *  device.
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "ldm_if.h"
#include "ldm_reg.h"
#include "drv_lcdim.h"
#include "nptv_debug.h"
//#include "hw_vdoin.h"
#include "x_hal_io.h"
#include "sv_const.h"


extern UINT8 _dimming_value[];
extern UINT32 u4HBlockNum;
extern UINT32 u4VBlockNum;
extern BOOL _fgLdmISR;


/*
*@ iWatt SPI  transfer function
*     send SPI data to iWatt 
*
* @param u2Command: read/write command send to iWatt
* @param u2Addr: iWatt register address 
* @param u4Len: send data length, no limitation
*/
void vDrvLDMHW_iWattTransfer(UINT16 u2Command, UINT16 u2Addr, UINT16* au1Data, UINT32 u4Len)
{
  
    UINT32 i;
	UINT8 u1cmdHibyte,u1cmdLobyte;
	UINT8 u1AddrHibyte,u1AddrLobyte;
	UINT8 u1DataHibyte,u1DataLobyte;

	vIO32WriteFldAlign(LED_NOB, u4Len+2, MDATA_LEN);    //reset  H  datalen
	vIO32WriteFldAlign(LED_NOB,  1, NDATA_LEN);         //reset  V  datalen

	u1cmdHibyte = (UINT8)(u2Command >>8);
	u1cmdLobyte = (UINT8)(u2Command & 0x00FF);
	
    u1AddrHibyte = (UINT8)(u2Addr >>8);
	u1AddrLobyte = (UINT8)(u2Addr & 0x00FF);

    vIO32Write4B(LED_WADR, 0);   // set WADR, 0x290
    vIO32Write4B(LED_DATA, u1cmdHibyte);
	vIO32Write4B(LED_DATA, u1cmdLobyte);
    vIO32Write4B(LED_DATA, u1AddrHibyte);
	vIO32Write4B(LED_DATA, u1AddrLobyte);
		
    for (i=0; i<u4Len; i++)
      {
            // write byte to SPI SRAM
          u1DataHibyte = (UINT8)((*au1Data)>>8);
		  u1DataLobyte = (UINT8)((*au1Data)& 0x00FF);
		  vIO32Write4B(LED_DATA, u1DataHibyte);
		  vIO32Write4B(LED_DATA, u1DataLobyte);
            
          au1Data++;
      }
		
     //vIO32Write4BMsk(LED_CON, 1, 1);   //trigger SW_TRIG, 0x2b8[0:0]=1
     vIO32WriteFldAlign(LED_CON, 1, SW_TRIG);  // trigger SW_TRIG, 0x2b8[0:0]=1    

    return;
}



void vDrvLDMHW_iWattInit(void)
{
  UINT8 i;
  static UINT16 Regtable[2+19];
  static UINT16 temp[2+32*2];
  	
  //iWatt One Time register setting
  Regtable[0]= 0x204;
  Regtable[1]= 0x00D;   //0x000 , current setting = 100mA(default 25mA)
  Regtable[2]= 0x300;
  Regtable[3]= 0x077;
  Regtable[4]= 0x16D;
  Regtable[5]= 0x039;

 _LDMHW_WaitReady();   //wait for SPI HW idle
  vDrvLDMHW_iWattTransfer(LDM_IWATT_WRITE, LDM_IWATT_ADDR1, Regtable, 6);  //iwatt One Time reg initial
 _LDMHW_WaitReady();

 //iWatt Use Mode register setting
 for(i=0; i<19; i++)
 	{
 	 Regtable[i]= 0;
 	}
  vDrvLDMHW_iWattTransfer(LDM_IWATT_WRITE, LDM_IWATT_ADDR2, Regtable, 19);  //iwatt User Mode reg initial

  _LDMHW_WaitReady();


  //light on iWatt PWM to max
    for(i=0; i<32; i++)
  	{
  	  temp[i*2]= 0x3FF;
	  temp[i*2+1]= temp[i*2];
  	}

   vDrvLDMHW_iWattTransfer(LDM_IWATT_WRITE, LDM_IWATT_CHANNEL1, temp, 32*2);
  
}


void vDrvLDMHW_TurnOffiWatt(void)
{
  UINT8 i;
  static UINT16 temp[2+32*2];

  _fgLdmISR = SV_OFF;   //disable SW Mode interrupt

  for(i=0; i<32; i++)
  	{
  	  temp[i*2]= 0x0000;
	  temp[i*2+1]= temp[i*2];
  	}

  vDrvLDMHW_iWattTransfer(LDM_IWATT_WRITE, LDM_IWATT_CHANNEL1, temp, 32*2);
  
}



void DRVCUST_LDMHW_DataGeniWatt(void)
{
 #ifndef CC_MTK_LOADER
  UINT32 i;
  UINT32 u4DataLen;
  static UINT16 temp[2+32*2];
  
  u4DataLen = u4HBlockNum * u4VBlockNum;          //total block number

  vDrvGetLcDimDimmingResult(_dimming_value); //move local dimming data to buffer
 
  for(i=0; i<u4DataLen; i++)
  	{
  	  temp[i*2]= ((UINT16)_dimming_value[i])<<2;
	  temp[i*2+1]= temp[i*2];
  	}

  vDrvLDMHW_iWattTransfer(LDM_IWATT_WRITE, LDM_IWATT_CHANNEL1, temp, 32*2);
 #endif

  return;

}


