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
 * $RCSfile: img_lib_if.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file img_lib_if.c
 *  Image library command state machine.
 */

/******************************************************************************
* Include files
******************************************************************************/
#include "img_flw.h"
#include "jpg_debug.h"
#include "drv_common.h"
#include "x_assert.h"
#include "jpg_if.h"
#include "dsp_jpg.h"

/******************************************************************************
* Local	Macros,	defines, typedefs, enums
******************************************************************************/
#define JPG_THREAD_NAME				"JPG"
#define JPG_STACK_SIZE					3072
#define JPG_THREAD_PRIORITY			80

#define JPG_CMD_SIZE					4
#define JPG_CMD_Q_SIZE					6
#define JPG_DATA_Q_SIZE					3

// for jpg thread synchronize dsp thread
#define JPG_WAIT_DSP_TIMEOUT 500
/******************************************************************************
* Function prototype
******************************************************************************/
static void _ImgSetCmd(UINT32 u4Mode);
static void _ImgReceiveCmd(BOOL bIsBlock);
static void _ImgSendData(HANDLE_T hDataQ, UINT32 u4DataSize, UINT32 u4DataAddr);
static void _ImgReceiveData(HANDLE_T hDataQ, UINT32 u4DataSize, void *pvData);
static void _ImgMainLoop(void* pvArg);

/******************************************************************************
* Local	variables
******************************************************************************/
static IMGHANDLE _hCurImg;
static HANDLE_T _hCmdQueue, _hDecDataQueue, _hSetDataQueue;
static HANDLE_T _hInitSema, _hReleaseSema, _hFillBufSema, _hWaitDspSema;
static UINT32 _u4CurState = IMG_RELEASE;
static BOOL _bImgModEnd = TRUE;
static UINT32 _u4NotifyType = JPG_HW_NFY_NONE;
static UINT32 _u4ImgID = 0;
/******************************************************************************
* Global Function definition
******************************************************************************/

/**
 * Initialize image library with async mode.
 *
 * @param void.
 * @return ENUM_IMG_RET.
 */
ENUM_IMG_RET IMG_Lib_Init(void)
{
    HANDLE_T hThread;

    if (_bImgModEnd == TRUE)
    {
        _bImgModEnd = FALSE;

        VERIFY(x_msg_q_create(&_hCmdQueue, "JPG_CMDQ", JPG_CMD_SIZE, JPG_CMD_Q_SIZE) == OSR_OK);
        VERIFY(x_msg_q_create(&_hDecDataQueue, "JPG_DECQ", sizeof(IMG_PARAM_T), JPG_DATA_Q_SIZE) == OSR_OK);
        VERIFY(x_msg_q_create(&_hSetDataQueue, "JPG_SETQ", sizeof(IMG_BUF_PARAM_T), JPG_DATA_Q_SIZE) == OSR_OK);

        VERIFY (x_sema_create(&_hInitSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
        VERIFY (x_sema_create(&_hReleaseSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
        VERIFY (x_sema_create(&_hFillBufSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
        VERIFY (x_sema_create(&_hWaitDspSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);

        //create main loop thread, create cmd msg queue, data msg queue
        VERIFY(x_thread_create(&hThread, JPG_THREAD_NAME, JPG_STACK_SIZE,
                               JPG_THREAD_PRIORITY,  _ImgMainLoop, 4, NULL) == OSR_OK);
    }
    return E_IMG_OK;
}

/**
 * Release image library with async mode.
 *
 * @param void.
 * @return void.
 */
void IMG_Lib_End(void)
{
    if (_bImgModEnd == FALSE)
    {
        VERIFY(x_msg_q_delete(_hCmdQueue) == OSR_OK);
        VERIFY(x_msg_q_delete(_hDecDataQueue) == OSR_OK);
        VERIFY(x_msg_q_delete(_hSetDataQueue) == OSR_OK);

        VERIFY(x_sema_delete(_hInitSema) == OSR_OK);
        VERIFY(x_sema_delete(_hReleaseSema) == OSR_OK);
        VERIFY(x_sema_delete(_hFillBufSema) == OSR_OK);
        VERIFY(x_sema_delete(_hWaitDspSema) == OSR_OK);
        _bImgModEnd = TRUE;
    }
}

/**
 * Initialize image module.
 *
 * @param eMemClass in/out buffer type.
 * @param pvTag private data.
 * @param pfnReg callback function pointer.
 * @return ENUM_IMG_RET
 */
ENUM_IMG_RET IMG_Init(ENUM_IMG_BUFFER_TYPE eMemClass, void *pvTag, UINT32 u4ImgID,
                      PFN_JPG_EVENT_NOTIFY pfnReg)
{
    _hCurImg = _IMG_Init(eMemClass, pvTag);
    if (_hCurImg == NULL)
    {
        return E_IMG_INIT_FAIL;
    }
    //register callback function
    _IMG_CBreg(pfnReg);
    _u4ImgID = u4ImgID;

    //send cmd msg queue, INIT
    _ImgSetCmd(IMG_INIT);
    VERIFY (x_sema_lock(_hInitSema, X_SEMA_OPTION_WAIT) == OSR_OK);

    return E_IMG_OK;
}


/**
 * Decode image and scale output raw data.
 *
 * @param rInfo input compressed image and output destination information.
 * @return ENUM_IMG_RET
 */
ENUM_IMG_RET IMG_Decode(IMG_PARAM_T rInfo)
{
    //send data msg queue
    //send cmd msg queue, DECODE

    _ImgSendData(_hDecDataQueue, sizeof(IMG_PARAM_T), (UINT32)&rInfo);
    _ImgSetCmd(IMG_DECODE);
    return E_IMG_OK;
}


/**
 * Scale operation.
 *
 * @param _IMGFillScBufCB fill scaling src buffer callback
 * @param  prScOptInfo operation parameter
 * @return void
 */
void IMG_Scale(PFN_JPG_FILL_SC_BUF pfnFillScBufCB, IMG_SCALE_INFO_T* prScOptInfo)
{
    _IMG_Scale(pfnFillScBufCB, prScOptInfo);
}


/**
 * Apply operations on decoded raw image.
 *
 * @param rCmd command for image operation(zoom and rotate).
 * @return void
 */
void IMG_Set(IMG_BUF_PARAM_T rCmd)
{
    _IMG_Set(rCmd);
}


/**
 * Query module capability.
 *
 * @param rCap max supported image size and buffer size.
 * @return void.
 */
void IMG_Get(IMG_CAP_T* prCap)
{
    //get information
    _IMG_Get(prCap);
}

/**
 * Set Image File Size.
 *
 * @Image File Size.
 * @return void
 */
void IMG_Set_ImgSize(UINT32 u4ImgSize)
{
    _IMG_SetImgSize(_hCurImg, u4ImgSize);
}


/**
 * Query decoding progress.
 *
 * @param decoding process.
 * @return void
 */
void IMG_GetProg(UINT32* pu4Status)
{
    _IMG_GetProgess(_hCurImg, pu4Status);
}

/**
 * Query decoding capabilities.
 *
 * @param ptr of decoding capabilities.
 * @return void
 */
void IMG_GetCap(VOID *pv_get_info)
{
    IMG_LIMIT_T *prLimit = (IMG_LIMIT_T *)pv_get_info;

    if (prLimit->u4Type == IMG_LIMIT_PROGRESSIVE)
    {
        prLimit->u.fgProgressiveSupport = TRUE;
    }
    else if (prLimit->u4Type == IMG_LIMIT_PGSV_RESOLUTION)
    {
        prLimit->u.t_pgsv_resolution.u4MaxArea = (UINT32)(4000 * 4000);
        prLimit->u.t_pgsv_resolution.u4MinWidth = 4;
        prLimit->u.t_pgsv_resolution.u4MinHeight = 4;
    }
    else if (prLimit->u4Type == IMG_LIMIT_RESOLUTION)
    {
        prLimit->u.t_resolution.u4MaxWidth = 17000;
        prLimit->u.t_resolution.u4MaxHeight = 10000;
        prLimit->u.t_resolution.u4MinWidth = 4;
        prLimit->u.t_resolution.u4MinHeight = 4;
    }
    else
    {
        prLimit->u.fgFullModeSpport = FALSE;
    }

}

extern VOID vJpegSetProgressiveStop(void);
/**
 * Stop decoding procedure, sync cmd.
 *
 * @param void.
 * @return ENUM_IMG_RET
 */
void IMG_Stop(void)
{
    //send cmd  msg queue, CANCEL
    _ImgSetCmd(IMG_STOP);
    //vJpegSetProgressiveStop();
    _IMG_Stop(_hCurImg);
}

/**
 * Release handler, sync cmd.
 *
 * @param void.
 * @return ENUM_IMG_RET
 */
void IMG_Release(void)
{
    //send cmd msg queue, RELEASE
    _ImgSetCmd(IMG_RELEASE);
    VERIFY (x_sema_lock(_hReleaseSema, X_SEMA_OPTION_WAIT) == OSR_OK);
}

/**
 * Connect video path.
 *
 * @param video plane id.
 * @return ENUM_IMG_RET
 */
ENUM_IMG_RET IMG_VDP_Connect(UINT32 u4VideoId)
{
    return _IMG_VDP_Connect(u4VideoId);
}


/**
 * Query video plane info, sync cmd.
 *
 * @param video plane information.
 * @return ENUM_IMG_RET
 */
ENUM_IMG_RET IMG_GetVideoPlaneInfo(IMG_VP_INFO_T *prVpInfo)
{
    return _IMG_GetVideoPlaneInfo(prVpInfo);
}


/**
 * Flip double buffer for video plane, swap pu1CurFrameAddr and pu1BackFrameAddr, sync cmd.
 *
 * @param void.
 * @return ENUM_IMG_RET
 */
ENUM_IMG_RET IMG_Flip(void)
{
    return _IMG_Flip();
}


/**
 * Set status while buffer filling operation finish.
 *
 * @param u4Entry1Size buffer entry1 filled size.
 * @param u4Entry2Size buffer entry2 filled size.
 * @param u4IsEOF flag if end of image file.
 * @return void
 */
void IMG_SetFillBsBufEnd(UINT32 u4Entry1Size, UINT32 u4Entry2Size, BOOL bIsEOF)
{
    //send cmd msg queue, FILLBUF
    _IMG_SetFillBsBufEnd(_hCurImg, u4Entry1Size, u4Entry2Size, bIsEOF);

    if (_u4CurState == IMG_BSBUF_UNDERFLOW)
    {
        VERIFY (x_sema_unlock(_hFillBufSema) == OSR_OK);
    }
}

/**
 * Get internal buffer info.
 *
 * @param prImgBufInfo buffer description.
 * @return void.
 */
void IMG_GetBufInfo(IMG_BUF_INFO_T* prImgBufInfo)
{
    _IMG_GetBufInfo(prImgBufInfo);
}

/**
 * Get internal bitstream ring buffer info.
 *
 * @param prStreamBufInfo buffer description.
 * @return UINT32 available size.
 */
UINT32 IMG_GetBSBufInfo(IMG_LIB_FILL_RING_BUF_T *prStreamBufInfo)
{
    return _IMG_GetBSBufInfo(_hCurImg, prStreamBufInfo);
}

/**
 * Get state of decoding process.
 *
 * @param void.
 * @return decoding state.
 */
UINT32 IMG_GetDecState(void)
{
    return _u4CurState;
}

ENUM_IMG_RET IMG_SetUseMw(BOOL fgUseMw)
{
    IMG_LIB_INFO_T* prImg = (IMG_LIB_INFO_T*)_hCurImg;
    if (!prImg)
    {
        return E_IMG_INIT_FAIL;
    }

    prImg->bUseMw = fgUseMw;
    return E_IMG_OK;
}

BOOL _IMGRZ_UsedByJpeg(void)
{
    return FALSE;
}


INT32 IMG_BsBufManager(VOID)
{
    INT32 i4Ret;

    _u4CurState = IMG_BSBUF_UNDERFLOW;
    i4Ret = _IMG_BsBufManager((IMGHANDLE)_hCurImg, _u4ImgID);
    if (i4Ret == (INT32)E_IMG_OK)
    {
        VERIFY (x_sema_lock(_hFillBufSema, X_SEMA_OPTION_WAIT) == OSR_OK);
        _u4CurState = IMG_DECODE;
    }

    return i4Ret;
}

/******************************************************************************
* Local	Function definition
******************************************************************************/
static void _ImgSetCmd(UINT32 u4Cmd)
{
    INT32 i4Ret;
    UINT16 u2MsgQIdx;
    SIZE_T zMsgSize;
    UINT32 u4CmdDump;

    i4Ret = x_msg_q_send(_hCmdQueue, (void *) & u4Cmd, JPG_CMD_SIZE, 255);

    if (i4Ret == OSR_TOO_MANY)
    {
        zMsgSize = JPG_CMD_SIZE;
        VERIFY(x_msg_q_receive(&u2MsgQIdx, &u4CmdDump, &zMsgSize,
                               &(_hCmdQueue), 1, X_MSGQ_OPTION_NOWAIT) == OSR_OK);
        i4Ret = x_msg_q_send(_hCmdQueue, (void *) & u4Cmd, JPG_CMD_SIZE, 255);
    }
    else
    {
        VERIFY(i4Ret == OSR_OK);
    }

    LOG(6, "Img Set command %x\n", u4Cmd);

}

static void _ImgReceiveCmd(BOOL bIsBlock)
{
    UINT16 u2MsgQIdx;
    SIZE_T zMsgSize;
    UINT32 u4Cmd;
    INT32 i4Ret;

    zMsgSize = JPG_CMD_SIZE;

    if (bIsBlock)
    {
        i4Ret = x_msg_q_receive(&u2MsgQIdx, &u4Cmd, &zMsgSize,
                                &(_hCmdQueue), 1, X_MSGQ_OPTION_WAIT);
    }
    else
    {
        i4Ret = x_msg_q_receive(&u2MsgQIdx, &u4Cmd, &zMsgSize,
                                &(_hCmdQueue), 1, X_MSGQ_OPTION_NOWAIT);
        if (i4Ret == OSR_NO_MSG)
        {
            return;
        }
    }

    if (i4Ret == OSR_OK)
    {
        ASSERT(zMsgSize == JPG_CMD_SIZE);
    }

    //filter illegal command
    switch (u4Cmd)
    {
        case IMG_STOP:
            if (_u4CurState == IMG_DECODE)
            {
                _u4NotifyType = JPG_HW_NFY_STOP_DONE;
            }
            else
            {
                _u4NotifyType = JPG_HW_NFY_ALREADY_STOPPED;
            }
            _u4CurState = IMG_STOP;
            return;
            //break;
        case IMG_INIT:
            if (_u4CurState == IMG_RELEASE)
            {
                _u4CurState = IMG_INIT;
                return;
            }
            break;
        case IMG_DECODE:
            if (_u4CurState == IMG_INIT)
            {
                _u4CurState = IMG_DEC_PREPARE;
                return;
            }
            break;
        case IMG_SET:
            if (_u4CurState == IMG_IDLE)
            {
                _u4CurState = IMG_SET;
                return;
            }
            break;
        case IMG_RELEASE:
            _u4CurState = IMG_RELEASE;
            return;
            //break;
        default:
            break;
    }

    if (_u4CurState == IMG_DECODE)
    {
        _ImgReceiveCmd(FALSE);
    }
    else
    {
        _ImgReceiveCmd(TRUE);
    }
}


static void _ImgSendData(HANDLE_T hDataQ, UINT32 u4DataSize, UINT32 u4DataAddr)
{
    INT32 i4Ret;
    UINT16 u2MsgQIdx;
    SIZE_T zMsgSize;
    void *pvDataDump;

    i4Ret = x_msg_q_send(hDataQ, (void *)u4DataAddr, u4DataSize, 255);

    if (i4Ret == OSR_TOO_MANY)
    {
        zMsgSize = u4DataSize;
        pvDataDump = x_mem_alloc(zMsgSize);
        VERIFY(x_msg_q_receive(&u2MsgQIdx, pvDataDump, &zMsgSize,
                               &hDataQ, 1, X_MSGQ_OPTION_NOWAIT) == OSR_OK);
        x_mem_free(pvDataDump);
        i4Ret = x_msg_q_send(hDataQ, (void *)u4DataAddr, u4DataSize, 255);
    }
    else
    {
        VERIFY(i4Ret == OSR_OK);
    }
}


static void _ImgReceiveData(HANDLE_T hDataQ, UINT32 u4DataSize, void *pvData)
{
    UINT16 u2MsgQIdx;
    SIZE_T zMsgSize;
    INT32 i4Ret;

    zMsgSize = u4DataSize;

    i4Ret = x_msg_q_receive(&u2MsgQIdx, pvData, &zMsgSize,
                            &hDataQ, 1, X_MSGQ_OPTION_WAIT);

    if (i4Ret == OSR_OK)
    {
        ASSERT(zMsgSize == u4DataSize);
    }
}


static void _ImgMainLoop(void* pvArg)
{
    IMG_PARAM_T rInfo;
    IMG_BUF_PARAM_T rCmd;
    IMG_LIB_INFO_T* prImg;
    INT32 i4Ret;

    UNUSED(pvArg);
    x_memset((void *)&rInfo, 0, sizeof(IMG_PARAM_T));
    x_memset((void *)&rCmd, 0, sizeof(IMG_BUF_PARAM_T));

    while (!_bImgModEnd)
    {
        if (_u4CurState == IMG_DECODE)
        {
            _ImgReceiveCmd(FALSE);
        }
        else
        {
            _ImgReceiveCmd(TRUE);
        }

        //implementation for each state
        switch (_u4CurState)
        {
            case IMG_INIT:
                VERIFY (x_sema_unlock(_hInitSema) == OSR_OK);
                break;
            case IMG_IDLE:
                break;
            case IMG_DEC_PREPARE:
                prImg = (IMG_LIB_INFO_T*)_hCurImg;
                if (_hCurImg == NULL)
                {
                    _u4CurState = IMG_RELEASE;
                    break;
                }

                _u4CurState = IMG_DECODE;
                //receive data
                _ImgReceiveData(_hDecDataQueue, sizeof(IMG_PARAM_T), (void *)&rInfo);
                //check parameter
                if (_IMG_CheckDecParam(_hCurImg, rInfo) != (INT32)E_IMG_OK)
                {
                    //notify error
                    _IMG_NotifyStatus(_hCurImg, _u4ImgID, IMG_DRV_HW_ERROR, JPG_HW_NFY_ERROR);
                    _IMG_Release(&_hCurImg);
                    _u4CurState = IMG_RELEASE;
                    break;
                }
	            //todo 53jpg interface
	           vGetDTVJpgInterface(prImg->hCurDec);
                  vSetDTVJpgInterface(prImg);
                  
                //--- fill buffer

                if (prImg->bUseMw)
                {
                    _u4CurState = IMG_BSBUF_UNDERFLOW;
                    if (_IMG_BsBufManager(_hCurImg, _u4ImgID) != (INT32)E_IMG_OK)
                    {
                        //notify error
                        _IMG_NotifyStatus(_hCurImg, _u4ImgID, IMG_DRV_HW_ERROR, JPG_HW_NFY_ERROR);
                        _IMG_Release(&_hCurImg);
                        _u4CurState = IMG_RELEASE;
                    }
                    else
                    {
                        if (prImg->u1MemClass != (UINT8)EXTERNAL_RING_IN_SINGLE_OUT)
                        {
                            VERIFY (x_sema_lock(_hFillBufSema, X_SEMA_OPTION_WAIT) == OSR_OK);
                        }
                        _u4CurState = IMG_DECODE;
                        //dsp_jpg
                        fgJpeg_Play();

                    }
                }

                //--- header parsing and decoding setting prepare
                i4Ret = _IMG_DecPrepare(_hCurImg, rInfo);
                if (i4Ret != (INT32)E_IMG_OK)
                {
                    if (i4Ret == (INT32)E_IMG_FMT_UNSUPPORT)
                    {
                        //notify error
                        _IMG_NotifyStatus(_hCurImg, _u4ImgID, IMG_DRV_NOT_SUPPORT, JPG_HW_NFY_ERROR);
                    }
                    else
                    {
                        _IMG_NotifyStatus(_hCurImg, _u4ImgID, IMG_DRV_HW_ERROR, JPG_HW_NFY_ERROR);
                    }
                    _IMG_Release(&_hCurImg);
                    _u4CurState = IMG_RELEASE;
                }


	          vGetDTVJpgInterface_2(prImg, rInfo.eRotateDegree);
	           
                break;
            case IMG_DECODE:
                prImg = (IMG_LIB_INFO_T*)_hCurImg;
                if (_hCurImg == NULL)   //for lint happy
                {
                    _u4CurState = IMG_RELEASE;
                    break;
                }
                else
                {
                    if (prImg->bUseMw)
                    {
                        _u4CurState = IMG_BSBUF_UNDERFLOW;
                    }
                }
                //dsp_jpg
                i4Ret = u4ImageDecode(_hCurImg, _u4ImgID, rInfo.eRotateDegree);
                if (i4Ret == (INT32)E_IMG_DEC_FINISH)
                {
                    //notify decoding done
                    _IMG_NotifyStatus(_hCurImg, _u4ImgID, 0, JPG_HW_NFY_FINISHED);
                    if (prImg->bUseMw)
                    {
                        _u4CurState = IMG_IDLE;

                    }
                    else
                    {
                        _IMG_Release(&_hCurImg);
                        _u4CurState = IMG_RELEASE;
                    }
                }
                else if (i4Ret == (INT32)E_IMG_BSBUF_UNDERFLOW)
                {
                    _u4CurState = IMG_BSBUF_UNDERFLOW;
                    VERIFY (x_sema_lock(_hFillBufSema, X_SEMA_OPTION_WAIT) == OSR_OK);
                    _u4CurState = IMG_DECODE;
                }
                else if (i4Ret != (INT32)E_IMG_OK)
                {
                    //notify error
                    if (i4Ret == E_IMG_CANCEL)
                    {
                        _IMG_NotifyStatus(_hCurImg, _u4ImgID,  0, JPG_HW_NFY_STOP_DONE);
                    }
                    else if (i4Ret == E_IMG_HWBUSY)
                    {
                        _IMG_NotifyStatus(_hCurImg, _u4ImgID,  IMG_DRV_BUSY, JPG_HW_NFY_ERROR);
                    }
                    else if (i4Ret == E_IMG_FMT_UNSUPPORT)
                    {
                        //notify error
                        _IMG_NotifyStatus(_hCurImg, _u4ImgID, IMG_DRV_NOT_SUPPORT, JPG_HW_NFY_ERROR);
                    }                    
                    else
                    {
                        _IMG_NotifyStatus(_hCurImg, _u4ImgID,  IMG_DRV_HW_ERROR, JPG_HW_NFY_ERROR);
                    }
                    _IMG_Release(&_hCurImg);
                    _u4CurState = IMG_RELEASE;
                }
                else
                {
                    _u4CurState = IMG_DECODE;
                }
                break;
            case IMG_SET:
                //receive data
                _ImgReceiveData(_hSetDataQueue, sizeof(IMG_BUF_PARAM_T), (void *)&rCmd);
                _IMG_Set(rCmd);
                _IMG_NotifyStatus(_hCurImg, _u4ImgID, 0, JPG_HW_NFY_FINISHED);
                _u4CurState = IMG_IDLE;
                break;
            case IMG_STOP:
                _IMG_NotifyStatus(_hCurImg, _u4ImgID, 0, _u4NotifyType);
                _u4NotifyType = JPG_HW_NFY_NONE;
                break;
            case IMG_RELEASE:
	            //todo 53jpg interface
	           vRetDTVJpgInterface();
				
                _IMG_Release(&_hCurImg);
                VERIFY (x_sema_unlock(_hReleaseSema) == OSR_OK);

                break;
            default:
                break;
        }
    }
}

void IMG_UnLockWaitDspSema(void)
{
    VERIFY (x_sema_unlock(_hWaitDspSema) == OSR_OK);
}

void IMG_LockWaitDspSema(void)
{
#if 1
    VERIFY (x_sema_lock(_hWaitDspSema, X_SEMA_OPTION_WAIT) == OSR_OK);
#elif 0
    INT32 i4Ret;    
    i4Ret =x_sema_lock_timeout(_hWaitDspSema, JPG_WAIT_DSP_TIMEOUT);
    
    if (i4Ret == OSR_TIMEOUT)
    {
        LOG(1, "Wait dsp iterrupt timeout\n");
    }  
#else
    //VERIFY (x_sema_lock(_hWaitDspSema, X_SEMA_OPTION_WAIT) == OSR_OK);
    x_thread_delay(10);
#endif
}


