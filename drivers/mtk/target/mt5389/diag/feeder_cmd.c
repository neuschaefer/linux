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
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: feeder_cmd.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file feeder_cmd.c
 *  This file contains implementation of CLI CMD for Feeder
 *
 */


//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

/*lint -save -e534 -e715 -e786 -e818 -e826 -e830 -e950 -e957 -e960 -e641
             -e613 -e836 */

#include "x_drv_cli.h"
#include "x_stl_lib.h"
#include "x_printf.h"
#include "x_assert.h"
#include "x_hal_926.h"
#include "x_hal_5381.h"
#include "x_os.h"
#include "feeder_if.h"

#define DEFINE_IS_LOG	FEEDER_IsLog
#include "x_debug.h"

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------
// t32 host command
enum
{
	E_T32_NOP,
	E_T32_EXEC,
	E_T32_RESTART,
	E_T32_EXIT,
	E_T32_EXEC_RELOAD_SYM,
	E_T32_DETECT_HOST,
	E_T32_END
};



//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------
typedef enum _ENUM_CLI_FEEDER_ERR_CODE_T
{
    E_CLI_FEEDER_OK                = 0,
    E_CLI_FEEDER_ERR_GENERAL    = 1000,
    E_CLI_FEEDER_ERR_PARAM        = 1001,
    E_CLI_FEEDER_ERR_MEM		=1002,
    // add here
    E_CLIJPG_UNKNOWN        = 1999
} ENUM_CLI_FEEDER_ERR_CODE_T;

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------

CLIMOD_DEBUG_FUNCTIONS(FEEDER)

#define FEEDER_CACHE_SIZE_ALIGNMENT (16*1024)
//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------
static UINT32 _u4FeederCLICacheSize[FEEDER_SOURCE_TOTAL] = {0, 0, 0, 0, 0};

//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------

static INT32 _FeedSetCache(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _FeedgetCache(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _FeedflushCache(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _FeederQuery(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _FeederDataDelay(INT32 i4Argc, const CHAR ** szArgv);
//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------


// cmd table
static CLI_EXEC_T _arFeederCmdTbl[] =
{
    CLIMOD_DEBUG_CLIENTRY(FEEDER),     // for LOG use
    {"setcache",        "stche",    _FeedSetCache, NULL,        "set feeder cli cache value", CLI_SUPERVISOR},
    {"getcache",        "gtche",    _FeedgetCache, NULL,        "get feeder cli cache value", CLI_SUPERVISOR},    
    {"flushcache",        "fche",    _FeedflushCache, NULL,        "get feeder cli cache value", CLI_SUPERVISOR},     
    {"query",                "q",    _FeederQuery, NULL,        "query feeder buffer info", CLI_SUPERVISOR},    
    {"delay",             "delay",_FeederDataDelay, NULL,        "data ack delay time,monitor network application delay", CLI_SUPERVISOR},    
    {NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};


//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------

static INT32 _FeederDataDelay(INT32 i4Argc, const CHAR ** szArgv)
{
   FeederSourceType eSourceType;
   UINT32 u4DelayTime;
   if(i4Argc>=3)
   {
      eSourceType=(FeederSourceType)StrToInt(szArgv[1]);
	  u4DelayTime=StrToInt(szArgv[2]);
   }
   else if(i4Argc==2)
   {
      eSourceType=FEEDER_PROGRAM_SOURCE;
	  u4DelayTime=StrToInt(szArgv[1]);
   }
   else
   {
      LOG(0,"feeder.delay [source] time\n");
	  return 0;
   }
   FeederSetDataDelay(eSourceType,u4DelayTime);
   return 0;
}

static INT32 _FeederQuery(INT32 i4Argc, const CHAR ** szArgv)
{
	FeederSourceType eDataType;
	FEEDER_BUF_INFO_T rBuffInfo;
	INT32 i4Ret=0;
    for(eDataType=FEEDER_AUDIO_SOURCE;eDataType<FEEDER_SOURCE_TOTAL;eDataType++)
    {
       i4Ret=FeederGetBufferInfo(eDataType,&rBuffInfo);
	   if(i4Ret==FEEDER_E_FAIL)
	   {
          continue;
	   }
	   LOG(3,"\nFeeder(%d):Ring=%d,StartAdd=0x%x,Size=0x%x,EndAddr=0x%x\n",eDataType,rBuffInfo.fgRingBuf,rBuffInfo.u4StartAddr,\
	   	      rBuffInfo.u4Size,rBuffInfo.u4EndAddr);
	   LOG(3,"WriteAddr=0x%x,ReadAddr=0x%x,ByteInFeeder=0x%x,UnAckCount=0x%x\n",rBuffInfo.u4WriteAddr,\
	   	      rBuffInfo.u4ReadAddr,rBuffInfo.u4BytesInBuf,rBuffInfo.u4UnAckNum);
    }

	return 0;
}
UINT32 FeederCLIGetCacheSize(FeederSourceType eDataType)
{
    if(eDataType>=FEEDER_SOURCE_TOTAL)
    {
        LOG(5, "Invalid feeder source type \n");
        return (E_CLI_FEEDER_ERR_PARAM);
    }
    return _u4FeederCLICacheSize[eDataType];
}

INT32 FeederCLISetCacheSize(FeederSourceType eDataType, UINT32 u4Size)
{

    if((eDataType>=FEEDER_SOURCE_TOTAL))
    {
        LOG(5, "Invalid feeder source type \n");
        return (E_CLI_FEEDER_ERR_PARAM);
    }    
    
    if((u4Size==0)||(u4Size>FEEDER_CACHED_MAX_READ_SIZE))
    {
        LOG(5, "Invalid feeder cache size \n");
        return (E_CLI_FEEDER_ERR_PARAM);
    }
    
    _u4FeederCLICacheSize[eDataType] = u4Size;

    return FEEDER_E_OK;
}

static INT32 _FeedSetCache(INT32 i4Argc, const CHAR ** szArgv)
{
        FeederSourceType    eDataType;
        UINT32 u4Size;

	if (i4Argc > 3)
	{ 
                return E_CLI_FEEDER_ERR_PARAM;
	}
	else
	{
		if((szArgv == NULL) || (szArgv[1]==NULL) || (szArgv[2]==NULL))
		{
			return E_CLI_FEEDER_ERR_PARAM;
		}

              if(StrToInt(szArgv[1])>=FEEDER_SOURCE_TOTAL)
              {
                    LOG(5, "FeederSourceType is larger than FEEDER_SOURCE_TOTAL[%d]\n",FEEDER_SOURCE_TOTAL);
                    return E_CLI_FEEDER_ERR_PARAM;
              }

              if(StrToInt(szArgv[2])>=FEEDER_CACHED_MAX_READ_SIZE)
              {
                    LOG(5,"Cache size is larger than FEEDER_CACHED_MAX_READ_SIZE[%d]\n",FEEDER_CACHED_MAX_READ_SIZE);
                    return E_CLI_FEEDER_ERR_PARAM;
              }

              if((StrToInt(szArgv[2])&(FEEDER_CACHE_SIZE_ALIGNMENT-1))!=0)
              {
                    LOG(5,"Cache size must be 16KB alignment\n");
                    return E_CLI_FEEDER_ERR_PARAM;
              }
              
		eDataType = (FeederSourceType)StrToInt(szArgv[1]);
		u4Size    = (UINT32)StrToInt(szArgv[2]);

              if(FeederCLISetCacheSize(eDataType, u4Size))
              {
                    return E_CLI_FEEDER_ERR_GENERAL;
              }
              else
              {
                    LOG(5,"\nSet Feeder cache type: [%d]  size:[%d] success.\n",eDataType, u4Size);
              }
            
	}
	
	return (INT32)E_CLI_FEEDER_OK;
}

static INT32 _FeedgetCache(INT32 i4Argc, const CHAR ** szArgv)
{
        UINT32 i;

	if (i4Argc > 2)
	{ 
                return E_CLI_FEEDER_ERR_PARAM;
	}
	else
	{
		if((szArgv == NULL))
		{
			return E_CLI_FEEDER_ERR_PARAM;
		}
              
              for(i=0;i<FEEDER_SOURCE_TOTAL;i++)
              {
                LOG(5,"Feeder cli cache src:[%d] size: [%d]  \n", i, _u4FeederCLICacheSize[i] );
              }
	}
	
	return (INT32)E_CLI_FEEDER_OK;
}

static INT32 _FeedflushCache(INT32 i4Argc, const CHAR ** szArgv)
{
        UINT32 i;

        for(i=0;i<FEEDER_SOURCE_TOTAL;i++)
        {
          _u4FeederCLICacheSize[i] =0;
        }

       LOG(5,"flush cache success. \n");
       
	return (INT32)E_CLI_FEEDER_OK;
}

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

LINT_SAVE_AND_DISABLE
CLI_MAIN_COMMAND_ITEM(Feeder)
{
    "feeder",
    NULL,
    NULL,
    _arFeederCmdTbl,
    "FEEDER command",
    CLI_SUPERVISOR
};
LINT_RESTORE

/*lint -restore */



