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

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#include "x_os.h"
#include "x_assert.h"

#include "mute_if.h"
#include "vdp_drvif.h"
#include "drv_scpos.h"
#include "sv_const.h"
#include "source_select.h"
#include "video_def.h"
//#include "nptv_debug.h"
#include "mute_debug.h"
#include "feature.h"
#include "tve_if.h"
 #ifdef TIME_MEASUREMENT
#include "x_time_msrt.h"
#include "u_time_msrt_name.h"
#endif  
#include "x_bim.h"
#ifdef CC_MT5395
#include "drv_mjc.h"
#endif
//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------
#define MODULE_STR_LEN                  26

#ifdef CC_COPLAT_MT82
// todo
typedef UINT32 SIZE_T;
#define x_msg_q_send(queue, data, unit, size)
static CHAR CODE _aszMuteModuleStr[1][MODULE_STR_LEN] = 
{
    "MODULE_API_FORCE", //reduce code size
};
#else

static CHAR _aszMuteModuleStr[MUTE_MODULE_MAX][MODULE_STR_LEN] = 
{
    "API_FORCE: AP", 
    "BLUE: AP",
    "BLACK: AP",
    "MDDI01",
    "SCPOS_PLA_EN",
    "SCPOS_DRAM",
    "SCPOS_PATTERN",
    "SCPOS_FIFO",
    "SCPOS_DISP_TUNE",
    "SCPOS_PRE_DOWN",
    "SCPOS_MISC",
    "SCPOS_WA",
    "SCPOS_TV3D",
    "MODECHG: DECODER",
    "SRM",
    "DTV",
    "B2R",
    "HDMI",
    "MEMORY_TEST",
    "PPCLOCK",
    "NR",
    "AutoSearch: SI",
    "MW_DISP_FMT"
};

#endif
static CHAR _aszMuteInvalidStr[MUTE_INVALID_MAX][MODULE_STR_LEN] = 
{
    "No signal Snow Scr",
    "MTAL No signal Snow Scr",
    "ATV Freeze chan chg",
    "ATV Auto search",
    "CLI"
};


//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------
#define VIDEO_MUTE(u4Path, bOnOff)      do { if ((u4Path) == SV_VP_MAIN)           \
                                            vDrvMainMute(bOnOff);           \
                                        else                                \
                                            vDrvPIPMute(bOnOff);} while(0)
                                        
//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------
static UINT32 _au4ValidIndex[VDP_NS];
static VIDEO_MUTE_INFO _rVideoMuteInfo[VDP_NS][MUTE_MODULE_MAX];
static VIDEO_MUTE_INFO * _prValidMuteInfo[VDP_NS];
static UINT32 _au4LastMuteState[VDP_NS] = {0xFF, 0xFF};

static BOOL _bMuteInit = TRUE;
static HANDLE_T _hMuteMsgQue = NULL;
static HANDLE_T _hMuteNotifyHandle = NULL;
static VIDEO_MUTE_CALLBACK_FUNC *prMuteChangeHead = NULL;
static VIDEO_MUTE_CALLBACK_FUNC *prMuteChangeEnd = NULL;
static VIDEO_MUTE_CALLBACK_FUNC *prVideoUnMuteHead = NULL;
static VIDEO_MUTE_CALLBACK_FUNC *prVideoUnMuteEnd = NULL;
/* boot up measurement in release version, record once.*/
static BOOL b_boot_rec_once = FALSE;

//---------------------------------------------------------------------------
// Internal functions
//---------------------------------------------------------------------------
#ifndef CC_COPLAT_MT82
VOID vDrvVideoMuteNotifyHandle(void *pvArg)
{
    UINT16 u2MsgIdx;
    SIZE_T zMsgSize = sizeof(VIDEO_MUTE_NOTIFY_MSG);
    INT32  i4MsgRet;
    VIDEO_MUTE_NOTIFY_MSG rMsg;
    VIDEO_MUTE_CALLBACK_FUNC *prNfyFunc;
        
    UNUSED(pvArg);
    while (1)
    {
        i4MsgRet = x_msg_q_receive(&u2MsgIdx, &rMsg, &zMsgSize,
                                   &_hMuteMsgQue, 1, X_MSGQ_OPTION_WAIT);
        if (i4MsgRet != OSR_OK)
        {
            continue;
        }
        
        // Check the notify event type
        switch (rMsg.eNotifyID)
        {
        case EVENT_VIDEO_MUTE_CHANGE:
            prNfyFunc = prMuteChangeHead;
            while (prNfyFunc)
            {
                prNfyFunc->pfnCallBack(rMsg.u4Path, rMsg.u4Tag);
                prNfyFunc = (VIDEO_MUTE_CALLBACK_FUNC *)prNfyFunc->prNext;
            }
            break;
                
        case EVENT_VIDEO_UNMUTE:
            prNfyFunc = prVideoUnMuteHead;
            while (prNfyFunc)
            {
                prNfyFunc->pfnCallBack(rMsg.u4Path, prNfyFunc->u4NotifyTag);
                prNfyFunc = (VIDEO_MUTE_CALLBACK_FUNC *)prNfyFunc->prNext;
            }
            break;
            
        default:
            break;
        }
    }
}
#endif

VOID vDrvVideoMuteInit(VOID)
{
    UINT32 u4Path, u4Module;
    CRIT_STATE_T rCritState;

    UNUSED(_au4ValidIndex);
    UNUSED(_prValidMuteInfo);

    rCritState = x_crit_start();
    
    for (u4Path = 0; u4Path < VDP_NS; u4Path++)
    {
        _au4ValidIndex[u4Path] = 0;
        _prValidMuteInfo[u4Path] = NULL;

        for (u4Module = 0; u4Module < MUTE_MODULE_MAX; u4Module++)
        {
            _rVideoMuteInfo[u4Path][u4Module].u4InvalidMask = 0;
            _rVideoMuteInfo[u4Path][u4Module].u4Delay = 0;
            _rVideoMuteInfo[u4Path][u4Module].u4InfoAddr = 0;
        }
    }
    
  /*  for (u4Module = 0; u4Module < MUTE_MODULE_MAX; u4Module++)
    {
        sprintf(_aszMuteModuleStr[u4Module], "Module_%d", u4Module);
    }
  */  
    
    #ifndef CC_COPLAT_MT82
    // Create the msg queue for power error code
    VERIFY(x_msg_q_create(&_hMuteMsgQue, "VideoMuteMsgQue", 
                          sizeof(VIDEO_MUTE_NOTIFY_MSG), 32) == OSR_OK);

    if (x_thread_create(&_hMuteNotifyHandle, "VideoMuteNotifyHandle", 0x800,
                        DRVCUST_OptGet(eReleaseWatchdogPriority),
                        vDrvVideoMuteNotifyHandle, 0, NULL) != OSR_OK)
    {
        LOG(1, "\nError: Create mute notify handle thread fail\n");
        x_msg_q_delete(_hMuteMsgQue);
        _hMuteMsgQue = NULL;
    }
    #endif
    
    _bMuteInit = FALSE;

    x_crit_end(rCritState);
}

static VOID vDrvDumpVideoMuteModuleInfo(UINT32 u4Path, MUTE_MODULE_ID eModule)
{
    if ((u4Path >= VDP_NS) || (eModule >= MUTE_MODULE_MAX))
    {
        return;
    }

    if (_bMuteInit)
    {
        vDrvVideoMuteInit();
    }

    #if CC_BOOT_MUTE_LOG
    MUTE_IsLog(2, "Path=%d Module=%2d (%s) \tInvalid=0x%x Count=%d Info=0x%x\n",
    #else
    LOG(2, "Path=%d Module=%2d (%s) \tInvalid=0x%x Count=%d Info=0x%x\n",
    #endif
           u4Path, eModule, _aszMuteModuleStr[eModule],
           _rVideoMuteInfo[u4Path][eModule].u4InvalidMask, 
           _rVideoMuteInfo[u4Path][eModule].u4Delay,
           _rVideoMuteInfo[u4Path][eModule].u4InfoAddr);   
}

//---------------------------------------------------------------------------
// Global functions
//---------------------------------------------------------------------------
VOID _vDrvDumpVideoMuteModule(UINT32 u4Path, UINT32 u4ForceDumpInfo)
{
    UINT32 u4Module;

    if (_bMuteInit)
    {
        vDrvVideoMuteInit();
    }

    if (u4ForceDumpInfo)
    {
        Printf("\nVideo Path = %d\n", u4Path);
    }
    else
    {
        LOG(3, "\nVideo Path = %d\n", u4Path);
    }
        
    for (u4Module = 0; u4Module < MUTE_MODULE_MAX; u4Module++)
    {
        //if (_rVideoMuteInfo[u4Path][u4Module].u4InvalidMask)
         //   continue;

        if (u4ForceDumpInfo)
        {
            Printf("Module = %2d (%s) \tInvalid=0x%x  Delay conter = %d\n",
               u4Module, _aszMuteModuleStr[u4Module],_rVideoMuteInfo[u4Path][u4Module].u4InvalidMask, 
               _rVideoMuteInfo[u4Path][u4Module].u4Delay);
        }
        else
        {            
            LOG(3, "Module = %2d (%s) \tInvalid=0x%x  Delay conter = %d\n",
               u4Module, _aszMuteModuleStr[u4Module],_rVideoMuteInfo[u4Path][u4Module].u4InvalidMask, 
               _rVideoMuteInfo[u4Path][u4Module].u4Delay);
        }
    }
	Printf("========================\n");
	Printf("Invalid Mask:\n");
	for(u4Module = 0; u4Module < MUTE_INVALID_MAX ; u4Module++)
	{

		Printf("%s:0x%x\n", _aszMuteInvalidStr[u4Module], (1<<u4Module));
	}
}

VOID _vDrvDumpVideoMuteInfo(UINT32 u4Path)
{
    //MUTE_MODULE_ID eModule;
    UINT32 u4Module;

    if (u4Path >= VDP_NS)
    {
        return;
    }

    if (_bMuteInit)
    {
        vDrvVideoMuteInit();
    }
    
  //  for (u4Path = SV_VP_MAIN; u4Path <= SV_VP_PIP; u4Path++)
    {
        LOG(3, "\nVideo Path = %d\n", u4Path);
        LOG(3, "Current valid Module = %d\n", _au4ValidIndex[u4Path]);
        LOG(3, "Current unmute delay counter = %d\n", 
               _prValidMuteInfo[u4Path]->u4Delay);
        
        for (u4Module = 0; u4Module < MUTE_MODULE_MAX; u4Module++)
        {
            vDrvDumpVideoMuteModuleInfo(u4Path, (MUTE_MODULE_ID)u4Module);
        }
    }
}

#ifndef CC_COPLAT_MT82
VOID _vDrvSendMuteCmd(VIDEO_MUTE_CMD *prDrvMuteCmd)
{
    UINT32 u4Path;
    MUTE_MODULE_ID eModule;
    CRIT_STATE_T rCritState;
    
    if (_bMuteInit)
    {
        vDrvVideoMuteInit();
    }

    if (prDrvMuteCmd == NULL)
    {
        LOG(5, "[_vDrvSendMuteCmd] Null parameters!\n");
        return;
    }
    
    // Check video path
    u4Path = prDrvMuteCmd->u1Path;
    if (u4Path >= VDP_NS)
    {
        LOG(5, "[_vDrvSendMuteCmd] Invalid video path = %d!\n", u4Path);
        return;
    }

    // Check module Id
    eModule = prDrvMuteCmd->eModuleId;
    if (eModule >= MUTE_MODULE_MAX)
    {
        LOG(5, "[_vDrvSendMuteCmd] Invalid module ID = %d!\n", eModule);
        return;
    }

    if (prDrvMuteCmd->rMuteInfo.u4Delay > 0)
    {
        VIDEO_MUTE(u4Path, SV_ON);
    }

    // Keep the mute information
    rCritState = x_crit_start();
    x_memcpy(&_rVideoMuteInfo[u4Path][eModule],
             &prDrvMuteCmd->rMuteInfo, sizeof(VIDEO_MUTE_INFO));
    x_crit_end(rCritState);

    if (prDrvMuteCmd->u4ModuleStrAddr)
    {
        if (x_strcmp(_aszMuteModuleStr[eModule], 
                     (CHAR *)((VOID *)prDrvMuteCmd->u4ModuleStrAddr)) != 0)
        {
            x_strcpy(_aszMuteModuleStr[eModule], 
                     (CHAR *)((VOID *)prDrvMuteCmd->u4ModuleStrAddr));
        }
    }
    
    _vDrvDumpVideoMuteModule(u4Path, 0);
}
#endif

UINT32 _u4DrvVideoGetMute(UINT32 u4Path)
{
    UINT32 u4Status;

    if (u4Path >= VDP_NS)
    {
        return 0;
    }

    u4Status = _au4LastMuteState[u4Path];
    #ifdef CC_MT5395
    u4Status |= (UINT32)u1DrvMJCGetMuteCount();
    #endif

    return u4Status;
}

VOID _vDrvVideoGetMuteStatus(VIDEO_MUTE_CMD *prDrvMuteCmd)
{
    UINT32 u4Path;
    MUTE_MODULE_ID eModule;
    CRIT_STATE_T rCritState;

    if (_bMuteInit)
    {
        return;
    }
    
    if (prDrvMuteCmd == NULL)
    {
        LOG(5, "[_vDrvVideoGetMuteState] Null parameters!\n");
        return;
    }

    // Check video path
    u4Path = prDrvMuteCmd->u1Path;
    if (u4Path >= VDP_NS)
    {
        LOG(5, "[_vDrvVideoGetMuteState] Invalid video path = %d!\n", u4Path);
        return;
    }

    // Check module Id
    eModule = prDrvMuteCmd->eModuleId;
    if (eModule >= MUTE_MODULE_MAX)
    {
        LOG(5, "[_vDrvVideoGetMuteState] Invalid module ID = %d!\n", eModule);
        return;
    }

    x_memset(&prDrvMuteCmd->rMuteInfo, 0, sizeof(VIDEO_MUTE_INFO));
    
    // Keep the mute information
    rCritState = x_crit_start();
    x_memcpy(&prDrvMuteCmd->rMuteInfo, &_rVideoMuteInfo[u4Path][eModule], 
             sizeof(VIDEO_MUTE_INFO));
    x_crit_end(rCritState);
}

UINT32 _vDrvVideoGetMuteDelay(UINT32 u4Path, MUTE_MODULE_ID eModule)
{
    if (_bMuteInit)
    {
        return 0;
    }
    
    if (u4Path >= VDP_NS)
    {
        LOG(5, "[_vDrvVideoGetMuteState] Invalid video path = %d!\n", u4Path);
        return 0;
    }

    // Check module Id
    if (eModule >= MUTE_MODULE_MAX)
    {
        LOG(5, "[_vDrvVideoGetMuteState] Invalid module ID = %d!\n", eModule);
        return 0;
    }    

    return _rVideoMuteInfo[u4Path][eModule].u4Delay;
}

VOID _vDrvVideoSetMute(MUTE_MODULE_ID eModule, UINT32 u4Path,
                       UINT32 u4UnMuteDelay, BOOL bReplaceByLongerDelay)
{
/*    VIDEO_MUTE_CMD rMuteCmd;

    x_memset(&rMuteCmd, 0, sizeof(VIDEO_MUTE_CMD));
    rMuteCmd.eModuleId = eModule;
    rMuteCmd.u1Path = (UINT8)u4Path;
    _vDrvVideoGetMuteStatus(&rMuteCmd);

    rMuteCmd.rMuteInfo.u4Delay = u4UnMuteDelay;
    _vDrvSendMuteCmd(&rMuteCmd);
*/
    UINT32 u4Invalid;
    BOOL bMuteChange = FALSE;
    VIDEO_MUTE_NOTIFY_MSG rNotifyMsg;

    CRIT_STATE_T rCritState;

    if (_bMuteInit)
    {
        vDrvVideoMuteInit();
    }

    // Check video path
    if (u4Path >= VDP_NS)
    {
        LOG(5, "[_vDrvVideoSetMute] Invalid video path = %d!\n", u4Path);
        return;
    }

    // Check module Id
    if (eModule >= MUTE_MODULE_MAX)
    {
        LOG(5, "[_vDrvVideoSetMute] Invalid module ID = %d!\n", eModule);
        return;
    }

    #if 0
    if(PANEL_IsSupport120Hz() && (u4UnMuteDelay != FOREVER_MUTE))
    {
        u4UnMuteDelay <<= 1; // mute delay doubled for 120hz panel
    }
    #endif
    
    rCritState = x_crit_start();
    if (bReplaceByLongerDelay)
    {
        UINT32 u4CurrentDelay = _rVideoMuteInfo[u4Path][eModule].u4Delay;
        
        _rVideoMuteInfo[u4Path][eModule].u4Delay = 
            (u4CurrentDelay < u4UnMuteDelay) ? u4UnMuteDelay : u4CurrentDelay;
    }
    else
    {
        _rVideoMuteInfo[u4Path][eModule].u4Delay = u4UnMuteDelay;
    }
    u4Invalid = _rVideoMuteInfo[u4Path][eModule].u4InvalidMask;

    
    if (!u4Invalid && (u4UnMuteDelay > 0))
    {
        VIDEO_MUTE(u4Path, SV_ON);

        if (_au4LastMuteState[u4Path] == SV_OFF)
        {
            bMuteChange = TRUE;
        }
        
   	_au4LastMuteState[u4Path]= SV_ON ;
    }
    x_crit_end(rCritState);   

    if (bMuteChange)
    {
        #ifdef CC_MT5395
        // mute will delay due to MJC buffer
        VDP_UpdateCmdDelay(u4Path);
        #endif
        #ifndef CC_COPLAT_MT82       
        if(eModule == MUTE_MODULE_B2R)
        {
            // Notify the mute state change    
            rNotifyMsg.eNotifyID = EVENT_VIDEO_MUTE_CHANGE;
            rNotifyMsg.u4Path = u4Path;
            rNotifyMsg.u4Tag = SV_ON;
            
            UNUSED(x_msg_q_send(_hMuteMsgQue, &rNotifyMsg, 
                                sizeof(VIDEO_MUTE_NOTIFY_MSG), 1));
        }
        #endif
    }
    
    vDrvDumpVideoMuteModuleInfo(u4Path, eModule);    	

    //_vDrvDumpVideoMuteModule(u4Path);  //remove becasue of too many log
}

VOID _vDrvVideoSetMuteInvalidFlag(MUTE_MODULE_ID eModule, 
                                  UINT32 u4Path, BOOL bInvalid, MUTE_INVALID_MASK_ID eInvalidID)
{
    CRIT_STATE_T rCritState;
    
    if (_bMuteInit)
    {
        return;
    }
    
    // Check video path
    if (u4Path >= VDP_NS)
    {
        LOG(5, "[_vDrvVideoSetMuteInvalid] Invalid video path = %d!\n", u4Path);
        return;
    }
    
    // Check module Id
    if (eModule >= MUTE_MODULE_MAX)
    {
        LOG(5, "[_vDrvVideoSetMuteInvalid] Invalid module ID = %d!\n", eModule);
        return;
    }
    
    rCritState = x_crit_start();
	if(bInvalid)
	{
		_rVideoMuteInfo[u4Path][eModule].u4InvalidMask |= (1 << eInvalidID);
	}
	else
	{
		_rVideoMuteInfo[u4Path][eModule].u4InvalidMask &= (~(1 << eInvalidID));
	}
    x_crit_end(rCritState);
}
#if 0
VOID vDrvClearAllMuteCnt(UINT32 u4Path)
{
    UINT32 u4Module;
    CRIT_STATE_T rCritState;
    
    if (u4Path >= VDP_NS)
    {
        return;
    }

    if (_bMuteInit)
    {
        vDrvVideoMuteInit();
    }


    // Keep the mute information
    rCritState = x_crit_start();
    
	for (u4Module = 0; u4Module < MUTE_MODULE_MAX; u4Module++)
	{
        _rVideoMuteInfo[u4Path][u4Module].u4Delay = 0 ;
	}
        
    x_crit_end(rCritState);

}
#endif
VOID _vDrvVideoForceUnMute(UINT32 u4Path, BOOL bOnOff)
{
    BOOL bForceUnMute = bOnOff;
 //   UINT32 u4Path;
    UINT32 u4Module;
    CRIT_STATE_T rCritState;
    
    if (u4Path >= VDP_NS)
    {
        return;
    }

    if (_bMuteInit)
    {
        vDrvVideoMuteInit();
    }


    // Keep the mute information
    rCritState = x_crit_start();
#if 0   
    if (bForceUnMute)
    {
 //       if (fgIsSrcAtv(SV_VP_MAIN) || fgIsMainDTV())
        {
            for (u4Module = 0; u4Module < MUTE_MODULE_MAX; u4Module++)
            {
                // Set all modules in main video path to be invalid
                _rVideoMuteInfo[u4Path][u4Module].bInvalid = TRUE;
            }
        }
        
/*        if (fgIsSrcAtv(SV_VP_PIP) || fgIsPipDTV())
        {
            for (u4Module = 0; u4Module < MUTE_MODULE_MAX; u4Module++)
            {
                // Set all modules in sub video path to be invalid
                _rVideoMuteInfo[SV_VP_PIP][u4Module].bInvalid = TRUE;
            }
        }
        */
        LOG(3, "[_vDrvVideoForceUnMute (%d)]: Force Unmute is ON\n", u4Path);
    }
    else
    {
    //    for (u4Path = SV_VP_MAIN; u4Path <= SV_VP_PIP; u4Path++)
        {
            for (u4Module = 0; u4Module < MUTE_MODULE_MAX; u4Module++)
            {
                // Set the command of other module to be valid
                _rVideoMuteInfo[u4Path][u4Module].bInvalid = FALSE;
            }
        }
        
        LOG(3, "[_vDrvVideoForceUnMute (%d)]: Force Unmute is OFF\n", u4Path);
    }
#else
    for (u4Module = 0; u4Module < MUTE_MODULE_MAX; u4Module++)
	{
		// Set all modules in main video path to be invalid
		_vDrvVideoSetMuteInvalidFlag((MUTE_MODULE_ID)u4Module, u4Path, bForceUnMute, MUTE_INVALID_CLI_FORCE);
	}

#endif
    x_crit_end(rCritState);

}
#ifdef DYNANMICS_USB_SWITCH
extern     UINT32 _u4BypassNptvMloop;
#endif
VOID _vDrvVideoHandleUnMute(VOID)
{
    UINT32 u4Module, u4SelectModule = MUTE_MODULE_MAX;
    UINT32 u4Path, u4CheckDelay;
    UINT32 u4MuteOnOFF;
    BOOL bFindValidModule;
    CRIT_STATE_T rCritState;
    
    if (_bMuteInit)
    {
        vDrvVideoMuteInit();
    }

    for (u4Path = SV_VP_MAIN; u4Path <= SV_VP_PIP; u4Path++)
    {
        bFindValidModule = FALSE;
        u4CheckDelay = 0;
        _prValidMuteInfo[u4Path] = NULL;
        
        for (u4Module = 0; u4Module < MUTE_MODULE_MAX; u4Module++)
        {
            rCritState = x_crit_start();
            
            if ((_rVideoMuteInfo[u4Path][u4Module].u4Delay != FOREVER_MUTE) &&
                (_rVideoMuteInfo[u4Path][u4Module].u4Delay > 0))
            {
                _rVideoMuteInfo[u4Path][u4Module].u4Delay--;
            }
            
            if (!_rVideoMuteInfo[u4Path][u4Module].u4InvalidMask)
            {
                if (_rVideoMuteInfo[u4Path][u4Module].u4Delay == FOREVER_MUTE)
                {
                    u4CheckDelay = _rVideoMuteInfo[u4Path][u4Module].u4Delay;
                    x_crit_end(rCritState);
                    
                    bFindValidModule = TRUE;
                    u4SelectModule = u4Module;
                   continue;
                }
                
                if (_rVideoMuteInfo[u4Path][u4Module].u4Delay > u4CheckDelay)
                {
                    bFindValidModule = TRUE;
                    u4SelectModule = u4Module;
                    u4CheckDelay = _rVideoMuteInfo[u4Path][u4Module].u4Delay;
                }
            }
            x_crit_end(rCritState);
        }

        if (bFindValidModule)
        {
            _au4ValidIndex[u4Path] = u4SelectModule;
            _prValidMuteInfo[u4Path] = &_rVideoMuteInfo[u4Path][u4SelectModule];
        }

        u4MuteOnOFF = (u4CheckDelay == 0) ? SV_OFF : SV_ON;
        if (_au4LastMuteState[u4Path] != u4MuteOnOFF)
        {
            VIDEO_MUTE_NOTIFY_MSG rNotifyMsg;

            #if CC_BOOT_MUTE_LOG
            MUTE_IsLog(2, "Mute State Changed :Path(%d), Latest module(%d), Mute State(%d) --> (%d)\n", u4Path, _au4ValidIndex[u4Path] , _au4LastMuteState[u4Path], u4MuteOnOFF);
            #else
            LOG(2, "Mute State Changed :Path(%d), Latest module(%d), Mute State(%d) --> (%d)\n", u4Path, _au4ValidIndex[u4Path] , _au4LastMuteState[u4Path], u4MuteOnOFF);
            #endif
            VIDEO_MUTE(u4Path, u4MuteOnOFF);

            #ifdef CC_MT5395
            if (u4MuteOnOFF)
            {
                // mute will delay due to MJC buffer
                VDP_UpdateCmdDelay(u4Path);
            }
            #endif
            
             if ((u4Path == SV_VP_MAIN) && (u4MuteOnOFF == SV_OFF))
            {
                /* boot up measurement in release version, record once.*/
                if (!b_boot_rec_once)
                {
	                x_os_drv_set_timestamp("Main Video Un-mute");
	                b_boot_rec_once = TRUE;
                }
#ifdef TIME_MEASUREMENT            
                TMS_DIFF_EX(TMS_FLAG_CHG_CHL, TMS_CHL_CHE_TIME, "UN-MUTE");
                TMS_END_EX(TMS_FLAG_CHG_CHL, TMS_CHL_CHE_TIME, "CHANNEL_CHG");
                TMS_END_EX(TMS_FLAG_BOOT, TMS_COOL_BOOT_TIME, NULL);
                TMS_END_EX(TMS_FLAG_INP_CHG, TMS_INP_SRC_CHE_TIME, "INP_SRC_CHG");
#endif                    
            }                  
            _au4LastMuteState[u4Path] = u4MuteOnOFF;

            rNotifyMsg.eNotifyID = EVENT_VIDEO_MUTE_CHANGE;
            rNotifyMsg.u4Path = u4Path;
            rNotifyMsg.u4Tag = u4MuteOnOFF;
            #ifndef CC_COPLAT_MT82
            UNUSED(x_msg_q_send(_hMuteMsgQue, &rNotifyMsg, 
                                sizeof(VIDEO_MUTE_NOTIFY_MSG), 1));
            #endif

#if defined(CC_SUPPORT_TVE) || defined(CC_SUPPORT_TVE_82xx)
            if (u4MuteOnOFF == SV_OFF)
            {
                vApiTVEVideoStatusNotify(u4Path, SV_TVE_NOTIFY_VIDEO_UNMUTE);
                rNotifyMsg.eNotifyID = EVENT_VIDEO_UNMUTE;
             #ifndef CC_COPLAT_MT82
                UNUSED(x_msg_q_send(_hMuteMsgQue, &rNotifyMsg, 
                                    sizeof(VIDEO_MUTE_NOTIFY_MSG), 1));
             #endif
            }
#endif
#ifdef  DYNANMICS_USB_SWITCH

             if ((u4MuteOnOFF == SV_OFF)&& fgIsSrcMM())
             {
                    vNotifySharedInfo(SI_B2R_TIMING,0,0);
                    _u4BypassNptvMloop = 1;
             }
#endif             
        }
    }
}

#ifndef CC_COPLAT_MT82
UINT32 _vDrvVideoRegisterMuteNotify(MUTE_CALLBACK prNotifyFunc, 
                                    MUTE_NOTIFY_TYPE eNotifyType, UINT32 u4Tag)
{
    VIDEO_MUTE_CALLBACK_FUNC *prNotify;
    VIDEO_MUTE_CALLBACK_FUNC **prListHead = 0, **prListEnd = 0;
        
    if (_bMuteInit)
    {
        vDrvVideoMuteInit();
    }
    
    switch (eNotifyType)
    {
    case EVENT_VIDEO_MUTE_CHANGE:
        prListHead = &prMuteChangeHead;
        prListEnd = &prMuteChangeEnd;
        break;
    case EVENT_VIDEO_UNMUTE:
        prListHead = &prVideoUnMuteHead;
        prListEnd = &prVideoUnMuteEnd;
        break;
    default:
        return (0xFFFFFFFF);
    }    

    prNotify = x_mem_alloc(sizeof(VIDEO_MUTE_CALLBACK_FUNC));
    if (prNotify != NULL)
    {
        prNotify->pfnCallBack = prNotifyFunc;
        prNotify->u4NotifyTag = u4Tag;
        prNotify->prNext = NULL;
        if (prVideoUnMuteHead == NULL)
        {
            *prListHead = prNotify;
            *prListEnd = prNotify;
        }
        else
        {
            (*prListEnd)->prNext = (VOID *)prNotify;
            *prListEnd = prNotify;
        }
        return 0;
    }

    return 0xFFFFFFFF;
}
void vDrvSetForceUmute(INT32 u4Path, UINT8 u1MuteType, BOOL bEnable)
{
	switch(u1MuteType)
	{
		case SV_FORCE_UNMUTE_TYPE_ON_NO_SIG:
		{
			LOG(3, "Path[%d] No signal  Force Unmute is %d\n", u4Path, bEnable);
			_vDrvVideoSetMuteInvalidFlag(MUTE_MODULE_MODECHG, u4Path, bEnable, MUTE_INVALID_ON_NO_SIG);
			_vDrvVideoSetMuteInvalidFlag(MUTE_MODULE_API_FORCE, u4Path, bEnable, MUTE_INVALID_ON_NO_SIG);
		}
		break;
		case SV_FORCE_UNMUTE_TYPE_ON_NO_SIG2:
		{   // for MTAL, only disable MODECHG mute
			LOG(3, "Path[%d] No signal  Force Unmute2 is %d\n", u4Path, bEnable);
			_vDrvVideoSetMuteInvalidFlag(MUTE_MODULE_MODECHG, u4Path, bEnable, MUTE_INVALID_ON_NO_SIG2);
		}
		break;
		case SV_FORCE_UNMUTE_TYPE_ATV_CHG_CH:
		{

			LOG(3, "Path[%d] Channel change Force Unmute is %d\n", u4Path, bEnable);
			if(!bEnable)
			{
				_rVideoMuteInfo[u4Path][MUTE_MODULE_MODECHG].u4Delay = 0 ;
				_rVideoMuteInfo[u4Path][MUTE_MODULE_MDDI01].u4Delay = 0 ;
				//don't clear the AP mute for no signal case
			}
			_vDrvVideoSetMuteInvalidFlag(MUTE_MODULE_MODECHG, u4Path, bEnable, MUTE_INVALID_ATV_CHG_CH_FREEZE);
			_vDrvVideoSetMuteInvalidFlag(MUTE_MODULE_API_FORCE, u4Path, bEnable, MUTE_INVALID_ATV_CHG_CH_FREEZE);
			_vDrvVideoSetMuteInvalidFlag(MUTE_MODULE_MDDI01, u4Path, bEnable, MUTE_INVALID_ATV_CHG_CH_FREEZE);
		}
		break;
		default:
		break;
	}
}
#endif
