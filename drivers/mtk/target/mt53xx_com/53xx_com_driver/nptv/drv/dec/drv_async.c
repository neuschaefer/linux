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
 * $RCSfile: drv_async.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

#define _DRV_ASYNC_C_

#ifdef CC_UP8032_ATV
#include "general_mt82.h"
#endif
#include "general.h"
#include "hw_vdoin.h"
#include "hw_async.h"
#include "drv_async.h"
#include "vga_table.h"
#include "util.h"
#include "drv_vga.h"
#include "video_def.h"
#include "drv_hdtv.h"
#include "drv_autocolor.h"
#include "vga_auto.h"
#ifndef CC_COPLAT_MT82
#include "x_os.h"
#endif
#ifdef CC_COPLAT_MT82
#include "video_timing.h"
#endif
#if MDDI_FIELD_INV_WA
#include "vdo_misc.h"
#include "drv_di.h"
#endif

// SP0
UINT16   wSP0Hclk;
UINT8   bSP0Vclk;
UINT16   wSP0Vtotal;
UINT16  _wSP0StableVtotal;
UINT16   wSP0HLength;
UINT8   fgSP0Hpol;
UINT8   fgSP0Vpol;
UINT8	_bSP0Flag;

UINT16   wSP0VCompare[4];
UINT16   wSP0HCompare1;
UINT8   bSP0VCount;
UINT32 wVGAADSpec;
UINT32 wHFHeight;
UINT32 wHFLow;
UINT32 wVFHeight;
UINT32 wVFLow;

#if ADAPTIVE_MONITOR_SLICER_MEASURE
SP2MonStr wSP2VCompare[wSP2VCompare_Num];
#endif
/**
 * @brief void vASCSSeparator(void)
 * Set csync separator threshold
 * @param   void
 * @retval	void
 */
void vASCSSeparatorThre(void)
{
    //set Schmitt trigger H/L thre and upper bound
    vRegWriteFldAlign(ASYNC_01,CS_SEPARATOR_THU_DEFAULT, AS_CSYNC_CONT_THU);
    vRegWriteFldAlign(ASYNC_01,CS_SEPARATOR_THL_DEFAULT, AS_CSYNC_CONT_THL);
    vRegWriteFldAlign(ASYNC_02,CS_SEPARATOR_THH_DEFAULT, AS_CSYNC_CONT_THH);
}

/**
 * @brief UINT8 fgASHPolarityMeasure(void)
 * Get H sync polarity
 * @param	void
 * @retval	Hsync polarity
 */
UINT8 fgASHPolarityMeasure(void)
{
    UINT8 polarity;

    polarity = RegReadFldAlign(STA_SYNC0_00, AS_HSYNC_P);

    return polarity;
}

/**
 * @brief UINT8 fgASVPolarityMeasure(void)
 * Get Vsync polarity
 * @param	void
 * @retval	Vsync polarity
 */
UINT8 fgASVPolarityMeasure(void)
{
    UINT8 polarity;

    polarity = RegReadFldAlign(STA_SYNC0_00, AS_VSYNC_P);

    return polarity;
}

/**
 * @brief UINT16  wASHLenMeasure(void)
 * Get  counter values in 27MHz between two hsyncs
 * @param	void
 * @retval	Hlen counter
 */
UINT16 wASHLenMeasure(void)
{
    UINT16 measure;

    //longer than byte status hold.
    vDrvAsStatHold(SV_ON);

    measure = RegReadFldAlign(STA_SYNC0_00, AS_H_LEN_S);

    vDrvAsStatHold(SV_OFF);

    return (measure);
}

/**
 * @brief UINT16  wASHSyncWidthMeasure(void)
 * Get  counter values in 27MHz of Hsync width
 * @param	void
 * @retval	Hlen counter
 */
UINT16 wASHSyncWidthMeasure(void)
{
    UINT16 measure;
    //longer than byte status hold.
    vDrvAsStatHold(SV_ON);

    measure = RegReadFldAlign(STA_SYNC0_01, AS_HSYNC_WIDTH_S);

    vDrvAsStatHold(SV_OFF);
    return (measure);
}

/**
 * @brief UINT16  wASVSyncWidthMeasure(void)
 * Get  counter values in 27MHz of Hsync width
 * @param	void
 * @retval	Hlen counter
 */
UINT16 wASVSyncWidthMeasure(void)
{
    UINT16 measure;
    //longer than byte status hold.
    vDrvAsStatHold(SV_ON);

    measure = RegReadFldAlign(STA_SYNC0_01, AS_VSYNC_WIDTH_S);

    vDrvAsStatHold(SV_OFF);
    return (measure);
}

/**
 * @brief UINT16 wASVtotalMeasure(void)
 * Get line numbers between two vsyncs
 * @param	void
 * @retval	vtotal number
 */
UINT16 wASVtotalMeasure(void)
{
    UINT16 measure;
    //longer than byte status hold.
    vDrvAsStatHold(SV_ON);

    measure = RegReadFldAlign(STA_SYNC0_00, AS_V_LEN_S);

    vDrvAsStatHold(SV_OFF);

    return (measure);
}

#if ADAPTIVE_MONITOR_SLICER_MEASURE
void vAS2SyncMeasure(SP2Mon_LVL type)
{
  if (type<wSP2VCompare_Num)
  {
  //longer than byte status hold.   
  vDrvAsStatHold(SV_ON);
    
  wSP2VCompare[type].SP2_V_LEN_S = RegReadFldAlign(STA_SYNC2_01, AS2_V_LEN_S);
  wSP2VCompare[type].SP2_H_LEN_S = RegReadFldAlign(STA_SYNC2_01, AS2_H_LEN_S);
  wSP2VCompare[type].SP2_V_WIDTH_S = RegReadFldAlign(STA_SYNC2_00, AS2_VSYNC_WIDTH_S);
  
  vDrvAsStatHold(SV_OFF);
  }  
}
#endif
UINT16 wASTopBCLine(void)
{
    return RegReadFldAlign(STA_SYNC0_06, AS_NEWTOPBC_S);
}


UINT16 wASBottomBCLine(void)
{
    return RegReadFldAlign(STA_SYNC0_06, AS_NEWBOTTOMBC_S);
}

/**
 * @brief UINT8 bASActiveChk(void)
 * get sync processor activity bits.
 * @param	void
 * @retval	H/V/SOG activity
 */
UINT8 bASActiveChk(void)
{
    UINT8 temp;
    temp =( u1RegRead1B(STA_SYNC0_00_0)&0x07);
    return temp;
}

/**
 * @brief UINT8 bASHDTVActiveChk(void)
 * get sync processor activity bits.
 * @param	void
 * @retval	CSYNC & VSYNC_OUT activity
 */
UINT8 bASHDTVActiveChk(void)
{
    UINT8 temp;    
    temp =((u1RegRead1B(STA_SYNC0_00_0)&0x01)|(RegReadFldAlign(STA_SYNC0_02,AS_VSYNC_OUTX_ACT)<<1));
    return temp;
}


/**
 * @brief void vDrvAsyncBDMask(UINT16 wstart1, UINT16 wend1)
 * set the Bouandary Mask
 * @param	wstart1, wend1 : line count  from each frame vsync
 * @retval	void
 */
void vDrvAsyncBDMask(UINT16 wstart1, UINT16 wend1)
{
    //Start
    vRegWriteFldAlign(ASYNC_13, wstart1, AS_BDMASK_ST);
    //End
    vRegWriteFldAlign(ASYNC_13, wend1, AS_BDMASK_END);
}
/**
 * @brief void vDrvAsyncVMask(UINT16 wstart1, UINT16 wend1)
 * set the VMask1 to PLL coast mask
 * @param	wstart1, wend1 : line count  from each frame vsync
 * @retval	void
 */
void vDrvAsyncVMask(UINT16 wstart1, UINT16 wend1)
{
    // Start
    vRegWriteFldAlign(ASYNC_0A, wstart1, AS_VMASK1_ST);
    // End
    vRegWriteFldAlign(ASYNC_0A, wend1, AS_VMASK1_END);
}

/**
 * @brief void vDrvAsyncVMask2(UINT16 wstart1, UINT16 wend1)
 * set the VMask2 for another PLL parameter
 * @param	wstart1, wend1 : line count  from each frame vsync
 * @retval	void
 */
void vDrvAsyncVMask2(UINT16 wstart1, UINT16 wend1)
{
    //Start
    vRegWriteFldAlign(ASYNC_0A, wstart1, AS_VMASK2_ST);
    //End
    vRegWriteFldAlign(ASYNC_0A, wend1, AS_VMASK2_END);
}

/**
 * @brief void vDrvAsyncClampMask(UINT16 wstart1, UINT16 wend1)
 * set the VMask3 for clamping mask
 * @param	wstart1, wend1 : line count  from each frame vsync
 * @retval	void
 */
void vDrvAsyncClampMask(UINT16 wstart1, UINT16 wend1)
{
   //Start
    vRegWriteFldAlign(ASYNC_18, wstart1, AS_VMASK3_ST);
    //End
    vRegWriteFldAlign(ASYNC_18, wend1, AS_VMASK3_END);
}

/**
 * @brief void vDrvAsyncPreDataActive(UINT16 wstart)
 * set the post margin for data active
 * @param	wstart: line count  from each frame vsync
 * @retval	void
 */
void vDrvAsyncPreDataActive(UINT16 wstart,UINT16 wend)
{
    vRegWriteFldAlign(ASYNC_0E, wstart>>12, AS_PRE_DATA_ACTIVE_12);
    vRegWriteFldAlign(ASYNC_0E, wend>>12, AS_POST_DATA_ACTIVE_12);
    vRegWriteFldAlign(ASYNC_0E, wstart, AS_PRE_DATA_ACTIVE_11_0);
    vRegWriteFldAlign(ASYNC_0E, wend, AS_POST_DATA_ACTIVE_11_0);
}

/**
 * @brief void vDrvAsyncHBDMask(UINT16 Left, UINT16 Right)
 * set the left/right for data active
 * @param	left from hsync , right count form hsync
 * @retval	void
 */
void vDrvAsyncHBDMask(UINT16 Left, UINT16 Right)
{
    //Boundary active region
    //Start : left from hsync
    //End : Htotal - right

    // Left
    vRegWriteFldAlign(ASYNC_16, Left>>12, AS_H_BD_MASK_L_12);    
    vRegWriteFldAlign(ASYNC_15, Left, AS_H_BD_MASK_L_11_0);
    // Right
    vRegWriteFldAlign(ASYNC_16, Right>>12, AS_H_BD_MASK_R_12);
    vRegWriteFldAlign(ASYNC_15, Right, AS_H_BD_MASK_R_11_0);    
}
/**
 * @brief void vSetAsyncMeasureBD(UINT8 bmode)
 * set the boundary mask according to the timing table
 * @param	bmode : VGA/HDTV Timing number
 * @retval	void
 */
void vSetAsyncMeasureBD(UINT8 bmode)
{

   if (bmode == DOMAIN_PIXEL)
    {

		UINT16 wHSyncWidthPix, wHLenPix;
		UINT16 wLeftBD, wRightBD ;
		UINT8 bTiming ;

               vDrvAsStatHold(SV_ON);
		wHSyncWidthPix = RegReadFldAlign(STA_SYNC0_05,AS_HSYNC_WIDTH_PIX_S_7_0)
						 +(RegReadFldAlign(STA_SYNC0_06,AS_HSYNC_WIDTH_PIX_S_11_8)<<8)
						 +(RegReadFldAlign(STA_SYNC0_0A,AS_HSYNC_WIDTH_PIX_S_12)<<12);
		wHLenPix =  RegReadFldAlign(STA_SYNC0_04,AS_H_LEN_PIX_S_7_0)
					 +(RegReadFldAlign(STA_SYNC0_06,AS_H_LEN_PIX_S_11_8)<<8)
					 +(RegReadFldAlign(STA_SYNC0_0A,AS_H_LEN_PIX_S_12)<<12); 
		vDrvAsStatHold(SV_OFF);
		DBG_Printf(DBG_MDET,"[SA7][1]wHSyncWidthPix=%u \n",wHSyncWidthPix);    
		DBG_Printf(DBG_MDET,"[SA7]wHLenPix=%u \n",wHLenPix);    
	   
		if((wHLenPix>>1)<wHSyncWidthPix)
		{
		  wHSyncWidthPix = wHLenPix - wHSyncWidthPix;
		  DBG_Printf(DBG_MDET,"[SA7][2]wHSyncWidthPix=%u \n",wHSyncWidthPix);    
		}


		//Set Boundary Mask
		vDrvAsyncBDMask(7, 1); // 15 -> 7 for 1680x1050 vstart too small problem.
		//vDrvAsyncBDMask(0x0, 0xa00); // 15 -> 7 for 1680x1050 vstart too small problem.

		// set pre-data and post-data , from h sync count forward and backword (for auto color)
		vDrvAsyncPreDataActive(0x20,0x20);

		if (wHSyncWidthPix > 500)
		{
			wLeftBD=wRightBD=10;
		}
		else
		{
			 wLeftBD = 10 ;
			 wRightBD = 5+wHSyncWidthPix ;
		}
		//set for SOG tri-level sync 1080P/720P
		
		if( bModeIndex == SYNCONGREEN)
		{
			if((fgIsMainYPbPr() || fgIsPipYPbPr()))
			{
					bTiming = _bHdtvTiming;
			}
			else
			{
					bTiming = _bVgaTiming;    
			}	
			if(((Get_VGAMODE_IPH_WID(bTiming)==1280) &&(Get_VGAMODE_IPV_LEN(bTiming)== 720)) ||  
			((Get_VGAMODE_IPH_WID(bTiming)==1920) &&(Get_VGAMODE_IPV_LEN(bTiming)== 1080)) 
			)
			{
				wLeftBD = 0x40	 ;
			}
		}

		vDrvAsyncHBDMask(wLeftBD , wRightBD);
    }
    else
    {
       
		vDrvAsyncBDMask(7, 1); //for 848x480 60 CVT-RB

		// set pre-data and post-data , from h sync count forward and backword (for auto color)
		vDrvAsyncPreDataActive(0x20,0x20);

		vDrvAsyncHBDMask(10, 10);
    }
}
/**
 * @brief void vDrvAsyncVsyncStart(UINT16 wStart)
 * Set programmable  vsync start
 * @param	vsync start
 * @retval	void
 */
void vDrvAsyncVsyncStart(UINT16 wStart)
{
    UINT16 u2Height ;
    u2Height = RegReadFldAlign(ASYNC_11, AS_NEW_VS_OUTP_H1);
    vDrvAsyncVsyncOut(wStart, u2Height);
}

/**
 * @brief void vDrvAsyncVsyncOut(UINT16 wStart1, UINT16 wH1)
 * Set programmable  vsync out S1,H1, S2/H2
 * @param	wStart1, wH1, : line count  from each frame vsync
 * @retval	void
 */
void vDrvAsyncVsyncOut(UINT16 wStart1, UINT16 wH1)
{
#if MDDI_FIELD_INV_WA
    UINT16 VS1,VS2;
    UINT8 bPath ;
#endif
#if ASYNC_FULL_SCREEN_WA
    BYTE bMode ;           
    if((fgIsMainYPbPr() || fgIsPipYPbPr()))
    {
    	bMode = _bHdtvTiming;
    }
    else if(VSS_MAJOR(_bSrcMainNew) == VSS_SCART || VSS_MAJOR(_bSrcSubNew) == VSS_SCART)
    {
        bMode = (_rTvd3dStatus.fgIs525==1)?MODE_525I_OVERSAMPLE: MODE_625I_OVERSAMPLE;
    }
    else
    {
    	bMode = _bVgaTiming;	
    }	
    //wH1 = wH1 + 1 ; // for HD full screen request.
#endif 	

#if MDDI_FIELD_INV_WA
    if(fgIsMainYPbPr() || fgIsMainVga())
    {
        bPath = SV_VP_MAIN ;
    }
    else
    {
         bPath = SV_VP_PIP;
    }
    if(bDrvVideoIsSrcInterlace(bPath)) 
    {
        VS1 = wDrvVideoGetVTotal(bPath)/2;

        VS2 = (wDrvVideoInputHeight(bPath)/2) ;
        if(wStart1 > (VS1-VS2) )				
        {
    	    vDrvDISetFieldInverse(bPath, 1);	
    	}
    	else
    	{
    		vDrvDISetFieldInverse(bPath, 0);	
    	}	
    }
#endif 


    #if VSYNC_OUT_UPDATE_TRIGGER 
    vRegWriteFldAlign(ASYNC_12, 0x01, AS_VS_UP_GATE);
    vRegWriteFldAlign(ASYNC_12, 0x00, AS_VS_UPDATE_EN);
    #endif
    // Field 0
    // set Vsync Start for field 0
    vRegWriteFldAlign(ASYNC_11, wStart1, AS_NEW_VS_OUTP_S1);
    // set Vsync end for field 0
    vRegWriteFldAlign(ASYNC_11, wH1, AS_NEW_VS_OUTP_H1);
#if ASYNC_FULL_SCREEN_WA
	if(Get_VGAMODE_INTERLACE(bMode) && (bModeIndex != SEPERATESYNC))
	{
		wStart1++;	
	}
#endif    
    // Field 1
    // set Vsync Start for field 1
    vRegWriteFldAlign(ASYNC_12, wStart1, AS_NEW_VS_OUTP_S2);
    // set Vsync end for field 1
    vRegWriteFldAlign(ASYNC_12, wH1, AS_NEW_VS_OUTP_H2);

    #if VSYNC_OUT_UPDATE_TRIGGER 
    vRegWriteFldAlign(ASYNC_12, 0x01, AS_VS_UPDATE_EN);
    vRegWriteFldAlign(ASYNC_12, 0x00, AS_VS_UPDATE_EN);    
    #endif    
}

/**
 * @brief void  vDrvAsyncSetFieldDet(void)
 * Set sync processor field detection window
 * @param	void
 * @retval	void
 */
void  vDrvAsyncSetFieldDet(UINT16 wHLEN)
{
    //UINT16 wHLEN;
    UINT16 wTmp=0 ;

    //wHLEN = wASHLenMeasure();
#if ASYNC_FULL_SCREEN_WA
	if((bModeIndex != SEPERATESYNC))
	{
		//Add decomposite delay
		wTmp = RegReadFldAlign(ASYNC_01, AS_DE_COMP_DIFF_TH)* 8;	
	}
#endif     

    // FIELD_DETECT_S
    vRegWriteFldAlign(ASYNC_03,((wHLEN>>2) +wTmp), AS_FLD_DETECT_S);
    // FIELD_DETECT_E
    vRegWriteFldAlign(ASYNC_03, (((wHLEN*3)>>2) + wTmp), AS_FLD_DETECT_E);
}

/**
 * @brief UINT16 wSP0IHSClock(void)
 * Calculate Hsync clock , IC count hsync number with crystal clock,then we should translate to real hsync clock
 * @param	whtotal : H len count measured in 27MHz,
 * @retval	H frequency
 */
UINT16 wSP0IHSClock(UINT16 whlen)
{
    UINT32 lhsync_freq;
    //multiply 2 for increasing 1bit resolution
    //multiply 100000 for Mega scale
    lhsync_freq = whlen;
    if (lhsync_freq == ZERO)
    {
        return 0;
    }
    lhsync_freq = (CRYSTAL * 1000000 * 2) / lhsync_freq;
    lhsync_freq = lhsync_freq / 100;	// resolution be with 100Hz as unit
    lhsync_freq = ((lhsync_freq + 1) >> 1);
    //avoid overflow issue
    return (UINT16) lhsync_freq;
}
/**
 * @brief UINT8 bSP0IVSClock(UINT16 whtotal, UINT16 wvtotal)
 * According to Htotal,Vtotal and crystal clock to get Vsync clock
 * @param	whtotal : H len count measured in 27MHz,
 * @param 	vtotal : H line count between two vsync
 * @retval	V frequency
 */
// Calculate Vertical Refresh Rate
// Formula ==>[refresh = Crystal freq / (vHtotalMeasure*vVtotalMeasure )
UINT8 bSP0IVSClock(UINT16 whlen, UINT16 wvtotal)
{
    UINT32 lvsync_freq;
    lvsync_freq = wvtotal;
    lvsync_freq = lvsync_freq * whlen;
    if (lvsync_freq == ZERO)
    {
        return 0;
    }
    lvsync_freq = (CRYSTAL * 1000000 * 2) / lvsync_freq;
    lvsync_freq = ((lvsync_freq + 1) >> 1) ;
    return ((UINT8) lvsync_freq);
}

/**
 * @brief void SP0Initial(void)
 * Common intial  for SP0 variable
 * @param	void
 * @retval	void
 */
void SP0Initial(void)
{
    wSP0Hclk = ZERO;
    bSP0Vclk = ZERO;
    fgSP0Hpol = ZERO;
    fgSP0Vpol = ZERO;
    wSP0Vtotal = ZERO;
    _bUnLockCnt = 0 ;
    x_memset(wSP0VCompare, 0, 8);

    // Set Serration Window instead of HLength Protection to Get Proper VTotal Value
    vRegWriteFldAlign(ASYNC_09, 20, AS_SERR_MASK_ST);
    vRegWriteFldAlign(ASYNC_09, 20, AS_SERR_MASK_END);
#if 0
    //reset  polarity
    vDrvHsLockInv((UINT8)FALSE);
    vDrvHsInv((UINT8)FALSE);
    vDrvVsInv((UINT8)FALSE);
    vDrvVsOutInvPol((UINT8)FALSE);
#endif
}

/**
 * @brief void vResetVLen(void)
 * Reset Vlen counter when losing V sync for VGA
 * @param	None
 * @retval	None
 */
void vResetVLen(void)
{
    vRegWriteFldAlign(ASYNC_02, 0x1, AS_HLEN_VLEN_RESET);   // Set HVLEN_RESET
    vUtDelay2us(50);
    vRegWriteFldAlign(ASYNC_02, 0x0, AS_HLEN_VLEN_RESET);  // Disable HVLEN_RESET
}


#if HDTV_MV_DETECT_SUPPORT
/**
 * @brief void vDrvAsyncMvDetectV(WORD wstart1, WORD wend1) large
 * set the MV V detetion Active range
 * @param	wstart1, wend1 : line count  from each frame vsync
 * @retval	void
 */
void vDrvAsyncMvDetectV (UINT16 wstart1, UINT16 wend1)
{
    //Start
    vRegWriteFldAlign(ASYNC_09, wstart1, AS_MV_VACTIVE_ST);
    //End
    vRegWriteFldAlign(ASYNC_09, wend1, AS_MV_VACTIVE_END);
}


/**
 * @brief void vDrvAsyncMvDetectH(WORD wstart1, WORD wend1) large
 * set the MV H detetion Active range
 * @param	wstart1, wend1,: H count  from start 1 to (htotal - end1)
 * @retval	void
 */
void vDrvAsyncMvDetectH (UINT16 wstart, UINT16 wend)
{
    // Start
    vRegWriteFldAlign(ASYNC_08, wstart, AS_MV_HSTART);
    // End
    vRegWriteFldAlign(ASYNC_08, wend, AS_MV_HEND);
}

/**
 * @brief void bDrvAsyncMvStatus(void) large
 * get MV status
 * @param	void
 * @retval	void
 */
UINT8 bDrvAsyncMvStatus (void)
{
    UINT8 f0;
    UINT8 mode ;

    if (fgIsMainVga()||fgIsPipVga())
    {
        mode=_bVgaTiming;
    }
    else
    {
        mode=_bHdtvTiming;
    }
    //check only SDTV timing support MV
    switch (mode)
    {
        case MODE_525I :
        case MODE_625I :
        case MODE_525I_OVERSAMPLE :
        case MODE_625I_OVERSAMPLE :
        case MODE_480P:
        case MODE_576P:
        case MODE_480P_OVERSAMPLE:
        case MODE_576P_OVERSAMPLE:
            break;
        default:
            return OFF ;
    }

    f0=RegReadFldAlign(STA_SYNC0_03,AS_F0_PSYNC_NO);

    if (f0 >=4)
    {
        return ON;
    }
    else
    {
        return OFF ;
    }
}
#else
UINT8 bDrvAsyncMvStatus (void)
{
    return OFF;
}
#endif


/**
 * @brief void vASDeCompSel(UINT8 bOn) set de composite method 
 * @param    DeCompSel : De-composite method #
 * @retval	void
 */
void vASDeCompSel(UINT8 DeCompSel)
{
    vRegWriteFldAlign(ASYNC_01, DeCompSel, AS_DE_COMP_SEL);

    if(DeCompSel == NEW_DECOMPOSITE1)
    {
        vRegWriteFldAlign(ASYNC_07, 0x90, AS_VSYNC_OUT_HYST_THR);
        //0x96 -> 0x90 [DTV00123929]
    }
    else
    {
        vRegWriteFldAlign(ASYNC_07, 0x0, AS_VSYNC_OUT_HYST_THR);
    }
}

#if SUPPORT_HDTV_HARDWARE_MUTE
/**
 * @brief void vDrvAsyncSetMuteCriteria() set hardware mute functionality 
 * @param    u4MuteFlag bit0: Enable mute when Hsync active changed
 *                      bit1: Enable mute when Vsync active changed
 *                      bit2: Enable mute when Csync active changed
 *                      bit3: Enable mute when Hsync polarity changed
 *                      bit4: Enable mute when Vsync polarity changed
 *                      bit5: Enable mute when Horizontal length changed
 *                      bit6: Enable mute when Vertical length changed
 * @retval	void
 */
void vDrvAsyncSetMuteCriteria(UINT32 u4MuteFlag)
{
    vRegWriteFldAlign(ASYNC_0C, u4MuteFlag, AS_MUTE_MULTI_EN);

    return;
}
#endif

void vDrvRETIMEReset(void)
{
    vRegWriteFldAlign(SYS_05, 1, RETIME_AUTO);          //reset SDDS internal Hsync & pixel clock
    vUtDelay2us(1);
    vRegWriteFldAlign(SYS_05, 0, RETIME_AUTO);

    _RETIMENeedReset = FALSE;
}
