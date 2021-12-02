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
/*****************************************************************************
*  SMC: Diagnostic command
*****************************************************************************/

#ifdef __MW_CLI_DEF__
#include "x_mid_cli.h"
#else
#include "x_drv_cli.h"
#endif

#include "x_stl_lib.h"

#define DEFINE_IS_LOG	CLI_IsLog
#include "x_debug.h"

#include "x_os.h"
#include "x_bim.h"
#include "x_hal_5381.h" //#include "x_hal_5371.h"

#include "smc_drvif.h"    //#include "../5371/smc/smc_drv.h"
#include "smc_emu.h"
#include "smc_if.h" 

#ifdef CC_FPGA  
EXTERN BOOL fgSmcFpgaEmuMode;
#endif


EXTERN void _SMC_SetStatus (SMC_CARD_STATUS_T rStatus);

/******************************************************************************
* Local definition
******************************************************************************/


/******************************************************************************
* Function prototype
******************************************************************************/
static INT32 _SmcEmuCmdRegTest (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _SmcEmuCmdIntTest (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _SmcEmuCmdActTest (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _SmcEmuCmdTimeTest (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _SmcEmuCmdT0Test (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _SmcEmuCmdT1Test (INT32 i4Argc, const CHAR ** szArgv);

static INT32 _SmcCmdResetReg (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _SmcCmdInit (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _SmcCmdResetModule (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _SmcCmdWrite (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _SmcCmdAct (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _SmcCmdDeact (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _SmcCmdReset (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _SmcCmdWarmReset (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _SmcCmdGetCardId(INT32 i4Argc, const CHAR * * szArgv);
static INT32 _SmcCmdInitialSettings(INT32 i4Argc, const CHAR * * szArgv);
static INT32 _SmcCmdIFS(INT32 i4Argc, const CHAR * * szArgv);
static INT32 _SmcCmdInitHotPlug (INT32 i4Argc, const CHAR ** szArgv);

static INT32 _SmcCmdBCASTest(INT32 i4Argc, const CHAR * * szArgv);

/******************************************************************************
* CLI Command Table
******************************************************************************/
CLIMOD_DEBUG_FUNCTIONS(SMC)


static CLI_EXEC_T _arSmcEmuCmdTbl[] =
{
 	{"Register  ",	"1",	_SmcEmuCmdRegTest, NULL,	"[SMC]<Emu> Register Test",         CLI_SUPERVISOR},
 	{"Interrupt ",	"2",	_SmcEmuCmdIntTest, NULL,	"[SMC]<Emu> Interrupt Test",        CLI_SUPERVISOR},
 	{"Activate  ",	"3",	_SmcEmuCmdActTest, NULL,	"[SMC]<Emu> Act/Deact/ATR/PPS Test",CLI_SUPERVISOR},
 	{"Timer     ",	"4",	_SmcEmuCmdTimeTest, NULL,	"[SMC]<Emu> Timer Test",            CLI_SUPERVISOR},
 	{"T0Command ",	"5",	_SmcEmuCmdT0Test, NULL,	    "[SMC]<Emu> T=0 Command Test",      CLI_SUPERVISOR},
 	{"T1Command ",	"6",	_SmcEmuCmdT1Test, NULL,	    "[SMC]<Emu> T=1 Command Test",      CLI_SUPERVISOR},

 	{NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};


static CLI_EXEC_T _arSmcCmdTbl[] =
{
 	{"emu",			"e",	NULL, _arSmcEmuCmdTbl,		"SmartCard emulation", CLI_SUPERVISOR},
 	{"reset_reg",	"rg",	_SmcCmdResetReg, NULL,		"SmartCard reset REGs to dfl values", CLI_SUPERVISOR},
 	{"init",		"i",	_SmcCmdInit, NULL,		    "SmartCard initialize driver", CLI_SUPERVISOR},
 	{"resetmodule",		"rm",	_SmcCmdResetModule, NULL,		    "SmartCard reset module", CLI_SUPERVISOR},
 	{"write",		"w",	_SmcCmdWrite, NULL,		    "Write a command to SmartCard", CLI_SUPERVISOR},
 	{"act",		"a",	_SmcCmdAct, NULL,		    "SmartCard act", CLI_SUPERVISOR},
 	{"deact",		"d",	_SmcCmdDeact, NULL,		    "SmartCard deact", CLI_SUPERVISOR}, 	
 	{"reset",		"r",	_SmcCmdReset, NULL,		    "SmartCard reset", CLI_SUPERVISOR}, 	 	
 	{"warmreset",		"wr",	_SmcCmdWarmReset, NULL,		    "SmartCard warm reset", CLI_SUPERVISOR}, 	 	 	
 	{"getcardid",		"gc",	_SmcCmdGetCardId, NULL,		    "SmartCard ID", CLI_SUPERVISOR}, 	 	 	
 	{"initialsettings",		"is",	_SmcCmdInitialSettings, NULL,		    "SmartCard initial settings", CLI_SUPERVISOR}, 	 	 	 	
 	{"ifsd",		"ifs",	_SmcCmdIFS, NULL,		    "SmartCard IFSD", CLI_SUPERVISOR}, 	 	 	 	 	
 	{"inithp",		"ihp",	_SmcCmdInitHotPlug, NULL,		    "SmartCard hotplug enable", CLI_SUPERVISOR}, 	  	

 	{"bcastest",		"btest",	_SmcCmdBCASTest, NULL,		    "SmartCard BCAS test", CLI_SUPERVISOR}, 	  	
 	
	CLIMOD_DEBUG_CLIENTRY(SMC),

 	{NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};


/******************************************************************************
* Local Function
******************************************************************************/
static INT32 _SmcEmuCmdRegTest (INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i, i4Loops;
    BOOL  bRet=FALSE;

    if(i4Argc != 2)
    {
        Printf("Usage: %s <loops_count>\n", szArgv[0]);
        return 0;
    }

#ifdef CC_FPGA      
    fgSmcFpgaEmuMode = TRUE;
#endif

    i4Loops = StrToInt(szArgv[1]);

    Printf("[SMC]<Emu> SMC Register R/W Test ... \n");

    // Default Value
    Printf("    (1-a) Register Default Value Test : ");
    for(i=0; i<i4Loops; i++)
    {
        bRet = SmcRegDflTest();
        if(bRet == FALSE)
        {
            break;
        }
    }
    Printf( bRet ? "PASS\n" : "FAIL\n" );

    // Read / Write
    Printf("    (1-b) Register Read/Write Test    : ");
    for(i=0; i<i4Loops; i++)
    {
        bRet = SmcRegRwTest();
        if(bRet == FALSE)
        {
            break;
        }
    }
    Printf( bRet ? "PASS\n" : "FAIL\n" );

#ifdef CC_FPGA  
    fgSmcFpgaEmuMode = FALSE;
#endif

    return 0;
}


static INT32 _SmcEmuCmdIntTest (INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i, i4Loops;
    BOOL  bRet=FALSE;

    if(i4Argc != 2)
    {
        Printf("Usage: %s <loops_count>\n", szArgv[0]);
        return 0;
    }

#ifdef CC_FPGA  
    fgSmcFpgaEmuMode = TRUE;
#endif

    i4Loops = StrToInt(szArgv[1]);

    // Register ISR
    SmcInitDrv();

    Printf("[SMC]<Emu> Please use T=0 Card to do this test.\n");
    Printf("[SMC]<Emu> SMC Interrupt Test ... ");

    for(i=0; i<i4Loops; i++)
    {
        bRet = SmcIntTest();
        if(bRet == FALSE)
        {
            break;
        }
    }

    Printf( bRet ? "PASS\n" : "FAIL\n" );
    
#ifdef CC_FPGA  
    fgSmcFpgaEmuMode = FALSE;
#endif

    return 0;
}


static INT32 _SmcEmuCmdActTest (INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i, i4Loops;
    BOOL  bRet=FALSE;
    UINT32 u4Mask = 0x2F;

    if(i4Argc < 2)
    {
        Printf("Usage: %s <loops_count> <item_mask>\n", szArgv[0]);
        return 0;
    }


    if(i4Argc == 3)
    {
        u4Mask = StrToInt(szArgv[2]);
    }

#ifdef CC_FPGA  
    fgSmcFpgaEmuMode = TRUE;
#endif

    i4Loops = StrToInt(szArgv[1]);

    // Register ISR
    SmcInitDrv();

    Printf("[SMC]<Emu> SMC Activation Test ... \n");

    // Deactivation
    if(u4Mask & 0x1)
    {
        Printf("    (3-1) Deactivation Test     : ");
        for(i=0; i<i4Loops; i++)
        {
            bRet = SmcDeactTest();
            if(bRet == FALSE)
            {
                break;
            }
        }
        if(bRet)
        {
            Printf( "PASS\n");        
        }
        else
        {
            Printf( "FAIL at loop %d\n", i);        
        }
    }
    
    // Activation
    if(u4Mask & 0x2)
    {    
        Printf("    (3-2) Activation Test       : ");
        for(i=0; i<i4Loops; i++)
        {
            bRet = SmcActTest();
            if(bRet == FALSE)
            {
                break;
            }
        }
        if(bRet)
        {
            Printf( "PASS\n");        
        }
        else
        {
            Printf( "FAIL at loop %d\n", i);        
        }
    }
    
    // ATR Timeout
    if(u4Mask & 0x4)
    {    
        Printf("    (3-3) Timeout Test          : ");
        for(i=0; i<i4Loops; i++)
        {
            bRet = SmcToutTest();
            if(bRet == FALSE)
            {
                break;
            }
        }
        if(bRet)
        {
            Printf( "PASS\n");        
        }
        else
        {
            Printf( "FAIL at loop %d\n", i);        
        }
    }

    // Direction
    if(u4Mask & 0x8)
    {      
        Printf("    (3-4) Direct/Indirect Test  : ");
        for(i=0; i<i4Loops; i++)
        {
            bRet = SmcDirTest();
            if(bRet == FALSE)
            {
                break;
            }
        }
        if(bRet)
        {
            Printf( "PASS\n");        
        }
        else
        {
            Printf( "FAIL at loop %d\n", i);        
        }
    }
    
    // PPS
    if(u4Mask & 0x10)
    {          
        Printf("    (3-5) PPS Test              : ");
        for(i=0; i<i4Loops; i++)
        {
            bRet = SmcPpsTest();
            if(bRet == FALSE)
            {
                break;
            }
        }
        if(bRet)
        {
            Printf( "PASS\n");        
        }
        else
        {
            Printf( "FAIL at loop %d\n", i);        
        }
    }

    if(u4Mask & 0x20)
    {       
        // Stop clock level
        Printf("    (3-6) Stop Clock Level Test : ");
        for(i=0; i<i4Loops; i++)
        {
            bRet = SmcStopClkTest();
            if(bRet == FALSE)
            {
                break;
            }
        }
        if(bRet)
        {
            Printf( "PASS\n");        
        }
        else
        {
            Printf( "FAIL at loop %d\n", i);        
        }
    }
    
#ifdef CC_FPGA  
    fgSmcFpgaEmuMode = FALSE;
#endif

    return 0;
}


static INT32 _SmcEmuCmdTimeTest (INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i, i4Loops;
    BOOL  bRet=FALSE;

    if(i4Argc != 2)
    {
        Printf("Usage: %s <loops_count>\n", szArgv[0]);
        return 0;
    }

#ifdef CC_FPGA  
    fgSmcFpgaEmuMode = TRUE;
#endif

    i4Loops = StrToInt(szArgv[1]);

    // Register ISR
    SmcInitDrv();

    Printf("[SMC]<Emu> Timer Test ... ");

    for(i=0; i<i4Loops; i++)
    {
        bRet = SmcTimerTest();
        if(bRet == FALSE)
        {
            break;
        }
    }

    Printf( bRet ? "PASS\n" : "FAIL\n" );

#ifdef CC_FPGA  
    fgSmcFpgaEmuMode = FALSE;
#endif

    return 0;
}


static INT32 _SmcEmuCmdT0Test (INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i, i4Loops;
    BOOL  bRet=FALSE;

    if(i4Argc != 2)
    {
        Printf("Usage: %s <loops_count>\n", szArgv[0]);
        return 0;
    }

#ifdef CC_FPGA  
    fgSmcFpgaEmuMode = TRUE;
#endif

    i4Loops = StrToInt(szArgv[1]);

    // Register ISR
    SmcInitDrv();

    Printf("[SMC]<Emu> T=0 Command Test ... ");

    for(i=0; i<i4Loops; i++)
    {
        bRet = SmcT0Test();
        if(bRet == FALSE)
        {
            break;
        }
    }

    Printf( bRet ? "PASS\n" : "FAIL\n" );

#ifdef CC_FPGA  
    fgSmcFpgaEmuMode = FALSE;
#endif

    return 0;
}


static INT32 _SmcEmuCmdT1Test (INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i, i4Loops;
    BOOL  bRet=FALSE;

    if(i4Argc != 2)
    {
        Printf("Usage: %s <loops_count>\n", szArgv[0]);
        return 0;
    }

#ifdef CC_FPGA  
    fgSmcFpgaEmuMode = TRUE;
#endif

    i4Loops = StrToInt(szArgv[1]);

    // Register ISR
    SmcInitDrv();

    Printf("[SMC]<Emu> T=1 Command Test ... ");

    for(i=0; i<i4Loops; i++)
    {
        bRet = SmcT1Test();
        if(bRet == FALSE)
        {
            break;
        }
    }

    Printf( bRet ? "PASS\n" : "FAIL\n" );

#ifdef CC_FPGA  
    fgSmcFpgaEmuMode = FALSE;
#endif

    return 0;
}


static INT32 _SmcCmdResetReg (INT32 i4Argc, const CHAR ** szArgv)
{
    SmcRegSetDfl();

	return 0;
}


static INT32 _SmcCmdInit (INT32 i4Argc, const CHAR ** szArgv)
{
    //SmcInitDrv();
    SMC_Init();
	return 0;
}


static INT32 _SmcCmdResetModule (INT32 i4Argc, const CHAR ** szArgv)
{
    SmcResetModule();

	return 0;
}

static INT32 _SmcCmdAct (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8   pucATR[ATR_BUF_LEN];
    UINT32  i, u4ATRLen;

    UNUSED(SmcActCard(pucATR, ATR_BUF_LEN, &u4ATRLen));

    _SMC_SetStatus (SMC_CARD_ACTIVATED);

    Printf("ATR = ");
    for(i=0; i<u4ATRLen; i++)
    {
        Printf("%02X ", pucATR[i]);
    }
    Printf("\n");

    return 0;
}

static INT32 _SmcCmdDeact (INT32 i4Argc, const CHAR ** szArgv)
{
    SmcDeactCard();

    _SMC_SetStatus (SMC_CARD_DEACTIVATED);

    return 0;
}

static INT32 _SmcCmdReset (INT32 i4Argc, const CHAR ** szArgv)
{
    SmcDeactCard();
    
    return 0;
}

static INT32 _SmcCmdWarmReset (INT32 i4Argc, const CHAR ** szArgv)
{
//    SmcWarmRst();
    
    return 0;
}

static INT32 _SmcCmdWrite (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32  i4T, i;
	//UINT8   txData[100]={0x00, 0xc1, 0x01, 0x75};
	UINT8   txData[100];
	UINT8   result[100];
	UINT16  rcvSize;
	UINT8   Error;
	UINT16  u2SW1SW2,txSize;

    x_memset( txData, 0, 100);
    x_memset( result, 0, 100);

    if(i4Argc < 3)
    {
        Printf("Usage: %s <T=0|1> <Command ...>\n", szArgv[0]);
        return 0;
    }

    i4T = StrToInt(szArgv[1]);
    txSize = (i4Argc-2);
    
    for(i=0; i<txSize/*i4Argc-2*/; i++)
    {
        //txData[i] = StrToHex(szArgv[i+2], x_strlen(szArgv[i+2]));
        txData[i] = StrToInt(szArgv[i+2]);
    }


    // Register ISR
    SmcInitDrv();

    if(i4T==0)
    {
        u2SW1SW2=SmcT0Cmd(txData, txSize, result, &rcvSize, &Error);
        Printf("SmcT0Cmd txData, txSize=%d, result, rcvSize=%d, Error=%d ret u2SW1SW2=%04X\n", txSize, rcvSize, Error, u2SW1SW2);
        Printf("<== ");
        for(i=0; i<rcvSize; i++)
        {
            Printf("%02X ", result[i]);
        }
        Printf("\n");
    }
    else if(i4T==1)
    {
        //SmcT1Cmd(txData, 4, result, &rcvSize, &Error);
        u2SW1SW2=SmcT1Cmd(txData, txSize, result, &rcvSize, &Error);
        Printf("SmcT1Cmd txData, txSize=%d, result, rcvSize=%d, Error=%d ret u2SW1SW2=%04X\n", txSize, rcvSize, Error, u2SW1SW2 );
        Printf("<== ");
        for(i=0; i<rcvSize; i++)
        {
            Printf("%02X ", result[i]);
        }
        Printf("\n");
    }
    else
    {
        Printf("Usage: %s <T=0|1> <Command ...>\n", szArgv[0]);
        return 0;
    }

	return 0;
}

static INT32 _SmcCmdGetCardId(INT32 i4Argc, const CHAR * * szArgv)
{
    UINT8 au1CardId[10];
    UINT32 i;
    
    if(SMC_BCAS_GetCardId(au1CardId) != SMCR_OK)
    {
        return -1024;
    }
    Printf("Card ID:\n");    
    for(i = 0; i < 10; i++)
    {
        Printf("0x%2X ", au1CardId[i]);    
    }
    Printf("\n");        	
        
    return 0;
}

static INT32 _SmcCmdInitialSettings(INT32 i4Argc, const CHAR * * szArgv)
{
    UINT32 i;
    SMC_BCAS_CARD_INFO_T rCardInfo;
    	
    if(SMC_BCAS_InitialSettings(&rCardInfo) != SMCR_OK)
    {
        return -1024;
    }

    Printf("CA system ID:\n%d\n", rCardInfo.ca_system_id);
        
    Printf("Card ID:\n");
    for(i = 0; i < 6; i++)
    {
        Printf("0x%2X ", rCardInfo.card_id[i]);
    }

    Printf("\nSystem Key:\n");
    for(i = 0; i < 32; i++)
    {
        Printf("0x%2X ", rCardInfo.system_key[i]);
    }

    Printf("\nInitial Vector:\n");
    for(i = 0; i < 8; i++)
    {
        Printf("0x%2X ", rCardInfo.cbc_initial_vector[i]);
    }
    Printf("\n");     
    
    return 0;
}

static INT32 _SmcCmdIFS(INT32 i4Argc, const CHAR * * szArgv)
{
    UINT8 u1IFS = 254;

    if(i4Argc >= 2)
    {
        u1IFS = StrToInt(szArgv[1]);
    }
    
    if(SMC_RequestIFS(u1IFS) != SMCR_OK)
    {
        return -1024;
    }
    Printf("Set IFS to %d.\n", u1IFS);
    
    return 0;
}


static void _HotPlugCb(UINT8 u1HotPlugStatus)
{
    if(u1HotPlugStatus)
    {
        LOG(5, "SMC is plugged!\n");
    }
    else
    {
        LOG(5, "SMC is un-plugged!\n");    
    }
}

static INT32 _SmcCmdInitHotPlug (INT32 i4Argc, const CHAR ** szArgv)
{
    //SmcInitDrv();
    SMC_SetHotPlugCb(_HotPlugCb);
    return 0;
}

static INT32 _SmcCmdBCASTest (INT32 i4Argc, const CHAR ** szArgv)
{
    static SMC_BCAS_CARD_INFO_T rCardInfo;
    static SMC_BCAS_CARD_INFO_T rPrevCardInfo;    
    UINT32 u4Loop = 10;
    UINT32 u4SubLoop = 100;    
    UINT32 i = 0, j;

    if(i4Argc < 3)
    {
        return -1024;
    }

    u4Loop = StrToInt(szArgv[1]);
    u4SubLoop = StrToInt(szArgv[2]);   	

    Printf("Loop %d cold reset.\n", i);    
 
    if(_SMC_Reactivate() != 0)
    {
        Printf("Activate fail at loop %d\n!", i);
        return -1024;
    }

    if(SMC_RequestIFS(0x80) != SMCR_OK)
    {
        Printf("Request IFS fail at loop %d\n!", i);    
        return -1024;
    }

    if(SMC_BCAS_InitialSettings(&rPrevCardInfo) != 0)
    {
        Printf("Initial settings fail at loop %d\n!", i);    
        return -1024;
    }

    for(i = 1; i < u4Loop; i++)
    {
        for(j = 0; j < u4SubLoop; j++)
        {
            if(SMC_BCAS_InitialSettings(&rCardInfo) != 0)
            {
                Printf("Initial settings fail at loop %d, sub loop %d\n!", i, j);                
                return -1024;
            }
            if(x_memcmp(&rCardInfo.card_id, rPrevCardInfo.card_id, 6) != 0 ||
            	x_memcmp(&rCardInfo.system_key, rPrevCardInfo.system_key, 32) != 0 ||
            	x_memcmp(&rCardInfo.cbc_initial_vector, rPrevCardInfo.cbc_initial_vector, 8) != 0)
            {
                Printf("Initial settings compare fail at loop %d, sub loop %d\n!", i, j);                
                return -1024;                
            }
        }

        Printf("Loop %d cold reset\n!", i);    

        if(_SMC_Reactivate() != 0)
        {
            Printf("Activate fail at loop %d", i);
            return -1024;
        }

        if(SMC_RequestIFS(0x80) != SMCR_OK)
        {
            Printf("Request IFS fail at loop %d\n!", i);    
            return -1024;
        }
        
    }
    return 0;
}

/******************************************************************************
* Global Function
******************************************************************************/
CLI_MAIN_COMMAND_ITEM(smc)
{
    "smc", 
    NULL, 
    NULL, 
    _arSmcCmdTbl, 
    "Smc command", 
    CLI_GUEST
};

