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
 * $RCSfile: nptv_di_cmd.c,v $
 * $Revision: #1 $
 *
 * Description:
 *         This file contains CLI implementation of NPTV Video.
 *---------------------------------------------------------------------------*/
#include "x_os.h"
#include "x_stl_lib.h"

#ifdef __MW_CLI_DEF__
#include "x_mid_cli.h"
#else
#include "x_drv_cli.h"
#endif

#include "x_assert.h"
#include "x_debug.h"
#include "x_mid.h"
#include "source_table.h"
#include "sv_const.h"
#include "video_def.h"

#include "drv_tdtv_drvif.h"

#ifdef CC_MT5396
#include "drv_mjc.h"
#include "drv_tdnavi.h"
#endif

#define UINT_CLIP(val, min, max) ((UINT32)((val>=max) ? max : ((val<=min) ? min : val)))  

static INT32 _TdtvSetLRSwitch(INT32 i4Argc, const CHAR **szArgv);
static INT32 _TdtvTv3dPack(INT32 i4Argc, const CHAR **szArgv);
static INT32 _TdtvTv3d22D(INT32 i4Argc, const CHAR **szArgv);
static INT32 _TdtvFPR(INT32 i4Argc, const CHAR **szArgv);
static INT32 _TdtvQuary(INT32 i4Argc, const CHAR **szArgv);
static INT32 _TdtvSet3DNaviInk(INT32 i4Argc, const CHAR **szArgv);
static INT32 _TdtvSet3DNaviInFwPara(INT32 i4Argc, const CHAR **szArgv);
static INT32 _TdtvSet3DNaviOutFwPara(INT32 i4Argc, const CHAR **szArgv);
static INT32 _TdtvSet3DNaviAdaptOnOff(INT32 i4Argc, const CHAR **szArgv);
static INT32 _TdtvSetRealDInFrmThr(INT32 i4Argc, const CHAR **szArgv);
static INT32 _TdtvSetRealDOutFrmThr(INT32 i4Argc, const CHAR **szArgv);
static INT32 _TdtvSetRealDInQuaThrRatio(INT32 i4Argc, const CHAR **szArgv);
static INT32 _TdtvSetRealDOutQuaThrRatio(INT32 i4Argc, const CHAR **szArgv);
static INT32 _TdtvSetRealDAdaptOnOff(INT32 i4Argc, const CHAR **szArgv);
static INT32 _TdtvSetRealDStatusPrint(INT32 i4Argc, const CHAR **szArgv);
static INT32 _TdtvRealDQuery(INT32 i4Argc, const CHAR **szArgv);
static INT32 _TdtvSet3D22DMode(INT32 i4Argc, const CHAR **szArgv);
static INT32 _TdtvGet3D22DMode(INT32 i4Argc, const CHAR **szArgv);

static INT32 _TdtvSet3DNaviAtuo(INT32 i4Argc, const CHAR **szArgv);

/******************************************************************************
* CLI Command Table
******************************************************************************/

// Table for DI command entry
CLI_EXEC_T arTDTVCmdTbl[] = {
    {"pk"           ,"pk"         ,_TdtvTv3dPack          ,NULL, "3D packing Mode"           , CLI_SUPERVISOR},
    {"322"          ,"322"        ,_TdtvTv3d22D           ,NULL, "3D to 2D"                  , CLI_SUPERVISOR},
    {"FPR Mode"     ,"fpr"        ,_TdtvFPR               ,NULL, "FPR Mode"                  , CLI_SUPERVISOR},       
    {"Config Query" ,"q"          ,_TdtvQuary             ,NULL, "Config Query"              , CLI_SUPERVISOR},    
    {"navi"         ,"navi"       ,_TdtvSet3DNaviAtuo     ,NULL, "3D Navigation"             , CLI_SUPERVISOR},    
    {"L/R switch"   ,"lrs"        ,_TdtvSetLRSwitch       ,NULL, "Set L/R Indicator switch"  , CLI_GUEST},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

// Table for PreProc 3D command entry
CLI_EXEC_T arPre3DCmdTbl[] = {
    {"3D Navi Ink"     , "tdnink", _TdtvSet3DNaviInk    , NULL, "Set 3D Navi Ink Mode"    , CLI_GUEST},
    {"3D Navi In FW Para"  , "tdninfw" , _TdtvSet3DNaviInFwPara  , NULL, "Set 3D Navi In FW Parameter" , CLI_GUEST},
    {"3D Navi Out FW Para" , "tdnoutfw", _TdtvSet3DNaviOutFwPara , NULL, "Set 3D Navi Out FW Parameter", CLI_GUEST},
    {"3D Navi Adapt On/Off", "tdnfw", _TdtvSet3DNaviAdaptOnOff , NULL, "Set 3D Navi Adapt On/Off", CLI_GUEST},
    {"RealD In FrmThr"     , "rdifc", _TdtvSetRealDInFrmThr   , NULL, "Set RealD In FrmThr"  , CLI_GUEST},
    {"RealD Out FrmThr"    , "rdofc", _TdtvSetRealDOutFrmThr  , NULL, "Set RealD Out FrmThr" , CLI_GUEST},
    {"RealD In QuaRatio"   , "rdiqr", _TdtvSetRealDInQuaThrRatio   , NULL, "Set RealD In QuaRatio"    , CLI_GUEST},
    {"RealD Out QuaRatio"  , "rdoqr", _TdtvSetRealDOutQuaThrRatio  , NULL, "Set RealD Out QuaRatio"   , CLI_GUEST},
    {"RealD Adapt On/Off", "rdfw" , _TdtvSetRealDAdaptOnOff , NULL, "Set RealD Adapt On/Off" , CLI_GUEST},
    {"RealD Status Print", "rdsta", _TdtvSetRealDStatusPrint, NULL, "Set RealD Status Print" , CLI_GUEST},
    {"RealD Query"       , "rdq"  , _TdtvRealDQuery         , NULL, "Set RealD Query"        , CLI_GUEST},
    {"3D-to-2D Mode"     , "3d22ddm" , _TdtvSet3D22DMode    , NULL, "Set 3D-to-2D Mode"       , CLI_GUEST},
    {"3D-to-2D Query"    , "3d22dq"  , _TdtvGet3D22DMode    , NULL, "Get 3D-to-2D Mode"       , CLI_GUEST},
    //CLIMOD_DEBUG_CLIENTRY(MJC),
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

/******************************************************************************
* Local Function
******************************************************************************/

static INT32 _TdtvTv3dPack(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Pack;
    if (i4Argc < 2)
    {
        Printf("Set Packing Mode\n");
        Printf("E_TDTV_UI_3D_MODE_OFF  =0\n");
        Printf("E_TDTV_UI_3D_MODE_AUTO =1\n");
        Printf("E_TDTV_UI_3D_MODE_TTD  =2\n");
        Printf("E_TDTV_UI_3D_MODE_FS   =3\n");
        Printf("E_TDTV_UI_3D_MODE_SBS  =4\n");         
        Printf("E_TDTV_UI_3D_MODE_TB   =5\n");
        Printf("E_TDTV_UI_3D_MODE_RD   =6\n");
        Printf("E_TDTV_UI_3D_MODE_SS   =7\n"); 
        Printf("E_TDTV_UI_3D_MODE_LI   =8\n");
        Printf("E_TDTV_UI_3D_MODE_DA   =9\n");         
        Printf("E_TDTV_UI_3D_MODE_CB   =10\n");
        return 0;
    }
    u4Pack=StrToInt(szArgv[1]);
    vDrvTDTVPackingDbg((E_TDTV_UI_3D_PACKING)u4Pack);

    return 0;
}

static INT32 _TdtvTv3d22D(INT32 i4Argc, const CHAR **szArgv)
{
    
    UINT32 u4322;
    if (i4Argc < 2)
    {
        Printf("E_TDTV_UI_3D_2_2D_OFF  =0\n");
        Printf("E_TDTV_UI_3D_2_2D_ON   =1\n");
        return 0;
    }
    u4322=StrToInt(szArgv[1]);
    vDrvTDTV3D22Ddbg((E_TDTV_UI_3D_2_2D)u4322);
    return 0;
}

static INT32 _TdtvFPR(INT32 i4Argc, const CHAR **szArgv)
{    
    UINT32 utFPR;
    if (i4Argc < 2)
    {
        Printf("E_TDTV_UI_3D_FPR_OFF  =0");
        Printf("E_TDTV_UI_3D_FPR_V1   =1");
        Printf("E_TDTV_UI_3D_FPR_V2   =2");        
        return 0;
    }
    utFPR=StrToInt(szArgv[1]);    
    vDrvTDTV3DFPRdbg((E_TDTV_UI_3D_FPR)utFPR);
    return 0;
}

static INT32 _TdtvSetLRSwitch(INT32 i4Argc, const CHAR **szArgv)
{
	return 0;
}

static INT32 _TdtvQuary(INT32 i4Argc, const CHAR **szArgv)
{
    vDrvTDTVQueryConfig();
    return 0;
}

static INT32 _TdtvSet3DNaviInk(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Count;

	if (i4Argc != 2)
	{
		Printf("Usage: %s <0: Off, 1: FW Ink 2: Navi Result>\n", szArgv[0]);
		return 1;
	}

    u4Count = StrToInt(szArgv[1]);
    vDrvTdNaviSetDbgType(u4Count);

	return 0;
}

static INT32 _TdtvSet3DNaviInFwPara(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 u1Idx;
    UINT32 u4Para[4];

    u4Para[0] = g_u1TdNaviDetInFrmNum;
    u4Para[1] = g_u1In3DSimilarityRatio;
    u4Para[2] = g_u1In3DSimilarThr;
    u4Para[3] = g_u1In3DDecisionType;

	if (i4Argc != 5)
	{
		Printf("Usage  : %s Para[0] Para[1] Para[2] Para[3]\n", szArgv[0]);
		Printf("Current: %s %d %d %d %d\n", szArgv[0], u4Para[0], u4Para[1], u4Para[2], u4Para[3]);
		Printf("Para[0] = 0~255 : 3D Navi In Frame Number\n");
		Printf("Para[1] = 0~255 : 3D Navi In Similarity Ratio\n");
		Printf("Para[2] = 0~31  : 3D Navi In Similar Threshold\n");
		Printf("Para[3] = 0~3   : 3D Navi In Decision Type\n");
		return 1;
	}

	for (u1Idx = 0; u1Idx < 4; u1Idx++)
	{
		u4Para[u1Idx] = StrToInt(szArgv[u1Idx+1]);
	}
	
    g_u1TdNaviDetInFrmNum  = (UINT8)(UINT_CLIP(u4Para[0], 0, 255));
    g_u1In3DSimilarityRatio  = (UINT8)(UINT_CLIP(u4Para[1], 0, 255));
    g_u1In3DSimilarThr  = (UINT8)(UINT_CLIP(u4Para[2], 0, 31));
    g_u1In3DDecisionType  = (UINT8)(UINT_CLIP(u4Para[3], 0, 3));

	return 0;
}


static INT32 _TdtvSet3DNaviOutFwPara(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 u1Idx;
    UINT32 u4Para[4];

    u4Para[0] = g_u1TdNaviDetOutFrmNum;
    u4Para[1] = g_u1Out3DSimilarityRatio;
    u4Para[2] = g_u1Out3DSimilarThr;
    u4Para[3] = g_u1Out3DDecisionType;

	if (i4Argc != 5)
	{
		Printf("Usage  : %s Para[0] Para[1] Para[2] Para[3]\n", szArgv[0]);
		Printf("Current: %s %d %d %d %d\n", szArgv[0], u4Para[0], u4Para[1], u4Para[2], u4Para[3]);
		Printf("Para[0] = 0~255 : 3D Navi Out Frame Number\n");
		Printf("Para[1] = 0~255 : 3D Navi Out Similarity Ratio\n");
		Printf("Para[2] = 0~31  : 3D Navi Out Similar Threshold\n");
		Printf("Para[3] = 0~3   : 3D Navi Out Decision Type\n");
		return 1;
	}

	for (u1Idx = 0; u1Idx < 4; u1Idx++)
	{
		u4Para[u1Idx] = StrToInt(szArgv[u1Idx+1]);
	}
	
    g_u1TdNaviDetOutFrmNum  = (UINT8)(UINT_CLIP(u4Para[0], 0, 255));
    g_u1Out3DSimilarityRatio  = (UINT8)(UINT_CLIP(u4Para[1], 0, 255));
    g_u1Out3DSimilarThr  = (UINT8)(UINT_CLIP(u4Para[2], 0, 31));
    g_u1Out3DDecisionType  = (UINT8)(UINT_CLIP(u4Para[3], 0, 3));

	return 0;
}

static INT32 _TdtvSet3DNaviAdaptOnOff(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 u1OnOff;
	
    if (i4Argc != 2)
    {
        Printf("Usage: %s u1OnOff\n", szArgv[0]);
		Printf("u1OnOff (0: Off, 1: On)\n");
        return 1;
    }

    u1OnOff  = (UINT8)(UINT_CLIP(StrToInt(szArgv[1]), 0, 1));

    g_u1TdNaviAdaptOnOff = u1OnOff;
    
    return 0;
}

static INT32 _TdtvSetRealDInFrmThr(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 u1VdpId, u1FrmCnt;

	if (i4Argc != 3)
	{
		Printf("Usage  : %s u1VdpId u1FrmCnt\n", szArgv[0]);
		Printf("u1VdpId (0: Main, 1: Pip)\n");
		Printf("u1FrmCnt(0~255)\n");
		return 1;
	}

    u1VdpId  = (UINT8)(UINT_CLIP(StrToInt(szArgv[1]), 0, 1));
    u1FrmCnt = (UINT8)(UINT_CLIP(StrToInt(szArgv[2]), 0, 255));


	if (u1VdpId < SV_VP_NA)
	{
    	g_u1RealDInFrmThr[u1VdpId] = u1FrmCnt;
	}

	return 0;
}


static INT32 _TdtvSetRealDOutFrmThr(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 u1VdpId, u1FrmCnt;

	if (i4Argc != 3)
	{
		Printf("Usage  : %s u1VdpId u1FrmCnt\n", szArgv[0]);
		Printf("u1VdpId (0: Main, 1: Pip)\n");
		Printf("u1FrmCnt(0~255)\n");
		return 1;
	}

    u1VdpId  = (UINT8)(UINT_CLIP(StrToInt(szArgv[1]), 0, 1));
    u1FrmCnt = (UINT8)(UINT_CLIP(StrToInt(szArgv[2]), 0, 255));

	if (u1VdpId < SV_VP_NA)
	{
    	g_u1RealDOutFrmThr[u1VdpId] = u1FrmCnt;
	}
	
	return 0;
}

static INT32 _TdtvSetRealDInQuaThrRatio(INT32 i4Argc, const CHAR **szArgv)
{
	UINT16 u2QuaThrRatio;

	if (i4Argc != 2)
	{
		Printf("Usage  : %s u2QuaThrRatio\n", szArgv[0]);
		Printf("u2QuaThrRatio (0~256)\n");
		return 1;
	}

    u2QuaThrRatio  = (UINT16)(UINT_CLIP(StrToInt(szArgv[1]), 0, 256));

    g_u2RealDInQuaThrRatio = u2QuaThrRatio;

	return 0;
}


static INT32 _TdtvSetRealDOutQuaThrRatio(INT32 i4Argc, const CHAR **szArgv)
{
	UINT16 u2QuaThrRatio;

	if (i4Argc != 2)
	{
		Printf("Usage  : %s u2QuaThrRatio\n", szArgv[0]);
		Printf("u2QuaThrRatio (0~256)\n");
		return 1;
	}

    u2QuaThrRatio  = (UINT16)(UINT_CLIP(StrToInt(szArgv[1]), 0, 256));

    g_u2RealDOutQuaThrRatio = u2QuaThrRatio;

	return 0;
}




static INT32 _TdtvSetRealDAdaptOnOff(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 u1VdpId, u1OnOff;
	
    if (i4Argc != 3)
    {
        Printf("Usage: %s u1VdpId u1OnOff\n", szArgv[0]);
		Printf("u1VdpId (0: Main, 1: Pip)\n");
		Printf("u1OnOff (0: Off, 1: On)\n");
        return 1;
    }

    u1VdpId  = (UINT8)(UINT_CLIP(StrToInt(szArgv[1]), 0, 1));
    u1OnOff  = (UINT8)(UINT_CLIP(StrToInt(szArgv[2]), 0, 1));

	if (u1VdpId < SV_VP_NA)
	{
	    g_u1RealDAdaptEn[u1VdpId] = u1OnOff;
	}
    
    return 0;
}

static INT32 _TdtvRealDQuery(INT32 i4Argc, const CHAR **szArgv)
{
    u2DrvRealDQuery();
    return 0;
}


static INT32 _TdtvSetRealDStatusPrint(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8  u1OnOff;
	UINT16 u2PrintNum;
	
    if (i4Argc != 3)
    {
        Printf("Usage: %s u1OnOff u2PrintNum\n", szArgv[0]);
		Printf("u1OnOff (0: Off, 1: On)\n");
		Printf("u2PrintNum (0~65535)\n");
        return 1;
    }

    u1OnOff  = (UINT8)(UINT_CLIP(StrToInt(szArgv[1]), 0, 1));
    u2PrintNum  = (UINT16)(UINT_CLIP(StrToInt(szArgv[2]), 0, 65535));

    g_u1RealDTrigger = u1OnOff;
    g_u2RealDPrint = u2PrintNum;

    return 0;
}


static INT32 _TdtvSet3D22DMode(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 u1VdpId, u1TD22DEn, u1ModeSel, u1MaskSel;

	if (i4Argc != 5)
	{
		Printf("Usage  : %s u1VdpId OnOff ModeSel MaskSel\n", szArgv[0]);
		Printf("u1VdpId (0: Main, 1: Pip)\n");
		Printf("OnOff   (0: On, 1: Off)\n");
		Printf("ModeSel (0: FS, 1: SBS, 2: TB, 3: LI)\n");
		Printf("MaskSel (0: Left/Top/Even, 1: Right/Bottom/Odd)\n");
		return 1;
	}

    u1VdpId   = (UINT8)(UINT_CLIP(StrToInt(szArgv[1]), 0, 1));
    u1TD22DEn = (UINT8)(UINT_CLIP(StrToInt(szArgv[2]), 0, 1));
    u1ModeSel = (UINT8)(UINT_CLIP(StrToInt(szArgv[3]), 0, 3));
    u1MaskSel = (UINT8)(UINT_CLIP(StrToInt(szArgv[4]), 0, 1));

    u1DrvTD22DSetMode(u1VdpId, u1TD22DEn, u1ModeSel, u1MaskSel);

	return 0;
}

static INT32 _TdtvGet3D22DMode(INT32 i4Argc, const CHAR **szArgv)
{
	Printf("---- Main Channel ----\n");
	Printf("Input Width  = %d\n", gMain3DTo2DInfo.u2Width);
	Printf("Input Height = %d\n", gMain3DTo2DInfo.u2Height);
	Printf("3D22D Enable = %d\n", gMain3DTo2DInfo.u1TD22DEn);
	Printf("Mode Select  = %d\n", gMain3DTo2DInfo.u1ModeSel);
	Printf("Mask select  = %d\n", gMain3DTo2DInfo.u1MaskSel);
	Printf("Is Interlace = %d\n", gMain3DTo2DInfo.u1IsIntr);

	Printf("---- PIP Channel ----\n");
	Printf("Input Width  = %d\n", gPip3DTo2DInfo.u2Width);
	Printf("Input Height = %d\n", gPip3DTo2DInfo.u2Height);
	Printf("3D22D Enable = %d\n", gPip3DTo2DInfo.u1TD22DEn);
	Printf("Mode Select  = %d\n", gPip3DTo2DInfo.u1ModeSel);
	Printf("Mask select  = %d\n", gPip3DTo2DInfo.u1MaskSel);
	Printf("Is Interlace = %d\n", gPip3DTo2DInfo.u1IsIntr);

	return 0;
}


static INT32 _TdtvSet3DNaviAtuo(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4OnOff;

	if (i4Argc != 2)
	{
		Printf("Usage: %s <0: MANUAL, 1: SEMI-AUTO , 2L FULL_AUTO\n", szArgv[0]);
		return 1;
	}

    u4OnOff = StrToInt(szArgv[1]);
    vDrvTDTVTDNAVI((E_TDTV_UI_NAVI)u4OnOff);
	return 0;
}
