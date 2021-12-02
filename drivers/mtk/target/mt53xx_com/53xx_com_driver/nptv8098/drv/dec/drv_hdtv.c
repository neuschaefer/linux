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
 * $RCSfile: drv_hdtv.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

#define _DRV_HDTV_C_


#ifdef CC_UP8032_ATV
#include "general_mt82.h"
#endif
#include "general.h"
#include "hw_async.h"
#include "hw_vdoin.h"
#include "hw_scslicer.h"
#ifdef CC_COPLAT_MT82
#include "debug.h"
#include "hw_hdtv.h"
#endif
#include "hw_scart.h"
#include "hw_pdwnc.h" 
#include "drv_async.h"
#include "vga_table.h"
#include "drv_vga.h"
#include "drv_hdtv.h"
#include "source_select.h"
#include "drv_vdoclk.h"
#include "drv_adcset.h"
#include "drv_di.h"
#include "drv_tvd.h"
#ifndef CC_COPLAT_MT82
#include "drv_scaler.h"
#endif
#include "video_def.h"
#include "util.h"
#include "drv_display.h"
#include "drv_video.h"
#include "drv_auto.h"
#include "vga_auto.h"
#include "x_printf.h"
#include "api_eep.h"
#include "eepdef.h"
#include "nptv_debug.h"
#include "x_typedef.h"
#include "vdo_misc.h"
#include "mute_if.h"
#if HDTV_MV_DETECT_SUPPORT
#include "api_notify.h"
#endif


UINT8   _bHdtvTiming;
#if ADAPTIVE_SLICER_DEFAULT_SETTING
UINT8   _bHdtvTiming_old;
#endif
UINT8   _IsHdtvDetectDone;
UINT8   _bUnLockCnt;

static UINT8   NoSignalCnt;

static UINT8   bHdtvMDStateMachine;
static UINT8   bHdtvMCErrorCnt;
#if (HDTV_240P_SUPPORT || HDTV_540P_SUPPORT || HDTV_288P_SUPPORT)
static UINT8   bHdtvFieldErrorCnt;
#endif
UINT8 bActStableCnt;

#define VGA_TIMEDELAY_DBG 0

#include "x_timer.h"
//static HAL_TIME_T cur_time[10];
///////////////////////////////////

#define CC_HDTV_NON_STANDARD 1
#define SUPPORT_54MHZ_LINECLAMP_EN 0


RHDTVNSTDStatus _rHDTV_NSTDStatus;

#if HDTV_CODE_SHRINK  
//clamp_start, clamp_end, blank_start, vmask_start, v_mask_end, phase
CODE const HDTVTimingPrmSet _arHDTVPrm[]=
{
  {0, 5, 30, 36, 0x7, 0xa, 0}, //default
  {MODE_525I_OVERSAMPLE, 0x06,0xa0,0xb4,0x06,0x0f,0/*0x1c*/}, // 525i oversample
  {MODE_625I_OVERSAMPLE, 0x06,0xc0,0xd4,0x05,0x12,0/*0x1c*/}, // 625i oversample  
  {MODE_480P_OVERSAMPLE, 0x05,0x38,0x4c,0x07,0x12,0x1c/*0x10*/}, // 480p oversample  
  {MODE_576P_OVERSAMPLE, 0x05,0x40,0x54,0x05,0x12,0x0f/*0x1c*/}, // 576p oversample  0xf
  {MODE_720p_50, 0x28,0xb0,0xc4,0x07,0x0a,0x1c/*0x04*/}, //720p50 
  {MODE_720p_60, 0x28,0xb0,0xc4,0x07,0x0a,0x1C/*0x08*/}, //72060   10
  {MODE_1080i_48, 0x28,0x80,0x92,0x07,0x0a,0/*0x16*/}, //1080i48  
  {MODE_1080i_50, 0x28,0x80,0x92,0x07,0x0a,0x10/*0x05*/}, // 1080i50 1F  10
  {MODE_1080i, 0x28,0x80,0x92,0x07,0x0a,0x10/*0x1c*/}, // 1080i60  JL 0A
  {MODE_1080p_24, 0x28,0x80,0x92,0x07,0x0a,0/*0x12*/}, //1080p24
  {MODE_1080p_25, 0x28,0x80,0x92,0x07,0x0a,0/*0x19*/}, // 1080p25
  {MODE_1080p_30, 0x28,0x80,0x92,0x07,0x0a,0/*0x12*/}, // 1080p30
  {MODE_1080p_50, 0x28,0x80,0x92,0x07,0x0a,0/*0x1c*/}, // 1080p50
  {MODE_1080p_60, 0x28,0x80,0x92,0x07,0x0a,0x1a/*0x18*/}, // 1080p60 
  {MODE_525I, 0x03,0x50,0x5A,0x06,0x0f,0/*0x1c*/}, // 525i
  {MODE_625I, 0x03,0x60,0x6a,0x05,0x12,0/*0x1c*/}, // 625i  
  {MODE_480P, 0x01,0x15,0x22,0x07,0x12,0/*0x10*/}, // 480p
  {MODE_576P, 0x15,0x22,0x05,0x12,0/*0x10*/}, // 576p
  {MODE_720p_24, 0x28,0xb0,0xc4,0x07,0x0a,0/*0x04*/}, //720p24
  {MODE_720p_25, 0x28,0xb0,0xc4,0x07,0x0a,0/*0x04*/}, //720p25
  {MODE_720p_30, 0x28,0xb0,0xc4,0x07,0x0a,0/*0x04*/}, //720p30
  {MODE_240P, 0x06,0x30,0x3d,0x07,0x0a,0x00}, // 240p
  {MODE_540P, 0x30,0x6a,0x7a,0x07,0x0a,0x00}, // 540p
  {MODE_288P, 0x01,0x15,0x22,0x07,0x0a,0x00}  // 288p
} ;
#endif

#ifdef CC_HDTV_NON_STANDARD
UINT8 _bNoSTdCnt ;
UINT16 _wNoStdVtotal ;
#endif

#if (HDTV_240P_SUPPORT || HDTV_540P_SUPPORT || HDTV_288P_SUPPORT)
UINT8 _bFieldAct ;
#endif

#if HDTV_MV_DETECT_SUPPORT
UINT8 _bHdtvMvOn ;
UINT8 _bHdtvMvChgCnt ;
#endif

UINT8  _bHdtvChkState;
UINT8 _bHdtvModeChged ;
UINT8 _bPLLlockCnt ;
#ifdef CC_UP8032_ATV
void HAL_GetTime(HAL_TIME_T* pTime)
{

}
void HAL_GetDeltaTime(HAL_TIME_T* pResult, HAL_TIME_T* pT0,
    HAL_TIME_T* pT1)
{

}
#if ADAPTIVE_SLICER_ENABLE
extern void vResetSliceTimer(void);
#else
#define vResetSliceTimer()
#endif
#endif
HAL_TIME_T _rHdtvModeChgTime;

extern void vHdtvSetModeCHG(void);
extern void vHdtvSetModeDone(void);
#ifdef CC_HDTV_NON_STANDARD
void vHDTVChkNSTD(UINT16 VTotal_Measure);
#endif



#define SUPPORT_DI_OVERSAMPLE 1
#if SUPPORT_DI_OVERSAMPLE
    #ifdef CC_SUPPORT_OVERSAMPLE_1440_ENABLE
        //control oversample to DI
        UINT8 _bHdtvOversampleToDi = TRUE ;
        //for oversample control to DI to select drop pixel or pre-down
        #if CC_OVERSAMPLE_1440_CEN_DROP_PIXEL
        UINT8 _bHdtvCenEnableForSD = TRUE ;
        #else
        UINT8 _bHdtvCenEnableForSD = FALSE ;
        #endif
    #else
        //control oversample to DI
        UINT8 _bHdtvOversampleToDi = FALSE ;
        UINT8 _bHdtvCenEnableForSD = FALSE ;
    #endif    
#endif

////////////////////////////////////////////////////////////////////////////////
// Local Function

/**
 * @brief vvHdtvReset(void)
 * Reset HDTV Decoder.
 * @param  void
 * @VgaInitial used
 */
void vHdtvReset(void)
{
    vHdtvResetOn();
    vUtDelay2us(20);
    vHdtvResetOff();
}

/**
 * @brief vVgaSetInput(UINT16 wstart, UINT16 wwidth)
 * Input VGA  Horizontal Signal Start Points and width .
 * @param  wstart: Start Points
 * @param wwidth: Scan line width
 * @retval void
 * @pre  Set Start points and san line width from vgatable or from auto action
 */
void vHdtvSetInput(UINT16 wstart, UINT16 wwidth)
{

    wwidth>>= (RegReadFldAlign(HDTV_00,HDTV_CEN_SEL));

    wwidth += DECODER_ADD_WIDTH;

    vRegWriteFldAlign(HDTV_01,wstart,HDTV_AV_START);
    vRegWriteFldAlign(HDTV_01,wwidth,HDTV_AV_WIDTH);
}

/**
 * @brief vHdtvBlankStart(UINT16 wclamp_start)
 * Input blanking start positions. The blanking area is (Start position, Start position + 16)
 * @param  wblank_start: blanking start position.
 * @retval void
 * @pre  Need to Set it in a suitable blanking area.
 */
void vHdtvBlankStart(UINT16 wblank_start)
{
    vRegWriteFldAlign(HDTV_03,wblank_start,HDTV_BLANK_START);
}

/**
 * @brief vHdtvClampWin(UINT16 wclamp_start, UINT16 wclamp_end)
 * Input Clamping Window Range
 * @param  wclamp_start: Clamping Window Start Position
 * @param wclamp_end: Clamping Window End Position
 * @retval void
 * @pre Need to Set it in a suitable clamping area.
 */
void vHdtvClampWin(UINT16 wclamp_start, UINT16 wclamp_end)
{
    vRegWriteFldAlign(HDTV_04, wclamp_start, HDTV_CLAMP_START);
    vRegWriteFldAlign(HDTV_04,wclamp_end,HDTV_CLAMP_END);
}

/**
 * @brief bHdtvTimingSearch(void)
 * HDTV Timing search according to bSP0Vclk & wSP0Hclk
 * @param  void
 * @retval Timing index.
 * @pre  Need to measure H/V Frequence and frequence transform.
 */
#ifdef CC_HDTV_NON_STANDARD 
#define HDTV_VSYNC_LOWER_BD  10
#define HDTV_HSYNC_LOWER_BD  10
#else
#define HDTV_VSYNC_LOWER_BD  5
#define HDTV_HSYNC_LOWER_BD  8
#endif
#define HDTV_VTOTAL_BD  50
PRIVATE UINT8 bHdtvTimingSearch(void)
{
    UINT8 btime_temp, bsearch;
    UINT16 wTmpVtotal, wTempVTotalOffset, wMinVTotalOffset ;
    #ifdef CC_HDTV_NON_STANDARD     
    UINT16 wTempHFOffset, wTempVFOffset;
    UINT32 wHFVFOffsetSum, wMinHFVFOffsetSum=0xFFFF;
    #endif
    bsearch = HDTV_SEARCH_START;	//HDTV start;

    DBG_Printf(DBG_MDET, "bSP0Vclk %d\n", bSP0Vclk);
    DBG_Printf(DBG_MDET, "wSP0Hclk %d\n", wSP0Hclk);

    btime_temp = MODE_NOSUPPORT;
    wMinVTotalOffset = 0xffff;

    if ((wSP0Hclk<50)||(bSP0Vclk<5)) /*H<5k V<5hz */
        return MODE_NOSUPPORT;

    do
    {
		if(Get_VGAMODE_YpbprDisabled(bsearch))
		{
			continue;
		}
        if ((bSP0Vclk > (Get_VGAMODE_IVF(bsearch) - HDTV_VSYNC_LOWER_BD)) && (bSP0Vclk < (Get_VGAMODE_IVF(bsearch) + HDTV_VSYNC_LOWER_BD)))
        {
            if ((wSP0Hclk > (Get_VGAMODE_IHF(bsearch) - HDTV_HSYNC_LOWER_BD)) && (wSP0Hclk < (Get_VGAMODE_IHF(bsearch) + HDTV_HSYNC_LOWER_BD)))//20 too much
            {
                /* V total */
                wTmpVtotal = Get_VGAMODE_IVTOTAL(bsearch) ;
                if ( Get_VGAMODE_INTERLACE(bsearch))
                {
                    wTmpVtotal = wTmpVtotal /2 ;
                }

                if((wSP0Vtotal > (wTmpVtotal + HDTV_VTOTAL_BD))  || (wSP0Vtotal < (wTmpVtotal - HDTV_VTOTAL_BD)))
                {
                    continue ;
                }

                /* interlace mode measure Vtotal= real Vtotal/2 */
                wTempVTotalOffset = (wSP0Vtotal > wTmpVtotal)? (wSP0Vtotal - wTmpVtotal) : (wTmpVtotal - wSP0Vtotal);

                // find the closest Vtotal
                if (wMinVTotalOffset > wTempVTotalOffset)
                {
                    wMinVTotalOffset = wTempVTotalOffset;
                    btime_temp = bsearch;     
                    #ifdef CC_HDTV_NON_STANDARD                    
                    wMinHFVFOffsetSum = ABS(Get_VGAMODE_IHF(bsearch) - wSP0Hclk) + ABS(Get_VGAMODE_IVF(bsearch) - bSP0Vclk);                    
                    #endif
                }                
                #ifdef CC_HDTV_NON_STANDARD                 
                else if(wMinVTotalOffset == wTempVTotalOffset)  //Check (Diff(HF)+Diff(VF))
                {
                    wTempHFOffset = ABS(Get_VGAMODE_IHF(bsearch) - wSP0Hclk);
                    wTempVFOffset = ABS(Get_VGAMODE_IVF(bsearch) - bSP0Vclk);
                    wHFVFOffsetSum = wTempHFOffset + wTempVFOffset; 

                    if( wHFVFOffsetSum < wMinHFVFOffsetSum )
                    {
                        btime_temp = bsearch;
                        wMinHFVFOffsetSum = wHFVFOffsetSum;
                    }
                }
                #endif
            }
        }
    }
    while ((HDTV_SEARCH_END + 1) != ++bsearch);
    //for 525i, 625i oversample

#if YPBPR_480IP_27MHZ
        if ((btime_temp == MODE_525I_OVERSAMPLE)) 
        {
            btime_temp = MODE_525I;
        }
    
        if ((btime_temp == MODE_625I_OVERSAMPLE))
        {
            btime_temp = MODE_625I;
        }
#endif

#if (HDTV_240P_SUPPORT || HDTV_540P_SUPPORT || HDTV_288P_SUPPORT)
    _bFieldAct = bDrvASFieldAct() ;
#endif

#if HDTV_240P_SUPPORT
#if YPBPR_480IP_27MHZ
    if ((btime_temp == MODE_525I) && (_bFieldAct == 0))
    {
        btime_temp = MODE_240P ;
        DBG_Printf(DBG_MDET, "240p\n");
    }
#else
if ((btime_temp == MODE_525I_OVERSAMPLE) && (_bFieldAct == 0))
{
    btime_temp = MODE_240P ;
    DBG_Printf(DBG_MDET, "240p\n");
}

#endif
#endif

#if HDTV_540P_SUPPORT
    if ((btime_temp == MODE_1080i) && (_bFieldAct == 0))
    {
        btime_temp = MODE_540P ;
        DBG_Printf(DBG_MDET, "540p\n");
    }
#endif

#if HDTV_288P_SUPPORT
#if YPBPR_480IP_27MHZ
    if ((btime_temp == MODE_625I) && (_bFieldAct == 0))
    {
        btime_temp = MODE_288P ;
        DBG_Printf(DBG_MDET, "288P\n");
    }
#else
    if ((btime_temp == MODE_625I_OVERSAMPLE) && (_bFieldAct == 0))
    {
        btime_temp = MODE_288P ;
        DBG_Printf(DBG_MDET, "288P\n");
    }
#endif
#endif

    return btime_temp;
}

#if SUPPORT_DI_OVERSAMPLE
void vHdtvSetOversampleForSD(UINT8 bmode)
{
        _bHdtvOversampleToDi = bmode;
}

UINT8 vHdtvGetOversampleForSD(void)
{
       return _bHdtvOversampleToDi;
}
#endif

/**
 * @brief vHdtvSetInputCapature(UINT8 bmode)
 * According to timing mode to set suitable Mask, windows etc.
 * @param  Timing Mode index
 * @retval void
 * @pre  Need to identify supported timing mode first.
 */
void vHdtvSetInputCapature(UINT8 bmode, UINT8 bIsHdtv) 	//set input start and length
{
    UINT16 VS1, VLEN ;


    if (bmode >= MAX_TIMING_FORMAT)
    {
        return;
    }
    vSetAsyncMeasureBD(DOMAIN_27MHz);

    vDrvAsyncSetFieldDet(wSP0HLength);
  
#if SUPPORT_54MHZ_LINECLAMP_EN
  
  if ((bmode == MODE_525I) || (bmode == MODE_625I)||(bmode == MODE_625I_OVERSAMPLE) || (bmode == MODE_480P_OVERSAMPLE) || (bmode == MODE_576P_OVERSAMPLE)) 
   {
	  vRegWriteFldAlign(HDTV_00, 1, HDTV_LCLAMP_EN); //Enable line clamp
   }
   else
   {
	  vRegWriteFldAlign(HDTV_00, 0, HDTV_LCLAMP_EN); //Disable line clamp
   }
#endif

    if ((bmode == MODE_525I) || (bmode == MODE_625I) || (bmode == MODE_525I_OVERSAMPLE) || (bmode == MODE_625I_OVERSAMPLE)) 
    {
#if SUPPORT_DI_OVERSAMPLE    
    if(_bHdtvOversampleToDi)
    {
    	vHdtvCenSel(_bHdtvCenEnableForSD);
    }
    else
    {
    	vHdtvCenSel((UINT8)TRUE);
    }
#else
        vHdtvCenSel((UINT8)TRUE);
#endif
    }
    else
    {
        vHdtvCenSel((UINT8)FALSE);
    }

    vHdtvSetInput(Get_VGAMODE_IPH_BP(bmode), Get_VGAMODE_IPH_WID(bmode));

    vDrvAsyncClampMask(15, 10);
    if (Get_VGAMODE_INTERLACE(bmode) == PROGRESSIVE)
    {    
        // Disable Field Detection
        vRegWriteFldAlign(ASYNC_12, FIELD_DISABLE, AS_FLD_SELECT); // 00:ck27,01:pix,10:free run,11:disable
    }
    else
    {
        // Set CLK27 Field Detection
        vRegWriteFldAlign(ASYNC_12, FIELD_DET_CLK27, AS_FLD_SELECT); // Set CLK27 Field Detection

        // 2. ClampMask => Clamping Mask
        vDrvAsyncClampMask(10,	10);

    }

    VS1 =  Get_VGAMODE_IPV_STA(bmode);     
    VLEN = Get_VGAMODE_IPV_LEN(bmode);

    if(Get_VGAMODE_INTERLACE(bmode))
    {
        VLEN >>= 1 ;
    }
#ifdef CC_HDTV_NON_STANDARD
		if (fgIsMainYPbPr() || fgIsPipYPbPr())
	{
		UINT8 bInterlaced ;
		bInterlaced = Get_VGAMODE_INTERLACE(bmode) ;
		VLEN = VLEN + _wNoStdVtotal - (Get_VGAMODE_IVTOTAL(bmode)/(bInterlaced+1));
	}
#endif		
        vDrvAsyncVsyncOut(VS1, VLEN);     


    vDrvAsyncVMask(7, 10);
    //-------------------------------
    // Set V-Mask Series
    // 4. VMask2 => 2'th CLIN Mask
    vDrvAsyncVMask2(0, 0);

    vRegWriteFldAlign(ASYNC_0B, 0x04, AS_MASK_SLICE_ST); //set HLEN2_S
    vRegWriteFldAlign(ASYNC_0B, 0x80, AS_MASK_SLICE_END); //set HLEN2_E
    vRegWriteFldAlign(ASYNC_08, 0x0, AS_MASK_SLICE_EN);


    //MV Lock-Window
    //vDrvAsyncVMask2(0x0,0x0);

    // Clamping Window & Blanking
#if HDTV_CODE_SHRINK
//if(bIsHdtv) 
{
	UINT8 bmodeInx, i;

	bmodeInx = 0;  //default setting
    for(i=0; i < (sizeof(_arHDTVPrm)/sizeof(HDTVTimingPrmSet)); i++)
    {
        if(bmode== _arHDTVPrm[i].bTimingIdx)
        {
            bmodeInx = i ;
            break;
        }
    }
    DBG_Printf(DBG_MDET,"match timing id %d\n", bmodeInx);
	vHdtvClampWin((UINT16)_arHDTVPrm[bmodeInx].bClampStart, (UINT16)_arHDTVPrm[bmodeInx].bClampEnd);
	vHdtvBlankStart((UINT16)_arHDTVPrm[bmodeInx].bBlankStart);
	vDrvAsyncVMask((UINT16)_arHDTVPrm[bmodeInx].bVmaskStart, (UINT16)_arHDTVPrm[bmodeInx].bVmaskEnd);
 	vDrvVGASetPhase(_arHDTVPrm[bmodeInx].bPhase);
	if(((bmode == MODE_525I) || (bmode == MODE_525I_OVERSAMPLE) || (bmode == MODE_625I) || (bmode == MODE_625I_OVERSAMPLE))
		&& (bModeIndex == SYNCONGREEN))
	{
		vRegWriteFldAlign(ASYNC_08, 0x01, AS_MASK_SLICE_EN);
	}
  #if SUPPORT_SCART
  	#ifdef SCART_CLK_SOURCE_CVBS       
    		switch (bmode)
    		{
        		case MODE_525I_OVERSAMPLE:
            			if(fgIsMainScart() || fgIsPipScart())
            			{
             				 vHdtvClampWin(6, 0x50);
              			vHdtvBlankStart(0x60);
            			}
		            break;
			case MODE_625I_OVERSAMPLE:
	 		    	if(fgIsMainScart() || fgIsPipScart())
            		   	{
             				vHdtvClampWin(6, 0x9A);
             				vHdtvBlankStart(0xAA);
            			}    
		            break;
        		default:
		            break;
    		}
    	#endif
    #endif
}	
#else    
    switch (bmode)
    {
        case MODE_480P:
            vHdtvClampWin(1, 0x15);
            vHdtvBlankStart(0x22);	//suggest blank 13 pixel after  clamp end
            vDrvAsyncVMask(0x7, 0x12); // for VSAG- (0x5, 0x12);
            vDrvVGASetPhase(16);
            break;

        case MODE_480P_OVERSAMPLE:
            vHdtvClampWin(0x05, 0x38);      //vHdtvClampWin(2, 0x2a);
            vHdtvBlankStart(0x4c);             //vHdtvBlankStart(0x3c);
            vDrvAsyncVMask(0x7, 0x12); // for VSAG- (0x5, 0x12);
            vDrvVGASetPhase(16);

            break;

        case MODE_576P:
            vHdtvClampWin(1, 0x15);
            vHdtvBlankStart(0x22);
            vDrvAsyncVMask(0x5, 0x12);
            vDrvVGASetPhase(16);

            break;

        case MODE_576P_OVERSAMPLE:
            vHdtvClampWin(5, 0x40);      //vHdtvClampWin(2, 0x2a);
            vHdtvBlankStart(0x54);         //vHdtvBlankStart(0x3c);
            vDrvAsyncVMask(0x5, 0x12);
            vDrvVGASetPhase(16);

            break;

        case MODE_525I:
            vHdtvClampWin(3, 0x50);
            vHdtvBlankStart(0x5A);
            vDrvAsyncVMask(0x6, 0xf); // Foe VSAG- (0x5, 0xf);
            vDrvVGASetPhase(28);

            //vDrvAsyncVMask2(0x6,0x0);

            if(bModeIndex == SYNCONGREEN)
            {
                vRegWriteFldAlign(ASYNC_08, 0x1, AS_MASK_SLICE_EN);
            }
            break;

        case MODE_525I_OVERSAMPLE:
            #if SUPPORT_SCART
            #ifdef SCART_CLK_SOURCE_CVBS     
			if(fgIsMainScart() || fgIsPipScart())
            //if((_bMainICIn == P_FB0) || (_bSubICIn == P_FB0) || (_bMainICIn == P_FB1) || (_bSubICIn == P_FB1))
            {
              vHdtvClampWin(6, 0x50);
              vHdtvBlankStart(0x60);
            }
            else
            #endif
            #endif
            {
              vHdtvClampWin(6, 0xa0);  //vHdtvClampWin(6, 0x58);
              vHdtvBlankStart(0xb4);    //vHdtvBlankStart(0x6D);
            }  
            vDrvAsyncVMask(0x6, 0xf); // Foe VSAG- (0x5, 0xf);
            vDrvVGASetPhase(28);

            //vDrvAsyncVMask2(0x6,0x0);

            if(bModeIndex == SYNCONGREEN)
            {
                vRegWriteFldAlign(ASYNC_08, 0x1, AS_MASK_SLICE_EN);
            }
            break;

        case MODE_625I:
            vHdtvClampWin(3, 0x60);
            vHdtvBlankStart(0x6A);
            vDrvAsyncVMask(0x5, 0x12);
            vDrvVGASetPhase(28);
            //vDrvAsyncVMask2(0x6,0x0);

            if(bModeIndex == SYNCONGREEN)
            {
                vRegWriteFldAlign(ASYNC_08, 0x1, AS_MASK_SLICE_EN);
            }
            break;

        case MODE_625I_OVERSAMPLE:
            #if SUPPORT_SCART
            #ifdef SCART_CLK_SOURCE_CVBS            
			if(fgIsMainScart() || fgIsPipScart())
            //if((_bMainICIn == P_FB0) || (_bSubICIn == P_FB0) || (_bMainICIn == P_FB1) || (_bSubICIn == P_FB1))
            {
             vHdtvClampWin(6, 0x9A);
             vHdtvBlankStart(0xAA);
            }
            else
            #endif
            #endif
            {
            vHdtvClampWin(6, 0xc0);  //vHdtvClampWin(6, 0x58);
             vHdtvBlankStart(0xD4);    //vHdtvBlankStart(0x6D);         
            }
            vDrvAsyncVMask(0x5, 0x12);
            vDrvVGASetPhase(28);
            //vDrvAsyncVMask2(0x6,0x0);

            if(bModeIndex == SYNCONGREEN)
            {
                vRegWriteFldAlign(ASYNC_08, 0x1, AS_MASK_SLICE_EN);
            }
            break;

        case MODE_720p_24:
        case MODE_720p_25:
        case MODE_720p_30:
        case MODE_720p_50:
            vHdtvClampWin(0x28, 0xb0);  //vHdtvClampWin(0x28, 0x66);
            vHdtvBlankStart(0xc4);     //vHdtvBlankStart(0x75);
            vDrvVGASetPhase(4);
            break;

        case MODE_720p_60:
            vHdtvClampWin(0x28, 0xb0);  //vHdtvClampWin(0x28, 0x88);
            vHdtvBlankStart(0xc4);     //vHdtvBlankStart(0x97);
            vDrvVGASetPhase(8);
            break;

        case MODE_1080i:
            vHdtvClampWin(0x28, 0x80);  // vHdtvClampWin(0x30, 0x6a);
            vHdtvBlankStart(0x92);         //vHdtvBlankStart(0x7a);
            vDrvVGASetPhase(28);
            break;

        case MODE_1080i_50:
            vHdtvClampWin(0x28, 0x80);  // vHdtvClampWin(0x28, 0x66);
            vHdtvBlankStart(0x92);         //vHdtvBlankStart(0x75);
            vDrvVGASetPhase(5);
            break;
        case MODE_1080i_48:
            vHdtvClampWin(0x28, 0x80);  // vHdtvClampWin(0x28, 0x66);
            vHdtvBlankStart(0x92);         //vHdtvBlankStart(0x75);
            vDrvVGASetPhase(22);
            break;
        case MODE_1080p_24:
            vHdtvClampWin(0x28, 0x80);  // vHdtvClampWin(0x28, 0x56);
            vHdtvBlankStart(0x92);         //vHdtvBlankStart(0x70);
            vDrvVGASetPhase(18);
            break;
        case MODE_1080p_25:
            vHdtvClampWin(0x28, 0x80);  // vHdtvClampWin(0x28, 0x56);
            vHdtvBlankStart(0x92);         //vHdtvBlankStart(0x70);
            vDrvVGASetPhase(25);
            break;
        case MODE_1080p_30:
            vHdtvClampWin(0x28, 0x80);  // vHdtvClampWin(0x28, 0x56);
            vHdtvBlankStart(0x92);         //vHdtvBlankStart(0x70);
            vDrvVGASetPhase(18);
            break;
        case MODE_1080p_50:
            vHdtvClampWin(0x28, 0x80);  // vHdtvClampWin(0x28, 0x56);
            vHdtvBlankStart(0x92);         //vHdtvBlankStart(0x70);
            vDrvVGASetPhase(28);
            break;

        case MODE_1080p_60:
            vHdtvClampWin(0x28, 0x80);  // vHdtvClampWin(0x28, 0x80);
            vHdtvBlankStart(0x92);
            vDrvVGASetPhase(24);
            break;

#if HDTV_240P_SUPPORT
        case MODE_240P:
            vHdtvClampWin(6, 0x30);
            vHdtvBlankStart(0x3D);
            break;
#endif

#if HDTV_540P_SUPPORT
        case MODE_540P:
            vHdtvClampWin(0x30, 0x6a);
            vHdtvBlankStart(0x7a);
            break;
#endif

#if HDTV_288P_SUPPORT
        case MODE_288P:
            vHdtvClampWin(1, 0x15);
            vHdtvBlankStart(0x22);
            break;
#endif

        default:
            vHdtvClampWin(5, 30);
            vHdtvBlankStart(36);
            vDrvVGASetPhase(0);
            break;
    }
#endif

    if (Get_VGAMODE_OverSample(bmode))
    {
        vRegWriteFldAlign(ASYNC_17, PSNE_ONLY, AS_C_PSNE_STA_SEL);
    }

	switch(bmode)
	{
		case MODE_1080p_50:
		case MODE_1080p_60:
		break;
		default:
              if(bIsHdtv)
              {
		    vRegWriteFldAlign(ASYNC_0B, 7, AS_C_DEGLITCH);
		    vRegWriteFldAlign(ASYNC_00, 1 , AS_CSYNC_DGLITCH_SEL);
		}    
			break;
	}


    //Enable vsync_out
    vRegWriteFldAlign(ASYNC_12, 0, AS_DISABLE_VS_OUT);

#if SUPPORT_HDTV_HARDWARE_MUTE
    vRegWriteFldAlign(ASYNC_0C, wSP0HLength, AS_H_STABLE_VALUE);
    vRegWriteFldAlign(ASYNC_0D, wSP0Vtotal, AS_V_STABLE_VALUE);

	if(fgIsMainScart() || fgIsPipScart())
    //if(VSS_MAJOR(_bSrcMainNew) == VSS_SCART || VSS_MAJOR(_bSrcSubNew) == VSS_SCART)
    {
        vDrvAsyncSetMuteCriteria(0);
    }
    else if(bModeIndex == SYNCONGREEN)
    {
        vDrvAsyncSetMuteCriteria(AS_MUTE_CSACT|AS_MUTE_HSPOL|AS_MUTE_VSPOL|AS_MUTE_HLEN|AS_MUTE_VLEN);
    }
    else
    {
        vDrvAsyncSetMuteCriteria(AS_MUTE_HSACT|AS_MUTE_HSPOL|AS_MUTE_VSPOL|AS_MUTE_HLEN|AS_MUTE_VLEN);
    }

#endif
#if HDTV_MV_DETECT_SUPPORT
	_bHdtvMvOn =  bDrvAsyncMvStatus();
	vApiNotifyMVChg(((fgIsMainYPbPr())?SV_VP_MAIN:SV_VP_PIP),_bHdtvMvOn);
	_bHdtvMvChgCnt = 0 ;
#endif

	if(!(fgIsMainScart() || fgIsPipScart()))
	{
		vRegWriteFldAlign(ASYNC_04, 0x10, AS_HLEN2_S); //set HLEN2_S
		vRegWriteFldAlign(ASYNC_04, (wSP0Vtotal -10), AS_HLEN2_E); //set HLEN2_E 
	}

}

void vHdtvHwInit (void)
{
    /*  Sync proc */
    vRegWriteFldAlign(ASYNC_0E, 1, AS_SP0_PIXEL_EN);
    vRegWriteFldAlign(ASYNC_02, 1, AS_SP2_EN);

    vASCSSeparatorThre();				//csync separator threshold
	vRegWriteFldAlign(HDTV_03, 0, HDTV_BLAK_SET);
	vRegWriteFldAlign(ASYNC_02, 0x159, AS_VACT_MP_TH);	  //for 720P 24Hz   

    // Add by Adam for VSYNCP/HSYNCP = 0
    vRegWriteFldAlign(ASYNC_13, 0x0, AS_BDHSYNCP);
    vRegWriteFldAlign(ASYNC_13, 0x0, AS_BDVSYNCP);
    vRegWriteFldAlign(ASYNC_12, VS_INPUT_VSYNC_INT, AS_VS_OUT_INT_SEL); //Vsync interrupt is input signal
    //vsync delay for HLEN/VLEN measurement
    vRegWriteFldAlign(ASYNC_00, 0, AS_VSYNC_DELAY_SEL);
    // disable auto invert
    vRegWriteFldAlign(ASYNC_00, 1, AS_AUTO_INVP);

    vRegWriteFldAlign(ASYNC_00, 0, AS_HSYNC_IN_RST_POL); //chang to default value by adam for MV detection and hsync_width

    // set new decomposite algorithm

    vASDeCompSel(NEW_DECOMPOSITE2);  	
  //for decomp2 method
    vRegWriteFldAlign(ASYNC_18, 16, AS_MAX_RST_CNT_THR);
    vRegWriteFldAlign(ASYNC_18, 2, AS_TOTAL_LINE_THR);
    vRegWriteFldMulti(ASYNC_19, P_Fld(0, AS_VMASK4_OFF)| P_Fld(0, AS_VMASK4_INV));
	//from 0xB --> 0x10
	vRegWriteFldAlign(ASYNC_01, 0x10, AS_DE_COMP_DIFF_TH); //set higher value for macrovision.


    // Enable HDTV path clock enable
    vRegWriteFldAlign(HDTV_03, 1, HDTV_EN);

    // select falling edge reset for - hsync from PLL
    vRegWriteFldAlign(HDTV_03, HDTV_FALLING_EDGE, HDTV_HEDGE_SEL);


    // set Mv detection range
#if HDTV_MV_DETECT_SUPPORT
    vDrvAsyncMvDetectV(6, 20); // from line 6-> 20
    vDrvAsyncMvDetectH(0xF, 0xF); // h ysnc , 15 ->  Htoal -15
#endif
    if (fgIsMainYPbPr() || fgIsPipYPbPr())
    {
        vRegWriteFldAlign(HDTV_00, 0, HDTV_RGB);                                        // michael add for mt5360 &&&
        //{1,2,3} = {Y,Pb, Pr}
        if ((bGetICInputType(SV_VP_MAIN)==P_VGACOMP) || (bGetICInputType(SV_VP_PIP)==P_VGACOMP))
        {
            vRegWriteFldAlign(HDTV_03, 0x2, HDTV_DATA_SEL);
        }
        else
        {
            //Set HDTV DATA SEL : Order=>0x6 :{Y,Pb,Pr} = {1,2,3}
            vRegWriteFldAlign(HDTV_03, 0x6 ,HDTV_DATA_SEL);
        }
    }
    else //VGA case
    {
        vRegWriteFldAlign(HDTV_00, 1, HDTV_RGB);

        //{1,2,3} = {B,G, R}
        //Set HDTV DATA SEL : Order=>0x6 :{Y,Pb,Pr} = {2,1,3}
        vRegWriteFldAlign(HDTV_03,0x2,HDTV_DATA_SEL);
    }

    // Get Proper HTotal Value
    vRegWriteFldAlign(ASYNC_04, ENABLE, AS_HLEN_USE_ACTIVE);
    vRegWriteFldAlign(ASYNC_04, 0x50, AS_HLEN2_S); //set HLEN2_S
    vRegWriteFldAlign(ASYNC_04, 0x80, AS_HLEN2_E); //set HLEN2_E

    // Enable Field from sync proc0
    vRegWriteFldAlign(HDTV_00, ENABLE,HDTV_SP0_F );

    // disable csync de-glitch
    vRegWriteFldAlign(ASYNC_00, 0 , AS_CSYNC_DGLITCH_SEL );

    //video mask enable for the picture rolling up after adjusting H-pos
    vRegWriteFldAlign(HDTV_00, 1, HDTV_AVMASK_EN);
    vRegWriteFldAlign(HDTV_00, 1, HDTV_AVMASK_HEDGE);

    vRegWriteFldAlign(ASYNC_0E, 1, AS_PIX_VCNT_FLD_RST_EN);
    //auto skew enable
    vRegWriteFldAlign(ASYNC_03, 1, AS_AUTO_HV_SKEW_CLK27_EN);
    vRegWriteFldAlign(ASYNC_1A, 1, AS_AUTO_HV_SKEW_PIX_EN);    
#if SUPPORT_HDTV_HARDWARE_MUTE
    vRegWriteFldAlign(ASYNC_0C, 0x3, AS_H_DIFF_TH);
    vRegWriteFldAlign(ASYNC_0C, 0x8, AS_MUTE_H_CNT_THR);
    vRegWriteFldAlign(ASYNC_0D, 0x3, AS_V_DIFF_TH);
    vRegWriteFldAlign(ASYNC_0D, 0x2, AS_MUTE_V_CNT_THR);

    vRegWriteFldAlign(ASYNC_0D, 0x3, AS_C_SYNC_H_MUTE);
    vRegWriteFldAlign(ASYNC_0C, 1, AS_MUTE_EN);
#endif

    vRegWriteFldAlign(SYS_05, 1, RETIME_SEL);               //Enable internal HSync & Pixel clock retime

#if defined(SCART_RGB_MENU_WA) || defined(SCART_CLK_SOURCE_CVBS)
    vRegWriteFldMulti(HDTV_03, P_Fld(0,HDTV_H_SEL)|P_Fld(0,HDTV_V_SEL));      
    vRegWriteFldAlign(HDTV_00,  0 ,HDTV_FLD_SEL );    //field detected
    vRegWriteFldMulti(ASYNC_08, P_Fld(0,AS_MASK_SLICE_EN)|P_Fld(0,AS_VMASK1_OFF)|P_Fld(0,AS_VMASK2_OFF)|P_Fld(0,AS_VMASK3_OFF));  
#endif
	vRegWriteFldAlign(HDTV_05 , 0, HDTV_YOFFSET);   

    // Field Detection method selection
    vRegWriteFldAlign(ASYNC_12, FIELD_DET_CLK27, AS_FLD_SELECT);
    vRegWriteFldAlign(ASYNC_03, 0, AS_FIELD_DEF_INV); //Not Inverse Field

#if ASYNC_FULL_SCREEN_WA
    vRegWriteFldAlign(ASYNC_03, 1, AS_FLD_DET_OLD); //Use Old Method                                                    
#else
    vRegWriteFldAlign(ASYNC_03, 0, AS_FLD_DET_OLD); //Use New Method                                                    
#endif
    //disable vmask2
    vRegWriteFldAlign(ASYNC_0A, 1, AS_VMASK2_OFF);
   vRegWriteFldAlign(VSRC_00, 0x01, ADCPLL_VMASK2_SEL);
   vRegWriteFldAlign(VSRC_00, 0x01, ADCPLL_VMASK_SEL);
	//Enable HDTV vsync out relatch on output hsync , rising edge
	// Input Vsync 3T delay with input Hsync , av_start min = 3
	vRegWriteFldMulti(HDTV_03, P_Fld(0,HDTV_V_RELATCH_POS)|P_Fld(1,HDTV_V_RELATCH_SEL));

	//Enable v-deglitch for vga v-sync width measurement
	vRegWriteFldAlign(ASYNC_00, 1 , AS_VSYNC_DGLITCH_SEL);

if (fgIsMainYPbPr()||fgIsPipYPbPr())
{
    #ifdef NEW_SYNC_SLICER_CIRCUIT 
	vRegWriteFldMulti(SyncSlice_Control_00,P_Fld(0,AD_SC_INV)|P_Fld(5,LEN_SUCC_THR)|P_Fld(3,SUCC_THR)|P_Fld(0,CALTH_SEL)|P_Fld(2,HSYNC_BW)|P_Fld(0xf,MEAS_BYP)|P_Fld(0x7,LEAVE_CODE)|P_Fld(0x0,RG_CAL)|P_Fld(0x0,RG_SEL)|P_Fld(0x0,RG_MASK)|P_Fld(0x0,RG_VT_EN)|P_Fld(0x0,SC_ON)|P_Fld(0x5,SYNC_ABS_THR)|P_Fld(0x0,CALTH_NO));
	vRegWriteFldMulti(SyncSlice_Control_01,P_Fld(2,DBG_SEL)|P_Fld(1,MAP_EN)|P_Fld(0,VTH_SEL)|P_Fld(0,VTL_SEL)|P_Fld(0x80,VWIDTH_DIFF)|P_Fld(0,VTH_SHFT)|P_Fld(1,VTL_SHFT)|P_Fld(0x8,SHIFT_LEVEL)|P_Fld(0x5,FAIL_THR));
	vRegWriteFldMulti(SyncSlice_Control_02,P_Fld(0,GATE_DISABLE)|P_Fld(1,ONLINE_EN)|P_Fld(0x14,HLEN_GATE_THR)|P_Fld(0x8,VMASK_THR)|P_Fld(0x3,UPDATE_THR)|P_Fld(0x64,HLEN_MASK_END)|P_Fld(0x64,HLEN_MASK_ST));
	vRegWriteFldMulti(SyncSlice_Control_03,P_Fld(0,INT_CLR)|P_Fld(1,SHFT_COMP_EN)|P_Fld(0x3,FREE_RESET)|P_Fld(0x69,COMP_MASK_END)|P_Fld(0x69,COMP_MASK_ST)|P_Fld(0x64,ONLINE_VPOS));
	vRegWriteFldMulti(SyncSlice_Control_04,P_Fld(0xF,CAL_UPLIM)|P_Fld(0,CAL_DNLIM)|P_Fld(1,SDDS_FK)|P_Fld(0x12C,ONLINE_POS_END)|P_Fld(0x64,ONLINE_POS_ST));
	vRegWriteFldMulti(SyncSlice_Control_05,P_Fld(0x0,MAN_VTSEL)|P_Fld(0,MAN_VT)|P_Fld(0x0,MAN_WRITE)|P_Fld(0x0,MAN_VALUE)|P_Fld(0x2,VTH_FIX)|P_Fld(0x1C,SMPL_ST)|P_Fld(0x2,VTL_FIX)|P_Fld(0x96,CAL_CYCLE));
	vRegWriteFldMulti(SyncSlice_Control_00,P_Fld(0x1,SC_ON));
   #else
      vRegWriteFldMulti(SyncSlice_Control_00,P_Fld(0x1,RG_VT_EN)|P_Fld(0x0,RG_MASK)|P_Fld(0x0,RG_SEL)|P_Fld(0x0,RG_CAL));
   #endif
 }
 else
 {
      vRegWriteFldMulti(SyncSlice_Control_00,P_Fld(0x1,RG_VT_EN)|P_Fld(0x0,RG_MASK)|P_Fld(0x0,RG_SEL)|P_Fld(0x0,RG_CAL));
 }
    vIO32WriteFldMulti(PDWNC_VGACFG6,P_Fld(0,FLD_RG_DIV2_EN)|P_Fld(0,FLD_RG_DIV3_EN)|P_Fld(0,FLD_RG_DIV5_EN)|P_Fld(0,FLD_RG_CKINV_SOGY_EN)|P_Fld(1,FLD_RG_CLKSEL_SOGY)|P_Fld(1,FLD_RG_SOGY_ADC_PWD));//use pixel clk
 #ifdef  SUPPORT_AV_COMP_AUTO
	//Hdiff = 0xf, H : 288 ~1792, V 240 ~1400
	vRegWriteFldAlign(ASYNC_06, 5 , AS_CV_DIFF_TH);
	vRegWriteFldAlign(ASYNC_06, 240 , AS_CVRANTH_L);
#endif

}

#ifdef CC_SOURCE_AUTO_DETECT
void vHdtvHwInit_SrcDetect (void)
{
    /*  Sync proc */
    vRegWriteFldAlign(ASYNC_0E, 1, AS_SP0_PIXEL_EN);
    vRegWriteFldAlign(ASYNC_02, 1, AS_SP2_EN);

    vASCSSeparatorThre();				//csync separator threshold
	vRegWriteFldAlign(HDTV_03, 0, HDTV_BLAK_SET);
	vRegWriteFldAlign(ASYNC_02, 0x159, AS_VACT_MP_TH);	  //for 720P 24Hz   

    // Add by Adam for VSYNCP/HSYNCP = 0
    vRegWriteFldAlign(ASYNC_13, 0x0, AS_BDHSYNCP);
    vRegWriteFldAlign(ASYNC_13, 0x0, AS_BDVSYNCP);
    vRegWriteFldAlign(ASYNC_12, VS_INPUT_VSYNC_INT, AS_VS_OUT_INT_SEL); //Vsync interrupt is input signal
    //vsync delay for HLEN/VLEN measurement
    vRegWriteFldAlign(ASYNC_00, 0, AS_VSYNC_DELAY_SEL);
    // disable auto invert
    vRegWriteFldAlign(ASYNC_00, 1, AS_AUTO_INVP);

    vRegWriteFldAlign(ASYNC_00, 0, AS_HSYNC_IN_RST_POL); //chang to default value by adam for MV detection and hsync_width

    // set new decomposite algorithm

    vASDeCompSel(NEW_DECOMPOSITE2);  	
  //for decomp2 method
    vRegWriteFldAlign(ASYNC_18, 16, AS_MAX_RST_CNT_THR);
    vRegWriteFldAlign(ASYNC_18, 2, AS_TOTAL_LINE_THR);
	//from 0xB --> 0x10
	vRegWriteFldAlign(ASYNC_01, 0x10, AS_DE_COMP_DIFF_TH); //set higher value for macrovision.


    // Enable HDTV path clock enable
    vRegWriteFldAlign(HDTV_03, 1, HDTV_EN);

    // select falling edge reset for - hsync from PLL
    vRegWriteFldAlign(HDTV_03, HDTV_FALLING_EDGE, HDTV_HEDGE_SEL);


	//Enable v-deglitch for vga v-sync width measurement
	vRegWriteFldAlign(ASYNC_00, 1 , AS_VSYNC_DGLITCH_SEL);

}

#endif



#if ADAPTIVE_MONITOR_SLICER_MEASURE_ONLINE
    UINT8 _bSetDefaultSlicer_Cnt;
    UINT8 _bSetDefaultSlicer_loop_Cnt;
#endif


/**
 * @brief vHdtvInitial(void)
 * HW and SW initial setting when mode change.
 * @param  void
 * @retval void
 * @pre  Call it when mode change to reinitial HW and SW Setting.
 */
PRIVATE void vHdtvInitial(UINT8 bReason)
{
    DBG_Printf(DBG_MCHG, "hdtv initial %d\n", bReason);
	vIO32WriteFldAlign(PDWNC_VGACFG2, 0, FLD_RG_SOGY_SORS_PWD);//power on
	vIO32WriteFldAlign(PDWNC_VGACFG2, 0, FLD_RG_SOGY_SINK_PWD);//power on
    // SW Init
    _bPLLlockCnt = 0 ;

    _bHdtvTiming = MODE_WAIT;

    vRegWriteFldAlign(ASYNC_12, 1, AS_DISABLE_VS_OUT);

    NoSignalCnt = 0;
    _bAutoISR =0; //YPbPr Auto Phase 2006/11/07
    vVgaAutoInit(); //YPbPr Auto Phase 2006/11/07
    if (fgIsMainYPbPr())
    {
        vVDOINIrqOff((MSK_SP0_VSYNCOUT | MSK_MAIN_DET));
    }
	if (fgIsPipYPbPr())
    {
        vVDOINIrqOff((MSK_SP0_VSYNCOUT | MSK_PIP_DET));
    }
    // Hw Init
    vDrvCLKINFreeRun(ENABLE);	//CLKIN free run modify by W.C Shih to prevent the crash of DDS 2004/12/30   //ADPART
    vHdtvReset();  //ADPART

    vDrvCLKINSyncSel(DCLK_IN_SOG);		//HS polarity  //ADPART

    vRegWriteFldMulti(HDTV_00, P_Fld(0,HDTV_FLD_SEL)|P_Fld(0,HDTV_RGB)|P_Fld(0,HDTV_CEN_SEL)); //Reset all SDTV, PROG, RGB, 422 Flag

    //field select => disable
    vRegWriteFldAlign(ASYNC_12, FIELD_DISABLE, AS_FLD_SELECT); // 00:ck27,01:pix,10:free run,11:disable

    bHdtvMDStateMachine = ZERO;
    bHdtvMCErrorCnt = 0;
#if (HDTV_240P_SUPPORT || HDTV_540P_SUPPORT || HDTV_288P_SUPPORT)
    bHdtvFieldErrorCnt = 0;
#endif
    vASSetSOGSync();
    bModeIndex = SYNCONGREEN; /* for checking the hardware mute setting */

#if ADAPTIVE_MONITOR_SLICER_MEASURE_ONLINE
    _bSetDefaultSlicer_Cnt = 0;
    _bSetDefaultSlicer_loop_Cnt = 0;
#endif

    SP0Initial();
    //enable c-deglitch
    vRegWriteFldAlign(ASYNC_00, 1 , AS_CSYNC_DGLITCH_SEL );	

    vRegWriteFldAlign(ASYNC_0B, 5, AS_C_DEGLITCH );

#ifdef FULLY_HW_AUTO_CALIBRATION
    vDrvHDTV_HW_AUTO_ONOFF(CALI_DISABLE, 0XFF);
    Printf("[SA7] -------vDrvHDTV_HW_AUTO_ONOFF----\n");
#endif
	//reset the HLEN_ACTIVE
    vRegWriteFldAlign(ASYNC_04, 0x50, AS_HLEN2_S); //set HLEN2_S
    vRegWriteFldAlign(ASYNC_04, 0x80, AS_HLEN2_E); //set HLEN2_E

}




/**
 * @brief 	HDTV Call-back function for source select connect or disconnect.
 * @param	bchannel - SV_VP_MAIN / SV_VP_PIP
 * @param	fgIsOn - SV_ON/SV_OFF
 * @retval	None
 */
void vHdtvConnect(UINT8 bchannel, UINT8 fgIsOn)
{
    //CKGEN_WRITE32(0x150, (CKGEN_READ32(0x150) & 0xfbffffff)); //Force Pixel Clock(27M) to Syncprocess0 by W.C Shi/Tomson
#ifdef SCART_CLK_SOURCE_CVBS
    vRegWriteFldAlign(SCART_02, 0x0, FLD_SCART_CVBS_H_SEL);    
#endif    
    if (bchannel == SV_VP_MAIN)
    {
        vVDOINIrqOff((MSK_SP0_VSYNCOUT | MSK_MAIN_DET | MSK_SP0_MD_CHG));
        _rYPBPRStat.bIsMain = fgIsOn;
    }
    if (bchannel == SV_VP_PIP)
    {
        vVDOINIrqOff((MSK_SP0_VSYNCOUT | MSK_PIP_DET | MSK_SP0_MD_CHG));
        _rYPBPRStat.bIsPip = fgIsOn;
    }

    if (fgIsOn == SV_ON)
    {
        vDrvCLKINFreeRun(ENABLE);	//CLKIN free run modify by W.C Shih to prevent the crash of DDS 2004/12/30
        _bSP0Flag = 0;
        vHdtvHwInit();
        vHdtvInitial(MDCHG_CON);

        _bHdtvModeChged = 0 ;
        _bHdtvChkState = HDTV_NO_SIGNAL;
        vHdtvSetModeCHG();
        _IsHdtvDetectDone = TRUE;

    }
    else if (fgIsOn == SV_OFF && !(fgIsMainYPbPr() || fgIsPipYPbPr()) )
    {
        vDrvCLKINFreeRun(ENABLE);	//CLKIN free run modify by W.C Shih to prevent the crash of DDS 2004/12/30

        _bHdtvTiming = MODE_NOSIGNAL;
#if ADAPTIVE_SLICER_DEFAULT_SETTING
        _bHdtvTiming_old = MODE_NOSIGNAL;
#endif        
        _IsHdtvDetectDone = TRUE;

    }
#if ADAPTIVE_SLICER_ENABLE
    vSetDefaultSlicer();
#endif
    // also check the decomposited vsync
    vRegWriteFldAlign(ASYNC_00, 1, AS_VSYNC_ACT_SEL);
}

// *********************************************************
// Function :UINT8 bHdtvModeDetectDone(void)
// Description :return the mode detect status
// Parameter : NONE
// Return    : SV_TRUE/SV_FALSE
// *********************************************************
/*UINT8 bHdtvModeDetectDone(void)
{
	return _IsHdtvDetectDone;
}*/

/**
 * @brief 	HDTV call-back function for getting HDTV input signal width.
 * @param	None
 * @retval	Height of input signal.
 */
UINT16 wHdtvInputWidth(void)
{
    if ((_IsHdtvDetectDone) && (_bHdtvTiming != NO_SIGNAL) && (_bHdtvTiming != MODE_NOSUPPORT) && (_bHdtvTiming < MAX_TIMING_FORMAT))
    {
        return (Get_VGAMODE_IPH_WID(_bHdtvTiming)>>(RegReadFldAlign(HDTV_00,HDTV_CEN_SEL)));
    }
    else
    {
        return (720);
    }
}


UINT16 wHdtvDEInputWidth(void)
{
        return RegReadFldAlign(HDTV_01, HDTV_AV_WIDTH);
}

/**
 * @brief 	HDTV call-back function for getting HDTV input signal width over sample
 * @param	None
 * @retval	Input width is oversample or not
 */

UINT8 bHdtvInputWidthOverSample(UINT8 bMode)
{
    if ((bMode == NO_SIGNAL) || (bMode == MODE_NOSUPPORT))
    {
        return FALSE;
    }

    if (RegReadFldAlign(HDTV_00,HDTV_CEN_SEL) && ((bMode == MODE_480P_OVERSAMPLE) ||(bMode == MODE_576P_OVERSAMPLE)))
    {
        return (FALSE);
    }
	else if (RegReadFldAlign(HDTV_00,HDTV_CEN_SEL) && ((bMode == MODE_525I) ||(bMode == MODE_625I)))
	{
		return (FALSE);
	}
        else if (_bHdtvOversampleToDi && ((bMode == MODE_525I_OVERSAMPLE) ||(bMode == MODE_625I_OVERSAMPLE)))
	{
	        //if drop pixel , return not predown
	        if(_bHdtvCenEnableForSD)
	        {
		    return (FALSE);
		 }
		 else
		 {
                    return (TRUE);
		 }
	}	
    else
    {
        return (Get_VGAMODE_OverSample(bMode));
    }
}


/**
 * @brief 	HDTV call-back function for getting HDTV input signal height.
 * @param	None
 * @retval	Height of input signal.
 */
UINT16 wHdtvInputHeight(void)
{
    if ((_IsHdtvDetectDone) && (_bHdtvTiming != NO_SIGNAL) && (_bHdtvTiming != MODE_NOSUPPORT) && (_bHdtvTiming < MAX_TIMING_FORMAT))
    {
        return (Get_VGAMODE_IPV_LEN(_bHdtvTiming));
    }
    else
    {
        return (480);
    }
}

/**
 * @brief 	HDTV call-back function used by display driver, video mainloop, video ISR general check.
 * @param	None
 * @retval	Refresh rate of input signal.
 */
UINT8 bHdtvRefreshRate(void)
{
    if ((_IsHdtvDetectDone) && (_bHdtvTiming != NO_SIGNAL) && (_bHdtvTiming != MODE_NOSUPPORT) && (_bHdtvTiming < MAX_TIMING_FORMAT))
    {
        return (Get_VGAMODE_IVF(_bHdtvTiming));
    }
    else
    {
        return (0);
    }
}

/**
 * @brief 	HDTV call-back function used by DI/Display/Scaler driver
 * @param	None
 * @retval	0 - progressive, 1 - interlace
 */
UINT8 bHdtvInterlace(void)
{
    if ((_IsHdtvDetectDone) && (_bHdtvTiming != NO_SIGNAL) && (_bHdtvTiming < MAX_TIMING_FORMAT))
    {
        return (Get_VGAMODE_INTERLACE(_bHdtvTiming));
    }
    return 0;
}



/**
 * @brief 	HDTV call-back function used by Scaler Driver
 * @param	bPath: SV_VP_MAIN/SV_VP_PIP
 * @retval	Vertical Back Porch for SCPOS Setting
 */
UINT16 wHdtvVPorch(UINT8 bPath) 	// 2005/01/10 modified by cindy
{
#ifndef CC_UP8032_ATV
    UNUSED(bPath);
#endif    

        return 0;

}

/**
 * @brief 	HDTV call-back function used by Scaler Driver
 * @param	bPath: SV_VP_MAIN/SV_VP_PIP
 * @retval	Horizontal Back Porch for SCPOS Setting
 */
UINT16 wHdtvHPorch(UINT8 bPath)
{
#ifndef CC_UP8032_ATV
    UNUSED(bPath);
#endif    
    return (0);
}

/**
 * @brief HDTV call-back function for vVdoMainState, vVdoPipState polling update video status to UI.
 * @param	None
 * @retval	SV_VDO_UNKNOWN / SV_VDO_NOSIGNAL / SV_VDO_NOSUPPORT / SV_VDO_STABLE
 */
UINT8 bHdtvSigStatus(void)
{
    if (!_IsHdtvDetectDone)
    {
        return (UINT8)SV_VDO_UNKNOWN;
    }
    else
    {
        switch (_bHdtvTiming)
        {
            case MODE_NOSIGNAL:
            case MODE_WAIT:            	
                return (SV_VDO_NOSIGNAL);

            case MODE_NOSUPPORT:
                return (SV_VDO_NOSUPPORT);

            default:
                return (SV_VDO_STABLE);
        }
    }
}

extern void vHdtvPhaseIsr(void);

/**
 * @brief 	Hdtv ISR - Set flags for HDTV related mainloop.
 *                            Hold Blank checking and setting
 * @param	None
 * @retval	None
 */
void vHdtvISR(void)
{
    if (fgIsMainYPbPr() || fgIsPipYPbPr())
    {
        if (fgIsVdoIntSp0Vsyncout())	//vsync out
        {
            if (_IsHdtvDetectDone)
            {
                vSetSP0Flg(SP0_VGA_AUTO_FLG | SP0_AUTOCOLOR_FLG);
                //YPbPr Auto Phase 2006/11/07
                if (_bAutoISR)
                {
                #if NEW_PHASE_ALGORITHM
                    vYPbPrPhaseIsr_New();
                #else
                    vYPbPrPhaseIsr();
                #endif
                }
                SDDS_wait_message();
                DDDS_wait_message();
                #if CHANGE_SDDS_KPI
                if(fgIsCLKLock())
                {
                 Set_SDDS_KPI(0);
                }
                #endif    
#ifdef CC_HDTV_NON_STANDARD            
				//read continuous 10 field    
				if (_bNoSTdCnt ==0)
                {
                     wSP0VCompare[1]  = wASVtotalMeasure() ;
                }
				else if (_bNoSTdCnt < 10)
             	{
                	if (wASVtotalMeasure() < wSP0VCompare[1])
                	{
                	 wSP0VCompare[1]  = wASVtotalMeasure() ;
                	}
             	}
				_bNoSTdCnt++;
#endif
            }
        }      
        if (fgIsCLKLock())
        {
            if(_RETIMENeedReset)
            {
                vDrvRETIMEReset();
            }
        }    
    }
}

// End
////////////////////////////////////////////////////////////////////////////////
/**
 * @brief vHdtvPolarityUniform( void )
 * According to XDATA fgSP0Hpol & fgSP0Vpol to set suitable setting in each step
 * @param  void
 * @retval void
 * @pre  Need to call the API of vHdtvPolarityUniform()
 */
PRIVATE void vHdtvPolarityUniform(void)
{

    vDrvCLKINFreeHsyncPol(1);
    vDrvCLKINHsyncPol(1);
    //27MHz (Input -/-) goal: --
    vDrvCsyncInvPol(0);
    vDrvHsInv((UINT8)FALSE);
    vDrvVsInv((UINT8)FALSE);
    //pixel clk : goal: ++
    vDrvHsLockInv((UINT8)TRUE);
    vDrvVsOutInvPol((UINT8)TRUE);
}

/**
 * @brief The Non-Standard Signal Status of current input signal of HDTV
 *
 * (Common Function of Video Decoders)
 * return the NSTD Signal Status of current input signal of HDTV.
 *
 * @param pHDTVNSTDStatus  - A pointer to RTvdNSTDStatus to receive HDTV status
 * @return the Signal Status of current video signal, return values include
 * fgIsNSTD, bRefreshRate, wVTotal and wVTotalDiff.
 *
 */
void vHDTVGetNSTDStatus(RHDTVNSTDStatus* pHDTVNSTDStatus)
{
    #ifndef CC_HDTV_NON_STANDARD
    _rHDTV_NSTDStatus.fgIsNSTD = 0;
    _rHDTV_NSTDStatus.wVTotal = Get_VGAMODE_IVTOTAL(_bHdtvTiming);
    _rHDTV_NSTDStatus.wHTotal = Get_VGAMODE_IHTOTAL(_bHdtvTiming);
    _rHDTV_NSTDStatus.bRefreshRate = Get_VGAMODE_IVF(_bHdtvTiming);
    _rHDTV_NSTDStatus.wVTotalDiff = 0x800;
    #endif
    
	pHDTVNSTDStatus->fgIsNSTD = _rHDTV_NSTDStatus.fgIsNSTD;
	pHDTVNSTDStatus->bRefreshRate = _rHDTV_NSTDStatus.bRefreshRate;
	pHDTVNSTDStatus->wVTotal = _rHDTV_NSTDStatus.wVTotal;
	pHDTVNSTDStatus->wHTotal = _rHDTV_NSTDStatus.wHTotal;
	pHDTVNSTDStatus->wVTotalDiff = _rHDTV_NSTDStatus.wVTotalDiff;
}


/**
 * @brief 	When VGA signal is not stable, check signal activity and then get hlen/vlen for timing search.
 * @param	None
 * @retval	None
 */
void vHdtvModeDetect(void)
{
    // Source select will do mode detect and INT will do ,too
    static UINT32 wTmp;
    if (!(fgIsMainYPbPr() || fgIsPipYPbPr()))
    {
        return;
    }

    if (!_IsHdtvDetectDone)
    {
        switch (bHdtvMDStateMachine)
        {
            case 0:
            case 2:
#if VGA_TIMEDELAY_DBG
                HAL_GetTime(&cur_time[2]);
                LOG(3,"[%d] %d:%d \n",2, cur_time[2].u4Seconds,cur_time[2].u4Micros);
#endif

                wSP0Hclk = wSP0IHSClock(wSP0HLength);
                bSP0Vclk= bSP0IVSClock(wSP0HLength,wSP0Vtotal);
                vHdtvPolarityUniform(); //for vlen calculate

                _bHdtvTiming = bHdtvTimingSearch();

                #if ADAPTIVE_SLICER_DEFAULT_SETTING
                if((_bHdtvTiming_old != _bHdtvTiming)&& (_bHdtvTiming_old != MODE_NOSIGNAL))
                {
                 DBG_Printf(DBG_MDET,"[SA7] Reset the _bCurSlicerIdx = %d ", _bCurSlicerIdx);                
                 _bCurSlicerIdx = 0;
                }
                _bHdtvTiming_old = _bHdtvTiming;
                #endif

                DBG_Printf(DBG_MDET, "wSP0Vtotal %d\n", wSP0Vtotal);
                DBG_Printf(DBG_MDET, "wSP0HLength %d\n", wSP0HLength);


#if ADAPTIVE_SLICER_ENABLE
                if (bHdtvOpt05_AdaptiveSlicer && fgIsVideoTiming(_bHdtvTiming))
                {
                    DBG_Printf(DBG_MDET, "slicer start\n" );
                    check_quaity_state=0;
                    vSliceQuality536x();/* fine-tune slicer quality here*/
                    bHdtvMDStateMachine = 5;
                }
                else
#endif
                {
                    bHdtvMDStateMachine=3;
                }
                break;
            case 3:
#if VGA_TIMEDELAY_DBG
                HAL_GetTime(&cur_time[3]);
                LOG(3,"[%d] %d:%d \n",3, cur_time[3].u4Seconds,cur_time[3].u4Micros);
#endif
                bHdtvMDStateMachine = 4;

                DBG_Printf(DBG_MDET,"hdtv timing %d\n", _bHdtvTiming);

                if ((_bHdtvTiming != MODE_NOSUPPORT) && (_bHdtvTiming != MODE_NOSIGNAL))
                {
#ifdef CC_HDTV_NON_STANDARD
                    _bNoSTdCnt = 0 ;
                    _wNoStdVtotal =  wSP0Vtotal ;
#endif

                    if (fgIsMainYPbPr())
                    {
                        vVDOINIrqOn((MSK_SP0_VSYNCOUT | MSK_MAIN_DET));
                    }
					if (fgIsPipYPbPr())
                    {
                        vVDOINIrqOn((MSK_SP0_VSYNCOUT | MSK_PIP_DET));
                    }

                    vDrvVGASetPhase_Simple(8);
                    vDrvADCPLLSet(Get_VGAMODE_ICLK(_bHdtvTiming), Get_VGAMODE_IHTOTAL(_bHdtvTiming));  //ADPART
                    vHdtvSetInputCapature(_bHdtvTiming, 1);


                }
                else
                {
                    _bHdtvTiming = MODE_NOSUPPORT;
                }
                /* gbsh following move to 4 */
                wTmp=bHdtvOpt01_MDmute0;
                break;
            case 4:
                if ((_bHdtvTiming)&&(_bHdtvTiming!=MODE_NOSUPPORT)&&(_bHdtvTiming!=MODE_WAIT)&&wTmp)
                {
                    wTmp--;

                    _bPLLlockCnt++;

                    if (!fgIsCLKLock()) //ADPART
                    {
                        break;//wait for DDS stable
                    }
                    else
                    {
                        if (bHdtvOpt01_MDmute1&&(wTmp>bHdtvOpt01_MDmute1))
                        {
                            wTmp=bHdtvOpt01_MDmute1;
                        }
                        break;	/* 4 more loop for really DDS stable*/
                    }
                }
#if VGA_TIMEDELAY_DBG
                HAL_GetTime(&cur_time[4]);
                LOG(3,"[%d] %d:%d \n",4, cur_time[4].u4Seconds,cur_time[4].u4Micros);
#endif
                _IsHdtvDetectDone = TRUE;
                bActStableCnt=0;

                if (_bHdtvTiming == MODE_NOSIGNAL)
                {
                    _bHdtvChkState = HDTV_NO_SIGNAL;
                }
                else
                {
                    _bHdtvChkState = HDTV_CHK_MODECHG;
                }

                if ((_bHdtvTiming != MODE_NOSUPPORT)   && (_bHdtvTiming != MODE_NOSIGNAL))
                {
                    //Printf("wASVtotalMeasure() %d\n", wASVtotalMeasure());
                    _wSP0StableVtotal = wASVtotalMeasure();
                    DBG_Printf(DBG_MDET,"_bPLLlockCnt  %d %d\n", _bPLLlockCnt, _wSP0StableVtotal);

                }

                vHdtvSetModeDone();             
                #ifdef CC_HDTV_NON_STANDARD
                vHDTVChkNSTD(wSP0VCompare[2]);
                #endif

#if SUPPORT_HDTV_HARDWARE_MUTE
                /* clear SYNC0_MUTE */
                vRegWriteFldAlign(ASYNC_0D, 1, AS_MUTE_CLR);
                vRegWriteFldAlign(ASYNC_0D, 0, AS_MUTE_CLR);
#endif
                break;
            case 5:
#if VGA_TIMEDELAY_DBG
                HAL_GetTime(&cur_time[5]);
                LOG(3,"[%d] %d:%d \n",5, cur_time[5].u4Seconds,cur_time[5].u4Micros);
#endif
#if ADAPTIVE_SLICER_ENABLE
                if (check_quaity_state==10)
                {
                    bHdtvMDStateMachine = 3;
                    DBG_Printf(DBG_MDET, "slicer end\n" );
                }
                else
                {
                    vSliceQuality536x();/* fine-tune slicer quality here*/
                }
#endif
                break;
        }
    }
}

UINT8 bSP0HVCheck(void)
{
    UINT16 wHsync, wVsync ;

    wHsync = wASHSyncWidthMeasure() ;
    wVsync = wASVSyncWidthMeasure() ;
    DBG_Printf(DBG_MDET,  "h/v sync width %d %d \n", wHsync, wVsync);
    if ((wSP0VCompare[0] < 240) || (wSP0VCompare[0] > 1600)) //vtotal upper and lower bound
    {
        DBG_Printf(DBG_MDET,  "v len over range\n");
        return FALSE;
    }
    if ((wSP0HCompare1 > 2300) ||(wSP0HCompare1 < 270))
    {
        DBG_Printf(DBG_MDET,  "h len over range\n");
        return FALSE;

    }
    if ((wHsync == 0 ) || (wVsync == 0))
    {
        DBG_Printf(DBG_MDET, "h/v sync width 0\n");
        return FALSE;
    }
#if 0
    if ((wSP0HCompare1 <  (6* wHsync)) && (bModeIndex == SYNCONGREEN))
    {
        DBG_Printf(DBG_MDET,  "h sync width fail\n");
        return FALSE;
    }
#endif	
    if ((wVsync > bVgaOpt06_SogMaxVsyncWidth)  && (bModeIndex == SYNCONGREEN))
    {
        DBG_Printf(DBG_MDET,  "v sync width fail\n");
        return FALSE;
    }

    if((bModeIndex == SYNCONGREEN) || bModeIndex == COMPOSITESYNC)
    {
       if(RegReadFldAlign(STA_SYNC0_02,AS_VSYNC_OUTX_ACT)==0)
       {
            DBG_Printf(DBG_MDET,  "vsync outx fail\n");     
            return FALSE ;
                   
       }     
    }

    return TRUE ;
}
/**
* @brief 	vHdtvSetModeCHG(void);Flag responses mode change
* @param	None
* @retval	None
*/
void vHdtvSetModeCHG(void)
{
    if (_bHdtvModeChged)
    {
        return ;
    }
    _bHdtvModeChged = 1 ;
    #ifndef CC_UP8032_ATV
    HAL_GetTime(&_rHdtvModeChgTime);
    #endif

    if (fgIsMainYPbPr())
    {   
        #ifdef CC_COPLAT_MT5387_TODO
        _vDrvVideoSetMute(MUTE_MODULE_MODECHG, SV_VP_MAIN, FOREVER_MUTE, FALSE);
        #endif
        vClrMainFlg(MAIN_FLG_MODE_DET_DONE);
        vSetMainFlg(MAIN_FLG_MODE_CHG);
        DBG_Printf(DBG_MDET, "Main Mode Chg #1 in vHdtvInitial\n");
    }
    if (fgIsPipYPbPr())
    {   
        #ifdef CC_COPLAT_MT5387_TODO
        _vDrvVideoSetMute(MUTE_MODULE_MODECHG, SV_VP_PIP, FOREVER_MUTE, FALSE);
        #endif
        vClrPipFlg(PIP_FLG_MODE_DET_DONE);
        vSetPipFlg(PIP_FLG_MODE_CHG);
        DBG_Printf(DBG_MDET, "Pip Mode Chg #1 in vHdtvInitial\n");
    }

}

void vHdtvSetModeDone(void)
{
    if (!_bHdtvModeChged)
    {
        return ;
    }
    if (fgIsMainYPbPr())
    {
        vSetMainFlg(MAIN_FLG_MODE_DET_DONE);
        DBG_Printf(DBG_MDET, "Main Mode Done\n");
    }
    if (fgIsPipYPbPr())
    {
        vSetPipFlg(PIP_FLG_MODE_DET_DONE);
        DBG_Printf(DBG_MDET, "Pip Mode Donel\n");
    }
    _bHdtvModeChged = 0 ;
}

#define HDTV_FORCE_MODET_DONE_TIME    1 //sec
#define HDTV_HLEN_STABLE_THR     2
#define HDTV_VLEN_STABLE_THR     2

#ifdef CC_HDTV_NON_STANDARD
#define HDTV_HLEN_MDCFG_THR 4 // For 5387 HSync & V Line Fix pull in
#define HDTV_VLEN_MDCFG_THR 5
#else
#define HDTV_HLEN_MDCFG_THR 10
#define HDTV_VLEN_MDCFG_THR 5
#endif

#ifdef CC_HDTV_NON_STANDARD
/**
 * @brief measure if current input signal is Non-Standard Signal Status or not
 *
 * (Common Function of Video Decoders)
 * return none.
 *
 * @param VTotal_Measure
 * @return none
 *
 */
void vHDTVChkNSTD(UINT16 VTotal_Measure)
{

    UINT8 bInterlaced;
    UINT32 wHDTV_IVF;
    UINT32 HTotal_Measure_27MHz;

    bInterlaced = Get_VGAMODE_INTERLACE(_bHdtvTiming);
    HTotal_Measure_27MHz = wASHLenMeasure();

    wHDTV_IVF = Get_VGAMODE_IVTOTAL(_bHdtvTiming)/(bInterlaced+1);
    
    if (VTotal_Measure != wHDTV_IVF)
    {
    _rHDTV_NSTDStatus.fgIsNSTD = 1;
    
    if(bInterlaced)
    _rHDTV_NSTDStatus.wVTotal = (VTotal_Measure*2+1);
    else
    _rHDTV_NSTDStatus.wVTotal = VTotal_Measure;
    
    _rHDTV_NSTDStatus.wHTotal = Get_VGAMODE_IHTOTAL(_bHdtvTiming);       
    _rHDTV_NSTDStatus.bRefreshRate = (UINT8)((27000000*(bInterlaced+1)+((UINT32)(HTotal_Measure_27MHz) * (UINT32)(_rHDTV_NSTDStatus.wVTotal))/2) / ((UINT32)(HTotal_Measure_27MHz) * (UINT32)(_rHDTV_NSTDStatus.wVTotal)));

    if (VTotal_Measure > wHDTV_IVF)
    {    
      _rHDTV_NSTDStatus.wVTotalDiff = 0x800+(VTotal_Measure-wHDTV_IVF)*(bInterlaced+1);
    }
    else if (VTotal_Measure < wHDTV_IVF)
    {
      _rHDTV_NSTDStatus.wVTotalDiff = 0x800-(wHDTV_IVF-VTotal_Measure)*(bInterlaced+1); 
    } 
     DBG_Printf(DBG_MCHG,"[SA7]==Non_Std==fgIsNSTD=%d\n", _rHDTV_NSTDStatus.fgIsNSTD);    
     DBG_Printf(DBG_MCHG,"[SA7]wVTotal=%d\n", _rHDTV_NSTDStatus.wVTotal);
     DBG_Printf(DBG_MCHG,"[SA7]wHTotal=%d\n", _rHDTV_NSTDStatus.wHTotal);
     DBG_Printf(DBG_MCHG,"[SA7]bRefreshRate=%d\n", _rHDTV_NSTDStatus.bRefreshRate);      
     DBG_Printf(DBG_MCHG,"[SA7]wVTotalDiff=0x%x\n", _rHDTV_NSTDStatus.wVTotalDiff);                                          
     DBG_Printf(DBG_MCHG,"[SA7]VTotal_Measure=%d\n", VTotal_Measure);                                                                                                                                                   
    }
    else
    {
     _rHDTV_NSTDStatus.fgIsNSTD = 0;
     _rHDTV_NSTDStatus.wVTotal = Get_VGAMODE_IVTOTAL(_bHdtvTiming);
     _rHDTV_NSTDStatus.wHTotal = Get_VGAMODE_IHTOTAL(_bHdtvTiming);
     _rHDTV_NSTDStatus.bRefreshRate = Get_VGAMODE_IVF(_bHdtvTiming);
     _rHDTV_NSTDStatus.wVTotalDiff = 0x800;
     DBG_Printf(DBG_MCHG,"[SA7]fgIsNSTD=%d\n", _rHDTV_NSTDStatus.fgIsNSTD);    
     DBG_Printf(DBG_MCHG,"[SA7]wVTotal=%d\n", _rHDTV_NSTDStatus.wVTotal);
     DBG_Printf(DBG_MCHG,"[SA7]wHTotal=%d\n", _rHDTV_NSTDStatus.wHTotal);
     DBG_Printf(DBG_MCHG,"[SA7]bRefreshRate=%d\n", _rHDTV_NSTDStatus.bRefreshRate);      
     DBG_Printf(DBG_MCHG,"[SA7]wVTotalDiff=0x%x\n", _rHDTV_NSTDStatus.wVTotalDiff);                                           
     DBG_Printf(DBG_MCHG,"[SA7]VTotal_Measure=%d\n", VTotal_Measure);                                                                        
    }

}
#endif

UINT8 HDTV_HV_KEEP_STABLE_THR=20;//10;for [DTV00207472]
#if ADAPTIVE_MONITOR_SLICER_MEASURE_ONLINE
 #define _bSetDefaultSlicer_Cnt_th 14
#endif

void vHdtvChkModeChange(void)
{
    UINT8 bCurrentSignal,  bModChg;
    UINT16 whtemp, wvtemp;
#ifdef CC_HDTV_NON_STANDARD
    UINT8 bInterlaced;
#endif    
#if ADAPTIVE_SLICER_PLLERR_CHK
        UINT16 wDDS_MAX_PERR_temp;
#endif
#if ADAPTIVE_MONITOR_SLICER_MEASURE
    UINT16 wVwidtemp;    
#endif

    if (fgIsSP0FlgSet(SP0_MCHG_BYPASS_FLG))
    {
        return;
    }

    if (fgIsMainYPbPr() || fgIsPipYPbPr())
    {
        bCurrentSignal = bASHDTVActiveChk();
        // check unstable than time out for mode detect done
        if (_bHdtvModeChged)
        {
            HAL_TIME_T rCurTime, rDeltaTime;
            HAL_GetTime(&rCurTime);
            HAL_GetDeltaTime(&rDeltaTime, &_rHdtvModeChgTime, &rCurTime);
            if ( (rDeltaTime.u4Seconds) >= HDTV_FORCE_MODET_DONE_TIME) // 2 sec
            {
                _bHdtvTiming = MODE_NOSIGNAL;
                DBG_Printf(DBG_MDET, "mc:  Force mode detect done  %d\n", _bHdtvTiming);
#ifndef  SUPPORT_AV_COMP_AUTO
                vHdtvSetModeDone();
#endif
#if 0//ADAPTIVE_SLICER_ENABLE
                vNextSlicer(100);
#endif
            }
        }


        if (_bHdtvChkState != HDTV_NO_SIGNAL)
        {
            if ((bCurrentSignal & 0x3)!=3)
            {
                if (++NoSignalCnt >= 24)//6 // for long time to stop audio play
                {
                    DBG_Printf(DBG_MDET,"mc: from activity detected to no signal\n");
                    vHdtvInitial(MCHG_NOSIG);
                    if (_bHdtvTiming != MODE_NOSIGNAL)
                    {

                        vHdtvSetModeCHG();
                        vHdtvSetModeDone();// for long time to stop audio play
                    }

                    _bHdtvTiming = MODE_NOSIGNAL;
                    _bHdtvChkState = HDTV_NO_SIGNAL ;
#if ADAPTIVE_SLICER_ENABLE                    
                    vResetSliceTimer();
                    vNextSlicer(0);
#endif                    
                    NoSignalCnt = 0 ;
                }
                return ;
            }
            else
            {
                NoSignalCnt = 0 ;
            }
        }
        wvtemp = wASVtotalMeasure();
        whtemp = wASHLenMeasure();
        #if ADAPTIVE_MONITOR_SLICER_MEASURE
        wVwidtemp = wASVSyncWidthMeasure();
        #if ADAPTIVE_MONITOR_SLICER_MEASURE_ONLINE
        if(_bHdtvChkState == HDTV_WAIT_STABLE)
        {
         if(bHdtvMCErrorCnt<18)
         {
          vAS2SyncMeasure(SP2_Specific_LVL);
         }
         else if (bHdtvMCErrorCnt>20)
         {
          vAS2SyncMeasure(SP2_Default_LVL);
         } 
        }
        else if (_bHdtvChkState == HDTV_CHK_MODECHG)
        {
         if(_bSetDefaultSlicer_Cnt<2)
         {
          vAS2SyncMeasure(SP2_Specific_LVL);
         } 
         
         else if (_bSetDefaultSlicer_Cnt>=4)
         {
          vAS2SyncMeasure(SP2_Default_LVL);
         }
        }
        #else
          vAS2SyncMeasure(SP2_Specific_LVL);        
        #endif
        #endif        

        switch (_bHdtvChkState)
        {
            case HDTV_NO_SIGNAL :
                if ((bCurrentSignal & 0x3) == 3)
                {
                    DBG_Printf(DBG_MDET, "mc: from no signal to wait stable\n");
                    _bHdtvChkState = HDTV_WAIT_STABLE ;
                    bHdtvMCErrorCnt = 0;
                    vHdtvInitial(MCHG_SIGIN);
                    vResetVLen();
#if ADAPTIVE_SLICER_ENABLE                    
                    vResetSliceTimer();
#endif                    
                }
                else
                {
#if ADAPTIVE_SLICER_ENABLE
                    vNextSlicer(100);
#endif
                }

                break;
            case HDTV_CHK_MODECHG:
                bModChg = MCHG_NO_CHG;
                if (fgIsVideoTiming(_bHdtvTiming))
                {
#if ADAPTIVE_SLICER_PLLERR_CHK
                    wDDS_MAX_PERR_temp=vDDS_MAX_PERR();
#endif                
                    // Test by W.C Shih 2005/07/06
                    if (!fgIsCLKLock())
                    {
                        _bUnLockCnt++;
                        if ((_bUnLockCnt>10) && (!fgIsAutoFlgSet(SP0_AUTO_ALL))) // vga auto may be unlock
                        {
                            DBG_Printf(DBG_MDET,"Mchg : Unlock %d\n",	_bUnLockCnt);
                            bModChg = MCHG_UNLOCK ;
                        }
                    }
#if ADAPTIVE_SLICER_PLLERR_CHK
                    else if (fgIsCLKLock()&&(wDDS_MAX_PERR_temp>0x300)) // >0x700))
                    {
                     _bUnLockCnt++;                                        
                     DBG_Printf(DBG_SLICER," [SA7] _bUnLockCnt %d vDDS_MAX_PERR %d\n", _bUnLockCnt,wDDS_MAX_PERR_temp);
                    
                      if ((_bUnLockCnt>15) && (!fgIsAutoFlgSet(SP0_AUTO_ALL)))
                      {
                       DBG_Printf(DBG_SLICER,"[SA7] Lock but Error to large : vDDS_MAX_PERR %d\n", wDDS_MAX_PERR_temp);
                       bModChg = MCHG_UNLOCK ;
                      }
                    }
#endif                    
                    else
                    {
                        _bUnLockCnt=ZERO;
                    }
                   // check non-standard signal

#if ADAPTIVE_MONITOR_SLICER_MEASURE_ONLINE    
                 if(bHdtvOpt05_AdaptiveSlicer)
                 {  
                   if(((_bCurSlicerIdx>=1)||((_bCurSlicerIdx==0)&&(_bCurSlicerIdx_best!=0))))
                   {

                   #if ADAPTIVE_MONITOR_SLICER_MEASURE_DEBUG
                   if(_bSetDefaultSlicer_Cnt>=2)
                   {
                    DBG_Printf(DBG_SLICER,"[1].SP2_V_LEN_S = %d , Cnt =%d, bReadMONSlicer = 0x%x \n",wSP2VCompare[SP2_Default_LVL].SP2_V_LEN_S,_bSetDefaultSlicer_Cnt,bReadMONSlicer());
                   }
                   #endif
                   
                   if(((wSP2VCompare[SP2_Specific_LVL].SP2_H_LEN_S<=(whtemp+1))&&(wSP2VCompare[SP2_Specific_LVL].SP2_H_LEN_S>=(whtemp-1)))
                   &&((wSP2VCompare[SP2_Specific_LVL].SP2_V_LEN_S<=(wvtemp+1))&&(wSP2VCompare[SP2_Specific_LVL].SP2_V_LEN_S>=(wvtemp-1)))
                   &&((wSP2VCompare[SP2_Specific_LVL].SP2_V_WIDTH_S<=(wVwidtemp+1))&&(wSP2VCompare[SP2_Specific_LVL].SP2_V_WIDTH_S)>=(wVwidtemp-1))
                   )
                   {
                       if((_bSetDefaultSlicer_Cnt==_bSetDefaultSlicer_Cnt_th))
                       {
                        _bSetDefaultSlicer_loop_Cnt++;
                        #if ADAPTIVE_MONITOR_SLICER_MEASURE_DEBUG
                         DBG_Printf(DBG_SLICER,"_bSetDefaultSlicer_loop_Cnt = %d \n",_bSetDefaultSlicer_loop_Cnt);
                        #endif
                        if (_bSetDefaultSlicer_loop_Cnt>=2)
                        {
                         #if ADAPTIVE_MONITOR_SLICER_MEASURE_DEBUG
                         DBG_Printf(DBG_SLICER,"[SA7][HDTV_CHK_MODECHG] vSetDefaultSlicer\n");                                            
                         DBG_Printf(DBG_SLICER,"bReadNewSlicer = 0x%x,bReadMONSlicer = 0x%x \n",bReadNewSlicer(),bReadMONSlicer());
                         DBG_Printf(DBG_SLICER,"SP2_H_LEN_S = %d,whtemp = %d \n",wSP2VCompare[SP2_Specific_LVL].SP2_H_LEN_S,whtemp);        
                         DBG_Printf(DBG_SLICER,"SP2_V_LEN_S = %d,wvtemp = %d \n",wSP2VCompare[SP2_Specific_LVL].SP2_V_LEN_S,wvtemp); 
                         DBG_Printf(DBG_SLICER,"SP2_V_WIDTH_S = %d,wVwidtemp = %d \n",wSP2VCompare[SP2_Specific_LVL].SP2_V_WIDTH_S,wVwidtemp);                                                         
                         DBG_Printf(DBG_SLICER,"[1].SP2_V_LEN_S = %d  \n",wSP2VCompare[SP2_Default_LVL].SP2_V_LEN_S);
                         #endif
                         vSetDefaultSlicer();                                                                                 
                         _bSetDefaultSlicer_loop_Cnt = 0;
                        } 
                       }
                   }    
                   if(((wSP2VCompare[SP2_Default_LVL].SP2_V_LEN_S<=(wvtemp+1))&&(wSP2VCompare[SP2_Default_LVL].SP2_V_LEN_S>=(wvtemp-1)))
                   && (_bSetDefaultSlicer_Cnt>=8)
                   )
                   {
                    _bSetDefaultSlicer_Cnt++;             
                   }
                   else if(_bSetDefaultSlicer_Cnt<8)
                   {
                    _bSetDefaultSlicer_Cnt++;    
                   }
                   else
                   {
                    _bSetDefaultSlicer_Cnt=0; 
                    _bSetDefaultSlicer_loop_Cnt = 0;
                   }
                   
                   if(_bSetDefaultSlicer_Cnt>_bSetDefaultSlicer_Cnt_th)
                   {
                    _bSetDefaultSlicer_Cnt=0;                      
                   } 
                   
                   if (_bSetDefaultSlicer_Cnt==0)
                   {
                    vSetMONSlicer_Matrix();
                   }
                   else if (_bSetDefaultSlicer_Cnt==3)  
                   {
                    vSetMONSlicer(0x69);
                   }
                   else if (_bSetDefaultSlicer_Cnt==4)  // skip transient
                   {
                    vResetVLenSP2();
                   }                   
                   else if (_bSetDefaultSlicer_Cnt==5)  // skip transient
                   {
                    vResetVLenSP2();
                   }                   
                }   
                }
                else
                {
                  _bSetDefaultSlicer_Cnt = 0;
                  _bSetDefaultSlicer_loop_Cnt = 0;
                }
#endif                
 

#ifdef CC_HDTV_NON_STANDARD
                    bInterlaced = Get_VGAMODE_INTERLACE(_bHdtvTiming);
                       	
				if (_bNoSTdCnt> 10)
				{
                            if (wSP0VCompare[1]  != _wNoStdVtotal)
                            {
                              UINT8 offset ;  
                              vHDTVChkNSTD(wSP0VCompare[1]);
					          offset = RegReadFldAlign(ASYNC_11, AS_NEW_VS_OUTP_S1);//Get_VGAMODE_IPV_STA(_bHdtvTiming);

                                _wNoStdVtotal = wSP0VCompare[1]  ;
								DBG_Printf(DBG_MCHG, " no-std vtal %d\n", _wNoStdVtotal);
                                

								wSP0VCompare[1] = (Get_VGAMODE_IPV_LEN(_bHdtvTiming)/(bInterlaced+1)) + _wNoStdVtotal - (Get_VGAMODE_IVTOTAL(_bHdtvTiming)/(bInterlaced+1));
                                vDrvAsyncVsyncOut(offset, wSP0VCompare[1]);

                            }
                            _bNoSTdCnt = 0 ;
                 }
#endif

                } //if(_bVgaTiming !=MODE_NOSUPPORT)

#if (HDTV_240P_SUPPORT  || HDTV_540P_SUPPORT || HDTV_288P_SUPPORT)
#if YPBPR_480IP_27MHZ
                if ((_bHdtvTiming == MODE_525I) || (_bHdtvTiming == MODE_240P)
#else
                if ((_bHdtvTiming == MODE_525I_OVERSAMPLE) || (_bHdtvTiming == MODE_240P)
#endif                
#if HDTV_540P_SUPPORT
                    || (_bHdtvTiming == MODE_1080i) || (_bHdtvTiming == MODE_540P)
#endif
#if HDTV_288P_SUPPORT
#if YPBPR_480IP_27MHZ
                    || (_bHdtvTiming == MODE_625I) || (_bHdtvTiming == MODE_288P)
#else
                    || (_bHdtvTiming == MODE_625I_OVERSAMPLE) || (_bHdtvTiming == MODE_288P)
#endif
#endif

                   )
                {
                    if (bDrvASFieldAct() != _bFieldAct)
                    {
                        bHdtvFieldErrorCnt++;
                        if (bHdtvFieldErrorCnt>20)
                        {
                            bModChg = MCHG_FLD_CHG ;
                            DBG_Printf(DBG_MCHG, "bDrvASFieldAct() != _bFieldAct \n");
                        }
                    }
                    else
                    {
                        bHdtvFieldErrorCnt=0;
                    }
                }
#endif

                if ( (wSP0HLength<(whtemp+HDTV_HLEN_MDCFG_THR))&&(wSP0HLength>(whtemp-HDTV_HLEN_MDCFG_THR))&&
                     (wSP0Vtotal<(wvtemp+HDTV_VLEN_MDCFG_THR))&&(wSP0Vtotal>(wvtemp-HDTV_VLEN_MDCFG_THR)))
                {
                    bHdtvMCErrorCnt = 0;
                }
                else
                {
                    bHdtvMCErrorCnt++;
                    if (bHdtvMCErrorCnt> 3)
                    {
                        DBG_Printf(DBG_MCHG, "Mchg : H/V Chg\n");
                        DBG_Printf(DBG_MCHG, "wSP0VCompare0 %d\n", wvtemp);
                        DBG_Printf(DBG_MCHG, "wSP0Vtotal %d\n", wSP0Vtotal);
                        DBG_Printf(DBG_MCHG, "wSP0HCompare1 %d\n", whtemp);
                        DBG_Printf(DBG_MCHG, "wSP0HLength %d\n", wSP0HLength);
                        bModChg = MCHG_HVLEN_CHG;
                    }
                }

#if SUPPORT_HDTV_HARDWARE_MUTE
                if((RegReadFldAlign(STA_SYNC0_01, AS_SYNC0_MUTE)!=0) && (_bHdtvTiming !=MODE_NOSUPPORT))
                {
                    bModChg = MCHG_HW_DET; 
                }
#endif

#if HDTV_MV_DETECT_SUPPORT
			if (_bHdtvTiming != MODE_NOSUPPORT && (Get_VGAMODE_IPV_LEN(_bHdtvTiming) <= 720) ) //SDTV Timing , check MV status
			{
				if (_bHdtvMvOn != bDrvAsyncMvStatus())
				{
					if(_bHdtvMvChgCnt++ > 5)
					{
						_bHdtvMvOn = bDrvAsyncMvStatus();
						DBG_Printf(DBG_MCHG, "_bHdtvMvOn %d\n", _bHdtvMvOn);					
						vApiNotifyMVChg(((fgIsMainYPbPr())?SV_VP_MAIN:SV_VP_PIP),_bHdtvMvOn);
						_bHdtvMvChgCnt = 0 ;
					}					
				}
				else
				{
					_bHdtvMvChgCnt = 0 ;
				}

			}
#endif
                if (bModChg)
                {
                    _bHdtvChkState = HDTV_WAIT_STABLE ;
                    bHdtvMCErrorCnt = 0;
                    if (fgIsVideoTiming(_bHdtvTiming))
                    {
                        vHdtvSetModeCHG();
                    }
                    vHdtvInitial(bModChg);
                    vResetVLen();
#if ADAPTIVE_SLICER_ENABLE                   
                    vResetSliceTimer();
                    vNextSlicer(0);
#endif                    
                }
                break;
            case HDTV_WAIT_STABLE:
#if ASYNC_FULL_SCREEN_WA				
				vDrvAsyncSetFieldDet(whtemp);
#endif
                if (((whtemp >= (wSP0HCompare1 - HDTV_HLEN_STABLE_THR)) && (whtemp <= (wSP0HCompare1 + HDTV_HLEN_STABLE_THR))) &&
                    ((wvtemp >= (wSP0VCompare[0] - HDTV_VLEN_STABLE_THR)) && (wvtemp <= (wSP0VCompare[0] + HDTV_VLEN_STABLE_THR))))
                {
                    if (bHdtvMCErrorCnt++ > HDTV_HV_KEEP_STABLE_THR)
                    {
                        if (bSP0HVCheck())
                        {
                            DBG_Printf(DBG_MDET, "mc:wait stable to timing search \n");
                            //vHdtvInitial(6);
                            wSP0HLength= whtemp ;
                            wSP0Vtotal= wvtemp;
                            vHdtvSetModeCHG();
                            _IsHdtvDetectDone = FALSE;
                            #if ADAPTIVE_MONITOR_SLICER_MEASURE                            
                             if(bHdtvOpt05_AdaptiveSlicer)
                             {                             
                              #if ADAPTIVE_MONITOR_SLICER_MEASURE_DEBUG
                              DBG_Printf(DBG_SLICER,"bReadNewSlicer = 0x%x,bReadMONSlicer = 0x%x \n",bReadNewSlicer(),bReadMONSlicer());
                              #endif
                              DBG_Printf(DBG_SLICER,"SP2_H_LEN_S = %d,whtemp = %d \n",wSP2VCompare[SP2_Specific_LVL].SP2_H_LEN_S,whtemp);        
                              DBG_Printf(DBG_SLICER,"SP2_V_LEN_S = %d,wvtemp = %d \n",wSP2VCompare[SP2_Specific_LVL].SP2_V_LEN_S,wvtemp); 
                              DBG_Printf(DBG_SLICER,"SP2_V_LEN_S = %d,wVwidtemp = %d \n",wSP2VCompare[SP2_Specific_LVL].SP2_V_WIDTH_S,wVwidtemp);                                                         
                              #if ADAPTIVE_MONITOR_SLICER_MEASURE_ONLINE
                              DBG_Printf(DBG_SLICER,"[1].SP2_V_LEN_S = %d \n",wSP2VCompare[SP2_Default_LVL].SP2_V_LEN_S);                                                                                     
                              #endif
                              if(((wSP2VCompare[SP2_Specific_LVL].SP2_H_LEN_S<=(whtemp+1))&&(wSP2VCompare[SP2_Specific_LVL].SP2_H_LEN_S>=(whtemp-1)))
                              &&((wSP2VCompare[SP2_Specific_LVL].SP2_V_LEN_S<=(wvtemp+1))&&(wSP2VCompare[SP2_Specific_LVL].SP2_V_LEN_S>=(wvtemp-1)))
                              &&((wSP2VCompare[SP2_Specific_LVL].SP2_V_WIDTH_S<=(wVwidtemp+1))&&(wSP2VCompare[SP2_Specific_LVL].SP2_V_WIDTH_S)>=(wVwidtemp-1))
                              #if ADAPTIVE_MONITOR_SLICER_MEASURE_ONLINE
                              &&((wSP2VCompare[SP2_Default_LVL].SP2_V_LEN_S<=(wvtemp+1))&&(wSP2VCompare[SP2_Default_LVL].SP2_V_LEN_S>=(wvtemp-1)))
                              #endif
                              &&(_bCurSlicerIdx>=1))                            
                              {
                               DBG_Printf(DBG_SLICER,"[SA7]vSetDefaultSlicer\n");
                               vSetDefaultSlicer(); 
                               vResetSliceTimer();                                                                                  
                              }
                             }        
                            #endif                            
                        }
                        else
                        {
                            bHdtvMCErrorCnt = 0 ;
                            #ifdef CC_HDTV_NON_STANDARD
                            wSP0VCompare[2]=0xFFFF;
                            #endif
                        }
                    }                    
                    else
                    {
                    
                    #ifdef CC_HDTV_NON_STANDARD                    
                      if (wASVtotalMeasure() < wSP0VCompare[2])
                      {
                        wSP0VCompare[2] = wASVtotalMeasure();
                      }
                    #endif                    
                   
                   #if ADAPTIVE_MONITOR_SLICER_MEASURE_ONLINE
                    if(bHdtvOpt05_AdaptiveSlicer)
                    {                     
                      if(bHdtvMCErrorCnt == (HDTV_HV_KEEP_STABLE_THR/2))
                      {
                       vSetMONSlicer(0x69);
                      } 
                      else if(bHdtvMCErrorCnt == ((HDTV_HV_KEEP_STABLE_THR/2)+2))
                      {
                       vResetVLenSP2();
                      }
                    }                      
                   #endif                                            
                   }
                }
                else
                {
                    bHdtvMCErrorCnt = 0;
                    #ifdef CC_HDTV_NON_STANDARD
                    wSP0VCompare[2]=0xFFFF;
                    #endif     
                }
                DBG_Printf(DBG_MDET,  "H/V Clk Chg, V:%d H:%d %d\n", wvtemp, whtemp, bHdtvMCErrorCnt);
#if ADAPTIVE_SLICER_ENABLE
                vNextSlicer(700);
#endif
                //update  for next check
                wSP0HCompare1 = whtemp;
                wSP0VCompare[0] = wvtemp;
                break;
            default:
                break;
        }

    }
}

#if SUPPORT_SCART
#define SCART_DEBUG	0
#if SCART_DEBUG
#define ScartDBG(fmt...)  Printf(fmt)
#else
#define ScartDBG(fmt...)
#endif

// init the HDTV decoder for 576i
void vDrvScartRGBHdtvInit(void)
{
    UINT32 _bOrder;

    ScartDBG("vDrvScartRGBHdtvInit");
    vHdtvHwInit();
    vDrvAllHDADCPow(SV_TRUE);		//for power saving , power on HD ADC, Yulia add for Scart 2007.11.4

    _bOrder = (UINT32)DRVCUST_OptGet(eSCARTRGBOrder);

    vRegWriteFldAlign(HDTV_03,_bOrder,HDTV_DATA_SEL);//Set HDTV DATA SEL : Order=>0x7 :{Y,Pb,Pr}


    vHdtvReset();

    vDrvCLKINSyncSel(DCLK_IN_SOG);        //PLL HS Mux

    vRegWriteFldMulti(HDTV_00, P_Fld(0,HDTV_FLD_SEL)|P_Fld(1,HDTV_RGB)|P_Fld(0,HDTV_CEN_SEL)); //Reset all SDTV, PROG, RGB, 422 Flag  RGB ON

    vASSetSOGSync();
    bModeIndex = SYNCONGREEN;

    SP0Initial();

    vHdtvPolarityUniform();

    vScartRGBChkModeChange();
    vDrvCLKINFreeRun(ENABLE);

#if SUPPORT_HDTV_HARDWARE_MUTE
    /* disable hardware mute functionality */
    vDrvAsyncSetMuteCriteria(0);
    /* clear SYNC0_MUTE */
    vRegWriteFldAlign(ASYNC_0D, 1, AS_MUTE_CLR);
    vRegWriteFldAlign(ASYNC_0D, 0, AS_MUTE_CLR);
#endif
#if defined(SCART_RGB_MENU_WA) || defined(SCART_CLK_SOURCE_CVBS) 
    vRegWriteFldMulti(HDTV_03, P_Fld(0,HDTV_H_SEL)|P_Fld(3,HDTV_V_SEL)); //wu  
    vRegWriteFldAlign(HDTV_00, 0x1, HDTV_FLD_SEL); //filed free run
    vRegWriteFldMulti(ASYNC_08, P_Fld(0,AS_MASK_SLICE_EN)|P_Fld(1,AS_VMASK1_OFF)|P_Fld(1,AS_VMASK2_OFF)|P_Fld(1,AS_VMASK3_OFF));
    vRegWriteFldAlign(SCART_02, 0x1, FLD_SCART_CVBS_H_SEL); 
    vRegWriteFldAlign(SyncSlice_Control_04,0x00,SDDS_FK);
   // vIO32WriteFldAlign(CKGEN_AFEPLLCFG5, 0x3, FLD_RG_VGAPLL_SDDS_HSEL);   
    vIO32WriteFldAlign(REG_PLL_GROUP_CFG7, 0x3, FLD_RG_VGAPLL_SDDS_HSEL);   
    
#endif		 
    //disable Vmask4
     vRegWriteFldMulti(ASYNC_19, P_Fld(1, AS_VMASK4_OFF)| P_Fld(0, AS_VMASK4_INV));
#ifdef SCART_CLK_SOURCE_CVBS            
    vHdtvClampWin(6, 0x9A);
    vHdtvBlankStart(0xAA);
#endif
}


void vScartRGBChkModeChange(void)
{

    UINT8 bMode ;
    UINT16 wPorch, wHLEN;

    //check scart rgb setting
    if (_rTvd3dStatus.fgIs525)
    {
        ScartDBG("525");
#if 0//YPBPR_480IP_27MHZ
        bMode = MODE_525I;
#else
        bMode = MODE_525I_OVERSAMPLE ;
#endif
        #ifdef SCART_CLK_SOURCE_CVBS 
         wPorch = 0x92;
        #else
         wPorch= Get_VGAMODE_IPH_BP(bMode);
        #endif
        #if ASYNC_FULL_SCREEN_WA
        wHLEN = 1716;
        #endif
    }
    else
    {
        ScartDBG("625");
#if 0//YPBPR_480IP_27MHZ
        bMode = MODE_625I;
        wPorch = 0x91;
#else
        bMode = MODE_625I_OVERSAMPLE ;
        #ifdef SCART_CLK_SOURCE_CVBS 
         wPorch = 0xC8;
        #else
         wPorch = 0x125;
        #endif
#endif
        #if ASYNC_FULL_SCREEN_WA
        wHLEN = 1728;
        #endif
    }
    vDrvADCPLLSet(Get_VGAMODE_ICLK(bMode), Get_VGAMODE_IHTOTAL(bMode));
    vHdtvSetInputCapature(bMode, 1);
    #if VSYNC_OUT_UPDATE_TRIGGER 
    vRegWriteFldAlign(ASYNC_12, 0x00, AS_VS_UP_GATE);
    #endif
    
#ifdef SCART_CLK_SOURCE_CVBS
    vRegWriteFldAlign(ASYNC_08, 0x0, AS_MASK_SLICE_EN);
#endif

    #if ASYNC_FULL_SCREEN_WA
    //set fixed field detection.
    vDrvAsyncSetFieldDet(wHLEN);    
	#endif
	
    vHdtvCenSel(FALSE);
    //HDTV 2880 output (54Mhz) -> scart 2x times down-sample to 720
    vHdtvSetInput(wPorch, Get_VGAMODE_IPH_WID(bMode));
	//retime reset for phase because no ISR enable in scart 
	vDrvRETIMEReset(); 
    #if VSYNC_OUT_UPDATE_TRIGGER 
    vRegWriteFldAlign(ASYNC_12, 0x00, AS_VS_UP_GATE);
    #endif    
#ifdef SCART_RGB_MENU_WA
    vDrvCLKINFreeRun(ENABLE);
#endif		
}

#if CHANGE_SDDS_KPI
UINT8 _bClkTrigger = 0 ;
#endif

void vDrvScartRGBFreeRun(UINT8 bOnOff)
{
    ScartDBG("scart free run");

    if (bOnOff)
    {
        ScartDBG("free run on");
        vDrvCLKINFreeRun(ENABLE);
     #if CHANGE_SDDS_KPI
        vDrvClkSetLockBandwidth(SV_OFF);
        _bClkTrigger = 0 ;
        ScartDBG("\n------ SV_OFF @ vDrvScartRGBFreeRun ------\n");
     #endif
    }
    else
    {
        ScartDBG("free run off");
        vScartRGBChkModeChange();
     #if CHANGE_SDDS_KPI
        _bClkTrigger = 1 ;
        ScartDBG("\n------ _bClkTrigger = 1 @ vDrvScartRGBFreeRun ------\n");
     #endif
    }
}
#endif      //SUPPORT_SCART
UINT16 wHdtvGetPorch(UINT8 bPath,UINT8 bPorchType)
{

	UINT8 u1Cen=0;
	u1Cen = RegReadFldAlign(HDTV_00,HDTV_CEN_SEL);

    switch (bPorchType)
    {

        case 	SV_HPORCH_CURRENT:
            #if (!(YPBPR_480IP_27MHZ))
            if ((_bHdtvTiming == MODE_525I_OVERSAMPLE)||(_bHdtvTiming == MODE_625I_OVERSAMPLE))
					u1Cen <<= 1 ;
            #endif
				return (RegReadFldAlign(HDTV_01, HDTV_AV_START) >> u1Cen );        
        
        case 	SV_HPORCH_DEFAULT:
            #if (!(YPBPR_480IP_27MHZ))        
            if ((_bHdtvTiming == MODE_525I_OVERSAMPLE)||(_bHdtvTiming == MODE_625I_OVERSAMPLE))        
					u1Cen <<= 1 ;
            #endif
            return (Get_VGAMODE_IPH_BP(_bHdtvTiming)>> u1Cen);        
        
        case	SV_VPORCH_CURRENT:
            return  RegReadFldAlign(ASYNC_11, AS_NEW_VS_OUTP_S1);

        case	SV_VPORCH_DEFAULT:
            return Get_VGAMODE_IPV_STA(_bHdtvTiming);            
            
        case    SV_HPORCH_MAX:
            #if (!(YPBPR_480IP_27MHZ))        
            if ((_bHdtvTiming == MODE_525I_OVERSAMPLE)||(_bHdtvTiming == MODE_625I_OVERSAMPLE))        
            return  (((wDrvVideoGetHTotal(bPath)-wDrvVideoInputWidth(bPath)))>>1);
            else
            #endif
            return  ((wDrvVideoGetHTotal(bPath)-wDrvVideoInputWidth(bPath)));            

        case    SV_HPORCH_MIN:
            return 10;            
        default: /*MinMax*/
            return wDrvVideoPorchStd(bPath,bPorchType);
    }
}

void vHdtvSetPorch(UINT8 bPath,UINT8 bPorchType, UINT16 wValue)
{
    vVgaSetPorch(bPath,bPorchType,wValue);
}
/******************************5371 diag********************************/
void vHdtvSwReset(void)
{
    UINT8 bchannel;

    if (fgIsMainYPbPr())
    {
        bchannel=SV_VP_MAIN;
    }
    else if (fgIsPipYPbPr())
    {
        bchannel=SV_VP_PIP;
    }
    else
    {
        return;
    }

    vHdtvConnect(bchannel, SV_OFF);
    vHdtvConnect(bchannel, SV_ON);
}

void vHdtvStatus(void)
{
   
    DBG_Printf(DBG_MDET,"Hdtv mode[%d]: %d*%d(%c) %dHz %d(clk)\n",
        _bHdtvTiming,Get_VGAMODE_IPH_WID(_bHdtvTiming),
        Get_VGAMODE_IPV_LEN(_bHdtvTiming),
        Get_VGAMODE_INTERLACE(_bHdtvTiming) ?'I':'P',
        Get_VGAMODE_IVF(_bHdtvTiming),
        Get_VGAMODE_ICLK(_bHdtvTiming));
    DBG_Printf(DBG_MDET,"Hdtv FSM[%d]  DetectDone[%d]\n", bHdtvMDStateMachine,_IsHdtvDetectDone);
    DBG_Printf(DBG_MDET,"Hdtv sync[%d] lock[%d]\n", bASHDTVActiveChk(),fgIsCLKLock());
    DBG_Printf(DBG_MDET,"Hdtv hfreq[%d] vfreq[%d] hlen[%d] vlen[%d]\n",wSP0Hclk,bSP0Vclk,wSP0HLength,wSP0Vtotal);
    DBG_Printf(DBG_MDET,"Hdtv CW[%x] CW_STA[%x] Htotal[%d] \n",u4RegRead4B(VFE_15),u4RegRead4B(VFE_STA_01),(RegReadFldAlign(VFE_16, DCLK_HLINE_LENGTH)+1));
    DBG_Printf(DBG_MDET,"VGA supports timing[%d]=%d(VGA)+%d(HDTV)  1 entry=[%d]\n", bAllTimings,bVgaTimings,bHdtvTimings,sizeof(VGAMODE));
    DBG_Printf(DBG_MDET,"Hdtv Phase sum %x\n", dVGAGetAllDiffValue());
    DBG_Printf(DBG_MCHG,"[SA7]==Non_Std==fgIsNSTD=%d\n", _rHDTV_NSTDStatus.fgIsNSTD);
    DBG_Printf(DBG_MCHG,"[SA7]wVTotal=%d\n", _rHDTV_NSTDStatus.wVTotal);
    DBG_Printf(DBG_MCHG,"[SA7]wHTotal=%d\n", _rHDTV_NSTDStatus.wHTotal);
    DBG_Printf(DBG_MCHG,"[SA7]bRefreshRate=%d\n", _rHDTV_NSTDStatus.bRefreshRate);
    DBG_Printf(DBG_MCHG,"[SA7]wVTotalDiff=0x%x\n", _rHDTV_NSTDStatus.wVTotalDiff);

}
#ifdef  SUPPORT_AV_COMP_AUTO
UINT8 u1DrvGetCompStatus(void)
{
	if(RegReadFldAlign(STA_SYNC0_02, AS_SOGV_STABLE))
	{
		return SV_VDO_STABLE;
	}
	else
	{
		return SV_VDO_NOSIGNAL;
	}

}
#endif


