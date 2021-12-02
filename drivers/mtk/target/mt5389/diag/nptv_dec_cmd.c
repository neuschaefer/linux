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
#include "x_pinmux.h"
#include "x_debug.h"

#include "source_table.h"
#include "sv_const.h"
#include "drv_video.h"
#include "drv_tvd.h"
#include "drv_tdc.h"
#include "drv_nr.h"
#include "vdo_misc.h"
#include "vdo_if.h"


#ifndef CC_CLI
#define CC_CLI
#endif
#ifdef CC_CLI

/* Functions for Decoder */
static INT32 _bDrvVideoGetType(INT32 i4Argc, const CHAR **szArgv);
static INT32 _bDrvDecQuery(INT32 i4Argc, const CHAR **szArgv);
static INT32 _bDrvVideoTrigModeDetect(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u2DrvVideoInputWidth(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u2DrvVideoInputHeight(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1DrvVideoGetRefreshRate(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1DrvVideoIsSrcInterlace(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u2DrvVideoGetVPorch(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u2DrvVideoGetHPorch(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1DrvVideoSignalStatus(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u2DrvVideoGetVTotal(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u2DrvVideoGetHTotal(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1DrvVideoGetTiming(INT32 i4Argc, const CHAR **szArgv);
static INT32 _DecIsVgaTiming(INT32 i4Argc, const CHAR **szArgv);

#endif /*#ifdef CC_CLI*/
/******************************************************************************
* CLI Command Table
******************************************************************************/

// Table for Decoder command entry
CLI_EXEC_T arDecCmdTbl[] = {
#ifdef CC_CLI
  {"GetDec", "gt", _bDrvVideoGetType, NULL, "Query current decoder type", CLI_SUPERVISOR},
  {"Query", "q", _bDrvDecQuery, NULL, "Query decoder information", CLI_SUPERVISOR},
  {"ModeDet", "tmd", _bDrvVideoTrigModeDetect, NULL, "Trigger Mode Detection", CLI_SUPERVISOR},
  {"Width", "w", _u2DrvVideoInputWidth, NULL, "Get Current Video Width", CLI_SUPERVISOR},
  {"Height", "h", _u2DrvVideoInputHeight, NULL, "Get Current Video Heigth", CLI_SUPERVISOR},
  {"FrameRate", "fr", _u1DrvVideoGetRefreshRate, NULL, "Get Current Frame Rate", CLI_SUPERVISOR},
  {"Interlace", "i", _u1DrvVideoIsSrcInterlace, NULL, "Query whether current input source is interlaced", CLI_SUPERVISOR},
  {"VPorch", "vp", _u2DrvVideoGetVPorch, NULL, "Get Current VPorch", CLI_SUPERVISOR},
  {"HPorch", "hp", _u2DrvVideoGetHPorch, NULL, "Get Current HPorch", CLI_SUPERVISOR},
  {"SigStatus", "ss", _u1DrvVideoSignalStatus, NULL, "Get Current Signal Status", CLI_SUPERVISOR},
  {"VTotal", "vt", _u2DrvVideoGetVTotal, NULL, "Get VTotal", CLI_SUPERVISOR},
  {"HTotal", "ht", _u2DrvVideoGetHTotal, NULL, "Get HTotal", CLI_SUPERVISOR},
  {"Timing", "t", _u1DrvVideoGetTiming, NULL, "Get Timing", CLI_SUPERVISOR},
  {"isvga", "isvga", _DecIsVgaTiming, NULL, "check if it is vga timing", CLI_SUPERVISOR},
#endif
  {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

#ifdef CC_CLI
/******************************************************************************
* Local Function
******************************************************************************/

static INT32 _bDrvDecQuery(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Path;
    CHAR szTmp[30];
    
    UNUSED(u1Path);
    if(i4Argc != 2)
    {
      Printf("Usage: %s <main/pip>\n", szArgv[0]);
      return (0);
    }
    
    if((x_strcmp(szArgv[1], "m") == 0) || (x_strcmp(szArgv[1], "main") == 0))
    {
      u1Path = SV_VP_MAIN;
    }
    else
    {
      u1Path = SV_VP_PIP;
    }
    x_snprintf(szTmp,30, "n.dec.gt %s", szArgv[1]);
    CLI_Parser(szTmp);

    x_snprintf(szTmp,30, "n.dec.ss %s", szArgv[1]);
    CLI_Parser(szTmp);
    
    x_snprintf(szTmp,30, "n.dec.ht %s", szArgv[1]);
    CLI_Parser(szTmp);    
    
    x_snprintf(szTmp,30, "n.dec.w %s", szArgv[1]);
    CLI_Parser(szTmp);
    
    x_snprintf(szTmp,30, "n.dec.vt %s", szArgv[1]);
    CLI_Parser(szTmp);
    
    x_snprintf(szTmp,30, "n.dec.h %s", szArgv[1]);
    CLI_Parser(szTmp);

    x_snprintf(szTmp,30, "n.dec.fr %s", szArgv[1]);
    CLI_Parser(szTmp);        
    
    x_snprintf(szTmp,30, "n.dec.t %s", szArgv[1]);
    CLI_Parser(szTmp);    
    
    x_snprintf(szTmp,30, "n.dec.isvga %s", szArgv[1]);
    CLI_Parser(szTmp);        

/// Display omux status
    vShowOmuxStatus();
    return 0 ;

}

/* Video Decoder Related Functions */
static INT32 _bDrvVideoGetType(INT32 i4Argc, const CHAR **szArgv)
{
  UINT8 u1Path, u1DecType;

  UNUSED(u1Path);
  if(i4Argc != 2)
  {
    Printf("Usage: %s <main/pip>\n", szArgv[0]);
    return (0);
  }

  if((x_strcmp(szArgv[1], "m") == 0) || (x_strcmp(szArgv[1], "main") == 0))
  {
    u1Path = SV_VP_MAIN;
  }
  else
  {
    u1Path = SV_VP_PIP;
  }

  // premitive api
  u1DecType = bGetVideoDecType(u1Path);

  switch(u1DecType)
  {
  case SV_VD_NA:
    Printf("%s Unknown\n", szArgv[1]);
    break;

  case SV_VD_TVD3D:
    Printf("%s TVD\n", szArgv[1]);
    break;

  case SV_VD_YPBPR:
    Printf("%s Ypbpr\n", szArgv[1]);
    break;

  case SV_VD_VGA:
    Printf("%s VGA\n", szArgv[1]);
    break;

  case SV_VD_DVI:
    Printf("%s DVI\n", szArgv[1]);
    break;

  case SV_VD_MPEGHD:
    Printf("%s MPEG_HD\n", szArgv[1]);
    break;

  case SV_VD_MPEGSD:
    Printf("%s MPEG_SD\n", szArgv[1]);
    break;


  default:
    assert(0);
    break;
  }  
  return 0;
}

static INT32 _bDrvVideoTrigModeDetect(INT32 i4Argc, const CHAR **szArgv)
{
  UINT8 u1Path;

  if(i4Argc != 2)
  {
    Printf("Usage: %s <main/pip>\n", szArgv[0]);
    return (0);
  }

  if((x_strcmp(szArgv[1], "m") == 0) || (x_strcmp(szArgv[1], "main") == 0))
  {
    u1Path = SV_VP_MAIN;
  }
  else
  {
    u1Path = SV_VP_PIP;
  }

  vDrvVideoTrigModeDetect(u1Path);

  return 0;

}

static INT32 _u2DrvVideoInputWidth(INT32 i4Argc, const CHAR **szArgv)
{
  UINT8 u1Path;
  UINT16 u2Tmp;

  if(i4Argc != 2)
  {
    Printf("Usage: %s <main/pip>\n", szArgv[0]);
    return 0;
  }

  if((x_strcmp(szArgv[1], "m") == 0) || (x_strcmp(szArgv[1], "main") == 0))
  {
    u1Path = SV_VP_MAIN;
  }
  else
  {
    u1Path = SV_VP_PIP;
  }

  u2Tmp = wDrvVideoInputWidth(u1Path);
  Printf("%s source width = %d\n", szArgv[1], u2Tmp);
  return 0;
}

static INT32 _u2DrvVideoInputHeight(INT32 i4Argc, const CHAR **szArgv)
{
  UINT8 u1Path;
  UINT16 u2Tmp;

  if(i4Argc != 2)
  {
    Printf("Usage: %s <main/pip>\n", szArgv[0]);
  }

  if((x_strcmp(szArgv[1], "m") == 0) || (x_strcmp(szArgv[1], "main") == 0))
  {
    u1Path = SV_VP_MAIN;
  }
  else
  {
    u1Path = SV_VP_PIP;
  }

  u2Tmp = wDrvVideoInputHeight(u1Path);
  Printf("%s source height = %d\n", szArgv[1], u2Tmp);
  return 0;
}

static INT32 _u1DrvVideoGetRefreshRate(INT32 i4Argc, const CHAR **szArgv)
{
  UINT8 u1Path, u1Tmp;

  if(i4Argc != 2)
  {
    Printf("Usage: %s <main/pip>\n", szArgv[0]);
  }

  if((x_strcmp(szArgv[1], "m") == 0) || (x_strcmp(szArgv[1], "main") == 0))
  {
    u1Path = SV_VP_MAIN;
  }
  else
  {
    u1Path = SV_VP_PIP;
  }

  u1Tmp = bDrvVideoGetRefreshRate(u1Path);
  Printf("%s refresh-rate = %d\n", szArgv[1], u1Tmp);
  return 0;

}

static INT32 _u1DrvVideoIsSrcInterlace(INT32 i4Argc, const CHAR **szArgv)
{
  UINT8 u1Path;

  if(i4Argc != 2)
  {
    Printf("Usage: %s <main/pip>\n", szArgv[0]);
  }

  if((x_strcmp(szArgv[1], "m") == 0) || (x_strcmp(szArgv[1], "main") == 0))
  {
    u1Path = SV_VP_MAIN;
  }
  else
  {
    u1Path = SV_VP_PIP;
  }

  if(bDrvVideoIsSrcInterlace(u1Path) == SV_TRUE)
  {
    Printf("%s is interlaced\n", szArgv[1]);
  }
  else
  {
    Printf("%s is non-interlaced\n", szArgv[1]);
  }
  return 0;
}

static INT32 _u2DrvVideoGetVPorch(INT32 i4Argc, const CHAR **szArgv)
{
  UINT8 u1Path;
  UINT16 u2Tmp;

  if(i4Argc != 2)
  {
    Printf("Usage: %s <main/pip>\n", szArgv[0]);
  }

  if((x_strcmp(szArgv[1], "m") == 0) || (x_strcmp(szArgv[1], "main") == 0))
  {
    u1Path = SV_VP_MAIN;
  }
  else
  {
    u1Path = SV_VP_PIP;
  }

  u2Tmp = wDrvVideoGetVPorch(u1Path);
  Printf("%s VPorch = %d\n", szArgv[1], u2Tmp);
  return 0;
}

#if 0
static INT32 _vDRVTVDBlankLevel(INT32 i4Argc, const CHAR **szArgv)
{
  if (i4Argc != 2)
  {
    Printf("Usage: blank blanklevel(default 256)\n");
    return 0;
  }

//  vDrvTVDBlankLevel((UINT16)StrToInt(szArgv[1]));

  return 0;
}

static INT32 _TdcStart(INT32 i4Argc, const CHAR **szArgv)
{
  if (i4Argc ==1)
  {
    Printf("TDC start is: 0x%x\n", wDrvTDCGetStart());
  }
  else if(i4Argc == 2)
  {
  	vDrvTDCSetStart((UINT16)StrToInt(szArgv[1]));
	Printf("Set TDC start as: 0x%x\n", (UINT16)StrToInt(szArgv[1]));
  }
  else
  {
  	Printf("Usage: start start_position\n");
  }

  return 0;
}

static INT32 _TdcLength(INT32 i4Argc, const CHAR **szArgv)
{
  if (i4Argc ==1)
  {
    Printf("TDC length is: 0x%x\n", wDrvTDCGetLength());
  }
  else if(i4Argc == 2)
  {
  	vDrvTDCSetLength((UINT16)StrToInt(szArgv[1]));
	Printf("Set TDC length as: 0x%x\n", (UINT16)StrToInt(szArgv[1]));
  }
  else
  {
  	Printf("Usage: length length_width\n");
  }

  return 0;
}

static INT32 _NRHeight(INT32 i4Argc, const CHAR **szArgv)
{
  if (i4Argc ==1)
  {
    Printf("NR height is: 0x%x\n", wDrvNRGetHeight());
  }
  else if(i4Argc == 2)
  {
  	vDrvNRSetHeight((UINT16)StrToInt(szArgv[1]));
	Printf("Set NR height as: 0x%x\n", (UINT16)StrToInt(szArgv[1]));
  }
  else
  {
  	Printf("Usage: height length_width\n");
  }

  return 0;
}

static INT32 _NRMeter(INT32 i4Argc, const CHAR **szArgv)
{
	extern INT8 _iANRSysStatus;
	extern UINT8 _bCurNoiseLevel;

	if (i4Argc == 1)
	{
		Printf("Noise=%d, IIR(noise)=%d\n", _bCurNoiseLevel, _iANRSysStatus);
	}
	else
	{
		Printf("%s \n", szArgv[1]);
	}

	return 0;
}

static INT32 _vDRVDFEAGCLevel(INT32 i4Argc, const CHAR **szArgv)
{
  if (i4Argc != 2)
  {
    Printf("Usage: VGA Target level(default 0x33)\n");
    return 0;
  }

//  vTvdDFEAGCTarget((UINT16)StrToInt(szArgv[1]));

  return 0;
}

static INT32 _vDRVCAGCLevel(INT32 i4Argc, const CHAR **szArgv)
{
  if (i4Argc != 2)
  {
    Printf("Usage:CAGC Target level(Range:0xB7(-0x49)~0x89)\n");
    return 0;
  }

//  vTVDCAGCTARGET((INT8)StrToInt(szArgv[1]));

  return 0;
}

static INT32 _vDRVTVDHW(INT32 i4Argc, const CHAR **szArgv)
{
  Printf("Height = %d\n", wTvd3dHeight() );
  Printf("Width = %d\n", 720);

  return 0;
}

static INT32 _vDRVTVDAGCPedEn(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 2)
	{
		Printf("Usage: AGCPedEn?\n");
		return 0;
	}

	vTVD3dAGCPedEn((UINT8)StrToInt(szArgv[1]));

	Printf("Set AGC Ped_En to %d\n", (UINT8)StrToInt(szArgv[1]));
	return 0;
}

#if TVD_AUTO_CALIB
static INT32 _vDRVTVDAdcCalib(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 2)
	{
		Printf("Usage: AdcCalib?\n");
		return 0;
	}

	if(bDrvTVDAdcCalib())
	{
		Printf("Adc Calib Success\n");
	}
	else
	{
		Printf("Adc Calib Fail\n");
	}

	return 0;
}
#endif

static INT32 _vDRVTVDAVdelay(INT32 i4Argc, const CHAR **szArgv)
{
  if (i4Argc != 2)
  {
    Printf("Usage:TVD AV Delay (BYTE) default 0xA2 \n");
    return 0;
  }

  //vTVDAVDelay((UINT16)StrToInt(szArgv[1]));

  return 0;
}
#endif

static INT32 _u2DrvVideoGetHPorch(INT32 i4Argc, const CHAR **szArgv)
{
  UINT8 u1Path;
  UINT16 u2Tmp;

  if(i4Argc != 2)
  {
    Printf("Usage: %s <main/pip>\n", szArgv[0]);
  }

  if((x_strcmp(szArgv[1], "m") == 0) || (x_strcmp(szArgv[1], "main") == 0))
  {
    u1Path = SV_VP_MAIN;
  }
  else
  {
    u1Path = SV_VP_PIP;
  }

  u2Tmp = wDrvVideoGetHPorch(u1Path);
  Printf("%s HPorch = %d\n", szArgv[1], u2Tmp);
  return 0;

}

static INT32 _u1DrvVideoSignalStatus(INT32 i4Argc, const CHAR **szArgv)
{
  UINT8 u1Path, u1Tmp;

  if(i4Argc != 2)
  {
    Printf("Usage: %s <main/pip>\n", szArgv[0]);
  }

  if((x_strcmp(szArgv[1], "m") == 0) || (x_strcmp(szArgv[1], "main") == 0))
  {
    u1Path = SV_VP_MAIN;
  }
  else
  {
    u1Path = SV_VP_PIP;
  }

  u1Tmp = bDrvVideoSignalStatus(u1Path);
  Printf("%s Signal Status = ", szArgv[1]);
  switch(u1Tmp)
  {
  case SV_VDO_NOSIGNAL:
  Printf("No Signal\n");
  break;
  case SV_VDO_NOSUPPORT:
  Printf("No Support\n");
  break;
  case SV_VDO_UNKNOWN:
  Printf("Unknown\n");
  break;

  case SV_VDO_STABLE:
  Printf("Stable\n");
  break;

  default:
  assert(0);
  break;
  }
  return 0;
}

static INT32 _u2DrvVideoGetVTotal(INT32 i4Argc, const CHAR **szArgv)
{
  UINT8 u1Path;
  UINT16 u2Tmp;

  if(i4Argc != 2)
  {
    Printf("Usage: %s <main/pip>\n", szArgv[0]);
  }

  if((x_strcmp(szArgv[1], "m") == 0) || (x_strcmp(szArgv[1], "main") == 0))
  {
    u1Path = SV_VP_MAIN;
  }
  else
  {
    u1Path = SV_VP_PIP;
  }

  u2Tmp = wDrvVideoGetVTotal(u1Path);
  Printf("%s VTotal = %d\n", szArgv[1], u2Tmp);
  return 0;

}

static INT32 _u2DrvVideoGetHTotal(INT32 i4Argc, const CHAR **szArgv)
{
  UINT8 u1Path;
  UINT16 u2Tmp;

  if(i4Argc != 2)
  {
    Printf("Usage: %s <main/pip>\n", szArgv[0]);
  }

  if((x_strcmp(szArgv[1], "m") == 0) || (x_strcmp(szArgv[1], "main") == 0))
  {
    u1Path = SV_VP_MAIN;
  }
  else
  {
    u1Path = SV_VP_PIP;
  }

  u2Tmp = wDrvVideoGetHTotal(u1Path);
  Printf("%s HTotal = %d\n", szArgv[1], u2Tmp);
  return 0;
}

static INT32 _u1DrvVideoGetTiming(INT32 i4Argc, const CHAR **szArgv)
{
  UINT8 u1Path;

  if(i4Argc != 2)
  {
    Printf("Usage: %s <main/pip>\n", szArgv[0]);
  }

  if((x_strcmp(szArgv[1], "m") == 0) || (x_strcmp(szArgv[1], "main") == 0))
  {
    u1Path = SV_VP_MAIN;
  }
  else
  {
    u1Path = SV_VP_PIP;
  }

  Printf("%s Timing = %s\n", szArgv[1], strDrvVideoGetTimingString(bDrvVideoGetTiming(u1Path)));
  return 0;

}

static INT32 _DecIsVgaTiming(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Path;

    if(i4Argc != 2)
    {
        Printf("Usage: %s <main/pip>\n", szArgv[0]);
        return -1;
    }
  if((x_strcmp(szArgv[1], "m") == 0) || (x_strcmp(szArgv[1], "main") == 0))
  {
    u1Path = SV_VP_MAIN;
  }
  else
  {
    u1Path = SV_VP_PIP;
  }    
      Printf("IsVga %d\n", fgApiVideoIsVgaMode(u1Path, 0));
      return 0 ;
}

#endif //#ifdef CC_CLI

