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
 * $RCSfile: venc_drvif.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file venc_drvif.c
 *  Video Encoder - public interface
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "venc_drvif.h"
#include "venc_debug.h"
#include "fbm_drvif.h"
#include "x_chip_id.h"

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

#ifndef CC_SUPPORT_VENC
#error "venc module need macro CC_SUPPORT_VENC"
#endif

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


#ifdef VENC_DRAM_PROFILE
/**
 ** Brief of HAL_DRAMC_T
 */
typedef struct
{
    UINT32 u4Reserved                       : 8;
    UINT32 u4Bmgp1ag                        : 5;
    UINT32                                  : 2;
    UINT32 fgBmbp1                          : 1;
    UINT32 u4Bmgp2ag                        : 3;
    UINT32 fgBmbp2                          : 1;
    UINT32 u4Bmgp3ag                        : 3;
    UINT32 fgBmbp3                          : 1;
    UINT32 fgReadOnly                       : 1;
    UINT32 fgWriteOnly                      : 1;
    UINT32                                  : 2;
    UINT32 fgFreeze                         : 1;
    UINT32 fgDynPrioEn                      : 1;
    UINT32 fgReqAle                         : 1;
    UINT32 fgPreAct                         : 1;
LINT_SUPPRESS_NEXT_EXPRESSION(950)
} __attribute__ ((packed))  HAL_DRAMC_T;
#endif

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

//just for dev
#define _VENC_DBG(fmt...)     LOG(6, fmt)
#define _VENC_FUNC_BEGIN()    _VENC_DBG("_VENC::func Begin:%s\n", __FUNCTION__)
#define _VENC_FUNC_END()      _VENC_DBG("_VEHC::func end:  %s\n", __FUNCTION__)
#define _VENC_FUNC_ENTER()    _VENC_DBG("_VENC::func Enter:%s\n", __FUNCTION__)


#define VENC_LINK_INSERT(node)                  \
    do{                                         \
        VENC_OBJECT_T *pt;                      \
        ASSERT(node);                           \
        pt = (VENC_OBJECT_T *)&_tVEncObjHead;   \
        while (pt->next)                        \
        {                                       \
            if (pt->next == node)               \
            {                                   \
                break;                          \
            }                                   \
            pt = pt->next;                      \
        }                                       \
        if (!pt->next)                          \
        {                                       \
            pt->next   = node;                  \
            node->next = NULL;                  \
        }                                       \
    }while(0)


#define VENC_LINK_REMOVE(node)                   \
    do{                                          \
        VENC_OBJECT_T *pt;                       \
        ASSERT(node);                            \
        pt = (VENC_OBJECT_T *)&_tVEncObjHead;    \
        while (pt->next)                         \
        {                                        \
                                                 \
            if (pt->next == node)                \
            {                                    \
                break;                           \
            }                                    \
            pt = pt->next;                       \
        }                                        \
        if (pt->next)                            \
        {                                        \
            pt->next = pt->next->next;           \
        }                                        \
    }while(0)

//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------
extern VENC_OBJECT_T *H264Open(void);

//-----------------------------------------------------------------------------
// Static function forward declarations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static VENC_OBJECT_HEAD_T _tVEncObjHead = {0};

//-----------------------------------------------------------------------------
// variables
//-----------------------------------------------------------------------------
VENC_OBJECT_HEAD_T *_ptVEncObjHead = &_tVEncObjHead;

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------


UINT32 _VEncSendCmd(VENC_OBJECT_T *pObj, VENC_CMD_T eVEncCmd)
{
    INT32 i4Ret;
    UINT32 u4Cmd = eVEncCmd;
    SIZE_T zMsgSize = sizeof(UINT32);
    VENC_FRMQ_DATA_T rFrm;

    _VENC_FUNC_ENTER();

    i4Ret = x_msg_q_send(pObj->hEncCmdQ, &u4Cmd, zMsgSize, VENC_CMDQ_PRIORITY);
    while (i4Ret == OSR_TOO_MANY)
    {
        x_thread_delay(10);
        i4Ret = x_msg_q_send(pObj->hEncCmdQ, &u4Cmd, zMsgSize, VENC_CMDQ_PRIORITY);
        LOG(0, "_VEncSendCmd, OSR_TOO_MANY\n");
    }

    ASSERT(i4Ret == OSR_OK);

    rFrm.eType = VENC_FRMQ_TYPE_DUMMY;
    _VENC_SendFrm(pObj, &rFrm);

    return TRUE;
}


UINT32 _VENC_ReceiveCmd(VENC_OBJECT_T *pObj, VOID *pvData)
{
    UINT16 u2MsgQIdx;
    SIZE_T zMsgSize;
    INT32 i4Ret;

    zMsgSize = sizeof(UINT32);

    if (pObj->eState != VENC_STATE_RUN)
    {
        i4Ret = x_msg_q_receive(&u2MsgQIdx, pvData, &zMsgSize,
                                &(pObj->hEncCmdQ), 1, X_MSGQ_OPTION_WAIT);
    }
    else
    {
        i4Ret = x_msg_q_receive(&u2MsgQIdx, pvData, &zMsgSize,
                                &(pObj->hEncCmdQ), 1, X_MSGQ_OPTION_NOWAIT);
    }

    ASSERT(zMsgSize == sizeof(UINT32));

    if (i4Ret != OSR_OK)
    {
        return VENC_INTERNAL_ERR;
    }

    return VENC_OK;
}


VOID _VENC_ProcessCmd(VENC_OBJECT_T *pObj, UINT32 u4Cmd, BOOL *pfgRun)
{
    VENC_CMD_T eCmd = (VENC_CMD_T)u4Cmd;

    if (eCmd == VENC_CMD_START_ASYNC || eCmd == VENC_CMD_START_SYNC)
    {
        _VENC_FUNC_ENTER();

        if (pObj->eState != VENC_STATE_STOP)
        {
            return;
        }

        pObj->eState    = VENC_STATE_RUN;
        pObj->eEncState = VENC_ENC_STATE_ENC;
    }
    else if (eCmd == VENC_CMD_STOP_ASYNC || eCmd == VENC_CMD_STOP_SYNC)
    {
        _VENC_FUNC_ENTER();

        pObj->eState    = VENC_STATE_STOP;
        pObj->eEncState = VENC_ENC_STATE_IDLE;
    }
    else if (eCmd == VENC_CMD_EXIT_ASYNC || eCmd == VENC_CMD_EXIT_SYNC)
    {
        pObj->eState    = VENC_STATE_STOP;
        pObj->eEncState = VENC_ENC_STATE_IDLE;
        *pfgRun = FALSE;
    }

    if ((((UINT8)eCmd)>>4) == 0X1)   //SYNC
    {
        _VEncNotifySyncCmdDone(pObj);
    }
}


static UINT32 _VEncEncRoutine(VENC_OBJECT_T *pObj)
{
    UINT16 u2MsgQIdx;
    SIZE_T zMsgSize;
    INT32 i4Ret;
    VENC_FRMQ_DATA_T tData;

    zMsgSize = sizeof(VENC_FRMQ_DATA_T);

    i4Ret = x_msg_q_receive(&u2MsgQIdx, (VOID*)&tData, &zMsgSize,
                            &(pObj->hEncFrmQ), 1, X_MSGQ_OPTION_WAIT);

    ASSERT(i4Ret == OSR_OK);

    if (VENC_FRMQ_TYPE_FRM == tData.eType)
    {
        pObj->EncFrameSync(pObj, &(tData.u.frm.tIn), &(tData.u.frm.tOut), (VOID*)&(tData.u.frm.tPram));

        //CALLBACK
        if (pObj->OutputCallback)
        {
            pObj->OutputCallback(pObj->pvCallbackData, &(tData.u.frm.tIn), &(tData.u.frm.tOut), 0);
        }
    }
    else if (VENC_FRMQ_TYPE_DUMMY == tData.eType)
    {
    }
    else
    {
        //todo
        //CALLBACK
        if (pObj->OutputCallback)
        {
            pObj->OutputCallback(pObj->pvCallbackData, &(tData.u.frm.tIn), &(tData.u.frm.tOut), 0);
        }
    }

    return VENC_OK;
}

static VOID _VEncMainLoop(VOID *pvArg)
{
    VENC_OBJECT_T *pObj = (VENC_OBJECT_T *)(*(UINT32 *)pvArg);
    UINT32 u4VEncCmd;
    BOOL fgRun = TRUE;

    _VENC_FUNC_ENTER();

    ASSERT(pObj);

    while (fgRun)
    {
        if (VENC_OK == _VENC_ReceiveCmd(pObj, &u4VEncCmd))
        {
            _VENC_ProcessCmd(pObj, u4VEncCmd, &fgRun);
        }

        if (VENC_ENC_STATE_ENC == pObj->eEncState)
        {
            _VEncEncRoutine(pObj);
        }
    }

    _VENC_FUNC_END();

    x_thread_exit();

}

static UINT32 _VEncSetParam(VENC_OBJECT_T *pObj, UINT32 u4Type, VOID *pvArg)
{
    UINT32 u4Ret = VENC_OK;

    _VENC_FUNC_ENTER();

    ASSERT(pObj && pvArg);

    switch(u4Type)
    {
    default:
        u4Ret = VENC_NOT_IMPL;
        break;
    case VENC_PRAM_BITRATE:
        pObj->u4Bitrate = (*(UINT32 *)pvArg);
        break;
    case VENC_PRAM_FRAMERATE:
        pObj->u1Framerate = (UINT8)(*(UINT32 *)pvArg);
        break;
    case VENC_PRAM_RESOLUTION:
    {
        VENC_PRAM_RES_T * pRes = (VENC_PRAM_RES_T *)pvArg;
        pObj->u2Height = pRes->u2Height;
        pObj->u2Width  = pRes->u2Width;
    }
        break;
    }

    return u4Ret;

}

static UINT32 _VEncGetParam(VENC_OBJECT_T *pObj, UINT32 u4Type, VOID *pvArg)
{
    UINT32 u4Ret = VENC_OK;

    _VENC_FUNC_ENTER();

    ASSERT(pObj && pvArg);

    switch(u4Type)
    {
    default:
        u4Ret = VENC_NOT_IMPL;
        break;
    case VENC_PRAM_BITRATE:
        (*(UINT32 *)pvArg) = pObj->u4Bitrate;
        break;
    case VENC_PRAM_FRAMERATE:
        (*(UINT32 *)pvArg) = pObj->u1Framerate;
        break;
    case VENC_PRAM_RESOLUTION:
    {
        VENC_PRAM_RES_T * pRes = (VENC_PRAM_RES_T *)pvArg;
        pRes->u2Height = pObj->u2Height;
        pRes->u2Width  = pObj->u2Width;
    }
        break;
    }

    return u4Ret;

}



static UINT32 _VEncSetCtrl(VENC_OBJECT_T *pObj, UINT32 u4Type, VOID *pvArg)
{
    UINT32 u4Ret = VENC_OK;

    _VENC_FUNC_ENTER();

    ASSERT(pObj && pvArg);

    switch(u4Type)
    {
    default:
        u4Ret = VENC_NOT_IMPL;
        break;

    case VENC_CTRL_SYNC_MODE:
        pObj->eSMode  = (VENC_SMODE_T)(*(UINT32 *)pvArg);
        break;

    case VENC_CTRL_PIX_FMT:
    {
        VENC_CTRL_PIXFMT_T *pPIx = (VENC_CTRL_PIXFMT_T *)pvArg;
        pObj->ePixFmt = pPIx->ePixFmt;
        pObj->u2Pitch = pPIx->u2Pitch;
    }
        break;

    case VENC_CTRL_BUFF:
    {
        VENC_CTRL_BUFF_T * pBuff = (VENC_CTRL_BUFF_T *)pvArg;
        pObj->u4BuffAddr = pBuff->u4Addr;
        pObj->u4BuffSize = pBuff->u4Size;
    }
        break;

    case VENC_CTRL_OUT_CB:
    {
        VENC_CTRL_OUTCB_T * pCB = (VENC_CTRL_OUTCB_T *)pvArg;
        pObj->OutputCallback = pCB->cb;
        pObj->pvCallbackData = pCB->pvData;
    }
        break;

    case VENC_CTRL_RZ:
    {
        VENC_CTRL_RZ_T * pRz = (VENC_CTRL_RZ_T *)pvArg;
        pObj->u4SrcPitch = pRz->u4SrcPitch;
        pObj->u4SrcWidth = pRz->u4SrcWidth;
        pObj->u4SrcHeight = pRz->u4SrcHeight;
        pObj->pvIn = pRz->pvIn;
        pObj->pvOut = pRz->pvOut;
    }
        break;
    }

    return u4Ret;

}


static UINT32 _VEncGetCtrl(VENC_OBJECT_T *pObj, UINT32 u4Type, VOID *pvArg)
{
    UINT32 u4Ret = VENC_OK;

    _VENC_FUNC_ENTER();

    ASSERT(pObj && pvArg);

    switch(u4Type)
    {
    default:
        u4Ret = VENC_NOT_IMPL;
        break;
    case VENC_CTRL_PIX_FMT:
    {
        VENC_CTRL_PIXFMT_T *pPIx = (VENC_CTRL_PIXFMT_T *)pvArg;
        pPIx->ePixFmt = pObj->ePixFmt;
        pPIx->u2Pitch = pObj->u2Pitch;
    }
        break;
    case VENC_CTRL_BUFF:
    {
        VENC_CTRL_BUFF_T * pBuff = (VENC_CTRL_BUFF_T *)pvArg;
        pBuff->u4Addr = pObj->u4BuffAddr;
        pBuff->u4Size = pObj->u4BuffSize;
    }
        break;
    case VENC_CTRL_INT_GET_STATE:
    {
        VENC_CTRL_STATE * pState = (VENC_CTRL_STATE *)pvArg;
        pState->eState    = pObj->eState;
        pState->eEncState = pObj->eEncState;
    }
        break;
    case VENC_CTRL_INT_GET_NAME:
        x_strncpy((char*)(pvArg), (char*)(pObj->u1aName), sizeof(pObj->u1aName));
        break;
    case VENC_CTRL_INT_GET_TYPE:
        (*(UINT32 *)pvArg) = pObj->u4Type;
        break;
    }

    return u4Ret;

}



//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
UINT32 _VENC_Open(VENC_HANDLE *phEnc, UINT32 u4Type, VENC_PROFILE_T eProfile)
{
    UINT32 ret = VENC_OK;
    VENC_OBJECT_T *pObj = NULL;

    _VENC_FUNC_BEGIN();

    if (NULL == phEnc)
    {
        return VENC_INV_ARG;
    }

    switch(u4Type)
    {
    case VENC_H264:
        if (!IS_SupportH264EN())
        {
            return VENC_FAIL;
        }
        pObj = H264Open();
        break;
    default:
        return VENC_NOT_IMPL;
    }

    if (!pObj)
    {
        return VENC_DEV_BUSY;
    }

    pObj->u4Type    = u4Type;

    //default settings
    pObj->eSMode    = VENC_SMODE_DUAL;
    pObj->eState    = VENC_STATE_STOP;
    pObj->eEncState = VENC_ENC_STATE_IDLE;

    pObj->pfnEncMainLoop = _VEncMainLoop;

    pObj->hParam = pObj->CreateParam(eProfile);

    pObj->hCtrl  = pObj->CreateCtrl(eProfile);


    if ((pObj->hParam == VENC_NULL_HANDLE)
        ||(pObj->hCtrl == VENC_NULL_HANDLE))
    {

        pObj->Close(pObj);
        ret = VENC_FAIL;
    }

    pObj->eProfile = eProfile;

    *phEnc = VENC_OBJECT_TO_HANDLE(pObj);

    VENC_LINK_INSERT(pObj);

    _VENC_FUNC_END();

    return ret;
}

UINT32 _VENC_Close(VENC_HANDLE hEnc)
{
    VENC_OBJECT_T *pObj = NULL;

    _VENC_FUNC_BEGIN();

    if (VENC_NULL_HANDLE == hEnc)
    {
        return VENC_INV_HANDLE;
    }

    pObj = VENC_HANDLE_TO_OBJECT(hEnc,VENC_OBJECT_T);

    if (pObj->Close)
    {
        if (pObj->Close(pObj))
        {
            return VENC_FAIL;
        }
    }

    if (NULL_HANDLE != pObj->hEncThread)
    {
        _VEncInitSyncCmdFlag(pObj);
        _VEncSendCmd(pObj, VENC_CMD_EXIT_SYNC);
        _VEncWaitSyncCmdDone(pObj);
        pObj->hEncThread = NULL_HANDLE;
    }

    if (NULL_HANDLE != pObj->hEncCmdQ)
    {
        VERIFY(x_msg_q_delete(pObj->hEncCmdQ) == OSR_OK);
        pObj->hEncCmdQ = NULL_HANDLE;
    }

    if (NULL_HANDLE != pObj->hEncFrmQ)
    {
        VERIFY(x_msg_q_delete(pObj->hEncFrmQ) == OSR_OK);
        pObj->hEncFrmQ = NULL_HANDLE;
    }

    if (NULL_HANDLE != pObj->hEncSyncSema)
    {
        VERIFY(x_sema_delete(pObj->hEncSyncSema) == OSR_OK);
        pObj->hEncSyncSema = NULL_HANDLE;
    }

    VENC_LINK_REMOVE(pObj);

    _VENC_FUNC_END();

    return VENC_OK;

}


UINT32 _VENC_Init(VENC_HANDLE hEnc)
{
    UINT32 u4Ret = VENC_OK;
    VENC_OBJECT_T *pObj = NULL;

    _VENC_FUNC_BEGIN();


    if (VENC_NULL_HANDLE == hEnc)
    {
        return VENC_INV_HANDLE;
    }

    pObj = VENC_HANDLE_TO_OBJECT(hEnc,VENC_OBJECT_T);

    do
    {
        if (VENC_STATE_STOP != pObj->eState)
        {
            u4Ret = VENC_INTERNAL_ERR;
            break;
        }


        //SUB INIT
        if (pObj->Init)
        {
            u4Ret = pObj->Init(pObj);
            VENC_CHK_OK_BREAK(u4Ret);
        }

        //COMMON PART INIT
        // create a  thread to dispatch msg in the async mode
        if (VENC_SUPPORT_ASYNC_MODE(pObj->eSMode))
        {
            INT32 i4Ret = OSR_OK;
            CHAR acName[16];
            UINT16 u2Hash = (UINT32)pObj;

            // create venc command queue
            if (NULL_HANDLE == pObj->hEncCmdQ)
            {
                x_snprintf(acName, 16, "EnCQ%4X", u2Hash);
                i4Ret = x_msg_q_create(&(pObj->hEncCmdQ), acName/*"VENC_CMDQ"*/,
                    sizeof(UINT32), VENC_CMDQ_SIZE);
                ASSERT(i4Ret == OSR_OK);
                if (i4Ret != OSR_OK)
                {
                    u4Ret = VENC_FAIL;
                    break;
                }
            }

            // create venc frame queue
            if (NULL_HANDLE == pObj->hEncFrmQ)
            {
                x_snprintf(acName, 16, "EnFQ%4X", u2Hash);
                i4Ret = x_msg_q_create(&(pObj->hEncFrmQ), acName/*"VENC_FRMQ"*/,
                    sizeof(VENC_FRMQ_DATA_T), VENC_FRMQ_SIZE);

                ASSERT(i4Ret == OSR_OK);
                if (i4Ret != OSR_OK)
                {
                    u4Ret = VENC_FAIL;
                    break;
                }
            }

            //sema for sync cmd
            if (NULL_HANDLE == pObj->hEncSyncSema)
            {
                i4Ret = x_sema_create(&(pObj->hEncSyncSema), X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK);
                ASSERT(i4Ret == OSR_OK);
                if (i4Ret != OSR_OK)
                {
                    u4Ret = VENC_FAIL;
                    break;
                }
            }

            // create enc main loop thread for command receiving
            if (NULL_HANDLE == pObj->hEncThread)
            {
                UINT32 arg = (UINT32)pObj;
                x_snprintf(acName, 16, "EncT%4X", u2Hash);
                ASSERT(pObj->pfnEncMainLoop);
                i4Ret = x_thread_create(&(pObj->hEncThread),
                    acName/*"EncThread"*/, VENC_STACK_SIZE, VENC_THREAD_PRIORITY,
                    (x_os_thread_main_fct)(pObj->pfnEncMainLoop), sizeof(UINT32), (VOID *)&arg);
                ASSERT(i4Ret == OSR_OK);
                if (i4Ret != OSR_OK)
                {
                    u4Ret = VENC_FAIL;
                    break;
                }
            }
            UNUSED(u2Hash);
            UNUSED(acName);
        }

    } while(0);

    _VENC_FUNC_END();

    return u4Ret;
}


UINT32 _VENC_Run(VENC_HANDLE hEnc)
{
    VENC_OBJECT_T *pObj = NULL;
    UINT32 u4Ret = VENC_OK;

    _VENC_FUNC_BEGIN();

    if (VENC_NULL_HANDLE == hEnc)
    {
        return VENC_INV_HANDLE;
    }

    pObj = VENC_HANDLE_TO_OBJECT(hEnc,VENC_OBJECT_T);

    do
    {
        u4Ret = (VENC_STATE_STOP != pObj->eState) ? VENC_INTERNAL_ERR : VENC_OK;
        VENC_CHK_OK_BREAK(u4Ret);

        //COMMON PART RUN
        if (VENC_SUPPORT_ASYNC_MODE(pObj->eSMode))
        {
            ASSERT((NULL_HANDLE != pObj->hEncFrmQ) && (NULL_HANDLE != pObj->hEncCmdQ));

            _VEncInitSyncCmdFlag(pObj);

            _VEncSendCmd(pObj, VENC_CMD_START_SYNC);
            //wait SYNC cmd done
            //VERIFY(x_sema_lock(pObj->hEncSyncSema, X_SEMA_OPTION_WAIT) == OSR_OK);
            _VEncWaitSyncCmdDone(pObj);
        }

        //SUB RUN
        if (pObj->Run)
        {
            u4Ret = pObj->Run(pObj);
            if (u4Ret != VENC_OK)
            {
                pObj->eState = VENC_STATE_STOP;
                break;
            }
        }

        pObj->eState = VENC_STATE_RUN;

    } while(0);

    _VENC_FUNC_END();

    return u4Ret;

}


UINT32 _VENC_Stop(VENC_HANDLE hEnc)
{
    VENC_OBJECT_T *pObj = NULL;
    UINT32 u4Ret = VENC_OK;

    _VENC_FUNC_BEGIN();

    if (VENC_NULL_HANDLE == hEnc)
    {
        return VENC_INV_HANDLE;
    }

    pObj = VENC_HANDLE_TO_OBJECT(hEnc,VENC_OBJECT_T);

    do
    {
        u4Ret = (VENC_STATE_RUN != pObj->eState) ? VENC_INTERNAL_ERR : VENC_OK;
        VENC_CHK_OK_BREAK(u4Ret);

        //COMMON PART STOP
        if (VENC_SUPPORT_ASYNC_MODE(pObj->eSMode))
        {
            ASSERT((NULL_HANDLE != pObj->hEncFrmQ)
                &&(NULL_HANDLE != pObj->hEncCmdQ)
                &&(NULL_HANDLE != pObj->hEncSyncSema));

            _VEncInitSyncCmdFlag(pObj);

            _VEncSendCmd(pObj, VENC_CMD_STOP_SYNC);
            _VEncWaitSyncCmdDone(pObj);
        }

        //SUB STOP
        if (pObj->Stop)
        {
            u4Ret = pObj->Stop(pObj);
            VENC_CHK_OK_BREAK(u4Ret);
        }
    } while(0);

    _VENC_FUNC_END();

    return u4Ret;
}


UINT32 _VENC_SetParam(VENC_HANDLE hEnc, UINT32 u4Type, VOID *pvArg)
{
    UINT32 u4Ret = VENC_NOT_IMPL;

    VENC_OBJECT_T *pObj = NULL;

    _VENC_FUNC_BEGIN();

    if (VENC_NULL_HANDLE == hEnc)
    {
        return VENC_INV_HANDLE;
    }

    pObj = VENC_HANDLE_TO_OBJECT(hEnc,VENC_OBJECT_T);

    ASSERT(pObj->SetParam);

    if ((u4Ret = pObj->SetParam(pObj, u4Type, pvArg)) == VENC_NOT_IMPL)
    {
        //default PART
        if (VENC_COMM == (u4Type & 0x0000FF00))
        {
            u4Ret =  _VEncSetParam(pObj, u4Type, pvArg);
        }
    }

    _VENC_FUNC_END();

    return u4Ret;

}


UINT32 _VENC_GetParam(VENC_HANDLE hEnc, UINT32 u4Type, VOID *pvArg)
{
    UINT32 u4Ret = VENC_NOT_IMPL;

    VENC_OBJECT_T *pObj = NULL;

    _VENC_FUNC_BEGIN();

    if (VENC_NULL_HANDLE == hEnc)
    {
        return VENC_INV_HANDLE;
    }

    pObj = VENC_HANDLE_TO_OBJECT(hEnc,VENC_OBJECT_T);

    ASSERT(pObj->GetParam);

    if ((u4Ret = pObj->GetParam(pObj, u4Type, pvArg)) == VENC_NOT_IMPL)
    {
        //default PART
        if (VENC_COMM == (u4Type & 0x0000FF00))
        {
            u4Ret =  _VEncGetParam(pObj, u4Type, pvArg);
        }
    }

    _VENC_FUNC_END();

    return u4Ret;

}



UINT32 _VENC_SetCtrl(VENC_HANDLE hEnc, UINT32 u4Type, VOID *pvArg)
{
    UINT32 u4Ret = VENC_NOT_IMPL;
    VENC_OBJECT_T *pObj = NULL;

    _VENC_FUNC_BEGIN();


    if (VENC_NULL_HANDLE == hEnc)
    {
        return VENC_INV_HANDLE;
    }

    pObj = VENC_HANDLE_TO_OBJECT(hEnc,VENC_OBJECT_T);

    ASSERT(pObj->SetCtrl);

    if ((u4Ret = pObj->SetCtrl(pObj, u4Type, pvArg)) == VENC_NOT_IMPL)
    {
        //default PART
        if (VENC_COMM == (u4Type & 0x0000FF00))
        {
            u4Ret =  _VEncSetCtrl(pObj, u4Type, pvArg);
        }
    }

    _VENC_FUNC_END();

    return u4Ret;

}

UINT32 _VENC_GetCtrl(VENC_HANDLE hEnc, UINT32 u4Type, VOID *pvArg)
{
    UINT32 u4Ret = VENC_NOT_IMPL;
    VENC_OBJECT_T *pObj = NULL;

    _VENC_FUNC_BEGIN();


    if (VENC_NULL_HANDLE == hEnc)
    {
        return VENC_INV_HANDLE;
    }

    pObj = VENC_HANDLE_TO_OBJECT(hEnc,VENC_OBJECT_T);

    ASSERT(pObj->GetCtrl);

    if ((u4Ret = pObj->GetCtrl(pObj, u4Type, pvArg)) == VENC_NOT_IMPL)
    {
        //default PART
        if (VENC_COMM == (u4Type & 0x0000FF00))
        {
            u4Ret =  _VEncGetCtrl(pObj, u4Type, pvArg);
        }
    }

    _VENC_FUNC_END();

    return u4Ret;

}


UINT32 _VENC_EncFrameSync(VENC_HANDLE hEnc, VENC_IN_PIC_T *ptIn, VENC_OUT_PIC_T *ptOut)
{
    VENC_OBJECT_T *pObj = NULL;

    _VENC_FUNC_ENTER();

    if (VENC_NULL_HANDLE == hEnc)
    {
        return VENC_INV_HANDLE;
    }

    if (!ptIn || !ptOut)
    {
        return VENC_INV_ARG;
    }

    pObj = VENC_HANDLE_TO_OBJECT(hEnc,VENC_OBJECT_T);

    return (pObj->EncFrameSync == NULL) ? VENC_NOT_IMPL : pObj->EncFrameSync(pObj, ptIn, ptOut, NULL);
}

UINT32 _VENC_EncFrameAsync(VENC_HANDLE hEnc, VENC_IN_PIC_T *ptIn, VENC_OUT_PIC_T *ptOut)
{
    VENC_OBJECT_T *pObj = NULL;

    _VENC_FUNC_ENTER();

    if (VENC_NULL_HANDLE == hEnc)
    {
        return VENC_INV_HANDLE;
    }

    if (!ptIn)
    {
        return VENC_INV_ARG;
    }

    pObj = VENC_HANDLE_TO_OBJECT(hEnc,VENC_OBJECT_T);

    return (pObj->EncFrameAsync == NULL) ? VENC_NOT_IMPL : pObj->EncFrameAsync(pObj, ptIn, ptOut, NULL);
}


UINT32 _VENC_SetRtParamSync(VENC_HANDLE hEnc, VENC_RT_PARM_T *ptParm)
{
    VENC_OBJECT_T *pObj = NULL;

    _VENC_FUNC_ENTER();

    if (VENC_NULL_HANDLE == hEnc)
    {
        return VENC_INV_HANDLE;
    }

    pObj = VENC_HANDLE_TO_OBJECT(hEnc,VENC_OBJECT_T);

    return (pObj->SetRtParamSync == NULL) ? VENC_NOT_IMPL : pObj->SetRtParamSync(pObj, ptParm);
}

UINT32 _VENC_SetRtParamAsync(VENC_HANDLE hEnc, VENC_RT_PARM_T *ptParm)
{
    VENC_OBJECT_T *pObj = NULL;

    _VENC_FUNC_ENTER();

    if (VENC_NULL_HANDLE == hEnc)
    {
        return VENC_INV_HANDLE;
    }

    pObj = VENC_HANDLE_TO_OBJECT(hEnc,VENC_OBJECT_T);

    return (pObj->SetRtParamAsync == NULL) ? VENC_NOT_IMPL : pObj->SetRtParamAsync(pObj, ptParm);
}


UINT32 _VENC_SetRtCtrlSync(VENC_HANDLE hEnc, VENC_RT_CTRL_T *ptParm)
{
    VENC_OBJECT_T *pObj = NULL;

    _VENC_FUNC_ENTER();

    if (VENC_NULL_HANDLE == hEnc)
    {
        return VENC_INV_HANDLE;
    }

    pObj = VENC_HANDLE_TO_OBJECT(hEnc,VENC_OBJECT_T);

    return (pObj->SetRtCtrlSync == NULL) ? VENC_NOT_IMPL : pObj->SetRtCtrlSync(pObj, ptParm);
}

UINT32 _VENC_SetRtCtrlAsync(VENC_HANDLE hEnc, VENC_RT_CTRL_T *ptParm)
{
    VENC_OBJECT_T *pObj = NULL;

    _VENC_FUNC_ENTER();

    if (VENC_NULL_HANDLE == hEnc)
    {
        return VENC_INV_HANDLE;
    }

    pObj = VENC_HANDLE_TO_OBJECT(hEnc,VENC_OBJECT_T);

    return (pObj->SetRtCtrlAsync == NULL) ? VENC_NOT_IMPL : pObj->SetRtCtrlAsync(pObj, ptParm);
}



VOID _VENC_SendFrm(VENC_OBJECT_T *pObj, VENC_FRMQ_DATA_T *prFrm)
{
    INT32 i4Ret;
    SIZE_T zMsgSize = sizeof(VENC_FRMQ_DATA_T);

    _VENC_FUNC_ENTER();

    i4Ret = x_msg_q_send(pObj->hEncFrmQ, prFrm, zMsgSize, VENC_FRMQ_PRIORITY);
    while (i4Ret == OSR_TOO_MANY)
    {
        x_thread_delay(10);
        LOG(0, "VEnc SendFrm Queue, OSR_TOO_MANY\n");
        i4Ret = x_msg_q_send(pObj->hEncFrmQ, prFrm, zMsgSize, VENC_FRMQ_PRIORITY);
    }

    ASSERT(i4Ret == OSR_OK);
}


#ifdef VENC_DRAM_PROFILE
VOID _VENC_DramAccessByteStart(VENC_OBJECT_T *this)
{
    UINT32 u4Base;
    volatile HAL_DRAMC_T* _prDramcReg;
    HAL_DRAMC_T rDramcReg;

    u4Base =  DRAM_BASE;
    _prDramcReg = (HAL_DRAMC_T*) (u4Base + 0x180);

#ifdef CC_MT5395
    IO_REG32(u4Base, 0x18C) = (BSP_GetDomainClock(CAL_SRC_DMPLL) >> 1);
#else
    IO_REG32(u4Base, 0x18C) = (BSP_GetDomainClock(SRC_MEM_CLK) >> 1);
#endif
    rDramcReg = *_prDramcReg;
    rDramcReg.fgBmbp1 = 0;
    rDramcReg.fgBmbp2 = 0;
    rDramcReg.fgBmbp3 = 0;

    rDramcReg.u4Bmgp1ag = 12;
    *_prDramcReg = rDramcReg;

    rDramcReg.fgBmbp1 = 1;
    rDramcReg.fgReqAle = 0;
    *_prDramcReg = rDramcReg;
}


VOID _VENC_DramAccessByteEnd(VENC_OBJECT_T *this)
{
    UINT32 u4DramCycle = IO_REG32(DRAM_BASE, 0x190);
    u4DramCycle <<= 4; // *16
    Printf("%d: %d, %d\n", this->u4FrmCnt, u4DramCycle, this->u4DramTotalCycle);
    this->u4DramTotalCycle += u4DramCycle;
    this->u4FrmCnt++;
}
#endif


