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
#include "drv_scaler.h"
#include "sv_const.h"
#include "osd_drvif.h"
#include "vdo_misc.h"

#ifdef __MW_CLI_DEF__
#include "x_mid_cli.h"
#else
#include "x_drv_cli.h"
#endif
#include "drv_nr.h"
#include "drv_ycproc.h"

#include "x_assert.h"
#include "x_debug.h"

#if defined(CC_MT5365) || defined(CC_MT5395)
#include "drv_scaler_drvif.h"
#endif

#if defined(CC_MT5365) || defined(CC_MT5395)
#include "drv_di.h"
#else
EXTERN void vDrvDIOnOff(UINT8 u1VdpId, UINT8 bOnOff);
#endif

#if defined(CC_MT5395)
#include "drv_mjc_if.h"
#endif

#if defined(CC_MT5365) || defined(CC_MT5395)
#include "drv_od.h"
#endif


#ifndef CC_CLI
#define CC_CLI
#endif
#ifdef CC_CLI

EXTERN UINT8 SerPollGetChar(void);

extern INT32 _VdpSetForcedHPSD(INT32 i4Argc, const CHAR ** szArgv);
extern INT32 _i4DiOnOff(INT32 i4Argc, const CHAR **szArgv);
extern INT32 _VdpSetCmd121(INT32 i4Argc, const CHAR ** szArgv);
extern INT32 _NRSetNRForceBypass(INT32 i4Argc, const CHAR **szArgv);
extern INT32 _i4YcprocPre(INT32 i4Argc, const CHAR ** szArgv);
extern INT32 _i4YcprocPost(INT32 i4Argc, const CHAR ** szArgv);
extern INT32 _i4YcprocOS(INT32 i4Argc, const CHAR ** szArgv);
extern INT32 _i4YcprocPQ(INT32 i4Argc, const CHAR ** szArgv);
extern INT32 _OsdPlaneCmdEnable(INT32 i4Argc, const CHAR ** szArgv);

static INT32 _u4VideoBypassAuto(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u4DisableAllBypass(INT32 i4Argc, const CHAR **szArgv);

#endif /*#ifdef CC_CLI*/
/******************************************************************************
* CLI Command Table
******************************************************************************/

// Table for Decoder command entry
CLI_EXEC_T arBypassCmdTbl[] = {
#ifdef CC_CLI
  {"auto", "auto", _u4VideoBypassAuto, NULL, "video bypass auto command", CLI_GUEST},
  {"DisableAll", "da", _u4DisableAllBypass, NULL, "Diable all bypass (Don't bypass)", CLI_GUEST},
  {"predown", "pdown", _VdpSetForcedHPSD, NULL, "bypass pre-scaling down", CLI_GUEST},
  {"NR", "nr", _NRSetNRForceBypass, NULL, "Enable/disable NR", CLI_GUEST},  
  {"DI", "di", _i4DiOnOff, NULL, "bypass De-interlace", CLI_GUEST},
  {"Scaler", "scl", _VdpSetCmd121, NULL, "bypass Scaler(121 Mapping)", CLI_GUEST},
  {"PreProc", "pre", _i4YcprocPre, NULL, "Enable/Disable Pre-Proc", CLI_GUEST},
  {"PostProc", "post", _i4YcprocPost, NULL, "Enable/Disable Post-Proc", CLI_GUEST},
  {"OutputStage", "os", _i4YcprocOS, NULL, "Enable/Disable Output Stage", CLI_GUEST},
  {"PQ", "pq", _i4YcprocPQ, NULL, "Turn On/Off All PQ module", CLI_GUEST},
  {"OSD", "osd", _OsdPlaneCmdEnable, NULL, "Disable/Enable OSD plane", CLI_SUPERVISOR},
#endif
  {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

typedef enum
{
    BYP_NONE,
    BYP_PRE_DOWN,  
    BYP_PRE_PROC, 
    BYP_NR, 
    BYP_PSCAN,    
    BYP_SCALER,    
    BYP_POST_PROC,
    BYP_OSTG,
    BYP_OSD,
    BYP_OD,
    BYP_MJC,    
    BYP_MAX
} E_BYP_MODULE;

#ifdef CC_CLI
/******************************************************************************
* Local Function
******************************************************************************/
static void vDisableAllBypass(UINT8 u1Path, UINT8 bBypID)
{
    #if 0//defined(CC_MT5365) || defined(CC_MT5395)
    #else
    UINT8 u1Mode;
    #endif
    
    if(bBypID !=BYP_PRE_DOWN)
    {
   	 _VDP_SetHorizontalPreScaleDownFactor(u1Path, SV_OFF);
    }
    
    //pre-proc
    if(bBypID !=BYP_PRE_PROC)
    {
	    vDrvAntiAliasingFilterOnOff(u1Path , SV_ON);
	    vDrvMatrixCtrlSel(u1Path , SV_ON ? 0 : 4);
	    vDrvChromaCoringOnOff(u1Path , SV_ON);
    }
    // end of pre-proc

    if(bBypID !=BYP_NR)
    {
    	vDrvNRSetForceBypass(SV_OFF);
    }

    if(bBypID !=BYP_PSCAN)
    {
    	#if 0//defined(CC_MT5365) || defined(CC_MT5395)
    	vDrvDIRecoverOnOff(u1Path); ///==> new api of 65 and 95
    	vDrvDISetDebugDataMode(u1Path, E_DI_UNKNOWN);
    	#else
    	u1Mode = getScalerMode(u1Path);   
	if((u1Mode ==VDP_SCPOS_DISPMODE_OFF) && (bDrvVideoIsSrcInterlace(u1Path)==SV_FALSE))
	{ //480p, 576p, 1080p, scaler dram mode
	   // Keep bypass PScan 
	}
	else
	{
	    vDrvDIOnOff(u1Path, SV_ON);
	}
    	#endif
    }

    if(bBypID !=BYP_SCALER)
    {
    	VDP_Set121Map(u1Path, SV_OFF);
    }

    //post-proc
    if(bBypID !=BYP_POST_PROC)
    {
	    vHalVideoSharpnessOnOff(SV_VP_MAIN , SV_ON);
	    vHalVideoLTIOnOff(SV_VP_MAIN , SV_ON);
	    vHalVideoCTIOnOff(SV_VP_MAIN , SV_ON);
	    vDrvSCEAllOnOff(SV_ON);
    }
    //end of post-proc

    //output stage
    if(bBypID !=BYP_OSTG)
    {
	    vDrvOSMatrixOnOff(u1Path , SV_ON);
	    //vDrvGammaOnOff(SV_ON);  // need to add back
    }
    #if 0
    if (u1OnOff == SV_OFF)
    {
        vHalVideoRGBGain(SV_VP_MAIN, SV_RGB_R, 512);
        vHalVideoRGBGain(SV_VP_MAIN, SV_RGB_G, 512);
        vHalVideoRGBGain(SV_VP_MAIN, SV_RGB_B, 512);
        vHalVideoRGBOffset(SV_VP_MAIN, SV_RGB_R, 0);
        vHalVideoRGBOffset(SV_VP_MAIN, SV_RGB_G, 0);
        vHalVideoRGBOffset(SV_VP_MAIN, SV_RGB_B, 0);
    }
    #endif
    // end of output stage

    if(bBypID !=BYP_OSD)
    {
	    OSD_PLA_SetWorkaround(0, 1);
	    OSD_PLA_Enable(0, 1);
	    OSD_PLA_SetWorkaround(1, 1);
	    OSD_PLA_Enable(1, 1);
	    OSD_PLA_SetWorkaround(0, 1);
	    OSD_PLA_Enable(1, 1);
    }
    
    #ifdef CC_MT5395
    if(bBypID !=BYP_MJC)
    {
	    if(u1Path == SV_VP_MAIN)
	    {
	    	MJC_SetOnOff(E_MJC_ON);
	    }
    }
    #endif

    #if defined(CC_MT5365) || defined(CC_MT5395)
    if(bBypID !=BYP_OD)
    {
    	vDrvODBypass(0);
    }
    #endif

}


static INT32 _u4DisableAllBypass(INT32 i4Argc, const CHAR **szArgv)
{
	if(i4Argc != 2)
	{
        	Printf("usage :%s  <main:0/ sub:1> \n", szArgv[0]);
        	return 0;
	}
	else
	{
	    vDisableAllBypass((UINT8)StrToInt(szArgv[1]), BYP_NONE);
	}

	return 0;
}

static void _PrintBypassItem(void)
    {
        Printf("%x: No bypass\n", BYP_NONE);
        Printf("%x: Bypass Pre-Scaling down\n",BYP_PRE_DOWN);
        Printf("%x: Bypass Pre-proc\n",BYP_PRE_PROC);
        Printf("%x: Bypass NR\n",BYP_NR);
        Printf("%x: Bypass DI(PScan)\n", BYP_PSCAN);
        Printf("%x: Bypass Scaler (dot by dot)\n",BYP_SCALER);
        Printf("%x: Bypass Post-Proc\n", BYP_POST_PROC);
        Printf("%x: Bypass Output Stage\n", BYP_OSTG);
        Printf("%x: Bypass OSD\n",BYP_OSD);
        #if defined(CC_MT5365) || defined(CC_MT5395)
        Printf("%x: Bypass OD\n", BYP_OD);
        #endif
        #ifdef CC_MT5395
        Printf("%x: Bypass MJC\n", BYP_MJC);
        #endif
}

static INT32 _u4VideoBypassAuto(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Char, u1Path, u1Mode, u1BypassNo;
    
    u1BypassNo = (UINT8)szArgv[2][0];

    if((i4Argc < 2) ||(i4Argc > 3) || ((i4Argc == 3)&&(((u1BypassNo<'0') || (u1BypassNo>'9')) && (u1BypassNo !='a') && (u1BypassNo!='A'))))
    {
    	Printf("Continuous mode usage :%s  <main:0/ sub:1> \n", szArgv[0] );
    	Printf("Single mode usage :%s  <main:0/ sub:1> <bypass No. 0~%x>\n", szArgv[0], BYP_MAX-1);
    	_PrintBypassItem();
    	return 0;
    }
    else
    {
        u1Path = StrToInt(szArgv[1]);
        u1Path = (u1Path ==1) ? 1 : 0;
        
        if(i4Argc ==3) // single bypass mode
        {
        }
        else     // continue bypass mode
        {
            Printf("Please select bypass:\n");
	     _PrintBypassItem();
            Printf("Esc : quit auto bypass\n");
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
            else if((u1Char>='A')&&(u1Char<='A'))  // can extend to 'Z'
            {
            	  u1Char = u1Char-55;
            }
	     else if((u1Char>='a') && (u1Char<='a'))// can extend to 'z'
            {
            	  u1Char = u1Char-87;
            }
	     else if(u1Char==27) // escape, end of auto pattern
	     {
	     	return 0;
	     }
	     else if((u1Char==13) || (u1Char==10)) // ncstool, enter
	     {
	     	continue;
	     }
	     else
	     {
	     	Printf("Press [Esc] to quit the process\n");
	     	continue;
	     }

            vDisableAllBypass(u1Path, u1Char);
            Printf("-------------- Video Bypass %d -----------------\n", u1Char);
            
            switch(u1Char)
            {
                case BYP_NONE: //Dont' bypass
                    Printf("Success: DISABLE all bypass of Path %d (Not bypass)\n",u1Path);
                    break;
                    
                case BYP_PRE_DOWN:
                    _VDP_SetHorizontalPreScaleDownFactor(u1Path, 0x8000);
                    Printf("Success: Bypass Pre-scaling H down of Path %d\n",u1Path);
                    break;
                    
                case BYP_PRE_PROC:
                    vDrvAntiAliasingFilterOnOff(u1Path , SV_OFF);
                    vDrvMatrixCtrlSel(u1Path , SV_OFF ? 0 : 4);
                    vDrvChromaCoringOnOff(u1Path , SV_OFF);
                    Printf("Success: Bypass Pre-Porcessof Path %d\n",u1Path);
                    break;

                case BYP_NR:
                    vDrvNRSetForceBypass(SV_ON);
                    Printf("Success: Bypass NR of Path %d\n",u1Path);
                    break;
                    
                case BYP_PSCAN:
                    u1Mode = getScalerMode(u1Path);   
                    if(u1Mode !=VDP_SCPOS_PSCAN_DISPMODE)
                    {
                        vDrvDIOnOff(u1Path, SV_OFF);
                        Printf("Success: Bypass DI(PScan) of Path %d\n",u1Path);
                    }
                    else
                    {
                        Printf("Warning: Please DON'T bypass PScan when PScan Display mode\n");
                    }
                    break;
                    
                case BYP_SCALER:
                    VDP_Set121Map(u1Path, SV_ON);
                    Printf("Success: Byapss Scaling (dot by dot mode) %d\n",u1Path);
                    break;
                    
                case BYP_POST_PROC:
                    if(u1Path ==0) //main only
                    {
                        vHalVideoSharpnessOnOff(SV_VP_MAIN , SV_OFF);
                        vHalVideoLTIOnOff(SV_VP_MAIN , SV_OFF);
                        vHalVideoCTIOnOff(SV_VP_MAIN , SV_OFF);
                        vDrvSCEAllOnOff(SV_OFF);
                        Printf("Success: Bypass post-proc (Only in main path)\n");
                    }
                    else
                    {
                        Printf("Warning : \"No\" Post-Proc in sub path\n");
                    }
                    break;
                    
                case BYP_OSTG:
                    vDrvOSMatrixOnOff(u1Path , SV_OFF);

                    if(u1Path ==0) //main only
                    {
                        vDrvGammaOnOff(SV_OFF);
                    }
                    Printf("Success: Bypass output stage\n");
                    break;
                    
                case BYP_OSD:
                    OSD_PLA_SetWorkaround(0, 0);
                    OSD_PLA_Enable(0, 0);
                    OSD_PLA_SetWorkaround(1, 0);
                    OSD_PLA_Enable(1, 0);
                    OSD_PLA_SetWorkaround(0, 0);
                    OSD_PLA_Enable(1, 0);
                    Printf("Success: Bypass OSD\n");
                    break;
                        
                #if defined(CC_MT5365) || defined(CC_MT5395)
                case BYP_OD:
                	if(u1Path ==0) //main only
                	{
                		Printf("Success: Bypass OD\n");
                		vDrvODBypass(1);
                	}
                	break;
                #endif

                #ifdef CC_MT5395
                case BYP_MJC:
                	if(u1Path ==0) //main only
                	{
                		Printf("Success: Bypass MJC\n");
                		MJC_SetOnOff(E_MJC_OFF);
                	}
                	break;
                #endif
                        
                default:
                    Printf("Press [Esc] to quit the process\n");
                    break;
            }

            if(i4Argc ==3) // single bypass mode
                return 0;
        }
    }
}

#endif //#ifdef CC_CLI



