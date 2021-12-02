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
 * $RCSfile: drv_vga.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/
#define _DRV_VGA_C_

#ifdef CC_UP8032_ATV
#include "general_mt82.h"
#endif
#include "general.h"
#include "hw_vdoin.h"
#include "hw_async.h"
#ifdef CC_COPLAT_MT82
#include "feature.h"
#include "debug.h"
#include "sv_const.h"
#endif
#include "hw_pdwnc.h" 
#include "drv_async.h"
#include "drv_vga.h"
#include "vga_table.h"
#include "source_select.h"
#include "drv_vdoclk.h"
#include "drv_adcset.h"
#include "vga_auto.h"
#include "drv_auto.h"
#ifndef CC_COPLAT_MT82
#include "drv_scaler.h"
#endif
#include "video_def.h"
#include "vdo_misc.h"
#include "util.h"
#include "api_eep.h"
#include "eepdef.h"
#include "drv_hdtv.h"
#include "drv_autocolor.h"
#include "drv_ycproc.h"
#include "drv_display.h"

#ifdef CC_UP8032_ATV
#include "drvcust_if.h"
#endif
#include "hw_ycproc.h"
#include "x_printf.h"
#include "drv_video.h"
#include "drv_di.h"

#include "nptv_debug.h"
#define VGA_TIMEDELAY_DBG 0
#ifndef CC_COPLAT_MT82
#include "x_os.h"
#endif
#include "x_timer.h"
#include "x_lint.h"
#include "mute_if.h"

#define POLARITY_DEBUG	0

#if VGA_TIMEDELAY_DBG
static HAL_TIME_T cur_time[20];
#endif
LINT_SUPPRESS_NEXT_EXPRESSION(683)
#define memset(p,v,size) x_memset(p,v,size)

UINT8   _bVgaTiming;
#ifndef CC_UP8032_ATV
BOOL   _IsVgaDetectDone;
#else
UINT8   _IsVgaDetectDone;
#endif
UINT8   bModeIndex;
UINT8   bVGAMDStateMachine;
UINT8   bVGAMCErrorCnt;
UINT8   _bVgaHvChkCnt;
UINT8 _bHsyncWidth;
UINT8   bVgadbgmask=0;
static BOOL _fgVgaAmbiguousUserSel = SV_FALSE;

#define VGA_TIMING_CHECK_POLARITY	1
#define VGA_SEPERATE_ONLY	0


#ifdef CC_UP8032_ATV
#ifndef ABS
#define ABS(x)                  (((x) >= 0) ? (x) : -(x))
#endif
#ifdef CC_COPLAT_MT82
extern BOOL fgApiEepromWriteByte(UINT16 wAddr, UINT8 bData);
#endif

#endif

#if SUPPORT_VGA_USERMODE
UINT8   _bVgaUserMode;			//just using for copy eep to ram
UINT8   _bVgaUserMode_FIFO;
UINT8   _bVgaUserMode_Flush;
extern UINT8 bVgaUsrTimingSearch(UINT8 eepinit) ;
#endif

#if SUPPORT_VGA_AMBIGUOUS_H_DETECT
UINT8   _bBestTiming ;
UINT32   _dPhaseDiff, _dMaxDiff  ;
#endif

#define AMBIGUOUS_HF_DELTA	10
#define AMBIGUOUS_VF_DELTA	3

UINT16 _dVgaStableCnt=0;

UINT8  _bVgaChkState;
UINT8 _bVgaModeChged ;
UINT8 _bSP0MuxCnt ;
UINT16 _bSP0MuxSate ;
HAL_TIME_T _rVgaModeChgTime;
UINT8 _bVgaStableDealy =0;
static UINT8   NoSignalCnt;
extern UINT8 _bPLLlockCnt ;
extern void vVgaSetModeCHG(void);
extern void vVgaSetModeDone(void);


BOOL _RETIMENeedReset = TRUE;
#if MANUAL_MAINSUB_FIFO_RST
BOOL _bMainSubFifoReset = FALSE;
#endif

    
// extern UINT8 _bUnLockCnt; // for checking tool - QAC Warning
extern UINT8 _bLockCnt;

UINT8 _u1Vga422ModeChg=0 ;

void vVgaUsrStable(void);
void vVgaUsrBroken(void);
void vVgaAmbiguousHInit(void);
void vVgaEraseUserMode(UINT8 index);
////////////////////////////////////////////////////////////////////////////////
/**
 * @brief vVgaPolarityUniform( void )
 * According to   fgSP0Hpol & fgSP0Vpol to set suitable setting in each step
 * @param  void
 * @retval void
 * @pre  Need to call the API of vVgaNewPolarity()
 */
PRIVATE void vVgaPolarityUniform(void)
{
    fgSP0Hpol = fgASHPolarityMeasure();
    fgSP0Vpol = fgASVPolarityMeasure();

    if (fgSP0Hpol == POSITIVE)
    {
        vDrvCLKINFreeHsyncPol(0); //ADPART
        vDrvCLKINHsyncPol(0); //ADPART
        vDrvHsInv((UINT8)TRUE); // Goal:- gbsh (+:stable vlen but buggy field detect)060612
#if POLARITY_DEBUG
        LogS("H+");
#endif
    }
    else
    {
        vDrvCLKINFreeHsyncPol(1); //ADPART
        vDrvCLKINHsyncPol(1); //ADPART
        vDrvHsInv((UINT8)FALSE); // Goal:- gbsh (+:stable vlen but buggy field detect)060612
#if POLARITY_DEBUG
        LogS("H-");
#endif
    }

    if (fgSP0Vpol == POSITIVE)
    {
        vDrvVsOutInvPol((UINT8)FALSE);	//Goal: +
        vDrvVsInv((UINT8)TRUE);	// Goal:-
#if POLARITY_DEBUG
        LogS("V +");
#endif
    }
    else
    {
        vDrvVsOutInvPol((UINT8)TRUE);	//Goal: +
        vDrvVsInv((UINT8)FALSE);	// Goal:-
#if POLARITY_DEBUG
        LogS("V -");
#endif
    }
    DBG_Printf(3,"vVgaPolarityUniform %d %d\n",fgSP0Hpol,fgSP0Vpol);
    //Always invert H-lock
    vDrvHsLockInv((UINT8)TRUE);
}
#ifdef SUPPORT_VGA_UI_AMBIGUOUS_TIMING_SELECT
PRIVATE UINT8 bAmbiguousHCheck(UINT8 CandidateTiming)
{
    UINT8 AmbTimingIndex, EEPromIndex, i, j;
    UINT16 _wCurHF, _wTempHF;
    UINT8 _bCurVF, _bTempVF;

    DBG_Printf(3, "**********************\n");
    DBG_Printf(3, "CandidateTiming = %d\n", CandidateTiming);

    _wCurHF = Get_VGAMODE_IHF(CandidateTiming);
    _bCurVF = Get_VGAMODE_IVF(CandidateTiming);

    for (AmbTimingIndex = (CandidateTiming - 1); AmbTimingIndex>0; AmbTimingIndex--)
    {
        _wTempHF = Get_VGAMODE_IHF(AmbTimingIndex);
        _bTempVF = Get_VGAMODE_IVF(AmbTimingIndex);

        _wTempHF = (_wTempHF>_wCurHF)?(_wTempHF-_wCurHF):(_wCurHF-_wTempHF);
        _bTempVF = (_bTempVF>_bCurVF)?(_bTempVF-_bCurVF):(_bCurVF-_bTempVF);

        if ( (_wTempHF<=AMBIGUOUS_HF_DELTA) && (_bTempVF<=AMBIGUOUS_VF_DELTA) && (Get_VGAMODE_AmbiguousH(AmbTimingIndex)) )
        {
            continue;
        }
        else
        {
            break;
        }
    }

    EEPromIndex = 0;
    for (i = 0; i<=CandidateTiming; i++)
    {
        if (Get_VGAMODE_AmbiguousH(i))
        {
            _wCurHF = Get_VGAMODE_IHF(i);
            _bCurVF = Get_VGAMODE_IVF(i);

            for (j=i; j<=CandidateTiming; j++)
            {
                _wTempHF = Get_VGAMODE_IHF(j);
                _bTempVF = Get_VGAMODE_IVF(j);

                _wTempHF = (_wTempHF>_wCurHF)?(_wTempHF-_wCurHF):(_wCurHF-_wTempHF);
                _bTempVF = (_bTempVF>_bCurVF)?(_bTempVF-_bCurVF):(_bCurVF-_bTempVF);

                if ( (_wTempHF<=AMBIGUOUS_HF_DELTA) && (_bTempVF<=AMBIGUOUS_VF_DELTA) && (Get_VGAMODE_AmbiguousH(j)) )
                {
                    continue;
                }
                else
                {
                    break;
                }
            }

            if (j<=CandidateTiming)
            {
                EEPromIndex++;
            }

            i = j-1;
        }
    }


    DBG_Printf(3, "Load EEP Loc = %d, ", EEP_VGA_AMBIGUOUS_DEFAULT_START+EEPromIndex);

    EEPromIndex = bApiEepromReadByte(EEP_VGA_AMBIGUOUS_DEFAULT_START+EEPromIndex);

    DBG_Printf(3, "Val = %d\n", EEPromIndex);

    if (EEPromIndex != 0xFF)		//default value stored.
    {
        DBG_Printf(3, "Return Val = %d\n", (EEPromIndex+AmbTimingIndex));
        DBG_Printf(3, "**********************\n");
        return (EEPromIndex+AmbTimingIndex);
    }
    return CandidateTiming ;

}
#endif //SUPPORT_VGA_UI_AMBIGUOUS_TIMING_SELECT
#ifndef CC_VGA_VSYNC_WIDTH_AMBIGUOUS_DISABLE
PRIVATE UINT8 bAmbiguousVsyncWidthCheck(UINT8 bMode)
{
    UINT8 bVsyncWidth ;
    UINT8 bRatioH, bRatioV;
    UINT8 bModeIdx ;
    UINT16 wDiff ;

    bVsyncWidth = wASVSyncWidthMeasure();
    bModeIdx = bMode ;

    while (Get_VGAMODE_VSyncWidthChk(bMode))
    {
        switch (bVsyncWidth)
        {
            case 4://4:3 , 1024x768 1440x1050
                bRatioH = 4 ;
                bRatioV = 3 ;
                break;
            case 5://16:9 1360x768 ,
                bRatioH = 16 ;
                bRatioV = 9 ;
                break;
            case 6: //16:10 1680x1050
                bRatioH = 16;
                bRatioV = 10 ;
                break;
            case 7://15:9 1280x768
                bRatioH = 15 ;
                bRatioV = 9 ;
                break;
            default:// if not support vsyncWidth return org mode
                return bMode ;
        }
        //Printf("bRatioH %d bRatioV %d v %d\n", bRatioH, bRatioV, Get_VGAMODE_IPV_LEN(bModeIdx));
        //Printf("%d %d %d\n", bModeIdx, ((Get_VGAMODE_IPV_LEN(bModeIdx)*bRatioH)/bRatioV), Get_VGAMODE_IPH_WID(bModeIdx));
        wDiff = (Get_VGAMODE_IPV_LEN(bModeIdx)*bRatioH)/bRatioV ;
        wDiff = (wDiff > Get_VGAMODE_IPH_WID(bModeIdx))? (wDiff - Get_VGAMODE_IPH_WID(bModeIdx)):(Get_VGAMODE_IPH_WID(bModeIdx) - wDiff);
        //Printf("wDiff %d\n", wDiff);
        if (wDiff < 20)
        {
            break;
        }
        if (Get_VGAMODE_IVTOTAL(bModeIdx+1) != Get_VGAMODE_IVTOTAL(bMode))
        {
            return bMode ;
        }
        bModeIdx++ ;
    }
    // Printf(" Vsync Width (%d)check mode = %d\n", bVsyncWidth, bModeIdx);
    return bModeIdx ;
}
#endif
/**
* @brief bVgaStdTimingSearch(void)
* VGA Timing search according to bSP0Vclk & wSP0Hclk
* @param  void
* @retval Timing index.
* @pre  Need to measure H/V Frequence and frequence transform.
*/
PRIVATE UINT8 bVgaStdTimingSearch(void)
{
    UINT8	CandidateTiming, bsearch, bPolMatchTiming;
    UINT16	wMinVTotalOffset, wTempVTotalOffset, wTmpVtotal;
    UINT32 wTempHsyncWOffset, wMinHsyncWOffset;
    UINT16 wTempHFOffset, wTempVFOffset;
    UINT32 wHFVFOffsetSum, wMinHFVFOffsetSum=0xFFFF;

    if ((wSP0Hclk<50)||(bSP0Vclk<5)) /*H<5k V<5hz */
        return MODE_NOSUPPORT;

    bsearch = VGA_SEARCH_START;
    if (bVgaOpt04_SearchHDTV)
    {
        bsearch = HDTV_SEARCH_START;
    }

    wMinVTotalOffset = 0xffff;
    CandidateTiming = MODE_NOSUPPORT;
    bPolMatchTiming = 0;
    _bHsyncWidth = wASHSyncWidthMeasure() ;
    wMinHsyncWOffset = 0xffff ;

    do
    {
        if ((bSP0Vclk > (Get_VGAMODE_IVF(bsearch) - (VSYNC_LOWER_BD))) && (bSP0Vclk < (Get_VGAMODE_IVF(bsearch) + (VSYNC_UPPER_BD))))
        {

			if(Get_VGAMODE_VgaDisabled(bsearch))
			{
				continue;
			}
            if (!Get_VGAMODE_IHF(bsearch))
                continue;

            if (
                (wSP0Hclk > (Get_VGAMODE_IHF(bsearch) - HSYNC_LOWER_BD))&&
                (wSP0Hclk < (Get_VGAMODE_IHF(bsearch) + HSYNC_UPPER_BD)))
            {
#if SUPPORT_VGA_USERMODE
                /* user mode need everything exactly the same, otherwise evaluate a new one */
                if (bsearch>=bUserVgaTimingBegin)
                {
                    UINT8 usrmode_fifo=bsearch-bUserVgaTimingBegin;

                    if (!bVgaOpt05_SearchNewMode)
                        break;

                    if (fgSP0Hpol!=rVgaUsrEEP[usrmode_fifo].hpol)
                        continue;
                    if (( bModeIndex == SEPERATESYNC)&&(fgSP0Vpol!=rVgaUsrEEP[usrmode_fifo].vpol))
                        continue;
                    if ((wSP0HLength<(rVgaUsrEEP[usrmode_fifo].hlen-1))||(wSP0HLength>(rVgaUsrEEP[usrmode_fifo].hlen+1)))
                        continue;
                    if ((wSP0Vtotal<(rVgaUsrEEP[usrmode_fifo].vlen-1))||(wSP0Vtotal>(rVgaUsrEEP[usrmode_fifo].vlen+1)))
                        continue;
                    if ((_bHsyncWidth< (rVgaUsrEEP[usrmode_fifo].hsync_w-1))||(_bHsyncWidth> (rVgaUsrEEP[usrmode_fifo].hsync_w+1)))
                        continue;

                    return bsearch ;
                }
#endif



                /* V total */
                wTmpVtotal = Get_VGAMODE_IVTOTAL(bsearch) ;
                if ( Get_VGAMODE_INTERLACE(bsearch))
                {
                    wTmpVtotal = wTmpVtotal /2 ;
                }

                /* interlace mode measure Vtotal= real Vtotal/2 */
                wTempVTotalOffset = (wSP0Vtotal > wTmpVtotal)? (wSP0Vtotal - wTmpVtotal) : (wTmpVtotal - wSP0Vtotal);

                // find the closest Vtotal
                if (wMinVTotalOffset > wTempVTotalOffset)
                {
                    wMinVTotalOffset = wTempVTotalOffset;
                    CandidateTiming = bsearch;
                    
                    wMinHFVFOffsetSum = ABS(Get_VGAMODE_IHF(bsearch) - wSP0Hclk) + ABS(Get_VGAMODE_IVF(bsearch) - bSP0Vclk);                    
                }
                else if(wMinVTotalOffset == wTempVTotalOffset)  //Check (Diff(HF)+Diff(VF))
                {
                    wTempHFOffset = ABS(Get_VGAMODE_IHF(bsearch) - wSP0Hclk);
                    wTempVFOffset = ABS(Get_VGAMODE_IVF(bsearch) - bSP0Vclk);
                    wHFVFOffsetSum = wTempHFOffset + wTempVFOffset; 

                    if( wHFVFOffsetSum < wMinHFVFOffsetSum )
                    {
                        CandidateTiming = bsearch;
                        wMinHFVFOffsetSum = wHFVFOffsetSum;
                    }
                }

                if (Get_VGAMODE_PolChk(bsearch) )
                {
                    if (Get_VGAMODE_HPol(bsearch) == fgSP0Hpol)
                    {
                        if ((bModeIndex != SEPERATESYNC)||
                            (( bModeIndex == SEPERATESYNC) && (Get_VGAMODE_VPol(bsearch) == fgSP0Vpol)))
                        {
                            if (wTempVTotalOffset <= wMinVTotalOffset)	//for 576p  and 640x480x50Hz ambiguous
                            {
                                bPolMatchTiming = bsearch;

#if POLARITY_DEBUG
                                LogSB("polarity match", bsearch);
                                LogSB("h", fgSP0Hpol);
                                LogSB("v", fgSP0Vpol);
                                //lint --e{*} Don't lint this function
                                LogSB("h1", Get_VGAMODE_HPol(bsearch));
                                //lint --e{*} Don't lint this function
                                LogSB("v1", Get_VGAMODE_VPol(bsearch));
#endif
                            }
                        }
                    }
                }
                /* H-sync width */
                if (Get_VGAMODE_HSyncWidthChk(bsearch))
                {
                    UINT16 wDiv_temp;
                    wDiv_temp = Get_VGAMODE_OverSample(bsearch)+1;
                    //if ((Get_VGAMODE_ICLK(bsearch))&&(Get_VGAMODE_IPH_STA(bsearch)>Get_VGAMODE_IPH_BP(bsearch)))
					if(Get_VGAMODE_ICLK(bsearch))
                    {
                        UINT16 wtable_width;


                        // Hwid/pixclk=STA29/270 ,we want Hwid
                        wTempHsyncWOffset= (((UINT32)_bHsyncWidth<<8)*(Get_VGAMODE_ICLK(bsearch)/wDiv_temp) )/270;
                        wTempHsyncWOffset >>=8;  /* now  Hwid */

                        //wtable_width=(Get_VGAMODE_IPH_STA(bsearch)/wDiv_temp)-(Get_VGAMODE_IPH_BP(bsearch)/wDiv_temp);
                        wtable_width= Get_VGAMODE_IPH_SYNCW(bsearch)/wDiv_temp ;
                        wTempHsyncWOffset=wTempHsyncWOffset>wtable_width?
                                          wTempHsyncWOffset -wtable_width  : wtable_width-wTempHsyncWOffset;
                        // find the closest hsync-width
                        if (wMinHsyncWOffset > wTempHsyncWOffset)
                        {
                            wMinHsyncWOffset = wTempHsyncWOffset;
                            CandidateTiming = bsearch;
                            DBG_Printf(DBG_MDET, "hsync width match %d\n", CandidateTiming);
                        }
                    }
                }
            }
        }
    }
    while (++bsearch <= VGA_SEARCH_END);


    // if polarity match replace timing.
    if (bPolMatchTiming)
    {
        CandidateTiming = bPolMatchTiming;
    }
    
    //check AD spec
    if (Get_VGAMODE_ICLK(CandidateTiming) > wVGAADSpec)
    {
        return  MODE_NOSUPPORT;
    }
    else
    {
        if ((Get_VGAMODE_IHF(CandidateTiming) > wHFHeight)||
            (Get_VGAMODE_IHF(CandidateTiming) < wHFLow))
        {
            return MODE_NOSUPPORT;
        }
        else
        {
            if ((Get_VGAMODE_IVF(CandidateTiming) > wVFHeight)||
                (Get_VGAMODE_IVF(CandidateTiming) < wVFLow))
            {
               return MODE_NOSUPPORT;
            }
        }
    }

#ifdef SUPPORT_VGA_UI_AMBIGUOUS_TIMING_SELECT
    //Check if ambiguous VGA has default setting, if yes, replace CandidateTiming with value from EEPROM
    if(_fgVgaAmbiguousUserSel != SV_FALSE)
    {
        if (Get_VGAMODE_AmbiguousH(CandidateTiming))
        {
            return bAmbiguousHCheck(CandidateTiming);
        }
    }
    else 
#endif //SUPPORT_VGA_UI_AMBIGUOUS_TIMING_SELECT   
    {
#ifndef CC_VGA_VSYNC_WIDTH_AMBIGUOUS_DISABLE
//move to first ambiguous timing
        if (Get_VGAMODE_VSyncWidthChk(CandidateTiming) && (CandidateTiming < bAllTimings))
        {
            UINT8 AmbTimingIndex;

            for (AmbTimingIndex=CandidateTiming;AmbTimingIndex>0;AmbTimingIndex--)
            {
                //check ambiguous flag and Vtotal
                if (Get_VGAMODE_VSyncWidthChk(AmbTimingIndex) &&
                                    Get_VGAMODE_IVTOTAL(AmbTimingIndex) == Get_VGAMODE_IVTOTAL(CandidateTiming))
                {
                    continue;
                }
                else
                {
                    break;
                }
            }
            CandidateTiming = AmbTimingIndex+1;
        }

        if (Get_VGAMODE_VSyncWidthChk(CandidateTiming))
        {
            CandidateTiming = bAmbiguousVsyncWidthCheck(CandidateTiming) ;
        }
#endif
    } 
#if YPBPR_480IP_27MHZ
	if ((CandidateTiming== MODE_625I_OVERSAMPLE)) 
	{
		CandidateTiming= MODE_625I;
    }

	if ((CandidateTiming== MODE_525I_OVERSAMPLE))
	{
		CandidateTiming= MODE_525I;
	}
#endif
    return CandidateTiming;

}


/**
 * @brief vVgaSetIputIClk(UINT8 bMode)
 * Set PLL related clock setting according to timing mode.
 * @param  timing mode index
 * @retval void
 * @pre  Must identify timing mode is supported mode first.
 */
PRIVATE void vVgaSetIputIClk(UINT8 bMode)
{
    UINT16 wPixelClk;

    if (bMode >= MAX_TIMING_FORMAT)
    {
        return;
    }

    if (bModeIndex == SYNCONGREEN)
    {
        vDrvCLKINSyncSel(DCLK_IN_SOG);	// DCLK_hsync_sel: 0: From SOG   //ADPART
    }
    else
    {
        vDrvCLKINSyncSel(DCLK_IN_HSYNC);	// DCLK_hsync_sel: 1: From hsync   //ADPART
    }

    
    if (bModeIndex == SEPERATESYNC)//kal 20091223 VG848 GaryBar INV
    {
		vIO32WriteFldAlign(SYNCFG2, 1	, FLD_RG_SOGY_SINK_PWD);//FLD_RG_SOGY_SORS_PWD
    }
    else
    {
		vIO32WriteFldAlign(SYNCFG2, 0	, FLD_RG_SOGY_SINK_PWD);//FLD_RG_SOGY_SORS_PWD
    }

    _wVgaClock = Get_VGAMODE_IHTOTAL(bMode);
    wPixelClk = Get_VGAMODE_ICLK(bMode);
    DBG_Printf(DBG_MDET, "wHtotal %d\n ", _wVgaClock);
    DBG_Printf(DBG_MDET, "wPixelClk %d\n", wPixelClk);

    vDrvADCPLLSet(wPixelClk, _wVgaClock);  //ADPART
}

/**
 * @brief vVgaSetInputCapature(UINT8 bmode)
 * According to timing mode to set suitable Mask, windows etc.
 * @param  Timing Mode index
 * @retval void
 * @pre  Need to identify supported timing mode first.
 */
void vVgaSetInputCapature(UINT8 bmode) 	//set input start and length
{
    UINT16 wWindow;
    if (bmode >= MAX_TIMING_FORMAT)
    {
        return;
    }

    vVgaSetIputIClk(bmode);
    vHdtvSetInputCapature(bmode, 0);
//reset clamp win and blank start
    if (fgIsMainVga()||fgIsPipVga()) //only for VGA
    {

        // [SA7_Michael] 20080415 Overcome VGA-Splitter effect
        if (bModeIndex != SYNCONGREEN)
        {
            vRegWriteFldAlign(ASYNC_08, 1, AS_VMASK3_OFF);
#if defined(CC_MT5387) && !defined(CC_MT5363)
            if (BSP_GetIcVersion() <= IC_VER_5387_AA) // Before ECO
             vPGA_RESISTOR_BUFFER_COM(SEPERATESYNC);            
#endif            
        }
        else
        {
            vRegWriteFldAlign(ASYNC_08,0, AS_VMASK3_OFF);
#if defined(CC_MT5387) && !defined(CC_MT5363)            
            if (BSP_GetIcVersion() <= IC_VER_5387_AA) // Before ECO
             vPGA_RESISTOR_BUFFER_COM(SYNCONGREEN);            
#endif            
        }

        if (Get_VGAMODE_OverSample(bmode)) //if no over sampling
        {
            wWindow=(Get_VGAMODE_IPH_BP(bmode))/3;
            vHdtvClampWin(5, wWindow);
            vHdtvBlankStart(wWindow+0x10);
        }
        else
        {
            wWindow=(Get_VGAMODE_IPH_BP(bmode));
            if (wWindow> 72)
            {
                wWindow=wWindow/3;
                vHdtvClampWin(5, wWindow+5);
                vHdtvBlankStart(wWindow+0x10);
            }
            else
            {
                wWindow=wWindow>>2;
                vHdtvClampWin(0, wWindow);      // START=0, 	END= 12
                vHdtvBlankStart(wWindow+8);       //20~36
            }
        }
        //vHdtvClampWin(5, 30);
        //vHdtvBlankStart(36);
    }

    /* for hdtv timing boundary */
    /*
    if ((bmode>=MODE_720p_50)&&(bmode<=MODE_1080p_60))
    {
        vDrvAsyncHBDMask(0x40, 0x20);
    }
    */

    if ( bModeIndex == SEPERATESYNC)
    {
        vDrvAsyncVMask(5 , 7);
    }

}


/**
 * @brief vVgaInitial(void)
 * HW and SW initial setting when mode change.
 * @param  void
 * @retval void
 * @pre  Call it when mode change to reinitial HW and SW Setting.
 */
PRIVATE void vVgaInitial(UINT8 bReason)
{

    DBG_Printf(DBG_MCHG, "vVgaInitial %d\n", bReason);
    // Change H-Lock Sync to SP0 Sync //Modify for Disaplymode delay
    vIO32WriteFldAlign(SYNCFG2, 0, FLD_RG_SOGY_SORS_PWD);//power on
	vIO32WriteFldAlign(SYNCFG2, 0, FLD_RG_SOGY_SINK_PWD);//power on
    vRegWriteFldAlign(ASYNC_12, 1, AS_DISABLE_VS_OUT);

    // SW Init
    _bVgaHvChkCnt = 0;

    _bVgaTiming = MODE_WAIT;
    bModeIndex = SEPERATESYNC;

    _bPLLlockCnt = 0 ;
    _bSP0MuxCnt = 0 ;
    _wSP0StableVtotal = 0;

#if SUPPORT_VGA_USERMODE
    vVgaUsrBroken(); //roll back if user mode unstable
#endif
    bVGAMCErrorCnt = 0;
    bVGAMDStateMachine = VGAMD_GET_ACTIVESYNC; //ZERO;
	_bVgaDelayCnt = 30;

    if (fgIsMainVga())
    {
        vVDOINIrqOff((MSK_SP0_VSYNCOUT | MSK_MAIN_DET));
    }
    if (fgIsPipVga())
    {
        vVDOINIrqOff((MSK_SP0_VSYNCOUT | MSK_PIP_DET));
    }

    // HW Init
    vDrvCLKINFreeRun(ENABLE);	//CLKIN free run modify by W.C Shih to prevent the crash of CLKIN 2004/12/30  //ADPART
    vVgaReset();  //CONFIRM
    /*power up pll */
    vDrvADCPLLMinPower(POWER_ON);  //ADPART

    vASPathReset();  //CONFIRM

    vRegWriteFldMulti(HDTV_00, P_Fld(0,HDTV_FLD_SEL)|P_Fld(1,HDTV_RGB)|P_Fld(0,HDTV_CEN_SEL)); //Reset all SDTV, PROG,  422 Flag and Enable RGB


    SP0Initial();

    vVgaAutoInit();
    

	//disable c-deglitch
	vRegWriteFldAlign(ASYNC_00, 0 , AS_CSYNC_DGLITCH_SEL );	    

#ifdef FULLY_HW_AUTO_CALIBRATION
    vDrvHDTV_HW_AUTO_ONOFF(CALI_DISABLE, 0XFF);
    Printf("[VGA] -------vDrvHDTV_HW_AUTO_ONOFF----\n");    
#endif
	//reset the HLEN_ACTIVE
    vRegWriteFldAlign(ASYNC_04, 0x50, AS_HLEN2_S); //set HLEN2_S
    vRegWriteFldAlign(ASYNC_04, 0x80, AS_HLEN2_E); //set HLEN2_E


}
#if SUPPORT_VGA_USERMODE
void vVgaUserModeTblInit(void)
{
    if (bVgaOpt05_SearchNewMode&&(!_bVgaUserMode))
    {
        /* loading EEPROM to RAM +setup vga table*/
        UINT8 i;
        for (i=0;i<USERMODE_TIMING;i++)
        {
            /* 1 copy from eeprom */
            vApiEepromDmaRead(EEP_VGA_USR_START+i*sizeof(VGA_USRMODE),&rVgaUsrEEP[i],sizeof(VGA_USRMODE));
            /* 2 setup vga table */
            if (rVgaUsrEEP[i].vlen&&rVgaUsrEEP[i].hlen&&rVgaUsrEEP[i].hsync_w)
            {
                wSP0Vtotal=rVgaUsrEEP[i].vlen;
                fgSP0Vpol=rVgaUsrEEP[i].vpol;
                wSP0HLength=rVgaUsrEEP[i].hlen;
                fgSP0Hpol=rVgaUsrEEP[i].hpol;
                _bHsyncWidth=rVgaUsrEEP[i].hsync_w;
                //_bHsyncWidth_var=rVgaUsrEEP[i].hsync_wvar;
                //wSP0HLength_var=rVgaUsrEEP[i].hlen_var;
                wSP0Hclk = wSP0IHSClock(wSP0HLength);
                bSP0Vclk = bSP0IVSClock(wSP0HLength, wSP0Vtotal);
                (void)bVgaUsrTimingSearch(1); //filling
            }
            else _bVgaUserMode_FIFO++;

        }
        _bVgaUserMode_FIFO=bApiEepromReadByte(EEP_VGA_USR_MODE_INDEX);
        //fix first time eeprom = 0xff problem.
        if (_bVgaUserMode_FIFO >= USERMODE_TIMING)
        {
            _bVgaUserMode_FIFO = 0 ;
        }
        _bVgaUserMode=1;
    }

}
#endif

void vVgaAdSpecInit(void)
{
    INT32 i;
    i = DRVCUST_OptQuery(eVGAADSpec,&wVGAADSpec);
    if (i < 0)
    {
        wVGAADSpec = 163; //163MHz
    }
    
    i = DRVCUST_OptQuery(eVGAHFreqSpecHeight,&wHFHeight);
    if (i < 0)
    {
        wHFHeight = 1000; //100KHz
    }
    
    i = DRVCUST_OptQuery(eVGAHFreqSpecLow,&wHFLow);
    if (i < 0)
    {
        wHFLow = 130; //13KHz
    }
    
    i = DRVCUST_OptQuery(eVGAVFreqSpecHeight,&wVFHeight);
    if (i < 0)
    {
        wVFHeight = 86; //86Hz
    }
    
    i = DRVCUST_OptQuery(eVGAVFreqSpecLow,&wVFLow);
    if (i < 0)
    {
        wVFLow = 49; //49Hz        
    }

}


/**
 * @brief 	VGA Call-back function for source select connect or disconnect.
 * @param	bchannel - SV_VP_MAIN / SV_VP_PIP
 * @param	fgIsOn - SV_ON/SV_OFF
 * @retval	None
 */
void vVgaConnect(UINT8 bchannel, UINT8 fgIsOn)
{

    if (bchannel == SV_VP_MAIN)
    {
        _rVGAStat.bIsMain = fgIsOn;
        vVDOINIrqOff((MSK_SP0_VSYNCOUT | MSK_MAIN_DET));  //CONFIRM
    }

    if (bchannel == SV_VP_PIP)
    {
        _rVGAStat.bIsPip = fgIsOn;
        vVDOINIrqOff((MSK_SP0_VSYNCOUT | MSK_PIP_DET));  //CONFIRM
    }

    if (fgIsOn == SV_ON)
    {
        vDrvCLKINFreeRun(ENABLE);

        vVgaAdSpecInit();

        vDrvSOGEN(bVgaOpt03_SoGen);

#if SUPPORT_VGA_USERMODE
        vVgaUserModeTblInit();
#endif

        vHdtvHwInit();

        vVgaInitial(MDCHG_CON);

        _bVgaModeChged = 0 ;
        _bVgaChkState = VGA_NO_SIGNAL;
        vVgaSetModeCHG();
        _IsVgaDetectDone = TRUE;

        // restore to check the seperate vsync
        vRegWriteFldAlign(ASYNC_00, 0, AS_VSYNC_ACT_SEL);
    }
    if (fgIsOn == SV_OFF && !(fgIsMainVga() || fgIsPipVga()))
    {
		vDrvCLKINFreeRun(ENABLE);
        vDrvADCPLLMinPower(TRUE);
        _IsVgaDetectDone = TRUE;


        _bVgaTiming = NO_SIGNAL;
    }

}


/*UINT8 bVgaModeDetectDone(void)
{
	return _IsVgaDetectDone;
}*/

/**
 * @brief 	VGA call-back function for getting VGA input signal width.
 * @param	None
 * @retval	Height of input signal.
 */
UINT16 wVgaInputWidth(void)
{

#ifdef CC_SCPOS_PATTERN_GENERATOR
		if(u1GetScposPtGenEnable())
		{
			return 1920;
		}
#endif
    if ((_IsVgaDetectDone) && (_bVgaTiming != NO_SIGNAL) && (_bVgaTiming != MODE_NOSUPPORT) && (_bVgaTiming < (VGA_SEARCH_END + 1)))
    {
        return (Get_VGAMODE_IPH_WID(_bVgaTiming)>>(RegReadFldAlign(HDTV_00,HDTV_CEN_SEL)));
    }
    else
    {
        return (720);
    }
}

/**
 * @brief 	VGA call-back function for getting VGA input signal height.
 * @param	None
 * @retval	Height of input signal.
 */
UINT16 wVgaInputHeight(void)
{
#ifdef CC_SCPOS_PATTERN_GENERATOR
        if(u1GetScposPtGenEnable())
        {
            return 1080;
        }
#endif

    if ((_IsVgaDetectDone) && (_bVgaTiming != NO_SIGNAL) && (_bVgaTiming != MODE_NOSUPPORT) && (_bVgaTiming < (VGA_SEARCH_END + 1)))
    {
        return (Get_VGAMODE_IPV_LEN(_bVgaTiming));
    }
    else
    {
        return (480);
    }
}

/**
 * @brief 	VGA call-back function used by display driver, video mainloop, video ISR general check.
 * @param	None
 * @retval	Refresh rate of input signal.
 */
UINT8 bVgaRefreshRate(void)
{
#ifdef CC_SCPOS_PATTERN_GENERATOR
        if(u1GetScposPtGenEnable())
        {
            return 60;
        }
#endif

    if ((_IsVgaDetectDone) &&( _bVgaTiming != NO_SIGNAL )&& (_bVgaTiming != MODE_NOSUPPORT) &&( _bVgaTiming < (VGA_SEARCH_END + 1)))
    {
        return (Get_VGAMODE_IVF(_bVgaTiming));
    }
    else
    {
        return (0);
    }
}

/**
 * @brief 	VGA call-back function used by DI/Display/Scaler driver
 * @param	None
 * @retval	0 - progressive, 1 - interlace
 */
UINT8 bVgaInterlace(void)
{
    if ((_IsVgaDetectDone) && (_bVgaTiming != NO_SIGNAL) && (_bVgaTiming != MODE_NOSUPPORT) && (_bVgaTiming < (VGA_SEARCH_END + 1)))
    {
        return Get_VGAMODE_INTERLACE(_bVgaTiming);
    }
    else
    {
        return 0;
    }
}

#if 0
// **********************************************************
// Function :vWORD wVgaVPorch(void)
// Description :
// Parameter :
// Return    :
// **********************************************************
UINT16 wVgaVPorch(void)
{
    /*
       if((_IsVgaDetectDone ==TRUE) &&( _bVgaTiming!=NO_SIGNAL) &&\
       (_bVgaTiming != MODE_NOSUPPORT) &&( _bVgaTiming  <  (VGA_SEARCH_END+1)))
       {
       return (Get_VGAMODE_IPV_STA(_bVgaTiming));
       }
       else
       {
       return(0);
       }
     */
    return (0);
}

// **********************************************************
// Function :vWORD wVgaHPorch(void)
// Description :
// Parameter :
// Return    :
// **********************************************************
UINT16 wVgaHPorch(void)
{
    /*
       if((_IsVgaDetectDone ==TRUE) &&( _bVgaTiming!=NO_SIGNAL) &&\
       (_bVgaTiming != MODE_NOSUPPORT) &&( _bVgaTiming  <  (VGA_SEARCH_END+1)))
       {
       return (Get_VGAMODE_IPH_STA(_bVgaTiming));
       }
       else
       {
       return(0);
       }
     */
    return (0);
}
#endif

/**
 * @brief 	VGA call-back function for vVdoMainState, vVdoPipState polling update video status to UI.
 * @param	None
 * @retval	SV_VDO_UNKNOWN / SV_VDO_NOSIGNAL / SV_VDO_NOSUPPORT / SV_VDO_STABLE
 */
/* mw_if  ignore other state now for 5371*/

UINT8 bVgaSigStatus(void)
{
#ifdef CC_SCPOS_PATTERN_GENERATOR
        if(u1GetScposPtGenEnable())
        {
            return SV_VDO_STABLE ;
        }
#endif

    if (!_IsVgaDetectDone)
    {
        return (UINT8)SV_VDO_UNKNOWN;
    }
    else
    {
        switch (_bVgaTiming)
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

/**
 * @brief 	VGA ISR - Set flags for VGA related mainloop.
 *                                 Set PLL Related Clock Setting/Hold Blank checking and setting
 * @param	None
 * @retval	None
 */

void vVgaISR(void)
{
    if (fgIsMainVga() || fgIsPipVga())
    {
        if (fgIsVdoIntSp0Vsyncout())	//vsync out
        {
#if VGA_AUTO_SPEEDUP
            if (_bAutoISR && _IsVgaDetectDone)
            {

                if (fgIsAutoFlgSet(SP0_AUTO_CLOCK))
                {
                    vVgaAutoClkIsr();
                }
                else
                {
                #if NEW_PHASE_ALGORITHM
                    vVgaPhaseIsr_New();
                #else
                    vVgaPhaseIsr();
                #endif
                }
            }
#endif //VGA_AUTO_SPEEDUP
            if (fgIsCLKLock()) // 5371  //ADPART
            {
                UINT32 tmp ;

                vDrvDDSCwGate(SV_ON);
                tmp = u4RegRead4B(VFE_STA_01);
                vDrvDDSCwGate(SV_OFF);  
                _bGetCLKByte3 = (tmp >> 24) & 0xf;
                _bGetCLKByte2 = (tmp >> 16) & 0xff;
                _bGetCLKByte1 = (tmp >> 8) & 0xff;
                _bGetCLKByte0 = tmp & 0xff;
                SDDS_wait_message();
                DDDS_wait_message();
                if(_RETIMENeedReset)
                {
                    vDrvRETIMEReset();                   
                }
                #if MANUAL_MAINSUB_FIFO_RST                
                if(_bMainSubFifoReset)
                {
                 if (fgIsPipVga())
                 {
                  vDrvMainSubFifoReset(1);
                 }
                 else
                 {
                  vDrvMainSubFifoReset(0);
                 }
                 _bMainSubFifoReset = 0 ;
                }
                #endif
            }

            if (_bCLKSetFlag & 0x01)
            {
                vRegWriteFldAlign(VFE_15, (((UINT32)_bCLKByte3<<24)|((UINT32)_bCLKByte2<<16)|((UINT32)_bCLKByte1<<8)), DCLK_FREQ_CW);  
                vDrvCLKINFreeRun(ENABLE);
                vDrvCLKINFreeRun(DISABLE);
                _bCLKSetFlag = 0;
                _RETIMENeedReset = TRUE;
                #if MANUAL_MAINSUB_FIFO_RST                
                _bMainSubFifoReset = TRUE ;
                #endif
            }
            #if CHANGE_SDDS_KPI
            if(fgIsCLKLock())
            {
              Set_SDDS_KPI(0);
            }
            #endif
            
            vSetSP0Flg(SP0_VGA_AUTO_FLG | SP0_AUTOCOLOR_FLG);
        }	//(_IsVgaDetectDone != TRUE)
    }		//fgIsVdoIntSp0Vsyncout
}

#if SUPPORT_VGA_AMBIGUOUS_H_DETECT
/**
 * @brief 	Set the initial setting for Ambiguous H detectino
 * @param	None
 * @retval	None
 */
void vVgaAmbiguousHInit(void)
{
    _wVgaClock = Get_VGAMODE_IHTOTAL(_bVgaTiming);
    vDrvADCPLLSet( Get_VGAMODE_ICLK(_bVgaTiming), _wVgaClock);  //ADPART
    bSP0VCount = 0 ;
    _bVgaDelayCnt = 5 ;//30
    vSetAutoFlg(SP0_AUTO_CLOCK);
}
#endif


/**
 * @brief 	When VGA signal is not stable, check signal activity and then get hlen/vlen for timing search.
 * @param	None
 * @retval	None
 */
void vVgaModeDetect(void)
{
    static UINT16 wTmp;

    // Source select will do mode detect and INT will do ,too
    if (!(fgIsMainVga() || fgIsPipVga()))
    {
        return;
    }

    if (!_IsVgaDetectDone)
    {

        switch (bVGAMDStateMachine)
        {
            default:
                /*lint -fallthrough */
            case VGAMD_GET_ACTIVESYNC: //0:
            case VGAMD_GET_HTOTAL_N_STDTIME:
#if VGA_TIMEDELAY_DBG
                HAL_GetTime(&cur_time[2]);
                LOG(3,"[%d] %d:%d \n",2, cur_time[2].u4Seconds,cur_time[2].u4Micros);
                LOG(3,"MD2 %d %d\n",fgSP0Hpol,fgSP0Vpol);
#endif
                wSP0HLength = wASHLenMeasure();	
                bSP0Vclk = bSP0IVSClock(wSP0HLength, wSP0Vtotal);
                wSP0Hclk = wSP0IHSClock(wSP0HLength);

                DBG_Printf(DBG_MDET, "wSP0HLength %d\n", wSP0HLength);
                DBG_Printf(DBG_MDET,"wSP0Vtotal %d\n", wSP0Vtotal);
                DBG_Printf(DBG_MDET,"bSP0Vclk %d\n", bSP0Vclk);
                DBG_Printf(DBG_MDET,"wSP0Hclk %d\n", wSP0Hclk);

                vVgaPolarityUniform();

                _bVgaTiming = bVgaStdTimingSearch();

#if SUPPORT_VGA_USERMODE
                if (bVgaOpt05_SearchNewMode&&(_bVgaTiming == MODE_NOSUPPORT)&&(wSP0HLength>50)&&_bHsyncWidth)
                {
                    _bVgaTiming = bVgaUsrTimingSearch(0);
                }
#endif

#if 0
                if ((_bVgaTiming == MODE_NOSUPPORT)&&(bModeIndex == COMPOSITESYNC))
                    _bVgaTiming = MODE_NOSIGNAL;
                if ((_bVgaTiming == MODE_NOSUPPORT)&&(bModeIndex == SOG))
                    _bVgaTiming = MODE_NOSIGNAL;
#endif

                if (( _bVgaTiming != MODE_NOSUPPORT) && ( _bVgaTiming != MODE_NOSIGNAL))
                {
                    // Turn on VSync INT when Signal mode is valid
                    if (fgIsMainVga())
                    {
                        vVDOINIrqOn((MSK_SP0_VSYNCOUT | MSK_MAIN_DET));  //CONFIRM

                    }
#if PIP5382Enable
                    if (fgIsPipVga())
                    {
                        vVDOINIrqOn((MSK_SP0_VSYNCOUT | MSK_PIP_DET)); //CONFIRM

                    }
#endif

                    vVgaSetInputCapature(_bVgaTiming);


#if SUPPORT_VGA_AMBIGUOUS_H_DETECT
                    if(_fgVgaAmbiguousUserSel == SV_FALSE)
                    {
                        #if 1
                        //move to first ambiguous timing
                        if (Get_VGAMODE_AmbiguousH(_bVgaTiming) && (_bVgaTiming < bAllTimings))
                        {
                            UINT8 AmbTimingIndex;

                            for (AmbTimingIndex=_bVgaTiming;AmbTimingIndex>0;AmbTimingIndex--)
                            {
                                //check ambiguous flag and Vtotal
                                if (Get_VGAMODE_AmbiguousH(AmbTimingIndex) &&
                                    Get_VGAMODE_IVTOTAL(AmbTimingIndex) == Get_VGAMODE_IVTOTAL(_bVgaTiming))
                                {
                                    continue;
                                }
                                else
                                {
                                    break;
                                }
                            }

                        #ifndef CC_VGA_VSYNC_WIDTH_AMBIGUOUS_DISABLE
                            if(Get_VGAMODE_VSyncWidthChk(_bVgaTiming) == 0)
                        #endif
                                _bVgaTiming = AmbTimingIndex+1;

                        }
                        #endif

                        if (Get_VGAMODE_AmbiguousH(_bVgaTiming+1) &&  ((_bVgaTiming+1) < bAllTimings) &&
                            (Get_VGAMODE_IVTOTAL(_bVgaTiming) == Get_VGAMODE_IVTOTAL(_bVgaTiming+1))
    #ifndef CC_VGA_VSYNC_WIDTH_AMBIGUOUS_DISABLE
                            && (Get_VGAMODE_VSyncWidthChk(_bVgaTiming) ==0 )
    #endif
                           )
                        {
                            vVgaAmbiguousHInit();
                            bVGAMDStateMachine = VGAMD_AMBCHK; //5;
                            _dMaxDiff = 0 ;
                            //LogS("to 6")	;
                            break;
                        }
                    }
#endif
                }

                vDrvVGASetPhase_Simple(8);

                DBG_Printf(DBG_MDET,"_bVgaTiming %d\n", _bVgaTiming);

                wTmp=bVgaOpt01_MDmute0;
                bVGAMDStateMachine = VGAMD_DETECTDONE;
                break;
            case VGAMD_DETECTDONE:
#if VGA_TIMEDELAY_DBG
                HAL_GetTime(&cur_time[4]);
                LOG(3,"[%d] %d:%d \n",4, cur_time[4].u4Seconds,cur_time[4].u4Micros);
                LOG(3,"MD4 %d %d\n",fgSP0Hpol,fgSP0Vpol);
#endif

                if ((_bVgaTiming)&&(_bVgaTiming!=MODE_NOSUPPORT)&&(_bVgaTiming!=MODE_WAIT)&&wTmp)
                {
                    wTmp--;

                    _bPLLlockCnt++;

                    if (!fgIsCLKLock())
                    {
                        break;//wait for DDS stable
                    }
                    else
                    {
                        if (bVgaOpt01_MDmute1&&(wTmp>bVgaOpt01_MDmute1))
                        {
                            wTmp=bVgaOpt01_MDmute1;
                        }
                        break;
                    }
                }
                _IsVgaDetectDone = TRUE;
                _dVgaStableCnt=0;

                if (_bVgaTiming == MODE_NOSIGNAL)
                {
                    _bVgaChkState = VGA_NO_SIGNAL;
                }
                else
                {
                    _bVgaChkState = VGA_CHK_MODECHG;
                }
                if ((_bVgaTiming != MODE_NOSUPPORT)   && (_bVgaTiming != MODE_NOSIGNAL))
                {
                    //Printf("wASVtotalMeasure() %d\n", wASVtotalMeasure());
                    _wSP0StableVtotal = wASVtotalMeasure();
                    DBG_Printf(DBG_MDET,"_bPLLlockCnt %d %d\n", _bPLLlockCnt, wSP0Vtotal);
                }

                vVgaSetModeDone();

#if SUPPORT_HDTV_HARDWARE_MUTE
                /* clear SYNC0_MUTE */
                vRegWriteFldAlign(ASYNC_0D, 1, AS_MUTE_CLR);
                vRegWriteFldAlign(ASYNC_0D, 0, AS_MUTE_CLR);
#endif
                break;
#if SUPPORT_VGA_AMBIGUOUS_H_DETECT
            case VGAMD_AMBCHK: //5:
                if(_fgVgaAmbiguousUserSel == SV_FALSE)
                {
                    if (fgIsAutoFlgSet(SP0_AUTO_CLOCK))
                    {
                        if (bSP0VCount++ > 254)
                        {
                            bVGAMDStateMachine = VGAMD_DETECTDONE;
                            vClrAutoFlg(SP0_AUTO_CLOCK);
                            //Printf("H fail\n");
                        }
                    }
                    else
                    {
                        if (_dMaxDiff <= _dPhaseDiff)
                        {
                            _bBestTiming = 	_bVgaTiming ;
                            _dMaxDiff = _dPhaseDiff;
                        }
    #if 0
                        Printf("_dMaxDiff %x\n", _dMaxDiff);
                        Printf("_dPhaseDiff  %x\n", _dPhaseDiff);
                        Printf("_bBestTiming %x\n", _bBestTiming);
    #endif

                        if (Get_VGAMODE_AmbiguousH(_bVgaTiming+1) &&  ((_bVgaTiming+1) < bAllTimings) &&
                            (Get_VGAMODE_IVTOTAL(_bVgaTiming) == Get_VGAMODE_IVTOTAL(_bVgaTiming+1))
                           )
                        {
                            _bVgaTiming ++ ;
                            vVgaAmbiguousHInit();
                            //Printf("next timing %d\n", _bVgaTiming);
                        }
                        else
                        {
                            bVGAMDStateMachine = VGAMD_DETECTDONE;
                            _bVgaTiming = _bBestTiming ;
                            vVgaSetInputCapature(_bVgaTiming);
                            //Printf("done\n");
                        }
                    }
                }
                break;
#endif


        }
    }
}

extern UINT8 bSP0HVCheck(void);
/**
* @brief 	vVgaSetModeCHG(void);Flag responses mode change
* @param	None
* @retval	None
*/
void vVgaSetModeCHG(void)
{
    if (_bVgaModeChged)
    {
        return ;
    }
    _bVgaModeChged = 1 ;
    HAL_GetTime(&_rVgaModeChgTime);

    if (fgIsMainVga())
    {
        #ifdef CC_COPLAT_MT5387_TODO
        _vDrvVideoSetMute(MUTE_MODULE_MODECHG, SV_VP_MAIN, FOREVER_MUTE, FALSE);
        #endif
        vClrMainFlg(MAIN_FLG_MODE_DET_DONE);
        vSetMainFlg(MAIN_FLG_MODE_CHG);
        DBG_Printf(DBG_MDET, "Main Mode Chg #1 \n");
    }
    if (fgIsPipVga())
    {
        #ifdef CC_COPLAT_MT5387_TODO
        _vDrvVideoSetMute(MUTE_MODULE_MODECHG, SV_VP_PIP, FOREVER_MUTE, FALSE);
        #endif
        vClrPipFlg(PIP_FLG_MODE_DET_DONE);
        vSetPipFlg(PIP_FLG_MODE_CHG);
        DBG_Printf(DBG_MDET, "Pip Mode Chg #1\n");
    }

}

void vVgaSetModeDone(void)
{
    if (!_bVgaModeChged)
    {
        return ;
    }
    if (fgIsMainVga())
    {
        vSetMainFlg(MAIN_FLG_MODE_DET_DONE);
        DBG_Printf(DBG_MDET, "Main Mode Done\n");
    }
    if (fgIsPipVga())
    {
        vSetPipFlg(PIP_FLG_MODE_DET_DONE);
        DBG_Printf(DBG_MDET, "Pip Mode Donel\n");
    }
    _bVgaModeChged = 0 ;
}
#define SUPPORT_VGA_SOG_FIRST 1
#ifdef SUPPORT_VGA_SOG_FIRST
PRIVATE void vSP0SwitchMux(void)
{
    UINT8 bAct ;
    bAct = bASActiveChk();
    DBG_Printf(DBG_MDET,"vSP0SwitchMux %d %d \n", bAct, _bSP0MuxSate);
    switch (_bSP0MuxSate) //get once at bDrvAsGetActive
    {
	case 0:
            _bSP0MuxSate = 1 ;
            if (bAct &0x1)
            {
                bModeIndex = SYNCONGREEN;
                DBG_Printf(DBG_MDET,"set to SOG %d \n", bAct);
                vASSetSOGSync();
                break;
            }

        case 1:
            _bSP0MuxSate = 2 ;
            if (bAct >= 6)
            {
                bModeIndex = SEPERATESYNC;
                DBG_Printf(DBG_MDET,"set to SEPERATE %d \n", bAct);
                vASSetSSync();
                break;
            }

        case 2:
            _bSP0MuxSate = 0 ;
            if (bAct &0x4)
            {
                bModeIndex = COMPOSITESYNC;
                DBG_Printf(DBG_MDET,"set to COMPOSITE  %d\n", bAct);
                vASSetCSync();
                break;
            }
        
    }
    vResetVLen();
    _bSP0MuxCnt = 0 ;
}
#else
PRIVATE void vSP0SwitchMux(void)
{
    UINT8 bAct ;
    bAct = bASActiveChk();
    DBG_Printf(DBG_MDET,"vSP0SwitchMux %d %d \n", bAct, _bSP0MuxSate);
    switch (_bSP0MuxSate) //get once at bDrvAsGetActive
    {
        case 0:
            _bSP0MuxSate = 1 ;
            if (bAct >= 6)
            {
                bModeIndex = SEPERATESYNC;
                DBG_Printf(DBG_MDET,"set to SEPERATE %d \n", bAct);
                vASSetSSync();
                break;
            }

        case 1:
            _bSP0MuxSate = 2 ;
            if (bAct &0x4)
            {
                bModeIndex = COMPOSITESYNC;
                DBG_Printf(DBG_MDET,"set to COMPOSITE  %d\n", bAct);
                vASSetCSync();
                break;
            }
        case 2:
            _bSP0MuxSate = 0 ;
            if (bAct &0x1)
            {
                bModeIndex = SYNCONGREEN;
                DBG_Printf(DBG_MDET,"set to SOG %d \n", bAct);
                vASSetSOGSync();
                break;
            }

    }
    vResetVLen();
    _bSP0MuxCnt = 0 ;
}
#endif

#define VGA_FORCE_MODET_DONE_TIME    1 //sec
#define VGA_HLEN_STABLE_THR     2
#define VGA_VLEN_STABLE_THR     2
#define VGA_HLEN_MDCFG_THR      10
#define VGA_VLEN_MDCFG_THR      5
#define WAIT_STABLE_DELAY	1
#if 0
#define VGA_HV_KEEP_STABLE_THR       20
#define VGA_MUX_CHG_CNT 50
#else
UINT8 VGA_HV_KEEP_STABLE_THR=5; 
UINT8 VGA_MUX_CHG_CNT=13 ;
UINT8 VGA_STABLE_DEALY   =10;
#endif

void vVgaChkModeChange(void)
{
    UINT8 bCurrentSignal,  bCurHPol,bCurVPol, bModChg;
    UINT16 whtemp, wvtemp;

    if (fgIsSP0FlgSet(SP0_MCHG_BYPASS_FLG)
#ifdef CC_SCPOS_PATTERN_GENERATOR
		|| u1GetScposPtGenEnable()
#endif
			)
    {
        return;
    }

    if (fgIsMainVga() || fgIsPipVga())
    {
#ifndef CC_UP8032_ATV    
#if SUPPORT_VGA_USERMODE
        //check flush user mode command
        if (_bVgaUserMode_Flush)
        {
            if ((_bVgaUserMode_Flush-bUserVgaTimingBegin)<USERMODE_TIMING) // for Klocwork warning
            memset(&rVgaUsrEEP[_bVgaUserMode_Flush-bUserVgaTimingBegin],0,sizeof(VGA_USRMODE));

            memset(&VGATIMING_TABLE[_bVgaUserMode_Flush],0,sizeof(VGAMODE));  //W.C Shih User Mode Modify

            _bVgaUserMode_Flush=0;
        }
#endif
#endif
        bCurrentSignal = bASActiveChk();
        // check unstable than time out for mode detect done (reduce the transition when mode change)
        if (_bVgaModeChged)
        {
            HAL_TIME_T rCurTime, rDeltaTime;
            HAL_GetTime(&rCurTime);
            HAL_GetDeltaTime(&rDeltaTime, &_rVgaModeChgTime, &rCurTime);
            if (  (rDeltaTime.u4Seconds) >= VGA_FORCE_MODET_DONE_TIME) // 2 sec
            {
                DBG_Printf(DBG_MDET, "mc:  Force mode detect done \n");
                
                _bVgaTiming = MODE_NOSIGNAL;
                
                vVgaSetModeDone();
            }
        }


        if (_bVgaChkState != VGA_NO_SIGNAL)
        {
            if ((bCurrentSignal == 0)  || (bCurrentSignal == 2))
            {
                if (++NoSignalCnt >= 24)//6 // for long time to stop audio play
                {
                    DBG_Printf(DBG_MDET,"mc: from activity detected to no signal\n");
                    vVgaInitial(MCHG_NOSIG);
                    if (_bVgaTiming != MODE_NOSIGNAL)
                    {
                        vVgaSetModeCHG();
                        vVgaSetModeDone(); // for long time to stop audio play
                    }

                    _bVgaTiming = MODE_NOSIGNAL;
                    _bVgaChkState = VGA_NO_SIGNAL ;
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

        switch (_bVgaChkState)
        {
            case VGA_NO_SIGNAL :
                if ( (bCurrentSignal != 0) && (bCurrentSignal != 2) )
                {
                    DBG_Printf(DBG_MDET, "mc: from no signal to wait stable\n");
                    _bVgaChkState = VGA_WAIT_STABLE ;
                    bVGAMCErrorCnt = 0;
                    vVgaInitial(MCHG_SIGIN);
					#if WAIT_STABLE_DELAY
					_bVgaStableDealy = VGA_STABLE_DEALY ;
					#else
                    _bSP0MuxSate =  0 ;
                    vSP0SwitchMux();
					#endif
                }
                break;
            case VGA_CHK_MODECHG:
                bModChg = MCHG_NO_CHG ;
                if (fgIsValidTiming(_bVgaTiming))
                {
                    if (!fgIsCLKLock())
                    {
                        _bUnLockCnt++;
                        if ((_bUnLockCnt>10) && (!fgIsAutoFlgSet(SP0_AUTO_ALL))) // vga auto may be unlock
                        {
                            DBG_Printf(DBG_MDET,"Mchg : Unlock %d\n",	_bUnLockCnt);
                            bModChg = MCHG_UNLOCK ;
                        }
                    }
                    else
                    {
                        _bUnLockCnt=ZERO;
                    }
                    if (_dVgaStableCnt<=10000)
                    {
                        _dVgaStableCnt++;
                    }    
                    if (_dVgaStableCnt==30)
                    {
                        vVgaUsrStable();
                    }    
                    

                    bCurHPol = fgASHPolarityMeasure();
                    bCurVPol = fgASVPolarityMeasure();

                    if ((bCurHPol!=fgSP0Hpol)||(bCurVPol!=fgSP0Vpol))
                    {
                        bModChg = MCHG_POL_CHG ;
                    }
                } //if(_bVgaTiming !=MODE_NOSUPPORT)

                    // Check H/V Lose
                    if (bModeIndex == SEPERATESYNC)
                    {
                        if (bASActiveChk() < 6) //Lose H or V
                        {
                            if (++_bVgaHvChkCnt > 10)
                            {
                                LOG(6,"Mchg : Lose H/V\n");
                                bModChg = MCHG_HVSYNC_LOSE ;
                            }
                        }
                        else
                        {
                            _bVgaHvChkCnt = 0 ;
                        }

                    }

                if ( (wSP0HLength<(whtemp+VGA_HLEN_MDCFG_THR))&&(wSP0HLength>(whtemp-VGA_HLEN_MDCFG_THR))&&
                     (wSP0Vtotal<(wvtemp+VGA_VLEN_MDCFG_THR))&&(wSP0Vtotal>(wvtemp-VGA_VLEN_MDCFG_THR)))
                {
                    bVGAMCErrorCnt = 0;
                }
                else
                {
                    bVGAMCErrorCnt++;
                    if (bVGAMCErrorCnt> 3)
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
                if((RegReadFldAlign(STA_SYNC0_01, AS_SYNC0_MUTE)!=0) && (_bVgaTiming !=MODE_NOSUPPORT))
                {
                    bModChg = MCHG_HW_DET;
                }
#endif

                if(_u1Vga422ModeChg)
                {
                    _u1Vga422ModeChg = 0 ;
                    bModChg = MCHG_VGA_422;
                }
                if (bModChg)
                {
                    _bVgaChkState = VGA_WAIT_STABLE ;
                    bVGAMCErrorCnt = 0;
                    //if (fgIsValidTiming(_bVgaTiming))
                    {
                        vVgaSetModeCHG();
                    }
                    vVgaInitial(bModChg);
					#if WAIT_STABLE_DELAY
					_bVgaStableDealy = VGA_STABLE_DEALY ;
					#else
                    _bSP0MuxSate =  0 ;
                    vSP0SwitchMux();
					#endif
                }
                break;
            case VGA_WAIT_STABLE:
				#if WAIT_STABLE_DELAY
	            if(_bVgaStableDealy !=0)
	            {
                        _bVgaStableDealy--;
                        //Printf("_bVgaStableDealy %d\n", _bVgaStableDealy);
                        if(_bVgaStableDealy == 1)
                        {
            		        _bSP0MuxSate =  0 ;
            		        vSP0SwitchMux();
                        }                                
                        break;
	            }
				#endif
#if ASYNC_FULL_SCREEN_WA				
				vDrvAsyncSetFieldDet(whtemp);
#endif				
                if (((whtemp >= (wSP0HCompare1 - VGA_HLEN_STABLE_THR)) && (whtemp <= (wSP0HCompare1 + VGA_HLEN_STABLE_THR))) &&
                    ((wvtemp >= (wSP0VCompare[0] - VGA_VLEN_STABLE_THR)) && (wvtemp <= (wSP0VCompare[0] + VGA_VLEN_STABLE_THR))))
                {
                    if (bVGAMCErrorCnt++ > VGA_HV_KEEP_STABLE_THR)
                    {
                        if (bSP0HVCheck())
                        {
                            DBG_Printf(DBG_MDET, "mc:wait stable to timing search \n");
                            wSP0HLength= whtemp ;
                            wSP0Vtotal= wvtemp;
                            vVgaSetModeCHG();
                            _IsVgaDetectDone = FALSE;

                        }
                        else
                        {
                            vSP0SwitchMux();
                            bVGAMCErrorCnt = 0 ;
                        }
                    }
                }
                else
                {
                    bVGAMCErrorCnt = 0;
                }
                DBG_Printf(DBG_MDET,  "H/V Clk Chg, V:%d H:%d %d %d\n", wvtemp, whtemp, bVGAMCErrorCnt, bCurrentSignal);
                //update  for next check
                wSP0HCompare1 = whtemp;
                wSP0VCompare[0] = wvtemp;
                //if go to timing search , don't change the mux 
                if(_IsVgaDetectDone)
                {
                    if (_bSP0MuxCnt++ > VGA_MUX_CHG_CNT)
                    {
                        vSP0SwitchMux();
                    }
                }
                break;
            default:
                break;
        }

    }
}

void vVgaEraseUserMode(UINT8 index)
{
    //clean DRAM shadow
    UINT8 i;

    LOG(3,"EEP_VGA_USR_START=%d,sizeof(VGA_USRMODE)=%d\n",
        EEP_VGA_USR_START,sizeof(VGA_USRMODE));
    if (index==0xff)
    {
        _bVgaUserMode_FIFO=0;
        for (i=0;i<bUserVgaTimings;i++)
        {
            #ifndef CC_UP8032_ATV                      
            if ((bUserVgaTimingBegin+i)!=_bVgaTiming)
            {
                memset(&rVgaUsrEEP[i],0,sizeof(VGA_USRMODE));
                memset(&VGATIMING_TABLE[bUserVgaTimingBegin+i],0,sizeof(VGAMODE)); //W.C Shih User Mode Modify
            }
            else		_bVgaUserMode_Flush=_bVgaTiming; //flush when mode detect
            #endif            
        }

        for (i=(UINT8)(EEP_VGA_USR_START);i<=(UINT8)EEP_VGA_USR_END;i++)
        {
            UNUSED(fgApiEepromWriteByte(i,0));
        }
        UNUSED(fgApiEepromWriteByte(EEP_VGA_USR_MODE_INDEX, 0));
    }
    else
    {
        if (index>USERMODE_TIMING)
            return;
        #ifndef CC_UP8032_ATV              
        if ((bUserVgaTimingBegin+index)!=_bVgaTiming)
        {
            LINT_SUPPRESS_NEXT_EXPRESSION(826)
            memset(&rVgaUsrEEP[index],0,sizeof(VGA_USRMODE));
            memset(&VGATIMING_TABLE[bUserVgaTimingBegin+index],0,sizeof(VGAMODE)); //W.C Shih User Mode Modify

        }
        else		_bVgaUserMode_Flush=_bVgaTiming; //flush when mode detect
        #endif
        LOG(3,"erasing:");
        for (i=(EEP_VGA_USR_START+index*sizeof(VGA_USRMODE));
             i<(EEP_VGA_USR_START+(index+1)*sizeof(VGA_USRMODE));i++)
        {
            UNUSED(fgApiEepromWriteByte(i,0));
            LOG(3,"%d ",i);
        }
        LOG(3,"\n");
    }

}




UINT16 wVgaGetPorch(UINT8 bPath,UINT8 bPorchType)
{

    switch (bPorchType)
    {
        case 	SV_HPORCH_CURRENT:
            return RegReadFldAlign(HDTV_01, HDTV_AV_START);
        case 	SV_HPORCH_DEFAULT:
            return Get_VGAMODE_IPH_BP(_bVgaTiming);		
        case	SV_VPORCH_CURRENT:
            return  RegReadFldAlign(ASYNC_11, AS_NEW_VS_OUTP_S1);

        case	SV_VPORCH_DEFAULT:
            return Get_VGAMODE_IPV_STA(_bVgaTiming);
        case	SV_VPORCH_MIN:
            return wDrvVGAVPositionMin();
        case	SV_VPORCH_MAX:
            return wDrvVGAVPositionMax();
        case	SV_HPORCH_MIN:
            return wDrvVGAHPositionMin();
        case	SV_HPORCH_MAX:
            return wDrvVGAHPositionMax();
        default:
            return 0;
    }
}


void vVgaSetPorch(UINT8 bPath,UINT8 bPorchType, UINT16 wValue)
{
    if (bPorchType==SV_HPORCH_CURRENT)
    {  	
			UINT8 u1Cen=0 ;

			u1Cen =  RegReadFldAlign(HDTV_00,HDTV_CEN_SEL);
            #if (!(YPBPR_480IP_27MHZ))        
            if ((_bHdtvTiming == MODE_525I_OVERSAMPLE)||(_bHdtvTiming == MODE_625I_OVERSAMPLE)) 
				u1Cen <<= 1 ;
#endif
            vRegWriteFldAlign(HDTV_01,(wValue<< u1Cen),HDTV_AV_START);
    }
    else
    {
        vDrvAsyncVsyncStart(wValue);  
    }
}

/******************************5371 diag********************************/
void vVgaTriggerModeChange(void)
{        
    DBG_Printf(DBG_MDET,"[SA7] ==== vVgaTriggerModeChange ==== \n");
    DBG_Printf(DBG_MDET,"[SA7] bSUPPORT_VGA_422 = %d \n",bSUPPORT_VGA_422);
	_u1Vga422ModeChg = 1 ;
}
void vVgaSwReset(void)
{
    UINT8 bchannel;

    if (fgIsMainVga())
    {
        bchannel=SV_VP_MAIN;
    }
    else if (fgIsPipVga())
    {
        bchannel=SV_VP_PIP;
    }
    else
    {
        return;
    }

    vVgaConnect(bchannel, SV_OFF);
    vVgaConnect(bchannel, SV_ON);
}

void vVgaStatus(void)
{

    DBG_Printf(DBG_MDET,"VGA mode[%d]: %d*%d(%c) %dHz %d(clk)\n",
        _bVgaTiming,Get_VGAMODE_IPH_WID(_bVgaTiming),
        Get_VGAMODE_IPV_LEN(_bVgaTiming),
        Get_VGAMODE_INTERLACE(_bVgaTiming) ?'I':'P',
        Get_VGAMODE_IVF(_bVgaTiming),
        Get_VGAMODE_ICLK(_bVgaTiming));
    DBG_Printf(DBG_MDET,"VGA FSM[%d]  DetectDone[%d]\n", bVGAMDStateMachine,_IsVgaDetectDone);
    DBG_Printf(DBG_MDET,"VGA sync[%d] lock[%d]\n", bASActiveChk(),fgIsCLKLock());
    DBG_Printf(DBG_MDET,"VGA hfreq[%d] vfreq[%d] hlen[%d] vlen[%d]\n",wSP0Hclk,bSP0Vclk,wSP0HLength,wSP0Vtotal);
    DBG_Printf(DBG_MDET,"VGA CW[%x] CW_STA[%x] Htotal[%d] \n",(u4RegRead4B(VFE_15) & 0x0fffffff),(u4RegRead4B(VFE_STA_01) & 0x0fffffff),(RegReadFldAlign(VFE_16, DCLK_HLINE_LENGTH)+1));
    DBG_Printf(DBG_MDET,"VGA supports timing[%d]=%d(VGA)+%d(HDTV)  1 entry=[%d]\n",
        bAllTimings,bVgaTimings,bHdtvTimings,sizeof(VGAMODE));
    DBG_Printf(DBG_MDET,"VGA Phase sum %x\n", dVGAGetAllDiffValue());


    /*
    LOG(3," hsync width 0x%x\n", wASHSyncWidthMeasure());
    LOG(3," vsync width 0x%x\n", wASVSyncWidthMeasure());	
    */
    /* fix -O2 problem by  refernce 3 vga table, or need add --noremove to armlink */

#if SUPPORT_VGA_USERMODE
    {
        UINT8  i;
        DBG_Printf(DBG_MDET,"VGA user mode:\n");
        for (i=bUserVgaTimingBegin;i<bAllTimings;i++)
        {
            DBG_Printf(DBG_MDET,"[%d] HF:%d VF:%d CLK:%d [%dx%d]\n",
                i,Get_VGAMODE_IHF(i),Get_VGAMODE_IVF(i),
                Get_VGAMODE_ICLK(i),Get_VGAMODE_IPH_WID(i)
                ,Get_VGAMODE_IPV_LEN(i));                   
        }
    }
#endif

}


UINT8 bVgaCurPhase(void)
{
    return bDrvVGAGetPhase();
}

#ifdef SUPPORT_VGA_UI_AMBIGUOUS_TIMING_SELECT
void vVgaSetAmbiguousUserSel(BOOL fgUserSel)
{
    _fgVgaAmbiguousUserSel = fgUserSel;
}

UINT8 bVgaGetAmbiguousTimingNum(void)
{
    UINT8 AmbTimingIndex, num;
    UINT16 _wCurHF, _wTempHF;
    UINT8 _bCurVF, _bTempVF;

    num = 1;

    if (Get_VGAMODE_AmbiguousH(_bVgaTiming))
    {
        _wCurHF = Get_VGAMODE_IHF(_bVgaTiming);
        _bCurVF = Get_VGAMODE_IVF(_bVgaTiming);

        // Finding ambiguous item in up/down direction
        for (AmbTimingIndex = (_bVgaTiming + 1); AmbTimingIndex<bAllTimings; AmbTimingIndex++)
        {
            _wTempHF = Get_VGAMODE_IHF(AmbTimingIndex);
            _bTempVF = Get_VGAMODE_IVF(AmbTimingIndex);

            _wTempHF = (_wTempHF>_wCurHF)?(_wTempHF-_wCurHF):(_wCurHF-_wTempHF);
            _bTempVF = (_bTempVF>_bCurVF)?(_bTempVF-_bCurVF):(_bCurVF-_bTempVF);

            if ( (_wTempHF<=AMBIGUOUS_HF_DELTA) && (_bTempVF<=AMBIGUOUS_VF_DELTA) && (Get_VGAMODE_AmbiguousH(AmbTimingIndex)) )
            {
                num++;
            }
            else
            {
                break;
            }
        }

        for (AmbTimingIndex = (_bVgaTiming - 1); AmbTimingIndex>0; AmbTimingIndex--)
        {
            _wTempHF = Get_VGAMODE_IHF(AmbTimingIndex);
            _bTempVF = Get_VGAMODE_IVF(AmbTimingIndex);

            _wTempHF = (_wTempHF>_wCurHF)?(_wTempHF-_wCurHF):(_wCurHF-_wTempHF);
            _bTempVF = (_bTempVF>_bCurVF)?(_bTempVF-_bCurVF):(_bCurVF-_bTempVF);

            if ( (_wTempHF<=AMBIGUOUS_HF_DELTA) && (_bTempVF<=AMBIGUOUS_VF_DELTA) && (Get_VGAMODE_AmbiguousH(AmbTimingIndex)) )
            {
                num++;
            }
            else
            {
                break;
            }
        }
    }

    return num;
}

void vVgaQueryAmbiguousTiming(UINT8 bCandiIdx, VGAAmbTimingInfo_t *rAmbTiming, UINT8 bSize)
{
    UINT8 AmbTimingIndex, bCount;
    UINT16 _wCurHF, _wTempHF;
    UINT8 _bCurVF, _bTempVF;

    if (Get_VGAMODE_AmbiguousH(_bVgaTiming))
    {
        _wCurHF = Get_VGAMODE_IHF(_bVgaTiming);
        _bCurVF = Get_VGAMODE_IVF(_bVgaTiming);

        // Let AmbTimingIndex = (START position of ambiguous timing group) - 1
        for (AmbTimingIndex = (_bVgaTiming - 1); AmbTimingIndex>0; AmbTimingIndex--)
        {
            _wTempHF = Get_VGAMODE_IHF(AmbTimingIndex);
            _bTempVF = Get_VGAMODE_IVF(AmbTimingIndex);

            _wTempHF = (_wTempHF>_wCurHF)?(_wTempHF-_wCurHF):(_wCurHF-_wTempHF);
            _bTempVF = (_bTempVF>_bCurVF)?(_bTempVF-_bCurVF):(_bCurVF-_bTempVF);

            if ( (_wTempHF<=AMBIGUOUS_HF_DELTA) && (_bTempVF<=AMBIGUOUS_VF_DELTA) && (Get_VGAMODE_AmbiguousH(AmbTimingIndex)) )
            {
                continue;
            }
            else
            {
                break;
            }
        }

        if(bCandiIdx == QUERY_CUR_AMBIGUOUS_TIMING) /* current ambiguous timing */
        {
            rAmbTiming->bCandiIdx = (_bVgaTiming - AmbTimingIndex);
            AmbTimingIndex = _bVgaTiming;
        }
        else
        {
            AmbTimingIndex += bCandiIdx;
        }

        for(bCount=0;bCount<bSize;bCount++, rAmbTiming++, AmbTimingIndex++)
        {
            UINT16 wDiv_temp;
            if (Get_VGAMODE_AmbiguousH(AmbTimingIndex))     //Double check Ambiguous
            {
                wDiv_temp = Get_VGAMODE_OverSample(AmbTimingIndex)+1;		//if oversampling, divided by 2.

                rAmbTiming->bVer_freq = Get_VGAMODE_IVF(AmbTimingIndex); //Cast is not coincided.
                rAmbTiming->wHor_res = Get_VGAMODE_IPH_WID(AmbTimingIndex)/wDiv_temp; //Cast is not coincided.
                rAmbTiming->wVer_res = Get_VGAMODE_IPV_LEN(AmbTimingIndex); //Cast is not coincided.
            }
            else
            {
                rAmbTiming->bVer_freq = 0;
                rAmbTiming->wHor_res = 0;
                rAmbTiming->wVer_res = 0;
                
                break; /* non ambiguous, break from the for-loop */
            }
        }
    }
    else
    {
        rAmbTiming->bVer_freq = 0;
        rAmbTiming->wHor_res = 0;
        rAmbTiming->wVer_res = 0;
    }
}

void vVgaSetAmbiguousTiming(UINT8 bCandiIdx, BOOL fgSetAsDefault)
{
    UINT8 AmbTimingIndex, EEPromIndex, i, j;
    UINT16 _wCurHF, _wTempHF;
    UINT8 _bCurVF, _bTempVF;

    DBG_Printf(3, "---------------------\n");
    DBG_Printf(3, "_bVgaTiming = %d\n", _bVgaTiming);

    if (!(fgIsValidTiming(_bVgaTiming)))
    {
     return;
    }

    if (fgIsMainVga())
    {
        _vDrvVideoSetMute(MUTE_MODULE_MODECHG, SV_VP_MAIN, FOREVER_MUTE, FALSE);
    }
    if (fgIsPipVga())
    {
        _vDrvVideoSetMute(MUTE_MODULE_MODECHG, SV_VP_PIP, FOREVER_MUTE, FALSE);
    }

    if (Get_VGAMODE_AmbiguousH(_bVgaTiming))
    {
        _wCurHF = Get_VGAMODE_IHF(_bVgaTiming);
        _bCurVF = Get_VGAMODE_IVF(_bVgaTiming);

        // Let AmbTimingIndex = (START position of ambiguous timing group) - 1
        for (AmbTimingIndex = (_bVgaTiming - 1); AmbTimingIndex>0; AmbTimingIndex--)
        {
            _wTempHF = Get_VGAMODE_IHF(AmbTimingIndex);
            _bTempVF = Get_VGAMODE_IVF(AmbTimingIndex);

            _wTempHF = (_wTempHF>_wCurHF)?(_wTempHF-_wCurHF):(_wCurHF-_wTempHF);
            _bTempVF = (_bTempVF>_bCurVF)?(_bTempVF-_bCurVF):(_bCurVF-_bTempVF);

            if ( (_wTempHF<=AMBIGUOUS_HF_DELTA) && (_bTempVF<=AMBIGUOUS_VF_DELTA) && (Get_VGAMODE_AmbiguousH(AmbTimingIndex)) )
            {
                continue;
            }
            else
            {
                break;
            }
        }

        AmbTimingIndex = AmbTimingIndex+bCandiIdx;

        if(fgSetAsDefault)
        {
            EEPromIndex = 0;
            for (i = 0; i<=AmbTimingIndex; i++)
            {
                if (Get_VGAMODE_AmbiguousH(i))
                {
                    _wCurHF = Get_VGAMODE_IHF(i);
                    _bCurVF = Get_VGAMODE_IVF(i);

                    for (j=i; j<=AmbTimingIndex; j++)
                    {
                        _wTempHF = Get_VGAMODE_IHF(j);
                        _bTempVF = Get_VGAMODE_IVF(j);

                        _wTempHF = (_wTempHF>_wCurHF)?(_wTempHF-_wCurHF):(_wCurHF-_wTempHF);
                        _bTempVF = (_bTempVF>_bCurVF)?(_bTempVF-_bCurVF):(_bCurVF-_bTempVF);

                        if ( (_wTempHF<=AMBIGUOUS_HF_DELTA) && (_bTempVF<=AMBIGUOUS_VF_DELTA) && (Get_VGAMODE_AmbiguousH(j)) )
                        {
                            continue;
                        }
                        else
                        {
                            break;
                        }
                    }

                    if (j<=AmbTimingIndex)
                    {
                        EEPromIndex++;
                    }

                    i = j-1;
                }
            }

            //Save EEPROM here

            DBG_Printf(3, " Save EEP Loc = %d, Val = %d\n", EEP_VGA_AMBIGUOUS_DEFAULT_START+EEPromIndex, bCandiIdx);
            DBG_Printf(3, "---------------------\n");
            fgApiEepromWriteByte( EEP_VGA_AMBIGUOUS_DEFAULT_START+EEPromIndex, bCandiIdx);

        }

        vVgaInitial(8);
    }
}

BOOL vVgaResetAmbiguousTimingEEPROM(void)
{
	UINT8 i;
	BOOL EEPFlg = TRUE;

	//Note that need to sync with default value of vApiHwInit() in api_hwinit.c   
	//Magic string reset.
	for(i=EEP_VGA_AMBIGUOUS_DEFAULT_START;i<=EEP_VGA_AMBIGUOUS_DEFAULT_END;i++)
	{
		EEPFlg = fgApiEepromWriteByte(i, 1);

		if(EEPFlg == FALSE)
			break;
	}

	return EEPFlg;
}
#endif //SUPPORT_VGA_UI_AMBIGUOUS_TIMING_SELECT

