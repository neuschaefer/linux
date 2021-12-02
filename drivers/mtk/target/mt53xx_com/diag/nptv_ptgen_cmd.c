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
 * $RCSfile: nptv_dec_cmd.c,v $
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
#include "x_debug.h"

#include "drv_ycproc.h"
#include "drv_di.h"
#include "drv_scaler.h"
#include "drv_scpos.h"
#include "vdo_misc.h"

#if defined(CC_MT5365) || defined(CC_MT5395)
#include "drv_scaler_drvif.h"
#endif
#ifdef CC_MT5395
#include "drv_mjc.h"
#include "drv_mjc_if.h"
#endif

#ifndef CC_CLI
#define CC_CLI
#endif
#ifdef CC_CLI

/* Functions for Decoder */
static INT32 _u1vOmuxPatternGenOnOff(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u4AutoPatternGen(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u4DisableAllPatternGen(INT32 i4Argc, const CHAR **szArgv);
extern INT32 _iDiSetOutputPattern(INT32 i4Argc, const CHAR **szArgv);
extern INT32 _iDiSetInputBorder(INT32 i4Argc, const CHAR **szArgv);
extern INT32 _ScposSetDSPtnCmd(INT32 i4Argc, const CHAR **szArgv);
extern INT32 _ScposSetUSPtnCmd(INT32 i4Argc, const CHAR **szArgv);
extern INT32 _i4YcprocSharpnessPatGen(INT32 i4Argc, const CHAR **szArgv);
extern INT32 _i4YcprocSCEPatGen(INT32 i4Argc, const CHAR **szArgv);
#ifdef CC_MT5395
static INT32 _i4MjcPatGen(INT32 i4Argc, const CHAR **szArgv);
#endif
extern INT32 _PmxPatternCmd(INT32 i4Argc, const CHAR ** szArgv);
extern INT32 _PmxSetCmdOSTGPt(INT32 i4Argc, const CHAR ** szArgv);
extern INT32 _PreprocEnablePattern(INT32 i4Argc, const CHAR **szArgv);
extern INT32 _OsdRgnCmdPattern(INT32 i4Argc, const CHAR ** szArgv);

extern void vOmuxPatternGenOnOff(UINT8,UINT8);

EXTERN UINT8 SerPollGetChar(void);

#endif /*#ifdef CC_CLI*/
/******************************************************************************
* CLI Command Table
******************************************************************************/

// Table for Decoder command entry
CLI_EXEC_T arPatgenCmdTbl[] = {
#if 0
//OMUX output pattern gen : Adam
Pre-Processor input pattern gen: Skywalker
//PSCAN input border pattern gen: Peggy
//PSCAN output pattern gen with border : Peggy
//DS input pattern gen: Cosh
//US input pattern gen: Cosh
MLC input pattern gen: Menghu
2D Sharpness input pattern gen: Menghu
//Output stage input pattern gen: Cosh
OSD pattern gen: Allen
//Output stage output pattern gen: Cosh
#endif
#ifdef CC_CLI
  {"auto", "auto", _u4AutoPatternGen, NULL, "auto pattern from begin to end", CLI_GUEST},
  {"DisableAll", "da", _u4DisableAllPatternGen, NULL, "Diable all pattern gen", CLI_GUEST},
  {"omux", "o", _u1vOmuxPatternGenOnOff, NULL, "omux pattern gen on/off", CLI_GUEST},
  {"Pre-Process", "pre", _PreprocEnablePattern, NULL, "Preproc pattern  gen on/off", CLI_GUEST}, 
  {"DIInputBorder", "dibor", _iDiSetInputBorder, NULL, "De-interlace input border", CLI_GUEST},
  {"DIOutputPattern", "di", _iDiSetOutputPattern, NULL, "De-interlace output pattern", CLI_GUEST},
  {"Down Scaler Input", "ds", _ScposSetDSPtnCmd, NULL, "Down scaler input pattern gen on/off", CLI_GUEST},
  {"Up Scaler Input", "us", _ScposSetUSPtnCmd, NULL, "Up scaler input pattern gen on/off", CLI_GUEST},
  {"Sharpness", "sharp", _i4YcprocSharpnessPatGen, NULL, "2D Sharpness Pattern Gen on/off", CLI_GUEST},
  {"SCE", "sce", _i4YcprocSCEPatGen, NULL, "SCE(MLC input) Pattern Gen  on/off", CLI_GUEST},
  #ifdef CC_MT5395
  {"MJC", "mjc", _i4MjcPatGen, NULL, "MJC Pattern Gen  on/off", CLI_GUEST},
  #endif
  {"OSTG input", "ostgin", _PmxSetCmdOSTGPt, NULL, "Output stage input pattern gen on/off", CLI_GUEST},
  {"OSTG output", "ostgout", _PmxPatternCmd, NULL, "Output stage output pattern  gen on/off", CLI_GUEST},
  {"OSD", "osd", _OsdRgnCmdPattern, NULL, "OSD pattern gen", CLI_GUEST},  
#endif

  {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

#ifdef CC_CLI
/******************************************************************************
* Local Function
******************************************************************************/
static void vPtgenDisableAll(UINT8 u1Path)
{
    vOmuxPatternGenOnOff(u1Path, 0);
    vDrvPreprocEnablePattern(u1Path, 0);
    vDrvDISetPattern(u1Path, 0);
#if defined(CC_MT5387) || defined(CC_MT5363)  
    vDrvSetDSPattern(u1Path, VDP_DS_PTN_OFF, 0, 0, 0, 0);    
    vDrvSetUSPattern(u1Path, VDP_DS_PTN_OFF, 0, 0);  
#else
    vScpipSetDSPattern(u1Path, VDP_DS_PTN_OFF, 0, 0, 0, 0);    
    vScpipSetUSPattern(u1Path, VDP_DS_PTN_OFF, 0, 0);  
#endif
    vDrvMainSharpPatGen(0);
    vHalSetSCEPat(0,0);
    vDrvSetOSTGInPattern(u1Path, 0);
    vDrvSetOSTGOutPattern(0);
    #ifdef CC_MT5395
    vDrvMJCEnablePatGen(0, 0);
    vDrvMJCEnablePatGen(1, 0);
    #endif
}


static INT32 _u4DisableAllPatternGen(INT32 i4Argc, const CHAR **szArgv)
{
	if(i4Argc != 2)
	{
        	Printf("usage :%s  <main:0/ sub:1> \n", szArgv[0]);
        	return 0;
	}
	else
	{
	    vPtgenDisableAll((UINT8)StrToInt(szArgv[1]));
	}

	return 0;
}

typedef enum
{
    PTGEN_NONE,
    PTGEN_OMUX,  
    PTGEN_PRE_PROC, 
    PTGEN_PSCAN_OUT,    
    PTGEN_DOWN_SCALER,   
    PTGEN_UP_SCALER,  
    PTGEN_POST_SHARP,
    PTGEN_POST_SCE,
    PTGEN_OSTG_IN,
    #ifdef CC_MT5395
    PTGEN_MJC_IN,    
    PTGEN_MJC_OUT,
    #endif
    PTGEN_OSTG_OUT,
    //PTGEN_OD,  //not good for use
    PTGEN_MAX
} E_BYP_MODULE;

static CHAR _aszPtGenStr[PTGEN_MAX][20] = 
{
    "Disable All",
    "OMux",
    "PRE_PROC",
    "PSCAN_OUT",
    "DOWN_SCALER",   
    "UP_SCALER",  
    "POST_SHARP",
    "POST_SCE",
    "OSTG_IN",
    #ifdef CC_MT5395
    "MJC_IN",    
    "MJC_OUT",
    #endif
    "OSTG_OUT",
    //"OD,  //not good for use
};

static void _PrintPtgenItem(void)
{
            
    	     Printf("%x: Disable all pattern\n",PTGEN_NONE );
            Printf("%x: Omux\n", PTGEN_OMUX);
            Printf("%x: Pre-proc\n",PTGEN_PRE_PROC);
            Printf("%x: DI(PScan) Out\n",PTGEN_PSCAN_OUT);
            Printf("%x: Down Scaler\n",PTGEN_DOWN_SCALER);
            Printf("%x: Up Scaler\n",PTGEN_UP_SCALER);
            Printf("%x: Post: Sharpness\n",PTGEN_POST_SHARP);
            Printf("%x: Post: SEC(MLC)\n",PTGEN_POST_SCE);
            Printf("%x: Output Stage IN\n",PTGEN_OSTG_IN);
		#ifdef CC_MT5395
		Printf("%x: MJC input pattern \n",PTGEN_MJC_IN);
		Printf("%x: MJC output pattern \n",PTGEN_MJC_OUT);
		#endif
            Printf("%x: Output Stage OUT(LVDS)\n",PTGEN_OSTG_OUT);
	     Printf("Esc : quit auto pattern gen\n\n");
}

static INT32 _u4AutoPatternGen(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Char, u1Path, u1Mode, u1PtGenNo, u1OldPtGenNo, u1PtType;

    u1PtGenNo = (UINT8)szArgv[2][0];
    u1OldPtGenNo = 0xff;
    u1PtType = 0;
    
    if((i4Argc < 2) ||(i4Argc > 3) || ((i4Argc == 3)&&((u1PtGenNo<'0') || (u1PtGenNo>'9'))))
    {
    	Printf("Continuous mode usage :%s  <main:0/ sub:1> \n", szArgv[0] );
    	Printf("Single mode usage :%s  <main:0/ sub:1> <pattern No. 0~%x>\n", szArgv[0] ,PTGEN_MAX-1);
	_PrintPtgenItem();
    	return 0;
    }
    else
    {
        u1Path = StrToInt(szArgv[1]);
        u1Path = (u1Path ==1) ? 1 : 0;
        
        if(i4Argc ==3) // single pattern mode
        {
        }
        else // auto pattern mode
        {
		Printf("Please select pattern:\n");
		_PrintPtgenItem();
        }

        while(1)
        {
            if(i4Argc ==3)
            {
                u1Char = (UINT8)szArgv[2][0];
            }
            else
            {
                u1Char = SerPollGetChar();
            }
            
            if((u1Char>='0') && (u1Char<='9')) 
            {
            	  u1Char = u1Char-48;
            }
            else if((u1Char>='A')&&(u1Char<='Z'))  // can extend to 'Z'
            {
            	  u1Char = u1Char-55;
            }
	     else if((u1Char>='a') && (u1Char<='z'))// can extend to 'z'
            {
            	  u1Char = u1Char-87;
            }
	     else if(u1Char==27) // escape, end of auto pattern
	     {
	       Printf("Quit auto pattern gen \n");
	     	return 0;
	     }
	     else if((u1Char==13) || (u1Char==10)) // ncstool, enter
	     {
	     	continue;
	     }
	     else
	     {
		   _PrintPtgenItem();
		   continue;
	     }

	     if(u1Char <PTGEN_MAX)
	     {
		vPtgenDisableAll(u1Path);
		u1PtGenNo = u1Char;
		Printf("-------------- PATTEN (%x) %s -----------------\n",u1Char,  _aszPtGenStr[u1PtGenNo]);
	     	}
			
            switch(u1Char)
            {
                case PTGEN_NONE: //Dont' enable any pattern gen
                    Printf("Success: DISABLE all pattern gen of Path %d\n",u1Path);
                    break;
                    
                case PTGEN_OMUX:
                    if(u1OldPtGenNo != u1PtGenNo)
                    {
                        u1PtType = 3;  //default omux patgen type 
                    }
                    else
                    {
                        u1PtType =u1PtType%3 +1; //next omux patgen type
                    }
                    vOmuxPatternGenOnOff(u1Path, u1PtType);
                    Printf("Success: Omux pattern gen of Path %d\n",u1Path);
                    Printf("If Omux pattern can make problem disappear ==> DECODER\n");
                    break;
                    
                case PTGEN_PRE_PROC:
                    if(u1OldPtGenNo != u1PtGenNo)
                    {
                        u1PtType = 5;  //default pre-proc patgen type 
                    }
                    else
                    {
                        u1PtType =(u1PtType+1)%6; //next omux patgen type
                    }
                    vDrvPreprocEnablePattern(u1Path, u1PtType);
                    Printf("Success: Pre-Porcess pattern gen of Path %d\n",u1Path);
                    Printf("If Pre-Proc pattern can make problem disappear ==> DECODER\n");
                    break;
                    
                case PTGEN_PSCAN_OUT:
                    u1Mode = getScalerMode(u1Path);   
                    vDrvDISetPattern(u1Path, 1);

                    if(bDrvVideoIsSrcInterlace(u1Path) || (u1Mode == VDP_SCPOS_PSCAN_DISPMODE)) 
                    {
                        Printf("Success: PScan(DI) output pattern gen of Path %d\n",u1Path);
                        Printf("If DI output pattern is good but Pre-proc pattern is fail,\n");
                        printf("please bypass PScan or NR to check which of them is the correct owner.\n");
                    }
                    else
                    {
                        Printf("Warning: Please DON'T use PScan pattern when Scaler dram mode + progressive timing\n");
                    }
                    break;
                    
                case PTGEN_DOWN_SCALER:
                    u1Mode = getScalerMode(u1Path);   
                    if(u1Mode ==VDP_SCPOS_DISPMODE_OFF)
                    {
                    #if defined(CC_MT5387) || defined(CC_MT5363)  
                        vDrvSetDSPattern(u1Path, VDP_DS_PTN_AUTO, 0, 0, 0, 0);   
                    #else
                        vScpipSetDSPattern(u1Path, VDP_DS_PTN_AUTO, 0, 0, 0, 0);    
                    #endif
                        Printf("Success: Down Scaler pattern gen of Path %d\n",u1Path);
                        Printf("If Down-Scaler pattern is good but DI output or Pre-proc pattern is fail,\n");
                        printf("please bypass PScan or NR to check which of them is the correct owner.\n");
                    }
                    else
                    {
                        Printf("Warning: Please DON'T use Down Scaler pattern when Display mode\n");
                    }
                    break;
                    
                case PTGEN_UP_SCALER:
                #if defined(CC_MT5387) || defined(CC_MT5363)  
                    vDrvSetUSPattern(u1Path, VDP_DS_PTN_AUTO, 0, 0);  
                #else
                    vScpipSetUSPattern(u1Path, VDP_DS_PTN_AUTO, 0, 0);  
                #endif
                    Printf("Success: Up Scaler pattern gen of Path %d\n",u1Path);
                    Printf("If Up-Scaler pattern is good but DI output or Down scaler pattern is fail ==> Scaler\n");
                    break;
                    
                case PTGEN_POST_SHARP:
                    if(u1Path ==0) //main only
                    {
                        vDrvMainSharpPatGen(1);
                        Printf("Success: Sharpness pattern gen (Only in main path)\n");
                        Printf("If Sharpness pattern is good but Up-Scaler pattern is fail  ==>  Scaler\n");
                    }
                    else
                    {
                        Printf("Warning : \"No\" Sharpness pattern gen in sub path\n");
                    }
                    break;
                    
                case PTGEN_POST_SCE:
                    if(u1Path ==0) //main only
                    {
                        vHalSetSCEPat(1,512);
                        Printf("Success: SCE(MLC) pattern gen (Only in main path)\n");
                        Printf("If SCE pattern is good but Sharpness pattern is fail ==> Post-Proc(PE1)\n");
                    }
                    else
                    {
                        Printf("Warning : \"No\" SCE(MLC) pattern gen in sub path\n");
                    }
                    break;
                    
                case PTGEN_OSTG_IN:
                    vDrvSetOSTGInPattern(u1Path, 1);
                    Printf("Success: Output Stage Input pattern gen of Path %d\n",u1Path);
                    Printf("If Output stage Input pattern is good but Sharpness or SCE or Up-scaler pattern is fail ==>Post-Proc(PE1)\n");
                    break;

                #ifdef CC_MT5395
                case PTGEN_MJC_IN:
                	if(u1Path ==0) //main only
                	{
	                	if (u1DrvMJCIsOnOff() == SV_FALSE)
				{
			        	Printf("Warning :MJC is bypassed. Can't display pattern!\n");
				}
	                	else
	                	{
	                		Printf("Success: MJC Input pattern gen \n");
			       	vDrvMJCEnablePatGen(0, 1);
	                	}
                	}
                	else
                	{
                		Printf("Warning : \"No\" MJC pattern gen in sub path\n");
                	}
                	break;

                case PTGEN_MJC_OUT:
                	if(u1Path ==0) //main only
                	{
	                	if (u1DrvMJCIsOnOff() == SV_FALSE)
				{
			        	Printf("Warning :MJC is bypassed. Can't display pattern!\n");
				}
	                	else
	                	{
	                		Printf("Success: MJC Output pattern gen\n");
			       	vDrvMJCEnablePatGen(1, 1);
	                	}
                	}
                	else
                	{
                		Printf("Warning : \"No\" MJC pattern gen in sub path\n");
                	}

                	break;
                #endif
                    
                 case PTGEN_OSTG_OUT:
                    vDrvSetOSTGOutPattern(1);
                    Printf("Success: Output Stage Output pattern gen of Path %d\n",u1Path);
                    Printf("If Output stage Output pattern is good but Output stage Input pattern is fail ==> Output Stage(PE1)\n");
                    break;

                default:
                    _PrintPtgenItem();
                    break;
            }

            if(i4Argc ==3) // single pattern mode
                return 0;
            else
                u1OldPtGenNo = u1PtGenNo;
        }
    }
}

#ifdef CC_MT5395
static INT32 _i4MjcPatGen(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 3)
	{
        printf("Usage: %s <input:0/output:1> <on:1/off:0>\n", szArgv[0]);
        return 1;
	}

	if (u1DrvMJCIsOnOff() == SV_FALSE)
	{
        printf("MJC is bypassed. Can't display pattern!\n");
        return 1;
	}

    vDrvMJCEnablePatGen(StrToInt(szArgv[1]), StrToInt(szArgv[2]));
    return 0;
}
#endif

static INT32 _u1vOmuxPatternGenOnOff(INT32 i4Argc, const CHAR **szArgv)
{
	if(i4Argc != 3)
	{
		Printf("usage :%s  <main:0/ sub:1> <0:off/ramp:1/full screen:2/all:3>\n", szArgv[0] );
	}
	else
	{
	    vOmuxPatternGenOnOff((UINT16)StrToInt(szArgv[1]),(UINT16)StrToInt(szArgv[2]));
	}

	return 0;
}

#endif //#ifdef CC_CLI

