

#include "vdec_drvif.h"
#include "vdec_push.h"
#include "fbm_drvif.h"

#include "imgrz_if.h"
#include "imgrz_drvif.h"

#include "dmx_mm_if.h"
#include "img_lib_if.h"

LINT_EXT_HEADER_BEGIN
#include "vdec_debug.h"
#include "x_hal_5381.h"
#include "x_assert.h"
#include "x_os.h"
#include "x_mid.h"
LINT_EXT_HEADER_END

static BOOL _fgPushInit = FALSE;
VDEC_DECODER_T _rVdecPush = {0};
static BOOL _fgFirstVideoChunk = TRUE;

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define CHECK_FALSE_RET(x) \
          do { \
            if (x == FALSE) { \
                LOG(3, "%s(%d) ret FALSE\n", __FUNCTION__, __LINE__); \
                return FALSE; \
            } \
          } while (0)

#define CHECK_FALSE(x) \
          do { \
            if (x == FALSE) { \
                LOG(3, "%s(%d) ret FALSE\n", __FUNCTION__, __LINE__); \
            } \
          } while (0)

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

//static HANDLE_T _hBufSem = NULL_HANDLE;


//-----------------------------------------------------------------------------
// Function prototype
//-----------------------------------------------------------------------------

VOID _VPUSH_PushLoop(VOID* pvArg);
void _VPUSH_SendCmd(VOID* prdec, UINT32 u4Cmd);

#if 0
//-----------------------------------------------------------------------------
/** Enter buffer critical section
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _BufLock(void)
{
    static BOOL _fgInit = FALSE;

    if (!_fgInit)
    {
        VERIFY(x_sema_create(&_hBufSem, X_SEMA_TYPE_MUTEX, X_SEMA_STATE_UNLOCK) == OSR_OK);
        _fgInit = TRUE;
    }

    VERIFY(x_sema_lock(_hBufSem, X_SEMA_OPTION_WAIT) == OSR_OK);
}

//-----------------------------------------------------------------------------
/** Leave buffer API critical section
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _BufUnlock(void)
{
    VERIFY(x_sema_unlock(_hBufSem) == OSR_OK);
}
#endif

static void _VPUSH_VDEC_Nfy(
    void*               pvNfyTag,
    VDEC_DEC_COND_T      eNfyCond,
    UINT32              u4Data1,
    UINT32              u4Data2)
{
    VDEC_T *prVdec;
    prVdec = (VDEC_T*)pvNfyTag;    
    if(eNfyCond == VDEC_COND_CTRL_DONE)
    {
        LOG(5, "%s(%d): VDEC_COND_CTRL_DONE(%d)\n", __FUNCTION__, __LINE__, u4Data1);
        if(!prVdec)
        {
            LOG(3, "%s(%d): prVdec null\n", __FUNCTION__, __LINE__);
            return;
        }

        if(u4Data1 == 1)//(UINT32)VID_DEC_CTRL_STOP)
        {
            _VPUSH_SendCmd((VOID*)prVdec, (UINT32)VPUSH_CMD_STOP);
        }
        else
        {
            LOG(1, "%s(%d): u4Data1(%d) unknown\n", __FUNCTION__, __LINE__, u4Data1);
            return;
        }
    }
}

static UINT32 _VPUSH_GetMsgCountInQ(VOID* prdec)
{
    VDEC_T *prVdec;
    INT32   i4Ret;
    UINT16  u2MstCount = 0;

    if(!prdec)
    {
        LOG(3, "%s(%d): prdec null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    prVdec = (VDEC_T*)prdec;
    i4Ret = x_msg_q_num_msgs(prVdec->hMsgQ, &u2MstCount);

    if (i4Ret == OSR_NO_MSG)
    {
        u2MstCount = 0;
    }

    prVdec->u2MstCount = u2MstCount;
    return (u2MstCount);
}

BOOL _VPUSH_VideoCallback(const DMX_PES_MSG_T* prPes)
{
    VDEC_PES_INFO_T rPesInfo;
    UINT32 u4Addr;
    VDEC_T *prVdec;
    prVdec = (VDEC_T*)prPes->pvInstanceTag;

    x_memset(&rPesInfo, 0, sizeof(VDEC_PES_INFO_T));

    rPesInfo.ucEsId = (UCHAR)prVdec->ucEsId;
    if (prVdec->eFmt == VDEC_FMT_MJPEG)
    {
        rPesInfo.ucMpvId = VLD1; // video format all use VLD expect jpeg
    }
    else
    {
        rPesInfo.ucMpvId = VLD0; // video format all use VLD expect jpeg
    }
    rPesInfo.u4PTS = prPes->u4Pts;
    rPesInfo.u4DTS = prPes->u4Dts;
    rPesInfo.u4FifoStart = prPes->u4BufStart;
    rPesInfo.u4FifoEnd = prPes->u4BufEnd;
    rPesInfo.u4VldReadPtr = prPes->u4FrameAddr;
    rPesInfo.fgDtsValid = prPes->fgPtsDts;
    rPesInfo.fgSeqHdr = prPes->fgSeqHeader;
    rPesInfo.fgGop = prPes->fgGop;
    rPesInfo.fgEos = prPes->fgEOS;
    rPesInfo.u8Offset = 0;
    #ifdef CC_VDEC_FMT_DETECT
    rPesInfo.u4DmxFrameType = prPes->u4FrameType;
    #endif

    if(!rPesInfo.fgEos)
    {
        ASSERT((rPesInfo.u4VldReadPtr < rPesInfo.u4FifoEnd) &&
            (rPesInfo.u4VldReadPtr >= rPesInfo.u4FifoStart));
    }
    else
    {
        LOG(5, "%s(%d): fgEos\n", __FUNCTION__, __LINE__);
    }
    if (prVdec->eFmt == VDEC_FMT_MPV)
    {
        switch (prPes->u4FrameType)
        {
        case PIC_TYPE_I:
            rPesInfo.ucPicType = MPV_I_TYPE;
            break;
        case PIC_TYPE_P:
            rPesInfo.ucPicType = MPV_P_TYPE;
            break;
        case PIC_TYPE_B:
            rPesInfo.ucPicType = MPV_B_TYPE;
            break;
        case PIC_TYPE_SEQ_END:
            rPesInfo.ucPicType = MPV_I_TYPE;
            rPesInfo.fgSeqEnd = TRUE;
            break;
        default:
            rPesInfo.ucPicType = 0;
            #ifndef CC_VDEC_FMT_DETECT
            ASSERT(0);
            break;
            #else
            return TRUE;
            #endif
        }

    }
    else if(prVdec->eFmt == VDEC_FMT_H264)
    {
        u4Addr = prPes->u4FrameAddr + 3;
        if(u4Addr >= prPes->u4BufEnd)
        {
            u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
        }
        rPesInfo.ucPicType = *(UINT8*)VIRTUAL(u4Addr);

        u4Addr = prPes->u4FrameAddr + 4;
        if(u4Addr >= prPes->u4BufEnd)
        {
            u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
        }
        rPesInfo.u4VldReadPtr = u4Addr;
    }
    /*else if (prVdec->eFmt == VDEC_FMT_WMV)
    {
        rPesInfo.u4VideoType = (UINT32)DMX_VIDEO_VC1;
        rPesInfo.ucPicType = (UCHAR)prPes->u4FrameType;
    }*/
    else if(prVdec->eFmt == VDEC_FMT_MP4)
    {
        if (VDEC_PUSH_FMT_MP4 == prVdec->ePushFmt)
        {
            UCHAR *pucBuf;

            u4Addr = prPes->u4FrameAddr + 3;
            if(u4Addr >= prPes->u4BufEnd)
            {
                u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
            }
            pucBuf = (UCHAR*)VIRTUAL(u4Addr);

            if (pucBuf[0] == 0xB6)
            {
                u4Addr = prPes->u4FrameAddr + 4;
                if(u4Addr >= prPes->u4BufEnd)
                {
                    u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
                }
                pucBuf = (UCHAR*)VIRTUAL(u4Addr);
                switch (pucBuf[0] >> 6)
                {
                    case 0:
                        rPesInfo.ucPicType  = I_TYPE;
                        break;
                    case 1: case 3:
                        rPesInfo.ucPicType  = P_TYPE;
                        break;
                    case 2:
                        rPesInfo.ucPicType = B_TYPE;
                        break;
                    default:
                        rPesInfo.ucPicType = UNKNOWN_TYPE;
                        break;
                }
            }
            else if ((pucBuf[0] & 0x20) == 0x20)
            {
                rPesInfo.fgSeqHdr = TRUE;
            }
        }
        else if (VDEC_PUSH_FMT_DIVX311 == prVdec->ePushFmt)
        {
            UCHAR *pucBuf;

            rPesInfo.fgDtsValid = TRUE;
            u4Addr = prPes->u4FrameAddr;
            if(u4Addr >= prPes->u4BufEnd)
            {
                u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
            }
            pucBuf = (UCHAR*)VIRTUAL(u4Addr);

            if ((pucBuf[0] >> 6) & 0x1)
            {
                rPesInfo.ucPicType  = P_TYPE;
            }
            else
            {
                rPesInfo.ucPicType = I_TYPE;
            }
        }
    }
    else if(prVdec->eFmt == VDEC_FMT_AVS)
    {
        rPesInfo.u4VideoType = (UINT32)DMX_VIDEO_AVS;
        u4Addr = prPes->u4FrameAddr + 3;
        if(u4Addr >= prPes->u4BufEnd)
        {
            u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
        }
        rPesInfo.ucPicType = *(UINT8*)VIRTUAL(u4Addr);

        if (rPesInfo.ucPicType == 0xB3)
        {
        }
        else if (rPesInfo.ucPicType == 0xB0)
        {
            rPesInfo.fgSeqHdr = TRUE;
        }
    }
    else if (prVdec->eFmt == VDEC_FMT_MJPEG)
    {
        rPesInfo.ucPicType = I_TYPE;
    }
    else if (prVdec->eFmt == VDEC_FMT_RV)
    {
        UCHAR ucPicType = *(UCHAR *)VIRTUAL(prPes->u4FrameAddr);

        if (VDEC_PUSH_FMT_RV30 == prVdec->ePushFmt)
        {
            rPesInfo.ucPicType = (ucPicType >> 3) & 3;
        }
        else if (VDEC_PUSH_FMT_RV40 == prVdec->ePushFmt)
        {
            rPesInfo.ucPicType = (ucPicType >> 5) & 3;
        }
        else
        {
            LOG(0, "%s(%d): unknown subtype %d\n", __FUNCTION__, __LINE__, prVdec->ePushFmt);
            ASSERT(0);
            rPesInfo.ucPicType = 0;
        }
    }
    else if (prVdec->eFmt == VDEC_FMT_VP8)
    {
        UCHAR ucBuf;

        rPesInfo.fgDtsValid = TRUE;
        u4Addr = prPes->u4FrameAddr;
        if(u4Addr >= prPes->u4BufEnd)
        {
            u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
        }
        ucBuf = *(UCHAR*)VIRTUAL(u4Addr);

        if (ucBuf & 0x80)
        {
            rPesInfo.ucPicType = I_TYPE;
        }
        else
        {
            rPesInfo.ucPicType  = P_TYPE;
        }
    }
    else if(prVdec->eFmt == VDEC_FMT_WMV)
    {
        if(prVdec->ePushFmt == VDEC_PUSH_FMT_VC1)
        {
            rPesInfo.u4VideoType = DMX_VIDEO_VC1;
        }
        else if(prVdec->ePushFmt == VDEC_PUSH_FMT_WMV7)
        {
            rPesInfo.u4VideoType = DMX_VIDEO_WMV7;
        }
        else if(prVdec->ePushFmt == VDEC_PUSH_FMT_WMV8)
        {
            rPesInfo.u4VideoType = DMX_VIDEO_WMV8;
        }
        else if(prVdec->ePushFmt == VDEC_PUSH_FMT_WMV9)
        {
            rPesInfo.u4VideoType = DMX_VIDEO_WMV9;
        }

        rPesInfo.ucPicType = (UCHAR)prPes->u4FrameType;

        if(_fgFirstVideoChunk && prVdec->ePushFmt != VDEC_PUSH_FMT_VC1)
        {
            VDEC_SEQUENCE_DATA_T rVDecSeqData;
            UINT32 u4SequenceHeader;
            UINT16 u4Width = prVdec->u4Width;
            UINT16 u4Height = prVdec->u4Height;

            x_memcpy((VOID*)&u4SequenceHeader, (VOID*)VIRTUAL(prPes->u4FrameAddr),4);
            u4SequenceHeader = ((u4SequenceHeader&0x000000FF)<<24) | ((u4SequenceHeader&0x0000FF00)<<8) | ((u4SequenceHeader&0x00FF0000)>>8) | ((u4SequenceHeader&0xFF000000)>>24);

            rVDecSeqData.u2_width = (UINT16)u4Width;
            rVDecSeqData.u2_height = (UINT16)u4Height;
            rVDecSeqData.u4SequenceHeader = u4SequenceHeader;
            rVDecSeqData.u2SequenceHeaderSize = 4;
            rVDecSeqData.e_frame_rate = prVdec->eFrameRate;

            VDEC_SetMMParam(prVdec->ucEsId, VDEC_MM_SEQ_INFO, (UINT32)&rVDecSeqData, 0, 0);

            _fgFirstVideoChunk = FALSE;
            
            // megaa 20110719: fix WMV7 divide by zero problem
            // (For WMV7, the 1st data that DMX moves is not only sequence header, so we have to send ES here.)
            if (prVdec->ePushFmt == VDEC_PUSH_FMT_WMV7)
            {
                VDEC_SendEs((void*)&rPesInfo);
            }
            
            return TRUE;
        }
    }
    if((prPes->u4FrameAddr == 0) && (!prPes->fgEOS))
    {
        LOG(3, "MMCallback Addr 0!\n");
    }
    else
    {
        VDEC_SendEs((void*)&rPesInfo);
    }
    return TRUE;
}

BOOL _VPUSH_JpegCallback(const DMX_PES_MSG_T* prPes)
{
    VDEC_PES_INFO_T rPesInfo;
    UINT8 u1Marker1, u1Marker2;
    UINT32 u4Addr, u4DataSz1 = 0, u4DataSz2 = 0;
    VDEC_T* prVdec = (VDEC_T*)prPes->pvInstanceTag;
    static UINT32 _u4TotalDataSz = 0;
    
    if(prPes->u4FrameAddr < prPes->u4Wp)
    {
        u4DataSz1 = prPes->u4Wp - prPes->u4FrameAddr;
    }
    else
    {
        u4DataSz1 = prPes->u4BufEnd - prPes->u4FrameAddr;
        u4DataSz2 = prPes->u4Wp - prPes->u4BufStart;
    }

    //check image start
    u1Marker1 = *(UINT8*)VIRTUAL(prPes->u4FrameAddr);
    u4Addr = prPes->u4FrameAddr + 1;
    if(u4Addr == prPes->u4BufEnd)
    {
        u4Addr = prPes->u4BufStart;
    }
    u1Marker2 = *(UINT8*)VIRTUAL(u4Addr);
    if(((u1Marker1 == 0xFF) && (u1Marker2 == 0xD8))||((u1Marker1 == 0x89) && (u1Marker2 == 0x50)))
    {
        _u4TotalDataSz = 0; //reset
        prVdec->fgInputBufReady = FALSE;
    }
    _u4TotalDataSz = _u4TotalDataSz + (u4DataSz1 + u4DataSz2);

    //can change to dmx find start code
    if(prPes->fgEOS)
    {
        LOG(5, "--- Callback EOS\n");
    }
    //Fix me, keep picture on screen
    //if(((u1Marker1 == 0xFF) && (u1Marker2 == 0xD8))/* || prPes->fgEOS*/)
    if(((u1Marker1 == 0xFF) && (u1Marker2 == 0xD8))||((u1Marker1 == 0x89) && (u1Marker2 == 0x50)) /*|| prPes->fgEOS*/)        
    {
        x_memset(&rPesInfo, 0, sizeof(VDEC_PES_INFO_T));

        rPesInfo.ucEsId = (UCHAR)prVdec->ucEsId;
        rPesInfo.ucMpvId = VLD1; // video format all use VLD expect jpeg  ??? 
        rPesInfo.u4PTS = prPes->u4Pts;
        rPesInfo.u4DTS = prPes->u4Dts;
        rPesInfo.u4FifoStart = prPes->u4BufStart;
        rPesInfo.u4FifoEnd = prPes->u4BufEnd;
        rPesInfo.u4VldReadPtr = prPes->u4FrameAddr;
        rPesInfo.fgDtsValid = prPes->fgPtsDts;
        rPesInfo.fgSeqHdr = prPes->fgSeqHeader;
        rPesInfo.fgGop = prPes->fgGop;
        rPesInfo.fgEos = prPes->fgEOS;
        rPesInfo.u8Offset = 0;
        rPesInfo.ucPicType = I_TYPE;
        
        VDEC_SendEs((void*)&rPesInfo);
        //flush ES Q
        //if(!prPes->fgEOS)
        {
            rPesInfo.fgSeqEnd = TRUE;
            VDEC_SendEs((void*)&rPesInfo);
        }
    }

    if(!prVdec->fgInputBufReady)
    {
        if((_u4TotalDataSz >= 0x80000) || prPes->fgEOS)
        {
            IMG_SetFillBsBufEnd(_u4TotalDataSz, 0, prPes->fgEOS);
            prVdec->fgInputBufReady = TRUE;
        }
    }
    else
    {
        IMG_SetFillBsBufEnd(u4DataSz1, u4DataSz2, prPes->fgEOS);
    }
    
    return TRUE;
}

VOID _VPUSH_SetVideoFormat(VOID* prdec)
{
    VDEC_T *prVdec;
    if(!prdec)
    {
        LOG(3, "%s(%d): prdec null\n", __FUNCTION__, __LINE__);
        return;
    }
    prVdec = (VDEC_T*)prdec;

    // to support multiple video format, demux need change format before
    // we call move data.
    if(prVdec->fgEnDmx)
    {
        switch (prVdec->eFmt)
        {
        case VDEC_FMT_WMV:
            DMX_MUL_SetVideoType(prVdec->u1DmxId, DMX_VIDEO_VC1);
            break;
        case VDEC_FMT_MP4:
        case VDEC_FMT_MJPEG:
            DMX_MUL_SetVideoType(prVdec->u1DmxId, DMX_VIDEO_MPEG4);
            break;
        case VDEC_FMT_MPV:
            DMX_MUL_SetVideoType(prVdec->u1DmxId, DMX_VIDEO_MPEG);
            break;
        case VDEC_FMT_H264:
            DMX_MUL_SetVideoType(prVdec->u1DmxId, DMX_VIDEO_H264);
            break;
        case VDEC_FMT_AVS:
            DMX_MUL_SetVideoType(prVdec->u1DmxId, DMX_VIDEO_AVS);
            break;
        case VDEC_FMT_RV:
            DMX_MUL_SetVideoType(prVdec->u1DmxId, DMX_VIDEO_RV);
            break;
#ifdef VDEC_IS_POST_MT5368
        case VDEC_FMT_VP6:
            DMX_MUL_SetVideoType(prVdec->u1DmxId, DMX_VIDEO_VP6);
            break;
        case VDEC_FMT_VP8:
            DMX_MUL_SetVideoType(prVdec->u1DmxId, DMX_VIDEO_VP8);
            break;
#endif
        default:
            LOG(3, "%s(%d): format(%d) not support\n", __FUNCTION__, __LINE__, (UINT32)(prVdec->eFmt));
            DMX_MUL_SetVideoType(prVdec->u1DmxId, DMX_VIDEO_MPEG);
            break;
        }
    }
}


BOOL _VPUSH_AllocDmx(VOID* prdec)
{
    DMX_DECODER_CALLBACKS_T rDmxCallback;
    FBM_POOL_T* prFbmPool;
    DMX_MM_T rDmxMM;
    UINT32 u4Flag = 0;
    VDEC_T *prVdec;
    BOOL fgRet;

    if(!prdec)
    {
        LOG(3, "%s(%d): prdec null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    prVdec = (VDEC_T*)prdec;

    if(prVdec->fgEnDmx)
    {
        LOG(3, "%s(%d): (prVdec->fgEnDmx) already allocated\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

	#if defined(CC_VOMX_TV_COEXIST)
    prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_DMX2);
	#else
    prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_DMX);
	#endif

    if(prFbmPool == NULL)
    {
        LOG(3, "%s(%d): (prFbmPool == NULL)\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    if((prFbmPool->u4Addr == 0) || (prFbmPool->u4Size == 0))
    {
        LOG(3, "%s(%d): (prFbmPool->u4Addr == 0)\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    DMX_MUL_EnableMultipleInst(TRUE);
    
    prVdec->u1DmxId =
        DMX_MUL_GetAvailableInst(
            (DMX_CONN_TYPE_T)((UINT32)DMX_CONN_TYPE_BUFAGENT_2 +
                prVdec->ucEsId));
    if(prVdec->u1DmxId == 0xFF)
    {
        LOG(3, "%s(%d): DMX_MUL_GetAvailableInst fail\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    fgRet = DMX_MUL_SetInstType(prVdec->u1DmxId, DMX_IN_PLAYBACK_MM);
    if(!fgRet)
    {
        LOG(3, "%s(%d): DMX_MUL_SetDecoderCallbacks fail\n", __FUNCTION__, __LINE__);
        return fgRet;
    }

    _VPUSH_SetVideoFormat(prdec);
    DMX_SetToDecoder(TRUE);
    x_memset(&rDmxCallback, 0, sizeof(rDmxCallback));
    if ((prVdec->eFmt == VDEC_FMT_JPG)||(prVdec->eFmt == VDEC_FMT_PNG))
    {
        rDmxCallback.pfnSendVideoPes = _VPUSH_JpegCallback;
    }
    else
    {
        rDmxCallback.pfnSendVideoPes = _VPUSH_VideoCallback;
    }
    rDmxCallback.pfnUpdateVideoWp = VDEC_SetWptr;
    fgRet = DMX_MUL_SetDecoderCallbacks(prVdec->u1DmxId, &rDmxCallback);
    if(!fgRet)
    {
        LOG(3, "%s(%d): DMX_MUL_SetDecoderCallbacks fail\n", __FUNCTION__, __LINE__);
        return fgRet;
    }

    prVdec->u1DmxPid =
        DMX_MUL_GetAvailablePidx(prVdec->u1DmxId);
    if(prVdec->u1DmxPid >= DMX_NUM_PID_INDEX)
    {
        LOG(3, "%s(%d): (prVdec->u1DmxPid == DMX_MULTIPLE_NULL_PIDX)\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    x_memset(&rDmxMM, 0, sizeof(DMX_MM_T));

    u4Flag = (UINT32)(DMX_MM_FLAG_TYPE
        | DMX_MM_FLAG_INSTANCE_TAG
        | DMX_MM_FLAG_VALID
        | DMX_MM_FLAG_BUF
        | DMX_MM_FLAG_SEARCH_START_CODE
        );

    if ((VDEC_FMT_MPV == prVdec->eFmt)
    || (VDEC_FMT_H264 == prVdec->eFmt)
    || ((VDEC_FMT_WMV == prVdec->eFmt) && (prVdec->ePushFmt == VDEC_PUSH_FMT_VC1))
    || (VDEC_FMT_AVS == prVdec->eFmt)
    )
    {
        rDmxMM.fgSearchStartCode = TRUE;
    }
    else
    {
        rDmxMM.fgSearchStartCode = FALSE;
    }

    rDmxMM.fgEnable = TRUE;
    rDmxMM.ePidType = DMX_PID_TYPE_ES_VIDEO;
    rDmxMM.pvInstanceTag = (void*)prVdec;
    rDmxMM.fgAllocBuf = FALSE;
    rDmxMM.u4BufAddr = prFbmPool->u4Addr;
    rDmxMM.u4BufSize = prFbmPool->u4Size;
    rDmxMM.u1ChannelId = prVdec->ucEsId;
    fgRet = DMX_MM_Set(prVdec->u1DmxPid, u4Flag, &rDmxMM);
    if(!fgRet)
    {
        LOG(3, "%s(%d): DMX_MM_Set fail\n", __FUNCTION__, __LINE__);
        DMX_MM_Free(prVdec->u1DmxPid);
        DMX_MUL_FreePidx(prVdec->u1DmxPid);

        DMX_MUL_FreeInst(prVdec->u1DmxId);
        prVdec->u1DmxId = 0xFF;
        return FALSE;
    }
    prVdec->fgEnDmx = TRUE;
    return fgRet;
}

BOOL _VPUSH_ReleaseDmx(VOID* prdec)
{
    DMX_MM_T rDmxMM;
    VDEC_T *prVdec;
    if(!prdec)
    {
        LOG(3, "%s(%d): prdec null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    prVdec = (VDEC_T*)prdec;

    if(!prVdec->fgEnDmx)
    {
        LOG(3, "%s(%d): (!prVdec->fgEnDmx) not allocated\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    x_memset(&rDmxMM, 0, sizeof(DMX_MM_T));
    rDmxMM.fgEnable = FALSE;

    VERIFY(DMX_MM_Set(prVdec->u1DmxPid, DMX_MM_FLAG_VALID, &rDmxMM));
    DMX_MM_Free(prVdec->u1DmxPid);
    DMX_MUL_FreePidx(prVdec->u1DmxPid);

    DMX_MUL_FreeInst(prVdec->u1DmxId);
    prVdec->u1DmxId = 0xFF;
    
    prVdec->fgEnDmx = FALSE;
    return TRUE;
}


BOOL _VPUSH_Mpeg4SetDmx(VDEC_T *prVdec, BOOL fgSearchStartCode)
{
    DMX_DECODER_CALLBACKS_T rDmxCallback;
    FBM_POOL_T* prFbmPool;
    DMX_MM_T rDmxMM;
    UINT32 u4Flag = 0;
    BOOL fgRet;

    if (!prVdec)
    {
        LOG(3, "%s(%d): prVdec is null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    if (prVdec->fgEnDmx)
    {
        x_memset(&rDmxMM, 0, sizeof(DMX_MM_T));
        rDmxMM.fgEnable = FALSE;

        VERIFY(DMX_MM_Set(prVdec->u1DmxPid, DMX_MM_FLAG_VALID, &rDmxMM));
        DMX_MM_Free(prVdec->u1DmxPid);
        DMX_MUL_FreePidx(prVdec->u1DmxPid);
        //prVdec->fgEnDmx = FALSE;

        DMX_MUL_FreeInst(prVdec->u1DmxId);
        prVdec->u1DmxId = 0xFF;
    }

	#if defined(CC_VOMX_TV_COEXIST)
    prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_DMX2);
	#else
    prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_DMX);
	#endif
	
    if(prFbmPool == NULL)
    {
        LOG(3, "%s(%d): (prFbmPool == NULL)\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    if((prFbmPool->u4Addr == 0) || (prFbmPool->u4Size == 0))
    {
        LOG(3, "%s(%d): (prFbmPool->u4Addr == 0)\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    DMX_MUL_EnableMultipleInst(TRUE);

    prVdec->u1DmxId =
        DMX_MUL_GetAvailableInst(
            (DMX_CONN_TYPE_T)((UINT32)DMX_CONN_TYPE_BUFAGENT_2 +
                prVdec->ucEsId));
    if(prVdec->u1DmxId == 0xFF)
    {
        LOG(3, "%s(%d): DMX_MUL_GetAvailableInst fail\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    fgRet = DMX_MUL_SetInstType(prVdec->u1DmxId, DMX_IN_PLAYBACK_MM);
    if(!fgRet)
    {
        LOG(3, "%s(%d): DMX_MUL_SetDecoderCallbacks fail\n", __FUNCTION__, __LINE__);
        return fgRet;
    }

    _VPUSH_SetVideoFormat((VOID *)prVdec);
    DMX_SetToDecoder(TRUE);
    x_memset(&rDmxCallback, 0, sizeof(rDmxCallback));
    if ((prVdec->eFmt == VDEC_FMT_JPG)||(prVdec->eFmt == VDEC_FMT_PNG))
    {
        rDmxCallback.pfnSendVideoPes = _VPUSH_JpegCallback;
    }
    else
    {
        rDmxCallback.pfnSendVideoPes = _VPUSH_VideoCallback;
    }
    rDmxCallback.pfnUpdateVideoWp = VDEC_SetWptr;
    fgRet = DMX_MUL_SetDecoderCallbacks(prVdec->u1DmxId, &rDmxCallback);
    if(!fgRet)
    {
        LOG(3, "%s(%d): DMX_MUL_SetDecoderCallbacks fail\n", __FUNCTION__, __LINE__);
        return fgRet;
    }

    prVdec->u1DmxPid =
        DMX_MUL_GetAvailablePidx(prVdec->u1DmxId);
    if(prVdec->u1DmxPid >= DMX_NUM_PID_INDEX)
    {
        LOG(3, "%s(%d): (prVdec->u1DmxPid == DMX_MULTIPLE_NULL_PIDX)\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    u4Flag = (UINT32)(DMX_MM_FLAG_TYPE
        | DMX_MM_FLAG_INSTANCE_TAG
        | DMX_MM_FLAG_VALID
        | DMX_MM_FLAG_BUF
        | DMX_MM_FLAG_SEARCH_START_CODE
        );

    x_memset(&rDmxMM, 0, sizeof(DMX_MM_T));
    rDmxMM.fgSearchStartCode = fgSearchStartCode;
    rDmxMM.fgEnable = TRUE;
    rDmxMM.ePidType = DMX_PID_TYPE_ES_VIDEO;
    rDmxMM.pvInstanceTag = (void*)prVdec;
    rDmxMM.fgAllocBuf = FALSE;
    rDmxMM.u4BufAddr = prFbmPool->u4Addr;
    rDmxMM.u4BufSize = prFbmPool->u4Size;
    rDmxMM.u1ChannelId = prVdec->ucEsId;
    fgRet = DMX_MM_Set(prVdec->u1DmxPid, u4Flag, &rDmxMM);
    if(!fgRet)
    {
        LOG(3, "%s(%d): DMX_MM_Set fail\n", __FUNCTION__, __LINE__);
        DMX_MM_Free(prVdec->u1DmxPid);
        DMX_MUL_FreePidx(prVdec->u1DmxPid);

        DMX_MUL_FreeInst(prVdec->u1DmxId);
        prVdec->u1DmxId = 0xFF;
        return FALSE;
    }

    prVdec->fgEnDmx = TRUE;

    return TRUE;
}


#define H264_STARTCODE_LEN 3
#define MOVEPSSIZE (1024)


BOOL _VPUSH_DmxMoveData(VOID* prdec, DMX_MM_DATA_T *prDmxMMData)
{
    VDEC_T *prVdec;
    BOOL fgRet;

    prVdec = (VDEC_T*)prdec;
    if(!prdec)
    {
        LOG(3, "%s(%d): prdec null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    HalFlushInvalidateDCacheMultipleLine(prDmxMMData->u4StartAddr,
        (UINT32)(prDmxMMData->u4FrameSize));

    // TODO: lock sema for multi-instance support
    _VPUSH_SetVideoFormat(prdec);
    fgRet = DMX_MUL_MoveData(prVdec->u1DmxId, prDmxMMData);
    return fgRet;
}

BOOL _VPUSH_MoveVC1FirstData(VOID* prdec, VDEC_BYTES_INTO_T *prBytesInfo)
{
    DMX_MM_DATA_T rDmxMMData;
    VDEC_T *prVdec;
    BOOL fgRet;
    static UCHAR* pucHdrBufAlloc = 0;
    UINT32 u4Tmp = 0x0D010000;
 
    if(!prdec)
    {
        LOG(3, "%s(%d): prdec null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    prVdec = (VDEC_T*)prdec;


    if(!pucHdrBufAlloc)
    {
        pucHdrBufAlloc = (UCHAR*)VIRTUAL((UINT32)BSP_AllocAlignedDmaMemory(MOVEPSSIZE,16));
        pucHdrBufAlloc[0] = 0x0;
        pucHdrBufAlloc[1] = 0x0;
        pucHdrBufAlloc[2] = 0x1;
        pucHdrBufAlloc[3] = 0xD;
    }

    x_memcpy((VOID*)(VIRTUAL((UINT32)&pucHdrBufAlloc[0])),(VOID*)&u4Tmp,4);
     
    //insert start code
    x_memset(&rDmxMMData, 0, sizeof(DMX_MM_DATA_T));
    rDmxMMData.u1Idx = prVdec->u1DmxPid;
    rDmxMMData.u4BufStart = (UINT32)pucHdrBufAlloc;
    rDmxMMData.u4BufEnd = (UINT32)(pucHdrBufAlloc + MOVEPSSIZE);
    rDmxMMData.u4StartAddr = (UINT32)pucHdrBufAlloc;
    rDmxMMData.u4FrameSize = 4;
    rDmxMMData.fgEOS = prBytesInfo->fgEos;
    rDmxMMData.u4Pts = prBytesInfo->u4BytesPTS;
    rDmxMMData.u4Dts = prBytesInfo->u4BytesPTS;
    rDmxMMData.fgFrameHead= TRUE;

    if (_VPUSH_DmxMoveData(prVdec, &rDmxMMData)  != TRUE)
    {
        LOG(1, "Demuxer fails to move sequence header. 1\n");
        fgRet = FALSE;
    }

    UNUSED(fgRet);
    return TRUE;
}

BOOL _VPUSH_Move264FirstData(VOID* prdec, VDEC_BYTES_INTO_T *prBytesInfo)
{
    DMX_MM_DATA_T rDmxMMData;
    VDEC_T *prVdec;
    BOOL fgRet;
    UCHAR* pucHdrBuf;
    UINT8 u1PSNum;
    UINT8 u1NumOfPS;
    UINT16 u2PSLength;
    UINT32 i;
    static UCHAR* pucHdrBufAlloc = 0;

    if(!prdec)
    {
        LOG(3, "%s(%d): prdec null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    prVdec = (VDEC_T*)prdec;


    if(!pucHdrBufAlloc)
    {
        pucHdrBufAlloc = (UCHAR*)VIRTUAL((UINT32)BSP_AllocAlignedDmaMemory(MOVEPSSIZE,16));
        pucHdrBufAlloc[0] = 0;
        pucHdrBufAlloc[1] = 0;
        pucHdrBufAlloc[2] = 0;
        pucHdrBufAlloc[3] = 1;
    }
    pucHdrBuf = (UCHAR*)VIRTUAL(prBytesInfo->u4BytesAddr);
    pucHdrBuf += 4; //skip unused info
    prVdec->u1StartCodeLenMinus1 = (*pucHdrBuf++) & 0x3;
    for(u1PSNum = 0; u1PSNum < 2; u1PSNum++)    // 0 for SPS, 1 for PPS
    {
        if(u1PSNum == 0)
        {
            u1NumOfPS = (*pucHdrBuf++) & 0x1f;
        }
        else
        {
            u1NumOfPS = *pucHdrBuf++;
        }
        for (i = 0; i < u1NumOfPS; i++)
        {
            u2PSLength = (UINT16)(*pucHdrBuf++);
            u2PSLength <<= 8;
            u2PSLength += (UINT16)(*pucHdrBuf++);

            //insert start code
            x_memset(&rDmxMMData, 0, sizeof(DMX_MM_DATA_T));
            rDmxMMData.u1Idx = prVdec->u1DmxPid;
            rDmxMMData.u4BufStart = (UINT32)pucHdrBufAlloc;
            rDmxMMData.u4BufEnd = (UINT32)(pucHdrBufAlloc + MOVEPSSIZE);
            rDmxMMData.u4StartAddr = (UINT32)pucHdrBufAlloc;
            rDmxMMData.u4FrameSize = 4;
            rDmxMMData.fgEOS = prBytesInfo->fgEos;
            rDmxMMData.u4Pts = prBytesInfo->u4BytesPTS;
            rDmxMMData.u4Dts = prBytesInfo->u4BytesPTS;

            if (_VPUSH_DmxMoveData(prVdec, &rDmxMMData)  != TRUE)
            {
                LOG(1, "Demuxer fails to move sequence header. 1\n");
                fgRet = FALSE;
            }

            //copy SPS or PPS
            //set move data parameter
            x_memset(&rDmxMMData, 0, sizeof(DMX_MM_DATA_T));
            rDmxMMData.u1Idx = prVdec->u1DmxPid;
            rDmxMMData.u4BufStart = prVdec->u4FifoStart;
            rDmxMMData.u4BufEnd = prVdec->u4FifoEnd;
            rDmxMMData.u4StartAddr = (UINT32)pucHdrBuf;
            rDmxMMData.u4FrameSize = u2PSLength;
            rDmxMMData.fgEOS = prBytesInfo->fgEos;
            rDmxMMData.u4Pts = prBytesInfo->u4BytesPTS;
            rDmxMMData.u4Dts = prBytesInfo->u4BytesPTS;

            if (_VPUSH_DmxMoveData(prVdec, &rDmxMMData)  != TRUE)
            {
                LOG(1, "Demuxer fails to move sequence header. 2\n");
                fgRet = FALSE;
            }
            pucHdrBuf += u2PSLength;
        }
    }
    UNUSED(fgRet);
    return TRUE;
}

BOOL _VPUSH_Move264Data(VOID* prdec, VDEC_BYTES_INTO_T *prBytesInfo)
{
    DMX_MM_DATA_T rDmxMMData;
    VDEC_T *prVdec;
    UCHAR *pucFdrBuf;
    UCHAR *pucBuf;
    UINT32 u4NaluSize;
    BOOL fgRet;
    UINT32 u4FrmSize;
    UINT32 i;

    if(!prdec)
    {
        LOG(3, "%s(%d): prdec null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    prVdec = (VDEC_T*)prdec;
    pucFdrBuf = (UCHAR*)prBytesInfo->u4BytesAddr;
    pucBuf = pucFdrBuf;
    u4FrmSize = 0;
    fgRet = TRUE;

    while(u4FrmSize < prBytesInfo->u4BytesSize)
    {
        u4NaluSize = 0;
        //insert start code
        for(i = 0; i < prVdec->u1StartCodeLenMinus1; i++)
        {
            u4NaluSize += (UINT32)*pucFdrBuf;
            u4NaluSize <<= 8;
            *pucFdrBuf++ = 0;
        }
        u4NaluSize += (UINT32)*pucFdrBuf;
        if (u4NaluSize == 0)
        {
            // move data and leave
            rDmxMMData.u1Idx = prVdec->u1DmxPid;
            rDmxMMData.u4BufStart = prVdec->u4FifoStart;
            rDmxMMData.u4BufEnd = prVdec->u4FifoEnd;
            rDmxMMData.u4StartAddr = prBytesInfo->u4BytesAddr + 4;
            rDmxMMData.u4FrameSize = prBytesInfo->u4BytesSize - 4;
            rDmxMMData.fgEOS = prBytesInfo->fgEos;
            rDmxMMData.u4Pts = prBytesInfo->u4BytesPTS;
            rDmxMMData.u4Dts = prBytesInfo->u4BytesPTS;

            fgRet = _VPUSH_DmxMoveData(prVdec, &rDmxMMData);
            return fgRet;
        }

        *pucFdrBuf++ = 1;

        //if((prSwdmxMp4Info->u1StartCodeLenMinus1 < H264_STARTCODE_LEN) &&
        //    (prSwdmxMp4Info->u1StartCodeLenMinus1 > 0))
        {
            UINT32   u4Loop = 0;
            UINT32   u4DataSize = 0;


            for(u4Loop=0; u4Loop<(H264_STARTCODE_LEN - prVdec->u1StartCodeLenMinus1); u4Loop++)
            {
                pucBuf--;
                *pucBuf = 0;
            }

            u4DataSize = u4NaluSize + (H264_STARTCODE_LEN -
                                prVdec->u1StartCodeLenMinus1)+
                                (prVdec->u1StartCodeLenMinus1 + 1);

            rDmxMMData.u1Idx = prVdec->u1DmxPid;
            rDmxMMData.u4BufStart = prVdec->u4FifoStart;
            rDmxMMData.u4BufEnd = prVdec->u4FifoEnd;
            rDmxMMData.u4StartAddr = (UINT32)pucBuf;
            rDmxMMData.u4FrameSize = u4DataSize;
            rDmxMMData.fgEOS = prBytesInfo->fgEos;
            rDmxMMData.u4Pts = prBytesInfo->u4BytesPTS;
            rDmxMMData.u4Dts = prBytesInfo->u4BytesPTS;

            fgRet = _VPUSH_DmxMoveData(prVdec, &rDmxMMData);
        }

        // nalu size
        u4FrmSize += ((prVdec->u1StartCodeLenMinus1 + 1) + u4NaluSize);
        pucFdrBuf += u4NaluSize;
        pucBuf = pucFdrBuf;
    }
    return fgRet;
}


// parse rv codec  opaque data
UINT32 _au4VpushRprSizeArray[16];
VOID _VPUSH_RvOpaqueParse(VDEC_T *prVdec, VDEC_BYTES_INTO_T *prBytesInfo)
{
#define HX_GET_MAJOR_VERSION(prodVer) (((prodVer) >> 28) & 0xF)
#define HX_GET_MINOR_VERSION(prodVer) (((prodVer) >> 20) & 0xFF)

#define RV30_MAJOR_BITSTREAM_VERSION  3
#define RV30_BITSTREAM_VERSION        2

#define RV40_MAJOR_BITSTREAM_VERSION  4
#define RV40_BITSTREAM_VERSION        0

    UINT8 *pu1Data;
    UINT32 ui4_spo_extra_flags = 0;
    UINT32 ui4_version = 0;
    UINT32 ui4_major_ver;
    UINT32 ui4_minor_ver;
    UINT32 ui4_num_rpr_size = 0;
    BOOL fgIsRv9 = TRUE;

    if (!prVdec)
    {
        LOG(0, "%s(%d): prVdec is null\n", __FUNCTION__, __LINE__);
        ASSERT(prVdec);
        return;
    }

    if (!prBytesInfo)
    {
        LOG(0, "%s(%d): prBytesInfo is null\n", __FUNCTION__, __LINE__);
        ASSERT(prBytesInfo);
        return;
    }

    pu1Data = (UINT8 *)VIRTUAL(prBytesInfo->u4BytesAddr);
    VDEC_LOADB_DWRD(pu1Data, ui4_spo_extra_flags);
    VDEC_LOADB_DWRD(pu1Data + 4, ui4_version);

    ui4_major_ver = HX_GET_MAJOR_VERSION(ui4_version);
    ui4_minor_ver = HX_GET_MINOR_VERSION(ui4_version);

    prVdec->ePushFmt = VDEC_PUSH_FMT_RV40;
    if (!(ui4_minor_ver & 0x80))
    {
        if (ui4_major_ver == RV30_MAJOR_BITSTREAM_VERSION
            && ui4_minor_ver == RV30_BITSTREAM_VERSION)
        {
            UINT8* pbBasePtr = pu1Data + 8;
            UINT32 i;

            prVdec->ePushFmt = VDEC_PUSH_FMT_RV30;
            fgIsRv9 = FALSE;
            _au4VpushRprSizeArray[0] = prVdec->u4Width;
            _au4VpushRprSizeArray[1] = prVdec->u4Height;
            ui4_num_rpr_size = (ui4_spo_extra_flags >> 16) & 7;
            if (ui4_num_rpr_size < 8)
            {
                if (prBytesInfo->u4BytesSize >= (ui4_num_rpr_size * 2 + 8))
                {
                    for (i = 0; i < ui4_num_rpr_size; i++)
                    {
                        _au4VpushRprSizeArray[2 * i + 2] = (UINT32)(*pbBasePtr << 2); 
                        pbBasePtr++;
                        _au4VpushRprSizeArray[2 * i + 3] = (UINT32)(*pbBasePtr << 2); 
                        pbBasePtr++;
                    }
                }
                else
                {
                    LOG(0, "%s(%d): opaque data(%d) is too small!! rpr size(%d)\n", 
                        __FUNCTION__, __LINE__, prBytesInfo->u4BytesSize, ui4_num_rpr_size);
                    ASSERT(prBytesInfo->u4BytesSize >= (ui4_num_rpr_size + 8));
                    goto PRS_OPAQUE_RTN;
                }
            }
        }
        else if (ui4_major_ver == RV40_MAJOR_BITSTREAM_VERSION
            && ui4_minor_ver == RV40_BITSTREAM_VERSION)
        {
            fgIsRv9 = TRUE;
        }
        else
        {
            LOG(0, "%s(%d): ui4_minor_ver(0x%08x) and ui4_minor_ver(0x%08x) are incorrect\n",
                __FUNCTION__, __LINE__, ui4_major_ver, ui4_minor_ver);
            ASSERT(0);
            goto PRS_OPAQUE_RTN;
        }
    }
    else
    {
        LOG(0, "%s(%d): ui4_minor_ver(0x%08x) is incorrect\n", __FUNCTION__, __LINE__, ui4_minor_ver);
        ASSERT(0);
        goto PRS_OPAQUE_RTN;
    }

    if (!fgIsRv9 && ui4_num_rpr_size > 0)  // It will be 0 for RV9.  For RV8, add the count of native size.
    {
        ui4_num_rpr_size++;
    }

PRS_OPAQUE_RTN:
    VDEC_SetMMParam(prVdec->ucEsId, VDEC_MM_RV_RPR_INFO, ui4_num_rpr_size, (UINT32)_au4VpushRprSizeArray, fgIsRv9);
}


/*
    ffmpeg RM data header format
    1byte: slice number - 1 (should be 0~126)
    slice number * 8byte(1, 0, 0, 0, 4b_slice_ofst_le)

    TODO: clarify what does 1, 0, 0, 0 mean
*/
BOOL _VPUSH_RvPreparse(VDEC_BYTES_INTO_T *prBytesInfo)
{
    UINT8 *pu1Data = (UINT8 *)VIRTUAL(prBytesInfo->u4BytesAddr);
    UINT32 *pu4Data = (UINT32 *)pu1Data;
    UINT32 u4SliceNum = (UINT32)*pu1Data + 1;
    UINT32 u4BytesSize = prBytesInfo->u4BytesSize;
    UINT32 i, j, u4SliceOfst = 0, u4NextSliceOfst = 0, u4HeaderSz;

    u4HeaderSz = u4SliceNum * 8 + 1;
    if (u4BytesSize <= u4HeaderSz)
    {
        LOG(0, "%s(%d): %d slice, push data size(%d) is too small!!\n",
            __FUNCTION__, __LINE__, u4SliceNum, u4BytesSize);
        ASSERT(0);
        return FALSE;
    }

    for (i = 5, j = 1; j < u4SliceNum; i += 8, j++)
    {
        VDEC_LOADL_DWRD(&pu1Data[i], u4SliceOfst);
        VDEC_LOADL_DWRD(&pu1Data[i + 8], u4NextSliceOfst);
        pu4Data[j] = u4NextSliceOfst - u4SliceOfst;
    }
    pu1Data[0] = (UINT8)u4SliceNum;

    if (u4BytesSize <= (u4NextSliceOfst + u4HeaderSz))
    {
        LOG(0, "%s(%d): push data size(%d) is too small!!\n",
            __FUNCTION__, __LINE__, u4BytesSize);
        ASSERT(0);
        return FALSE;
    }
    pu4Data[u4SliceNum] = u4BytesSize - u4NextSliceOfst - u4HeaderSz;

    return TRUE;
}


VOID _VPUSH_Mpeg4Config(VDEC_T *prVdec, VDEC_BYTES_INTO_T *prBytesInfo)
{
#define M4V_START_CODE			0x000001	
#define H263_VIDEO_START_MASK            	0xfffffc00
#define H263_VIDEO_START_CODE          	0x00008000
#define FLV_VIDEO_START_CODE   0x00008400

    UINT32 u4Temp;
    VDEC_LOADB_DWRD((UINT8 *)VIRTUAL(prBytesInfo->u4BytesAddr), u4Temp);

    if ((u4Temp >> 8) == M4V_START_CODE)
    {
        LOG(0, "FMT_MP4\n");
        _VPUSH_SetPushType(prVdec, VDEC_PUSH_FMT_MP4);
        _VPUSH_Mpeg4SetDmx(prVdec, TRUE);
    }
    else if ((u4Temp  & H263_VIDEO_START_MASK) == H263_VIDEO_START_CODE)
    {
        LOG(0, "FMT_H263\n");
        _VPUSH_SetPushType(prVdec, VDEC_PUSH_FMT_H263);
        _VPUSH_Mpeg4SetDmx(prVdec, FALSE);
    }
    else if ((u4Temp  & H263_VIDEO_START_MASK) == FLV_VIDEO_START_CODE)
    {
        LOG(0, "FMT_SORENSEN\n");
        _VPUSH_SetPushType(prVdec, VDEC_PUSH_FMT_SORENSEN);
        _VPUSH_Mpeg4SetDmx(prVdec, FALSE);
    }
    // Because the biCompression is not fixed for DivX311
    // we can't use any start code to specify the video type for DivX311.
    else //if (u4Temp == DX311_VIDEO_START_CODE)
    {
        LOG(0, "FMT_DIVX311\n");
        _VPUSH_SetPushType(prVdec, VDEC_PUSH_FMT_DIVX311);
        _VPUSH_Mpeg4SetDmx(prVdec, FALSE);
    }
}


/*
 * mkv demuxer will insert sequence header before each block
 * but a MPEG picture might be divided into multiple block
 * This behavior will cause vfifo has improper data
 */ 
//#define MKV_INSERT_MPEG_SEQ_HDR_SW_WORKAROUND
#ifdef MKV_INSERT_MPEG_SEQ_HDR_SW_WORKAROUND
#define MPEG_SEQ_LEN (144) // max mpeg1/2 seq len is 140
UINT8 _u1MpegSeq[MPEG_SEQ_LEN];
UINT32 _u4MpegSeqLen;
#endif
BOOL _fgInsertStartcode = FALSE;
BOOL _VPUSH_MoveData(VOID* prdec, VDEC_BYTES_INTO_T *prBytesInfo)
{
    DMX_MM_DATA_T rDmxMMData;
    VDEC_T *prVdec;
    BOOL fgRet;
    int dummy_data[4];
    UCHAR *ptr;

    if(!prdec)
    {
        LOG(3, "%s(%d): prdec null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    prVdec = (VDEC_T*)prdec;

    if(!prBytesInfo)
    {
        LOG(3, "%s(%d): prBytesInfo null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    if(prBytesInfo->fgEos)
    {
        DMX_MM_T rDmxMM;
        x_memset(&rDmxMM, 0, sizeof(DMX_MM_T));
        rDmxMM.fgSearchStartCode = FALSE;
        fgRet = DMX_MM_Set(prVdec->u1DmxPid, DMX_MM_FLAG_SEARCH_START_CODE, &rDmxMM);
        LOG(5, "vpush got fgEos\n");
    }
    else
    {
        LOG(9, "vpush: size=%d, addr=0x%08x, pts=0x%x\n", prBytesInfo->u4BytesSize, prBytesInfo->u4BytesAddr, prBytesInfo->u4BytesPTS);
    }

    if (VDEC_FMT_H264 == prVdec->eFmt)
    {
        if ((!prVdec->fgNonFirst) && (!_fgInsertStartcode))
        {
            ptr = (UCHAR*)prBytesInfo->u4BytesAddr;
            if(((ptr[0] == 0x00) && (ptr[1] == 0x00) && (ptr[2] == 0x01)) ||
               ((ptr[0] == 0x00) && (ptr[1] == 0x00) && (ptr[2] == 0x00) && (ptr[3] == 0x01)))
            {
            }
            else
            {
                _fgInsertStartcode = TRUE;
            }
        }

        if(_fgInsertStartcode)
        {
            if (!prVdec->fgNonFirst)
            {
                prVdec->fgNonFirst = TRUE;
                return _VPUSH_Move264FirstData(prVdec, prBytesInfo);
            }
            else
            {
                return _VPUSH_Move264Data(prVdec, prBytesInfo);
            }
        }
    }
    else if(prVdec->eFmt == VDEC_FMT_WMV && prVdec->ePushFmt == VDEC_PUSH_FMT_VC1)
    {
        if(prVdec->fgNonFirst)
        {
            if (!_fgInsertStartcode)
        {
            ptr = (UCHAR*)prBytesInfo->u4BytesAddr;
                if(((ptr[0] == 0x00) && (ptr[1] == 0x00) && (ptr[2] == 0x01)) ||
                   ((ptr[0] == 0x00) && (ptr[1] == 0x00) && (ptr[2] == 0x00) && (ptr[3] == 0x01)))
            {
                }
                else
                {
                    _fgInsertStartcode = TRUE;
                }
            }

            if(_fgInsertStartcode)
        {
        if (!prVdec->fgNonFirst)
        {
            prVdec->fgNonFirst = TRUE;
                    _VPUSH_MoveVC1FirstData(prVdec, prBytesInfo);
                }
                else
                {
                    _VPUSH_MoveVC1FirstData(prVdec, prBytesInfo);
                }
            }      
        }
    }
    else if (VDEC_FMT_MP4 == prVdec->eFmt)
    {
        if (!prVdec->fgNonFirst)
        {
            VDEC_SEQUENCE_DATA_T rVDecSeqData;
            x_memset(&rVDecSeqData, 0, sizeof(rVDecSeqData));
            rVDecSeqData.u2_width = (UINT16)prVdec->u4Width;
            rVDecSeqData.u2_height = (UINT16)prVdec->u4Height;
            rVDecSeqData.e_frame_rate = prVdec->eFrameRate;
            VDEC_SetMMParam(prVdec->ucEsId, VDEC_MM_SEQ_INFO, (UINT32)&rVDecSeqData, 0, 0);
            _VPUSH_Mpeg4Config(prVdec, prBytesInfo);
            prVdec->fgNonFirst = TRUE;
        }
    }
    else if (VDEC_FMT_RV == prVdec->eFmt)
    {
        if (!prVdec->fgNonFirst)
        {
            VDEC_SEQUENCE_DATA_T rVDecSeqData;
            x_memset(&rVDecSeqData, 0, sizeof(rVDecSeqData));
            rVDecSeqData.u2_width = (UINT16)prVdec->u4Width;
            rVDecSeqData.u2_height = (UINT16)prVdec->u4Height;
            rVDecSeqData.e_frame_rate = prVdec->eFrameRate;
            VDEC_SetMMParam(prVdec->ucEsId, VDEC_MM_SEQ_INFO, (UINT32)&rVDecSeqData, 0, 0);
            _VPUSH_RvOpaqueParse(prVdec, prBytesInfo);
            prVdec->fgNonFirst = TRUE;
            return TRUE;
        }

        if (!prBytesInfo->fgEos)
        {
            if (!_VPUSH_RvPreparse(prBytesInfo))
            {
                return TRUE;
            }
        }
    }
#ifdef MKV_INSERT_MPEG_SEQ_HDR_SW_WORKAROUND
    else if (VDEC_FMT_MPV == prVdec->eFmt)
    {
        if (!prVdec->fgNonFirst)
        {
            prVdec->fgNonFirst = TRUE;
            _u4MpegSeqLen = 0;

            ptr = (UCHAR*)VIRTUAL(prBytesInfo->u4BytesAddr);
            if ((ptr[0] == 0x00) && (ptr[1] == 0x00) && (ptr[2] == 0x01) && (ptr[3] == 0xB3))
            {
                UINT32 i;

                for (i = 4; i < MPEG_SEQ_LEN; i++)
                {
                    if ((ptr[i] == 0x00) && (ptr[i + 1] == 0x00) && (ptr[i + 2] == 0x01))
                    {
                        // find next start code, seq hdr got
                        break;
                    }
                }

                if (i < MPEG_SEQ_LEN)
                {
                    _u4MpegSeqLen = i;
                    x_memcpy(_u1MpegSeq, ptr, i);
                    LOG(0, "1st seq hdr got, len is %d\n", _u4MpegSeqLen);
                }
            }
        }
        else if (_u4MpegSeqLen && (prBytesInfo->u4BytesSize > _u4MpegSeqLen))
        {
            ptr = (UCHAR*)VIRTUAL(prBytesInfo->u4BytesAddr);

            if (0 == x_memcmp(_u1MpegSeq, ptr, _u4MpegSeqLen))
            {
                // seq hdr is duplicate
                prBytesInfo->u4BytesSize -= _u4MpegSeqLen;
                prBytesInfo->u4BytesAddr += _u4MpegSeqLen;
            }
        }
    }
#endif

    x_memset(&rDmxMMData, 0, sizeof(DMX_MM_DATA_T));
    rDmxMMData.u1Idx = prVdec->u1DmxPid;
    rDmxMMData.u4BufStart = prVdec->u4FifoStart;
    rDmxMMData.u4BufEnd = prVdec->u4FifoEnd;
    rDmxMMData.u4StartAddr = prBytesInfo->u4BytesAddr;
    rDmxMMData.u4FrameSize = prBytesInfo->u4BytesSize;
    rDmxMMData.fgEOS = prBytesInfo->fgEos;
    rDmxMMData.u4Pts = prBytesInfo->u4BytesPTS;
    rDmxMMData.u4Dts = prBytesInfo->u4BytesPTS;

    if(prVdec->eFmt == VDEC_FMT_WMV && prVdec->ePushFmt != VDEC_PUSH_FMT_VC1)
    {
        rDmxMMData.fgFrameHead = TRUE;
    }
    
    if (rDmxMMData.fgEOS)
    {
        rDmxMMData.u4BufStart = (UINT32)(&(dummy_data[0]))&0xFFFFFFF0;    
        rDmxMMData.u4BufEnd = ((UINT32)(&(dummy_data[2])) + 0x10+188)&0xFFFFFFF0;    
        rDmxMMData.u4StartAddr = (UINT32)&(dummy_data[1]);    
        rDmxMMData.u4FrameSize = 4;    
        rDmxMMData.fgEOS = TRUE;
    }

    if (0 == rDmxMMData.u4FrameSize)
    {
        fgRet = TRUE;
    }
	else
	{
    	fgRet = _VPUSH_DmxMoveData(prVdec, &rDmxMMData);
	}

    if(!prVdec->fgNonFirst)
    {
        LOG(5, "1st move, Fifo(0x%x-0x%x) Addr(0x%x) Size(0x%x)\n",
            rDmxMMData.u4BufStart,
            rDmxMMData.u4BufEnd,
            rDmxMMData.u4StartAddr,
            rDmxMMData.u4FrameSize);
        prVdec->fgNonFirst = TRUE;
    }

    return fgRet;
}

void _VPUSH_SendCmd(VOID* prdec, UINT32 u4Cmd)
{
    INT32 i4Ret;
    UINT16 u2MsgQIdx;
    SIZE_T zMsgSize;
    VDEC_T *prVdec;
    VDEC_MSG_INTO_T rMsg;
    VDEC_MSG_INTO_T rMsgDump;

    if(!prdec)
    {
        LOG(3, "%s(%d): prdec null\n", __FUNCTION__, __LINE__);
        return;
    }
    prVdec = (VDEC_T*)prdec;

    LOG(3, "Send Cmd(%d) 0x%x\n", (UINT32)prVdec->ucEsId, (UINT32)u4Cmd);

    x_memset(&rMsg, 0, sizeof(VDEC_MSG_INTO_T));
    rMsg.eMsgType = VPUSH_MSG_CMD;
    rMsg.u.eCmd = (VPUSH_CMD_T)u4Cmd;

    i4Ret = x_msg_q_send(prVdec->hMsgQ,(void*)&rMsg, sizeof(VDEC_MSG_INTO_T), 254);
    if (i4Ret == OSR_TOO_MANY)
    {
        LOG(1, "Send Cmd(%d) 0x%x -> drop previous Msg\n", (UINT32)prVdec->ucEsId, (UINT32)u4Cmd);
        zMsgSize = sizeof(VDEC_MSG_INTO_T);
        VERIFY(x_msg_q_receive(&u2MsgQIdx, &rMsgDump, &zMsgSize,
                &(prVdec->hMsgQ), 1, X_MSGQ_OPTION_NOWAIT) == OSR_OK);
        i4Ret = x_msg_q_send(prVdec->hMsgQ, (void *)&rMsg, sizeof(VDEC_MSG_INTO_T), 254);
    }
    else
    {
        VERIFY(i4Ret == OSR_OK);
    }
    UNUSED(_VPUSH_GetMsgCountInQ(prdec));
}

ENUM_VPUSH_MSG_T _VPUSH_ReceiveMsg(VOID* prdec, BOOL bIsBlock)
{
    VDEC_ES_INFO_T *prVdecEsInfo;
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;
    UINT16 u2MsgQIdx;
    SIZE_T zMsgSize;
    INT32 i4Ret;
    VDEC_MSG_INTO_T rMsgTmp;
    ENUM_VPUSH_STATE_T ePreStatus;
    VDEC_T *prVdec;
    UINT32 u4DmxAvailSize;

    if(!prdec)
    {
        LOG(3, "%s(%d): prdec null\n", __FUNCTION__, __LINE__);
        return VPUSH_MSG_NONE;
    }
    prVdec = (VDEC_T*)prdec;

    zMsgSize = sizeof(VDEC_MSG_INTO_T);

    if(bIsBlock)
    {
        i4Ret = x_msg_q_receive(&u2MsgQIdx, &prVdec->rMsg, &zMsgSize,
                &(prVdec->hMsgQ), 1, X_MSGQ_OPTION_WAIT);
        ASSERT(i4Ret == OSR_OK);
    }
    else
    {
        i4Ret = x_msg_q_receive(&u2MsgQIdx, &prVdec->rMsg, &zMsgSize,
                &(prVdec->hMsgQ), 1, X_MSGQ_OPTION_NOWAIT);
    }

    UNUSED(_VPUSH_GetMsgCountInQ(prdec));
    if (i4Ret == OSR_NO_MSG)
    {
        return VPUSH_MSG_NONE;
    }
    else if (i4Ret == OSR_OK)
    {
        ASSERT(zMsgSize == sizeof(VDEC_MSG_INTO_T));
    }

    if(prVdec->rMsg.eMsgType == VPUSH_MSG_CMD)
    {
        LOG(3, "[VPUSH] Rsv Es%d u4Cmd %x\n", prVdec->ucEsId, prVdec->rMsg.u.eCmd);

        ePreStatus = prVdec->eCurState;

        prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(prVdec->ucEsId);
        prVdecEsInfo = _VDEC_GetEsInfo(prVdec->ucEsId);

        //filter illegal command
        switch (prVdec->rMsg.u.eCmd)
        {
            case VPUSH_CMD_STOP:
                prVdecEsInfoKeep->fgVPush = FALSE;
                prVdecEsInfo->fgMMPlayback = FALSE;
                prVdec->ePreviousState = prVdec->eCurState;
                prVdec->eCurState = VPUSH_ST_STOP;
                prVdec->u4PreviousStopId = prVdec->u4CurStopId;
                prVdec->u4CurStopId ++;
                _fgInsertStartcode = FALSE;

                i4Ret = OSR_OK;
                while(i4Ret == OSR_OK)
                {
                    i4Ret = x_msg_q_receive(&u2MsgQIdx, &rMsgTmp, &zMsgSize,
                            &(prVdec->hMsgQ), 1, X_MSGQ_OPTION_NOWAIT);
                }
                UNUSED(_VPUSH_GetMsgCountInQ(prdec));
                LOG(5, "%s(%d): VPUSH_CMD_STOP\n", __FUNCTION__, __LINE__);
                if(prVdec->rInpStrm.fnCb.pfnVdecStopDone)
                {
                    prVdec->rInpStrm.fnCb.pfnVdecStopDone(
                             prVdec->rInpStrm.fnCb.u4StopTag);
                }
                break;
            case VPUSH_CMD_FLUSH:
                i4Ret = OSR_OK;
                while(i4Ret == OSR_OK)
                {
                    i4Ret = x_msg_q_receive(&u2MsgQIdx, &rMsgTmp, &zMsgSize,
                            &(prVdec->hMsgQ), 1, X_MSGQ_OPTION_NOWAIT);
                }
                UNUSED(_VPUSH_GetMsgCountInQ(prdec));
                MPV_FlushEsmQ(prVdec->ucEsId, FALSE);
                VDEC_ReleaseDispQ(prVdec->ucEsId);
                DMX_MM_FlushBuffer(prVdec->u1DmxPid);
                u4DmxAvailSize = DMX_MUL_GetEmptySize(
                    prVdec->u1DmxId, DMX_PID_TYPE_ES_VIDEO, prVdec->u1DmxPid);
                LOG(5, "%s(%d): VPUSH_CMD_FLUSH u4DmxAvailSize(%d)\n",
                    __FUNCTION__, __LINE__, u4DmxAvailSize);
                if(prVdec->rInpStrm.fnCb.pfnVdecFlushDone)
                {
                    prVdec->rInpStrm.fnCb.pfnVdecFlushDone(
                             prVdec->rInpStrm.fnCb.u4FlushTag);
                }
                prVdec->fgNonFirst = FALSE;
                break;
            case VPUSH_CMD_PLAY:
                if((prVdec->eCurState == VPUSH_ST_STOP) ||
                   (prVdec->eCurState == VPUSH_ST_IDLE))
                {
                    prVdec->ePreviousState = prVdec->eCurState;
                    prVdec->eCurState = VPUSH_ST_PLAY;
                }
                break;
            case VPUSH_CMD_PAUSE:
                if (prVdec->eCurState == VPUSH_ST_PLAY)
                {
                    prVdec->ePreviousState = prVdec->eCurState;
                    prVdec->eCurState = VPUSH_ST_IDLE;
                }
                break;
            default:
                break;
        }

        if (prVdec->eCurState != ePreStatus)
        {
            LOG(3, "[VPUSH] Es%d Status Change %x to %x\n",
                prVdec->ucEsId, (UINT32)ePreStatus, (UINT32)prVdec->eCurState);
        }
    }

    return prVdec->rMsg.eMsgType;
}

BOOL _VPUSH_Play(VOID* prdec)
{
    VDEC_ES_INFO_T *prVdecEsInfo;
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;
    VDEC_T *prVdec;
    if(!prdec)
    {
        LOG(1, "%s(%d): prdec null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    prVdec = (VDEC_T*)prdec;
    
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(prVdec->ucEsId);
    prVdecEsInfo = _VDEC_GetEsInfo(prVdec->ucEsId);
    if((!prVdecEsInfoKeep) || (!prVdecEsInfo))
    {
        LOG(1, "%s(%d): instance null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    
    prVdec->eCurStateSync = VPUSH_ST_PLAY;
    prVdecEsInfoKeep->fgVPush = TRUE;
    prVdecEsInfo->fgMMPlayback = TRUE;
    _VPUSH_SendCmd(prdec, (UINT32)VPUSH_CMD_PLAY);
    return VDEC_Play(prVdec->ucEsId, prVdec->eFmt);
}


BOOL _VPUSH_Stop(VOID* prdec)
{
    VDEC_T *prVdec;
    VDEC_DEC_NFY_INFO_T rNfyInfo;
    if(!prdec)
    {
        LOG(3, "%s(%d): prdec null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    LOG(5, "%s(%d)\n", __FUNCTION__, __LINE__);

    prVdec = (VDEC_T*)prdec;

    rNfyInfo.pvTag = (void*)prdec;
    rNfyInfo.pfDecNfy = _VPUSH_VDEC_Nfy;
    VDEC_SetDecNfy(prVdec->ucEsId, &rNfyInfo);

    prVdec->eCurStateSync = VPUSH_ST_STOP;
    VDEC_Stop(prVdec->ucEsId);
    return TRUE;
}


BOOL _VPUSH_Flush(VOID* prdec)
{
    //VDEC_T *prVdec;
    //VDEC_DEC_NFY_INFO_T rNfyInfo;
    if(!prdec)
    {
        LOG(3, "%s(%d): prdec null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    LOG(5, "%s(%d)\n", __FUNCTION__, __LINE__);

    //prVdec = (VDEC_T*)prdec;

    _VPUSH_SendCmd(prdec, (UINT32)VPUSH_CMD_FLUSH);
    return TRUE;
}


BOOL _VPUSH_SetInfo(VOID* prdec, VDEC_SET_INTO_T *prVdecSetInfo)
{
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;
    VDEC_T *prVdec;
    if(!prdec)
    {
        LOG(3, "%s(%d): prdec null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    prVdec = (VDEC_T*)prdec;
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(prVdec->ucEsId);
    if(!prVdecEsInfoKeep)
    {
        LOG(3, "%s(%d): prdec null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    if(!prVdecSetInfo)
    {
        LOG(3, "%s(%d): prVdecSetInfo null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    /*
    if(prVdecSetInfo->fgVPushDecodeOnly)
    {
        _fgInsertStartcode = FALSE;
    }
    else
    {
        _fgInsertStartcode = TRUE;
    }
    */
    _fgInsertStartcode = FALSE;
    prVdecEsInfoKeep->fgVPushDecodeOnly = prVdecSetInfo->fgVPushDecodeOnly;
    return TRUE;
}

BOOL _VPUSH_RegCb(VOID* prdec, VDEC_PUSH_CB_T *prVdecPushCb)
{
    VDEC_T *prVdec;
    if(!prdec)
    {
        LOG(3, "%s(%d): prdec null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    if(!prVdecPushCb)
    {
        LOG(3, "%s(%d): prVdecInputCb null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    prVdec = (VDEC_T*)prdec;
    if(prVdecPushCb->u4Mask & VDEC_PUSH_CB_PUT_DATA_DONE)
    {
        prVdec->rInpStrm.fnCb.u4PutTag = prVdecPushCb->u4PutTag;
        prVdec->rInpStrm.fnCb.pfnVdecPutDataDone = prVdecPushCb->pfnVdecPutDataDone;
    }
    if(prVdecPushCb->u4Mask & VDEC_PUSH_CB_STOP_DONE)
    {
        prVdec->rInpStrm.fnCb.u4StopTag = prVdecPushCb->u4StopTag;
        prVdec->rInpStrm.fnCb.pfnVdecStopDone = prVdecPushCb->pfnVdecStopDone;
    }
    if(prVdecPushCb->u4Mask & VDEC_PUSH_CB_FLUSH_DONE)
    {
        prVdec->rInpStrm.fnCb.u4FlushTag = prVdecPushCb->u4FlushTag;
        prVdec->rInpStrm.fnCb.pfnVdecFlushDone = prVdecPushCb->pfnVdecFlushDone;
    }
    if(prVdecPushCb->u4Mask & VDEC_PUSH_CB_DECODE_DONE)
    {
        prVdec->rInpStrm.fnCb.u4DecodeTag = prVdecPushCb->u4DecodeTag;
        prVdec->rInpStrm.fnCb.pfnVdecDecodeDone = prVdecPushCb->pfnVdecDecodeDone;
    }
    if(prVdecPushCb->u4Mask & VDEC_PUSH_CB_IMG_FRAME_DONE)
    {
        prVdec->rInpStrm.fnCb.u4ImageTag = prVdecPushCb->u4ImageTag;
        prVdec->rInpStrm.fnCb.pfnVdecImgFrmDone= prVdecPushCb->pfnVdecImgFrmDone;
    }

    return TRUE;
}

BOOL _VPUSH_PutData(VOID* prdec, VDEC_BYTES_INTO_T *prBytesInfo)
{
    INT32 i4Ret;
    VDEC_MSG_INTO_T rMsg;

    VDEC_T *prVdec;
    if((!prdec) || (!prBytesInfo))
    {
        LOG(3, "%s(%d): prdec null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    prVdec = (VDEC_T*)prdec;
    if(prVdec->eCurStateSync == VPUSH_ST_STOP)
    {
        LOG(3, "%s(%d): under stop\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    x_memset(&rMsg, 0, sizeof(VDEC_MSG_INTO_T));
    rMsg.eMsgType = VPUSH_MSG_DATA;
    x_memcpy(&rMsg.u.rBytesInfo, prBytesInfo, sizeof(VDEC_BYTES_INTO_T));
    i4Ret = x_msg_q_send(prVdec->hMsgQ, (void *)&rMsg,
        sizeof(VDEC_MSG_INTO_T), 255);
    UNUSED(_VPUSH_GetMsgCountInQ(prdec));
    if(i4Ret != OSR_OK)
    {
        ASSERT(0);
        LOG(3, "%s(%d): i4Ret(%d))\n", __FUNCTION__, __LINE__, i4Ret);
        return FALSE;
    }

    return TRUE;
}

BOOL _VPUSH_PutDataDone(VOID* prdec, UINT32 u4Tag)
{
    VDEC_T *prVdec;
    if(!prdec)
    {
        LOG(3, "%s(%d): prdec null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    UNUSED(u4Tag);//not used currently
    prVdec = (VDEC_T*)prdec;
    if(prVdec->rInpStrm.fnCb.pfnVdecPutDataDone)
    {
        return prVdec->rInpStrm.fnCb.pfnVdecPutDataDone(
            prVdec->rInpStrm.fnCb.u4PutTag);
    }
    return TRUE;
}

VOID _VPUSH_DecodeInit(VOID)
{
    CHAR szBuf[16];
    UNUSED(_fgPushInit);
    if(!_rVdecPush.fgInited)
    {
        UINT32 i, j;
        UINT32 u4Addr, u4Size;
        FBM_POOL_T* prFbmPool;
        x_memset(&_rVdecPush, 0, sizeof(VDEC_DECODER_T));
        //DMX_Init();
        //DMX_MM_Init();
        //VDP_Init();
        //VDEC_Init();
        _rVdecPush.fgInited = TRUE;

        // FBM
        prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_FEEDER);
        if(prFbmPool == NULL)
        {
            LOG(1, "_SWDMX_GetVideoFifo FBM_GetPoolInfo Fail\n");
            return;
        }
        if((prFbmPool->u4Addr == 0) || (prFbmPool->u4Size == 0))
        {
            LOG(1, "_SWDMX_GetVideoFifo prFbmPool->u4Addr/u4Size 0\n");
            return;
        }

        // DDI is 4 bytes start address alignment
        // DDI is 16 bytes buffer size alignment. (188 is better)
        u4Addr = VIRTUAL(prFbmPool->u4Addr);
        u4Size = (prFbmPool->u4Size / (VDEC_MAX_DECODER * VDEC_BUF_COUNT));
        u4Size -= (u4Size % 752);
        for(i=0;i<VDEC_MAX_DECODER;i++)
        {
            _rVdecPush.arDec[i].u4FifoStart =
                u4Addr + ((i) * (u4Size * VDEC_BUF_COUNT));
            _rVdecPush.arDec[i].u4FifoEnd =
                _rVdecPush.arDec[i].u4FifoStart +
                (u4Size * VDEC_BUF_COUNT);

            _rVdecPush.arDec[i].ucEsId = 0xFF;
            for(j=0;j<VDEC_BUF_COUNT;j++)
            {
                //_rVdecPush.arDec[i].arBufInfo.fgUsed = FALSE;
                _rVdecPush.arDec[i].arBufInfo[j].u4BufAddr =
                    _rVdecPush.arDec[i].u4FifoStart + (j * u4Size);
                _rVdecPush.arDec[i].arBufInfo[j].u4BufSize = u4Size;
            }

            x_snprintf(szBuf, sizeof(szBuf), "PVBYTQ%d", i);
            VERIFY(x_msg_q_create(&_rVdecPush.arDec[i].hMsgQ, szBuf,
                sizeof(VDEC_MSG_INTO_T), VDEC_PUSH_MSG_Q_SIZE) == OSR_OK);

            x_snprintf(szBuf, sizeof(szBuf), "PVDEC%d", i);
            VERIFY(x_thread_create(&_rVdecPush.arDec[i].hThread, szBuf,
                VDEC_PUSH_STACK_SIZE, VDEC_PUSH_THREAD_PRIORITY,
                _VPUSH_PushLoop, 4, (VOID*)&i) == OSR_OK);
        }
    }
}

BOOL _VPUSH_DecodeReset(VOID* prdec)
{
    VDEC_T *prVdec;
    if(!prdec)
    {
        LOG(3, "%s(%d): prdec is null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    prVdec = (VDEC_T*)prdec;
    UNUSED(prVdec);
    ASSERT(prVdec->ucEsId < VDEC_MAX_ES);
    CHECK_FALSE_RET(_VPUSH_SetupFifoBuffer(prdec));
    return TRUE;
}

BOOL _VPUSH_GetVFifoInfo(VDEC_VFIFO_INTO_T *prVfifoInfo)
{
    if(!prVfifoInfo)
    {
        LOG(3, "%s(%d): prVfifoInfo is null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    prVfifoInfo->u4BufferCount = VDEC_BUF_COUNT;
    prVfifoInfo->u4BufferSize = _rVdecPush.arDec[0].arBufInfo[0].u4BufSize;
    return TRUE;
}

VOID VOID_VPUSH_SetSeamlessInfo(VOID* prdec,BOOL fgSeamlessPlay, UINT32 u4SeamlessWidth, UINT32 u4SeamlessHeight)
{
    VDEC_T *prVdec;
    VDEC_ES_INFO_T *prVdecEsInfo = NULL;
    
    if(!prdec)
    {
        LOG(3, "%s(%d): prdec is null\n", __FUNCTION__, __LINE__);
        return;
    }
    
    prVdec = (VDEC_T*)prdec;
    prVdecEsInfo = _VDEC_GetEsInfo(prVdec->ucEsId);

    if(prVdecEsInfo != NULL)
    {
        LOG(3,"VOID_VPUSH_SetSeamlessInfo , fgSeamlessPlay = %d, u4SeamlessWidth = %d, u4SeamlessHeight = %d\n",fgSeamlessPlay,u4SeamlessWidth,u4SeamlessHeight);
        
        prVdecEsInfo->fgSeamlessPlay= fgSeamlessPlay;
        prVdecEsInfo->u4SeamlessWidth = u4SeamlessWidth;
        prVdecEsInfo->u4SeamlessHeight = u4SeamlessHeight;
    }
}

VOID _VPUSH_SetPicSz(VOID* prdec, UINT32 u4Width, UINT32 u4Height,UINT32 u4FrameRate)
{
    VDEC_T *prVdec;
    VDEC_SEQUENCE_DATA_T rVDecSeqData;
    if(!prdec)
    {
        LOG(3, "%s(%d): prdec is null\n", __FUNCTION__, __LINE__);
        return;
    }
    prVdec = (VDEC_T*)prdec;

    x_memset(&rVDecSeqData, 0, sizeof(rVDecSeqData));
                
    prVdec->u4Width = u4Width;
    prVdec->u4Height = u4Height;

    LOG(3,"video codec frame rate = %d\n",u4FrameRate);
    
    switch(u4FrameRate)
    {
        case 23 :
            prVdec->eFrameRate = VDEC_SRC_FRAME_RATE_23_976;
            break;
        case 24 :
            prVdec->eFrameRate = VDEC_SRC_FRAME_RATE_24;
            break;
        case 25 :
            prVdec->eFrameRate = VDEC_SRC_FRAME_RATE_25;
            break;
        case 29 :
            prVdec->eFrameRate = VDEC_SRC_FRAME_RATE_29_97;
            break;        
        case 30 :
            prVdec->eFrameRate = VDEC_SRC_FRAME_RATE_30;
            break;
        case 50 :
            prVdec->eFrameRate = VDEC_SRC_FRAME_RATE_50;
            break;
        case 59 :
            prVdec->eFrameRate = VDEC_SRC_FRAME_RATE_59_94;
            break;            
        case 60 :
            prVdec->eFrameRate = VDEC_SRC_FRAME_RATE_60;
            break;
        default :
            prVdec->eFrameRate = VDEC_SRC_FRAME_RATE_VARIABLE;
    }

    rVDecSeqData.e_frame_rate = prVdec->eFrameRate;
    
    VDEC_SetMMParam(prVdec->ucEsId, VDEC_MM_FRAME_RATE, (UINT32)&rVDecSeqData, 0, 0);
}


BOOL _VPUSH_SetPushType(VOID* prdec, ENUM_VDEC_PUSH_FMT_T ePushFmt)
{
    VDEC_T *prVdec;

    if(!prdec)
    {
        LOG(3, "%s(%d): prdec is null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    prVdec = (VDEC_T*)prdec;
    prVdec->ePushFmt = ePushFmt;

    return TRUE;
}


BOOL _VPUSH_AllocVFifo(VOID* prdec,
    UINT32 *pu4BufSA, UINT32 u4Size, ENUM_VDEC_FMT_T eFmt)
{
    UINT32 i;
    VDEC_T *prVdec;

    if(!prdec)
    {
        LOG(3, "%s(%d): prdec null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    prVdec = (VDEC_T*)prdec;
    prVdec->eFmt = eFmt;

    if(prVdec->u4AllocBufCnt == 0)
    {
        if(!_VPUSH_AllocDmx(prVdec))
        {
            LOG(3, "%s(%d): _VPUSH_AllocDmx fail\n", __FUNCTION__, __LINE__);
            ASSERT(0);
        }
    }

    for(i=0;i<VDEC_BUF_COUNT;i++)
    {
        if(!prVdec->arBufInfo[i].fgUsed)
        {
            prVdec->arBufInfo[i].fgUsed = TRUE;
            prVdec->u4AllocBufCnt++;
            break;
        }
    }
    if(i >= VDEC_BUF_COUNT)
    {
        LOG(3, "%s(%d): (i >= VDEC_BUF_COUNT)\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    *pu4BufSA = prVdec->arBufInfo[i].u4BufAddr;
    ASSERT(u4Size <= prVdec->arBufInfo[i].u4BufSize);
    //prVdec->eFmt = eFmt;
    return TRUE;
}

BOOL _VPUSH_FreeVFifo(VOID* prdec, UINT32 u4BufSA)
{
    BOOL fgFound = FALSE;
    UINT32 i;
    VDEC_T *prVdec;

    if(!prdec)
    {
        LOG(3, "%s(%d): prdec null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    prVdec = (VDEC_T*)prdec;

    for(i=0;i<VDEC_BUF_COUNT;i++)
    {
        if((prVdec->arBufInfo[i].fgUsed) &&
           (prVdec->arBufInfo[i].u4BufAddr == u4BufSA))
        {
            prVdec->arBufInfo[i].fgUsed = FALSE;
            prVdec->u4AllocBufCnt --;
            fgFound = TRUE;
            break;
        }
    }
    if(fgFound)
    {
        if(prVdec->u4AllocBufCnt == 0)
        {
            // release dmx
            if(!_VPUSH_ReleaseDmx(prdec))
            {
                LOG(3, "%s(%d): _VPUSH_ReleaseDmx fail\n", __FUNCTION__, __LINE__);
                ASSERT(0);
            }
        }
        return TRUE;
    }
    LOG(3, "%s(%d): 0x%x not found\n", __FUNCTION__, __LINE__, u4BufSA);
    return fgFound;
}

BOOL _VPUSH_SetupFifoBuffer(VOID* prdec)
{
    VDEC_T *prVdec;
    VDEC_INPUT_STREAM_T *prInpStrm;
    FBM_POOL_T* prFbmPool;

    if(!prdec)
    {
        LOG(3, "%s(%d): prdec null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    prVdec = (VDEC_T*)prdec;
    prInpStrm = &prVdec->rInpStrm;

    ASSERT(prVdec->ucEsId < VDEC_MAX_ES);

	#if defined(CC_VOMX_TV_COEXIST)
    prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_DMX2);
	#else
    prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_DMX);
	#endif

	if(prFbmPool == NULL)
    {
        LOG(1, "_SWDMX_GetVideoFifo FBM_GetPoolInfo Fail\n");
        return FALSE;
    }
    if((prFbmPool->u4Addr == 0) || (prFbmPool->u4Size == 0))
    {
        LOG(1, "_SWDMX_GetVideoFifo prFbmPool->u4Addr/u4Size 0\n");
        return FALSE;
    }

    x_memset((VOID*)prInpStrm, 0, sizeof(VDEC_INPUT_STREAM_T));

    prInpStrm->u4FifoStart = prFbmPool->u4Addr;
    prInpStrm->u4FifoEnd = prFbmPool->u4Addr + prFbmPool->u4Size;
    prInpStrm->u4FifoRpt = prFbmPool->u4Addr;
    prInpStrm->u4FifoWpt = prFbmPool->u4Addr;
    prInpStrm->u4FifoRptParse = prFbmPool->u4Addr;
    prInpStrm->u4FifoRptParseStart = prFbmPool->u4Addr;
    prInpStrm->u4FifoRptValue = 0xFFFFFFFF;
    prInpStrm->u4FifoRptValueLast = 0xFFFFFFFF;
    return TRUE;
}


VOID _VPUSH_PushLoop(VOID* pvArg)
{
    ENUM_VPUSH_MSG_T eMsg;
    //VDEC_MSG_INTO_T rBytesInfo;
    //UINT16 u2MsgQIdx;
    UCHAR ucEsId;
    VDEC_T *prVdec;
    UINT32 u4DmxAvailSize;
    UINT32 u4CurRptr, u4VFifoStart = 0, u4VFifoEnd = 0;
    IMG_LIB_FILL_RING_BUF_T rStreamBufInfo;
    FBM_POOL_T* prFbmPool;
    INT32 i4Ret;
    BOOL fgEsmQFull;

    if(pvArg == NULL)
    {
        ASSERT(!pvArg);
        ucEsId = ES0;
        LOG(1, "_VPUSH_PushLoop(): (pvArg == NULL)\n");
        return;
    }
    else
    {
        ucEsId = *(UCHAR*)pvArg;
        if(ucEsId >= VDEC_MAX_DECODER)
        {
            LOG(1, "_VPUSH_PushLoop(): (ucEsId >= VDEC_MAX_DECODER)\n");
            return;
        }
    }

    //for JPEG
	#if defined(CC_VOMX_TV_COEXIST)
    prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_DMX2);
	#else
    prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_DMX);
	#endif

	if(prFbmPool)
    {
        u4VFifoStart = VIRTUAL(prFbmPool->u4Addr);
        u4VFifoEnd = u4VFifoStart + prFbmPool->u4Size;
    }
    //---for JPEG
    
    prVdec = (VDEC_T*)&_rVdecPush.arDec[ucEsId];
    while (1)//!prVdec->fgGoDead)
    {
        /*
        if(prVdec->eCurState == VPUSH_ST_STOP)
        {
            eMsg = _VPUSH_ReceiveMsg(prVdec, TRUE);
        }
        else if(prVdec->eCurState == VPUSH_ST_IDLE)
        {
            eMsg = _VPUSH_ReceiveMsg(prVdec, TRUE);
        }
        else
        {
            eMsg = _VPUSH_ReceiveMsg(prVdec, FALSE);
        }
        */

        eMsg = _VPUSH_ReceiveMsg(prVdec, FALSE);
        if(eMsg == VPUSH_MSG_NONE)
        {
            eMsg = _VPUSH_ReceiveMsg(prVdec, TRUE);
        }

        if((eMsg == VPUSH_MSG_DATA) && (prVdec->eCurState != VPUSH_ST_STOP))
        {
            fgEsmQFull = FALSE;
            if ((prVdec->eFmt == VDEC_FMT_JPG)||(prVdec->eFmt == VDEC_FMT_PNG))
            {
                if(prVdec->rMsg.u.rBytesInfo.fgEos)
                {
                    LOG(5, "--- Rcv EOS\n");
                }
                x_memset((void*)&rStreamBufInfo, 0, sizeof(rStreamBufInfo));

                if(!prVdec->fgNonFirst || !prVdec->fgInputBufReady)
                {
                    prVdec->fgNonFirst = TRUE;
                    u4DmxAvailSize = prVdec->u4FifoEnd - prVdec->u4FifoStart;
                }
                else if((IMG_GetDecState() == IMG_RELEASE) && 
                    (!prVdec->rMsg.u.rBytesInfo.fgEos))   //img driver not ready
                {
                    i4Ret = x_msg_q_send(prVdec->hMsgQ, (void *)&prVdec->rMsg,
                        sizeof(VDEC_MSG_INTO_T), 254);
                    UNUSED(_VPUSH_GetMsgCountInQ((VOID*)prVdec));
                    if(i4Ret != OSR_OK)
                    {
                        ASSERT(0);
                        LOG(3, "%s(%d): i4Ret(%d))\n", __FUNCTION__, __LINE__, i4Ret);
                        continue;
                    }
                    x_thread_delay(10);
                    continue;
                }
                else
                {
                    u4DmxAvailSize = IMG_GetBSBufInfo(&rStreamBufInfo);
                }
                if(rStreamBufInfo.rEntry2.bValid)
                {
                    u4CurRptr = (UINT32)rStreamBufInfo.rEntry2.pvStartAddr + (UINT32)rStreamBufInfo.rEntry2.u4RequiredLen + 64;
                }
                else if(rStreamBufInfo.rEntry1.bValid)
                {
                    u4CurRptr = (UINT32)rStreamBufInfo.rEntry1.pvStartAddr + (UINT32)rStreamBufInfo.rEntry1.u4RequiredLen + 64;
                }
                else
                {
                    u4CurRptr = 0;
                }
                if(u4CurRptr > 0)
                {
                    u4CurRptr = (u4CurRptr >= u4VFifoEnd)?(u4CurRptr - (u4VFifoEnd - u4VFifoStart)):u4CurRptr;
                    if (!DMX_UpdateVideoReadPointer(prVdec->ucEsId, u4CurRptr, u4CurRptr))                
                    {                    
                        //LOG(0, "-");                
                    }
                }
            }
            else
            {
                UINT16 u2QueueSize, u2MaxQueueSize;
                u4DmxAvailSize = DMX_MUL_GetEmptySize(
                    prVdec->u1DmxId, DMX_PID_TYPE_ES_VIDEO, prVdec->u1DmxPid);
                VDEC_GetQueueInfo(ucEsId, &u2QueueSize, &u2MaxQueueSize);
                if(u2QueueSize > (u2MaxQueueSize - 100))
                {
                    fgEsmQFull = TRUE;
                }
            }
            if((u4DmxAvailSize < prVdec->rMsg.u.rBytesInfo.u4BytesSize) ||
               (fgEsmQFull))
            {
                i4Ret = x_msg_q_send(prVdec->hMsgQ, (void *)&prVdec->rMsg,
                    sizeof(VDEC_MSG_INTO_T), 254);
                UNUSED(_VPUSH_GetMsgCountInQ((VOID*)prVdec));
                if(i4Ret != OSR_OK)
                {
                    ASSERT(0);
                    LOG(3, "%s(%d): i4Ret(%d))\n", __FUNCTION__, __LINE__, i4Ret);
                    continue;
                }

                x_thread_delay(10);
                continue;
            }
            if(!_VPUSH_MoveData(prVdec, &prVdec->rMsg.u.rBytesInfo))
            {
                LOG(3, "%s(%d): _VPUSH_MoveData fail\n", __FUNCTION__, __LINE__);
            }
            if(!_VPUSH_PutDataDone(prVdec, prVdec->rMsg.u.rBytesInfo.u4BytesTag))
            {
                LOG(3, "%s(%d): _VPUSH_PutDataDone fail\n", __FUNCTION__, __LINE__);
            }
        }

    }
}

VOID* _VPUSH_AllocVideoDecoder(ENUM_VDEC_FMT_T eFmt)
{
    BOOL fgFound = FALSE;
    UINT32 i;
    //VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;
    for(i=0;i<VDEC_MAX_DECODER;i++)
    {
        if(_rVdecPush.fgOccupied[i] == FALSE)
        {
            _rVdecPush.fgOccupied[i] = TRUE;
            _rVdecPush.arDec[i].ucEsId = i;
            _rVdecPush.arDec[i].fgNonFirst = FALSE;
            _rVdecPush.arDec[i].fgInputBufReady = FALSE;
            _rVdecPush.arDec[i].eFmt = eFmt;

            fgFound = TRUE;
            break;
        }
    }
    if(fgFound)
    {
        //UNUSED(_VPUSH_DecodeReset(&_rVdecPush.arDec[i]));

        ASSERT(i < VDEC_MAX_ES);

        FBM_SetFrameBufferGlobalFlag(0xFF, FBM_FLAG_FB_DECODE_ONLY);
        FBM_SetFrameBufferGlobalFlag(0xFF, FBM_FLAG_FB_NO_TIMEOUT);

        _rVdecPush.arDec[i].eFmt = eFmt;

        return &_rVdecPush.arDec[i];
    }
    return NULL;
}

VOID _VPUSH_ReleaseVideoDecoder(VOID* prdec)
{
    VDEC_T *prVdec;
    //VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;

    if(!prdec)
    {
        LOG(3, "%s(%d): prdec null\n", __FUNCTION__, __LINE__);
        return;
    }

    prVdec = (VDEC_T*)prdec;
    if(prVdec->ucEsId >= VDEC_MAX_ES)
    {
        // already released at stop
        return;
    }

    // TODO
    #if 0
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(prVdec->ucEsId);
    VDEC_Stop(prVdec->ucEsId);
    while(prVdecEsInfoKeep->eCurState != VDEC_ST_STOP)
    {
        x_thread_delay(1);
    }
    #endif
    FBM_ClrFrameBufferGlobalFlag(0xFF, FBM_FLAG_FB_DECODE_ONLY);

    _rVdecPush.fgOccupied[prVdec->ucEsId] = FALSE;
    _fgFirstVideoChunk = TRUE;
    prVdec->ucEsId = 0xFF;
    return;
}

VOID _VPUSH_DecodeDone(UCHAR ucEsId, VOID *pPicNfyInfo)
{
    UINT32 i;
    FBM_PIC_NTF_INFO_T *prPicNfyInfo;
    VDEC_T *prVdec = NULL;

    if(!pPicNfyInfo)
    {
        LOG(1, "%s(%d): fail\n", __FUNCTION__, __LINE__);
        ASSERT(0);
        return;
    }

    prPicNfyInfo = (FBM_PIC_NTF_INFO_T*)pPicNfyInfo;

    for(i=0;i<VDEC_MAX_DECODER;i++)
    {
        if(_rVdecPush.arDec[i].ucEsId == ucEsId)
        {
            prVdec = &_rVdecPush.arDec[i];
        }
        ASSERT(i == ucEsId);
    }
    if(!prVdec)
    {
        LOG(1, "%s(%d): fail\n", __FUNCTION__, __LINE__);
        ASSERT(0);
        return;
    }
    if(prVdec->eCurStateSync == VPUSH_ST_STOP)
    {
        LOG(3, "%s(%d): under stop\n", __FUNCTION__, __LINE__);
        return;
    }

    if(prVdec->rInpStrm.fnCb.pfnVdecDecodeDone)
    {
        prVdec->rInpStrm.fnCb.pfnVdecDecodeDone(
            prVdec->rInpStrm.fnCb.u4DecodeTag,
            prPicNfyInfo->ucFbgId,
            prPicNfyInfo->ucFbId,
            prPicNfyInfo->fgEos);
    }
    return;
}

VOID _VPUSH_ImgFrmDone(UCHAR ucEsId, VOID *pNfyInfo)
{
    UINT32 i;
    VDEC_T *prVdec = NULL;

#if 0
    if(!pNfyInfo)
    {
        LOG(1, "%s(%d): fail\n", __FUNCTION__, __LINE__);
        ASSERT(0);
        return;
    }
#endif

    for(i=0;i<VDEC_MAX_DECODER;i++)
    {
        if(_rVdecPush.arDec[i].ucEsId == ucEsId)
        {
            prVdec = &_rVdecPush.arDec[i];
        }
        ASSERT(i == ucEsId);
    }
    if(!prVdec)
    {
        LOG(1, "%s(%d): fail\n", __FUNCTION__, __LINE__);
        ASSERT(0);
        return;
    }

    if(prVdec->rInpStrm.fnCb.pfnVdecImgFrmDone)
    {
        prVdec->rInpStrm.fnCb.pfnVdecImgFrmDone(
            prVdec->rInpStrm.fnCb.u4ImageTag,
            pNfyInfo);
    }
    return;
}

UCHAR _VPUSH_FbgId2EsId(UCHAR ucFbgId)
{
    UCHAR ucTmp;
    VDEC_ES_INFO_T *prVdecEsInfo;
    for(ucTmp = 0;ucTmp < VDEC_MAX_DECODER;ucTmp++)
    {
        prVdecEsInfo = _VDEC_GetEsInfo(ucTmp);
        if(prVdecEsInfo->ucFbgId == ucFbgId)
        {
            return ucTmp;
        }
    }
    return 0xFF;
}

UINT32 u4YResizeAddr=0;
UINT32 u4CResizeAddr=0;
BOOL _VPUSH_B2R(UCHAR ucFbgId, UCHAR ucFbId, BOOL fgIsRsIn, BOOL fgIsRsOut)
{
    UCHAR ucEsId;
    UINT32 u4AddrY;
    UINT32 u4AddrC;
    UINT32 u4Xaddr=0;
    UINT32 u4Yaddr=0;
    RZ_VDO_SCL_PARAM_SET_T rScaleParamV;
    FBM_SEQ_HDR_T* prFbmSeqHdr = NULL;

    ucEsId = _VPUSH_FbgId2EsId(ucFbgId);
    if(ucEsId >= VDEC_MAX_DECODER)
    {
        return FALSE;
    }

    prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(ucFbgId);
    if(!prFbmSeqHdr)
    {
        return FALSE;
    }

    FBM_GetFrameBufferAddr(ucFbgId, ucFbId, &u4AddrY, &u4AddrC);

    if(u4YResizeAddr == 0)
    {
        u4YResizeAddr = ((UINT32)x_mem_alloc(1920*1088 + 0x3F)) & (~0x3F);
    }
    if(u4CResizeAddr == 0)
    {
        u4CResizeAddr = ((UINT32)x_mem_alloc(1920*1088/2 + 0x3F)) & (~0x3F);
    }

    rScaleParamV.u4IsRsIn = fgIsRsIn; // 0 block output, 1 raster output
    rScaleParamV.u4InMode = E_RZ_VDO_OUTMD_420;
    rScaleParamV.u4YSrcBase = u4AddrY;
    rScaleParamV.u4YSrcBufLen = prFbmSeqHdr->u2LineSize;
    rScaleParamV.u4YSrcHOffset = 0;
    rScaleParamV.u4YSrcVOffset = 0;
    rScaleParamV.u4YSrcW = prFbmSeqHdr->u2HSize;
    rScaleParamV.u4YSrcH = prFbmSeqHdr->u2VSize;
    rScaleParamV.u4CSrcBase = u4AddrC;
    rScaleParamV.u4CSrcHOffset = 0;
    rScaleParamV.u4CSrcVOffset = 0;
    rScaleParamV.u4CSrcW = prFbmSeqHdr->u2HSize >> 1;
    rScaleParamV.u4CSrcH = prFbmSeqHdr->u2VSize >> 1;
    rScaleParamV.u4IsRsOut = fgIsRsOut; // 0 block output, 1 raster output
    rScaleParamV.u4OutMode = E_RZ_VDO_OUTMD_420;
    rScaleParamV.u4IsVdo2Osd = 0;
    rScaleParamV.u4YTgBase = u4YResizeAddr;
    rScaleParamV.u4YTgCM = E_RZ_VDO_OUTMD_420;
    rScaleParamV.u4YTgHOffset = u4Xaddr;
    rScaleParamV.u4YTgVOffset = u4Yaddr;

    rScaleParamV.u4YTgW = prFbmSeqHdr->u2HSize;
    rScaleParamV.u4YTgH = prFbmSeqHdr->u2VSize;
    rScaleParamV.u4YTgBufLen= prFbmSeqHdr->u2LineSize;

    rScaleParamV.u4CTgBase = u4CResizeAddr;

    rScaleParamV.u4CTgW=rScaleParamV.u4YTgW>>1;
    rScaleParamV.u4CTgH=rScaleParamV.u4YTgH>>1;
    rScaleParamV.u4OutSwap = 6;
    rScaleParamV.u4SrcSwap = 6;

    IMGRZ_Lock();
    IMGRZ_ReInit();
    IMGRZ_SetScaleOpt(E_RZ_INOUT_TYPE_VDOMD);
    IMGRZ_Scale((void *)(&rScaleParamV));
    IMGRZ_Flush();
    IMGRZ_Wait();
    IMGRZ_Unlock();
    return TRUE;
}

BOOL _VPUSH_R2B(UCHAR ucFbgId, UCHAR ucFbId, BOOL fgIsRsIn, BOOL fgIsRsOut)
{
    UCHAR ucEsId;
    UINT32 u4AddrY;
    UINT32 u4AddrC;
    UINT32 u4Xaddr=0;
    UINT32 u4Yaddr=0;
    RZ_VDO_SCL_PARAM_SET_T rScaleParamV;
    FBM_SEQ_HDR_T* prFbmSeqHdr = NULL;

    ucEsId = _VPUSH_FbgId2EsId(ucFbgId);
    if(ucEsId >= VDEC_MAX_DECODER)
    {
        return FALSE;
    }

    prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(ucFbgId);
    if(!prFbmSeqHdr)
    {
        return FALSE;
    }

    FBM_GetFrameBufferAddr(ucFbgId, ucFbId, &u4AddrY, &u4AddrC);

    if(u4YResizeAddr == 0)
    {
        u4YResizeAddr = (UINT32)x_mem_alloc(1920*1088);
    }
    if(u4CResizeAddr == 0)
    {
        u4CResizeAddr = (UINT32)x_mem_alloc(1920*1088/2);
    }

    rScaleParamV.u4IsRsIn = fgIsRsIn; // 0 block output, 1 raster output
    rScaleParamV.u4InMode = E_RZ_VDO_OUTMD_420;
    rScaleParamV.u4YSrcBase = u4YResizeAddr;
    rScaleParamV.u4YSrcBufLen = prFbmSeqHdr->u2LineSize;
    rScaleParamV.u4YSrcHOffset = 0;
    rScaleParamV.u4YSrcVOffset = 0;
    rScaleParamV.u4YSrcW = prFbmSeqHdr->u2HSize;
    rScaleParamV.u4YSrcH = prFbmSeqHdr->u2VSize;
    rScaleParamV.u4CSrcBase = u4CResizeAddr;
    rScaleParamV.u4CSrcHOffset = 0;
    rScaleParamV.u4CSrcVOffset = 0;
    rScaleParamV.u4CSrcW = prFbmSeqHdr->u2HSize >> 1;
    rScaleParamV.u4CSrcH = prFbmSeqHdr->u2VSize >> 1;
    rScaleParamV.u4IsRsOut = fgIsRsOut; // 0 block output, 1 raster output
    rScaleParamV.u4OutMode = E_RZ_VDO_OUTMD_420;
    rScaleParamV.u4IsVdo2Osd = 0;
    rScaleParamV.u4YTgBase = u4AddrY;
    rScaleParamV.u4YTgCM = E_RZ_VDO_OUTMD_420;
    rScaleParamV.u4YTgHOffset = u4Xaddr;
    rScaleParamV.u4YTgVOffset = u4Yaddr;

    rScaleParamV.u4YTgW = prFbmSeqHdr->u2HSize;
    rScaleParamV.u4YTgH = prFbmSeqHdr->u2VSize;
    rScaleParamV.u4YTgBufLen= prFbmSeqHdr->u2LineSize;

    rScaleParamV.u4CTgBase = u4AddrC;

    rScaleParamV.u4CTgW=rScaleParamV.u4YTgW>>1;
    rScaleParamV.u4CTgH=rScaleParamV.u4YTgH>>1;
    rScaleParamV.u4OutSwap = 6;
    rScaleParamV.u4SrcSwap = 6;

    IMGRZ_Lock();
    IMGRZ_ReInit();
    IMGRZ_SetScaleOpt(E_RZ_INOUT_TYPE_VDOMD);
    IMGRZ_Scale((void *)(&rScaleParamV));
    IMGRZ_Flush();
    IMGRZ_Wait();
    IMGRZ_Unlock();
    return TRUE;
}


