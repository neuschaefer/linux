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
 * $RCSfile: nptv_vfe_cmd.c,v $
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

#include "drv_adcset.h"
#include "drv_vdoclk.h"
#ifdef CC_MT5395
#include "drv_vbyone.h"
#endif

#ifndef CC_CLI
#define CC_CLI
#endif

#ifdef CC_CLI

/* Functions for VFE*/
static INT32 _NptvAutocolorCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NptvAvCal (INT32 i4Argc, const CHAR **szArgv);
#if defined(CC_MT5365) || defined(CC_MT5395)
static INT32 _NptvCVBSOffset (INT32 i4Argc, const CHAR **szArgv);
#endif
#ifdef CC_MT5395
static INT32 _VByOneScript (INT32 i4Argc, const CHAR **szArgv);
#endif
static INT32 _NptvSlicerIns(INT32 i4Argc, const CHAR * * szArgv);
static INT32 _NptvSlicerCheck (INT32 i4Argc, const CHAR **szArgv);
static INT32 _NptvVopllSpreadCmd(INT32 i4Argc, const CHAR **szArgv);
#if CLI_CVBS
static INT32 _NptvPGACheck (INT32 i4Argc, const CHAR **szArgv);
static INT32 _NptvTargetBlankCheck (INT32 i4Argc, const CHAR **szArgv);
static INT32 _NptvInOutOffsetBlankCheck (INT32 i4Argc, const CHAR **szArgv);
static INT32 _NptvPeakLimitCheck (INT32 i4Argc, const CHAR **szArgv);
static INT32 _NptvSVFCheck (INT32 i4Argc, const CHAR **szArgv);
#endif
#endif /*#ifdef CC_CLI*/
/******************************************************************************
* CLI Command Table
******************************************************************************/

// Table for VFE command entry
CLI_EXEC_T arVfeCmdTbl[] = {
#ifdef CC_CLI
  	{"Autocolor",  "a",    _NptvAutocolorCmd,NULL,           "VGA autocolor",  CLI_SUPERVISOR},
   	{"CvbsCalibration","cal", _NptvAvCal,       NULL,           "CVBS Calibration",  CLI_SUPERVISOR},  	
	{"SlicerInspection", "slicer",    _NptvSlicerIns,     NULL,           "Slicer level Inspection",  CLI_SUPERVISOR},
 	{"SLICERCheck","slc", _NptvSlicerCheck,       NULL,           "SLICER Check",  CLI_SUPERVISOR},                   
	{"VopllSpreadSpectrum", "vss", _NptvVopllSpreadCmd,NULL, "VOPLL Spread Spectrum",  CLI_SUPERVISOR},
#if defined(CC_MT5365) || defined(CC_MT5395)
	{"CVBS Offset Cal.","CVBSOffset", _NptvCVBSOffset,       NULL,           "CVBS Offset Calibration",  CLI_GUEST},
#endif	
#ifdef CC_MT5395
    {"VByOne Script","VB1Set", _VByOneScript,       NULL,           "V By One Script",  CLI_SUPERVISOR},
#endif

#if CLI_CVBS
  {"CvbsPGACheck","pga", _NptvPGACheck,       NULL,           "CVBS PGA Check",  CLI_SUPERVISOR},
  {"TargetBlankCheck","TBcheck", _NptvTargetBlankCheck,       NULL,           "Target Blank Check",  CLI_SUPERVISOR},
  {"InOutOffsetBlankCheck","IOOBcheck", _NptvInOutOffsetBlankCheck,       NULL,           "In Out Offset Blank Check",  CLI_SUPERVISOR},
  {"PeakLimitCheck","PKcheck", _NptvPeakLimitCheck,       NULL,           "Peak Y YC Bottom Check",  CLI_SUPERVISOR},
  {"SVFCheck","SVFcheck", _NptvSVFCheck,       NULL,           "S-video First Check",  CLI_SUPERVISOR},
#endif


#endif
  {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

#ifdef CC_CLI
/******************************************************************************
* Local Function
******************************************************************************/
extern UINT8 _bAutoColorState0;
static INT32 _NptvAutocolorCmd (INT32 i4Argc, const CHAR **szArgv)
{
   _bAutoColorState0 = VDO_AUTO_COLOR_START;
   Printf("VGA Autocolor start");
   return 0;
}

static INT32 _NptvAvCal (INT32 i4Argc, const CHAR **szArgv)
{
   vDrvTVADCDefaultSetting();
   vDrvCvbsAdcCal();
   Printf("CVBS Calibration done");
   return 0;
}

#if defined(CC_MT5365) || defined(CC_MT5395)
static INT32 _NptvCVBSOffset (INT32 i4Argc, const CHAR **szArgv)
{
   vDrvCVBSOffsetTest();
   Printf("CVBS input/output offset done");
   return 0;
}
#endif


#ifdef CC_MT5395
static INT32 _VByOneScript (INT32 i4Argc, const CHAR **szArgv)
{
  if(x_strcmp(szArgv[1], "0") ==0 )
  { 
    VByOneLane4_8bit_script(); 
    Printf("load 8 bits 4 lanes done");
  }
  else if(x_strcmp(szArgv[1], "1") ==0 )
  {
    VByOneLane4_10bit_script();
    Printf("load 10 bits 4 lanes done");
  }
  else
  {
    VByOneLane4_12bit_script();
    Printf("load 12 bits 4 lanes done");
  }
  
  return 0;
}

#endif

static INT32 _NptvSlicerIns (INT32 i4Argc, const CHAR **szArgv)
{
   vVFESLT_slicer_test_verC();
   Printf("Slicer Inspection  done");
   return 0;
}

static INT32 _NptvSlicerCheck (INT32 i4Argc, const CHAR **szArgv)
{
   vDrvSlicerCheck();
   Printf("Slicer Check done");
   return 0;
}


void vDrvSetVoSpreSTEP(UINT32 bVal);

static INT32 _NptvVopllSpreadCmd(INT32 i4Argc, const CHAR **szArgv)
{
    #if defined(CC_MT8223)
    UINT32 temp1=0;
    #else 
	UINT32 temp1=vDrvReadVoSpreSTEP();
    #endif
if(x_strcmp(szArgv[1], "+") ==0 )
  { 
    #if defined(CC_MT8223)
    #else 
    vDrvSetVoSpreSTEP(temp1+32);
    #endif
  }
    else if(((x_strcmp(szArgv[1], "-")== 0)&&(temp1>32)))
  {
    #if defined(CC_MT8223)
    #else
    vDrvSetVoSpreSTEP(temp1-32);
    #endif
  }
    else if(x_strcmp(szArgv[1], "-") == 0)
  {
    #if defined(CC_MT8223)
    #else
    vDrvSetVoSpreSTEP(temp1>>1);
    #endif
  }

    	return 0;
}

#if CLI_CVBS
static INT32 _NptvPGACheck (INT32 i4Argc, const CHAR **szArgv)
{
   vDrvCVBSPGACheck();
   Printf("CVBS PGA Check done");
   return 0;
}

static INT32 _NptvTargetBlankCheck (INT32 i4Argc, const CHAR **szArgv)
{
   vDrvTargetBlankCheck();
   Printf("Target Blank Check done");
   return 0;
}

static INT32 _NptvInOutOffsetBlankCheck (INT32 i4Argc, const CHAR **szArgv)
{
   vDrvIOOffsetBlankCheck();
   Printf("In Out Offset Blank Check done");
   return 0;
}

static INT32 _NptvPeakLimitCheck (INT32 i4Argc, const CHAR **szArgv)
{
   UINT8 PYC;

   if(x_strcmp(szArgv[1], "PY") ==0 )
      PYC = 1;
   else if(x_strcmp(szArgv[1], "PYC") ==0 )
      PYC = 2;
   else if(x_strcmp(szArgv[1], "BYC") ==0 )
      PYC = 3;
   else
      PYC = 0;

   Printf("i4Argc=%d,  PYC =%d\n",i4Argc, PYC);
   vDrvPeakLimitCheck(PYC);
   Printf("Peak Limit Check done");
   return 0;
}

static INT32 _NptvSVFCheck (INT32 i4Argc, const CHAR **szArgv)
{
   vDrvSVFTest();
   Printf("vDrvSVFTest done");
   return 0;
}

#endif




#endif /*#ifdef CC_CLI*/

