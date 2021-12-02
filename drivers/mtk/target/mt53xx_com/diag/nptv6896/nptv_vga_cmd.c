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
 *-----------------------------------------------------------------------------
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: nptv_vga_cmd.c,v $
 * $Revision: #1 $
 *
 * Description:
 *         This file contains CLI implementation of NPTV Video.
 *---------------------------------------------------------------------------*/

#include "x_os.h"
#include "x_stl_lib.h"
#include "video_def.h"

#ifdef __MW_CLI_DEF__
#include "x_mid_cli.h"
#else
#include "x_drv_cli.h"
#endif

#include "x_assert.h"
#include "x_pinmux.h"
#include "x_debug.h"

#include "source_table.h"
#include "sv_const.h"
#include "drv_auto.h"
#include "hw_vdoin.h"

#include "drv_adcset.h"
#include "drv_vga.h"
#include "drv_autocolor.h"
#include "drv_vdoclk.h"
#include "vdo_misc.h"
#include "eepdef.h" //kalcheng
#ifndef CC_CLI
#define CC_CLI
#endif

#ifdef CC_CLI

/* Functions for VGA */
static INT32 _i4VgaReset(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4VgaStatus(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4VgaAuto(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4VgaAutoCLR(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4VgaMeasureBlankLevel(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4VgaAdjustBlankLevel(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4VgaPGALinearityVerify(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4VgaADCLinearityVerify(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4VgaSDDSLoopFilterID(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4VgaDDDSLoopFilterID(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4VgaPos(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4VgaClock(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4VgaUserMode(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4VgaPhaseSel(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4VgaBoundSel(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4VgaFw(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4VgaDebug(INT32 i4Argc, const CHAR **szArgv);
//static INT32 _i4VgaSLT(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4VgaSetAdcPll(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4VgaSetAdSlicer(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4VgaVGARefVoltageVerify(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4VgaSyncSlicerRefVoltageVerify(INT32 i4Argc, const CHAR **szArgv);

#ifdef SUPPORT_VGA_UI_AMBIGUOUS_TIMING_SELECT 
static INT32 _i4VgaAmbiguousQuery(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4VgaAmbiguousSet(INT32 i4Argc, const CHAR **szArgv);
#endif
#if defined(__linux__)
static INT32 _VgaGetClock(INT32 i4Argc, const CHAR **szArgv);
static INT32 _VgaGetPhase(INT32 i4Argc, const CHAR **szArgv);
static INT32 _VgaAuto(INT32 i4Argc, const CHAR **szArgv);
static INT32 _VgaAutoColor(INT32 i4Argc, const CHAR **szArgv);
//static INT32 _DecIsVgaTiming(INT32 i4Argc, const CHAR **szArgv);
#endif /* __LINUX__ */
static INT32 _i4VgaSetOversample(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4AutocolorResult(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4EmbeddedGain(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4EmbeddedOverRule(INT32 i4Argc, const CHAR **szArgv);
extern UINT8 bApiEepromReadByte(UINT16 wAddr); 
extern BOOL fgApiEepromWriteByte(UINT16 wAddr, UINT8 bData);
#endif /*#ifdef CC_CLI*/
/******************************************************************************
* CLI Command Table
******************************************************************************/

// Table for vga command entry
CLI_EXEC_T arVgaCmdTbl[] = {
#ifdef CC_CLI
  {"reset", "r", _i4VgaReset, NULL, "reset(reset VGA FSM)", CLI_SUPERVISOR},
  {"status", "s", _i4VgaStatus, NULL, "status(report VGA status)", CLI_SUPERVISOR},
  {"auto", "a", _i4VgaAuto, NULL, "auto(now),auto [0,1](set default)", CLI_SUPERVISOR},
  {"autocolor", "ac", _i4VgaAutoCLR, NULL, "ac(now)", CLI_GUEST},
  {"blank_level_measure", "bl", _i4VgaMeasureBlankLevel, NULL, "bl(now)", CLI_GUEST},
  {"blank_level_adjust", "adj", _i4VgaAdjustBlankLevel, NULL, "bl(now)", CLI_SUPERVISOR},
  {"VGA_PGA_Linearity", "pga", _i4VgaPGALinearityVerify, NULL, "pga(now)", CLI_SUPERVISOR},
  {"VGA_ADC_linearity", "adc", _i4VgaADCLinearityVerify, NULL, "adc(now)", CLI_SUPERVISOR},
  {"SDDS_ID","sddsid", _i4VgaSDDSLoopFilterID,       NULL, "SDDS KI0 KP0 ID",  CLI_SUPERVISOR},  
  {"DDDS_ID","dddsid", _i4VgaDDDSLoopFilterID,       NULL, "DDDS KI0 KP0 ID",  CLI_SUPERVISOR},  
  {"clock", "clk", _i4VgaClock, NULL, "clk ,clk [xx]", CLI_SUPERVISOR},
  {"usermode", "um", _i4VgaUserMode, NULL, "um ", CLI_SUPERVISOR},
  {"position", "pos", _i4VgaPos, NULL, "pos,pos [xx]", CLI_SUPERVISOR},
  {"phasesel", "ps", _i4VgaPhaseSel, NULL, "ps [0~31] (set input phase+fixup)", CLI_SUPERVISOR},
  {"phase_plus", "ps+", _i4VgaPhaseSel, NULL, "phase +=1  (max 31)", CLI_SUPERVISOR},
  {"phase_minus", "ps-", _i4VgaPhaseSel, NULL, "phase -=1 (min 0) ", CLI_SUPERVISOR},
  {"phase_justset", "pjs", _i4VgaPhaseSel, NULL, "ps [0~31] (set input phase,no fixup)", CLI_SUPERVISOR},
  {"bdsel", "bds", _i4VgaBoundSel, NULL, "bds [0~255] (set boundary threshold)", CLI_SUPERVISOR},
  {"fw", "fw", _i4VgaFw, NULL, "Fw on/off", CLI_SUPERVISOR},
  {"debug", "dbg", _i4VgaDebug, NULL, "Debug [1:MDET, 2:MDCHG 4:AUTO]", CLI_SUPERVISOR},
  {"setpll", "setpll", _i4VgaSetAdcPll, NULL, "pixelclk(/10 MHz)  htotal ", CLI_SUPERVISOR},
  {"adslicer", "adslicer", _i4VgaSetAdSlicer, NULL, "0/1 ", CLI_SUPERVISOR},
  {"Oversample", "Oversample", _i4VgaSetOversample, NULL, "0/1 ", CLI_SUPERVISOR},
  {"autocolor result", "acresult", _i4AutocolorResult, NULL, "0/1 ", CLI_GUEST},
  {"Embedded gain result", "ebgain", _i4EmbeddedGain, NULL, "0/1 ", CLI_GUEST},
  {"OverRule Embedded gain", "ebover", _i4EmbeddedOverRule, NULL, "on/off", CLI_GUEST},
  {"VGA_Ref_Voltage", "vgarvol", _i4VgaVGARefVoltageVerify, NULL, "VGA reference voltage ", CLI_SUPERVISOR},
  {"SyncSlicer_Ref_Voltage", "syncrvol", _i4VgaSyncSlicerRefVoltageVerify, NULL, "SyncSlicer reference voltage ", CLI_SUPERVISOR},

#ifdef SUPPORT_VGA_UI_AMBIGUOUS_TIMING_SELECT  
  {"ambquery", "amq", _i4VgaAmbiguousQuery, NULL, "Ambiguous Timing Query", CLI_SUPERVISOR},
  {"ambSet", "ams", _i4VgaAmbiguousSet, NULL, "Ambiguous Timing Set [Candidate Id]", CLI_SUPERVISOR},
#endif  
#if defined(__linux__)
  {"getclock", "gclk", _VgaGetClock, NULL, "get vga clock", CLI_SUPERVISOR},
  {"getphase", "gps", _VgaGetPhase, NULL, "Get vga phase", CLI_SUPERVISOR},
  {"beginauto", "ba", _VgaAuto, NULL, "begin vga auto adjust", CLI_SUPERVISOR},
  {"beginautocolor", "bac", _VgaAutoColor, NULL, "begin vga auto color", CLI_SUPERVISOR},
#endif
//  {"slt", "slt", _i4VgaSLT, NULL, "SLT testing", CLI_SUPERVISOR},
#endif/*#ifdef CC_CLI*/
  {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

#ifdef CC_CLI
/******************************************************************************
* Local Function
******************************************************************************/

/*---------------------------------VGA --------------------------------*/
void vVgaSwReset(void);
void vVgaStatus(void);
void vHdtvSwReset(void);
void vHdtvStatus(void);
static INT32 _i4VgaReset(INT32 i4Argc, const CHAR **szArgv)
{
	if (fgIsMainYPbPr() || fgIsPipYPbPr()) {
			vHdtvSwReset();
	} else {		vVgaSwReset(); }
	return 0;
}
static INT32 _i4VgaStatus(INT32 i4Argc, const CHAR **szArgv)
{
	if (fgIsMainYPbPr() || fgIsPipYPbPr()) {
			vHdtvStatus();
	} else {	vVgaStatus(); }
    return 0;
}
extern UINT8   _bAutoFlag;
extern UINT8   _bVdoSP0AutoState;

static INT32 _i4VgaAuto(INT32 i4Argc, const CHAR **szArgv)
{
	if(i4Argc==2)
	{

		if(x_strcmp(szArgv[1],"pos")==0)
				_bAutoFlag=0x9;
		if(x_strcmp(szArgv[1],"ps")==0)
				_bAutoFlag=0x2;
		if(x_strcmp(szArgv[1],"clk")==0)
				_bAutoFlag=0x4;
		if(x_strcmp(szArgv[1],"yps")==0)
			_bVdoSP0AutoState = VDO_AUTO_PHASE_START;

	}
	else
	{
		Printf("doing auto...\n");
		_bAutoFlag=0xf;
	}
    return 0;
}

extern UINT8   _bAutoColorState0;
static INT32 _i4VgaAutoCLR(INT32 i4Argc, const CHAR **szArgv)
{
	Printf("doing auto color...must use full Black+White,Full Color pattern\n");
	_bAutoColorState0=1;
    return 0;
}

static INT32 _i4VgaMeasureBlankLevel(INT32 i4Argc, const CHAR **szArgv)   
{
	Printf("Measure the blank level\n");
	_bAutoColorState0=0;
	vDrvEnableBlankLevelMeasure();
    return 0;
}                                                                                                                
static INT32 _i4VgaAdjustBlankLevel(INT32 i4Argc, const CHAR **szArgv)   
{
	Printf("Adjust the blank level\n");
	_bAutoColorState0=0;
	vDrvEnableBlankLevelAdjust();
    return 0;
}                                                                                                                

static INT32 _i4VgaSDDSLoopFilterID(INT32 i4Argc, const CHAR **szArgv)   
{
   UINT8 KP0_start=0;
   UINT8 KP0_end=0;
   UINT8 KI0_start=0;
   UINT8 KI0_end=0;

   if(i4Argc==1)
   {
      if (x_strcmp(szArgv[0], "sddsid") == 0)
      {
         KP0_start = 2;
         KP0_end = 0x0a;
         KI0_start = 2;
         KI0_end = 0x0a;
        Printf("KP0_start =%d, KP0_end =%d,KI0_start =%d,KI0_end =%d\n",KP0_start,KP0_end,KI0_start,KI0_end);
        #if defined(CC_MT8223)
        #else
	    vDrvEnable_SDDS_Calibration(KP0_start,KP0_end,KI0_start,KI0_end);
	    #endif
      }
      else
      {
        Printf("typing error\n");
      }
   }
   else
   {
      if(i4Argc==5)
      {
        KP0_start = StrToInt(szArgv[1]);
        KP0_end = StrToInt(szArgv[2]);
        KI0_start = StrToInt(szArgv[3]);
        KI0_end = StrToInt(szArgv[4]);
        Printf("KP0_start =%d, KP0_end =%d,KI0_start =%d,KI0_end =%d\n",KP0_start,KP0_end,KI0_start,KI0_end);
        #if defined(CC_MT8223)
        #else
	    vDrvEnable_SDDS_Calibration(KP0_start,KP0_end,KI0_start,KI0_end);
	    #endif
      }
      else
      {
         Printf("Input parameter not completely\n");
      }
   }
    return 0;
}                       


static INT32 _i4VgaDDDSLoopFilterID(INT32 i4Argc, const CHAR **szArgv)   
{
   UINT8 KP0_start=0;
   UINT8 KP0_end=0;
   UINT8 KI0_start=0;
   UINT8 KI0_end=0;
   UINT8 ERROR_LIM=0xFF;
   UINT32 CW=0;

   if(i4Argc==1)
   {
      if (x_strcmp(szArgv[0], "sddsid") == 0)
      {
         KP0_start = 2;
         KP0_end = 0x0a;
         KI0_start = 2;
         KI0_end = 0x0a;
         ERROR_LIM =0xff;
        Printf("DKP0_start =%d, DKP0_end =%d,DKI0_start =%d,DKI0_end =%d\n",KP0_start,KP0_end,KI0_start,KI0_end);
      #if defined(CC_MT8223)
      #else
	  vDrvEnable_DDDS_Calibration(KP0_start,KP0_end,KI0_start,KI0_end,ERROR_LIM,CW);
	  #endif
      }
      else
      {
        Printf("typing error\n");
      }
   }
   else
   {
      if(i4Argc==7)
      {
        KP0_start = StrToInt(szArgv[1]);
        KP0_end = StrToInt(szArgv[2]);
        KI0_start = StrToInt(szArgv[3]);
        KI0_end = StrToInt(szArgv[4]);
        ERROR_LIM = StrToInt(szArgv[5]);
        CW=StrToInt(szArgv[6]);
        Printf("DKP0_start =%d, DKP0_end =%d,DKI0_start =%d,DKI0_end =%d, ERROR_LIM=%d, CW=%x\n",KP0_start,KP0_end,KI0_start,KI0_end,ERROR_LIM,CW);
      #if defined(CC_MT8223)
      #else
	  vDrvEnable_DDDS_Calibration(KP0_start,KP0_end,KI0_start,KI0_end,ERROR_LIM,CW);
      #endif 
      }
      else
      {
         Printf("Input parameter not completely\n");
      }
   }
    return 0;
}                       


static INT32 _i4VgaPGALinearityVerify(INT32 i4Argc, const CHAR **szArgv)   
{
	Printf("Verify PGA linearity\n");
	vDrvEnablePGALinearityVerify();
    return 0;
}    

static INT32 _i4VgaADCLinearityVerify(INT32 i4Argc, const CHAR **szArgv)   
{
	Printf("Verify ADC linearity\n");
	if(i4Argc==1)
	    vDrvEnableADCLinearityVerify(16);
	else
	    vDrvEnableADCLinearityVerify(StrToInt(szArgv[1]));
    return 0;
}                                                                                                                

static INT32 _i4VgaVGARefVoltageVerify(INT32 i4Argc, const CHAR **szArgv)   
{
	Printf("VGA Ref Voltage Verify\n");
    vDrvEnableVGARefVoltageVerify(StrToInt(szArgv[1]));
    return 0;
}                                                                                                                

static INT32 _i4VgaSyncSlicerRefVoltageVerify(INT32 i4Argc, const CHAR **szArgv)   
{
	Printf("SyncSlicer Ref Voltage Verify\n");
    vDrvEnableSyncSlicerRefVoltageVerify(StrToInt(szArgv[1]));
    return 0;
}                                                                                                                

static INT32 _i4VgaPos(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Path ;
        if(fgIsMainVga()||fgIsMainYPbPr())
        {
            u1Path =SV_VP_MAIN;
        }
        else
        {
             u1Path =SV_VP_PIP;
        }
        if(i4Argc < 2)
        {

            Printf("hporch--> %d, vporch --> %d\n",wDrvVideoGetPorch(u1Path, SV_HPORCH_CURRENT),wDrvVideoGetPorch(u1Path, SV_VPORCH_CURRENT));
        }
    
	if(i4Argc==2)
	{
		int target_h,target_v, target_h_old;
		target_h=target_h_old =wDrvVideoGetPorch(u1Path, SV_HPORCH_CURRENT);
		target_v =wDrvVideoGetPorch(u1Path, SV_VPORCH_CURRENT);
		if(x_strcmp(szArgv[1],"h+")==0)
			target_h++;
		else if(x_strcmp(szArgv[1],"h-")==0)
			target_h--;
		else if(x_strcmp(szArgv[1],"H+")==0)
			target_h++;
		else if(x_strcmp(szArgv[1],"H-")==0)
			target_h--;
		else if(x_strcmp(szArgv[1],"v+")==0)
			target_v++;
		else if(x_strcmp(szArgv[1],"v-")==0)
			target_v--;
		else if(x_strcmp(szArgv[1],"V+")==0)
			target_v++;
		else if(x_strcmp(szArgv[1],"V-")==0)
			target_v--;

		if(target_h_old != target_h)
		{
                    wDrvVideoSetPorch(u1Path, SV_HPORCH_CURRENT, target_h);
		}
		else
		{
                    wDrvVideoSetPorch(u1Path, SV_VPORCH_CURRENT, target_v);
		}
		 Printf("Set hporch--> %d, vporch --> %d\n",target_h,target_v);
	}



	if(i4Argc==3)
	{
	       Printf("Set hporch--> %d, vporch --> %d\n",StrToInt(szArgv[1]),StrToInt(szArgv[2]));
		wDrvVideoSetPorch(u1Path, SV_HPORCH_CURRENT, StrToInt(szArgv[1]));
              wDrvVideoSetPorch(u1Path, SV_VPORCH_CURRENT, StrToInt(szArgv[2]));
	}
    return 0;
}


void  vDrvVGASetClock(UINT16  wValue);
UINT16 wDrvVGAGetClock(void);
UINT8 fgApiVideoVgaSetClock(UINT16 wValue) ;
static INT32 _i4VgaClock(INT32 i4Argc, const CHAR **szArgv)
{
	if(i4Argc==2)
	{
		int target_clk=wDrvVGAGetClock();

		if(szArgv[1][0]=='+')
			target_clk++;
		else if(szArgv[1][0]=='-')
			target_clk--;
		else target_clk=StrToInt(szArgv[1]);

		fgApiVideoVgaSetClock(target_clk);
	}
		Printf("Vga Clock set to %d\n", wDrvVGAGetClock());
    return 0;
}



void vVgaEraseUserMode(UINT8 index);

//extern UINT16	_wForceXRes,_wForceYRes;
static INT32 _i4VgaUserMode(INT32 i4Argc, const CHAR **szArgv)
{
	if (x_strcmp(szArgv[1], "flush") == 0)
	{
		if(i4Argc==3)
		{
			vVgaEraseUserMode(StrToInt(szArgv[2]));
		}
		else
		{
			vVgaEraseUserMode(0xff);
		}
	}
	Printf(" usage: flush\n");

	return 0;
}


void vDrvVGASetPhase(UINT8 bVal);
static INT32 _i4VgaPhaseSel(INT32 i4Argc, const CHAR **szArgv)
{
	if(i4Argc==2)
	{
		if (x_strcmp(szArgv[0], "ps") == 0)
			vDrvVGASetPhase(StrToInt(szArgv[1])&0x1f);
		else if (x_strcmp(szArgv[0], "pjs") == 0)
		{
//			vDrvVGASetPhase_Simple(StrToInt(szArgv[1])&0x1f);
			UINT32 i;
				UINT16 rb,lb,hact;
			for(i=0;i<32;i++)
			{
				//vDrvVGASetPhase(i);
				vDrvVGASetPhase_Simple(i);
				x_thread_delay(100);
				rb=(*(volatile UINT32*)0x2002208c)&0xffff;
				lb=(rb&0xff)|((rb>>4)&0xf00);
				rb=((*(volatile UINT32*)0x2002208c))&0xf00;
				rb|=((*(volatile UINT32*)0x20022090)>>24)&0xff;
				hact=rb-lb;
			Printf("(%d)LB=%d RB=%d Hact=%d\n",i,lb,rb,hact);
			}
		}
	}
	else
	{
		if (x_strcmp(szArgv[0], "ps+") == 0)
			vDrvVGASetPhase((bVgaCurPhase()+1)&0x1f);
		else if (x_strcmp(szArgv[0], "ps-") == 0)
			vDrvVGASetPhase((bVgaCurPhase()-1)&0x1f);

			Printf("current phase [%d], may set [0~31]\n",
				bVgaCurPhase());
	}
    return 0;
}

extern void vDrvADCPLLSet(UINT16 wPixClk, UINT16 wHtotal) ;
static INT32 _i4VgaSetAdcPll(INT32 i4Argc, const CHAR **szArgv)
{
    if(i4Argc==3)
    {
        UINT16 u2PixelClk, u2Htotal ;
        u2PixelClk = StrToInt(szArgv[1]);
        u2Htotal = StrToInt(szArgv[2]);
        vDrvADCPLLSet(u2PixelClk, u2Htotal);
        Printf("PixelClk %d Hotal %d\n", u2PixelClk, u2Htotal);
    }
    return 0;
}

extern UINT8 bHdtvOpt05_AdaptiveSlicer; //Enable adaptive slicer
void vQuerySlicer(void);
static INT32 _i4VgaSetAdSlicer(INT32 i4Argc, const CHAR **szArgv)
{
	if(i4Argc==2)
        {
                if(StrToInt(szArgv[1]) == 1)
                {
                	bHdtvOpt05_AdaptiveSlicer = 1 ;
                }
                else if(StrToInt(szArgv[1]) == 0)
                {
                	bHdtvOpt05_AdaptiveSlicer = 0;
                }
                else
                {
                	Printf("adslicer  0/1 \n");
                }
        }
        else
        {
            Printf("adslicer  0/1 \n");
            Printf("Current Slicer setting :%d\n", bHdtvOpt05_AdaptiveSlicer);
            vQuerySlicer();
        }
        return 0;
}



extern UINT8 _bBdThrsh;
static INT32 _i4VgaBoundSel(INT32 i4Argc, const CHAR **szArgv)
{
	if(i4Argc==2)
	{
		_bBdThrsh=StrToInt(szArgv[1])&0xff;

	}
	else
	{
		Printf("current bdthrsh [%d], may set [0~255]\n",
			_bBdThrsh);
	}
    return 0;
}

extern UINT8 _bSP0Flag ;

static INT32 _i4VgaFw(INT32 i4Argc, const CHAR **szArgv)
{
	if(i4Argc==2)
{
			if(StrToInt(szArgv[1]) == 1)
{
				_bSP0Flag = 0x0 ;
			}
			else if(StrToInt(szArgv[1]) == 0)
	{
				_bSP0Flag = 0x80;
			}
			else
				{
				Printf("fw  0/1 \n");
				}
	}
	else
		{
		Printf("fw  0/1 \n");
		}

    return 0;
}


extern UINT8   bVgadbgmask;
static INT32 _i4VgaDebug(INT32 i4Argc, const CHAR **szArgv)
{

	if(i4Argc==2)
	{
        bVgadbgmask=StrToInt(szArgv[1]);
        //Printf("bVgadbgmask  %d\n",bVgadbgmask);
	}
	else
	{
		Printf("bVgadbgmask  %d\n",bVgadbgmask);
	}

    return 0;
}

#ifdef SUPPORT_VGA_UI_AMBIGUOUS_TIMING_SELECT
static INT32 _i4VgaAmbiguousQuery(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 bCandNum, bTmpIdx, bCurrAmb;
    VGAAmbTimingInfo_t rVgaAmbTiming;
    bCandNum = bVgaGetAmbiguousTimingNum();

    if(bCandNum <= 1)
    {
        Printf("Input is not ambiguous timing\n");
    }
    else
    {
        vVgaQueryAmbiguousTiming(QUERY_CUR_AMBIGUOUS_TIMING, &rVgaAmbTiming, 1);
        bCurrAmb = rVgaAmbTiming.bCandiIdx;
        for(bTmpIdx=1;bTmpIdx<=bCandNum;bTmpIdx++)
        {
            vVgaQueryAmbiguousTiming(bTmpIdx, &rVgaAmbTiming, 1);
            if(bTmpIdx == bCurrAmb) 
            {
                Printf(" *");
            }
            else
            {
                Printf("  ");
            }
            Printf(" %d. %dx%d@%d\n", bTmpIdx, rVgaAmbTiming.wHor_res,
                                               rVgaAmbTiming.wVer_res,
                                               rVgaAmbTiming.bVer_freq);
        } 
    } 
    return 0;
}
static INT32 _i4VgaAmbiguousSet(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4CandiIdx;
    if(i4Argc != 2)
    {
        return -1;
    }
    u4CandiIdx = StrToInt(szArgv[1]);
    if(u4CandiIdx == 0)
    {
        vVgaSetAmbiguousUserSel(SV_FALSE);
    }
    else
    {
        vVgaSetAmbiguousUserSel(SV_TRUE);
        vVgaSetAmbiguousTiming(u4CandiIdx, 1);
    }

    return 0;
}
#endif  //#ifdef SUPPORT_VGA_UI_AMBIGUOUS_TIMING_SELECT

#if defined(__linux__)

static INT32 _VgaGetClock(INT32 i4Argc, const CHAR **szArgv)
{
    return (INT32)wDrvVGAGetClock();
}

static INT32 _VgaGetPhase(INT32 i4Argc, const CHAR **szArgv)
{
    return (INT32)u1DrvVGAGetPhase();
}

static INT32 _VgaAuto(INT32 i4Argc, const CHAR **szArgv)
{
    if(fgApiVideoVgaAuto() != (UINT8)TRUE)
    {
        return -1;
    }
    while(_bAutoFlag != 0)
    {
        x_thread_delay(100);
    }
    return  0;
}

static INT32 _VgaAutoColor(INT32 i4Argc, const CHAR **szArgv)
{
    if(fgApiVideoAutoColor() != (UINT8)TRUE)
    {
        return -1;
    }
    while(_bAutoColorState0 != (UINT8)VDO_AUTO_COLOR_NOT_BEGIN)
    {
        x_thread_delay(100);
    }

    return  0;
}

/*static INT32 _DecIsVgaTiming(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4VdpId;
    if(i4Argc != 2)
    {
        return -1;
    }
    u4VdpId = StrToInt(szArgv[1]);
    return  fgApiVideoIsVgaMode((UINT8)u4VdpId, 0);
}*/

#endif /* __LINUX__ */


extern UINT8  _bHdtvCenEnableForSD ;
static INT32 _i4VgaSetOversample(INT32 i4Argc, const CHAR **szArgv)
{
	if(i4Argc==2)
        {
            if(StrToInt(szArgv[1]) == 1)
            {
                _bHdtvCenEnableForSD = 0 ;
            }
            else if(StrToInt(szArgv[1]) == 0)
            {
                _bHdtvCenEnableForSD = 1;
            }
            else
            {
            Printf("Oversample  0/1 \n");
            }
        }
        else
        {
         Printf("Oversample  0/1 \n");
        }
    return 0;
}

extern INT8 code bEFuseCompensation[4][3];
#ifdef CC_MT5365_V2
extern UINT16 _bAutoColorGain[4][3];
#else
extern UINT8 _bAutoColorGain[4][3];
#endif
static INT32 _i4EmbeddedGain(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 bCh;
#ifdef CC_MT5365_V2
    UINT16 _bEmbeddedGain[3]={0,0,0};
#else    
    UINT8 _bEmbeddedGain[3]={0,0,0};
#endif    
    
	if(i4Argc==1)
	{
   	for(bCh=0; bCh<3; bCh++)
	{  //compel Read VGA E Fuse address 0x20008674
		_bEmbeddedGain[bCh] = ((IO32ReadFldAlign(0x20008674, AC_FULLDW))>>(8*bCh));
	}
	}
	else if(i4Argc==2)
	{
      //if(x_strcmp(szArgv[1],"c")==0)//"c" = compensation
      //{
      // for(bCh=0; bCh<3; bCh++)
	  // {	//compel Read VGA E Fuse address 0x20008674
      //  _bEmbeddedGain[bCh] = ((IO32ReadFldAlign(0x20008674, AC_FULLDW))>>(8*bCh))+bEFuseCompensation[bCh];
      // }
	  // Printf("The compensated Gain1=0x%2x, Gain2=0x%2x, Gain3=0x%2x\n", bEFuseCompensation[0], bEFuseCompensation[1], bEFuseCompensation[2]);
      //}
      //else 
      if(StrToInt(szArgv[1])==0)
      {
	    for(bCh=0; bCh<3; bCh++)
		{	
		  _bEmbeddedGain[bCh] = _bAutoColorGain_for_EFuse[0][bCh];//YPbPr
		}
		Printf("After YPbPr transformation\n");
      }
      else if(StrToInt(szArgv[1])==1)
      {
		for(bCh=0; bCh<3; bCh++)
		{   
		  _bEmbeddedGain[bCh] = _bAutoColorGain_for_EFuse[1][bCh];//VGA
		}
		Printf("After VGA transformation\n");
      }
      else if(StrToInt(szArgv[1])==2)
      {
		for(bCh=0; bCh<3; bCh++)
		{   
		  _bEmbeddedGain[bCh] = _bAutoColorGain_for_EFuse[2][bCh];//SCART
		}
		Printf("After SCART transformation\n");
      }
    }
    
	Printf("The Embedded Gain1=0x%x, Gain2=0x%x, Gain3=0x%x\n",_bEmbeddedGain[0],_bEmbeddedGain[1],_bEmbeddedGain[2]);

	return 0;
}

static INT32 _i4EmbeddedOverRule(INT32 i4Argc, const CHAR **szArgv)
{
        
	if(i4Argc==2)
	{
      if(x_strcmp(szArgv[1],"on")==0)//on->overrule
      {
	   UNUSED(fgApiEepromWriteByte(EEP_ADC_AUTOCOLOR_OVER_RULE_EFUSE, 0xAA));
	   Printf("Use Traditional auto color gain\n");
      }
      else if(x_strcmp(szArgv[1],"off")==0)
      {
	   UNUSED(fgApiEepromWriteByte(EEP_ADC_AUTOCOLOR_OVER_RULE_EFUSE, 0xFF));
	   Printf("Use Embedded E Fuse gain\n");
      }
      else
      {
	    Printf("Please add parameter on/off\n");
      }
    }
    else
    {
      Printf("Please add parameter on/off\n");
    }
	return 0;
}

static INT32 _i4AutocolorResult(INT32 i4Argc, const CHAR **szArgv)
{
       UINT8 bStatus,bOrder;
       UINT8 bGain[3];
       UINT8 bOffset[3];
       
       
	if(i4Argc==2)
        {
            if(StrToInt(szArgv[1]) == 0)
            {
               bStatus = bApiEepromReadByte(EEP_VIDEO_AUTO_COLOR_HDTV_DONE); 
               for(bOrder=0;bOrder<3;bOrder++)
               {
                   bGain[bOrder] = bApiEepromReadByte(EEP_VIDEO_AUTO_COLOR_HDTV_GAIN0  + bOrder);
                   bOffset[bOrder] = bApiEepromReadByte(EEP_ADC_OFFSET_HDTV_CLAMP0  + bOrder);
               }
               if(bStatus==1)
               	{
               	   Printf("YPbPr AC done! Gain1=%2x, Gain2=%2x, Gain3=%2x, bOffset1=%2x, bOffset2=%2x, bOffset3=%2x\n",bGain[0],bGain[1],bGain[2],bOffset[0],bOffset[1],bOffset[2]);
               	}
               else
               	{  
               	   Printf("Not doing YPbPr autocolor yet\n");
               	}
              
            }
            else if(StrToInt(szArgv[1]) == 1)
            {
               bStatus = bApiEepromReadByte(EEP_VIDEO_AUTO_COLOR_VGA_DONE); 
               for(bOrder=0;bOrder<3;bOrder++)
               {
                   bGain[bOrder] = bApiEepromReadByte(EEP_VIDEO_AUTO_COLOR_VGA_GAIN0  + bOrder);
                   bOffset[bOrder] = bApiEepromReadByte(EEP_ADC_OFFSET_VGA_CLAMP0  + bOrder);
               }
               if(bStatus==1)
               	{
               	   Printf("VGA AC done! Gain1=%2x, Gain2=%2x, Gain3=%2x, bOffset1=%2x, bOffset2=%2x, bOffset3=%2x\n",bGain[0],bGain[1],bGain[2],bOffset[0],bOffset[1],bOffset[2]);
               	}
               else
               	{  
               	   Printf("Not doing VGA autocolor yet\n");
               	}
            }
            else
            {
               bStatus = bApiEepromReadByte(EEP_VIDEO_AUTO_COLOR_SCART_DONE); 
               for(bOrder=0;bOrder<3;bOrder++)
               {
                   bGain[bOrder] = bApiEepromReadByte(EEP_VIDEO_AUTO_COLOR_SCART_GAIN0  + bOrder);
                   bOffset[bOrder] = bApiEepromReadByte(EEP_ADC_OFFSET_SCART_CLAMP0  + bOrder);
               }
               if(bStatus==1)
               	{
               	   Printf("SCART AC done! Gain1=%2x, Gain2=%2x, Gain3=%2x, bOffset1=%2x, bOffset2=%2x, bOffset3=%2x\n",bGain[0],bGain[1],bGain[2],bOffset[0],bOffset[1],bOffset[2]);
               	}
               else
               	{  
               	   Printf("Not doing SCART autocolor yet\n");
               	}
            }
        }
        else
        {
         Printf("Parameter error \n");
        }
    return 0;
}

#endif // #ifdef CC_CLI

