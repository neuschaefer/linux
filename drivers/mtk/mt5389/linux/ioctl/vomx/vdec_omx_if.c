/********************************************************************************************
 *     LEGAL DISCLAIMER
 *
 *     (Header of MediaTek Software/Firmware Release or Documentation)
 *
 *     BY OPENING OR USING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 *     THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE") RECEIVED
 *     FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON AN "AS-IS" BASIS
 *     ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, EXPRESS OR IMPLIED,
 *     INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR
 *     A PARTICULAR PURPOSE OR NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY
 *     WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY,
 *     INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK
 *     ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
 *     NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S SPECIFICATION
 *     OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
 *
 *     BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE LIABILITY WITH
 *     RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION,
 *     TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE
 *     FEES OR SERVICE CHARGE PAID BY BUYER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 *     THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE WITH THE LAWS
 *     OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF LAWS PRINCIPLES.
 ************************************************************************************************/

#if defined(LINUX_TURNKEY_SOLUTION) && defined(CC_ENABLE_VOMX)

#include "vomx_if.h"
#include "vdec_omx_debug.h"
#include "vdec_drvif.h"
#include "vdp_if.h"

#include "x_typedef.h"
#include "x_os.h"
#include "x_printf.h"
#include "x_assert.h"
#include <asm/uaccess.h>
#include <linux/sched.h>

//fix89 #include "drv_vdec.h"
#include "vdec_mod.h"
#include "x_rm_dev_types.h"

#include "vdec_drvif.h"
//fix89 #include "drv_comp_id.h"
#include <OMX_CoreExt.h>

// OpenMAX Video Component interfaces

VDO_OMX_HANDLE_T _prVDOOmxHandle[VDO_COMP_MAX]; // currently only support single instance

#define FULL_IMAGE 0
#define THUMBNAIL_IMAGE 1
#define OUTPUT_RES_X 1920
#define OUTPUT_RES_Y 1080
#define RESTART_AFTER_FLUSH

static BOOL _bIsThumbnailImg = FALSE;
static VDEC_IMG_FRMDONE_DATA _arImgCBData[2];//= {0, 0};    // 0 for full image, 1 for thumbnail.
static VDP_REGION_T _rSrcRegion = {0};
static UINT32 _u4CoordinateScale = 0;   // 0: 1x, 1: 2x, 2: 4x
static UINT32 _u4XCoordinate = OUTPUT_RES_X;
static UINT32 _u4YCoordinate = OUTPUT_RES_Y;

static void vVDOOmxCompDoFreeHandle(VDO_OMX_HANDLE_T *pHandle);

INT32 i4VDOOmxCompInit(void)
{
    UINT32 u4Index;
    VDEC_IMG_FRMDONE_DATA *prImgCbData;
    for (u4Index = 0; u4Index < VDO_COMP_MAX; u4Index ++)
    	{
    	_prVDOOmxHandle[u4Index].fgUsed = FALSE;
    	_prVDOOmxHandle[u4Index].eColorFormatIndex0 = OMX_COLOR_Format32bitARGB8888;
    	_prVDOOmxHandle[u4Index].eState = OMX_StateMax;
    	_prVDOOmxHandle[u4Index].eStateTo = OMX_StateMax;
    	prImgCbData = _arImgCBData;
    	prImgCbData += u4Index;
    	x_memset(prImgCbData, 0, sizeof(VDEC_IMG_FRMDONE_DATA));
    	}
    return 0;
}

INT32 i4VDOOmxCompUninit(void)
{
    UINT32 u4Index;
    for (u4Index = 0; u4Index < VDO_COMP_MAX; u4Index ++)
    	{
      	  if (_prVDOOmxHandle[u4Index].fgUsed)
      	  {
      	    vVDOOmxCompDoFreeHandle(&_prVDOOmxHandle[u4Index]);
      	  }
    	}
    return 0;
}

static inline BOOL bVDOOmxAddrUsr2Phys(void __user *x, unsigned long *y)
{
    register unsigned long i, j;
    asm volatile (
    "mrs	%1, cpsr\n"
    "	cpsid	if\n"
    "	mcr	p15, 0, %2, c7, c8, 2\n"
    "	mrc	p15, 0, %0, c7, c4, 0\n"
    "	msr	cpsr_c, %1"
    : "=r" (i), "=r" (j) : "r" (x) : "cc");
    if (i & 1)
        return false;
    *y = (i & PAGE_MASK) | ((unsigned long)(x) & ~PAGE_MASK);
    return true;
}

void vVDOOmxEmptyBufDone(VDO_OMX_HANDLE_T *pHandle)
{
    //UINT32 u4Index;
    //BOOL fgAllEmptied = TRUE;
    VDO_OMX_BUFFER_T *pToEmptyBuf = NULL;
    if (pHandle->pEmptyQ[pHandle->u4EmptyReadPos] == NULL)
	{
    	VOMX_Printf("[VDO][vVDOOmxEmptyBufDone] NULL Callback\n");
    	return;
	}
    pToEmptyBuf = pHandle->pEmptyQ[pHandle->u4EmptyReadPos];

    // if empty is done, align queue to left and check if there is pending empty event
    pHandle->pEmptyQ[pHandle->u4EmptyReadPos] = NULL;
    pHandle->u4EmptyReadPos++;
    if(pHandle->u4EmptyReadPos >= VDO_INPUT_BUF_MAX)
    {
        pHandle->u4EmptyReadPos = 0;
    }
    if (!pHandle->pEmptyQ[pHandle->u4EmptyReadPos])
    {
        // empty queue is empty
        pHandle->fgEmptying = FALSE;
    }

    pToEmptyBuf->rHeader.nFlags = 0;
    pToEmptyBuf->rHeader.nTimeStamp = 0;
    // if empty is done, callback now
    i4VDOOmxCompNotifyEmptyDone(pHandle, pToEmptyBuf);
}


void vVDOOmxFillBufDone(VDO_OMX_HANDLE_T *pHandle)
{
    BOOL fgAtEosFrame = FALSE;
    VDO_OMX_BUFFER_T *pToFillBuf = NULL;
    if (pHandle->pFillQ[pHandle->u4FillReadPos] == NULL)
	{
    	VOMX_Printf("[VDO][vVDOOmxFillBufDone] NULL Callback\n");
    	return;
	}
    pToFillBuf = pHandle->pFillQ[pHandle->u4FillReadPos];
    pToFillBuf->rHeader.nTimeStamp = pHandle->u8TimeStamp;
    pToFillBuf->rHeader.nTimeStamp += pHandle->nOutputDelay;

    // if fill is done, align queue to left and check if there is pending fill event
    pHandle->pFillQ[pHandle->u4FillReadPos] = NULL;
    pHandle->u4FillCounter++;
    pHandle->u4FillReadPos++;
    if(pHandle->u4FillReadPos >= VDO_OUTPUT_BUF_MAX)
    {
        pHandle->u4FillReadPos = 0;
    }
    if (!pHandle->pFillQ[pHandle->u4FillReadPos])
    {
        // fill queue is empty
        pHandle->fgFilling = FALSE;
        LOG(5, "%s(%d), u4FillReadPos(%d) nTimeStamp(%lld), fill queue is empty\n",
            __FUNCTION__, __LINE__,
            pHandle->u4FillReadPos, pToFillBuf->rHeader.nTimeStamp);
    }
    else
    {
        LOG(6, "%s(%d), u4FillReadPos(%d) nTimeStamp(%lld)\n",
            __FUNCTION__, __LINE__,
            pHandle->u4FillReadPos, pToFillBuf->rHeader.nTimeStamp);
    }
    fgAtEosFrame = (BOOL)(pToFillBuf->rHeader.nFlags & OMX_BUFFERFLAG_EOS);

    // if fill is done, callback now
    i4VDOOmxCompNotifyFillDone(pHandle, pToFillBuf);
    if (pHandle->fgGotEOS && fgAtEosFrame)// && gAllFilled)
    {
        // if EOS received and no output sample now, callback
        PARAMETER_OMX_CB_EVENTHANDLER rEvent;

        rEvent.eEvent     = OMX_EventBufferFlag;
        rEvent.nData1     = 1; // output port index
        rEvent.nData2     = OMX_BUFFERFLAG_EOS;
        rEvent.pEventData = NULL;

        LOG(3, "%s(%d), fgSentEOS\n", __FUNCTION__, __LINE__);
        if(pHandle->fgSentEOS)
        {
            LOG(3, "%s(%d), already fgSentEOS\n", __FUNCTION__, __LINE__);
        }
        pHandle->fgSentEOS = TRUE;
        //VOMX_Printf("[VDO][OMX] notify OMX_BUFFERFLAG_EOS\n");
        i4VDOOmxCompNotifyEvent(pHandle, &rEvent);
    }
}

void vVDOOmxCompPicInfoChange(VDO_OMX_HANDLE_T *pHandle)
{
    PARAMETER_OMX_CB_EVENTHANDLER rEvent;

        // callback for width and height changed
        rEvent.eEvent     = OMX_EventPortSettingsChanged;
        rEvent.nData1     = 1;// 1 for output port
        rEvent.nData2     = 0;
        rEvent.pEventData = NULL;

        i4VDOOmxCompNotifyEvent(pHandle, &rEvent);
}

void vVDecOmxCbfunc(void *pvCbPrivData, HANDLE_T h_Handle, VCONV_VDEC_OMX_NOTIFY_TYPE_T eNotifyType, VOID* pvNfyParam)
{
    UINT64 u8Tmp;
    UINT64 u8Remainder;
    VCONV_VDEC_OMX_NFY_PARAM* pOMXCbInfo = pvNfyParam;
    //VOMX_Printf("[VDO][OMX] vVDecOmxCbfunc : eNotifyType = 0x%x\n", eNotifyType);
 //           VOMX_Printf("[VDO][OMX] vVDecOmxCbfunc, NoityType = %d, CbInfo : FillSize = %d,\n", eNotifyType, pOMXCbInfo->u4FillSize);
    if(h_Handle >= VDO_COMP_MAX)
    {
 		LOG(1, "%s(%d): vVDecOmxCbfunc error\n", __FUNCTION__, __LINE__);
 		return;
    }
 switch (eNotifyType)
 	{
 	case VCONV_NOTIFY_FILLBUF_DONE:
 	_prVDOOmxHandle[h_Handle].pFillQ[_prVDOOmxHandle[h_Handle].u4FillReadPos]->u4FilledLen = pOMXCbInfo->u4FillSize;
 	_prVDOOmxHandle[h_Handle].fgFilling= FALSE;
    //VOMX_Printf("[VDO][OMX] vVDecOmxCbfunc : pOMXCbInfo->fgEos = 0x%x\n", pOMXCbInfo->fgEos);
 	if (pOMXCbInfo->fgEos)
 		_prVDOOmxHandle[h_Handle].pFillQ[_prVDOOmxHandle[h_Handle].u4FillReadPos]->rHeader.nFlags |= OMX_BUFFERFLAG_EOS;
 	else
 		_prVDOOmxHandle[h_Handle].pFillQ[_prVDOOmxHandle[h_Handle].u4FillReadPos]->rHeader.nFlags &= ~OMX_BUFFERFLAG_EOS;

 	u8Tmp = (UINT64)(pOMXCbInfo->u4PTS & ((UINT32)(0x7FFFFFFF)));
 	_prVDOOmxHandle[h_Handle].u8TimeStamp = (UINT64)u8Div6432((u8Tmp*100), 9, &u8Remainder);

 	if (pOMXCbInfo->u4PTS & ((UINT32)0x80000000))
 	{
 	 	LOG(4, "[VDO][OMX] *** NEGATIVE OUTPUT ***\n");
 	 	_prVDOOmxHandle[h_Handle].u8TimeStamp = _prVDOOmxHandle[h_Handle].u8TimeStampBase - _prVDOOmxHandle[h_Handle].u8TimeStamp;
 	}
 	else
 	{
 	 	_prVDOOmxHandle[h_Handle].u8TimeStamp = _prVDOOmxHandle[h_Handle].u8TimeStampBase + _prVDOOmxHandle[h_Handle].u8TimeStamp;
 	}
 	LOG(4, "[VDO][OMX] OUTPUT u8Pts 0x%llx, u8Base 0x%llx, u4Pts 0x%x\n", _prVDOOmxHandle[h_Handle].u8TimeStamp, _prVDOOmxHandle[h_Handle].u8TimeStampBase, pOMXCbInfo->u4PTS);

 	vVDOOmxFillBufDone(&_prVDOOmxHandle[h_Handle]);
	break;

 	case VCONV_NOTIFY_STOP_DONE:
 		//if (_prVDOOmxHandle[h_Handle].eState == OMX_StateLoaded)
 		//{
 		//_i4VConv_FreeHandle(h_Handle);
 		//}
 		if(_prVDOOmxHandle[h_Handle].eStopStatus != VOMX_STOPPING_VCONV)
	    {
     		LOG(3, "%s(%d): eStopStatus(%d)\n", __FUNCTION__, __LINE__,
     		    (UINT32)_prVDOOmxHandle[h_Handle].eStopStatus);
	    }
 		LOG(5, "%s(%d): VOMX_STOPPING_VCONV -> VOMX_STOPPING_VPUSH\n", __FUNCTION__, __LINE__);
        _prVDOOmxHandle[h_Handle].eStopStatus = VOMX_STOPPING_VPUSH;
        _VPUSH_Stop(_prVDOOmxHandle[h_Handle].pVdecPush);

	break;

 	case VCONV_NOTIFY_PAUSE_DONE:
	break;

	case VCONV_NOTIFY_CHANGE_BUF:
		vVDOOmxCompPicInfoChange(&_prVDOOmxHandle[h_Handle]);
	break;

 	case VCONV_NOTIFY_FLUSH_DONE:
 		if(_prVDOOmxHandle[h_Handle].eFlushStatus != VOMX_FLUSHING_VCONV)
	    {
     		LOG(3, "%s(%d): eFlushStatus(%d)\n", __FUNCTION__, __LINE__,
     		    (UINT32)_prVDOOmxHandle[h_Handle].eFlushStatus);
	    }
 		LOG(5, "%s(%d): VOMX_FLUSHING_VCONV -> VOMX_FLUSHING_VPUSH\n", __FUNCTION__, __LINE__);
        _prVDOOmxHandle[h_Handle].eFlushStatus = VOMX_FLUSHING_VPUSH;
        _VPUSH_Flush(_prVDOOmxHandle[h_Handle].pVdecPush);
	break;

	case VDEC_NOTIFY_PIC_INFO_CHANGE:
		vVDOOmxCompPicInfoChange(&_prVDOOmxHandle[h_Handle]);
	break;

 	case VDEC_NOTIFY_STOP_DONE:
    //VOMX_Printf("[VDO][OMX] VDEC_NOTIFY_STOP_DONE : eState = 0x%x\n", _prVDOOmxHandle[h_Handle].eState);
 		//if (_prVDOOmxHandle[h_Handle].eState == OMX_StateLoaded)
 		//{
        //_VPUSH_ReleaseVideoDecoder(_prVDOOmxHandle[h_Handle].pVdecPush);
 		//}
 		//LOG(5, "%s(%d): VDEC_NOTIFY_STOP_DONE\n", __FUNCTION__, __LINE__);
 		if(_prVDOOmxHandle[h_Handle].eStopStatus != VOMX_STOPPING_VPUSH)
	    {
     		LOG(3, "%s(%d): eStopStatus(%d)\n", __FUNCTION__, __LINE__,
     		    (UINT32)_prVDOOmxHandle[h_Handle].eStopStatus);
	    }
 		LOG(5, "%s(%d): VOMX_STOPPING_VPUSH -> VOMX_STOPPING_NONE\n", __FUNCTION__, __LINE__);
        _prVDOOmxHandle[h_Handle].eStopStatus = VOMX_STOPPING_NONE;

        if (_prVDOOmxHandle[h_Handle].fgStateTransing)
        {
            i4VDOOmxCompCheckStateTransition(&_prVDOOmxHandle[h_Handle]);
        }
 	break;

 	case VDEC_NOTIFY_FLUSH_DONE:
 		if(_prVDOOmxHandle[h_Handle].eFlushStatus != VOMX_FLUSHING_VPUSH)
	    {
     		LOG(3, "%s(%d): eFlushStatus(%d)\n", __FUNCTION__, __LINE__,
     		    (UINT32)_prVDOOmxHandle[h_Handle].eFlushStatus);
	    }
 		LOG(5, "%s(%d): VOMX_FLUSHING_VPUSH -> VOMX_FLUSHING_NONE\n", __FUNCTION__, __LINE__);
        _prVDOOmxHandle[h_Handle].eFlushStatus = VOMX_FLUSHING_NONE;

 		if (_prVDOOmxHandle[h_Handle].fgFlush[0]== TRUE)
 		{
 		    i4VDOOmxCompFlush(&_prVDOOmxHandle[h_Handle], 1, 0);
 		}
 		if (_prVDOOmxHandle[h_Handle].fgFlush[1]== TRUE)
 		{
 		    i4VDOOmxCompFlush(&_prVDOOmxHandle[h_Handle], 0, 1);
 		}
        if (_prVDOOmxHandle[h_Handle].fgStateTransing)
        {
            i4VDOOmxCompCheckStateTransition(&_prVDOOmxHandle[h_Handle]);
        }
    break;

 	case VDEC_NOTIFY_EMPTY_BUFFER_DONE:
 	_prVDOOmxHandle[h_Handle].fgEmptying = FALSE;
 	vVDOOmxEmptyBufDone(&_prVDOOmxHandle[h_Handle]);
	break;

	default:
	break;
 	}
}


BOOL i4VDOOmxCompVPushStopDone(UINT32 u4Tag)
{
    vVDecOmxCbfunc(NULL, (HANDLE_T)u4Tag, VDEC_NOTIFY_STOP_DONE, 0);
    return TRUE;
}

BOOL i4VDOOmxCompVPushFlushDone(UINT32 u4Tag)
{
    vVDecOmxCbfunc(NULL, (HANDLE_T)u4Tag, VDEC_NOTIFY_FLUSH_DONE, 0);
    return TRUE;
}

BOOL i4VDOOmxCompVPushPutDone(UINT32 u4Tag)
{
    vVDecOmxCbfunc(NULL, (HANDLE_T)u4Tag, VDEC_NOTIFY_EMPTY_BUFFER_DONE, 0);
    return TRUE;
}

INT32 i4VDOOmxCompGetHandle(PARAMETER_OMX_GETHANDLE *prUArg, PARAMETER_OMX_CB *prOmxCb)
{
    INT32 i4Ret = 0;
    UINT32 u4Index;
    OMX_ERRORTYPE omxRet = OMX_ErrorNone;
    char cCompName[255];
    PARAMETER_OMX_GETHANDLE OMX_Get_Handle;
    VDO_OMX_HANDLE_T *pHandle=NULL;
    HANDLE_T h_Handle;
    UINT32 u4cbdata;
    ENUM_VDEC_FMT_T eFmt = VDEC_FMT_MAX;

    i4Ret = copy_from_user(&OMX_Get_Handle, prUArg, sizeof(PARAMETER_OMX_GETHANDLE));
    i4Ret = copy_from_user(cCompName, prUArg->cComponentName, 255);

    if (_prVDOOmxHandle[0].fgUsed && _prVDOOmxHandle[1].fgUsed)
    {
        VOMX_Printf("[VDO][OMX] No Resource.\n");
        omxRet = OMX_ErrorInsufficientResources;
        i4Ret = copy_to_user(prUArg, (void *)&OMX_Get_Handle, sizeof(PARAMETER_OMX_GETHANDLE));
        return -1;
    }

    #if DTV_VOMX
    {
        static BOOL fgVConvInit = FALSE;
        if(!fgVConvInit)
        {
            fgVConvInit = TRUE;
            _i4VConv_DrvInit();
        }
    }
    #endif
    _i4VConv_Init();
    i4Ret = _i4VConv_GetHandle(&h_Handle);
    if(i4Ret != 0)
	{
        VOMX_Printf("[VDO][OMX] No Resource0.\n");
        omxRet = OMX_ErrorInsufficientResources;
        i4Ret = copy_to_user(prUArg, (void *)&OMX_Get_Handle, sizeof(PARAMETER_OMX_GETHANDLE));
        return -1;
	}

    // check the component name
    if (0 == strcmp(cCompName, "OMX.MTK.Video.Decoder.H264"))
    {
        VOMX_Printf("[VDO][OMX] H264.\n");
        _prVDOOmxHandle[h_Handle].eCodecType = VC_H264;
        eFmt = VDEC_FMT_H264;
    }
    else if ((0 == strcmp(cCompName, "OMX.MTK.Video.Decoder.MPEG1")) ||
             (0 == strcmp(cCompName, "OMX.MTK.Video.Decoder.MPEG2")))
    {
        VOMX_Printf("[VDO][OMX] MPEG2.\n");
        _prVDOOmxHandle[h_Handle].eCodecType = VC_MPEG2;
        eFmt = VDEC_FMT_MPV;
    }
    else if ((0 == strcmp(cCompName, "OMX.MTK.Video.Decoder.WMV")) ||
             (0 == strcmp(cCompName, "OMX.MTK.Video.Decoder.VC1")))
    {
        VOMX_Printf("[VDO][OMX] VC1.\n");
        _prVDOOmxHandle[h_Handle].eCodecType = VC_VC1;
        eFmt = VDEC_FMT_WMV;
    }
    else if ((0 == strcmp(cCompName, "OMX.MTK.Video.Decoder.MPEG4")) ||
             (0 == strcmp(cCompName, "OMX.MTK.Video.Decoder.H263")) ||
             (0 == strcmp(cCompName, "OMX.MTK.Video.Decoder.SORENSON")))
    {
        VOMX_Printf("[VDO][OMX] MPEG4.\n");
        _prVDOOmxHandle[h_Handle].eCodecType = VC_MPEG4;
        eFmt = VDEC_FMT_MP4;
    }
#ifdef VDEC_IS_POST_MT5368
    else if (0 == strcmp(cCompName, "OMX.MTK.Video.Decoder.VP6"))
    {
        VOMX_Printf("[VDO][OMX] VP6.\n");
        _prVDOOmxHandle[h_Handle].eCodecType = VC_VP6;
        eFmt = VDEC_FMT_VP6;
    }
    else if (0 == strcmp(cCompName, "OMX.MTK.Video.Decoder.VP8"))
    {
        VOMX_Printf("[VDO][OMX] VP8.\n");
        _prVDOOmxHandle[h_Handle].eCodecType = VC_VP8;
        eFmt = VDEC_FMT_VP8;
    }
#endif
    else if (0 == strcmp(cCompName, "OMX.MTK.Video.Decoder.MJPEG"))
    {
        VOMX_Printf("[VDO][OMX] MJPEG.\n");
        _prVDOOmxHandle[h_Handle].eCodecType = VC_MJPG;
        eFmt = VDEC_FMT_MJPEG;
    }
    else if (0 == strcmp(cCompName, "OMX.MTK.Video.Decoder.JPEG"))
    {
        VOMX_Printf("[VDO][OMX] MJPEG.\n");
        _prVDOOmxHandle[h_Handle].eCodecType = VC_JPG;
        eFmt = VDEC_FMT_JPG;
    }
    else if (0 == strcmp(cCompName, "OMX.MTK.Video.Decoder.PNG"))
    {
        VOMX_Printf("[VDO][OMX] VC_PNG.\n");
        _prVDOOmxHandle[h_Handle].eCodecType = VC_PNG;
        eFmt = VDEC_FMT_PNG;
    }     
    else if (0 == strcmp(cCompName, "OMX.MTK.Video.Decoder.RV"))
    {
        VOMX_Printf("[VDO][OMX] RV.\n");
        _prVDOOmxHandle[h_Handle].eCodecType = VC_RV40;
        eFmt = VDEC_FMT_RV;
    }
    else if (0 == strcmp(cCompName, "OMX.MTK.Video.Decoder.AVS"))
    {
        VOMX_Printf("[VDO][OMX] AVS.\n");
        _prVDOOmxHandle[h_Handle].eCodecType = VC_AVS;
        eFmt = VDEC_FMT_AVS;
    }
    else
    {
        VOMX_Printf("[VDO][OMX] component not found - %s\n", cCompName);
        omxRet = OMX_ErrorComponentNotFound;
    }
    pHandle = &_prVDOOmxHandle[h_Handle];

    #if DTV_VOMX
    _VPUSH_DecodeInit();
    _prVDOOmxHandle[h_Handle].pVdecPush = _VPUSH_AllocVideoDecoder(eFmt);
    if(_prVDOOmxHandle[h_Handle].pVdecPush == NULL)
    	{
        VOMX_Printf("[VDO][OMX] No Resource1.\n");
        return -1;
    	}
    #else
    _i4Vdec_OMX_Init();
    #endif
    _i4VConv_SetNotifyFct(h_Handle,(vOmxVdecVConvCbFunc)vVDecOmxCbfunc, &u4cbdata);
    #if DTV_VOMX
    {
        VDEC_PUSH_CB_T rPushCb;
        x_memset(&rPushCb, 0, sizeof(VDEC_PUSH_CB_T));
        rPushCb.u4Mask |= VDEC_PUSH_CB_STOP_DONE;
        rPushCb.u4Mask |= VDEC_PUSH_CB_FLUSH_DONE;
        rPushCb.u4Mask |= VDEC_PUSH_CB_PUT_DATA_DONE;
        rPushCb.u4Mask |= VDEC_PUSH_CB_IMG_FRAME_DONE;
        rPushCb.u4StopTag = h_Handle;
        rPushCb.u4PutTag = h_Handle;
        rPushCb.u4ImageTag = (UINT32)pHandle;
        rPushCb.pfnVdecPutDataDone = i4VDOOmxCompVPushPutDone;
        rPushCb.pfnVdecStopDone = i4VDOOmxCompVPushStopDone;
        rPushCb.pfnVdecFlushDone = i4VDOOmxCompVPushFlushDone;
        rPushCb.pfnVdecImgFrmDone = i4VDOOmxCompImgFrmDone;
        if(!_VPUSH_RegCb(_prVDOOmxHandle[h_Handle].pVdecPush, &rPushCb))
        {
            VOMX_Printf("_VPUSH_RegCb NG\n");
            ASSERT(0);
        }
        _i4VConv_SetVPush(h_Handle, _prVDOOmxHandle[h_Handle].pVdecPush);
    }
    #else
    _i4Vdec_SetNotifyFct(h_Handle,(vOmxVdecVConvCbFunc)vVDecOmxCbfunc, &u4cbdata);
    #endif

    if (OMX_ErrorNone == omxRet)
    {
        // initialize
        VOMX_Printf("[VDO][OMX] OMX_ErrorNone.\n");
        _prVDOOmxHandle[h_Handle].h_handle = h_Handle;
        _prVDOOmxHandle[h_Handle].fgUsed           = TRUE;
        _prVDOOmxHandle[h_Handle].eState           = OMX_StateLoaded;

        _prVDOOmxHandle[h_Handle].pid              = current_thread_info()->task->pid;
        _prVDOOmxHandle[h_Handle].fgEmptying       = FALSE;
        _prVDOOmxHandle[h_Handle].fgFilling        = FALSE;
        _prVDOOmxHandle[h_Handle].fgGotEOS         = FALSE;
        _prVDOOmxHandle[h_Handle].fgSentEOS        = FALSE;
        _prVDOOmxHandle[h_Handle].pAppData         = OMX_Get_Handle.pAppData;
        _prVDOOmxHandle[h_Handle].pEventHandler    = prOmxCb->pEventHandler;
        _prVDOOmxHandle[h_Handle].pEmptyBufferDone = prOmxCb->pEmptyBufferDone;
        _prVDOOmxHandle[h_Handle].pFillBufferDone  = prOmxCb->pFillBufferDone;
        _prVDOOmxHandle[h_Handle].pFifoPntr        = NULL;
        _prVDOOmxHandle[h_Handle].u4FifoSize       = 0;
        _prVDOOmxHandle[h_Handle].pFifoWptr        = NULL;
        _prVDOOmxHandle[h_Handle].fgFlush[0]        = FALSE;
        _prVDOOmxHandle[h_Handle].fgFlush[1]        = FALSE;
        _prVDOOmxHandle[h_Handle].u8TimeStampBase	= VDO_OMX_INV_PTS;
        // not reset it.
        //_prVDOOmxHandle[h_Handle].nOutputDelay      = 0;

        _prVDOOmxHandle[h_Handle].eStopStatus       = VOMX_STOPPING_NONE;
        _prVDOOmxHandle[h_Handle].eFlushStatus      = VOMX_FLUSHING_NONE;

        _prVDOOmxHandle[h_Handle].u4EmptySize       = 0;
        _prVDOOmxHandle[h_Handle].u4EmptyCounter    = 0;
        _prVDOOmxHandle[h_Handle].u4EmptyWritePos   = 0;
        _prVDOOmxHandle[h_Handle].u4EmptyReadPos    = 0;
        for (u4Index = 0; u4Index < VDO_INPUT_BUF_MAX; u4Index ++)
        {
            _prVDOOmxHandle[h_Handle].pEmptyQ[u4Index] = NULL;
        }

        _prVDOOmxHandle[h_Handle].u4FillCounter    = 0;
        _prVDOOmxHandle[h_Handle].u4FillWritePos   = 0;
        _prVDOOmxHandle[h_Handle].u4FillReadPos    = 0;
        for (u4Index = 0; u4Index < VDO_OUTPUT_BUF_MAX; u4Index ++)
        {
            _prVDOOmxHandle[h_Handle].pFillQ[u4Index] = NULL;
        }
        // create task & command queue
        i4VDOOmxTaskCreate(&_prVDOOmxHandle[h_Handle]);

    }
    OMX_Get_Handle.pHandle = pHandle;
    OMX_Get_Handle.eReturnValue = omxRet;
     i4Ret = copy_to_user(prUArg, (void *)&OMX_Get_Handle, sizeof(PARAMETER_OMX_GETHANDLE));

    return 0;
}

static void vVDOOmxCompDoFreeHandle(VDO_OMX_HANDLE_T *pHandle)
{
    if (pHandle->fgUsed)
    {
        // TODO: for error handling, stop decoder in case not stopped

        // destroy task
        i4VDOOmxTaskDestroy(pHandle);

        pHandle->fgUsed = FALSE;
    }
}


INT32 i4VDOOmxCompFreeHandle(PARAMETER_OMX_FREEHANDLE *prUArg)
{
    INT32 i4Ret = 0;
    OMX_ERRORTYPE omxRet = OMX_ErrorNone;
    PARAMETER_OMX_FREEHANDLE OMX_Free_Handle;
    VDO_OMX_HANDLE_T *Omx_Handle;

    i4Ret = copy_from_user(&OMX_Free_Handle, prUArg, sizeof(PARAMETER_OMX_FREEHANDLE));

    Omx_Handle = OMX_Free_Handle.pHandle;
    if ((Omx_Handle == NULL) || (i4Ret != 0))
	{
        VOMX_Printf("%s(L:%d) Incorrect Handle.\n", __FUNCTION__, __LINE__);
        OMX_Free_Handle.eReturnValue = OMX_ErrorUndefined;
        i4Ret = copy_to_user(prUArg, (void *)&OMX_Free_Handle, sizeof(PARAMETER_OMX_FREEHANDLE));
        return i4Ret;
	}

    	 //_VPUSH_Stop(Omx_Handle->pVdecPush);
    	 _VPUSH_ReleaseVideoDecoder(_prVDOOmxHandle[Omx_Handle->h_handle].pVdecPush);
    	 //_i4VConv_Stop(Omx_Handle->h_handle);
    	 _i4VConv_FreeHandle(Omx_Handle->h_handle);


    if (Omx_Handle->fgUsed && (OMX_Free_Handle.pHandle == &_prVDOOmxHandle[Omx_Handle->h_handle]))
    {
        omxRet = OMX_ErrorNone;
        vVDOOmxCompDoFreeHandle(OMX_Free_Handle.pHandle);
    }
    else
    {
        omxRet = OMX_ErrorUndefined;
    }

    OMX_Free_Handle.eReturnValue = omxRet;
     i4Ret = copy_to_user(prUArg, (void *)&OMX_Free_Handle, sizeof(PARAMETER_OMX_FREEHANDLE));

    return i4Ret;
}


INT32 i4VDOOmxCompGetState(PARAMETER_OMX_GETSTATE *prUArg)
{
    INT32 i4Ret = 0;
    OMX_ERRORTYPE omxRet = OMX_ErrorNone;
    PARAMETER_OMX_GETSTATE OMX_GetState_Param;
    VDO_OMX_HANDLE_T *Omx_Handle;

    i4Ret = copy_from_user(&OMX_GetState_Param, prUArg, sizeof(PARAMETER_OMX_GETSTATE));
    Omx_Handle = OMX_GetState_Param.pHandle;
        //VOMX_Printf("[VDO][OMX_GetState] handle = %d\n", Omx_Handle->h_handle);
        
    if ((Omx_Handle == NULL) || (i4Ret != 0))
    {
        VOMX_Printf("%s(L:%d) Incorrect Handle.\n", __FUNCTION__, __LINE__);
        OMX_GetState_Param.eReturnValue = OMX_ErrorUndefined;
        i4Ret = copy_to_user(prUArg, (void *)&OMX_GetState_Param, sizeof(PARAMETER_OMX_GETSTATE));
        return i4Ret;
    }

    if (_prVDOOmxHandle[Omx_Handle->h_handle].fgUsed && (OMX_GetState_Param.pHandle == &_prVDOOmxHandle[Omx_Handle->h_handle]))
    {
        omxRet = OMX_ErrorNone;
        i4Ret = copy_to_user(OMX_GetState_Param.pState, (void *)&(_prVDOOmxHandle[Omx_Handle->h_handle].eState), sizeof(OMX_STATETYPE));
    }
    else
    {
        omxRet = OMX_ErrorUndefined;
    }

    OMX_GetState_Param.eReturnValue = omxRet;
     i4Ret = copy_to_user(prUArg, (void *)&OMX_GetState_Param, sizeof(PARAMETER_OMX_GETSTATE));

    return i4Ret;
}



static OMX_ERRORTYPE i4VDOOmxCompSetStateFromLoaded(VDO_OMX_HANDLE_T *pHandle, OMX_STATETYPE omxToState)
{
    OMX_ERRORTYPE omxRet = OMX_ErrorUndefined;

    if (OMX_StateIdle == omxToState)
    {
        // allowed
        omxRet = OMX_ErrorNone;

        #if DTV_VOMX
        pHandle->eStateTo = OMX_StateIdle;
        pHandle->fgStateTransing = TRUE;
        #else
        pHandle->eState = OMX_StateIdle;
        #endif

        VOMX_Printf("[VDO][OMX] Component state: Loaded -> Idle.\n");

    }
    else
    {
        VOMX_Printf("[VDO][OMX] Component state: Loaded -> unknown(%d).\n", omxToState);
    }

    return omxRet;
}


static OMX_ERRORTYPE i4VDOOmxCompSetStateFromIdle(VDO_OMX_HANDLE_T *pHandle, OMX_STATETYPE omxToState)
{
    OMX_ERRORTYPE omxRet = OMX_ErrorUndefined;

    if (OMX_StateExecuting == omxToState)
    {
        // allowed
        omxRet = OMX_ErrorNone;
        pHandle->eState = OMX_StateExecuting;

        VOMX_Printf("[VDO][OMX] Component state: Idle -> Executing.\n");

        // start playback (before sending play command, VFIFO SA/EA must be ready)
        _VPUSH_Play(pHandle->pVdecPush);

    }
    else if (OMX_StateLoaded == omxToState)
    {
        omxRet = OMX_ErrorNone;

        VOMX_Printf("[VDO][OMX] Component state: Idle -> Loaded.\n");
        // 3.1.1.2.2.1 OMX_StateIdle to OMX_StateLoaded
        // On a transition from OMX_StateIdle to OMX_StateLoaded, each buffer supplier shall call
        // OMX_FreeBuffer on the non-supplier port for each buffer residing at the non-supplier port.
        // If the supplier allocated the buffer, it shall free the buffer before calling OMX_FreeBuffer.
        // If the non-supplier port allocated the buffer, it shall free the buffer upon receipt of an
        // OMX_FreeBuffer call. Furthermore, a non-supplier port shall always free the buffer header
        // upon receipt of an OMX_FreeBuffer call. When all of the buffers have been removed from the
        // component, the state transition is complete; the component communicates that the initiating
        // OMX_SendCommand call has completed via a callback event.
        pHandle->eStateTo = OMX_StateLoaded;
        pHandle->fgStateTransing = TRUE;
    }
    else
    {
        VOMX_Printf("[VDO][OMX] Component state: Idle -> unknown(%d).\n", omxToState);
    }

    return omxRet;
}


static OMX_ERRORTYPE i4VDOOmxCompSetStateFromExecuting(VDO_OMX_HANDLE_T *pHandle, OMX_STATETYPE omxToState)
{
    OMX_ERRORTYPE omxRet = OMX_ErrorUndefined;

    if (OMX_StateIdle == omxToState)
    {
        // allowed
        VOMX_Printf("[VDO][OMX] Component state: Executing -> Idle.\n");

        // stop playback
        // stop sequence is:
        // VConv_Stop -> VConv main loop -> VConv stop done ->
        // VPUSH_Stop -> VDEC_Stop -> VDEC main loop -> Vdec stop done ->
        // _VPUSH_SendCmd(STOP) -> VPUSH main loop -> VPush stop done
        _prVDOOmxHandle[pHandle->h_handle].eStopStatus = VOMX_STOPPING_VCONV;
        _i4VConv_Stop(pHandle->h_handle);

        // TODO:
        // If the IL client requests a state transition from OMX_StateExecuting to OMX_StateIdle,
        // the component shall return all buffers to their respective suppliers and receive all buffers
        // belonging to its supplier ports before completing the transition. Any port communicating
        // with the IL client shall return any buffers it is holding via EmptyBufferDone and FillBufferDone
        // callbacks, which are used by input and output ports, respectively.

        omxRet = OMX_ErrorNone;
        pHandle->eStateTo = OMX_StateIdle;
        pHandle->fgStateTransing = TRUE;
        //pHandle->eState = OMX_StateIdle;
    }
    else if (OMX_StatePause == omxToState)
    {
        // allowed
        VOMX_Printf("[VDO][OMX] Component state: Executing -> Pause.\n");

        // Pause
          _i4VConv_Pause(_prVDOOmxHandle[pHandle->h_handle].h_handle);


        omxRet = OMX_ErrorNone;
        pHandle->eState = OMX_StatePause;
    }
    else
    {
        VOMX_Printf("[VDO][OMX] Component state: Executing -> unknown(%d).\n", omxToState);
    }
    return omxRet;
}


static OMX_ERRORTYPE i4VDOOmxCompSetStateFromPause(VDO_OMX_HANDLE_T *pHandle, OMX_STATETYPE omxToState)
{
    OMX_ERRORTYPE omxRet = OMX_ErrorUndefined;

    if (OMX_StateIdle == omxToState)
    {
        // allowed
        VOMX_Printf("[VDO][OMX] Component state: Pause -> Idle.\n");

        // stop playback
        _prVDOOmxHandle[pHandle->h_handle].eStopStatus = VOMX_STOPPING_VCONV;
        _i4VConv_Stop(pHandle->h_handle);

        // TODO: ?
        // If the IL client requests a state transition from OMX_StateExecuting to OMX_StateIdle,
        // the component shall return all buffers to their respective suppliers and receive all buffers
        // belonging to its supplier ports before completing the transition. Any port communicating
        // with the IL client shall return any buffers it is holding via EmptyBufferDone and FillBufferDone
        // callbacks, which are used by input and output ports, respectively.

        omxRet = OMX_ErrorNone;        
        pHandle->eStateTo = OMX_StateIdle;
        pHandle->fgStateTransing = TRUE;
        //pHandle->eState = OMX_StateIdle;
    }
    else if (OMX_StateExecuting== omxToState)
    {
        // allowed
        VOMX_Printf("[VDO][OMX] Component state: Pause -> Executing.\n");

        // Pause
        _i4VConv_Resume(_prVDOOmxHandle[pHandle->h_handle].h_handle);

        omxRet = OMX_ErrorNone;
        pHandle->eState = OMX_StateExecuting;
    }
    else
    {
        VOMX_Printf("[VDO][OMX] Component state: Pause -> unknown(%d).\n", omxToState);
    }
    return omxRet;
}


INT32 i4VDOOmxCompSetState(VDO_OMX_HANDLE_T *pHandle, OMX_STATETYPE omxStateTo)
{
    INT32 i4Ret = 0;
    OMX_ERRORTYPE omxRet = OMX_ErrorNone;
    OMX_STATETYPE omxStateFrom;
    PARAMETER_OMX_CB_EVENTHANDLER rEvent;

    omxStateFrom = pHandle->eState;

    if (omxStateTo == omxStateFrom)
    {
        omxRet = OMX_ErrorSameState;
        VOMX_Printf("[VDO][OMX] Component state: from(%d) -> to(%d) NG.\n", omxStateFrom, omxStateTo);
    }
    else
    {
        switch (omxStateFrom)
        {
        case OMX_StateLoaded:
            omxRet = i4VDOOmxCompSetStateFromLoaded(pHandle, omxStateTo);
            break;
        case OMX_StateIdle:
            omxRet = i4VDOOmxCompSetStateFromIdle(pHandle, omxStateTo);
            break;
        case OMX_StateExecuting:
            omxRet = i4VDOOmxCompSetStateFromExecuting(pHandle, omxStateTo);
            break;
        case OMX_StatePause:
            omxRet = i4VDOOmxCompSetStateFromPause(pHandle, omxStateTo);
            break;
        default:
            VOMX_Printf("[VDO][OMX] Component state: from(%d) -> to(%d) NG.\n", omxStateFrom, omxStateTo);
            omxRet = OMX_ErrorUndefined;
            break;
        }
    }


    if (pHandle->fgStateTransing)
    {
        // do not callback now, need wait for transiting finished
        return i4Ret;
    }

    // callback for state changed
    if (OMX_ErrorNone == omxRet)
    {
        rEvent.eEvent     = OMX_EventCmdComplete;
        rEvent.nData1     = OMX_CommandStateSet;
        rEvent.nData2     = pHandle->eState;
        rEvent.pEventData = NULL;
    }
    else if (OMX_ErrorSameState == omxRet)
    {
        rEvent.eEvent     = OMX_ErrorSameState;
        rEvent.nData1     = OMX_CommandStateSet;
        rEvent.nData2     = pHandle->eState;
        rEvent.pEventData = NULL;
    }
    else
    {
        rEvent.eEvent     = OMX_EventError;
        rEvent.nData1     = omxRet;
        rEvent.nData2     = 0;
        rEvent.pEventData = NULL;
    }

    i4VDOOmxCompNotifyEvent(pHandle, &rEvent);

    return i4Ret;
}

INT32 i4VDOOmxCompCheckStateTransition(VDO_OMX_HANDLE_T *pHandle)
{
    INT32 i4Ret = 0;
    BOOL fgDone = FALSE;
    OMX_STATETYPE omxStateFrom;
    OMX_STATETYPE omxStateTo;
    PARAMETER_OMX_CB_EVENTHANDLER rEvent;

    omxStateFrom = pHandle->eState;
    omxStateTo   = pHandle->eStateTo;

    if (OMX_StateLoaded == omxStateTo)
    {
        // need check if all buffers are freed
        if (fgVDOOmxAllInputBufferFreed(pHandle) && fgVDOOmxAllOutputBufferFreed(pHandle))
        {
            fgDone = TRUE;
            VOMX_Printf("[VDO][OMX] Component state: asynchronous to Loaded done.\n");
        }
        else
        {
            VOMX_Printf("[VDO][OMX] Component state: buffers are not freed.\n");
        }
    }
    else if (OMX_StateIdle == omxStateTo)
    {
        // need check if buffers are allocated
        if (!fgVDOOmxAllInputBufferFreed(pHandle) && !fgVDOOmxAllOutputBufferFreed(pHandle) &&
            (pHandle->eStopStatus == VOMX_STOPPING_NONE))
        {
            fgDone = TRUE;

            VOMX_Printf("[VDO][OMX] Component state: asynchronous to Idle done.\n");
        }
    }

    if (fgDone)
    {
        pHandle->eState          = pHandle->eStateTo;
        pHandle->eStateTo        = OMX_StateMax;
        pHandle->fgStateTransing = FALSE;

        // callback for state changed
        rEvent.eEvent     = OMX_EventCmdComplete;
        rEvent.nData1     = OMX_CommandStateSet;
        rEvent.nData2     = pHandle->eState;
        rEvent.pEventData = NULL;

        i4VDOOmxCompNotifyEvent(pHandle, &rEvent);
    }

    return i4Ret;
}

INT32 i4VDOOmxCompSetStateAsync(PARAMETER_OMX_COMMAND_STATESET *prUArg)
{
    INT32 i4Ret = 0;
    OMX_ERRORTYPE omxRet = OMX_ErrorNone;
    PARAMETER_OMX_COMMAND_STATESET OMX_Set_State;
    VDO_OMX_MSG_T rMsgSetState;
    VDO_OMX_HANDLE_T *Omx_Handle;

    i4Ret = copy_from_user(&OMX_Set_State, prUArg, sizeof(PARAMETER_OMX_COMMAND_STATESET));
    Omx_Handle = OMX_Set_State.pHandle;
    if ((Omx_Handle == NULL) || (i4Ret != 0))
    {
        VOMX_Printf("%s(L:%d) Incorrect Handle.\n", __FUNCTION__, __LINE__);
        OMX_Set_State.eReturnValue = OMX_ErrorUndefined;
        i4Ret = copy_to_user(prUArg, (void *)&OMX_Set_State, sizeof(PARAMETER_OMX_COMMAND_STATESET));
        return i4Ret;
    }

    //VOMX_Printf("[VDO][i4VDOOmxCompSetStateAsync] pFifoPntr = 0x%x, u4FifoSize = 0x%x.\n", Omx_Handle->pFifoPntr, Omx_Handle->u4FifoSize);
    if (_prVDOOmxHandle[Omx_Handle->h_handle].fgUsed && (OMX_Set_State.pHandle == &_prVDOOmxHandle[Omx_Handle->h_handle]))
    {
        // status is valid, proceed to send command to queue
        rMsgSetState.eCmd     = VDO_OMX_CMD_SET_STATE;
        rMsgSetState.eStateTo = OMX_Set_State.eStateType;

        #if 0
        i4VDOOmxCompSetState(Omx_Handle, rMsgSetState.eStateTo);

        if (Omx_Handle->fgStateTransing)
        {
            i4VDOOmxCompCheckStateTransition(Omx_Handle);
        }
        #else
        i4Ret = i4VDOOmxTaskSendCommand(OMX_Set_State.pHandle, &rMsgSetState);
        #endif
    }
    else
    {
        omxRet = OMX_ErrorUndefined;
    }

    OMX_Set_State.eReturnValue = omxRet;
    i4Ret = copy_to_user(prUArg, (void *)&OMX_Set_State, sizeof(PARAMETER_OMX_COMMAND_STATESET));

    return i4Ret;
}


INT32 i4VDOOmxCompAllocateBuffer(PARAMETER_OMX_ALLOCATEBUFFER *prUArg)
{
    UINT32 u4PhysAddr = 0;
    INT32 i4Ret = 0;
    OMX_ERRORTYPE omxRet = OMX_ErrorUndefined;
    PARAMETER_OMX_ALLOCATEBUFFER OMX_AllocBuf_Param;
    VDO_OMX_BUFFER_T *pBuffer = NULL; // internal buffer handle
    VDO_OMX_MSG_T rMsgAllocated;
    VDO_OMX_HANDLE_T *Omx_Handle;

    i4Ret = copy_from_user(&OMX_AllocBuf_Param, prUArg, sizeof(PARAMETER_OMX_ALLOCATEBUFFER));
    Omx_Handle = OMX_AllocBuf_Param.pHandle;
    if ((Omx_Handle == NULL) || (i4Ret != 0))
    {
        VOMX_Printf("%s(L:%d) Incorrect Handle.\n", __FUNCTION__, __LINE__);
        OMX_AllocBuf_Param.eReturnValue = OMX_ErrorUndefined;
        i4Ret = copy_to_user(prUArg, (void *)&OMX_AllocBuf_Param, sizeof(PARAMETER_OMX_ALLOCATEBUFFER));
        return i4Ret;
    }

        //VOMX_Printf("[VDO][OMX_AllocateBuffer] handle = 0x%x\n", Omx_Handle);

    if (_prVDOOmxHandle[Omx_Handle->h_handle].fgUsed && (OMX_AllocBuf_Param.pHandle == &_prVDOOmxHandle[Omx_Handle->h_handle]))
    {
        if(OMX_AllocBuf_Param.fgUseBuf)
        {
        #if 0
            if(!bVDOOmxAddrUsr2Phys(OMX_AllocBuf_Param.pUseBuffer, &u4PhysAddr))
            {
                // it's a user space address
                // alloc a physical one, and copy into the memory at user
                // space.
                OMX_AllocBuf_Param.fgUseBuf = FALSE;
            }
        #endif
        }
        if (0 == OMX_AllocBuf_Param.nPortIndex)
        {
            // input port
            i4VDOOmxAllocInputBuf(Omx_Handle, OMX_AllocBuf_Param.nSizeBytes, OMX_AllocBuf_Param.nPortIndex, &pBuffer);
        }
        else if ((1 == OMX_AllocBuf_Param.nPortIndex) || (2 == OMX_AllocBuf_Param.nPortIndex))
        {
            // output port
            i4VDOOmxAllocOutputBuf(Omx_Handle, OMX_AllocBuf_Param.fgUseBuf, u4PhysAddr,
                OMX_AllocBuf_Param.nSizeBytes, OMX_AllocBuf_Param.nPortIndex, &pBuffer);
        }
            //VOMX_Printf("[VDO][OMX] To allocate input buffer, pBuffer = 0x%x, size = 0x%x bytes\n", pBuffer, OMX_AllocBuf_Param.nSizeBytes);

        if (NULL != pBuffer)
        {
            OMX_AllocBuf_Param.hPBufHeaderBuf = pBuffer->hBuffer;
            OMX_AllocBuf_Param.hPBufHeader = pBuffer->hHeader;

            pBuffer->rHeader.pAppPrivate = OMX_AllocBuf_Param.pAppPrivate;

            #if 0
            if (Omx_Handle->fgStateTransing)
            {
                i4VDOOmxCompCheckStateTransition(Omx_Handle);
            }
            #else
            rMsgAllocated.eCmd = VDO_OMX_CMD_BUFFER_ALLOCATED;
            i4VDOOmxTaskSendCommand(OMX_AllocBuf_Param.pHandle, &rMsgAllocated);
            #endif
            //VOMX_Printf("[VDO][OMX] Buffer allocated: pBuffer = 0x%X, pBuffer->pOwner = 0x%X.\n", pBuffer, pBuffer->pOwner);

            omxRet = OMX_ErrorNone;
        }


    }

    OMX_AllocBuf_Param.eReturnValue = omxRet;
    i4Ret = copy_to_user(prUArg, (void *)&OMX_AllocBuf_Param, sizeof(PARAMETER_OMX_ALLOCATEBUFFER));

    return i4Ret;
}


INT32 i4VDOOmxCompFreeBuffer(PARAMETER_OMX_FREEBUFFER *prUArg)
{
    INT32 i4Ret = VDecOMX_ERR;
    OMX_ERRORTYPE omxRet = OMX_ErrorUndefined;
    PARAMETER_OMX_FREEBUFFER OMX_FreeBuf_Param;
    VDO_OMX_MSG_T rMsgFreed;
    VDO_OMX_HANDLE_T *Omx_Handle;

    i4Ret = copy_from_user(&OMX_FreeBuf_Param, prUArg, sizeof(PARAMETER_OMX_FREEBUFFER));
    Omx_Handle = OMX_FreeBuf_Param.pHandle;
    //VOMX_Printf("[VDO][OMX_FreeBuffer] handle = %d\n", Omx_Handle->h_handle);
    if ((Omx_Handle == NULL) || (i4Ret != 0))
    {
        VOMX_Printf("%s(L:%d) Incorrect Handle.\n", __FUNCTION__, __LINE__);
        OMX_FreeBuf_Param.eReturnValue = OMX_ErrorUndefined;
        i4Ret = copy_to_user(prUArg, (void *)&OMX_FreeBuf_Param, sizeof(PARAMETER_OMX_FREEBUFFER));
        return i4Ret;
    }

    if (_prVDOOmxHandle[Omx_Handle->h_handle].fgUsed && (OMX_FreeBuf_Param.pHandle == &_prVDOOmxHandle[Omx_Handle->h_handle]))
    {
        if (0 == OMX_FreeBuf_Param.nPortIndex)
        {
            // input port
            i4Ret = i4VDOOmxFreeInputBuf(OMX_FreeBuf_Param.pHandle, OMX_FreeBuf_Param.hPBufHeader);
        }
        else if (1 == OMX_FreeBuf_Param.nPortIndex)
        {
            // output port
            i4Ret = i4VDOOmxFreeOutputBuf(OMX_FreeBuf_Param.pHandle, OMX_FreeBuf_Param.hPBufHeader);
        }
        if (VDecOMX_OK == i4Ret)
        {
            omxRet = OMX_ErrorNone;
            #if 0
            if (Omx_Handle->fgStateTransing)
            {
                i4VDOOmxCompCheckStateTransition(Omx_Handle);
            }
            #else
            rMsgFreed.eCmd = VDO_OMX_CMD_BUFFER_FREED;
            i4VDOOmxTaskSendCommand(OMX_FreeBuf_Param.pHandle, &rMsgFreed);
            #endif
        }
    }

    OMX_FreeBuf_Param.eReturnValue = omxRet;
    i4Ret = copy_to_user(prUArg, (void *)&OMX_FreeBuf_Param, sizeof(PARAMETER_OMX_FREEBUFFER));

    return i4Ret;
}


// The OMX_EmptyThisBuffer macro will send a filled buffer to an input port of a component.
// When the buffer contains data, the value of the nFilledLen field of the buffer header will not be zero.
// If the buffer contains no data, the value of nFilledLen is 0x0. The OMX_EmptyThisBuffer macro is
// invoked to pass buffers containing data when the component is in or making a transition to the
// OMX_StateExecuting or in the OMX_StatePause state.
INT32 i4VDOOmxCompEmptyBuffer(PARAMETER_OMX_EMPTYTHISBUFFER *prUArg)
{
    INT32 i4Ret = VDecOMX_ERR;
    OMX_ERRORTYPE omxRet = OMX_ErrorUndefined;
    PARAMETER_OMX_EMPTYTHISBUFFER OMX_EmptyBuf_Param;
    VDO_OMX_HANDLE_T *Omx_Handle;

    i4Ret = copy_from_user(&OMX_EmptyBuf_Param, prUArg, sizeof(PARAMETER_OMX_EMPTYTHISBUFFER));
     Omx_Handle = OMX_EmptyBuf_Param.pHandle;
    //VOMX_Printf("[VDO][OMX_EmptyBuffer] handle = 0x%x\n", Omx_Handle);
    if ((Omx_Handle == NULL) || (i4Ret != 0))
    {
        VOMX_Printf("%s(L:%d) Incorrect Handle.\n", __FUNCTION__, __LINE__);
        OMX_EmptyBuf_Param.eReturnValue = OMX_ErrorUndefined;
        i4Ret = copy_to_user(prUArg, (void *)&OMX_EmptyBuf_Param, sizeof(PARAMETER_OMX_EMPTYTHISBUFFER));
        return i4Ret;
    }

    if (_prVDOOmxHandle[Omx_Handle->h_handle].fgUsed && (OMX_EmptyBuf_Param.pHandle == &_prVDOOmxHandle[Omx_Handle->h_handle]))
    {
        // status is valid, proceed to send command to queue
    //VOMX_Printf("[VDO][OMX_EmptyBuffer] EOS flag = 0x%x\n", Omx_Handle->fgGotEOS);
        i4VDOOmxQueueEmptyBuf(OMX_EmptyBuf_Param.pHandle, OMX_EmptyBuf_Param.hPBufHeader);

        omxRet = OMX_ErrorNone;
    }

    OMX_EmptyBuf_Param.eReturnValue = omxRet;
    i4Ret = copy_to_user(prUArg, (void *)&OMX_EmptyBuf_Param, sizeof(PARAMETER_OMX_EMPTYTHISBUFFER));

    return i4Ret;
}


// The OMX_FillThisBuffer macro will send an empty buffer to an output port of a component.
// The OMX_FillThisBuffer macro is invoked to pass buffers containing no data when the component is in
// or making a transition to the OMX_StateExecuting state or is in the OMX_StatePaused state.
INT32 i4VDOOmxCompFillBuffer(PARAMETER_OMX_FILLTHISBUFFER *prUArg)
{
    INT32 i4Ret = VDecOMX_ERR;
    OMX_ERRORTYPE omxRet = OMX_ErrorUndefined;
    PARAMETER_OMX_FILLTHISBUFFER OMX_FillBuf_Param;
    VDO_OMX_HANDLE_T *Omx_Handle;

    i4Ret = copy_from_user(&OMX_FillBuf_Param, prUArg, sizeof(PARAMETER_OMX_FILLTHISBUFFER));
     Omx_Handle = OMX_FillBuf_Param.pHandle;
    //VOMX_Printf("[VDO][OMX_FillBuffer] handle = %d\n", Omx_Handle->h_handle);
    if ((Omx_Handle == NULL) || (i4Ret != 0))
    {
        VOMX_Printf("%s(L:%d) Incorrect Handle.\n", __FUNCTION__, __LINE__);
        OMX_FillBuf_Param.eReturnValue = OMX_ErrorUndefined;
        i4Ret = copy_to_user(prUArg, (void *)&OMX_FillBuf_Param, sizeof(PARAMETER_OMX_FILLTHISBUFFER));
        return i4Ret;
    }

    if (_prVDOOmxHandle[Omx_Handle->h_handle].fgUsed && (OMX_FillBuf_Param.pHandle == &_prVDOOmxHandle[Omx_Handle->h_handle]))
    {
        // status is valid, proceed to send command to queue
                i4VDOOmxQueueFillBuf(Omx_Handle, OMX_FillBuf_Param.hPBufHeader);
        if (Omx_Handle->fgFilling)
        {
//            i4VDOOmxDoFillBuf(Omx_Handle);
        }

        omxRet = OMX_ErrorNone;
    }

    OMX_FillBuf_Param.eReturnValue = omxRet;
    i4Ret = copy_to_user(prUArg, (void *)&OMX_FillBuf_Param, sizeof(PARAMETER_OMX_FILLTHISBUFFER));

    return i4Ret;
}


// A component uses the EmptyBufferDone callback to pass a buffer from an input port back to the IL client.
// A component sets the nOffset and nFilledLen values of the buffer header to reflect the portion of the
// buffer it consumed; for example, nFilledLen is set equal to 0x0 if completely consumed.
// The EmptyBufferDone call is a blocking call that should return from within five milliseconds.
INT32 i4VDOOmxCompNotifyEmptyDone(VDO_OMX_HANDLE_T *pHandle, VDO_OMX_BUFFER_T *pBufEmptied)
{
    PARAMETER_OMX_CB_BUFFER rCbInfo;

    //VOMX_Printf("[VDO][OMX] i4VDOOmxCompNotifyEmptyDone: pBufEmptied = 0x%X, pid = %d, pHandle->pEmptyBufferDone = 0x%X.\n",
//               pBufEmptied, pHandle->pid, pHandle->pEmptyBufferDone);
    // always full emptied
    pBufEmptied->rHeader.nFilledLen = pBufEmptied->u4FilledLen;

    x_memset(&rCbInfo, 0, sizeof(PARAMETER_OMX_CB_BUFFER));
    rCbInfo.hPBufHeader = pBufEmptied->hHeader;
    rCbInfo.pPlatformPrivate = pBufEmptied->rHeader.pPlatformPrivate;

    // since it is a blocking callback, use sync callback agent

    #if DTV_VOMX
    if(pHandle && pHandle->pEmptyBufferDone)
    {
        pHandle->pEmptyBufferDone(&rCbInfo, pHandle->pAppData);
    }
    #else
    vCBAgent_SyncCb_Nfy_EX(pHandle->pEmptyBufferDone,
                           pHandle->pAppData,
                           &rCbInfo,
                           pHandle->pid,
                           DRVT_VID_DEC,
                           sizeof(PARAMETER_OMX_CB_BUFFER));
    #endif

    return VDecOMX_OK;
}


// A component uses the FillBufferDone callback to pass a buffer from an output port back to the IL client.
// A component sets the nOffset and nFilledLen of the buffer header to reflect the portion of the buffer
// it filled (for example, nFilledLen is equal to 0x0 if it contains no data).
// The FillBufferDone call is a blocking call that should return from within five milliseconds.
INT32 i4VDOOmxCompNotifyFillDone(VDO_OMX_HANDLE_T *pHandle, VDO_OMX_BUFFER_T *pBufFilled)
{
    PARAMETER_OMX_CB_BUFFER rCbInfo;

    pBufFilled->rHeader.nFilledLen = pBufFilled->u4FilledLen;
    pBufFilled->rHeader.nOffset    = 0;
    x_memset(&rCbInfo, 0, sizeof(PARAMETER_OMX_CB_BUFFER));
    rCbInfo.hPBufHeader = pBufFilled->hHeader;
    rCbInfo.pPlatformPrivate = pBufFilled->rHeader.pPlatformPrivate;

    #if DTV_VOMX
    if(pHandle && pHandle->pFillBufferDone)
    {
        pHandle->pFillBufferDone(&rCbInfo, pHandle->pAppData);
    }
    #else
    vCBAgent_SyncCb_Nfy_EX(pHandle->pFillBufferDone,
                        pHandle->pAppData,
                        &rCbInfo,
                        pHandle->pid,
                        DRVT_VID_DEC,
                        sizeof(PARAMETER_OMX_CB_BUFFER));
    #endif

    return VDecOMX_OK;
}


// A call to EventHandler is a blocking call.
INT32 i4VDOOmxCompNotifyEvent(VDO_OMX_HANDLE_T *pHandle, PARAMETER_OMX_CB_EVENTHANDLER *pEventInfo)
{
    #if DTV_VOMX
    if(pHandle && pHandle->pEventHandler)
    {
        pHandle->pEventHandler(pEventInfo, pHandle->pAppData);
    }
    #else
    vCBAgent_SyncCb_Nfy_EX(pHandle->pEventHandler,
                        pHandle->pAppData,
                        pEventInfo,
                        pHandle->pid,
                        DRVT_VID_DEC,
                        sizeof(PARAMETER_OMX_CB_EVENTHANDLER));
    #endif

    return VDecOMX_OK;
}

INT32 i4VDOOmxCompGetParamVideoInit(PARAMETER_OMX_PORT_PARAM_TYPE *prUArg)
{
    INT32 i4Ret = 0;
    OMX_ERRORTYPE omxRet = OMX_ErrorUndefined;
    PARAMETER_OMX_PORT_PARAM_TYPE OMX_PortType_Param;
    OMX_PORT_PARAM_TYPE rPortInfo;
    VCONV_GET_PARAM_T GetParam;
    VDO_OMX_HANDLE_T *Omx_Handle;
    i4Ret = copy_from_user(&OMX_PortType_Param, prUArg, sizeof(PARAMETER_OMX_PORT_PARAM_TYPE));
     Omx_Handle = OMX_PortType_Param.pHandle;
    //VOMX_Printf("[VDO][OMX_GetParamVideoInit] handle = %d\n", Omx_Handle->h_handle);
    if ((Omx_Handle == NULL) || (i4Ret != 0))
    {
        VOMX_Printf("%s(L:%d) Incorrect Handle.\n", __FUNCTION__, __LINE__);
        OMX_PortType_Param.eReturnValue = OMX_ErrorUndefined;
        i4Ret = copy_to_user(prUArg, (void *)&OMX_PortType_Param, sizeof(PARAMETER_OMX_PORT_PARAM_TYPE));
        return i4Ret;
    }

    if (_prVDOOmxHandle[Omx_Handle->h_handle].fgUsed && (OMX_PortType_Param.pHandle == &_prVDOOmxHandle[Omx_Handle->h_handle]))
    {
    //VOMX_Printf("[VDO][OMX_GetParamVideoInit] \n");
        i4Ret = copy_from_user(&rPortInfo, OMX_PortType_Param.pPortParamType, sizeof(OMX_PORT_PARAM_TYPE));
        GetParam.u4PortNum = 0;
        i4Ret = _i4VConv_GetParam(Omx_Handle->h_handle, &GetParam);

        if (OMX_PARAM_SET == OMX_PortType_Param.eDir)
        {
            // Set Parameter
            //VOMX_Printf("[VDO][OMX] set OMX_IndexParamVideoInit?\n");
        }
        else
        {
            // Get Parameter
            //VOMX_Printf("[VDO][OMX] get OMX_IndexParamVideoInit\n");
            rPortInfo.nPorts = GetParam.u4PortCount; // 2 ports: 0 - input, 1 - output
            rPortInfo.nStartPortNumber = GetParam.u4StartPortNumber;

            omxRet = OMX_ErrorNone;

            i4Ret = copy_to_user(OMX_PortType_Param.pPortParamType, (void *)&rPortInfo, sizeof(OMX_PORT_PARAM_TYPE));
       }
    }

    OMX_PortType_Param.eReturnValue = omxRet;
    i4Ret = copy_to_user(prUArg, (void *)&OMX_PortType_Param, sizeof(PARAMETER_OMX_PORT_PARAM_TYPE));
            //VOMX_Printf("[VDO][OMX] get OMX_IndexParamVideoInit : i4Ret = %d\n", i4Ret);

    return i4Ret;
}


INT32 i4VDOOmxCompParamPortDef(PARAMETER_OMX_PARAM_PORTDEFINITIONTYPE *prUArg)
{
    INT32 i4Ret = 0;
    OMX_ERRORTYPE omxRet = OMX_ErrorNone;
    PARAMETER_OMX_PARAM_PORTDEFINITIONTYPE OMX_PortDefType_Param;
    OMX_PARAM_PORTDEFINITIONTYPE rPortDef;
    VCONV_GET_PARAM_T GetParam;
    VCONV_SET_PARAM_T SetParam;
    VDO_OMX_HANDLE_T *Omx_Handle;
    VDEC_SET_INTO_T rVdecSetInfo;
    i4Ret = copy_from_user(&OMX_PortDefType_Param, prUArg, sizeof(PARAMETER_OMX_PARAM_PORTDEFINITIONTYPE));
    Omx_Handle = OMX_PortDefType_Param.pHandle;
    //VOMX_Printf("[VDO][OMX_ParamPortDef] handle = %d\n", Omx_Handle->h_handle);
    if ((Omx_Handle == NULL) || (i4Ret != 0))
    {
        VOMX_Printf("%s(L:%d) Incorrect Handle.\n", __FUNCTION__, __LINE__);
        OMX_PortDefType_Param.eReturnValue = OMX_ErrorUndefined;
        i4Ret = copy_to_user(prUArg, (void *)&OMX_PortDefType_Param, sizeof(PARAMETER_OMX_PARAM_PORTDEFINITIONTYPE));
        return i4Ret;
    }

    if (_prVDOOmxHandle[Omx_Handle->h_handle].fgUsed && (OMX_PortDefType_Param.pHandle == &_prVDOOmxHandle[Omx_Handle->h_handle]))
    {
        i4Ret = copy_from_user(&rPortDef, OMX_PortDefType_Param.pPortDefinitionType, sizeof(OMX_PARAM_PORTDEFINITIONTYPE));
        if (OMX_PARAM_SET == OMX_PortDefType_Param.eDir)
        {
            // Set Parameter
            //VOMX_Printf("[VDO][OMX] set OMX_IndexParamPortDefinition, not processed.\n");
            x_memset(&SetParam, 0, sizeof(VCONV_SET_PARAM_T));
            SetParam.u4PortNum = rPortDef.nPortIndex;
            if((rPortDef.nPortIndex)%2 == 0)
            {
                VOMX_Printf("[VDO][OMX] input compression format(%d).\n",
                    (UINT32)rPortDef.format.video.eCompressionFormat);
            }
            else if((rPortDef.nPortIndex)%2 == 1)
            {
                VOMX_Printf("[VDO][OMX] output color format(%d).\n",
                    (UINT32)rPortDef.format.video.eColorFormat);
            }
            else
            {
                VOMX_Printf("[VDO][OMX] output color format unsupported(%d).\n",
                    (UINT32)rPortDef.format.video.eColorFormat);
                return OMX_ErrorUndefined;
            }
            if(rPortDef.eDomain == OMX_PortDomainVideo)
            {
                if((rPortDef.nPortIndex)%2 == 1)
                {
                    x_memset(&rVdecSetInfo, 0, sizeof(VDEC_SET_INTO_T));
                    if(rPortDef.format.video.eColorFormat == OMX_COLOR_Format32bitARGB8888)
                    {
                        SetParam.eVConvFormat = VCONV_COLOR_ARBG8888;
                        rVdecSetInfo.fgVPushDecodeOnly = TRUE;
                    }
                    else if(rPortDef.format.video.eColorFormat == OMX_COLOR_FormatYUV420Planar)
                    {
                        SetParam.eVConvFormat = VCONV_COLOR_YUV420;
                        rVdecSetInfo.fgVPushDecodeOnly = TRUE;
                    }
                    else if(rPortDef.format.video.eColorFormat == OMX_COLOR_FormatYUV420SemiPlanar)
                    {
                        SetParam.eVConvFormat = VCONV_COLOR_YUV420SemiPlanar;
                        rVdecSetInfo.fgVPushDecodeOnly = TRUE;
                    }
                    else if(rPortDef.format.video.eColorFormat == OMX_COLOR_FormatVendorStartUnused)
                    {
                        SetParam.eVConvFormat = VCONV_COLOR_B2R2;
                        rVdecSetInfo.fgVPushDecodeOnly = FALSE;
                    }
                    else
                    {
                        VOMX_Printf("[VDO][OMX] output color format unsupported(%d).\n",
                            (UINT32)rPortDef.format.video.eColorFormat);
                        return OMX_ErrorUndefined;
                    }
                    _prVDOOmxHandle[Omx_Handle->h_handle].eColorFormatIndex0 =
                    rPortDef.format.video.eColorFormat;
                    UNUSED(_VPUSH_SetInfo(_prVDOOmxHandle[Omx_Handle->h_handle].pVdecPush, &rVdecSetInfo));
                }
                SetParam.u4FrameWidth = rPortDef.format.video.nFrameWidth;
                SetParam.u4FrameHeight = rPortDef.format.video.nFrameHeight;

                SetParam.u4FrameRate = rPortDef.format.video.xFramerate;
                
                if (rPortDef.nPortIndex == 0 && SetParam.u4FrameWidth && SetParam.u4FrameHeight)
                {
                    _VPUSH_SetPicSz(
                        _prVDOOmxHandle[Omx_Handle->h_handle].pVdecPush,
                        SetParam.u4FrameWidth,
                        SetParam.u4FrameHeight,
                        rPortDef.format.video.xFramerate);
                }
            }
            else if((rPortDef.eDomain == OMX_PortDomainImage) &&
               ((rPortDef.nPortIndex)%2 == OMX_DirInput))
            {
                //cancel type redefine (from MJPG)
            }
            i4Ret = _i4VConv_SetParam(Omx_Handle->h_handle, &SetParam);
        }
        else
        {
            GetParam.u4PortNum = rPortDef.nPortIndex;
            i4Ret = _i4VConv_GetParam(Omx_Handle->h_handle, &GetParam);

            // Get Parameter
            //VOMX_Printf("[VDO][OMX] get OMX_IndexParamPortDefinition, part implemented.\n");

            if(_prVDOOmxHandle[Omx_Handle->h_handle].eCodecType == VC_JPG)
            {
                if((rPortDef.nPortIndex)%2 == OMX_DirInput)
                {
                    rPortDef.eDir= OMX_DirInput;
                    rPortDef.eDomain = OMX_PortDomainImage;
                    rPortDef.format.image.eCompressionFormat = OMX_IMAGE_CodingJPEG;
                }
                else if((rPortDef.nPortIndex)%2 == OMX_DirOutput)
                {
                    rPortDef.eDir= OMX_DirOutput;
                    rPortDef.format.image.eColorFormat = OMX_COLOR_FormatVendorStartUnused;
                    rPortDef.format.image.nFrameWidth = 0;
                    rPortDef.format.image.nFrameHeight = 0;
                }
            }
            else if ((rPortDef.nPortIndex)%2 == 0)
            {
                rPortDef.eDir= OMX_DirInput;
                rPortDef.eDomain = OMX_PortDomainVideo;
                rPortDef.format.video.eColorFormat = OMX_COLOR_FormatUnused;
                switch(_prVDOOmxHandle[Omx_Handle->h_handle].eCodecType)
                {
                case VC_MPEG2:
                    rPortDef.format.video.eCompressionFormat = OMX_VIDEO_CodingMPEG2;
                    break;
                case VC_H263:
                case VC_H263_SORENSON:
                    rPortDef.format.video.eCompressionFormat = OMX_VIDEO_CodingH263;
                    break;
                case VC_MPEG4:
                    rPortDef.format.video.eCompressionFormat = OMX_VIDEO_CodingMPEG4;
                    break;
                case VC_WMV1:
                case VC_WMV2:
                case VC_WMV3:
                case VC_VC1:
                    rPortDef.format.video.eCompressionFormat = OMX_VIDEO_CodingWMV;
                    break;
                case VC_RV30:
                case VC_RV40:
                    rPortDef.format.video.eCompressionFormat = OMX_VIDEO_CodingRV;
                    break;
                case VC_H264:
                    rPortDef.format.video.eCompressionFormat = OMX_VIDEO_CodingAVC;
                    break;
                case VC_MJPG:
                    rPortDef.format.video.eCompressionFormat = OMX_VIDEO_CodingMJPEG;
                    break;
#ifdef VDEC_IS_POST_MT5368
                case VC_VP6:
                    rPortDef.format.video.eCompressionFormat = OMX_VIDEO_CodingVP6;
                    break;
                case VC_VP8:
                    rPortDef.format.video.eCompressionFormat = OMX_VIDEO_CodingVP8;
                    break;
#endif
                default:
                    rPortDef.format.video.eCompressionFormat = OMX_VIDEO_CodingUnused;
                    break;
                }
            }
            else
            {
                rPortDef.eDir= OMX_DirOutput;
                rPortDef.eDomain = OMX_PortDomainVideo;
                rPortDef.format.video.eCompressionFormat = OMX_VIDEO_CodingUnused;

                if(GetParam.eVConvFormat == VCONV_COLOR_ARBG8888)
                {
                    rPortDef.format.video.eColorFormat = OMX_COLOR_Format32bitARGB8888;
                }
                else if(GetParam.eVConvFormat == VCONV_COLOR_YUV420)
                {
                    rPortDef.format.video.eColorFormat = OMX_COLOR_FormatYUV420Planar;
                }
                else if(GetParam.eVConvFormat == VCONV_COLOR_YUV420SemiPlanar)
                {
                    rPortDef.format.video.eColorFormat = OMX_COLOR_FormatYUV420SemiPlanar;
                }
                else if(GetParam.eVConvFormat == VCONV_COLOR_B2R2)
                {
                    rPortDef.format.video.eColorFormat = OMX_COLOR_FormatVendorStartUnused;
                }
                else
                {
                    VOMX_Printf("[VDO][OMX] VConv output color format unsupported(%d).\n",
                        (UINT32)GetParam.eVConvFormat);
                    return OMX_ErrorUndefined;
                }

                rPortDef.format.video.nFrameWidth = GetParam.u4FrameWidth;
                rPortDef.format.video.nFrameHeight = GetParam.u4FrameHeight;
                rPortDef.format.video.nSliceHeight = GetParam.u4FrameHeight;
                rPortDef.format.video.nStride = GetParam.u4Stride;
            }
            rPortDef.nBufferCountMin = GetParam.u4BufferCountMin;
            rPortDef.nBufferCountActual = GetParam.u4BufferCountActual;
            rPortDef.nBufferSize = GetParam.u4BufferSize;
            rPortDef.nBufferAlignment = 0x40;

            i4Ret = copy_to_user(OMX_PortDefType_Param.pPortDefinitionType, (void *)&rPortDef, sizeof(OMX_PARAM_PORTDEFINITIONTYPE));
        }
    }
    else
    {
        omxRet = OMX_ErrorUndefined;
    }

    OMX_PortDefType_Param.eReturnValue = omxRet;
    i4Ret = copy_to_user(prUArg, (void *)&OMX_PortDefType_Param, sizeof(PARAMETER_OMX_PARAM_PORTDEFINITIONTYPE));


    return i4Ret;
}


INT32 i4VDOOmxCompFlush(VDO_OMX_HANDLE_T *pHandle, BOOL fgInput, BOOL fgOutput)
{
    INT32 i4Ret = 0;
    PARAMETER_OMX_CB_EVENTHANDLER rEvent;

    if (fgInput)
    {
        // flush all pending empty buffer request
        i4VDOOmxForceAllBufEmptyDone(pHandle);

        // 20110803, flush all pending fill buffer request
        i4VDOOmxForceAllBufFillDone(pHandle);

        // callback for flush done
        rEvent.eEvent     = OMX_EventCmdComplete;
        rEvent.nData1     = OMX_CommandFlush;
        rEvent.nData2     = 0;
        rEvent.pEventData = NULL;

        LOG(5, "%s(%d), i4VDOOmxCompFlush input done\n", __FUNCTION__, __LINE__);
        _prVDOOmxHandle[pHandle->h_handle].fgFlush[0]        = FALSE;
        pHandle->fgGotEOS = FALSE;
        i4VDOOmxCompNotifyEvent(pHandle, &rEvent);
    }

    if (fgOutput)
    {
        // flush all pending fill buffer request
        i4VDOOmxForceAllBufFillDone(pHandle);

        // callback for flush done
        rEvent.eEvent     = OMX_EventCmdComplete;
        rEvent.nData1     = OMX_CommandFlush;
        rEvent.nData2     = 1;
        rEvent.pEventData = NULL;

        LOG(5, "%s(%d), i4VDOOmxCompFlush output done\n", __FUNCTION__, __LINE__);
        _prVDOOmxHandle[pHandle->h_handle].fgFlush[1]        = FALSE;
        pHandle->fgSentEOS = FALSE;
        i4VDOOmxCompNotifyEvent(pHandle, &rEvent);
    }

    return i4Ret;
}


INT32 i4VDOOmxCompFlushAsync(PARAMETER_OMX_COMMAND_PORT *prUArg)
{
    INT32 i4Ret = 0;
    OMX_ERRORTYPE omxRet = OMX_ErrorNone;
    PARAMETER_OMX_COMMAND_PORT rKArg;
//    VDO_OMX_MSG_T rMsgFlush;
    VDO_OMX_HANDLE_T *Omx_Handle;

    i4Ret = copy_from_user(&rKArg, prUArg, sizeof(PARAMETER_OMX_COMMAND_PORT));
    Omx_Handle = rKArg.pHandle;
    //VOMX_Printf("[VDO][OMX_FlushAsync] handle = %d\n", Omx_Handle->h_handle);
    if ((Omx_Handle == NULL) || (i4Ret != 0))
    {
        VOMX_Printf("%s(L:%d) Incorrect Handle.\n", __FUNCTION__, __LINE__);
        rKArg.eReturnValue = OMX_ErrorUndefined;
        i4Ret = copy_to_user(prUArg, (void *)&rKArg, sizeof(PARAMETER_OMX_COMMAND_PORT));
        return i4Ret;
    }

    if (_prVDOOmxHandle[Omx_Handle->h_handle].fgUsed && (rKArg.pHandle == &_prVDOOmxHandle[Omx_Handle->h_handle]))
    {
        // status is valid, proceed to send command to queue
        if (0 == rKArg.nPorts)
        {
            _prVDOOmxHandle[Omx_Handle->h_handle].fgFlush[0]        = TRUE;
        }
        else if (1 == rKArg.nPorts)
        {
            _prVDOOmxHandle[Omx_Handle->h_handle].fgFlush[1]        = TRUE;
        }
        else if (OMX_ALL == rKArg.nPorts)
        {
            _prVDOOmxHandle[Omx_Handle->h_handle].fgFlush[0]        = TRUE;
            _prVDOOmxHandle[Omx_Handle->h_handle].fgFlush[1]        = TRUE;
        }
        if(_prVDOOmxHandle[Omx_Handle->h_handle].eFlushStatus == VOMX_FLUSHING_NONE)
        {
            // flush sequence is:
            // VConv_Flush -> VConv main loop -> VConv flush done ->
            // VPUSH_Flush -> VPUSH main loop -> MPV_FlushEsmQ -> VPush flush done
            _prVDOOmxHandle[Omx_Handle->h_handle].eFlushStatus = VOMX_FLUSHING_VCONV;
            _i4VConv_Flush(Omx_Handle->h_handle);
        }
    }
    else
    {
        omxRet = OMX_ErrorUndefined;
    }

    rKArg.eReturnValue = omxRet;
    i4Ret = copy_to_user(prUArg, (void *)&rKArg, sizeof(PARAMETER_OMX_COMMAND_PORT));

    return i4Ret;
}

INT32 i4VDOOmxCompPortDisable(VDO_OMX_HANDLE_T *pHandle, BOOL fgInput, BOOL fgOutput)
{
    INT32 i4Ret = 0;
    PARAMETER_OMX_CB_EVENTHANDLER rEvent;

    if (fgInput)
    {
        // flush all pending empty buffer request
    //    i4VDOOmxForceAllBufEmptyDone(pHandle);

        // TODO: wait all buffers freed before callback command complete

        // callback for port disabling done
        rEvent.eEvent     = OMX_EventCmdComplete;
        rEvent.nData1     = OMX_CommandPortDisable;
        rEvent.nData2     = 0;
        rEvent.pEventData = NULL;

        i4VDOOmxCompNotifyEvent(pHandle, &rEvent);
    }

    if (fgOutput)
    {
        // this is a temp solution for resolution when disable/enable port
        // this only works before buffer is used.
        if((pHandle->eState == OMX_StateExecuting) ||
           (pHandle->eState == OMX_StatePause))
        {
            _i4VConv_Pause(_prVDOOmxHandle[pHandle->h_handle].h_handle);
        }

        // flush all pending fill buffer request
  //      i4VDOOmxForceAllBufFillDone(pHandle);

        // TODO: wait all buffers freed before callback command complete

        // callback for port disabling done
        rEvent.eEvent     = OMX_EventCmdComplete;
        rEvent.nData1     = OMX_CommandPortDisable;
        rEvent.nData2     = 1;
        rEvent.pEventData = NULL;

        i4VDOOmxCompNotifyEvent(pHandle, &rEvent);
    }

    return i4Ret;
}


INT32 i4VDOOmxCompPortDisableAsync(PARAMETER_OMX_COMMAND_PORT *prUArg)
{
    INT32 i4Ret = 0;
    OMX_ERRORTYPE omxRet = OMX_ErrorNone;
    PARAMETER_OMX_COMMAND_PORT rKArg;
    VDO_OMX_MSG_T rMsgFlush;
    VDO_OMX_HANDLE_T *Omx_Handle;

    i4Ret = copy_from_user(&rKArg, prUArg, sizeof(PARAMETER_OMX_COMMAND_PORT));

    Omx_Handle = rKArg.pHandle;
    if ((Omx_Handle == NULL) || (i4Ret != 0))
    {
        VOMX_Printf("%s(L:%d) Incorrect Handle.\n", __FUNCTION__, __LINE__);
        rKArg.eReturnValue = OMX_ErrorUndefined;
        i4Ret = copy_to_user(prUArg, (void *)&rKArg, sizeof(PARAMETER_OMX_COMMAND_PORT));
        return i4Ret;
    }

    if (_prVDOOmxHandle[Omx_Handle->h_handle].fgUsed && (rKArg.pHandle == &_prVDOOmxHandle[Omx_Handle->h_handle]))
    {
        // status is valid, proceed to send command to queue
        rMsgFlush.eCmd     = VDO_OMX_CMD_PORT_DISABLE;
        rMsgFlush.fgInput  = (0 == rKArg.nPorts) || (OMX_ALL == rKArg.nPorts);
        rMsgFlush.fgOutput = (1 == rKArg.nPorts) || (OMX_ALL == rKArg.nPorts);

        // TODO: modify port definition bEnabled flag

        #if 0
        i4VDOOmxCompPortDisable(Omx_Handle, rMsgFlush.fgInput, rMsgFlush.fgOutput);
        #else
        i4Ret = i4VDOOmxTaskSendCommand(rKArg.pHandle, &rMsgFlush);
        #endif
    }
    else
    {
        omxRet = OMX_ErrorUndefined;
    }

    rKArg.eReturnValue = omxRet;
    i4Ret = copy_to_user(prUArg, (void *)&rKArg, sizeof(PARAMETER_OMX_COMMAND_PORT));

    return i4Ret;
}


INT32 i4VDOOmxCompPortEnable(VDO_OMX_HANDLE_T *pHandle, BOOL fgInput, BOOL fgOutput)
{
    INT32 i4Ret = 0;
    PARAMETER_OMX_CB_EVENTHANDLER rEvent;

    if (fgInput)
    {
        // TODO:

        // callback for port enabling done
        rEvent.eEvent     = OMX_EventCmdComplete;
        rEvent.nData1     = OMX_CommandPortEnable;
        rEvent.nData2     = 0;
        rEvent.pEventData = NULL;

        i4VDOOmxCompNotifyEvent(pHandle, &rEvent);
    }

    if (fgOutput)
    {
        // this is a temp solution for resolution when disable/enable port
        // this only works before buffer is used.
        if((pHandle->eState == OMX_StateExecuting) ||
           (pHandle->eState == OMX_StatePause))
        {
            _i4VConv_Resume(_prVDOOmxHandle[pHandle->h_handle].h_handle);
        }

        // TODO:

        // callback for port enabling done
        rEvent.eEvent     = OMX_EventCmdComplete;
        rEvent.nData1     = OMX_CommandPortEnable;
        rEvent.nData2     = 1;
        rEvent.pEventData = NULL;

        i4VDOOmxCompNotifyEvent(pHandle, &rEvent);
    }

    return i4Ret;
}


INT32 i4VDOOmxCompPortEnableAsync(PARAMETER_OMX_COMMAND_PORT *prUArg)
{
    INT32 i4Ret = 0;
    OMX_ERRORTYPE omxRet = OMX_ErrorNone;
    PARAMETER_OMX_COMMAND_PORT rKArg;
    VDO_OMX_MSG_T rMsgFlush;
    VDO_OMX_HANDLE_T *Omx_Handle;

    i4Ret = copy_from_user(&rKArg, prUArg, sizeof(PARAMETER_OMX_COMMAND_PORT));

    Omx_Handle = rKArg.pHandle;
    if ((Omx_Handle == NULL) || (i4Ret != 0))
    {
        VOMX_Printf("%s(L:%d) Incorrect Handle.\n", __FUNCTION__, __LINE__);
        rKArg.eReturnValue = OMX_ErrorUndefined;
        i4Ret = copy_to_user(prUArg, (void *)&rKArg, sizeof(PARAMETER_OMX_COMMAND_PORT));
        return i4Ret;
    }

    if (_prVDOOmxHandle[Omx_Handle->h_handle].fgUsed && (rKArg.pHandle == &_prVDOOmxHandle[Omx_Handle->h_handle]))
    {
        // status is valid, proceed to send command to queue
        rMsgFlush.eCmd     = VDO_OMX_CMD_PORT_ENABLE;
        rMsgFlush.fgInput  = (0 == rKArg.nPorts) || (OMX_ALL == rKArg.nPorts);
        rMsgFlush.fgOutput = (1 == rKArg.nPorts) || (OMX_ALL == rKArg.nPorts);

        // TODO: modify port definition bEnabled flag

        #if 0
        i4VDOOmxCompPortEnable(Omx_Handle, rMsgFlush.fgInput, rMsgFlush.fgOutput);
        #else
        i4Ret = i4VDOOmxTaskSendCommand(rKArg.pHandle, &rMsgFlush);
        #endif
    }
    else
    {
        omxRet = OMX_ErrorUndefined;
    }

    rKArg.eReturnValue = omxRet;
    i4Ret = copy_to_user(prUArg, (void *)&rKArg, sizeof(PARAMETER_OMX_COMMAND_PORT));

    return i4Ret;
}

INT32 i4VDOOmxCompParamVideoProfileLevelQuerySupported(PARAMETER_OMX_VIDEO_PARAM_PROFILELEVELTYPE *prUArg)
{
    INT32 i4Ret = 0;
    OMX_ERRORTYPE omxRet = OMX_ErrorNone;
    PARAMETER_OMX_VIDEO_PARAM_PROFILELEVELTYPE rKArg;
    OMX_VIDEO_PARAM_PROFILELEVELTYPE rVideoParamPro;
    VDO_OMX_HANDLE_T *Omx_Handle;

    i4Ret = copy_from_user(&rKArg, prUArg, sizeof(PARAMETER_OMX_VIDEO_PARAM_PROFILELEVELTYPE));


    Omx_Handle = rKArg.pHandle;
    if ((Omx_Handle == NULL) || (i4Ret != 0))
    {
        VOMX_Printf("%s(L:%d) Incorrect Handle.\n", __FUNCTION__, __LINE__);
        rKArg.eReturnValue = OMX_ErrorUndefined;
        i4Ret = copy_to_user(prUArg, (void *)&rKArg, sizeof(PARAMETER_OMX_VIDEO_PARAM_PROFILELEVELTYPE));
        return i4Ret;
    }

    if (_prVDOOmxHandle[0].fgUsed && (rKArg.pHandle == &_prVDOOmxHandle[0]))
    {
        i4Ret = copy_from_user(&rVideoParamPro, rKArg.pVideoParamPro, sizeof(OMX_VIDEO_PARAM_PROFILELEVELTYPE));

        /*
        rVideoParamPro.nSize = sizeof(OMX_VIDEO_PARAM_PROFILELEVELTYPE);
        rVideoParamPro.nVersion.s.nVersionMajor = 1;
        rVideoParamPro.nVersion.s.nVersionMinor = 1;
        rVideoParamPro.nVersion.s.nRevision = 2;
        rVideoParamPro.nVersion.s.nStep = 0;
        rVideoParamPro.nPortIndex = 0;
        */

        switch(_prVDOOmxHandle[Omx_Handle->h_handle].eCodecType)
        {
        case VC_MPEG2:
            if(rVideoParamPro.nProfileIndex == 0)
            {
                rVideoParamPro.eProfile = OMX_VIDEO_MPEG2ProfileSimple;
                rVideoParamPro.eLevel = OMX_VIDEO_MPEG2LevelHL;
            }
            else if(rVideoParamPro.nProfileIndex == 1)
            {
                rVideoParamPro.eProfile = OMX_VIDEO_MPEG2ProfileMain;
                rVideoParamPro.eLevel = OMX_VIDEO_MPEG2LevelHL;
            }
            else
            {
                omxRet = OMX_ErrorUndefined;
            }
            break;
        case VC_H263:
        case VC_H263_SORENSON:
            if(rVideoParamPro.nProfileIndex == 0)
            {
                rVideoParamPro.eProfile = OMX_VIDEO_H263ProfileBaseline;
                rVideoParamPro.eLevel = OMX_VIDEO_H263Level10;
            }
            else
            {
                omxRet = OMX_ErrorUndefined;
            }
            break;
        case VC_MPEG4:
            if(rVideoParamPro.nProfileIndex == 0)
            {
                rVideoParamPro.eProfile = OMX_VIDEO_MPEG4ProfileSimple;
                rVideoParamPro.eLevel = OMX_VIDEO_MPEG4Level5;
            }
            else if(rVideoParamPro.nProfileIndex == 1)
            {
                rVideoParamPro.eProfile = OMX_VIDEO_MPEG4ProfileCore;
                rVideoParamPro.eLevel = OMX_VIDEO_MPEG4Level5;
            }
            else if(rVideoParamPro.nProfileIndex == 2)
            {
                rVideoParamPro.eProfile = OMX_VIDEO_MPEG4ProfileMain;
                rVideoParamPro.eLevel = OMX_VIDEO_MPEG4Level5;
            }
            else if(rVideoParamPro.nProfileIndex == 3)
            {
                rVideoParamPro.eProfile = OMX_VIDEO_MPEG4ProfileAdvancedSimple;
                rVideoParamPro.eLevel = OMX_VIDEO_MPEG4Level5;
            }
            else
            {
                omxRet = OMX_ErrorUndefined;
            }
            break;
        case VC_WMV1:
        case VC_WMV2:
        case VC_WMV3:
        case VC_VC1:
            if(rVideoParamPro.nProfileIndex == 0)
            {
                rVideoParamPro.eProfile = OMX_VIDEO_WMVFormat7;
            }
            else if(rVideoParamPro.nProfileIndex == 1)
            {
                rVideoParamPro.eProfile = OMX_VIDEO_WMVFormat8;
            }
            else if(rVideoParamPro.nProfileIndex == 2)
            {
                rVideoParamPro.eProfile = OMX_VIDEO_WMVFormat9;
            }
            else
            {
                omxRet = OMX_ErrorUndefined;
            }
            break;
        case VC_RV30:
        case VC_RV40:
            if(rVideoParamPro.nProfileIndex == 0)
            {
                rVideoParamPro.eProfile = OMX_VIDEO_RVFormat8;
            }
            else if(rVideoParamPro.nProfileIndex == 1)
            {
                rVideoParamPro.eProfile = OMX_VIDEO_RVFormat9;
            }
            else
            {
                omxRet = OMX_ErrorUndefined;
            }
            break;
        case VC_H264:
            if(rVideoParamPro.nProfileIndex == 0)
            {
                rVideoParamPro.eProfile = OMX_VIDEO_AVCProfileBaseline;
                rVideoParamPro.eLevel = OMX_VIDEO_AVCLevel41;
            }
            else if(rVideoParamPro.nProfileIndex == 1)
            {
                rVideoParamPro.eProfile = OMX_VIDEO_AVCProfileMain;
                rVideoParamPro.eLevel = OMX_VIDEO_AVCLevel41;
            }
            else if(rVideoParamPro.nProfileIndex == 2)
            {
                rVideoParamPro.eProfile = OMX_VIDEO_AVCProfileHigh;
                rVideoParamPro.eLevel = OMX_VIDEO_AVCLevel41;
            }
            else
            {
                omxRet = OMX_ErrorUndefined;
            }
            break;
        case VC_MJPG:
        case VC_VP6:
        case VC_VP8:
        default:
            // all support ?
            rVideoParamPro.eProfile = 0xFFFFFF;
            rVideoParamPro.eLevel = 0xFFFFFF;
            break;
        }

        i4Ret = copy_to_user(rKArg.pVideoParamPro, (void *)&rVideoParamPro, sizeof(OMX_VIDEO_PARAM_PROFILELEVELTYPE));
    }
    else
    {
        omxRet = OMX_ErrorUndefined;
    }

    rKArg.eReturnValue = omxRet;
    i4Ret = copy_to_user(prUArg, (void *)&rKArg, sizeof(PARAMETER_OMX_VIDEO_PARAM_PROFILELEVELTYPE));

    return i4Ret;
}


INT32 i4VDOOmxCompParamVideoAvc(PARAMETER_OMX_VIDEO_PARAM_AVC *prUArg)
{
    INT32 i4Ret = 0;
    OMX_ERRORTYPE omxRet = OMX_ErrorNone;
    PARAMETER_OMX_VIDEO_PARAM_AVC rKArg;
    OMX_VIDEO_PARAM_AVCTYPE rVideoParamAvc;
    VDO_OMX_HANDLE_T *Omx_Handle;

    i4Ret = copy_from_user(&rKArg, prUArg, sizeof(PARAMETER_OMX_VIDEO_PARAM_AVC));


    Omx_Handle = rKArg.pHandle;
    if ((Omx_Handle == NULL) || (i4Ret != 0))
    {
        VOMX_Printf("%s(L:%d) Incorrect Handle.\n", __FUNCTION__, __LINE__);
        rKArg.eReturnValue = OMX_ErrorUndefined;
        i4Ret = copy_to_user(prUArg, (void *)&rKArg, sizeof(PARAMETER_OMX_VIDEO_PARAM_AVC));
        return i4Ret;
    }

    if (_prVDOOmxHandle[Omx_Handle->h_handle].fgUsed && (rKArg.pHandle == &_prVDOOmxHandle[Omx_Handle->h_handle]))
    {
        i4Ret = copy_from_user(&rVideoParamAvc, rKArg.pVideoParamAvc, sizeof(OMX_VIDEO_PARAM_AVCTYPE));

        i4Ret = copy_to_user(rKArg.pVideoParamAvc, (void *)&rVideoParamAvc, sizeof(OMX_VIDEO_PARAM_AVCTYPE));
    }
    else
    {
        omxRet = OMX_ErrorUndefined;
    }

    rKArg.eReturnValue = omxRet;
    i4Ret = copy_to_user(prUArg, (void *)&rKArg, sizeof(PARAMETER_OMX_VIDEO_PARAM_AVC));

    return i4Ret;
}


INT32 i4VDOOmxCompParamVideoRv(PARAMETER_OMX_VIDEO_PARAM_RV *prUArg)
{
    INT32 i4Ret = 0;
    OMX_ERRORTYPE omxRet = OMX_ErrorNone;
    PARAMETER_OMX_VIDEO_PARAM_RV rKArg;
    OMX_VIDEO_PARAM_RVTYPE rVideoParamRv;
    VDO_OMX_HANDLE_T *Omx_Handle;

    i4Ret = copy_from_user(&rKArg, prUArg, sizeof(PARAMETER_OMX_VIDEO_PARAM_RV));


    Omx_Handle = rKArg.pHandle;
    if ((Omx_Handle == NULL) || (i4Ret != 0))
    {
        VOMX_Printf("%s(L:%d) Incorrect Handle.\n", __FUNCTION__, __LINE__);
        rKArg.eReturnValue = OMX_ErrorUndefined;
        i4Ret = copy_to_user(prUArg, (void *)&rKArg, sizeof(PARAMETER_OMX_VIDEO_PARAM_RV));
        return i4Ret;
    }

    if (_prVDOOmxHandle[Omx_Handle->h_handle].fgUsed && (rKArg.pHandle == &_prVDOOmxHandle[Omx_Handle->h_handle]))
    {
        i4Ret = copy_from_user(&rVideoParamRv, rKArg.pVideoParamRv, sizeof(OMX_VIDEO_PARAM_RVTYPE));

        if(rKArg.eDir == OMX_PARAM_SET)
        {
            if(rVideoParamRv.eFormat == OMX_VIDEO_RVFormat8)
            {
                _prVDOOmxHandle[Omx_Handle->h_handle].eCodecType = VC_RV30;
                LOG(5, "%s(%d), OMX_VIDEO_RVFormat8\n", __FUNCTION__, __LINE__);
            }
            else if(rVideoParamRv.eFormat == OMX_VIDEO_RVFormat9)
            {
                _prVDOOmxHandle[Omx_Handle->h_handle].eCodecType = VC_RV40;
                LOG(5, "%s(%d), OMX_VIDEO_RVFormat9\n", __FUNCTION__, __LINE__);
            }
            else
            {
                LOG(5, "%s(%d), unknow format\n", __FUNCTION__, __LINE__);
            }
            VOMX_Printf("[VDO][OMX] i4VDOOmxCompParamVideoRv unknown(%d)\n", (UINT32)rVideoParamRv.eFormat);
        }

        i4Ret = copy_to_user(rKArg.pVideoParamRv, (void *)&rVideoParamRv, sizeof(OMX_VIDEO_PARAM_RVTYPE));
    }
    else
    {
        omxRet = OMX_ErrorUndefined;
    }

    rKArg.eReturnValue = omxRet;
    i4Ret = copy_to_user(prUArg, (void *)&rKArg, sizeof(PARAMETER_OMX_VIDEO_PARAM_RV));

    return i4Ret;
}


INT32 i4VDOOmxCompParamVideoWmv(PARAMETER_OMX_VIDEO_PARAM_WMV *prUArg)
{
    INT32 i4Ret = 0;
    OMX_ERRORTYPE omxRet = OMX_ErrorNone;
    PARAMETER_OMX_VIDEO_PARAM_WMV rKArg;
    OMX_VIDEO_PARAM_WMVTYPE rVideoParamWmv;
    VDO_OMX_HANDLE_T *Omx_Handle;

    i4Ret = copy_from_user(&rKArg, prUArg, sizeof(PARAMETER_OMX_VIDEO_PARAM_WMV));


    Omx_Handle = rKArg.pHandle;
    if ((Omx_Handle == NULL) || (i4Ret != 0))
    {
        VOMX_Printf("%s(L:%d) Incorrect Handle.\n", __FUNCTION__, __LINE__);
        rKArg.eReturnValue = OMX_ErrorUndefined;
        i4Ret = copy_to_user(prUArg, (void *)&rKArg, sizeof(PARAMETER_OMX_VIDEO_PARAM_WMV));
        return i4Ret;
    }

    if (_prVDOOmxHandle[Omx_Handle->h_handle].fgUsed && (rKArg.pHandle == &_prVDOOmxHandle[Omx_Handle->h_handle]))
    {
        i4Ret = copy_from_user(&rVideoParamWmv, rKArg.pVideoParamWmv, sizeof(OMX_VIDEO_PARAM_WMVTYPE));

        if(rKArg.eDir == OMX_PARAM_SET)
        {
            if(rVideoParamWmv.eFormat == OMX_VIDEO_WMVFormat7)
            {
                _prVDOOmxHandle[Omx_Handle->h_handle].eCodecType = VC_WMV1;
                LOG(5, "%s(%d), OMX_VIDEO_WMVFormat7\n", __FUNCTION__, __LINE__);
            }
            else if(rVideoParamWmv.eFormat == OMX_VIDEO_WMVFormat8)
            {
                _prVDOOmxHandle[Omx_Handle->h_handle].eCodecType = VC_WMV2;
                LOG(5, "%s(%d), OMX_VIDEO_WMVFormat8\n", __FUNCTION__, __LINE__);
            }
            else if(rVideoParamWmv.eFormat == OMX_VIDEO_WMVFormat9)
            {
                _prVDOOmxHandle[Omx_Handle->h_handle].eCodecType = VC_WMV3;
                LOG(5, "%s(%d), OMX_VIDEO_WMVFormat9\n", __FUNCTION__, __LINE__);
            }
            else
            {
                _prVDOOmxHandle[Omx_Handle->h_handle].eCodecType = VC_VC1;
                LOG(5, "%s(%d), else -> VC1 \n", __FUNCTION__, __LINE__);
            }
            VOMX_Printf("[VDO][OMX] i4VDOOmxCompParamVideoWmv format(%d)\n", (UINT32)rVideoParamWmv.eFormat);
        }

        i4Ret = copy_to_user(rKArg.pVideoParamWmv, (void *)&rVideoParamWmv, sizeof(OMX_VIDEO_PARAM_WMVTYPE));
    }
    else
    {
        omxRet = OMX_ErrorUndefined;
    }

    rKArg.eReturnValue = omxRet;
    i4Ret = copy_to_user(prUArg, (void *)&rKArg, sizeof(PARAMETER_OMX_VIDEO_PARAM_WMV));

    return i4Ret;
}


INT32 i4VDOOmxCompParamVideoPortFmt(PARAMETER_OMX_VIDEO_PARAM_PORTFORMATTYPE *prUArg)
{
    INT32 i4Ret = 0;
    OMX_ERRORTYPE omxRet = OMX_ErrorNone;
    PARAMETER_OMX_VIDEO_PARAM_PORTFORMATTYPE rVideoPortType;
    OMX_VIDEO_PARAM_PORTFORMATTYPE rVideoPortFmt;
    VDO_OMX_HANDLE_T *Omx_Handle;
    VCONV_SET_PARAM_T SetParam;
    VDEC_SET_INTO_T rVdecSetInfo;

    i4Ret = copy_from_user(&rVideoPortType, prUArg, sizeof(PARAMETER_OMX_VIDEO_PARAM_PORTFORMATTYPE));

    Omx_Handle = rVideoPortType.pHandle;
    if ((Omx_Handle == NULL) || (i4Ret != 0))
    {
        VOMX_Printf("%s(L:%d) Incorrect Handle.\n", __FUNCTION__, __LINE__);
        rVideoPortType.eReturnValue = OMX_ErrorUndefined;
        i4Ret = copy_to_user(prUArg, (void *)&rVideoPortType, sizeof(PARAMETER_OMX_VIDEO_PARAM_PORTFORMATTYPE));
        return i4Ret;
    }

    //if (_prVDOOmxHandle[0].fgUsed && (rVideoPortType.pHandle == &_prVDOOmxHandle[0]))
    {
        i4Ret = copy_from_user(&rVideoPortFmt, rVideoPortType.pVideoPortFmt, sizeof(OMX_VIDEO_PARAM_PORTFORMATTYPE));

        if (OMX_PARAM_SET == rVideoPortType.eDir)
        {
            // Set Parameter
            //VOMX_Printf("[VDO][OMX] set OMX_IndexParamPortDefinition, not processed.\n");
            x_memset(&SetParam, 0, sizeof(VCONV_SET_PARAM_T));
            SetParam.u4PortNum = rVideoPortFmt.nPortIndex;
            if((rVideoPortFmt.nPortIndex)%2 == 0)
            {
                VOMX_Printf("[VDO][OMX] input compression format(%d).\n",
                    (UINT32)rVideoPortFmt.eCompressionFormat);
            }
            else if((rVideoPortFmt.nPortIndex)%2 == 1)
            {
                VOMX_Printf("[VDO][OMX] output color format(%d).\n",
                    (UINT32)rVideoPortFmt.eColorFormat);
            }
            else
            {
                VOMX_Printf("[VDO][OMX] output color format unsupported(%d).\n",
                    (UINT32)rVideoPortFmt.eColorFormat);
                return OMX_ErrorUndefined;
            }

            if((rVideoPortFmt.nPortIndex)%2 == 1)
            {
                x_memset(&rVdecSetInfo, 0, sizeof(VDEC_SET_INTO_T));
                if(rVideoPortFmt.eColorFormat == OMX_COLOR_Format32bitARGB8888)
                {
                    SetParam.eVConvFormat = VCONV_COLOR_ARBG8888;
                    rVdecSetInfo.fgVPushDecodeOnly = TRUE;
                }
                else if(rVideoPortFmt.eColorFormat == OMX_COLOR_FormatYUV420Planar)
                {
                    SetParam.eVConvFormat = VCONV_COLOR_YUV420;
                    rVdecSetInfo.fgVPushDecodeOnly = TRUE;
                }
                else if(rVideoPortFmt.eColorFormat == OMX_COLOR_FormatYUV420SemiPlanar)
                {
                    SetParam.eVConvFormat = VCONV_COLOR_YUV420SemiPlanar;
                    rVdecSetInfo.fgVPushDecodeOnly = TRUE;
                }
                else if(rVideoPortFmt.eColorFormat == OMX_COLOR_FormatVendorStartUnused)
                {
                    SetParam.eVConvFormat = VCONV_COLOR_B2R2;
                    rVdecSetInfo.fgVPushDecodeOnly = FALSE;
                }
                else
                {
                    VOMX_Printf("[VDO][OMX] output color format unsupported(%d).\n",
                        (UINT32)rVideoPortFmt.eColorFormat);
                    return OMX_ErrorUndefined;
                }
                _prVDOOmxHandle[Omx_Handle->h_handle].eColorFormatIndex0 =
                    rVideoPortFmt.eColorFormat;
                UNUSED(_VPUSH_SetInfo(_prVDOOmxHandle[Omx_Handle->h_handle].pVdecPush, &rVdecSetInfo));
                i4Ret = _i4VConv_SetParam(Omx_Handle->h_handle, &SetParam);
            }
        }
        else
        {
            // Get Parameter
            //VOMX_Printf("[VDO][OMX] get OMX_IndexParamPortDefinition, part implemented.\n");

            if ((rVideoPortFmt.nPortIndex)%2 == 0)
            {
                rVideoPortFmt.eColorFormat = OMX_COLOR_FormatUnused;
                if(rVideoPortFmt.nIndex >= 5)
                {
                    //VOMX_Printf("[VDO][OMX] VConv output color format index(%d) no more.\n",
                    //    (UINT32)rVideoPortFmt.nIndex);
                    omxRet = OMX_ErrorNoMore;
                }
                else
                {
                    switch(_prVDOOmxHandle[Omx_Handle->h_handle].eCodecType)
                    {
                    case VC_MPEG2:
                        rVideoPortFmt.eCompressionFormat = OMX_VIDEO_CodingMPEG2;
                        break;
                    case VC_H263:
                    case VC_H263_SORENSON:
                        rVideoPortFmt.eCompressionFormat = OMX_VIDEO_CodingH263;
                        break;
                    case VC_MPEG4:
                        rVideoPortFmt.eCompressionFormat = OMX_VIDEO_CodingMPEG4;
                        break;
                    case VC_WMV1:
                    case VC_WMV2:
                    case VC_WMV3:
                    case VC_VC1:
                        rVideoPortFmt.eCompressionFormat = OMX_VIDEO_CodingWMV;
                        break;
                    case VC_RV30:
                    case VC_RV40:
                        rVideoPortFmt.eCompressionFormat = OMX_VIDEO_CodingRV;
                        break;
                    case VC_H264:
                        rVideoPortFmt.eCompressionFormat = OMX_VIDEO_CodingAVC;
                        break;
                    case VC_MJPG:
                        rVideoPortFmt.eCompressionFormat = OMX_VIDEO_CodingMJPEG;
                        break;
    #ifdef VDEC_IS_POST_MT5368
                    case VC_VP6:
                        rVideoPortFmt.eCompressionFormat = OMX_VIDEO_CodingVP6;
                        break;
                    case VC_VP8:
                        rVideoPortFmt.eCompressionFormat = OMX_VIDEO_CodingVP8;
                        break;
    #endif
                    default:
                        rVideoPortFmt.eCompressionFormat = OMX_VIDEO_CodingUnused;
                        omxRet = OMX_ErrorUndefined;
                        break;
                    }
                }
            }
            else
            {
                rVideoPortFmt.eCompressionFormat = OMX_VIDEO_CodingUnused;
                if(rVideoPortFmt.nIndex == 0)
                {
                    rVideoPortFmt.eColorFormat = _prVDOOmxHandle[Omx_Handle->h_handle].eColorFormatIndex0;
                }
                else if(rVideoPortFmt.nIndex == 1)
                {
                    rVideoPortFmt.eColorFormat = OMX_COLOR_Format32bitARGB8888;
                }
                else if(rVideoPortFmt.nIndex == 2)
                {
                    rVideoPortFmt.eColorFormat = OMX_COLOR_FormatYUV420SemiPlanar;
                }
                else if(rVideoPortFmt.nIndex == 3)
                {
                    rVideoPortFmt.eColorFormat = OMX_COLOR_FormatVendorStartUnused;
                }
                else if(rVideoPortFmt.nIndex == 4)
                {
                    rVideoPortFmt.eColorFormat = OMX_COLOR_FormatYUV420Planar;
                }
                else
                {
                    //VOMX_Printf("[VDO][OMX] VConv output color format index(%d) no more.\n",
                    //    (UINT32)rVideoPortFmt.nIndex);
                    omxRet = OMX_ErrorNoMore;
                }
            }
        }

        i4Ret = copy_to_user(rVideoPortType.pVideoPortFmt, (void *)&rVideoPortFmt, sizeof(OMX_VIDEO_PARAM_PORTFORMATTYPE));
    }
    /*
    else
    {
        omxRet = OMX_ErrorUndefined;
    }
    */

    rVideoPortType.eReturnValue = omxRet;
    i4Ret = copy_to_user(prUArg, (void *)&rVideoPortType, sizeof(PARAMETER_OMX_VIDEO_PARAM_PORTFORMATTYPE));

    return i4Ret;
}


INT32 i4VDOOmxCompParamVideoSeamlessFmt(PARAMETER_OMX_VIDEO_PARAM_SEAMLESSFORMATTYPE *prUArg)
{
    INT32 i4Ret = 0;
    OMX_ERRORTYPE omxRet = OMX_ErrorNone;
    PARAMETER_OMX_VIDEO_PARAM_SEAMLESSFORMATTYPE rVideoSeamlessType;
    OMX_SEAMLESSFORMATTYPE rVideoSeamlessFmt;
    VDO_OMX_HANDLE_T *Omx_Handle;

    i4Ret = copy_from_user(&rVideoSeamlessType, prUArg, sizeof(PARAMETER_OMX_VIDEO_PARAM_SEAMLESSFORMATTYPE));

    Omx_Handle = rVideoSeamlessType.pHandle;
    if ((Omx_Handle == NULL) || (i4Ret != 0))
    {
        VOMX_Printf("%s(L:%d) Incorrect Handle.\n", __FUNCTION__, __LINE__);
        rVideoSeamlessType.eReturnValue = OMX_ErrorUndefined;
        i4Ret = copy_to_user(prUArg, (void *)&rVideoSeamlessType, sizeof(PARAMETER_OMX_VIDEO_PARAM_SEAMLESSFORMATTYPE));
        return i4Ret;
    }

    //if (_prVDOOmxHandle[0].fgUsed && (rVideoPortType.pHandle == &_prVDOOmxHandle[0]))
    {
        i4Ret = copy_from_user(&rVideoSeamlessFmt, rVideoSeamlessType.pVideoSeamlessFmt, sizeof(OMX_SEAMLESSFORMATTYPE));

        if (OMX_PARAM_SET == rVideoSeamlessType.eDir)
        {
            // call VDEC here
            VOID_VPUSH_SetSeamlessInfo(
                _prVDOOmxHandle[Omx_Handle->h_handle].pVdecPush,
                rVideoSeamlessFmt.bFlagSeamless,
                rVideoSeamlessFmt.nFrameWidth,
                rVideoSeamlessFmt.nFrameHeight);
                        
            LOG(5, "%s, width(%d), height(%d), stride(%d)\n", __FUNCTION__,
                rVideoSeamlessFmt.nFrameWidth,
                rVideoSeamlessFmt.nFrameHeight,
                rVideoSeamlessFmt.nStride);
        }
        else
        {
        }

        i4Ret = copy_to_user(rVideoSeamlessType.pVideoSeamlessFmt, (void *)&rVideoSeamlessFmt, sizeof(OMX_SEAMLESSFORMATTYPE));
    }
    /*
    else
    {
        omxRet = OMX_ErrorUndefined;
    }
    */

    rVideoSeamlessType.eReturnValue = omxRet;
    i4Ret = copy_to_user(prUArg, (void *)&rVideoSeamlessType, sizeof(PARAMETER_OMX_VIDEO_PARAM_SEAMLESSFORMATTYPE));

    return i4Ret;
}


BOOL i4VDOOmxCompImgFrmDone(UINT32 u4Tag, VOID* pvData)
{
    INT32 i4Ret = 0;
    VDO_OMX_HANDLE_T *pHandle;
    PARAMETER_OMX_CB_EVENTHANDLER rEvent;
    VDEC_IMG_FRMDONE_DATA* prCBData;
    OMX_BUFFERHEADERTYPE* prCbHeader;
    VDEC_IMG_FRMDONE_DATA *pImgFrmDone;

    pHandle = (VDO_OMX_HANDLE_T*)u4Tag;
    prCBData = (VDEC_IMG_FRMDONE_DATA*)pvData;
    if(!pHandle)
    {
        LOG(0, "i4VDOOmxCompImgFrmDone pHandle null\n");
        return FALSE;
    }
    prCbHeader = &pHandle->rCbHeader;
    // callback for flush done
    x_memset((void*)&rEvent, 0, sizeof(PARAMETER_OMX_CB_EVENTHANDLER));
    rEvent.eEvent     = OMX_EventCmdComplete;
    rEvent.nData1     = OMX_Command_Mtk_Used;
    rEvent.nData2     = OMX_IMAGE_Command_FrameDone;

    if(pvData)
    {
        if(!_bIsThumbnailImg)
        {
            //pixel-based
            prCBData->u2DisplayedStartX = _rSrcRegion.u4X << _u4CoordinateScale;
            prCBData->u2DisplayedStartY = _rSrcRegion.u4Y << _u4CoordinateScale;
            prCBData->u2ExpandedWidth = _u4XCoordinate;
            prCBData->u2ExpandedHeight= _u4YCoordinate;
        }
        else
        {
            if(prCbHeader->pPlatformPrivate)
            {
                //x_kmem_destroy_handle(prCbHeader->pPlatformPrivate);
                LOG(0, "i4VDOOmxCompImgFrmDone pPlatformPrivate re-alloc?\n");
            }
            if(prCbHeader->pAppPrivate)
            {
                //x_kmem_destroy_handle(prCbHeader->pAppPrivate);
                LOG(0, "i4VDOOmxCompImgFrmDone pAppPrivate re-alloc?\n");
            }
           rEvent.pEventData =
                (OMX_PTR)x_kmem_create_handle((VOID*)prCbHeader, sizeof(OMX_BUFFERHEADERTYPE));

            x_memset((void*)prCbHeader, 0, sizeof(OMX_BUFFERHEADERTYPE));
            prCbHeader->nAllocLen = (prCBData->u2OutputWidth * prCBData->u2OutputHeight) << 2;  //argb format
            prCbHeader->pAppPrivate = rEvent.pEventData;
            prCbHeader->pPlatformPrivate =
                (OMX_PTR)x_kmem_create_handle((VOID*)prCBData->pBuffer, prCbHeader->nAllocLen);
            //free by set parameter
        }
        if(_bIsThumbnailImg < 2)
        {
            pImgFrmDone = _arImgCBData;
            pImgFrmDone += (UINT32)_bIsThumbnailImg;
            x_memcpy((void*)pImgFrmDone, pvData, sizeof(VDEC_IMG_FRMDONE_DATA));
        }
        else
        {
            LOG(0, "_bIsThumbnailImg >= 2 error\n");
        }
        LOG(0, "\n");
        LOG(0, "#######################\n");
        LOG(0, "#  Frame-done Signal  #\n");
        LOG(0, "#######################\n\n");

        LOG(0, "u2DisplayedStartX=%d\n", prCBData->u2DisplayedStartX);
        LOG(0, "u2DisplayedStartY=%d\n", prCBData->u2DisplayedStartY);
        LOG(0, "u2ExpandedWidth=%d\n", prCBData->u2ExpandedWidth);
        LOG(0, "u2ExpandedHeight=%d\n", prCBData->u2ExpandedHeight);
        LOG(0, "u2OriginalWidth=%d\n", prCBData->u2OriginalWidth);
        LOG(0, "u2OriginalHeight=%d\n", prCBData->u2OriginalHeight);
        LOG(0, "u2OutputWidth=%d\n", prCBData->u2OutputWidth);
        LOG(0, "u2OutputHeight=%d\n", prCBData->u2OutputHeight);
       LOG(0, "pBuffer handle=0x%x, physical addr 0x%x\n",
            (UINT32)prCBData->pBuffer, PHYSICAL((UINT32)(prCBData->pBuffer)));
        if(rEvent.pEventData)
        {
            LOG(0, "i4VDOOmxCompImgFrmDone buf data  %x %x %x %x, nAllocLen %d\n",
                *prCBData->pBuffer,
                *(prCBData->pBuffer + 1),
                *(prCBData->pBuffer + 2),
                *(prCBData->pBuffer + 3), prCbHeader->nAllocLen);
        }
    }
    i4VDOOmxCompNotifyEvent(pHandle, &rEvent);
    return i4Ret;
}

INT32 i4VDOOmxCompConfigPictureMode(PARAMETER_OMX_IMAGE_MODE* prUArg)
{
    OMX_ERRORTYPE omxRet = OMX_ErrorNone;
    VDO_OMX_HANDLE_T *Omx_Handle;
    PARAMETER_OMX_IMAGE_MODE rKArg;
    INT32 i4Ret = 0;
    OMX_IMAGE_MODE rImageMode;
    UCHAR ucEsId = 0;
    //UINT32 u4Type = 0xFF;
    VDEC_IMG_FRMDONE_DATA *pImgFrmDone;

    i4Ret = copy_from_user(&rKArg, prUArg, sizeof(PARAMETER_OMX_IMAGE_MODE));
    Omx_Handle = rKArg.pHandle;
    i4Ret = copy_from_user(&rImageMode, rKArg.pImageMode, sizeof(OMX_IMAGE_MODE));
    
    //ucEsId = ((VDEC_T*)(_prVDOOmxHandle[Omx_Handle->h_handle].pVdecPush))->ucEsId;
    _bIsThumbnailImg = (BOOL)(rImageMode.nMode == OMX_IMAGE_THUMBNAIL) ? 1 : 0;
    VDEC_SetMMParam(ucEsId, VDEC_MM_PIC_DEC_IS_THUMBNAIL, _bIsThumbnailImg, 0, 0);
    
    //reset static image info
    pImgFrmDone = _arImgCBData;
    pImgFrmDone += (UINT32)FULL_IMAGE;
    x_memset((void*)pImgFrmDone, 0, sizeof(VDEC_IMG_FRMDONE_DATA));
    pImgFrmDone = _arImgCBData;
    pImgFrmDone += (UINT32)THUMBNAIL_IMAGE;
    x_memset((void*)&pImgFrmDone, 0, sizeof(VDEC_IMG_FRMDONE_DATA));
    if(!_bIsThumbnailImg)   //full image
    {
        _u4CoordinateScale = 0;
        if(VDP_GetScalerPixelBased())
        {
            _rSrcRegion.u4Width = OUTPUT_RES_X;
            _rSrcRegion.u4Height = OUTPUT_RES_Y;
        }
        else
        {
            _rSrcRegion.u4Width = 10000;
            _rSrcRegion.u4Height = 10000;
        }
        _rSrcRegion.u4X = 0;
        _rSrcRegion.u4Y = 0;
        VDP_SetSrcRegion(0, 0, _rSrcRegion);
    }    
    rKArg.eReturnValue = omxRet;
    i4Ret = copy_to_user(prUArg, (void *)&rKArg, sizeof(PARAMETER_OMX_IMAGE_MODE));

    return i4Ret;
}

INT32 i4VDOOmxCompConfigClearImageFrame(PARAMETER_OMX_IMAGE_PROCESSING* prUArg)
{
    OMX_ERRORTYPE omxRet = OMX_ErrorNone;
    VDO_OMX_HANDLE_T *Omx_Handle;
    PARAMETER_OMX_IMAGE_PROCESSING rKArg;
    INT32 i4Ret = 0;
    OMX_IMAGE_PROCESSING rImageProcessing;
    UCHAR ucEsId = 0;
    //UINT32 u4Type = 0xFF;

    i4Ret = copy_from_user(&rKArg, prUArg, sizeof(PARAMETER_OMX_IMAGE_PROCESSING));
    Omx_Handle = rKArg.pHandle;
    i4Ret = copy_from_user(&rImageProcessing, rKArg.pImageProcessing, sizeof(OMX_IMAGE_PROCESSING));

    //ucEsId = ((VDEC_T*)(_prVDOOmxHandle[Omx_Handle->h_handle].pVdecPush))->ucEsId;
    VDEC_SetMMParam(ucEsId, VDEC_MM_PIC_CLEARFRAME, rImageProcessing.bClear, 0, 0);

    rKArg.eReturnValue = omxRet;
    i4Ret = copy_to_user(prUArg, (void *)&rKArg, sizeof(PARAMETER_OMX_IMAGE_PROCESSING));

    return i4Ret;
}

INT32 i4VDOOmxCompConfigCommonOutputSize(PARAMETER_OMX_FRAMESIZETYPE* prUArg)
{
    OMX_ERRORTYPE omxRet = OMX_ErrorNone;
    VDO_OMX_HANDLE_T *Omx_Handle;
    PARAMETER_OMX_FRAMESIZETYPE rKArg;
    INT32 i4Ret = 0;
    OMX_FRAMESIZETYPE rFrameSizeType;
    UCHAR ucEsId = 0;
    //UINT32 u4Type = 0xFF;

    i4Ret = copy_from_user(&rKArg, prUArg, sizeof(PARAMETER_OMX_FRAMESIZETYPE));
    Omx_Handle = rKArg.pHandle;
    i4Ret = copy_from_user(&rFrameSizeType, rKArg.pFrameSizeType, sizeof(OMX_FRAMESIZETYPE));

    if(rKArg.eDir == OMX_PARAM_SET)
    {
        VDEC_SetMMParam(ucEsId, VDEC_MM_PIC_OUTPUT_RESOLUTION, rFrameSizeType.nWidth, rFrameSizeType.nHeight, 0);
    }
    else    //get
    {
        if(_bIsThumbnailImg < 2)
        {
        rFrameSizeType.nWidth = _arImgCBData[_bIsThumbnailImg].u2OutputWidth;
        rFrameSizeType.nHeight = _arImgCBData[_bIsThumbnailImg].u2OutputHeight;
        }
        else
        {
            LOG(0, "_bIsThumbnailImg >= 2 error\n");
            rFrameSizeType.nWidth = 0;
            rFrameSizeType.nHeight = 0;
        }
    }

    i4Ret = copy_to_user(rKArg.pFrameSizeType, (void *)&rFrameSizeType, sizeof(OMX_FRAMESIZETYPE));
    rKArg.eReturnValue = omxRet;
    i4Ret = copy_to_user(prUArg, (void *)&rKArg, sizeof(PARAMETER_OMX_FRAMESIZETYPE));

    return i4Ret;
}

INT32 i4VDOOmxCompConfigCommonRotate(PARAMETER_OMX_CONFIG_ROTATIONTYPE* prUArg)
{
    OMX_ERRORTYPE omxRet = OMX_ErrorNone;
    VDO_OMX_HANDLE_T *Omx_Handle;
    PARAMETER_OMX_CONFIG_ROTATIONTYPE rKArg;
    INT32 i4Ret = 0;
    OMX_CONFIG_ROTATIONTYPE rRotationType;
    UCHAR ucEsId = 0;
    //UINT32 u4Type = 0xFF;

    i4Ret = copy_from_user(&rKArg, prUArg, sizeof(PARAMETER_OMX_CONFIG_ROTATIONTYPE));
    Omx_Handle = rKArg.pHandle;
    i4Ret = copy_from_user(&rRotationType, rKArg.pRotationType, sizeof(OMX_CONFIG_ROTATIONTYPE));

    //ucEsId = ((VDEC_T*)(_prVDOOmxHandle[Omx_Handle->h_handle].pVdecPush))->ucEsId;
    VDEC_SetMMParam(ucEsId, VDEC_MM_PIC_ROTATION, rRotationType.nRotation, 0, 0);

    rKArg.eReturnValue = omxRet;
    i4Ret = copy_to_user(prUArg, (void *)&rKArg, sizeof(PARAMETER_OMX_CONFIG_ROTATIONTYPE));

    return i4Ret;
}

INT32 i4VDOOmxCompConfigCommonScale(PARAMETER_OMX_CONFIG_SCALEFACTORTYPE* prUArg)
{
    OMX_ERRORTYPE omxRet = OMX_ErrorNone;
    VDO_OMX_HANDLE_T *Omx_Handle;
    PARAMETER_OMX_CONFIG_SCALEFACTORTYPE rKArg;
    INT32 i4Ret = 0;
    OMX_CONFIG_SCALEFACTORTYPE rScaleFactorType;
    UCHAR ucVdpId = 0;
    //UINT32 u4Type = 0xFF;
    //VDEC_IMG_FRMDONE_DATA rCBData;

    i4Ret = copy_from_user(&rKArg, prUArg, sizeof(PARAMETER_OMX_CONFIG_SCALEFACTORTYPE));
    Omx_Handle = rKArg.pHandle;
    i4Ret = copy_from_user(&rScaleFactorType, rKArg.pScaleFactorType, sizeof(OMX_CONFIG_SCALEFACTORTYPE));

    //ucEsId = ((VDEC_T*)(_prVDOOmxHandle[Omx_Handle->h_handle].pVdecPush))->ucEsId;
    switch(rScaleFactorType.xWidth >> 16)
    {
        case 1:
            _u4CoordinateScale = 0;
            break;
        case 2:
            _u4CoordinateScale = 1;
            break;
        case 4:
            _u4CoordinateScale = 2;
            break;
        default:
            LOG(0, "Parameter Error !!, Enlarge value %d\n", rScaleFactorType.xWidth >> 16);
            _u4CoordinateScale = 0;
            break;
    }
     _u4XCoordinate = OUTPUT_RES_X << _u4CoordinateScale;
     _u4YCoordinate = OUTPUT_RES_Y << _u4CoordinateScale;

    if(VDP_GetScalerPixelBased())
    {
        _rSrcRegion.u4Width = OUTPUT_RES_X / (rScaleFactorType.xWidth >> 16);
        _rSrcRegion.u4Height = OUTPUT_RES_Y / (rScaleFactorType.xHeight >> 16);
        _rSrcRegion.u4X = (OUTPUT_RES_X - _rSrcRegion.u4Width) >> 1;
        _rSrcRegion.u4Y = (OUTPUT_RES_Y - _rSrcRegion.u4Height) >> 1;
    }
    else
    {
        _rSrcRegion.u4Width = 10000 / (rScaleFactorType.xWidth >> 16);
        _rSrcRegion.u4Height = 10000 / (rScaleFactorType.xHeight >> 16);
        _rSrcRegion.u4X = (10000 - _rSrcRegion.u4Width) >> 1;
        _rSrcRegion.u4Y = (10000 - _rSrcRegion.u4Height) >> 1;
    }
    VDP_SetSrcRegion(ucVdpId, 0, _rSrcRegion);
    LOG(0, "SetSrcRegion %d, %d, %d, %d\n", _rSrcRegion.u4X, _rSrcRegion.u4Y, _rSrcRegion.u4Width, _rSrcRegion.u4Height);

    //callback if full picture already decoded
    if((_arImgCBData[FULL_IMAGE].u2OriginalWidth > 0) && (_arImgCBData[FULL_IMAGE].u2OriginalHeight > 0))
    {
        _bIsThumbnailImg = FALSE;
       i4VDOOmxCompImgFrmDone((UINT32)Omx_Handle, (VOID*)&_arImgCBData[FULL_IMAGE]);
    }
    
    rKArg.eReturnValue = omxRet;
    i4Ret = copy_to_user(prUArg, (void *)&rKArg, sizeof(PARAMETER_OMX_CONFIG_SCALEFACTORTYPE));
    
    return i4Ret;
}

INT32 i4VDOOmxCompConfigCommonOutputPosition(PARAMETER_OMX_CONFIG_POINTTYPE* prUArg)
{
    OMX_ERRORTYPE omxRet = OMX_ErrorNone;
    VDO_OMX_HANDLE_T *Omx_Handle;
    PARAMETER_OMX_CONFIG_POINTTYPE rKArg;
    INT32 i4Ret = 0;
    OMX_CONFIG_POINTTYPE rPointType;
    //UCHAR ucEsId = 0;
    //UINT32 u4Type = 0xFF;

    i4Ret = copy_from_user(&rKArg, prUArg, sizeof(PARAMETER_OMX_CONFIG_POINTTYPE));
    Omx_Handle = rKArg.pHandle;
    i4Ret = copy_from_user(&rPointType, rKArg.pPointType, sizeof(OMX_CONFIG_POINTTYPE));

    if(rKArg.eDir == OMX_PARAM_SET)
    {
        _rSrcRegion.u4X = rPointType.nX >> _u4CoordinateScale;
        _rSrcRegion.u4Y = rPointType.nY >> _u4CoordinateScale;
        
        if(!VDP_GetScalerPixelBased())
        {
            _rSrcRegion.u4X = (_rSrcRegion.u4X * 10000) /OUTPUT_RES_X;
            _rSrcRegion.u4Y = (_rSrcRegion.u4Y * 10000) /OUTPUT_RES_Y;
        }
        VDP_SetSrcRegion(0, 0, _rSrcRegion);
        LOG(0, "SetSrcRegion %d, %d, %d, %d\n", _rSrcRegion.u4X, _rSrcRegion.u4Y, _rSrcRegion.u4Width, _rSrcRegion.u4Height);

        //callback
       i4VDOOmxCompImgFrmDone((UINT32)Omx_Handle, (VOID*)&_arImgCBData[FULL_IMAGE]);
    }
    else    //OMX_PARAM_GET
    {
        rPointType.nX =  _arImgCBData[FULL_IMAGE].u2DisplayedStartX;
        rPointType.nY =  _arImgCBData[FULL_IMAGE].u2DisplayedStartY;
    }

    i4Ret = copy_to_user(rKArg.pPointType, (void *)&rPointType, sizeof(OMX_CONFIG_POINTTYPE));
    rKArg.eReturnValue = omxRet;
    i4Ret = copy_to_user(prUArg, (void *)&rKArg, sizeof(PARAMETER_OMX_CONFIG_POINTTYPE));

    return i4Ret;
}


INT32 i4VDOOmxCompParamExpandedSize(PARAMETER_OMX_FRAMESIZETYPE* prUArg)
{
    OMX_ERRORTYPE omxRet = OMX_ErrorNone;
    VDO_OMX_HANDLE_T *Omx_Handle;
    PARAMETER_OMX_FRAMESIZETYPE rKArg;
    INT32 i4Ret = 0;
    OMX_FRAMESIZETYPE rFrameSizeType;

    i4Ret = copy_from_user(&rKArg, prUArg, sizeof(PARAMETER_OMX_FRAMESIZETYPE));
    Omx_Handle = rKArg.pHandle;
    i4Ret = copy_from_user(&rFrameSizeType, rKArg.pFrameSizeType, sizeof(OMX_FRAMESIZETYPE));

    //get
    rFrameSizeType.nWidth = _arImgCBData[FULL_IMAGE].u2ExpandedWidth;
    rFrameSizeType.nHeight = _arImgCBData[FULL_IMAGE].u2ExpandedHeight;

    i4Ret = copy_to_user(rKArg.pFrameSizeType, (void *)&rFrameSizeType, sizeof(OMX_FRAMESIZETYPE));
    rKArg.eReturnValue = omxRet;
    i4Ret = copy_to_user(prUArg, (void *)&rKArg, sizeof(PARAMETER_OMX_FRAMESIZETYPE));

    return i4Ret;
}


INT32 i4VDOOmxCompParamOriginalSize(PARAMETER_OMX_FRAMESIZETYPE* prUArg)
{
    OMX_ERRORTYPE omxRet = OMX_ErrorNone;
    VDO_OMX_HANDLE_T *Omx_Handle;
    PARAMETER_OMX_FRAMESIZETYPE rKArg;
    INT32 i4Ret = 0;
    OMX_FRAMESIZETYPE rFrameSizeType;

    i4Ret = copy_from_user(&rKArg, prUArg, sizeof(PARAMETER_OMX_FRAMESIZETYPE));
    Omx_Handle = rKArg.pHandle;
    i4Ret = copy_from_user(&rFrameSizeType, rKArg.pFrameSizeType, sizeof(OMX_FRAMESIZETYPE));

    //get
    rFrameSizeType.nWidth = _arImgCBData[FULL_IMAGE].u2OriginalWidth;
    rFrameSizeType.nHeight = _arImgCBData[FULL_IMAGE].u2OriginalHeight;

    i4Ret = copy_to_user(rKArg.pFrameSizeType, (void *)&rFrameSizeType, sizeof(OMX_FRAMESIZETYPE));
    rKArg.eReturnValue = omxRet;
    i4Ret = copy_to_user(prUArg, (void *)&rKArg, sizeof(PARAMETER_OMX_FRAMESIZETYPE));

    return i4Ret;
}


INT32 i4VDOOmxCompParamImageFreeBuffer(PARAMETER_OMX_PARAM_IMAGE_FREEBUFFER* prUArg)
{
    OMX_ERRORTYPE omxRet = OMX_ErrorNone;
    VDO_OMX_HANDLE_T *Omx_Handle;
    PARAMETER_OMX_PARAM_IMAGE_FREEBUFFER rKArg;
    INT32 i4Ret = 0;
    OMX_BUFFERHEADERTYPE rBufHeader;

    i4Ret = copy_from_user(&rKArg, prUArg, sizeof(PARAMETER_OMX_PARAM_IMAGE_FREEBUFFER));
    Omx_Handle = rKArg.pHandle;
    i4Ret = copy_from_user(&rBufHeader, rKArg.pBufferHeaderType, sizeof(OMX_BUFFERHEADERTYPE));

    if(rBufHeader.pBuffer != Omx_Handle->rCbHeader.pBuffer)
    {
        LOG(0, "i4VDOOmxCompParamImageFreeBuffer pBuffer mismatch!!param 0x%x, save 0x%x\n",
            rBufHeader.pBuffer,
            Omx_Handle->rCbHeader.pBuffer);
        omxRet = OMX_ErrorBadParameter;
    }
    else
    {
        x_kmem_destroy_handle((HANDLE_T)Omx_Handle->rCbHeader.pPlatformPrivate);
        x_kmem_destroy_handle((HANDLE_T)Omx_Handle->rCbHeader.pAppPrivate);
        Omx_Handle->rCbHeader.pPlatformPrivate = NULL;
        Omx_Handle->rCbHeader.pAppPrivate = NULL;
        Omx_Handle->rCbHeader.pBuffer = NULL;
    }
    
    rKArg.eReturnValue = omxRet;
    i4Ret = copy_to_user(prUArg, (void *)&rKArg, sizeof(PARAMETER_OMX_CONFIG_ROTATIONTYPE));

    return i4Ret;
}

INT32 i4VDOOmxCompParamImageUsrDefineData(PARAMETER_OMX_PARAM_IMAGE_USER_DEFINED* prUArg)
{
    OMX_ERRORTYPE omxRet = OMX_ErrorNone;
    VDO_OMX_HANDLE_T *Omx_Handle;
    PARAMETER_OMX_PARAM_IMAGE_USER_DEFINED rKArg;
    INT32 i4Ret = 0;
    OMX_IMAGE_USER_DEFINED rImageUsrDefineData;
    UCHAR ucEsId = 0;
    //UINT32 u4Type = 0xFF;

    i4Ret = copy_from_user(&rKArg, prUArg, sizeof(PARAMETER_OMX_PARAM_IMAGE_USER_DEFINED));
    Omx_Handle = rKArg.pHandle;
    i4Ret = copy_from_user(&rImageUsrDefineData, rKArg.pImageUserDefined, sizeof(OMX_IMAGE_USER_DEFINED));

    if(rKArg.eDir == OMX_PARAM_SET)
    {
        VDEC_SetMMParam(ucEsId, VDEC_MM_PIC_FILESIZE, rImageUsrDefineData.nUserDefined, 0, 0);
    }
    else    //get
    {
        rImageUsrDefineData.nUserDefined = 0;
    }

    i4Ret = copy_to_user(rKArg.pImageUserDefined, (void *)&rImageUsrDefineData, sizeof(OMX_IMAGE_USER_DEFINED));
    rKArg.eReturnValue = omxRet;
    i4Ret = copy_to_user(prUArg, (void *)&rKArg, sizeof(PARAMETER_OMX_PARAM_IMAGE_USER_DEFINED));

    return i4Ret;
}

INT32 i4VDOOmxCompParamTimeScale(PARAMETER_OMX_TIME_CONFIG_SCALETYPE* prUArg)
{
    OMX_ERRORTYPE omxRet = OMX_ErrorNone;
    VDO_OMX_HANDLE_T *Omx_Handle;
    PARAMETER_OMX_TIME_CONFIG_SCALETYPE rKArg;
    INT32 i4Ret = 0;
    OMX_TIME_CONFIG_SCALETYPE   rScaleType;
    UCHAR ucEsId = 0;
    INT16 i2Speed;
    UINT32 u4Speed;

    i4Ret = copy_from_user(&rKArg, prUArg, sizeof(PARAMETER_OMX_TIME_CONFIG_SCALETYPE));
    Omx_Handle = rKArg.pHandle;
    i4Ret = copy_from_user(&rScaleType, rKArg.pScaleType, sizeof(OMX_TIME_CONFIG_SCALETYPE));

    i2Speed = (INT16)(rScaleType.xScale >> 16);
    
    LOG(0,"passing speed1 = %d !!!!!!!!!!!!\n",i2Speed);
    
    switch(rScaleType.xScale)
    {
    		case 131072: //2x
    			u4Speed = STC_SPEED_TYPE_FORWARD_2X;
    			break;
    		case 98304: //1.5x
    			u4Speed = STC_SPEED_TYPE_FORWARD_1_DOT_5X;
    			break;	
    		default:
    			u4Speed = STC_SPEED_TYPE_FORWARD_1X;
					break;    			
    }
    VDP_SetSpeed(0,u4Speed);
    
    //i2Speed /= 10;
    
    LOG(0,"passing speed2 = %d !!!!!!!!!!!!\n",i2Speed);
    	
    switch(i2Speed)
    {
    case 0:
    case 1:
    case 2:
        u4Speed = STC_SPEED_TYPE_FORWARD_1X;
        break;
    case 4:
        u4Speed = STC_SPEED_TYPE_FORWARD_4X;
        break;
    case 8:
        u4Speed = STC_SPEED_TYPE_FORWARD_8X;
        break;
    case 16:
        u4Speed = STC_SPEED_TYPE_FORWARD_16X;
        break;
    case 32:
        u4Speed = STC_SPEED_TYPE_FORWARD_32X;
        break;
    default:
        if(i2Speed < 0)
        {
            u4Speed = STC_SPEED_TYPE_REWIND_1X;
        }
        else
        {
            u4Speed = STC_SPEED_TYPE_FORWARD_1X;
        }
        break;
    }
    if(rKArg.eDir == OMX_PARAM_SET)
    {
        VDEC_SetMMParam(ucEsId, VDEC_MM_PLAY_SPEED, u4Speed, 0, 0);
    }
    else    //get
    {
        // not implement
    }

    i4Ret = copy_to_user(rKArg.pScaleType, (void *)&rScaleType, sizeof(OMX_TIME_CONFIG_SCALETYPE));
    rKArg.eReturnValue = omxRet;
    i4Ret = copy_to_user(prUArg, (void *)&rKArg, sizeof(PARAMETER_OMX_TIME_CONFIG_SCALETYPE));

    return i4Ret;
}

#endif

