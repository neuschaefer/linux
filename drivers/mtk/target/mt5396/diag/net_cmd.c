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

#include "net_if.h"
#ifdef __MW_CLI_DEF__
#include "x_mid_cli.h"
#else
#include "x_drv_cli.h"
#endif

#include "net_cmd.h"
#include "x_printf.h"
#include "x_stl_lib.h"

#define DEFINE_IS_LOG	CLI_IsLog
#include "x_debug.h"

/******************************************************************************
* Function prototype
******************************************************************************/

/******************************************************************************
* Ethernet CLI Command Functions
******************************************************************************/
static INT32 _EtherInitCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;
    
    UNUSED(i4Argc);
    UNUSED(szArgv);

    i4Ret = _i4EtherInit();
    
	return (i4Ret);
}

static INT32 _EtherDeInitCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;
    
    UNUSED(i4Argc);
    UNUSED(szArgv);

    i4Ret = _i4EtherDeInit();
    
	return (i4Ret);
}

static INT32 _EtherStartCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;
    
    UNUSED(i4Argc);
    UNUSED(szArgv);

    i4Ret = _i4EtherStart();
    
	return (i4Ret);
}

static INT32 _EtherStopCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;
    
    UNUSED(i4Argc);
    UNUSED(szArgv);

    i4Ret = _i4EtherStop();
    
	return (i4Ret);
}

static INT32 _EtherSendFrame (INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;
    
    UNUSED(i4Argc);
    UNUSED(szArgv);

    i4Ret = _i4EtherSendFrame(i4Argc, szArgv);
    
	return (i4Ret);
}

static INT32 _EtherIoctlSet (INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;
    
    UNUSED(i4Argc);
    UNUSED(szArgv);

    i4Ret = _i4EtherOidSet(i4Argc, szArgv);
    
	return (i4Ret);
}

static INT32 _EtherIoctlGet (INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;
    
    UNUSED(i4Argc);
    UNUSED(szArgv);

    i4Ret = _i4EtherOidGet(i4Argc, szArgv);
    
	return (i4Ret);
}

static INT32 _EtherLoopBack (INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret, i4Enable;
    
	if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL))
	{
		Printf("Arg: enable:1 / disable:0\n");
		return 0;
	}
	i4Enable = (INT32) StrToInt(szArgv[1]);

    i4Ret = _i4EtherSetLoopBack(i4Enable);
    
	return (i4Ret);
}

static INT32 _EtherMacAddr(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;
    
    UNUSED(i4Argc);
    UNUSED(szArgv);

    i4Ret = _i4EtherMacAddr(i4Argc, szArgv);
    
    return (i4Ret);
}

static INT32 _EtherPromiscuous(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;
    
    UNUSED(i4Argc);
    UNUSED(szArgv);

    i4Ret = _i4EtherPromiscuous(i4Argc, szArgv);
    
    return (i4Ret);
}

static INT32 _EtherMulticast (INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;
    
    UNUSED(i4Argc);
    UNUSED(szArgv);

    i4Ret = _i4EtherMulticastList(i4Argc, szArgv);
    
    return (i4Ret);
}

static INT32 _EtherStatInfo (INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;
    
    UNUSED(i4Argc);
    UNUSED(szArgv);

    i4Ret = _i4EtherStatInfo(i4Argc, szArgv);
    
    return (i4Ret);
}

static INT32 _EtherReadPhyCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;
    UINT32 u4RegAddr;
    UINT32 u4Len=1;
    
	if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL))
	{
		Printf("Arg: reg\n");
		return 0;
	}

	u4RegAddr = (UCHAR) StrToInt(szArgv[1]);
	if(szArgv[2]!=NULL && (i4Argc > 2))
	{
	  u4Len = (UINT32)((UCHAR) StrToInt(szArgv[2]));
	}
    
    i4Ret = _i4EtherReadPhy(u4RegAddr, u4Len);
    
	return (i4Ret);
}

static INT32 _EtherWritePhyCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;
    UINT32 u4RegAddr, u4Val;
    
	if ((i4Argc < 3) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL))
	{
		Printf("Arg: reg, value\n");
		return 0;
	}

	u4RegAddr = (UINT32) StrToInt(szArgv[1]);
    u4Val = (UINT32) StrToInt(szArgv[2]);

    i4Ret = _i4EtherWritePhy(u4RegAddr, u4Val);
    
	return (i4Ret);
}


static INT32 _EtherSltTest (INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;
    
    UNUSED(i4Argc);
    UNUSED(szArgv);

    i4Ret = _i4EtherSltTest(i4Argc, szArgv);
    
    return (i4Ret);
}

static INT32 _EtherPaketTypeSet (INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;
    
    UNUSED(i4Argc);
    UNUSED(szArgv);

    i4Ret = _i4EtherSetTxPacketType(i4Argc, szArgv);
    
    return (i4Ret);
}

extern INT32  _i4EtherResetPhyTest(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _EtherResetPhyTest (INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;
    
    UNUSED(i4Argc);
    UNUSED(szArgv);

    i4Ret = _i4EtherResetPhyTest(i4Argc, szArgv);
    
    return (i4Ret);
}

extern INT32 fgPrintDes;
static INT32 _EtherRxPrint (INT32 i4Argc, const CHAR ** szArgv)
{
		INT32 i4Ret=0;
		
		UNUSED(i4Argc);
		UNUSED(szArgv);

	if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL) )
    {
        UTIL_Printf("value : print counter\n");
        return 0;
    }

		fgPrintDes =  (UINT8)StrToInt(szArgv[1]); 
		UTIL_Printf("print counter = %d\n",fgPrintDes);
		
		return (i4Ret);
}


static INT32 _EtherTxRxDMAPollingTest (INT32 i4Argc, const CHAR ** szArgv)
{
	extern INT32 _i4EtherDMAAutoPollingTest(INT32 i4Argc, const CHAR ** szArgv);

	INT32 i4Ret=0;
			
		UNUSED(i4Argc);
		UNUSED(szArgv);
	
		if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL))
		{
			UTIL_Printf("[0/1 ] \n");
			return 0;
		}
	
		i4Ret = _i4EtherDMAAutoPollingTest(i4Argc,szArgv);

			
		return (i4Ret);
}


static INT32 _EtherINTTest (INT32 i4Argc, const CHAR ** szArgv)
{
	extern INT32 _i4EtherINTTest(INT32 i4Argc, const CHAR ** szArgv);

	INT32 i4Ret=0;
			
		UNUSED(i4Argc);
		UNUSED(szArgv);
	
		if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL))
		{
			UTIL_Printf("[0/1: disable/enable] \n");
			return 0;
		}
	
		i4Ret = _i4EtherINTTest(i4Argc,szArgv);
			
		return (i4Ret);
}


static INT32 _EtherTxDesSet (INT32 i4Argc, const CHAR ** szArgv)
{
	extern   INT32 _i4EtherTxDesSet (INT32 i4Argc, const CHAR ** szArgv);

    _i4EtherTxDesSet( i4Argc,   szArgv);

	return 0;
}

static INT32 _EtherPauseFrmDecTest (INT32 i4Argc, const CHAR ** szArgv)
{
	extern   INT32 _i4EtherPauseFrmDecTest(INT32 i4Argc, const CHAR ** szArgv);

    _i4EtherPauseFrmDecTest( i4Argc,   szArgv);

	return 0;

}

static INT32 _EtherDbgLevel (INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;
    
    UNUSED(i4Argc);
    UNUSED(szArgv);

    i4Ret = _i4EtherDbgLevel(i4Argc, szArgv);
    
    return (i4Ret);
}

static INT32 _EtherPhyANTestCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;
//    UINT32 u4RegAddr, u4Val;
    
    UNUSED(i4Argc);
    UNUSED(szArgv);

    i4Ret = _i4EtherPhyANTest();
    
    return (i4Ret);
}

static INT32 _EtherDiag_AdjPhySignal(INT32 i4Argc, const CHAR ** szArgv)
{
  INT32 i4Ret;
    
  UNUSED(i4Argc);
  UNUSED(szArgv);	
  
  i4Ret = _i4EtherAdjPhySignal(i4Argc, szArgv);
    
  return (i4Ret);
  
}

INT32 _EtherDiag_GetPhySignalSetting(INT32 i4Argc, const CHAR ** szArgv)
{
  INT32 i4Ret;
    
  UNUSED(i4Argc);
  UNUSED(szArgv);	
  
  i4Ret = _i4EtherGetPhySignalSetting(i4Argc, szArgv);
    
  return (i4Ret);
}

static INT32 _EtherTxCheckSumOffLoadSet (INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;
    
    UNUSED(i4Argc);
    UNUSED(szArgv);

    i4Ret = _i4EtherSetTxCheckSumOffLoad(i4Argc, szArgv);
    
    return (i4Ret);
}

static INT32 _EtherWriteHashTable(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;
    
    UNUSED(i4Argc);
    UNUSED(szArgv);

    i4Ret = _i4EtherWriteHashTable(i4Argc, szArgv);
    
    return (i4Ret);
}


#if CONFIG_CHKSUM
static INT32 _CheckSumTestInitCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;
    
    UNUSED(i4Argc);
    UNUSED(szArgv);

    i4Ret = _i4CheckSumInit();
    
    return (i4Ret);
}

static INT32 _EtherTxChecksumTest (INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;
    
    UNUSED(i4Argc);
    UNUSED(szArgv);

    i4Ret = _i4EtherTxChkSumTest(i4Argc, szArgv);
    
    return (i4Ret);
}

static INT32 _EtherRxChecksumTest (INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;
    
    UNUSED(i4Argc);
    UNUSED(szArgv);

    i4Ret = _i4EtherRxChkSumTest(i4Argc, szArgv);
    
    return (i4Ret);
}


static INT32 _EtherTxRxChecksumTest(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;
    
    UNUSED(i4Argc);
    UNUSED(szArgv);

    i4Ret = _i4EtherTxRxChkSumTest(i4Argc, szArgv);
    
    return (i4Ret);
}


#if 0
/******************************************************************************
* CheckSum module CLI Command Functions
******************************************************************************/
static INT32 _ChkSumInitCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;

    UNUSED(i4Argc);
    UNUSED(szArgv);

    i4Ret = _i4CheckSumInit();
    
    return (i4Ret);
}

//FIX Build error
INT32 _i4ChkSumTest(SIZE_T zSize, UINT32 u4Count)
{
    return 0;
}

static INT32 _ChkSumTestCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;
    UINT32 u4Size, u4Count;

    if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL))
    {
	Printf("Arg: size, count\n");
	return 0;
    }

    u4Size = (UINT32) StrToInt(szArgv[1]);
    u4Count = (UINT32) StrToInt(szArgv[2]);

    i4Ret = _i4ChkSumTest(u4Size, u4Count);
    
    return (i4Ret);
}
#endif
#endif
static INT32 _EtherStopPhyPollingLoop(INT32 i4Argc, const CHAR ** szArgv)
{
	UINT8 u1Stop;
	
    if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL))
    {
        UTIL_Printf("Arg: 1: stop poolling 0:enable polling\n");
        return 0;
    }

    u1Stop = (UINT8) StrToInt(szArgv[1]);

    vStopSoftPollingPhy(u1Stop);
    
    return (0);
}

static INT32 _EtherReadRegister (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Offset, u4Len;
    if (i4Argc != 3)
    {
        Printf("Argument error!\n");
        Printf("readreg offset len\n");
        return 0;
    }
    u4Offset = (UINT32)StrToInt(szArgv[1]) & 0xfffffffc;
    u4Len = (UINT32)StrToInt(szArgv[2]);

    return (_i4EtherReadRegister(u4Offset, u4Len));
}

static INT32 _EtherWriteRegister (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Offset, u4Value;
    
    if (i4Argc != 3)
    {
        Printf("Argument error!\n");
        Printf("writereg offset value\n");
        return 0;
    }
    u4Offset = (UINT32)StrToInt(szArgv[1]) & 0xfffffffc;
    u4Value = (UINT32)StrToInt(szArgv[2]);

    return (_i4EtherWriteRegister(u4Offset, u4Value));
}

static INT32 _EtherGetBits (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Offset, u4MSB, u4LSB;

    if (i4Argc != 4)
    {
        Printf("Argument error!\n");
        Printf("getbits offset msb lsb\n");
        return 0;
    }
    u4Offset = (UINT32)StrToInt(szArgv[1]) & 0xfffffffc;
    u4MSB = (UINT32)StrToInt(szArgv[2]);
    u4LSB = (UINT32)StrToInt(szArgv[3]);

    return (_i4EtherGetBits(u4Offset, u4MSB, u4LSB));
}

static INT32 _EtherSetBits (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Offset, u4MSB, u4LSB, u4Value;

    if (i4Argc != 5)
    {
        Printf("Argument error!\n");
        Printf("setbits offset msb lsb value\n");
        return 0;
    }
    u4Offset = (UINT32)StrToInt(szArgv[1]) & 0xfffffffc;
    u4MSB = (UINT32)StrToInt(szArgv[2]);
    u4LSB = (UINT32)StrToInt(szArgv[3]);
    u4Value = (UINT32)StrToInt(szArgv[4]);

    return (_i4EtherSetBits(u4Offset, u4MSB, u4LSB, u4Value));
}

static INT32 _EtherPktInit (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Len;

    if (i4Argc != 2)
    {
        UTIL_Printf("Argument error!\n");
        UTIL_Printf("pktinit len\n");
        return 0;
    }
    u4Len = (UINT32)StrToInt(szArgv[1]);

    return (_i4EtherPktInit(u4Len));
}
static INT32 _EtherPktSet (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Offset;

    if (i4Argc <3)
    {
        UTIL_Printf("Argument error!\n");
        UTIL_Printf("pktset offset data...\n");
        return 0;
    }
    u4Offset = (UINT32)StrToInt(szArgv[1]);

    return (_i4EtherPktSet(u4Offset, i4Argc-2, szArgv+2));
}
static INT32 _EtherPktDump (INT32 i4Argc, const CHAR ** szArgv)
{
    if (i4Argc != 1)
    {
        UTIL_Printf("Argument error!\n");
        UTIL_Printf("pktdump\n");
        return 0;
    }

    return (_i4EtherPktDump());
}


/******************************************************************************
* M6S56 CLI Command Functions
******************************************************************************/
#if CONFIG_DTCP
static INT32 _M6s56InitCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;

    UNUSED(i4Argc);
    UNUSED(szArgv);

    i4Ret = _i4M6s56Init();
    
	return (i4Ret);
}
static INT32 _M6s56Encryption (INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;
    UINT32 u4Size, u4Count;
    
	if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL))
	{
		Printf("Arg: size, count\n");
		return 0;
	}

    u4Size = (UINT32) StrToInt(szArgv[1]);
    u4Count = (UINT32) StrToInt(szArgv[2]);
    i4Ret = _i4M6s56Encrypt(u4Size, u4Count);
    
    return (i4Ret);
}

static INT32 _M6s56Decryption (INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;
    UINT32 u4Size, u4Count;
    
	if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL))
	{
		Printf("Arg: size, count\n");
		return 0;
	}

    u4Size = (UINT32) StrToInt(szArgv[1]);
    u4Count = (UINT32) StrToInt(szArgv[2]);
    i4Ret = _i4M6s56Decrypt(u4Size, u4Count);
    
	return (i4Ret);
}

static INT32 _M6s56EncryptDecrypt (INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;
    UINT32 u4Size, u4Count;
    
	if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL))
	{
        Printf("Arg: size, count\n");
		return 0;
	}

    u4Size = (UINT32) StrToInt(szArgv[1]);
    u4Count = (UINT32) StrToInt(szArgv[2]);
    i4Ret = _i4M6s56EncryptDecrypt(u4Size, u4Count);
    
	return (i4Ret);
}
static INT32 _M6s56DmaMover (INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;
    UINT32 u4Size, u4Count;
    
	if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL))
	{
		Printf("Arg: size, count\n");
		return 0;
	}

    u4Size = (UINT32) StrToInt(szArgv[1]);
    u4Count = (UINT32) StrToInt(szArgv[2]);
    i4Ret = _i4M6s56DmaMove(u4Size, u4Count);

    return (i4Ret);
}

static INT32 _M6s56Chksum (INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;
    UINT32 u4Size, u4Count;
    
    if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL))
    {
        Printf("Arg: size, count\n");
        return 0;
    }

    u4Size = (UINT32) StrToInt(szArgv[1]);
    u4Count = (UINT32) StrToInt(szArgv[2]);
    i4Ret = _i4M6s56Chksum(u4Size, u4Count);
    
	return (i4Ret);
}

static INT32 _M6s56WholeTest (INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;
    UINT32 u4Count;
    
	if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL))
	{
        Printf("Arg: count\n");
		return 0;
	}

    u4Count = (UINT32) StrToInt(szArgv[1]);

    i4Ret = _i4M6s56WholeTest(u4Count);
    
	return (i4Ret);
}
#endif    

#if CONFIG_WLAN_WPD
/******************************************************************************
* WPD Wlan CLI Command Functions
******************************************************************************/
static INT32 _WlanInitCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;
    
    UNUSED(i4Argc);
    UNUSED(szArgv);

    i4Ret = _i4WlanInit();
    
	return (i4Ret);
}

static INT32 _WlanDeInitCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;
    
    UNUSED(i4Argc);
    UNUSED(szArgv);

    i4Ret = _i4WlanDeInit();
    
	return (i4Ret);
}

static INT32 _WlanStartCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;
    
    UNUSED(i4Argc);
    UNUSED(szArgv);

    i4Ret = _i4WlanStart();
    
	return (i4Ret);
}

static INT32 _WlanStopCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;
    
    UNUSED(i4Argc);
    UNUSED(szArgv);

    i4Ret = _i4WlanStop();
    
	return (i4Ret);
}

static INT32 _WlanSendFrame (INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;
    
    UNUSED(i4Argc);
    UNUSED(szArgv);

    i4Ret = _i4WlanSendFrame(i4Argc, szArgv);
    
	return (i4Ret);
}

static INT32 _WlanLoopBack (INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;
    
    UNUSED(i4Argc);
    UNUSED(szArgv);

    i4Ret = _i4WlanSetLoopBack();
    
	return (i4Ret);
}

static INT32 _WlanConnection (INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;

	if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == '\0'))
	{
		Printf("Arg: specify SSID\n");
	}
    
    i4Ret = _i4WlanConnection(szArgv[1]);
    
	return (i4Ret);
}

static INT32 _WlanDiscovery (INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;
    
    UNUSED(i4Argc);
    UNUSED(szArgv);

    i4Ret = _i4WlanDiscovery();
    
	return (i4Ret);
}

static INT32 _WlanIoctlSet (INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;
    
    UNUSED(i4Argc);
    UNUSED(szArgv);

    i4Ret = _i4WlanOidSet(i4Argc, szArgv);
    
	return (i4Ret);
}

static INT32 _WlanIoctlGet (INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;
    
    UNUSED(i4Argc);
    UNUSED(szArgv);

    i4Ret = _i4WlanOidGet(i4Argc, szArgv);
    
	return (i4Ret);
}

static INT32 _WlanSetRftestMode (INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;
    UINT8 ucEnable;
	
	if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL))
	{
		Printf("Arg: RFtest enable:1 / disable:0\n");
		return 0;
	}

	ucEnable = (UCHAR) StrToInt(szArgv[1]);

    i4Ret = _i4WlanSetRftestMode(ucEnable);
    
	return (i4Ret);
}

static INT32 _WlanSetRftestPktTx (INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;
    UINT8 ucChNum, ucTxRate;
    
	if ((i4Argc < 3) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL))
	{
		Printf("Arg: channel, rate\n");
	}

	ucChNum = (UCHAR) StrToInt(szArgv[1]);
    ucTxRate = (UCHAR) StrToInt(szArgv[2]);
    
    i4Ret = _i4WlanSetRftestPktTx(ucChNum, ucTxRate);
    
	return (i4Ret);
}

static INT32 _WlanSetRftestPktRx (INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;
    UINT8 ucChNum;
    
	if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL))
	{
		Printf("Arg: channel 1-14\n");
		return 0;
	}

	ucChNum = (UCHAR) StrToInt(szArgv[1]);

    i4Ret = _i4WlanSetRftestPktRx(ucChNum);
    
	return (i4Ret);
}
#endif // CONFIG_WLAN_WPD

/******************************************************************************
* CLI Command Table
******************************************************************************/
CLIMOD_DEBUG_FUNCTIONS(NET)

/* Ethernet CLI command table */
static CLI_EXEC_T _arNetEtherCmdTbl[] =
{
 	{"init",		"i",	_EtherInitCmd, NULL,		"ether init", CLI_GUEST},
 	{"deinit",		"di",	_EtherDeInitCmd, NULL,		"ether deinit", CLI_GUEST},
 	{"start",		NULL,	_EtherStartCmd, NULL,		"ether start", CLI_GUEST},
 	{"stop",		NULL,	_EtherStopCmd, NULL,		"ether stop", CLI_GUEST},
 	{"tx",			"t",	_EtherSendFrame, NULL,		"tx: [byte] [cnt] [delay] [compare]", CLI_GUEST},
 	{"ioset",		"s",	_EtherIoctlSet, NULL,		"OID set: [ID] [parm]", CLI_GUEST},
 	{"ioget",		"g",	_EtherIoctlGet, NULL,		"OID get: [ID] [parm]", CLI_GUEST},
 	{"mac",     	NULL,	_EtherMacAddr, NULL,		"r/w mac from/to flash: [xx:xx:xx:xx:xx:xx]", CLI_GUEST},
 	{"multicast",   "mc",	_EtherMulticast, NULL,		"r/w multicast: [xx:xx:xx:xx:xx:xx]", CLI_GUEST},
 	{"promiscuous", "pr",	_EtherPromiscuous, NULL,	"promiscuous mode: [1:on | 0:off]", CLI_GUEST},
 	{"stat",	    NULL,	_EtherStatInfo, NULL,		"statistics info: [0:reset]", CLI_GUEST},
 	{"loopbback",	"lb",	_EtherLoopBack, NULL,		"loopback mode", CLI_GUEST},
 	{"readPhy",		"rp",	_EtherReadPhyCmd, NULL,		"read phy: [addr]", CLI_GUEST},
 	{"writePhy",	"wp",	_EtherWritePhyCmd, NULL,	"write phy: [addr] [val]", CLI_GUEST},
 	{"slt",	        NULL,	_EtherSltTest, NULL,	    "slt phy loopback test", CLI_GUEST},
 	{"packet",	    "pa",	_EtherPaketTypeSet, NULL,	"Set packet Type: [type]", CLI_GUEST},
 	{"txchk",	    "txchk",_EtherTxCheckSumOffLoadSet, NULL,	"Set TX checksum offload: [enable]", CLI_GUEST},
#if (CC_MT5396)
	{"ResetPhyTest", "RPT",		_EtherResetPhyTest, NULL,		"reset phy test ", CLI_GUEST},
	{"ShowRxDesAndData", "rxp",	_EtherRxPrint, 		NULL,	    "print RX descriptor and rx data ", CLI_GUEST},
    {"TxRxDMAPollingTest", "trpt",	_EtherTxRxDMAPollingTest, NULL,	  "Tx Rx DMA Auto Polling Test ", CLI_GUEST},
    {"InterruptTest",	"intt",	_EtherINTTest, 		NULL, 		"Interrupt test", CLI_GUEST},
	{"TxDesSet",	 "TDS",		_EtherTxDesSet, 	NULL,		"Tx descriptor  set flag", CLI_GUEST},
	{"PauseFrmDecTest",	 "pdt", _EtherPauseFrmDecTest, NULL,	"pause frame decode	test", CLI_GUEST},
 	{"PhyANTest",	"AN",	_EtherPhyANTestCmd, NULL,	"Change phy adv 15 combiniation", CLI_GUEST},
#endif
 	{"whash",    "whash",	_EtherWriteHashTable, NULL,    "Write Hash Table", CLI_GUEST},
 	{"phyloop",    "phyloop",	_EtherStopPhyPollingLoop, NULL,    "Write Hash Table", CLI_GUEST},
	{"adjphy",      "adjp", _EtherDiag_AdjPhySignal,   NULL,  "Adjust PHY Signal (adjp [chipId] [adjtype] [val])", CLI_GUEST},
    {"getphy",      "gp",   _EtherDiag_GetPhySignalSetting,   NULL,  "Get PHY Signal Setting (gp [chipId] [gettype])", CLI_GUEST},
 	{"debug_level",	"dl",	_EtherDbgLevel, NULL,	    "debug level: [level]", CLI_GUEST},
#if CONFIG_CHKSUM
 	{"chki",		"chki",	_CheckSumTestInitCmd, NULL,		"Checksum init", CLI_GUEST},
 	{"Txchecksum",  "tcks",	_EtherTxChecksumTest, NULL,    "HW TX checksum test", CLI_GUEST},
 	{"Rxchecksum",  "rcks",	_EtherRxChecksumTest, NULL,    "HW Rx checksum test", CLI_GUEST},
 	{"RxTxchecksum","cks",	_EtherTxRxChecksumTest, NULL,    "HW Tx, Rx checksum test", CLI_GUEST},
#endif 	
    {"rr",          "readreg",  _EtherReadRegister, NULL,   "Read MAC register: offset len", CLI_GUEST },
    {"wr",          "writereg", _EtherWriteRegister, NULL,  "Write MAC register: offset value", CLI_GUEST },
    {"getb",        "getbits",  _EtherGetBits, NULL,        "Get MAC register bits: offset MSB LSB", CLI_GUEST },
    {"setb",        "setbits",  _EtherSetBits, NULL,        "Set MAC register bits: offset MSB LSB value", CLI_GUEST },
    {"pktinit",     "pkti",     _EtherPktInit, NULL,        "Init custom packet: len", CLI_GUEST },
    {"pktset",      "pkts",     _EtherPktSet, NULL,         "Set custom packet content: offset data...", CLI_GUEST },
    {"pktdump",     "pktd",     _EtherPktDump, NULL,        "Dump custom packet content: no arguments", CLI_GUEST },
    {"whash",    "whash",	_EtherWriteHashTable, NULL,    "Write Hash Table", CLI_GUEST},
 	{NULL, NULL, NULL, NULL, NULL, CLI_GUEST}
};

/* M6S56 CLI command table */
#if CONFIG_DTCP
static CLI_EXEC_T _arNetM6s56CmdTbl[] =
{
 	{"init",		"i",	_M6s56InitCmd, NULL,		"m6s56 module init", CLI_GUEST},
 	{"encrypt",		"e",	_M6s56Encryption, NULL,		"m6s56 encryption", CLI_GUEST},
 	{"decrypt",		"d",	_M6s56Decryption, NULL,		"m6s56 decryption", CLI_GUEST},
 	{"En/Decrypt",	"ed",	_M6s56EncryptDecrypt, NULL,	"encrypt/decrypt: [size] [cnt]", CLI_GUEST},
 	{"move",		"m",	_M6s56DmaMover, NULL,		"DMA mover: [size] [cnt]", CLI_GUEST},
 	{"chksum",		"cs",	_M6s56Chksum, NULL,		    "Checksum: [size] [cnt]", CLI_GUEST},
 	{"WholeTest",	"w",	_M6s56WholeTest, NULL,	    "encrypt/decrypt/mover: [cnt]", CLI_GUEST},

 	{NULL, NULL, NULL, NULL, NULL, CLI_GUEST}
};
#endif

#if CONFIG_WLAN_WPD
/* Wlan CLI command table */
static CLI_EXEC_T _arNetWlanCmdTbl[] =
{
 	{"init",		"i",	_WlanInitCmd, NULL,			"wlan init", CLI_GUEST},
 	{"deinit",		"di",	_WlanDeInitCmd, NULL,		"wlan deinit", CLI_GUEST},
 	{"start",		NULL,	_WlanStartCmd, NULL,		"wlan start", CLI_GUEST},
 	{"stop",		NULL,	_WlanStopCmd, NULL,			"wlan stop", CLI_GUEST},
 	{"connection",	"c",	_WlanConnection, NULL,		"connect AP: [SSID]", CLI_GUEST},
 	{"discovery",	"d",	_WlanDiscovery, NULL,		"scan AP", CLI_GUEST},
 	{"tx",			"t",	_WlanSendFrame, NULL,		"tx: [byte] [cnt] [delay]", CLI_GUEST},
 	{"ioset",		"s",	_WlanIoctlSet, NULL,		"OID set: [ID] [parm]", CLI_GUEST},
 	{"ioget",		"g",	_WlanIoctlGet, NULL,		"OID get: [ID] [parm]", CLI_GUEST},
 	{"loopbback",	"lb",	_WlanLoopBack, NULL,		"loopback mode", CLI_GUEST},
 	{"rftest",		"rf",	_WlanSetRftestMode, NULL,	"rftest mode", CLI_GUEST},
 	{"rftx",		"rt",	_WlanSetRftestPktTx, NULL,	"rftest pkt tx", CLI_GUEST},
 	{"rfrx",		"rr",	_WlanSetRftestPktRx, NULL,	"rftest pkt rx", CLI_GUEST},

 	{NULL, NULL, NULL, NULL, NULL, CLI_GUEST}
};
#endif

static CLI_EXEC_T _arNetCmdTbl[] =
{
#if CONFIG_WLAN_WPD
 	{"wlan",		NULL,	NULL, _arNetWlanCmdTbl,		"WPD Wlan command", CLI_GUEST},
#endif
 	{"ethernet",	"eth",	NULL, _arNetEtherCmdTbl,	"Ethernet command", CLI_GUEST},
#if CONFIG_DTCP
 	{"m6s56",	    "m6",	NULL, _arNetM6s56CmdTbl,	"M6-S56 command", CLI_GUEST},
#endif
	CLIMOD_DEBUG_CLIENTRY(NET),
 	
 	{NULL, NULL, NULL, NULL, NULL, CLI_GUEST}
};

//static CLI_EXEC_T _rNetModCmdTbl =
CLI_MAIN_COMMAND_ITEM(NET)
{
	"net",
	NULL,
	NULL,
	_arNetCmdTbl,
	"Net command",
	CLI_GUEST
};

/******************************************************************************
* Global Function
******************************************************************************/
