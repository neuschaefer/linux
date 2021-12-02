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
 * $RCSfile: osd_cmd.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file osd_cmd.h
 *  This header file includes type definitions for OSD CLI module.
 */


#ifndef OSD_CMD_H
#define OSD_CMD_H


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#ifdef __MW_CLI_DEF__
#include "x_mid_cli.h"
#else
#include "x_drv_cli.h"
#endif


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

typedef enum
{
    OSD_CLI_RET_OK             = 0,
    OSD_CLI_RET_QUIT           = 1,
    OSD_CLI_RET_ERR_GENERAL    = 1000,
    OSD_CLI_RET_ERR_PARAM      = 1001,
    OSD_CLI_RET_ERR_PLANE      = 1002,
    OSD_CLI_RET_ERR_LIST       = 1003,
    OSD_CLI_RET_ERR_REGION     = 1004,
    // add here
    OSD_CLI_RET_UNKNOWN        = 1999
} OSD_CLI_RET_CODE_T;

typedef struct _BMP_INFO_T
{
    UINT16 u4Width;
    UINT16 u4Height;
    UINT32 u4Address;
    UINT32 u4ColorMode;
} BMP_INFO_T;


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

#define CHK_NUM_MIN_PARAM(N) \
    do \
    { \
        if (i4Argc < ((N) + 1)) \
        { \
            return -(INT32)OSD_CLI_RET_ERR_PARAM; \
        } \
    } while (0)

#define PARAM(IDX) (UINT32)StrToInt(szArgv[IDX+1])

#define GET_PLANE(X_VAR) \
    do \
    { \
        if ((szArgv[1][0] < (CHAR)'1') || (szArgv[1][0] > (CHAR)'3')) \
        { \
            Printf("OSD Plane range (1, 2, 3). Input value : %s\n", \
                   szArgv[1][0]); \
            return -(INT32)OSD_CLI_RET_ERR_PLANE; \
        } \
        else \
        { \
            X_VAR = (INT32)szArgv[1][0] - (INT32)'1'; \
        } \
    } while (0)

#define GET_RGNLIST(X_VAR, X_POS) \
    do \
    { \
        X_VAR = StrToInt(szArgv[X_POS]); \
        if ((X_VAR < 0) || (X_VAR >= OSD_MAX_NUM_RGN_LIST)) \
        { \
            return -(INT32)OSD_CLI_RET_ERR_LIST; \
        } \
    } while (0)

#define GET_REGION(X_VAR, X_POS) \
    do \
    { \
        X_VAR = StrToInt(szArgv[X_POS]); \
        if ((X_VAR < 0) || (X_VAR >= OSD_MAX_NUM_RGN)) \
        { \
            return -(INT32)OSD_CLI_RET_ERR_REGION; \
        } \
    } while (0)

#define DELAY(X, Y) \
    do \
    { \
        UINT8 ch = _OsdGetSerialChar(); \
        if (ch == 'q') \
        { \
            Printf("value=%d(0x%08x)\n", Y, Y); \
            return OSD_CLI_RET_QUIT; \
        } \
        else if (ch == 'p') \
        { \
            while (_OsdGetSerialChar() != 'p') \
            { \
                x_thread_delay(X); \
            } \
        } \
        x_thread_delay(X); \
    } while (0)

#define CLI(X, ARG...) \
    do \
    { \
        LOG(5, X"\n", ARG); \
        CLI_Parser_Arg(X, ARG); \
    } \
    while (0)

#define CLI_RET(X, ARG...) \
    do \
    { \
        LOG(5, X"\n", ARG); \
        if ((ret = CLI_Parser_Arg(X, ARG)) != OSD_CLI_RET_OK) \
        { \
            return ret; \
        } \
    } while (0)


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

#ifdef __MW_CLI_DEF__
EXTERN CLI_EXEC_T* GetOsdMwCmdTbl(void);
#else
EXTERN CLI_EXEC_T* GetOsdCmdTbl(void);
#endif
EXTERN CLI_EXEC_T* GetOsdDiagCmdTbl(void);
EXTERN INT32 OSD_FindField(const CHAR *szText);
EXTERN INT32 OSD_DumpRegion(INT32 hRegion);


#endif
