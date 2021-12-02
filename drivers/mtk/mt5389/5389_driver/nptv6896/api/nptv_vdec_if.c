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
 * $RCSfile: nptv_vdec_if.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/


//#define _API_VIDEO_C_
#ifdef CC_UP8032_ATV
#include "general_mt82.h"
#endif
#include "nptv_vdec_if.h"
#include "general.h"
#include "hw_vdoin.h"
#include "drv_video.h"
#include "video_def.h"
#include "nptv_drvif.h"
#include "source_table.h"
#include "source_select.h"
#include "api_backlt.h"
#include "api_eep.h"
#include "eepdef.h"
#include "util.h"
#include "drv_hdmi.h"
#include "drv_tvd.h"
#include "drv_vga.h"
#include "drv_hdtv.h"
#include "drv_vdoclk.h"
#ifdef CC_COPLAT_MT82
#include "drv_auto.h"
#include "hw_async.h"
#endif

/**
 * @brief vApiVideoVgaSetPhase(UINT8 bPath, UINT8 bValue) 
 * Set Vga phase
 * @param  bPath :main/sub path (SV_VP_MAIN / SV_VP_PIP)
 * @param bValue  : Phase value (0~31)
 * @retval void
 * @example vApiVideoVgaSetPhase(SV_VP_MAIN, 5);
 */
UINT8 fgApiVideoVgaSetPhase(UINT8 bValue) 
{
    if (fgIsMainVga()||fgIsMainYPbPr()||fgIsPipVga()|| fgIsPipYPbPr())
    {
        vDrvVGASetPhase(bValue);
    }
    else
    {
        return (FALSE);
    }

    return (TRUE);
}

/**
 * @brief vApiVideoVgaSetClock(UINT8 bPath, UINT16 wValue) 
 * Set VGA Clock value
 * @param  bPath :main/sub path (SV_VP_MAIN / SV_VP_PIP)
 * @param wValue  : clock  value (not UI value)
 * @retval void
 * @example vApiVideoVgaSetClock(SV_VP_MAIN, 1024);
 */
UINT8 fgApiVideoVgaSetClock(UINT16 wValue) 
{
    //for 5371 mw if
    if(fgIsMainVga()||fgIsPipVga())        
        wValue=Get_VGAMODE_IHTOTAL(_bVgaTiming) +wValue-127;
    else    wValue=Get_VGAMODE_IHTOTAL(_bHdtvTiming) +wValue-127;
    if(wValue<300)
    {
        wValue=300;
//        ASSERT(0);
    }


    if(((fgIsMainVga()||fgIsPipVga())&&(!bVgaInterlace()))  ||    
           ((fgIsMainYPbPr()||fgIsPipYPbPr())&&(!bHdtvInterlace())))
    {
        #if CHANGE_SDDS_KPI
         Set_SDDS_KPI(1);
        #endif    
        vDrvVGASetClock(wValue);    
        #if CHANGE_SDDS_KPI
         vDrvEnableChang_SDDS_BW();
        #endif         
        return (TRUE);
    }
    else
    {
        return  (FALSE);    
    }
}

/**
 * @brief fgApiVideoGeoHPosition(UINT8 bPath, UINT16 wValue) 
 * Set VGA H Position
 * @param  bPath :main/sub path (SV_VP_MAIN / SV_VP_PIP)
 * @param wValue  : H Position (not UI value) (H porch)
 * @retval void
 * @example fgApiVideoGeoHPosition(SV_VP_MAIN, 100);
 */
UINT8 fgApiVideoGeoHPosition(UINT8 bPath, UINT16 wValue) 
{
#if 0
    //for 5371 mw if
    if(wValue>511)
        wValue=511;
    if(fgIsMainVga()||fgIsPipVga())        
        wValue=Get_VGAMODE_IPH_BP(_bVgaTiming) +256-wValue;
    else    wValue=Get_VGAMODE_IPH_BP(_bHdtvTiming) +256-wValue;
    if(wValue<30)
    {
        wValue=30;
//        ASSERT(0);
    }
#endif

    if (((bPath==SV_VP_MAIN) && (fgIsMainVga()||fgIsMainYPbPr())) ||
        ((bPath==SV_VP_PIP) && (fgIsPipVga()|| fgIsPipYPbPr())))    
    {
        vDrvSetHPosition(wValue);
    }
    else
    {
        return (FALSE);
    }

    return (TRUE);
}

/**
 * @brief fgApiVideoGeoVPosition(UINT8 bPath, UINT16 wValue) 
 * Set VGA V Position
 * @param  bPath :main/sub path (SV_VP_MAIN / SV_VP_PIP)
 * @param wValue  : V Position (not UI value) 
 * @retval void
 * @example fgApiVideoGeoVPosition(SV_VP_MAIN, 30);
 */
UINT8 fgApiVideoGeoVPosition(UINT8 bPath, UINT16 wValue) 
{

#if 0
    //for 5371 mw if
    if(wValue>511)
        wValue=511;
    if(fgIsMainVga()||fgIsPipVga())    
        wValue=Get_VGAMODE_IPV_STA(_bVgaTiming)-1+256-wValue;
    else    wValue=Get_VGAMODE_IPV_STA(_bHdtvTiming)-1+256-wValue;
    if(wValue<2)
    {
        wValue=2;
//        ASSERT(0);
    }
#endif


    if (((bPath==SV_VP_MAIN) && (fgIsMainVga()||fgIsMainYPbPr())) ||
        ((bPath==SV_VP_PIP) && (fgIsPipVga()|| fgIsPipYPbPr())))    
    {
        vDrvSetVPosition(wValue);
    }
    else
    {
        return (FALSE);
    }

    return (TRUE);
    
}

/**
 * @brief fgApiVideoVgaAuto(void) 
 * Trigger Auto state machine entry,Do it when path is VGA or DVI+ext AD
 * @param void
 * @retval void
 * @example vApiVideoVgaAuto(void);
 */
UINT8 fgApiVideoVgaAuto(void) 
{
    //By Adam to prevent the Vga auto postition error from tri-sync 

    //both vga timing & video timing call auto, and going to notify MW when done
    vDrvVgaAutoStart();
    return (TRUE);
}

void vApiVgaAutoStop(void)
{
    vDrvVgaAutoStop();
}

UINT8 bApiVgaSigStatus(void)
{
    return bVgaSigStatus();
}

/**
 * @brief fgApiVideoVgaAuto(void) 
 * Trigger Auto state machine entry,Do it when path is VGA or DVI+ext AD
 * @param void
 * @retval void
 * @example vApiVideoVgaAuto(void);
 */
UINT8 fgApiVideoYPbPrAuto(void)
{
    vDrvYPbPrAutoStart();
    return(TRUE);
}

/**
 * @brief fgApiVideoAutoColor(void) 
 * Trigger Auto Color state machine (VGA or YPbPr)
 * @param void
 * @retval UINT8
 * @example fgApiVideoAutoColor(void);
 */
UINT8 fgApiVideoAutoColor(void) 
{
    //UINT8 bInputType;                                                          //MC20081115 add for SCART
    //bInputType   = bGetICInputType(SV_VP_MAIN);        //MC20081115 add for SCART ***
    //if((bInputType==P_FB0) ||(bInputType==P_FB1))
	if(fgIsMainScart())
    {
        vDrvIntAutoColorStart();
        vApiVideoSetEvtFlg(VDO_FLG_VGAAUTO_DOING);    
        return (TRUE);
    }                                                                               
    else                                                                            //MC20081115 add for SCART &&&
    {                                                                         
        if((((_bVgaTiming) >= VGA_SEARCH_START) && ((_bVgaTiming) <= VGA_SEARCH_END))||fgIsVideoTiming(_bHdtvTiming))
        {
            vDrvIntAutoColorStart();
            vApiVideoSetEvtFlg(VDO_FLG_VGAAUTO_DOING);    
            return (TRUE);
        }
        else
        {
            return (FALSE);
        }
    }
}

/**
 * @brief fgApiVideoCVBSAutoColor(void) 
 * Trigger CVBS Auto Color state machine 
 * @param void
 * @retval UINT8
 * @example fgApiVideoCVBSAutoColor(void);
 */
UINT8 fgApiVideoCVBSAutoColor(void) 
{
#if TVD_AUTO_CALIB
    if (fgIsMainTvd3d()||fgIsPipTvd3d())    
    {
        vDrvTVDAutoCalibStart();
        vApiVideoSetEvtFlg(VDO_FLG_TVDAUTO_DOING);    
        return (TRUE);
    }
    else
#endif
    {
        return (FALSE);
    }
}

/**
 * @brief wApiVideoGetVgaStdHtotal(void) 
 * Get  Standard Htotal value of current VGA timing
 * @param void
 * @retval :Htotal value
 * @example wApiVideoGetVgaStdHtotal(void);
 */    
UINT16 wApiVideoGetVgaStdHtotal(void) 
{
    return Get_VGAMODE_IHTOTAL(_bVgaTiming); // HTotal
}


/**
 * @brief wApiVideoGetVgaStdHPos(void) 
 * Get  Standard H position value of current VGA timing
 * @param void
 * @retval :H position value
 * @example wApiVideoGetVgaStdHPos(void);
 */
UINT16 wApiVideoGetVgaStdHPos(void) 
{
    return Get_VGAMODE_IPH_BP(_bVgaTiming); // V Position
}


/**
 * @brief wApiVideoGetVgaStdVPos(void) 
 * Get  Standard V position value of current VGA timing
 * @param void
 * @retval :V position value
 * @example wApiVideoGetVgaStdVPos(void);
 */
UINT16 wApiVideoGetVgaStdVPos(void) 
{
    return Get_VGAMODE_IPV_STA(_bVgaTiming); // H Position
}


#ifdef CC_COPLAT_MT5387_TODO
void vApiVideoClearEEP(void)
{
	UINT16 i ;
  	UINT16 wData = 0xffff;
	// ocean : 1 // 10 vga timing from the top of vga_table will be saved into Eeprom
  	for ( i = 0 ; i < VGA_EEP_NO; i++)
  	{
		vApiEepromDmaWrite(EEP_VGA_H_POSITION_START + (i*sizeof(UINT16)), &wData, 2);
		
		vApiEepromDmaWrite(EEP_VGA_V_POSITION_START + (i*sizeof(UINT16)), &wData, 2);

		vApiEepromWriteByte((EEP_VGA_PHASE_START + i), 0xff);
  		
		vApiEepromDmaWrite(EEP_VGA_CLOCK_START + (i*sizeof(UINT16)), &wData, 2);

		// reset timing index
		vApiEepromWriteByte((EEP_VGA_TIMING_INDEX_START + i), 0xff);
	}
	// reset current index to 0
	vApiEepromWriteByte(EEP_VGA_CURRENT_TIMING_INDEX, 0);
}

extern UINT8 _bCurrentTimingIdx;
void vApiVideoEepAutoDataSet(void)
{
	UINT16 wTmp;

	//LogSB ("wCurrentIdx", wCurrentIdx);
	
	// read the phase from register
	wTmp = bDrvVGAGetPhase();
	
	vApiEepromWriteByte(EEP_VGA_PHASE_START + _bCurrentTimingIdx, (UINT8)wTmp);
	
	// read the clock from register
	wTmp = RegReadFldAlign(VFE_16, C_HLINE_LENGTH) + 1; // ??? difference 1 btwn real data & register data!
	vApiEepromDmaWrite(EEP_VGA_CLOCK_START + (_bCurrentTimingIdx*sizeof(UINT16)), &wTmp, 2);
	
	// read the h position from register : there is big difference btwn Register data and vga_table
	// but it's OK
	wTmp = wDrvVGAGetHStart();
	vApiEepromDmaWrite(EEP_VGA_H_POSITION_START + (_bCurrentTimingIdx*sizeof(UINT16)), &wTmp, 2);
	
	// read the v position from register
	wTmp = RegReadFldAlign(ASYNC_11, AS_NEW_VS_OUTP_H1);
	vApiEepromDmaWrite(EEP_VGA_V_POSITION_START + (_bCurrentTimingIdx*sizeof(UINT16)), &wTmp, 2);	
}
#else
void vApiVideoClearEEP(void)
{

}
void vApiVideoEepAutoDataSet(void)
{

}
#endif

