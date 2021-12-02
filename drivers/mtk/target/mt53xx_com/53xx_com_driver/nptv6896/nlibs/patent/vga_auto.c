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
 * $Author: junmin.wu $
 * $Date: 2012/10/29 $
 * $RCSfile: vga_auto.c,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/
#define _VGA_AUTO_

#ifndef CC_MTK_LOADER


/******************************************************************************
 * Header Files
 *****************************************************************************/
#ifdef CC_UP8032_ATV
#include "general_mt82.h"
#endif
#include "general.h"
#include "hw_vdoin.h"

#include "drv_adcset.h"
#include "drv_dvi.h"
#include "drv_vga.h"
#include "drv_hdtv.h"
#include "vga_table.h"
#include "vga_auto.h"
#include "drv_async.h"
#include "drv_dsync.h"
#include "drv_vdoclk.h"
#include "drv_auto.h"
#include "video_def.h"
#include "nptv_drvif.h"
#include "api_eep.h"
#include "vdo_misc.h"
#include "x_printf.h"
#include "drv_video.h"
#include "api_notify.h"
#ifdef CC_COPLAT_MT82
#include "video_timing.h"
#endif
#ifndef CC_COPLAT_MT82
#include "drv_scaler.h"
#include "srm_drvif.h"
#endif



/******************************************************************************
 * Macro, Type Definition, and Variable Declaratoin
 *****************************************************************************/

UINT8   _bAutoFlag;
UINT8   _bVdoSP0AutoState;
//UINT32	_dSumTmp[3];
UINT16   _wVgaHStart;
UINT16   _wVgaHEnd;
UINT16   _wVgaVStart;
UINT16   _wVgaVEnd;
UINT8   _fgIsAuto0PosFlg;

UINT8	_bCurPhase, _bBestPhase;
UINT8	_bClkCnt;

UINT16   _wVgaClock;
UINT8   _bVgaDelayCnt;
UINT16   _wCurClk, _wEndClk ;

UINT16	_wBestClk;
UINT32	_dBestSum;
UINT16  _wOrgClock;
UINT8  _bOrgPhase;
UINT8   _bAutoClockFail=0 ;


UINT8   _bBdThrsh=BDTHRSH;

#if VGA_AUTO_SPEEDUP
UINT8  _bAutoISR ;
#endif
#if VGA_HW_AUTO
UINT8 _bWaitHwRdy ;
UINT8 _bHwAuto =1 ;
#endif


#if SUPPORT_VGA_AMBIGUOUS_H_DETECT
extern UINT32 _dPhaseDiff;
#endif

#define BD_RETRY 80
#define VGA_TIMEDELAY_DBG 0


#if VGA_TIMEDELAY_DBG
#include "x_timer.h"
static HAL_TIME_T cur_time[8];
#endif


//UINT8 _bdynamic_autothr=0;
UINT32 _u4VPorchChanged = 0;
#if NOTIFY_PROGRESS
UINT8  _bProgressBar = 0 ;
#endif


#if 0
extern UINT16 _wVgaVPorchOffsetS;
extern UINT16 _wVgaVPorchOffsetE;
#endif
/******************************************************************************
 * Function Forward Declaration
 *****************************************************************************/
/**
 * @brief bVGAAutoGetBoundary( void )
 * Wait Input Vsync to Read R/G/B Channel Bounadry of SP0
 * @param  void
 * @retval AUTO_CONTINUED Continue to get Bounadary
 * @retval AUTO_FINISHED Get Bounadary Finished
 * @pre  Need to call vVgaAutoPosInit() to Initialize setting for Auto Boundary
 * @example if (bVGAAutoGetBoundary() == AUTO_FINISHED)
 */
PRIVATE void vVGAAutoGetBoundary(void)
{

#if VGA_AUTO_POS_DBG
	LogS("Auto Pos debug <==");
#endif

	_wVgaHStart = wDrvVGAGetLeftBound();
	_wVgaVStart = wDrvVGAGetTopBound();
	_wVgaHEnd = wDrvVGAGetRightBound();
	_wVgaVEnd= wDrvVGAGetBottomBound();
#if VGA_AUTO_POS_DBG
	LogSW("LeftBound", _wVgaHStart);
	LogSW("TopBound", _wVgaVStart);
	LogSW("RightBound", _wVgaHEnd);
	LogSW("BottomBound", _wVgaVEnd);
#endif
}

/**
 * @brief  vVgaAutoPosInit(void)
 * VGA auto position initialize function
 * @param  void
 * @retval void
 * @example vVgaAutoPosInit()
 */
void vVgaAutoPosInit(void)
{
	vSetSp0Auto(SP0_H);
	vSetSp0Auto(SP0_V);
  vSetAsyncMeasureBD(DOMAIN_PIXEL);
}

/**
 * @brief  vVgaAutoInit(void)
 * VGA auto initialize function
 * @param  void
 * @retval void
 * @example vVgaAutoInit()
 */
void vVgaAutoInit(void)
{
	//Set phase , phase mode, and boundary threshold
	vVGAPhaseModeSet(Bmode); //sc_yang for phase mode init
	vDrvVGASetBDDataTh(64);

    if (fgApiVideoIsEvtFlgSet(VDO_FLG_VGAAUTO_DOING))
	{
		UINT8 bPath;
		if(fgIsMainVga() || fgIsPipVga())
		{
			bPath = (fgIsMainVga())?0:1 ;
		}
		else
		{
			bPath = (fgIsMainYPbPr())?0:1 ;
		}

		//Printf("reset auto %d\n", bPath);
		vApiNotifyAutoDone(bPath, FALSE);
		#if NOTIFY_PROGRESS
		vApiNotifyVGAAutoProgress(bPath, 100);
		#endif
	}
	vClrAutoFlg(SP0_AUTO_ALL);

	vSetSP0AutoState(VDO_AUTO_NOT_BEGIN);
	vApiVideoClrEvtFlg(VDO_FLG_VGAAUTO_DOING);
	//_bVgaDelayCnt =  0 ;
#if VGA_AUTO_SPEEDUP
	_bAutoISR = 0 ;
#endif
#if VGA_HW_AUTO
	vVgaHwAutoClkEnable(SV_OFF);
	vVgaHwAutoPhaseEnable(SV_OFF);
#endif
	//boundary detection for defautl all channel
	vDrvVGASetBDCha(ALL_BD_SEL);
}

/**
 * @brief vVgaAutoPosSet(void)
 * Check Auto position result and store setting
 * @param  void
 * @retval void
 * @pre  enter  VDO_AUTO_POSITION_1_START state of SP0 for boundary measurement
 * @example vVgaAutoPosSet();
 */
PRIVATE void vVgaAutoPosSet(void)
{
	UINT16	wHWidth, wHeight;
	UINT16 wTmp ;
	UINT16 MaxVStart;
	UINT8 bHcenter, bVcenter;
    UINT16 offset;
	UINT16 MaxHStart;

	bHcenter = 1 ;
	bVcenter = 1;

	//for black pattern check
	if((_wVgaHStart == 0x1fff) && (_wVgaHEnd == 0))
	{
		vClrSp0Auto(SP0_H);
		LogS("H fail");

	}
	if((_wVgaVStart == 0xfff) && (_wVgaVEnd == 0))
	{
		vClrSp0Auto(SP0_V);
		LogS("V fail");
	}
#if 0
	if(_bAutoClockFail && bVgaOpt07_AutoKeepOldVal)
	{
		vClrSp0Auto(SP0_H);
		vClrSp0Auto(SP0_V);
		LogS("Auto Pos skip");
	}
#endif
	wHWidth = (_wVgaHEnd - _wVgaHStart) + 2;
#if VGA_AUTO_DBG_MSG
	LogSW("wHWidth", wHWidth);
	LogSW("tbl width", Get_VGAMODE_IPH_WID(_bVgaTiming));
	LogSW("_wVgaHStart", _wVgaHStart);
	LogSW("_wVgaHEnd", _wVgaHEnd);
	LogSW("tbl bporch", Get_VGAMODE_IPH_BP(_bVgaTiming));
#endif

	if((wHWidth > Get_VGAMODE_IPH_WID(_bVgaTiming) + 20) || (wHWidth < Get_VGAMODE_IPH_WID(_bVgaTiming) - 20))
       {
        	//check for the gray ramp case
        	wTmp =  _wVgaHEnd - Get_VGAMODE_IPH_WID(_bVgaTiming) + 2;
        	if((_wVgaHStart < (Get_VGAMODE_IPH_BP(_bVgaTiming)+20)) && (_wVgaHStart > (Get_VGAMODE_IPH_BP(_bVgaTiming) -20)) )
        	{
        	    bHcenter = 0 ;
#if VGA_AUTO_DBG_MSG
        		//ramp case left side ok
        		LogS("Left ok");
#endif
        	}
        	else if( (wTmp < (Get_VGAMODE_IPH_BP(_bVgaTiming)+20)) &&(wTmp > (Get_VGAMODE_IPH_BP(_bVgaTiming) -20)) )
        	{
        		//ramp case right  side ok
        	    bHcenter = 0 ;
#if VGA_AUTO_DBG_MSG
        		LogS("Right ok");
#endif
        		_wVgaHStart = wTmp ;
        	}
        }
	wHeight = (_wVgaVEnd - _wVgaVStart) + 2;			// +2 for 8202B version , +1 for 8202A version
#if VGA_AUTO_DBG_MSG
	LogSW("wHeight", wHeight);
	LogSW("tbl height", Get_VGAMODE_IPV_LEN(_bVgaTiming));
#endif

	wTmp = Get_VGAMODE_IPV_LEN(_bVgaTiming);
	if (Get_VGAMODE_INTERLACE(_bVgaTiming) == INTERLACE)
	{
		wTmp >>= 1 ;
	}
	if((wHeight > (wTmp + 20)) || (wHeight < (wTmp - 20)))
   	{
		//check for the gray ramp case
		wTmp =  _wVgaVEnd - wTmp + 2;
		if((_wVgaVStart < (Get_VGAMODE_IPV_STA(_bVgaTiming)+20)) && (_wVgaVStart > (Get_VGAMODE_IPV_STA(_bVgaTiming) -20)) )
		{
#if VGA_AUTO_DBG_MSG
			//ramp case left side ok
			LogS("top ok");
#endif
                    bVcenter = 0 ;
		}
		else if( (wTmp < (Get_VGAMODE_IPV_STA(_bVgaTiming)+20)) &&(wTmp > (Get_VGAMODE_IPV_STA(_bVgaTiming) -20)) )
		{
			//ramp case right  side ok
#if VGA_AUTO_DBG_MSG
			LogS("bt ok");
#endif
			_wVgaVStart = wTmp ;
                    bVcenter = 0 ;
		}
    }
	//_wVgaHStart = _wVgaHStart - VGA_H_OFST;
#ifdef MT5360B_WA4
    if ( (wVgaInputWidth() != 1920) && (SRM_GetTvMode() == SRM_TV_MODE_TYPE_NORMAL ||
             SRM_GetTvMode() == SRM_TV_MODE_TYPE_PIP) &&  fgIsMainVga() )
    {
            _wVgaHStart = _wVgaHStart + VGA_H_OFST - MT5360B_WA4_DLY1;
    }
    else
    {
            _wVgaHStart = _wVgaHStart + VGA_H_OFST ;
    }
#else
	_wVgaHStart = _wVgaHStart + VGA_H_OFST ;
#endif

	//Modify for Disaplymode delay
	if (_wVgaVStart >= (VGA_V_OFST+Displaymode_delay))
	{
		_wVgaVStart = _wVgaVStart - (VGA_V_OFST+Displaymode_delay);
	}

	//------------------------------------------------------------
#if VGA_AUTO_UI_DBG
	LogSW("autoH=>", _wVgaHStart);
	LogSW("autoV=>", _wVgaVStart);
#endif

	if (IsSp0SetAuto(SP0_H))
	{
#if VGA_AUTO_DBG_MSG
		LogS("H auto OK");
#endif

		if(bHcenter)
		{
			if (wHWidth < (Get_VGAMODE_IPH_WID(_bVgaTiming)-1))
			{
				offset=(((Get_VGAMODE_IPH_WID(_bVgaTiming) - wHWidth) + 1)/2);

				if (_wVgaHStart>offset)
				{
					_wVgaHStart -=	offset;
				}
				else
				{
					_wVgaHStart = 0;
				}

				//LogSW("new h start", _wVgaHStart);
			}
			else if (wHWidth > (Get_VGAMODE_IPH_WID(_bVgaTiming)+1))
			{
				offset=(((wHWidth- Get_VGAMODE_IPH_WID(_bVgaTiming))  + 1)/2) ;

				if ((_wVgaHStart + offset)>(Get_VGAMODE_IHTOTAL(_bVgaTiming)-10)) //see vDrvSetHPosition
				{
					_wVgaHStart= (Get_VGAMODE_IHTOTAL(_bVgaTiming)-10);
				}
				else
				{
					_wVgaHStart += offset;
				}
			}
		}
	}
	else
	{
		_wVgaHStart = Get_VGAMODE_IPH_BP(_bVgaTiming);
		vVgaSetInput(_wVgaHStart, Get_VGAMODE_IPH_WID(_bVgaTiming));
	}

	MaxHStart = wDrvVGAHPositionMax();

	if (_wVgaHStart>MaxHStart)
	{
		_wVgaHStart = MaxHStart ;
	}

	//keepOldVal == 1 , auto fail , keep old value.
	if((IsSp0SetAuto(SP0_H)) || (bVgaOpt07_AutoKeepOldVal==0))
	{
		vVgaSetInput(_wVgaHStart, Get_VGAMODE_IPH_WID(_bVgaTiming));
	}

	if (IsSp0SetAuto(SP0_V))
	{
#if VGA_AUTO_DBG_MSG
		LogS("V auto OK");
#endif
		wTmp = Get_VGAMODE_IPV_LEN(_bVgaTiming);
		if (Get_VGAMODE_INTERLACE(_bVgaTiming) == INTERLACE)
		{
			wTmp >>= 1 ;
		}
		if(bVcenter)
		{
			if (wHeight <(wTmp -1))
			{
				UINT16 offset;
				offset=(((wTmp - wHeight) + 1)/2);

				if (offset>=_wVgaVStart)
				{
					_wVgaVStart=3;
				}
				else
				{
					_wVgaVStart -= offset;
				}

				//LogSW("v s", _wVgaVStart);
			}
			else if (wHeight > (wTmp +1))
			{
				_wVgaVStart +=	(((wHeight - wTmp) + 1)/2) ;

				if (_wVgaVStart > (wSP0Vtotal>>1)) //see. wDrvVGAVPositionMax
				{
					_wVgaVStart =  (wSP0Vtotal>>1);
				}
			}
		}

		_u4VPorchChanged = 1;	// trigger flag for SCPOS to check she needs to reset Dispmode FreeRun

	}
	else
	{
		_wVgaVStart = Get_VGAMODE_IPV_STA(_bVgaTiming);
		vDrvAsyncVsyncStart(_wVgaVStart);
	}

	MaxVStart = wDrvVGAVPositionMax();
	if (_wVgaVStart>MaxVStart)
	{
		_wVgaVStart = MaxVStart ;
	}

	//keepOldVal == 1 , auto fail , keep old value.
	if((IsSp0SetAuto(SP0_V)) || (bVgaOpt07_AutoKeepOldVal==0))
	{
		vDrvAsyncVsyncStart(_wVgaVStart);
	}


	//----(End of Restoring)
#if VGA_AUTO_UI_DBG
	LogSW("H Pos eeprom=>", _wVgaHStart);
	LogSW("V Pos eeprom=>", _wVgaVStart);
#endif
}

#if VGA_AUTO_SPEEDUP
void vVgaAutoClkIsr(void)
{
	UINT32 dTmp ;

	//this state	to get the max phase diff of 0, 4, 8 .. 28 phase sum

	if (_bCurPhase == 0) 							 // phase counter
	{
		_dSumTmp[0] = dVGAGetAllDiffValue();
		_dSumTmp[1] = _dSumTmp[0];
	}

	if (_bCurPhase <= 31)
	{
		dTmp = dVGAGetAllDiffValue();

		if (_dSumTmp[0] < dTmp)
		{
			_dSumTmp[0] = dTmp;					  // max diff value
		}

		if (_dSumTmp[1] > dTmp)
		{
			_dSumTmp[1] = dTmp;					  // min diff value
		}

		_dSumTmp[2] = (_dSumTmp[0] - _dSumTmp[1]);	  // max/min difference

		_bCurPhase = _bCurPhase + 4;
		//vDrvVGASetPhase(_bCurPhase);
		vDrvVGASetPhase_Simple(_bCurPhase); //Do auto replace current setting
		_bVgaDelayCnt = 1;
	}
	else
	{
		vSetSP0AutoState(VDO_AUTO_CLOCK_1_START);
		_bAutoISR = 0 ;
	}

}

#endif //VGA_AUTO_SPEEDUP

#if 0
/* vVgaAutoDynThr() maybe still need fine-tune ,20060902,gbsh */
void vDrvVGASetPsneThr(UINT8 bThr1, UINT8 bThr2) ;
PRIVATE vVgaAutoDynThr(void)
{
	UINT16 wRGBmaxsum;
	UINT8 peak_limit,pnse_limit;
	/* update limit */
	/* (peak)20~50, (AD max) 50~e0) */
	wRGBmaxsum=RegReadFldAlign(STA_SYNC0_00, RMAX)+ RegReadFldAlign(STA_SYNC0_00, GMAX) + RegReadFldAlign(STA_SYNC0_00, BMAX);
	peak_limit=((wRGBmaxsum/3)-0x50)/3+0x20;

	if (peak_limit<0x20)
	{
		peak_limit=0x20;
	}
	else if (peak_limit>0x50)
	{
		peak_limit=0x50;
	}

	vDrvVGASetTopThr(peak_limit);

	/* (peak)20~70, (AD max) 50~e0) */
	pnse_limit=((wRGBmaxsum/3)-0x70)/3+0x20;

	if (pnse_limit<0x20)
	{
		pnse_limit=0x20;
	}
	else if (pnse_limit>0x70)
	{
		pnse_limit=0x70;
	}

	vDrvVGASetPsneThr(pnse_limit, PSNE_THRE2);

	if (peak_limit<=0x30)
	{
		vDrvVGASetPhsMix(PSNE2_ADD);
	}
	else
	{
		vDrvVGASetPhsMix(PSNE1_ADD);
	}

}
#endif
/**
 * @brief vVdoSP0AutoState( void )
 * Sync Processor 0 Auto state machine
 * @param  void
 * @retval void
 * @example vVdoSP0AutoState()
 */
void vVdoSP0AutoState(void)
{
#if !VGA_AUTO_SPEEDUP
	UINT32 dTmp;
#endif
	if (!fgIsSP0FlgSet(SP0_VGA_AUTO_FLG))
	{
		return;
	}

	vClrSP0Flg(SP0_VGA_AUTO_FLG);

	if (_bVgaDelayCnt != 0)
	{
		_bVgaDelayCnt--;
		return;
	}

	switch (bGetSP0AutoState())
	{

		case VDO_AUTO_NOT_BEGIN:
			if (fgIsAutoFlgSet(SP0_AUTO_ALL))	//Flag to decide do auto or not
			{
				//if ((fgIsMainVga() || fgIsPipVga()) /*&& (_IsVgaDetectDone == TRUE)*/  && (fgIsCLKLock()))
				if ((fgIsMainVga() || fgIsPipVga()) && (fgIsCLKLock()))
				{
					if (fgIsAutoFlgSet(SP0_AUTO_CLOCK))
					{
						_wOrgClock = _wVgaClock ;
						_bOrgPhase = u1DrvVGAGetPhase();
						//set clock to default and do the auto boundary for auto clock first
                                         if (_IsVgaDetectDone)
                                         {
                                             vDrvVGASetClock(Get_VGAMODE_IHTOTAL(_bVgaTiming));
                                         }
					      vSetSP0AutoState(VDO_AUTO_CLOCK_START);
						//for vDrvVGASetClock , remember to delay 1 vsync for ISR clock setting.
						_bVgaDelayCnt = 2 ;
					}
				}
			}

			break;

		case VDO_AUTO_CLOCK_START:
			if ((fgIsMainVga() || fgIsPipVga())
			                                        //&& (_IsVgaDetectDone == TRUE)
			                                        && (fgIsCLKLock()))
			{
				UINT16	wTableWidth, wMeasWidth;

                      #if THERMAL_COMPENSATION_ENABLE
				//vThermalCompInit();
                      #endif

                      #if VGA_TIMEDELAY_DBG
				HAL_GetTime(&cur_time[1]);
                      #endif
				vVGAAutoGetBoundary();
				wTableWidth = Get_VGAMODE_IPH_WID(_bVgaTiming);
				wMeasWidth = (_wVgaHEnd - _wVgaHStart) + 1;
                      #if VGA_AUTO_CLK_DBG
				LogSW("auto clk tbl width 0", wTableWidth);
				LogSW("auto clk width 1", wMeasWidth);
                      #endif


                      #if SUPPORT_VGA_AMBIGUOUS_H_DETECT

				if (_IsVgaDetectDone)
                      #endif
				{
					UINT16 wClockDiff;
					//wClockDiff = 127;
					wClockDiff = wTableWidth/3;
					_wCurClk = Get_VGAMODE_IHTOTAL(_bVgaTiming);
					_bAutoClockFail = 0 ;

					if ((wMeasWidth > (wTableWidth - wClockDiff)) && (wMeasWidth < (wTableWidth + wClockDiff)))
					{
						UINT16	wTmp;

						wTmp = (((UINT32) wTableWidth) * _wVgaClock) / wMeasWidth;
                                   #if VGA_AUTO_CLK_DBG
						LogSW("_wVgaClock", _wVgaClock);
						LogSW("tbl clk", Get_VGAMODE_IHTOTAL(_bVgaTiming));
						LogSW("Cal clk", wTmp);
                                   #endif
						_wEndClk = (wTmp & (~0x1)) + 4;						// even clock, clk-4, clk-2, clk, clk+2, clk+4
						_wBestClk = _wCurClk;					// set the best clock to  table default
						if(  (wSP0Hclk*wTmp/1000)> wVGAADSpec)
						{

						_wEndClk=(_wCurClk & (~1)) + 4;
					    #if VGA_AUTO_CLK_DBG
							LogS(" pixel clok over, reset clock val");
					    #endif
						}
					}
					else
					{

						_wEndClk=(_wCurClk & (~1)) + 4;
						_wBestClk=_wCurClk;
                                    #if VGA_AUTO_DBG_MSG
						LogS("auto clk fail\n");
                                    #endif
						_bAutoClockFail = 1 ;
						//if fail , continue to do phase and position
					}
				}

				// set the initial value for auto clock
				{
					_bCurPhase = 0;							// current phase
					_bClkCnt = 0;							// clk point cnt
					_dBestSum = 0;							//Max clock diff
					_dSumTmp[2] = 0;						//diff accu for clock

					//go to clock2 for default clock phase diff measure
					vSetSP0AutoState(VDO_AUTO_CLOCK_2_START);
					_bVgaDelayCnt = 1;
					//vDrvVGASetPhase(_bCurPhase);
					vDrvVGASetPhase_Simple(_bCurPhase); //Do auto replace current setting
                            #if VGA_HW_AUTO
					_bWaitHwRdy = 0 ;
                            #endif
				}
			}
			else
			{
				vSetSP0AutoState(VDO_AUTO_NOT_BEGIN);
				vApiVideoClrEvtFlg(VDO_FLG_VGAAUTO_DOING);
				vClrAutoFlg(SP0_AUTO_ALL);
				vApiNotifyAutoDone(((fgIsMainVga())?0:1),FALSE);
		       #if NOTIFY_PROGRESS
				vApiNotifyVGAAutoProgress(((fgIsMainVga())?0:1),100);
		       #endif
			}

			break;

		case VDO_AUTO_CLOCK_1_START:
			#if NOTIFY_PROGRESS
			if (_IsVgaDetectDone)
			{
				_bProgressBar += 8 ; //6*8=48 ;
				vApiNotifyVGAAutoProgress(((fgIsMainVga())?0:1),_bProgressBar);
			}
			#endif
			//this state is to find max phase diff of several clock settings.
			if (_bClkCnt < 6)								// even clock, clk-4, clk-2, clk, clk+2, clk+4 and table default
			{
                      #if VGA_AUTO_CLK_DBG
				LogD(_dSumTmp[2]);
                      #endif

				if (_dBestSum < _dSumTmp[2])
				{
					_dBestSum = _dSumTmp[2];
					_wBestClk = _wCurClk;					// Find the clock that makes the maximum difference.
				}

				if (_bClkCnt == 0)
				{
					_wCurClk = _wEndClk - 8;				// Start from -4 (Initial is +4)
	                  #if VGA_AUTO_CLK_SKIP_TABLE_HTOTAL
                                    _dBestSum = 0;
                                    LogS("Skipped table value");
	                  #endif
				}
				else
				{
					_wCurClk = _wCurClk + 2;				// Inc by 2 each time
				}

				_bClkCnt++;

				//LogSW("_wCurClk", _wCurClk);
                      #if SUPPORT_VGA_AMBIGUOUS_H_DETECT

				if (_IsVgaDetectDone)
                      #endif
				{
					vSetSP0AutoState(VDO_AUTO_CLOCK_2_START);
					vDrvVGASetClock(_wCurClk);
					_bVgaDelayCnt = 1;
                            #if VGA_HW_AUTO
					_bWaitHwRdy = 0 ;
                            #endif
				}
                      #if SUPPORT_VGA_AMBIGUOUS_H_DETECT
				else //Ambiguous timing Test case
				{
					vSetSP0AutoState(VDO_AUTO_NOT_BEGIN);
					_dPhaseDiff = _dSumTmp[2];
					vClrAutoFlg(SP0_AUTO_CLOCK);

				}

                     #endif

				_bCurPhase = 0;									// phase counter
				vDrvVGASetPhase_Simple(_bCurPhase); //Do auto replace current setting
			}
			else
			{
				UINT8 bSkipPhase=0;
				if ((_dBestSum == 0)
				//	 || (_dBestSum < VGA_CLK_PH_THRE)
				)
				{
					//if phase diff is zero , reset to default clock
                            #if VGA_AUTO_CLK_DBG
					LogS(" reset clock val");
                            #endif
					_wBestClk = (bVgaOpt07_AutoKeepOldVal==1)?_wOrgClock:Get_VGAMODE_IHTOTAL(_bVgaTiming) ;
					bSkipPhase = 1 ;
				}

                //check AD spec is over range or not.
				if(  (wSP0Hclk*_wBestClk/1000)> wVGAADSpec)
				{

                            #if VGA_AUTO_CLK_DBG
					LogS(" pixel clok over, reset clock val");
                            #endif
					_wBestClk = (bVgaOpt07_AutoKeepOldVal==1)?_wOrgClock:Get_VGAMODE_IHTOTAL(_bVgaTiming) ;
				}
#ifdef __MODEL_slt__
				LogS("SLT skip auto clock and set to tbl clock");
				_wBestClk = Get_VGAMODE_IHTOTAL(_bVgaTiming) ;
#else
				if(_bAutoClockFail && bVgaOpt07_AutoKeepOldVal)
				{
					_wBestClk = _wOrgClock ;
					bSkipPhase = 1 ;
				}
#endif

				vDrvVGASetClock(_wBestClk);


				if(bSkipPhase)
				{
					#if VGA_AUTO_CLK_DBG
					LogS(" skip auto phase");
					#endif
					vDrvVGASetPhase(_bOrgPhase);
					vClrAutoFlg(SP0_AUTO_PHASE);
					vSetSP0AutoState(VDO_AUTO_POSITION_1_START);
					vVgaAutoPosInit();
					_bVgaDelayCnt = 2;
                    #if CHANGE_SDDS_KPI
		            vDrvEnableChang_SDDS_BW();
	             	#endif
				}
				else
				{
					vSetSP0AutoState(VDO_AUTO_PHASE_START);
				}

				vClrAutoFlg(SP0_AUTO_CLOCK);
				_wVgaClock = _wBestClk;
                     #if VGA_AUTO_UI_DBG
				LogSW("auot clk=>", _wVgaClock);
                     #endif
			}


			break;

		case VDO_AUTO_CLOCK_2_START:
              #if VGA_HW_AUTO
			if (_bHwAuto)
			{
				if (!_bWaitHwRdy)
				{
					if (fgIsCLKLock())
					{
						// auto clock trigger
						vVgaHwAutoClkEnable(SV_ON);
						_bWaitHwRdy =1 ;
					}
				}
				else
				{
					if (bDrvHwAutoClkRdy())
					{
						//read the max/min phase difference
						_dSumTmp[2] =	dDrvAutoGetPhsMaxMinDiff();
						vVgaHwAutoClkEnable(SV_OFF);
						_bVdoSP0AutoState = VDO_AUTO_CLOCK_1_START;
					}
				}

			}
			else
                #endif //#if VGA_HW_AUTO
			{

				if (_IsVgaDetectDone)
				{
					_bAutoISR = 1 ;
				}
				else
				{	  // no speedup
					_bAutoISR = 0 ;
					//this state  to get the max phase diff of 0, 4, 8 .. 28 phase sum
					if (_bCurPhase == 0)							  // phase counter
					{
						_dSumTmp[0] = dVGAGetAllDiffValue();
						_dSumTmp[1] = _dSumTmp[0];
					}

					if (_bCurPhase <= 31)
					{
						UINT32 dTmp;

						dTmp = dVGAGetAllDiffValue();
						if (_dSumTmp[0] < dTmp)
						{
							_dSumTmp[0] = dTmp;					   // max diff value
						}

						if (_dSumTmp[1] > dTmp)
						{
							_dSumTmp[1] = dTmp;					   // min diff value
						}

						_dSumTmp[2] = (_dSumTmp[0] - _dSumTmp[1]);  // max/min difference
						_bCurPhase = _bCurPhase + 4;
						//vDrvVGASetPhase(_bCurPhase);
						vDrvVGASetPhase_Simple(_bCurPhase); //Do auto replace current setting
						_bVgaDelayCnt = 1;
					}
					else
					{
						_bVdoSP0AutoState = VDO_AUTO_CLOCK_1_START;
					}

				}
			}
			break;

		case VDO_AUTO_PHASE_START:
                #if VGA_TIMEDELAY_DBG
			HAL_GetTime(&cur_time[2]);
                #endif
                #if VGA_HW_AUTO
			//YPbPr use software auto phase
			if (_bHwAuto&& fgIsMainVga())
			{
				_bWaitHwRdy = 0 ;
			}
			else
                #endif //					#if VGA_HW_AUTO
			{
				_bCurPhase = 31;					//phase value
				_bBestPhase = 31;					// best phase
				_dBestSum = 0;						//Max phase sum
				//vDrvVGASetPhase(_bCurPhase); // Initial set phase to 31
				vDrvVGASetPhase_Simple(_bCurPhase); //Do auto replace current setting
				_bVgaDelayCnt = 1;
				vDrvInitPhaseVar();
			}
			vSetSP0AutoState(VDO_AUTO_PHASE_1_START);

			break;

		case VDO_AUTO_PHASE_1_START:
              #if VGA_HW_AUTO
			//YPbPr use software auto phase
			if (_bHwAuto && fgIsMainVga())
			{
				if (!_bWaitHwRdy)
				{
					if (fgIsCLKLock())
					{
						// auto clock trigger
						vVgaHwAutoPhaseEnable(SV_ON);
						_bWaitHwRdy =1 ;
					}
				}
				else
				{
					if (bDrvHwAutoPhsRdy())
					{
			              #if NOTIFY_PROGRESS
						_bProgressBar += 32 ; // 32+48 = 80 ;
						vApiNotifyVGAAutoProgress(((fgIsMainVga())?0:1),_bProgressBar);
			              #endif
						//read the max/min phase difference
						_bBestPhase = bDrvAutoGetPhsGood();
						// reset the clock trigger
						vVgaHwAutoPhaseEnable(SV_OFF);
						/*	vDrvADCPLLSetRelatch's setting of auto adjust and UI setting are different. It causes the position shift 1 pixel. */
						vDrvVGASetPhase(_bBestPhase);	//only this not Simple
                                          vDrvRETIMEReset();            // Reset for FIFO pointer. For next step auto position
                                                                                      // Solved VGA right line disappear when source switch
						//vDrvVGASetPhase_Simple(_bBestPhase); //Do auto replace current setting
                                   #if VGA_AUTO_UI_DBG
						LogSB("Phase eeprom=>", _bBestPhase);
                                   #endif
						vClrAutoFlg(SP0_AUTO_PHASE);
						vSetSP0AutoState(VDO_AUTO_POSITION_1_START);
						vVgaAutoPosInit();
						_bVgaDelayCnt = 2;
					}
				}
			}
			else
               #endif //#if VGA_HW_AUTO
			{
				_bAutoISR = 1 ;
			}
			break;


		case VDO_AUTO_POSITION_1_START:
			if ((fgIsMainVga() || fgIsPipVga())
			                                        //&& (_IsVgaDetectDone == TRUE)
			                                        && (fgIsCLKLock()))
			{
				vVGAAutoGetBoundary();
                       #if VGA_AUTO_POS_DBG
				LogSW("_wVgaHStart", _wVgaHStart);
				LogSW("_wVgaVStart", _wVgaVStart);
				LogSW("_wVgaHEnd", _wVgaHEnd);
				LogSW("_wVgaVEnd", _wVgaVEnd);
                       #endif

				vClrAutoFlg(SP0_AUTO_POSITION);
#if 	VGA_AUTO_SPEEDUP
				//  bVGAAutoGetBoundary();
				vVgaAutoPosSet();
				vClrAutoFlg(SP0_AUTO_POSSET);
				vApiVideoClrEvtFlg(VDO_FLG_VGAAUTO_DOING);
				vApiNotifyAutoDone(((fgIsMainVga())?0:1),TRUE);
		         #if NOTIFY_PROGRESS
				vApiNotifyVGAAutoProgress(((fgIsMainVga())?0:1),100);
		         #endif
#endif //VGA_AUTO_SPEEDUP
                       #if VGA_TIMEDELAY_DBG
				HAL_GetTime(&cur_time[3]);

				{
					int i;

					for (i=0;i<5;i++)
						LOG(3,"[%d] %d:%d \n",i, cur_time[i].u4Seconds,cur_time[i].u4Micros);
				}

                      #endif
				vSetSP0AutoState(VDO_AUTO_NOT_BEGIN);
			}

			break;
		default:
			break;
	}	//switch case
}



/**
 * @brief vSetHPosition(UINT8 bValue, UINT8 sp)
 * Set H Active Position
 * @param  wValue : H start value
 * @retval void
 * @example vDrvSetHPosition(20, 0) - set sp0 H start position 20
 */
void vDrvSetHPosition(UINT16 wValue)
{
	UINT16 max;

	if (fgIsMainVga()||fgIsPipVga())
	{
		max=Get_VGAMODE_IHTOTAL(_bVgaTiming)-10; //very unreasonable

		if (wValue>max)
		{
			wValue=max;
		}

		vVgaSetInput(wValue, Get_VGAMODE_IPH_WID(_bVgaTiming));	// for the bug: the right side of the VGA picture has the pink garbage
	}
	else
	{

		max=Get_VGAMODE_IHTOTAL(_bHdtvTiming)-10; //very unreasonable

		if (wValue>max)
		{
			wValue=max;
		}

		vVgaSetInput(wValue, Get_VGAMODE_IPH_WID(_bHdtvTiming));	// for the bug: the right side of the VGA picture has the pink garbage
	}

//	UNUSED(sp);
}

/**
 * @brief vDrvSetVPosition(UINT8 bValue, UINT8 sp)
 * Set V Active Position
 * @param  wValue : V start value
 * @retval void
 * @example vDrvSetVPosition(20, 0) - set sp0 V start position 20
 */
void vDrvSetVPosition(UINT16 wValue)
{
	UINT8  bTmpTiming;

	if (fgIsMainVga()||fgIsPipVga())
	{
		bTmpTiming=_bVgaTiming;
	}
	else
	{
		bTmpTiming = _bHdtvTiming;
	}

	if (Get_VGAMODE_INTERLACE(bTmpTiming) == PROGRESSIVE)
	{
		vDrvAsyncVsyncStart(wValue);

	}

//	UNUSED(sp);
}

// **********************************************************
// Function : void vDrvVgaAutoStart(void)
// Description :
// Parameter :
// Return    :
// **********************************************************
/**
 * @brief vDrvVgaAutoStart(void)
 * Trigger  SP0/SP1 Auto state machine to start
 * @param  void
 * @retval void
 * @example vDrvVgaAutoStart()
 */
void vDrvVgaAutoStart(void)
{
	if (fgIsMainVga() || fgIsPipVga())
	{
		if (_IsVgaDetectDone && fgIsValidTiming(_bVgaTiming))
		{
#if VGA_TIMEDELAY_DBG
			HAL_GetTime(&cur_time[0]);
#endif
			vSetAsyncMeasureBD(DOMAIN_PIXEL); // penggang add 20090825 for cr DTV00212128
			vVgaAutoInit();
			vSetAutoFlg(SP0_AUTO_ALL);
			vApiVideoSetEvtFlg(VDO_FLG_VGAAUTO_DOING);
			#if NOTIFY_PROGRESS
			_bProgressBar = 0 ;
			#endif
#if THERMAL_COMPENSATION_ENABLE
			vThermalCompInit();
#endif
#if CHANGE_SDDS_KPI
            Set_SDDS_KPI(1);
#endif
		}
	}
}

void vDrvVgaAutoStop(void)
{
	if (fgIsMainVga() || fgIsPipVga())
	{
		if ((_IsVgaDetectDone) && (fgIsCLKLock()))
		{
#if VGA_TIMEDELAY_DBG
			HAL_GetTime(&cur_time[0]);
#endif
			vClrAutoFlg(SP0_AUTO_ALL);
			vApiVideoClrEvtFlg(VDO_FLG_VGAAUTO_DOING);
			vSetSP0AutoState(VDO_AUTO_NOT_BEGIN);
			#if NOTIFY_PROGRESS
			_bProgressBar = 0 ;
			#endif

#if THERMAL_COMPENSATION_ENABLE
			vThermalCompInit();
#endif
#if CHANGE_SDDS_KPI
            vDrvEnableChang_SDDS_BW();
#endif
		}
	}
}

// **********************************************************
// Function : void vDrvYPbPrAutoStart(void)
// Description :
// Parameter :
// Return    :
// **********************************************************
/**
 * @brief vDrvYPbPrAutoStart(void)
 * Trigger  SP0/SP1 Auto state machine to start
 * @param  void
 * @retval void
 * @example vDrvVgaAutoStart()
 */
void vDrvYPbPrAutoStart(void)  //YPbPr Auto Phase 2006/11/07
{

	if (fgIsMainYPbPr() || fgIsPipYPbPr())
	{
		if ((_IsHdtvDetectDone) && (fgIsCLKLock()))
		{
			vSetSP0AutoState(VDO_AUTO_PHASE_START);
			vApiVideoSetEvtFlg(VDO_FLG_VGAAUTO_DOING);
#if CHANGE_SDDS_KPI
            Set_SDDS_KPI(1);
#endif
		}
	}
}


#endif
