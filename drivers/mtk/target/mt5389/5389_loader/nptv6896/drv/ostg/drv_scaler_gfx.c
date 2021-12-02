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
 * $RCSfile: drv_scaler_gfx.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file drv_scaler_nonlinear.c
 *  Brief of file drv_scaler_nonlinear.c.
 *  Details of file drv_scaler_nonlinear.c (optional).
 */

//-----------------------------------------------------------------------------
// Include files
//----------------------------------------------------------------------------- 
#include "scpos_debug.h"
#include "hw_scpos.h"
#include "drv_scaler_gfx.h"
#include "sv_const.h"  
#include "vdo_misc.h"
#include "drv_scaler_drvif.h"


R3DGfxInf _arGfxInf=
{
	0xff,	//	UINT8 u1PathIn;
	0xff,	//	UINT8 u1Sel444;
	0xff,	//	UINT8 u1FrameNumber;
	0xff,	//	UINT8 u1WriteMode;
	0,		//	UINT8 u1WriteEnable;
	0,		//	UINT8 u13D22DEnable;

	0xffffffff,	//	UINT32 u4BassAddr0;
	0xffffffff,	//	UINT32 u4BassAddrOffset;
	0xffffffff,	//	UINT32 u4WAddrHight;
	0xffffffff,	//	UINT32 u4WAddrLow;

	FORMAT_NONE,	//	SCPIP_GFX_FORMAT Format;
	0xff,		//	UINT8 u1AlphaPos;
	0x7f,		//	UINT8 u1AlphaValue;
	
	0,			//	UINT8 u1HBoundEn;
	0xffff,	//	UINT32 u2HBoundValue;
	
	0,			//	UINT8 u1FrameDropEn;
	0xff,	//	UINT32 u4DropM;
	0xff,	//	UINT32 u4DropN;

	1,		//	UINT8 u1MuxB;
	0,		//	UINT8 u1MuxG;
	2,		//	UINT8 u1MuxR;
};



/**
 * @brief Graphic dump initial function.
 * @param void
 * @return void
*/
void  vScpip_GFX_init(void)
{
	vRegWriteFldAlign(SCPIP_SYSTEM_17, 1, SYSTEM_17_COLOR_3X3_IN_EN);
}

/**
 * @brief select witch path write gfx
 * @param bPath 0:main, 1:sub
 * @return true
 */
UINT8 u1Scpip_GFX_Source_Sel(UINT8 bPath)
{	
	vRegWriteFldAlign(SCPIP_SYSTEM_17,bPath,SYSTEM_17_COLOR_3X3_IN_3DGFX_SOURCE_SEL);
	_arGfxInf.u1PathIn = bPath;
	return TRUE;
}

/**
 * @brief Get current write frame index
 * @param void
 * @return 0~(N-1), N is frame number.
 */
UINT8 u1Scpip_GFX_GetCurrentFrameIdx(void)
{
	return ((UINT8)(RegReadFldAlign(SCPIP_DRAM_M_03, DRAM_M_03_STA_GFX_WR_FRAME_CNT)));
}

/**
 * @brief Get current write status
 * @param void
 * @return 0: stop writing, 1: still writing
 */
UINT8 u1Scpip_GFX_GetWriteStatus(void)
{
	return ((UINT8)(RegReadFldAlign(SCPIP_DRAM_M_14, DRAM_M_14_STA_GFX_WRITE_EN)));
}


/**
 * @brief set frame number
 * @param frame number
 * @return true:OK, FLASE: frame number limited to 4.
 */
UINT8 u1Scpip_GFX_Set_Frame_Num(UINT8 u1Number)
{
	if(u1Number>4)
	{
		return FALSE;
	}
	else
	{
		vRegWriteFldAlign(SCPIP_DRAM_M_14,u1Number-1,DRAM_M_14_DA_GFX_FRAME_NUM);
		_arGfxInf.u1FrameNumber = u1Number;
		return TRUE;
	}
}

/**
 * @brief set address
 * @param fu4BaseAddr 
  * @param u4Size means one frame size 
 * @return true
 */
UINT8 u1Scpip_GFX_Set_Dram_Address(UINT32 u4BaseAddr, UINT32 u4FrameSize, UINT8 u1FrameNum)
{
	UINT8 u1Idx=0;
	
	u4BaseAddr >>= 4;
	u4FrameSize >>=4;

	if(u1FrameNum>4)
	{
		u1FrameNum=4;  // maximum 4 frames.
	}

	_arGfxInf.u4BassAddr0 = u4BaseAddr;
	_arGfxInf.u4BassAddrOffset = u4FrameSize;
	_arGfxInf.u1FrameNumber = u1FrameNum;

	// update frame number.
	vRegWriteFldAlign(SCPIP_DRAM_M_14,u1FrameNum-1,DRAM_M_14_DA_GFX_FRAME_NUM);

	// update frame addresses
	for(u1Idx=0; u1Idx<u1FrameNum; u1Idx++)
	{
		vRegWriteFldAlign(SCPIP_DRAM_M_0D+(0x4*u1Idx), u4BaseAddr+(u4FrameSize*u1Idx), DRAM_M_0D_DA_GFX_ADDR_BASE_0);
	}

	// Set Graphic dump dram proteciton region.
	_arGfxInf.u4WAddrLow = u4BaseAddr;
	_arGfxInf.u4WAddrHight = (u4BaseAddr + (u1FrameNum * u4FrameSize));
	vRegWriteFldAlign(SCPIP_DRAM_M_12,_arGfxInf.u4WAddrLow, DRAM_M_12_DA_GFX_WADDR_LO_LIMIT);
	vRegWriteFldAlign(SCPIP_DRAM_M_11,_arGfxInf.u4WAddrHight,DRAM_M_11_DA_GFX_WADDR_HI_LIMIT);

	return TRUE;
}


/**
 * @brief select trigger mode or flag mode
 * @param 0 continue mode ,  1 trigger mode
 * @return true
 */
UINT8 u1Scpip_GFX_Set_Write_Mode(UINT8 u1Select)
{
	vRegWriteFldAlign(SCPIP_DRAM_M_14,u1Select,DRAM_M_14_DA_GFX_WRITE_EN_MODE);
	_arGfxInf.u1WriteMode = u1Select;
	return TRUE;
}

/**
 * @brief trigger 0->1
 * @return true
 */
UINT8 u1Scpip_GFX_Write_Trigger(void)
{
	UINT8 u1Mode;
	u1Mode = RegReadFldAlign(SCPIP_DRAM_M_14, DRAM_M_14_DA_GFX_WRITE_EN_MODE);
	if(u1Mode)
	{
		vRegWriteFldAlign(SCPIP_DRAM_M_14,0,DRAM_M_14_DA_GFX_WRITE_TRIGGER);
		vRegWriteFldAlign(SCPIP_DRAM_M_14,1,DRAM_M_14_DA_GFX_WRITE_TRIGGER);
		return TRUE;
	}
	else
	{	
		LOG(4,"warning: GFX not trigger mode \n");
		return FALSE;
	}
}

/**
 * @brief enable 3DGFX write
 * @param on/off
 * @return true
 */

UINT8 u1Scpip_GFX_Write_Enable(UINT8 u1Enable)
{
	if(u1Enable==SV_ON)
	{
		RDispPrm* dispPrm = getDispPrm(_arGfxInf.u1PathIn);
		
		_arGfxInf.u1Sel444 = (bIsScalerInput444(_arGfxInf.u1PathIn) == SV_TRUE)? 1 :0;  
		_arGfxInf.u1HBoundEn = !(_arGfxInf.u1Sel444);
		_arGfxInf.u2HBoundValue = dispPrm->u4HBound;

              vRegWriteFldAlign(SCPIP_SYSTEM_17, !(_arGfxInf.u1Sel444) ,SYSTEM_17_COLOR_3X3_IN_EN);
		vRegWriteFldAlign(SCPIP_SYSTEM_17, _arGfxInf.u1Sel444 ,SYSTEM_17_SEL444_3X3);
		vRegWriteFldAlign(SCPIP_SYSTEM_17,_arGfxInf.u1HBoundEn ,SYSTEM_17_REG_H_BOUND_EN_3X3);
		vRegWriteFldAlign(SCPIP_SYSTEM_17,_arGfxInf.u2HBoundValue,SYSTEM_17_REG_H_BOUND_3X3);
	}

	vRegWriteFldAlign(SCPIP_DRAM_M_14,u1Enable,DRAM_M_14_DA_GFX_WRITE_ENABLE);
	_arGfxInf.u1WriteEnable = u1Enable;
	return TRUE;
}


/**
 * @brief set  3d to 2d enable 
 * @param path 
 * @param enable
 * @return true
 */
UINT8 u1Scpip_GFX_Set_3DTo2D_En(UINT8 u1Enable)
{
	vRegWriteFldAlign(SCPIP_DRAM_M_13,u1Enable,DRAM_M_13_DA_GFX_3D_TO_2D_EN);
	_arGfxInf.u13D22DEnable = u1Enable;
	return TRUE;
}

/**
 * @brief set  3d to 2d enable 
 * @param path 
 * @param enable
 * @param u4DropM  u4DropN  ,deside drop ratio
 * @return true
 */
UINT8 u1Scpip_GFX_Set_Frame_Drop(UINT8 u1Enable,UINT8 u1DropN, UINT8 u1DropM)
{
	vRegWriteFldAlign(SCPIP_DRAM_M_04,u1Enable,DRAM_M_04_DA_GFX_FRAME_DROP_EN);
	vRegWriteFldAlign(SCPIP_DRAM_M_04,u1DropM,DRAM_M_04_DA_FRAME_DROP_M);
	vRegWriteFldAlign(SCPIP_DRAM_M_04,u1DropN,DRAM_M_04_DA_FRAME_DROP_N);

	_arGfxInf.u1FrameDropEn = u1Enable;
	_arGfxInf.u1DropM = u1DropM;
	_arGfxInf.u1DropN = u1DropN;
	return TRUE;
}


/**
 * @brief set date format 
 * @param u1Mode mode select
 * @param u1AlphaPos alpha positon ,ARGB,RGBA
 * @param u1Alpha  set alpha value 
 * @return true
 */
UINT8 u1Scpip_GFX_Set_Alpha(UINT8 u1Alpha)
{
	vRegWriteFldAlign(SCPIP_DRAM_M_14,u1Alpha,DRAM_M_14_DA_GFX_ALPHA);
	_arGfxInf.u1AlphaValue = u1Alpha;
	return TRUE;
}

/**
 * @brief set  dump format
 * @param Format ,ARGB8888 / RGB565
 * @return true
 */
UINT8 u1Scpip_GFX_Set_Dump_Format(SCPIP_GFX_FORMAT Format)
{
	switch(Format) 
	{
		case RGBA8888:
			vRegWriteFldAlign(SCPIP_DRAM_M_14,1,DRAM_M_14_DA_GFX_MODE);
			vRegWriteFldAlign(SCPIP_DRAM_M_14,0,DRAM_M_14_DA_GFX_ALPHA_POS_SEL);
			vRegWriteFldAlign(SCPIP_DRAM_M_14,2,DRAM_M_14_DA_GFX_MUX_B);
			vRegWriteFldAlign(SCPIP_DRAM_M_14,1,DRAM_M_14_DA_GFX_MUX_R);
			_arGfxInf.Format = RGBA8888;
			_arGfxInf.u1AlphaPos = 0;  //Scaler format( 0:ARGB , 1:RGBA) different order with OpenGL.
			_arGfxInf.u1MuxB = 2;
			_arGfxInf.u1MuxR = 1;
			
			break;
		case RGB565:
			vRegWriteFldAlign(SCPIP_DRAM_M_14,2,DRAM_M_14_DA_GFX_MODE);
			vRegWriteFldAlign(SCPIP_DRAM_M_14,1,DRAM_M_14_DA_GFX_MUX_B);
			vRegWriteFldAlign(SCPIP_DRAM_M_14,2,DRAM_M_14_DA_GFX_MUX_R);
			_arGfxInf.Format = RGB565;
			_arGfxInf.u1MuxB = 1;
			_arGfxInf.u1MuxR = 2;
			break;
		default:
			break;
	}
	return TRUE;
}

/**
 * @brief query 3DGFX setting for cli cmd
 * @param bPath
 * @return true
 */
void vScpip_GFX_Query(void)
{
	//LOG(level,fmt...)
	
	LOG(2,"path(%d),WEnable(%d),WMode(%d),3Dto2D(%d)\n",_arGfxInf.u1PathIn,_arGfxInf.u1WriteEnable,_arGfxInf.u1WriteMode,_arGfxInf.u13D22DEnable);
	LOG(2,"FrameNumber(%d),bassaddr(0x%x),offset(0x%x),addrlimit(0x%x,0x%x)\n",_arGfxInf.u1FrameNumber,_arGfxInf.u4BassAddr0,_arGfxInf.u4BassAddrOffset,_arGfxInf.u4WAddrLow,_arGfxInf.u4WAddrHight);
	LOG(2,"boundary:en(%d),value(%d)\n",_arGfxInf.u1HBoundEn,_arGfxInf.u2HBoundValue);
	LOG(2,"Format:format(%d),alphapos(%d),alphavalue(%d)\n",_arGfxInf.Format,_arGfxInf.u1AlphaPos,_arGfxInf.u1AlphaValue);
	LOG(2,"Frame drop: en(%d),M(%d),N(%d)\n",_arGfxInf.u1FrameDropEn,_arGfxInf.u1DropM,_arGfxInf.u1DropN);
	LOG(2,"R-G-B:(%d,%d,%d)\n",_arGfxInf.u1MuxR,_arGfxInf.u1MuxG,_arGfxInf.u1MuxB);
}


