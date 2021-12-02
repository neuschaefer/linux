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
 * $RCSfile: nptv_hdmi_cmd.c,v $
 * $Revision: #1 $
 *
 * Description:
 *         This file contains CLI implementation of NPTV Video.
 *---------------------------------------------------------------------------*/


#include "x_os.h"
#include "x_stl_lib.h"
#include "video_def.h"
#include "vdp_drvif.h"

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
#include "drv_hdmi.h"
#include "drv_dvi.h"
#include "hw_hdmi.h"
#include "hw_vdoin.h"

#ifndef CC_CLI
#define CC_CLI
#endif

//#ifdef CC_CLI


/* Functions for HDMI */
static INT32 _u1vHDMIInit(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMIMainLoop(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMIStopFw(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMIResumeFw(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMICRC(INT32 i4Argc, const CHAR **szArgv);
#if defined(CC_MT5365) || defined(CC_MT5395)
static INT32 _u1vHDMIPHYCRC(INT32 i4Argc, const CHAR **szArgv);
#endif
static INT32 _u1vDVICRC(INT32 i4Argc, const CHAR **szArgv);
#ifdef NPTV_CMD_HDMI_RW_DEV_SUPPORT
static INT32 _u1vHDMIWriteDevL(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMIWriteDevH(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1bHDMIReadDevL(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1bHDMIReadDevH(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1bHDMIDumpDevL(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1bHDMIDumpDevH(INT32 i4Argc, const CHAR **szArgv);
#endif //NPTV_CMD_HDMI_RW_DEV_SUPPORT
static INT32 _u1GPIO_Output(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMIDebugMsg(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMIHDCPKey(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMIReloadHDCPKey(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMIHDCPSelfBist(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMIBypassVdo(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMIvLoadColorMatrix(INT32 i4Argc, const CHAR **szArgv);
static INT32  _u1vHDMIMCMHdcpWrite(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMISetHDMIState(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMISetSwitch(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMIEQSel(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMILBWSet(INT32 i4Argc, const CHAR **szArgv);

static INT32 _u1vHDMIEQGain(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMITHR(INT32 i4Argc, const CHAR **szArgv);

static INT32 _u1vHDMIHPDHigh(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMIHPDLow(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMIGetInfoFrame(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMIHPDIndepCtrl(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMIModeSel(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMIDebug(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMIForce3D(INT32 i4Argc, const CHAR **szArgv);


//#endif /*#ifdef CC_CLI*/
/******************************************************************************
* CLI Command Table
******************************************************************************/

// Table for HDMI command entry
CLI_EXEC_T arHDMICmdTbl[] = {
  //#ifdef CC_CLI
  {"Init", "i", _u1vHDMIInit, NULL, "HDMI initialization", CLI_SUPERVISOR},
  {"Mainloop", "m", _u1vHDMIMainLoop, NULL, "HDMI mainloop", CLI_SUPERVISOR},
  {"stop fw", "stop", _u1vHDMIStopFw, NULL, "stop HDMI mainloop", CLI_GUEST},
  {"resume fw", "resume", _u1vHDMIResumeFw, NULL, "resume HDMI mainloop", CLI_GUEST},
  {"set fw state", "set", _u1vHDMISetHDMIState, NULL, "control fw state machine", CLI_SUPERVISOR},
  #if defined(CC_MT5365) || defined(CC_MT5395)
  {"PHYCRC", "pc", _u1vHDMIPHYCRC, NULL, "HDMI analog PHY CRC check", CLI_GUEST},
  #endif
  {"CRC", "c", _u1vHDMICRC, NULL, "HDMI CRC check", CLI_GUEST},
  {"DVICRC", "dc", _u1vDVICRC, NULL, "DVI CRC check", CLI_SUPERVISOR},
  {"dmesg", "dmsg", _u1vHDMIDebugMsg, NULL, "Print HDMI status", CLI_GUEST},
  {"hdcp log key", "hlk", _u1vHDMIHDCPKey, NULL, "Print hdcp key", CLI_SUPERVISOR},
  {"hdcp reload key", "hrk", _u1vHDMIReloadHDCPKey, NULL, "Reload hdcp key", CLI_SUPERVISOR},
  {"hdcp bist", "hdb", _u1vHDMIHDCPSelfBist, NULL, "Hdcp seltbist", CLI_SUPERVISOR},
  {"hdmi bypass", "hbv", _u1vHDMIBypassVdo, NULL, "Bypass HDMI processing", CLI_SUPERVISOR},
  {"switch", "switch", _u1vHDMISetSwitch, NULL, "switch HDMI1/HDMI2", CLI_SUPERVISOR},
  {"GPIO", "io", _u1GPIO_Output, NULL, "I/O", CLI_SUPERVISOR},
  {"EQSEL", "eq", _u1vHDMIEQSel, NULL, "set EQ_SEL", CLI_GUEST},
  {"LBW", "lbw", _u1vHDMILBWSet, NULL, "set LBW", CLI_GUEST},
  {"EQ_GAIN", "eqg", _u1vHDMIEQGain, NULL, "set EQ_GAIN", CLI_GUEST},
  {"HDMI THR", "thr", _u1vHDMITHR, NULL, "set mode change THR and mode detect THR", CLI_GUEST},
  {"HPD high", "hh", _u1vHDMIHPDHigh, NULL, "set HPD HIGH", CLI_SUPERVISOR},
  {"HPD Low", "hl", _u1vHDMIHPDLow, NULL, "set HPD LOW", CLI_SUPERVISOR},
   {"InfoFrame read", "info", _u1vHDMIGetInfoFrame, NULL, "read infoframe", CLI_SUPERVISOR},
   {"CKDT detect", "ckdt", _u1vHDMIHPDIndepCtrl, NULL, "5v detect /CKDT detect", CLI_SUPERVISOR},
  {"3x3", "33", _u1vHDMIvLoadColorMatrix, NULL, "3x3 color transform", CLI_SUPERVISOR},
  {"MCM", "MCM", _u1vHDMIMCMHdcpWrite, NULL, "MCM EEPROM write", CLI_SUPERVISOR},
  {"Mode Sel", "mode", _u1vHDMIModeSel, NULL, "Auto/Graphic/Video mode select", CLI_SUPERVISOR},
  {"HDMIdebug", "debug", _u1vHDMIDebug, NULL, "HDMI debug command", CLI_GUEST},
  {"Force 3D", "f3d", _u1vHDMIForce3D, NULL, "HDMI force 3D", CLI_SUPERVISOR},
  //#endif
  {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

//#ifdef CC_CLI
/******************************************************************************
* Local Function
******************************************************************************/
static INT32 _u1vHDMIInit(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 1)
	{
		Printf("Usage: %s\n", szArgv[0]);
		return 0;
	}

	vHDMIInit();

	Printf("vHDMIInit\n");

	return 0;
}

static INT32 _u1vHDMIMainLoop(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 1)
	{
		Printf("Usage: %s\n", szArgv[0]);
		return 0;
	}

	vHDMIMainLoop();

	Printf("vHDMIMainLoop\n");

	return 0;
}

static INT32 _u1vHDMIStopFw(INT32 i4Argc, const CHAR **szArgv)
{
	vHDMIStopFw();
    return 0;
}

static INT32 _u1vHDMIResumeFw(INT32 i4Argc, const CHAR **szArgv)
{
	vHDMIResumeFw();
    return 0;
}

static INT32 _u1vHDMICRC(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 2)
	{
		Printf("Usage: %s\n", szArgv[0]);
		return 0;
	}

	UNUSED(fgHDMICRC((UINT16)StrToInt(szArgv[1])));

	return 0;
}
#if defined(CC_MT5365) || defined(CC_MT5395)
static INT32 _u1vHDMIPHYCRC(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 2)
	{
		Printf("Usage: %s\n", szArgv[0]);
		return 0;
	}

	UNUSED(fgHDMIPHYCRC((UINT16)StrToInt(szArgv[1])));

	return 0;
}
#endif
static INT32 _u1vDVICRC(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 2)
	{
		Printf("Usage: %s\n", szArgv[0]);
		return 0;
	}

	UNUSED(fgDVICRC((UINT16)StrToInt(szArgv[1])));

	return 0;
}

#ifdef NPTV_CMD_HDMI_RW_DEV_SUPPORT
static INT32 _u1vHDMIWriteDevL(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 3)
	{
		Printf("Usage: %s addr value\n", szArgv[0]);
		return 0;
	}
//	vHDMIWriteDevL((UINT8)StrToInt(szArgv[1]), (UINT8)StrToInt(szArgv[2]));

	return 0;
}

static INT32 _u1vHDMIWriteDevH(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 3)
	{
		Printf("Usage: %s addr value\n", szArgv[0]);
		return 0;
	}
//	vHDMIWriteDevH((UINT8)StrToInt(szArgv[1]), (UINT8)StrToInt(szArgv[2]));

	return 0;
}
static INT32 _u1bHDMIReadDevL(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 2)
	{
		Printf("Usage: %s addr\n", szArgv[0]);
		return 0;
	}

//	Printf("read low value 0x%x\n", bHDMIReadDevL((UINT8)StrToInt(szArgv[1])));

	return 0;
}
static INT32 _u1bHDMIReadDevH(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 2)
	{
		Printf("Usage: %s addr\n", szArgv[0]);
		return 0;
	}

//	Printf("read high value 0x%x\n", bHDMIReadDevH((UINT8)StrToInt(szArgv[1])));

	return 0;
}


static INT32 _u1bHDMIDumpDevL(INT32 i4Argc, const CHAR **szArgv)
{
    INT32  i;
	// UINT32 u4Idx;

	for (i=0; i<0xff ; i++)
	{
//	    Printf("0x%2X read low value 0x%x\n", i, bHDMIReadDevL(i));
	}

	return 0;
}

static INT32 _u1bHDMIDumpDevH(INT32 i4Argc, const CHAR **szArgv)
{
    INT32  i;
	// UINT32 u4Idx;

	for (i=0; i<0xff ; i++)
	{
//	    Printf("0x%2X read high value 0x%x\n", i, bHDMIReadDevH(i));
	}

	return 0;
}

#endif //NPTV_CMD_HDMI_RW_DEV_SUPPORT

static INT32 _u1GPIO_Output(INT32 i4Argc, const CHAR **szArgv)
{
	INT32 i;

	if (i4Argc != 3)
	{
		Printf("Usage: %s addr high/low\n", szArgv[0]);
		return 0;
	}

	Printf("addr: %d\n", StrToInt(szArgv[1]));
	if ((UINT8)StrToInt(szArgv[2]) == 0)
	{
		i = 0;
		Printf("output low\n");
	}
	else
	{
		i = 1;
		Printf("output high\n");
	}
	GPIO_Output(StrToInt(szArgv[1]), &i);
    return 0;
}

static INT32 _u1vHDMIEQSel(INT32 i4Argc, const CHAR **szArgv)
{
	#if defined(CC_MT5387) ||defined(CC_MT5363)
	UINT32 _dwVal = 0;
	if (i4Argc != 2)
	{
        Printf("***************EQ = %x*********************\n", RegReadFldAlign(TMDS_CTRL1, EQSEL));
        Printf("Usage Example: EQ 0xd\n");
        return 0;
	}
	_dwVal = (UINT8)StrToInt(szArgv[1]);
	Printf("***************pre EQ = %x*********************\n", RegReadFldAlign(TMDS_CTRL1, EQSEL));
	vRegWriteFldAlign(TMDS_CTRL1,_dwVal, EQSEL);
	Printf("***************now EQ = %x*********************\n", RegReadFldAlign(TMDS_CTRL1, EQSEL));
	#endif

	#if defined(CC_MT5365) || defined(CC_MT5395)
		#if !Support_HW_Adaptive_EQ  // Fix EQ
			UINT32 _dwVal = 0;
			if (i4Argc != 2)
			{
		        Printf("***************HDMI_EQ_FILT = 0x%x*********************\n", RegReadFldAlign(ANA_INTF_1, RG_HDMI_EQ_FILT));
		        Printf("Usage Example: EQ 0xd\n");
			  Printf("Max->Min: 0x7, 0x6, 0x5, 0x4, 0x3, 0x2, 0x1, 0x0, 0xf, 0xe, 0xd, 0xc, 0xb, 0xa, 0x9, 0x8\n");
		        return 0;
			}
			_dwVal = (UINT8)StrToInt(szArgv[1]);
			Printf("***************pre HDMI_EQ_FILT = 0x%x*********************\n", RegReadFldAlign(ANA_INTF_1, RG_HDMI_EQ_FILT));
			vRegWriteFldAlign(ANA_INTF_1,_dwVal, RG_HDMI_EQ_FILT);
			Printf("***************now HDMI_EQ_FILT = 0x%x*********************\n", RegReadFldAlign(ANA_INTF_1, RG_HDMI_EQ_FILT));
		#endif
	#endif
	return 0;
	
}

static INT32 _u1vHDMILBWSet(INT32 i4Argc, const CHAR **szArgv)
{
	#if defined(CC_MT5387) ||defined(CC_MT5363)
	UINT32 _dwVal = 0;
	if (i4Argc != 2)
	{
        Printf("***************LBW = %x*********************\n", RegReadFldAlign(TMDS_CTRL0, HDMI_LBW));
        Printf("Usage Example: LBW 0x2\n");
        return 0;
	}
	_dwVal = (UINT8)StrToInt(szArgv[1]);
	Printf("***************pre LBW = %x*********************\n", RegReadFldAlign(TMDS_CTRL0, HDMI_LBW));
    vRegWriteFldAlign(TMDS_CTRL0,_dwVal, HDMI_LBW);
	Printf("***************now LBW = %x*********************\n", RegReadFldAlign(TMDS_CTRL0, HDMI_LBW));
	#endif

	#if defined(CC_MT5365) || defined(CC_MT5395)
		#if !Support_HW_Adaptive_EQ  // Fix EQ
			UINT32 _dwVal = 0;
			if (i4Argc != 3)
			{
		        Printf("***************HDMI_EQBIAS = 0x%x, HDMI_PLLBIAS = 0x%x*********************\n", RegReadFldAlign(ANA_INTF_1,RG_HDMI_EQBIAS), RegReadFldAlign(ANA_INTF_2,RG_HDMI_PLLBIAS));
		        Printf("Usage Example: LBW 0x4 0x2\n");
		        return 0;
			}
			_dwVal = (UINT8)StrToInt(szArgv[1]);
			Printf("***************pre HDMI_EQBIAS = 0x%x*********************\n", RegReadFldAlign(ANA_INTF_1,RG_HDMI_EQBIAS));
			vRegWriteFldAlign(ANA_INTF_1,_dwVal, RG_HDMI_EQBIAS);
			Printf("***************now HDMI_EQBIAS = 0x%x*********************\n", RegReadFldAlign(ANA_INTF_1, RG_HDMI_EQBIAS));
			_dwVal = (UINT8)StrToInt(szArgv[2]);
			Printf("***************pre HDMI_PLLBIAS = 0x%x*********************\n", RegReadFldAlign(ANA_INTF_2, RG_HDMI_PLLBIAS));
			vRegWriteFldAlign(ANA_INTF_2,_dwVal, RG_HDMI_PLLBIAS);
			Printf("***************now HDMI_PLLBIAS = 0x%x*********************\n", RegReadFldAlign(ANA_INTF_2, RG_HDMI_PLLBIAS));
		#endif
	#endif
	return 0;
}

static INT32 _u1vHDMIEQGain(INT32 i4Argc, const CHAR **szArgv)
{
	#if defined(CC_MT5387) ||defined(CC_MT5363)
	UINT32 _dwVal = 0;
	if (i4Argc != 2)
	{
		Printf("***************EQ_GAIN = %x*********************\n", RegReadFldAlign(TMDS_CTRL1, EQ_GAIN));
        Printf("Usage Example: EQG 0x2\n");
        return 0;
	}
	_dwVal = (UINT8)StrToInt(szArgv[1]);
	Printf("***************pre EQ_GAIN = %x*********************\n", RegReadFldAlign(TMDS_CTRL1, EQ_GAIN));
    vRegWriteFldAlign(TMDS_CTRL1,_dwVal, EQ_GAIN);
	Printf("***************now EQ_GAIN = %x*********************\n", RegReadFldAlign(TMDS_CTRL1, EQ_GAIN));
	#endif

	#if defined(CC_MT5365) || defined(CC_MT5395)
		#if !Support_HW_Adaptive_EQ  // Fix EQ
			UINT32 _dwVal = 0;
			if (i4Argc != 2)
			{
		        Printf("***************HDMI_EQ_GAIN = 0x%x*********************\n", RegReadFldAlign(ANA_INTF_2,RG_HDMI_EQ_GAIN));
		        Printf("Usage Example: EQG 0x0\n");
		        return 0;
			}
			_dwVal = (UINT8)StrToInt(szArgv[1]);
			Printf("***************pre HDMI_EQ_GAIN = 0x%x*********************\n", RegReadFldAlign(ANA_INTF_2,RG_HDMI_EQ_GAIN));
			vRegWriteFldAlign(ANA_INTF_2,_dwVal, RG_HDMI_EQ_GAIN);
			Printf("***************now HDMI_EQ_GAIN = 0x%x*********************\n", RegReadFldAlign(ANA_INTF_2, RG_HDMI_EQ_GAIN));
		#endif
	#endif
	return 0;
}

extern UINT8 _bDviMdChgCntTHR ;
extern UINT8 _bDviDeChgCntTHR ;
extern UINT8 _bDviPixClkChgCntTHR ;
#if CCIR_decode_444
extern UINT8 _bCCIRNeedModeChgCntTHR;  // jiewen, 20100519
#endif
extern UINT32 _wDviTmpTHR ;
extern UINT32  _wDVI_WAIT_STABLE_COUNTTHR ;

static INT32 _u1vHDMITHR(INT32 i4Argc, const CHAR **szArgv)
{
        if (i4Argc != 6)
        {
			Printf(" _bDviMdChgCntTHR = %d\n", _bDviMdChgCntTHR);
			Printf(" _bDviDeChgCntTHR = %d\n", _bDviDeChgCntTHR);
			Printf("_bDviPixClkChgCntTHR = %d\n", _bDviPixClkChgCntTHR);
			Printf(" _wDviTmpTHR = %d\n", _wDviTmpTHR);
			Printf(" _wDVI_WAIT_STABLE_COUNTTHR = %d\n", _wDVI_WAIT_STABLE_COUNTTHR);
		#if CCIR_decode_444
			Printf("_bCCIRNeedModeChgCntTHR = %d\n", _bCCIRNeedModeChgCntTHR);  // jiewen, 20100519
		#endif
            return 0;
        }
		Printf("pre _bDviMdChgCntTHR = %d\n", _bDviMdChgCntTHR);
		Printf("pre _bDviDeChgCntTHR = %d\n", _bDviDeChgCntTHR);
		Printf("pre _bDviPixClkChgCntTHR = %d\n", _bDviPixClkChgCntTHR);
		Printf("pre _wDviTmpTHR = %d\n", _wDviTmpTHR);
		Printf("pre _wDVI_WAIT_STABLE_COUNTTHR = %d\n", _wDVI_WAIT_STABLE_COUNTTHR);
	#if CCIR_decode_444
		Printf("pre _bCCIRNeedModeChgCntTHR = %d\n", _bCCIRNeedModeChgCntTHR);  // jiewen, 20100519
	#endif

		_bDviMdChgCntTHR = (UINT8)StrToInt(szArgv[1]);
		_bDviDeChgCntTHR = (UINT8)StrToInt(szArgv[2]);
		_bDviPixClkChgCntTHR = (UINT8)StrToInt(szArgv[3]) ;
		_wDviTmpTHR = (UINT8)StrToInt(szArgv[4]);
		_wDVI_WAIT_STABLE_COUNTTHR = (UINT8)StrToInt(szArgv[5]);
	#if CCIR_decode_444
		_bCCIRNeedModeChgCntTHR = (UINT8)StrToInt(szArgv[6]);  // jiewen, 20100519
	#endif
		
		Printf("now _bDviMdChgCntTHR = %d\n", _bDviMdChgCntTHR);
		Printf("now _bDviDeChgCntTHR = %d\n", _bDviDeChgCntTHR);
		Printf("now _bDviPixClkChgCntTHR = %d\n", _bDviPixClkChgCntTHR);
		Printf("now _wDviTmpTHR = %d\n", _wDviTmpTHR);
		Printf("now _wDVI_WAIT_STABLE_COUNTTHR = %d\n", _wDVI_WAIT_STABLE_COUNTTHR);
	#if CCIR_decode_444
		Printf("now _bCCIRNeedModeChgCntTHR = %d\n", _bCCIRNeedModeChgCntTHR);  // jiewen, 20100519
	#endif
        return 0;
}
static INT32 _u1vHDMIHPDHigh(INT32 i4Argc, const CHAR **szArgv)
{
        if (i4Argc != 2)
        {
                Printf("Usage: %s switch\n", szArgv[0]);
                return 0;
        }
        vHDMIHPDHigh((UINT8)StrToInt(szArgv[1]));
        return 0;
}
static INT32 _u1vHDMIHPDLow(INT32 i4Argc, const CHAR **szArgv)
{
        if (i4Argc != 2)
        {
                Printf("Usage: %s switch\n", szArgv[0]);
                return 0;
        }
        vHDMIHPDLow((UINT8)StrToInt(szArgv[1]));
        return 0;
}
static INT32 _u1vHDMIDebug(INT32 i4Argc, const CHAR **szArgv)
{
        if (i4Argc != 3)
        {
                Printf("Usage: %s number  value\n", szArgv[0]);
                return 0;
        }
        vHDMIDebug((UINT8)StrToInt(szArgv[1]),(UINT32)StrToInt(szArgv[2]));
        return 0;
}
static INT32 _u1vHDMIGetInfoFrame(INT32 i4Argc, const CHAR **szArgv)
{
        if (i4Argc != 2)
        {
                Printf("Usage: %s (ex:0,1,2,3,4)\n", szArgv[0]);
                return 0;
        }
        vHDMIGetInfoFrame((UINT8)StrToInt(szArgv[1]));
        return 0;
}
static INT32 _u1vHDMIHPDIndepCtrl(INT32 i4Argc, const CHAR **szArgv)
{
        if (i4Argc != 2)
        {
                Printf("Usage: %s 1 (5v)  or 0 (CKDT) \n", szArgv[0]);
                return 0;
        }
        vHDMIHPDIndepCtrl((UINT8)StrToInt(szArgv[1]));
        return 0;
}


static INT32 _u1vHDMIDebugMsg(INT32 i4Argc, const CHAR **szArgv)
{
	//if (i4Argc != 2)
	//{
	//	Printf("Usage: %s cmd\n", szArgv[0]);
	//	return 0;
	//}
	vHDMIDebugMsg();
	return 0;
}

static INT32 _u1vHDMIHDCPKey(INT32 i4Argc, const CHAR **szArgv)
{
	vHDMIHDCPKey();
	return 0;
}

static INT32 _u1vHDMIReloadHDCPKey(INT32 i4Argc, const CHAR **szArgv)
{
	vHDMIReloadHDCPKey();
	return 0;
}

static INT32 _u1vHDMIHDCPSelfBist(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 2)
	{
		Printf("Usage: %s mode\n", szArgv[0]);
		return 0;
	}

	vHDMIHDCPSelfBist((UINT8)StrToInt(szArgv[1]));
	return 0;
}

static INT32 _u1vHDMIBypassVdo(INT32 i4Argc, const CHAR **szArgv)
{
#if 0
	UINT8 cmd;
	if (i4Argc != 2)
	{
		Printf("Usage: %s cmd\n", szArgv[0]);
		return 0;
	}
	cmd = (UINT8)StrToInt(szArgv[1]);
	if (cmd == 0x01)
	{
		vHDMIBypassVdo();
	}
	else if (cmd == 0x02)
	{
		// HDMI APLL
		CKGEN_WRITE32(0x380, 0x000000c0);
		CKGEN_WRITE32(0x404, 0x00000000);
		CKGEN_WRITE32(0x52c, 0x1c000000);
		CKGEN_WRITE32(0x304, 0x01088000);
		vHDMIWriteDevL(0x8b, 0x01);
		vHDMIWriteDevH(0x3f, 0xcf);
		vHDMIWriteDevL(0x8c, 0xc4);
		vHDMIWriteDevL(0x88, 0x00);
	}
	else if (cmd == 0x03)
	{
		// HDMI TMDSPLL
		CKGEN_WRITE32(0x380, 0x000000c0);
		CKGEN_WRITE32(0x400, 0x000333c0);
		IO_WRITE32(0x20000000, 0x8094, 0x00050003);
	}
#endif
	return 0;
}

static INT32 _u1vHDMIvLoadColorMatrix(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 2)
	{
		Printf("Usage: %s idx\n", szArgv[0]);
		return 0;
	}
	return 0;
}

static INT32 _u1vHDMIMCMHdcpWrite(INT32 i4Argc, const CHAR **szArgv)
{
	if(bHDMIMCMHdcpWrite()!=TRUE)
	{
		Printf("MCM EEPROM Write HDCP fail\n");
	}
	else
	{
		Printf("MCM EEPROM Write HDCP success\n");
	}
	return 0;
}
static INT32 _u1vHDMISetHDMIState(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 2)
	{
		Printf("Usage: %s state\n", szArgv[0]);
		return 0;
	}
	vHDMISetHDMIState((UINT8)StrToInt(szArgv[1]));
	return 0;
}

static INT32 _u1vHDMISetSwitch(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 2)
	{
		Printf("Usage: %s switch\n", szArgv[0]);
		return 0;
	}
	vHDMISetSwitch((UINT8)StrToInt(szArgv[1]));
	return 0;
}

extern UINT8 u1HdmiColorMode;
static INT32 _u1vHDMIModeSel(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 2)
	{
		Printf("Usage: %s <Auto(0)/Graphic(1)/Video(2)>\n", szArgv[0]);
		return 0;
	}
	u1HdmiColorMode = (UINT8)StrToInt(szArgv[1]);
	return 0;
}

static INT32 _u1vHDMIForce3D(INT32 i4Argc, const CHAR **szArgv)
{
    #if !defined(CC_MT5396)
	if (i4Argc != 4)
	{
		Printf("Usage: %s Force3D_En(0/1) 3DStructure 3DExtData>\n", szArgv[0]);
		Printf("3DStructure: (0)FP (1)FieldAlternative (2)LineAlternative (3)SBS_F (4)LDepth (5)LDepthGraph (6)TB (8)SBS_H\n");
		Printf("3DExtData: Default value=0; If 3DStructure=SBS_H, should fill it!\n");
		return 0;
	}

	_fgForce3D = (UINT8)StrToInt(szArgv[1]);
	_bForce3DStructure = (UINT8)StrToInt(szArgv[2]);
	_bForce3DExtData = (UINT8)StrToInt(szArgv[3]);
	
	_bDviChkState = 2;  // DVI_WAIT_STABLE
    vDviInitial();
	
    #endif
	return 0;
}
//#endif 

