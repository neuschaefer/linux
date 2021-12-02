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
 * $Author: hongjun.chu $
 * $Date: 2012/05/03 $
 * $RCSfile: drv_tcon.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/
#include "sv_const.h"
#include "feature.h"

#include "hw_tg.h"

#include "sif_if.h"

#include "drv_tcon.h"

#ifdef DEFINE_IS_LOG
#undef DEFINE_IS_LOG
#endif
#ifdef CC_MTK_LOADER
#define DEFINE_IS_LOG(level, fmt...)   Printf(fmt)
#else /* CC_MTK_LOADER */
#define DEFINE_IS_LOG   IR_IsLog
#endif /* CC_MTK_LOADER */

// for gamma IC
#define GAMMAIC_BUS 0
#define GAMMAIC_CLK_RATE 0x100
#define GAMMAIC_ID (0x74 << 1)
#define GAMMAIC_DATA_CNT 2

// Gamma
struct sTCONREGTABLE sRegCustGamma[] = {
  // Gamma voltage
  0x0000, 0x6B03, 0xffffffff,
  0x0001, 0xEB03, 0xffffffff,
  0x0002, 0x6B03, 0xffffffff,
  0x0003, 0x3E03, 0xffffffff,
  0x0004, 0xFD02, 0xffffffff,
  0x0005, 0xB602, 0xffffffff,
  0x0006, 0x8402, 0xffffffff,
  0x0007, 0x0902, 0xffffffff,
  0x0008, 0xEF01, 0xffffffff,
  0x0009, 0x7601, 0xffffffff,
  0x000A, 0x4201, 0xffffffff,
  0x000B, 0xFB00, 0xffffffff,
  0x000C, 0xB900, 0xffffffff,
  0x000D, 0x8E00, 0xffffffff,
  0x000E, 0x1300, 0xffffffff,
  0x000F, 0x8E00, 0xffffffff,
  0x0012, 0xB581, 0xffffffff,
};

struct sTCONREGTABLE sRegCustTg[] = {
// LGDV4

  // SOE, TG0
  0xf0034820, 0x00C807d0, 0xffffffff,
  0xf0034824, 0x00000000, 0xffffffff,
  0xf0034828, 0x00C807d0, 0xffffffff,  
  0xf003482c, 0x04430007, 0xffffffff,
  0xf0034830, 0x20000000, 0xffffffff,
  0xf0034834, 0x80000000, 0xffffffff,
  0xf0034838, 0x00000007, 0xffffffff,
  0xf003483c, 0x00000000, 0xffffffff,

  // POL, TG1
  0xf0034840, 0x07080708, 0xffffffff,
  0xf0034844, 0x00000000, 0xffffffff,
  0xf0034848, 0x07080708, 0xffffffff,  
  0xf003484c, 0x04430007, 0xffffffff,
  0xf0034850, 0x80000404, 0xffffffff,
  0xf0034854, 0x80020004, 0xffffffff,
  0xf0034858, 0x00000007, 0xffffffff,
  0xf003485c, 0x10000000, 0xffffffff,

  // GSP, TG11
  0xf0034980, 0x04060406, 0xffffffff,
  0xf0034984, 0x00000000, 0xffffffff,
  0xf0034988, 0x00000000, 0xffffffff,
  0xf003498c, 0x00080007, 0xffffffff,
  0xf0034990, 0x00000000, 0xffffffff,
  0xf0034994, 0x00000000, 0xffffffff,
  0xf0034998, 0x00000004, 0xffffffff,
  0xf003499c, 0x00000000, 0xffffffff,

  // GSP_R, TG8
  0xf0034920, 0x04060406, 0xffffffff,
  0xf0034924, 0x00000000, 0xffffffff,
  0xf0034928, 0x00000000, 0xffffffff,
  0xf003492c, 0x00080007, 0xffffffff,
  0xf0034930, 0x00000000, 0xffffffff,
  0xf0034934, 0x00000000, 0xffffffff,
  0xf0034938, 0x00000004, 0xffffffff,
  0xf003493c, 0x00000000, 0xffffffff,

  // GOE, TG2
  0xf0034860, 0x07BC062C, 0xffffffff,
  0xf0034864, 0x00000000, 0xffffffff,
  0xf0034868, 0x07BC062C, 0xffffffff,
  0xf003486c, 0x04420006, 0xffffffff,
  0xf0034870, 0x20000000, 0xffffffff,
  0xf0034874, 0x80000000, 0xffffffff,
  0xf0034878, 0x00000007, 0xffffffff,
  0xf003487c, 0x00000000, 0xffffffff,
  
  // GSC, TG12
  0xf00349a0, 0x07260406, 0xffffffff,
  0xf00349a4, 0x00000000, 0xffffffff,
  0xf00349a8, 0x07260406, 0xffffffff,
  0xf00349ac, 0x04420006, 0xffffffff,
  0xf00349b0, 0x20000000, 0xffffffff,
  0xf00349b4, 0x80000000, 0xffffffff,
  0xf00349b8, 0x00000107, 0xffffffff,
  0xf00349bc, 0x00000000, 0xffffffff,
   
  // FLK, TG9  
  0xf0034940, 0x049C0726, 0xffffffff,
  0xf0034944, 0x00000000, 0xffffffff,
  0xf0034948, 0x049C0726, 0xffffffff,
  0xf003494c, 0x04420006, 0xffffffff,
  0xf0034950, 0x20000000, 0xffffffff,
  0xf0034954, 0x80000000, 0xffffffff,
  0xf0034958, 0x00000007, 0xffffffff,
  0xf003495c, 0x00000000, 0xffffffff,
  
  // DPM, TG10
  0xf0034978, 0x00000030, 0xffffffff,
  
  // H_CONV, TG5
  0xf00348d8, 0x00000020, 0xffffffff,

  // OPT_N, TG6
  0xf00348f8, 0x00000030, 0xffffffff,
  
  // 0PT_P , TG7
  0xf0034918, 0x00000020, 0xffffffff,
};

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
static UINT32 _u4TconCustGammaWrite(UINT8 u1offset, UINT8 *pu1Buf, UINT8 u1Count)
{
	Printf("0x%8x, 0x%8x, 0xffffffff,\n", u1offset, (pu1Buf[0]<<8) | pu1Buf[1]);
	return (SIF_Write(GAMMAIC_CLK_RATE, GAMMAIC_ID, u1offset, (UINT8 *)pu1Buf, GAMMAIC_DATA_CNT));
}

#if 0 // for warning free
static UINT32 _u4TconCustGammaRead(UINT8 u1offset, UINT8 *pu1Buf, UINT8 u1Count)
{
	return (SIF_Read(GAMMAIC_CLK_RATE, GAMMAIC_ID, u1offset, (UINT8 *)pu1Buf, GAMMAIC_DATA_CNT));
}
#endif

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

UINT8 u1TconCustDPMCtrlEN1(UINT8 val)
{
  vIO32WriteFldAlign(TIMAR6, 1, TIMCR6_TCGPOEN);
	
  if (val)
  {
      vIO32WriteFldAlign(TIMAR6, 1, TIMCR6_TCGPO);
  }
  else
  {
      vIO32WriteFldAlign(TIMAR6, 0, TIMCR6_TCGPO);
  }
  
  return(1);
}

UINT8 u1TconCustDPMCtrlEN2(UINT8 val)
{
    if (val)
    {
        // TODO
    }
    else
    {
        // TODO
    }

    return(1);
}

// Gamma
UINT8 u1TconCustGammaInit(void)
{
	UINT8 u1Err = 0;
	UINT8 i = 0;
	UINT8 u1count;
	UINT8 u1tmp[2];
	UINT16 u2GammaVoltage = 0;

	Printf("Tcon Gamma init\n");

	u1count = (sizeof(sRegCustGamma) / sizeof(struct sTCONREGTABLE));

	for ( i = 0 ; i < u1count ; i++)
    {
                //#if  HISENSE_IPS_2010_PANEL 
      		//u1tmp[1] = (UINT8)((sRegCustGamma[i].u4Val) >> 8);
		//u1tmp[0] = (UINT8)((sRegCustGamma[i].u4Val) & 0xff);
		u2GammaVoltage = (((sRegCustGamma[i].u4Val)&0x03FF)|0x4000);
		u1tmp[0] = (UINT8)(u2GammaVoltage >> 8);
		u1tmp[1] = (UINT8)(u2GammaVoltage & 0xff);
		if (_u4TconCustGammaWrite(sRegCustGamma[i].u4Addr, u1tmp, 2) == 0)
		{
		
			u1Err = 1;
			Printf("!!!Tcon Gamma write fail %d, %d, %d \n", \
				              sRegCustGamma[i].u4Addr, \
				              u1tmp[0], \
				              u1tmp[1]);			
		}
		else
		{
    		Printf("Tcon Gamma write ok %d , %d , %d \n", \
                              sRegCustGamma[i].u4Addr, \
                              u1tmp[0], \
                              u1tmp[1]);
		}
    }

    // check if init ok
	if (u1Err == 0)
	{
		Printf("Program Tcon Gamma dacs %d counts, %d / %d\n", i, sizeof(sRegCustGamma), sizeof(struct sTCONREGTABLE));
	}
	else
	{
	    Printf("Program Tcon Gamma dacs fail !!!\n");
	}
	
	return(1);
}

// program gamma value into non-volatile memory
UINT8  u1TconCustGammaFlashing(void)
{
	UINT8 u1Err = 0;
	UINT8 i = 0;
	UINT8 u1count;
	UINT8 u1tmp[2];
	UINT8 u1Ret = 0;
	UINT16 u2GammaVoltage = 0;

#ifdef TCON_DBG_MSG_EN
   	printf("Tcon Gamma init\n");
#endif
	u1count = (sizeof(sRegCustGamma) / sizeof(struct sTCONREGTABLE));

	for ( i = 0 ; i < u1count ; i++)
    {
    
		//when write non-voltial memory  D15 D14 must be 0 1
		u2GammaVoltage = (((sRegCustGamma[i].u4Val)&0x03FF)|0x4000);
		u1tmp[0] = (UINT8)((u2GammaVoltage >> 8) );  // program gamma into non-voilate
		u1tmp[1] = (UINT8)(u2GammaVoltage & 0xff);

		if (_u4TconCustGammaWrite(sRegCustGamma[i].u4Addr, u1tmp, 2) == 0)
		{
		
			u1Err = 1;
#ifdef TCON_DBG_MSG_EN
 		  	printf("!!!Tcon Gamma write fail 0x%x, 0x%x, 0x%x\n",\
				              sRegCustGamma[i].u4Addr,\
				              u1tmp[0],\
				              u1tmp[1]);
#endif
		}
		else
		{
#ifdef TCON_DBG_MSG_EN
		   	printf("Tcon Gamma write ok 0x%x, 0x%x, 0x%x\n", \
                              sRegCustGamma[i].u4Addr, \
                              u1tmp[0], \
                              u1tmp[1]);
#endif
		}
    }

    // check if init ok
	if (u1Err == 0)
	{
#ifdef TCON_DBG_MSG_EN
    	printf("Program Tcon Gamma dacs %d counts, %d / %d\n", i, sizeof(sRegCustGamma), sizeof(struct sTCONREGTABLE));
#endif
	}
	else
	{
#ifdef TCON_DBG_MSG_EN
   	   printf("Program Tcon Gamma dacs fail !!!\n");
#endif
	}

    return (u1Ret);
}

UINT8 u1TconCustTgInit(void)
{
	UINT8 i = 0;
	UINT8 u1count;
	UINT32 u4tmp, u4tmp1;

	LOG(1,"cust tg init\n");

	u1count = (sizeof(sRegCustTg) / sizeof(struct sTCONREGTABLE));

	for ( i = 0 ; i < u1count ; i++)
    {
        u4tmp = sRegCustTg[i].u4Addr;
		u4tmp1 = (u4IO32Read4B(u4tmp) & ~(sRegCustTg[i].u4Msk)) | (sRegCustTg[i].u4Val);

		vIO32Write4B(u4tmp,u4tmp1);
    }

  	LOG(1,"Tcon reg flashing %d counts, %d / %d\n", i, sizeof(sRegCustTg), sizeof(struct sTCONREGTABLE));

  return(1);
}

UINT8 u1TconCustMlvdsInit(void)
{
  return(0);
}
