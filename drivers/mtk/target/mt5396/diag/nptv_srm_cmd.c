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
#include "fbm_drvif.h"

#ifdef __MW_CLI_DEF__
#include "x_mid_cli.h"
#else
#include "x_drv_cli.h"
#endif
#include "drv_nr.h"
#include "drv_ycproc.h"

#include "x_assert.h"
#include "x_debug.h"


#ifndef CC_CLI
#define CC_CLI
#endif

#ifdef CC_CLI
static INT32 _u4SRM_DummyTrigger(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u4SRM_SetDispMode(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u4SRM_ClearManSetting(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u4SRM_SetPscanMode(INT32 i4Argc, const CHAR **szArgv);
extern void SRM_TriggerCliSrm(UINT32 u4VdpId);
extern void SRM_CliSetDispMode(UINT32 u4VdpId, UINT32 u4Mode);
extern void SRM_ClearCliSetting(void);
#endif

// Table for Decoder command entry
CLI_EXEC_T arSrmCmdTbl[] = {
#ifdef CC_CLI
  {"send", "send", _u4SRM_DummyTrigger, NULL, "srm dummy trigger", CLI_GUEST},
  {"setdisp", "setdisp", _u4SRM_SetDispMode, NULL, "set disp mode", CLI_GUEST},
  {"clear", "clear", _u4SRM_ClearManSetting, NULL, "clear manual setting", CLI_GUEST},
  {"setditest", "ditest", _u4SRM_SetPscanMode, NULL, "set di test mode", CLI_GUEST},
#endif
  {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

#ifdef CC_CLI

static INT32 _u4SRM_DummyTrigger(INT32 i4Argc, const CHAR **szArgv)
{
    if(i4Argc != 2)
    {
        Printf("usage :%s  <main:0/ sub:1> \n", szArgv[0]);
        return 0;
    }
    else
    {
        //SRM_TriggerCliSrm((UINT32)StrToInt(szArgv[1]));
    }

    return 0;
}

static INT32 _u4SRM_SetDispMode(INT32 i4Argc, const CHAR **szArgv)
{
    if(i4Argc != 3)
    {
        Printf("usage :%s  <main:0/ sub:1> <dram:0/scaler disp:1,2/pscan disp:3>\n", szArgv[0]);
        return 0;
    }
    else
    {
        //SRM_CliSetDispMode((UINT32)StrToInt(szArgv[1]), (UINT32)StrToInt(szArgv[2]));
    }

    return 0;
}

static INT32 _u4SRM_SetPscanMode(INT32 i4Argc, const CHAR **szArgv)
{
    if(i4Argc != 2)
    {
        Printf("usage :%s  <Off:0/ On:1> \n", szArgv[0]);
        return 0;
    }
    else
    {
    	//Printf("_u4SRM_SetPscanMode!!\n");
        FBM_SetPscanTestMode(StrToInt(szArgv[1]));
    }

    return 0;
}

INT32 _u4SRM_ClearManSetting(INT32 i4Argc, const CHAR **szArgv)
{
    if(i4Argc != 1)
    {
        Printf("usage :%s\n", szArgv[0]);
        return 0;
    }
    else
    {
        //SRM_ClearCliSetting();
    }

    return 0;
}
#endif //#ifdef CC_CLI

