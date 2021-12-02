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
 * $Date  $
 * $RCSfile: api_eep.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/
#define _API_EEP_C_


#include "general.h"
#include "eeprom_if.h"
#include "eepdef.h"


//#define SKIP_NPTV_EEP //only for IC comeback test

void vApiEepromSet(UINT16 wPrm, UINT8 bVal, UINT16 wLen) ;
UINT8 bApiEepromReadByte(UINT16 wAddr) ;
BOOL fgApiEepromWriteByte(UINT16 wAddr, UINT8 bData) ;
void vApiEepromDmaRead(UINT16 wPrm, void *pDst, UINT16 wLen) ;
void vApiEepromDmaWrite(UINT16 wPrm, void *pSrc, UINT16 wLen) ;


void vApiEepromSet(UINT16 wPrm, UINT8 bVal, UINT16 wLen) 
{
	UNUSED(wPrm);
	UNUSED(bVal);
	UNUSED(wLen);
  return;
}

UINT8 bApiEepromReadByte(UINT16 wAddr) 
{

#ifdef SKIP_NPTV_EEP
  return 0;
#else
 UINT8 u1Data=0;
 INT32 i4Ret;

  i4Ret = EEPNPTV_Read((UINT64)wAddr, (UINT32)&u1Data, 1);
  if(i4Ret)
  {
    Printf("EEP ReadByte Error\n");
    return (0);
  }

  return (u1Data);
#endif
}

BOOL fgApiEepromWriteByte(UINT16 wAddr, UINT8 bData) 
{  
#ifdef SKIP_NPTV_EEP
  return 0;
#else
  INT32 i4Ret;
  i4Ret = EEPNPTV_Write((UINT64)wAddr, (UINT32)&bData, 1);

  if(i4Ret)
  {
    return (FALSE);
  }
  else
  {
    return (TRUE);
  }
#endif
}

void vApiEepromDmaRead(UINT16 wPrm, void *pDst, UINT16 wLen) 
{
#ifdef SKIP_NPTV_EEP
  return;
#else
  UNUSED(EEPNPTV_Read((UINT64)wPrm, (UINT32)pDst, wLen));
#endif
}

void vApiEepromDmaWrite(UINT16 wPrm, void *pSrc, UINT16 wLen) 
{
#ifdef SKIP_NPTV_EEP
  return;
#else
   UNUSED(EEPNPTV_Write((UINT64)wPrm, (UINT32)pSrc, wLen));
#endif
}


//===================================================================
// EEPROM Magic and Version
//===================================================================
#ifndef CC_DISABLE_EEP_MAG_CHK    
#define EEPROM_MAGIC0     'M'
#define EEPROM_MAGIC1     'K'
#define EEPROM_VERSION0   (UINT8)(FW_VERSION)			// FW version
#define EEPROM_VERSION1   (UINT8)(FW_VERSION>>8)	// FW version

UINT8 code _pbEepromMagic[4] =
{
	EEPROM_MAGIC0,
	EEPROM_MAGIC1,
	EEPROM_VERSION0,
	EEPROM_VERSION1
};

BOOL fgApiIsEepromMagicOK(void)
{
	UINT8 bData, bCnt;

	bData = 0;
	for (bCnt = 0; bCnt < sizeof(_pbEepromMagic); bCnt++)
	{
		bData = bApiEepromReadByte(EEP_MAGIC0+bCnt);
		if (bData != (UINT8) (_pbEepromMagic[bCnt]))
		{
			return (FALSE);
		}
	}

	return (TRUE);
}


void  vApiEepromDefaultInit(void)
{
		UINT8 i;

		printf("Load EEPROM default value...\n");

		for (i = 0; i < sizeof(_pbEepromMagic); i++)				//Write Magic string in EEPROM
		{
			fgApiEepromWriteByte(EEP_MAGIC0+i, _pbEepromMagic[i]) ;
		}
#ifdef SUPPORT_VGA_UI_AMBIGUOUS_TIMING_SELECT
		//Set default value for ambiguous VGA group 0~9
		for(i=EEP_VGA_AMBIGUOUS_DEFAULT_START;i<=EEP_VGA_AMBIGUOUS_DEFAULT_END;i++)
		{
			fgApiEepromWriteByte(i, 1);
		}
#endif		
		//TVD clean auto calibration flag
              fgApiEepromWriteByte(EEP_VIDEO_AUTO_CALIB_TV_DONE,0);
              fgApiEepromWriteByte(EEP_VIDEO_AUTO_CALIB_AV_DONE,0);
}
#endif

