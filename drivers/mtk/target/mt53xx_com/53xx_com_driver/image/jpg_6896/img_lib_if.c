/*----------------------------------------------------------------------------*
 * No Warranty                                                                *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MTK with respect to any MTK *
 * Deliverables or any use thereof, and MTK Deliverables are provided on an   *
 * "AS IS" basis.  MTK hereby expressly disclaims all such warranties,        *
 * including any implied warranties of merchantability, non-infringement and  *
 * fitness for a particular purpose and any warranties arising out of course  *
 * of performance, course of dealing or usage of trade.  Parties further      *
 * acknowledge that Company may, either presently and/or in the future,       *
 * instruct MTK to assist it in the development and the implementation, in    *
 * accordance with Company's designs, of certain softwares relating to        *
 * Company's product(s) (the "Services").  Except as may be otherwise agreed  *
 * to in writing, no warranties of any kind, whether express or implied, are  *
 * given by MTK with respect to the Services provided, and the Services are   *
 * provided on an "AS IS" basis.  Company further acknowledges that the       *
 * Services may contain errors, that testing is important and Company is      *
 * solely responsible for fully testing the Services and/or derivatives       *
 * thereof before they are used, sublicensed or distributed.  Should there be *
 * any third party action brought against MTK, arising out of or relating to  *
 * the Services, Company agree to fully indemnify and hold MTK harmless.      *
 * If the parties mutually agree to enter into or continue a business         *
 * relationship or other arrangement, the terms and conditions set forth      *
 * hereunder shall remain effective and, unless explicitly stated otherwise,  *
 * shall prevail in the event of a conflict in the terms in any agreements    *
 * entered into between the parties.                                          *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Copyright (c) 2005, MediaTek Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *-----------------------------------------------------------------------------
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
#include "jpg_hw.h"
#include "drvcust_if.h"

#ifndef CC_IMAGE_PNG_DISABLE
#include "../png/png_drv.h"
#include "../png/png_hal_dec.h"
#endif

#ifndef CC_IMAGE_WEBP_DISABLE
#include "../webp/webp_drv.h"
#endif

/******************************************************************************
* Local	Macros,	defines, typedefs, enums
******************************************************************************/
#define JPG_THREAD_NAME				"JPG"
#define JPG_STACK_SIZE					3072
#define JPG_THREAD_PRIORITY			80

#define JPG_CMD_SIZE					4
#define JPG_CMD_Q_SIZE					6
#define JPG_DATA_Q_SIZE					3

/******************************************************************************
* Function prototype
******************************************************************************/
static void _ImgSetCmd(UINT32 u4Mode);
static void _ImgReceiveCmd(BOOL bIsBlock);
static void _ImgSendData(HANDLE_T hDataQ, UINT32 u4DataSize, UINT32 u4DataAddr);
static void _ImgReceiveData(HANDLE_T hDataQ, UINT32 u4DataSize, void *pvData);
static void _ImgMainLoop(void* pvArg);
void _ImgHWPowerTurnOff(void);

/******************************************************************************
* Local	variables
******************************************************************************/
static IMGHANDLE _hCurImg;
static HANDLE_T _hCmdQueue, _hDecDataQueue, _hSetDataQueue;
static HANDLE_T _hInitSema, _hReleaseSema, _hFillBufSema;
#ifndef CC_IMAGE_PNG_DISABLE
static HANDLE_T	_hPngSema;
#endif

static UINT32 _u4CurState = IMG_RELEASE;
static BOOL _bImgModEnd = TRUE;
static UINT32 _u4NotifyType = JPG_HW_NFY_NONE;
static UINT32 _u4ImgID = 0;
static IMG_INFO_T _rImgInfo;
#ifdef __KERNEL__
extern UINT32 VIRTUAL(UINT32);
#endif
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
#ifndef CC_IMAGE_PNG_DISABLE
        VERIFY(i4PngHalInitHwInst() == OSR_OK);
        VERIFY(x_sema_create(&_hPngSema, X_SEMA_TYPE_MUTEX, X_SEMA_STATE_UNLOCK) == OSR_OK);
#endif
		//create main loop thread, create cmd msg queue, data msg queue
        VERIFY(x_thread_create(&hThread, JPG_THREAD_NAME, JPG_STACK_SIZE,
                               JPG_THREAD_PRIORITY,  _ImgMainLoop, 4, NULL) == OSR_OK);
    }

    _ImgHWPowerTurnOff();

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
        VERIFY(x_msg_q_delete(_hCmdQueue)== OSR_OK);
        VERIFY(x_msg_q_delete(_hDecDataQueue)== OSR_OK);
        VERIFY(x_msg_q_delete(_hSetDataQueue)== OSR_OK);

        VERIFY(x_sema_delete(_hInitSema) == OSR_OK);
        VERIFY(x_sema_delete(_hReleaseSema) == OSR_OK);
        VERIFY(x_sema_delete(_hFillBufSema) == OSR_OK);
		
#ifndef CC_IMAGE_PNG_DISABLE
        VERIFY(i4PngHalUnInitHwInst() == OSR_OK);
        VERIFY(x_sema_delete(_hPngSema) == OSR_OK);
#endif
        _bImgModEnd = TRUE;
    }
}

 #ifdef IMG_INPUT_CHECKSUM
extern UINT32 _u4Entry1StartAddr ;
extern UINT32 _u4Entry2StartAddr ;
UINT32 _CalcCheckSum(void* pvData, UINT32 u4ByteLen)
{
    UINT32* pu4Data;
    UINT32 u4Sum = 0;

    //ASSERT(pvData!=NULL);
    if(pvData == NULL)
    {
       LOG(1,"JPG _CalcCheckSum pvData == NULL");
       return 0;
    }
    pu4Data = (UINT32*)pvData;
    while (u4ByteLen > 3)
    {
        u4Sum += *pu4Data;
        if (u4ByteLen >= 4)
        {
            u4ByteLen -= 4;
        }
        else
        {
            u4ByteLen = 0;
        }
        pu4Data++;
    }

    return u4Sum;
}
UINT32 _u4Checksum = 0;
UINT32 _u4Size1 = 0;
UINT32 _u4Size2 = 0;
UINT32 _u4Checksum1 = 0;
#endif

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
    IMG_INFO_T *prImgInfo = _IMG_GetInfo();
    if (prImgInfo->pfnImgInit)
    {
        _hCurImg = prImgInfo->pfnImgInit(eMemClass, pvTag);
    }
    else
    {
        return E_IMG_INIT_FAIL;
    }
    if (_hCurImg == NULL)
    {
        return E_IMG_INIT_FAIL;
    }
    //register callback function
    _IMG_CBreg(pfnReg);
    _u4ImgID = u4ImgID;
    
#ifdef IMG_INPUT_CHECKSUM
   _u4Checksum = 0;
    _u4Entry1StartAddr = 0;
    _u4Entry2StartAddr = 0;
    _u4Size1 = 0;
    _u4Size2 = 0;
    _u4Checksum1 = 0;
#endif
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
void IMG_Set(IMG_BUF_PARAM_T rCmd, IMG_REGION_T *rDstRegion)
{
    _IMG_Set(rCmd, rDstRegion);
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
    _IMG_SetImgSize(_hCurImg,u4ImgSize);
}


/**
 * Query decoding progress.
 *
 * @param decoding process.
 * @return void
 */
void IMG_GetProg(UINT32* pu4Status)
{
    IMG_INFO_T *prImgInfo = _IMG_GetInfo();
    if (prImgInfo->pfnImgGetInfo)
    {
        prImgInfo->pfnImgGetInfo(_hCurImg,IMG_GETINFO_PROGRESS,(UINT32)pu4Status,0);
    }
	return;
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
    else if (prLimit->u4Type == IMG_LIMIT_PNG_RESOLUTION)
    {
        prLimit->u.t_resolution.u4MaxArea = (UINT32)(5760 * 4096);
        prLimit->u.t_resolution.u4MaxWidth = 8192;
        prLimit->u.t_resolution.u4MinWidth = 4;
        prLimit->u.t_resolution.u4MinHeight = 4;
    }
    else
    {
#ifdef CC_JPEG_PIC_MODE
        prLimit->u.fgFullModeSpport = _IMG_IsFullImgDecoding();
#else
        prLimit->u.fgFullModeSpport = FALSE;
#endif
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
    IMG_INFO_T *prImgInfo = _IMG_GetInfo();
    _ImgSetCmd(IMG_STOP);
    vJpegSetProgressiveStop();
    if (prImgInfo->pfnImgStop)
    {
        prImgInfo->pfnImgStop(_hCurImg);
    }
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
    IMG_INFO_T *prImgInfo = _IMG_GetInfo();

    prImgInfo->pfnImgSetFillBsBufEnd(_hCurImg, u4Entry1Size, u4Entry2Size, bIsEOF);
    


#ifdef IMG_INPUT_CHECKSUM
    _u4Size1 = u4Entry1Size;
    _u4Size2 = u4Entry2Size;

    if(u4Entry1Size != 0)
    {
       _u4Checksum += _CalcCheckSum((void*)_u4Entry1StartAddr,u4Entry1Size);       
       LOG(7, "IMG_SetFillBsBufEnd _u4Entry1StartAddr = 0x%08x,u4Entry1Size = 0x%08x,ChecksumA = 0x%08x\n",_u4Entry1StartAddr,u4Entry1Size,_u4Checksum);
    }
    if(u4Entry2Size != 0)
    {
       _u4Checksum += _CalcCheckSum((void*)_u4Entry2StartAddr,u4Entry2Size);
       LOG(7, "IMG_SetFillBsBufEnd _u4Entry2StartAddr = 0x%08x,u4Entry2Size = 0x%08x,ChecksumB = 0x%08x\n",_u4Entry2StartAddr,u4Entry2Size,_u4Checksum);
    }

    if(bIsEOF == 1)
    {
        LOG(0, "IMG_SetFillBsBufEnd Checksum = 0x%08x!!!!!!!!!\n",_u4Checksum );
        _u4Checksum = 0;
    }
 #endif
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

    prImg->bUseMw=fgUseMw;
    return E_IMG_OK;
}

BOOL _IMGRZ_UsedByJpeg(void)
{
#ifdef CC_JPEG_PIC_MODE
    if (_u4CurState!=IMG_RELEASE)
    {
        return TRUE;
    }
#endif
    return FALSE;
}

//for verification only
INT32 IMG_GetOutputBuffer(void **ppvmem)
{
    return _IMG_GetOutputBuffer(_hCurImg, ppvmem);
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

INT32 IMG_BsGetData(UINT32 u4DstAddr, UINT32 u4Offset, UINT32 u4Size)
{
    INT32 i4Ret;

    _u4CurState = IMG_BSBUF_UNDERFLOW;
    i4Ret = _IMG_BsGetData((IMGHANDLE)_hCurImg, _u4ImgID, u4DstAddr, u4Offset, u4Size);
    if (i4Ret == (INT32)E_IMG_OK)
    {
        VERIFY (x_sema_lock(_hFillBufSema, X_SEMA_OPTION_WAIT) == OSR_OK);
        _u4CurState = IMG_DECODE;
    }

    return i4Ret;
}
INT32 IMG_BsBufStartTrsData(UINT32 u4TrsOfst)
{
    INT32 i4Ret = E_IMG_OK;
#ifndef CC_IMAGE_PNG_DISABLE
    IMG_LIB_INFO_T* prImg = (IMG_LIB_INFO_T*)_hCurImg;
    PNG_INST_T* prPngInst = (PNG_INST_T*)prImg->hCurDec;
    LOG(7, "IMG_BsBufStartTrsData u4TrsOfst = 0x%08x\n",u4TrsOfst);

    if(prPngInst->u4ImgSize <  (PNG_FIFO_SIZE - 64))
    {    
        prPngInst->tFifoInfo.u4RdPoint = u4TrsOfst + prPngInst->tFifoInfo.u4FifoStart;
        prPngInst->tFifoInfo.u4PreRdPoint = prPngInst->tFifoInfo.u4RdPoint;
        prPngInst->tFifoInfo.u4ValidData = prPngInst->u4ImgSize - u4TrsOfst;
    }
    else
#endif
    {
        _u4CurState = IMG_BSBUF_UNDERFLOW;
        i4Ret = _IMG_BsStartTrsData((IMGHANDLE)_hCurImg, _u4ImgID,u4TrsOfst);
        if (i4Ret == (INT32)E_IMG_OK)
        {
            VERIFY (x_sema_lock(_hFillBufSema, X_SEMA_OPTION_WAIT) == OSR_OK);
            _u4CurState = IMG_DECODE;
        }
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

    i4Ret = x_msg_q_send(_hCmdQueue, (void *)&u4Cmd, JPG_CMD_SIZE, 255);

    if (i4Ret == OSR_TOO_MANY)
    {
        zMsgSize = JPG_CMD_SIZE;
        VERIFY(x_msg_q_receive(&u2MsgQIdx, &u4CmdDump, &zMsgSize,
                               &(_hCmdQueue), 1, X_MSGQ_OPTION_NOWAIT) == OSR_OK);
        i4Ret = x_msg_q_send(_hCmdQueue, (void *)&u4Cmd, JPG_CMD_SIZE, 255);
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
    IMG_LIB_INFO_T* prImg=NULL;
    INT32 i4Ret;
    IMG_REGION_T rDstRegion;
    IMG_INFO_T *prImgInfo = _IMG_GetInfo();

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

            prImg=(IMG_LIB_INFO_T*)_hCurImg;

            if (_hCurImg==NULL)
            {
                _u4CurState=IMG_RELEASE;
                break;
            }
			
            if(prImgInfo->eImgType==IMAGE_WEBP)
            {
              prImg->bUseMw=FALSE;
            }
			
            _u4CurState = IMG_DECODE;
            //receive data
            _ImgReceiveData(_hDecDataQueue, sizeof(IMG_PARAM_T), (void *)&rInfo);

            //check parameter
            if (prImgInfo->pfnImgCheckDecParam)
            {
                if (prImgInfo->pfnImgCheckDecParam(_hCurImg, &rInfo)!= (INT32)E_IMG_OK)
                {
                    //notify error
                    _IMG_NotifyStatus(_hCurImg, _u4ImgID, IMG_DRV_HW_ERROR, JPG_HW_NFY_ERROR);
                    _IMG_Release(&_hCurImg);
                    _u4CurState = IMG_RELEASE;
                    break;
                }
            }

            if (prImg->bUseMw) //call IMG_SetUseMw(TRUE) in nav_viewer.c before decode.
            {
                _u4CurState = IMG_BSBUF_UNDERFLOW;
                if (_IMG_BsBufManager(_hCurImg, _u4ImgID)!= (INT32)E_IMG_OK)
                {
                    //notify error
                    _IMG_NotifyStatus(_hCurImg, _u4ImgID, IMG_DRV_HW_ERROR, JPG_HW_NFY_ERROR);
                    _IMG_Release(&_hCurImg);
                    _u4CurState = IMG_RELEASE;
                }
                else
                {
                    //if (prImg->u1MemClass != (UINT8)EXTERNAL_RING_IN_DOUBLE_OUT)
                    {
                        VERIFY (x_sema_lock(_hFillBufSema, X_SEMA_OPTION_WAIT) == OSR_OK);
                    }
                    _u4CurState = IMG_DECODE;
                }
            }
            //--- header parsing and decoding setting prepare
            
            if (prImgInfo->pfnImgDecPrepare)
            {
                i4Ret = prImgInfo->pfnImgDecPrepare(_hCurImg, &rInfo);
            }
            else
            {
                i4Ret = E_IMG_NULL_HANDLER;
            }
            if (i4Ret != (INT32)E_IMG_OK)
            {
                if (i4Ret == (INT32)E_IMG_FMT_UNSUPPORT)
                {
                    //notify error
                    _IMG_NotifyStatus(_hCurImg, _u4ImgID, IMG_DRV_NOT_SUPPORT, JPG_HW_NFY_ERROR);
                }
#ifdef __KERNEL__
                else if (i4Ret == (INT32)E_IMG_ERR_COMPONENT)
                {
                    //notify error 	//code sync
                    _IMG_NotifyStatus(_hCurImg, _u4ImgID, IMG_DRV_ERR_COMPONENT, JPG_HW_NFY_ERROR);
                }
#endif
                else
                {
                    _IMG_NotifyStatus(_hCurImg, _u4ImgID, IMG_DRV_HW_ERROR, JPG_HW_NFY_ERROR);
                }
                _IMG_Release(&_hCurImg);
                _u4CurState = IMG_RELEASE;
            }

            break;
        case IMG_DECODE:

            prImg = (IMG_LIB_INFO_T*)_hCurImg;
            prImgInfo=_IMG_GetInfo();
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

            if (prImgInfo->pfnImgDecOneSegment)
            {
                i4Ret = prImgInfo->pfnImgDecOneSegment(_hCurImg, _u4ImgID, &rInfo);
            }
            else
            {
                i4Ret = E_IMG_NULL_HANDLER;
            }
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
            else if (i4Ret!=(INT32)E_IMG_OK)
            {
                //notify error
                LOG(3,"Decode error type = %d\n",i4Ret);
                if (i4Ret == E_IMG_CANCEL)
                {
                    _IMG_NotifyStatus(_hCurImg, _u4ImgID,  0, JPG_HW_NFY_STOP_DONE);
                }
                else if (i4Ret == E_IMG_HWBUSY)
                {
                    _IMG_NotifyStatus(_hCurImg, _u4ImgID,  IMG_DRV_BUSY, JPG_HW_NFY_ERROR);
                }
                else
                {
                    _IMG_NotifyStatus(_hCurImg, _u4ImgID,  IMG_DRV_HW_ERROR, JPG_HW_NFY_ERROR);
                }
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
            else
            {
                _u4CurState = IMG_DECODE;
            }
            break;
        case IMG_SET:
            //receive data
            _ImgReceiveData(_hSetDataQueue, sizeof(IMG_BUF_PARAM_T), (void *)&rCmd);
            _IMG_Set(rCmd, &rDstRegion);
            _IMG_NotifyStatus(_hCurImg, _u4ImgID, 0, JPG_HW_NFY_FINISHED);
            _u4CurState = IMG_IDLE;
            break;
        case IMG_STOP:
            _IMG_NotifyStatus(_hCurImg, _u4ImgID, 0, _u4NotifyType);
            _u4NotifyType = JPG_HW_NFY_NONE;
            break;
        case IMG_RELEASE:
            _IMG_Release(&_hCurImg);
            VERIFY (x_sema_unlock(_hReleaseSema) == OSR_OK);
            break;
        default:
            break;
        }
    }
}

IMG_INFO_T* _IMG_GetInfo(void)
{
    return (IMG_INFO_T*)&_rImgInfo;
}

INT32 IMG_Config(ENUM_IMG_FORMAT img_type)
{
    IMG_INFO_T *prImgInfo = _IMG_GetInfo();
	
    switch (img_type)
    {
#ifndef CC_IMAGE_PNG_DISABLE
    case IMAGE_PNG:
        prImgInfo->pfnImgInit = _PNG_Init;
        prImgInfo->pfnImgDecPrepare = _PNG_DecPrepare;
        prImgInfo->pfnImgDecOneSegment = _PNG_DecOneSegment;
        prImgInfo->pfnImgBsBufManager = _IMG_BsBufManager;
        prImgInfo->pfnImgSetFillBsBufEnd = _PNG_SetFillBsBufEnd;
        prImgInfo->pfnImgCheckDecParam = NULL;
        prImgInfo->pfnImgStop = _PNG_Stop;

        prImgInfo->pfnImgMwFillBuffer = PNG_MwFillBuffer;
        prImgInfo->pfnImgSetFileSize = PNG_SetFileSize;
        prImgInfo->pfnImgFreeInstance = PNG_FreeInstance;
	    prImgInfo->pfnImgGetInfo=NULL;
        break;
#endif

#ifndef CC_IMAGE_WEBP_DISABLE
    case IMAGE_WEBP:
        prImgInfo->pfnImgInit = _WEBP_Init;
        prImgInfo->pfnImgDecPrepare = _WEBP_DecPrepare;
        prImgInfo->pfnImgDecOneSegment = _WEBP_DecOneSegment;
        prImgInfo->pfnImgBsBufManager = _WEBP_BsBufManager;
        prImgInfo->pfnImgSetFillBsBufEnd = _WEBP_SetFillBsBufEnd;
        prImgInfo->pfnImgCheckDecParam = _WEBP_CheckDecParam;
        prImgInfo->pfnImgStop = _WEBP_Stop;
        prImgInfo->pfnImgMwFillBuffer = _WEBP_MwFillBuffer;
        prImgInfo->pfnImgSetFileSize = _WEBP_SetFileSize;
        prImgInfo->pfnImgFreeInstance = _WEBP_FreeInstance;
		prImgInfo->pfnImgGetInfo=_WEBP_GetDecInfo;
        break;
#endif
    case IMAGE_JPEG:
        prImgInfo->pfnImgInit = _IMG_Init;
        prImgInfo->pfnImgDecPrepare = _IMG_DecPrepare;
        prImgInfo->pfnImgDecOneSegment = _IMG_DecOneSegment;
        prImgInfo->pfnImgBsBufManager = _IMG_BsBufManager;
        prImgInfo->pfnImgSetFillBsBufEnd = _IMG_SetFillBsBufEnd;
        prImgInfo->pfnImgCheckDecParam = _IMG_CheckDecParam;
        prImgInfo->pfnImgStop = _IMG_Stop;
		prImgInfo->pfnImgGetInfo=_IMG_GetDecInfo;
        prImgInfo->pfnImgMwFillBuffer = JDEC_MwFillBuffer;
        prImgInfo->pfnImgSetFileSize = JDEC_SetFileSize;
        prImgInfo->pfnImgFreeInstance = JDEC_FreeInstance;
        break;
    default:
		prImgInfo->eImgType=IMAGE_MAX;
        return -1;
    }
	
	prImgInfo->eImgType=img_type;
    return 0;
}

HANDLE_T IMG_GetPngSema(void)
{
#ifndef CC_IMAGE_PNG_DISABLE
    return (HANDLE_T)_hPngSema;
#else
    return (HANDLE_T)NULL;
#endif
}

ENUM_IMG_FORMAT IMG_GetImageType(void)
{
    IMG_INFO_T *prImgInfo = _IMG_GetInfo();
    return prImgInfo->eImgType;
}


void _ImgHWPowerTurnOff(void)
{
    VLDJPG_SetPowerOff();
}
#ifdef __KERNEL__
static UINT32 _u4DFBPhyAddr = 0;
static UINT32 _u4DFBVirAddr = 0;
static UINT32 _u4DFBSize = 0;
#endif
void IMG_SeDirectFBMem(UINT32 u4PhyAddr, UINT32 u4VirAddr,UINT32 u4Size)
{
#ifdef __KERNEL__
      _u4DFBPhyAddr = u4PhyAddr;
      _u4DFBVirAddr = u4VirAddr;
      _u4DFBSize = u4Size;
#endif
      return;
}

UINT32 VIRTUAL_DFB(UINT32 addr)
{
#ifdef __KERNEL__
    UINT32 u4FbmAddr,u4FbmSize;
    UINT32 u4Ret;
    u4FbmAddr = DRVCUST_OptGet(eFbmMemAddr);
    u4FbmSize = DRVCUST_OptGet(eFbmMemSize);
    if((addr >= u4FbmAddr) && (addr <u4FbmAddr+u4FbmSize))
    {
        u4Ret = VIRTUAL(addr);
    }
    else if((addr >= _u4DFBPhyAddr) && (addr < _u4DFBPhyAddr+_u4DFBSize))
    {
        u4Ret = (addr - _u4DFBPhyAddr + _u4DFBVirAddr);
    }
    else
    {
        LOG(0,"The dst addr not a DFB FBM addr!!!\n");
        u4Ret =  0;
    }
    return u4Ret;
#else
     return addr;
#endif
}

