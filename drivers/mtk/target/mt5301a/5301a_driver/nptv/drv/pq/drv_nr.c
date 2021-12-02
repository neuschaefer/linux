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
 * $RCSfile: drv_nr.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/**
 * @file NR driver
 *
 * @author mf_tien@mtk.com.tw
 * @function entry point
 *  vDrvNRInit
 *  vDrvNRSetMode
 * @user case
 *  power on -> vDrvNRInit
 *  mode change done -> vDrvNRSetMode
 *  user menu -> vDrvNRSetMode
 */

/**************************************************************************
 * Inclusions
 *************************************************************************/
#define _DRV_NR_C_

#include "general.h"
#include "hw_vdoin.h"
#include "hw_nr.h"
#include "hw_di.h"
#include "hw_scpos.h"
#include "drv_nr.h"
#include "drv_tvd.h"
#include "api_eep.h"
#include "source_select.h"
#include "source_table.h"
#include "vdo_misc.h"
#include "video_def.h"
#include "eepdef.h"
//#include "nptv_debug.h"
#include "nr_debug.h"
#include "drv_ycproc.h"
#include "hw_ycproc.h"
#include "fbm_drvif.h"
#include "hw_tdc.h"
#include "srm_drvif.h"
#include "drv_mpeg.h"
#include "drv_scaler.h"
#include "drv_di.h"
#include "x_hal_5381.h"
#include "pe_if.h"
#include "drv_contrast.h"

//NR global parameter
NR_PRM_T _NRParam;

//Variable for setting chroma line avg. for TVD
UINT8 _TVD_path;
NR_CHROMA_LINE_AVERAGE_MODE _TVD_Chroma_mode;

//Deblocking status
UINT8 _Auto_DB_Status;

////////////////////////////////////////////////////////////////////////////////
// Register table
////////////////////////////////////////////////////////////////////////////////
//const CODE NR_REGTBL_T NR_SYS_INIT[]=
const NR_REGTBL_T CODE  NR_SYS_INIT[]=
{
    {NR_3DNR01,     0x00000000, 0x00003FFF},  // Disable chroma line avg
    {NR_3DNR0D,     0x40000000, 0xFFFFFFFF},  //line buffer auto
    {NR_2DNR1E,     0x00000000, 0x00000010},  //boundary repeat
	{NR_REGTBL_END, 0x00000000, 0x00000000}
};

////////////////////////////////////////////////////////////////////////////////
// static function for internal use
////////////////////////////////////////////////////////////////////////////////
static void vDrvNRSetCurrentPath(UINT8 u1VdpId);
//functions for setting NR-related window size
static void vDrvNRSetWindows(UINT16 wWidth, UINT16 wHeight);
static void vDrvNRSetLocalBlockDetectorWindow(UINT16 wWidth,UINT16 wHeight);
static void vDrvNRSetBlockMeterWindow(UINT16 wWidth,UINT16 wHeight);
static void vDrvNRSet3DActiveWindow(UINT16 wWidth,UINT16 wHeight);
static void vDrvNRSet3DMetricWindow(UINT16 wWidth,UINT16 wHeight);
static void vDrvNRSetFrameBaseStillWindow(UINT16 wWidth,UINT16 wHeight);
//functions for setting Chroma line avg mode for TVD
static void vDrvNRSetChromaLineAvgModeReg(UINT8 bPath, NR_CHROMA_LINE_AVERAGE_MODE bMode);

static void vDrvNRCheckInputSize(void);
static void vDrvNRCheckLineBufferSetting(void);

//3D NR YC on/ off rule
static void vDrvNRDetermine3DNRYCOnoff(void);
//vga mode
static void vDrvNRCheckVGAModeOnOff(void);
//check if v bound protect should be turned on/off
static void vDrvNRCheckVBoundProtect(void);

#if NR_DISABLE_PIXEL_REP_WHEN_BLANKING_IS_SAMLL
static void vDrvNRCheckBourndaryRepeat(UINT8 bPath);
#endif

#if NR_ENABLE_ROUNDING_TOGGLE_ON_OFF
UINT8 bRoundingOnCounter;
UINT8 bRoundingOffCounter;
#endif

#if ANM_ENABLE
UINT8 vDrvRFNMLevel(void);
#endif
extern UINT8 bTvdSignalStrengthLevel(UINT8 bMode);
////////////////////////////////////////////////////////////////////////////////
//NR Process
////////////////////////////////////////////////////////////////////////////////

#if SUPPORT_SEAMLESS_MODE
/**
 * @brief Set split line for demo mode
 * @param bOnOff SV_ON/SV_OFF
 * @param bPath SV_VP_MAIN/SV_VP_PIP
 * @param wXCenter Split line position
 * @warning bPath does not have any effect for now
 */
void vDrvNRDemo(UINT8 bOnOff, UINT8 bRegion, UINT16 wXCenter)
{
    if (bOnOff)
	{
        // 2DNR
        //enable slice mode
        vRegWriteFldAlign(NR_2DNR1F, 0x1, rslice);	
        //set source on left or right
        if(bRegion == SV_LEFT)
        {
            vRegWriteFldAlign(NR_2DNR1F, 0x1, rleft_src);	            
        }
        else
        {
            vRegWriteFldAlign(NR_2DNR1F, 0x0, rleft_src);	            
        }
        //set slice line x position
        vRegWriteFldAlign(NR_2DNR02, wXCenter, ruiSliceXPos);	                    

        // 3DNR
        vRegWriteFldAlign(NR_3DNR3E, wXCenter, hend);
	}
	else
	{
        // 2DNR
        //disable slice mode
        vRegWriteFldAlign(NR_2DNR1F, 0x0, rslice);	
        //set source on left or right
        vRegWriteFldAlign(NR_2DNR1F, 0x1, rleft_src);	            
        //set slice line x position
        vRegWriteFldAlign(NR_2DNR02, 0x7FF, ruiSliceXPos);	                    

        // 3DNR
        vRegWriteFldAlign(NR_3DNR3E, 0x7FF, hend);
	}
}
#endif //SUPPORT_SEAMLESS_MODE

/**
 * @brief Function to load a register table.
 * @param prRegTbl table to load
 */
void vDrvNRLoadRegTbl(const NR_REGTBL_T* prRegTbl)
{
	while (prRegTbl->u2Addr != NR_REGTBL_END)
	{
		vRegWrite4BMsk(prRegTbl->u2Addr, prRegTbl->u4Value, prRegTbl->u4Mask);
		prRegTbl++;
	}
}
 
/**
* @brief Set 3D NR IIR Path
* @param u1VdpId VDP_1/ VDP_2            
*/
void vDrvNRSetIIRPath(UINT8 u1VdpId)
{    
    if(u1VdpId != bDrvNRGetCurrentPath())
    {
        return;
    }
    vRegWriteFldAlign(NR_3DNR00, (u1VdpId == VDP_1) ? 0 : 1, nr_main_pip_sel);
}

/**
* @brief Set NR read enable/disable
* @param bOnOff SV_TRUE Enable
*               SV_FALSE Disable             
*/ 
void vDrvNRSetReadEnable(UINT8 u1VdpId,UINT8 bOnOff)
{
    if(u1VdpId != bDrvNRGetCurrentPath())
    {
        return;
    }
    vRegWriteFldAlign(NR_3DNR00, (bOnOff == SV_FALSE) ? 0 : 1, nr_read_enable);
}

/**
* @brief Set NR scan mode
* @param scanMode NR_SCAN_MODE_INTERLACE/ NR_SCAN_MODE_PROGRASSIVE
*/
void vDrvNRSetScanMode(UINT8 u1VdpId,NR_SCAN_MODE scanMode)
{
    if(u1VdpId != bDrvNRGetCurrentPath())
    {
        return;
    }
    vRegWriteFldAlign(NR_3DNR00, (scanMode == NR_SCAN_MODE_INTERLACE) ? 0 : 1, nr_progressive);
}

/**
* @brief Set NR YUV mode
* @param nrYuvMode Y_ONLY/ 420/ 422/ 444
*/
void vDrvNRSetYUVMode(UINT8 u1VdpId,NR_YUV_MODE nrYuvMode)
{
    UINT8 bNrYuvMode = 0;
    UINT8 bSrc420 = 0;
    
    if(u1VdpId != bDrvNRGetCurrentPath())
    {
        return;
    }    
    
    if(nrYuvMode == NR_YUV_MODE_Y_ONLY)
    {
        bNrYuvMode = 0;
        bSrc420 = 0;
    }
    else if(nrYuvMode == NR_YUV_MODE_420)
    {
        bNrYuvMode = 1;
        bSrc420 = 1;
    }
    else if(nrYuvMode == NR_YUV_MODE_422)
    {
        bNrYuvMode = 2;    
        bSrc420 = 0;
    }
    else if(nrYuvMode == NR_YUV_MODE_444)
    {
        bNrYuvMode = 3;    
        bSrc420 = 0;
    }

    //set Mode related register
    vRegWriteFldAlign(NR_3DNR00, bNrYuvMode, nr_yuv_mode);
    vRegWriteFldAlign(NR_3DNR00, bSrc420, c_src_420);    

}

/**
* @brief Set NR Bit depth
* @param nrBitSel 8BIT/ 10BIT
*/
void vDrvNRSetBitSel(UINT8 u1VdpId,NR_BIT_SEL nrBitSel)
{
    if(u1VdpId != bDrvNRGetCurrentPath())
    {
        return;
    }

    UINT32 dwVal = (nrBitSel == NR_BIT_SEL_8BIT) ? 1 : 0;
    
    vRegWriteFldMulti(NR_3DNR00,
                  P_Fld(dwVal, nr_bit_sel_rv) | 
                  P_Fld(dwVal, nr_bit_sel_ru) | 
                  P_Fld(dwVal, nr_bit_sel_ry));
}

/**
* @brief Set chroma line average mode. This function only keep the mode and path
*        in variable. The real hw register setting will be wrote into register until
*        vDrvNRSetChromaLineAvgModeReg() was invoked.
* @param bPath VDP_1/ VDP_2
* @param bMode 	NR_CHROMA_LINE_AVERAGE_MODE_OFF/ NR_CHROMA_LINE_AVERAGE_MODE_UP/
*           	NR_CHROMA_LINE_AVERAGE_MODE_DOWN/ NR_CHROMA_LINE_AVERAGE_MODE_121
*/
void vDrvNRSetChromaLineAvgMode(UINT8 bPath, NR_CHROMA_LINE_AVERAGE_MODE bMode)
{
	_TVD_path = bPath;
	_TVD_Chroma_mode = bMode;
}

/**
* @brief Set chroma line average mode register.
* @param bPath VDP_1/ VDP_2
* @param bMode 	NR_CHROMA_LINE_AVERAGE_MODE_OFF/ NR_CHROMA_LINE_AVERAGE_MODE_UP/
*           	NR_CHROMA_LINE_AVERAGE_MODE_DOWN/ NR_CHROMA_LINE_AVERAGE_MODE_121
*/
static void vDrvNRSetChromaLineAvgModeReg(UINT8 bPath, NR_CHROMA_LINE_AVERAGE_MODE bMode)
{
    //if NR works on TVD path    
	if (bPath == bDrvNRGetCurrentPath())
	{
	    if(bMode == NR_CHROMA_LINE_AVERAGE_MODE_OFF)
	    {
	        //off
	        vRegWriteFldAlign(NR_3DNR01, 0x0, clavgsel);
			LOG(3, "NR : TVD turn off chroma line avg\n");	        
	    }
	    else if(bMode == NR_CHROMA_LINE_AVERAGE_MODE_UP)
	    {
	        //up
	        vRegWriteFldAlign(NR_3DNR01, 0x1, clavgsel);
	    }
	    else if(bMode == NR_CHROMA_LINE_AVERAGE_MODE_DOWN)
	    {
	        //down
	        vRegWriteFldAlign(NR_3DNR01, 0x2, clavgsel);	        
	    }
	    else if(bMode == NR_CHROMA_LINE_AVERAGE_MODE_121)
	    {
	        //121
	        vRegWriteFldAlign(NR_3DNR01, 0x3, clavgsel);	        	        
	    }
	    else
	    {
	        //error handle
	        vRegWriteFldAlign(NR_3DNR01, 0x0, clavgsel);	        	        	    
	    }
    }
	else
	{
        //off
        vRegWriteFldAlign(NR_3DNR01, 0x0, clavgsel);
		LOG(3, "NR : TVD path is different from NR : turn off chroma line avg\n");
		LOG(3, "NR : TVD path is :%d\n", bPath);
		LOG(3, "NR : NR path is :%d\n", _NRParam.bNRPath);
	}
}

/**
 * @brief Check if input width/ height is changed.
 *        If changed: 1. Check line buffer setting.
 *                    2. Check NR window setting.
 */
static void vDrvNRCheckInputSize(void)
{
	UINT8 bPath;
	UINT16 u2Width;
	UINT16 u2Height;
    UINT16 u2XOffset;
    UINT16 u2YOffset;
	
    PSCAN_CLIP_INFO_T ScalerOverscanInfo;	

    //1. get current NR path
    bPath = bDrvNRGetCurrentPath();
	//2. get input height (field domain)
    if (bScalerGetClipInfo(bPath, &ScalerOverscanInfo))
    {
    	u2Width = ScalerOverscanInfo.wWidth;
    	u2Height = ScalerOverscanInfo.wHeight;
    	u2XOffset = ScalerOverscanInfo.wX;
    	u2YOffset = ScalerOverscanInfo.wY;
    }
    else
    {
    	u2Width = _NRParam.u2NRWidth;
    	u2Height = _NRParam.u2NRHeight;
    	u2XOffset = _NRParam.u2XOffset;
    	u2YOffset = _NRParam.u2YOffset;    	
    }

	if(bDrvVideoIsSrcInterlace(bPath))
	{
		u2Height = u2Height/2;
	}
    
    //4. if input size changed.
	if((_NRParam.u2NRWidth != u2Width) || 
	   (_NRParam.u2NRHeight != u2Height) ||
	   (_NRParam.u2XOffset != u2XOffset) ||
	   (_NRParam.u2YOffset != u2YOffset))
	{
	    LOG(3,"[NR] Input changed Width(%d->%d) Height(%d->%d)\n",_NRParam.u2NRWidth,u2Width,_NRParam.u2NRHeight,u2Height);
        //4.1 update input width/ height
		_NRParam.u2NRWidth = u2Width;
		_NRParam.u2NRHeight = u2Height;
		_NRParam.u2XOffset = u2XOffset;
	    _NRParam.u2YOffset = u2YOffset;
	    
		//4.2 check line buffer setting
        vDrvNRCheckLineBufferSetting();
        //4.3 set input windows (2D/3D/Meters)
		vDrvNRSetWindows(_NRParam.u2NRWidth, _NRParam.u2NRHeight);

        #if NR_DISABLE_PIXEL_REP_WHEN_BLANKING_IS_SAMLL
        vDrvNRCheckBourndaryRepeat(bPath);	
    	#endif

    	vDrvNRCheckVBoundProtect();
	}
}

/**
 * @brief check line buffer length setting
 */
static void vDrvNRCheckLineBufferSetting(void)
{
    //if widht is between 0 and 1920
    if(_NRParam.u2NRWidth<=1920)
    {
        vRegWriteFldAlign(NR_3DNR0D, 0x1, lnbufmode);
        vRegWriteFldAlign(NR_3DNR0D, 0x0, lnbuf_manual_length);
    }
    else //if widht > 1920, use manual length
    {
        vRegWriteFldAlign(NR_3DNR0D, 0x2, lnbufmode);
        vRegWriteFldAlign(NR_3DNR0D, (_NRParam.u2NRWidth / 2) - 1, lnbuf_manual_length);    
    }
}

/**
 *  @brief Set all NR related window
 *         1. Local block detector window
 *         2. Block meter window
 *         3. 3D NR active window
 *         4. 3D NR metric window
 *         5. Frame base still detection window
 *  @param wWidth input widht
 *  @param wHeight input height
 */
static void vDrvNRSetWindows(UINT16 wWidth, UINT16 wHeight)
{
    vDrvNRSetLocalBlockDetectorWindow(wWidth,wHeight);
    vDrvNRSetBlockMeterWindow(wWidth,wHeight);
    vDrvNRSet3DActiveWindow(wWidth,wHeight);
    vDrvNRSet3DMetricWindow(wWidth,wHeight);
    vDrvNRSetFrameBaseStillWindow(wWidth,wHeight);
}

/**
 *  @brief Set local block detector window. 
 *         This window will only take effect when register "r_block_pdp" is set to enable.
 *  @param wWidth input widht
 *  @param wHeight input height 
 */
static void vDrvNRSetLocalBlockDetectorWindow(UINT16 wWidth,UINT16 wHeight)
{
    vRegWriteFldAlign(NR_2DNR01, wWidth, rImgWidth);
    vRegWriteFldAlign(NR_2DNR01, wHeight, rImgHeight);    
}

/**
 *  @brief Set block meter window.
 *  @param wWidth input widht
 *  @param wHeight input height 
 */
static void vDrvNRSetBlockMeterWindow(UINT16 wWidth,UINT16 wHeight)
{
    if( wWidth > 1920 )
    {
        wWidth = 1920;
    }
    vRegWriteFldAlign(NR_2DNR5A, 0x0, rhpos_start);
    vRegWriteFldAlign(NR_2DNR5A, (wWidth>>3) - 1, rhpos_end);
    vRegWriteFldAlign(NR_2DNR5A, 0x0, rvpos_start);
    if( wHeight <= 1020 )
    {
        vRegWriteFldAlign(NR_2DNR5A, (wHeight>>2) -1, rvpos_end);
    }
    else
    {
        vRegWriteFldAlign(NR_2DNR5A, 0xff, rvpos_end);
    }    
    vRegWriteFldAlign(NR_2DNR5B, (wWidth-1)>>1, rvideo_width);
    vRegWriteFldAlign(NR_2DNR5B, wHeight, rvideo_height);    
}

/**
 *  @brief Set 3D NR Active window
 *  @param wWidth input widht
 *  @param wHeight input height 
 */
static void vDrvNRSet3DActiveWindow(UINT16 wWidth,UINT16 wHeight)
{
    vRegWriteFldAlign(NR_3DNR3E, 0x0, hstart);
    vRegWriteFldAlign(NR_3DNR3E, 0x7ff, hend);    
//    vRegWriteFldAlign(NR_3DNR3E, wWidth, hend);
    vRegWriteFldAlign(NR_3DNR3F, 0x1, vstart);
    vRegWriteFldAlign(NR_3DNR3F, 0x7ff, vend);        
//    vRegWriteFldAlign(NR_3DNR3F, wHeight, vend);    
}

/**
 *  @brief Set 3D NR Metric window
 *  @param wWidth input widht
 *  @param wHeight input height 
 */
static void vDrvNRSet3DMetricWindow(UINT16 wWidth,UINT16 wHeight)
{
    vRegWriteFldAlign(NR_3DNR33, 0x0, hstart_metric);
    vRegWriteFldAlign(NR_3DNR33, wWidth, hend_metric);
    vRegWriteFldAlign(NR_3DNR34, 0x1, vstart_metric);
    vRegWriteFldAlign(NR_3DNR34, wHeight, vend_metric);    
}

/**
 *  @brief Set 3D Frame base still detector window
 *  @param wWidth input widht
 *  @param wHeight input height 
 */
static void vDrvNRSetFrameBaseStillWindow(UINT16 wWidth,UINT16 wHeight)
{
    vRegWriteFldAlign(NR_3DNR48, 0x0, hstart_FrSt);
    vRegWriteFldAlign(NR_3DNR48, wWidth, hend_FrSt);
    vRegWriteFldAlign(NR_3DNR49, 0x1, vstart_FrSt);
    vRegWriteFldAlign(NR_3DNR49, wHeight, vend_FrSt);    
}

/**
 *  @brief Set 3D NR manual weight
 */
void DrvNRSetManualWeight(UINT8 bOnOff,UINT8 bWeight)
{
    vRegWriteFldAlign(NR_3DNR02, bOnOff, manwcu);
    vRegWriteFldAlign(NR_3DNR3C, bWeight,manbwcu);
    LOG(3,"Set manual mode (%d) weight(%d)\n",bOnOff,bWeight);
}

UINT8 Is_MT8223_E1()
{
    return (u1RegRead1B(0x83) != 0xA6);
}

/**
 * @brief Initialize NR
 */
void vDrvNRInit(void)
{
    //1. Load init table    
	vDrvNRLoadRegTbl(NR_SYS_INIT);
    //2. Load init quality table
    vDrvNRQtyInit();
    //3. turn off 3D NR
	vDrvNRSet3DNROnOff(SV_OFF);

	//4. init NR window size
	_NRParam.u2NRWidth = 1;
	_NRParam.u2NRHeight = 1;
    //Disable dbg_en 
	#if 1
	vRegWriteFldAlign(NR_MISC, 0x0, dbg_en);    
	vRegWriteFldAlign(NR_MISC, 0x0, en_flagw_4f);    
	vRegWriteFldAlign(NR_MISC, 0x0, en_flagr);    
	#endif	
	vRegWriteFldAlign(NR_3DNR03, 0x0, bpyc2d);	            // disable 2D bypass
	vRegWriteFldAlign(PAT_MAIN_02, 0x0, C_MAIN_SELF_FIELD_VCHG);	//set filed 1 delay to 0 line
	vRegWriteFldAlign(NR_3DNR00, 0x1, c_v_bound_protect);   //Vertical boundary protect for NR linebuf

    /*TODO: REMOVE THIS LINE AFTER NR FLOW IS STABLE*/
	vDrvNRBypass(SV_ON);
    vDrvNRSet3DNRUIOnOff(SV_ON);

#ifdef CC_MT5363
    vRegWriteFldAlign(NR_2DNR03, 0x1 , r_bBoundRepeat10Pxl);
    vRegWriteFldAlign(NR_2DNR03, 0x1 , r_bBoundaryRepeatEn);
#endif

	//for sharing sram with demod in eco ic
	if (Is_MT8223_E1())
	{
		vRegWriteFldAlign(NR_2DNR97, 0x1, DEMOD_ECO_RST_B); 		 
	}

    //for cli debug
    _NRParam.bNRDebugPath = SV_VP_NA;    
}

/**
 * @brief Decide 1. NR path should be on Main or PIP.
 *               2. NR bypass should be Turned on/ off.
 */
UINT8 bDrvNRDeterminePath(void)
{
    //for cli debug
    if(_NRParam.bNRDebugPath != SV_VP_NA)
    {
        _NRParam.bNRPath = ((_NRParam.bNRDebugPath==SV_VP_PIP)?SV_VP_PIP:SV_VP_MAIN);
        _NRParam.bNRBypass = SV_OFF;
        return _NRParam.bNRPath;
    }
    
    /*1. decide NR path. 
     * Rule: 
     *     1. In most of cases, NR would be on main path
     *     2. If main is 444 input and sub is ATV/ DTV, NR would be on sub
     */
    /*     
    if(bIsScalerInput444(SV_VP_MAIN) && 
       (bGetVideoDecType(SV_VP_PIP) == SV_VD_TVD3D) || (bGetVideoDecType(SV_VP_PIP) == SV_VD_MPEGHD))
    {
        _NRParam.bNRPath = SV_VP_PIP;
    }
    else
    {
        _NRParam.bNRPath = SV_VP_MAIN;
    }
    */

    //Currently, NR will always act on main path.
    //Todo: If we want to switch to sub path, need to notify srm that nr will 
    //      on sub and srm should allocate dram to sub pscan to avoid dram overlap.
    _NRParam.bNRPath = SV_VP_MAIN;
    
	/*2. Decide if NR bypass on/ off
	 * Rule:
     *	   NR will be bypassed when
	 *     1. If Current NR path is pip and is 444 input. 
	 *   or
	 *     2. If Current NR path have no source selected.
	 *   or
	 *     3. If Current NR path have no signal
	 */
	if((_NRParam.bNRPath == SV_VP_PIP && bIsScalerInput444(_NRParam.bNRPath)) || 
	   bGetVideoDecType(_NRParam.bNRPath) == SV_VD_NA ||
	   bDrvVideoSignalStatus(_NRParam.bNRPath) != SV_VDO_STABLE)
	{
	    _NRParam.bNRBypass = SV_ON;
	}
	else
	{
	    _NRParam.bNRBypass = SV_OFF;
	}

	return _NRParam.bNRPath;
}

/**
* @brief Get NR current depth
* @return NR path
*/
UINT8 bDrvNRGetCurrentPath(void)
{
    return _NRParam.bNRPath;
}

/**
* @brief Set NR current path
* @param u1VdpId VDP_1/ VDP_2
*/
void vDrvNRSetCurrentPath(UINT8 u1VdpId)
{
    vRegWriteFldAlign(NR_3DNR00, (u1VdpId==VDP_1)?0:1, c_nr_src_sel);
}

void vDrvNRSetDebugPath(UINT8 u1VdpId)
{
    _NRParam.bNRDebugPath = u1VdpId;

    vDrvNRSetSystem();
}

void vDrvNRSetForceBypass(UINT8 bOnOff)
{
    _NRParam.bForceNRBypass = bOnOff;
    vDrvNRSetSystem();
}

/**
 * @brief Set NR bypass on/off
 * @param bOnOff SV_ON: to bypass NR
 *               SV_FALSE: to enable NR
 */
void vDrvNRBypass(UINT8 bOnOff)
{
#ifdef __MODEL_slt__
	if((bGetVideoDecType(SV_VP_MAIN) == SV_VD_MPEGHD) || (bGetVideoDecType(SV_VP_MAIN) == SV_VD_MPEGSD))
	{
		return;
	}
#endif

    if(bOnOff == SV_ON) //bypass NR
    {
        //bypass Main path NR
        vRegWriteFldAlign(NR_TOP_MAIN_00, 0X0, c_main_nr_en);
        //bypass PIP path NR
        vRegWriteFldAlign(NR_TOP_PIP_00, 0X0, C_PIP_NR_EN);
        
        //todo: turn off 3D NR??
    }
    else    //enable NR
    {
        if(_NRParam.bNRPath == SV_VP_MAIN)
        {
            //enable NR on Main path
            vRegWriteFldAlign(NR_TOP_MAIN_00, 0X1, c_main_nr_en);
            //disable NR on Sub Path
            vRegWriteFldAlign(NR_TOP_PIP_00, 0X0, C_PIP_NR_EN);
        }
        else
        {
            //enable NR on PIP path
            vRegWriteFldAlign(NR_TOP_PIP_00, 0X1, C_PIP_NR_EN);
            //disable NR on Main path
            vRegWriteFldAlign(NR_TOP_MAIN_00, 0X0, c_main_nr_en);
        }        
        //todo: turn on 3D NR??
    }
}

/**
 * @brief Set 2D NR On/ Off
 * @param bOnOff SV_ON/ SV_OFF
 */
void vDrvNRSet2DNROnOff(UINT8 bOnOff)
{
	_NRParam.b2DNROnOff = bOnOff;
	vRegWriteFldAlign(NR_2DNR1F, bOnOff , r2D_enable);
}

/**
 *  @brief Set deblocking On/Off
 *  @param bOnOff SV_ON/ SV_OFF
 */
void vDrvNRSetDeblockingOnOff(UINT8 bOnOff)
{
    _NRParam.bDeblockingOnOff = bOnOff;
    vRegWriteFldAlign(NR_2DNR1F, bOnOff , rbNewPxlBkEn);
}

/**
 *  @brief Set mosquito On/Off
 *  @param bOnOff SV_ON/ SV_OFF
 */
void vDrvNRSetMosquitoOnOff(UINT8 bOnOff)
{
    _NRParam.bMosquitoOnOff = bOnOff;
    vRegWriteFldAlign(NR_2DNR6B, bOnOff , c_motion_mos3_en);
    vRegWriteFldAlign(NR_2DNR6B, bOnOff , c_motion_mos2_en);
    vRegWriteFldAlign(NR_2DNR6B, bOnOff , c_motion_mos1_en);
    vRegWriteFldAlign(NR_2DNR6A, bOnOff , c_still_mos3_en);
    vRegWriteFldAlign(NR_2DNR6A, bOnOff , c_still_mos2_en);
    vRegWriteFldAlign(NR_2DNR6A, bOnOff , c_still_mos1_en);
}

/**
 * @brief Set Auto NR On/ Off
 * @param bOnOff SV_ON/ SV_OFF
 */

UINT32 fgDrvGetAutoNROnOff()
{
    return fgIsAutoNREnabled;
}

/**
 * @brief Set Auto NR On/ Off
 * @param bOnOff SV_ON/ SV_OFF
 */
void vDrvSetAutoNROnOff(UINT8 bOnOff)
{   
    NMEnable = bOnOff;
    fgIsAutoNREnabled=bOnOff;

    if(fgIsAutoNREnabled==SV_OFF)
    {
        //Reset All Condition Effect by AUTO NR
        vRegWriteFldAlign(NR_3DNR02, bOnOff, enforce_tbl);
        bReducedSaturation=0;    
        _bNMChkState=NM_INIT;    
        vIO32WriteFld(NR_AUTO_NR_STA,0, NR_AUTO_CUR_LV);
        //bApiVideoProc(bDrvNRGetCurrentPath(), PE_ARG_3D_NR);

        if (fgIsAutoNREnabled != fgPreAutoNREnabled)
        {
            bApiVideoProc(SV_VP_MAIN, PE_ARG_SHARPNESS);//Only Work On Main Path
            bApiVideoProc(bDrvNRGetCurrentPath(),PE_ARG_SATURATION);
            fgPreAutoNREnabled = fgIsAutoNREnabled;
        }
    }
}
    
void vDrvNRSetNRDataMode(UINT8 u1VdpId, E_NR_DMODE u1DataMode)
{

    UINT8 _2DNROnOff, _3DNROnOff, nrSource444Mode;
    
    if(u1VdpId != bDrvNRGetCurrentPath())
    {
        return;
    }

    LOG(3,"PSCAN set NR data mode (%d)\n",u1DataMode);
    
    _NRParam.bSrm3DNRMode = u1DataMode;

    switch (u1DataMode)
    {        
        case E_DI_NR_Y_ONLY:
            _2DNROnOff = SV_ON;
            _3DNROnOff = SV_ON;
            nrSource444Mode = 0;
            break;
        case E_DI_NR_YC_420:
            _2DNROnOff = SV_ON;
            _3DNROnOff = SV_ON;
            nrSource444Mode = 0;
            break;
        case E_DI_NR_YC_422:
            _2DNROnOff = SV_ON;
            _3DNROnOff = SV_ON;
            nrSource444Mode = 0;
            break;
        case E_DI_NR_RGB:
            _2DNROnOff = SV_OFF;
            _3DNROnOff = SV_ON;
            nrSource444Mode = 1;
            break;
        case E_DI_NR_OFF:
        case E_DI_NR_UNKNOWN:
            _2DNROnOff = SV_OFF;
            _3DNROnOff = SV_OFF;
            nrSource444Mode = 0;
            break;
        default:
            _2DNROnOff = SV_OFF;
            _3DNROnOff = SV_OFF;
            nrSource444Mode = 0;
            return;
    }

    vDrvNRSet2DNROnOff(_2DNROnOff);
    vDrvNRSet3DNROnOff(_3DNROnOff);        

    vRegWriteFldAlign(PAT_MAIN_00, nrSource444Mode, C_MAIN_SOURCE_444); //Only enabled for 444

    //when 420 mode and have overscan, set c_sw_init to 1
    if(_NRParam.bSrm3DNRMode == E_DI_NR_YC_420)
    {
        if(_NRParam.u2XOffset == 0 && 
           _NRParam.u2YOffset == 0)
        {
            //no overscan
            vRegWriteFldAlign(NR_3DNR00,0x0,c_sw_init);
        }
        else
        {
            //with overscan
            vRegWriteFldAlign(NR_3DNR00,0x1,c_sw_init);
        }
    }

    //config vga bit
    //vDrvNRCheckVGAModeOnOff();    //Removed. Due to Is444 not ready yet.
                                    //vDrvNRCheckVGAModeOnOff() would be set when mode change done.
}


/**
 * @brief Set 3D NR IIR On/ Off.
 *  This function will turn off all 3D NR function,
 *  include dram access.
 * @param bOnOff SV_ON/SV_OFF
 */
void vDrvNRSet3DNROnOff(UINT8 bOnOff)
{
    LOG(3,"Set 3D NR(%d)\n",bOnOff);
	_NRParam.b3DNROnOff = bOnOff;
	vRegWriteFldAlign(NR_3DNR00, bOnOff, TNRenable);	
	
	//determine 3D YC should be on/ off
	vDrvNRDetermine3DNRYCOnoff();
}

/**
 * @brief Set 3D NR function On/ Off. Dram access still working.
 *  When user select 3D NR On/ Off in OSD.
 *  This function could be called to turn on/ off 3D NR function.
 *  Be noticed that dram access will still work.
 * @param bOnOff SV_ON/SV_OFF
 */
void vDrvNRSet3DNRUIOnOff(UINT8 bOnOff)
{
    LOG(3,"Set 3D NR UI(%d)\n",bOnOff);
	_NRParam.b3DNRUIOnOff =bOnOff;
	//determine 3D YC should be on/ off	
    vDrvNRDetermine3DNRYCOnoff();
}

static void vDrvNRDetermine3DNRYCOnoff(void)
{
    UINT8 bYOnOff = SV_OFF;
    UINT8 bCOnOff = SV_OFF;

    if(_NRParam.b3DNRUIOnOff == SV_OFF)
    {
        bYOnOff = SV_OFF;
        bCOnOff = SV_OFF;
    }
    else
    {
        if(_NRParam.bSrm3DNRMode == E_DI_NR_OFF ||
           _NRParam.bSrm3DNRMode == E_DI_NR_UNKNOWN)
        {
            bYOnOff = SV_OFF;
            bCOnOff = SV_OFF;            
        }
        else if(_NRParam.bSrm3DNRMode == E_DI_NR_Y_ONLY)
        {
            bYOnOff = SV_ON;
            bCOnOff = SV_OFF;        
        }
        else if(_NRParam.bSrm3DNRMode == E_DI_NR_YC_420 ||
                _NRParam.bSrm3DNRMode == E_DI_NR_YC_422 ||
                _NRParam.bSrm3DNRMode == E_DI_NR_RGB)
        {
            bYOnOff = SV_ON;
            bCOnOff = SV_ON;        
        }
        else
        {
            bYOnOff = SV_OFF;
            bCOnOff = SV_OFF;                    
        }
    }    
    
	vRegWriteFldAlign(NR_3DNR07, bYOnOff, enytiir);
	vRegWriteFldAlign(NR_3DNR07, bCOnOff, enctiir);
	LOG(3,"Set 3D NR Y(%d) C(%d)\n",bYOnOff,bCOnOff);
	
}

void vDrv2DNRCliOnOff(UINT8 bMode)
{
}

void vDrv3DNRCliOnOff(UINT8 bMode)
{
}

/**
 * @brief Set parameters according to signal characteristics
 * @param bIs525 SV_TRUE(525)/SV_FALSE(625)
 */
void vDrvNRSetSystem(void)
{
    //1. Check NR should switch to Main or Sub and if NR should be bypassed.
	bDrvNRDeterminePath();
    //2.set NR Path
    vDrvNRSetCurrentPath(_NRParam.bNRPath);
    //3. if NR should be bypassed. 
    if(_NRParam.bForceNRBypass == SV_ON ||
       _NRParam.bNRBypass == SV_ON)
	{
	    //set NR bypass
		vDrvNRBypass(SV_ON);
		//Skip other actions.
		return;
	}
	else
	{
	    vDrvNRBypass(SV_OFF);
	}
	//4. check if input size changed
	vDrvNRCheckInputSize();
    //5. Update chroma line average register
    vDrvNRSetChromaLineAvgModeReg(_TVD_path,_TVD_Chroma_mode);
    
    //6. load quality table
    vDrvNRLoadQualityTable();

    //7. check VGA mode
    vDrvNRCheckVGAModeOnOff();    //move this function call to setDatamode for PHILIPS VGA problem

    //8. force chroma average for SECAM mode
    vRegWriteFldAlign(NR_3DNR02, (_rTvd3dStatus.bTvdMode == SV_CS_SECAM), tavgmode);
}

void vDrvNRProc(void)
{  
    //1. check adaptive flag
    #if 0
    if (!fgIsAdaptiveFlgSet(FLG_ADAPTIVE_NR))
	{
        return;
    }
    #endif
    //2. check fifo length??
     vDrvNRCheckInputSize();

    //Auto DeBlocking
    #if ADB_ENABLE
    if (fgIsAdaptiveFlgSet(FLG_ADAPTIVE_DB))
    {
         _Auto_DB_Status = 0;
         vDrvAutoDB();
    }
    #endif

//. Auto NR   
#if ANM_ENABLE

    if(NMEnable)
    {
        DynamicNRCurLevel = DRVCUST_NoiseLevel();
        
        if( DynamicNRCurLevel != NM_INVALIDLEVEL)
        {
            vDrvDynamicNR(DynamicNRCurLevel);
        }
                
    }
    else
    {
        DynamicNRCurLevel = NM_INVALIDLEVEL;
    }
#endif  //ANM_ENABLE


#if NR_BASIC_MODE_ENABLED		
    vUpdateNRBasicModeRegister();
#endif 

#if NR_ENABLE_ROUNDING_TOGGLE_ON_OFF
    vDrvNRToggleRoundingOnOff();
#endif

    if (fgIsAdaptiveFlgSet(FLG_ADAPTIVE_NR))
    {
        vDrvColorFrmStChg();
    }

    #if NR_ADAPTIVE_DEBA_ENABLED
    vDrvAdaptiveDebaCheckState();
    #endif
}

void vDrvNRSetNrBufferCb(UINT32 u4Address, UINT32 u4Width, UINT32 u4Height,
                            BOOL fg10bit, BOOL fgLumaOnly)
{
	//UINT32 u4DramSize;

    LOG(3, "%s, a=0x%x, w=%d, h=%d\n", __FUNCTION__, u4Address, u4Width, u4Height);

	//u4DramSize = (u4Width*u4Height*(2-fgLumaOnly)*8+(6-fg10bit))/(7-fg10bit);
	//vDrvNRSetBufRegion(u4Address , u4Address+u4DramSize);
}


void vDrvNRSrmCb(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4PDSize, UINT32 u4SramMode)
{
    FBM_POOL_T* prPool;

    if (u4Arg1 != VDP_1)
    {
        return;
    }

    prPool = FBM_GetPoolInfoAuto((UCHAR)FBM_POOL_TYPE_NR, NULL);
    if (prPool != NULL)
    {
        vDrvNRSetNrBufferCb(prPool->u4Addr, prPool->u4Width, prPool->u4Height,
                         (u4Arg2 & FBM_POOL_MODE_10BIT) > 0,
                         (u4Arg2 & FBM_POOL_MODE_NR_3D_Y_ONLY) > 0);
    }
    else
    {
        LOG(1, "_VdpSrmNrCb: fail to get pool\n");
    }
#if 0
    if (u4Arg2 & FBM_POOL_MODE_NR_BYPASS)
    {
        //3D NR OFF
        vApiVideoNRTIIROnOff(SV_OFF);
        //2D NR OFF
        vApiVideo2DNROnOff(SV_OFF);
    }
    else if (u4Arg2 & FBM_POOL_MODE_NR_3D_Y_C)
    {
        //3D NR ON
        vApiVideoNRTIIROnOff(SV_ON);
    }
    else if (u4Arg2 & FBM_POOL_MODE_NR_3D_Y_ONLY)
    {
        //3D NR ON
        vApiVideoNRTIIROnOff(SV_ON);
    }
    else if (u4Arg2 & FBM_POOL_MODE_NR_2D)
    {
        //3D NR OFF
        vApiVideoNRTIIROnOff(SV_OFF);
    }
    else
    {
    }
#endif
}

/**
 * @brief Set 3D NR Freeze on/off
 * @param bOnOff SV_ON/SV_OFF
 */
void vDrvNRSetFreeze(UINT8 bOnOff)
{
    vRegWriteFldAlign(NR_3DNR02, bOnOff, freeze);
}

void vDrvNRSet3DINKDebugMode(UINT8 bOnOff)
{
#ifdef PSCAN_8223_PORTING_FINISH //MARK FOR PSCAN 8223 PROTING JOB
    UINT32 u4MainAddrYMsb,u4MainAddrYLsb;
    UINT32 u4MainAddrUMsb,u4MainAddrULsb;
    UINT32 u4MainAddrLoLimit,u4MainAddrHiLimit;
    //UINT32 u4MainAddrVMsb,u4MainAddrVLsb;

    UINT32 u4SubAddrYMsb,u4SubAddrYLsb;
    UINT32 u4SubAddrUMsb,u4SubAddrULsb;
    UINT32 u4SubAddrVMsb,u4SubAddrVLsb;
    UINT32 u4SubAddrLoLimit,u4SubAddrHiLimit;
   
    if(bOnOff == SV_ON)
    {
        #if defined(CC_MT5387) || defined(CC_MT5363)
        //keep current PSCAN PIP base Address
        _NRParam.NR3DInkInfo.u4AddrYMsb = MDDI_READ_FLD(VDP_2, MDDI_DA_01, RG_ADDR_BASE_MSB_Y);
        _NRParam.NR3DInkInfo.u4AddrYLsb = MDDI_READ_FLD(VDP_2, MDDI_DA_02, RG_ADDR_BASE_LSB_Y);
        _NRParam.NR3DInkInfo.u4AddrUMsb = MDDI_READ_FLD(VDP_2, MDDI_DA_03, RG_ADDR_BASE_MSB_U);
        _NRParam.NR3DInkInfo.u4AddrULsb = MDDI_READ_FLD(VDP_2, MDDI_DA_04, RG_ADDR_BASE_LSB_U);
        _NRParam.NR3DInkInfo.u4AddrVMsb = MDDI_READ_FLD(VDP_2, MDDI_DA_05, RG_ADDR_BASE_MSB_V);
        _NRParam.NR3DInkInfo.u4AddrVLsb = MDDI_READ_FLD(VDP_2, MDDI_DA_06, RG_ADDR_BASE_LSB_V);    
        _NRParam.NR3DInkInfo.u4AddrLoLimit = MDDI_READ_FLD(VDP_2, MDDI_DA_08,RG_WADDR_LO_LIMIT);
        _NRParam.NR3DInkInfo.u4AddrHiLimit = MDDI_READ_FLD(VDP_2, MDDI_DA_07,RG_WADDR_HI_LIMIT);
        //set new base address according to PSCAN main base address
        u4MainAddrYMsb = MDDI_READ_FLD(VDP_1, MDDI_DA_01, RG_ADDR_BASE_MSB_Y);
        u4MainAddrYLsb = MDDI_READ_FLD(VDP_1, MDDI_DA_02, RG_ADDR_BASE_LSB_Y);
        u4MainAddrUMsb = MDDI_READ_FLD(VDP_1, MDDI_DA_03, RG_ADDR_BASE_MSB_U);
        u4MainAddrULsb = MDDI_READ_FLD(VDP_1, MDDI_DA_04, RG_ADDR_BASE_LSB_U);
        //u4MainAddrVMsb = MDDI_READ_FLD(VDP_1, MDDI_DA_05, RG_ADDR_BASE_MSB_V);
        //u4MainAddrVLsb = MDDI_READ_FLD(VDP_1, MDDI_DA_06, RG_ADDR_BASE_LSB_V);
        u4MainAddrLoLimit = MDDI_READ_FLD(VDP_1, MDDI_DA_08,RG_WADDR_LO_LIMIT);
        u4MainAddrHiLimit = MDDI_READ_FLD(VDP_1, MDDI_DA_07,RG_WADDR_HI_LIMIT);
        #else
        //keep current PSCAN PIP base Address
        _NRParam.NR3DInkInfo.u4AddrYMsb = MDDI_READ_FLD(VDP_2, MDDI_MC_06, DA_ADDR_BASE_MSB_Y);
        _NRParam.NR3DInkInfo.u4AddrYLsb = MDDI_READ_FLD(VDP_2, MDDI_MC_07, DA_ADDR_BASE_LSB_Y);
        _NRParam.NR3DInkInfo.u4AddrUMsb = MDDI_READ_FLD(VDP_2, MDDI_MC_08, DA_ADDR_BASE_MSB_U);
        _NRParam.NR3DInkInfo.u4AddrULsb = MDDI_READ_FLD(VDP_2, MDDI_MC_09, DA_ADDR_BASE_LSB_U);
        _NRParam.NR3DInkInfo.u4AddrVMsb = MDDI_READ_FLD(VDP_2, MDDI_MC_0A, DA_ADDR_BASE_MSB_V);
        _NRParam.NR3DInkInfo.u4AddrVLsb = MDDI_READ_FLD(VDP_2, MDDI_MC_0B, DA_ADDR_BASE_LSB_V);    
        _NRParam.NR3DInkInfo.u4AddrLoLimit = MDDI_READ_FLD(VDP_2, MDDI_MC_0F, DA_WADDR_LO_LIMIT);
        _NRParam.NR3DInkInfo.u4AddrHiLimit = MDDI_READ_FLD(VDP_2, MDDI_MC_0E, DA_WADDR_HI_LIMIT);
        //set new base address according to PSCAN main base address
        u4MainAddrYMsb = MDDI_READ_FLD(VDP_1, MDDI_MC_06, DA_ADDR_BASE_MSB_Y);
        u4MainAddrYLsb = MDDI_READ_FLD(VDP_1, MDDI_MC_07, DA_ADDR_BASE_LSB_Y);
        u4MainAddrUMsb = MDDI_READ_FLD(VDP_1, MDDI_MC_08, DA_ADDR_BASE_MSB_U);
        u4MainAddrULsb = MDDI_READ_FLD(VDP_1, MDDI_MC_09, DA_ADDR_BASE_LSB_U);
        //u4MainAddrVMsb = MDDI_READ_FLD(VDP_1, MDDI_MC_0A, DA_ADDR_BASE_MSB_V);
        //u4MainAddrVLsb = MDDI_READ_FLD(VDP_1, MDDI_MC_0B, DA_ADDR_BASE_LSB_V);
        u4MainAddrLoLimit = MDDI_READ_FLD(VDP_1, MDDI_MC_0F,DA_WADDR_LO_LIMIT);
        u4MainAddrHiLimit = MDDI_READ_FLD(VDP_1, MDDI_MC_0E,DA_WADDR_HI_LIMIT);
        #endif
        
        u4SubAddrYMsb = _NRParam.NR3DInkInfo.u4AddrYMsb;
        u4SubAddrYLsb = u4SubAddrYMsb + (u4MainAddrYLsb - u4MainAddrYMsb);
        u4SubAddrUMsb = u4SubAddrYLsb + (u4MainAddrUMsb - u4MainAddrYLsb);
        u4SubAddrULsb = u4SubAddrUMsb + (u4MainAddrULsb - u4MainAddrUMsb);
        u4SubAddrVMsb = u4SubAddrUMsb;
        u4SubAddrVLsb = u4SubAddrULsb;
        u4SubAddrLoLimit = u4SubAddrYMsb;
        u4SubAddrHiLimit = u4SubAddrYMsb + (u4MainAddrHiLimit - u4MainAddrLoLimit);

        #if defined(CC_MT5387) || defined(CC_MT5363)
        MDDI_WRITE_FLD(VDP_2, MDDI_DA_01, u4SubAddrYMsb, RG_ADDR_BASE_MSB_Y);
        MDDI_WRITE_FLD(VDP_2, MDDI_DA_02, u4SubAddrYLsb, RG_ADDR_BASE_LSB_Y);
        MDDI_WRITE_FLD(VDP_2, MDDI_DA_03, u4SubAddrUMsb, RG_ADDR_BASE_MSB_U);
        MDDI_WRITE_FLD(VDP_2, MDDI_DA_04, u4SubAddrULsb, RG_ADDR_BASE_LSB_U);
        MDDI_WRITE_FLD(VDP_2, MDDI_DA_05, u4SubAddrVMsb, RG_ADDR_BASE_MSB_V);
        MDDI_WRITE_FLD(VDP_2, MDDI_DA_06, u4SubAddrVLsb, RG_ADDR_BASE_LSB_V);        
        MDDI_WRITE_FLD(VDP_2, MDDI_DA_08, u4SubAddrLoLimit, RG_WADDR_LO_LIMIT);
        MDDI_WRITE_FLD(VDP_2, MDDI_DA_07, u4SubAddrHiLimit, RG_WADDR_HI_LIMIT);                        
        #else
        MDDI_WRITE_FLD(VDP_2, MDDI_MC_06, u4SubAddrYMsb, DA_ADDR_BASE_MSB_Y);
        MDDI_WRITE_FLD(VDP_2, MDDI_MC_07, u4SubAddrYLsb, DA_ADDR_BASE_LSB_Y);
        MDDI_WRITE_FLD(VDP_2, MDDI_MC_08, u4SubAddrUMsb, DA_ADDR_BASE_MSB_U);
        MDDI_WRITE_FLD(VDP_2, MDDI_MC_09, u4SubAddrULsb, DA_ADDR_BASE_LSB_U);
        MDDI_WRITE_FLD(VDP_2, MDDI_MC_0A, u4SubAddrVMsb, DA_ADDR_BASE_MSB_V);
        MDDI_WRITE_FLD(VDP_2, MDDI_MC_0B, u4SubAddrVLsb, DA_ADDR_BASE_LSB_V);        
        MDDI_WRITE_FLD(VDP_2, MDDI_MC_0F, u4SubAddrLoLimit, DA_WADDR_LO_LIMIT);
        MDDI_WRITE_FLD(VDP_2, MDDI_MC_0E, u4SubAddrHiLimit, DA_WADDR_HI_LIMIT);                        
        #endif
        
        //enable 3d ink debug mode
        vRegWriteFldAlign(NR_3DNR00, 0x1, enink_path);
        vRegWriteFldAlign(NR_3DNR00, 0x1, nr_main_pip_sel);
        vRegWriteFldAlign(NR_3DNR00, 0x1, nr_bit_sel_rv);
        vRegWriteFldAlign(NR_3DNR00, 0x1, nr_bit_sel_ru);
        vRegWriteFldAlign(NR_3DNR00, 0x1, nr_bit_sel_ry);        
    }
    else
    {
        #if defined(CC_MT5387) || defined(CC_MT5363)
        //set PSCAN PIP base address back     
        MDDI_WRITE_FLD(VDP_2, MDDI_DA_01, _NRParam.NR3DInkInfo.u4AddrYMsb, RG_ADDR_BASE_MSB_Y);
        MDDI_WRITE_FLD(VDP_2, MDDI_DA_02, _NRParam.NR3DInkInfo.u4AddrYLsb, RG_ADDR_BASE_LSB_Y);
        MDDI_WRITE_FLD(VDP_2, MDDI_DA_03, _NRParam.NR3DInkInfo.u4AddrUMsb, RG_ADDR_BASE_MSB_U);
        MDDI_WRITE_FLD(VDP_2, MDDI_DA_04, _NRParam.NR3DInkInfo.u4AddrULsb, RG_ADDR_BASE_LSB_U);
        MDDI_WRITE_FLD(VDP_2, MDDI_DA_05, _NRParam.NR3DInkInfo.u4AddrVMsb, RG_ADDR_BASE_MSB_V);
        MDDI_WRITE_FLD(VDP_2, MDDI_DA_06, _NRParam.NR3DInkInfo.u4AddrVLsb, RG_ADDR_BASE_LSB_V);        
        MDDI_WRITE_FLD(VDP_2, MDDI_DA_08, _NRParam.NR3DInkInfo.u4AddrLoLimit, RG_WADDR_LO_LIMIT);
        MDDI_WRITE_FLD(VDP_2, MDDI_DA_07, _NRParam.NR3DInkInfo.u4AddrHiLimit, RG_WADDR_HI_LIMIT);        
        #else
        //set PSCAN PIP base address back     
        MDDI_WRITE_FLD(VDP_2, MDDI_MC_06, _NRParam.NR3DInkInfo.u4AddrYMsb, DA_ADDR_BASE_MSB_Y);
        MDDI_WRITE_FLD(VDP_2, MDDI_MC_07, _NRParam.NR3DInkInfo.u4AddrYLsb, DA_ADDR_BASE_LSB_Y);
        MDDI_WRITE_FLD(VDP_2, MDDI_MC_08, _NRParam.NR3DInkInfo.u4AddrUMsb, DA_ADDR_BASE_MSB_U);
        MDDI_WRITE_FLD(VDP_2, MDDI_MC_09, _NRParam.NR3DInkInfo.u4AddrULsb, DA_ADDR_BASE_LSB_U);
        MDDI_WRITE_FLD(VDP_2, MDDI_MC_0A, _NRParam.NR3DInkInfo.u4AddrVMsb, DA_ADDR_BASE_MSB_V);
        MDDI_WRITE_FLD(VDP_2, MDDI_MC_0B, _NRParam.NR3DInkInfo.u4AddrVLsb, DA_ADDR_BASE_LSB_V);        
        MDDI_WRITE_FLD(VDP_2, MDDI_MC_0F, _NRParam.NR3DInkInfo.u4AddrLoLimit, DA_WADDR_LO_LIMIT);
        MDDI_WRITE_FLD(VDP_2, MDDI_MC_0E, _NRParam.NR3DInkInfo.u4AddrHiLimit, DA_WADDR_HI_LIMIT);        
        #endif
        
        //disable 3d ink debug mode
        vRegWriteFldAlign(NR_3DNR00, 0x0, enink_path);
        vRegWriteFldAlign(NR_3DNR00, 0x0, nr_main_pip_sel);
        vRegWriteFldAlign(NR_3DNR00, 0x0, nr_bit_sel_rv);
        vRegWriteFldAlign(NR_3DNR00, 0x0, nr_bit_sel_ru);
        vRegWriteFldAlign(NR_3DNR00, 0x0, nr_bit_sel_ry);        
    }
#endif    
}

static void vDrvNRCheckVGAModeOnOff(void)
{
    UINT8 fgVGAOnOff = SV_FALSE;

    //rule 1: if 3D NR is in RGB mode.
    if(_NRParam.bSrm3DNRMode == E_DI_NR_RGB)
    {
        fgVGAOnOff = SV_TRUE;
    }

    //rule 2: if input is 444 and 3D NR is OFF
    if(bIsScalerInput444(_NRParam.bNRPath) && _NRParam.bSrm3DNRMode == E_DI_NR_OFF)
    {
    	fgVGAOnOff = SV_TRUE;
    }

    vRegWriteFldAlign(NR_3DNR05, fgVGAOnOff, vga3d);            
}

#if NR_DISABLE_PIXEL_REP_WHEN_BLANKING_IS_SAMLL
/**
 * @brief Check if boundary repeat should be turned on/off
 * @param bPath SV_VP_MAIN/SV_VP_PIP
 */
static void vDrvNRCheckBourndaryRepeat(UINT8 bPath)
{        
    UINT16 u2PreDownBlanking = 0;    
    UINT16 hTotal;
    UINT16 hActive;

    //get HTotal
    hTotal = wDrvVideoGetHTotal(bPath);                
    hActive = bDrvVideoGetWidthOverSample(bPath);    
    u2PreDownBlanking = hTotal - hActive;

	u2PreDownBlanking = (u2PreDownBlanking * u4ScalerGetHPS(bPath)) / 0x8000;
	
    LOG(3,"Blank size after pre-scaling down(%d)\n",u2PreDownBlanking);
    //disable boundary repeat when h blank(after pre-scaling down) is smaller than 40
	if(u2PreDownBlanking<= 40)
	{
	    vRegWriteFldAlign(NR_2DNR1E, 0x1, rbBoundaryRepeatDis);
	    LOG(3,"Disable bourndary repeat function in NR\n");
	}
	else
	{
	    vRegWriteFldAlign(NR_2DNR1E, 0x0, rbBoundaryRepeatDis);	
	}
}
#endif

static void vDrvNRCheckVBoundProtect(void)
{
    if(_NRParam.u2XOffset!=0 ||
       _NRParam.u2YOffset!=0)
    {
        vRegWriteFldAlign(NR_3DNR00, 0x0, c_v_bound_protect);
    }
    else
    {
        vRegWriteFldAlign(NR_3DNR00, 0x1, c_v_bound_protect);
    }
}

void vDrvNRGetNRStatus(void)
{
    //1. print nr parameter
    Printf("NR is on %s Path.\n",(_NRParam.bNRPath==SV_VP_MAIN)?"Main":"Sub");
    Printf("NR is %s bypassed.\n",(_NRParam.bNRBypass==SV_TRUE || _NRParam.bForceNRBypass==SV_TRUE)?"":"NOT");
    Printf("2D NR is turned %s.\n",(_NRParam.b2DNROnOff==SV_ON)?"ON":"OFF");
    Printf("3D NR is turned %s.\n",(_NRParam.b3DNROnOff==SV_ON)?"ON":"OFF");    
    Printf("NR Width(%d) Height(%d)\n",_NRParam.u2NRWidth,_NRParam.u2NRHeight);
    //2. print sync regen info
    if(_NRParam.bNRPath==SV_VP_MAIN)
    {
    	Printf("Sync Regen is turned %s\n", (RegReadFldAlign(PAT_MAIN_00, C_MAIN_SYNC_MODIFY) == 1)?"ON":"OFF");
        Printf("Sync Regen size (HSTART,HEND,VSTART,VEND) = (%d,%d,%d,%d)\n",
    			RegReadFldAlign(PAT_MAIN_01, C_MAIN_SELF_HSTART),
    			RegReadFldAlign(PAT_MAIN_01, C_MAIN_SELF_HEND),
    			RegReadFldAlign(PAT_MAIN_02, C_MAIN_SELF_VSTART),
    			RegReadFldAlign(PAT_MAIN_02, C_MAIN_SELF_VEND));
    }
    #if 0 //remove pip for 8223 porting
    else
    {
    	Printf("Sync Regen is turned %s\n", (RegReadFldAlign(PAT_PIP_00, C_PIP_SYNC_MODIFY) == 1)?"ON":"OFF");
        Printf("Sync Regen size (HSTART,HEND,VSTART,VEND) = (%d,%d,%d,%d)\n",
    			RegReadFldAlign(PAT_PIP_01, C_PIP_SELF_HSTART),
    			RegReadFldAlign(PAT_PIP_01, C_PIP_SELF_HEND),
    			RegReadFldAlign(PAT_PIP_02, C_PIP_SELF_VSTART),
    			RegReadFldAlign(PAT_PIP_02, C_PIP_SELF_VEND));    
    }
    #endif
}

#if NR_ENABLE_ROUNDING_TOGGLE_ON_OFF
void vDrvNRToggleRoundingOnOff()
{
    UINT8 bOnPeriod;
    UINT8 bOffPeriod;
    UINT8 bCurRoundingStatus;
    
    bOnPeriod = IO32ReadFldAlign(NR_BASIC_MODE_03, NR_ROUNDING_ON_PERIOD);
    bOffPeriod = IO32ReadFldAlign(NR_BASIC_MODE_03, NR_ROUNDING_OFF_PERIOD);
    bCurRoundingStatus = RegReadFldAlign(NR_3DNR02, enwcufix);
        
    if(bOnPeriod==0 && bOffPeriod==0)
    {
        bRoundingOnCounter = 0;
        bRoundingOffCounter = 0;
        return;
    }

    if(bCurRoundingStatus == 1) //rounding is on
    {
        bRoundingOnCounter++;
       
        if(bRoundingOnCounter>=bOnPeriod)
        {
            //turn off rounding
            vRegWriteFldAlign(NR_3DNR02, 0, enwcufix);
            //reset counter
            bRoundingOnCounter = 0;
            bRoundingOffCounter = 0;
        }
    }
    else
    {
        bRoundingOffCounter++;
        if(bRoundingOffCounter>=bOffPeriod)
        {
            //turn on rounding
            vRegWriteFldAlign(NR_3DNR02, 1, enwcufix);
            //reset counter
            bRoundingOnCounter = 0;
            bRoundingOffCounter = 0;            
        }
    }    
}
#endif


#if NR_ADAPTIVE_DEBA_ENABLED
NR_ADAPTIVE_DEBA_STATE eDebaState;
UINT8 bCounterToStartMax = 20;     //have to count exceed this counter before starting accumulate
UINT8 bCounterToStartCur = 0;
UINT8 bRoundCounterMax = 30;
UINT8 bRoundCounterCur = 0;
UINT32 accumulatedLines[2] = {0,0};

void vDrvAdaptiveDebaSetState(NR_ADAPTIVE_DEBA_STATE state)
{
    eDebaState = state;
    LOG(6,"[DEBA]Change state to (%d)\n",eDebaState);
    vIO32WriteFldAlign(NR_ADAPTIVE_DEBA_02,eDebaState,NR_ADAPTIVE_DEBA_CUR_STATE);
}

NR_ADAPTIVE_DEBA_STATE vDrvAdaptiveDebaGetState(void)
{
    return eDebaState;
}

UINT8 bDrvAdaptiveDebaGetVLineCount(void)
{
    return RegReadFldAlign(NR_2DNR8F, rVLineCnt);
}

void vDrvAdaptiveDebaSetHDelay(UINT8 delay)
{
    vRegWriteFldAlign(NR_2DNR63, delay, rbkmeter_hdelay);
}

UINT8 bDrvAdaptiveDebaGetHDelay(void)
{
    return RegReadFldAlign(NR_2DNR63, rbkmeter_hdelay);
}

void vDrvAdaptiveDebaCheckState(void)
{
    UINT8 delay = 0;
    UINT8 bStateChange = SV_FALSE;
    UINT32 thl;

    switch(vDrvAdaptiveDebaGetState())
    {
        case NR_ADAPTIVE_DEBA_STATE_INIT:
            accumulatedLines[0] = 0;
            accumulatedLines[1] = 0;
            bCounterToStartCur = 0;
            bRoundCounterCur = 0;
        break;

        case NR_ADAPTIVE_DEBA_STATE_RESET:
            accumulatedLines[0] = 0;
            accumulatedLines[1] = 0;
            bCounterToStartCur = 0;
            bRoundCounterCur = 0;
            vDrvAdaptiveDebaSetState(NR_ADAPTIVE_DEBA_STATE_CONFIG0);
        break;
        
        case NR_ADAPTIVE_DEBA_STATE_CONFIG0:
            //set to config 0
            vDrvAdaptiveDebaSetHDelay(0);

            //before count to MAX, do nothing
            if(bCounterToStartCur < bCounterToStartMax)
            {
                bCounterToStartCur++;
                return;
            }
            else
            {
                //start to accumulate
                accumulatedLines[0] += bDrvAdaptiveDebaGetVLineCount();
                bRoundCounterCur++;
                LOG(6,"[DEBA]STATE_CONFIG0 (CNT,VLINES,ACCUMULATED_LINES)=(%d,%d,%d)\n",bRoundCounterCur,bDrvAdaptiveDebaGetVLineCount(),accumulatedLines[0]);            
            }
            
            if(bRoundCounterCur>bRoundCounterMax)
            {
                bCounterToStartCur = 0;
                bRoundCounterCur = 0;
                vDrvAdaptiveDebaSetState(NR_ADAPTIVE_DEBA_STATE_CONFIG1);
            }
                        
        break;
        
        case NR_ADAPTIVE_DEBA_STATE_CONFIG1:
            //set to config 1
            vDrvAdaptiveDebaSetHDelay(1);

            //before count to MAX, do nothing
            if(bCounterToStartCur < bCounterToStartMax)
            {
                bCounterToStartCur++;
                return;
            }
            else
            {
                //start to accumulate
                accumulatedLines[1] += bDrvAdaptiveDebaGetVLineCount();
                bRoundCounterCur++;
                LOG(6,"[DEBA]STATE_CONFIG1 (CNT,VLINES,ACCUMULATED_LINES)=(%d,%d,%d)\n",bRoundCounterCur,bDrvAdaptiveDebaGetVLineCount(),accumulatedLines[1]);        
            }
            
            if(bRoundCounterCur>bRoundCounterMax)
            {
                bCounterToStartCur = 0;
                bRoundCounterCur = 0;
                vDrvAdaptiveDebaSetState(NR_ADAPTIVE_DEBA_STATE_COMPARE);
            }
            
        break;
        
        case NR_ADAPTIVE_DEBA_STATE_COMPARE:

            if(accumulatedLines[0] >= accumulatedLines[1])
            {
                delay = 0;
            }
            else
            {
                delay = 1;
            }

            vDrvAdaptiveDebaSetHDelay(delay);
            LOG(6,"[DEBA]STATE_COMPARE Set Delay to (%d)\n",delay);            
            vDrvAdaptiveDebaSetState(NR_ADAPTIVE_DEBA_STATE_MONITOR);            
        break;
        
        case NR_ADAPTIVE_DEBA_STATE_MONITOR:

            //before count to MAX, do nothing
            if(bCounterToStartCur < bCounterToStartMax)
            {
                bCounterToStartCur++;
                return;
            }

            thl = (bDrvAdaptiveDebaGetHDelay()==0)? 
                  (accumulatedLines[0]/(bRoundCounterMax*2)): 
                  (accumulatedLines[1]/(bRoundCounterMax*2));

            //update S/W Register
            vIO32WriteFldAlign(NR_ADAPTIVE_DEBA_00,accumulatedLines[0],NR_ADAPTIVE_DEBA_ACCUMULATE0);
            vIO32WriteFldAlign(NR_ADAPTIVE_DEBA_01,accumulatedLines[1],NR_ADAPTIVE_DEBA_ACCUMULATE1);
            vIO32WriteFldAlign(NR_ADAPTIVE_DEBA_02,bDrvAdaptiveDebaGetHDelay(),NR_ADAPTIVE_DEBA_HDELAY);
            vIO32WriteFldAlign(NR_ADAPTIVE_DEBA_02,bDrvAdaptiveDebaGetVLineCount(),NR_ADAPTIVE_DEBA_VLINE_CNT);
            vIO32WriteFldAlign(NR_ADAPTIVE_DEBA_02,thl,NR_ADAPTIVE_DEBA_THL);
            
            if(bDrvAdaptiveDebaGetVLineCount() <= thl)
            {
                bStateChange = SV_TRUE;
            }

            if(bDrvAdaptiveDebaGetVLineCount() <= 5)
            {
                bStateChange = SV_TRUE;
            }

            if(bStateChange == SV_TRUE)
            {               
                vDrvAdaptiveDebaSetState(NR_ADAPTIVE_DEBA_STATE_RESET);
                LOG(6,"[DEBA]State Changed from STATE_MONITOR to RESET VLine(%d) < Thl(%d)\n",bDrvAdaptiveDebaGetVLineCount(),thl);
            }
        break;
    }
}
#endif

