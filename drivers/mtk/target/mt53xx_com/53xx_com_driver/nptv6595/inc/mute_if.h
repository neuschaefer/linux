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
#ifndef _DRV_MUTE_H_
#define _DRV_MUTE_H_

#include "typedef.h"

#define FOREVER_MUTE            0xFFFFFFFF

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


typedef enum
{
/* Notice :::::::
// Pleae update static CHAR _aszMuteModuleStr[MUTE_MODULE_MAX][MODULE_STR_LEN] when you add new mute module. 
*/
    MUTE_MODULE_API_FORCE = 0,
    MUTE_MODULE_BLUE,
    MUTE_MODULE_BLACK,
    MUTE_MODULE_MDDI01,
    MUTE_MODULE_SCPOS_PLA_EN,
    MUTE_MODULE_SCPOS_DRAM,
    MUTE_MODULE_SCPOS_PATTERN,
    MUTE_MODULE_SCPOS_FIFO,
    MUTE_MODULE_SCPOS_DISP_TUNE,
    MUTE_MODULE_SCPOS_PRE_DOWN,
    MUTE_MODULE_SCPOS_MISC,
    MUTE_MODULE_SCPOS_WA,
    MUTE_MODULE_SCPOS_TV3D,
    MUTE_MODULE_MODECHG,
    MUTE_MODULE_SRM,
    MUTE_MODULE_DTV,
    MUTE_MODULE_B2R,
    MUTE_MODULE_HDMI,
    MUTE_MODULE_MEMORY_TEST,
    MUTE_MODULE_PPCLOCK,
    MUTE_MODULE_NR,
    MUTE_MODULE_AUTOSEARH,
    MUTE_MODULE_MW_DISP_FMT,
    MUTE_MODULE_MAX
} MUTE_MODULE_ID;

typedef enum
{
/* Notice :::::::
// Pleae update static CHAR _aszMuteInvalidStr[MUTE_INVALID_MASK_ID][MODULE_STR_LEN] when you add new mute module. 
*/

	MUTE_INVALID_ON_NO_SIG = 0,
	MUTE_INVALID_ON_NO_SIG2,
       MUTE_INVALID_ATV_CHG_CH_FREEZE,
	MUTE_INVALID_AUTO_SEARCH,
	MUTE_INVALID_CLI_FORCE,
	MUTE_INVALID_MAX,
} MUTE_INVALID_MASK_ID;

typedef enum
{
    EVENT_VIDEO_MUTE_CHANGE = 0,
    EVENT_VIDEO_UNMUTE,
} MUTE_NOTIFY_TYPE;

typedef struct _strucVideoMuteInfo
{
    UINT32  u4InvalidMask;           // Valid module
	UINT32	u4Delay;	        // Unmute delay counter
	UINT32	u4InfoAddr;	        // Addr of debug information 
} VIDEO_MUTE_INFO;

typedef struct _strctVideoMuteCmd
{
	MUTE_MODULE_ID eModuleId;   // Mute module ID
	UINT8	u1Path;		        // Video path
	UINT32  u4ModuleStrAddr;    // Module name
	VIDEO_MUTE_INFO rMuteInfo;
} VIDEO_MUTE_CMD;

typedef struct _strctVideoMute
{
    MUTE_NOTIFY_TYPE eNotifyID;
    UINT32 u4Path;
    UINT32 u4Tag;
} VIDEO_MUTE_NOTIFY_MSG;

/// Callbalck function when the mute state is change
typedef VOID  (*MUTE_CALLBACK)(UINT8 u1Path, UINT8 u1Tag);

typedef struct _strctMuteCallBack
{
    MUTE_CALLBACK pfnCallBack;
    UINT32 u4NotifyTag;
    VOID * prNext;
} VIDEO_MUTE_CALLBACK_FUNC;

//-----------------------------------------------------------------------------
//function Declare
//-----------------------------------------------------------------------------

/**
 * @brief _vDrvDumpVideoMuteModule
 * Dump delay counter of all mute modules
 * @param  u4Path
 * @param  u4ForceDumpInfo
 * @return void
 */
EXTERN VOID _vDrvDumpVideoMuteModule(UINT32 u4Path, UINT32 u4ForceDumpInfo);

/**
 * @brief _vDrvDumpVideoMuteInfo
 * Dump mute information of all modules
 * @param  u4Path
 * @return void
 */
EXTERN VOID _vDrvDumpVideoMuteInfo(UINT32 u4Path);

/**
 * @brief _vDrvSendMuteCmd
 * Set mute/unmute information of the selected module
 * @param  prDrvMuteCmd
 * @return void
 */
EXTERN VOID _vDrvSendMuteCmd(VIDEO_MUTE_CMD *prDrvMuteCmd);

/**
 * @brief _u4DrvVideoGetMute
 * Get mute/unmute status of the selected videp path
 * @param  u4Path
 * @return Mute or not
 */
EXTERN UINT32 _u4DrvVideoGetMute(UINT32 u4Path);

/**
 * @brief _vDrvVideoGetMuteState
 * Set mute/unmute information of the selected module
 * @param  prDrvMuteCmd
 * @return void
 */
EXTERN VOID _vDrvVideoGetMuteStatus(VIDEO_MUTE_CMD *prDrvMuteCmd);

/**
 * @brief _vDrvVideoGetMuteStatus
 * Get the mute dealy of the selected module
 * @param  u4Path,eModule
 * @return UINT32
 */
EXTERN UINT32 _vDrvVideoGetMuteDelay(UINT32 u4Path, MUTE_MODULE_ID eModule);

/**
 * @brief _vDrvVideoSetMute
 * Set mute command of the selected module
 * @param  eModuel, u4Path, u4UnMuteDelay, bReplaceByLongerDelay
 * @return void
 */
EXTERN VOID _vDrvVideoSetMute(MUTE_MODULE_ID eModule, UINT32 u4Path,
                              UINT32 u4UnMuteDelay, BOOL bReplaceByLongerDelay);

/**
 * @brief _vDrvVideoSetMuteInvalidFlag
 * Set mute command of the selected module to be invalid
 * @param  eModuel, u4Path, bInvalid , eInvalidID
 * @return void
 */
EXTERN VOID _vDrvVideoSetMuteInvalidFlag(MUTE_MODULE_ID eModule, 
                                         UINT32 u4Path, BOOL bInvalid, MUTE_INVALID_MASK_ID eInvalidID);
/**
 * @brief _vDrvVideoForceUnMute
 * Set Force Mute Off for Main and PIP Channel
 * @param  u4Path
 * @param  bOnOff
 * @return void
 */
EXTERN VOID _vDrvVideoForceUnMute(UINT32 u4Path, BOOL bOnOff);

/**
 * @brief _vDrvVideoHandleUnMute
 * Handle unmute delay counter and mute on/off for Main/PIP channel
 * @param  none
 * @return void
 */
EXTERN VOID _vDrvVideoHandleUnMute(VOID);

/**
 * @brief _vDrvVideoRegisterMuteNotify
 * Register the notify function 
 * @param   prNotifyFunc, eNotifyType, u4Tag
 * @return  0: success
            0xFFFFFFFF : fail
 */
EXTERN UINT32 _vDrvVideoRegisterMuteNotify(MUTE_CALLBACK prNotifyFunc, 
                                    MUTE_NOTIFY_TYPE eNotifyType, UINT32 u4Tag);

/**
 * @brief vDrvSetForceUnmute
 * set force mute 
 * @param   path, type, enable
 * @return  void
 */
EXTERN VOID vDrvSetForceUmute(INT32 u4Path, UINT8 u1MuteType, BOOL bEnable);

#endif /* _DRV_MUTE_H */

